How to Add Support for a Module
============================================

:link_to_translation:`zh_CN:[中文]`

The ESP-AT project supports multiple modules, and provides configuration for them in the :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` table and the files in the :project:`module_config` folder. If you want to add support for an {IDF_TARGET_NAME} module in your ESP-AT project, you need to modify those configuration files. The "{IDF_TARGET_NAME} module" here means:

- Modules that the ESP-AT project has not supported yet, including those of supported platform and not supported platform. However, adding support for the latter requires extra huge work, thus not recommended and not explained in this document.

- Modules that the ESP-AT project supports, but you want to modify the default configuration.

.. only:: esp32c2 or esp32c3 or esp32c6

  The document uses an example to explain how to add support for an {IDF_TARGET_NAME} module in the ESP-AT project. The example module is {IDF_TARGET_NAME}-MINI-1 that uses SPI instead of the default UART interface.

.. only:: esp32 or esp32s2

  This document will explain how to add new module support for a {IDF_TARGET_NAME} chip/module that ESP-AT already supports in the ESP-AT project. The following example will enable the default :ref:`FileSystem AT commands <FS-AT>` and add support for a new module.

.. contents::
   :local:
   :depth: 1

Step 1: Configure the Factory Parameters for the New Module
-----------------------------------------------------------------------------

Open your local :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`, insert a new row at the end, and set the relevant parameters as needed. In this example, we set ``platform`` to ``PLATFORM_{IDF_TARGET_CFG_PREFIX}`` and ``module_name`` to ``{IDF_TARGET_CFG_PREFIX}-USER-DEFINED``. The values for other parameters are shown in the table below (for parameter meanings, please refer to :ref:`factory-param-intro`).

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

Step 2: Configure OTA for the Newly Added Module
----------------------------------------------------

Add customized module information in the ``esp_at_module_info`` structure in :component_file:`at/src/at_default_config.c`.

The ``esp_at_module_info`` structure provides ``OTA`` upgrade verification ``token``:

.. code-block:: c

    typedef struct {
        char* module_name;
        char* ota_token;
        char* ota_ssl_token;
    } esp_at_module_info_t;

If you do not want to use ``OTA`` features, member 2 ``ota_token`` and member 3 ``ota_ssl_token`` should be set to ``NULL``. Member 1 ``module_name`` must correspond to the field ``module_name`` in the factory_param_data.csv file.

The modified ``esp_at_module_info`` structure is as follows:

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

Macro ``CONFIG_ESP_AT_OTA_TOKEN_MY_MODULE`` and macro ``CONFIG_ESP_AT_OTA_SSL_TOKEN_MY_MODULE`` are defined in the header file :component_file:`at/private_include/at_ota_token.h`.

.. code-block:: none

    #if defined(CONFIG_IDF_TARGET_{IDF_TARGET_CFG_PREFIX})
    ...
    #define CONFIG_ESP_AT_OTA_TOKEN_MY_MODULE       CONFIG_ESP_AT_OTA_TOKEN_DEFAULT

    ...
    #define CONFIG_ESP_AT_OTA_SSL_TOKEN_MY_MODULE       CONFIG_ESP_AT_OTA_SSL_TOKEN_DEFAULT

Step 3: Add New Module Configuration
------------------------------------------------

The ESP-AT project supports multiple platforms, each of which supports multiple module configurations and provides configuration files for each module configuration: :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` and :project:`module_config`. The table below lists the names of the platforms (i.e., chip series) supported by the ESP-AT project, the names of the module configurations, and the locations of the corresponding configuration files for each module configuration.

.. list-table::
   :header-rows: 1
   :widths: 5 10 40

   * - Platform
     - Module Configuration Name
     - Corresponding Default Configuration File
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

.. note::

  - When the ``silence mode`` in :ref:`python build.py install <esp-at-install-env>` is ``0``, the default sdkconfig corresponding to the module is ``sdkconfig.defaults``.
  - When the ``silence mode`` in :ref:`python build.py install <esp-at-install-env>` is ``1``, the default sdkconfig corresponding to the module is ``sdkconfig_silence.defaults``.

Firstly, enter ``module_config`` folder, and create a new folder to store all the configuration files for your module. Note that the folder name should be in lower case. Then, add the configuration files in the new folder: IDF_VERSION, patch, at_customize.csv, partitions_at.csv, sdkconfig.defaults, and sdkconfig_silence.defaults.

In this example, we copy the ``module_{IDF_TARGET_PATH_NAME}_default`` folder as well as the files within it and rename it as ``module_{IDF_TARGET_PATH_NAME}-user-defined``. The copied IDF_VERSION, patch, at_customize.csv, and partitions_at.csv do not need any modification in our case. We only need to modify the sdkconfig.defaults and sdkconfig_silence.defaults:

- Modify the two files to use the partition table in the ``module_{IDF_TARGET_PATH_NAME}-user-defined`` folder as follows:

  ::

    CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="module_config/module_{IDF_TARGET_PATH_NAME}-user-defined/partitions_at.csv"
    CONFIG_PARTITION_TABLE_FILENAME="module_config/module_{IDF_TARGET_PATH_NAME}-user-defined/partitions_at.csv"
    CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_FILE="module_config/module_{IDF_TARGET_PATH_NAME}-user-defined/at_customize.csv"

.. only:: esp32c2 or esp32c3 or esp32c6

  - Modify the two files to use the SPI configuration and remove the UART configuration as follows:
  
    - Remove the UART configuration
    
      ::
  
        CONFIG_AT_BASE_ON_UART=n
  
    - Add the SPI configuration
  
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

  - Add support for :ref:`FileSystem AT commands <FS-AT>` configuration

    ::
  
      CONFIG_AT_FS_COMMAND_SUPPORT=y

After completing the above steps, you can recompile the ESP-AT project to generate the module firmware. In this example, when we compile the AT firmware locally during :ref:`Step 3: Install Environment <esp-at-install-env>`, we can select ``PLATFORM_{IDF_TARGET_CFG_PREFIX}`` and ``{IDF_TARGET_CFG_PREFIX}-USER-DEFINED`` to generate the AT firmware.
