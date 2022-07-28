Hardware Connection
===================

:link_to_translation:`zh_CN:[中文]`

This document introduces what hardware you need to prepare and how to connect them in order to download AT firmware, send AT commands, and receive AT responses.

For different series of modules, the commands supported by AT firmware are different. Please refer to :doc:`/Compile_and_Develop/esp-at_firmware_differences` for more details.

What You Need
--------------

.. list-table:: List of Components Required for ESP-AT Testing
   :header-rows: 1

   * - Component
     - Function
   * - {IDF_TARGET_NAME} board
     - Slave MCU.
   * - USB cable ({IDF_TARGET_NAME} borad to PC)
     - Download/Log output connection.
   * - PC
     - Act as Host MCU. Download firmware to Slave MCU.
   * - USB cable (PC to serial port converter)
     - AT command/response connection.
   * - USB to serial port converter
     - Convert between USB signals and TTL signals.
   * - Jumper wires (serial port converter to {IDF_TARGET_NAME} board)
     - AT command/response connection.

.. figure:: ../../_static/hw-connection-what-you-need.png
   :align: center
   :alt: Connection of Components for ESP-AT Testing
   :figclass: align-center

   Connection of Components for ESP-AT Testing

Please note that in the above picture, four jump wires are used to connect the {IDF_TARGET_NAME} board and USB to serial converter. If you don't use hardware flow control, two wires connecting TX/RX and a simpler converter will be enough.

.. only:: esp32

  {IDF_TARGET_NAME} Series
  ------------------------

  {IDF_TARGET_NAME} AT uses two UART ports: UART0 is used to download firmware and log output; UART1 is used to send AT commands and receive AT responses. Both UART0 and UART1 use ``115200`` baud rate for communication by default.

  All {IDF_TARGET_NAME} modules use GPIO1 and GPIO3 as UART0, but they use different GPIOs as UART1. The following sections illustrate which GPIOs you should connect for each {IDF_TARGET_NAME} series of modules.

  For more details of {IDF_TARGET_NAME} modules and boards, please refer to `{IDF_TARGET_NAME} Modules and Boards <https://docs.espressif.com/projects/esp-idf/en/stable/hw-reference/modules-and-boards.html#wroom-solo-wrover-and-pico-modules>`_.

  ESP32-WROOM-32 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  .. list-table:: ESP32-WROOM-32 Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board Pins
      - Other Device Pins
    * - Download/Log output :sup:`1`
      - UART0
          * GPIO3 (RX)
          * GPIO1 (TX)
      - PC
          * TX
          * RX
    * - AT command/response :sup:`2`
      - UART1
          * GPIO16 (RX)
          * GPIO17 (TX)
          * GPIO15 (CTS)
          * GPIO14 (RTS)
      - USB to serial converter
          * TX
          * RX
          * RTS
          * CTS

  **Note** 1: Connection between individual pins of the {IDF_TARGET_NAME} board and the PC is already established internally on the {IDF_TARGET_NAME} board. You only need to provide USB cable between the board and PC.

  **Note** 2: Connection between CTS/RTS is optional, depending on whether you want to use hardware flow control.

  .. figure:: ../../_static/esp32-wroom-hw-connection.png
    :align: center
    :alt: ESP32-WROOM-32 Series Hardware Connection
    :figclass: align-center

    ESP32-WROOM-32 Series Hardware Connection

  If you want to connect your device directly with ESP32-WROOM-32 rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `ESP32-WROOM-32 Datasheet <https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf>`_ for more details.

  .. _hw-connection-esp32-wrover-series:

  ESP32-WROVER Series
  ^^^^^^^^^^^^^^^^^^^^^^^^
  .. list-table:: ESP32-WROVER Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board Pins
      - Other Device Pins
    * - Download/Log output :sup:`1`
      - UART0
          * GPIO3 (RX)
          * GPIO1 (TX)
      - PC
          * TX
          * RX
    * - AT command/response :sup:`2`
      - UART1
          * GPIO19 (RX)
          * GPIO22 (TX)
          * GPIO15 (CTS)
          * GPIO14 (RTS)
      - USB to serial converter
          * TX
          * RX
          * RTS
          * CTS

  **Note** 1: Connection between individual pins of the {IDF_TARGET_NAME} board and the PC is already established internally on the {IDF_TARGET_NAME} board. You only need to provide USB cable between the board and PC.

  **Note** 2: Connection between CTS/RTS is optional, depending on whether you want to use hardware flow control.

  .. figure:: ../../_static/esp32-wrover-hw-connection.png
    :align: center
    :alt: ESP32-WROVER Series Hardware Connection
    :figclass: align-center

    ESP32-WROVER Series Hardware Connection

  If you want to connect your device directly with ESP32-WROVER rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `ESP32-WROVER Datasheet <https://www.espressif.com/sites/default/files/documentation/esp32-wrover_datasheet_en.pdf>`_ for more details.

  ESP32-PICO Series
  ^^^^^^^^^^^^^^^^^^

  .. list-table:: ESP32-PICO Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board Pins
      - Other Device Pins
    * - Download/Log output :sup:`1`
      - UART0
          * GPIO3 (RX)
          * GPIO1 (TX)
      - PC
          * TX
          * RX
    * - AT command/response :sup:`2`
      - UART1
          * GPIO19 (RX)
          * GPIO22 (TX)
          * GPIO15 (CTS)
          * GPIO14 (RTS)
      - USB to serial converter
          * TX
          * RX
          * RTS
          * CTS

  **Note** 1: Connection between individual pins of the {IDF_TARGET_NAME} board and the PC is already established internally on the {IDF_TARGET_NAME} board. You only need to provide USB cable between the board and PC.

  **Note** 2: Connection between CTS/RTS is optional, depending on whether you want to use hardware flow control.

  .. figure:: ../../_static/esp32-pico-hw-connection.png
    :align: center
    :alt: ESP32-PICO Series Hardware Connection
    :figclass: align-center

    ESP32-PICO Series Hardware Connection

  If you want to connect your device directly with ESP32-PICO-D4 rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `ESP32-PICO-D4 Datasheet <https://www.espressif.com/sites/default/files/documentation/esp32-pico-d4_datasheet_en.pdf>`_ for more details.

  ESP32-SOLO Series
  ^^^^^^^^^^^^^^^^^^

  .. list-table:: ESP32-SOLO Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board Pins
      - Other Device Pins
    * - Download/Log output :sup:`1`
      - UART0
          * GPIO3 (RX)
          * GPIO1 (TX)
      - PC
          * TX
          * RX
    * - AT command/response :sup:`2`
      - UART1
          * GPIO16 (RX)
          * GPIO17 (TX)
          * GPIO15 (CTS)
          * GPIO14 (RTS)
      - USB to serial converter
          * TX
          * RX
          * RTS
          * CTS

  **Note** 1: Connection between individual pins of the {IDF_TARGET_NAME} board and the PC is already established internally on the {IDF_TARGET_NAME} board. You only need to provide USB cable between the board and PC.

  **Note** 2: Connection between CTS/RTS is optional, depending on whether you want to use hardware flow control.

  .. figure:: ../../_static/esp32-solo-hw-connection.png
    :align: center
    :alt: ESP32-SOLO Series Hardware Connection
    :figclass: align-center

    ESP32-SOLO Series Hardware Connection

  If you want to connect your device directly with ESP32-SOLO-1 rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `ESP32-SOLO-1 Datasheet <https://www.espressif.com/sites/default/files/documentation/esp32-solo-1_datasheet_en.pdf>`_ for more details.

.. only:: esp32c2

  {IDF_TARGET_NAME} Series
  ------------------------

  {IDF_TARGET_NAME} AT uses two UART ports: UART0 is used to download firmware and log output; UART1 is used to send AT commands and receive AT responses. Both UART0 and UART1 use ``115200`` baud rate for communication by default.

  .. list-table:: {IDF_TARGET_NAME} Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board Pins
      - Other Device Pins
    * - Download/Log output :sup:`1`
      - UART0
          * GPIO20 (RX)
          * GPIO21 (TX)
      - PC
          * TX
          * RX
    * - AT command/response :sup:`2`
      - UART1
          * GPIO6 (RX)
          * GPIO7 (TX)
          * GPIO5 (CTS)
          * GPIO4 (RTS)
      - USB to serial converter
          * TX
          * RX
          * RTS
          * CTS

  **Note** 1: Connection between individual pins of the {IDF_TARGET_NAME} board and the PC is already established internally on the {IDF_TARGET_NAME} board. You only need to provide USB cable between the board and PC.

  **Note** 2: Connection between CTS/RTS is optional, depending on whether you want to use hardware flow control.

  .. figure:: ../../_static/esp32-c2-hw-connection.png
    :align: center
    :alt: {IDF_TARGET_NAME} Series Hardware Connection
    :figclass: align-center

    {IDF_TARGET_NAME} Series Hardware Connection

  If you want to connect your device directly with ESP32-C2-MINI-1 module rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `ESP32-C2-MINI-1 Datasheet <https://www.espressif.com/sites/default/files/documentation/esp32-c2-mini-1_datasheet_en.pdf>`_ for more details.

.. only:: esp32c3

  {IDF_TARGET_NAME} Series
  ------------------------

  {IDF_TARGET_NAME} AT uses two UART ports: UART0 is used to download firmware and log output; UART1 is used to send AT commands and receive AT responses. Both UART0 and UART1 use ``115200`` baud rate for communication by default.

  .. list-table:: {IDF_TARGET_NAME} Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board Pins
      - Other Device Pins
    * - Download/Log output :sup:`1`
      - UART0
          * GPIO20 (RX)
          * GPIO21 (TX)
      - PC
          * TX
          * RX
    * - AT command/response :sup:`2`
      - UART1
          * GPIO6 (RX)
          * GPIO7 (TX)
          * GPIO5 (CTS)
          * GPIO4 (RTS)
      - USB to serial converter
          * TX
          * RX
          * RTS
          * CTS

  **Note** 1: Connection between individual pins of the {IDF_TARGET_NAME} board and the PC is already established internally on the {IDF_TARGET_NAME} board. You only need to provide USB cable between the board and PC.

  **Note** 2: Connection between CTS/RTS is optional, depending on whether you want to use hardware flow control.

  .. figure:: ../../_static/esp32-c3-hw-connection.png
    :align: center
    :alt: {IDF_TARGET_NAME} Series Hardware Connection
    :figclass: align-center

    {IDF_TARGET_NAME} Series Hardware Connection

  If you want to connect your device directly with ESP32-C3-MINI-1 module rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `ESP32-C3-MINI-1 Datasheet <https://www.espressif.com/sites/default/files/documentation/esp32-c3-mini-1_datasheet_en.pdf>`_ for more details.
