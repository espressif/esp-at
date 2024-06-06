.. _Basic-AT:

基础 AT 命令集
=================

:link_to_translation:`en:[English]`

.. list::

  - :ref:`介绍 <cmd-basic-intro>`
  - :ref:`AT <cmd-AT>`：测试 AT 启动
  - :ref:`AT+RST <cmd-RST>`：重启模块
  - :ref:`AT+GMR <cmd-GMR>`：查看版本信息
  - :ref:`AT+CMD <cmd-cmd>`：查询当前固件支持的所有命令及命令类型
  - :ref:`AT+GSLP <cmd-GSLP>`：进⼊ Deep-sleep 模式
  - :ref:`ATE <cmd-ATE>`：开启或关闭 AT 回显功能
  - :ref:`AT+RESTORE <cmd-RESTORE>`：恢复出厂设置
  - :ref:`AT+SAVETRANSLINK <cmd-SAVET>`：设置开机 :term:`透传模式` 信息
  - :ref:`AT+TRANSINTVL <cmd-TRANSINTVL>`：设置 :term:`透传模式` 模式下的数据发送间隔
  - :ref:`AT+UART_CUR <cmd-UARTC>`：设置 UART 当前临时配置，不保存到 flash
  - :ref:`AT+UART_DEF <cmd-UARTD>`：设置 UART 默认配置, 保存到 flash
  - :ref:`AT+SLEEP <cmd-SLEEP>`：设置 sleep 模式
  - :ref:`AT+SYSRAM <cmd-SYSRAM>`：查询当前剩余堆空间和最小堆空间
  - :ref:`AT+SYSMSG <cmd-SYSMSG>`：查询/设置系统提示信息
  - :ref:`AT+SYSMSGFILTER <cmd-SYSMSGFILTER>`：启用或禁用 :term:`系统消息` 过滤
  - :ref:`AT+SYSMSGFILTERCFG <cmd-SYSMSGFILTERCFG>`：查询/配置 :term:`系统消息` 的过滤器
  - :ref:`AT+SYSFLASH <cmd-SYSFLASH>`：查询或读写 flash 用户分区
  - :ref:`AT+SYSMFG <cmd-SYSMFG>`：查询或读写 :term:`manufacturing nvs` 用户分区
  - :ref:`AT+RFPOWER <cmd-RFPOWER>`：查询/设置 RF TX Power
  - :ref:`AT+SYSROLLBACK <cmd-SYSROLLBACK>`：回滚到以前的固件
  - :ref:`AT+SYSTIMESTAMP <cmd-SETTIME>`：查询/设置本地时间戳
  - :ref:`AT+SYSLOG <cmd-SYSLOG>`：启用或禁用 AT 错误代码提示
  - :ref:`AT+SLEEPWKCFG <cmd-WKCFG>`：设置 Light-sleep 唤醒源和唤醒 GPIO
  - :ref:`AT+SYSSTORE <cmd-SYSSTORE>`：设置参数存储模式
  - :ref:`AT+SYSREG <cmd-SYSREG>`：读写寄存器
  :esp32c3: - :ref:`AT+SYSTEMP <cmd-SYSTEMP>`：读取芯片内部摄氏温度值

.. _cmd-basic-intro:

介绍
------

.. important::
  默认的 AT 固件支持此页面下的所有 AT 命令。

.. _cmd-AT:

:ref:`AT <Basic-AT>`：测试 AT 启动
------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    AT  

**响应：**

::

    OK  

.. _cmd-RST:

:ref:`AT+RST <Basic-AT>`：重启模块
-------------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    AT+RST  

**响应：**

::

    OK  

.. only:: esp32c2 or esp32c3

    设置命令
    ^^^^^^^^

    **命令：**

    ::

        AT+RST=<mode>

    **响应：**

    ::

        OK

    参数
    ^^^^

    - **<mode>**：

      - 0：重启 {IDF_TARGET_NAME} 并进入正常运行模式
      - 1：重启 {IDF_TARGET_NAME} 并进入固件下载模式

    说明
    ^^^^

    - 如果您要实现下载，可以考虑发送此设置命令让 {IDF_TARGET_NAME} 进入下载模式，这样您可以在硬件上节省 Boot 管脚。

.. _cmd-GMR:

:ref:`AT+GMR <Basic-AT>`：查看版本信息
--------------------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    AT+GMR

**响应：**

::

    <AT version info>
    <SDK version info>
    <compile time>
    <Bin version>

    OK

参数
^^^^

-  **<AT version info>**：AT 核心库的版本信息，它们在 ``esp-at/components/at/lib/`` 目录下。代码是闭源的，无开放计划。
-  **<SDK version info>**：AT 使用的平台 SDK 版本信息，它们定义在 ``esp-at/module_config/module_{platform}_default/IDF_VERSION`` 文件中。
-  **<compile time>**：固件生成时间。
-  **<Bin version>**: AT 固件版本信息。版本信息可以在 menuconfig 中修改。（``python build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``AT firmware version.``）

说明
^^^^

- 如果您在使用 ESP-AT 固件中有任何问题，请先提供 ``AT+GMR`` 版本信息。

示例
^^^^

::

    AT+GMR
    AT version:2.2.0.0-dev(ca41ec4 - {IDF_TARGET_NAME} - Sep 16 2020 11:28:17)
    SDK version:v4.0.1-193-ge7ac221b4
    compile time(98b95fc):Oct 29 2020 11:23:25
    Bin version:2.1.0(MINI-1)

    OK

.. _cmd-CMD:

:ref:`AT+CMD <Basic-AT>`：查询当前固件支持的所有命令及命令类型
----------------------------------------------------------------

查询命令
^^^^^^^^

**命令：**

::

    AT+CMD?

**响应：**

::

    +CMD:<index>,<AT command name>,<support test command>,<support query command>,<support set command>,<support execute command>

    OK

参数
^^^^

-  **<index>**：AT 命令序号
-  **<AT command name>**：AT 命令名称
-  **<support test command>**：0 表示不支持，1 表示支持
-  **<support query command>**：0 表示不支持，1 表示支持
-  **<support set command>**：0 表示不支持，1 表示支持
-  **<support execute command>**：0 表示不支持，1 表示支持

.. _cmd-GSLP:

:ref:`AT+GSLP <Basic-AT>`：进入 Deep-sleep 模式
--------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+GSLP=<time>

**响应：**

::

    <time>

    OK

参数
^^^^

-  **<time>**：设备进入 Deep-sleep 的时长，单位：毫秒。设定时间到后，设备自动唤醒，调用深度睡眠唤醒桩，然后加载应用程序。

   .. only:: esp32c3 or esp32c2 or esp32 or esp32s2

       - 0 表示立即重启

   .. only:: esp32c6

       - 最小 Deep-sleep 时长为 5 毫秒。

   - 最大 Deep-sleep 时长约为 28.8 天（2 :sup:`31`-1 毫秒）。

说明
^^^^

- 由于外部因素的影响，所有设备进入 Deep-sleep 的实际时长与理论时长之间会存在差异。

.. _cmd-ATE:

:ref:`ATE <Basic-AT>`：开启或关闭 AT 回显功能
----------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    ATE0  

或

::

    ATE1  

**响应：**

::

    OK  

参数
^^^^

-  **ATE0**：关闭回显
-  **ATE1**：开启回显

.. _cmd-RESTORE:

:ref:`AT+RESTORE <Basic-AT>`：恢复出厂设置
-----------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    AT+RESTORE  

**响应：**

::

    OK  

说明
^^^^

-  该命令将擦除所有保存到 flash 的参数，并恢复为默认参数。
-  运行该命令会重启设备。

.. _cmd-SAVET:

:ref:`AT+SAVETRANSLINK <Basic-AT>`：设置开机 Wi-Fi/Bluetooth LE :term:`透传模式` 信息
-----------------------------------------------------------------------------------------

.. list::

    * :ref:`savetrans-tcpssl`
    * :ref:`savetrans-udp`
    :esp32 or esp32c3 or esp32c6 or esp32c2: * :ref:`savetrans-ble`

.. _savetrans-tcpssl:

设置开机进入 TCP/SSL :term:`透传模式` 信息
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

设置命令
""""""""""""""

**命令：**

::

    AT+SAVETRANSLINK=<mode>,<"remote host">,<remote port>[,<"type">,<keep_alive>]

**响应：**

::

    OK

参数
""""""""""""""

-  **<mode>**:

   -  0: 关闭 {IDF_TARGET_NAME} 上电进入 Wi-Fi :term:`透传模式`
   -  1: 开启 {IDF_TARGET_NAME} 上电进入 Wi-Fi :term:`透传模式`

-  **<"remote host">**：字符串参数，表示远端 IPv4 地址、IPv6 地址，或域名。最长为 64 字节。
-  **<remote port>**：远端端口值
-  **<"type">**：字符串参数，表示传输类型："TCP"，"TCPv6"，"SSL"，或 "SSLv6"。默认值："TCP"
-  **<keep_alive>**：配置套接字的 ``SO_KEEPALIVE`` 选项（参考：`SO_KEEPALIVE 介绍 <https://man7.org/linux/man-pages/man7/socket.7.html#SO_KEEPALIVE>`_），单位：秒。

  - 范围：[0,7200]。

    - 0：禁用 keep-alive 功能；（默认）
    - 1 ~ 7200：开启 keep-alive 功能。`TCP_KEEPIDLE <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPIDLE>`_ 值为 **<keep_alive>**，`TCP_KEEPINTVL <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPINTVL>`_ 值为 1，`TCP_KEEPCNT <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPCNT>`_ 值为 3。

  -  本命令中的 ``<keep_alive>`` 参数与 :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` 命令中的 ``<keep_alive>`` 参数相同，最终值由后设置的命令决定。如果运行本命令时不设置 ``<keep_alive>`` 参数，则默认使用上次配置的值。

说明
"""""""

- 本设置将 Wi-Fi 开机 :term:`透传模式` 信息保存在 NVS 区，若参数 ``<mode>`` 为 1 ，下次上电自动进入 :term:`透传模式`。需重启生效。

示例
""""""""

::

    AT+SAVETRANSLINK=1,"192.168.6.110",1002,"TCP"
    AT+SAVETRANSLINK=1,"www.baidu.com",443,"SSL"
    AT+SAVETRANSLINK=1,"240e:3a1:2070:11c0:55ce:4e19:9649:b75",8080,"TCPv6"
    AT+SAVETRANSLINK=1,"240e:3a1:2070:11c0:55ce:4e19:9649:b75",8080,"SSLv6

.. _savetrans-udp:

设置开机进入 UDP :term:`透传模式` 信息
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

设置
""""

**命令：**

::

    AT+SAVETRANSLINK=<mode>,<"remote host">,<remote port>,[<"type">,<local port>]

**响应：**

::

    OK

参数
""""

-  **<mode>**:

   -  0: 关闭 {IDF_TARGET_NAME} 上电进入 Wi-Fi :term:`透传模式`
   -  1: 开启 {IDF_TARGET_NAME} 上电进入 Wi-Fi :term:`透传模式`

-  **<"remote host">**：字符串参数，表示远端 IPv4 地址、IPv6 地址，或域名。最长为 64 字节。
-  **<remote port>**：远端端口值
-  **<"type">**：字符串参数，表示传输类型："UDP" 或 "UDPv6"。默认值："TCP"
-  **[<local port>]**：开机进入 UDP 传输时，使用的本地端口

说明
"""""""

- 本设置将 Wi-Fi 开机 :term:`透传模式` 信息保存在 NVS 区，若参数 ``<mode>`` 为 1 ，下次上电自动进入 :term:`透传模式`。需重启生效。

- 如果您想基于 IPv6 网络建立一个 UDP 传输，请执行以下操作：

  - 确保 AP 支持 IPv6
  - 设置 :ref:`AT+CIPV6=1 <cmd-IPV6>`
  - 通过 :ref:`AT+CWJAP <cmd-JAP>` 命令获取到一个 IPv6 地址
  - （可选）通过 :ref:`AT+CIPSTA? <cmd-IPSTA>` 命令检查 {IDF_TARGET_NAME} 是否获取到 IPv6 地址

示例
"""""""""

::

    AT+SAVETRANSLINK=1,"192.168.6.110",1002,"UDP",1005
    AT+SAVETRANSLINK=1,"240e:3a1:2070:11c0:55ce:4e19:9649:b75",8081,"UDPv6",1005

.. only:: esp32c2 or esp32c3 or esp32c6 or esp32

    .. _savetrans-ble:

    设置开机进入 BLE :term:`透传模式` 信息
    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    设置
    """"

    **命令：**

    ::

        AT+SAVETRANSLINK=<mode>,<role>,<tx_srv>,<tx_char>,<rx_srv>,<rx_char>,<peer_addr>

    **响应：**

    ::

        OK

    参数
    """"

    -  **<mode>**：

        -  0: 关闭 {IDF_TARGET_NAME} 上电进入 BLE :term:`透传模式`
        -  2: 开启 {IDF_TARGET_NAME} 上电进入 BLE :term:`透传模式`

    -  **<role>**：

        -  1: client 角色
        -  2: server 角色

    -  **<tx_srv>**：tx 服务序号。AT 作为 GATTC 时，通过 :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> 命令查询；作为 GATTS 时，通过 :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` 命令查询。
    -  **<tx_char>**：tx 服务特征序号。AT 作为 GATTC 时，通过 :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> 命令查询；作为 GATTS 时，通过 :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` 命令查询。
    -  **<rx_srv>**：rx 服务序号。AT 作为 GATTC 时，通过 :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> 命令查询；作为 GATTS 时，通过 :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` 命令查询。
    -  **<rx_char>**：rx 服务特征序号。AT 作为 GATTC 时，通过 :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> 命令查询；作为 GATTS 时，通过 :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` 命令查询。
    -  **<peer_addr>**：对方 Bluetooth LE 地址

    说明
    """""""

    - 本设置将 BLE 开机 :term:`透传模式` 信息保存在 NVS 区，若参数 ``<mode>`` 为 2，下次上电自动进入 Bluetooth LE :term:`透传模式`。需重启生效。

    示例
    """""""""

    ::

        AT+SAVETRANSLINK=2,2,1,7,1,5,"26:a2:11:22:33:88"

.. _cmd-TRANSINTVL:

:ref:`AT+TRANSINTVL <Basic-AT>`：设置 :term:`透传模式` 模式下的数据发送间隔
----------------------------------------------------------------------------------

查询命令
^^^^^^^^

**命令：**

::

    AT+TRANSINTVL?

**响应：**

::

    +TRANSINTVL:<interval>

    OK

设置命令
^^^^^^^^

**命令：**

::

    AT+TRANSINTVL=<interval>

**响应：**

::

    OK

参数
^^^^

-  **<interval>**：数据发送间隔。单位：毫秒。默认值：20。范围：[0,1000]。

说明
^^^^

- 透传模式下，当 {IDF_TARGET_NAME} 从 UART 接收到数据后，如果收到的数据长度大于等于 2920 字节，数据会立即被分为每 2920 字节一组的块进行发送，否则会等待 ``<interval>`` 毫秒或等待收到的数据大于等于 2920 字节再发送数据。
- 当数据量很小，且数据发送间隔很短时，可以通过设置 ``<interval>`` 来调整数据发送的时机。当 ``<interval>`` 很小时，可以降低向协议栈发送数据的延时，但这会增加协议栈数据向网络发送的次数，一定程度降低了吞吐性能。

示例
^^^^

::

    // 设置收到数据后立即发送
    AT+TRANSINTVL=0

.. _cmd-UARTC:

:ref:`AT+UART_CUR <Basic-AT>`：设置 UART 当前临时配置，不保存到 flash
----------------------------------------------------------------------------------

查询命令
^^^^^^^^

**命令：**

::

    AT+UART_CUR?

**响应：**

::

    +UART_CUR:<baudrate>,<databits>,<stopbits>,<parity>,<flow control>

    OK

设置命令
^^^^^^^^

**命令：**

::

    AT+UART_CUR=<baudrate>,<databits>,<stopbits>,<parity>,<flow control>

**响应：**

::

    OK

参数
^^^^

-  **<baudrate>**：UART 波特率

   - {IDF_TARGET_NAME} 设备：支持范围为 80 ~ 5000000

-  **<databits>**：数据位

   -  5：5 bit 数据位
   -  6：6 bit 数据位
   -  7：7 bit 数据位
   -  8：8 bit 数据位

-  **<stopbits>**：停止位

   -  1：1 bit 停止位
   -  2：1.5 bit 停止位
   -  3：2 bit 停止位

-  **<parity>**：校验位

   -  0：None
   -  1：Odd
   -  2：Even

-  **<flow control>**：流控

   -  0：不使能流控
   -  1：使能 RTS
   -  2：使能 CTS
   -  3：同时使能 RTS 和 CTS

说明
^^^^

-  查询命令返回的是 UART 配置参数的实际值，由于时钟分频的原因，可能与设定值有细微的差异。
-  本设置不保存到 flash。
-  使用硬件流控功能需要连接设备的 CTS/RTS 管脚，详情请见 :doc:`../Get_Started/Hardware_connection` 和 ``components/customized_partitions/raw_data/factory_param/factory_param_data.csv``。

示例
^^^^

::

    AT+UART_CUR=115200,8,1,0,3  

.. _cmd-UARTD:

:ref:`AT+UART_DEF <Basic-AT>`：设置 UART 默认配置，保存到 flash
----------------------------------------------------------------

查询命令
^^^^^^^^

**命令：**

::

    AT+UART_DEF?

**响应：**

::

    +UART_DEF:<baudrate>,<databits>,<stopbits>,<parity>,<flow control>

    OK

设置命令
^^^^^^^^

**命令：**

::

    AT+UART_DEF=<baudrate>,<databits>,<stopbits>,<parity>,<flow control>

**响应：**

::

    OK

参数
^^^^

-  **<baudrate>**：UART 波特率

   - {IDF_TARGET_NAME} 设备：支持范围为 80 ~ 5000000

-  **<databits>**：数据位

   -  5：5 bit 数据位
   -  6：6 bit 数据位
   -  7：7 bit 数据位
   -  8：8 bit 数据位

-  **<stopbits>**：停止位

   -  1：1 bit 停止位
   -  2：1.5 bit 停止位
   -  3：2 bit 停止位

-  **<parity>**：校验位

   -  0：None
   -  1：Odd
   -  2：Even

-  **<flow control>**：流控

   -  0：不使能流控
   -  1：使能 RTS
   -  2：使能 CTS
   -  3：同时使能 RTS 和 CTS

说明
^^^^

-  配置更改将保存在 NVS 分区，当设备再次上电时仍然有效。
-  使用硬件流控功能需要连接设备的 CTS/RTS 管脚，详情请见 :doc:`../Get_Started/Hardware_connection` 和 ``components/customized_partitions/raw_data/factory_param/factory_param_data.csv``。 

示例
^^^^

::

    AT+UART_DEF=115200,8,1,0,3  

.. _cmd-SLEEP:

:ref:`AT+SLEEP <Basic-AT>`：设置睡眠模式
---------------------------------------------------

查询命令
^^^^^^^^

**命令：**

::

    AT+SLEEP?

**响应：**

::

    +SLEEP:<sleep mode>

    OK

设置命令
^^^^^^^^

**命令：**

::

    AT+SLEEP=<sleep mode>

**响应：**

::

    OK

参数
^^^^

-  **<sleep mode>**：

   - 0：禁用睡眠模式

   - 1：Modem-sleep 模式

     - 单 Wi-Fi 模式

       - 射频模块将根据 AP 的 ``DTIM`` 定期关闭

     - 单 BLE 模式

       - 在 BLE 广播态下，射频模块将根据广播间隔定期关闭
       - 在 BLE 连接态下，射频模块将根据连接间隔定期关闭

   - 2：Light-sleep 模式

     - 无 Wi-Fi 模式

       - CPU 将自动进入睡眠，射频模块将关闭

     - 单 Wi-Fi 模式

       - CPU 将自动进入睡眠，射频模块也将根据 :ref:`AT+CWJAP <cmd-JAP>` 命令设置的 ``listen interval`` 参数定期关闭

     - 单 Bluetooth 模式

       - 在 Bluetooth 广播态下，CPU 将自动进入睡眠，射频模块也将根据广播间隔定期关闭
       - 在 Bluetooth 连接态下，CPU 将自动进入睡眠，射频模块也将根据连接间隔定期关闭

     - Wi-Fi 和 Bluetooth 共存模式

        - CPU 将自动进入睡眠，射频模块根据电源管理模块定期关闭

   - 3：Modem-sleep listen interval 模式

     - 单 Wi-Fi 模式

       - 射频模块将根据 :ref:`AT+CWJAP <cmd-JAP>` 命令设置的 ``listen interval`` 参数定期关闭

     - 单 BLE 模式

       - 在 BLE 广播态下，射频模块将根据广播间隔定期关闭
       - 在 BLE 连接态下，射频模块将根据连接间隔定期关闭

说明
^^^^

-  当禁用睡眠模式后，Bluetooth LE 不可以被初始化。当 Bluetooth LE 初始化后，不可以禁用睡眠模式。
-  Modem-sleep 模式和 Light-sleep 模式均可以在 Wi-Fi 模式或者 BLE 模式下设置，但在 Wi-Fi 模式下，这两种模式只能在 ``station`` 模式下设置
-  设置 Light-sleep 模式前，建议提前通过 :ref:`AT+SLEEPWKCFG <cmd-WKCFG>` 命令设置好唤醒源，否则没法唤醒，设备将一直处于睡眠状态
-  设置 Light-sleep 模式后，如果 Light-sleep 唤醒条件不满足时，设备将自动进入睡眠模式，当 Light-sleep 唤醒条件满足时，设备将自动从睡眠模式中唤醒
-  对于 BLE 模式下的 Light-sleep 模式，用户必须确保外接 32KHz 晶振，否则，Light-sleep 模式会以 Modem-sleep 模式工作。
-  AT+SLEEP 更多示例请参考文档 :doc:`../AT_Command_Examples/sleep_at_examples`。

示例
^^^^

::

    AT+SLEEP=0

.. _cmd-SYSRAM:

:ref:`AT+SYSRAM <Basic-AT>`：查询当前剩余堆空间和最小堆空间
------------------------------------------------------------

查询命令
^^^^^^^^

**命令：**

::

    AT+SYSRAM?  

**响应：**

::

    +SYSRAM:<remaining RAM size>,<minimum heap size>
    OK  

参数
^^^^

-  **<remaining RAM size>**：当前剩余堆空间，单位：byte
-  **<minimum heap size>**：运行时的最小堆空间，单位：byte。当 ``<minimum heap size>`` 小于或接近于 10 KB 时，{IDF_TARGET_NAME} 的 Wi-Fi 和低功耗蓝牙的功能可能会受影响。

示例
^^^^

::

    AT+SYSRAM?
    +SYSRAM:148408,84044
    OK

.. _cmd-SYSMSG:

:ref:`AT+SYSMSG <Basic-AT>`：查询/设置系统提示信息
-----------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询当前系统提示信息状态

**命令：**

::

    AT+SYSMSG?

**响应：**

::

    +SYSMSG:<state>
    OK

设置命令
^^^^^^^^

**功能：**

设置系统提示信息。如果您需要更加精细的管理 AT 消息，请使用 :ref:`AT+SYSMSGFILTER <cmd-SYSMSGFILTER>` 命令。

**命令：**

::

    AT+SYSMSG=<state>

**响应：**

::

    OK  

参数
^^^^

-  **<state>**：

   - Bit0：退出 Wi-Fi :term:`透传模式`, Bluetooth LE SPP 及 Bluetooth SPP 时是否打印提示信息

     - 0：不打印
     - 1：打印 ``+QUITT``

   - Bit1：连接时提示信息类型

     - 0：使用简单版提示信息，如 ``XX,CONNECT``
     - 1：使用详细版提示信息，如 ``+LINK_CONN:status_type,link_id,ip_type,terminal_type,remote_ip,remote_port,local_port``

   - Bit2：连接状态提示信息，适用于 Wi-Fi :term:`透传模式`、Bluetooth LE SPP 及 Bluetooth SPP

     - 0：不打印提示信息
     - 1：当 Wi-Fi、socket、Bluetooth LE 或 Bluetooth 状态发生改变时，打印提示信息，如：

      ::

           - "CONNECT\r\n" 或以 "+LINK_CONN:" 开头的提示信息
           - "CLOSED\r\n"
           - "WIFI CONNECTED\r\n"
           - "WIFI GOT IP\r\n"
           - "WIFI GOT IPv6 LL\r\n"
           - "WIFI GOT IPv6 GL\r\n"
           - "WIFI DISCONNECT\r\n"
           - "+ETH_CONNECTED\r\n"
           - "+ETH_DISCONNECTED\r\n"
           - 以 "+ETH_GOT_IP:" 开头的提示信息
           - 以 "+STA_CONNECTED:" 开头的提示信息
           - 以 "+STA_DISCONNECTED:" 开头的提示信息
           - 以 "+DIST_STA_IP:" 开头的提示信息
           - 以 "+BLECONN:" 开头的提示信息 
           - 以 "+BLEDISCONN:" 开头的提示信息

说明
^^^^

-  若 :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`，配置更改将被保存在 NVS 分区。
-  若设 Bit0 为 1，退出 Wi-Fi :term:`透传模式` 时会提示 ``+QUITT``。
-  若设 Bit1 为 1，将会影响 :ref:`AT+CIPSTART <cmd-START>` 和 :ref:`AT+CIPSERVER <cmd-SERVER>` 命令，系统将提示 "+LINK_CONN:status_type,link_id,ip_type,terminal_type,remote_ip,remote_port,local_port"，而不是 "XX,CONNECT"。

示例
^^^^

::

    // 退出 Wi-Fi 透传模式时不打印提示信息
    // 连接时打印详细版提示信息
    // 连接状态发生改变时不打印信息
    AT+SYSMSG=2

或

::

    // 透传模式下，Wi-Fi、socket、Bluetooth LE 或 Bluetooth 状态改变时会打印提示信息 
    AT+SYSMSG=4

.. _cmd-SYSMSGFILTER:

:ref:`AT+SYSMSGFILTER <Basic-AT>`：启用或禁用 :term:`系统消息` 过滤
--------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询当前系统信息过滤的状态

**命令：**

::

    AT+SYSMSGFILTER?

**响应：**

::

    +SYSMSGFILTER:<enable>

    OK

设置命令
^^^^^^^^

**功能：**

启用或禁用系统消息过滤

**命令：**

::

    AT+SYSMSGFILTER=<enable>

**响应：**

::

    OK

参数
^^^^

- **<enable>**：

    - 0：禁用系统消息过滤。系统默认值。禁用后，系统消息不会被设置的过滤器过滤。
    - 1：启用系统消息过滤。开启后，系统消息被正则表达式匹配上的数据会被 AT 过滤掉，MCU 不会收到；而未被正则表达式匹配上的数据，会原样发往 MCU。

说明
^^^^

- 请先使用 :ref:`AT+SYSMSGFILTERCFG <cmd-SYSMSGFILTERCFG>` 命令配置有效的过滤器，再通过本命令启用或禁用系统消息过滤，实现更加精细的系统消息管理。
- 请谨慎使用 :ref:`AT+SYSMSGFILTER=1 <cmd-SYSMSGFILTER>` 命令，建议您开启系统消息过滤后要及时禁用，防止 AT 的系统消息被过度过滤。
- 在进入 :term:`透传模式` 前，强烈建议使用 :ref:`AT+SYSMSGFILTER=0 <cmd-SYSMSGFILTER>` 命令，禁用系统消息过滤。
- 如果您基于 AT 工程二次开发，请使用如下的 APIs 实现 AT 命令口的数据发送。

::

    // 原生的 AT 命令口的数据发送。数据不会被 AT+SYSMSGFILTER 命令过滤，发送数据前也不会唤醒 MCU（AT+USERWKMCUCFG 命令设置的 MCU 唤醒功能）。
    int32_t esp_at_port_write_data_without_filter(uint8_t data, int32_t len);

    // 具有过滤功能的 AT 命令口的数据发送。数据会被 AT+SYSMSGFILTER 命令过滤（如果启用），发送数据前不会唤醒 MCU（AT+USERWKMCUCFG 命令设置的 MCU 唤醒功能）。
    int32_t esp_at_port_write_data(uint8_t data, int32_t len);

    // 具有唤醒 MCU 功能的 AT 命令口的数据发送。数据不会被 AT+SYSMSGFILTER 命令过滤，发送数据前会唤醒 MCU（AT+USERWKMCUCFG 命令设置的 MCU 唤醒功能）。
    int32_t esp_at_port_active_write_data_without_filter(uint8_t data, int32_t len);

    // 同时具有唤醒 MCU 功能和过滤功能的 AT 命令口的数据发送。数据会被 AT+SYSMSGFILTER 命令过滤（如果启用），发送数据前会唤醒 MCU（AT+USERWKMCUCFG 命令设置的 MCU 唤醒功能）。
    int32_t esp_at_port_active_write_data(uint8_t data, int32_t len);

示例
""""""

详细示例参考： :ref:`系统消息过滤示例 <example-sysmfgfilter>`。

.. _cmd-SYSMSGFILTERCFG:

:ref:`AT+SYSMSGFILTERCFG <Basic-AT>`：查询/配置 :term:`系统消息` 的过滤器
-------------------------------------------------------------------------

* :ref:`sysmsgfiltercfg-query`
* :ref:`sysmsgfiltercfg-clear`
* :ref:`sysmsgfiltercfg-add`
* :ref:`sysmsgfiltercfg-delete`

.. _sysmsgfiltercfg-query:

查询过滤器
^^^^^^^^^^^^^^^^^

查询命令
""""""""""""""

**命令：**

::

    AT+SYSMSGFILTERCFG?

**响应：**

::

    +SYSMSGFILTERCFG:<index>,"<head_regexp>","<tail_regexp>"

    OK

参数
""""""""""""""

- **<index>**：过滤器的索引。
- **<"head_regexp">**：头部正则表达式。
- **<"tail_regexp">**：尾部正则表达式。

.. _sysmsgfiltercfg-clear:

清除所有过滤器
^^^^^^^^^^^^^^^^^^^

设置命令
""""""""""""""

**命令：**

::

    AT+SYSMSGFILTERCFG=<operator>

**响应：**

::

    OK

参数
""""""""""""""

- **<operator>**：

    - 0：清除所有过滤器。清除后，可以释放一些过滤器所占用的堆空间大小。

示例
""""""

::

    // 清除所有过滤器
    AT+SYSMSGFILTERCFG=0

.. _sysmsgfiltercfg-add:

增加一个过滤器
^^^^^^^^^^^^^^^^^^^

设置命令
""""""""""""""

**命令：**

::

    AT+SYSMSGFILTERCFG=<operator>,<head_regexp_len>,<tail_regexp_len>

**响应：**

::

    OK

    >

上述响应表示 AT 已准备好接收 AT 命令口的数据，此时您可以输入数据（即：头部正则表达式和尾部正则表达式），当 AT 接收到的数据长度达到 ``<head_regexp_len>`` + ``<tail_regexp_len>`` 后，进行正则表达式完整性校验。

如果正则表达式完整性校验失败或添加过滤器失败，返回：

::

    ERROR

如果正则表达式完整性校验成功且添加过滤器成功，返回：

::

    OK

参数
""""""""""""""

- **<operator>**：

    - 1：增加一个过滤器。一个过滤器包含头部正则表达式和尾部正则表达式。

- **<head_regexp_len>**：头部正则表达式长度。范围：[0,64]。如果设置为 0，代表忽略头部正则表达式的匹配，同时 ``<tail_regexp_len>`` 不能为 0。
- **<tail_regexp_len>**：尾部正则表达式长度。范围：[0,64]。如果设置为 0，代表忽略尾部正则表达式的匹配，同时 ``<head_regexp_len>`` 不能为 0。

说明
""""""

- 请先使用本命令配置有效的过滤器，再通过 :ref:`AT+SYSMSGFILTER <cmd-SYSMSGFILTER>` 命令启用或禁用系统消息过滤，实现更加精细的系统消息管理。
- 头部和尾部正则表达式格式参考 `POSIX 基本正则语法（BRE） <https://en.wikipedia.org/wiki/Regular_expression#POSIX_basic_and_extended>`_。
- 为了避免 :term:`系统消息` (AT 命令口的 TX 数据) 被错误过滤，**强烈建议** 头部正则表达式以 ``^`` 开头，尾部正则表达式以 ``$`` 结束。
- 只有系统消息 **同时匹配** 上头部正则表达式和尾部正则表达式时，系统消息才会被过滤。过滤后，系统消息被正则表达式匹配上的数据会被 AT 过滤掉，MCU 不会收到；而未被正则表达式匹配上的数据，会原样发往 MCU。
- 当系统消息匹配到一个过滤器后，不会再继续匹配其它的过滤器。
- 系统消息匹配过滤器时，系统消息不会缓存，即不会将上一条的系统消息和本条系统消息合在一起，进行匹配。
- 对于吞吐量较大的设备，强烈建议您设置较少的过滤器，同时及时通过 :ref:`AT+SYSMSGFILTER=0 <cmd-SYSMSGFILTER>` 命令禁用系统消息过滤。

示例
""""""

::

    // 设置过滤器，过滤掉 "WIFI CONNECTED" 系统消息报告
    AT+SYSMSGFILTERCFG=1,17,0
    // 等待命令返回 OK 和 > 后，输入 ^WIFI CONNECTED\r\n（注意 \r\n 占用 2 个字节，对应 ASCII 码中的 0D 0A）

    // 开启系统消息过滤
    AT+SYSMSGFILTER=1

    // 测试过滤功能
    AT+CWMODE=1
    AT+CWJAP="ssid","password"
    // AT 不再输出 WIFI CONNECTED 系统消息报告

详细示例参考：:ref:`系统消息过滤示例 <example-sysmfgfilter>`。

.. _sysmsgfiltercfg-delete:

删除一个过滤器
^^^^^^^^^^^^^^^^^^^

设置命令
""""""""""""""

**命令：**

::

    AT+SYSMSGFILTERCFG=<operator>,<head_regexp_len>,<tail_regexp_len>

**响应：**

::

    OK

    >

上述响应表示 AT 已准备好接收 AT 命令口的数据，此时您可以输入数据（即：头部正则表达式和尾部正则表达式），当 AT 接收到的数据长度达到 ``<head_regexp_len>`` + ``<tail_regexp_len>`` 后，进行正则表达式完整性校验。

如果正则表达式完整性校验失败或删除过滤器失败，返回：

::

    ERROR

如果正则表达式完整性校验成功且删除过滤器成功，返回：

::

    OK

参数
""""""""""""""

- **<operator>**：

    - 2：删除一个过滤器。

- **<head_regexp_len>**：头部正则表达式长度。范围：[0,64]。如果设置为 0，则 ``<tail_regexp_len>`` 不能为 0。
- **<tail_regexp_len>**：尾部正则表达式长度。范围：[0,64]。如果设置为 0，则 ``<head_regexp_len>`` 不能为 0。

说明
""""""

- 待删除的过滤器应在已增加的过滤器中。

示例
""""""

::

    // 删除上述添加的过滤器
    AT+SYSMSGFILTERCFG=2,17,0
    // 等待命令返回 OK 和 > 后，输入 ^WIFI CONNECTED\r\n（注意 \r\n 占用 2 个字节，对应 ASCII 码中的 0D 0A）

    // 测试功能
    AT+CWMODE=1
    AT+CWJAP="ssid","password"
    // AT 会输出 WIFI CONNECTED 系统消息报告

.. _cmd-SYSFLASH:

:ref:`AT+SYSFLASH <Basic-AT>`：查询或读写 flash 用户分区
---------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 flash 用户分区

**命令：**

::

    AT+SYSFLASH?

**响应：**

::

    +SYSFLASH:<partition>,<type>,<subtype>,<addr>,<size>
    OK  

设置命令
^^^^^^^^

**功能：**

读、写、擦除 flash 用户分区

**命令：**

::

    AT+SYSFLASH=<operation>,<partition>,<offset>,<length>

**响应：**

::

    +SYSFLASH:<length>,<data>
    OK  

参数
^^^^

-  **<operation>**：

   -  0：擦除分区
   -  1：写分区
   -  2：读分区

-  **<partition>**：用户分区名称
-  **<offset>**：偏移地址
-  **<length>**：数据长度
-  **<type>**：用户分区类型
-  **<subtype>**：用户分区子类型
-  **<addr>**：用户分区地址
-  **<size>**：用户分区大小

说明
^^^^

-  使用本命令需烧录 at_customize.bin，详细信息可参考 :doc:`../Compile_and_Develop/How_to_customize_partitions`。
-  擦除分区时，请完整擦除该目标分区。这可以通过省略 ``<offset>`` 和 ``<length>`` 参数来完成。例如，命令 ``AT+SYSFLASH=0,"mfg_nvs"`` 可擦除整个 "mfg_nvs" 区域。
-  关于分区的定义可参考 `ESP-IDF 分区表 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/partition-tables.html>`_。
-  当 ``<operator>`` 为 ``write`` 时，系统收到此命令后先换行返回 ``>``，此时您可以输入要写的数据，数据长度应与 ``<length>`` 一致。
-  写分区前，请先擦除该分区。
-  如果您想修改 mfg_nvs 分区中的某些数据，请使用 :ref:`AT+SYSMFG <cmd-SYSMFG>` 命令（NVS 中的键值对操作）。如果您想修改整个 mfg_nvs 分区，请使用 :ref:`AT+SYSFLASH <cmd-SYSFLASH>` 命令（分区操作）。

示例
^^^^

::

    // 擦除整个 "mfg_nvs" 分区
    AT+SYSFLASH=0,"mfg_nvs"

    // 在 "mfg_nvs" 分区偏移地址 0 处写入新的 "mfg_nvs" 分区（大小为 0x1C000）
    AT+SYSFLASH=1,"mfg_nvs",0,0x1C000

.. _cmd-SYSMFG:

:ref:`AT+SYSMFG <Basic-AT>`：查询或读写 :term:`manufacturing nvs` 用户分区
--------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 manufacturing nvs 用户分区内的命名空间 (namespace)

**命令：**

::

    AT+SYSMFG?

**响应：**

::

    +SYSMFG:<"namespace">

    OK

擦除命名空间或键值对
^^^^^^^^^^^^^^^^^^^^^^^

设置命令
"""""""""

**命令：**

::

    AT+SYSMFG=<operation>,<"namespace">[,<"key">]

**响应：**

::

    OK

参数
"""""

- **<operation>**：

   - 0：擦除操作
   - 1：读取操作
   - 2：写入操作

- **<"namespace">**：命名空间。
- **<"key">**：主键，或称为键。当 ``<"key">`` 缺省时，擦除 ``<"namespace">`` 内所有的键值对；否则只擦除当前指定的 ``<"key">`` 的键值对。

说明
^^^^
- 请先阅读 `非易失性存储 (NVS) <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-reference/storage/nvs_flash.html>`_，了解命名空间、键值对的概念。

示例
"""""

::

    // 擦除 client_cert 命名空间内所有的键值对（即：擦除默认的第 0 套和第 1 套客户端证书）
    AT+SYSMFG=0,"client_cert"

    // 擦除 client_cert 命名空间内的 client_cert.0 键值对（即：擦除默认的第 0 套客户端证书）
    AT+SYSMFG=0,"client_cert","client_cert.0"

读取命名空间或键值对
^^^^^^^^^^^^^^^^^^^^^^^

设置命令
"""""""""""

**命令：**

::

    AT+SYSMFG=<operation>[,<"namespace">][,<"key">][,<offset>,<length>]

**响应：**

当 ``<"namespace">`` 以及之后参数缺省时，返回：

::

    +SYSMFG:<"namespace">

    OK

当 ``<"key">`` 以及之后参数缺省时，返回：

::

    +SYSMFG:<"namespace">,<"key">,<type>

    OK

其余情况，返回：

::

    +SYSMFG:<"namespace">,<"key">,<type>,<length>,<value>

    OK

参数
"""""

- **<operation>**：

   - 0：擦除操作
   - 1：读取操作
   - 2：写入操作

- **<"namespace">**：命名空间。
- **<"key">**：主键，或称为键。
- **<offset>**：键值的偏移。
- **<length>**：键值的长度。
- **<type>**：键值的类型。

  - 1：u8
  - 2：i8
  - 3：u16
  - 4：i16
  - 5：u32
  - 6：i32
  - 7：string
  - 8：binary

- **<value>**：键值的数据。

说明
^^^^
- 请先阅读 `非易失性存储 (NVS) <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-reference/storage/nvs_flash.html>`_，了解命名空间、键值对的概念。

示例
"""""

::

    // 读取当前所有的命名空间
    AT+SYSMFG=1

    // 读取 client_cert 命名空间内所有的主键
    AT+SYSMFG=1,"client_cert"

    // 读取 client_cert 命名空间内的 client_cert.0 主键的值
    AT+SYSMFG=1,"client_cert","client_cert.0"

    // 读取 client_cert 命名空间内的 client_cert.0 主键的值，从偏移 100 的位置读取 200 字节
    AT+SYSMFG=1,"client_cert","client_cert.0",100,200

向命名空间内写入键值对
^^^^^^^^^^^^^^^^^^^^^^^

设置命令
"""""""""

**命令：**

::

    AT+SYSMFG=<operation>,<"namespace">,<"key">,<type>,<value>

**响应：**

::

    OK

参数
"""""

- **<operation>**：

   - 0：擦除操作
   - 1：读取操作
   - 2：写入操作

- **<"namespace">**：命名空间。
- **<"key">**：主键，或称为键。
- **<type>**：键值的类型。

  - 1：u8
  - 2：i8
  - 3：u16
  - 4：i16
  - 5：u32
  - 6：i32
  - 7：string
  - 8：binary

- **<value>**：参数 ``<type>`` 不同，则此参数意义不同：

  - 如果 ``<type>`` 是 1-6，``<value>`` 代表键值的数据。
  - 如果 ``<type>`` 是 7-8，``<value>`` 代表键值的数据的长度。在您发送完此条命令后，AT 返回 ``>``，表示 AT 已准备好接收串行数据，此时您可以输入数据，当 AT 接收到的数据长度达到 ``<value>`` 后，则立即向命名空间内写入键值对。

说明
^^^^
- 请先阅读 `非易失性存储 (NVS) <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-reference/storage/nvs_flash.html>`_，了解命名空间、键值对的概念。
- 写入前，您无需主动擦除命名空间或键值对（NVS 会根据需要自动擦除键值对）。
- 如果您想修改 mfg_nvs 分区中的某些数据，请使用 :ref:`AT+SYSMFG <cmd-SYSMFG>` 命令（NVS 中的键值对操作）。如果您想修改整个 mfg_nvs 分区，请使用 :ref:`AT+SYSFLASH <cmd-SYSFLASH>` 命令（分区操作）。

示例
"""""

::

    // 向 client_cert 命名空间内的 client_cert.0 键写入新的值 (即：更新 client_cert 命名空间内的第 0 套客户端证书)
    AT+SYSMFG=2,"client_cert","client_cert.0",8,1164

    // 等待串口返回 > 后，写入 1164 字节的证书文件

.. _cmd-RFPOWER:

:ref:`AT+RFPOWER <Basic-AT>`：查询/设置 RF TX Power
-----------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 RF TX Power

**命令：**

::

    AT+RFPOWER?

**响应：**

.. only:: esp32 or esp32c3 or esp32c6

  ::

    +RFPOWER:<wifi_power>,<ble_adv_power>,<ble_scan_power>,<ble_conn_power>
    OK

.. only:: esp32c2 or esp32s2

  ::

    +RFPOWER:<wifi_power>
    OK

设置命令
^^^^^^^^

**命令：**

.. only:: esp32 or esp32c3 or esp32c6

  ::

    AT+RFPOWER=<wifi_power>[,<ble_adv_power>,<ble_scan_power>,<ble_conn_power>]

.. only:: esp32c2 or esp32s2

  ::

    AT+RFPOWER=<wifi_power>

**响应：**

::

    OK

参数
^^^^

- **<wifi_power>**：单位为 0.25 dBm，比如设定的参数值为 78，则实际的 RF Power 值为 78 * 0.25 dBm = 19.5 dBm。配置后可运行 ``AT+RFPOWER?`` 命令确认实际的 RF Power 值。

  .. only:: esp32

    - {IDF_TARGET_NAME} 设备的取值范围为 [40,84]：

      ========= ============ ============ ==========
      设定值     读取值        实际值        实际 dBm
      ========= ============ ============ ==========
      [40,43]   34           34           8.5
      [44,51]   44           44           11
      [52,55]   52           52           13
      [56,59]   56           56           14
      [60,65]   60           60           15
      [66,71]   66           66           16.5
      [72,77]   72           72           18
      [78,84]   78           78           19.5
      ========= ============ ============ ==========

  .. only:: esp32c3 or esp32c2 or esp32c6

    - {IDF_TARGET_NAME} 设备的取值范围为 [40,84]：

      ========= ============ ============ ==========
      设定值     读取值        实际值        实际 dBm
      ========= ============ ============ ==========
      [40,80]   <设定值>      <设定值>      <设定值> * 0.25
      [81,84]   <设定值>      80           20
      ========= ============ ============ ==========

  .. only:: esp32s2

    - {IDF_TARGET_NAME} 设备的取值范围为 [40,84]：

      ========= ============ ============ ==========
      设定值     读取值        实际值        实际 dBm
      ========= ============ ============ ==========
      [40,78]   <设定值>      <设定值>      <设定值> * 0.25
      [79,84]   <设定值>      78           19.5
      ========= ============ ============ ==========

.. only:: esp32

  -  **<ble_adv_power>**：Bluetooth LE 广播的 RF TX Power。取值范围为 [0,7]：

    -  0: 7 dBm
    -  1: 4 dBm
    -  2: 1 dBm
    -  3: -2 dBm
    -  4: -5 dBm
    -  5: -8 dBm
    -  6: -11 dBm
    -  7: -14 dBm

.. only:: esp32c3

  -  **<ble_adv_power>**：Bluetooth LE 广播的 RF TX Power。取值范围为 [0,15]：

    -  0: -24 dBm
    -  1: -21 dBm
    -  2: -18 dBm
    -  3: -15 dBm
    -  4: -12 dBm
    -  5: -9 dBm
    -  6: -6 dBm
    -  7: -3 dBm
    -  8: -0 dBm
    -  9: 3 dBm
    -  10: 6 dBm
    -  11: 9 dBm
    -  12: 12 dBm
    -  13: 15 dBm
    -  14: 18 dBm
    -  15: 21 dBm

.. only:: esp32c6

  -  **<ble_adv_power>**：Bluetooth LE 广播的 RF TX Power。取值范围为 [3,15]：

    -  3: -15 dBm
    -  4: -12 dBm
    -  5: -9 dBm
    -  6: -6 dBm
    -  7: -3 dBm
    -  8: -0 dBm
    -  9: 3 dBm
    -  10: 6 dBm
    -  11: 9 dBm
    -  12: 12 dBm
    -  13: 15 dBm
    -  14: 18 dBm
    -  15: 20 dBm

.. only:: esp32 or esp32c3 or esp32c6

  -  **<ble_scan_power>**：Bluetooth LE 扫描的 RF TX Power，参数取值同 ``<ble_adv_power>`` 参数。
  -  **<ble_conn_power>**：Bluetooth LE 连接的 RF TX Power，参数取值同 ``<ble_adv_power>`` 参数。

说明
-----

- 当 Wi-Fi 关闭或未初始化时，``AT+RFPOWER`` 命令无法设置/查询 Wi-Fi 的 RF TX Power。当 Bluetooth LE 未初始化时，``AT+RFPOWER`` 命令无法设置/查询 Bluetooth LE 的 RF TX Power。
- 由于 RF TX Power 分为不同的等级，而每个等级都有与之对应的取值范围，所以通过 ``esp_wifi_get_max_tx_power`` 查询到的 ``wifi_power`` 的值可能与 ``esp_wifi_set_max_tx_power`` 设定的值存在差异，但不会比该值大。
- 建议将 <ble_scan_power> 和 <ble_conn_power> 两个参数值设置为与 <ble_adv_power> 参数相同的值，否则，这两个参数将会被自动设置为与 <ble_adv_power> 相同的值。

.. _cmd-SYSROLLBACK:

:ref:`AT+SYSROLLBACK <Basic-AT>`：回滚到以前的固件
----------------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    AT+SYSROLLBACK

**响应：**

::

    OK

说明
^^^^

.. only:: esp32c2

  - **{IDF_TARGET_CFG_PREFIX}-4MB AT 固件支持此命令，而 {IDF_TARGET_CFG_PREFIX}-2MB AT 固件由于采用了压缩 OTA 分区，因此不支持此命令**。

-  本命令不通过 OTA 升级，只会回滚到另一 OTA 分区的固件。

.. _cmd-SETTIME:

:ref:`AT+SYSTIMESTAMP <Basic-AT>`：查询/设置本地时间戳
------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询本地时间戳

**命令：**

::

    AT+SYSTIMESTAMP?

**响应：**

::

    +SYSTIMESTAMP:<Unix_timestamp>
    OK

设置命令
^^^^^^^^

**功能：**

设置本地时间戳，当 SNTP 时间更新后，将与之同步更新

**命令：**

::

    AT+SYSTIMESTAMP=<Unix_timestamp>

**响应：**

::

    OK

参数
^^^^

-  **<Unix-timestamp>**：Unix 时间戳，单位：秒。

示例
^^^^

::

    AT+SYSTIMESTAMP=1565853509    //2019-08-15 15:18:29

.. _cmd-SYSLOG:

:ref:`AT+SYSLOG <Basic-AT>`：启用或禁用 AT 错误代码提示
----------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 AT 错误代码提示是否启用

**命令：**

::

    AT+SYSLOG?  

**响应：**

::

    +SYSLOG:<status>  

    OK  

设置命令
^^^^^^^^

**功能：**

启用或禁用 AT 错误代码提示

**命令：**

::

    AT+SYSLOG=<status>

**响应：**

::

    OK

参数
^^^^

-  **<status>**：错误代码提示状态

   -  0：禁用
   -  1：启用

示例
^^^^

::

   // 启用 AT 错误代码提示
   AT+SYSLOG=1

   OK
   AT+FAKE
   ERR CODE:0x01090000

   ERROR


::

   // 禁用 AT 错误代码提示
   AT+SYSLOG=0

   OK
   AT+FAKE
   // 不提示 `ERR CODE:0x01090000` 

   ERROR  

AT 错误代码是一个 32 位十六进制数值，定义如下：

.. list-table::
   :header-rows: 1

   * - 类型
     - 子类型
     - 扩展
   * - bit32 ~ bit24
     - bit23 ~ bit16
     - bit15 ~ bit0 

-  **category：** 固定值 0x01
-  **subcategory：** 错误类型

   .. list-table::    
      :header-rows: 1 
       
      * - 错误类型
        - 错误代码
        - 说明
      * - ESP_AT_SUB_OK
        - 0x00
        - OK
      * - ESP_AT_SUB_COMMON_ERROR
        - 0x01
        - 保留  
      * - ESP_AT_SUB_NO_TERMINATOR
        - 0x02
        - 未找到结束符（应以 "\r\n" 结尾）
      * - ESP_AT_SUB_NO_AT
        - 0x03
        - 未找到起始 AT（输入的可能是 at、At 或 aT）
      * - ESP_AT_SUB_PARA_LENGTH_MISMATCH
        - 0x04
        - 参数长度不匹配
      * - ESP_AT_SUB_PARA_TYPE_MISMATCH
        - 0x05
        - 参数类型不匹配
      * - ESP_AT_SUB_PARA_NUM_MISMATCH
        - 0x06
        - 参数数量不匹配
      * - ESP_AT_SUB_PARA_INVALID
        - 0x07
        - 无效参数
      * - ESP_AT_SUB_PARA_PARSE_FAIL
        - 0x08
        - 解析参数失败
      * - ESP_AT_SUB_UNSUPPORT_CMD
        - 0x09
        - 不支持该命令
      * - ESP_AT_SUB_CMD_EXEC_FAIL
        - 0x0A
        - 执行命令失败 
      * - ESP_AT_SUB_CMD_PROCESSING
        - 0x0B
        - 仍在执行上一条命令
      * - ESP_AT_SUB_CMD_OP_ERROR
        - 0x0C
        - 命令操作类型错误

-  **extension：** 错误扩展信息，不同的子类型有不同的扩展信息，详情请见 ``components/at/include/esp_at.h``。

例如，错误代码 ``ERR CODE:0x01090000`` 表示“不支持该命令”。

.. _cmd-WKCFG:

:ref:`AT+SLEEPWKCFG <Basic-AT>`：设置 Light-sleep 唤醒源和唤醒 GPIO
----------------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+SLEEPWKCFG=<wakeup source>,<param1>[,<param2>]

**响应：**

::

    OK

参数
^^^^

-  **<wakeup source>**: 唤醒源

   -  0：保留配置，暂不支持
   -  1：保留配置，暂不支持
   -  2：GPIO 唤醒

-  **<param1>**:

   -  当唤醒源为定时器时，该参数表示睡眠时间，单位：毫秒
   -  当唤醒源为 GPIO 时，该参数表示 GPIO 管脚

-  **<param2>**:

   -  当唤醒源为 GPIO 时，该参数表示唤醒电平

     - 0：低电平
     - 1：高电平

示例
^^^^

::

    // GPIO12 置为低电平时唤醒
    AT+SLEEPWKCFG=2,12,0

.. _cmd-SYSSTORE:

:ref:`AT+SYSSTORE <Basic-AT>`：设置参数存储模式
-------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 AT 参数存储模式 

**命令：**

::

    AT+SYSSTORE?  

**响应：**

::

    +SYSSTORE:<store_mode>  

    OK  

设置命令
^^^^^^^^

**命令：**

::

    AT+SYSSTORE=<store_mode>

**响应：**

::

    OK

参数
^^^^

-  **<store_mode>**：参数存储模式

   -  0：命令配置不存入 flash
   -  1：命令配置存入 flash（默认）

说明
^^^^

- 该命令只影响设置命令，不影响查询命令，因为查询命令总是从 RAM 中调用。
- 本命令会影响以下命令：

.. list::

  - :ref:`AT+SYSMSG <cmd-SYSMSG>`
  - :ref:`AT+CWMODE <cmd-MODE>`
  - :ref:`AT+CIPV6 <cmd-IPV6>`
  - :ref:`AT+CWJAP <cmd-JAP>`
  - :ref:`AT+CWSAP <cmd-SAP>`
  - :ref:`AT+CWRECONNCFG <cmd-RECONNCFG>`
  - :ref:`AT+CIPAP <cmd-IPAP>`
  - :ref:`AT+CIPSTA <cmd-IPSTA>`
  - :ref:`AT+CIPAPMAC <cmd-APMAC>`
  - :ref:`AT+CIPSTAMAC <cmd-STAMAC>`
  - :ref:`AT+CIPDNS <cmd-DNS>`
  - :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
  - :ref:`AT+CIPRECONNINTV <cmd-AUTOCONNINT>`
  - :ref:`AT+CIPTCPOPT <cmd-TCPOPT>`
  - :ref:`AT+CWDHCPS <cmd-DHCPS>`
  - :ref:`AT+CWDHCP <cmd-DHCP>`
  - :ref:`AT+CWSTAPROTO <cmd-STAPROTO>`
  - :ref:`AT+CWAPPROTO <cmd-APPROTO>`
  - :ref:`AT+CWJEAP <cmd-JEAP>`
  :esp32: - :ref:`AT+CIPETH <cmd-ETHIP>`
  :esp32: - :ref:`AT+CIPETHMAC <cmd-ETHMAC>`
  :esp32 or esp32c3: - :ref:`AT+BLENAME <cmd-BNAME>`
  :esp32: - :ref:`AT+BTNAME <cmd-BTNAME>`
  :esp32 or esp32c3: - :ref:`AT+BLEADVPARAM <cmd-BADVP>`
  :esp32 or esp32c3: - :ref:`AT+BLEADVDATA <cmd-BADVD>`
  :esp32 or esp32c3: - :ref:`AT+BLEADVDATAEX <cmd-BADVDEX>`
  :esp32 or esp32c3: - :ref:`AT+BLESCANRSPDATA <cmd-BSCANR>`
  :esp32 or esp32c3: - :ref:`AT+BLESCANPARAM <cmd-BSCANP>`
  :esp32: - :ref:`AT+BTSCANMODE <cmd-BTSCANMODE>`

示例
^^^^

::

   AT+SYSSTORE=0
   AT+CWMODE=1  // 不存入 flash
   AT+CWJAP="test","1234567890" // 不存入 flash

   AT+SYSSTORE=1
   AT+CWMODE=3  // 存入 flash
   AT+CWJAP="test","1234567890" // 存入 flash

.. _cmd-SYSREG:

:ref:`AT+SYSREG <Basic-AT>`：读写寄存器
---------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+SYSREG=<direct>,<address>[,<write value>]

**响应：**

::

    +SYSREG:<read value>  // 仅适用于读寄存器时
    OK

参数
^^^^

-  **<direct>**：读或写寄存器

   -  0：读寄存器
   -  1：写寄存器

-  **<address>**：(uint32) 寄存器地址，详情请参考相关的《技术参考手册》
-  **<write value>**：(uint32) 写入值，仅适用于写寄存器时

说明
^^^^

- AT 不检查寄存器地址，因此请确保操作的寄存器地址有效

.. only:: esp32c3

  .. _cmd-SYSTEMP:

  :ref:`AT+SYSTEMP <Basic-AT>`：读取芯片内部摄氏温度值
  -----------------------------------------------------

  **功能：**

  读取芯片内部温度传感器的数据，转为摄氏温度。

  查询命令
  ^^^^^^^^

  **命令：**

  ::

      AT+SYSTEMP?

  **响应：**

  ::

      +SYSTEMP:<value>
      OK

  参数
  ^^^^

  - **<value>**：摄氏温度值。浮点类型，保留两位小数。
