.. _BT-AT:

{IDF_TARGET_NAME} Classic Bluetooth® AT 命令集
===================================================

:link_to_translation:`en:[English]`

- :ref:`介绍 <cmd-bt-intro>`
- :ref:`AT+BTINIT <cmd-BTINIT>`：Classic Bluetooth 初始化
- :ref:`AT+BTNAME <cmd-BTNAME>`：查询/设置 Classic Bluetooth 设备名称
- :ref:`AT+BTSCANMODE <cmd-BTSCANMODE>`：设置 Classic Bluetooth 扫描模式
- :ref:`AT+BTSTARTDISC <cmd-BTDISC>`：开始发现周边 Classic Bluetooth 设备
- :ref:`AT+BTSPPINIT <cmd-BTSPPINIT>`：Classic Bluetooth SPP 协议初始化
- :ref:`AT+BTSPPCONN <cmd-BTSPPCONN>`：查询/建立 SPP 连接
- :ref:`AT+BTSPPDISCONN <cmd-BTSPPDISCONN>`：断开 SPP 连接
- :ref:`AT+BTSPPSTART <cmd-BTSPPSTART>`：开启 Classic Bluetooth SPP 协议
- :ref:`AT+BTSPPSEND <cmd-BTSPPSEND>`：发送数据到对方 Classic Bluetooth SPP 设备
- :ref:`AT+BTA2DPINIT <cmd-BTA2DPINIT>`：Classic Bluetooth A2DP 协议初始化
- :ref:`AT+BTA2DPCONN <cmd-BTA2DPCONN>`：查询/建立 A2DP 连接
- :ref:`AT+BTA2DPDISCONN <cmd-BTA2DPDISCONN>`：断开 A2DP 连接
- :ref:`AT+BTA2DPSRC <cmd-BTA2DPSRC>`：查询/设置音频文件 URL
- :ref:`AT+BTA2DPCTRL <cmd-BTA2DPCTRL>`：控制音频播放
- :ref:`AT+BTSECPARAM <cmd-BTSECPARAM>`：查询/设置 Classic Bluetooth 安全参数
- :ref:`AT+BTKEYREPLY <cmd-BTKEYREPLY>`：输入简单配对密钥 (Simple Pair Key)
- :ref:`AT+BTPINREPLY <cmd-BTPINREPLY>`：输入传统配对密码 (Legacy Pair PIN Code)
- :ref:`AT+BTSECCFM <cmd-BTSECCFM>`：给对方设备回复确认结果（传统连接阶段）
- :ref:`AT+BTENCDEV <cmd-BTENCDEV>`：查询 Classic Bluetooth 加密设备列表
- :ref:`AT+BTENCCLEAR <cmd-BTENCCLEAR>`：清除 Classic Bluetooth 加密设备列表
- :ref:`AT+BTCOD <cmd-BTCOD>`：设置设备类型
- :ref:`AT+BTPOWER <cmd-BTPWR>`：查询/设置 Classic Bluetooth 的 TX 功率

.. _cmd-bt-intro:

介绍
------

{IDF_TARGET_NAME} AT 固件支持 `蓝牙核心规范 4.2 版本 <https://www.bluetooth.com/specifications/specs/>`_。

.. important::
  默认的 AT 固件不支持此页面下的 AT 命令。如果您需要 {IDF_TARGET_NAME} 支持经典蓝牙命令，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择（下面每项是独立的，根据您的需要选择）：

  - 启用通用蓝牙命令：``Component config`` -> ``AT`` -> ``AT bt command support``
  - 启用 SPP 命令： ``Component config`` -> ``AT`` -> ``AT bt command support`` -> ``AT bt spp command support``
  - 启用 A2DP 命令：``Component config`` -> ``AT`` -> ``AT bt command support`` -> ``AT bt a2dp command support``

.. _cmd-BTINIT:

:ref:`AT+BTINIT <BT-AT>`：Classic Bluetooth 初始化
---------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Classic Bluetooth 初始化状态

**命令：**

::

    AT+BTINIT?

**响应：**

若已初始化，则返回：

::

    +BTINIT:1
    OK

若未初始化，则返回：

::

    +BTINIT:0
    OK


设置命令
^^^^^^^^

**功能：**

初始化或注销 Classic Bluetooth

**命令：**

::

    AT+BTINIT=<init>

**响应：**

::

    OK

参数
^^^^

-  **<init>**：

   -  0: 注销 Classic Bluetooth；
   -  1: 初始化 Classic Bluetooth。

说明
^^^^

- 为获得更好的性能，建议在使用 Classic Bluetooth 功能前，先发送 :ref:`AT+CWMODE=0/1 <cmd-MODE>` 命令禁用 SoftAP。如您想了解更多细节，请阅读 `RF 共存 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/coexist.html>`_ 文档。
- 如果 Classic Bluetooth 已初始化，则 :ref:`AT+CIPMODE <cmd-IPMODE>` 无法设置为 1。

示例
^^^^

::

    AT+BTINIT=1

.. _cmd-BTNAME:

:ref:`AT+BTNAME <BT-AT>`：查询/设置 Classic Bluetooth 设备名称
-------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Classic Bluetooth 设备名称

**命令：**

::

    AT+BTNAME?

**响应：**

::

    +BTNAME:<device_name>
    OK

设置命令
^^^^^^^^

**功能：**

设置 Classic Bluetooth 设备名称

**命令：**

::

    AT+BTNAME=<device_name>

**响应：**

::

    OK

参数
^^^^

-  **<device_name>**：Classic Bluetooth 设备名称，最大长度为：32。默认："ESP32_AT"。

说明
^^^^

-  若 :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`，配置更改将保存在 NVS 区。
-  默认 Classic Bluetooth 设备名称为 "{IDF_TARGET_NAME}_AT"。

示例
^^^^

::

    AT+BTNAME="esp_demo"

.. _cmd-BTSCANMODE:

:ref:`AT+BTSCANMODE <BT-AT>`：设置 Classic Bluetooth 扫描模式
---------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

设置 Classic Bluetooth 扫描模式

**命令：**

::

    AT+BTSCANMODE=<scan_mode>

**响应：**

::

    OK

参数
^^^^

-  **<scan_mode>**：

   -  0: 不可发现且不可连接；
   -  1: 可连接但不可发现；
   -  2: 既可发现也可连接；
   -  3: 可发现但不可连接。

示例
^^^^

::

    AT+BTSCANMODE=2   // 既可发现也可连接

.. _cmd-BTDISC:

:ref:`AT+BTSTARTDISC <BT-AT>`：开始发现周边 Classic Bluetooth 设备
-------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

开始发现 Classic Bluetooth 设备

**命令：**

::

    AT+BTSTARTDISC=<inq_mode>,<inq_len>,<inq_num_rsps>

**响应：**

::

    +BTSTARTDISC:<bt_addr>,<dev_name>,<major_dev_class>,<minor_dev_class>,<major_srv_class>,<rssi>

    OK

参数
^^^^

-  **<inq_mode>**：

   -  0: general inquiry mode；
   -  1: limited inquiry mode。

-  **<inq_len>**：inquiry 时长，范围：0x01 ~ 0x30。
-  **<inq_num_rsps>**：可以收到的 inquiry responses 的数量，若设为 0，AT 将收到无限个 response。
-  **<bt_addr>**：Classic Bluetooth 地址。
-  **<dev_name>**：设备名称。
-  **<major_dev_class>**：主要设备类型：

   -  0x0: 其他；
   -  0x1: 计算机；
   -  0x2: 电话（手机、无绳、支付电话、调制解调器）；
   -  0x3: LAN、网络接入点；
   -  0x4: 音频/视频（耳机、扬声器、立体声、视频显示、VCR）；
   -  0x5: 配件（鼠标、游戏杆、键盘）；
   -  0x6: 成像（打印、扫描仪、相机、显示）；
   -  0x7: 可穿戴；
   -  0x8: 玩具；
   -  0x9: 健康；
   -  0x1F: 未分类。

-  **<minor_dev_class>**：请参考 `次要设备类型 (Minor Device Class field) <https://www.bluetooth.com/specifications/assigned-numbers/>`_。

-  **<major_srv_class>**：主要服务类型：

   -  0x0: 无效值；
   -  0x1: 有限可发现模式 (Limited Discoverable Mode)；
   -  0x8: 定位（位置标志）；
   -  0x10: 网络，如 LAN、点对点；
   -  0x20: 渲染，如打印、扬声器；
   -  0x40: 捕捉，如扫描仪、麦克风；
   -  0x80: 对象传输，如 v-Inbox、v-Folder；
   -  0x100: 音频，如扬声器、麦克风、耳机服务；
   -  0x200: 电话，如无绳电话、调制解调器、耳机服务；
   -  0x400: 信息，如 WEB 服务器、WAP 服务器。

-  **<rssi>**：信号强度。

示例
^^^^

::

    AT+BTINIT=1
    AT+BTSCANMODE=2
    AT+BTSTARTDISC=0,10,10

.. _cmd-BTSPPINIT:

:ref:`AT+BTSPPINIT <BT-AT>`：Classic Bluetooth SPP 协议初始化
------------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Classic Bluetooth SPP 协议初始化状态

**命令：**

::

    AT+BTSPPINIT?

**响应：**

若已初始化，则返回：

::

    +BTSPPINIT:1
    OK

若未初始化，则返回：

::

    +BTSPPINIT:0
    OK


设置命令
^^^^^^^^

**功能：**

初始化或注销 Classic Bluetooth SPP 协议

**命令：**

::

    AT+BTSPPINIT=<init>

**响应：**

::

    OK

参数
^^^^

-  **<init>**：

   -  0: 注销 Classic Bluetooth SPP 协议；
   -  1: 初始化 Classic Bluetooth SPP 协议，角色为 master；
   -  2: 初始化 Classic Bluetooth SPP 协议，角色为 slave。

示例
^^^^

::

    AT+BTSPPINIT=1    // master
    AT+BTSPPINIT=2    // slave

.. _cmd-BTSPPCONN:

:ref:`AT+BTSPPCONN <BT-AT>`：查询/建立 SPP 连接
-------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Classic Bluetooth SPP 连接

**命令：**

::

    AT+BTSPPCONN?

**响应：**

::

    +BTSPPCONN:<conn_index>,<remote_address>
    OK

如果未建立连接，则返回：

::

   +BTSPPCONN:-1

设置命令
^^^^^^^^

**功能：**

建立 Classic Bluetooth SPP 连接

**命令：**

::

    AT+BTSPPCONN=<conn_index>,<sec_mode>,<remote_address>

**响应：**

::

    OK

若建立连接成功，则 AT 返回：

::

    +BTSPPCONN:<conn_index>,<remote_address>

若建立连接失败，则 AT 返回：

::

    +BTSPPCONN:<conn_index>,-1

参数
^^^^

-  **<conn_index>**：Classic Bluetooth SPP 连接号，当前只支持单连接，连接号为 0。
-  **<sec_mode>**：

   -  0x0000: 无安全保障；
   -  0x0001: 需要授权（仅对外连接需要）；
   -  0x0036: 需要加密；
   -  0x3000: 中间人保护；
   -  0x4000: 最少 16 位密码。

-  **<remote_address>**：对方 Classic Bluetooth SPP 设备地址。

示例
^^^^

::

    AT+BTSPPCONN=0,0,"24:0a:c4:09:34:23"

.. _cmd-BTSPPDISCONN:

:ref:`AT+BTSPPDISCONN <BT-AT>`：断开 SPP 连接
--------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

断开 Classic Bluetooth SPP 连接

**命令：**

::

    AT+BTSPPDISCONN=<conn_index>

**响应：**

::

    OK

若命令运行成功，则返回：

::

    +BTSPPDISCONN:<conn_index>,<remote_address>

若命令运行失败，则返回：

::

    +BTSPPDISCONN:-1

参数
^^^^

-  **<conn_index>**：Classic Bluetooth SPP 连接号，当前只支持单连接，连接号为 0。
-  **<remote_address>**：对方 Classic Bluetooth A2DP 设备地址。

示例
^^^^

::

    AT+BTSPPDISCONN=0

.. _cmd-BTSPPSEND:

:ref:`AT+BTSPPSEND <BT-AT>`：发送数据到对方 Classic Bluetooth SPP 设备
-----------------------------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

进入 Classic Bluetooth SPP 模式

**命令：**

::

    AT+BTSPPSEND

**响应：**

::

    >   

设置命令
^^^^^^^^^^^^

**功能：**

发送数据到对方 Classic Bluetooth SPP 设备

**命令：**

::

    AT+BTSPPSEND=<conn_index>,<data_len>

**响应：**

::

    OK

参数
^^^^

-  **<conn_index>**：Classic Bluetooth SPP 连接号，当前只支持单连接，连接号为 0。
-  **<data_len>**：发送数据的长度。

说明
^^^^

-  系统收到此命令后先换行返回 ``>``，然后 {IDF_TARGET_NAME} 设备进入 UART Bluetooth 透传模式，当系统收到只含有 :ref:`+++ <cmd-PLUS>` 的包时，设备返回到普通命令模式，请等待一秒再发送下一个 AT 命令。

示例
^^^^

::

    AT+BTSPPSEND=0,100
    AT+BTSPPSEND

.. _cmd-BTSPPSTART:

:ref:`AT+BTSPPSTART <BT-AT>`：开启 Classic Bluetooth SPP 协议
---------------------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

开启 Classic Bluetooth SPP 协议

**命令：**

::

    AT+BTSPPSTART

**响应：**

::

    OK

说明
^^^^

-  在 SPP 传输中，如果未设置 :ref:`AT+SYSMSG <cmd-SYSMSG>` 命令的 bit2 为 1，则系统不会提示任何连接状态改变的信息。

示例
^^^^

::

    AT+BTSPPSTART

.. _cmd-BTA2DPINIT:

:ref:`AT+BTA2DPINIT <BT-AT>`：Classic Bluetooth A2DP 协议初始化
--------------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Classic Bluetooth A2DP 协议的初始化状态

**命令：**

::

    AT+BTA2DPINIT?

**响应：**

若已初始化，则返回：

::

    +BTA2DPINIT:<role>

    OK

若未初始化，则返回：

::

    +BTA2DPINIT:0

    OK

设置命令
^^^^^^^^

**功能：**

初始化或注销 Classic Bluetooth A2DP 协议

**命令：**

::

    AT+BTA2DPINIT=<role>

**响应：**

::

    OK

参数
^^^^

-  **<role>**：角色

   -  0: 注销 Classic Bluetooth A2DP 协议；
   -  1: source；
   -  2: sink。

示例
^^^^

::

    AT+BTA2DPINIT=2

.. _cmd-BTA2DPCONN:

:ref:`AT+BTA2DPCONN <BT-AT>`：查询/建立 A2DP 连接
---------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Classic Bluetooth A2DP 连接

**命令：**

::

    AT+BTA2DPCONN?

**响应：**

::

    +BTA2DPCONN:<conn_index>,<remote_address>
    OK

若未建立连接，则 AT 不会返回 ``<conn_index>`` 和 ``<remote_address>`` 参数。

设置命令
^^^^^^^^

**功能：**

建立 Classic Bluetooth A2DP 连接

**命令：**

::

    AT+BTA2DPCONN=<conn_index>,<remote_address>

**响应：**

::

    OK

若建立连接成功，则返回：

::

    +BTA2DPCONN:<conn_index>,<remote_address>

若建立连接失败，则返回：

::

    +BTA2DPCONN:<conn_index>,-1

参数
^^^^

-  **<conn_index>**：Classic Bluetooth A2DP 连接号，当前只支持单连接，连接号为 0。
-  **<remote_address>**：对方 Classic Bluetooth A2DP 设备地址。

示例
^^^^

::

    AT+BTA2DPCONN=0,0,0,"24:0a:c4:09:34:23"

.. _cmd-BTA2DPDISCONN:

:ref:`AT+BTA2DPDISCONN <BT-AT>`：断开 A2DP 连接
----------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

断开 Classic Bluetooth A2DP 连接

**命令：**

::

    AT+BTA2DPDISCONN=<conn_index>

**响应：**

::

   +BTA2DPDISCONN:<conn_index>,<remote_address>
   OK

参数
^^^^

-  **<conn_index>**：Classic Bluetooth A2DP 连接号，当前只支持单连接，连接号为 0。
-  **<remote_address>**：对方 Classic Bluetooth A2DP 设备地址。

示例
^^^^

::

    AT+BTA2DPDISCONN=0

.. _cmd-BTA2DPSRC:

:ref:`AT+BTA2DPSRC <BT-AT>`：查询/设置音频文件 URL
-----------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询音频文件 URL

**命令：**

::

    AT+BTA2DPSRC?

**响应：**

::

    +BTA2DPSRC:<url>,<type>
    OK

执行命令
^^^^^^^^

**功能：**

设置音频文件 URL

**命令：**

::

    AT+BTA2DPSRC=<conn_index>,<url>

**响应：**

::

    OK

参数
^^^^

-  **<conn_index>**：Classic Bluetooth A2DP 连接号，当前只支持单连接，连接号为 0。
-  **<url>**：源文件路径，当前只支持 HTTP、HTTPS 和 FLASH。
-  **<type>**：音频文件类型，如 "mp3"。

说明
^^^^

-  当前只支持 mp3 格式文件。

示例
^^^^

::

    AT+BTA2DPSRC=0,"https://dl.espressif.com/dl/audio/ff-16b-2c-44100hz.mp3"
    AT+BTA2DPSRC=0,"flash://spiffs/zhifubao.mp3"

.. _cmd-BTA2DPCTRL:

:ref:`AT+BTA2DPCTRL <BT-AT>`：控制音频播放
---------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

控制音频播放

**命令：**

::

    AT+BTA2DPCTRL=<conn_index>,<ctrl>

**响应：**

::

    OK

参数
^^^^

-  **<conn_index>**：Classic Bluetooth A2DP 连接号，当前只支持单连接，连接号为 0。
-  **<ctrl>**：控制类型：

   -  0: A2DP Sink，停止播放；
   -  1: A2DP Sink，开始播放；
   -  2: A2DP Sink，快进；
   -  3: A2DP Sink，后退；
   -  4: A2DP Sink，快进启动；
   -  5: A2DP Sink，快进停止；
   -  0: A2DP Source，停止播放；
   -  1: A2DP Source，开始播放；
   -  2: A2DP Source，暂停播放。

示例
^^^^

::

    AT+BTA2DPCTRL=0,1  // 开始播放音频

.. _cmd-BTSECPARAM:

:ref:`AT+BTSECPARAM <BT-AT>`：查询/设置 Classic Bluetooth 安全参数
------------------------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Classic Bluetooth 安全参数

**命令：**

::

    AT+BTSECPARAM?

**响应：**

::

    +BTSECPARAM:<io_cap>,<pin_type>,<pin_code>
    OK

设置命令
^^^^^^^^

**功能：**

设置 Classic Bluetooth 安全参数

**命令：**

::

    AT+BTSECPARAM=<io_cap>,<pin_type>,<pin_code>

**响应：**

::

    OK

参数
^^^^

-  **<io_cap>**：输入输出能力：

   -  0: DisplayOnly；
   -  1: DisplayYesNo；
   -  2: KeyboardOnly；
   -  3: NoInputNoOutput。

-  **<pin_type>**：使用可变或固定密码：

   -  0: 可变密码；
   -  1: 固定密码。

-  **<pin_code>**：传统配对密码，最大长度：16 字节。

说明
^^^^

-  若设置 ``<pin_type>`` 为 0，则会自动忽略 ``<pin_code>`` 参数。

示例
^^^^

::

    AT+BTSECPARAM=3,1,"9527"

.. _cmd-BTKEYREPLY:

:ref:`AT+BTKEYREPLY <BT-AT>`：输入简单配对密钥 (Simple Pair Key)
--------------------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

输入简单配对密钥 (Simple Pair Key)

**命令：**

::

    AT+BTKEYREPLY=<conn_index>,<Key>

**响应：**

::

    OK

参数
^^^^

-  **<conn_index>**：Classic Bluetooth 连接号，当前只支持单连接，连接号为 0。
-  **<Key>**：简单配对密钥 (Simple Pair Key)。

示例
^^^^

::

    AT+BTKEYREPLY=0,123456

.. _cmd-BTPINREPLY:

:ref:`AT+BTPINREPLY <BT-AT>`：输入传统配对密码 (Legacy Pair PIN Code)
-----------------------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

输入传统配对密码 (Legacy Pair PIN Code)

**命令：**

::

    AT+BTPINREPLY=<conn_index>,<Pin>

**响应：**

::

    OK

参数
^^^^

-  **<conn_index>**：Classic Bluetooth 连接号，当前只支持单连接，连接号为 0。
-  **<Pin>**：传统配对密码 (Legacy Pair PIN Code)。

示例
^^^^

::

    AT+BTPINREPLY=0,"6688"

.. _cmd-BTSECCFM:

:ref:`AT+BTSECCFM <BT-AT>`：给对方设备回复确认结果（传统连接阶段）
--------------------------------------------------------------------------------------------------

执行命令
^^^^^^^^

**功能：**

给对方设备回复确认结果（传统连接阶段）

**命令：**

::

    AT+BTSECCFM=<conn_index>,<accept>

**响应：**

::

    OK

参数
^^^^

-  **<conn_index>**：Classic Bluetooth 连接，当前只支持单连接，连接号为 0。
-  **<accept>**：拒绝或接受：

   -  0: 拒绝；
   -  1: 接受。

示例
^^^^

::

    AT+BTSECCFM=0,1

.. _cmd-BTENCDEV:

:ref:`AT+BTENCDEV <BT-AT>`：查询 Classic Bluetooth 加密设备列表
----------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询绑定设备

**命令：**

::

    AT+BTENCDEV?

**响应：**

::

    +BTENCDEV:<enc_dev_index>,<mac_address>
    OK

参数
^^^^

-  **<enc_dev_index>**：绑定设备序号。
-  **<mac_address>**：MAC 地址。

示例
^^^^

::

    AT+BTENCDEV?

.. _cmd-BTENCCLEAR:

:ref:`AT+BTENCCLEAR <BT-AT>`：清除 Classic Bluetooth 加密设备列表
------------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

从安全数据库列表中删除某一序号的设备

**命令：**

::

    AT+BTENCCLEAR=<enc_dev_index>

**响应：**

::

    OK

执行命令
^^^^^^^^

**功能：**

删除安全数据库所有设备

**命令：**

::

    AT+BTENCCLEAR

**响应：**

::

    OK

参数
^^^^

-  **<enc_dev_index>**：绑定设备序号。

示例
^^^^

::

    AT+BTENCCLEAR

.. _cmd-BTCOD:

:ref:`AT+BTCOD <BT-AT>`：设置设备类型
-------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

设置 Classic Bluetooth 设备类型

**命令：**

::

    AT+BTCOD=<major>,<minor>,<service>

**响应：**

::

    OK

参数
^^^^

-  **<major>**：`主要设备类型 (major class) <https://btprodspecificationrefs.blob.core.windows.net/assigned-numbers/Assigned%20Number%20Types/Assigned_Numbers.pdf>`_；
-  **<minor>**：`次要设备类型 (minor class) <https://btprodspecificationrefs.blob.core.windows.net/assigned-numbers/Assigned%20Number%20Types/Assigned_Numbers.pdf>`_；
-  **<service>**：`服务类型 (service class) <https://btprodspecificationrefs.blob.core.windows.net/assigned-numbers/Assigned%20Number%20Types/Assigned_Numbers.pdf>`_。

示例
^^^^

::

    AT+BTCOD=6,32,32   // 打印机

.. _cmd-BTPWR:

:ref:`AT+BTPOWER <BT-AT>`：查询/设置 Classic Bluetooth 的 TX 功率
-------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Classic Bluetooth 的 TX 功率

**命令：**

::

    AT+BTPOWER?

**响应：**

::

    +BTPOWER:<min_tx_power>,<max_tx_power>
    OK


设置命令
^^^^^^^^

**功能：**

设置 Classic Bluetooth 的 TX 功率

**命令：**

::

    AT+BTPOWER=<min_tx_power>,<max_tx_power>

**响应：**

::

    OK

参数
^^^^

-  **<min_tx_power>**：最小功率水平，范围：[0,7]。
-  **<max_tx_power>**：最大功率水平，范围：[0,7]。

示例
^^^^

::

    AT+BTPOWER=5,6   // 设置 Classic Bluetooth tx 功率
