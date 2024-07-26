.. _user-defined_at_cmd:

************************************
How to Add User-defined AT Commands
************************************

:link_to_translation:`en:[English]`

This document provides a detailed introduction on how to add user-defined AT commands. In fact, the ESP-AT project already provides the `at_custom_cmd example <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd>`_, and the following text mainly focuses on this example, using the ``AT+TEST`` command to demonstrate each step with example code.

Before customizing AT commands, please understand:

- :ref:`at-custom_cmd`

To define a basic, functional command, at least the following four steps are required:

- :ref:`define-at-commands`
- :ref:`register-at-commands`
- :ref:`update-cmakelist`
- :ref:`set-component-env-and-compile`

After completing the above steps, let's look at the operation and response of the new command (AT+TEST):

- :ref:`user-at-cmd-give-it-a-try`

The following steps are suitable for customizing relatively complex commands and can be selected according to your needs:

- :ref:`define-return-values`
- :ref:`access-command-parameters`
- :ref:`omit-command-parameters`
- :ref:`block-command-execution`
- :ref:`access-input-data-from-at-command-port`

The source code for the AT command set is not open source and is presented in the form of :component:`library files <at/lib>`, which is also the basis for parsing custom AT commands.

.. _at-custom_cmd:

:ref:`at_custom_cmd component introduction <user-defined_at_cmd>`
==================================================================

Introduction to the at_custom_cmd component
--------------------------------------------

You can first read the `README <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd/README.md>`_ of the at_custom_cmd component to get a brief understanding of it.

For concepts and related knowledge about components, refer to the `ESP-IDF Build System documentation <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/build-system.html#id1>`_.

.. _at_custom_cmd_component:

at_custom_cmd Component Directory Structure
--------------------------------------------

The directory structure of the at_custom_cmd component is as follows:

.. code-block:: none

    - at_custom_cmd/
                    - custom/ - at_custom_cmd.c
                    - include/ - at_custom_cmd.h
                    - CMakeLists.txt
                    - README.md

This example project "at_custom_cmd" includes the following components:

  - The .c files under the at_custom_cmd/custom/ directory are used to store the source code for :ref:`custom AT commands <define-at-commands>`. Besides the at_custom_cmd.c file, you can create additional .c files in this directory.
  - The .h files under the at_custom_cmd/include/ directory are used to store the header files for custom AT commands. Besides the at_custom_cmd.h file, you can create additional .h files in this directory.
  - The CMakeLists.txt file defines variables to control the build process of this component and its integration with the ESP-AT project. For more detailed information, refer to `Component CMakeLists File <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/build-system.html#component-directories>`_.
  - The README.md file is used to describe this component.

at_custom_cmd Component CMakeLists.txt File
--------------------------------------------

- Add the path of the `.c` files to the `srcs` variable:

  .. code-block:: none

      file(GLOB_RECURSE srcs *.c)

- Set the include directory for the header file (at_custom_cmd.h file):

  .. code-block:: none
    
      set(includes "include")

.. _add-component_dependencies:

- Add component dependencies and add the component to the build system:

  - Add the required component dependencies in the CMakeLists.txt file as needed for the :ref:`custom AT command code <define-at-commands>`. Use idf_component_register to add the component to the build system. For more details, refer to `idf_component_register <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html#cmake-component-register>`_.

  .. code-block:: none

      set(require_components at freertos nvs_flash)

      idf_component_register(
        SRCS ${srcs}
        INCLUDE_DIRS ${includes}
        REQUIRES ${require_components})

  .. note::
    If you use other components in the :ref:`custom AT command code <define-at-commands>`, you need to add these component dependencies in the `esp-at/examples/at_custom_cmd/CMakeLists.txt` file. For example, if you also use the LwIP component, your setting should be as follows:

    .. code-block:: none

        set(require_components at freertos nvs_flash lwip)

.. _add-link_options:

- Link the name of your :ref:`custom registered AT command function <register-at-commands>` as a link option to ${COMPONENT_LIB} to ensure that the program can find this custom registered AT command function correctly at runtime. The following link example uses the `esp_at_custom_cmd_register` registration of AT command function in the at_custom_cmd example:

  .. code-block:: none

      target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_custom_cmd_register")

  .. note::
    If the name of your :ref:`custom registered AT command function <register-at-commands>` is ``esp_at_custom_cmd_register_1``, you need to add the following code:

    .. code-block:: none

        target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_custom_cmd_register_1")

.. _component-usage:

Usage of the at_custom_cmd component
-------------------------------------

1. Add your custom AT command code to the `at_custom_cmd/custom/**/*.c` and `at_custom_cmd/custom/**/*.h` files (you can also create new source and header files if needed, ensuring that the AT command registration function is handled in the .c files).
2. If your code uses other components, you may need to add more component dependencies in the `at_custom_cmd/CMakeLists.txt` file.

.. _compinents-3:

3. Set the `at_custom_cmd` component environment variable so that the `ESP-AT` project can find this component during compilation. Choose one of the following two methods:

.. _set-path_1:

- **Method 1:** Set the `AT_CUSTOM_COMPONENTS` environment variable directly in the command line

    .. note::
        - You need to replace (path_of_at_custom_cmd) with the actual absolute path of the `at_custom_cmd` directory.
        - You can specify multiple components. For example: `export AT_CUSTOM_COMPONENTS="~/prefix/my_path1 ~/prefix/my_path2"`.

    - Linux or macOS

    .. code-block:: none

        export AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)
     
    - Windows

    .. code-block:: none

        set AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)

.. _set-path_2:

- **Method 2:** Directly add the code to set the AT_CUSTOM_COMPONENTS environment variable in the `esp-at/build.py` file's ``setup_env_variables()`` function. The code example is as follows:

    .. code-block:: none

        # set AT_CUSTOM_COMPONENTS
        at_custom_cmd_path=os.path.join(os.getcwd(), 'examples/at_custom_cmd')
        os.environ['AT_CUSTOM_COMPONENTS']=at_custom_cmd_path

.. _compile-at:

4. Choose one of the following two methods to recompile the AT firmware

  - **Method 1:** Recompile the AT firmware according to :doc:`local compilation guide for the ESP-AT project <../Compile_and_Develop/How_to_clone_project_and_compile_it>`.
  - **Method 2:** Recompile the AT firmware according to :doc:`web compilation guide for the ESP-AT project <../Compile_and_Develop/How_to_build_project_with_web_page>` (if you choose this method to compile the AT firmware, the third step :ref:`component-usage` must use :ref:`Method 2 <set-path_2>`).

.. _step-define_at_command:

:ref:`Custom AT Command Steps <user-defined_at_cmd>`
====================================================

.. _define-at-commands:

:ref:`Custom AT Commands <user-defined_at_cmd>`
------------------------------------------------

Before customizing AT commands, please decide on the name and type of the AT command.

**Command naming rules:**

- It should start with the ``+`` character.
- Alphabetic characters (``A~Z, a~z``), numeric characters (``0~9``), and some other characters (``!``, ``%``, ``-``, ``.``, ``/``, ``:``, ``_``) are supported. See :ref:`at-command-types` for more information.

**Command types:**

Each AT command can have up to four types: Test Command, Query Command, Set Command, and Execute Command. See :ref:`at-command-types` for more information.

Then, define desired type of command. Assuming that ``AT+TEST`` supports all the four types. Below is the sample code to define each type.

Test Command:

.. code-block:: c

    static uint8_t at_test_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};
        snprintf((char *)buffer, 64, "test command: <AT%s=?> is executed\r\n", cmd_name);
        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

Query Command:

.. code-block:: c

    static uint8_t at_query_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};
        snprintf((char *)buffer, 64, "query command: <AT%s?> is executed\r\n", cmd_name);
        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

.. _user-defined-set-command:

Set Command:

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

Execute Command:

.. code-block:: c

    static uint8_t at_exe_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};
        snprintf((char *)buffer, 64, "execute command: <AT%s> is executed\r\n", cmd_name);
        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

Finally, call :cpp:type:`esp_at_cmd_struct` to define the name and type(s) that your AT command supports. The sample code below defined the name ``+TEST`` (omitting ``AT``) and all the four types.

.. note::
  If you do not want to define a particular type, set it to ``NULL``.

.. code-block:: c

    static const esp_at_cmd_struct at_custom_cmd[] = {
        {"+TEST", at_test_cmd_test, at_query_cmd_test, at_setup_cmd_test, at_exe_cmd_test},
        /**
         * @brief You can define your own AT commands here.
         */
    };

.. _register-at-commands:

:ref:`Registering AT Command Functions and Initialization <user-defined_at_cmd>`
----------------------------------------------------------------------------------

In the at_custom_cmd example, the esp_at_custom_cmd_register function calls the API :cpp:func:`esp_at_custom_cmd_array_regist` to register the AT command. Below is an example code for registering ``AT+TEST``.

.. code-block:: c
  
    bool esp_at_custom_cmd_register(void)
    {
        return esp_at_custom_cmd_array_regist(at_custom_cmd, sizeof(at_custom_cmd) / sizeof(esp_at_cmd_struct));
    }

Finally, call the API `ESP_AT_CMD_SET_INIT_FN <https://github.com/espressif/esp-at/blob/113702d9bf0224ed15e873bdc09898e804f4bd28/components/at/include/esp_at_cmd_register.h#L67>`_ to initialize your implemented registration AT command function `esp_at_custom_cmd_register`. Below is an example code for initializing the registration of the `AT+TEST` command.

.. code-block:: c

    ESP_AT_CMD_SET_INIT_FN(esp_at_custom_cmd_register, 1);

.. note::
  If you choose not to define AT commands in the `at_custom_cmd.c` and `at_custom_cmd.h` files, but instead create new source and header files in the `esp-at/examples/at_custom_cmd/custom` and `esp-at/examples/at_custom_cmd/include` directories to customize AT commands and the custom registration function, you should avoid naming your registration function `esp_at_custom_cmd_register`. This is because there is already a function named esp_at_custom_cmd_register in the at_custom_cmd example, which has been initialized. You can define a function with a different name to register AT commands, and then use ESP_AT_CMD_SET_INIT_FN to initialize your defined registration function.

.. _update-cmakelist:

:ref:`Update the CMakeLists.txt File <user-defined_at_cmd>`
------------------------------------------------------------

Now you need to update the `CMakeLists.txt file <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd/CMakeLists.txt>`_ based on the :ref:`custom AT command code <register-at-commands>` and :ref:`custom register AT command function and initialization code <register-at-commands>` implemented above. Pay special attention to the following two points:

1. :ref:`Add component dependencies <add-component_dependencies>`
2. :ref:`Add link options <add-link_options>`

.. _set-component-env-and-compile:

:ref:`Setting Component Environment Variables and Compiling the AT Project <user-defined_at_cmd>`
-----------------------------------------------------------------------------------------------------

- Please choose one of the following two methods to recompile the AT firmware.

    - **Method 1:** If you choose to recompile the AT firmware according to the :doc:`ESP-AT Project Compilation Guide <How_to_clone_project_and_compile_it>`, please set the environment variables for the at_custom_cmd component using one of the two methods described in the `Setting Environment Variables Document <compinents-3>`_ before you start compiling the AT project.
    - **Method 2:** If you choose to recompile the AT firmware according to the :doc:`Web Compilation Guide for the ESP-AT Project <How_to_build_project_with_web_page>`, please set the environment variables for the at_custom_cmd component using the :ref:`second method <set-path_2>` in the `Setting Environment Variables Document <compinents-3>`_ before you start compiling the AT project.

- Please `flash the AT firmware you compiled <../Get_Started/Downloading_guide>`_.

.. _user-at-cmd-give-it-a-try:

:ref:`Give the AT+TEST Command a Try <user-defined_at_cmd>`
------------------------------------------------------------------------

If you have completed the four steps of :ref:`define-at-commands`, :ref:`register-at-commands`, :ref:`update-cmakelist`, and :ref:`set-component-env-and-compile`, the `AT+TEST` command should run correctly on your device. Give it a try!

If you have followed the steps correctly, below is the execution result of the `AT+TEST` command you defined.

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

Custom Complex AT Command Code
===============================

**If you have completed the above steps, you have successfully created a simple AT+TEST command. Below are some code examples suitable for defining relatively complex commands, which you can choose based on your needs.**

.. _define-return-values:

:ref:`Define Return Values <user-defined_at_cmd>`
--------------------------------------------------

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

:ref:`Access Command Parameters <user-defined_at_cmd>`
-------------------------------------------------------

ESP-AT provides two APIs to access command parameters:

- :cpp:func:`esp_at_get_para_as_digit` obtains digital parameters.
- :cpp:func:`esp_at_get_para_as_str` obtains string parameters.

See :ref:`Set Command <user-defined-set-command>` for an example.

.. _omit-command-parameters:

:ref:`Omit Command Parameters <user-defined_at_cmd>`
-----------------------------------------------------

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

:ref:`Block Command Execution <user-defined_at_cmd>`
-----------------------------------------------------

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

:ref:`Access Input Data from AT Command Port <user-defined_at_cmd>`
--------------------------------------------------------------------

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
