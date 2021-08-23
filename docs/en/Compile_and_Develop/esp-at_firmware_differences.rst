.. include:: ../../inline_substitutions

ESP-AT Firmware Differences
===========================

:link_to_translation:`zh_CN:[中文]` 

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

The table lists which command set is supported by default in the official AT firmware applicable to ESP32 series of modules (marked with |icon-green-check|), which is not supported by default but can be supported after configuration of the ESP-AT project (marked with |icon-orange-check|), and which is not supported at all (marked with |icon-red-cross|). Note that the command set that is not shown in this table is not supported either. Applicable firmware that has not been :doc:`officially released <../AT_Binary_Lists/index>` requires compilation by yourself. Those self-compiled firmware cannot be upgraded OTA from Espressif official server.

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
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - user
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - wifi
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - net
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - MDNS
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - WPS
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - smartconfig
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - ping
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - MQTT
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - http
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - ble
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - ble hid
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
   * - blufi
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
   * - bt spp
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
   * - bt a2dp
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
   * - ethernet
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
   * - FS
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
   * - driver
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
   * - WPA2
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
   * - WEB
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
   * - OTA
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-red-cross|
     - |icon-green-check|
   * - qcloud IoT
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|

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
     - |icon-orange-check|
     - 8 MB
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
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
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
   * - ESP32-WROOM-32UE
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
   * - ESP32-WROOM-32D
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
   * - ESP32-WROOM-32U
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
   * - ESP32-WROOM-32
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
   * - ESP32-WROOM-32SE
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
   * - ESP32-WROVER-E
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
   * - ESP32-WROVER-IE
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
   * - ESP32-WROVER-B
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
   * - ESP32-WROVER-IB
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
   * - ESP32-WROVER
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
   * - ESP32-WROVER-I
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
   * - ESP32-SOLO-1
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
   * - ESP32-D2WD
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-orange-check|
   * - ESP32-MINI-1
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|
   * - ESP32-PICO-D4
     - |icon-orange-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-orange-check|
     - |icon-green-check|
     - |icon-green-check|
     - |icon-orange-check|

ESP32-C3 Series
----------------

This section describes the differences among the ESP32-C3 Series of AT firmware, including

- ESP32-C3-MINI-1_AT_Bin (referred to as **MINI-1 Bin** in this section);
- ESP32-C3-QCLOUD_AT_BIN (referred to as **QCLOUD Bin** in this section).

Supported Command Set 
^^^^^^^^^^^^^^^^^^^^^

The table lists which command set is supported by default in the official AT firmware applicable to ESP32-C3 series of modules (marked with |icon-green-check|), which is not supported by default but can be supported after configuration of the ESP-AT project (marked with |icon-orange-check|), and which is not supported at all (marked with |icon-red-cross|). Note that the command set that is not shown in this table is not supported either. Applicable firmware that has not been :doc:`officially released <../AT_Binary_Lists/index>` requires compilation by yourself. Those self-compiled firmware cannot be upgraded OTA from Espressif official server.

.. list-table::
   :header-rows: 1

   * - Command Set
     - MINI-1 Bin
     - QCLOUD Bin
   * - base
     - |icon-green-check|
     - |icon-green-check|
   * - user
     - |icon-green-check|
     - |icon-green-check|
   * - wifi
     - |icon-green-check|
     - |icon-green-check|
   * - net
     - |icon-green-check|
     - |icon-green-check|
   * - MDNS
     - |icon-green-check|
     - |icon-green-check|
   * - WPS
     - |icon-green-check|
     - |icon-green-check|
   * - smartconfig
     - |icon-green-check|
     - |icon-green-check|
   * - ping
     - |icon-green-check|
     - |icon-green-check|
   * - MQTT
     - |icon-green-check|
     - |icon-green-check|
   * - http
     - |icon-green-check|
     - |icon-green-check|
   * - FS
     - |icon-orange-check|
     - |icon-orange-check|
   * - driver
     - |icon-orange-check|
     - |icon-orange-check|
   * - WPA2
     - |icon-orange-check|
     - |icon-orange-check|
   * - WEB
     - |icon-orange-check|
     - |icon-orange-check|
   * - OTA
     - |icon-green-check|
     - |icon-green-check|
   * - qcloud IoT
     - |icon-orange-check|
     - |icon-green-check|

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
     - |icon-orange-check|
     - |icon-orange-check|
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
     - |icon-green-check|
     - |icon-green-check|