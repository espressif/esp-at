How to Enable More AT Debug Logs
==================================

:link_to_translation:`zh_CN:[中文]`

This article mainly introduces how to enable log configuration options. To obtain more log information for debugging ESP-AT functionalities, please refer to the guide to :doc:`compile the ESP-AT project <../Compile_and_Develop/How_to_clone_project_and_compile_it>`, and in step three, set the silence mode to ``No``. In step five, based on your actual situation, enable the specific logging configuration options.

.. list::

  * :ref:`AT Wi-Fi Function Debugging <debug-at_wifi>`
  * :ref:`AT Network Function Debugging <debug-at_network>`
  :not esp32s2: * :ref:`AT BLE Function Debugging <debug-at_ble>`
  * :ref:`AT Interface Function Debugging <debug-at_interface>`
  * :ref:`Debugging Examples <debug-at_examples>`

The final debugging logs will be output from the :term:`AT log port`. Please refer to the :doc:`Hardware Connection <../Get_Started/Hardware_connection>` document to find the TX GPIO pin corresponding to the **Output Log** and use a serial tool to view the logs on that pin.

.. note::
  Enabling log configuration options will significantly increase the log output. Therefore, it is recommended to first confirm which module the issue belongs to and then enable the corresponding module's log configuration. This prevents issues such as being unable to reproduce or encountering new problems like ``task wdt`` due to excessive log output.

  Enabling log configuration options will increase the firmware size. This may lead to compilation failures. To resolve this, you can reduce the firmware size by disabling unused AT functionalities via the configuration menu: ``python build.py menuconfig`` > ``Component config`` > ``AT``.

.. _debug-at_wifi:

AT Wi-Fi Function Debugging
--------------------------------

- You can enable the following configurations to turn on logging for Wi-Fi packet statistics:

  ::

    Component config > Log > Log Level > Default log verbosity > Info (or Info and above levels)
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Periodically dump Wi-Fi statistics
    Component config > AT > Enable ESP-AT Debug > Periodically dump Wi-Fi statistics > The interval of dumping Wi-Fi statistics (ms)

- You can enable the following configurations to turn on debugging logs for the Wi-Fi interaction process:

  .. only:: esp32 or esp32c2 or esp32c3 or esp32s2

    ::

      Component config > Log > Log Level > Default log verbosity > Debug
      Component config > Supplicant > Print debug messages from WPA Supplicant

  .. only:: esp32c5 or esp32c6

    ::

      Component config > Log > Log Level > Default log verbosity > Debug
      Component config > Wi-Fi > Print debug messages from WPA Supplicant

.. note::

  When analyzing logs, you may encounter Wi-Fi reason codes. For the specific meanings, please refer to the `Wi-Fi Reason Code <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#wi-fi-reason-code>`_.

.. _debug-at_network:

AT Network Function Debugging
----------------------------------------

.. contents::
   :local:
   :depth: 1

.. _debug-at_tcp:

TCP Debugging
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  To monitor outgoing (TX) and incoming (RX) packets for TCP or TCP-based protocols (such as SSL, HTTP, MQTT, WebSocket, etc.), enable the following configuration. The final logs will print TCP packet information, including IP total length, TCP data length, TCP sequence number, TCP acknowledgment number, TCP source port, destination port, and TCP flags. The TCP flags are cumulative.

  ::

    Component config > Log > Log Level > Default log verbosity > Info (or Info and above levels)
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the TCP packet debug messages
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the TCP packet debug messages > Specify the list of TCP port numbers to monitor > 0

UDP Debugging
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  To monitor outgoing (TX) and incoming (RX) packets for UDP or UDP-based protocols (such as DHCP, DNS, SNTP, mDNS, etc.), enable the following configuration. The final logs will print UDP packet information, including IP total length, source port, destination port, and UDP data length.

  ::

    Component config > Log > Log Level > Default log verbosity > Info (or Info and above levels)
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the UDP packet debug messages
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the UDP packet debug messages > Specify the list of outgoing UDP (UDP TX) port numbers to monitor
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the UDP packet debug messages > Specify the list of incoming UDP (UDP RX) port numbers to monitor

SSL Debugging
^^^^^^^^^^^^^^^^^^^^^

  You can enable the following configurations to enable debugging logs for SSL functionality:

  ::

    Component config > Log > Log Level > Default log verbosity > Verbose
    Component config > mbedTLS > Enable mbedTLS debugging
    Component config > mbedTLS > Enable mbedTLS debugging > Set mbedTLS debugging level > Verbose

ICMP Debugging
^^^^^^^^^^^^^^^^^^^^^^^

  You can enable the following configurations to enable debugging logs for the ICMP functionality (:ref:`AT+PING <cmd-CIPPING>`):

  ::

    Component config > Log > Log Level > Default log verbosity > Info (or Info and above levels)
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the ICMP packet debug messages

lwIP Network Debugging
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  If the logs output by enabling the above configurations still do not meet your debugging needs, you can enable the required debugging configurations under the path ``Component config`` > ``LWIP`` > ``Enable LWIP Debug``.

.. _debug-at_ble:

.. only:: not esp32s2

  AT BLE Function Debugging
  -------------------------------

  - You can directly view the disconnection reason code on the :term:`AT log port` to debug the BLE disconnection process.

  .. only:: esp32 or esp32c3

    - You can enable the following configurations to enable debugging logs for BLE scanning, connection, advertising, data transmission, and other interaction processes:

      ::

        Component config > Log > Log Level > Default log verbosity > Debug
        Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL > HCI layer > DEBUG

    - You can enable the following configurations to turn on debugging logs for the BLE pairing process:

      ::

        Component config > Log > Log Level > Default log verbosity > Debug
        Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL > HCI layer > DEBUG
        Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL > SMP layer > DEBUG

    - You can enable the following configurations to turn on BLE GATT layer debugging logs:

      ::

        Component config > Log > Log Level > Default log verbosity > Debug
        Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL > GATT layer > DEBUG

    - You can enable the following configurations to turn on debugging logs for the AT BluFi functionality:

      ::

        Component config > Log > Log Level > Default log verbosity > Debug
        Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL > BLUFI layer > DEBUG

    If the above configurations do not meet your debugging needs, you can enable other required debugging configurations under ``Component config`` > ``Bluetooth`` > ``Bluedroid Options`` > ``BT DEBUG LOG LEVEL``.

  .. only:: esp32c2 or esp32c5 or esp32c6

    - You can enable the following configurations to turn on BLE function debugging logs:

      ::

        Component config > Log > Log Level > Default log verbosity > Debug
        Component config > Bluetooth > NimBLE Options > NimBLE Host log verbosity > Debug logs

AT Interface Function Debugging
-------------------------------------

.. _debug-at_interface:

- When you want to obtain the data sent from AT to MCU through TX at the :term:`AT log port`, you can enable the following configurations:

  ::

    Component config > Log > Log Level > Default log verbosity > Info (or Info and above levels)
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Logging the data sent from AT to MCU (AT ---> MCU)
    Component config > AT > Enable ESP-AT Debug > Logging the data sent from AT to MCU (AT ---> MCU) > The maximum length of the data sent from AT to MCU to be logged > 8192

- When you want to obtain the data sent from MCU to AT through RX at the :term:`AT log port`, you can enable the following configurations:

  ::

    Component config > Log > Log Level > Default log verbosity > Info (or Info and above levels)
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Logging the data received by AT from MCU (AT <---- MCU)
    Component config > AT > Enable ESP-AT Debug > Logging the data received by AT from MCU (AT <---- MCU) > The maximum length of the data received by AT from MCU to be logged > 8192

.. _debug-at_examples:

Debugging Examples
-------------------

Example 1: Debugging the Data Sending and Receiving Process of TCP Connections
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Scene: With a baud rate of 115200 and using full-duplex flow control, the ESP module establishes a TCP connection as a TCP client with the server. It enters transparent transmission mode and sends data to the server.

.. figure:: ../../_static/optimize-throughput.png
   :align: center
   :alt: Data Stream in Throughput
   :figclass: align-center

   Data Stream

As shown by the arrows in the figure:

  - The Data stream sent by ESP-AT is (TX): S1 -> S2 -> S3 -> S4 -> S5 -> S6 -> S7 -> S8
  - The Data stream received by ESP-AT is (RX): R8 -> R7 -> R6 -> R5 -> R4 -> R3 -> R2 -> R1

To troubleshoot data transmission issues in the AT firmware across various communication layers (UART, AT interface, network, Wi-Fi), follow the structured guidance below and choose the appropriate debugging method:

  - For S1/R1 (UART Communication):

    Use a logic analyzer to capture the serial data sent from the MCU to the ESP module (TX -> RX lines) to verify UART communication. If any anomalies are detected (typically manifested as data errors or data loss), please check the wiring first. Common causes include excessively high baud rate, overly long UART connections between the MCU and ESP, incorrect bidirectional flow control configuration, or improper hardware design.

  - For S2/R2 (AT Interface Layer):

    Enable the :ref:`AT Interface Debugging <debug-at_interface>` configuration and recompile the AT firmware. During the retest, check the logs from the :term:`AT log port` to ensure RX (receive) and TX (transmit) data are functioning correctly. If data loss is detected, please first confirm whether hardware flow control is enabled. If the issue is confirmed to be caused by AT, please submit an issue at `esp-at/issues <https://github.com/espressif/esp-at/issues/new?assignees=&labels=Type%3A+Bug&projects=&template=02_runtime_bug.yml>`_.

  - For S3/R3 (AT Internal Processing):

    No user intervention is required.

  - For S4-S5/R4-R5 (Network Layer):

    Enable the :ref:`TCP Debugging <debug-at_tcp>` configuration and recompile the AT firmware. During the retest, analyze the logs from the :term:`AT log port` to verify LwIP layer transmission. If you find issues, submit an issue at `esp-at/issues <https://github.com/espressif/esp-at/issues/new?assignees=&labels=Type%3A+Bug&projects=&template=02_runtime_bug.yml>`_.

  - For S6/R6 (Wi-Fi Layer):

    Enable the :ref:`Wi-Fi Debugging <debug-at_wifi>` configuration and recompile the AT firmware. During the retest, refer to the `Espressif Wireshark User Guide <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/wireshark-user-guide.html>`_ to capture airborne packets, and analyze the Wi-Fi layer data transmission alongside the logs from the :term:`AT log port`. If issues are confirmed at the Wi-Fi layer, submit an issue at `esp-at/issues <https://github.com/espressif/esp-at/issues/new?assignees=&labels=Type%3A+Bug&projects=&template=02_runtime_bug.yml>`_. If the problem originates from the server side, please investigate accordingly.

.. only:: not esp32s2

  Example 2: Debugging BLE connection and scanning processes
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  Scene: The ESP module, as a BLE client, repeatedly establishes and disconnects BLE connection with the mobile phone.

    - If you want to identify the cause of the BLE disconnection, please capture the debug logs from the :term:`AT log port` during the retest for analysis. The logs may contain disconnection reason codes, which are common to BLE and not specific to AT. Example:

      .. code-block:: none

          Disconnect reason = 0x13

    - If you want to debug processes such as BLE scanning or connection, please enable the configurations for :ref:`AT BLE Function Debugging <debug-at_ble>` and compile the AT firmware. During the retest, capture the debug logs from the :term:`AT log port` for analysis.
