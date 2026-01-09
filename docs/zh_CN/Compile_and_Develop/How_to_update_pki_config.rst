如何更新 :term:`PKI` 配置
=============================================

:link_to_translation:`en:[English]`

本文档介绍了如何通过重新编译固件来更新 ESP-AT 提供的默认的 :term:`PKI` 配置。PKI 配置包括 TLS 客户端、TLS 服务器、MQTT 客户端、WebSocket 客户端、HTTP 客户端和 WPA2 Enterprise 客户端的证书和密钥。

.. note::
  如果您想在运行时通过 AT 命令更新证书而无需重新编译固件，请参考 :ref:`AT+SYSMFG 命令示例 <sysmfg-pki>`。

.. contents::
   :local:
   :depth: 1

.. _factory-pki-intro:

PKI 配置介绍
------------------

当前默认 PKI 配置的源文件位于 :component:`customized_partitions/raw_data` 目录下，如下所示：

   .. list-table::
      :header-rows: 1
      :widths: 60 60 70

      * - 功能
        - 当前配置
        - 相关 AT 命令
      * - TLS 客户端
        - 第 0 套客户端配置

          * :component_file:`client_ca_00.crt <customized_partitions/raw_data/client_ca/client_ca_00.crt>`
          * :component_file:`client_cert_00.crt <customized_partitions/raw_data/client_cert/client_cert_00.crt>`
          * :component_file:`client_key_00.key <customized_partitions/raw_data/client_key/client_key_00.key>`

          第 1 套客户端配置

          * :component_file:`client_ca_01.crt <customized_partitions/raw_data/client_ca/client_ca_01.crt>`
          * :component_file:`client_cert_01.crt <customized_partitions/raw_data/client_cert/client_cert_01.crt>`
          * :component_file:`client_key_01.key <customized_partitions/raw_data/client_key/client_key_01.key>`
        -
          * :ref:`AT+CIPSTART <cmd-START>`
          * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`

      * - TLS 服务器
        -
          * :component_file:`server_ca.crt <customized_partitions/raw_data/server_ca/server_ca.crt>`
          * :component_file:`server_cert.crt <customized_partitions/raw_data/server_cert/server_cert.crt>`
          * :component_file:`server.key <customized_partitions/raw_data/server_key/server.key>`
        - :ref:`AT+CIPSERVER <cmd-SERVER>`

      * - MQTT 客户端
        -
          * :component_file:`mqtt_ca.crt <customized_partitions/raw_data/mqtt_ca/mqtt_ca.crt>`
          * :component_file:`mqtt_client.crt <customized_partitions/raw_data/mqtt_cert/mqtt_client.crt>`
          * :component_file:`mqtt_client.key <customized_partitions/raw_data/mqtt_key/mqtt_client.key>`
        - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`

      * - WebSocket 客户端
        - 第 0 套客户端配置

          * :component_file:`wss_ca_00.crt <customized_partitions/raw_data/wss_ca/wss_ca_00.crt>`
          * :component_file:`wss_client_00.crt <customized_partitions/raw_data/wss_cert/wss_client_00.crt>`
          * :component_file:`wss_client_00.key <customized_partitions/raw_data/wss_key/wss_client_00.key>`

          第 1 套客户端配置

          * :component_file:`wss_ca_01.crt <customized_partitions/raw_data/wss_ca/wss_ca_01.crt>`
          * :component_file:`wss_client_01.crt <customized_partitions/raw_data/wss_cert/wss_client_01.crt>`
          * :component_file:`wss_client_01.key <customized_partitions/raw_data/wss_key/wss_client_01.key>`
        - :ref:`AT+WSCFG <cmd-WSCFG>`

      * - HTTP 客户端
        - 第 0 套客户端配置

          * :component_file:`https_ca_00.crt <customized_partitions/raw_data/https_ca/https_ca_00.crt>`
          * :component_file:`https_client_00.crt <customized_partitions/raw_data/https_cert/https_client_00.crt>`
          * :component_file:`https_client_00.key <customized_partitions/raw_data/https_key/https_client_00.key>`

          第 1 套客户端配置

          * :component_file:`https_ca_01.crt <customized_partitions/raw_data/https_ca/https_ca_01.crt>`
          * :component_file:`https_client_01.crt <customized_partitions/raw_data/https_cert/https_client_01.crt>`
          * :component_file:`https_client_01.key <customized_partitions/raw_data/https_key/https_client_01.key>`
        - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`

      * - WPA2 Enterprise 客户端
        -
          * :component_file:`wpa2_ca.pem <customized_partitions/raw_data/wpa2_ca/wpa2_ca.pem>`
          * :component_file:`wpa2_client.crt <customized_partitions/raw_data/wpa2_cert/wpa2_client.crt>`
          * :component_file:`wpa2_client.key <customized_partitions/raw_data/wpa2_key/wpa2_client.key>`
        - :ref:`AT+CWJEAP <cmd-JEAP>`

请根据自己的需求修改 PKI 配置，然后生成 ``mfg_nvs.bin`` 文件。

生成 mfg_nvs.bin 文件
---------------------------------

请参考 :ref:`mfg-nvs-generate` 文档生成带有 PKI 配置的 ``mfg_nvs.bin``。

下载 mfg_nvs.bin 文件
---------------------------------

请参考 :ref:`mfg-nvs-download` 文档。
