AT FAQ
======

:link_to_translation:`en:[English]`

.. _faq-at-index:

  - :ref:`AT 文档 <faq-at-doc>`

    .. list::

      - :ref:`如何选择正确的 AT 文档版本？ <faq-choose-doc-version>`

  - :ref:`AT 固件 <faq-at-firmware>`

    .. list::

      - :ref:`我的模组没有官方发布的固件，如何获取适用的固件？ <faq-no-official-firmware>`
      - :ref:`如何获取 AT 固件源码？ <faq-at-source-code>`
      - :ref:`官网上放置的 AT 固件如何下载？ <faq-download-firmware>`
      - :ref:`如何整合 ESP-AT 编译出来的所有 bin 文件？ <faq-combine-bins>`
      - :ref:`新购买的 ESP 系列模组上电后，串口打印错误 "flash read err" 是什么原因？如何使用 AT 命令？ <faq-esp-series-flash-error>`
      - :ref:`模组出厂 AT 固件是否支持流控？ <faq-flow-control>`

  - :ref:`AT 命令与响应 <faq-at-cmd>`

    .. list::

      - :ref:`AT 提示 busy p... 是什么原因？ <faq-busy-error>`
      - :ref:`上电后发送第一个命令总是返回错误信息？ <faq-first-cmd-busy>`
      - :ref:`在不同模组上的默认 AT 固件支持哪些命令？ <faq-cmd-support>`
      - :ref:`MCU 给设备发 AT 命令无返回，是什么原因？ <faq-no-response>`
      - :ref:`AT 命令中特殊字符如何处理？ <faq-special-char>`
      - :ref:`AT 命令中串口波特率是否可以修改？ <faq-change-baudrate>`
      - :ref:`如何修改 AT 命令端口的 UART 管脚？ <faq-change-uart-pins>`
      - :ref:`如何解决 TCP/SSL 数据传输时的丢失问题？ <faq-data-loss>`
      - :ref:`透传模式热点断开能否给出提示信息？ <faq-passthrough-disconnect>`
      - :ref:`Wi-Fi 断开是为什么？ <faq-wifi-disconnect>`
      - :ref:`Wi-Fi 常见的兼容性问题有哪些？ <faq-wifi-compatibility>`
      - :ref:`AT 命令是否支持 ESP-WIFI-MESH？ <faq-wifi-mesh>`
      - :ref:`是否有 AT 命令连接阿里云以及腾讯云示例？ <faq-cloud-examples>`
      :esp32: - :ref:`ESP32-WROOM-32 模块设置为 HID 键盘模式？ <faq-hid-keyboard>`
      :esp32 or esp32c2 or esp32c3 or esp32c5 or esp32c6 or esp32c61: - :ref:`AT 命令是否可以设置低功耗蓝牙发射功率？ <faq-ble-tx-power>`
      :esp32 or esp32c2 or esp32c3 or esp32c5 or esp32c6 or esp32c61: - :ref:`低功耗蓝牙客户端如何使能 notify 和 indicate 功能？ <faq-ble-notify-indicate>`

  - :ref:`硬件 <faq-hardware>`

    .. list::

      - :ref:`在不同模组上的 AT 固件要求芯片 flash 多大？ <faq-flash-size>`
      - :ref:`AT 固件如何查看 error log？ <faq-view-error-log>`
      - :ref:`AT 在模组上的 UART1 通信管脚不一致？ <faq-uart1-pin>`

  - :ref:`性能 <faq-performance>`

    .. list::

      - :ref:`AT Wi-Fi 连接耗时多少？ <faq-wifi-conn-time>`
      - :ref:`AT 固件中 TCP 发送窗口大小是否可以修改？ <faq-tcp-window>`
      - :ref:`AT 吞吐量如何测试及优化？ <faq-throughput>`
      - :ref:`如何修改 AT 默认 TCP 数据段最大重传次数？ <faq-tcp-retrans>`

  - :ref:`其他 <faq-other>`

    .. list::

      - :ref:`乐鑫芯片可以通过哪些接口来传输 AT 命令？ <faq-at-interface>`
      - :ref:`AT 如何使能调试日志？ <faq-debug-log>`
      - :ref:`如何修改 AT 固件首次启动时的默认 Wi-Fi 模式？ <faq-default-wifi-mode>`
      - :ref:`AT 指令如何实现 HTTP 断点续传功能？ <faq-http-resume>`
      - :ref:`如何从 HTTP 服务器下载文件并存储到 FATFS，或将 FATFS 文件系统中的文件上传到 HTTP 服务器？ <faq-http-fs>`
      :esp32: - :ref:`如何进行 BQB 认证？ <faq-bqb>`
      :esp32: - :ref:`AT 支持 PPP 吗？ <faq-ppp>`

.. _faq-at-doc:

:ref:`AT 文档 <faq-at-index>`
----------------------------------------

.. _faq-choose-doc-version:

:ref:`如何选择正确的 AT 文档版本？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

请根据实际使用的 AT 固件版本，选择对应芯片平台和文档版本；不同固件版本之间可能存在功能和行为差异。

- 对于 :ref:`官方发布版固件 <official-released-firmware>`，可直接通过 :ref:`AT+USERDOCS? <cmd-USERDOCS>` 命令获取对应文档链接，无需额外确认版本信息。
- 对于非官方发布版固件，请先确认以下信息，再选择与之相匹配的文档版本：

  - 所用 AT 固件对应的芯片平台（如 ESP32、ESP32-C3 等）
  - AT 固件版本号（可通过 :ref:`AT+GMR <cmd-GMR>` 命令查看 AT 核心库的版本号）

  .. figure:: ../_static/faq/choose_target_version_cn.png
      :scale: 100 %
      :align: center
      :alt: 选择 AT 文档目标和版本

.. _faq-at-firmware:

:ref:`AT 固件 <faq-at-index>`
---------------------------------

.. _faq-no-official-firmware:

:ref:`我的模组没有官方发布的固件，如何获取适用的固件？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

如果 :doc:`AT_Binary_Lists/index` 中没有相关固件，可：

  - 使用相同硬件配置的模组固件（参考 :doc:`../Compile_and_Develop/esp-at_firmware_differences`）
  - 参考：:ref:`firmware-selection`

.. _faq-at-source-code:

:ref:`如何获取 AT 固件源码？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

esp-at 项目以源代码和预编译库的组合形式在此仓库中分发。预编译的核心库（位于 ``esp-at/components/at/lib/`` 目录下）是闭源的，无开源计划。

.. _faq-download-firmware:

:ref:`官网上放置的 AT 固件如何下载？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

使用 `Flash 下载工具 <https://dl.espressif.com/public/flash_download_tool.zip>`_ 烧录，详细指导参阅 `Flash 下载工具用户指南 <https://docs.espressif.com/projects/esp-test-tools/zh_CN/latest/{IDF_TARGET_PATH_NAME}/production_stage/tools/flash_download_tool.html>`_。烧录地址参考 :doc:`AT 下载指南 <Get_Started/Downloading_guide>`。

.. _faq-combine-bins:

:ref:`如何整合 ESP-AT 编译出来的所有 bin 文件？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

无需手动整合。发布的固件和编译生成的固件已自带合成好的 2MB/4MB bin 文件，位于 ``build/factory`` 目录下。

.. _faq-esp-series-flash-error:

:ref:`新购买的 ESP 系列模组上电后，串口打印错误 "flash read err" 是什么原因？该模组如何使用 AT 命令？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ESP 系列模组出厂时 Flash 中未烧录 ESP-AT 固件，上电后会提示 "flash read err" 错误。如需使用 AT 命令功能，请参考 :doc:`AT 下载指南 <Get_Started/Downloading_guide>`。

.. _faq-flow-control:

:ref:`模组出厂 AT 固件是否支持流控？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

支持硬件流控，不支持软件流控。可通过 :ref:`AT+UART_CUR <cmd-UARTC>` 或 :ref:`AT+UART_DEF <cmd-UARTD>` 配置硬件流控。硬件接线参考 :doc:`硬件连接 <Get_Started/Hardware_connection>`。

.. _faq-at-cmd:

:ref:`AT 命令与响应 <faq-at-index>`
------------------------------------------

.. _faq-busy-error:

:ref:`AT 提示 busy p... 是什么原因？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

提示 "busy p..." 表示正在处理前一条命令，无法响应当前输入。AT 命令处理是线性的，必须等前一条命令完成后才能接收下一条。

当有多余的不可见字符输入时（如命令后的空格），系统也会提示 "busy p..." 或 "ERROR"，因为任何串口输入均被视为命令输入。

如果 MCU 发送 AT+CIPSEND 后收到 busy p... 响应，表示上一条命令正在执行，当前输入无效。建议等待上一条命令响应完成后再发送新命令。

.. _faq-first-cmd-busy:

:ref:`AT 固件，上电后发送第一个命令总是会返回下面的信息，为什么？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: text

  ERR CODE:0x010b0000
  busy p...

表示"正在处理上一条命令"。一般只显示 "busy p..."，显示 ERR CODE 是因为打开了错误代码提示。

如果上电后的第一条命令就返回此错误，可能原因：

  - 命令后多跟了换行符/空格/其他符号
  - 连续发送了多个 AT 命令

.. _faq-cmd-support:

:ref:`在不同模组上的默认 AT 固件支持哪些命令，以及哪些命令从哪个版本开始支持？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

各模组默认固件支持的命令：参考 :doc:`Compile_and_Develop/esp-at_firmware_differences`

命令版本支持和修复问题：参考 `release notes <https://github.com/espressif/esp-at/releases>`_

.. _faq-no-response:

:ref:`MCU 给 {IDF_TARGET_NAME} 设备发 AT 命令无返回，是什么原因？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

发送 AT 命令后需添加结束符号，程序中写法为："AT\\r\\n"。参考 :ref:`check-whether-at-works`。

.. _faq-special-char:

:ref:`AT 命令中特殊字符如何处理？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

可以参考 :ref:`at-command-types` 章节中的转义字符语法。

.. _faq-change-baudrate:

:ref:`AT 命令中串口波特率是否可以修改？（默认：115200） <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

可以修改，方法：

  - 第一种方法，您可以通过串口命令 :ref:`AT+UART_CUR <cmd-UARTC>` 或 :ref:`AT+UART_DEF <cmd-UARTD>`。
  - 第二种方法，您可以重新编译 AT 固件，编译介绍： :doc:`如何编译 AT 工程 <Compile_and_Develop/How_to_clone_project_and_compile_it>` 与 :doc:`修改 UART 波特率配置 <Compile_and_Develop/How_to_set_AT_port_pin>`。
  - 第三种方法，您可以使用 :ref:`AT+SYSMFG <cmd-SYSMFG>` 命令修改出厂默认的 UART 波特率，请参考 :ref:`AT+SYSMFG 命令示例 <sysmfg-uart-config>`。

.. _faq-change-uart-pins:

:ref:`如何修改 AT 命令端口的 UART 管脚？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  有多种方法可以修改 AT 命令端口的 UART 管脚：

  - 第一种方法，请参考 :doc:`如何设置 AT 端口管脚 <Compile_and_Develop/How_to_set_AT_port_pin>` 文档，修改 ``factory_param_data.csv`` 文件中的 UART 配置参数，然后重新编译和烧录固件。
  - 第二种方法，如果您想在运行时通过 AT 命令修改 UART 配置而无需重新编译固件，请参考 :ref:`AT+SYSMFG 命令示例 <sysmfg-uart-config>`。此方法可以动态修改出厂默认的 UART 配置，包括波特率、管脚等参数。
  - 第三种方法，如果您已有 AT 固件并且只需要修改 UART 管脚，可以使用 :doc:`at.py 工具 <Compile_and_Develop/tools_at_py>` 直接修改固件参数，无需重新编译。

.. _faq-data-loss:

:ref:`如何解决 TCP/SSL 数据传输时的丢失问题？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

若在未启用硬件流控时出现数据丢失，建议先启用硬件流控（:ref:`AT+UART_CUR <cmd-UARTC>` 或 :ref:`AT+UART_DEF <cmd-UARTD>`）验证是否由此导致。若启用流控后问题仍存在，请参考 :ref:`调试 TCP 连接的数据发送和接收过程 <debug-at_examples>` 进一步排查；如确认为 AT 引发的问题，请在 `esp-at/issues <https://github.com/espressif/esp-at/issues>`_ 提交 issue。

.. _faq-passthrough-disconnect:

:ref:`{IDF_TARGET_NAME} 使用 AT 命令进入透传模式，如果连接的热点断开，{IDF_TARGET_NAME} 能否给出相应的提示信息？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

可通过 :ref:`AT+SYSMSG <cmd-SYSMSG>` 配置，设置 AT+SYSMSG=4，热点断开时串口会上报 ``WIFI DISCONNECT\\r\\n``。

注意：该命令在 AT v2.1.0.0 之后添加，v2.1.0.0 及之前的版本无法使用。

.. _faq-wifi-disconnect:

:ref:`Wi-Fi 为什么断开（WIFI DISCONNECT）？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

可在 :term:`AT 日志端口` 查看断开原因，通常打印 ``wifi disconnected, rc:<reason_code>``。
原因代码说明见 `Wi-Fi Reason Code <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#id34>`_。

通用固件可能无法提供详细原因。若需获取具体信息，需在代码中注册 Wi-Fi disconnect 事件并输出原因代码。

.. _faq-wifi-compatibility:

:ref:`Wi-Fi 常见的兼容性问题有哪些？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- AMPDU 兼容性问题。

  - 如果路由器不支持 AMPDU，那么 {IDF_TARGET_NAME} 会在和路由器交互时，自动关闭 AMPDU 功能。
  - 如果路由器支持 AMPDU，但是路由器和 {IDF_TARGET_NAME} 之间的 AMPDU 传输存在兼容性问题，那么建议关闭路由器的 AMPDU 功能或者 {IDF_TARGET_NAME} 的 AMPDU 功能。如果您要禁用 {IDF_TARGET_NAME} 的 AMPDU 功能，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：

    - 禁用 ``Component config`` > ``Wi-Fi`` > ``WiFi AMPDU TX``
    - 禁用 ``Component config`` > ``Wi-Fi`` > ``WiFi AMPDU RX``

- phy mode 兼容性问题。如果路由器和 {IDF_TARGET_NAME} 之间的 phy mode 存在兼容性问题，那么建议切换路由器的 phy mode 或者 {IDF_TARGET_NAME} 的 phy mode。如果您要切换 {IDF_TARGET_NAME} 的 phy mode，请参考 :ref:`AT+CWSTAPROTO <cmd-STAPROTO>` 命令。

.. _faq-wifi-mesh:

:ref:`AT 命令是否支持 ESP-WIFI-MESH？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

AT 当前不支持 ESP-WIFI-MESH。

.. _faq-cloud-examples:

:ref:`是否有 AT 命令连接阿里云以及腾讯云示例？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- 阿里云示例：`AT+MQTT aliyun <https://blog.csdn.net/espressif/article/details/107367189>`_
- 腾讯云示例：`AT+MQTT QCloud <https://blog.csdn.net/espressif/article/details/104714464>`_

.. only:: esp32

  .. _faq-hid-keyboard:

  :ref:`可以通过 AT 命令将 ESP32-WROOM-32 模块设置为 HID 键盘模式吗？ <faq-at-index>`
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  可以，参考 :doc:`Bluetooth LE AT 命令集 <AT_Command_Set/BLE_AT_Commands>`。

  演示链接：https://pan.baidu.com/s/1TgNE2DpJtVARGqB-jb8UIQ 提取码：f6hu

.. only:: esp32 or esp32c2 or esp32c3 or esp32c5 or esp32c6 or esp32c61

  .. _faq-ble-tx-power:

  :ref:`AT 命令是否可以设置低功耗蓝牙发射功率？ <faq-at-index>`
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  可以。{IDF_TARGET_NAME} 的 Wi-Fi 和 Bluetooth LE 共用一根天线，使用 :ref:`AT+RFPOWER <cmd-RFPOWER>` 命令设置。

  .. _faq-ble-notify-indicate:

  :ref:`低功耗蓝牙客户端如何使能 notify 和 indicate 功能？ <faq-at-index>`
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  - 低功耗蓝牙的特征的属性除了读、写还有 ``notify`` 和 ``indicate``。这两种都是服务端向客户端发送数据的方式，但是要想真的发送成功需要客户端提前注册 ``notification``，也就是写 ``CCCD`` 的值。
  - 如果要使能 ``notify``，需要写 ``0x01``；如果要使能 ``indicate``，需要写 ``0x02`` （写 ``0x2902`` 这个描述符）；如果是既想使能 ``notify`` 又想使能 ``indicate``，需要写 ``0x03``。
  - 比如，ESP-AT 的默认的服务中，``0xC305`` 是可 ``notify`` 的，``0xC306`` 是可 ``indicate`` 的。我们分别写这两个特征下面的 ``0x2902`` 描述符：

    .. code-block:: text

      AT+BLEGATTCWR=0,3,6,1,2
      >
      // 写低位 0x01 高位 0x00（如果要使用 hex 格式写的话就是：0100）
      OK
      // server: +WRITE:0,1,6,1,2,<0x01>,<0x00>
      AT+BLEGATTCWR=0,3,7,1,2
      >
      // 写低位 0x02 高位 0x00（如果要使用 hex 格式写的话就是：0200）
      OK
      // server: +WRITE:0,1,6,1,2,<0x02>,<0x00>
      // 写 ccc 是 server 可以发送 notify 和 indicate 的前提条件

  .. _faq-hardware:

.. only:: esp32s2

    .. _faq-hardware:

:ref:`硬件 <faq-at-index>`
-------------------------------

.. _faq-flash-size:

:ref:`在不同模组上的 AT 固件要求芯片 flash 多大？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

参考 :doc:`ESP-AT 固件差异 <Compile_and_Develop/esp-at_firmware_differences>`。

.. _faq-view-error-log:

:ref:`AT 固件如何查看 error log？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

通常从 :term:`AT 日志端口` 查看 error log。

.. _faq-uart1-pin:

:ref:`AT 在 {IDF_TARGET_NAME} 模组上的 UART1 通信管脚与 {IDF_TARGET_NAME} 模组的 datasheet 默认 UART1 管脚不一致？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

{IDF_TARGET_NAME} 支持 IO 矩阵变换，编译 ESP-AT 时可在 menuconfig 中配置 UART1 管脚，因此可能与 datasheet 默认管脚不一致。
管脚详情参考 `factory_param_data.csv <https://github.com/espressif/esp-at/blob/master/components/customized_partitions/raw_data/factory_param/factory_param_data.csv>`_。

.. _faq-performance:

:ref:`性能 <faq-at-index>`
-------------------------------

.. _faq-wifi-conn-time:

:ref:`AT Wi-Fi 连接耗时多少？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

办公室场景下实测约 5 秒。实际耗时取决于路由器性能、网络环境、模块天线性能等因素。
可通过 :ref:`AT+CWJAP <cmd-JAP>` 的 **<jap_timeout>** 参数设置最大超时时间。

.. _faq-tcp-window:

:ref:`AT 固件中 TCP 发送窗口大小是否可以修改？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

无法通过命令修改，需重新编译固件。在 menuconfig 中配置：``Component config`` > ``LWIP`` > ``TCP`` > ``Default send buffer size``。

.. _faq-throughput:

:ref:`AT 吞吐量如何测试及优化？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

建议使用 esp-idf 中的 iperf 示例进行测试（AT 测试时请使用透传方式，并将数据量调整为 1460 字节连续发送）。
若速率不满足需求，参考 :doc:`Compile_and_Develop/How_to_optimize_throughput` 进行优化。

.. _faq-tcp-retrans:

:ref:`如何修改 AT 默认 TCP 数据段最大重传次数？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

默认重传最大次数为 6 次，取值范围 [3-12]。重新配置方法：

- **本地编译**：参考 :doc:`本地编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在 menuconfig 中配置：
  
  ::

    Component config > LWIP > TCP > Maximum number of retransmissions of data segments

- **网页编译**：参考 :doc:`网页编译 ESP-AT 工程 <../Compile_and_Develop/How_to_build_project_with_web_page>`，修改 `CONFIG_LWIP_TCP_MAXRTX <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-reference/kconfig.html#config-lwip-tcp-maxrtx>`_ 的值。

.. _faq-other:

:ref:`其他 <faq-at-index>`
-------------------------------

.. _faq-at-interface:

:ref:`乐鑫芯片可以通过哪些接口来传输 AT 命令？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. list::

  :esp32 or esp32c5: - {IDF_TARGET_NAME} 支持 UART、SDIO 接口通信
  :esp32c2 or esp32c3 or esp32c5 or esp32c6 or esp32c61: - {IDF_TARGET_NAME} 支持 UART、SPI 接口通信
  - AT 默认固件使用 UART 接口。如需使用 SDIO 或 SPI 接口，需基于 ESP-AT 配置编译，详情见 :doc:`编译和开发 <Compile_and_Develop/index>`
  - 参考文档：:project_file:`使用 AT SDIO 接口 <main/interface/sdio/README.md>`，:project_file:`使用 AT SPI 接口 <main/interface/spi/README.md>`，:project_file:`使用 AT 套接字接口 <main/interface/socket/README.md>`

.. _faq-debug-log:

:ref:`AT 如何使能调试日志？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

参考 :doc:`如何启用更多 AT 调试日志 <../Compile_and_Develop/How_to_enable_more_AT_debug_logs>` 文档。

.. _faq-default-wifi-mode:

:ref:`如何修改 AT 固件首次启动时的默认 Wi-Fi 模式？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

修改源码，在 :cpp:type:`esp_at_ready_before()` 函数中调用 `esp_wifi_set_mode() <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-reference/network/esp_wifi.html#_CPPv417esp_wifi_set_mode11wifi_mode_t>`_ 设置 Wi-Fi 模式，例如 ``esp_wifi_set_mode(WIFI_MODE_STA)``。

.. _faq-http-resume:

:ref:`AT 指令如何实现 HTTP 断点续传功能？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

两种方法：

  - 使用 HTTP 的 Range 字段指定读取范围，参考 :ref:`AT+HTTPCHEAD 示例 <cmd-HTTPCHEAD_example>`
  - 使用 AT TCP 指令自行构造 HTTP GET 请求。在 :ref:`{IDF_TARGET_NAME} 设备获取被动接收模式下的套接字数据示例 <example-passive_recv>` 的第 6 步和第 7 步之间，使用 :ref:`AT+CIPSEND <cmd-SEND>` 发送构造的 HTTP GET 请求。被动接收模式下，使用 :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>` 读取数据

.. _faq-http-fs:

:ref:`如何从 HTTP 服务器下载文件并存储到 FATFS，或将 FATFS 文件系统中的文件上传到 HTTP 服务器？ <faq-at-index>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

参考 GitHub 示例：

  - :example:`at_http_get_to_fs` - 从 HTTP 服务器下载文件并存储到 FATFS 文件系统
  - :example:`at_fs_to_http_server` - 将 FATFS 文件系统中的文件上传到 HTTP 服务器

.. only:: esp32

  .. _faq-bqb:

  :ref:`如何进行 BQB 认证？ <faq-at-index>`
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  可参考 `ESP32 更新多项 BQB 蓝牙认证 <https://mp.weixin.qq.com/s?__biz=MzA4Nzc5MjkwNw==&mid=2651783810&idx=1&sn=fb0e132af240606d8178347966721542&chksm=8bcfaee6bcb827f03992aa200a2eb2baef5114712a4001da0c8282502a9183f5379605412cea&mpshare=1&scene=1&srcid=0920VLpOLubCew48DrCRdjCT&sharer_sharetime=1583218643838&sharer_shareid=1a1137fefea7b87a843519e48151f9a4&rd2werd=1#wechat_redirect>`_。

  .. _faq-ppp:

  :ref:`AT 支持 PPP 吗? <faq-at-index>`
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  不支持，可参考 `pppos_client <https://github.com/espressif/esp-idf/tree/v4.4.2/examples/protocols/pppos_client>`_ 示例自行实现。
