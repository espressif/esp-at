How to Add User-defined AT Commands
====================================

{IDF_TARGET_VER: default="undefined", esp32="5.0", esp32c2="5.0", esp32c3="5.0", esp32c6="5.1", esp32s2="5.0"}

:link_to_translation:`zh_CN:[中文]`

This document provides guidance on how to add custom AT commands. It uses the example of `at_custom_cmd <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd>`_ to demonstrate each step with the sample code.

To define a basic, functional command, at least the following six steps are required:

- :ref:`define-at-commands`
- :ref:`register-at-commands`
- :ref:`add-component_dependencies`
- :ref:`add-link_options`
- :ref:`set-component-env-and-compile`
- :ref:`user-compile_at`

After completing the above steps, please :ref:`execute the AT+TEST command to get result <user-at-cmd-give-it-a-try>`.

To customize relatively complex commands, please refer to the sample codes below:

- :ref:`define-return-values`
- :ref:`access-command-parameters`
- :ref:`omit-command-parameters`
- :ref:`block-command-execution`
- :ref:`access-input-data-from-at-command-port`

The source code for the AT command set is not open source and is presented in the form of :component:`library files <at/lib>`, which is also the basis for parsing custom AT commands.

.. _step-define_at_command:

Customize AT Commands
-----------------------------------------------------

.. _define-at-commands:

Step 1: Define AT Commands
************************************************

You can define AT commands in the files `at_custom_cmd.c <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd/custom/at_custom_cmd.c>`_ and `at_custom_cmd.h <https://github.com/espressif/esp-at/blob/master/examples/at_custom_cmd/include/at_custom_cmd.h>`_, or create new source files and header files in the directories `examples/at_custom_cmd/custom <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd/custom/>`_ and `examples/at_custom_cmd/include <https://github.com/espressif/esp-at/blob/master/examples/at_custom_cmd/include/>`_ to define AT commands.

Before customizing AT commands, please first determine the name and type of the AT command.

**Command naming rules:**

- Start a command with ``+`` character.
- Alphabetic characters (``A~Z, a~z``), numeric characters (``0~9``), and some other characters (``!``, ``%``, ``-``, ``.``, ``/``, ``:``, ``_``) are supported. See :ref:`at-command-types` for more information.

**Command types:**

Each AT command can have up to four types: Test Command, Query Command, Set Command, and Execute Command. See :ref:`at-command-types` for more information.

Then, define desired type of command. Assuming that ``AT+TEST`` supports all the four types. Below is the code to define the name and types of the AT command, as well as sample code to define each type.

- First, call :cpp:type:`esp_at_cmd_struct` to define the name and type(s) that your AT command supports. The sample code below defined the name ``+TEST`` (omitting ``AT``) and all the four types.

    .. code-block:: c
    
        static const esp_at_cmd_struct at_custom_cmd[] = {
            {"+TEST", at_test_cmd_test, at_query_cmd_test, at_setup_cmd_test, at_exe_cmd_test},
            /**
             * @brief You can define your own AT commands here.
             */
        };

    .. note::
      If you do not want to define a particular type, set it to ``NULL``.

- Test Command:

    .. code-block:: c
    
        static uint8_t at_test_cmd_test(uint8_t *cmd_name)
        {
            uint8_t buffer[64] = {0};
            snprintf((char *)buffer, 64, "test command: <AT%s=?> is executed\r\n", cmd_name);
            esp_at_port_write_data(buffer, strlen((char *)buffer));
    
            return ESP_AT_RESULT_CODE_OK;
        }

- Query Command:

    .. code-block:: c
    
        static uint8_t at_query_cmd_test(uint8_t *cmd_name)
        {
            uint8_t buffer[64] = {0};
            snprintf((char *)buffer, 64, "query command: <AT%s?> is executed\r\n", cmd_name);
            esp_at_port_write_data(buffer, strlen((char *)buffer));
    
            return ESP_AT_RESULT_CODE_OK;
        }

.. _user-defined-set-command:

- Set Command:

    .. code-block:: c
    
        static uint8_t at_setup_cmd_test(uint8_t para_num)
        {
            uint8_t index = 0;
    
            // get first parameter, and parse it into a digit
            int32_t digit = 0;
            if (esp_at_get_para_as_digit(index++, &digit) != ESP_AT_PARA_PARSE_RESULT_OK) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
    
            // get second parameter, and parse it into a string
            uint8_t *str = NULL;
            if (esp_at_get_para_as_str(index++, &str) != ESP_AT_PARA_PARSE_RESULT_OK) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
    
            // allocate a buffer and construct the data, then send the data to mcu via interface (uart/spi/sdio/socket)
            uint8_t *buffer = (uint8_t *)malloc(512);
            if (!buffer) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            int len = snprintf((char *)buffer, 512, "setup command: <AT%s=%d,\"%s\"> is executed\r\n",
                               esp_at_get_current_cmd_name(), digit, str);
            esp_at_port_write_data(buffer, len);
    
            // remember to free the buffer
            free(buffer);
    
            return ESP_AT_RESULT_CODE_OK;
        }

- Execute Command:

    .. code-block:: c
    
        static uint8_t at_exe_cmd_test(uint8_t *cmd_name)
        {
            uint8_t buffer[64] = {0};
            snprintf((char *)buffer, 64, "execute command: <AT%s> is executed\r\n", cmd_name);
            esp_at_port_write_data(buffer, strlen((char *)buffer));
    
            return ESP_AT_RESULT_CODE_OK;
        }

.. _register-at-commands:

Step 2: Register AT Command Functions
***********************************************************************************

- Please define the :cpp:type:`esp_at_custom_cmd_register` function and call the API :cpp:func:`esp_at_custom_cmd_array_regist` to register AT commands.

  Sample code:

  .. code-block:: c
  
      bool esp_at_custom_cmd_register(void)
      {
          return esp_at_custom_cmd_array_regist(at_custom_cmd, sizeof(at_custom_cmd) / sizeof(esp_at_cmd_struct));
      }

- Then, call the API `ESP_AT_CMD_SET_INIT_FN <https://github.com/espressif/esp-at/blob/113702d9bf0224ed15e873bdc09898e804f4bd28/components/at/include/esp_at_cmd_register.h#L67>`_ to initialize your implemented registration AT command function :cpp:type:`esp_at_custom_cmd_register`.

  Sample code:

  .. code-block:: c

      ESP_AT_CMD_SET_INIT_FN(esp_at_custom_cmd_register, 1);

.. note::
  To customize AT commands in the ``examples/at_custom_cmd/custom`` and ``examples/at_custom_cmd/include`` directories, please avoid naming the registered AT command function :cpp:type:`esp_at_custom_cmd_register`, as this function is already defined and initialized in the `at_custom_cmd <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd>`_ example. Instead, name it something like :cpp:type:`esp_at_custom_cmd_register_foo`, and use :cpp:enumerator:`ESP_AT_CMD_SET_INIT_FN` to initialize it.

.. _add-component_dependencies:

Step 3: Add Component Dependencies
***********************************

If you use components other than `at <https://github.com/espressif/esp-at/tree/master/components/at>`_, `freertos <https://github.com/espressif/esp-idf/tree/release/v{IDF_TARGET_VER}/components/freertos>`_, `nvs_flash <https://github.com/espressif/esp-idf/tree/release/v{IDF_TARGET_VER}/components/nvs_flash>`_ during :ref:`define-at-commands`, please add these component dependencies in the ``examples/at_custom_cmd/CMakeLists.txt`` file. Otherwise, you can skip this step. For example, if you additionally use the `lwip <https://github.com/espressif/esp-idf/tree/release/v{IDF_TARGET_VER}/components/lwip>`_ component, the sample code is as follows:

.. code-block:: none

    set(require_components at freertos nvs_flash lwip)

.. _add-link_options:

Step 4: Add Link Options
**************************

Please link the name of your custom :ref:`registered AT command function <register-at-commands>` as a link option to ${COMPONENT_LIB} in the ``examples/at_custom_cmd/CMakeLists.txt`` file to ensure that the program can find this function at runtime. The sample code is as follows:

.. code-block:: none

    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_custom_cmd_register")

.. note::
  If the name of the custom :ref:`registered AT command function <register-at-commands>` is :cpp:type:`esp_at_custom_cmd_register_foo`, the sample code is as follows:

  .. code-block:: none

      target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_custom_cmd_register_foo")

.. _set-component-env-and-compile:

Step 5: Set Component Environment Variables
********************************************************

This section introduces two methods for setting the ``at_custom_cmd`` component environment variables to ensure that the ESP-AT project can locate this component correctly during compilation. Choose the method that best suits your needs. If you customize AT commands or modify code in the original components under the ``esp-at/components`` directory, you do not need to perform this step. However, it is not recommended to customize AT commands in the original components under the ``esp-at/components`` directory, and this document does not explain this.

**Method 1:** Set the ``AT_CUSTOM_COMPONENTS`` environment variable directly in the command line (Suitable for :doc:`local compilation <../Compile_and_Develop/How_to_clone_project_and_compile_it>`).

    - Linux or macOS

    .. code-block:: none

        export AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)
     
    - Windows

    .. code-block:: none

        set AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)

    .. note::
        - Please replace ``(path_of_at_custom_cmd)`` with the actual absolute path of the ``at_custom_cmd`` directory.
        - You can specify multiple components. For example:

          ``export AT_CUSTOM_COMPONENTS="~/prefix/my_path1 ~/prefix/my_path2"``

**Method 2:** Add the code to set the ``AT_CUSTOM_COMPONENTS`` environment variable in the `esp-at/build.py <https://github.com/espressif/esp-at/tree/master/build.py>`_ file's :cpp:type:`setup_env_variables()` function. (Suitable for :doc:`local compilation <../Compile_and_Develop/How_to_clone_project_and_compile_it>` and :doc:`web compilation <../Compile_and_Develop/How_to_build_project_with_web_page>`). The sample code is as follows:

    .. code-block:: none

        # set AT_CUSTOM_COMPONENTS
        at_custom_cmd_path=os.path.join(os.getcwd(), 'examples/at_custom_cmd')
        os.environ['AT_CUSTOM_COMPONENTS']=at_custom_cmd_path

.. _user-compile_at:

Step 6: Compile the AT Firmware
****************************************

After completing these steps, choose either :doc:`web compilation <../Compile_and_Develop/How_to_build_project_with_web_page>` or :doc:`local compilation <../Compile_and_Develop/How_to_clone_project_and_compile_it>` of the AT firmware according to your needs, and :doc:`flash <../Get_Started/Downloading_guide>` it to your device.

.. _user-at-cmd-give-it-a-try:

Execute the AT+TEST Command to Get Result
**************************************************************

If you have followed the steps correctly, below is the execution result of the ``AT+TEST`` command you defined.

**Test Command:**

.. code-block:: none

    AT+TEST=?

**Response:**

.. code-block:: none

    AT+TEST=?
    test command: <AT+TEST=?> is executed

    OK

**Query Command:**

.. code-block:: none

    AT+TEST?

**Response:**

.. code-block:: none

    AT+TEST?
    query command: <AT+TEST?> is executed

    OK

**Set Command:**

.. code-block:: none

    AT+TEST=1,"espressif"

**Response:**

.. code-block:: none

    AT+TEST=1,"espressif"
    setup command: <AT+TEST=1,"espressif"> is executed

    OK

**Execute Command:**

.. code-block:: none

    AT+TEST

**Response:**

.. code-block:: none

    AT+TEST
    execute command: <AT+TEST> is executed

    OK

Customize Complex AT Commands
-------------------------------

The sample codes below are used to customize more complex commands, from which you can choose based on personal needs.

.. _define-return-values:

Define Return Values
******************************************************

ESP-AT has defined return values in :cpp:type:`esp_at_result_code_string_index`. See :ref:`at-messages` for more return values.

In addition to output return values through the return mode, you can also use API :cpp:func:`esp_at_response_result` to output the execution result of the command. :cpp:enumerator:`ESP_AT_RESULT_CODE_SEND_OK` and :cpp:enumerator:`ESP_AT_RESULT_CODE_SEND_FAIL` can be used with the API in code.

For example, when you send data to the server or MCU with the Execute Command of ``AT+TEST``, you can use :cpp:func:`esp_at_response_result` to output the sending result, and the return mode to output the command execution result. Below is the sample code:

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
*********************************************************

ESP-AT provides two APIs to access command parameters:

- :cpp:func:`esp_at_get_para_as_digit` obtains digital parameters.
- :cpp:func:`esp_at_get_para_as_str` obtains string parameters.

See :ref:`Set Command <user-defined-set-command>` for an example.

.. _omit-command-parameters:

Omit Command Parameters
*******************************************************

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
^^^^^^^^^^^^^^^^^^^^^^^

Let's say you want to make the string parameter ``<param_3>`` of ``AT+TEST`` optional, which is also the last parameter.

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
******************************************************

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
        // users do not have to create semaphores here
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
********************************************************************

ESP-AT supports accessing input data from AT Command port. It provides two APIs for this purpose.

- :cpp:func:`esp_at_port_enter_specific` sets the callback function which will be called by AT port after receiving the input data.
- :cpp:func:`esp_at_port_exit_specific` deletes the callback function set by ``esp_at_port_enter_specific``.

Approaches to access the data vary depending on whether the data length has been specified or not.

Input Data of Specified Length
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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
        // users do not have to create semaphores here
        if (!at_sync_sema) {
            at_sync_sema = xSemaphoreCreateBinary();
            assert(at_sync_sema != NULL);
        }

        // output input prompt ">"
        esp_at_port_write_data((uint8_t *)">", strlen(">"));

        // set the callback function which will be called by AT port after receiving the input data
        esp_at_port_enter_specific(wait_data_callback);

        // receive input data
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
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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
        // users do not have to create semaphores here
        if (!at_sync_sema) {
            at_sync_sema = xSemaphoreCreateBinary();
            assert(at_sync_sema != NULL);
        }

        // output input prompt ">"
        esp_at_port_write_data((uint8_t *)">", strlen(">"));

        // set the callback function which will be called by AT port after receiving the input data
        esp_at_port_enter_specific(wait_data_callback);

        // receive input data
        while(xSemaphoreTake(at_sync_sema, portMAX_DELAY)) {
            memset(buf, 0, BUFFER_LEN);

            received_len = esp_at_port_read_data(buf, BUFFER_LEN);
            // check whether to exit the mode
            // the exit condition is the "+++" string received
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
