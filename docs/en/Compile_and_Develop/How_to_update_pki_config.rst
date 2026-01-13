How to Update PKI Configuration
===============================

:link_to_translation:`zh_CN:[中文]`

This document describes how to update the default :term:`PKI` configuration provided by ESP-AT by recompiling the firmware. The PKI configuration includes certificates and keys for TLS clients, TLS servers, MQTT clients, WebSocket clients, HTTP clients, and WPA2 Enterprise clients.

.. note::
  If you want to update certificates at runtime using AT commands without recompiling the firmware, please refer to :ref:`AT+SYSMFG command examples <sysmfg-pki>`.

.. contents::
   :local:
   :depth: 1

.. _factory-pki-intro:

Introduction to PKI Configuration
---------------------------------

The source file for the current default PKI configuration is located in the :component:`customized_partitions/raw_data` directory, as shown below:

   .. list-table::
      :header-rows: 1
      :widths: 60 60 70

      * - Function
        - Current Configuration
        - Related AT Commands
      * - TLS client
        - Set 0 client configuration

          * :component_file:`client_ca_00.crt <customized_partitions/raw_data/client_ca/client_ca_00.crt>`
          * :component_file:`client_cert_00.crt <customized_partitions/raw_data/client_cert/client_cert_00.crt>`
          * :component_file:`client_key_00.key <customized_partitions/raw_data/client_key/client_key_00.key>`

          Set 1 client configuration

          * :component_file:`client_ca_01.crt <customized_partitions/raw_data/client_ca/client_ca_01.crt>`
          * :component_file:`client_cert_01.crt <customized_partitions/raw_data/client_cert/client_cert_01.crt>`
          * :component_file:`client_key_01.key <customized_partitions/raw_data/client_key/client_key_01.key>`
        -
          * :ref:`AT+CIPSTART <cmd-START>`
          * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`

      * - TLS server
        -
          * :component_file:`server_ca.crt <customized_partitions/raw_data/server_ca/server_ca.crt>`
          * :component_file:`server_cert.crt <customized_partitions/raw_data/server_cert/server_cert.crt>`
          * :component_file:`server.key <customized_partitions/raw_data/server_key/server.key>`
        - :ref:`AT+CIPSERVER <cmd-SERVER>`

      * - MQTT client
        -
          * :component_file:`mqtt_ca.crt <customized_partitions/raw_data/mqtt_ca/mqtt_ca.crt>`
          * :component_file:`mqtt_client.crt <customized_partitions/raw_data/mqtt_cert/mqtt_client.crt>`
          * :component_file:`mqtt_client.key <customized_partitions/raw_data/mqtt_key/mqtt_client.key>`
        - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`

      * - WebSocket client
        - Set 0 client configuration

          * :component_file:`wss_ca_00.crt <customized_partitions/raw_data/wss_ca/wss_ca_00.crt>`
          * :component_file:`wss_client_00.crt <customized_partitions/raw_data/wss_cert/wss_client_00.crt>`
          * :component_file:`wss_client_00.key <customized_partitions/raw_data/wss_key/wss_client_00.key>`

          Set 1 client configuration

          * :component_file:`wss_ca_01.crt <customized_partitions/raw_data/wss_ca/wss_ca_01.crt>`
          * :component_file:`wss_client_01.crt <customized_partitions/raw_data/wss_cert/wss_client_01.crt>`
          * :component_file:`wss_client_01.key <customized_partitions/raw_data/wss_key/wss_client_01.key>`
        - :ref:`AT+WSCFG <cmd-WSCFG>`

      * - HTTP client
        - Set 0 client configuration

          * :component_file:`https_ca_00.crt <customized_partitions/raw_data/https_ca/https_ca_00.crt>`
          * :component_file:`https_client_00.crt <customized_partitions/raw_data/https_cert/https_client_00.crt>`
          * :component_file:`https_client_00.key <customized_partitions/raw_data/https_key/https_client_00.key>`

          Set 1 client configuration

          * :component_file:`https_ca_01.crt <customized_partitions/raw_data/https_ca/https_ca_01.crt>`
          * :component_file:`https_client_01.crt <customized_partitions/raw_data/https_cert/https_client_01.crt>`
          * :component_file:`https_client_01.key <customized_partitions/raw_data/https_key/https_client_01.key>`
        - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`

      * - WPA2 Enterprise client
        -
          * :component_file:`wpa2_ca.pem <customized_partitions/raw_data/wpa2_ca/wpa2_ca.pem>`
          * :component_file:`wpa2_client.crt <customized_partitions/raw_data/wpa2_cert/wpa2_client.crt>`
          * :component_file:`wpa2_client.key <customized_partitions/raw_data/wpa2_key/wpa2_client.key>`
        - :ref:`AT+CWJEAP <cmd-JEAP>`

Please modify the PKI configurations according to your own needs and generate ``mfg_nvs.bin`` file.

Generate mfg_nvs.bin
^^^^^^^^^^^^^^^^^^^^^

Please refer to :ref:`mfg-nvs-generate` document to generate the ``mfg_nvs.bin`` file with the PKI configurations.

Download mfg_nvs.bin
^^^^^^^^^^^^^^^^^^^^^

Please refer to :ref:`mfg-nvs-download` document.
