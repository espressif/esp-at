如何增加一个新的模组支持
============================

:link_to_translation:`en:[English]`

ESP-AT 工程支持多个模组，并提供了模组的配置文件：:component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` 和 :project:`module_config`。如果要在 ESP-AT 工程中添加对某个 {IDF_TARGET_NAME} 模组的支持，则需要修改这些配置文件。此处的”{IDF_TARGET_NAME} 模组“指的是：

- ESP-AT 工程暂未适配支持的模组，包括 ESP-AT 已适配相应芯片的模组，和未适配相应芯片的模组。但不建议添加后者，因为工作量巨大，此文档也不做阐述。

- ESP-AT 工程已适配支持的模组，但用户需要对其修改默认配置的。

.. only:: esp32c2 or esp32c3 or esp32c6

  本文档将说明如何在 ESP-AT 工程中为 ESP-AT 已支持的某款 {IDF_TARGET_NAME} 芯片添加新的模组支持，下文中以添加对 {IDF_TARGET_NAME}-MINI-1 支持为例，该模组使用 SPI 而不是默认的 UART 接口。

.. only:: esp32 or esp32s2

  本文档将说明如何在 ESP-AT 工程中为 ESP-AT 已支持的某款 {IDF_TARGET_NAME} 芯片/模组添加新的模组支持。下文将增加默认启用 :ref:`文件系统 AT 命令集 <FS-AT>` 的配置为例，来添加新的模组支持。

.. contents::
   :local:
   :depth: 1

第一步：配置新增模组的出厂参数
------------------------------------------------

打开本地的 :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`，在表格最后插入一行，根据实际需要设置相关参数。本例中，我们将 ``platform`` 设置为 ``PLATFORM_{IDF_TARGET_CFG_PREFIX}``、``module_name`` 设置为 ``{IDF_TARGET_CFG_PREFIX}-USER-DEFINED``，其他参数设置值见下表（参数含义请参考 :ref:`factory-param-intro`）。

- platform: PLATFORM_{IDF_TARGET_CFG_PREFIX}
- module_name: {IDF_TARGET_CFG_PREFIX}-USER-DEFINED
- description:
- version: 4
- max_tx_power: 78
- uart_port: 1
- start_channel: 1
- channel_num: 13
- country_code: CN
- uart_baudrate: -1
- uart_tx_pin: -1
- uart_rx_pin: -1
- uart_cts_pin: -1
- uart_rts_pin: -1

第二步：配置新增模组的 OTA
-----------------------------------

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

    #if defined(CONFIG_IDF_TARGET_ESP32C6)
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

第三步：新增模组配置
---------------------------

下表列出了 ESP-AT 工程支持的平台（即芯片系列）名称、模组配置名称以及各个模组配置对应的配置文件的位置。

.. list-table::
   :header-rows: 1
   :widths: 5 10 40

   * - 平台
     - 模组配置名称
     - 对应的默认配置文件
   * - ESP32
     - WROOM-32
     - - :project_file:`module_config/module_esp32_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32_default/sdkconfig_silence.defaults`
   * - ESP32
     - PICO-D4
     - - :project_file:`module_config/module_esp32_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32_default/sdkconfig_silence.defaults`
   * - ESP32
     - SOLO-1
     - - :project_file:`module_config/module_esp32_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32_default/sdkconfig_silence.defaults`
   * - ESP32
     - MINI-1
     - - :project_file:`module_config/module_esp32_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32_default/sdkconfig_silence.defaults`
   * - ESP32
     - WROVER-32
     - - :project_file:`module_config/module_wrover-32/sdkconfig.defaults`
       - :project_file:`module_config/module_wrover-32/sdkconfig_silence.defaults`
   * - ESP32
     - ESP32-D2WD
     - - :project_file:`module_config/module_esp32-d2wd/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32-d2wd/sdkconfig_silence.defaults`
   * - ESP32
     - ESP32-SDIO
     - - :project_file:`module_config/module_esp32-sdio/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32-sdio/sdkconfig_silence.defaults`
   * - ESP32-C2
     - ESP32C2-2MB
     - - :project_file:`module_config/module_esp32c2-2mb/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c2-2mb/sdkconfig_silence.defaults`
   * - ESP32-C2
     - ESP32C2-BLE-2MB
     - - :project_file:`module_config/module_esp32c2-ble-2mb/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c2-ble-2mb/sdkconfig_silence.defaults`
   * - ESP32-C2
     - ESP32C2-4MB
     - - :project_file:`module_config/module_esp32c2_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c2_default/sdkconfig_silence.defaults`
   * - ESP32-C3
     - MINI-1
     - - :project_file:`module_config/module_esp32c3_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c3_default/sdkconfig_silence.defaults`
   * - ESP32-C3
     - ESP32C3-SPI
     - - :project_file:`module_config/module_esp32c3-spi/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c3-spi/sdkconfig_silence.defaults`
   * - ESP32-C3
     - ESP32C3_RAINMAKER
     - - :project_file:`module_config/module_esp32c3_rainmaker/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c3_rainmaker/sdkconfig_silence.defaults`
   * - ESP32-C6
     - ESP32C6-4MB
     - - :project_file:`module_config/module_esp32c6_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c6_default/sdkconfig_silence.defaults`

.. 注意::

  - 当 :ref:`python build.py install <esp-at-install-env>` 中的 ``silence mode`` 为 ``0`` 时，模组配置对应的配置文件为 ``sdkconfig.defaults``。
  - 当 :ref:`python build.py install <esp-at-install-env>` 中的 ``silence mode`` 为 ``1`` 时，模组配置对应的配置文件为 ``sdkconfig_silence.defaults``。

首先，进入 ``module_config`` 文件夹，创建一个子文件夹来存放模组配置的配置文件（文件夹名称为小写），然后在其中加入配置文件 IDF_VERSION、patch、at_customize.csv、partitions_at.csv、sdkconfig.defaults 以及 sdkconfig_silence.defaults。

本例中，我们复制粘贴 ``module_{IDF_TARGET_PATH_NAME}_default`` 文件夹及其中的配置文件，并重命名为 ``module_{IDF_TARGET_PATH_NAME}-user-defined``。在本例中，配置文件 IDF_VERSION、patch、at_customize.csv 和 partitions_at.csv 无需修改，我们只需修改 sdkconfig.defaults 和 sdkconfig_silence.defaults：

- 使用 ``module_{IDF_TARGET_PATH_NAME}-user-defined`` 文件夹下的分区表，需要修改如下配置

  ::

    CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="module_config/module_{IDF_TARGET_PATH_NAME}-user-defined/partitions_at.csv"
    CONFIG_PARTITION_TABLE_FILENAME="module_config/module_{IDF_TARGET_PATH_NAME}-user-defined/partitions_at.csv"
    CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_FILE="module_config/module_{IDF_TARGET_PATH_NAME}-user-defined/at_customize.csv"

.. only:: esp32c2 or esp32c3 or esp32c6

  - 使用 SPI 配置，移除 UART 配置
  
    - 移除 UART 配置
    
      ::
  
        CONFIG_AT_BASE_ON_UART=n
  
    - 新增 SPI 配置
  
      ::
  
        CONFIG_AT_BASE_ON_SPI=y
        CONFIG_SPI_STANDARD_MODE=y
        CONFIG_SPI_SCLK_PIN=6
        CONFIG_SPI_MOSI_PIN=7
        CONFIG_SPI_MISO_PIN=2
        CONFIG_SPI_CS_PIN=10
        CONFIG_SPI_HANDSHAKE_PIN=3
        CONFIG_SPI_NUM=1
        CONFIG_SPI_MODE=0
        CONFIG_TX_STREAM_BUFFER_SIZE=4096
        CONFIG_RX_STREAM_BUFFER_SIZE=4096

.. only:: esp32 or esp32s2

  - 添加启用 :ref:`文件系统 AT 命令 <FS-AT>` 配置
    
    ::
  
      CONFIG_AT_FS_COMMAND_SUPPORT=y

完成上述步骤后，可重新编译 ESP-AT 工程生成模组固件。本例中，我们在本地编译 AT 固件 :ref:`第三步安装环境 <esp-at-install-env>` 时，就可以选择 ``PLATFORM_{IDF_TARGET_CFG_PREFIX}`` 和 ``{IDF_TARGET_CFG_PREFIX}-USER-DEFINED`` 来生成 AT 固件。
