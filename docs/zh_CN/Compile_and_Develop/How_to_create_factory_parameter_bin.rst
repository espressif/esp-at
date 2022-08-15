如何生成出厂参数二进制文件
======================================

{IDF_TARGET_COMPILE_MNAME: default="undefined", esp32="WROOM-32", esp32c2="ESP32C2-4MB", esp32c3="MINI-1"}
{IDF_TARGET_AT_PARAMS_ADDR: default="undefined", esp32="0x30000", esp32c2="0x2B000", esp32c3="0x31000"}
{IDF_TARGET_INDEX: default="undefined", esp32="1", esp32c2="2", esp32c3="3"}

:link_to_translation:`en:[English]`

本文介绍了如何为模组生成一个自定义的 ESP-AT 出厂参数二进制文件 (factory_MODULE_NAME.bin)。例如，您可能想在 ESP-AT 固件中自定义国家代码、射频限制或 UART 管脚，则可以通过下面的两个表格定义此类参数。

- :ref:`factory-param-type-csv`
- :ref:`factory-param-data-csv`

下面介绍如何通过修改这两个表格来生成自定义的出厂参数二进制文件。

- :ref:`add-a-customized-module`
- :ref:`add-a-customized-parameter`
- :ref:`modify-factory-parameter-data-on-an-existing-module`

.. _factory-param-type-csv:

factory_param_type.csv
-----------------------

factory_param_type.csv 表列出了您可以定义的所有参数，以及每个参数的偏移量、类型和大小，它储存在 :component_file:`customized_partitions/raw_data/factory_param/factory_param_type.csv`。

下表提供了每个参数的信息。

.. list-table::
   :header-rows: 1
   :widths: 20 100

   * - param_name
     - 说明
   * - description
     - 在编译 ESP-AT 工程时提示的模组附加信息。
   * - magic_flag
     - 该值必须是 ``0xfcfc``。
   * - version
     - 供内部使用的出厂参数管理版本，当前版本为 ``3``，不建议修改。
   * - reserved1
     - 保留。
   * - tx_max_power
     - {IDF_TARGET_NAME} 的 Wi-Fi 最大发射功率：[40,84]，详情请见 `{IDF_TARGET_NAME} 发射功率 <https://docs.espressif.com/projects/esp-idf/en/release-v4.3/{IDF_TARGET_PATH_NAME}/api-reference/network/esp_wifi.html#_CPPv425esp_wifi_set_max_tx_power6int8_t>`_ 设置范围。
   * - uart_port
     - 用于发送 AT 命令和接收 AT 响应的 UART 端口。
   * - start_channel
     - 起始 Wi-Fi 信道。
   * - channel_num
     - Wi-Fi 的总信道数。
   * - country_code
     - 国家代码。
   * - uart_baudrate
     - UART 的波特率。
   * - uart_tx_pin
     - UART tx 管脚。
   * - uart_rx_pin
     - UART rx 管脚。
   * - uart_cts_pin
     - UART cts 管脚，不使用时请置为 -1。
   * - uart_rts_pin
     - UART rts 管脚，不使用时请置为 -1。
   * - tx_control_pin
     - 某些开发板上电时，该 tx 管脚需要与 MCU 分开。不使用时请置为 -1。
   * - rx_control_pin
     - 某些开发板上电时，该 rx 管脚需要与 MCU 分开。不使用时请置为 -1。
   * - reserved2
     - 保留。
   * - platform
     - 运行当前固件的平台（也称为芯片系列）。
   * - module_name
     - 运行当前固件的模组。

.. _factory-param-data-csv:

factory_param_data.csv
-----------------------

factory_param_data.csv 表格保存了 :ref:`factory-param-type-csv` 中定义的所有参数的值，支持 {IDF_TARGET_NAME} 系列的模组。您可通过修改表中的值来自定义出厂参数二进制文件。该表储存在 :component_file:`customized_partitions/raw_data/factory_param/factory_param_data.csv` 中。

.. _add-a-customized-module:

新增一个自定义模组
-----------------------

本节通过一个示例介绍如何在 factory_param_data.csv 中添加一个自定义模组，并为其生成出厂参数二进制文件。假设您想为一个名为 ``MY_MODULE`` 的 {IDF_TARGET_NAME} 模组生成出厂参数二进制文件，其国家代码为 JP，Wi-Fi 信道为 1 至 14，其它参数与 ``PLATFORM_{IDF_TARGET_CFG_PREFIX}`` 的 ``{IDF_TARGET_COMPILE_MNAME}`` 模组相同，可按照以下步骤进行操作。

.. contents::
  :local:
  :depth: 1

修改 factory_param_data.csv
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

在 factory_param_data.csv 表中设置 ``MY_MODULE`` 的所有参数值。

首先，在表格底部插入一行，然后输入以下参数值。

- param_name: value
- platform: PLATFORM_{IDF_TARGET_CFG_PREFIX}
- module_name: ``MY_MODULE``
- description: ``MY_DESCRIPTION``
- magic_flag: 0xfcfc
- version: 3
- reserved1: 0
- tx_max_power: 78
- uart_port: 1
- start_channel: ``1``
- channel_num: ``14``
- country_code: ``JP``
- uart_baudrate: 115200
- uart_tx_pin: 17
- uart_rx_pin: 16
- uart_cts_pin: 15
- uart_rts_pin: 14
- tx_control_pin: -1
- rx_control_pin: -1

修改后的 factory_param_data.csv 表格如下所示。

.. code-block:: none

  platform,module_name,description,magic_flag,version,reserved1,tx_max_power,uart_port,start_channel,channel_num,country_code,uart_baudrate,uart_tx_pin,uart_rx_pin,uart_cts_pin,uart_rts_pin,tx_control_pin,rx_control_pin
  PLATFORM_ESP32,WROOM-32,,0xfcfc,3,0,78,1,1,13,CN,115200,17,16,15,14,-1,-1
  ...
  PLATFORM_{IDF_TARGET_CFG_PREFIX},MY_MODULE,MY_DESCRIPTION,0xfcfc,3,0,78,1,1,14,JP,115200,17,16,15,14,-1,-1

.. _modify-esp-at-module-info-structure:

修改 ``esp_at_module_info`` 结构体
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

在 :component_file:`at/src/at_default_config.c` 中的 ``esp_at_module_info`` 结构体中添加自定义模组的信息。 

``esp_at_module_info`` 结构体提供 ``OTA`` 升级验证 ``token``：

.. code-block:: c

    typedef struct {
        char* module_name;
        char* ota_token;
        char* ota_ssl_token;
    } esp_at_module_info_t;

若不想使用 ``OTA`` 功能，那么第二个参数 ``ota_token`` 和第三个参数 ``ota_ssl_token`` 应该设置为 ``NULL``，第一个参数 ``module_name`` 必须与 factory_param_data.csv 文件中的 ``module_name`` 一致。

下面是修改后的 ``esp_at_module_info`` 结构体。

.. code-block:: c

    static const esp_at_module_info_t esp_at_module_info[] = {
    #if defined(CONFIG_IDF_TARGET_ESP32)
      ...
    #endif

    #if defined(CONFIG_IDF_TARGET_ESP32C3)
      ...
    #endif

    #if defined(CONFIG_IDF_TARGET_ESP32C2)
      ...
    #endif

    #if defined(CONFIG_IDF_TARGET_{IDF_TARGET_CFG_PREFIX})
      {"MY_MODULE",       CONFIG_ESP_AT_OTA_TOKEN_MY_MODULE,      CONFIG_ESP_AT_OTA_SSL_TOKEN_MY_MODULE },     // MY_MODULE
    #endif
    };

宏 ``CONFIG_ESP_AT_OTA_TOKEN_MY_MODULE`` 和宏 ``CONFIG_ESP_AT_OTA_SSL_TOKEN_MY_MODULE`` 定义在头文件 :component_file:`at/private_include/at_ota_token.h` 中。

.. code-block:: none

    #if defined(CONFIG_IDF_TARGET_{IDF_TARGET_CFG_PREFIX})
    ...
    #define CONFIG_ESP_AT_OTA_TOKEN_MY_MODULE       CONFIG_ESP_AT_OTA_TOKEN_DEFAULT

    ...
    #define CONFIG_ESP_AT_OTA_SSL_TOKEN_MY_MODULE       CONFIG_ESP_AT_OTA_SSL_TOKEN_DEFAULT

重新编译工程并选择自定义模组
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

在添加自定义模组信息后，根据 :doc:`How_to_clone_project_and_compile_it` 重新编译整个工程，在配置工程时选择自定义模组。

::

    Platform name:
    1. PLATFORM_ESP32
    2. PLATFORM_ESP32C3
    3. PLATFORM_ESP32C2
    choose(range[1,3]):{IDF_TARGET_INDEX}

    Module name:
    ...
    x. MY_MODULE (description: MY_DESCRIPTION)
    choose(range[1,x]):x

编译完成后可在 ``esp-at/build/customized_partitions`` 文件夹下找到生成的出厂参数二进制文件。 

.. _add-a-customized-parameter:

新增一个自定义参数
---------------------

本节通过一个示例介绍如何新增一个自定义参数。假设您想为 ``MY_MODULE`` 添加参数 ``date``，并将其设置为 ``20210603``，可按照以下步骤进行操作。

.. contents::
  :local:
  :depth: 1

修改 factory_param_type.csv
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

在 factory_param_type.csv 中定义参数 ``date``。

首先，在表格的底部插入一行，然后设置参数的名称 (param_name)、偏移量 (offset)、类型 (type) 和大小 (size)。

.. list-table::
   :header-rows: 1

   * - param_name
     - offset
     - type
     - size
   * - description
     - -1
     - String
     - 0
   * - ...
     - ...
     - ...
     - ...
   * - ``date``
     - ``88``
     - ``String``
     - ``9``

修改 factory_param_data.csv
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

在 factory_param_data.csv 最后一列的后面插入一列，并命名为 ``date``，然后将 ``MY_MODULE`` 对应的值设置为 ``20210603``。

以下是修改后的 CSV 表格。

::

    platform,module_name,description,magic_flag,version,reserved1,tx_max_power,uart_port,start_channel,channel_num,country_code,uart_baudrate,uart_tx_pin,uart_rx_pin,uart_cts_pin,uart_rts_pin,tx_control_pin,rx_control_pin,date
    PLATFORM_ESP32,WROOM-32,,0xfcfc,3,0,78,1,1,13,CN,115200,17,16,15,14,-1,-1
    ...
    PLATFORM_{IDF_TARGET_CFG_PREFIX},MY_MODULE,MY_DESCRIPTION,0xfcfc,3,0,78,1,1,14,JP,115200,17,16,15,14,-1,-1,20210603

处理自定义参数
^^^^^^^^^^^^^^

您可以自定义函数来处理自定义的参数 ``date``，以下只是简单输出参数值。

.. code-block:: c

    static void esp_at_factory_parameter_date_init(void)
    {
        const esp_partition_t * partition = esp_at_custom_partition_find(0x40, 0xff, "factory_param");
        char* data = NULL;
        char* str_date = NULL;

        if (!partition) {
            printf("factory_parameter partition missed\r\n");
            return;
        }

        data = (char*)malloc(ESP_AT_FACTORY_PARAMETER_SIZE); // 说明
        assert(data != NULL);
        if(esp_partition_read(partition, 0, data, ESP_AT_FACTORY_PARAMETER_SIZE) != ESP_OK){
            free(data);
            return;
        }

        if ((data[0] != 0xFC) || (data[1] != 0xFC)) { // 检查 magic flag 是否为 0xfc 0xfc
            return;
        }

        // 示例代码
        // 可自定义如何处理参数 date
        // 此处仅简单打印 date 参数值
        str_date = &data[88];   // date 字段偏移地址
        printf("date is %s\r\n", str_date);

        free(data);

        return;
    }

重新编译工程
^^^^^^^^^^^^

参考 :doc:`How_to_clone_project_and_compile_it` 来编译整个工程。

编译完成后可在 ``esp-at/build/customized_partitions`` 文件夹下找到生成的出厂参数二进制文件。 

.. _modify-factory-parameter-data-on-an-existing-module:

修改现有模组的出厂参数数据
---------------------------

假设您需要修改 factory_param_data.csv 中现有的某个模组的出厂参数数据，可采用下面任意一种方法。

.. contents::
  :local:
  :depth: 1

重新编译整个工程
^^^^^^^^^^^^^^^^^^^^^

打开 factory_param_data.csv 并根据需要修改参数。

重新编译 ESP-AT 工程（参考 :doc:`How_to_clone_project_and_compile_it`），出厂参数二进制文件会在 ``esp-at/build/customized_partitions`` 文件夹生成。

只编译出厂参数二进制文件
^^^^^^^^^^^^^^^^^^^^^^^^^^^

首先，克隆整个 ESP-AT 工程。

然后，前往 ESP-AT 工程的根目录，输入以下命令，并替换一些参数。

::

    python tools/factory_param_generate.py --platform PLATFORM --module MODULE --define_file DEFINE_FILE --module_file MODULE_FILE --bin_name BIN_NAME --log_file LOG_FILE

- ``PLATFORM`` 替换为模组的平台，必须与 factory_param_data.csv 中 ``platform`` 的值一致。

- ``MODULE`` 替换为模组的名称，必须与 factory_param_data.csv 中 ``module_name`` 的值一致。

- ``DEFINE_FILE`` 替换为 factory_param_type.csv 的相对路径。

- ``MODULE_FILE`` 替换为 factory_param_data.csv 的相对路径。

- ``BIN_NAME`` 替换为出厂参数二进制文件名。

- ``LOG_FILE`` 储存模组名称的文件名。

以下为 ``MY_MODULE`` 的示例代码。

::

    python tools/factory_param_generate.py --platform PLATFORM_{IDF_TARGET_CFG_PREFIX} --module MY_MODULE --define_file components/customized_partitions/raw_data/factory_param/factory_param_type.csv --module_file components/customized_partitions/raw_data/factory_param/factory_param_data.csv --bin_name ./factory_param.bin --log_file ./factory_parameter.log

执行上述命令后，将在当前目录下生成以下三个文件。

- factory_param.bin
- factory_parameter.log
- factory_param_MY_MODULE.bin

将新生成的 ``factory_param_MY_MODULE.bin`` 下载到 flash 中，可使用 ESP-AT 提供的 `esptool.py <https://github.com/espressif/esptool/#readme>`_ 进行下载，在 ESP-AT 项目的根目录下执行以下命令，并替换一些参数。

::

    python esp-idf/components/esptool_py/esptool/esptool.py -p PORT -b BAUD --before default_reset --after hard_reset --chip auto  write_flash --flash_mode dio --flash_size detect --flash_freq 40m ADDRESS FILEDIRECTORY

- ``PORT`` 替换为端口名称。

- ``BAUD`` 替换为波特率。

- ``ADDRESS`` 替换为 flash 中开始的地址。ESP-AT 对 ``ADDRESS`` 参数有严格的要求，不同固件的出厂参数二进制文件的地址不同，请参考下面的表格。

  .. only:: esp32

    .. list-table:: 出厂参数二进制文件下载地址
      :header-rows: 1

      * - 平台
        - 固件
        - 地址
      * - PLATFORM_ESP32
        - 所有固件
        - {IDF_TARGET_AT_PARAMS_ADDR}

.. only:: esp32c2

    .. list-table:: 出厂参数二进制文件下载地址
      :header-rows: 1

      * - 平台
        - 固件
        - 地址
      * - PLATFORM_ESP32C2
        - ESP32C2-4MB 固件
        - {IDF_TARGET_AT_PARAMS_ADDR}

  .. only:: esp32c3

    .. list-table:: 出厂参数二进制文件下载地址
      :header-rows: 1

      * - 平台
        - 固件
        - 地址
      * - PLATFORM_ESP32C3
        - MINI-1 固件
        - {IDF_TARGET_AT_PARAMS_ADDR}
      * - PLATFORM_ESP32C3
        - QCLOUD 固件
        - 0x30000

- ``FILEDIRECTORY`` 替换为出厂参数二进制文件的相对路径。

下面是将生成的出厂参数二进制文件烧录到 ``MY_MODULE`` 的命令示例。

::

    python esp-idf/components/esptool_py/esptool/esptool.py -p /dev/ttyUSB0 -b 921600 --before default_reset --after hard_reset --chip auto  write_flash --flash_mode dio --flash_size detect --flash_freq 40m {IDF_TARGET_AT_PARAMS_ADDR} ./factory_param_MY_MODULE.bin

直接修改出厂参数二进制文件
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

用二进制工具打开出厂参数二进制文件，根据 factory_param_type.csv 中的参数偏移量，直接在相应位置进行修改。

将修改后的 factory_param.bin 烧录至 flash（详情请见 :doc:`../Get_Started/Downloading_guide`）。
