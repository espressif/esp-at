.. include:: ../../inline_substitutions

ESP-AT 固件差异
====================

:link_to_translation:`en:[English]`

本文档比较了同一 {IDF_TARGET_NAME} 系列的 AT 固件在支持的命令集、硬件、模组方面的差异。

{IDF_TARGET_NAME} 系列
------------------------

本节介绍以下 {IDF_TARGET_NAME} 系列 AT 固件的区别：

.. only:: esp32

  - ESP32-WROOM-32-AT-Vx.x.x.x.zip（本节简称为 **WROOM Bin**）
  - ESP32-WROVER-32-AT-Vx.x.x.x.zip（本节简称为 **WROVER Bin**）
  - ESP32-PICO-D4-AT-Vx.x.x.x.zip（本节简称为 **PICO-D4 Bin**）
  - ESP32-SOLO-1-AT-Vx.x.x.x.zip（本节简称为 **SOLO-1 Bin**
  - ESP32-MINI-1-AT-Vx.x.x.x.zip（本节简称为 **MINI-1 Bin**）
  - ESP32-D2WD-AT-Vx.x.x.x.zip（本节简称为 **D2WD Bin**）

.. only:: esp32c2

  - ESP32C2-2MB-AT-Vx.x.x.x.zip（本节简称为 **ESP32C2-2MB Bin**）
  - ESP32C2-4MB-AT-Vx.x.x.x.zip（本节简称为 **ESP32C2-4MB Bin**）
  - ESP32C2-BLE-2MB Bin（固件未发布，您可以从 :doc:`GitHub Actions 里下载 <How_to_download_the_latest_temporary_version_of_AT_from_github>` ``esp32c2-ble-2mb-at`` 固件，或者 :doc:`本地编译 ESP-AT 工程 <How_to_clone_project_and_compile_it>`）

.. only:: esp32c3

  - ESP32-C3-MINI-1-AT-Vx.x.x.x.zip（本节简称为 **MINI-1 Bin**）

.. only:: esp32c6

  - ESP32C6-4MB-AT-Vx.x.x.x.zip（本节简称为 **ESP32C6-4MB Bin**）

.. only:: esp32s2

  - ESP32-S2-MINI-AT-Vx.x.x.x.zip（本节简称为 **MINI Bin**）

支持的命令集
^^^^^^^^^^^^

下表列出了官方适配的 {IDF_TARGET_NAME} 系列 AT 固件默认支持哪些命令集（用 |icon-green-check| 表示）、默认不支持但可以在配置和编译 ESP-AT 工程后支持的命令集（用 |icon-orange-check| 表示）、完全不支持的命令集（用 |icon-red-cross| 表示），下表没有列出的命令集也为完全不支持的命令集。正式发布的固件见 :doc:`../AT_Binary_Lists/index`，已适配但未发布的模组固件，需要自行编译。自行编译的固件无法从乐鑫官方服务器进行 OTA 升级。

.. only:: esp32

  .. list-table::
    :header-rows: 1

    * - 命令集
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

    * - 命令集
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

    * - 命令集
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

    * - 命令集
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

    * - 命令集
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

硬件差异
^^^^^^^^

.. only:: esp32

  .. list-table::
    :header-rows: 1

    * - 硬件
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
    * - UART 管脚 [#one]_
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

    * - 硬件
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
    * - UART 管脚 [#one]_
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

    * - 硬件
      - MINI-1 Bin
    * - Flash
      - 4 MB
    * - PSRAM
      - |icon-red-cross|
    * - UART 管脚 [#one]_
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4

.. only:: esp32c6

  .. list-table::
    :header-rows: 1

    * - 硬件
      - ESP32C6-4MB Bin
    * - Flash
      - 4 MB
    * - PSRAM
      - |icon-red-cross|
    * - UART 管脚 [#one]_
      - | TX: 7
        | RX: 6
        | CTS: 5
        | RTS: 4

.. only:: esp32s2

  .. list-table::
    :header-rows: 1

    * - 硬件
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

.. [#one] UART 管脚可自定义，详情请参考 :doc:`How_to_set_AT_port_pin`。

支持的模组
^^^^^^^^^^

下表列出了官方发布的 {IDF_TARGET_NAME} 系列 AT 固件默认支持哪些模组或芯片（用 |icon-green-check| 表示）、默认不支持但可以通过 :doc:`../Compile_and_Develop/tools_at_py` 修改后支持的模组（用 |icon-orange-check| 表示），以及完全不支持的模组（用 |icon-red-cross| 表示）。对于完全不支持的模组，您可以 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it` 修改您需要的配置后支持。

.. only:: esp32

  .. list-table::
    :header-rows: 1
    :widths: 100 40 40 40 40 40 40

    * - 模组/芯片
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

    * - 模组/芯片
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

    * - 模组/芯片
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

    * - 模组/芯片
      - ESP32C6-4MB Bin
    * - ESP32-C6-MINI-1
      - |icon-green-check|
    * - ESP32-C6-WROOM-01
      - |icon-green-check|

.. only:: esp32s2

  .. list-table::
    :header-rows: 1

    * - 模组/芯片
      - MINI Bin
    * - ESP32-S2-MINI-2/2U
      - |icon-green-check|
    * - ESP32-S2-SOLO-2/2U
      - |icon-green-check|
