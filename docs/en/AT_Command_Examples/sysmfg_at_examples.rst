AT+SYSMFG Command Examples
==========================================================================

:link_to_translation:`zh_CN:[中文]`

This document provides detailed examples for using the :ref:`AT+SYSMFG <cmd-SYSMFG>` command to operate the :term:`manufacturing nvs` user partition.

.. list::

   - :ref:`Introduction <sysmfg-intro>`

     - :ref:`Overview <sysmfg-overview>`
     - :ref:`Namespace Structure <sysmfg-namespace-structure>`
     - :ref:`Data Types <sysmfg-data-types>`
     - :ref:`Other Important Notes <sysmfg-other-notes>`

   - :ref:`Factory Parameter Configuration <sysmfg-factory-param>`

     - :ref:`Module Name Operations <sysmfg-module-name>`
     - :ref:`UART Configuration Operations <sysmfg-uart-config>`
     - :ref:`Maximum TX Power Operations <sysmfg-max-tx-power>`
     - :ref:`Country Code Operations <sysmfg-country-code>`
     - :ref:`System Store Mode Operations <sysmfg-sys-store>`
     - :ref:`Adding Custom Key-Value Pairs <sysmfg-custom-key-value>`

   - :ref:`PKI Configuration <sysmfg-pki>`

     - :ref:`SSL Client Certificates <sysmfg-ssl-client>`
     - :ref:`SSL Server Certificates <sysmfg-ssl-server>`

   :not esp32s2: - :ref:`GATTS Configuration <sysmfg-gatts>`
   - :ref:`Custom Data Storage <sysmfg-custom-data>`

.. _sysmfg-intro:

Introduction
------------

.. _sysmfg-overview:

Overview
**********

The manufacturing NVS partition contains multiple namespaces, each containing several key-value pairs to store different types of configuration information. Commonly used namespaces include:

.. only:: esp32 or esp32c3

  .. list-table::
     :header-rows: 1
     :widths: 20 45 25

     * - Namespace
       - Description
       - Related AT Commands
     * - ``factory_param``
       - Factory parameters (module name, Wi-Fi configuration, UART configuration, etc.)
       - See :ref:`factory-param-intro`
     * - ``client_cert``
       - SSL client certificates
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_key``
       - SSL client keys
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_ca``
       - SSL client CA certificates
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``server_cert``
       - SSL server certificates
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_key``
       - SSL server keys
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_ca``
       - SSL server CA certificates
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``mqtt_cert``
       - MQTT client certificates
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_key``
       - MQTT client keys
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_ca``
       - MQTT client CA certificates
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``https_cert``
       - HTTPS client certificates
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_key``
       - HTTPS client private keys
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_ca``
       - HTTPS client CA certificates
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``wpa2_cert``
       - WPA2-Enterprise client certificates
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_key``
       - WPA2-Enterprise client private keys
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_ca``
       - WPA2-Enterprise client CA certificates
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wss_cert``
       - WebSocket client certificates
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_key``
       - WebSocket client private keys
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_ca``
       - WebSocket client CA certificates
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``ble_data``
       - Bluetooth LE service configurations
       - :ref:`AT+BLEGATTSSRVCRE <cmd-GSSRVCRE>`

.. only:: esp32c2 or esp32c5 or esp32c6 or esp32c61

  .. list-table::
     :header-rows: 1
     :widths: 20 45 25

     * - Namespace
       - Description
       - Related AT Commands
     * - ``factory_param``
       - Factory parameters (module name, Wi-Fi configuration, UART configuration, etc.)
       - See :ref:`factory-param-intro`
     * - ``client_cert``
       - SSL client certificates
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_key``
       - SSL client keys
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_ca``
       - SSL client CA certificates
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``server_cert``
       - SSL server certificates
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_key``
       - SSL server keys
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_ca``
       - SSL server CA certificates
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``mqtt_cert``
       - MQTT client certificates
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_key``
       - MQTT client keys
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_ca``
       - MQTT client CA certificates
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``https_cert``
       - HTTPS client certificates
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_key``
       - HTTPS client private keys
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_ca``
       - HTTPS client CA certificates
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``wpa2_cert``
       - WPA2-Enterprise client certificates
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_key``
       - WPA2-Enterprise client private keys
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_ca``
       - WPA2-Enterprise client CA certificates
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wss_cert``
       - WebSocket client certificates
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_key``
       - WebSocket client private keys
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_ca``
       - WebSocket client CA certificates
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``ble_data``
       - Bluetooth LE service configurations
       - :ref:`AT+BLEINIT <cmd-BINIT>`

.. only:: esp32s2

  .. list-table::
     :header-rows: 1
     :widths: 20 45 25

     * - Namespace
       - Description
       - Related AT Commands
     * - ``factory_param``
       - Factory parameters (module name, Wi-Fi configuration, UART configuration, etc.)
       - See :ref:`factory-param-intro`
     * - ``client_cert``
       - SSL client certificates
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_key``
       - SSL client keys
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_ca``
       - SSL client CA certificates
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``server_cert``
       - SSL server certificates
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_key``
       - SSL server keys
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_ca``
       - SSL server CA certificates
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``mqtt_cert``
       - MQTT client certificates
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_key``
       - MQTT client keys
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_ca``
       - MQTT client CA certificates
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``https_cert``
       - HTTPS client certificates
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_key``
       - HTTPS client private keys
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_ca``
       - HTTPS client CA certificates
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``wpa2_cert``
       - WPA2-Enterprise client certificates
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_key``
       - WPA2-Enterprise client private keys
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_ca``
       - WPA2-Enterprise client CA certificates
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wss_cert``
       - WebSocket client certificates
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_key``
       - WebSocket client private keys
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_ca``
       - WebSocket client CA certificates
       - :ref:`AT+WSCFG <cmd-WSCFG>`

Query all namespaces actually supported by the current firmware:

  **Command:**

  .. code-block:: none

    AT+SYSMFG?

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param"
    +SYSMFG:"client_cert"
    +SYSMFG:"client_key"
    +SYSMFG:"client_ca"
    +SYSMFG:"server_cert"
    +SYSMFG:"server_key"
    +SYSMFG:"server_ca"
    ...
    +SYSMFG:"mqtt_cert"
    +SYSMFG:"mqtt_key"
    +SYSMFG:"mqtt_ca"

    OK

.. note::

  - The supported namespaces may vary depending on the chip and firmware version.
  - All command response examples in this document are for reference only. Actual output may differ by chip model, firmware version, and configuration.

.. _sysmfg-namespace-structure:

Namespace Structure
*********************

Each namespace is a collection of key-value pairs. Different namespaces use different key naming conventions:

- Certificate-related namespaces (``client_cert``, ``client_key``, ``client_ca``, ``server_cert``, ``server_key``, ``server_ca``): Use ``<base_name>.<index>`` format (e.g., ``client_cert.0``, ``client_cert.1``) to store multiple certificate sets.
- ``factory_param`` namespace: Uses descriptive key names (e.g., ``module_name``, ``uart_port``, ``country_code``).
- ``ble_data`` namespace: Uses ``cfg`` with index format (e.g., ``cfg0``, ``cfg1``, ``cfg2``).

.. _sysmfg-data-types:

Data Types
***********

The ``<type>`` parameter in ``AT+SYSMFG`` commands indicates the data type:

- 1: u8 (8-bit unsigned integer, range: 0-255, e.g., UART port number, channel number)
- 2: i8 (8-bit signed integer, range: -128 to 127, for signed configuration values)
- 3: u16 (16-bit unsigned integer, range: 0-65535, e.g., GPIO pin numbers)
- 4: i16 (16-bit signed integer, range: -32768 to 32767)
- 5: u32 (32-bit unsigned integer, range: 0-4294967295)
- 6: i32 (32-bit signed integer, range: -2147483648 to 2147483647, e.g., UART baudrate like 115200)
- 7: string (text string, e.g., module name "MINI-1", country code "CN")
- 8: binary (binary data, e.g., certificates, private keys, BLE service configurations)

.. _sysmfg-other-notes:

Other Important Notes
***********************

- **Length Parameter Explanation**: The meaning of the ``<length>`` parameter in responses varies by data type:

  - For numeric types (types 1-6): ``<length>`` represents the number of characters in the string representation of the value. For example, the i32 baudrate value 115200 is displayed as 6 characters, so length is 6.
  - For string and binary types (types 7-8): ``<length>`` represents the actual number of bytes.

- **Binary Data Transmission**: When writing binary data (type 8), the command returns ``>`` and waits for you to send exactly the number of bytes specified in the ``<value>`` parameter.

- **Data Persistence**: Data written to manufacturing NVS persists across device reboots until explicitly erased.

- **Offset Reading**: You can read partial data from a key using the offset and length parameters, useful for large data blocks.

- **Compilation vs Runtime**: While some data (like BLE services) can be customized at compile time by modifying the source CSV files and regenerating ``mfg_nvs.bin``, the ``AT+SYSMFG`` command allows you to update certain values at runtime without recompilation.

- **References**: For more information about Non-Volatile Storage (NVS) concepts and other NVS operations, please refer to the `ESP-IDF NVS documentation <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html>`_.

.. _sysmfg-factory-param:

Factory Parameter Configuration
--------------------------------

The factory_param namespace stores factory parameters, such as Wi-Fi configuration, UART configuration, and module information. ESP-AT uses these parameters during initialization. They affect the device's default behavior. For details, please refer to :doc:`factory parameter configuration <../Compile_and_Develop/How_to_update_factory_parameters>`.

.. important::

  After you modify factory parameters, run :ref:`AT+RST <cmd-RST>` or :ref:`AT+RESTORE <cmd-RESTORE>` to restart the device and apply changes.

Query All Parameters
**********************

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","version",2
    +SYSMFG:"factory_param","module_name",7
    +SYSMFG:"factory_param","max_tx_power",2
    +SYSMFG:"factory_param","country_code",7
    +SYSMFG:"factory_param","start_channel",1
    +SYSMFG:"factory_param","channel_num",1
    +SYSMFG:"factory_param","uart_port",2
    +SYSMFG:"factory_param","uart_baudrate",6
    +SYSMFG:"factory_param","uart_tx_pin",6
    +SYSMFG:"factory_param","uart_rx_pin",6
    +SYSMFG:"factory_param","uart_cts_pin",6
    +SYSMFG:"factory_param","uart_rts_pin",6
    +SYSMFG:"factory_param","sys_store",1

    OK

.. _sysmfg-module-name:

Module Name Operations
***********************

1. Read module name:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","module_name"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","module_name",7,6,MINI-1

    OK

2. Write a new module name:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","module_name",7,<name_length>

  **Response:**

  .. code-block:: none

    OK

    >

  After ``>`` is returned, send the module name string. For example, send "MY-MODULE-01".

  **Response after data transmission:**

  .. code-block:: none

    OK

3. Verify the configuration:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","module_name"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","module_name",7,12,MY-MODULE-01

    OK

  The module name has been successfully updated to "MY-MODULE-01".

.. _sysmfg-uart-config:

UART Configuration Operations
******************************

1. To read UART port:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","uart_port"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","uart_port",2,1,1

    OK

2. To read UART baudrate:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","uart_baudrate"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","uart_baudrate",6,6,115200

    OK

3. Update UART TX pin:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","uart_tx_pin",6,17

  **Response:**

  .. code-block:: none

    OK

  .. note::

    After you restart, switch your hardware connection to the new pins or send AT commands using the new UART configuration to verify the change.

.. _sysmfg-max-tx-power:

Maximum TX Power Operations
****************************

1. Read maximum TX power:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","max_tx_power"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","max_tx_power",2,2,78

    OK

2. Update maximum TX power:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","max_tx_power",2,60

  **Response:**

  .. code-block:: none

    OK

  For the valid Wi-Fi maximum TX power values, please refer to the ``<wifi_power>`` parameter range in :ref:`AT+RFPOWER <cmd-RFPOWER>`.

3. Restart the device to apply changes:

  **Command:**

  .. code-block:: none

    AT+RST

  **Response:**

  .. code-block:: none

    ready

4. Verify the configuration:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","max_tx_power"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","max_tx_power",2,2,60

    OK

  The maximum TX power has been successfully updated.

5. Verify using AT+RFPOWER command:

  **Command:**

  .. code-block:: none

    AT+RFPOWER?

  **Response:**

  .. code-block:: none

    +RFPOWER:60

    OK

  The maximum TX power is now 60 (60 × 0.25 dBm = 15 dBm).

.. _sysmfg-country-code:

Country Code Operations
************************

1. Read country code:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","country_code"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","country_code",7,2,CN

    OK

2. Update country code:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","country_code",7,2

  **Response:**

  .. code-block:: none

    OK

    >

  After ``>`` is returned, send the country code (e.g., "US", "JP", "EU").

  **Response after data transmission:**

  .. code-block:: none

    OK

3. Restart the device to apply changes:

  **Command:**

  .. code-block:: none

    AT+RST

  **Response:**

  .. code-block:: none

    ready

4. Verify the configuration:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","country_code"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","country_code",7,2,US

    OK

  The country code has been successfully updated to "US".

5. Update channel number to 11 (US standard):

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","channel_num",1,11

  **Response:**

  .. code-block:: none

    OK

6. Restart the device to apply changes:

  **Command:**

  .. code-block:: none

    AT+RST

  **Response:**

  .. code-block:: none

    ready

7. Verify country code and channel configuration using AT+CWCOUNTRY command:

  **Command:**

  .. code-block:: none

    AT+CWCOUNTRY?

  **Response:**

  .. code-block:: none

    +CWCOUNTRY:0,"US",1,11

    OK

  **Response description:**

  - ``0``: Country code policy (0 means using default country code configuration)
  - ``"US"``: Country code
  - ``1``: Starting channel (start_channel)
  - ``11``: Number of channels (channel_num)

  The configuration has been successfully applied. The device now uses the US country code and channel configuration.

.. _sysmfg-sys-store:

System Store Mode Operations
******************************

The ``sys_store`` parameter controls whether the system automatically stores configuration changes to flash. This parameter is related to the :ref:`AT+SYSSTORE <cmd-SYSSTORE>` command functionality.

1. Read the current system store mode:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","sys_store"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","sys_store",1,1,1

    OK

  ``sys_store`` values:

  - ``0``: Do not automatically store configurations to flash (equivalent to ``AT+SYSSTORE=0``)
  - ``1``: Automatically store configurations to flash (equivalent to ``AT+SYSSTORE=1``, default value)

2. Change system store mode to disable automatic storage:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","sys_store",1,0

  **Response:**

  .. code-block:: none

    OK

3. Restart the device to apply changes:

  **Command:**

  .. code-block:: none

    AT+RST

  **Response:**

  .. code-block:: none

    ready

4. Verify the configuration:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","sys_store"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","sys_store",1,1,0

    OK

5. Verify using AT+SYSSTORE command:

  **Command:**

  .. code-block:: none

    AT+SYSSTORE?

  **Response:**

  .. code-block:: none

    +SYSSTORE:0

    OK

  The system store mode has been successfully changed to not automatically store (``0`` means auto-storage is disabled).

**Impact of the change:**

When ``sys_store`` is set to ``0``, configuration changes (such as Wi-Fi settings or other commands that support auto-storage) are not saved to flash automatically. After a restart, the device will use the previously saved settings. For details on which commands are affected, see :ref:`AT+SYSSTORE <cmd-SYSSTORE>`.

If you need to save specific configurations while in ``sys_store=0`` mode, you can use the :ref:`AT+SYSSTORE <cmd-SYSSTORE>` command to temporarily enable storage, execute the configuration command, and then disable it.

.. _sysmfg-custom-key-value:

Adding Custom Key-Value Pairs
******************************

In addition to predefined factory parameters, you can add custom key-value pairs to the ``factory_param`` namespace to store application-specific configuration data.

This example demonstrates how to add a custom key-value pair, such as storing a device serial number:

1. Add a custom string-type key-value pair:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","device_sn",7,16

  **Response:**

  .. code-block:: none

    OK

    >

  After ``>`` is returned, send the device serial number (e.g., "SN202601050001").

  **Response after data transmission:**

  .. code-block:: none

    OK

2. Read the custom key-value pair:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","device_sn"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","device_sn",7,16,SN202601050001

    OK

3. Add a numeric-type custom key-value pair:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","hw_version",6,1001

  **Response:**

  .. code-block:: none

    OK

  This stores the hardware version number 1001.

4. Query all key-value pairs (including custom ones):

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param"

  **Response:**

  .. code-block:: none

    +SYSMFG:"factory_param","version",2
    +SYSMFG:"factory_param","module_name",7
    ...
    +SYSMFG:"factory_param","device_sn",7
    +SYSMFG:"factory_param","hw_version",6

    OK

5. Delete the custom key-value pair:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=0,"factory_param","device_sn"

  **Response:**

  .. code-block:: none

    OK

.. note::

  - Custom key names should avoid conflicts with predefined key names (such as ``module_name``, ``uart_port``, etc.)
  - Use meaningful key names for easier maintenance
  - Custom key-value pairs persist across reboots
  - Too many key-value pairs may affect performance; it's recommended to store only necessary configuration information
  - After modifying values in ``factory_param``, it's recommended to restart the device to ensure the application correctly reads the new values

.. _sysmfg-pki:

PKI Configuration
------------------

PKI (Public Key Infrastructure) configuration is used to create, manage, distribute, store, and revoke digital certificates, as well as manage public key cryptography. ESP-AT supports configuring certificates for various functions, including SSL client/server, MQTT client, HTTPS client, and more. The configuration method for all certificates is identical, implemented through the :ref:`AT+SYSMFG <cmd-SYSMFG>` command to operate the corresponding namespaces. For more details, please refer to :ref:`PKI Configuration Introduction <factory-pki-intro>`.

**Namespace mapping for different features:**

.. list-table::
   :header-rows: 1
   :widths: 30 35

   * - Feature
     - Certificate/Key Namespaces
   * - SSL Client
     - ``client_cert``, ``client_key``, ``client_ca``
   * - SSL Server
     - ``server_cert``, ``server_key``, ``server_ca``
   * - MQTT Client
     - ``mqtt_cert``, ``mqtt_key``, ``mqtt_ca``
   * - HTTPS Client
     - ``https_cert``, ``https_key``, ``https_ca``
   * - WebSocket Client
     - ``wss_cert``, ``wss_key``, ``wss_ca``
   * - WPA2 Enterprise Client
     - ``wpa2_cert``, ``wpa2_key``, ``wpa2_ca``

.. note::
  You can query all namespaces supported by the current firmware using the ``AT+SYSMFG?`` command to confirm if the certificate namespaces you need are available.

This section uses SSL certificates as an example to illustrate the complete process of certificate configuration. Certificate configuration for other functions (such as MQTT, HTTPS) can follow the same method, only requiring modification of the corresponding namespace names according to the table above.

.. _sysmfg-ssl-client:

SSL Client Certificates
************************

The client_cert, client_key, and client_ca namespaces store SSL certificates and keys used for secure connections. These three namespaces can work together to enable SSL authentication (one-way or mutual authentication).

Query Certificate Information
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. To read all keys in the client certificate namespace:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"client_cert"

  **Response:**

  .. code-block:: none

    +SYSMFG:"client_cert","client_cert.0",8
    +SYSMFG:"client_cert","client_cert.1",8

    OK

  Similarly, you can query client_key and client_ca namespaces:

  .. code-block:: none

    AT+SYSMFG=1,"client_key"
    AT+SYSMFG=1,"client_ca"

2. Read a specific certificate or key:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"client_cert","client_cert.0"

  **Response:**

  .. code-block:: none

    +SYSMFG:"client_cert","client_cert.0",8,1164,<value>

    OK

Configure Certificates
^^^^^^^^^^^^^^^^^^^^^^^

1. Write client certificate:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"client_cert","client_cert.0",8,1164

  **Response:**

  .. code-block:: none

    OK

    >

  After ``>`` is returned, send the certificate data (1164 bytes). After the data is sent, the system returns:

  .. code-block:: none

    OK

2. Write client key:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"client_key","client_key.0",8,<key_length>

  **Response:**

  .. code-block:: none

    OK

    >

  After ``>`` is returned, send the key data. After the data is sent, the system returns:

  .. code-block:: none

    OK

3. Write CA certificate:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"client_ca","client_ca.0",8,<ca_length>

  **Response:**

  .. code-block:: none

    OK

    >

  After ``>`` is returned, send the CA certificate data. After the data is sent, the system returns:

  .. code-block:: none

    OK

Use Certificates
^^^^^^^^^^^^^^^^^

After writing client certificates, keys (and client CA certificate for mutual authentication) to the manufacturing NVS partition, you can use them to establish a secure SSL connection. Here's an example:

1. Ensure certificates are written to the manufacturing partition:

   - Client certificate in ``client_cert`` namespace
   - Client key in ``client_key`` namespace
   - Client CA certificate in ``client_ca`` namespace

2. Configure SSL connection to use manufacturing partition certificates:

   **Command:**

   .. code-block:: none

     AT+CIPSSLCCONF=3,0,0

   **Response:**

   .. code-block:: none

     OK

   This configures the SSL client to use certificates from the manufacturing partition (Index 0).

3. Connect to Wi-Fi:

   **Command:**

   .. code-block:: none

     AT+CWJAP="your_ssid","your_password"

   **Response:**

   .. code-block:: none

     OK

4. Establish SSL connection to a server:

   **Command:**

   .. code-block:: none

     AT+CIPSTART="SSL","example.com",8883

   **Response:**

   .. code-block:: none

     CONNECT

     OK

   The SSL connection will use the certificates from the manufacturing partition for authentication.

.. note::

  For more details about SSL configuration, please refer to the :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>` command documentation.

Erase Certificates
^^^^^^^^^^^^^^^^^^^

Erase a specific certificate:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=0,"client_cert","client_cert.0"

  **Response:**

  .. code-block:: none

    OK

Erase all keys in a namespace:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=0,"client_cert"

  **Response:**

  .. code-block:: none

    OK

You can use the same commands to erase client_key and client_ca data.

.. _sysmfg-ssl-server:

SSL Server Certificates
************************

The server_cert, server_key, and server_ca namespaces store the SSL certificates and keys required for the device to act as an SSL server. These three namespaces can work together to enable secure server-side SSL connections (one-way or mutual authentication).

Query Certificate Information
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. To read all keys in the server certificate namespace:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"server_cert"

  **Response:**

  .. code-block:: none

    +SYSMFG:"server_cert","server_cert.0",8
    +SYSMFG:"server_cert","server_cert.1",8

    OK

  Similarly, you can query server_key and server_ca namespaces:

  .. code-block:: none

    AT+SYSMFG=1,"server_key"
    AT+SYSMFG=1,"server_ca"

2. Read a specific server certificate or key:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"server_cert","server_cert.0"

  **Response:**

  .. code-block:: none

    +SYSMFG:"server_cert","server_cert.0",8,<length>,<value>

    OK

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"server_key","server_key.0"

  **Response:**

  .. code-block:: none

    +SYSMFG:"server_key","server_key.0",8,<length>,<value>

    OK

Configure Certificates
^^^^^^^^^^^^^^^^^^^^^^^

1. Write server certificate:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"server_cert","server_cert.0",8,<cert_length>

  **Response:**

  .. code-block:: none

    OK

    >

  After ``>`` is returned, send the certificate data.

  **Response after data transmission:**

  .. code-block:: none

    OK

2. Write server key:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"server_key","server_key.0",8,<key_length>

  **Response:**

  .. code-block:: none

    OK

    >

  After ``>`` is returned, send the key data.

  **Response after data transmission:**

  .. code-block:: none

    OK

3. Write CA certificate:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"server_ca","server_ca.0",8,<ca_length>

  **Response:**

  .. code-block:: none

    OK

    >

  After ``>`` is returned, send the CA certificate data.

  **Response after data transmission:**

  .. code-block:: none

    OK

Use Certificates
^^^^^^^^^^^^^^^^^

After writing the server certificate, key (and server CA certificate for mutual authentication) to the manufacturing NVS partition, you can create an SSL server based on these certificates. Here is a complete example:

1. Ensure server credentials are written to the manufacturing partition:

   - Server certificate in ``server_cert`` namespace
   - Server key in ``server_key`` namespace
   - Server CA certificate in ``server_ca`` namespace (optional, only needed if client authentication is required)

2. Enable multiple connections mode:

   **Command:**

   .. code-block:: none

     AT+CIPMUX=1

   **Response:**

   .. code-block:: none

     OK

3. Start the SSL server:

   - For one-way authentication (server provides certificate to client):

     **Command:**

     .. code-block:: none

       AT+CIPSERVER=1,8883,"SSL"

     **Response:**

     .. code-block:: none

       OK

   - For mutual authentication (server and client verify each other's certificates):

     **Command:**

     .. code-block:: none

       AT+CIPSERVER=1,8883,"SSL",1

     **Response:**

     .. code-block:: none

       OK

     The last parameter ``1`` enables CA verification, and the server will verify client certificates using the CA certificate from the manufacturing partition.

   The device is now listening for SSL connections on port 8883. The SSL server will automatically use the server certificate and key from the manufacturing partition.

4. When a client connects, the SSL handshake will use the server certificate and key from the manufacturing partition for authentication.

.. note::

  - The SSL server automatically reads certificates and keys from the ``server_cert`` and ``server_key`` namespaces in the manufacturing partition without additional configuration.
  - If CA verification is enabled in the :ref:`AT+CIPSERVER <cmd-SERVER>` command (``<CA enable>`` = 1), the server will also read the CA certificate from the ``server_ca`` namespace to verify client certificates, enabling mutual authentication.
  - For more details about SSL server configuration, please refer to the :ref:`AT+CIPSERVER <cmd-SERVER>` command documentation.

Erase Certificates
^^^^^^^^^^^^^^^^^^^

Erase a specific server certificate:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=0,"server_cert","server_cert.0"

  **Response:**

  .. code-block:: none

    OK

Erase all keys in a namespace:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=0,"server_cert"

  **Response:**

  .. code-block:: none

    OK

You can use the same commands to erase server_key and server_ca data.

.. only:: not esp32s2

  .. _sysmfg-gatts:

  GATTS Configuration
  -------------------

  The ble_data namespace stores Bluetooth Low Energy GATTS (Generic Attribute Profile Server) service configurations. This namespace contains individual GATT service configuration items organized as ``cfg0``, ``cfg1``, ``cfg2``, etc., where each configuration item represents one attribute (service, characteristic, or descriptor) in the GATT table. For more details, please refer to :doc:`Bluetooth LE service source file introduction <../Compile_and_Develop/How_to_customize_BLE_services>`.

  .. important::

    - When modifying the ``ble_data`` namespace with the ``AT+SYSMFG`` command, it must be executed before BLE is initialized. If BLE is already initialized, run ``AT+BLEINIT=0`` to disable BLE first.
    - Before using ``AT+SYSMFG`` to read or write the ``ble_data`` namespace, please read the :doc:`Bluetooth LE service source file introduction <../Compile_and_Develop/How_to_customize_BLE_services>` document to understand the structure and format of Bluetooth LE service configurations.

  Query Configuration Information
  ********************************

  Read Configuration Items
  ^^^^^^^^^^^^^^^^^^^^^^^^^

  1. To read all configuration items in the ble_data namespace:

    **Command:**

    .. code-block:: none

      AT+SYSMFG=1,"ble_data"

    **Response:**

    .. code-block:: none

      +SYSMFG:"ble_data","cfg0",7
      +SYSMFG:"ble_data","cfg1",7
      +SYSMFG:"ble_data","cfg2",7
      +SYSMFG:"ble_data","cfg3",7
      ...
      +SYSMFG:"ble_data","cfg30",7

      OK

  2. Read a specific BLE configuration item (e.g., ``cfg2`` represents one row in the GATT service table):

    **Command:**

    .. code-block:: none

      AT+SYSMFG=1,"ble_data","cfg2"

    **Response:**

    .. code-block:: none

      +SYSMFG:"ble_data","cfg2",7,23,2,16,0xC300,0x01,1,1,30

      OK

    **Response Format:**

    The response data represents the following fields from the GATT table:

    - ``2``: index
    - ``16``: uuid_len (UUID length in bits)
    - ``0xC300``: uuid
    - ``0x01``: perm (permission)
    - ``1``: val_max_len (maximum value length)
    - ``1``: val_cur_len (current value length)
    - ``30``: value

  Configure Services
  **********************

  Modify BLE Characteristic UUID
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  This example shows how to modify a characteristic's UUID from ``0xC300`` to ``0xC500`` at runtime.

  1. Query the current configuration:

     **Command:**

     .. code-block:: none

       AT+SYSMFG=1,"ble_data","cfg2"

     **Response:**

     .. code-block:: none

       +SYSMFG:"ble_data","cfg2",7,23,2,16,0xC300,0x01,1,1,30

       OK

     The current UUID is ``0xC300``.

  2. Write the new configuration with UUID ``0xC500``:

     **Command:**

     .. code-block:: none

       AT+SYSMFG=2,"ble_data","cfg2",7,23

     **Response:**

     .. code-block:: none

       OK
     
       >

     **Send the data:**

     .. code-block:: none

       2,16,0xC500,0x01,1,1,30

     **Response:**

     .. code-block:: none

       OK

  3. Query the configuration item to verify the UUID modification:

     **Command:**

     .. code-block:: none

       AT+SYSMFG=1,"ble_data","cfg2"

     **Response:**

     .. code-block:: none

       +SYSMFG:"ble_data","cfg2",7,23,2,16,0xC500,0x01,1,1,30

       OK

     The UUID has been successfully changed to ``0xC500``.

  4. Re-initialize Bluetooth LE and create services to apply changes:

     **Command:**

     .. code-block:: none

       AT+BLEINIT=2

     **Response:**

     .. code-block:: none

       OK

     .. only:: esp32 or esp32c3

        **Command:**

        .. code-block:: none

          AT+BLEGATTSSRVCRE

        **Response:**

        .. code-block:: none

          OK

        **Command:**

        .. code-block:: none

          AT+BLEGATTSSRVSTART

        **Response:**

        .. code-block:: none

          OK

  5. Query services to verify:

     **Command:**

     .. code-block:: none

       AT+BLEGATTSSRV?

     **Response:**

     .. code-block:: none

       +BLEGATTSSRV:1,1,0xA002,1
       +BLEGATTSSRV:1,2,0xA003,1

       OK

  6. Query characteristics to verify the UUID change:

     **Command:**

     .. code-block:: none

       AT+BLEGATTSCHAR?

     **Response:**

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC500
       +BLEGATTSCHAR:"char",1,2,0xC301
       +BLEGATTSCHAR:"char",1,3,0xC302
       +BLEGATTSCHAR:"char",1,4,0xC303
       +BLEGATTSCHAR:"char",1,5,0xC304
       +BLEGATTSCHAR:"char",1,6,0xC305
       +BLEGATTSCHAR:"char",1,7,0xC306
       +BLEGATTSCHAR:"char",1,8,0xC307
       +BLEGATTSCHAR:"char",2,1,0xC400
       +BLEGATTSCHAR:"char",2,2,0xC401

       OK

     The first characteristic now shows UUID ``0xC500`` instead of ``0xC300``.

  Add New Custom BLE Service
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  This example demonstrates how to add a series of new BLE services at runtime without modifying existing service configurations. Let's assume we want to add a custom service (UUID: 0xA100) with one readable and writable characteristic (UUID: 0xB100).

  1. Query the current last configuration item index:

     **Command:**

     .. code-block:: none

       AT+SYSMFG=1,"ble_data"

     **Response:**

     .. code-block:: none

       +SYSMFG:"ble_data","cfg0",7
       +SYSMFG:"ble_data","cfg1",7
       ...
       +SYSMFG:"ble_data","cfg30",7

       OK

     Assuming the current last configuration item is ``cfg30``, we will start adding new services from ``cfg31``.

  2. Add Primary Service Declaration:

     **Command:**

     .. code-block:: none

       AT+SYSMFG=2,"ble_data","cfg31",7,26

     **Response:**

     .. code-block:: none

       OK

       >

     **Send data:**

     .. code-block:: none

       31,16,0x2800,0x01,2,2,A100

     **Data format explanation:**

     - ``31``: Configuration item index
     - ``16``: UUID length (16 bits)
     - ``0x2800``: Primary Service Declaration UUID
     - ``0x01``: Permission (readable)
     - ``2``: Maximum value length (2 bytes for service UUID)
     - ``2``: Current value length
     - ``A100``: Custom service UUID

     **Response:**

     .. code-block:: none

       OK

  3. Add Characteristic Declaration:

     **Command:**

     .. code-block:: none

       AT+SYSMFG=2,"ble_data","cfg32",7,24

     **Response:**

     .. code-block:: none

       OK

       >

     **Send data:**

     .. code-block:: none

       32,16,0x2803,0x01,3,1,02

     **Data format explanation:**

     - ``32``: Configuration item index
     - ``16``: UUID length
     - ``0x2803``: Characteristic Declaration
     - ``0x01``: Permission (readable)
     - ``3``: Maximum value length
     - ``1``: Current value length
     - ``02``: Set permission to read

     **Response:**

     .. code-block:: none

       OK

  4. Add Characteristic Value:

     **Command:**

     .. code-block:: none

       AT+SYSMFG=2,"ble_data","cfg33",7,31

     **Response:**

     .. code-block:: none

       OK

       >

     **Send data:**

     .. code-block:: none

       33,16,0xB100,0x11,20,8,12345678

     **Data format explanation:**

     - ``33``: Configuration item index
     - ``16``: UUID length
     - ``0xB100``: Characteristic value UUID (matches the UUID in characteristic declaration)
     - ``0x11``: Permission (read + write: ESP_GATT_PERM_READ (0x01) + ESP_GATT_PERM_WRITE (0x10) = 0x11)
     - ``20``: Maximum value length (allows up to 20 bytes of data)
     - ``8``: Current value length (initial value is 8 bytes)
     - ``12345678``: Initial value (8 bytes of data)

     **Response:**

     .. code-block:: none

       OK

  5. Query the newly added configuration items to verify:

     **Command:**

     .. code-block:: none

       AT+SYSMFG=1,"ble_data","cfg31"

     **Response:**

     .. code-block:: none

       +SYSMFG:"ble_data","cfg31",7,26,31,16,0x2800,0x01,2,2,A100

       OK

     **Command:**

     .. code-block:: none

       AT+SYSMFG=1,"ble_data","cfg32"

     **Response:**

     .. code-block:: none

       +SYSMFG:"ble_data","cfg32",7,24,32,16,0x2803,0x01,3,1,02

       OK

     **Command:**

     .. code-block:: none

       AT+SYSMFG=1,"ble_data","cfg33"

     **Response:**

     .. code-block:: none

       +SYSMFG:"ble_data","cfg33",7,31,33,16,0xB100,0x11,20,8,12345678

       OK

  6. Re-initialize Bluetooth LE and create services to apply changes:

     **Command:**

     .. code-block:: none

       AT+BLEINIT=2

     **Response:**

     .. code-block:: none

       OK

     .. only:: esp32 or esp32c3

        **Command:**

        .. code-block:: none

          AT+BLEGATTSSRVCRE

        **Response:**

        .. code-block:: none

          OK

        **Command:**

        .. code-block:: none

          AT+BLEGATTSSRVSTART

        **Response:**

        .. code-block:: none

          OK

  7. Query services to verify the new service has been added:

     **Command:**

     .. code-block:: none

       AT+BLEGATTSSRV?

     **Response:**

     .. code-block:: none

       +BLEGATTSSRV:1,1,0xA002,1
       +BLEGATTSSRV:1,2,0xA003,1
       +BLEGATTSSRV:1,3,0xA100,1

       OK

     The newly added service UUID ``0xA100`` now appears in the service list.

  8. Query characteristics to verify the new characteristic has been added:

     **Command:**

     .. code-block:: none

       AT+BLEGATTSCHAR?

     **Response:**

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300
       +BLEGATTSCHAR:"char",1,2,0xC301
       ...
       +BLEGATTSCHAR:"char",3,1,0xB100

       OK

     The newly added characteristic UUID ``0xB100`` now appears in the characteristic list (belongs to service 3, characteristic index 1).

  .. note::

    **BLE Service Configuration Points:**

    - When adding new services, ensure the ``cfg`` index used does not conflict with existing configurations
    - A complete BLE service requires at least three configuration items: primary service declaration, characteristic declaration, and characteristic value
    - If characteristic descriptors are needed (such as Client Characteristic Configuration Descriptor), add additional configuration items
    - Characteristic property values can be combined. Common properties include: 0x01 (broadcast), 0x02 (read), 0x04 (write without response), 0x08 (write), 0x10 (notify), 0x20 (indicate), 0x40 (authenticated signed writes), 0x80 (extended properties)

    **Data Format and Activation:**

    .. list::

      - Each ``cfg`` item corresponds to one row in the GATT service table defined in the gatts_data.csv file
      - Data format: ``index,uuid_len,uuid,perm,val_max_len,val_cur_len,value``
      :esp32 or esp32c3: - Configurations written via ``AT+SYSMFG`` take effect after re-initializing BLE (``AT+BLEINIT``) and recreating services (``AT+BLEGATTSSRVCRE``)
      :esp32c2 or esp32c5 or esp32c6 or esp32c61: - Configurations written via ``AT+SYSMFG`` take effect after re-initializing BLE (``AT+BLEINIT``)
      - For more details on GATT table structure, see :doc:`../Compile_and_Develop/How_to_customize_BLE_services`

  .. _sysmfg-custom-data:

.. only:: esp32s2

  .. _sysmfg-custom-data:

Custom Data Storage
-------------------

In addition to the predefined namespaces (such as ``factory_param``, ``client_cert``, ``server_cert``, etc.), you can create custom namespaces to store your own application data. This is useful for storing small amounts of persistent data that need to survive reboots, such as custom configuration, user preferences, or device-specific settings.

Create and Use Custom Namespace
********************************

This example demonstrates how to create a custom namespace called ``my_app_ca`` to store application-specific certificates or configuration data.

1. Create a new namespace and write data:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"my_app_ca","my_app_ca.0",8,100

  **Response:**

  .. code-block:: none

    OK

    >

  After ``>`` is returned, send your binary data (100 bytes). After the data is sent, the system returns:

  .. code-block:: none

    OK

  The namespace ``my_app_ca`` is now created with a key ``my_app_ca.0`` containing your data.

2. Query the custom namespace:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"my_app_ca"

  **Response:**

  .. code-block:: none

    +SYSMFG:"my_app_ca","my_app_ca.0",8

    OK

3. Read the data from your custom namespace:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"my_app_ca","my_app_ca.0"

  **Response:**

  .. code-block:: none

    +SYSMFG:"my_app_ca","my_app_ca.0",8,100,<data>

    OK

  The response includes the data type (8 for binary), length (100 bytes), and the actual data.

4. Add more keys to the same namespace:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"my_app_ca","my_app_ca.1",7,20

  **Response:**

  .. code-block:: none

    OK

    >

  **Send data:**

  .. code-block:: none

    MyCustomConfigValue1

  **Response:**

  .. code-block:: none

    OK

5. Query all keys in the namespace:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"my_app_ca"

  **Response:**

  .. code-block:: none

    +SYSMFG:"my_app_ca","my_app_ca.0",8
    +SYSMFG:"my_app_ca","my_app_ca.1",7

    OK

6. Create another namespace for application configuration parameters:

  If you need to store configuration parameters (rather than certificate data), it's recommended to create a separate namespace to maintain semantic clarity.

  **Command:**

  .. code-block:: none

    AT+SYSMFG=2,"my_app_config","timeout",6,5000

  **Response:**

  .. code-block:: none

    OK

  This stores an i32 configuration value (type 6) in the ``my_app_config`` namespace with the key name "timeout" and value 5000.

7. Read the configuration parameter:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=1,"my_app_config","timeout"

  **Response:**

  .. code-block:: none

    +SYSMFG:"my_app_config","timeout",6,4,5000

    OK

8. Erase a specific key:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=0,"my_app_ca","my_app_ca.1"

  **Response:**

  .. code-block:: none

    OK

9. Erase the entire namespace:

  **Command:**

  .. code-block:: none

    AT+SYSMFG=0,"my_app_ca"

  **Response:**

  .. code-block:: none

    OK

.. note::

  **Custom Namespace Best Practices:**

  - Custom namespaces can be used to store any type of persistent data
  - Choose the appropriate data type based on your needs: numeric types (1-6) for numbers, string (7) for text, binary (8) for certificates or binary data
  - **Separate namespaces by purpose**: For example, store certificate data (``my_app_ca``) separately from configuration parameters (``my_app_config``) for easier management and maintenance
  - Namespace names should be descriptive and clearly indicate their purpose (e.g., ``my_app_certs``, ``my_app_settings``)
  - Key names can be any string, but it's recommended to use meaningful names for easier identification
  - All custom data persists across reboots until explicitly erased
  - Custom namespaces are independent of system namespaces and do not affect AT firmware functionality
  - After creating a custom namespace and storing data, you can query it using ``AT+SYSMFG?`` to see it in the namespace list

