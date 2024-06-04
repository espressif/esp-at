at.py Tool
=================

{IDF_TARGET_VER: default="undefined", esp32="5.0", esp32c2="5.0", esp32c3="5.0", esp32c6="5.1", esp32s2="5.0"}

:link_to_translation:`zh_CN:[中文]`

The ``at.py`` tool is used to modify various parameter configurations in the official release firmware, GitHub temporary firmware, and 2MB/4MB firmware of ESP-AT. These configurations include Wi-Fi configurations, certificate and key configurations, UART configurations, GATTS configurations, and more. When the default firmware does not meet your requirements, you can use the ``at.py`` tool to modify these parameter configurations in the firmware.

.. _esp-at-py-steps:

Detailed Steps
--------------

Please follow the detailed steps below to download the ``at.py`` tool, modify the configurations in the firmware, and flash the firmware. **It is recommended to use a Linux system for development**.

* :ref:`esp-at-py-prerequisites`
* :ref:`esp-at-py-download`
* :ref:`esp-at-py-usage`
* :ref:`esp-at-py-modify-bin`
* :ref:`esp-at-firmware-download`

.. _esp-at-py-prerequisites:

Step 1: Install Python
----------------------

If you have already installed Python on your local machine, you can skip this step. Otherwise, please download and install Python according to the official `Python documentation <https://www.python.org/downloads/>`_. **It is recommended to use Python 3.8.0 or later versions**.

After the installation is complete, you can open a command line and enter ``python --version`` to check the Python version.

.. _esp-at-py-download:

Step 2: Download at.py
----------------------

Visit the :project_file:`at.py <tools/at.py>` webpage, click the "Download raw file" button to download the at.py file to your local machine.

.. figure:: ../../_static/compile_and_develop/at-py-download.png
  :align: center
  :alt: Download at.py File
  :figclass: align-center

  Download at.py File

.. _esp-at-py-usage:

Step 3: Use at.py
-----------------

Currently, ``at.py`` supports modifying parameter configurations in the firmware. To view the supported usage and instructions, enter ``python at.py modify_bin --help`` in the command line for more details.

.. _esp-at-py-modify-bin:

Step 4: Examples: Modify Firmware Configurations with at.py
-----------------------------------------------------------

.. list::

  * :ref:`at-py-modify-wifi`
  * :ref:`at-py-modify-pki`
  * :ref:`at-py-modify-uart`
  :not esp32s2: * :ref:`at-py-modify-gatts`

.. note::

  - You can modify the parameter configurations of multiple functions at one go. For example, you can modify both Wi-Fi configuration and certificate and key configuration at the same time.
  - The script does not check the validity of the modified parameter configurations. Please ensure that the input configurations are valid.
  - The modified parameter configurations will have corresponding records in the ``mfg_nvs.csv`` file under the current mfg_nvs directory.

.. _at-py-modify-wifi:

Modify Wi-Fi Configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^

The currently modifiable Wi-Fi parameter configurations are shown in the following table:

.. list-table::
  :header-rows: 1
  :widths: 50 60 100

  * - Parameter
    - Function
    - Description
  * - \--tx_power
    - Maximum transmission power of Wi-Fi
    - See `{IDF_TARGET_NAME} Maximum transmission power <https://docs.espressif.com/projects/esp-idf/en/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-reference/network/esp_wifi.html#_CPPv425esp_wifi_set_max_tx_power6int8_t>`_ for more details.
  * - \--country_code
    - Wi-Fi country code
    - See the ``cc`` field of `Wi-Fi Country Code <https://docs.espressif.com/projects/esp-idf/en/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#wi-fi-country-code>`_
  * - \--start_channel
    - Wi-Fi starting channel
    - See the ``schan`` field of `Wi-Fi Country Code <https://docs.espressif.com/projects/esp-idf/en/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#wi-fi-country-code>`_
  * - \--channel_number
    - Total number of Wi-Fi channels
    - See the ``nchan`` field of `Wi-Fi Country Code <https://docs.espressif.com/projects/esp-idf/en/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#wi-fi-country-code>`_

For example, you can use the following command to modify the maximum transmission power of Wi-Fi to 18 dBm, set the country code to US, starting channel to 1, and total channel number to 11:

.. code-block:: none

  python at.py modify_bin --tx_power 72 --country_code "US" --start_channel 1 --channel_number 11 --input factory_XXX.bin

- **\--tx_power 72**: The unit is 0.25 dBm, and 72 represents 18 dBm.
- **\--input factory_XXX.bin**: The input firmware file.

.. _at-py-modify-pki:

Modify Certificate and Key Configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The currently modifiable certificate and key configurations are shown in the following table:

.. list-table::
  :header-rows: 1
  :widths: 50 60 70

  * - Parameter
    - Function
    - Original File
  * - \--server_ca
    - CA certificate for TLS server
    - :component_file:`server_ca.crt <customized_partitions/raw_data/server_ca/server_ca.crt>`
  * - \--server_cert
    - Certificate for TLS server
    - :component_file:`server_cert.crt <customized_partitions/raw_data/server_cert/server_cert.crt>`
  * - \--server_key
    - Key for TLS server
    - :component_file:`server.key <customized_partitions/raw_data/server_key/server.key>`
  * - \--client_ca0
    - CA certificate for client 0
    - :component_file:`client_ca_00.crt <customized_partitions/raw_data/client_ca/client_ca_00.crt>`
  * - \--client_cert0
    - Certificate for client 0
    - :component_file:`client_cert_00.crt <customized_partitions/raw_data/client_cert/client_cert_00.crt>`
  * - \--client_key0
    - Key for client 0
    - :component_file:`client_key_00.key <customized_partitions/raw_data/client_key/client_key_00.key>`
  * - \--client_ca1
    - CA certificate for client 1
    - :component_file:`client_ca_01.crt <customized_partitions/raw_data/client_ca/client_ca_01.crt>`
  * - \--client_cert1
    - Certificate for client 1
    - :component_file:`client_cert_01.crt <customized_partitions/raw_data/client_cert/client_cert_01.crt>`
  * - \--client_key1
    - Key for client 1
    - :component_file:`client_key_01.key <customized_partitions/raw_data/client_key/client_key_01.key>`
  * - \--mqtt_ca
    - CA certificate for MQTT client
    - :component_file:`mqtt_ca.crt <customized_partitions/raw_data/mqtt_ca/mqtt_ca.crt>`
  * - \--mqtt_cert
    - Certificate for MQTT client
    - :component_file:`mqtt_client.crt <customized_partitions/raw_data/mqtt_cert/mqtt_client.crt>`
  * - \--mqtt_key
    - Key for MQTT client
    - :component_file:`mqtt_client.key <customized_partitions/raw_data/mqtt_key/mqtt_client.key>`
  * - \--wpa2_ca
    - CA certificate for WPA2-Enterprise client
    - :component_file:`wpa2_ca.pem <customized_partitions/raw_data/wpa2_ca/wpa2_ca.pem>`
  * - \--wpa2_cert
    - Certificate for WPA2-Enterprise client
    - :component_file:`wpa2_client.crt <customized_partitions/raw_data/wpa2_cert/wpa2_client.crt>`
  * - \--wpa2_key
    - Key for WPA2-Enterprise client
    - :component_file:`wpa2_client.key <customized_partitions/raw_data/wpa2_key/wpa2_client.key>`

For example, you can use the following command to modify the CA certificate, certificate, and key for the MQTT client:

.. code-block:: none

  python at.py modify_bin --mqtt_ca mqtt/mqtt_ca.crt --mqtt_cert mqtt/mqtt.crt --mqtt_key mqtt/mqtt.key --input factory_XXX.bin

- **\--input factory_XXX.bin**: The input firmware file.

.. _at-py-modify-uart:

Modify UART Configuration
^^^^^^^^^^^^^^^^^^^^^^^^^

The modifiable UART configuration includes only the UART configuration for the :term:`AT command port`. The configurable parameter configurations are shown in the following table:

.. list-table::
  :header-rows: 1
  :widths: 40 60 100

  * - Parameter
    - Function
    - Description
  * - \--uart_num
    - UART number for the AT command port
    - Only needs to be modified if the AT command port is also used as the AT log port. Ensure that the ``tx_pin`` and ``rx_pin`` below have the same pins as the :term:`AT log port`.
  * - \--baud
    - Baud rate of the AT command port
    - Original value: 115200
  * - \--tx_pin
    - TX pin of the AT command port
    - Please ensure that the set pin is not used by other functions.
  * - \--rx_pin
    - RX pin of the AT command port
    - Please ensure that the set pin is not used by other functions.
  * - \--cts_pin
    - CTS pin of the AT command port
    - Please ensure that the set pin is not used by other functions. If flow control is not used, set this parameter to -1.
  * - \--rts_pin
    - RTS pin of the AT command port
    - Please ensure that the set pin is not used by other functions. If flow control is not used, set this parameter to -1.

For example, you can use the following command to modify the baud rate to 921600, set the TX pin to GPIO17, the RX pin to GPIO16, and disable flow control for the AT command port:

.. code-block:: none

  python at.py modify_bin --baud 921600 --tx_pin 17 --rx_pin 16 --cts_pin -1 --rts_pin -1 --input factory_XXX.bin

- **\--input factory_XXX.bin**: The input firmware file.

.. only:: not esp32s2

  .. _at-py-modify-gatts:

  Modify GATTS Configuration
  ^^^^^^^^^^^^^^^^^^^^^^^^^^

  Before making modifications, please read the :doc:`How to Customize Bluetooth® LE Services <How_to_customize_BLE_services>` document to understand the meaning of each field in the GATTS configuration file :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>`.

  The currently modifiable GATTS configurations are shown in the following table:

  .. list-table::
    :header-rows: 1
    :widths: 20 60

    * - Parameter
      - Function
    * - \--gatts_cfg0
      - Update the row with index 0 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg1
      - Update the row with index 1 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg2
      - Update the row with index 2 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg3
      - Update the row with index 3 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg4
      - Update the row with index 4 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg5
      - Update the row with index 5 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg6
      - Update the row with index 6 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg7
      - Update the row with index 7 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg8
      - Update the row with index 8 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg9
      - Update the row with index 9 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg10
      - Update the row with index 10 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg11
      - Update the row with index 11 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg12
      - Update the row with index 12 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg13
      - Update the row with index 13 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg14
      - Update the row with index 14 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg15
      - Update the row with index 15 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg16
      - Update the row with index 16 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg17
      - Update the row with index 17 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg18
      - Update the row with index 18 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg19
      - Update the row with index 19 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg20
      - Update the row with index 20 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg21
      - Update the row with index 21 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg22
      - Update the row with index 22 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg23
      - Update the row with index 23 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg24
      - Update the row with index 24 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg25
      - Update the row with index 25 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg26
      - Update the row with index 26 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg27
      - Update the row with index 27 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg28
      - Update the row with index 28 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg29
      - Update the row with index 29 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file
    * - \--gatts_cfg30
      - Update the row with index 30 in the :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` file

  For example, you can use the following command to modify the "perm" permission of the row with index 0:

  .. code-block:: none

    python at.py modify_bin --gatts_cfg0 "0,16,0x2800,0x011,2,2,A002" --input factory_XXX.bin

  - **\--input factory_XXX.bin**: The input firmware file.

  .. // The following section is a temporary workaround and it cannot be avoided. Refer to https://docs.espressif.com/projects/esp-docs/en/latest/writing-documentation/writing-for-multiple-targets.html#target-specific-paragraph for more information.

  .. _esp-at-firmware-download:

  Step 5: Flash onto the Device
  -----------------------------

  .. attention::
    **The AT firmware modified by at.py needs to be tested and verified for functionality based on your own product.**

    **Please save the firmware before and after modification, and the download link**, for possible issue debugging in the future.

  Please follow the :ref:`Flash firmware <flash-at-firmware-into-your-device>` to complete it.

.. only:: esp32s2

  .. _esp-at-firmware-download:

  Step 5: Flash onto the Device
  -----------------------------

  .. attention::
    **The AT firmware modified by at.py needs to be tested and verified for functionality based on your own product.**

    **Please save the firmware before and after modification, and the download link**, for possible issue debugging in the future.

  Please follow the :ref:`Flash firmware <flash-at-firmware-into-your-device>` to complete it.
