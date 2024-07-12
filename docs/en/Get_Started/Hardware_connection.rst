Hardware Connection
===================

:link_to_translation:`zh_CN:[中文]`

This document introduces what hardware you need to prepare and how to connect them in order to download AT firmware, send AT commands, and receive AT responses.

The commands supported by each series of AT firmware vary, and its compatibility with modules or chips also differs. For more information, refer to :doc:`/Compile_and_Develop/esp-at_firmware_differences`.

If you don't want to use the default AT pin, you can refer to the :doc:`/Compile_and_Develop/How_to_set_AT_port_pin` document to change the pin.

What You Need
--------------

.. list-table:: List of Components Required for ESP-AT Testing
   :header-rows: 1

   * - Component
     - Function
   * - {IDF_TARGET_NAME} board
     - Slave MCU.
   * - USB cable ({IDF_TARGET_NAME} board to PC)
     - Download/Log output connection.
   * - PC
     - Act as Host MCU. Download firmware to Slave MCU.
   * - USB cable (PC to serial port converter)
     - AT command/response connection.
   * - USB to serial port converter
     - Convert between USB signals and TTL signals.
   * - Jumper wires (serial port converter to {IDF_TARGET_NAME} board)
     - AT command/response connection.

.. figure:: ../../_static/get_started/hw_connection/hw-connection-what-you-need.png
   :align: center
   :alt: Connection of Components for ESP-AT Testing
   :figclass: align-center

   Connection of Components for ESP-AT Testing

Note:

.. only:: esp32c2

  - The official default :doc:`../AT_Binary_Lists/index` only supports 26 MHz crystal oscillator. If your crystal oscillator of {IDF_TARGET_NAME} is 40 MHz, please refer to :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it` document to compile {IDF_TARGET_NAME} AT firmware, and configurate in the step 5:

    ::

      python build.py menuconfig -> Component config -> Hardware Settings -> Main XTAL Config -> Main XTAL frequency -> 40 MHz

- In the above picture, four jump wires are used to connect the {IDF_TARGET_NAME} board and USB to serial converter. If you do not use hardware flow control, two wires connecting TX/RX and a simpler converter will be enough.

.. only:: esp32 or esp32c6 or esp32s2

  - If you use an {IDF_TARGET_NAME} module instead of a development board and flash firmware via UART, you need to reserve the UART pins (refer to {IDF_TARGET_DATASHEET_EN_URL} > Section Pin Description for more details) and strapping pins (refer to {IDF_TARGET_DATASHEET_EN_URL} > Section Strapping Pins for more details), and enter the download mode by controlling the strapping pin level.

.. only:: esp32c2

  - If you use an {IDF_TARGET_NAME} module instead of a development board and flash firmware via UART, you need to reserve the UART pins (refer to {IDF_TARGET_DATASHEET_EN_URL} > Section Pin Description for more details) and one of the following conditions needs to be met:

    - Reserve the Strapping pins (refer to {IDF_TARGET_DATASHEET_EN_URL} > Section Strapping Pins for more details), and enter the download mode by controlling the Strapping pin level.
    - Enter the download mode by sending :ref:`AT+RST=1,1 <cmd-RST>` command.

.. only:: esp32c3

  - If you use an {IDF_TARGET_NAME} module instead of a development board, you need to reserve the UART/USB pins (refer to {IDF_TARGET_DATASHEET_EN_URL} > Section Pin Description for more details) and one of the following conditions needs to be met:

    - Reserve the Strapping pins (refer to {IDF_TARGET_DATASHEET_EN_URL} > Section Strapping Pins for more details), and enter the download mode by controlling the Strapping pin level.
    - Enter the download mode by sending :ref:`AT+RST=1,1 <cmd-RST>` command.

.. only:: esp32

  {IDF_TARGET_NAME} Series
  ------------------------

  {IDF_TARGET_NAME} AT uses two UART ports: UART0 is used to download firmware and log output; UART1 is used to send AT commands and receive AT responses. Both UART0 and UART1 use ``115200`` baud rate for communication by default.

  All {IDF_TARGET_NAME} modules use GPIO1 and GPIO3 as UART0, but they use different GPIOs as UART1. The following sections illustrate which GPIOs you should connect for each {IDF_TARGET_NAME} series of modules.

  For more details of {IDF_TARGET_NAME} modules and boards, please refer to `{IDF_TARGET_NAME} modules <https://espressif.com/en/products/modules?id={IDF_TARGET_NAME}>`_ and `{IDF_TARGET_NAME} boards <https://www.espressif.com/en/products/devkits?id={IDF_TARGET_NAME}>`_.

  ESP32-WROOM-32 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  .. list-table:: ESP32-WROOM-32 Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board or Module Pins
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

  .. figure:: ../../_static/get_started/hw_connection/esp32-wroom-hw-connection.png
    :align: center
    :alt: ESP32-WROOM-32 Series Hardware Connection
    :figclass: align-center

    ESP32-WROOM-32 Series Hardware Connection

  If you want to connect your device directly with ESP32-WROOM-32 rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `ESP32-WROOM-32 Datasheet <https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32e_esp32-wroom-32ue_datasheet_en.pdf>`_ for more details.

  ESP32-MINI-1 Series
  ^^^^^^^^^^^^^^^^^^^^^^

  .. list-table:: ESP32-MINI-1 Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board or Module Pins
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

  .. figure:: ../../_static/get_started/hw_connection/esp32-mini-hw-connection.jpg
    :align: center
    :alt: ESP32-MINI-1 Series Hardware Connection
    :figclass: align-center

    ESP32-MINI-1 Series Hardware Connection

  .. _hw-connection-esp32-wrover-series:

  ESP32-WROVER Series
  ^^^^^^^^^^^^^^^^^^^^^^^^
  .. list-table:: ESP32-WROVER Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board or Module Pins
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

  .. figure:: ../../_static/get_started/hw_connection/esp32-wrover-hw-connection.png
    :align: center
    :alt: ESP32-WROVER Series Hardware Connection
    :figclass: align-center

    ESP32-WROVER Series Hardware Connection

  If you want to connect your device directly with ESP32-WROVER rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `ESP32-WROVER Datasheet <https://www.espressif.com/sites/default/files/documentation/esp32-wrover-e_esp32-wrover-ie_datasheet_en.pdf>`_ for more details.

  ESP32-PICO Series
  ^^^^^^^^^^^^^^^^^^

  .. list-table:: ESP32-PICO Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board or Module Pins
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

  .. figure:: ../../_static/get_started/hw_connection/esp32-pico-hw-connection.png
    :align: center
    :alt: ESP32-PICO Series Hardware Connection
    :figclass: align-center

    ESP32-PICO Series Hardware Connection

  If you want to connect your device directly with ESP32-PICO-D4 rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `ESP32-PICO-D4 Datasheet <https://www.espressif.com/sites/default/files/documentation/esp32-pico_series_datasheet_en.pdf>`_ for more details.

  ESP32-SOLO Series
  ^^^^^^^^^^^^^^^^^^

  .. list-table:: ESP32-SOLO Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board or Module Pins
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

  .. figure:: ../../_static/get_started/hw_connection/esp32-solo-hw-connection.png
    :align: center
    :alt: ESP32-SOLO Series Hardware Connection
    :figclass: align-center

    ESP32-SOLO Series Hardware Connection

  If you want to connect your device directly with ESP32-SOLO-1 rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `ESP32-SOLO-1 Datasheet <https://www.espressif.com/sites/default/files/documentation/esp32-solo-1_datasheet_en.pdf>`_ for more details.

.. only:: esp32c2

  {IDF_TARGET_CFG_PREFIX}-4MB Series
  ----------------------------------

  {IDF_TARGET_CFG_PREFIX}-4MB series refer to the module or board that has a built-in {IDF_TARGET_NAME}/ESP8684 chip with a 4 MB flash, such as {IDF_TARGET_NAME} MINI series device and {IDF_TARGET_NAME} WROOM series device.

  {IDF_TARGET_CFG_PREFIX}-4MB AT uses two UART ports: UART0 is used to download firmware and log output; UART1 is used to send AT commands and receive AT responses. Both UART0 and UART1 use ``115200`` baud rate for communication by default. Because a 26 MHz XTAL is used, the log is outputted at a baud rate of ``74880`` via UART0 (TX: GPIO20) during the ROM stage. If a 40 MHz XTAL is used instead, the log will be outputted at a baud rate of ``115200``.

  .. list-table:: {IDF_TARGET_CFG_PREFIX}-4MB Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_CFG_PREFIX}-4MB Board or Module Pins
      - Other Device Pins
    * - Download/Log output :sup:`1`
      - UART0
          * GPIO19 (RX)
          * GPIO20 (TX)
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

  **Note** 1: Connection between individual pins of the {IDF_TARGET_CFG_PREFIX}-4MB board and the PC is already established internally on the {IDF_TARGET_CFG_PREFIX}-4MB board. You only need to provide USB cable between the board and PC.

  **Note** 2: Connection between CTS/RTS is optional, depending on whether you want to use hardware flow control.

  .. figure:: ../../_static/get_started/hw_connection/esp32-c2-4mb-hw-connection.png
    :align: center
    :alt: {IDF_TARGET_CFG_PREFIX}-4MB Series Hardware Connection
    :figclass: align-center

    {IDF_TARGET_CFG_PREFIX}-4MB Series Hardware Connection

  If you want to connect your device directly with {IDF_TARGET_CFG_PREFIX}-4MB module rather than the {IDF_TARGET_CFG_PREFIX}-4MB board that integrates it, please refer to the corresponding module `datasheet <https://www.espressif.com/en/support/documents/technical-documents>`_ for more details.

  {IDF_TARGET_CFG_PREFIX}-2MB Series
  ----------------------------------

  {IDF_TARGET_CFG_PREFIX}-2MB series refers to the module or board that has a built-in {IDF_TARGET_NAME}/ESP8684 chip with a 2 MB flash.

  {IDF_TARGET_CFG_PREFIX}-2MB AT uses two UART ports: UART0 is used to download firmware and log output; UART1 is used to send AT commands and receive AT responses. Both UART0 (GPIO8, used to output log) and UART1 use ``115200`` baud rate for communication by default. Because a 26 MHz XTAL is used, the log is outputted at a baud rate of ``74880`` via UART0 (TX: GPIO20) during the ROM stage. If a 40 MHz XTAL is used instead, the log will be outputted at a baud rate of ``115200``.

  .. list-table:: {IDF_TARGET_CFG_PREFIX}-2MB Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_CFG_PREFIX}-2MB Board or Module Pins
      - Other Device Pins
    * - Download/Log output :sup:`1`
      - UART0
          * GPIO19 (RX)
          * GPIO20 (TX)
      - PC
          * TX
          * RX
    * - AT command/response :sup:`2`
      - UART1
          * GPIO6 (RX)
          * GPIO7 (TX)
          * GPIO19 (CTS)
          * GPIO20 (RTS)
      - USB to serial converter
          * TX
          * RX
          * RTS
          * CTS
    * - Log output
      - UART0
          * GPIO8 (TX)
      - USB to serial converter
          * RX

  **Note** 1: Connection between individual pins of the {IDF_TARGET_CFG_PREFIX}-2MB board and the PC is already established internally on the {IDF_TARGET_CFG_PREFIX}-2MB board. You only need to provide USB cable between the board and PC.

  **Note** 2: Connection between CTS/RTS is optional, depending on whether you want to use hardware flow control.

  .. figure:: ../../_static/get_started/hw_connection/esp32-c2-2mb-hw-connection.png
    :align: center
    :alt: {IDF_TARGET_CFG_PREFIX}-2MB Series Hardware Connection
    :figclass: align-center

    {IDF_TARGET_CFG_PREFIX}-2MB Series Hardware Connection

  If you want to connect your device directly with {IDF_TARGET_CFG_PREFIX}-2MB module rather than the {IDF_TARGET_CFG_PREFIX}-2MB board that integrates it, please refer to the corresponding module `datasheet <https://www.espressif.com/en/support/documents/technical-documents>`_ for more details.

.. only:: esp32c3

  {IDF_TARGET_NAME} Series
  ------------------------

  {IDF_TARGET_NAME} series refer to the module or board that has a built-in {IDF_TARGET_NAME} chip, such as {IDF_TARGET_CFG_PREFIX}-MINI series device and {IDF_TARGET_CFG_PREFIX}-WROOM series device.

  {IDF_TARGET_NAME} AT uses two UART ports: UART0 is used to download firmware and log output; UART1 is used to send AT commands and receive AT responses. Both UART0 and UART1 use ``115200`` baud rate for communication by default.

  .. list-table:: {IDF_TARGET_NAME} Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board or Module Pins
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

  .. figure:: ../../_static/get_started/hw_connection/esp32-c3-hw-connection.png
    :align: center
    :alt: {IDF_TARGET_NAME} Series Hardware Connection
    :figclass: align-center

    {IDF_TARGET_NAME} Series Hardware Connection

  If you want to connect your device directly with ESP32-C3-MINI-1 module rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `ESP32-C3-MINI-1 Datasheet <https://www.espressif.com/sites/default/files/documentation/esp32-c3-mini-1_datasheet_en.pdf>`_ for more details.

.. only:: esp32c6

  {IDF_TARGET_CFG_PREFIX}-4MB Series
  ----------------------------------

  {IDF_TARGET_CFG_PREFIX}-4MB series refer to the module or board that has a built-in {IDF_TARGET_NAME} chip with a 4 MB flash, such as {IDF_TARGET_CFG_PREFIX}-MINI series device and {IDF_TARGET_CFG_PREFIX}-WROOM series device.

  {IDF_TARGET_CFG_PREFIX}-4MB AT uses two UART ports: UART0 is used to download firmware and log output; UART1 is used to send AT commands and receive AT responses. Both UART0 and UART1 use ``115200`` baud rate for communication by default.

  .. list-table:: {IDF_TARGET_CFG_PREFIX}-4MB Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_CFG_PREFIX}-4MB Board or Module Pins
      - Other Device Pins
    * - Download/Log output :sup:`1`
      - UART0
          * GPIO17 (RX)
          * GPIO16 (TX)
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

  **Note** 1: Connection between individual pins of the {IDF_TARGET_CFG_PREFIX}-4MB board and the PC is already established internally on the {IDF_TARGET_CFG_PREFIX}-4MB board. You only need to provide USB cable between the board and PC.

  **Note** 2: Connection between CTS/RTS is optional, depending on whether you want to use hardware flow control.

  .. figure:: ../../_static/get_started/hw_connection/esp32-c6-4mb-hw-connection.jpg
    :align: center
    :alt: {IDF_TARGET_CFG_PREFIX}-4MB Series Hardware Connection
    :figclass: align-center

    {IDF_TARGET_CFG_PREFIX}-4MB Series Hardware Connection

  If you want to connect your device directly with {IDF_TARGET_CFG_PREFIX}-4MB module rather than the {IDF_TARGET_CFG_PREFIX}-4MB board that integrates it, please refer to the corresponding module `datasheet <https://www.espressif.com/en/support/documents/technical-documents>`_ for more details.

.. only:: esp32s2

  {IDF_TARGET_NAME} Series
  ------------------------

  {IDF_TARGET_NAME} series refer to the module or board that has a built-in {IDF_TARGET_NAME} chip, such as {IDF_TARGET_CFG_PREFIX}-MINI series device and {IDF_TARGET_CFG_PREFIX}-WROOM series device.

  {IDF_TARGET_NAME} AT uses two UART ports: UART0 is used to download firmware and log output; UART1 is used to send AT commands and receive AT responses. Both UART0 and UART1 use ``115200`` baud rate for communication by default.

  .. list-table:: {IDF_TARGET_NAME} Series Hardware Connection Pinout
    :header-rows: 1

    * - Function of Connection
      - {IDF_TARGET_NAME} Board or Module Pins
      - Other Device Pins
    * - Download/Log output :sup:`1`
      - UART0
          * GPIO44 (RX)
          * GPIO43 (TX)
      - PC
          * TX
          * RX
    * - AT command/response :sup:`2`
      - UART1
          * GPIO21 (RX)
          * GPIO17 (TX)
          * GPIO20 (CTS)
          * GPIO19 (RTS)
      - USB to serial converter
          * TX
          * RX
          * RTS
          * CTS

  **Note** 1: Connection between individual pins of the {IDF_TARGET_NAME} board and the PC is already established internally on the {IDF_TARGET_NAME} board. You only need to provide USB cable between the board and PC.

  **Note** 2: Connection between CTS/RTS is optional, depending on whether you want to use hardware flow control.

  .. figure:: ../../_static/get_started/hw_connection/esp32-s2-hw-connection.jpg
    :align: center
    :alt: {IDF_TARGET_NAME} Series Hardware Connection
    :figclass: align-center

    {IDF_TARGET_NAME} Series Hardware Connection

  If you want to connect your device directly with {IDF_TARGET_NAME} module rather than the {IDF_TARGET_NAME} board that integrates it, please refer to `Datasheet for your module <https://www.espressif.com/zh-hans/support/documents/technical-documents>`_ for more details.
