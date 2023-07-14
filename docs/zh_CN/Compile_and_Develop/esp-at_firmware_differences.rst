.. include:: ../../inline_substitutions

ESP-AT 固件差异
====================

:link_to_translation:`en:[English]`

本文档比较了同一 {IDF_TARGET_NAME} 系列的 AT 固件在支持的命令集、硬件、模组方面的差异。

{IDF_TARGET_NAME} 系列
------------------------

本节介绍以下 {IDF_TARGET_NAME} 系列 AT 固件的区别：

.. only:: esp32

  - ESP32-WROOM-32_AT_Bin（本节简称为 **WROOM Bin**）
  - ESP32-WROVER-32_AT_Bin（本节简称为 **WROVER Bin**）
  - ESP32-PICO-D4_AT_Bin（本节简称为 **PICO-D4 Bin**）
  - ESP32-SOLO-1_AT_Bin（本节简称为 **SOLO-1 Bin**
  - ESP32-MINI-1_AT_Bin（本节简称为 **MINI-1 Bin**）
  - ESP32-D2WD_AT_Bin（本节简称为 **D2WD Bin**）

.. only:: esp32c2

  - ESP32C2-2MB_AT_Bin（本节简称为 **ESP32C2-2MB Bin**）
  - ESP32C2-4MB_AT_Bin（本节简称为 **ESP32C2-4MB Bin**）

.. only:: esp32c3

  - ESP32-C3-MINI-1_AT_Bin（本节简称为 **MINI-1 Bin**）

.. only:: esp32c6

  - ESP32C6-4MB_AT_Bin（本节简称为 **ESP32C6-4MB Bin**）

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
    * - wifi
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
    * - MDNS
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
    * - smartconfig
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
    * - http
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
    * - ble hid
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - blufi
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
      - |icon-green-check|
    * - bt spp
      - |icon-orange-check|
      - |icon-green-check|
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
    * - ethernet
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
    * - driver
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
    * - WEB
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
      - |icon-orange-check|
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
    * - blufi
      - |icon-orange-check|
      - |icon-green-check|

.. only:: esp32c3 or esp32c6

  .. list-table::
    :header-rows: 1

    * - 命令集
      - MINI-1 Bin
    * - base
      - |icon-green-check|
    * - user
      - |icon-green-check|
    * - wifi
      - |icon-green-check|
    * - net
      - |icon-green-check|
    * - MDNS
      - |icon-green-check|
    * - WPS
      - |icon-green-check|
    * - smartconfig
      - |icon-green-check|
    * - ping
      - |icon-green-check|
    * - MQTT
      - |icon-green-check|
    * - http
      - |icon-green-check|
    * - FS
      - |icon-orange-check|
    * - driver
      - |icon-orange-check|
    * - WPA2
      - |icon-orange-check|
    * - WEB
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
    * - Flash
      - 2 MB
      - 4 MB
    * - PSRAM
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

.. only:: esp32c3 or esp32c6

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
    :widths: 100 60 60

    * - 模组/芯片
      - ESP32C2-2MB Bin
      - ESP32C2-4MB Bin
    * - ESP8684-MINI-1/1U
      - |icon-green-check|
      - |icon-green-check|
    * - ESP8684-WROOM-02C/02UC
      - |icon-green-check|
      - |icon-green-check|
    * - ESP8684-WROOM-03
      - |icon-red-cross|
      - |icon-green-check|
    * - ESP8684-WROOM-05
      - |icon-red-cross|
      - |icon-green-check|
    * - ESP8684-WROOM-07
      - |icon-red-cross|
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
