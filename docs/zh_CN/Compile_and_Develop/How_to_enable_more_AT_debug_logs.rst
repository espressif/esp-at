如何启用更多 AT 调试日志
=========================

:link_to_translation:`en:[English]`

本文主要介绍了如何启用日志配置选项。为了获取更多用于调试 ESP-AT 功能的日志信息，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第三步配置工程里选择 silence mode 为 ``No``。在第五步配置工程里，根据您的实际情况，针对性地启用日志配置选项。

.. list::

  * :ref:`AT Wi-Fi 功能调试 <debug-at_wifi>`
  * :ref:`AT 网络功能调试 <debug-at_network>`
  :not esp32s2: * :ref:`AT BLE 功能调试 <debug-at_ble>`
  * :ref:`AT 接口功能调试 <debug-at_interface>`
  * :ref:`调试示例 <debug-at_examples>`

最终的调试日志，将从 :term:`AT 日志端口` 输出，请根据 :doc:`硬件连接 <../Get_Started/Hardware_connection>` 文档，查找 **输出日志** 对应的 TX GPIO 管脚，通过串口工具查看该管脚的日志。

.. note::
  启用日志配置选项后，日志输出量将显著增加，因此建议先确认问题属于哪个模块，再启用对应模块的日志配置。防止因日志输出量过大，导致无法复现或者出现 ``task wdt`` 等新的问题。

  启用日志配置选项后，固件大小将会增加，可能导致编译失败。此时，您可以在 ``python build.py menuconfig`` > ``Component config`` > ``AT`` 路径下禁用不需要的 AT 功能来减小固件大小。

.. _debug-at_wifi:

AT Wi-Fi 功能调试
------------------------------

- 您可以启用以下配置以开启 Wi-Fi 收发包的统计数据日志：

  ::

    Component config > Log > Log Level > Default log verbosity > Info （或 Info 以上等级）
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Periodically dump Wi-Fi statistics
    Component config > AT > Enable ESP-AT Debug > Periodically dump Wi-Fi statistics > The interval of dumping Wi-Fi statistics (ms)

- 您可以启用以下配置以开启 Wi-Fi 交互过程的调试日志：

  .. only:: esp32 or esp32c2 or esp32c3 or esp32s2

    ::

      Component config > Log > Log Level > Default log verbosity > Debug
      Component config > Supplicant > Print debug messages from WPA Supplicant

  .. only:: esp32c5 or esp32c6

    ::

      Component config > Log > Log Level > Default log verbosity > Debug
      Component config > Wi-Fi > Print debug messages from WPA Supplicant

.. note::

  在分析日志时，您可能会遇到 Wi-Fi 原因码，具体含义请参考 `Wi-Fi 原因代码 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#id34>`_。

.. _debug-at_network:

AT 网络功能调试
------------------------

.. contents::
   :local:
   :depth: 1

.. _debug-at_tcp:

TCP 调试
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  若要监控 TCP 或基于 TCP 的协议（如 SSL、HTTP、MQTT、WebSocket 等）的传出 (TX) 和传入 (RX) 数据包，请启用以下配置，最终的日志中将打印 TCP 数据包信息，包括 IP 总长度、TCP 数据长度、TCP 序列号、TCP 确认号、TCP 源端口、目标端口和 TCP 标志位 (TCP flags)。其中，TCP 标志位是 CWR(128)、ECN(64)、Urgent(32)、Ack(16)、Push(8)、Reset(4)、SYN(2) 和 FIN(1) 的累积值。

  ::

    Component config > Log > Log Level > Default log verbosity > Info （或 Info 以上等级）
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the TCP packet debug messages
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the TCP packet debug messages > Specify the list of TCP port numbers to monitor > 0

UDP 调试
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  若要监控 UDP 或基于 UDP 的协议（如 DHCP、DNS、SNTP、mDNS 等）的传出 (TX) 和传入 (RX) 数据包，请启用以下配置，最终的日志中将会打印 UDP 数据包信息，其中包括 IP 总长度、源端口、目标端口和 UDP 数据长度。

  ::

    Component config > Log > Log Level > Default log verbosity > Info （或 Info 以上等级）
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the UDP packet debug messages
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the UDP packet debug messages > Specify the list of outgoing UDP (UDP TX) port numbers to monitor
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the UDP packet debug messages > Specify the list of incoming UDP (UDP RX) port numbers to monitor

SSL 调试
^^^^^^^^^^^^^^^^^^^^

  您可以启用以下配置以开启 SSL 功能的调试日志：

  ::

    Component config > Log > Log Level > Default log verbosity > Verbose
    Component config > mbedTLS > Enable mbedTLS debugging
    Component config > mbedTLS > Enable mbedTLS debugging > Set mbedTLS debugging level > Verbose

ICMP 调试
^^^^^^^^^^^^^^^^^^^^^^

  您可以启用以下配置以开启 ICMP 功能 (:ref:`AT+PING <cmd-CIPPING>`) 的调试日志：

  ::

    Component config > Log > Log Level > Default log verbosity > Info （或 Info 以上等级）
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug
    Component config > AT > Enable ESP-AT Debug > Enable Network Debug > Enable the ICMP packet debug messages

lwIP 网络调试
^^^^^^^^^^^^^^^^^^^^^^^^^

  如果启用以上配置输出的日志仍无法满足您的调试需求，您可以在路径 ``Component config`` > ``LWIP`` > ``Enable LWIP Debug`` 下启用所需的调试配置。

.. _debug-at_ble:

.. only:: not esp32s2

  AT BLE 功能调试
  -----------------------

  - 您可以直接在 :term:`AT 日志端口` 查看断开原因码，从而调试 BLE 断开过程。

  .. only:: esp32 or esp32c3

    - 您可以启用以下配置以开启 BLE 扫描、连接、广播、数据收发等交互过程的调试日志：

      ::

        Component config > Log > Log Level > Default log verbosity > Debug
        Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL > HCI layer > DEBUG

    - 您可以启用以下配置以开启 BLE 配对过程的调试日志：

      ::

        Component config > Log > Log Level > Default log verbosity > Debug
        Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL > HCI layer > DEBUG
        Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL > SMP layer > DEBUG

    - 您可以启用以下配置以开启 BLE GATT 层的调试日志：

      ::

        Component config > Log > Log Level > Default log verbosity > Debug
        Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL > GATT layer > DEBUG

    - 您可以启用以下配置以开启 AT BluFi 功能的调试日志：

      ::

        Component config > Log > Log Level > Default log verbosity > Debug
        Component config > Bluetooth > Bluedroid Options > BT DEBUG LOG LEVEL > BLUFI layer > DEBUG

    如果以上配置无法满足您的调试需求，您可以在路径 ``Component config`` > ``Bluetooth`` > ``Bluedroid Options`` > ``BT DEBUG LOG LEVEL`` 下启用其他所需的调试配置。

  .. only:: esp32c2 or esp32c5 or esp32c6

    - 您可以启用以下配置以开启 BLE 功能的调试日志：

      ::

        Component config > Log > Log Level > Default log verbosity > Debug
        Component config > Bluetooth > NimBLE Options > NimBLE Host log verbosity > Debug logs

AT 接口功能调试
--------------------

.. _debug-at_interface:

- 当您想要在 :term:`AT 日志端口` 获取到 AT 通过 TX 发送给 MCU 的数据时，您可开启如下配置：

  ::

    Component config > Log > Log Level > Default log verbosity > Info （或 Info 以上等级）
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Logging the data sent from AT to MCU (AT ---> MCU)
    Component config > AT > Enable ESP-AT Debug > Logging the data sent from AT to MCU (AT ---> MCU) > The maximum length of the data sent from AT to MCU to be logged > 8192

- 当您想要在 :term:`AT 日志端口` 获取 AT 通过 RX 接收到的 MCU 发送数据时，您可开启如下配置：

  ::

    Component config > Log > Log Level > Default log verbosity > Info （或 Info 以上等级）
    Component config > AT > Enable ESP-AT Debug
    Component config > AT > Enable ESP-AT Debug > Logging the data received by AT from MCU (AT <---- MCU)
    Component config > AT > Enable ESP-AT Debug > Logging the data received by AT from MCU (AT <---- MCU) > The maximum length of the data received by AT from MCU to be logged > 8192

.. _debug-at_examples:

调试示例
-----------

示例 1：调试 TCP 连接的数据发送和接收过程
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

场景：ESP 模组在波特率为 115200 以及使用双向流控情况下，作为 TCP 客户端与服务器建立了 TCP 连接，进入透传模式，并发送数据给服务器。

.. figure:: ../../_static/optimize-throughput.png
   :align: center
   :alt: 吞吐数据流
   :figclass: align-center

   数据流

如图中箭头所示：

  - ESP-AT 发送 (TX) 的数据流为 S1 -> S2 -> S3 -> S4 -> S5 -> S6 -> S7 -> S8
  - ESP-AT 接收 (RX) 的数据流为 R8 -> R7 -> R6 -> R5 -> R4 -> R3 -> R2 -> R1

如需排查 AT 固件在各通信层级（串口通信、AT 接口、网络、Wi-Fi 等）中的数据传输问题，请参考以下分层指导，选择相应的调试方式进行分析：

  - 对于 S1/R1（串口通信）：

    使用逻辑分析仪捕获 MCU 向 ESP 模组发送的串口数据（TX -> RX 线路），以确认 UART 通信是否正常。如果发现异常（通常表现为数据错误和数据丢失），请先检查线路。常见的原因包括波特率过高、MCU 和 ESP 的 UART 线路太长、双向流控配置错误、硬件设计不合理等。

  - 对于 S2/R2（AT 接口层）：

    启用 :ref:`AT 接口调试 <debug-at_interface>` 配置并重新编译 AT 固件。复测场景时，通过 :term:`AT 日志端口` 输出的日志检查 RX（接收）和 TX（发送）数据是否正常。如果发现数据丢失，请先确认硬件流控是否开启。否则如果确认为 AT 引发的问题，请在 `esp-at/issues <https://github.com/espressif/esp-at/issues/new?assignees=&labels=Type%3A+Bug&projects=&template=02_runtime_bug.yml>`_ 提交 issue。

  - 对于 S3/R3（AT 内部处理）：

    无需用户关注。

  - 对于 S4-S5/R4-R5（网络层）：

    启用 :ref:`TCP 调试 <debug-at_tcp>` 配置并重新编译 AT 固件。复测场景时，通过 :term:`AT 日志端口` 输出的日志分析 LwIP 层的传输是否正常。如果发现问题，请在 `esp-at/issues <https://github.com/espressif/esp-at/issues/new?assignees=&labels=Type%3A+Bug&projects=&template=02_runtime_bug.yml>`_ 提交 issue。

  - 对于 S6/R6（Wi-Fi 层）：

    启用 :ref:`Wi-Fi 调试 <debug-at_wifi>` 配置并重新编译 AT 固件。复测场景时，参考 `乐鑫 Wireshark 使用指南 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/wireshark-user-guide.html>`_ 捕获空中数据包，并结合 :term:`AT 日志端口` 的日志分析 Wi-Fi 层数据传输是否正常。如果确认问题出在 Wi-Fi 层，请在 `esp-at/issues <https://github.com/espressif/esp-at/issues/new?assignees=&labels=Type%3A+Bug&projects=&template=02_runtime_bug.yml>`_ 提交 issue；若问题源自服务器端，请自行排查。

.. only:: not esp32s2

  示例 2：调试 BLE 连接、扫描过程
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  场景：ESP 模组作为 BLE 客户端与手机反复建立 BLE 连接和断开连接。

    - 若您想调试 BLE 断开的原因，请在复测场景时，抓取 :term:`AT 日志端口` 的调试日志进行分析。日志中可能包含断开原因码，这些原因码是 BLE 通用的，并非 AT 特有。示例：

      .. code-block:: none

          Disconnect reason = 0x13

    - 若您想调试 BLE 扫描或连接等过程，请启用 :ref:`AT BLE 功能 <debug-at_ble>` 调试配置，并编译 AT 固件。在复测场景时，抓取 :term:`AT 日志端口` 调试日志进行分析。
