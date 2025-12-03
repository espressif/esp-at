{IDF_TARGET_NAME} Classic Bluetooth AT 示例
==========================================

:link_to_translation:`en:[English]`

本文档主要提供在 {IDF_TARGET_NAME} 设备上运行 :doc:`../AT_Command_Set/BT_AT_Commands` 命令的详细示例。

.. contents::
   :local:
   :depth: 1

.. Important::
  
  固件默认不支持 Classic Bluetooth 命令。有关如何使能对 Classic Bluetooth 的支持，请参考文档 :ref:`Classic Bluetooth AT 命令的介绍 <cmd-bt-intro>`。

以普通传输模式在移动电话或者 PC 和 {IDF_TARGET_NAME} 之间建立 SPP 连接并且设置 IO 能力为 NoInputNoOutput
------------------------------------------------------------------------------------------------------------------------------------------

在本例中，移动电话或 PC 为主机，{IDF_TARGET_NAME} 为从机。该示例展示了如何建立 SPP 连接。

#. Classic Bluetooth 初始化。
   
   命令：

   .. code-block:: none

     AT+BTINIT=1

   响应：
  
   .. code-block:: none

     OK

#. Classic Bluetooth SPP 协议初始化并且设置角色为 slave。
   
   命令：

   .. code-block:: none

     AT+BTSPPINIT=2

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 设备名称。
   
   命令：

   .. code-block:: none

     AT+BTNAME="EXAMPLE"

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 扫描模式为可发现可连接。
   
   命令：

   .. code-block:: none

     AT+BTSCANMODE=2

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 安全参数。设置 ``io_cap`` 为 ``NoInputNoOutput``，``pin_type`` 为 ``fixed``，``pin_code`` to ``9527``。
   
   命令：

   .. code-block:: none

     AT+BTSECPARAM=3,1,"9527"

   响应：
  
   .. code-block:: none

     OK

#. 开启 Classic Bluetooth SPP 协议。
   
   命令：

   .. code-block:: none

     AT+BTSPPSTART

   响应：
  
   .. code-block:: none

     OK

#. 移动电话或者 PC 发起连接。

   移动电话或 PC 应能找到名为 “EXAMPLE” 的蓝牙设备。如果移动电话或 PC 发起连接并成功建立连接，{IDF_TARGET_NAME} 将提示：

   .. code-block:: none

     +BTSPPCONN:0,"e0:24:81:47:90:bc"

   说明：

   - 您获取到的地址可能与上述响应中的不同。

#. 发送 4 字节数据。

   命令：

   .. code-block:: none

     AT+BTSPPSEND=0,4

   响应：

   .. code-block:: none

     >

   符号 ``>`` 表示 AT 准备好接收串口数据，此时您可以输入数据，当数据长度达到参数 ``<data_len>`` 的值时，执行写入操作。

   输入 4 字节数据，例如输入数据是 ``test``，之后 AT 将会输出以下信息。

   .. code-block:: none

     OK

   说明：

   - 若输入的字节数目超过 ``AT+BTSPPSEND`` 命令设定的长度 (n)，则系统会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``OK``。
   - ``AT`` 输出 ``>`` 字符后，数据中的特殊字符不需要转义字符进行转义，也不需要以新行结尾 (CR-LF)。

#. 接收 4 字节数据。

   假设移动电话或者 PC 发送 4 字节的数据（数据为 ``test``），则系统会提示：

   .. code-block:: none

     +BTDATA:4,test

#. 断开 Classic Bluetooth SPP 连接。

   命令：

   .. code-block:: none

     AT+BTSPPDISCONN=0

   响应：

   .. code-block:: none

     +BTSPPDISCONN:0,"e0:24:81:47:90:bc"

     OK

   说明：

   - 您获取到的地址可能与上述响应中的不同。

以透传模式在移动电话或者 PC 和 {IDF_TARGET_NAME} 之间建立 SPP 连接并且设置 IO 能力为 NoInputNoOutput
-------------------------------------------------------------------------------------------------------------------------------

在本例中，移动电话或 PC 为主机，{IDF_TARGET_NAME} 为从机。该示例展示了如何建立 SPP 连接。

#. Classic Bluetooth 初始化。
   
   命令：

   .. code-block:: none

     AT+BTINIT=1

   响应：
  
   .. code-block:: none

     OK

#. Classic Bluetooth SPP 协议初始化并且设置角色为 slave。
   
   命令：

   .. code-block:: none

     AT+BTSPPINIT=2

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 设备名称。
   
   命令：

   .. code-block:: none

     AT+BTNAME="EXAMPLE"

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 扫描模式为可发现可连接。
   
   命令：

   .. code-block:: none

     AT+BTSCANMODE=2

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 安全参数。设置 ``io_cap`` 为 ``NoInputNoOutput``，``pin_type`` 为 ``fixed``，``pin_code`` 为 ``9527``。
   
   命令：

   .. code-block:: none

     AT+BTSECPARAM=3,1,"9527"

   响应：
  
   .. code-block:: none

     OK

#. 开启 Classic Bluetooth SPP 协议。
   
   命令：

   .. code-block:: none

     AT+BTSPPSTART

   响应：
  
   .. code-block:: none

     OK

#. 移动电话或者 PC 发起连接。

   移动电话或 PC 应能找到名为 “EXAMPLE” 的蓝牙设备。如果移动电话或 PC 发起连接并成功建立连接，{IDF_TARGET_NAME} 将提示：

   .. code-block:: none

     +BTSPPCONN:0,"e0:24:81:47:90:bc"

   说明：

   - 您获取到的地址可能与上述响应中的不同。

#. 在透传模式下发送数据。

   命令：

   .. code-block:: none

     AT+BTSPPSEND

   响应：

   .. code-block:: none

     OK

     >

   上述响应表示 AT 已经进入透传模式。

   说明：

   - AT 进入透传模式后，串口收到的数据会传输到移动电话或者 PC 端。

#. 停止发送数据。

   在透传发送数据过程中，若识别到单独的一包数据 ``+++``，则系统会退出透传发送。此时请至少等待 1 秒，再发下一条 AT 命令。请注意，如果直接用键盘打字输入 ``+++``，有可能因时间太慢而不能被识别为连续的三个 ``+``。更多介绍请参考 :ref:`AT+BTSPPSEND <cmd-BTSPPSEND>`。

   .. Important::

     使用 ``+++`` 可退出透传发送数据，回到正常 AT 命令模式。您也可以使用 ``AT+BTSPPSEND`` 命令恢复透传。

#. 断开 Classic Bluetooth SPP 连接。

   命令：

   .. code-block:: none

     AT+BTSPPDISCONN=0

   响应：

   .. code-block:: none

     +BTSPPDISCONN:0,"e0:24:81:47:90:bc"

     OK

   说明：

   - 您获取到的地址可能与上述响应中的不同。

在移动电话或者 PC 和 {IDF_TARGET_NAME} 之间建立 SPP 连接并且设置 IO 能力为 KeyboardOnly
--------------------------------------------------------------------------------------------

该过程基本和 `以普通传输模式在移动电话或者 PC 和 {IDF_TARGET_NAME} 之间建立 SPP 连接并且设置 IO 能力为 NoInputNoOutput`_ 描述的一样。唯一的区别在于安全参数设置。

#. Classic Bluetooth 初始化。
   
   命令：

   .. code-block:: none

     AT+BTINIT=1

   响应：
  
   .. code-block:: none

     OK

#. Classic Bluetooth SPP 协议初始化并且设置角色为 slave。
   
   命令：

   .. code-block:: none

     AT+BTSPPINIT=2

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 设备名称。
   
   命令：

   .. code-block:: none

     AT+BTNAME="EXAMPLE"

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 扫描模式为可发现可连接。
   
   命令：

   .. code-block:: none

     AT+BTSCANMODE=2

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 安全参数。设置 ``io_cap`` 为 ``KeyboardOnly``，``pin_type`` 为 ``variable``，``pin_code`` 为 ``9527``。
   
   命令：

   .. code-block:: none

     AT+BTSECPARAM=2,0,"9527"

   响应：
  
   .. code-block:: none

     OK

#. 开启 Classic Bluetooth SPP 协议。
   
   命令：

   .. code-block:: none

     AT+BTSPPSTART

   响应：
  
   .. code-block:: none

     OK

#. 移动电话或者 PC 发起连接。

   移动电话或者 PC 可以发起连接并且产生 PIN 码，您可以在 {IDF_TARGET_NAME} 端输入 PIN 码。

   .. code-block:: none

     AT+BTKEYREPLY=0,676572

   如果连接建立成功，系统则会提示：

   .. code-block:: none

     +BTSPPCONN:0,"e0:24:81:47:90:bc"

   说明：

   - 您输入的 PIN 码可能与上述命令中的不同。请使用真实的 PIN 码代替。
   - 您获取到的地址可能与上述响应中的不同。

#. 断开 Classic Bluetooth SPP 连接。

   命令：

   .. code-block:: none

     AT+BTSPPDISCONN=0

   响应：

   .. code-block:: none

     +BTSPPDISCONN:0,"e0:24:81:47:90:bc"

     OK

   说明：

   - 您获取到的地址可能与上述响应中的不同。

在两个 {IDF_TARGET_NAME} 开发板之间建立 SPP 连接
----------------------------------------------------

下面是使用两块 {IDF_TARGET_NAME} 开发板的示例，一块作为主机，另一块作为从机。

.. Important::

  在以下步骤中以 ``主机`` 开头的操作只需要在主机端执行即可，以 ``从机`` 开头的操作只需要在从机端执行即可。如果操作没有特别指明在哪端操作，则需要在主机端和从机端都执行。

#. Classic Bluetooth 初始化。
   
   命令：

   .. code-block:: none

     AT+BTINIT=1

   响应：
  
   .. code-block:: none

     OK

#. Classic Bluetooth SPP 协议初始化。

   主机：

   命令：

   .. code-block:: none

     AT+BTSPPINIT=1

   响应：
  
   .. code-block:: none

     OK

   从机：

   命令：

   .. code-block:: none

     AT+BTSPPINIT=2

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 设备名称。

   从机：

   命令：

   .. code-block:: none

     AT+BTNAME="EXAMPLE"

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 扫描模式为可发现可连接。

   从机：

   命令：

   .. code-block:: none

     AT+BTSCANMODE=2

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 安全参数。设置 ``io_cap`` 为 ``NoInputNoOutput``，``pin_type`` 为 ``fixed``，``pin_code`` 为 ``9527``。

   从机：

   命令：

   .. code-block:: none

     AT+BTSECPARAM=3,1,"9527"

   响应：
  
   .. code-block:: none

     OK

#. 开启 Classic Bluetooth SPP 协议。

   从机：

   命令：

   .. code-block:: none

     AT+BTSPPSTART

   响应：
  
   .. code-block:: none

     OK

#. 开启发现 Classic Bluetooth 周围设备。设置持续时间为 10 秒，可以收到的回应的数量为 10。

   主机：

   命令：

   .. code-block:: none

     AT+BTSTARTDISC=0,10,10

   响应：
  
   .. code-block:: none

     +BTSTARTDISC:"10:f6:05:f9:bc:4f",realme V11 5G,0x2,0x3,0x2d0,-34
     +BTSTARTDISC:"24:0a:c4:d6:e4:46",EXAMPLE,,,,-27
     +BTSTARTDISC:"10:f6:05:f9:bc:4f",realme V11 5G,0x2,0x3,0x2d0,-33
     +BTSTARTDISC:"24:0a:c4:d6:e4:46",EXAMPLE,,,,-25
     +BTSTARTDISC:"ac:d6:18:47:0c:ae",,0x2,0x3,0x2d0,-72
     +BTSTARTDISC:"24:0a:c4:d6:e4:46",EXAMPLE,,,,-26
     +BTSTARTDISC:"10:f6:05:f9:bc:4f",,0x2,0x3,0x2d0,-41
     +BTSTARTDISC:"24:0a:c4:2c:a8:a2",,,,,-50
     +BTSTARTDISC:"24:0a:c4:d6:e4:46",EXAMPLE,,,,-26
     +BTSTARTDISC:"10:f6:05:f9:bc:4f",realme V11 5G,0x2,0x3,0x2d0,-39
     +BTSTARTDISC:"24:0a:c4:d6:e4:46",EXAMPLE,,,,-23
     +BTSTARTDISC:"10:f6:05:f9:bc:4f",realme V11 5G,0x2,0x3,0x2d0,-36
     +BTSTARTDISC:"10:f6:05:f9:bc:4f",realme V11 5G,0x2,0x3,0x2d0,-41
     +BTSTARTDISC:"b4:a5:ac:16:14:8c",,0x2,0x3,0x2d0,-57
     +BTSTARTDISC:"24:0a:c4:2c:a8:a2"
     +BTSTARTDISC:"b4:a5:ac:16:14:8c"

     OK

   说明：

   - 您的发现结果可能与上述响应中的不同。

#. 建立 SPP 连接。

   主机：

   命令：

   .. code-block:: none

     AT+BTSPPCONN=0,0,"24:0a:c4:d6:e4:46"

   响应：
  
   .. code-block:: none

     +BTSPPCONN:0,"24:0a:c4:d6:e4:46"

     OK

   说明：

   - 输入上述命令时，请使用您的从机地址。
   - 如果连接建立成功，从机端则会提示 ``+BTSPPCONN:0,"30:ae:a4:80:06:8e"``。

#. 断开 Classic Bluetooth SPP 连接。

   从机：

   命令：

   .. code-block:: none

     AT+BTSPPDISCONN=0

   响应：

   .. code-block:: none

     +BTSPPDISCONN:0,"30:ae:a4:80:06:8e"

     OK

   说明：

   - 主机和从机都可以主动断开连接。
   - 如果连接被成功断开，主机端则会提示 ``+BTSPPDISCONN:0,"24:0a:c4:d6:e4:46"``。

建立 A2DP 连接并启用 A2DP Sink 播放音乐
-------------------------------------------

.. Important::
  
  - 使用 ``A2DP Sink`` 需要客户自己添加 ``I2S`` 部分的代码。初始化 ``I2S`` 部分的代码请参考 `a2dp sink 例程 <https://github.com/espressif/esp-idf/blob/master/examples/bluetooth/bluedroid/classic_bt/a2dp_sink/main/main.c>`__。
  - ``decoder`` 芯片部分的驱动代码也需要客户自行添加或使用现成的开发板。

#. Classic Bluetooth 初始化。

   命令：

   .. code-block:: none

     AT+BTINIT=1

   响应：
  
   .. code-block:: none

     OK

#. Classic Bluetooth A2DP 协议初始化并且设置角色为 sink。

   命令：

   .. code-block:: none

     AT+BTA2DPINIT=2

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 设备名称。
   
   命令：

   .. code-block:: none

     AT+BTNAME="EXAMPLE"

   响应：
  
   .. code-block:: none

     OK

#. 设置 Classic Bluetooth 扫描模式为可发现可连接。

   命令：

   .. code-block:: none

     AT+BTSCANMODE=2

   响应：
  
   .. code-block:: none

     OK

#. 建立连接。

   source 角色应能找到名为 “EXAMPLE” 的蓝牙设备。在本例中您可以使用您的移动电话发起连接。如果连接成功建立，{IDF_TARGET_NAME} 将提示：

   .. code-block:: none

     +BTA2DPCONN:0,"e0:24:81:47:90:bc"

   说明：

   - 您获取到的地址可能与上述响应中的不同。

#. 开始播放音乐。

   命令：

   .. code-block:: none

     AT+BTA2DPCTRL=0,1

   响应：
  
   .. code-block:: none

     OK

   说明：

   - 更多类型控制请参考 :ref:`AT+BTA2DPCTRL <cmd-BTA2DPCTRL>`。

#. 停止播放音乐。

   命令：

   .. code-block:: none

     AT+BTA2DPCTRL=0,0

   响应：
  
   .. code-block:: none

     OK

   说明：

   - 更多类型控制请参考 :ref:`AT+BTA2DPCTRL <cmd-BTA2DPCTRL>`。

#. 断开 A2DP 连接。

   命令：

   .. code-block:: none

     AT+BTA2DPDISCONN=0

   响应：
  
   .. code-block:: none

     OK
     +BTA2DPDISCONN:0,"e0:24:81:47:90:bc"

查询和清除 Classic Bluetooth 加密设备列表
-----------------------------------------

#. 获取加密设备列表

   命令：

   .. code-block:: none

     AT+BTENCDEV?

   响应：
  
   .. code-block:: none

     +BTA2DPDISCONN:0,"e0:24:81:47:90:bc"
     OK

   说明：

   - 如果之前没有设备成功绑定过，AT 只会提示 ``OK``。

#. 清除 Classic Bluetooth 加密设备列表。

   有两种方式可以清除加密设备列表。

   1. 通过索引值删除加密列表中的指定设备。

      命令：

      .. code-block:: none

        AT+BTENCCLEAR=0

      响应：
     
      .. code-block:: none

        OK

   2. 删除加密列表中的全部设备。

      命令：

      .. code-block:: none

        AT+BTENCCLEAR

      响应：
     
      .. code-block:: none

        OK
