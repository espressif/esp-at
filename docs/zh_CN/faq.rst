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

- 使用相同硬件配置的模组的固件（点击 :doc:`../Compile_and_Develop/esp-at_firmware_differences`。
- 为你的模组 :doc:`创建出厂参数二进制文件 <Compile_and_Develop/How_to_create_factory_parameter_bin>` 并 :doc:`自行编译固件 <Compile_and_Develop/How_to_clone_project_and_compile_it>`。

如何获取 AT 固件源码？
^^^^^^^^^^^^^^^^^^^^^^^^^^

  ESP-AT 固件部分开源，开源仓库参考 `esp-at <https://github.com/espressif/esp-at>`_。

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

  当主 MCU 给 {IDF_TARGET_NAME} 设备发送 AT 命令后需要添加结束符号，在程序中的写法为："AT\r\n"。可参见 :ref:`check-whether-at-works`。

ESP-AT 命令是否支持 ESP-WIFI-MESH？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  ESP-AT 当前不支持 ESP-WIFI-MESH。

AT 是否支持 websocket 命令？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 默认命令不支持 websocket 命令。
  - 可通过自定义命令实现，代码参考 `websocket <https://github.com/espressif/esp-idf/tree/master/examples/protocols/websocket>`_，以及 :doc:`Compile_and_Develop/How_to_add_user-defined_AT_commands`。


是否有 AT 命令连接阿里云以及腾讯云示例？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  若使用 :doc:`通用 AT 固件 <AT_Binary_Lists/index>`，可参考以下示例：

  - 阿里云应用参考：`AT+MQTT aliyun <https://blog.csdn.net/espressif/article/details/107367189>`_。
  - 腾讯云应用参考：`AT+MQTT QCloud <https://blog.csdn.net/espressif/article/details/104714464>`_。

  .. only:: esp32 or esp32c3

    若使用 :doc:`QCloud AT 固件 <Customized_AT_Commands_and_Firmware/Tencent_Cloud_IoT_AT/tencent_cloud_iot_at_binaries>`，请采用 :doc:`腾讯云 AT 命令 <Customized_AT_Commands_and_Firmware/Tencent_Cloud_IoT_AT/Tencent_Cloud_IoT_AT_Command_Set>` 连接至腾讯云。

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

{IDF_TARGET_NAME} 使用 AT 指令进入透传模式，如果连接的热点断开，{IDF_TARGET_NAME} 能否给出相应的提示信息？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 可以通过命令 :ref:`AT+SYSMSG <cmd-SYSMSG>` 进行配置，可设置 AT+SYSMSG=4，如果连接的热点断开，串口会上报 "WIFI DISCONNECT\r\n"。
  - 需要注意的是，该命令在 AT v2.1.0 之后添加，v2.1.0 及之前的版本无法使用该命令。

.. only:: esp32 or esp32c3

  ADV 广播参数超过 32 字节之后应该如何设置？
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    :ref:`AT+BLEADVDATA <cmd-BADVD>` 命令支持 adv 广播参数最大为 32 字节，如果需要设置更长的广播参数，请调用 :ref:`AT+BLESCANRSPDATA <cmd-BSCANR>` 命令来设置。

硬件
----

在不同模组上的 AT 固件要求芯片 flash 多大？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 对于 {IDF_TARGET_NAME} 系列模组，您可以参考 :doc:`ESP-AT 固件差异 <Compile_and_Develop/esp-at_firmware_differences>`。

.. only:: esp32

  {IDF_TARGET_NAME} AT 如何从 UART0 口通信？
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    默认 AT 固件是通过 UART1 口通信的，如果要从 UART0 通信，需要下载并编译 ESP-AT。

    - 参考 :doc:`Compile_and_Develop/How_to_clone_project_and_compile_it` 搭建好编译环境；
    - 修改 :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` 表中对应模组的 UART 管脚，将 uart_tx_pin 修改为 GPIO1，uart_tx_pin 修改为 GPIO3；
    - 调整配置：./build.py menuconfig > Component config > Common ESP-related > UART for console output(Custom) > Uart peripheral to use for console output(0-1)(UART1) > (1)UART TX on GPIO# (NEW) > (3)UART TX on GPIO# (NEW)。

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
    :esp32c2: - {IDF_TARGET_NAME} 支持 UART 接口通信。
    :esp32c3: - {IDF_TARGET_NAME} 支持 UART、SPI 接口通信。
    - AT 默认固件是使用 UART 接口来传输。用户如果需要使用 SDIO 或者 SPI 接口进行通信，可以基于 ESP-AT 配置编译，详情请见 :doc:`编译和开发 <Compile_and_Develop/index>`。
    - 更多资料请参考 :project_file:`使用 AT SDIO 接口 <main/interface/sdio/README.md>`，:project_file:`使用 AT SPI 接口 <main/interface/hspi/README.md>`，或 :project_file:`使用 AT 套接字接口 <main/interface/socket/README.md>`。

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

  编译 ESP-AT 工程时，可以在 ./build.py menuconfig -> Component config -> mbedTLS 目录下，可以将不需要的版本关闭使能。

AT 固件如何修改 TCP 连接数？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 目前 AT 默认固件的 TCP 最大连接数为 5。
  - {IDF_TARGET_NAME} AT 最大支持 16 个 TCP 连接，可以在 menuconfig 中进行配置，配置方法如下：
    
    - ./build.py menuconfig---> Component config---> AT--->  (16)AT socket maximum connection number
    - ./build.py menuconfig---> LWIP---> (16)Max number of open sockets
