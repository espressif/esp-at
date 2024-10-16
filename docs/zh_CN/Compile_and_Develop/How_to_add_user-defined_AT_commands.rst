添加自定义 AT 命令
====================

{IDF_TARGET_VER: default="undefined", esp32="5.0", esp32c2="5.0", esp32c3="5.0", esp32c6="5.1", esp32s2="5.0"}

:link_to_translation:`en:[English]`

本文档介绍了如何添加自定义的 AT 命令，以 `at_custom_cmd <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd>`_ 为例，展示每个步骤的示例代码。

自定义基本的、功能良好的命令，请参考以下步骤。

- :ref:`define-at-commands`
- :ref:`register-at-commands`
- :ref:`add-component_dependencies`
- :ref:`add-link_options`
- :ref:`set-component-env-and-compile`
- :ref:`user-compile_at`

正确完成上述步骤后，请 :ref:`user-at-cmd-give-it-a-try`。

自定义相对复杂的命令，参考示例如下：

- :ref:`define-return-values`
- :ref:`access-command-parameters`
- :ref:`omit-command-parameters`
- :ref:`block-command-execution`
- :ref:`access-input-data-from-at-command-port`

AT 命令集的源代码不开源，以 :component:`库文件 <at/lib>` 的形式呈现，它也是解析自定义的 AT 命令的基础。

.. _step-define_at_command:

自定义 AT 命令
-----------------

.. _define-at-commands:

第一步：定义 AT 命令
***********************

您可在 `at_custom_cmd.c <https://github.com/espressif/esp-at/blob/master/examples/at_custom_cmd/custom/at_custom_cmd.c>`_ 和 `at_custom_cmd.h <https://github.com/espressif/esp-at/blob/master/examples/at_custom_cmd/include/at_custom_cmd.h>`_ 文件中定义 AT 命令，也可在 `examples/at_custom_cmd/custom <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd/custom/>`_ 和 `examples/at_custom_cmd/include <https://github.com/espressif/esp-at/blob/master/examples/at_custom_cmd/include/>`_ 目录下创建新的源文件和头文件定义 AT 命令。

在自定义 AT 命令之前，请先确定 AT 命令的名称和类型。

**命令命名规则：**

- 命令应以 ``+`` 符号开头。
- 支持字母 (``A~Z, a~z``)、数字 (``0~9``) 及其它一些字符 (``!``、 ``%``、``-``、``.``、``/``、``:``、``_``)，详情请见 :ref:`at-command-types`。

**命令类型：**

每条 AT 命令最多可以有四种类型：测试命令、查询命令、设置命令和执行命令，更多信息参见 :ref:`at-command-types`。

然后，定义所需类型的命令。假设 ``AT+TEST`` 支持所有的四种类型，下面是定义 AT 命令的名称和所需支持的类型，以及定义每种类型的示例代码。

- 首先，调用 :cpp:type:`esp_at_cmd_struct` 来定义 AT 命令的名称和所需支持的类型，下面的示例代码定义了名称为 ``+TEST``（省略了 ``AT``）并支持所有四种类型的命令。

    .. code-block:: c
    
        static const esp_at_cmd_struct at_custom_cmd[] = {
            {"+TEST", at_test_cmd_test, at_query_cmd_test, at_setup_cmd_test, at_exe_cmd_test},
            /**
             * @brief 您可以在此处定义自己的 AT 命令
             */
        };
    
    .. note::
      如果不定义某个类型，将其设置为 ``NULL``。

- 测试命令：

    .. code-block:: c
    
        static uint8_t at_test_cmd_test(uint8_t *cmd_name)
        {
            uint8_t buffer[64] = {0};
            snprintf((char *)buffer, 64, "test command: <AT%s=?> is executed\r\n", cmd_name);
            esp_at_port_write_data(buffer, strlen((char *)buffer));
    
            return ESP_AT_RESULT_CODE_OK;
        }
    
- 查询命令：

    .. code-block:: c
    
        static uint8_t at_query_cmd_test(uint8_t *cmd_name)
        {
            uint8_t buffer[64] = {0};
            snprintf((char *)buffer, 64, "query command: <AT%s?> is executed\r\n", cmd_name);
            esp_at_port_write_data(buffer, strlen((char *)buffer));
    
            return ESP_AT_RESULT_CODE_OK;
        }

.. _user-defined-set-command:

- 设置命令：

    .. code-block:: c
    
        static uint8_t at_setup_cmd_test(uint8_t para_num)
        {
            uint8_t index = 0;
    
            // 获取第一个参数，并将其解析为一个数字
            int32_t digit = 0;
            if (esp_at_get_para_as_digit(index++, &digit) != ESP_AT_PARA_PARSE_RESULT_OK) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
    
            // 获取第二个参数，并将其解析为一个字符串
            uint8_t *str = NULL;
            if (esp_at_get_para_as_str(index++, &str) != ESP_AT_PARA_PARSE_RESULT_OK) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
    
            // 分配一个缓冲区，构建数据，然后通过接口 (uart/spi/sdio/socket) 将数据发送到 MCU
            uint8_t *buffer = (uint8_t *)malloc(512);
            if (!buffer) {
                return ESP_AT_RESULT_CODE_ERROR;
            }
            int len = snprintf((char *)buffer, 512, "setup command: <AT%s=%d,\"%s\"> is executed\r\n",
                               esp_at_get_current_cmd_name(), digit, str);
            esp_at_port_write_data(buffer, len);
    
            // 记得释放缓冲区
            free(buffer);
    
            return ESP_AT_RESULT_CODE_OK;
        }

- 执行命令：

    .. code-block:: c
    
        static uint8_t at_exe_cmd_test(uint8_t *cmd_name)
        {
            uint8_t buffer[64] = {0};
            snprintf((char *)buffer, 64, "execute command: <AT%s> is executed\r\n", cmd_name);
            esp_at_port_write_data(buffer, strlen((char *)buffer));
    
            return ESP_AT_RESULT_CODE_OK;
        }

.. _register-at-commands:

第二步：定义注册 AT 命令函数
************************************

- 请定义 :cpp:type:`esp_at_custom_cmd_register` 函数，并在函数中调用 API :cpp:func:`esp_at_custom_cmd_array_regist` 注册 AT 命令。

  示例代码：

  .. code-block:: c
  
      bool esp_at_custom_cmd_register(void)
      {
          return esp_at_custom_cmd_array_regist(at_custom_cmd, sizeof(at_custom_cmd) / sizeof(esp_at_cmd_struct));
      }

- 然后，调用 API `ESP_AT_CMD_SET_INIT_FN <https://github.com/espressif/esp-at/blob/113702d9bf0224ed15e873bdc09898e804f4bd28/components/at/include/esp_at_cmd_register.h#L67>`_ 来初始化您实现的注册 AT 命令函数 :cpp:type:`esp_at_custom_cmd_register`。

  示例代码：
  
  .. code-block:: c
  
      ESP_AT_CMD_SET_INIT_FN(esp_at_custom_cmd_register, 1);

.. note::
  如果您是在 ``examples/at_custom_cmd/custom`` 和 ``examples/at_custom_cmd/include`` 目录下创建新的源文件和头文件自定义 AT 命令，请避免将注册函数命名为 :cpp:type:`esp_at_custom_cmd_register`，因为该函数在 `at_custom_cmd <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd>`_ 示例中已被定义和初始化。您可以将函数命名为 :cpp:type:`esp_at_custom_cmd_register_foo`，然后使用 :cpp:enumerator:`ESP_AT_CMD_SET_INIT_FN` 初始化该函数。

.. _add-component_dependencies:

第三步：增加组件依赖
***********************************

如果您在 :ref:`define-at-commands` 时使用了除 `at <https://github.com/espressif/esp-at/tree/master/components/at>`_、 `freertos <https://github.com/espressif/esp-idf/tree/release/v{IDF_TARGET_VER}/components/freertos>`_、 `nvs_flash <https://github.com/espressif/esp-idf/tree/release/v{IDF_TARGET_VER}/components/nvs_flash>`_ 外其他组件，请在 ``examples/at_custom_cmd/CMakeLists.txt`` 文件中添加这些组件依赖。反之，可以跳过此步骤。例如新增使用 `lwip <https://github.com/espressif/esp-idf/tree/release/v{IDF_TARGET_VER}/components/lwip>`_ 组件，则示例代码如下：

.. code-block:: none

    set(require_components at freertos nvs_flash lwip)

.. _add-link_options:

第四步：增加链接选项
**************************

请在 ``examples/at_custom_cmd/CMakeLists.txt`` 文件中，将自定义的 :ref:`注册 AT 命令函数 <register-at-commands>` 名称作为一个链接选项强制链接给 ${COMPONENT_LIB}，确保程序运行时可以找到该函数。示例代码如下：

.. code-block:: none

    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_custom_cmd_register")

.. note::
  如果自定义的 :ref:`注册 AT 命令函数 <register-at-commands>` 名称为 :cpp:type:`esp_at_custom_cmd_register_foo`，则示例代码如下：

  .. code-block:: none

      target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_custom_cmd_register_foo")

.. _set-component-env-and-compile:

第五步：设置组件环境变量
*****************************

本节介绍了两种设置 ``at_custom_cmd`` 组件环境变量的方法，以确保 ESP-AT 项目在编译时能够正确找到该组件。根据您的需求选择适合的方法。如果您在 ``esp-at/components`` 目录下的原始组件中自定义 AT 命令或修改代码，则无需执行此步骤。但不建议在 ``esp-at/components`` 目录下的原始组件中自定义 AT 命令，本文也不对此进行说明。

**方法 1：** 在命令行中设置 ``AT_CUSTOM_COMPONENTS`` 环境变量（适用于 :doc:`本地编译 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`）。

    - Linux or macOS

    .. code-block:: none

        export AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)
    
    - Windows

    .. code-block:: none

        set AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)

    .. note::
      - 请将 ``(path_of_at_custom_cmd)`` 替换为 ``at_custom_cmd`` 目录的真实绝对路径。
      - 您可以指定多个组件。例如：

        ``export AT_CUSTOM_COMPONENTS="~/prefix/my_path1 ~/prefix/my_path2"``

**方法 2：** 在 `esp-at/build.py <https://github.com/espressif/esp-at/tree/master/build.py>`_ 文件 :cpp:type:`setup_env_variables()` 函数中加入设置 ``AT_CUSTOM_COMPONENTS`` 环境变量的代码（适用于 :doc:`本地编译 <../Compile_and_Develop/How_to_clone_project_and_compile_it>` 和 :doc:`网页编译 <../Compile_and_Develop/How_to_build_project_with_web_page>`）。示例代码如下：

    .. code-block:: none

        # set AT_CUSTOM_COMPONENTS
        at_custom_cmd_path=os.path.join(os.getcwd(), 'examples/at_custom_cmd')
        os.environ['AT_CUSTOM_COMPONENTS']=at_custom_cmd_path

.. _user-compile_at:

第六步：编译 AT 固件
*****************************

完成以上步骤后，可根据需要选择通过 :doc:`网页编译 <../Compile_and_Develop/How_to_build_project_with_web_page>` 或 :doc:`本地编译 <../Compile_and_Develop/How_to_clone_project_and_compile_it>` AT 固件，并将其 :doc:`烧录 <../Get_Started/Downloading_guide>` 到您的设备上。

.. _user-at-cmd-give-it-a-try:

运行 ``AT+TEST`` 命令获取运行结果
------------------------------------

正确操作上面步骤后，运行 ``AT+TEST`` 命令获取结果如下。

**测试命令：**

.. code-block:: none

    AT+TEST=?

**响应：**

.. code-block:: none

    AT+TEST=?
    test command: <AT+TEST=?> is executed

    OK

**查询命令：**

.. code-block:: none

    AT+TEST?

**响应：**

.. code-block:: none

    AT+TEST?
    query command: <AT+TEST?> is executed

    OK

**设置命令：**

.. code-block:: none

    AT+TEST=1,"espressif"

**响应：**

.. code-block:: none

    AT+TEST=1,"espressif"
    setup command: <AT+TEST=1,"espressif"> is executed

    OK

**执行命令：**

.. code-block:: none

    AT+TEST

**响应：**

.. code-block:: none

    AT+TEST
    execute command: <AT+TEST> is executed

    OK

自定义复杂的 AT 命令
--------------------------

下面列举的示例代码适用于定义更加复杂的命令，请根据实际需要进行自定义。

.. _define-return-values:

定义返回消息
****************

ESP-AT 已经在 :cpp:type:`esp_at_result_code_string_index` 定义了一些返回消息，更多返回消息请参见 :ref:`at-messages`。

除了通过 return 模式返回消息，也可调用 API :cpp:func:`esp_at_response_result` 来返回命令执行结果。可在代码中同时使用 API 和 :cpp:enumerator:`ESP_AT_RESULT_CODE_SEND_OK` 及 :cpp:enumerator:`ESP_AT_RESULT_CODE_SEND_FAIL`。

例如，当使用 ``AT+TEST`` 的执行命令向服务器或 MCU 发送数据时，用 :cpp:func:`esp_at_response_result` 来返回发送结果，用 return 模式来返回命令执行结果，示例代码如下。

.. code-block:: c

    uint8_t at_exe_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};

        snprintf((char *)buffer, 64, "this cmd is execute cmd: %s\r\n", cmd_name);

        esp_at_port_write_data(buffer, strlen((char *)buffer));

        // 向服务器或 MCU 发送数据的自定义操作
        send_data_to_server();

        // 返回 SEND OK
        esp_at_response_result(ESP_AT_RESULT_CODE_SEND_OK);

        return ESP_AT_RESULT_CODE_OK;
    }

运行命令及返回的响应：

.. code-block:: none

    AT+TEST
    this cmd is execute cmd: +TEST

    SEND OK

    OK

.. _access-command-parameters:

获取命令参数
*********************

ESP-AT 提供以下两个 API 获取命令参数。

- :cpp:func:`esp_at_get_para_as_digit` 可获取数字参数。
- :cpp:func:`esp_at_get_para_as_str` 可获取字符串参数。

示例请见 :ref:`设置命令 <user-defined-set-command>`。

.. _omit-command-parameters:

省略命令参数
***********************

本节介绍如何设置某些命令参数为可选参数。

- :ref:`omit-the-first-or-middle-parameter`
- :ref:`omit-the-last-parameter`

.. _omit-the-first-or-middle-parameter:

省略首位或中间参数
^^^^^^^^^^^^^^^^^^^

假设您想将 ``AT+TEST`` 命令的 ``<param_2>`` 和 ``<param_3>`` 参数设置为可选参数，其中 ``<param_2>`` 为数字参数，``<param_3>`` 为字符串参数。

.. code-block:: none

    AT+TEST=<param_1>[,<param_2>][,<param_3>],<param_4>

实现代码如下。

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
                // 示例代码
                // 需要自定义操作
                memset(buffer, 0, 64);
                snprintf((char *)buffer, 64, "second parameter is: %d\r\n", para_int_2);
                esp_at_port_write_data(buffer, strlen((char *)buffer));
            }
        } else {
            // 示例代码
            // 省略第二个参数
            // 需要自定义操作
            memset(buffer, 0, 64);
            snprintf((char *)buffer, 64, "second parameter is omitted\r\n");
            esp_at_port_write_data(buffer, strlen((char *)buffer));
        }

        parse_result = esp_at_get_para_as_str(num_index++, &para_str_3);
        if (parse_result != ESP_AT_PARA_PARSE_RESULT_OMITTED) {
            if (parse_result != ESP_AT_PARA_PARSE_RESULT_OK) {
                return ESP_AT_RESULT_CODE_ERROR;
            } else {
                // 示例代码
                // 需自定义操作
                memset(buffer, 0, 64);
                snprintf((char *)buffer, 64, "third parameter is: %s\r\n", para_str_3);
                esp_at_port_write_data(buffer, strlen((char *)buffer));
            }
        } else {
            // 示例代码
            // 省略第三个参数
            // 需自定义操作
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

  如果输入的字符串参数为 ``""``，则该参数没有被省略。

.. _omit-the-last-parameter:

省略最后一位参数
^^^^^^^^^^^^^^^^^

假设 ``AT+TEST`` 命令的 ``<param_3>`` 参数为字符串参数，且为最后一位参数，您想将它设置为可选参数。

.. code-block:: none

    AT+TEST=<param_1>,<param_2>[,<param_3>]

则有以下两种省略情况。

- AT+TEST=<param_1>,<param_2>
- AT+TEST=<param_1>,<param_2>,

实现代码如下。

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
                    // 示例代码
                    // 需自定义操作
                    memset(buffer, 0, 64);
                    snprintf((char *)buffer, 64, "third parameter is: %s\r\n", para_str_3);
                    esp_at_port_write_data(buffer, strlen((char *)buffer));
                }
            } else {
                // 示例代码
                // 省略第三个参数
                // 需自定义操作
                memset(buffer, 0, 64);
                snprintf((char *)buffer, 64, "third parameter is omitted\r\n");
                esp_at_port_write_data(buffer, strlen((char *)buffer));
            }
        }

        return ESP_AT_RESULT_CODE_OK;
    }

.. note::

  如果输入的字符串参数为 ``""``，则该参数没有被省略。

.. _block-command-execution:

阻塞命令的执行
******************

有时您想阻塞某个命令的执行，等待另一个执行结果，然后系统基于这个结果可能会返回不同的值。

一般来说，这类命令需要与其它任务的结果进行同步。

推荐使用 ``semaphore`` 来同步。

示例代码如下。

.. code-block:: c

    xSemaphoreHandle at_operation_sema = NULL;

    uint8_t at_exe_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};

        snprintf((char *)buffer, 64, "this cmd is execute cmd: %s\r\n", cmd_name);

        esp_at_port_write_data(buffer, strlen((char *)buffer));

        // 示例代码
        // 不必在此处创建 semaphores
        at_operation_sema = xSemaphoreCreateBinary();
        assert(at_operation_sema != NULL);

        // 阻塞命令的执行
        // 等待另一个执行的结果
        // 其它任务可调用 xSemaphoreGive 来释放 semaphore
        xSemaphoreTake(at_operation_sema, portMAX_DELAY);

        return ESP_AT_RESULT_CODE_OK;
    }

.. _access-input-data-from-at-command-port:

从 AT 命令端口获取输入的数据
************************************************************

ESP-AT 支持从 AT 命令端口访问输入的数据，为此提供以下两个 API。

- :cpp:func:`esp_at_port_enter_specific` 设置回调函数，AT 端口接收到输入的数据后，将调用该函数。
- :cpp:func:`esp_at_port_exit_specific` 删除由 ``esp_at_port_enter_specific`` 设置的回调函数。

获取数据的方法会根据数据长度是否被指定而有所不同。

指定长度的输入数据
^^^^^^^^^^^^^^^^^^^

假设您已经使用 ``<param_1>`` 指定了数据长度，如下所示。

.. code-block:: none

    AT+TEST=<param_1>

以下示例代码介绍如何从 AT 命令端口获取长度为 ``<param_1>`` 的输入数据。

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

        // 示例代码
        // 不必在此处创建 semaphores
        if (!at_sync_sema) {
            at_sync_sema = xSemaphoreCreateBinary();
            assert(at_sync_sema != NULL);
        }

        // 返回输入数据提示符 ">"
        esp_at_port_write_data((uint8_t *)">", strlen(">"));

        // 设置回调函数，在接收到输入数据后由 AT 端口调用
        esp_at_port_enter_specific(wait_data_callback);

        // 接收输入的数据
        while(xSemaphoreTake(at_sync_sema, portMAX_DELAY)) {
            received_len += esp_at_port_read_data(buf + received_len, specified_len - received_len);

            if (specified_len == received_len) {
                esp_at_port_exit_specific();

                // 获取剩余输入数据的长度
                remain_len = esp_at_port_get_data_length();
                if (remain_len > 0) {
                    // 示例代码
                    // 如果剩余数据长度 > 0，则实际输入数据长度大于指定的接收数据长度
                    // 可自定义如何处理这些剩余数据
                    // 此处只是简单打印出剩余数据
                    esp_at_port_recv_data_notify(remain_len, portMAX_DELAY);
                }

                // 示例代码
                // 输出接收到的数据
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

因此，如果您设置 ``AT+TEST=5``，输入的数据为 ``1234567890``，那么 ``ESP-AT`` 返回的结果如下所示。

.. code-block:: none

    AT+TEST=5
    >67890
    received data is: 12345
    OK

未指定长度的输入数据
^^^^^^^^^^^^^^^^^^^^^

这种情况类似 Wi-Fi :term:`透传模式`，不指定数据长度。

::

    AT+TEST

假设 ``ESP-AT`` 结束命令的执行并返回执行结果，示例代码如下。

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

        // 示例代码
        // 不必在此处创建 semaphores
        if (!at_sync_sema) {
            at_sync_sema = xSemaphoreCreateBinary();
            assert(at_sync_sema != NULL);
        }

        // 返回输入数据提示符 ">"
        esp_at_port_write_data((uint8_t *)">", strlen(">"));

        // 设置回调函数，在接收到输入数据后由 AT 端口调用
        esp_at_port_enter_specific(wait_data_callback);

        // 接收输入的数据
        while(xSemaphoreTake(at_sync_sema, portMAX_DELAY)) {
            memset(buf, 0, BUFFER_LEN);

            received_len = esp_at_port_read_data(buf, BUFFER_LEN);
            // 检查是否退出该模式
            // 退出条件是接收到 "+++" 字符串
            if ((received_len == 3) && (strncmp((const char *)buf, "+++", 3)) == 0) {
                esp_at_port_exit_specific();

                // 示例代码
                // 如果剩余数据长度 > 0，说明缓冲区内仍有数据需要处理
                // 可自定义如何处理剩余数据
                // 此处只是简单打印出剩余数据
                remain_len = esp_at_port_get_data_length();
                if (remain_len > 0) {
                    esp_at_port_recv_data_notify(remain_len, portMAX_DELAY);
                }

                break;
            } else if (received_len > 0) {
                // 示例代码
                // 可自定义如何处理接收到的数据
                // 此处只是简单打印出接收到的数据
                memset(buffer, 0, 64);
                snprintf((char *)buffer, 64, "\r\nreceived data is: ");
                esp_at_port_write_data(buffer, strlen((char *)buffer));

                esp_at_port_write_data(buf, strlen((char *)buf));
            }
        }

        free(buf);

        return ESP_AT_RESULT_CODE_OK;
    }

因此，如果第一个输入数据是 ``1234567890``，第二个输入数据是 ``+++``，那么 ``ESP-AT`` 返回结果如下所示。

.. code-block:: none

    AT+TEST
    >
    received data is: 1234567890
    OK
