AT FAQ
======

:link_to_translation:`en:[English]`

.. contents::
   :local:
   :depth: 2

AT 固件
-------

我的模组没有官方发布的固件，如何获取适用的固件？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

如果 :doc:`AT_Binary_Lists/index` 章节中没有发布相关固件，您可考虑以下选择：

- 使用相同硬件配置的模组的固件（点击 :doc:`../Compile_and_Develop/esp-at_firmware_differences`）。
- 参考：:ref:`firmware-selection`。

如何获取 AT 固件源码？
^^^^^^^^^^^^^^^^^^^^^^^^^^

  esp-at 项目以源代码和预编译库的组合形式在此仓库中分发。预编译的核心库（位于 ``esp-at/components/at/lib/`` 目录下）是闭源的，无开源计划。

官网上放置的 AT 固件如何下载？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 烧录工具请下载 `Flash 下载工具 <https://www.espressif.com/zh-hans/support/download/other-tools>`_。
  - 烧录地址请参考 :doc:`AT 下载指南 <Get_Started/Downloading_guide>`。

如何整合 ESP-AT 编译出来的所有 bin 文件？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  可以使用 `Flash 下载工具 <https://www.espressif.com/zh-hans/support/download/other-tools>`_ 的 combine 按钮进行整合。

.. only:: esp32

  新购买的 ESP32-WROVE-E 模组上电后，串口打印错误 "flash read err,1000" 是什么原因？该模组如何使用 AT 命令？
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    - ESP32-WROVER-E 的模组出厂没有烧录 ESP-AT 固件，因此出现 "flash read err" 的错误。
    - 如果想要使用 ESP32-WROVER-E 模组的 AT 命令功能 ，请参考如下链接获取固件和烧录固件。

      - :ref:`下载固件 <firmware-esp32-wrover-32-series>`;
      - :ref:`连接硬件 <hw-connection-esp32-wrover-series>`;
      - :ref:`烧录固件 <flash-at-firmware-into-your-device>`.

模组出厂 AT 固件是否支持流控？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 该模组支持硬件流控，但是不支持软件流控。
  - 对于是否开启硬件流控，您可以通过串口命令 :ref:`AT+UART_CUR <cmd-UARTC>` 或者 :ref:`AT+UART_DEF <cmd-UARTD>` 进行修改。
  - :doc:`硬件接线参考 <Get_Started/Hardware_connection>`。

AT 命令与响应
------------------

AT 提示 busy 是什么原因？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 提示 "busy" 表示正在处理前⼀条命令，⽆法响应当前输⼊。因为 AT 命令的处理是线性的，只有处理完前⼀条命令后，才能接收下⼀条命令。
  - 当有多余的不可⻅字符输⼊时，系统也会提示 "busy" 或 "ERROR"，因为任何串⼝的输⼊，均被认为是命令输⼊。

    - 串⼝输⼊ AT+GMR (换⾏符 CR LF) (空格符)，由于 AT+GMR (换⾏符 CR LF) 已经是⼀条完整的 AT 命令了，系统会执⾏该命令。此时如果系统尚未完成 AT+GMR 操作，就收到了后⾯的空格符，将被认为是新的命令输⼊，系统提示 "busy"。但如果是系统已经完成了 AT+GMR 操作，再收到后⾯的空格符，空格符将被认为是⼀条错误的命令，系统提示 "ERROR"。
    - MCU 发送 AT+CIPSEND 后，收到 busy p.. 响应，MCU 需要重新发送数据。因为 busy p.. 代表上一条命令正在执行，当前输入无效。建议等 AT 上一条命令响应后，MCU 再重新发送新命令。

AT 固件，上电后发送第一个命令总是会返回下面的信息，为什么？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  .. code-block:: text

    ERR CODE:0x010b0000
    busy p...

  - 此信息代表的是"正在处理上一条命令"。
  - 一般情况下只会显示"busy p..."，显示 ERR CODE 是因为打开了错误代码提示。
  - 如果是上电的第一条命令就返回了这个错误码信息，可能的原因是：这条命令后面多跟了换行符/空格/其他符号，或者连续发送了两个或多个 AT 命令。

在不同模组上的默认 AT 固件支持哪些命令，以及哪些命令从哪个版本开始支持？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 如果您想了解 ESP-AT 在不同模组上默认固件都支持哪些命令，您可以参考 :doc:`Compile_and_Develop/esp-at_firmware_differences`。
  - 如果您想查找某个命令从哪个版本开始支持，以及各个版本上修复了哪些问题，您可以参考 `release notes <https://github.com/espressif/esp-at/releases>`_。

主 MCU 给 {IDF_TARGET_NAME} 设备发 AT 命令无返回，是什么原因？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  当主 MCU 给 {IDF_TARGET_NAME} 设备发送 AT 命令后需要添加结束符号，在程序中的写法为："AT\\r\\n"。可参见 :ref:`check-whether-at-works`。

Wi-Fi 断开（打印 WIFI DISCONNECT） 是为什么？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  您可以在 :term:`AT 日志端口` 查看到 Wi-Fi 断开的原因，通常会打印 "wifi disconnected, rc:<reason_code>"。此处的 <reason_code> 请参考： `Wi-Fi 原因代码 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#id34>`_。

Wi-Fi 常见的兼容性问题有哪些？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - AMPDU 兼容性问题。

    - 如果路由器不支持 AMPDU，那么 {IDF_TARGET_NAME} 会在和路由器交互时，自动关闭 AMPDU 功能。
    - 如果路由器支持 AMPDU，但是路由器和 {IDF_TARGET_NAME} 之间的 AMPDU 传输存在兼容性问题，那么建议关闭路由器的 AMPDU 功能或者 {IDF_TARGET_NAME} 的 AMPDU 功能。如果您要禁用 {IDF_TARGET_NAME} 的 AMPDU 功能，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：

      - 禁用 ``Component config`` -> ``Wi-Fi`` -> ``WiFi AMPDU TX``
      - 禁用 ``Component config`` -> ``Wi-Fi`` -> ``WiFi AMPDU RX``

  - phy mode 兼容性问题。如果路由器和 {IDF_TARGET_NAME} 之间的 phy mode 存在兼容性问题，那么建议切换路由器的 phy mode 或者 {IDF_TARGET_NAME} 的 phy mode。如果您要切换 {IDF_TARGET_NAME} 的 phy mode，请参考 :ref:`AT+CWSTAPROTO <cmd-STAPROTO>` 命令。

ESP-AT 命令是否支持 ESP-WIFI-MESH？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  ESP-AT 当前不支持 ESP-WIFI-MESH。

是否有 AT 命令连接阿里云以及腾讯云示例？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  若使用 :doc:`通用 AT 固件 <AT_Binary_Lists/index>`，可参考以下示例：

  - 阿里云应用参考：`AT+MQTT aliyun <https://blog.csdn.net/espressif/article/details/107367189>`_。
  - 腾讯云应用参考：`AT+MQTT QCloud <https://blog.csdn.net/espressif/article/details/104714464>`_。

AT 命令是否可以设置低功耗蓝牙发射功率？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  可以。{IDF_TARGET_NAME} 的 Wi-Fi 和 Bluetooth LE 共用一根天线，可使用 :ref:`AT+RFPOWER <cmd-RFPOWER>` 命令设置。

.. only:: esp32

  可以通过 AT 命令将 ESP32-WROOM-32 模块设置为 HID 键盘模式吗？
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    可以的，请参考 :doc:`Bluetooth LE AT 命令集 <AT_Command_Set/BLE_AT_Commands>`。下面这个链接是简单的演示链接：https://pan.baidu.com/s/1TgNE2DpJtVARGqB-jb8UIQ 提取码：f6hu。

如何支持那些默认固件不支持但可以在配置和编译 ESP-AT 工程后支持的命令？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  例如在 {IDF_TARGET_NAME} 系列支持连接 WPA2 企业级路由器功能，需编译时在 menuconfig 中开启该功能 ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``[*]AT WPA2 Enterprise command support``。

AT 命令中特殊字符如何处理？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  可以参考 :ref:`at-command-types` 章节中的转义字符语法。

AT 命令中串口波特率是否可以修改？（默认：115200）
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  AT 命令串口的波特率是可以修改的。

  - 第一种方法，您可以通过串口命令 :ref:`AT+UART_CUR <cmd-UARTC>` 或 :ref:`AT+UART_DEF <cmd-UARTD>`。
  - 第二种方法，您可以重新编译 AT 固件，编译介绍： :doc:`如何编译 AT 工程 <Compile_and_Develop/How_to_clone_project_and_compile_it>` 与 :doc:`修改 UART 波特率配置 <Compile_and_Develop/How_to_set_AT_port_pin>`。

{IDF_TARGET_NAME} 使用 AT 命令进入透传模式，如果连接的热点断开，{IDF_TARGET_NAME} 能否给出相应的提示信息？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 可以通过命令 :ref:`AT+SYSMSG <cmd-SYSMSG>` 进行配置，可设置 AT+SYSMSG=4，如果连接的热点断开，串口会上报 "WIFI DISCONNECT\\r\\n"。
  - 需要注意的是，该命令在 AT v2.1.0 之后添加，v2.1.0 及之前的版本无法使用该命令。

.. only:: esp32

  ADV 广播参数超过 31 字节之后应该如何设置？
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    :ref:`AT+BLEADVDATA <cmd-BADVD>` 命令支持 adv 广播参数最大为 31 字节，如果需要设置更长的广播参数，请调用 :ref:`AT+BLESCANRSPDATA <cmd-BSCANR>` 命令来设置。

低功耗蓝牙客户端如何使能 notify 和 indicate 功能？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 低功耗蓝牙的特征的属性除了读、写还有 ``notify`` 和 ``indicate``。这两种都是服务端向客户端发送数据的方式，但是要想真的发送成功需要客户端提前注册 ``notification``，也就是写 ``CCCD`` 的值。
  - 如果要使能 ``notify``，需要写 ``0x01``；如果要使能 ``indicate``，需要写 ``0x02`` （写 ``0x2902`` 这个描述符）；如果是既想使能 ``notify`` 又想使能 ``indicate``，需要写 ``0x03``。
  - 比如，ESP-AT 的默认的服务中，``0xC305`` 是可 ``notify`` 的，``0xC306`` 是可 ``indicate`` 的。我们分别写这两个特征下面的 ``0x2902`` 描述符：

  .. code-block:: text

    AT+BLEGATTCWR=0,3,6,1,2
    >
    // 写 0x01
    OK
    // server: +WRITE:0,1,6,1,2,<0x01>,<0x00>
    AT+BLEGATTCWR=0,3,7,1,2
    >
    // 写 0x02
    OK
    // server: +WRITE:0,1,6,1,2,<0x02>,<0x00>
    // 写 ccc 是 server 可以发送 notify 和 indicate 的前提条件

硬件
----

在不同模组上的 AT 固件要求芯片 flash 多大？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 对于 {IDF_TARGET_NAME} 系列模组，您可以参考 :doc:`ESP-AT 固件差异 <Compile_and_Develop/esp-at_firmware_differences>`。

AT 固件如何查看 error log？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  .. list::

    :esp32: - {IDF_TARGET_NAME} 在 download port 查看 error log，默认 UART0 为 GPIO1、GPIO3。
    :esp32c2 or esp32c3: - {IDF_TARGET_NAME} 在 download port 查看 error log，默认 UART0 为 GPIO21、GPIO20。
    - 详情可以参阅 :doc:`硬件连接 <Get_Started/Hardware_connection>`。

AT 在 {IDF_TARGET_NAME} 模组上的 UART1 通信管脚与 {IDF_TARGET_NAME} 模组的 datasheet 默认 UART1 管脚不一致？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - {IDF_TARGET_NAME} 支持 IO 矩阵变换，在编译 ESP-AT 的时候，可以在 menuconfig 中通过软件配置修改 UART1 的管脚配置，所以就会出现和 datasheet 管脚不一致的情况。
  - 管脚详情可以参阅 `factory_param_data.csv <https://github.com/espressif/esp-at/blob/master/components/customized_partitions/raw_data/factory_param/factory_param_data.csv>`_。

性能
----

AT Wi-Fi 连接耗时多少？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 在办公室场景下，AT Wi-Fi 连接耗时实测为 5 秒。但在实际使用中，Wi-Fi 连接时间取决于路由器性能，⽹络环境，模块天线性能等多个条件。
  - 可以通过 :ref:`AT+CWJAP <cmd-JAP>` 的 **<jap_timeout>** 参数，来设置最大超时时间。

ESP-AT 固件中 TCP 发送窗口大小是否可以修改？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - TCP 发送窗口当前无法通过命令修改，需要配置和编译 ESP-AT 工程生成新的固件。
  - 可以重新配置 menuconfig 参数，``Component config`` > ``LWIP`` > ``TCP`` > ``Default send buffer size``。

{IDF_TARGET_NAME} AT 吞吐量如何测试及优化？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - AT 吞吐量测试的影响因素较多，建议使⽤ esp-idf 中的 iperf 示例进行测试（用 AT 测试时，请使用透传方式，并将数据量调整为 1460 字节连续发送）。
  - 若测试速率不满⾜需求，您可以参考 :doc:`Compile_and_Develop/How_to_optimize_throughput` 来提高速率。

.. only:: esp32

  {IDF_TARGET_NAME} AT 默认固件 Bluetooth LE UART 透传的最大传输率是？
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    办公室开放环境下，串口波特率为 2000000 时，ESP-AT Bluetooth 平均传输速率为 0.56 Mbps，ESP-AT Bluetooth LE 平均传输速率为 0.101 Mbps。

其他
----

乐鑫芯片可以通过哪些接口来传输 AT 命令？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  .. list::

    :esp32: - {IDF_TARGET_NAME} 支持 UART、SDIO 接口通信。
    :esp32c2 or esp32c3 or esp32c6: - {IDF_TARGET_NAME} 支持 UART、SPI 接口通信。
    - AT 默认固件是使用 UART 接口来传输。用户如果需要使用 SDIO 或者 SPI 接口进行通信，可以基于 ESP-AT 配置编译，详情请见 :doc:`编译和开发 <Compile_and_Develop/index>`。
    - 更多资料请参考 :project_file:`使用 AT SDIO 接口 <main/interface/sdio/README.md>`，:project_file:`使用 AT SPI 接口 <main/interface/spi/README.md>`，或 :project_file:`使用 AT 套接字接口 <main/interface/socket/README.md>`。

.. only:: esp32

  {IDF_TARGET_NAME} AT 以太网功能如何使用？
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    AT 默认固件是不开启以太网功能的，您如果想要开启以太网功能，您可以参考 :doc:`Compile_and_Develop/How_to_enable_ESP_AT_Ethernet`。

.. only:: esp32

  ESP-AT 如何进行 BQB 认证？
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    可参考 `ESP32 更新多项 BQB 蓝牙认证 <https://mp.weixin.qq.com/s?__biz=MzA4Nzc5MjkwNw==&mid=2651783810&idx=1&sn=fb0e132af240606d8178347966721542&chksm=8bcfaee6bcb827f03992aa200a2eb2baef5114712a4001da0c8282502a9183f5379605412cea&mpshare=1&scene=1&srcid=0920VLpOLubCew48DrCRdjCT&sharer_sharetime=1583218643838&sharer_shareid=1a1137fefea7b87a843519e48151f9a4&rd2werd=1#wechat_redirect>`_。

{IDF_TARGET_NAME} AT 如何指定 TLS 协议版本？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  编译 ESP-AT 工程时，可以在 ``./build.py menuconfig`` > ``Component config`` > ``mbedTLS`` 目录下，可以将不需要的版本关闭使能。

AT 固件如何修改 TCP 连接数？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 目前 AT 默认固件的 TCP 最大连接数为 5。
  - {IDF_TARGET_NAME} AT 最大支持 16 个 TCP 连接，可以在 menuconfig 中进行配置，配置方法如下：
    
    - ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``(16)AT socket maximum connection number``
    - ``./build.py menuconfig`` > ``LWIP`` > ``(16)Max number of open sockets``

.. only:: esp32

{IDF_TARGET_NAME} AT 支持 PPP 吗?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 不支持，可参考 `pppos_client <https://github.com/espressif/esp-idf/tree/v4.4.2/examples/protocols/pppos_client>`_ 示例自行实现。

AT 如何使能调试日志？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 使能 log 等级： ``./build.py menuconfig`` > ``Component Config`` > ``Log output`` > ``Default log verbosity`` 设置到 ``Debug``。

    - 使能 Wi-Fi debug： ``./build.py menuconfig`` > ``Component config`` > ``Wi-Fi`` > ``Wi-Fi debug log level`` 设置到 ``Debug``。
    - 使能 TCP/IP debug： ``./build.py menuconfig`` > ``Component config`` > ``LWIP`` > ``Enable LWIP Debug`` > 将具体想要调试的部分 log 等级设置到 ``Debug``。
    - 使能 BLE debug： ``./build.py menuconfig`` > ``Component config`` > ``Bluetooth`` > ``Bluedroid Options`` > ``Disable BT debug logs`` > ``BT DEBUG LOG LEVEL`` > 将具体想要调试的部分 log 等级设置到 ``Debug``。

AT 指令如何实现 HTTP 断点续传功能？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 目前 AT 指令提供两种方法：

    - 通过 HTTP 的 Range 字段指定读取的数据范围，具体详情请参考 :ref:`AT+HTTPCHEAD 示例 <cmd-HTTPCHEAD_example>`。
    - 可以使用 AT TCP 系列指令自行构造 HTTP GET 请求。在 :ref:`{IDF_TARGET_NAME} 设备获取被动接收模式下的套接字数据示例 <example-passive_recv>` 的第 6 步和第 7 步之间，添加一步：设备使用 :ref:`AT+CIPSEND <cmd-SEND>` 命令发送您自行构造的 HTTP GET 请求包给服务端即可。在被动接收模式下，对于从服务端获取的 HTTP GET 请求数据，MCU 需要通过主动下发 :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>` 命令来读取这些数据，以避免因服务端传输大量数据而导致 MCU 端无法及时处理的情况。
