如何更新出厂参数
======================================

{IDF_TARGET_VER: default="undefined", esp32="5.0", esp32c2="5.0", esp32c3="5.0", esp32c6="5.1", esp32s2="5.0"}

:link_to_translation:`en:[English]`

本文档介绍了如何更新 ESP-AT 默认的出厂参数配置。出厂参数配置包括一些 Wi-Fi 配置、UART 配置、模组配置。

.. contents::
   :local:
   :depth: 1

.. _factory-param-intro:

出厂参数配置介绍
------------------

当前默认的出厂参数配置的源文件 :component_file:`customized_partitions/raw_data/factory_param/factory_param_data.csv`，如下所示：

   .. list-table::
      :header-rows: 1
      :widths: 35 90 75

      * - 功能
        - 当前配置
        - 相关 AT 命令
      * - Wi-Fi 配置
        -
          * max_tx_power ({IDF_TARGET_NAME} 的 Wi-Fi 最大发射功率，详情请见 `{IDF_TARGET_NAME} 发射功率 <https://docs.espressif.com/projects/esp-idf/zh_CN/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-reference/network/esp_wifi.html#_CPPv425esp_wifi_set_max_tx_power6int8_t>`_ 设置范围)
          * country_code (国家代码)
          * start_channel (起始 Wi-Fi 信道)
          * channel_num (Wi-Fi 的总信道数量)
        - 所有需要 Wi-Fi 功能的 AT 命令

      * - UART 配置
        -
          * uart_port (用于发送 AT 命令和接收 AT 响应的 UART 端口)
          * uart_baudrate (UART 波特率)
          * uart_tx_pin (UART TX 引脚)
          * uart_rx_pin (UART RX 引脚)
          * uart_cts_pin (UART CTS 引脚)
          * uart_rts_pin (UART RTS 引脚)
        - 所有需要 UART 功能的 AT 命令

      * - 模组名称
        - module_name
        - :ref:`AT+GMR <cmd-GMR>`

请根据自己的需求修改出厂参数配置，然后生成 ``mfg_nvs.bin`` 文件。

生成 mfg_nvs.bin 文件
---------------------------------

请参考 :ref:`mfg-nvs-generate` 文档生成带有出厂参数配置的 ``mfg_nvs.bin``。

下载 mfg_nvs.bin 文件
---------------------------------

请参考 :ref:`mfg-nvs-download` 文档。
