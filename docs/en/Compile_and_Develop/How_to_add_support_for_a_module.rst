How to Add Support for a Module
================================

:link_to_translation:`zh_CN:[中文]`

The ESP-AT project supports multiple modules, and provides configuration for them in the :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` table and the files in the :project:`module_config` folder. See the table below for the supported platforms (chip series) and modules, as well as locations of the default configuration files.

.. list-table:: default configuration files
   :header-rows: 1
   :widths: 5 10 40

   * - Platform
     - Module
     - Default
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
     - ESP32C2-2MB (all ESP32-C2 (ESP8684) series with 2 MB flash)
     - - :project_file:`module_config/module_esp32c2-2mb/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c2-2mb/sdkconfig_silence.defaults`
   * - ESP32-C2
     - ESP32C2-4MB (all ESP32-C2 (ESP8684) series with 4 MB flash)
     - - :project_file:`module_config/module_esp32c2_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c2_default/sdkconfig_silence.defaults`
   * - ESP32-C3
     - MINI-1
     - - :project_file:`module_config/module_esp32c3_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c3_default/sdkconfig_silence.defaults`
   * - ESP32-C6
     - ESP32C6-4MB (all ESP32-C6 series with 4 MB flash)
     - - :project_file:`module_config/module_esp32c6_default/sdkconfig.defaults`
       - :project_file:`module_config/module_esp32c6_default/sdkconfig_silence.defaults`

.. note::

  - When the ``silence mode`` in ``./build.py menuconfig`` is ``0``, the default sdkconfig corresponding to the module is ``sdkconfig.defaults``.
  - When the ``silence mode`` in ``./build.py menuconfig`` is ``1``, the default sdkconfig corresponding to the module is ``sdkconfig_silence.defaults``.

If you want to add support for an {IDF_TARGET_NAME} module in your ESP-AT project, you need to modify those configuration files. The "{IDF_TARGET_NAME} module" here means:

- Modules that the ESP-AT project has not supported yet, including those of supported platform and not supported platform. However, adding support for the latter requires extra huge work, thus not recommended and not explained in this document.

- Modules that the ESP-AT project supports, but you want to modify the default configuration.

The document uses an example to explain how to add support for an {IDF_TARGET_NAME} module in the ESP-AT project. The example module is ESP32-WROOM-32 that uses SDIO instead of the default UART interface.

.. contents::
   :local:
   :depth: 1

Add Module to factory_param_data.csv
------------------------------------

Open your local :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`, insert a new row at the end, set the parameters as needed. In the example, we set ``platform`` to ``PLATFORM_ESP32``, ``module_name`` to ``WROOM32-SDIO``, as well as other parameters as follows (see :ref:`factory-param-intro` for what each parameter represents):

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

Modify esp_at_module_info Structure
-----------------------------------

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

Configure the Module
---------------------

Firstly, enter ``module_config`` folder, and create a new folder to store all the configuration files for your module. Note that the folder name should be in lower case. Then, add the configuration files in the new folder: IDF_VERSION, at_customize.csv, partitions_at.csv, sdkconfig.defaults, and sdkconfig_silence.defaults.

In this example, we copy the ``module_esp32_default`` folder as well as the files within it and rename it as ``module_wroom32-sdio``. The copied IDF_VERSION, at_customize.csv, and partitions_at.csv do not need any modification in our case. We only need to modify the sdkconfig.defaults and sdkconfig_silence.defaults:

- Modify the two files to use the partition table in the ``module_wroom32-sdio`` folder as follows:

  ::

    CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="module_config/module_wroom32-sdio/partitions_at.csv"
    CONFIG_PARTITION_TABLE_FILENAME="module_config/module_wroom32-sdio/partitions_at.csv"
    CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_FILE="module_config/module_wroom32-sdio/at_customize.csv"

- Modify the two files to use the SDIO configuration and remove the UART configuration as follows:

  - Remove the UART configuration
  
    ::

      CONFIG_AT_BASE_ON_UART=n

  - Add the SDIO configuration

    ::

      CONFIG_AT_BASE_ON_SDIO=y

After finishing the above steps, you can recompile the ESP-AT project to generate the firmware for your module. In this example, we choose ``PLATFORM_ESP32`` and ``WROOM32-SDIO`` when configuring the project to generate the firmware for it.
