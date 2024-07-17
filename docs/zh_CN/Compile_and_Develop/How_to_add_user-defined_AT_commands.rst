.. _user-defined_at_cmd:

************************
增加自定义 AT 命令
************************

:link_to_translation:`en:[English]`

本文档详细介绍了如何添加自定义的 AT 命令，其实在 ESP-AT 工程中已经提供了 `at_custom_cmd 示例 <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd>`_，下文主要是围绕该示例展开说明，将以 ``AT+TEST`` 命令为例展示每个步骤的示例代码。

在您自定义 AT 命令之前，请先了解：

- :ref:`at-custom_cmd`

自定义一个基本的、功能良好的命令至少需要以下四个步骤：

- :ref:`define-at-commands`
- :ref:`register-at-commands`
- :ref:`update-cmakelist`
- :ref:`set-component-env-and-compile`

完成上述步骤后，我们来看下新命令（AT+TEST）的运行及响应情况：

- :ref:`user-at-cmd-give-it-a-try`

下面步骤适用于自定义相对复杂的命令，可根据您的需要进行选择：

- :ref:`define-return-values`
- :ref:`access-command-parameters`
- :ref:`omit-command-parameters`
- :ref:`block-command-execution`
- :ref:`access-input-data-from-at-command-port`

AT 命令集的源代码不开源，以 :component:`库文件 <at/lib>` 的形式呈现，它也是解析自定义的 AT 命令的基础。

.. _at-custom_cmd:

:ref:`at_custom_cmd 组件 <user-defined_at_cmd>`
==================================================

at_custom_cmd 组件简介
-------------------------

您可以先阅读 at_custom_cmd 组件的 `README <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd/README.md>`_ 简单了解下它。

关于组件概念和相关知识，具体参考ESP-IDF 构建系统中 `介绍组件的文档  <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/build-system.html#id1>`_。

.. _at_custom_cmd_component:

at_custom_cmd 组件目录树结构
------------------------------

at_custom_cmd 组件的目录树结构如下所示：

.. code-block:: none

    - at_custom_cmd/
                    - custom/ - at_custom_cmd.c
                    - include/ - at_custom_cmd.h
                    - CMakeLists.txt
                    - README.md

该示例项目 "at_custom_cmd" 包含以下组成部分：

  - at_custom_cmd/custom/ 目录下的 .c 文件用来存放 :ref:`自定义 AT 命令的源代码 <define-at-commands>`。除了 at_custom_cmd.c 文件，可在此目录下再另外创建 .c 文件。
  - at_custom_cmd/include/ 目录下的 .h 文件用来存放自定义 AT 命令的头文件。除了 at_custom_cmd.h 文件，可在此目录下再另外创建 .h 文件。
  - CMakeLists.txt 文件，里面会定义一些变量以控制该组件的构建过程，以及其与 ESP-AT 项目的集成。更多详细信息请参阅 `组件 CMakeLists 文件 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/build-system.html#component-directories>`_。
  - README.md 文件，用来对该组件进行说明。

at_custom_cmd 组件 CMakeLists.txt 文件
-----------------------------------------

- 将 `.c` 文件路径添加到变量 `srcs` 中：

  .. code-block:: none

      file(GLOB_RECURSE srcs *.c)

- 设置头文件（at_custom_cmd.h 文件）包含目录：

  .. code-block:: none
    
      set(includes "include")

.. _add-component_dependencies:

- 增加组件依赖，并将组件添加到构建系统中：
 
  - 在 CMakeLists.txt 文件中添加 :ref:`自定义 AT 命令代码 <define-at-commands>` 中需要的组件依赖。并使用 idf_component_register 将组件添加到构建系统中，具体可参考 `idf_component_register <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-guides/build-system.html#cmake-component-register>`_。

  .. code-block:: none

      set(require_components at freertos nvs_flash)

      idf_component_register(
        SRCS ${srcs}
        INCLUDE_DIRS ${includes}
        REQUIRES ${require_components})

  .. note::
    如果您 :ref:`自定义 AT 命令代码 <define-at-commands>` 中使用了其他组件，您需要在 `esp-at/examples/at_custom_cmd/CMakeLists.txt` 文件中添加这些组件依赖。例如您还使用到了 LwIP 组件，则您设置应如下：

    .. code-block:: none

        set(require_components at freertos nvs_flash lwip)

.. _add-link_options:

- 将您 :ref:`自定义的注册 AT 命令函数 <register-at-commands>` 的名称作为一个链接选项强制链接给 ${COMPONENT_LIB}，确保程序运行时可以正确找到该自定义的注册 AT 命令函数。以下链接示例是以 at_custom_cmd 示例中的 `esp_at_custom_cmd_register` 注册 AT 命令函数为例：

  .. code-block:: none

      target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_custom_cmd_register")

  .. note::
    如果您 :ref:`自定义的注册 AT 命令函数 <register-at-commands>` 的名称为 ``esp_at_custom_cmd_register_1`` 则需增加如下代码：

    .. code-block:: none

        target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_custom_cmd_register_1")

.. _component-usage:

at_custom_cmd 组件用法
------------------------

1. 将 :ref:`自定义 AT 命令代码 <define-at-commands>` 添加到 `at_custom_cmd/custom/**/*.c` 和 `at_custom_cmd/custom/**/*.h` 文件中（您也可以创建新的源文件和头文件，如果使用此方法的话，您需要注意在 .c 文件中对 :ref:`注册 AT 命令函数 <register-at-commands>` 的处理）。
2. 如果您的代码中使用了其他组件，您可能需要在 `at_custom_cmd/CMakeLists.txt` 文件中添加更多组件依赖。

.. _compinents-3:

3. 设置 `at_custom_cmd` 组件环境变量，以便让 `ESP-AT` 项目在编译时能找到该组件，以下两种方法，选择其一即可。

.. _set-path_1:

- **方法一：** 直接在命令行中设置 `AT_CUSTOM_COMPONENTS` 环境变量

    .. note::
      - 您需要将 (path_of_at_custom_cmd) 替换为 `at_custom_cmd` 目录的真实绝对路径。
      - 您可以指定多个组件。例如：`export AT_CUSTOM_COMPONENTS="~/prefix/my_path1 ~/prefix/my_path2"`。

    - Linux or macOS

    .. code-block:: none

        export AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)
    
    - Windows

    .. code-block:: none

        set AT_CUSTOM_COMPONENTS=(path_of_at_custom_cmd)

.. _set-path_2:

- **方法二：** 直接在 `esp-at/build.py` 文件 ``setup_env_variables()`` 函数中加入设置 AT_CUSTOM_COMPONENTS 环境变量的代码，代码示例如下：

    .. code-block:: none

        # set AT_CUSTOM_COMPONENTS
        at_custom_cmd_path=os.path.join(os.getcwd(), 'examples/at_custom_cmd')
        os.environ['AT_CUSTOM_COMPONENTS']=at_custom_cmd_path

.. _compile-at:

4. 选择以下两种方法重新编译 AT 固件

  - **方法一：** 根据 :doc:`本地编译 ESP-AT 项目指南 <../Compile_and_Develop/How_to_clone_project_and_compile_it>` 来重新编译 AT 固件。
  - **方法二：** 根据 :doc:`网页编译 ESP-AT 项目指南 <../Compile_and_Develop/How_to_build_project_with_web_page>` 来重新编译 AT 固件（如果选择了此方法进行编译 AT 固件，则上述的第三步 :ref:`component-usage` 必须选用 :ref:`第二种方法 <set-path_2>`）。

.. _step-define_at_command:

:ref:`自定义 AT 命令步骤 <user-defined_at_cmd>`
==================================================

.. _define-at-commands:

:ref:`自定义 AT 命令代码 <user-defined_at_cmd>`
--------------------------------------------------

在自定义 AT 命令之前，请先决定 AT 命令的名称和类型。

**命令命名规则：**

- 命令应以 ``+`` 符号开头。
- 支持字母 (``A~Z, a~z``)、数字 (``0~9``) 及其它一些字符 (``!``、 ``%``、``-``、``.``、``/``、``:``、``_``)，详情请见 :ref:`at-command-types`。

**命令类型：**

每条 AT 命令最多可以有四种类型：测试命令、查询命令、设置命令和执行命令，更多信息参见 :ref:`at-command-types`。

然后，定义所需类型的命令。假设 ``AT+TEST`` 支持所有的四种类型，下面是定义每种类型的示例代码。

测试命令：

.. code-block:: c

    static uint8_t at_test_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};
        snprintf((char *)buffer, 64, "test command: <AT%s=?> is executed\r\n", cmd_name);
        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

查询命令：

.. code-block:: c

    static uint8_t at_query_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};
        snprintf((char *)buffer, 64, "query command: <AT%s?> is executed\r\n", cmd_name);
        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

.. _user-defined-set-command:

设置命令：

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

执行命令：

.. code-block:: c

    static uint8_t at_exe_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};
        snprintf((char *)buffer, 64, "execute command: <AT%s> is executed\r\n", cmd_name);
        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

最后，调用 :cpp:type:`esp_at_cmd_struct` 来定义 AT 命令的名称和支持的类型，下面的示例代码定义了名称为 ``+TEST`` （省略了 ``AT``） 并支持所有四种类型的命令。

.. note::
  如果不定义某个类型，将其设置为 ``NULL``。

.. code-block:: c

    static const esp_at_cmd_struct at_custom_cmd[] = {
        {"+TEST", at_test_cmd_test, at_query_cmd_test, at_setup_cmd_test, at_exe_cmd_test},
        /**
         * @brief You can define your own AT commands here.
         */
    };

.. _register-at-commands:

:ref:`定义注册 AT 命令函数并初始化 <user-defined_at_cmd>`
----------------------------------------------------------

at_custom_cmd 示例中 esp_at_custom_cmd_register 函数调用 API :cpp:func:`esp_at_custom_cmd_array_regist` 来注册 AT 命令，以下是注册 ``AT+TEST`` 的示例代码。

.. code-block:: c
  
    bool esp_at_custom_cmd_register(void)
    {
        return esp_at_custom_cmd_array_regist(at_custom_cmd, sizeof(at_custom_cmd) / sizeof(esp_at_cmd_struct));
    }

最后调用 API `ESP_AT_CMD_SET_INIT_FN <https://github.com/espressif/esp-at/blob/113702d9bf0224ed15e873bdc09898e804f4bd28/components/at/include/esp_at_cmd_register.h#L67>`_ 来初始化您实现的注册 AT 命令函数 esp_at_custom_cmd_register，以下是初始化注册 ``AT+TEST`` 命令函数的示例代码。

.. code-block:: c

    ESP_AT_CMD_SET_INIT_FN(esp_at_custom_cmd_register, 1);

.. note::
  如果您选择不在 `at_custom_cmd.c` 和 `at_custom_cmd.h` 文件中 :ref:`define-at-commands`，而是在 `esp-at/examples/at_custom_cmd/custom` 和 `esp-at/examples/at_custom_cmd/include` 目录下创建新的源文件和头文件来 :ref:`自定义 AT 命令 <define-at-commands>` 和 :ref:`自定义注册 AT 命令函数并初始化 <register-at-commands>`，则您在实现注册 AT 命令函数时就要避免将函数名称设置为 `esp_at_custom_cmd_register`，因为在 at_custom_cmd 示例中已经存在了名为 esp_at_custom_cmd_register 的函数，并对它进行了初始化，您可以定义一个名称不是 `esp_at_custom_cmd_register` 的函数去注册 AT 命令，然后您再使用 ESP_AT_CMD_SET_INIT_FN 将您定义的注册 AT 命令函数初始化即可。

.. _update-cmakelist:

:ref:`更新 CMakeLists.txt 文件 <user-defined_at_cmd>`
--------------------------------------------------------

现在您需要根据以上实现的 :ref:`自定义 AT 命令代码 <register-at-commands>` 和 :ref:`自定义注册 AT 命令函数并初始化代码 <register-at-commands>` 来更新 `CMakeLists.txt 文件 <https://github.com/espressif/esp-at/tree/master/examples/at_custom_cmd/CMakeLists.txt>`_。特别注意以下两点：

1. :ref:`增加组件依赖 <add-component_dependencies>`
2. :ref:`增加链接选项 <add-link_options>`

.. _set-component-env-and-compile:

:ref:`设置组件环境变量以及编译 AT 工程 <user-defined_at_cmd>`
---------------------------------------------------------------

- 请在以下两种编译 AT 工程方法中选择其一重新编译 AT 固件。

  - **方法一：** 如果您选择根据 :doc:`本地编译 ESP-AT 项目指南 <../Compile_and_Develop/How_to_clone_project_and_compile_it>` 来重新编译 AT 固件，那么请您在开始编译 AT 工程之前，先根据 `设置环境变量文档 <compinents-3>`_ 中描述的两种方法选择其一设置 at_custom_cmd 组件的环境变量。
  - **方法二：** 如果您选择根据 :doc:`网页编译 ESP-AT 项目指南 <../Compile_and_Develop/How_to_build_project_with_web_page>` 来重新编译 AT 固件，那么请您在开始编译 AT 工程之前，先根据 `设置环境变量文档 <compinents-3>`_ 中 :ref:`第二种方法 <set-path_2>` 设置 at_custom_cmd 组件的环境变量。

- 请 `烧录您编译的 AT 固件 <../Get_Started/Downloading_guide>`_。

.. _user-at-cmd-give-it-a-try:

:ref:`尝试运行 AT+TEST 命令吧 <user-defined_at_cmd>`
=====================================================

如果你已经完成了 :ref:`define-at-commands`， :ref:`register-at-commands`， :ref:`update-cmakelist` 以及 :ref:`set-component-env-and-compile` 四个步骤， `AT+TEST` 命令即可在您的设备上正常运行。尝试运行一下吧！

如果你上面的步骤都操作正确了，那么下面就是您定义的 ``AT+TEST`` 命令的运行情况。

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

自定义复杂的 AT 命令代码
=========================

**如果您完成了上述步骤，则您已经可以完成一个简单的 AT+TEST 命令了，下面会介绍一些适用于定义相对复杂的命令的代码示例，可根据您的需要进行选择。**

.. _define-return-values:

:ref:`定义返回消息 <user-defined_at_cmd>`
------------------------------------------

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

:ref:`获取命令参数 <user-defined_at_cmd>`
-------------------------------------------------

ESP-AT 提供以下两个 API 获取命令参数。

- :cpp:func:`esp_at_get_para_as_digit` 可获取数字参数。
- :cpp:func:`esp_at_get_para_as_str` 可获取字符串参数。

示例请见 :ref:`执行命令 <user-defined-set-command>`。

.. _omit-command-parameters:

:ref:`省略命令参数 <user-defined_at_cmd>`
------------------------------------------------

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

:ref:`阻塞命令的执行 <user-defined_at_cmd>`
---------------------------------------------

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

:ref:`从 AT 命令端口获取输入的数据 <user-defined_at_cmd>`
-----------------------------------------------------------

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
