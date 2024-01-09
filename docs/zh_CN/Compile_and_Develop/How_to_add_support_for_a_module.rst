如何增加一个新的模组支持
=========================

:link_to_translation:`en:[English]`

ESP-AT 工程支持多个模组，并提供了模组的配置文件：:component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` 和 :project:`module_config`。下表列出了 ESP-AT 工程支持的平台（即芯片系列）、模组以及模组配置文件的位置。

.. list-table:: 
   :header-rows: 1
   :widths: 5 10 40

   * - 平台
     - 模组
     - 默认配置文件
   * - ESP32
     - - WROOM-32
       - PICO-D4
       - SOLO-1
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
   * - ESP32-C2
     - ESP32C2-2MB（所有带 2 MB flash 的 ESP32-C2（ESP8684） 系列）
     - - :project_file:`module_config/module_esp32c2-2mb/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c2-2mb/sdkconfig_silence.defaults`
   * - ESP32-C2
     - ESP32C2-4MB（所有带 4 MB flash 的 ESP32-C2（ESP8684） 系列）
     - - :project_file:`module_config/module_esp32c2_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c2_default/sdkconfig_silence.defaults`
   * - ESP32-C3
     - MINI-1
     - - :project_file:`module_config/module_esp32c3_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c3_default/sdkconfig_silence.defaults`
   * - ESP32-C6
     - ESP32C6-4MB（所有带 4 MB flash 的 ESP32-C6 系列）
     - - :project_file:`module_config/module_esp32c6_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c6_default/sdkconfig_silence.defaults`

.. 注意::

  - 当 ``./build.py menuconfig`` 中的 ``silence mode`` 为 ``0`` 时，对应模块的配置文件为 ``sdkconfig.defaults``。
  - 当 ``./build.py menuconfig`` 中的 ``silence mode`` 为 ``1`` 时，对应模块的配置文件为 ``sdkconfig_silence.defaults``。

如果要在 ESP-AT 工程中添加对某个 {IDF_TARGET_NAME} 模组的支持，则需要修改这些配置文件。此处的“{IDF_TARGET_NAME} 模组”指的是：

- ESP-AT 工程暂未适配支持的模组，包括 ESP-AT 已适配相应芯片的模组，和未适配相应芯片的模组。但不建议添加后者，因为工作量巨大，此文档也不做阐述。

- ESP-AT 工程已适配支持的模组，但用户需要对其修改默认配置的。

本文档将说明如何在 ESP-AT 工程中为 ESP-AT 已支持的某款 {IDF_TARGET_NAME} 芯片添加新的模组支持，下文中以添加对 ESP32-WROOM-32 支持为例，该模组使用 SDIO 而不是默认的 UART 接口。

.. contents::
   :local:
   :depth: 1

在 factory_param_data.csv 添加模组信息
--------------------------------------

打开本地的 :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`，在表格最后插入一行，根据实际需要设置相关参数。本例中，我们将 ``platform`` 设置为 ``PLATFORM_ESP32``、``module_name`` 设置为 ``WROOM32-SDIO``，其他参数设置值见下表（参数含义请参考 :ref:`factory-param-intro`）。

- platform: PLATFORM_ESP32
- module_name: WROOM32-SDIO
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

修改 esp_at_module_info 结构体
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

配置模组文件
------------

首先，进入 ``module_config`` 文件夹，创建一个子文件夹来存放模组的配置文件（文件夹名称为小写），然后在其中加入配置文件 IDF_VERSION、at_customize.csv、partitions_at.csv、sdkconfig.defaults 以及 sdkconfig_silence.defaults。

本例中，我们复制粘贴 ``module_esp32_default`` 文件夹及其中的配置文件，并重命名为 ``module_wroom32-sdio``。在本例中，配置文件 IDF_VERSION、at_customize.csv 和 partitions_at.csv 无需修改，我们只需修改 sdkconfig.defaults 和 sdkconfig_silence.defaults：

- 使用 ``module_wroom32-sdio`` 文件夹下的分区表，需要修改如下配置

  ::

    CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="module_config/module_wroom32-sdio/partitions_at.csv"
    CONFIG_PARTITION_TABLE_FILENAME="module_config/module_wroom32-sdio/partitions_at.csv"
    CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_FILE="module_config/module_wroom32-sdio/at_customize.csv"

- 使用 SDIO 配置，移除 UART 配置

  - 移除 UART 配置
  
    ::

      CONFIG_AT_BASE_ON_UART=n

  - 新增 SDIO 配置

    ::

      CONFIG_AT_BASE_ON_SDIO=y

完成上述步骤后，可重新编译 ESP-AT 工程生成模组固件。本例中，我们在配置工程时，应选择 ``PLATFORM_ESP32`` 和 ``WROOM32-SDIO`` 来生成模组固件。
