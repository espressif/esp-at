AT FAQ
======

:link_to_translation:`zh_CN:[中文]`

.. contents::
   :local:
   :depth: 2

AT Firmware
-----------

There is no released firmware for my module. Where can I get the firmware for it?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If there is no released fimware for your module in the :doc:`AT_Binary_Lists/index` chapter, please consider the following options:

- Use the firmware for the module that has the same hardware configuration as yours (see :doc:`Compile_and_Develop/esp-at_firmware_differences` for which module has the same configuration).
- Refer to :ref:`firmware-selection`.

How to get the source code of AT firmware?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  The esp-at project is distributed as a combination of source code and pre-compiled libraries in this repository. The pre-compiled core libraries (under the directory: ``esp-at/components/at/lib/``) are closed-source, and there is no plan for open-sourcing them.

How to download the AT firmware on Espressif's official website?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - Download the flash tool: `Flash Download Tools <https://www.espressif.com/en/support/download/other-tools>`_.
  - See :doc:`AT Downloading Guide <Get_Started/Downloading_guide>` for the download address.

How to combine all the bin files compiled by ESP-AT?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  You can use the **combine** button of the `Flash Download Tools <https://www.espressif.com/en/support/download/other-tools>`_.

.. only:: esp32

  Why is the error "flash read err,1000" printed on the serial port after powering up the newly purchased ESP32-WROVE-E module? How to use AT commands for this module?
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    - The ESP32-WROVER-E module is shipped without AT firmware, so the error "flash read err" appears.
    - If you want to use the AT command function of ESP32-WROVER-E, please refer to the following links to get the firmware and flash it.

      - :ref:`Download firmware <firmware-esp32-wrover-32-series>`;
      - :ref:`Connect hardware <hw-connection-esp32-wrover-series>`;
      - :ref:`Flash firmware <flash-at-firmware-into-your-device>`.

Does the AT firmware shipped in modules support flow control?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - Hardware flow control is supported, but software flow control is not.
  - To enable or disable hardware flow control, run :ref:`AT+UART_CUR <cmd-UARTC>` or :ref:`AT+UART_DEF <cmd-UARTD>`. 
  - See :doc:`Hardware connection <Get_Started/Hardware_connection>` for more details.

AT Commands and Responses
-------------------------

What is the reason why AT prompts busy?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - The "busy" prompt indicates that the previous command is being executed, and the system cannot respond to the current input. The processing mechanism of the AT commands is serial, i.e. one command at a time. 
  - Any input through serial ports is considered to be a command input, so the system will also prompt "busy" or "ERROR" when there is any extra invisible character input.

    - Serial input AT+GMR (change character CR LF) (space character), because AT+GMR (change character CR LF) is already a complete AT command, the system will execute the command. At this time, if the system has not completed the AT+GMR operation, it has received the following space character, which will be regarded as a new command input, and the system will prompt "busy". But if the system has completed the AT+GMR operation, and then receives the following space character, the space character will be regarded as an error command, and the system will prompt "ERROR".
    - After the MCU sends AT+CIPSEND and receives the busy p.. response, the MCU needs to resend the data. Because busy p.. represents the previous command is being executed, the current input is invalid. It is recommended to wait for the response of the last AT command before the MCU sends a new command again.

Why does the ESP-AT firmware always return the following message after the I powered up the device and sent the first command?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  .. code-block:: text

    ERR CODE:0x010b0000
    busy p...

  - This message means that the previous command is being executed.
  - Normally only "busy p..." is displayed. The ERR CODE is displayed because the error code prompt is enabled.
  - If you receive this message after sending the first command on power-up, the possible reasons are: the command is followed by the unnecessary newline/space/other symbols; or two or more AT commands are sent in succession.

What commands are supported by the default ESP-AT firmware on different modules, and from which version are they supported?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - To learn what commands are supported by the default ESP-AT firmware on different modules, please refer to :doc:`Compile_and_Develop/esp-at_firmware_differences`.
  - To learn from which version a command is supported and what issues are fixed in each version, please refer to `release notes <https://github.com/espressif/esp-at/releases>`_.

When the host MCU sends an AT command to the {IDF_TARGET_NAME} device, there is no response. What is the reason?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  A terminator ("AT\\r\\n") must be added after an AT command when the host MCU sending AT commands to an {IDF_TARGET_NAME} device. Please see :ref:`check-whether-at-works`.

Why is Wi-Fi disconnected (``WIFI DISCONNECT`` printed)?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  You can check the Wi-Fi disconnection reason code on the :term:`AT log port`, which usually prints ``wifi disconnected, rc:<reason_code>``. The ``<reason_code>`` here refers to `Wi-Fi Reason Code <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#wi-fi-reason-code>`_.

What are the common Wi-Fi compatibility issues?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - AMPDU compatibility issue. 
 
    - If the router does not support AMPDU, {IDF_TARGET_NAME} will automatically disable the AMPDU function when interacting with the router. 
    - If the router supports AMPDU but there is a compatibility issue with AMPDU transmission between the router and {IDF_TARGET_NAME}, it is recommended to disable the function on the router or {IDF_TARGET_NAME}. For information on how to disable it on {IDF_TARGET_NAME}, please refer to :doc:`Compile_and_Develop/How_to_clone_project_and_compile_it` and select the following options in the fifth step of configuring the project:

      - Disable ``Component config`` -> ``Wi-Fi`` -> ``WiFi AMPDU TX``
      - Disable ``Component config`` -> ``Wi-Fi`` -> ``WiFi AMPDU RX``

  - Phy mode compatibility issue. If there is a compatibility issue with the phy mode between the router and {IDF_TARGET_NAME}, it is recommended to switch it on the router or {IDF_TARGET_NAME}. For how to switch it on {IDF_TARGET_NAME}, please refer to the :ref:`AT+CWSTAPROTO <cmd-STAPROTO>` command.

Do AT commands support ESP-WIFI-MESH?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  Currently, AT commands do not support ESP-WIFI-MESH.

.. Are there any examples of using AT commands to connect to aliyun or Tencent Cloud?
.. ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
..
..  - Download and flash :doc:`AT firmware <AT_Binary_Lists/index>`.
..  - Aliyun: `AT+MQTT aliyun <https://blog.csdn.net/espressif/article/details/107367189>`_.
..  - Tencent Cloud: `AT+MQTT QCloud <https://blog.csdn.net/espressif/article/details/104714464>`_.
..

Can AT command set Bluetooth LE transmit power?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  Yes, the :ref:`AT+RFPOWER <cmd-RFPOWER>` command can set Bluetooth LE transmit power. {IDF_TARGET_NAME} Wi-Fi and Bluetooth LE share the same antenna.

.. only:: esp32

  Is it possible to set the ESP32-WROOM-32 module to HID keyboard mode with AT commands?
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    Yes, please refer to :doc:`Bluetooth LE AT Commands <AT_Command_Set/BLE_AT_Commands>`.

How to support commands that are not supported by the default firmware but can be supported after configuring and compiling the ESP-AT project?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  For example, if you need to support the  WPA2 Enterprise function on the {IDF_TARGET_NAME} series, configure and compile the firmware by yourself. Open the  WPA2 Enterprise function in menuconfig when compiling: ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``[*]AT WPA2 Enterprise command support``. 

How to handle special characters in AT commands?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  Please refer to the escape character syntax described in the :ref:`at-command-types` section.

Can the serial port baudrate be modified in AT Commands? (Default: 115200)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  Yes, you can use either of the two ways below to modify it: 

  - Use the command :ref:`AT+UART_CUR <cmd-UARTC>` or :ref:`AT+UART_DEF <cmd-UARTD>`.
  - Re-compile the AT firmware: :doc:`establish the compiling environment <Compile_and_Develop/How_to_clone_project_and_compile_it>` and :doc:`change the UART baudrate <Compile_and_Develop/How_to_set_AT_port_pin>`.

After {IDF_TARGET_NAME} enters the passthrough mode using AT commands, can {IDF_TARGET_NAME} give a message if the connected hotspot is disconnected?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - Yes, you can configure it with :ref:`AT+SYSMSG <cmd-SYSMSG>`, i.e., set AT+SYSMSG=4. In this way, the serial port will report ``WIFI DISCONNECT\\r\\n`` when the connected hotspot is disconnected.
  - Note that this command is added after AT v2.1.0. It is not available for v2.1.0 and earlier versions.

.. only:: esp32

  How to set ADV broadcast parameters after it exceeds 31 bytes?
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    The :ref:`AT+BLEADVDATA <cmd-BADVD>` command supports up to 31 bytes of ADV broadcast parameters. If you need to set a bigger parameter, please use command :ref:`AT+BLESCANRSPDATA <cmd-BSCANR>`.

How to enable the notify and indicate functions on Bluetooth LE clients?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - Besides the read and write properties, Bluetooth LE characteristics also have the ``notify`` and ``indicate`` properties, which allow the server to send data to the client, but the client must first register for ``notification`` by writing the value of "CCCD".
  - To enable ``notify``, write ``0x01``. To enable ``indicate``, write ``0x02`` (for the descriptor "0x2902"). To enable both ``notify`` and ``indicate``, write ``0x03``.
  - The example below demonstrates how to enable the ``notify`` and ``indicate`` properties for the descriptor ``0x2902`` under the services ``0xC305`` and ``0xC306`` respectively. The two services are default services in ESP-AT, of which ``0xC305`` can be notified and ``0xC306`` can be indicated. 

  .. code-block:: text

    AT+BLEGATTCWR=0,3,6,1,2
    >
    // Write 0x01
    OK
    // Server: +WRITE:0,1,6,1,2,<0x01>,<0x00>
    AT+BLEGATTCWR=0,3,7,1,2
    >
    // Write 0x02
    OK
    // Server: +WRITE:0,1,6,1,2,<0x02>,<0x00>
    // Writing ccc is a prerequisite for the server to be able to send notify and indicate

Hardware
--------

How big is the chip flash required for ESP-AT firmware on different modules?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - For {IDF_TARGET_NAME} series modules, please refer to :doc:`ESP-AT Firmware Differences <Compile_and_Develop/esp-at_firmware_differences>`.

How to view the error log of AT firmware?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  .. list::

    :esp32: - For {IDF_TARGET_NAME}, the error log is output through the download port. By default, UART0 is GPIO1 and GPIO3.
    :esp32c2 or esp32c3: - For {IDF_TARGET_NAME}, the error log is output through the download port. By default, UART0 is GPIO21 and GPIO20.
    - See :doc:`Get_Started/Hardware_connection` for more details.

The UART1 communication pin used by ESP-AT on the {IDF_TARGET_NAME} module is inconsistent with the default UART1 pin described in the {IDF_TARGET_NAME} module's datasheet?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - {IDF_TARGET_NAME} supports IO matrix. When compiling ESP-AT, you can configure UART1 pins in menuconfig, so they may be inconsistent with the pins described in the module datasheet.
  - See :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` for more details. 

Performance
-----------

How long does it take for the AT to connect to Wi-Fi?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - In an office scenario, the connection time is 5 seconds. However, in actual practice, Wi-Fi connection time depends on the router performance, network environment, module antenna performance, etc.
  - The maximum timeout time can be set by the **<jap_timeout>** parameter of :ref:`AT+CWJAP <cmd-JAP>`. 

Is it possible to change the TCP send window size in AT firmware?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - Currently, it cannot be changed by AT commands, but you can configure and compile the ESP-AT project to generate a new firmware.
  - You can configure the menuconfig parameter: ``Component config`` > ``LWIP`` > ``TCP`` > ``Default send buffer size``.

How to test and optimize the throughput of {IDF_TARGET_NAME} AT?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - Many factors are affecting the AT throughput test. It is recommended to use the iperf example in esp-idf for testing. While testing, please use the passthrough mode, adjust the data length to 1460 bytes, and send data continuously.
  - If the test rate does not meet your requirements, please refer to :doc:`Compile_and_Develop/How_to_optimize_throughput`.

.. only:: esp32

  What is the maximum rate of {IDF_TARGET_NAME} AT default firmware Bluetooth LE UART transparent transmission? 
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    In an open office environment, when the serial port baud rate is 2000000, the average transmission rate of ESP-AT Bluetooth is 0.56 Mbps, and the average transmission rate of ESP-AT Bluetooth LE is 0.101 Mbps.

Other
-----

What interfaces of {IDF_TARGET_NAME} chips can be used to transmit AT commands?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  .. list::

    :esp32: - {IDF_TARGET_NAME} can transmit AT commands through UART and SDIO.
    :esp32c2 or esp32c3 or esp32c6: - {IDF_TARGET_NAME} can transmit AT commands through UART and SPI.
    - The default firmware uses UART for transmission. If you need SDIO or SPI interface to transmit AT commands, you can configure it through ``./build.py menuconfig`` > ``Component config`` > ``AT`` when compiling the ESP-AT project by yourself.
    - See :project_file:`AT through SDIO <main/interface/sdio/README.md>`, :project_file:`AT through SPI <main/interface/spi/README.md>`, or :project_file:`AT through socket <main/interface/socket/README.md>` for more details.

.. only:: esp32

  How to use the Ethernet function of the {IDF_TARGET_NAME} AT?
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    The Ethernet function is disable in AT default firmware, if you need to enable the Ethernet function, please refer to :doc:`How to Enable ESP-AT Ethernet <Compile_and_Develop/How_to_enable_ESP_AT_Ethernet>`.

.. only:: esp32

How does ESP-AT conduct BQB certification?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  Please contact `Espressif <https://www.espressif.com/en/contact-us/sales-questions>`_ for solutions.

How do I specify the TLS protocol version for {IDF_TARGET_NAME} AT?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  When compiling the esp-at project, you can disable the unwanted versions in the ``./build.py menuconfig`` > ``Component config`` > ``mbedTLS``.

How to modify the number of TCP connections in AT?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - At present, the maximum number of TCP connections of the AT default firmware is 5.
  - The {IDF_TARGET_NAME} AT supports a maximum of 16 TCP connections, which can be configured in menuconfig as follows:
    
    - ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``(16)AT socket maximum connection number``
    - ``./build.py menuconfig`` > ``LWIP`` > ``(16)Max number of open sockets``

.. only:: esp32

Does {IDF_TARGET_NAME} AT support PPP?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - Not supported, please refer to `pppos_client <https://github.com/espressif/esp-idf/tree/v4.4.2/examples/protocols/pppos_client>`_ demos for your own implementation.

How to enable debug log for AT?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - Enable log level: ``./build.py menuconfig`` > ``Component Config`` > ``Log output`` > ``Default log verbosity`` set to ``Debug``.

    - Enable Wi-Fi debug: ``./build.py menuconfig`` > ``Component config`` > ``Wi-Fi`` > ``Wi-Fi debug log level`` set to ``Debug``.
    - Enable TCP/IP debug: ``./build.py menuconfig`` > ``Component config`` > ``LWIP`` > ``Enable LWIP Debug`` > Set the log level of the specific part you want to debug to ``Debug``.
    - Enable Bluetooth LE debug: ``./build.py menuconfig`` > ``Component config`` > ``Bluetooth`` > ``Bluedroid Options`` > ``Disable BT debug logs`` > ``BT DEBUG LOG LEVEL`` > Set the log level of the specific part you want to debug to ``Debug``.

How does the AT command implement the functionality of resuming HTTP transfers after interrupts?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - Currently, AT commands provide two methods:

    - Specify the data range to be read using the HTTP Range field. For specific details, please refer to the example of :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD_example>`.
    - You can construct an HTTP GET request using AT TCP series commands. Between steps 6 and 7 of the example :ref:`{IDF_TARGET_NAME} obtains socket data in passive receiving mode <example-passive_recv>`, add a step: Use the :ref:`AT+CIPSEND <cmd-SEND>` command to send your own HTTP GET request header to the server. In passive receive mode, for HTTP GET request data received from the server, the MCU needs to actively send the :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>` command to read the data. This helps avoid situations where the MCU may be unable to process data promptly due to large amounts of data being transferred from the server.
