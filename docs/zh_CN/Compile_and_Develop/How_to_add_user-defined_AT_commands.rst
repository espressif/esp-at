添加自定义 AT 命令
=====================

:link_to_translation:`en:[English]`

本文档详细介绍了如何在 `ESP-AT <https://github.com/espressif/esp-at>`_ 工程中添加用户定义的 AT 命令，以 ``AT+TEST`` 命令为例展示每个步骤的示例代码。

自定义一个基本的、功能良好的命令至少需要以下两个步骤。

- :ref:`define-at-commands`
- :ref:`register-at-commands`

这一步介绍新命令的运行及响应情况。

- :ref:`user-define-at-commands-give-it-a-try`

其余的步骤适用于定义相对复杂的命令，可根据您的需要进行选择。

- :ref:`define-return-values`
- :ref:`access-command-parameters`
- :ref:`omit-command-parameters`
- :ref:`block-command-execution`
- :ref:`access-input-data-from-at-command-port`

AT 命令集的源代码不开源，以 :component:`库文件 <at/lib>` 的形式呈现，它也是解析自定义的 AT 命令的基础。

.. _define-at-commands:

定义 AT 命令
------------------

在定义 AT 命令之前，请先决定 AT 命令的名称和类型。

**命令命名规则：**

- 命令应以 ``+`` 符号开头。
- 支持字母 (``A~Z, a~z``)、数字 (``0~9``) 及其它一些字符 (``!``、 ``%``、``-``、``.``、``/``、``:``、``_``)，详情请见 :ref:`at-command-types`。

**命令类型：**

每条 AT 命令最多可以有四种类型：测试命令、查询命令、设置命令和执行命令，更多信息参见 :ref:`at-command-types`。

然后，定义所需类型的命令。假设 ``AT+TEST`` 支持所有的四种类型，下面是定义每种类型的示例代码。

测试命令：

.. code-block:: c

    uint8_t at_test_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};

        snprintf((char *)buffer, 64, "this cmd is test cmd: %s\r\n", cmd_name);

        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

查询命令：

.. code-block:: c

    uint8_t at_query_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};

        snprintf((char *)buffer, 64, "this cmd is query cmd: %s\r\n", cmd_name);

        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

.. _user-defined-set-command:

设置命令：

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

执行命令：

.. code-block:: c

    uint8_t at_exe_cmd_test(uint8_t *cmd_name)
    {
        uint8_t buffer[64] = {0};

        snprintf((char *)buffer, 64, "this cmd is execute cmd: %s\r\n", cmd_name);

        esp_at_port_write_data(buffer, strlen((char *)buffer));

        return ESP_AT_RESULT_CODE_OK;
    }

最后，调用 :cpp:type:`esp_at_cmd_struct` 来定义 AT 命令的名称和支持的类型，下面的示例代码定义了名称为 ``+TEST``（省略了 ``AT``） 并支持所有四种类型的命令。

.. code-block:: c

    static esp_at_cmd_struct at_custom_cmd[] = {
        {"+TEST", at_test_cmd_test, at_query_cmd_test, at_setup_cmd_test, at_exe_cmd_test},
    };

如果不定义某个类型，将其设置为 ``NULL``。

.. _register-at-commands:

注册 AT 命令
--------------------

调用 API :cpp:func:`esp_at_custom_cmd_array_regist` 来注册 AT 命令，以下是注册 ``AT+TEST`` 的示例代码。

.. code-block:: c
  
  esp_at_custom_cmd_array_regist(at_custom_cmd, sizeof(at_custom_cmd) / sizeof(at_custom_cmd[0]));

.. note::

  建议把 ``esp_at_custom_cmd_array_regist`` 加入 ``app_main()`` 中的 ``at_custom_init()``。

.. _user-define-at-commands-give-it-a-try:

尝试一下吧
-------------

如果你已经完成了上述两个步骤，请编译 ESP-AT 工程并烧录固件，该命令即可在您的设备上正常运行。尝试运行一下吧！

下面是 ``AT+TEST`` 的运行情况。

**测试命令：**

.. code-block:: none

    AT+TEST=?

**响应：**

.. code-block:: none

    AT+TEST=?
    this cmd is test cmd: +TEST

    OK

**查询命令：**

.. code-block:: none

    AT+TEST?

**响应：**

.. code-block:: none

    AT+TEST?
    this cmd is query cmd: +TEST

    OK

**设置命令：**

.. code-block:: none

    AT+TEST=1,"espressif"

**响应：**

.. code-block:: none

    AT+TEST=1,"espressif"
    this cmd is setup cmd and cmd num is: 2
    first parameter is: 1
    second parameter is: espressif

    OK

**执行命令：**

.. code-block:: none

    AT+TEST

**响应：**

.. code-block:: none

    AT+TEST
    this cmd is execute cmd: +TEST

    OK

.. _define-return-values:

定义返回消息
---------------------

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
-------------

ESP-AT 提供以下两个 API 获取命令参数。

- :cpp:func:`esp_at_get_para_as_digit` 可获取数字参数。
- :cpp:func:`esp_at_get_para_as_str` 可获取字符串参数。

示例请见 :ref:`执行命令 <user-defined-set-command>`。

.. _omit-command-parameters:

省略命令参数
------------------------

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
----------------

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
-------------------------------

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