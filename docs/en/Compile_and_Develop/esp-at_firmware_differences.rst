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

The table lists which command set is supported in the official default ESP32 series of AT firmware (marked with ``✅``), and which is not supported but can be supported after configuration and compilation of the ESP-AT project (marked with ``❌``). Note that the command set that is not shown in this table is not supported at all.

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
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - user
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - wifi
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - net
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - MDNS
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - WPS
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - smartconfig
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - ping
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - MQTT
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - http
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - ble
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - ble hid
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
   * - blufi
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
   * - bt spp
     - ❌
     - ✅
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
   * - bt a2dp
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
   * - ethernet
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
   * - FS
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
   * - driver
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
   * - WPA2
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
   * - WEB
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
   * - OTA
     - ✅
     - ✅
     - ✅
     - ✅
     - ✅
     - ❌
     - ✅
   * - qcloud IoT
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ✅

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
     - ❌
     - 8 MB
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
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
     - ✅
     - ❌
     - ❌
     - ✅
     - ❌
     - ❌
     - ✅
   * - ESP32-WROOM-32UE
     - ✅
     - ❌
     - ❌
     - ✅
     - ❌
     - ❌
     - ✅
   * - ESP32-WROOM-32D
     - ✅
     - ❌
     - ❌
     - ✅
     - ❌
     - ❌
     - ✅
   * - ESP32-WROOM-32U
     - ✅
     - ❌
     - ❌
     - ✅
     - ❌
     - ❌
     - ✅
   * - ESP32-WROOM-32
     - ✅
     - ❌
     - ❌
     - ✅
     - ❌
     - ❌
     - ✅
   * - ESP32-WROOM-32SE
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
   * - ESP32-WROVER-E
     - ❌
     - ✅
     - ✅
     - ❌
     - ✅
     - ✅
     - ❌
   * - ESP32-WROVER-IE
     - ❌
     - ✅
     - ✅
     - ❌
     - ✅
     - ✅
     - ❌
   * - ESP32-WROVER-B
     - ❌
     - ✅
     - ✅
     - ❌
     - ✅
     - ✅
     - ❌
   * - ESP32-WROVER-IB
     - ❌
     - ✅
     - ✅
     - ❌
     - ✅
     - ✅
     - ❌
   * - ESP32-WROVER
     - ❌
     - ✅
     - ✅
     - ❌
     - ✅
     - ✅
     - ❌
   * - ESP32-WROVER-I
     - ❌
     - ✅
     - ✅
     - ❌
     - ✅
     - ✅
     - ❌
   * - ESP32-SOLO-1
     - ✅
     - ❌
     - ❌
     - ✅
     - ❌
     - ❌
     - ✅
   * - ESP32-D2WD
     - ❌
     - ❌
     - ❌
     - ❌
     - ❌
     - ✅
     - ❌
   * - ESP32-MINI-1
     - ❌
     - ❌
     - ✅
     - ❌
     - ✅
     - ✅
     - ❌
   * - ESP32-PICO-D4
     - ❌
     - ❌
     - ✅
     - ❌
     - ✅
     - ✅
     - ❌

ESP32-C3 Series
----------------

This section describes the differences among the ESP32-C3 Series of AT firmware, including

- ESP32-C3-MINI-1_AT_Bin (referred to as **MINI-1 Bin** in this section);
- ESP32-C3-QCLOUD_AT_BIN (referred to as **QCLOUD Bin** in this section).

Supported Command Set 
^^^^^^^^^^^^^^^^^^^^^

The table lists which command set is supported in the official default ESP32-C3 series of AT firmware (marked with ``✅``), and which is not supported but can be supported after configuration and compilation of the ESP-AT project (marked with ``❌``). Note that the command set that is not shown in this table is not supported at all.

.. list-table::
   :header-rows: 1

   * - Command Set
     - MINI-1 Bin
     - QCLOUD Bin
   * - base
     - ✅
     - ✅
   * - user
     - ✅
     - ✅
   * - wifi
     - ✅
     - ✅
   * - net
     - ✅
     - ✅
   * - MDNS
     - ✅
     - ✅
   * - WPS
     - ✅
     - ✅
   * - smartconfig
     - ✅
     - ✅
   * - ping
     - ✅
     - ✅
   * - MQTT
     - ✅
     - ✅
   * - http
     - ✅
     - ✅
   * - FS
     - ❌
     - ❌
   * - driver
     - ❌
     - ❌
   * - WPA2
     - ❌
     - ❌
   * - WEB
     - ❌
     - ❌
   * - OTA
     - ✅
     - ✅
   * - qcloud IoT
     - ❌
     - ✅

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
     - ❌
     - ❌
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
     - ✅
     - ✅