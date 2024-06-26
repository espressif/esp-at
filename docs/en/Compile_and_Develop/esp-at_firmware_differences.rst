.. include:: ../../inline_substitutions

ESP-AT Firmware Differences
===========================

:link_to_translation:`zh_CN:[中文]` 

This document compares the differences among AT firmwares of a certain {IDF_TARGET_NAME} series in terms of the supported commands set, hardware, and supported modules.

{IDF_TARGET_NAME} Series
------------------------

This section describes the differences among AT firmwares of {IDF_TARGET_NAME} series, including

.. only:: esp32

  - ESP32-WROOM-32-AT-Vx.x.x.x.zip (referred to as **WROOM Bin** in this section);
  - ESP32-WROVER-32-AT-Vx.x.x.x.zip (referred to as **WROVER Bin** in this section);
  - ESP32-PICO-D4-AT-Vx.x.x.x.zip (referred to as **PICO-D4 Bin** in this section);
  - ESP32-SOLO-1-AT-Vx.x.x.x.zip (referred to as **SOLO-1 Bin** in this section);
  - ESP32-MINI-1-AT-Vx.x.x.x.zip (referred to as **MINI-1 Bin** in this section);
  - ESP32-D2WD-AT-Vx.x.x.x.zip (referred to as **D2WD Bin** in this section);

.. only:: esp32c2

  - ESP32C2-2MB-AT-Vx.x.x.x.zip (referred to as **ESP32C2-2MB Bin** in this section);
  - ESP32C2-4MB-AT-Vx.x.x.x.zip (referred to as **ESP32C2-4MB Bin** in this section);
  - ESP32C2-BLE-2MB Bin (firmware is not released, but you can download the ``esp32c2-ble-2mb-at`` firmware from :doc:`GitHub Actions <How_to_download_the_latest_temporary_version_of_AT_from_github>`, or :doc:`Compile ESP-AT Project Locally <How_to_clone_project_and_compile_it>` by yourself);

.. only:: esp32c3

  - ESP32-C3-MINI-1-AT-Vx.x.x.x.zip (referred to as **MINI-1 Bin** in this section);

.. only:: esp32c6

  - ESP32C6-4MB-AT-Vx.x.x.x.zip (referred to as **ESP32C6-4MB Bin** in this section);

.. only:: esp32s2

  - ESP32-S2-MINI-AT-Vx.x.x.x.zip (referred to as **MINI Bin** in this section);

Supported Command Set
^^^^^^^^^^^^^^^^^^^^^

The table lists which command set is supported by default in the official AT firmware applicable to {IDF_TARGET_NAME} series of modules (marked with |icon-green-check|), which is not supported by default but can be supported after configuration of the ESP-AT project (marked with |icon-orange-check|), and which is not supported at all (marked with |icon-red-cross|). Note that the command set that is not shown in this table is not supported either. Applicable firmware that has not been :doc:`officially released <../AT_Binary_Lists/index>` requires compilation by yourself. Those self-compiled firmware cannot be upgraded OTA from Espressif official server.

.. only:: esp32

  .. list-table::
    :header-rows: 1

    * - Command Set
      - WROOM Bin
      - WROVER Bin
      - PICO-D4 Bin
      - SOLO-1 Bin
      - MINI-1 Bin
      - D2WD Bin
    * - base
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
    * - Wi-Fi
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - TCP-IP
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - mDNS
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
    * - SmartConfig
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
    * - MQTT
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - HTTP
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - Bluetooth LE
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - Bluetooth LE HID
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - BluFi
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - Bluetooth SPP
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - Bluetooth A2DP
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
    * - FileSystem
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
    * - WPA2 enterprise
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - Web server
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - WebSocket
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

.. only:: esp32c2

  .. list-table::
    :header-rows: 1

    * - Command Set
      - ESP32C2-2MB Bin
      - ESP32C2-4MB Bin
      - ESP32C2-BLE-2MB Bin
    * - base
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - user
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - Wi-Fi
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - TCP-IP
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - mDNS
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - WPS
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - SmartConfig
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - ping
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - MQTT
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - HTTP
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-red-cross|
    * - Bluetooth LE
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-green-check|
    * - BluFi
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-red-cross|
    * - FileSystem
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - driver
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-orange-check|
    * - WPA2 enterprise
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-red-cross|
    * - WEB server
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-red-cross|
    * - WebSocket
      - |icon-orange-check|
      - |icon-orange-check|
      - |icon-red-cross|
    * - OTA
      - |icon-green-check|
      - |icon-green-check|
      - |icon-red-cross|

.. only:: esp32c3

  .. list-table::
    :header-rows: 1

    * - Command Set
      - MINI-1 Bin
    * - base
      - |icon-green-check|
    * - user
      - |icon-green-check|
    * - Wi-Fi
      - |icon-green-check|
    * - TCP-IP
      - |icon-green-check|
    * - mDNS
      - |icon-green-check|
    * - WPS
      - |icon-green-check|
    * - SmartConfig
      - |icon-green-check|
    * - ping
      - |icon-green-check|
    * - MQTT
      - |icon-green-check|
    * - HTTP
      - |icon-green-check|
    * - Bluetooth LE
      - |icon-green-check|
    * - Bluetooth LE HID
      - |icon-green-check|
    * - BluFi
      - |icon-green-check|
    * - FileSystem
      - |icon-orange-check|
    * - driver
      - |icon-orange-check|
    * - WPA2 enterprise
      - |icon-orange-check|
    * - Web server
      - |icon-orange-check|
    * - WebSocket
      - |icon-orange-check|
    * - OTA
      - |icon-green-check|

.. only:: esp32c6

  .. list-table::
    :header-rows: 1

    * - Command Set
      - ESP32C6-4MB Bin
    * - base
      - |icon-green-check|
    * - user
      - |icon-green-check|
    * - Wi-Fi
      - |icon-green-check|
    * - TCP-IP
      - |icon-green-check|
    * - mDNS
      - |icon-green-check|
    * - WPS
      - |icon-green-check|
    * - SmartConfig
      - |icon-green-check|
    * - ping
      - |icon-green-check|
    * - MQTT
      - |icon-green-check|
    * - HTTP
      - |icon-green-check|
    * - Bluetooth LE
      - |icon-green-check|
    * - BluFi
      - |icon-green-check|
    * - FileSystem
      - |icon-orange-check|
    * - driver
      - |icon-orange-check|
    * - WPA2 enterprise
      - |icon-orange-check|
    * - Web server
      - |icon-orange-check|
    * - WebSocket
      - |icon-orange-check|
    * - OTA
      - |icon-green-check|

.. only:: esp32s2

  .. list-table::
    :header-rows: 1

    * - Command Set
      - MINI Bin
    * - base
      - |icon-green-check|
    * - user
      - |icon-green-check|
    * - Wi-Fi
      - |icon-green-check|
    * - TCP-IP
      - |icon-green-check|
    * - mDNS
      - |icon-green-check|
    * - WPS
      - |icon-green-check|
    * - SmartConfig
      - |icon-green-check|
    * - ping
      - |icon-green-check|
    * - MQTT
      - |icon-green-check|
    * - HTTP
      - |icon-green-check|
    * - FileSystem
      - |icon-orange-check|
    * - driver
      - |icon-orange-check|
    * - WPA2 enterprise
      - |icon-orange-check|
    * - Web server
      - |icon-orange-check|
    * - WebSocket
      - |icon-orange-check|
    * - OTA
      - |icon-green-check|

Hardware Differences
^^^^^^^^^^^^^^^^^^^^

.. only:: esp32

  .. list-table::
    :header-rows: 1

    * - Hardware
      - WROOM Bin
      - WROVER Bin
      - PICO-D4 Bin
      - SOLO-1 Bin
      - MINI-1 Bin
      - D2WD Bin
    * - Flash
      - 4 MB
      - 4 MB
      - 4 MB
      - 4 MB
      - 4 MB
      - 2 MB
    * - PSRAM
      - |icon-red-cross|
      - 8 MB
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
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

.. only:: esp32c2

  .. list-table::
    :header-rows: 1

    * - Hardware
      - ESP32C2-2MB Bin
      - ESP32C2-4MB Bin
      - ESP32C2-BLE-2MB Bin
    * - Flash
      - 2 MB
      - 4 MB
      - 2 MB
    * - PSRAM
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
    * - UART Pins [#one]_
      - | TX: 7
        | RX: 6
        | CTS: 19
        | RTS: 20
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4

.. only:: esp32c3

  .. list-table::
    :header-rows: 1

    * - Hardware
      - MINI-1 Bin
    * - Flash
      - 4 MB
    * - PSRAM
      - |icon-red-cross|
    * - UART Pins [#one]_
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4

.. only:: esp32c6

  .. list-table::
    :header-rows: 1

    * - Hardware
      - ESP32C6-4MB Bin
    * - Flash
      - 4 MB
    * - PSRAM
      - |icon-red-cross|
    * - UART Pins [#one]_
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4

.. only:: esp32s2

  .. list-table::
    :header-rows: 1

    * - Hardware
      - MINI Bin
    * - Flash
      - 4 MB
    * - PSRAM
      - |icon-red-cross|
    * - UART Pins [#one]_
      - | TX: 17
        | RX: 21
        | CTS: 20
        | RTS: 19

.. [#one] UART pins can be customized. See :doc:`How_to_set_AT_port_pin` for details.

Supported Modules
^^^^^^^^^^^^^^^^^

The table below lists the modules or chips that are default supported by the officially released {IDF_TARGET_NAME} series AT firmware (indicated by |icon-green-check|), the modules that are not supported by default but can be modified to be supported using the :doc:`../Compile_and_Develop/tools_at_py` (indicated by |icon-orange-check|), and the modules that are not supported at all (indicated by |icon-red-cross|). For modules that are not supported at all, you can refer to :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it` to modify the configuration as needed for support.

.. only:: esp32

  .. list-table::
    :header-rows: 1
    :widths: 100 40 40 40 40 40 40

    * - Module/Chip
      - WROOM Bin
      - WROVER Bin
      - PICO-D4 Bin
      - SOLO-1 Bin
      - MINI-1 Bin
      - D2WD Bin
    * - ESP32-WROOM-32
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32D
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32E
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32U
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32UE
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32DA
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROOM-32SE
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROVER-E
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROVER-IE
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROVER-B
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROVER-IB
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-WROVER
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-SOLO-1
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-D2WD
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-green-check|
    * - ESP32-MINI-1
      - |icon-orange-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-MINI-1U
      - |icon-orange-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-PICO-D4
      - |icon-green-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP32-PICO-V3-ZERO
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
      - |icon-red-cross|
    * - ESP32-PICO-MINI-02
      - |icon-orange-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-orange-check|
    * - ESP32-PICO-MINI-02U
      - |icon-orange-check|
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-orange-check|
      - |icon-green-check|
      - |icon-orange-check|

.. only:: esp32c2

  .. list-table::
    :header-rows: 1
    :widths: 100 60 60 60

    * - Module/Chip
      - ESP32C2-2MB Bin
      - ESP32C2-4MB Bin
      - ESP32C2-BLE-2MB Bin
    * - ESP8684-MINI-1/1U
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP8684-WROOM-02C/02UC
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP8684-WROOM-03
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP8684-WROOM-05
      - |icon-red-cross|
      - |icon-green-check|
      - |icon-green-check|
    * - ESP8684-WROOM-07
      - |icon-red-cross|
      - |icon-orange-check|
      - |icon-orange-check|

.. only:: esp32c3

  .. list-table::
    :header-rows: 1
    :widths: 100 60

    * - Module/Chip
      - MINI-1 Bin
    * - ESP32-C3-MINI-1/1U
      - |icon-green-check|
    * - ESP32-C3-WROOM-02/02U
      - |icon-green-check|
    * - ESP8685-WROOM-01
      - |icon-green-check|
    * - ESP8685-WROOM-03
      - |icon-green-check|
    * - ESP8685-WROOM-04
      - |icon-green-check|
    * - ESP8685-WROOM-05
      - |icon-green-check|
    * - ESP8685-WROOM-06
      - |icon-green-check|
    * - ESP8685-WROOM-07
      - |icon-orange-check|

.. only:: esp32c6

  .. list-table::
    :header-rows: 1

    * - Module/Chip
      - ESP32C6-4MB Bin
    * - ESP32-C6-MINI-1
      - |icon-green-check|
    * - ESP32-C6-WROOM-01
      - |icon-green-check|

.. only:: esp32s2

  .. list-table::
    :header-rows: 1

    * - Module/Chip
      - MINI Bin
    * - ESP32-S2-MINI-2/2U
      - |icon-green-check|
    * - ESP32-S2-SOLO-2/2U
      - |icon-green-check|
