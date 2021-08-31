.. include:: ../../inline_substitutions

ESP-AT Firmware Differences
===========================

This document compares the differences among AT firmwares of a certain ESP series in terms of the supported commands set, hardware, and supported modules.

ESP32 Series
------------

This section describes the differences among AT firmwares of ESP32 series, including

- ESP32-WROOM-32_AT_Bin (referred to as **WROOM Bin** in this section);
- ESP32-WROVER-32_AT_Bin (referred to as **WROOM Bin** in this section);
- ESP32-PICO-D4_AT_Bin (referred to as **PICO-D4 Bin** in this section);
- ESP32-SOLO-1_AT_Bin (referred to as **SOLO-1 Bin** in this section);
- ESP32-MINI-1_AT_Bin (referred to as **MINI-1 Bin** in this section);
- ESP32-D2WD_AT_Bin (referred to as **D2WD Bin** in this section);
- ESP32-QLOUD_AT_Bin (referred to as **QCLOUD Bin** in this section).

Supported Command Set
^^^^^^^^^^^^^^^^^^^^^

The table lists which command set is supported in the official default ESP32 series of AT firmware (marked with |icon-check|), and which is not supported but can be supported after configuration and compilation of the ESP-AT project (marked with |icon-orange-circle|). Note that the command set that is not shown in this table is not supported at all.

.. list-table::
   :header-rows: 1

   * - Command Set
     - WROOM Bin
     - WROVER Bin
     - PICO-D4 Bin
     - SOLO-1 Bin
     - MINI-1 Bin
     - D2WD Bin
     - QCLOUD Bin
   * - base
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - user
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - wifi
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - net
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - MDNS
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - WPS
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - smartconfig
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - ping
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - MQTT
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - http
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - ble
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - ble hid
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
   * - blufi
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - bt spp
     - |icon-orange-circle|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - bt a2dp
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - ethernet
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - FS
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - driver
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - WPA2
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - WEB
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - OTA
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-check|
   * - qcloud IoT
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|

Hardware Differences
^^^^^^^^^^^^^^^^^^^^

.. list-table::
   :header-rows: 1

   * - Hardware
     - WROOM Bin
     - WROVER Bin
     - PICO-D4 Bin
     - SOLO-1 Bin
     - MINI-1 Bin
     - D2WD Bin
     - QCLOUD Bin
   * - Flash
     - 4 MB
     - 4 MB
     - 4 MB
     - 4 MB
     - 4 MB
     - 2 MB
     - 4 MB
   * - PSRAM
     - |icon-orange-circle|
     - 8 MB
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - UART Pins [#one]_
     - | TX: 17
       | RX: 16
       | CTS: 15
       | RTS: 14
     - | TX: 22 
       | RX: 19 
       | CTS: 15
       | RTS: 14
     - | TX: 22 
       | RX: 19 
       | CTS: 15
       | RTS: 14
     - | TX: 17 
       | RX: 16 
       | CTS: 15 
       | RTS: 14
     - | TX: 22 
       | RX: 19 
       | CTS: 15
       | RTS: 14
     - | TX: 22 
       | RX: 19 
       | CTS: 15
       | RTS: 14
     - | TX: 17 
       | RX: 16 
       | CTS: 15 
       | RTS: 14

.. [#one] UART pins can be customized. See :doc:`How_to_set_AT_port_pin` for details.

Supported Modules
^^^^^^^^^^^^^^^^^

The table lists the ESP modules or chips that each AT firmware supports.

.. list-table::
   :header-rows: 1

   * - Module/Chip
     - WROOM Bin
     - WROVER Bin
     - PICO-D4 Bin
     - SOLO-1 Bin
     - MINI-1 Bin
     - D2WD Bin
     - QCLOUD Bin
   * - ESP32-WROOM-32E
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
   * - ESP32-WROOM-32UE
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
   * - ESP32-WROOM-32D
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
   * - ESP32-WROOM-32U
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
   * - ESP32-WROOM-32
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
   * - ESP32-WROOM-32SE
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - ESP32-WROVER-E
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
   * - ESP32-WROVER-IE
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
   * - ESP32-WROVER-B
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
   * - ESP32-WROVER-IB
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
   * - ESP32-WROVER
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
   * - ESP32-WROVER-I
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
   * - ESP32-SOLO-1
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
   * - ESP32-D2WD
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-orange-circle|
   * - ESP32-MINI-1
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|
   * - ESP32-PICO-D4
     - |icon-orange-circle|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-orange-circle|
     - |icon-check|
     - |icon-check|
     - |icon-orange-circle|

ESP32-C3 Series
----------------

This section describes the differences among the ESP32-C3 Series of AT firmware, including

- ESP32-C3-MINI-1_AT_Bin (referred to as **MINI-1 Bin** in this section);
- ESP32-C3-QCLOUD_AT_BIN (referred to as **QCLOUD Bin** in this section).

Supported Command Set 
^^^^^^^^^^^^^^^^^^^^^

The table lists which command set is supported in the official default ESP32-C3 series of AT firmware (marked with |icon-check|), and which is not supported but can be supported after configuration and compilation of the ESP-AT project (marked with |icon-orange-circle|). Note that the command set that is not shown in this table is not supported at all.

.. list-table::
   :header-rows: 1

   * - Command Set
     - MINI-1 Bin
     - QCLOUD Bin
   * - base
     - |icon-check|
     - |icon-check|
   * - user
     - |icon-check|
     - |icon-check|
   * - wifi
     - |icon-check|
     - |icon-check|
   * - net
     - |icon-check|
     - |icon-check|
   * - MDNS
     - |icon-check|
     - |icon-check|
   * - WPS
     - |icon-check|
     - |icon-check|
   * - smartconfig
     - |icon-check|
     - |icon-check|
   * - ping
     - |icon-check|
     - |icon-check|
   * - MQTT
     - |icon-check|
     - |icon-check|
   * - http
     - |icon-check|
     - |icon-check|
   * - FS
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - driver
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - WPA2
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - WEB
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - OTA
     - |icon-check|
     - |icon-check|
   * - qcloud IoT
     - |icon-orange-circle|
     - |icon-check|

Hardware Differences
^^^^^^^^^^^^^^^^^^^^

.. list-table::
   :header-rows: 1

   * - Hardware
     - MINI-1
     - QCLOUD
   * - Flash
     - 4 MB
     - 4 MB
   * - PSRAM
     - |icon-orange-circle|
     - |icon-orange-circle|
   * - UART Pins [#two]_
     - | TX: 7
       | RX: 6
       | CTS: 5
       | RTS: 4
     - | TX: 7
       | RX: 6
       | CTS: 5
       | RTS: 4

.. [#two] UART pins can be customized. See :doc:`How_to_set_AT_port_pin` for details.

Firmware Supported Modules
^^^^^^^^^^^^^^^^^^^^^^^^^^

The table lists the ESP modules or chips that each AT firmware supports.

.. list-table::
   :header-rows: 1

   * - Module/Chip
     - MINI-1 Bin
     - QCLOUD Bin
   * - ESP32-C3-MINI-1
     - |icon-check|
     - |icon-check|