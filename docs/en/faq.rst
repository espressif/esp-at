AT FAQ
======

:link_to_translation:`zh_CN:[中文]`

.. _faq-at-index:

  - :ref:`AT Documentation <faq-at-doc>`

    .. list::

      - :ref:`How to select the correct AT documentation version? <faq-choose-doc-version>`

  - :ref:`AT Firmware <faq-at-firmware>`

    .. list::

      - :ref:`There is no released firmware for my module <faq-no-official-firmware>`
      - :ref:`How to get the source code of AT firmware? <faq-at-source-code>`
      - :ref:`How to download the AT firmware? <faq-download-firmware>`
      - :ref:`How to combine all the bin files compiled by ESP-AT? <faq-combine-bins>`
      - :ref:`Why does a newly purchased ESP series module print "flash read err" error after power-up? How to use AT commands? <faq-esp-series-flash-error>`
      - :ref:`Does the AT firmware shipped in modules support flow control? <faq-flow-control>`

  - :ref:`AT Commands and Responses <faq-at-cmd>`

    .. list::

      - :ref:`Why does AT return busy p...? <faq-busy-error>`
      - :ref:`Why does the first command after power-up return an error? <faq-first-cmd-busy>`
      - :ref:`What commands are supported by default AT firmware? <faq-cmd-support>`
      - :ref:`MCU sends AT command with no response <faq-no-response>`
      - :ref:`How to handle special characters in AT commands? <faq-special-char>`
      - :ref:`Can the UART baudrate be modified? <faq-change-baudrate>`
      - :ref:`How to modify the UART pins of the AT command port? <faq-change-uart-pins>`
      - :ref:`How to solve TCP/SSL data loss during transmission? <faq-data-loss>`
      - :ref:`Can passthrough mode notify hotspot disconnection? <faq-passthrough-disconnect>`
      - :ref:`Why does Wi-Fi disconnect? <faq-wifi-disconnect>`
      - :ref:`What are the common Wi-Fi compatibility issues? <faq-wifi-compatibility>`
      - :ref:`Do AT commands support ESP-WIFI-MESH? <faq-wifi-mesh>`
      - :ref:`Are there examples for connecting to cloud services? <faq-cloud-examples>`
      :esp32: - :ref:`Set ESP32-WROOM-32 module to HID keyboard mode? <faq-hid-keyboard>`
      :esp32 or esp32c2 or esp32c3 or esp32c5 or esp32c6 or esp32c61: - :ref:`Can AT command set Bluetooth LE transmit power? <faq-ble-tx-power>`
      :esp32 or esp32c2 or esp32c3 or esp32c5 or esp32c6 or esp32c61: - :ref:`How to enable notify and indicate on BLE clients? <faq-ble-notify-indicate>`

  - :ref:`Hardware <faq-hardware>`

    .. list::

      - :ref:`How big is the chip flash required for AT firmware? <faq-flash-size>`
      - :ref:`How to view the error log of AT firmware? <faq-view-error-log>`
      - :ref:`UART1 communication pin inconsistent with datasheet? <faq-uart1-pin>`

  - :ref:`Performance <faq-performance>`

    .. list::

      - :ref:`How long does it take for AT to connect to Wi-Fi? <faq-wifi-conn-time>`
      - :ref:`Is it possible to change the TCP send window size? <faq-tcp-window>`
      - :ref:`How to test and optimize the throughput? <faq-throughput>`
      - :ref:`How to modify TCP segment retransmissions? <faq-tcp-retrans>`

  - :ref:`Other <faq-other>`

    .. list::

      - :ref:`What interfaces can be used to transmit AT commands? <faq-at-interface>`
      - :ref:`How to enable debug log for AT? <faq-debug-log>`
      - :ref:`How to modify the default Wi-Fi mode? <faq-default-wifi-mode>`
      - :ref:`How to implement HTTP resume transfer functionality? <faq-http-resume>`
      - :ref:`How to download files from HTTP server and store them to FATFS, or upload files from FATFS file system to HTTP server? <faq-http-fs>`
      :esp32: - :ref:`How to conduct BQB certification? <faq-bqb>`
      :esp32: - :ref:`Does AT support PPP? <faq-ppp>`

.. _faq-at-doc:

:ref:`AT Documentation <faq-at-index>`
---------------------------------------

.. _faq-choose-doc-version:

:ref:`How to select the correct AT documentation version? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Please select the corresponding chip platform and documentation version based on the actual AT firmware version you are using; there may be functional and behavioral differences between different firmware versions.

- For :ref:`official-released-firmware`, you can directly obtain the corresponding document link through the :ref:`AT+USERDOCS? <cmd-USERDOCS>` command, without needing to confirm the version information.
- For non-official released firmware, please confirm the following information first, and then select the corresponding document version:

  - The chip platform of the AT firmware you are using (e.g., ESP32, ESP32-C3, etc.)
  - The version number of the AT firmware (you can use the :ref:`AT+GMR <cmd-GMR>` command to view the version number of the AT core library)

  .. figure:: ../_static/faq/choose_target_version_en.png
      :scale: 100 %
      :align: center
      :alt: Choose AT documentation target and version

.. _faq-at-firmware:

:ref:`AT Firmware <faq-at-index>`
----------------------------------

.. _faq-no-official-firmware:

:ref:`There is no released firmware for my module. Where can I get the firmware for it? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If there is no released firmware for your module in :doc:`AT_Binary_Lists/index`, consider:

  - Use the firmware for a module with the same hardware configuration (see :doc:`Compile_and_Develop/esp-at_firmware_differences`)
  - Refer to :ref:`firmware-selection`

.. _faq-at-source-code:

:ref:`How to get the source code of AT firmware? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The esp-at project is distributed as a combination of source code and pre-compiled libraries in this repository. The pre-compiled core libraries (under the directory: ``esp-at/components/at/lib/``) are closed-source, and there is no plan for open-sourcing them.

.. _faq-download-firmware:

:ref:`How to download the AT firmware on Espressif's official website? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Download the `Flash Download Tool <https://dl.espressif.com/public/flash_download_tool.zip>`_. Refer to the `Flash Download Tool User Guide <https://docs.espressif.com/projects/esp-test-tools/en/latest/{IDF_TARGET_PATH_NAME}/production_stage/tools/flash_download_tool.html>`_ for instructions. See :doc:`AT Downloading Guide <Get_Started/Downloading_guide>` for the download address.

.. _faq-combine-bins:

:ref:`How to combine all the bin files compiled by ESP-AT? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

No manual combination is required. The released and compiled firmware already includes pre-combined 2MB/4MB bin files in the ``build/factory`` directory.

.. _faq-esp-series-flash-error:

:ref:`Why does a newly purchased ESP series module print "flash read err" error after power-up? How to use AT commands? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ESP series modules are shipped without ESP-AT firmware in Flash, so the error "flash read err" is reported after power-up. To use AT commands, refer to :doc:`AT Downloading Guide <Get_Started/Downloading_guide>`.

.. _faq-flow-control:

:ref:`Does the AT firmware shipped in modules support flow control? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Hardware flow control is supported, but software flow control is not. Configure hardware flow control using :ref:`AT+UART_CUR <cmd-UARTC>` or :ref:`AT+UART_DEF <cmd-UARTD>`. See :doc:`Hardware connection <Get_Started/Hardware_connection>` for wiring details.

.. _faq-at-cmd:

:ref:`AT Commands and Responses <faq-at-index>`
------------------------------------------------

.. _faq-busy-error:

:ref:`Why does AT prompt busy p...? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The "busy p..." prompt indicates that the previous command is being executed, and the system cannot respond to the current input. AT commands are processed serially, so you must wait for the previous command to complete before receiving the next one.

When there are extra invisible characters (such as spaces after a command), the system will also prompt "busy p..." or "ERROR", because any serial port input is considered a command input.

If the MCU sends AT+CIPSEND and receives a busy p... response, it means the previous command is still executing and the current input is invalid. Wait for the previous command response to complete before sending a new command.

.. _faq-first-cmd-busy:

:ref:`Why does the AT firmware always return the following message after I power up the device and sent the first command? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: text

  ERR CODE:0x010b0000
  busy p...

This message means "the previous command is being executed". Normally only "busy p..." is displayed. The ERR CODE is shown because the error code prompt is enabled.

If you receive this error on the first command after power-up, possible reasons:

  - The command is followed by unnecessary newline/space/other symbols
  - Multiple AT commands were sent in succession

.. _faq-cmd-support:

:ref:`What commands are supported by the default AT firmware on different modules, and from which version are they supported? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Commands supported by default firmware on different modules: refer to :doc:`Compile_and_Develop/esp-at_firmware_differences`

Command version support and fixes: refer to `release notes <https://github.com/espressif/esp-at/releases>`_

.. _faq-no-response:

:ref:`When the host MCU sends an AT command to the {IDF_TARGET_NAME} device, there is no response. What is the reason? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A terminator ("AT\\r\\n") must be added after an AT command. See :ref:`check-whether-at-works`.

.. _faq-special-char:

:ref:`How to handle special characters in AT commands? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Refer to the escape character syntax described in the :ref:`at-command-types` section.

.. _faq-change-baudrate:

:ref:`Can the serial port baudrate be modified in AT Commands? (Default: 115200) <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  Yes, you can use either of the three ways below to modify it: 

  - Use the command :ref:`AT+UART_CUR <cmd-UARTC>` or :ref:`AT+UART_DEF <cmd-UARTD>`.
  - Re-compile the AT firmware: :doc:`establish the compiling environment <Compile_and_Develop/How_to_clone_project_and_compile_it>` and :doc:`change the UART baudrate <Compile_and_Develop/How_to_set_AT_port_pin>`.
  - Use the :ref:`AT+SYSMFG <cmd-SYSMFG>` command to modify the factory default UART configuration, please refer to :ref:`AT+SYSMFG command examples <sysmfg-uart-config>`.

.. _faq-change-uart-pins:

:ref:`How to modify the UART pins of the AT command port? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  You can use either of the three ways below to modify it:

  - Please refer to the :doc:`How to set AT port pins <Compile_and_Develop/How_to_set_AT_port_pin>` document. Modify the UART configuration parameters in the ``factory_param_data.csv`` file, then recompile and flash the firmware.
  - If you want to modify the UART configuration at runtime through AT commands without recompiling the firmware, please refer to :ref:`AT+SYSMFG command examples <sysmfg-uart-config>`. This method allows you to dynamically modify the factory default UART configuration, including baudrate, pins, and other parameters.
  - If you already have AT firmware and only need to modify the UART pins, you can use the :doc:`at.py tool <Compile_and_Develop/tools_at_py>` to directly modify the firmware parameters without recompiling.

.. _faq-data-loss:

:ref:`How to solve TCP/SSL data loss during transmission? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If data loss occurs without hardware flow control enabled, it is recommended to enable hardware flow control (:ref:`AT+UART_CUR <cmd-UARTC>` or :ref:`AT+UART_DEF <cmd-UARTD>`) to verify if it is caused by the lack of flow control. If the problem persists after enabling flow control, please refer to :ref:`Debugging the Data Sending and Receiving Process of TCP Connections <debug-at_examples>` for further troubleshooting; if it is confirmed to be an issue caused by AT, please submit an issue on `esp-at/issues <https://github.com/espressif/esp-at/issues>`_.

.. _faq-passthrough-disconnect:

:ref:`After {IDF_TARGET_NAME} enters the passthrough mode using AT commands, can {IDF_TARGET_NAME} give a message if the connected hotspot is disconnected? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Configure with :ref:`AT+SYSMSG <cmd-SYSMSG>`, set AT+SYSMSG=4. The serial port will report ``WIFI DISCONNECT\\r\\n`` when the connected hotspot is disconnected.

Note: This command was added after AT v2.1.0.0. It is not available for v2.1.0.0 and earlier versions.

.. _faq-wifi-disconnect:

:ref:`Why is Wi-Fi disconnected (WIFI DISCONNECT)? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Check the disconnection reason on :term:`AT log port`, which prints ``wifi disconnected, rc:<reason_code>``.
See `Wi-Fi Reason Code <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#wi-fi-reason-code>`_ for details.

Generic firmware may not provide detailed reasons. To obtain specific information, register the Wi-Fi disconnect event in code and output reason code.

.. _faq-wifi-compatibility:

:ref:`What are the common Wi-Fi compatibility issues? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- AMPDU compatibility issue. 
 
  - If the router does not support AMPDU, {IDF_TARGET_NAME} will automatically disable the AMPDU function when interacting with the router. 
  - If the router supports AMPDU but there is a compatibility issue with AMPDU transmission between the router and {IDF_TARGET_NAME}, it is recommended to disable the function on the router or {IDF_TARGET_NAME}. For information on how to disable it on {IDF_TARGET_NAME}, please refer to :doc:`Compile_and_Develop/How_to_clone_project_and_compile_it` and select the following options in the fifth step of configuring the project:

    - Disable ``Component config`` > ``Wi-Fi`` > ``WiFi AMPDU TX``
    - Disable ``Component config`` > ``Wi-Fi`` > ``WiFi AMPDU RX``

- Phy mode compatibility issue. If there is a compatibility issue with the phy mode between the router and {IDF_TARGET_NAME}, it is recommended to switch it on the router or {IDF_TARGET_NAME}. For how to switch it on {IDF_TARGET_NAME}, please refer to the :ref:`AT+CWSTAPROTO <cmd-STAPROTO>` command.

.. _faq-wifi-mesh:

:ref:`Do AT commands support ESP-WIFI-MESH? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Currently, AT commands do not support ESP-WIFI-MESH.

  .. _faq-cloud-examples:

  .. :ref:`Are there any examples of using AT commands to connect to aliyun or Tencent Cloud? <faq-at-index>`
  .. ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  ..
  ..  - Download and flash :doc:`AT firmware <AT_Binary_Lists/index>`.
  ..  - Aliyun: `AT+MQTT aliyun <https://blog.csdn.net/espressif/article/details/107367189>`_.
  ..  - Tencent Cloud: `AT+MQTT QCloud <https://blog.csdn.net/espressif/article/details/104714464>`_.
  ..

.. only:: esp32

  .. _faq-hid-keyboard:

  :ref:`Is it possible to set the ESP32-WROOM-32 module to HID keyboard mode with AT commands? <faq-at-index>`
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  Yes, refer to :doc:`Bluetooth LE AT Commands <AT_Command_Set/BLE_AT_Commands>`.

.. only:: esp32 or esp32c2 or esp32c3 or esp32c5 or esp32c6 or esp32c61

  .. _faq-ble-tx-power:

  :ref:`Can AT command set Bluetooth LE transmit power? <faq-at-index>`
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  Yes, use the :ref:`AT+RFPOWER <cmd-RFPOWER>` command to set Bluetooth LE transmit power. {IDF_TARGET_NAME} Wi-Fi and Bluetooth LE share the same antenna.

  .. _faq-ble-notify-indicate:

  :ref:`How to enable the notify and indicate functions on Bluetooth LE clients? <faq-at-index>`
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - Besides the read and write properties, Bluetooth LE characteristics also have the ``notify`` and ``indicate`` properties, which allow the server to send data to the client, but the client must first register for ``notification`` by writing the value of "CCCD".
  - To enable ``notify``, write ``0x01``. To enable ``indicate``, write ``0x02`` (for the descriptor "0x2902"). To enable both ``notify`` and ``indicate``, write ``0x03``.
  - The example below demonstrates how to enable the ``notify`` and ``indicate`` properties for the descriptor ``0x2902`` under the services ``0xC305`` and ``0xC306`` respectively. ESP-AT includes these two services by default. You can notify ``0xC305`` and indicate ``0xC306``.

    .. code-block:: text

      AT+BLEGATTCWR=0,3,6,1,2
      >
      // Write low byte 0x01 high byte 0x00 (if you want to use hex format, it is: 0100)
      OK
      // Server: +WRITE:0,1,6,1,2,<0x01>,<0x00>
      AT+BLEGATTCWR=0,3,7,1,2
      >
      // Write low byte 0x02 high byte 0x00 (if you want to use hex format, it is: 0200)
      OK
      // Server: +WRITE:0,1,6,1,2,<0x02>,<0x00>
      // Writing ccc is a prerequisite for the server to be able to send notify and indicate

  .. _faq-hardware:

.. only:: esp32s2

    .. _faq-hardware:

:ref:`Hardware <faq-at-index>`
-------------------------------

.. _faq-flash-size:

:ref:`How big is the chip flash required for AT firmware on different modules? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Refer to :doc:`ESP-AT Firmware Differences <Compile_and_Develop/esp-at_firmware_differences>`.

.. _faq-view-error-log:

:ref:`How to view the error log of AT firmware? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Usually check the error log from :term:`AT log port`.

.. _faq-uart1-pin:

:ref:`The UART1 communication pin used by AT on the {IDF_TARGET_NAME} module is inconsistent with the default UART1 pin described in the {IDF_TARGET_NAME} module's datasheet? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

{IDF_TARGET_NAME} supports IO matrix. When compiling ESP-AT, you can configure UART1 pins in menuconfig, so they may be inconsistent with the pins described in the module datasheet.
See :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` for details. 

.. _faq-performance:

:ref:`Performance <faq-at-index>`
----------------------------------

.. _faq-wifi-conn-time:

:ref:`How long does it take for AT to connect to Wi-Fi? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In an office scenario, AT takes about 5 seconds to connect. Actual connection time depends on router performance, network environment, module antenna performance, etc.
The maximum timeout can be set using the **<jap_timeout>** parameter of :ref:`AT+CWJAP <cmd-JAP>`. 

.. _faq-tcp-window:

:ref:`Is it possible to change the TCP send window size in AT firmware? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

It cannot be changed by AT commands. Re-compile the firmware and configure in menuconfig: ``Component config`` > ``LWIP`` > ``TCP`` > ``Default send buffer size``.

.. _faq-throughput:

:ref:`How to test and optimize the AT throughput? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

It is recommended to use the iperf example in esp-idf for testing (use passthrough mode, adjust data length to 1460 bytes, and send data continuously).
If the test rate does not meet your requirements, refer to :doc:`Compile_and_Develop/How_to_optimize_throughput`.

.. _faq-tcp-retrans:

:ref:`How to modify the default maximum number of TCP segment retransmissions for AT? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, the maximum number of TCP segment retransmissions is 6 (range: [3-12]). Reconfigure it as follows:

- **Local compilation**: Refer to :doc:`compile ESP-AT project locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>`, configure in menuconfig:
  
  ::

    Component config > LWIP > TCP > Maximum number of retransmissions of data segments

- **Web compilation**: Refer to :doc:`compile ESP-AT project on the webpage <../Compile_and_Develop/How_to_build_project_with_web_page>`, modify the value of `CONFIG_LWIP_TCP_MAXRTX <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-reference/kconfig.html#config-lwip-tcp-maxrtx>`_.

.. _faq-other:

:ref:`Other <faq-at-index>`
----------------------------

.. _faq-at-interface:

:ref:`What interfaces of {IDF_TARGET_NAME} chips can be used to transmit AT commands? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. list::

  :esp32 or esp32c5: - {IDF_TARGET_NAME} can transmit AT commands through UART and SDIO
  :esp32c2 or esp32c3 or esp32c5 or esp32c6 or esp32c61: - {IDF_TARGET_NAME} can transmit AT commands through UART and SPI
  - The default firmware uses UART for transmission. If you need SDIO or SPI interface, configure it when compiling the ESP-AT project yourself. See :doc:`Compile_and_Develop/index` for details
  - Reference documents: :project_file:`AT through SDIO <main/interface/sdio/README.md>`, :project_file:`AT through SPI <main/interface/spi/README.md>`, :project_file:`AT through socket <main/interface/socket/README.md>`

.. _faq-debug-log:

:ref:`How to enable debug log for AT? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Refer to the :doc:`How to enable more AT debug logs <../Compile_and_Develop/How_to_enable_more_AT_debug_logs>` document.

.. _faq-default-wifi-mode:

:ref:`How to modify the default Wi-Fi mode when AT firmware starts for the first time? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Modify the source code. Call `esp_wifi_set_mode() <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-reference/network/esp_wifi.html#_CPPv417esp_wifi_set_mode11wifi_mode_t>`_ in the :cpp:type:`esp_at_ready_before()` function to set the Wi-Fi mode, for example: ``esp_wifi_set_mode(WIFI_MODE_STA)``.

.. _faq-http-resume:

:ref:`How does the AT command implement the functionality of resuming HTTP transfers after interrupts? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Two methods:

  - Use the HTTP Range field to specify the data range to be read. Refer to the example of :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD_example>`
  - Construct an HTTP GET request using AT TCP commands. Between steps 6 and 7 of :ref:`{IDF_TARGET_NAME} obtains socket data in passive receiving mode <example-passive_recv>`, use :ref:`AT+CIPSEND <cmd-SEND>` to send your own HTTP GET request. In passive receive mode, use :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>` to read data

.. _faq-http-fs:

:ref:`How to download files from HTTP server and store them to FATFS, or upload files from FATFS file system to HTTP server? <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Refer to GitHub examples:

  - :example:`at_http_get_to_fs` - Download files from HTTP server and store them to file system
  - :example:`at_fs_to_http_server` - Upload files from file system to HTTP server

.. only:: esp32

  .. _faq-bqb:

  :ref:`How to conduct BQB certification? <faq-at-index>`
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  Please contact `Espressif <https://www.espressif.com/en/contact-us/sales-questions>`_ for solutions.

  .. _faq-ppp:

  :ref:`Does AT support PPP? <faq-at-index>`
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  Not supported, refer to `pppos_client <https://github.com/espressif/esp-idf/tree/v4.4.2/examples/protocols/pppos_client>`_ demos for your own implementation.
