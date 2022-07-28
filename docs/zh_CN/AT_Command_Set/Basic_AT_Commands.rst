.. _Basic-AT:

基础 AT 命令
=================

:link_to_translation:`en:[English]`

.. list::

  - :ref:`AT <cmd-AT>`：测试 AT 启动
  - :ref:`AT+RST <cmd-RST>`：重启模块
  - :ref:`AT+GMR <cmd-GMR>`：查看版本信息
  - :ref:`AT+CMD <cmd-cmd>`：查询当前固件支持的所有命令及命令类型
  - :ref:`AT+GSLP <cmd-GSLP>`：进⼊ Deep-sleep 模式
  - :ref:`ATE <cmd-ATE>`：开启或关闭 AT 回显功能
  - :ref:`AT+RESTORE <cmd-RESTORE>`：恢复出厂设置
  - :ref:`AT+UART_CUR <cmd-UARTC>`：设置 UART 当前临时配置，不保存到 flash
  - :ref:`AT+UART_DEF <cmd-UARTD>`：设置 UART 默认配置, 保存到 flash
  - :ref:`AT+SLEEP <cmd-SLEEP>`：设置 sleep 模式
  - :ref:`AT+SYSRAM <cmd-SYSRAM>`：查询当前剩余堆空间和最小堆空间
  - :ref:`AT+SYSMSG <cmd-SYSMSG>`：查询/设置系统提示信息
  - :ref:`AT+SYSFLASH <cmd-SYSFLASH>`：查询或读写 flash 用户分区
  - :ref:`AT+FS <cmd-FS>`：文件系统操作
  - :ref:`AT+RFPOWER <cmd-RFPOWER>`：查询/设置 RF TX Power
  - :ref:`AT+SYSROLLBACK <cmd-SYSROLLBACK>`：回滚到以前的固件
  - :ref:`AT+SYSTIMESTAMP <cmd-SETTIME>`：查询/设置本地时间戳
  - :ref:`AT+SYSLOG <cmd-SYSLOG>`：启用或禁用 AT 错误代码提示
  - :ref:`AT+SLEEPWKCFG <cmd-WKCFG>`：设置 Light-sleep 唤醒源和唤醒 GPIO
  - :ref:`AT+SYSSTORE <cmd-SYSSTORE>`：设置参数存储模式
  - :ref:`AT+SYSREG <cmd-SYSREG>`：读写寄存器
  :esp32c3: - :ref:`AT+SYSTEMP <cmd-SYSTEMP>`：读取芯片内部摄氏温度值

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
-  **<Bin version>**: AT 固件版本信息。版本信息可以在 menuconfig 中修改。

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
   
   - 0 表示立即重启 
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

     - 单 Wi-Fi 模式

       - CPU 将自动进入睡眠，射频模块也将根据 :ref:`AT+CWJAP <cmd-JAP>` 命令设置的 ``listen interval`` 参数定期关闭

     - 单 BLE 模式

       - 在 BLE 广播态下，CPU 将自动进入睡眠，射频模块也将根据广播间隔定期关闭
       - 在 BLE 连接态下，CPU 将自动进入睡眠，射频模块也将根据连接间隔定期关闭

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
-  **<minimum heap size>**：最小堆空间，单位：byte

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

设置系统提示信息

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
-  烧录二级用户分区前，请参考 :doc:`../Compile_and_Develop/how_to_generate_pki_files` 生成二进制用户分区文件。
-  擦除分区时，设置指令可省略 ``<offset>`` 和 ``<length>`` 参数，用于完整擦除该目标分区。例如，指令 ``AT+SYSFLASH=0,"ble_data"`` 可擦除整个 "ble_data" 区域。如果擦除分区时不省略 ``<offset>`` 和 ``<length>`` 参数，则这两个参数值要求是 4 KB 的整数倍。
-  关于分区的定义可参考 `ESP-IDF 分区表 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/partition-tables.html>`_。
-  当 ``<operator>`` 为 ``write`` 时，系统收到此命令后先换行返回 ``>``，此时您可以输入要写的数据，数据长度应与 ``<length>`` 一致。
-  写分区前，请先擦除该分区。
-  写 `PKI bin <https://github.com/espressif/esp-at/blob/master/tools/README.md#2-pki-bin>`_ 时，参数 ``<length>`` 应为 4 字节的整数倍。

示例
^^^^

::

    // 从 "ble_data" 分区偏移地址 0 处读取 100 字节
    AT+SYSFLASH=2,"ble_data",0,100

    // 在 "ble_data" 分区偏移地址 100 处写入 10 字节
    AT+SYSFLASH=1,"ble_data",100,10

    // 从 "ble_data" 分区偏移地址 4096 处擦除 8192 字节
    AT+SYSFLASH=0,"ble_data",4096,8192

.. _cmd-FS:

:ref:`AT+FS <Basic-AT>`：文件系统操作
---------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+FS=<type>,<operation>,<filename>,<offset>,<length>

**响应：**

::

    OK  

参数
^^^^

-  **<type>**：目前仅支持 FATFS

   -  0：FATFS

-  **<operation>**:

   -  0：删除文件
   -  1：写文件
   -  2：读文件
   -  3：查询文件大小
   -  4：查询路径下文件，目前仅支持根目录

-  **<offset>**：偏移地址，仅针对读写操作设置
-  **<length>**：长度，仅针对读写操作设置

说明
^^^^

-  使用本命令需烧录 at_customize.bin，详细信息可参考 `ESP-IDF 分区表 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/partition-tables.html>`_ 和 :doc:`../Compile_and_Develop/How_to_customize_partitions`。
-  若读取数据的长度大于实际文件大小，仅返回实际长度的数据。
-  当 ``<operator>`` 为 ``write`` 时，系统收到此命令后先换行返回 ``>``，此时您可以输入要写的数据，数据长度应与 ``<length>`` 一致。

示例
^^^^

::

    // 删除某个文件
    AT+FS=0,0,"filename"

    // 在某个文件偏移地址 100 处写入 10 字节
    AT+FS=0,1,"filename",100,10

    // 从某个文件偏移地址 0 处读取 100 字节
    AT+FS=0,2,"filename",0,100

    // 列出根目录下所有文件
    AT+FS=0,4,"."

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

::

    +RFPOWER:<wifi_power>,<ble_adv_power>,<ble_scan_power>,<ble_conn_power>
    OK

设置命令
^^^^^^^^

**命令：**

::

    AT+RFPOWER=<wifi_power>[,<ble_adv_power>,<ble_scan_power>,<ble_conn_power>]

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

  .. only:: esp32c3 or esp32c2

    - {IDF_TARGET_NAME} 设备的取值范围为 [40,84]：

      ========= ============ ============ ==========
      设定值     读取值        实际值        实际 dBm
      ========= ============ ============ ==========
      [40,80]   <设定值>      <设定值>      <设定值> * 0.25
      [81,84]   <设定值>      80           20
      ========= ============ ============ ==========

-  **<ble_adv_power>**：Bluetooth LE 广播的 RF TX Power。

  .. only:: esp32

    -  0: 7 dBm
    -  1: 4 dBm
    -  2: 1 dBm
    -  3: -2 dBm
    -  4: -5 dBm
    -  5: -8 dBm
    -  6: -11 dBm
    -  7: -14 dBm

  .. only:: esp32c3 or esp32c2

    -  0: -27 dBm
    -  1: -24 dBm
    -  2: -21 dBm
    -  3: -18 dBm
    -  4: -15 dBm
    -  5: -12 dBm
    -  6: -9 dBm
    -  7: -6 dBm
    -  8: -3 dBm
    -  9: -0 dBm
    -  10: 3 dBm
    -  11: 6 dBm
    -  12: 9 dBm
    -  13: 12 dBm
    -  14: 15 dBm
    -  15: 18 dBm

-  **<ble_scan_power>**：Bluetooth LE 扫描的 RF TX Power，参数取值同 ``<ble_adv_power>`` 参数。
-  **<ble_conn_power>**：Bluetooth LE 连接的 RF TX Power，参数取值同 ``<ble_adv_power>`` 参数。

说明
-----

- 由于 RF TX Power 分为不同的等级，而每个等级都有与之对应的取值范围，所以通过 ``esp_wifi_get_max_tx_power`` 查询到的 ``wifi_power`` 的值可能与 ``esp_wifi_set_max_tx_power`` 设定的值存在差异，但不会比该值大。

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

   -  0：定时器唤醒
   -  1：保留配置
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

    // 定时器唤醒
    AT+SLEEPWKCFG=0,1000

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
