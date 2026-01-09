How to Update Factory Parameters
================================

{IDF_TARGET_VER: default="5.4", esp32c5="5.5", esp32c61="5.5"}

:link_to_translation:`zh_CN:[中文]`

This document describes how to update the default factory parameter configuration for ESP-AT by recompiling the firmware. The factory parameter configuration includes some Wi-Fi configurations, UART configurations, and module configurations.

.. note::
  If you want to update factory parameters at runtime using AT commands without recompiling the firmware, please refer to :ref:`AT+SYSMFG command examples <sysmfg-factory-param>`.

.. contents::
   :local:
   :depth: 1

.. _factory-param-intro:

Factory Parameter Configuration
-------------------------------

The default factory parameters are configured in the source file :component_file:`customized_partitions/raw_data/factory_param/factory_param_data.csv`, as shown below:

   .. list-table::
      :header-rows: 1
      :widths: 40 90 75

      * - Function
        - Current Configuration
        - Related AT Commands
      * - Version Information
        - version (version number of the factory parameter structure, currently version 4, used to identify parameter format and compatibility)
        - :ref:`AT+SYSMFG <cmd-SYSMFG>`

      * - Wi-Fi Configuration
        -
          * max_tx_power (Wi-Fi maximum transmission power for {IDF_TARGET_NAME}, see `{IDF_TARGET_NAME} Transmit Power <https://docs.espressif.com/projects/esp-idf/en/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-reference/network/esp_wifi.html#_CPPv425esp_wifi_set_max_tx_power6int8_t>`_ for the setting range)
          * country_code (country code)
          * start_channel (starting Wi-Fi channel)
          * channel_num (total number of Wi-Fi channels)
        - All AT commands requiring Wi-Fi functionality

      * - UART Configuration
        -
          * uart_port (UART port used for sending AT commands and receiving AT responses)
          * uart_baudrate (UART baud rate)
          * uart_tx_pin (UART TX pin)
          * uart_rx_pin (UART RX pin)
          * uart_cts_pin (UART CTS pin)
          * uart_rts_pin (UART RTS pin)
        - All AT commands requiring UART functionality

      * - System Store Mode
        - sys_store (default system store mode)

          * 0: Do not automatically store configuration changes to flash
          * 1: Automatically store configuration changes to flash (default)
        - :ref:`AT+SYSSTORE <cmd-SYSSTORE>`

      * - Module Name
        - module_name
        - :ref:`AT+GMR <cmd-GMR>`

Please modify the factory parameters configurations according to your own needs and generate ``mfg_nvs.bin`` file.

Generate mfg_nvs.bin
^^^^^^^^^^^^^^^^^^^^^

Please refer to :ref:`mfg-nvs-generate` document to generate the ``mfg_nvs.bin`` file with the factory parameters configurations.

Download mfg_nvs.bin
^^^^^^^^^^^^^^^^^^^^^

Please refer to :ref:`mfg-nvs-download` document.
