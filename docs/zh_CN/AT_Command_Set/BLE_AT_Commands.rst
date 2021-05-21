.. _BLE-AT:

[ESP32 Only] Bluetooth® Low Energy AT 命令集
==================================================

:link_to_translation:`en:[English]`

ESP32 AT 固件支持 `蓝牙核心规范 5.0 版本 <https://www.bluetooth.com/specifications/archived-specifications/>`_。

-  [ESP32 Only] :ref:`AT+BLEINIT <cmd-BINIT>`：Bluetooth LE 初始化
-  [ESP32 Only] :ref:`AT+BLEADDR <cmd-BADDR>`：设置 Bluetooth LE 设备地址
-  [ESP32 Only] :ref:`AT+BLENAME <cmd-BNAME>`：查询/设置 Bluetooth LE 设备名称
-  [ESP32 Only] :ref:`AT+BLESCANPARAM <cmd-BSCANP>`：查询/设置 Bluetooth LE 扫描参数
-  [ESP32 Only] :ref:`AT+BLESCAN <cmd-BSCAN>`：使能 Bluetooth LE 扫描
-  [ESP32 Only] :ref:`AT+BLESCANRSPDATA <cmd-BSCANR>`：设置 Bluetooth LE 扫描响应
-  [ESP32 Only] :ref:`AT+BLEADVPARAM <cmd-BADVP>`：查询/设置 Bluetooth LE 广播参数
-  [ESP32 Only] :ref:`AT+BLEADVDATA <cmd-BADVD>`：设置 Bluetooth LE 广播数据
-  [ESP32 Only] :ref:`AT+BLEADVDATAEX <cmd-BADVDEX>`：自动设置 Bluetooth LE 广播数据
-  [ESP32 Only] :ref:`AT+BLEADVSTART <cmd-BADVSTART>`：开始 Bluetooth LE 广播
-  [ESP32 Only] :ref:`AT+BLEADVSTOP <cmd-BADVSTOP>`：停止 Bluetooth LE 广播
-  [ESP32 Only] :ref:`AT+BLECONN <cmd-BCONN>`：建立 Bluetooth LE 连接
-  [ESP32 Only] :ref:`AT+BLECONNPARAM <cmd-BCONNP>`：查询/更新 Bluetooth LE 连接参数
-  [ESP32 Only] :ref:`AT+BLEDISCONN <cmd-BDISC>`：断开 Bluetooth LE 连接
-  [ESP32 Only] :ref:`AT+BLEDATALEN <cmd-BDLEN>`：设置 Bluetooth LE 数据包长度
-  [ESP32 Only] :ref:`AT+BLECFGMTU <cmd-BMTU>`：设置 Bluetooth LE MTU 长度
-  [ESP32 Only] :ref:`AT+BLEGATTSSRVCRE <cmd-GSSRVCRE>`：GATTS 创建服务
-  [ESP32 Only] :ref:`AT+BLEGATTSSRVSTART <cmd-GSSRVSTART>`：GATTS 开启服务
-  [ESP32 Only] :ref:`AT+BLEGATTSSRVSTOP <cmd-GSSRVSTOP>`：GATTS 停止服务
-  [ESP32 Only] :ref:`AT+BLEGATTSSRV <cmd-GSSRV>`：GATTS 发现服务
-  [ESP32 Only] :ref:`AT+BLEGATTSCHAR <cmd-GSCHAR>`：GATTS 发现服务特征
-  [ESP32 Only] :ref:`AT+BLEGATTSNTFY <cmd-GSNTFY>`：服务器 notify 服务特征值给客户端
-  [ESP32 Only] :ref:`AT+BLEGATTSIND <cmd-GSIND>`：服务器 indicate 服务特征值给客户端
-  [ESP32 Only] :ref:`AT+BLEGATTSSETATTR <cmd-GSSETA>`：GATTS 设置服务特征值
-  [ESP32 Only] :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`：GATTC 发现基本服务
-  [ESP32 Only] :ref:`AT+BLEGATTCINCLSRV <cmd-GCINCLSRV>`：GATTC 发现包含的服务
-  [ESP32 Only] :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`：GATTC 发现服务特征
-  [ESP32 Only] :ref:`AT+BLEGATTCRD <cmd-GCRD>`：GATTC 读取服务特征值
-  [ESP32 Only] :ref:`AT+BLEGATTCWR <cmd-GCWR>`：GATTC 写服务特征值
-  [ESP32 Only] :ref:`AT+BLESPPCFG <cmd-BLESPPCFG>`：查询/设置 Bluetooth LE SPP 参数
-  [ESP32 Only] :ref:`AT+BLESPP <cmd-BLESPP>`：进入 Bluetooth LE SPP 模式
-  [ESP32 Only] :ref:`AT+BLESECPARAM <cmd-BLESMPPAR>`：查询/设置 Bluetooth LE 加密参数
-  [ESP32 Only] :ref:`AT+BLEENC <cmd-BLEENC>`：发起 Bluetooth LE 加密请求
-  [ESP32 Only] :ref:`AT+BLEENCRSP <cmd-BLEENCRSP>`：回复对端设备发起的配对请求
-  [ESP32 Only] :ref:`AT+BLEKEYREPLY <cmd-BLEKEYREPLY>`：给对方设备回复密钥（传统连接阶段）
-  [ESP32 Only] :ref:`AT+BLECONFREPLY <cmd-BLECONFREPLY>`：给对方设备回复确认结果（传统连接阶段）
-  [ESP32 Only] :ref:`AT+BLEENCDEV <cmd-BLEENCDEV>`：查询绑定的 Bluetooth LE 加密设备列表
-  [ESP32 Only] :ref:`AT+BLEENCCLEAR <cmd-BLEENCCLEAR>`：清除 Bluetooth LE 加密设备列表
-  [ESP32 Only] :ref:`AT+BLESETKEY <cmd-BLESETKEY>`：设置 Bluetooth LE 静态配对密钥
-  [ESP32 Only] :ref:`AT+BLEHIDINIT <cmd-BLEHIDINIT>`：Bluetooth LE HID 协议初始化
-  [ESP32 Only] :ref:`AT+BLEHIDKB <cmd-BLEHIDKB>`：发送 Bluetooth LE HID 键盘信息
-  [ESP32 Only] :ref:`AT+BLEHIDMUS <cmd-BLEHIDMUS>`：发送 Bluetooth LE HID 鼠标信息
-  [ESP32 Only] :ref:`AT+BLEHIDCONSUMER <cmd-BLEHIDC>`：发送 Bluetooth LE HID consumer 信息
-  [ESP32 Only] :ref:`AT+BLUFI <cmd-BLUFI>`：开启或关闭 BluFi
-  [ESP32 Only] :ref:`AT+BLUFINAME <cmd-BLUFINAME>`：查询/设置 BluFi 设备名称

.. _cmd-BINIT:

[ESP32 Only] :ref:`AT+BLEINIT <BLE-AT>`：Bluetooth LE 初始化
---------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Bluetooth LE 是否初始化

**命令：**

::

    AT+BLEINIT?

**响应：**

若已初始化，AT 返回：

::

    +BLEINIT:<role>
    OK

若未初始化，AT 返回：

::

    +BLEINIT:0
    OK

设置命令
^^^^^^^^

**功能：**

设置 Bluetooth LE 初始化角色

**命令：**

::

    AT+BLEINIT=<init>

**响应：**

::

    OK

参数
^^^^

-  **<init>**:

   -  0: 注销 Bluetooth LE
   -  1: client 角色
   -  2: server 角色

说明
^^^^

-  使用相关命令之前，请先下载 "at_customize.bin" 文件，详情请见 :doc:`../Compile_and_Develop/How_to_customize_BLE_services`。
-  使用其它 Bluetooth LE 命令之前，请先调用本命令，初始化 Bluetooth LE 角色。
-  Bluetooth LE 角色初始化后，不能直接切换。如需切换角色，需要先调用 :ref:`AT+RST <cmd-RST>` 命令重启系统，再重新初始化 Bluetooth LE 角色。
-  若使用 ESP 作为 Bluetooth LE server，需烧录 service bin 到 flash：

   -  对于如何生成 service bin 文件，请参考 esp-at/tools/readme.md；
   -  service bin 文件的烧录地址，请见 esp-at/module_config/module_${platform}_default/at_customize.csv 文件中 "ble_data" 对应的地址。

示例
^^^^

::

    AT+BLEINIT=1

.. _cmd-BADDR:

[ESP32 Only] :ref:`AT+BLEADDR <BLE-AT>`：设置 Bluetooth LE 设备地址
-------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Bluetooth LE 设备的公共地址

**命令：**

::

    AT+BLEADDR?

**响应：**

::

    +BLEADDR:<BLE_public_addr>
    OK

设置命令
^^^^^^^^

**功能：**

设置 Bluetooth LE 设备的地址类型

**命令：**

::

    AT+BLEADDR=<addr_type>[,<random_addr>]

**响应：**

::

    OK

参数
^^^^

-  **<addr_type>**:

   -  0: 公共地址 (Public Address)
   -  1: 随机地址 (Random Address)

说明
^^^^

-  静态地址 (Static Address) 应满足以下条件：

   -  地址最高两位应为 1；
   -  随机地址部分至少有 1 位为 0；
   -  随机地址部分至少有 1 位为 1。

示例
^^^^

::

    AT+BLEADDR=1,"f8:7f:24:87:1c:7b"    // 设置随机设备地址的静态地址
    AT+BLEADDR=1                        // 设置随机设备地址的私有地址
    AT+BLEADDR=0                        // 设置公共设备地址

.. _cmd-BNAME:

[ESP32 Only] :ref:`AT+BLENAME <BLE-AT>`：查询/设置 Bluetooth LE 设备名称
---------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Bluetooth LE 设备名称

**命令：**

::

    AT+BLENAME?

**响应：**

::

    +BLENAME:<device_name>
    OK

设置命令
^^^^^^^^

**功能：**

设置 Bluetooth LE 设备名称

**命令：**

::

    AT+BLENAME=<device_name>

**响应：**

::

    OK

参数
^^^^

-  **<device_name>**：Bluetooth LE 设备名称，最大长度：32，默认名称为 "BLE_AT"。

说明
^^^^

-  若 :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`，配置更改将保存在 NVS 区。

示例
^^^^

::

    AT+BLENAME="esp_demo"

.. _cmd-BSCANP:

[ESP32 Only] :ref:`AT+BLESCANPARAM <BLE-AT>`：查询/设置 Bluetooth LE 扫描参数
---------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Bluetooth LE 扫描参数

**命令：**

::

    AT+BLESCANPARAM?

**响应：**

::

    +BLESCANPARAM:<scan_type>,<own_addr_type>,<filter_policy>,<scan_interval>,<scan_window>
    OK

设置命令
^^^^^^^^

**功能：**

设置 Bluetooth LE 扫描参数

**命令：**

::

    AT+BLESCANPARAM=<scan_type>,<own_addr_type>,<filter_policy>,<scan_interval>,<scan_window>

**响应：**

::

    OK

参数
^^^^

-  **<scan_type>**：扫描类型

   -  0: 被动扫描
   -  1: 主动扫描

-  **<own_addr_type>**：地址类型

   -  0: 公共地址
   -  1: 随机地址
   -  2: RPA 公共地址
   -  3: RPA 随机地址

-  **<filter_policy>**：扫描过滤方式

   -  0: BLE_SCAN_FILTER_ALLOW_ALL
   -  1: BLE_SCAN_FILTER_ALLOW_ONLY_WLST
   -  2: BLE_SCAN_FILTER_ALLOW_UND_RPA_DIR
   -  3: BLE_SCAN_FILTER_ALLOW_WLIST_PRA_DIR

-  **<scan_interval>**：扫描间隔
-  **<scan_window>**：扫描窗口

说明
^^^^

-  ``<scan_window>`` 参数的值不能大于 ``<scan_interval>`` 的值。

示例
^^^^

::

    AT+BLEINIT=1   // 角色：客户端
    AT+BLESCANPARAM=0,0,0,100,50

.. _cmd-BSCAN:

[ESP32 Only] :ref:`AT+BLESCAN <BLE-AT>`：使能 Bluetooth LE 扫描
----------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

开始/停止 Bluetooth LE 扫描

**命令：**

::

    AT+BLESCAN=<enable>[,<interval>][,<filter_type>,<filter_param>]

**响应：**

::

    +BLESCAN:<addr>,<rssi>,<adv_data>,<scan_rsp_data>,<addr_type>
    OK

参数
^^^^

-  **<enable>**：

   -  1: 开始持续扫描
   -  0: 停止持续扫描

-  **[<interval>]**：扫描持续时间，单位：秒。

   -  若设置停止扫描，无需设置本参数；
   -  若设置开始扫描，需设置本参数：

     - 本参数设为 0 时，则表示开始持续扫描；
     - 本参数设为非 0 值时，例如 ``AT+BLESCAN=1,3``，则表示扫描 3 秒后自动结束扫描，然后返回扫描结果。

-  **[<filter_type>]**：过滤选项

   -  1: "MAC"
   -  2: "NAME"

-  **<filter_param>**：过滤参数，表示对方设备 MAC 地址或名称
-  **<addr>**：Bluetooth LE 地址
-  **<rssi>**：信号强度
-  **<adv_data>**：广播数据
-  **<scan_rsp_data>**：扫描响应数据
-  **<addr_type>**：广播设备地址类型

说明
^^^^

-  响应中的 ``OK`` 和 ``+BLESCAN:<addr>,<rssi>,<adv_data>,<scan_rsp_data>,<addr_type>`` 在输出顺序上没有严格意义上的先后顺序。``OK`` 可能在 ``+BLESCAN:<addr>,<rssi>,<adv_data>,<scan_rsp_data>,<addr_type>`` 之前输出，也有可能在 ``+BLESCAN:<addr>,<rssi>,<adv_data>,<scan_rsp_data>,<addr_type>`` 之后输出。 

示例
^^^^

::

    AT+BLEINIT=1    // 角色：客户端
    AT+BLESCAN=1    // 开始扫描
    AT+BLESCAN=0    // 停止扫描
    AT+BLESCAN=1,3,1,"24:0A:C4:96:E6:88"  // 开始扫描，过滤类型为 MAC 地址
    AT+BLESCAN=1,3,2,"ESP-AT"  // 开始扫描，过滤类型为设备名称

.. _cmd-BSCANR:

[ESP32 Only] :ref:`AT+BLESCANRSPDATA <BLE-AT>`：设置 Bluetooth LE 扫描响应
--------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

设置 Bluetooth LE 扫描响应

**命令：**

::

    AT+BLESCANRSPDATA=<scan_rsp_data>

**响应：**

::

    OK  

参数
^^^^

-  **<scan_rsp_data>**：扫描响应数据，为 HEX 字符串。例如，若想设置扫描响应数据为 "0x11 0x22 0x33 0x44 0x55"，则命令为 ``AT+BLESCANRSPDATA="1122334455"``。

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLESCANRSPDATA="1122334455"

.. _cmd-BADVP:

[ESP32 Only] :ref:`AT+BLEADVPARAM <BLE-AT>`：查询/设置 Bluetooth LE 广播参数
----------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询广播参数

**命令：**

::

    AT+BLEADVPARAM?

**响应：**

::

    +BLEADVPARAM:<adv_int_min>,<adv_int_max>,<adv_type>,<own_addr_type>,<channel_map>,<adv_filter_policy>,<peer_addr_type>,<peer_addr>
    OK

设置命令
^^^^^^^^

**功能：**

设置广播参数

**命令：**

::

    AT+BLEADVPARAM=<adv_int_min>,<adv_int_max>,<adv_type>,<own_addr_type>,<channel_map>[,<adv_filter_policy>][,<peer_addr_type>][,<peer_addr>]

**响应：**

::

    OK

参数
^^^^

-  **<adv_int_min>**：最小广播间隔，本参数值应小于 ``<adv_int_max>`` 参数值。参数范围：0x0020 ~ 0x4000。
-  **<adv_int_max>**：最大广播间隔，本参数值应大于 ``<adv_int_min>`` 参数值。参数范围：0x0020 ~ 0x4000。
-  **<adv_type>**:

   -  0: ADV_TYPE_IND
   -  2: ADV_TYPE_SCAN_IND
   -  3: ADV_TYPE_NONCONN_IND

-  **<own_addr_type>**：Bluetooth LE 地址类型

   -  0: BLE_ADDR_TYPE_PUBLIC
   -  1: BLE_ADDR_TYPE_RANDOM

-  **<channel_map>**：广播信道

   -  1: ADV_CHNL_37
   -  2: ADV_CHNL_38
   -  4: ADV_CHNL_39
   -  7: ADV_CHNL_ALL

-  **[<adv_filter_policy>]**：广播过滤器规则

   -  0: ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY
   -  1: ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY
   -  2: ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST
   -  3: ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST

-  **[<peer_addr_type>]**：对方 Bluetooth LE 地址类型

   -  0: PUBLIC
   -  1: RANDOM

-  **[<peer_addr>]**：对方 Bluetooth LE 地址

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLEADVPARAM=50,50,0,0,4,0,0,"12:34:45:78:66:88"

.. _cmd-BADVD:

[ESP32 Only] :ref:`AT+BLEADVDATA <BLE-AT>`：设置 Bluetooth LE 广播数据
-------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

设置广播数据

**命令：**

::

    AT+BLEADVDATA=<adv_data>

**响应：**

::

    OK

参数
^^^^

-  **<adv_data>**：广播数据，为 HEX 字符串。例如，若想设置广播数据为 "0x11 0x22 0x33 0x44 0x55"，则命令为 ``AT+BLEADVDATA="1122334455"``。

说明
^^^^

-  如果之前已经使用命令 :ref:`AT+BLEADVDATAEX <cmd-BADVDEX>`\=<dev_name>,<uuid>,<manufacturer_data>,<include_power> 设置了广播数据，则会被本命令设置的广播数据覆盖。

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLEADVDATA="1122334455"

.. _cmd-BADVDEX:

[ESP32 Only] :ref:`AT+BLEADVDATAEX <BLE-AT>`：自动设置 Bluetooth LE 广播数据
-----------------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询广播数据的参数

**命令：**

::

    AT+BLEADVDATAEX?

**响应：**

::

    +BLEADVDATAEX:<dev_name>,<uuid>,<manufacturer_data>,<include_power>

    OK

设置命令
^^^^^^^^

**功能：**

设置广播数据并开始广播

**命令：**

::

    AT+BLEADVDATAEX=<dev_name>,<uuid>,<manufacturer_data>,<include_power>

**响应：**

::

    OK

参数
^^^^

-  **<dev_name>**：字符串参数，表示设备名称。例如，若想设置设备名称为 "just-test"，则命令为 ``AT+BLEADVSTARTEX="just-test",<uuid>,<manufacturer_data>,<include_power>``。

-  **<uuid>**：字符串参数。例如，若想设置 UUID 为 "0xA002"，则命令为 ``AT+BLEADVSTARTEX=<dev_name>,"A002",<manufacturer_data>,<include_power>``。

-  **<manufacturer_data>**：制造商数据，为 HEX 字符串。例如，若想设置制造商数据为 "0x11 0x22 0x33 0x44 0x55"，则命令为 ``AT+BLEADVSTARTEX=<dev_name>,<uuid>,"1122334455",<include_power>``。

-  **<include_power>**：若广播数据需包含 TX 功率，本参数应该设为 ``1``；否则，为 ``0``。

说明
^^^^

-  如果之前已经使用命令 :ref:`AT+BLEADVDATA <cmd-BADVD>`\=<adv_data> 设置了广播数据，则会被本命令设置的广播数据覆盖。

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLEADVDATAEX="ESP-AT","A002","0102030405",1

.. _cmd-BADVSTART:

[ESP32 Only] :ref:`AT+BLEADVSTART <BLE-AT>`：开始 Bluetooth LE 广播
-----------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

开始广播

**命令：**

::

    AT+BLEADVSTART

**响应：**

::

    OK

说明
^^^^

-  若未使用命令 :ref:`AT+BLEADVPARAM <cmd-BADVP>`\=<adv_parameter> 设置广播参数，则使用默认广播参数。
-  若未使用命令 :ref:`AT+BLEADVDATA <cmd-BADVD>`\=<adv_data> 设置广播数据，则发送全 0 数据包。
-  若之前已经使用命令 :ref:`AT+BLEADVDATA <cmd-BADVD>`\=<adv_data> 设置过广播数据，则会被 :ref:`AT+BLEADVDATAEX <cmd-BADVDEX>`\=<dev_name>,<uuid>,<manufacturer_data>,<include_power> 设置的广播数据覆盖，相反，如果先使用 AT+BLEADVDATAEX，则会被 AT+BLEADVDATA 设置的广播数据覆盖。

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLEADVSTART

.. _cmd-BADVSTOP:

[ESP32 Only] :ref:`AT+BLEADVSTOP <BLE-AT>`：停止 Bluetooth LE 广播
---------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

停止广播

**命令：**

::

    AT+BLEADVSTOP

**响应：**

::

    OK

说明
^^^^

-  若开始广播后，成功建立 Bluetooth LE 连接，则会自动结束 Bluetooth LE 广播，无需调用本命令。

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLEADVSTART
    AT+BLEADVSTOP

.. _cmd-BCONN:

[ESP32 Only] :ref:`AT+BLECONN <BLE-AT>`：建立 Bluetooth LE 连接
----------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Bluetooth LE 连接

**命令：**

::

    AT+BLECONN?

**响应：**

::

    +BLECONN:<conn_index>,<remote_address>
    OK

若未建立连接，则响应不显示 <conn_index> 和 <remote_address> 参数。

设置命令
^^^^^^^^

**功能：**

建立 Bluetooth LE 连接

**命令：**

::

    AT+BLECONN=<conn_index>,<remote_address>[,<addr_type>,<timeout>]

**响应：**

::

    OK

若建立连接成功，则提示：

::

    +BLECONN:<conn_index>,<remote_address>

若失败，则提示：

::

    +BLECONN:<conn_index>,-1

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<remote_address>**：对方 Bluetooth LE 设备地址。
-  **[<addr_type>]**：广播设备地址类型。
-  **[<timeout>]**：连接超时时间，单位：秒。范围：[3,30]。

说明
^^^^

-  建议在建立新连接之前，先运行 :ref:`AT+BLESCAN <cmd-BSCAN>` 命令扫描设备，确保目标设备处于广播状态。
-  最大连接超时为 30 秒。
-  如果 Bluetooth LE server 已初始化且连接已成功建立，则可以使用此命令在对等设备 (GATTC) 中发现服务。还可以使用以下 GATTC 命令：

   -  :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`
   -  :ref:`AT+BLEGATTCINCLSRV <cmd-GCINCLSRV>`
   -  :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`
   -  :ref:`AT+BLEGATTCRD <cmd-GCRD>`
   -  :ref:`AT+BLEGATTCWR <cmd-GCWR>`
   -  :ref:`AT+BLEGATTSIND <cmd-GSIND>`

示例
^^^^

::

    AT+BLEINIT=1   // 角色：客户端
    AT+BLECONN=0,"24:0a:c4:09:34:23",0,10

.. _cmd-BCONNP:

[ESP32 Only] :ref:`AT+BLECONNPARAM <BLE-AT>`：查询/更新 Bluetooth LE 连接参数
-------------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Bluetooth LE 连接参数

**命令：**

::

    AT+BLECONNPARAM?

**响应：**

::

    +BLECONNPARAM:<conn_index>,<min_interval>,<max_interval>,<cur_interval>,<latency>,<timeout>
    OK

设置命令
^^^^^^^^

**功能：**

更新 Bluetooth LE 连接参数

**命令：**

::

    AT+BLECONNPARAM=<conn_index>,<min_interval>,<max_interval>,<latency>,<timeout>

**响应：**

::

    OK

若设置失败，则提示以下信息：

::

    +BLECONNPARAM: <conn_index>,-1

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<min_interval>**：最小连接间隔，范围：0x0006 ~ 0x0C80。
-  **<max_interval>**：最大连接间隔，范围：0x0006 ~ 0x0C80。
-  **<cur_interval>**：当前连接间隔。
-  **<latency>**：延迟，范围：0x0000 ~ 0x01F3。
-  **<timeout>**：超时，范围：0x000A ~ 0x0C80。

说明
^^^^

-  本命令要求先建立连接，并且仅支持 client 角色更新连接参数。

示例
^^^^

::

    AT+BLEINIT=1   // 角色：客户端
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLECONNPARAM=0,12,14,1,500  

.. _cmd-BDISC:

[ESP32 Only] :ref:`AT+BLEDISCONN <BLE-AT>`：断开 Bluetooth LE 连接
-------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

断开 Bluetooth LE 连接

**命令：**

::

    AT+BLEDISCONN=<conn_index>

**响应：**

::

    OK  // 收到 AT+BLEDISCONN 命令
    +BLEDISCONN:<conn_index>,<remote_address>  // 运行命令成功

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<remote_address>**：对方 Bluetooth LE 设备地址。

说明
^^^^

-  仅支持客户端运行本命令断开连接。

示例
^^^^

::

    AT+BLEINIT=1   // 角色：客户端
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLEDISCONN=0

.. _cmd-BDLEN:

[ESP32 Only] :ref:`AT+BLEDATALEN <BLE-AT>`：设置 Bluetooth LE 数据包长度
--------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

设置 Bluetooth LE 数据包长度

**命令：**

::

    AT+BLEDATALEN=<conn_index>,<pkt_data_len>

**响应：**

::

    OK 

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<pkt_data_len>**：数据包长度，范围：0x001b ~ 0x00fb。

说明
^^^^

-  需要先建立 Bluetooth LE 连接，才能设置数据包长度。

示例
^^^^

::

    AT+BLEINIT=1   // 角色：客户端
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLEDATALEN=0,30

.. _cmd-BMTU:

[ESP32 Only] :ref:`AT+BLECFGMTU <BLE-AT>`：设置 Bluetooth LE MTU 长度
-----------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 MTU（maximum transmission unit，最大传输单元）长度

**命令：**

::

    AT+BLECFGMTU?

**响应：**

::

    +BLECFGMTU:<conn_index>,<mtu_size>
    OK

设置命令
^^^^^^^^

**功能：**

设置 MTU 的长度

**命令：**

::

    AT+BLECFGMTU=<conn_index>,<mtu_size>

**响应：**

::

    OK  // 收到本命令

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<mtu_size>**：MTU 长度。

说明
^^^^

-  本命令要求先建立 Bluetooth LE 连接。
-  仅支持客户端运行本命令设置 MTU 的长度。
-  MTU 的实际长度需要协商，响应 ``OK`` 只表示尝试协商 MTU 长度，因此设置长度不一定生效，建议调用 :ref:`AT+BLECFGMTU? <cmd-BMTU>` 查询实际 MTU 长度。

示例
^^^^

::

    AT+BLEINIT=1   // 角色：客户端
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLECFGMTU=0,300

.. _cmd-GSSRVCRE:

[ESP32 Only] :ref:`AT+BLEGATTSSRVCRE <BLE-AT>`：GATTS 创建服务
--------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

GATTS (Generic Attributes Server) 创建 Bluetooth LE 服务

**命令：**

::

    AT+BLEGATTSSRVCRE

**响应：**

::

    OK

说明
^^^^

-  使用 ESP 作为 Bluetooth LE server 创建服务，需烧录 service bin 文件到 flash 中。

   -  如何生成 service bin 文件，请参考 esp-at/tools/readme.md。
   -  service bin 文件的烧录地址为 esp-at/module_config/module_${platform}_default/at_customize.csv 文件中的 "ble_data" 地址。

-  Bluetooth LE server 初始化后，请及时调用本命令创建服务；如果先建立 Bluetooth LE 连接，则无法创建服务。
-  如果 Bluetooth LE client 已初始化成功，可以使用此命令创建服务；也可以使用其他一些相应的 GATTS 命令，例如启动和停止服务、设置服务特征值和 notification/indication，具体命令如下：

   -  :ref:`AT+BLEGATTSSRVCRE <cmd-GSSRVCRE>` (建议在 Bluetooth LE 连接建立之前使用)
   -  :ref:`AT+BLEGATTSSRVSTART <cmd-GSSRVSTART>` (建议在 Bluetooth LE 连接建立之前使用)
   -  :ref:`AT+BLEGATTSSRV <cmd-GSSRV>`
   -  :ref:`AT+BLEGATTSCHAR <cmd-GSCHAR>`
   -  :ref:`AT+BLEGATTSNTFY <cmd-GSNTFY>`
   -  :ref:`AT+BLEGATTSIND <cmd-GSIND>`
   -  :ref:`AT+BLEGATTSSETATTR <cmd-GSSETA>`

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLEGATTSSRVCRE

.. _cmd-GSSRVSTART:

[ESP32 Only] :ref:`AT+BLEGATTSSRVSTART <BLE-AT>`：GATTS 开启服务
---------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

GATTS 开启全部服务

**命令：**

::

    AT+BLEGATTSSRVSTART

设置命令
^^^^^^^^

**功能：**

GATTS 开启某指定服务

**命令：**

::

    AT+BLEGATTSSRVSTART=<srv_index>

**响应：**

::

    OK  

参数
^^^^

-  **<srv_index>**：服务序号，从 1 开始递增。

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART

.. _cmd-GSSRVSTOP:

[ESP32 Only] :ref:`AT+BLEGATTSSRVSTOP <BLE-AT>`：GATTS 停止服务
-------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

GATTS 停止全部服务

**命令：**

::

    AT+BLEGATTSSRVSTOP

设置命令
^^^^^^^^

**功能：**

GATTS 停止某指定服务

**命令：**

::

    AT+BLEGATTSSRVSTOP=<srv_index>

**响应：**

::

    OK  

参数
^^^^

-  **<srv_index>**：服务序号，从 1 开始递增。

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEGATTSSRVSTOP

.. _cmd-GSSRV:

[ESP32 Only] :ref:`AT+BLEGATTSSRV <BLE-AT>`：GATTS 发现服务
-------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

GATTS 发现服务

**命令：**

::

    AT+BLEGATTSSRV?

**响应：**

::

    +BLEGATTSSRV:<srv_index>,<start>,<srv_uuid>,<srv_type>
    OK

参数
^^^^

-  **<srv_index>**：服务序号，从 1 开始递增。
-  **<start>**：

   -  0: 服务未开始；
   -  1: 服务已开始。

-  **<srv_uuid>**：服务的 UUID。
-  **<srv_type>**：服务的类型：

   -  0: 次要服务；
   -  1: 首要服务。

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRV?

.. _cmd-GSCHAR:

[ESP32 Only] :ref:`AT+BLEGATTSCHAR <BLE-AT>`：GATTS 发现服务特征
---------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

GATTS 发现服务特征

**命令：**

::

    AT+BLEGATTSCHAR?

**响应：**

对于服务特征信息，响应如下：

::

    +BLEGATTSCHAR:"char",<srv_index>,<char_index>,<char_uuid>,<char_prop>

对于描述符信息，响应如下：

::

    +BLEGATTSCHAR:"desc",<srv_index>,<char_index>,<desc_index> 
    OK

参数
^^^^

-  **<srv_index>**：服务序号，从 1 开始递增。
-  **<char_index>**：服务特征的序号，从 1 起始递增。
-  **<char_uuid>**：服务特征的 UUID。
-  **<char_prop>**：服务特征的属性。
-  **<desc_index>**：特征描述符序号。
-  **<desc_uuid>**：特征描述符的 UUID。

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEGATTSCHAR?

.. _cmd-GSNTFY:

[ESP32 Only] :ref:`AT+BLEGATTSNTFY <BLE-AT>`：服务器 notify 服务特征值给客户端
---------------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

服务器 notify 服务特征值给客户端

**命令：**

::

    AT+BLEGATTSNTFY=<conn_index>,<srv_index>,<char_index>,<length>

**响应：**

::

    >

符号 ``>`` 表示 AT 准备好接收串口数据，此时您可以输入数据，当数据长度达到参数 ``<length>`` 的值时，执行 notify 操作。

若数据传输成功，则提示：

::

   OK

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<srv_index>**：服务序号，可运行 :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` 查询。
-  **<char_index>**：服务特征的序号，可运行 :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` 查询。
-  **<length>**：数据长度。

示例
^^^^

::

    AT+BLEINIT=2      // 角色：服务器
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEADVSTART    // 开始广播，当 client 连接后，必须配置接收 notify
    AT+BLEGATTSCHAR?  // 查询允许 notify 客户端的特征
    // 例如，使用 3 号服务的 6 号特征 notify 长度为 4 字节的数据，使用如下命令：
    AT+BLEGATTSNTFY=0,3,6,4 
    // 提示 ">" 符号后，输入 4 字节的数据，如 "1234"，然后数据自动传输

.. _cmd-GSIND:

[ESP32 Only] :ref:`AT+BLEGATTSIND <BLE-AT>`：服务器 indicate 服务特征值给客户端
------------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**
 
服务器 indicate 服务特征值给客户端

**命令：**

::

    AT+BLEGATTSIND=<conn_index>,<srv_index>,<char_index>,<length>

**响应：**

::

    >

符号 ``>`` 表示 AT 准备好接收串口数据，此时您可以输入数据，当数据长度达到参数 ``<length>`` 的值时，执行 indicate 操作。

若数据传输成功，则提示：

::

   OK

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<srv_index>**：服务序号，可运行 :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` 查询。
-  **<char_index>**：服务特征的序号，可运行 :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` 查询。
-  **<length>**：数据长度。

示例
^^^^

::

    AT+BLEINIT=2      // 角色：服务器
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEADVSTART    // 开始广播，当 client 连接后，必须配置接收 indication
    AT+BLEGATTSCHAR?  // 查询客户端可以接收 indication 的特征
    // 例如，使用 3 号服务的 7 号特征 indicate 长度为 4 字节的数据，命令如下：
    AT+BLEGATTSIND=0,3,7,4 
    // 提示 ">" 符号后，输入 4 字节的数据，如 "1234"，然后数据自动传输

.. _cmd-GSSETA:

[ESP32 Only] :ref:`AT+BLEGATTSSETATTR <BLE-AT>`：GATTS 设置服务特征值
------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

GATTS 设置服务特征值或描述符值

**命令：**

::

    AT+BLEGATTSSETATTR=<srv_index>,<char_index>,[<desc_index>],<length>

**响应：**

::

    >

符号 ``>`` 表示 AT 准备好接收串口数据，此时您可以输入数据，当数据长度达到参数 ``<length>`` 的值时，执行设置操作。

若数据传输成功，则提示：

::

   OK

参数
^^^^

-  **<srv_index>**：服务序号，可运行 :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` 查询。
-  **<char_index>**：服务特征的序号，可运行 :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` 查询。
-  **[<desc_index>]**：特征描述符序号：

   -  若填写，则设置描述符的值；
   -  若未填写，则设置特征值。

-  **<length>**：数据长度。

说明
^^^^

-  如果 ``<length>`` 参数值大于支持的最大长度，则设置会失败。关于 service table，请见 `components/customized_partitions/raw_data/ble_data`。

示例
^^^^

::

    AT+BLEINIT=2   // 角色：服务器
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEGATTSCHAR? 
    // 例如，向 1 号服务的 1 号特征写入长度为 1 字节的数据，命令如下：
    AT+BLEGATTSSETATTR=1,1,,1
    // 提示 ">" 符号后，输入 1 字节的数据即可，例如 "8"，然后设置开始

.. _cmd-GCPRIMSRV:

[ESP32 Only] :ref:`AT+BLEGATTCPRIMSRV <BLE-AT>`：GATTC 发现基本服务
-------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

GATTC (Generic Attributes Client) 发现基本服务

**命令：**

::

    AT+BLEGATTCPRIMSRV=<conn_index>

**响应：**

::

    +BLEGATTCPRIMSRV:<conn_index>,<srv_index>,<srv_uuid>,<srv_type>
    OK

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<srv_index>**：服务序号，从 1 开始递增。
-  **<srv_uuid>**：服务的 UUID。
-  **<srv_type>**：服务的类型：

   -  0: 次要服务；
   -  1: 首要服务。

说明
^^^^

-  使用本命令，需要先建立 Bluetooth LE 连接。

示例
^^^^

::

    AT+BLEINIT=1   // 角色：客户端
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0

.. _cmd-GCINCLSRV:

[ESP32 Only] :ref:`AT+BLEGATTCINCLSRV <BLE-AT>`：GATTC 发现包含的服务
--------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

GATTC 发现包含服务

**命令：**

::

    AT+BLEGATTCINCLSRV=<conn_index>,<srv_index>

**响应：**

::

    +BLEGATTCINCLSRV:<conn_index>,<srv_index>,<srv_uuid>,<srv_type>,<included_srv_uuid>,<included_srv_type>
    OK

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<srv_index>**：服务序号，可运行 :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> 查询。
-  **<srv_uuid>**：服务的 UUID。
-  **<srv_type>**：服务的类型：

   -  0: 次要服务；
   -  1: 首要服务。

-  **<included_srv_uuid>**：包含服务的 UUID。
-  **<included_srv_type>**：包含服务的类型：

   -  0: 次要服务；
   -  1: 首要服务。

说明
^^^^

-  使用本命令，需要先建立 Bluetooth LE 连接。

示例
^^^^

::

    AT+BLEINIT=1   // 角色：客户端
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCINCLSRV=0,1  // 根据前一条命令的查询结果，指定 index 查询

.. _cmd-GCCHAR:

[ESP32 Only] :ref:`AT+BLEGATTCCHAR <BLE-AT>`：GATTC 发现服务特征
---------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

GATTC 发现服务特征

**命令：**

::

    AT+BLEGATTCCHAR=<conn_index>,<srv_index>

**响应：**

对于服务特征信息，响应如下：

::

    +BLEGATTCCHAR:"char",<conn_index>,<srv_index>,<char_index>,<char_uuid>,<char_prop>

对于描述符信息，响应如下：

::

    +BLEGATTCCHAR:"desc",<conn_index>,<srv_index>,<char_index>,<desc_index>,<desc_uuid> 
    OK

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<srv_index>**：服务序号，可运行 :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> 查询。
-  **<char_index>**：服务特征的序号，从 1 开始递增。
-  **<char_uuid>**：服务特征的 UUID。
-  **<char_prop>**：服务特征的属性。
-  **<desc_index>**：特征描述符序号。
-  **<desc_uuid>**：特征描述符的 UUID。

说明
^^^^

-  使用本命令，需要先建立 Bluetooth LE 连接。

示例
^^^^

::

    AT+BLEINIT=1   // 角色：客户端
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCCHAR=0,1 // 根据前一条命令的查询结果，指定 index 查询

.. _cmd-GCRD:

[ESP32 Only] :ref:`AT+BLEGATTCRD <BLE-AT>`：GATTC 读取服务特征值
----------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

GATTC 读取服务特征值或描述符值

**命令：**

::

    AT+BLEGATTCRD=<conn_index>,<srv_index>,<char_index>[,<desc_index>]

**响应：**

::

    +BLEGATTCRD:<conn_index>,<len>,<value>
    OK

参数
^^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<srv_index>**：服务序号，可运行 :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> 查询。
-  **<char_index>**：服务特征序号，可运行 :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> 查询。
-  **[<desc_index>]**：特征描述符序号：

   -  若设置，读取目标描述符的值；
   -  若未设置，读取目标特征的值。

-  **<len>**：数据长度。
-  **<char_value>**：服务特征值，HEX 字符串，运行 :ref:`AT+BLEGATTCRD <cmd-GCRD>`\=<conn_index>,<srv_index>,<char_index> 读取。例如，若响应为 ``+BLEGATTCRD:1,30``，则表示数据长度为 1，内容为 "0x30"。
-  **[<desc_value>]**：服务特征描述符的值，HEX 字符串，运行 :ref:`AT+BLEGATTCRD <cmd-GCRD>`\=<conn_index>,<srv_index>,<char_index>,<desc_index> 读取。例如，若响应为 ``+BLEGATTCRD:4,30313233``，则表示数据长度为 4，内容为 "0x30 0x31 0x32 0x33"。

说明
^^^^

-  使用本命令，需要先建立 Bluetooth LE 连接。
-  若目标服务特征不支持读操作，则返回 "ERROR"。

示例
^^^^

::

    AT+BLEINIT=1   // 角色：客户端
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCCHAR=0,3 // 根据前一条命令的查询结果，指定 index 查询
    // 例如，读取第 3 号服务的第 2 号特征的第 1 号描述符信息，命令如下：
    AT+BLEGATTCRD=0,3,2,1

.. _cmd-GCWR:

[ESP32 Only] :ref:`AT+BLEGATTCWR <BLE-AT>`：GATTC 写服务特征值
---------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

GATTC 写服务特征值或描述符值

**命令：**

::

    AT+BLEGATTCWR=<conn_index>,<srv_index>,<char_index>[,<desc_index>],<length>

**Response:**

::

    >

符号 ``>`` 表示 AT 准备好接收串口数据，此时您可以输入数据，当数据长度达到参数 ``<length>`` 的值时，执行写入操作。

若数据传输成功，则提示：

::

   OK

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<srv_index>**：服务序号，可运行 :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> 查询。
-  **<char_index>**：服务特征序号，可运行 :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> 查询。
-  **[<desc_index>]**：特征描述符序号：

   -  若设置，则写目标描述符的值；
   -  若未设置，则写目标特征的值。

-  **<length>**：数据长度。

说明
^^^^

-  使用本命令，需要先建立 Bluetooth LE 连接。
-  若目标服务特征不支持写操作，则返回 "ERROR"。

示例
^^^^

::

    AT+BLEINIT=1   // 角色：客户端
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCCHAR=0,3 // 根据前一条命令的查询结果，指定 index 查询
    // 例如，向第 3 号服务的第 4 号特征，写入长度为 6 字节的数据，命令如下：
    AT+BLEGATTCWR=0,3,4,,6 
    // 提示 ">" 符号后，输入 6 字节的数据即可，如 "123456"，然后开始写入

.. _cmd-BLESPPCFG:

[ESP32 Only] :ref:`AT+BLESPPCFG <BLE-AT>`：查询/设置 Bluetooth LE SPP 参数
--------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Bluetooth LE SPP (Serial Port Profile) 参数

**命令：**

::

    AT+BLESPPCFG?

**响应：**

::

    +BLESPPCFG:<tx_service_index>,<tx_char_index>,<rx_service_index>,<rx_char_index>
    OK

设置命令
^^^^^^^^

**功能：**

设置或重置 Bluetooth LE SPP 参数

**命令：**

::

    AT+BLESPPCFG=<cfg_enable>[,<tx_service_index>,<tx_char_index>,<rx_service_index>,<rx_char_index>]

**响应：**

::

    OK

参数
^^^^

-  **<cfg_enable>**：

   -  0: 重置所有 SPP 参数，后面四个参数无需填写；
   -  1: 后面四个参数需要填写。

-  **<tx_service_index>**：tx 服务序号，可运行 :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> 和 :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` 查询。
-  **<tx_char_index>**：tx 服务特征序号，可运行 :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> 和 :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` 查询。
-  **<rx_service_index>**：rx 服务序号，可运行 :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> 和 :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` 查询。
-  **<rx_char_index>**：rx 服务特征序号，可运行 :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> 和 :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` 查询。

说明
^^^^

-  对于 Bluetooth LE 客户端，tx 服务特征属性必须是 ``write with response`` 或 ``write without response``，rx 服务特征属性必须是 ``indicate`` 或 ``notify``。
-  对于 Bluetooth LE 服务器，tx 服务特征属性必须是 ``indicate`` 或 ``notify``，rx 服务特征属性必须是 ``write with response`` 或 ``write without response``。

示例
^^^^

::

    AT+BLESPPCFG=0          // 重置 Bluetooth LE SPP 参数
    AT+BLESPPCFG=1,3,5,3,7  // 设置 Bluetooth LE SPP 参数
    AT+BLESPPCFG?           // 查询 Bluetooth LE SPP 参数

.. _cmd-BLESPP:

[ESP32 Only] :ref:`AT+BLESPP <BLE-AT>`：进入 Bluetooth LE SPP 模式
------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

进入 Bluetooth LE SPP 模式

**命令：**

::

    AT+BLESPP

**响应：**

::

    >

说明
^^^^

-  若 Bluetooth LE SPP 参数非法，则命令返回 ``ERROR``。
-  在 SPP 传输中，若未设置 :ref:`AT+SYSMSG <cmd-SYSMSG>` 为 1，则 AT 不会提示任何连接状态变更信息。

示例
^^^^

::

    AT+BLESPP   // 进入 Bluetooth LE SPP 模式

.. _cmd-BLESMPPAR:

[ESP32 Only] :ref:`AT+BLESECPARAM <BLE-AT>`：查询/设置 Bluetooth LE 加密参数
-------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Bluetooth LE SMP 加密参数

**命令：**

::

    AT+BLESECPARAM?

**响应：**

::

    +BLESECPARAM:<auth_req>,<iocap>,<key_size>,<init_key>,<rsp_key>,<auth_option>
    OK

设置命令
^^^^^^^^

**功能：**

设置 Bluetooth LE SMP 加密参数

**命令：**

::

    AT+BLESECPARAM=<auth_req>,<iocap>,<key_size>,<init_key>,<rsp_key>[,<auth_option>]

**响应：**

::

    OK

参数
^^^^

-  **<auth_req>**：认证请求。

   -  0: NO_BOND
   -  1: BOND
   -  4: MITM
   -  8: SC_ONLY
   -  9: SC_BOND
   -  12: SC_MITM
   -  13: SC_MITM_BOND

-  **<iocap>**：输入输出能力。

   -  0: DisplayOnly
   -  1: DisplayYesNo
   -  2: KeyboardOnly
   -  3: NoInputNoOutput
   -  4: Keyboard display

-  **<key_size>**：密钥长度，取值范围 7 ~ 16 字节。
-  **<init_key>**：多个比特位组成的初始密钥。
-  **<rsp_key>**：多个比特位组成的响应密钥。
-  **<auth_option>**：安全认证选项：

   -  0: 自动选择安全等级；
   -  1: 如果无法满足之前设定的安全等级，则会断开连接。

说明
^^^^

-  ``<init_key>`` 和 ``<rsp_key>`` 参数的比特位组合模式如下：

   -  Bit0: 用于交换初始密钥和响应密钥的加密密钥；
   -  Bit1: 用于交换初始密钥和响应密钥的 IRK 密钥；
   -  Bit2: 用于交换初始密钥和响应密钥的 CSRK 密钥；
   -  Bit3: 用于交换初始密钥和响应密钥的 link 密钥（仅用于 Bluetooth LE 和 BR/EDR 共存模式）。

示例
^^^^

::

    AT+BLESECPARAM=1,4,16,3,3,0

.. _cmd-BLEENC:

[ESP32 Only] :ref:`AT+BLEENC <BLE-AT>`：发起 Bluetooth LE 加密请求
----------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

发起配对请求

**命令：**

::

    AT+BLEENC=<conn_index>,<sec_act>

**响应：**

::

    OK

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<sec_act>**：

   -  0: SEC_NONE；
   -  1: SEC_ENCRYPT；
   -  2: SEC_ENCRYPT_NO_MITM；
   -  3: SEC_ENCRYPT_MITM。

说明
^^^^

-  使用本命令前，请先设置安全参数、建立与对方设备的连接。

示例
^^^^

::

    AT+BLESECPARAM=1,4,16,3,3
    AT+BLEENC=0,3

.. _cmd-BLEENCRSP:

[ESP32 Only] :ref:`AT+BLEENCRSP <BLE-AT>`：回复对端设备发起的配对请求
-----------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

回复对端设备发起的配对请求

**命令：**

::

    AT+BLEENCRSP=<conn_index>,<accept>

**响应：**

::

    OK

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<accept>**：

   -  0: 拒绝；
   -  1: 接受。

说明
^^^^

-  使用本命令后，AT 会在配对请求流程结束后输出配对结果。

::

    +BLEAUTHCMPL:<conn_index>,<enc_result>

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<enc_result>**:

   - 0: 加密配对成功；
   - 1: 加密配对失败。

示例
^^^^

::

    AT+BLEENCRSP=0,1

.. _cmd-BLEKEYREPLY:

[ESP32 Only] :ref:`AT+BLEKEYREPLY <BLE-AT>`：给对方设备回复密钥（传统连接阶段）
------------------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

回复配对密钥

**命令：**

::

    AT+BLEKEYREPLY=<conn_index>,<key>

**响应：**

::

    OK

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<key>**：配对密钥。

示例
^^^^

::

    AT+BLEKEYREPLY=0,649784

.. _cmd-BLECONFREPLY:

[ESP32 Only] :ref:`AT+BLECONFREPLY <BLE-AT>`：给对方设备回复确认结果（传统连接阶段）
-----------------------------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

回复配对结果

**命令：**

::

    AT+BLECONFREPLY=<conn_index>,<confirm>

**响应：**

::

    OK

参数
^^^^

-  **<conn_index>**：Bluetooth LE 连接号，范围：[0,2]。
-  **<confirm>**：

   -  0: 否
   -  1: 是

示例
^^^^

::

    AT+BLECONFREPLY=0,1

.. _cmd-BLEENCDEV:

[ESP32 Only] :ref:`AT+BLEENCDEV <BLE-AT>`：查询绑定的 Bluetooth LE 加密设备列表
---------------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询绑定的 Bluetooth LE 加密设备列表

**命令：**

::

    AT+BLEENCDEV?

**响应：**

::

    +BLEENCDEV:<enc_dev_index>,<mac_address>
    OK

参数
^^^^

-  **<enc_dev_index>**：已绑定设备的连接号。
-  **<mac_address>**：MAC 地址。

示例
^^^^

::

    AT+BLEENCDEV?

.. _cmd-BLEENCCLEAR:

[ESP32 Only] :ref:`AT+BLEENCCLEAR <BLE-AT>`：清除 Bluetooth LE 加密设备列表
----------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

从安全数据库列表中删除某一连接号的设备

**命令：**

::

    AT+BLEENCCLEAR=<enc_dev_index>

**响应：**

::

    OK

执行命令
^^^^^^^^

**功能：**

删除安全数据库所有设备

**命令：**

::

    AT+BLEENCCLEAR

**响应：**

::

    OK

参数
^^^^

-  **<enc_dev_index>**：已绑定设备的连接号。

示例
^^^^

::

    AT+BLEENCCLEAR

.. _cmd-BLESETKEY:

[ESP32 Only] :ref:`AT+BLESETKEY <BLE-AT>`：设置 Bluetooth LE 静态配对密钥
-------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Bluetooth LE 静态配对密钥，若未设置，则 AT 返回 -1

**命令：**

::

    AT+BLESETKEY?

**响应：**

::

    +BLESETKEY:<static_key>
    OK

设置命令
^^^^^^^^

**功能：**

为所有 Bluetooth LE 连接设置一个 Bluetooth LE 静态配对密钥

**命令：**

::

    AT+BLESETKEY=<static_key>

**响应：**

::

    OK

参数
^^^^

-  **<static_key>**：Bluetooth LE 静态配对密钥。

示例
^^^^

::

    AT+BLESETKEY=123456

.. _cmd-BLEHIDINIT:

[ESP32 Only] :ref:`AT+BLEHIDINIT <BLE-AT>`：Bluetooth LE HID 协议初始化
------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Bluetooth LE HID 协议初始化情况

**命令：**

::

    AT+BLEHIDINIT?

**响应：**

若未初始化，则 AT 返回：

::

    +BLEHIDINIT:0
    OK

若已初始化，则 AT 返回：

::

    +BLEHIDINIT:1
    OK

设置命令
^^^^^^^^

**功能：**

初始化 Bluetooth LE HID 协议

**命令：**

::

    AT+BLEHIDINIT=<init>

**响应：**

::

    OK

参数
^^^^

-  **<init>**：

   -  0: 取消 Bluetooth LE HID 协议的初始化；
   -  1: 初始化 Bluetooth LE HID 协议。

说明
^^^^

-  Bluetooth LE HID 无法与通用 GATT/GAP 命令同时使用。

示例
^^^^

::

    AT+BLEHIDINIT=1 

.. _cmd-BLEHIDKB:

[ESP32 Only] :ref:`AT+BLEHIDKB <BLE-AT>`：发送 Bluetooth LE HID 键盘信息
--------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

发送键盘信息

**命令：**

::

    AT+BLEHIDKB=<Modifier_keys>,<key_1>,<key_2>,<key_3>,<key_4>,<key_5>,<key_6>

**响应：**

::

    OK

参数
^^^^

-  **<Modifier_keys>**：组合键。
-  **<key_1>**：键代码 1。
-  **<key_2>**：键代码 2。
-  **<key_3>**：键代码 3。
-  **<key_4>**：键代码 4。
-  **<key_5>**：键代码 5。
-  **<key_6>**：键代码 6。

说明
^^^^

- 更多键代码的信息，请参考 `Universal Serial Bus HID Usage Tables <https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf>`_ 的 Keyboard/Keypad Page 章节。

示例
^^^^

::

    AT+BLEHIDKB=0,4,0,0,0,0,0   // 输入字符串 "a"

.. _cmd-BLEHIDMUS:

[ESP32 Only] :ref:`AT+BLEHIDMUS <BLE-AT>`：发送 Bluetooth LE HID 鼠标信息
-----------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

发送鼠标信息

**命令：**

::

    AT+BLEHIDMUS=<buttons>,<X_displacement>,<Y_displacement>,<wheel>

**响应：**

::

    OK

参数
^^^^

-  **<buttons>**：鼠标按键。
-  **<X_displacement>**：X 位移。
-  **<Y_displacement>**：Y 位移。
-  **<wheel>**：滚轮。

示例
^^^^

::

    AT+BLEHIDMUS=0,10,10,0

.. _cmd-BLEHIDC:

[ESP32 Only] :ref:`AT+BLEHIDCONSUMER <BLE-AT>`：发送 Bluetooth LE HID consumer 信息
--------------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

发送 consumer 信息

**命令：**

::

    AT+BLEHIDCONSUMER=<consumer_usage_id>

**响应：**

::

    OK

参数
^^^^

-  **<consumer_usage_id>**：consumer ID，如 power、reset、help、volume 等。详情请参考 `HID Usage Tables for Universal Serial Bus (USB) <https://usb.org/sites/default/files/hut1_21_0.pdf>`_ 中的 Consumer Page (0x0C) 章节。

示例
^^^^

::

    AT+BLEHIDCONSUMER=233   // 调高音量

.. _cmd-BLUFI:

[ESP32 Only] :ref:`AT+BLUFI <BLE-AT>`：开启或关闭 BluFi
--------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 BluFi 状态

**命令：**

::

    AT+BLUFI?

**响应：**

若 BluFi 未开启，则返回：

::

    +BLUFI:0
    OK

若 BluFi 已开启，则返回：

::

    +BLUFI:1
    OK

设置命令
^^^^^^^^

**功能：**

开启或关闭 BluFi

**命令：**

::

    AT+BLUFI=<option>[,<auth floor>]

**响应：**

::

    OK

参数
^^^^

-  **<option>**：

   -  0: 关闭 BluFi；
   -  1: 开启 BluFi。

-  **<auth floor>**：Wi-Fi 认证模式阈值，ESP-AT 不会连接到认证模式低于此阈值的 AP：

   -  0: OPEN（默认）；
   -  1: WEP；
   -  2: WPA_PSK；
   -  3: WPA2_PSK；
   -  4: WPA_WPA2_PSK；
   -  5: WPA2_ENTERPRISE；
   -  6: WPA3_PSK；
   -  7: WPA2_WPA3_PSK。

示例
^^^^

::

    AT+BLUFI=1

.. _cmd-BLUFINAME:

[ESP32 Only] :ref:`AT+BLUFINAME <BLE-AT>`：查询/设置 BluFi 设备名称
-------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 BluFi 名称

**命令：**

::

    AT+BLUFINAME?

**响应：**

::

    +BLUFINAME:<device_name>
    OK

设置命令
^^^^^^^^

**功能：**

设置 BluFi 设备名称

**命令：**

::

    AT+BLUFINAME=<device_name>

**响应：**

::

    OK

参数
^^^^

-  **<device_name>**：BluFi 设备名称。

说明
^^^^

-  如需设置 BluFi 设备名称，请在运行 :ref:`AT+BLUFI=1 <cmd-BLUFI>` 命令前设置，否则将使用默认名称 ``BLUFI_DEVICE``。
-  BluFi 设备名称最大长度为 29 字节。

示例
^^^^

::

    AT+BLUFINAME="BLUFI_DEV"
    AT+BLUFINAME?