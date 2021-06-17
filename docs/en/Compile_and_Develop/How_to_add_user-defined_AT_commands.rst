How to add user-defined AT commands
===================================

This document details how to add a user-defined AT command based on the `esp-at <https://github.com/espressif/esp-at>`_ project. It uses the ``AT+TEST`` command as an example to show the sample code for each step.

Customizing a basic and well-functioned command requires at least the two steps below:

- :ref:`define-at-commands`
- :ref:`register-at-commands`

This step checks how the newly defined command works out.

- :ref:`user-define-at-commands-give-it-a-try`

The remaining steps are for relatively complex AT commands and are optional depending on your needs.

- :ref:`define-return-values`
- :ref:`access-command-parameters`
- :ref:`omit-command-parameters`
- :ref:`block-command-execution`
- :ref:`access-input-data-from-at-command-port`

The source code of AT command set is not open-source, and is provided in the form of :component:`library file <at/lib>`. It is also the basis to parse user-defined AT commands.

.. _define-at-commands:

Define AT Commands
------------------

Before defining any AT command, you should first decide on the name and type of the AT command you want to define.

**Naming rules:**

- AT command should start with the ``+`` character.
- Alphabetic characters (``A~Z, a~z``), numeric characters (``0~9``), and some other characters (``!``, ``%``, ``-``, ``.``, ``/``, ``:``, ``_``) are supported. See :ref:`at-command-types` for more information.

**Command types:**

Each AT command can have up to four types: Test Command, Query Command, Set Command, and Execute Command. See :ref:`at-command-types` for more information.

Then, define each type of command. Assuming that ``AT+TEST`` supports all the four types. Below is the sample code to define each type.

Test Command:

.. code-block:: c

    uint8_t at_test_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};

        snprintf((char *)buffer, 64, "this cmd is test cmd: %s\r\n", cmd_name);

        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

Query Command:

.. code-block:: c

    uint8_t at_query_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};

        snprintf((char *)buffer, 64, "this cmd is query cmd: %s\r\n", cmd_name);

        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

.. _user-defined-set-command:

Set Command:

.. code-block:: c

    uint8_t at_setup_cmd_test(uint8_t para_num)
    {
        int32_t para_int_1 = 0;
        uint8_t *para_str_2 = NULL;
        uint8_t num_index = 0;
        uint8_t buffer[64] = {0};

        if (esp_at_get_para_as_digit(num_index++, &para_int_1) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        if (esp_at_get_para_as_str(num_index++, &para_str_2) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        snprintf((char *)buffer, 64, "this cmd is setup cmd and cmd num is: %u\r\n", para_num);
        esp_at_port_write_data(buffer, strlen((char *)buffer));

        memset(buffer, 0, 64);
        snprintf((char *)buffer, 64, "first parameter is: %d\r\n", para_int_1);
        esp_at_port_write_data(buffer, strlen((char *)buffer));

        memset(buffer, 0, 64);
        snprintf((char *)buffer, 64, "second parameter is: %s\r\n", para_str_2);
        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

Execute Command:

.. code-block:: c

    uint8_t at_exe_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};

        snprintf((char *)buffer, 64, "this cmd is execute cmd: %s\r\n", cmd_name);

        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

Finally, call :cpp:type:`esp_at_cmd_struct` to define the name and type(s) that your AT command supports. The sample code below defined the name ``+TEST`` (omitting ``AT``) and all the four types.

.. code-block:: c

    static esp_at_cmd_struct at_custom_cmd[] = {
        {"+TEST", at_test_cmd_test, at_query_cmd_test, at_setup_cmd_test, at_exe_cmd_test},
    };

If you do not want to define a particular type, set it to ``NULL``.

.. _register-at-commands:

Register AT Commands
--------------------

Call API :cpp:func:`esp_at_custom_cmd_array_regist` to register your AT command. Below is the sample code to register ``AT+TEST``:

.. code-block:: c
  
  esp_at_custom_cmd_array_regist(at_custom_cmd, sizeof(at_custom_cmd) / sizeof(at_custom_cmd[0]));

.. note::

  ``esp_at_custom_cmd_array_regist`` is recommended to be added to the ``at_custom_init()`` in ``app_main()``.

.. _user-define-at-commands-give-it-a-try:

Give it a try
-------------

If you have finished the above two steps, the command should work after you build the esp-at project and flash the firmware to your device. Give it a try!

Below is how ``AT+TEST`` works out.

**Test Command:**

.. code-block:: none

    AT+TEST=?

**Response:**

.. code-block:: none

    AT+TEST=?
    this cmd is test cmd: +TEST

    OK

**Query Command:**

.. code-block:: none

    AT+TEST?

**Response:**

.. code-block:: none

    AT+TEST?
    this cmd is query cmd: +TEST

    OK

**Set Command:**

.. code-block:: none

    AT+TEST=1,"espressif"

**Response:**

.. code-block:: none

    AT+TEST=1,"espressif"
    this cmd is setup cmd and cmd num is: 2
    first parameter is: 1
    second parameter is: espressif

    OK

**Execute Command:**

.. code-block:: none

    AT+TEST

**Response:**

.. code-block:: none

    AT+TEST
    this cmd is execute cmd: +TEST

    OK

.. _define-return-values:

Define Return Values
---------------------

``ESP-AT`` has defined return values in :cpp:type:`esp_at_result_code_string_index`. See :ref:`at-messages` for more return values.

In addition to output return values through the return mode, you can also use API :cpp:func:`esp_at_response_result` to output the execution result of the command. :cpp:enumerator:`ESP_AT_RESULT_CODE_SEND_OK` and :cpp:enumerator:`ESP_AT_RESULT_CODE_SEND_FAIL` can be used with the API in code.

For example, when you send data to the server or MCU with the Execute Command of ``AT+TEST``, you can use ``esp_at_response_result`` to output the sending result, and the return mode to output the command execution result. Below is the sample code:

.. code-block:: c

    uint8_t at_exe_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};

        snprintf((char *)buffer, 64, "this cmd is execute cmd: %s\r\n", cmd_name);

        esp_at_port_write_data(buffer, strlen((char *)buffer));

        // user-defined operation of sending data to server or MCU
        send_data_to_server();

        // output SEND OK
        esp_at_response_result(ESP_AT_RESULT_CODE_SEND_OK);

        return ESP_AT_RESULT_CODE_OK;
    }

How it works out:

.. code-block:: none

    AT+TEST
    this cmd is execute cmd: +TEST

    SEND OK

    OK

.. _access-command-parameters:

Access Command Parameters
-------------------------

ESP-AT provides two APIs to access command parameters:

- :cpp:func:`esp_at_get_para_as_digit` obtains digital parameters.
- :cpp:func:`esp_at_get_para_as_str` obtains string parameters.

See :ref:`Set Command <user-defined-set-command>` for an example.

.. _omit-command-parameters:

Omit Command Parameters
------------------------

This section describes how to provide optional command parameters:

- :ref:`omit-the-first-or-middle-parameter`
- :ref:`omit-the-last-parameter`

.. _omit-the-first-or-middle-parameter:

Omit the First or Middle Parameter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Let's say you want to make ``<param_2>`` and ``<param_3>`` of ``AT+TEST`` optional. ``<param_2>`` is a digital parameter, and ``<param_3>`` a string parameter.

.. code-block:: none

    AT+TEST=<param_1>[,<param_2>][,<param_3>],<param_4>

Below is the sample code to achieve it:

.. code-block:: c

    uint8_t at_setup_cmd_test(uint8_t para_num)
    {
        int32_t para_int_1 = 0;
        int32_t para_int_2 = 0;
        uint8_t *para_str_3 = NULL;
        uint8_t *para_str_4 = NULL;
        uint8_t num_index = 0;
        uint8_t buffer[64] = {0};
        esp_at_para_parse_result_type parse_result = ESP_AT_PARA_PARSE_RESULT_OK;

        snprintf((char *)buffer, 64, "this cmd is setup cmd and cmd num is: %u\r\n", para_num);
        esp_at_port_write_data(buffer, strlen((char *)buffer));

        parse_result = esp_at_get_para_as_digit(num_index++, &para_int_1);
        if (parse_result != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        } else {
            memset(buffer, 0, 64);
            snprintf((char *)buffer, 64, "first parameter is: %d\r\n", para_int_1);
            esp_at_port_write_data(buffer, strlen((char *)buffer));
        }

        parse_result = esp_at_get_para_as_digit(num_index++, &para_int_2);
        if (parse_result != ESP_AT_PARA_PARSE_RESULT_OMITTED) {
            if (parse_result != ESP_AT_PARA_PARSE_RESULT_OK) {
                return ESP_AT_RESULT_CODE_ERROR;
            } else {
                // sample code
                // user needs to customize the operation
                memset(buffer, 0, 64);
                snprintf((char *)buffer, 64, "second parameter is: %d\r\n", para_int_2);
                esp_at_port_write_data(buffer, strlen((char *)buffer));
            }
        } else {
            // sample code
            // the second parameter is omitted
            // user needs to customize the operation
            memset(buffer, 0, 64);
            snprintf((char *)buffer, 64, "second parameter is omitted\r\n");
            esp_at_port_write_data(buffer, strlen((char *)buffer));
        }

        parse_result = esp_at_get_para_as_str(num_index++, &para_str_3);
        if (parse_result != ESP_AT_PARA_PARSE_RESULT_OMITTED) {
            if (parse_result != ESP_AT_PARA_PARSE_RESULT_OK) {
                return ESP_AT_RESULT_CODE_ERROR;
            } else {
                // sample code
                // user needs to customize the operation
                memset(buffer, 0, 64);
                snprintf((char *)buffer, 64, "third parameter is: %s\r\n", para_str_3);
                esp_at_port_write_data(buffer, strlen((char *)buffer));
            }
        } else {
            // sample code
            // the third parameter is omitted
            // user needs to customize the operation
            memset(buffer, 0, 64);
            snprintf((char *)buffer, 64, "third parameter is omitted\r\n");
            esp_at_port_write_data(buffer, strlen((char *)buffer));
        }

        parse_result = esp_at_get_para_as_str(num_index++, &para_str_4);
        if (parse_result != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        } else {
            memset(buffer, 0, 64);
            snprintf((char *)buffer, 64, "fourth parameter is: %s\r\n", para_str_4);
            esp_at_port_write_data(buffer, strlen((char *)buffer));
        }

        return ESP_AT_RESULT_CODE_OK;
    }

.. note::

  If the string parameter input is ``""``, it is not omitted.

.. _omit-the-last-parameter:

Omit the Last Parameter
------------------------

Let's say you want to make the string parameter ``<param_3>`` optional, which is also the last parameter.

.. code-block:: none

    AT+TEST=<param_1>,<param_2>[,<param_3>]

There are two cases of omission:

- AT+TEST=<param_1>,<param_2>
- AT+TEST=<param_1>,<param_2>,

Below is the sample code to achieve it:

.. code-block:: c

    uint8_t at_setup_cmd_test(uint8_t para_num)
    {
        int32_t para_int_1 = 0;
        uint8_t *para_str_2 = NULL;
        uint8_t *para_str_3 = NULL;
        uint8_t num_index = 0;
        uint8_t buffer[64] = {0};
        esp_at_para_parse_result_type parse_result = ESP_AT_PARA_PARSE_RESULT_OK;

        snprintf((char *)buffer, 64, "this cmd is setup cmd and cmd num is: %u\r\n", para_num);
        esp_at_port_write_data(buffer, strlen((char *)buffer));

        parse_result = esp_at_get_para_as_digit(num_index++, &para_int_1);
        if (parse_result != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        } else {
            memset(buffer, 0, 64);
            snprintf((char *)buffer, 64, "first parameter is: %d\r\n", para_int_1);
            esp_at_port_write_data(buffer, strlen((char *)buffer));
        }

        parse_result = esp_at_get_para_as_str(num_index++, &para_str_2);
        if (parse_result != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        } else {
            memset(buffer, 0, 64);
            snprintf((char *)buffer, 64, "second parameter is: %s\r\n", para_str_2);
            esp_at_port_write_data(buffer, strlen((char *)buffer));
        }

        if (num_index == para_num) {
            memset(buffer, 0, 64);
            snprintf((char *)buffer, 64, "third parameter is omitted\r\n");
            esp_at_port_write_data(buffer, strlen((char *)buffer));
        } else {
            parse_result = esp_at_get_para_as_str(num_index++, &para_str_3);
            if (parse_result != ESP_AT_PARA_PARSE_RESULT_OMITTED) {
                if (parse_result != ESP_AT_PARA_PARSE_RESULT_OK) {
                    return ESP_AT_RESULT_CODE_ERROR;
                } else {
                    // sample code
                    // user needs to customize the operation
                    memset(buffer, 0, 64);
                    snprintf((char *)buffer, 64, "third parameter is: %s\r\n", para_str_3);
                    esp_at_port_write_data(buffer, strlen((char *)buffer));
                }
            } else {
                // sample code
                // the third parameter is omitted
                // user needs to customize the operation
                memset(buffer, 0, 64);
                snprintf((char *)buffer, 64, "third parameter is omitted\r\n");
                esp_at_port_write_data(buffer, strlen((char *)buffer));
            }
        }

        return ESP_AT_RESULT_CODE_OK;
    }

.. note::

  If the string parameter input is ``""``, it is not omitted.

.. _block-command-execution:

Block Command Execution
------------------------

Sometimes you want to block the execution of one command to wait for another execution result, and the system may return different values according to the result.

Generally, this kind of command needs to synchronize the results of other tasks.

``semaphore`` is recommended to handle synchronization.

The sample code is as follows:

.. code-block:: c

    xSemaphoreHandle at_operation_sema = NULL;

    uint8_t at_exe_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};

        snprintf((char *)buffer, 64, "this cmd is execute cmd: %s\r\n", cmd_name);

        esp_at_port_write_data(buffer, strlen((char *)buffer));

        // sample code
        // users don't have to create semaphores here
        at_operation_sema = xSemaphoreCreateBinary();
        assert(at_operation_sema != NULL);

        // block command execution
        // wait for another execution result
        // other tasks can call xSemaphoreGive to release the semaphore
        xSemaphoreTake(at_operation_sema, portMAX_DELAY);

        return ESP_AT_RESULT_CODE_OK;
    }

.. _access-input-data-from-at-command-port:

Access Input Data from AT Command Port
--------------------------------------

``ESP-AT`` supports accessing input data from AT Command port. It provides two APIs for this purpose.

- :cpp:func:`esp_at_port_enter_specific` sets the callback function which will be called by AT port after receiving the input data.
- :cpp:func:`esp_at_port_exit_specific` deletes the callback function set by ``esp_at_port_enter_specific``.

Approaches to access the data vary depending on whether the data length has been specified or not.

Input Data of Specified Length
------------------------------

Assuming that you have specified the data length in ``<param_1>`` as follows:

.. code-block:: none

    AT+TEST=<param_1>

Below is the sample to access the input data of ``<param_1>`` length from AT Command Port:

.. code-block:: c

    static xSemaphoreHandle at_sync_sema = NULL;

    void wait_data_callback(void)
    {
        xSemaphoreGive(at_sync_sema);
    }

    uint8_t at_setup_cmd_test(uint8_t para_num)
    {
        int32_t specified_len = 0;
        int32_t received_len = 0;
        int32_t remain_len = 0;
        uint8_t *buf = NULL;
        uint8_t buffer[64] = {0};

        if (esp_at_get_para_as_digit(0, &specified_len) != ESP_AT_PARA_PARSE_RESULT_OK) {
            return ESP_AT_RESULT_CODE_ERROR;
        }

        buf = (uint8_t *)malloc(specified_len);
        if (buf == NULL) {
            memset(buffer, 0, 64);
            snprintf((char *)buffer, 64, "malloc failed\r\n");
            esp_at_port_write_data(buffer, strlen((char *)buffer));
        }

        // sample code
        // users don't have to create semaphores here
        if (!at_sync_sema) {
            at_sync_sema = xSemaphoreCreateBinary();
            assert(at_sync_sema != NULL);
        }

        // output input prompt ">"
        esp_at_port_write_data((uint8_t *)">", strlen(">"));

        // set the callback function which will be called by AT port after receiving the input data
        esp_at_port_enter_specific(wait_data_callback);

        // receie input data
        while(xSemaphoreTake(at_sync_sema, portMAX_DELAY)) {
            received_len += esp_at_port_read_data(buf + received_len, specified_len - received_len);

            if (specified_len == received_len) {
                esp_at_port_exit_specific();

                // get the length of the remaining input data
                remain_len = esp_at_port_get_data_length();
                if (remain_len > 0) {
                    // sample code
                    // if the remaining data length > 0, the actual input data length is greater than the specified received data length
                    // users can customize the operation to process the remaining data
                    // here is just a simple print out of the remaining data
                    esp_at_port_recv_data_notify(remain_len, portMAX_DELAY);
                }

                // sample code
                // output received data
                memset(buffer, 0, 64);
                snprintf((char *)buffer, 64, "\r\nreceived data is: ");
                esp_at_port_write_data(buffer, strlen((char *)buffer));

                esp_at_port_write_data(buf, specified_len);

                break;
            }
        }

        free(buf);

        return ESP_AT_RESULT_CODE_OK;
    }

So, if you set ``AT+TEST=5`` and the input data is ``1234567890``, the ``ESP-AT`` output is as follows.

.. code-block:: none

    AT+TEST=5
    >67890
    received data is: 12345
    OK

Input Data of Unspecified Length
--------------------------------

This scenario is similar to the Wi-Fi :term:`Passthrough Mode`. You do not specify the data length.

::

    AT+TEST

Assuming that ``ESP-AT`` ends the execution of the command and returns the execution result, the sample code is as follows:

.. code-block:: c

    #define BUFFER_LEN (2048)
    static xSemaphoreHandle at_sync_sema = NULL;

    void wait_data_callback(void)
    {
        xSemaphoreGive(at_sync_sema);
    }

    uint8_t at_exe_cmd_test(uint8_t *cmd_name)
    {
        int32_t received_len = 0;
        int32_t remain_len = 0;
        uint8_t *buf = NULL;
        uint8_t buffer[64] = {0};


        buf = (uint8_t *)malloc(BUFFER_LEN);
        if (buf == NULL) {
            memset(buffer, 0, 64);
            snprintf((char *)buffer, 64, "malloc failed\r\n");
            esp_at_port_write_data(buffer, strlen((char *)buffer));
        }

        // sample code
        // users don't have to create semaphores here
        if (!at_sync_sema) {
            at_sync_sema = xSemaphoreCreateBinary();
            assert(at_sync_sema != NULL);
        }

        // output input prompt ">"
        esp_at_port_write_data((uint8_t *)">", strlen(">"));

        // set the callback function which will be called by AT port after receiving the input data
        esp_at_port_enter_specific(wait_data_callback);

        // receie input data
        while(xSemaphoreTake(at_sync_sema, portMAX_DELAY)) {
            memset(buf, 0, BUFFER_LEN);

            received_len = esp_at_port_read_data(buf, BUFFER_LEN);
            // check whether to exit the mode
            // the exit condition is the “+++” string received
            if ((received_len == 3) && (strncmp((const char *)buf, "+++", 3)) == 0) {
                esp_at_port_exit_specific();

                // sample code
                // if the remaining data length > 0, it means that there is still data left in the buffer to be processed
                // users can customize the operation to process the remaining data
                // here is just a simple print out of the remaining data
                remain_len = esp_at_port_get_data_length();
                if (remain_len > 0) {
                    esp_at_port_recv_data_notify(remain_len, portMAX_DELAY);
                }

                break;
            } else if (received_len > 0) {
                // sample code
                // users can customize the operation to process the received data
                // here is just a simple print received data
                memset(buffer, 0, 64);
                snprintf((char *)buffer, 64, "\r\nreceived data is: ");
                esp_at_port_write_data(buffer, strlen((char *)buffer));

                esp_at_port_write_data(buf, strlen((char *)buf));
            }
        }

        free(buf);

        return ESP_AT_RESULT_CODE_OK;
    }

So, if the first input data is ``1234567890``, and the second input data is ``+++``, the ``ESP-AT`` output is as follows:

.. code-block:: none

    AT+TEST
    >
    received data is: 1234567890
    OK