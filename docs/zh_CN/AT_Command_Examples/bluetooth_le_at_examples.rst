Bluetooth LE AT 示例
=================================

:link_to_translation:`en:[English]`

本文档主要介绍 :doc:`../AT_Command_Set/BLE_AT_Commands` 的使用方法，并提供在 {IDF_TARGET_NAME} 设备上运行这些命令的详细示例。

.. contents::
   :local:
   :depth: 1

简介
------------

ESP-AT 当前仅支持 **Bluetooth LE 4.2 协议规范**，本文档中的描述仅适用于 **Bluetooth LE protocol 4.2 协议规范**。请参考 `核心规范 4.2 <https://www.bluetooth.com/specifications/specs/core-specification-4-2/>`__ 获取更多信息。

Bluetooth LE 协议架构
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Bluetooth LE 协议栈从下至上分为几个层级：``Physical Layer (PHY)``、``Link Layer (LL)``、``Host Controller Interface (HCI)``、``Logical Link Control and Adaptation Protocol Layer (L2CAP)``、``Attribute Protocol (ATT)``、``Security Manager Protocol (SMP)``、``Generic Attribute Profile (GATT)``、``Generic Access Profile (GAP)``。

- ``PHY``：PHY 层主要负责在物理信道上发送和接收信息包。Bluetooth LE 使用 ``40`` 个射频信道。频率范围：2402 MHz 到 2480 MHz。
- ``LL``：LL 层主要负责创建、修改和释放逻辑链路（以及，如果需要，它们相关的逻辑传输），以及与设备之间的物理链路相关的参数的更新。它控制链路层状态机处于 ``准备``、``广播``、``监听/扫描``、``发起连接``、``已连接`` 五种状态之一。
- ``HCI``：HCI 层向主机和控制器提供一个标准化的接口。该层可以由软件 ``API`` 实现或者使用硬件接口 ``UART``、``SPI``、``USB`` 来控制。
- ``L2CAP``：L2CAP 层负责对主机和协议栈之间交换的数据进行协议复用能力、分段和重组操作。
- ``ATT``：ATT 层实现了属性服务器和属性客户端之间的点对点协议。ATT 客户端向 ATT 服务端发送命令、请求和确认。ATT 服务端向客户端发送响应、通知和指示。
- ``SMP``：SMP 层用于生成加密密钥和身份密钥。SMP 还管理加密密钥和身份密钥的存储，并负责生成随机地址并将随机地址解析为已知设备身份。
- ``GATT``：GATT 层表示属性服务器和可选的属性客户端的功能。该配置文件描述了属性服务器中使用的服务、特征和属性的层次结构。该层提供用于发现、读取、写入和指示服务特性和属性的接口。
- ``GAP``：GAP 层代表所有蓝牙设备通用的基本功能，例如传输、协议和应用程序配置文件使用的模式和访问程序。GAP 服务包括设备发现、连接模式、安全、身份验证、关联模型和服务发现。

Bluetooth LE 角色划分
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

在 Bluetooth LE 协议栈中不同的层级有不同的角色划分。这些角色划分互不影响。

- ``LL``：设备可以划分为 ``主机`` 和 ``从机``，从机广播，主机可以发起连接。
- ``GAP``：定义了 4 种特定角色：``广播者``、``观察者``、``外围设备`` 和 ``中心设备``。
- ``GATT``：设备可以分为 ``服务端`` 和 ``客户端``。

.. Important::

  - 本文档中描述的 ``Bluetooth LE 服务端`` 和 ``Bluetooth LE 客户端`` 都是 ``GATT`` 层角色。
  - 当前，ESP-AT 支持 ``Bluetooth LE 服务端`` 和 ``Bluetooth LE 客户端`` 同时存在。
  - 不论 ESP-AT 初始化为 Bluetooth LE 服务端还是 Bluetooth LE 客户端，同时连接的最大设备数量为 ``3``。

GATT 其实是一种属性传输协议，简单的讲可以认为是一种属性传输的应用层协议。这个属性的结构非常简单。它由 ``服务`` 组成，每个服务由不同数量的 ``特征`` 组成，每个 ``特征`` 又由很多其它的元素组成。

``GATT 服务端`` 和 ``GATT 客户端`` 这两种角色存在于 Bluetooth LE 连接建立之后。GATT 服务器存储通过属性协议传输的数据，并接受来自 GATT 客户端的属性协议请求、命令和确认。简而言之，提供数据的一端称为 ``GATT 服务端``，访问数据的一端称为 ``GATT 客户端``。

Bluetooth LE 客户端读写服务特征值
---------------------------------------------------------

以下示例同时使用两块 {IDF_TARGET_NAME} 开发板，其中一块作为 Bluetooth LE 服务端（只作为 Bluetooth LE 服务端角色），另一块作为 Bluetooth LE 客户端（只作为 Bluetooth LE 客户端角色）。这个例子展示了应如何使用 AT 命令建立 Bluetooth LE 连接，完成数据通信。

.. Important::

  在以下步骤中以 ``{IDF_TARGET_NAME} Bluetooth LE 服务端`` 开头的操作只需要在 {IDF_TARGET_NAME} Bluetooth LE 服务端执行即可，以 ``{IDF_TARGET_NAME} Bluetooth LE 客户端`` 开头的操作只需要在 {IDF_TARGET_NAME} Bluetooth LE 客户端执行即可。

#. 初始化 Bluetooth LE 功能。

   {IDF_TARGET_NAME} Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLEINIT=2

   响应：
  
   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLEINIT=1

   响应：
  
   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端获取其 MAC 地址。

   命令：

   .. code-block:: none

     AT+BLEADDR?

   响应：

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   说明：

   - 您查询到的地址可能与上述响应中的不同，请记住您的地址，下面的步骤中会用到。

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE 服务端创建服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSRVCRE
  
     响应：
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端开启服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSRVSTART
  
     响应：
  
     .. code-block:: none
  
       OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端发现服务特征。

   命令：

   .. code-block:: none

     AT+BLEGATTSCHAR?

   响应：

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300,0x02
       +BLEGATTSCHAR:"desc",1,1,1,0x2901
       +BLEGATTSCHAR:"char",1,2,0xC301,0x02
       +BLEGATTSCHAR:"desc",1,2,1,0x2901
       +BLEGATTSCHAR:"char",1,3,0xC302,0x08
       +BLEGATTSCHAR:"desc",1,3,1,0x2901
       +BLEGATTSCHAR:"char",1,4,0xC303,0x04
       +BLEGATTSCHAR:"desc",1,4,1,0x2901
       +BLEGATTSCHAR:"char",1,5,0xC304,0x08
       +BLEGATTSCHAR:"char",1,6,0xC305,0x10
       +BLEGATTSCHAR:"desc",1,6,1,0x2902
       +BLEGATTSCHAR:"char",1,7,0xC306,0x20
       +BLEGATTSCHAR:"desc",1,7,1,0x2902
       +BLEGATTSCHAR:"char",1,8,0xC307,0x02
       +BLEGATTSCHAR:"desc",1,8,1,0x2901
       +BLEGATTSCHAR:"char",2,1,0xC400,0x02
       +BLEGATTSCHAR:"desc",2,1,1,0x2901
       +BLEGATTSCHAR:"char",2,2,0xC401,0x02
       +BLEGATTSCHAR:"desc",2,2,1,0x2901

       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300
       +BLEGATTSCHAR:"char",1,2,0xC301
       +BLEGATTSCHAR:"char",1,3,0xC302
       +BLEGATTSCHAR:"char",1,4,0xC303
       +BLEGATTSCHAR:"char",1,5,0xC304
       +BLEGATTSCHAR:"char",1,6,0xC305
       +BLEGATTSCHAR:"char",1,7,0xC306
       +BLEGATTSCHAR:"char",1,8,0xC307
       +BLEGATTSCHAR:"char",2,1,0xC400
       +BLEGATTSCHAR:"char",2,2,0xC401

       OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端开始广播，之后 {IDF_TARGET_NAME} Bluetooth LE 客户端开始扫描并且持续 3 秒钟。

   {IDF_TARGET_NAME} Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLEADVSTART

   响应：

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLESCAN=1,3

   响应：

   .. code-block:: none

     OK
     +BLESCAN:"5b:3b:6c:51:90:49",-87,02011a020a0c0aff4c001005071c3024dc,,1
     +BLESCAN:"c4:5b:be:93:ec:66",-84,0201060303111809095647543147572d58020a03,,0
     +BLESCAN:"24:0a:c4:d6:e4:46",-29,,,0

   说明：

   - 您的扫描结果可能与上述响应中的不同。

#. 建立 Bluetooth LE 连接。

   {IDF_TARGET_NAME} Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLECONN=0,"24:0a:c4:d6:e4:46"

   响应：

   .. code-block:: none

     +BLECONN:0,"24:0a:c4:d6:e4:46"
     
     OK

   说明：

   - 输入上述命令时，请使用您的 {IDF_TARGET_NAME} Bluetooth LE 服务端地址。
   - 如果 Bluetooth LE 连接成功，则会提示 ``+BLECONN:0,"24:0a:c4:d6:e4:46``。
   - 如果 Bluetooth LE 连接失败，则会提示 ``+BLECONN:0,-1``。

#. {IDF_TARGET_NAME} Bluetooth LE 客户端发现服务。

   命令：

   .. code-block:: none

     AT+BLEGATTCPRIMSRV=0

   响应：

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1801,1
       +BLEGATTCPRIMSRV:0,2,0x1800,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1800,1
       +BLEGATTCPRIMSRV:0,2,0x1801,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   说明：

   - {IDF_TARGET_NAME} Bluetooth LE 客户端查询服务的结果，比 {IDF_TARGET_NAME} Bluetooth LE 服户端查询服务的结果多两个默认服务（UUID: 0x1800 和 0x1801），这是正常现象。正因如此，对于同一服务，{IDF_TARGET_NAME} Bluetooth LE 客户端查询的 <srv_index> 值等于 {IDF_TARGET_NAME} Bluetooth LE 服户端查询的 <srv_index> 值 + 2。例如上述示例中的服务 0xA002，当前在 {IDF_TARGET_NAME} Bluetooth LE 客户端查询到的 <srv_index> 为 3，如果在 {IDF_TARGET_NAME} Bluetooth LE 服户端通过 :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` 命令查询，则 <srv_index> 为 1。

#. {IDF_TARGET_NAME} Bluetooth LE 客户端发现特征值。

   命令：

   .. code-block:: none

     AT+BLEGATTCCHAR=0,3

   响应：

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
       +BLEGATTCCHAR:"desc",0,3,1,1,0x2901
       +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
       +BLEGATTCCHAR:"desc",0,3,2,1,0x2901
       +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
       +BLEGATTCCHAR:"desc",0,3,3,1,0x2901
       +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
       +BLEGATTCCHAR:"desc",0,3,4,1,0x2901
       +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
       +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
       +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
       +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
       +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
       +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02
       +BLEGATTCCHAR:"desc",0,3,8,1,0x2901
      
       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
       +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
       +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
       +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
       +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
       +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
       +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
       +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
       +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
       +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02

       OK

#. {IDF_TARGET_NAME} Bluetooth LE 客户端读取一个特征值。

   命令：

   .. code-block:: none

     AT+BLEGATTCRD=0,3,1

   响应：

   .. code-block:: none

     +BLEGATTCRD:0,1,0

     OK

   说明：

   - 请注意目标特征值必须要有读权限。
   - 如果 {IDF_TARGET_NAME} Bluetooth LE 客户端读取特征成功，{IDF_TARGET_NAME} Bluetooth LE 服务端则会提示 ``+READ:0,"7c:df:a1:b3:8d:de"``。

#. {IDF_TARGET_NAME} Bluetooth LE 客户端写一个特征值。

   命令：

   .. code-block:: none

     AT+BLEGATTCWR=0,3,3,,2

   响应：

   .. code-block:: none

     >

   符号 ``>`` 表示 AT 准备好接收串口数据，此时您可以输入数据，当数据长度达到参数 ``<length>`` 的值时，执行写入操作。

   .. code-block:: none

     OK

   说明：

   - 如果 {IDF_TARGET_NAME} Bluetooth LE 客户端写特征描述符成功，{IDF_TARGET_NAME} Bluetooth LE 服务端则会提示 ``+WRITE:<conn_index>,<srv_index>,<char_index>,[<desc_index>],<len>,<value>``。

#. Indicate 一个特征值。

   {IDF_TARGET_NAME} Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLEGATTCWR=0,3,7,1,2

   响应：

   .. code-block:: none

     >

   符号 ``>`` 表示 AT 准备好接收串口数据，此时您可以输入数据，当数据长度达到参数 ``<length>`` 的值时，执行写入操作。

   为了接收 {IDF_TARGET_NAME} Bluetooth LE 服务端发送过来的数据（通过 ``notify`` 方式或者 ``indicate`` 方式），{IDF_TARGET_NAME} Bluetooth LE 客户端需要提前向服务端注册。对于 ``notify`` 方式，需要写入值 ``0x0001``，对于 ``indicate`` 方式，需要写入值 ``0x0002``。在本例中写入 ``0x0002`` 来使用 ``indicate`` 方式。

   .. code-block:: none

     OK

   说明：

   - 如果 {IDF_TARGET_NAME} Bluetooth LE 客户端写特征描述符成功，{IDF_TARGET_NAME} Bluetooth LE 服务端则会提示 ``+WRITE:<conn_index>,<srv_index>,<char_index>,<desc_index>,<len>,<value>``。

   {IDF_TARGET_NAME} Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLEGATTSIND=0,1,7,3

   响应：

   .. code-block:: none

     >

   符号 ``>`` 表示 AT 准备好接收串口数据，此时您可以输入数据，当数据长度达到参数 ``<length>`` 的值时，执行 indicate 操作。

   .. code-block:: none

     OK

   说明：

   - 如果 {IDF_TARGET_NAME} Bluetooth LE 客户端接收到 indication, 则会提示 ``+INDICATE:<conn_index>,<srv_index>,<char_index>,<len>,<value>``。
   - 对于同一服务，{IDF_TARGET_NAME} Bluetooth LE 客户端的 <srv_index> 值等于 {IDF_TARGET_NAME} Bluetooth LE 服户端的 <srv_index> 值 + 2，这是正常现象。
   - 对于服务中特征的权限，您可参考文档 :doc:`../Compile_and_Develop/How_to_customize_BLE_services`。

.. only:: esp32 or esp32c3 or esp32c5 or esp32c6

  Bluetooth LE 服务端读写服务特征值
  ---------------------------------------------------------
  
  以下示例同时使用两块 {IDF_TARGET_NAME} 开发板，其中一块作为 Bluetooth LE 服务端（只作为 Bluetooth LE 服务端角色），另一块作为 Bluetooth LE 客户端（只作为 Bluetooth LE 客户端角色）。这个例子展示了应如何建立 Bluetooth LE 连接，以及服务端读写服务特征值和客户端设置，notify 服务特征值。
  
  .. Important::
    步骤中以 ``{IDF_TARGET_NAME} Bluetooth LE 服务端`` 开头的操作只需要在 {IDF_TARGET_NAME} Bluetooth LE 服务端执行即可，以 ``{IDF_TARGET_NAME} Bluetooth LE 客户端`` 开头的操作只需要在 {IDF_TARGET_NAME} Bluetooth LE 客户端执行即可。
  
  #. 初始化 Bluetooth LE 功能。
  
     {IDF_TARGET_NAME} Bluetooth LE 服务端：
  
     命令：
  
     .. code-block:: none
  
       AT+BLEINIT=2
  
     响应：
  
     .. code-block:: none
  
       OK
  
     {IDF_TARGET_NAME} Bluetooth LE 客户端：
    
     命令：
  
     .. code-block:: none
  
       AT+BLEINIT=1
  
     响应：
  
     .. code-block:: none
  
       OK

  .. only:: esp32 or esp32c3

    #. {IDF_TARGET_NAME} Bluetooth LE 服务端创建服务。
    
       命令：
    
       .. code-block:: none
    
         AT+BLEGATTSSRVCRE
    
       响应：
    
       .. code-block:: none
    
         OK
    
    #. {IDF_TARGET_NAME} Bluetooth LE 服务端开启服务。
    
       命令：
    
       .. code-block:: none
    
         AT+BLEGATTSSRVSTART
    
       响应：
    
       .. code-block:: none
    
         OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端获取其 MAC 地址。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEADDR?
  
     响应：
  
     .. code-block:: none
  
       +BLEADDR:"24:0a:c4:d6:e4:46"
       OK
  
     说明：
  
     - 您查询到的地址可能与上述响应中的不同，请记住您的地址，下面的步骤中会用到。
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端设置广播参数。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEADVPARAM=50,50,0,0,7,0,,
  
     响应：
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端设置广播数据。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEADVDATA="0201060A09457370726573736966030302A0"
  
     响应：
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端开始广播。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEADVSTART
  
     响应：
  
     .. code-block:: none
  
       OK

  .. only:: esp32 or esp32c3

    #. {IDF_TARGET_NAME} Bluetooth LE 客户端创建服务。
    
       命令：
    
       .. code-block:: none
    
         AT+BLEGATTSSRVCRE
    
       响应：
    
       .. code-block:: none
    
         OK
    
    #. {IDF_TARGET_NAME} Bluetooth LE 客户端开启服务。
    
       命令：
    
       .. code-block:: none
    
         AT+BLEGATTSSRVSTART
    
       响应：
    
       .. code-block:: none
    
         OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 客户端获取其 MAC 地址。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEADDR?
  
     响应：
  
     .. code-block:: none
  
       +BLEADDR:"24:0a:c4:03:a7:4e"
       OK
  
     说明：
  
     - 您查询到的地址可能与上述响应中的不同，请记住您的地址，下面的步骤中会用到。
  
  #. {IDF_TARGET_NAME} Bluetooth LE 客户端开始扫描，持续 3 秒。
  
     命令：
  
     .. code-block:: none
  
       AT+BLESCAN=1,3
  
     响应：
  
     .. code-block:: none
  
       OK
       +BLESCAN:"24:0a:c4:d6:e4:46",-78,0201060a09457370726573736966030302a0,,0
       +BLESCAN:"45:03:cb:ac:aa:a0",-62,0201060aff4c001005441c61df7d,,1
       +BLESCAN:"24:0a:c4:d6:e4:46",-26,0201060a09457370726573736966030302a0,,0
  
     说明：
  
     - 您的扫描结果可能与上述响应中的不同。
  
  #. 建立 the Bluetooth LE 连接。
  
     {IDF_TARGET_NAME} Bluetooth LE 客户端：
  
     命令：
  
     .. code-block:: none
  
       AT+BLECONN=0,"24:0a:c4:d6:e4:46"
  
     响应：
  
     .. code-block:: none
  
       +BLECONN:0,"24:0a:c4:d6:e4:46"
       
       OK
  
     说明：
  
     - 输入上述命令时，请使用您的 {IDF_TARGET_NAME} Bluetooth LE 服务端地址。
     - 如果 Bluetooth LE 连接成功，则会提示 ``+BLECONN:0,"24:0a:c4:d6:e4:46``。
     - 如果 Bluetooth LE 连接失败，则会提示 ``+BLECONN:0,-1``。
  
    {IDF_TARGET_NAME} Bluetooth LE 服务端：
  
     命令：
  
     .. code-block:: none
  
       AT+BLECONN=0,"24:0a:c4:03:a7:4e"
  
     响应：
  
     .. code-block:: none
       
       +BLECONN:0,"24:0a:c4:03:a7:4e"
  
       OK
  
     说明：
  
     - 输入上述命令时，请使用您的 {IDF_TARGET_NAME} Bluetooth LE 客户端地址。
     - 如果 Bluetooth LE 连接成功，则会提示 ``OK``，不会提示 ``+BLECONN:0,"24:0a:c4:03:a7:4e``。
     - 如果 Bluetooth LE 连接失败，则会提示 ``ERROR``，不会提示 ``+BLECONN:0,-1``。
  
  #. {IDF_TARGET_NAME} Bluetooth LE 客户端查询本地服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSRV?
  
     响应：
  
     .. code-block:: none
  
       +BLEGATTSSRV:1,1,0xA002,1
       +BLEGATTSSRV:2,1,0xA003,1
       
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 客户端发现本地特征。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSCHAR?
  
     响应：
  
     .. code-block:: none
  
       +BLEGATTSCHAR:"char",1,1,0xC300,0x02
       +BLEGATTSCHAR:"desc",1,1,1,0x2901
       +BLEGATTSCHAR:"char",1,2,0xC301,0x02
       +BLEGATTSCHAR:"desc",1,2,1,0x2901
       +BLEGATTSCHAR:"char",1,3,0xC302,0x08
       +BLEGATTSCHAR:"desc",1,3,1,0x2901
       +BLEGATTSCHAR:"char",1,4,0xC303,0x04
       +BLEGATTSCHAR:"desc",1,4,1,0x2901
       +BLEGATTSCHAR:"char",1,5,0xC304,0x08
       +BLEGATTSCHAR:"char",1,6,0xC305,0x10
       +BLEGATTSCHAR:"desc",1,6,1,0x2902
       +BLEGATTSCHAR:"char",1,7,0xC306,0x20
       +BLEGATTSCHAR:"desc",1,7,1,0x2902
       +BLEGATTSCHAR:"char",1,8,0xC307,0x02
       +BLEGATTSCHAR:"desc",1,8,1,0x2901
       +BLEGATTSCHAR:"char",2,1,0xC400,0x02
       +BLEGATTSCHAR:"desc",2,1,1,0x2901
       +BLEGATTSCHAR:"char",2,2,0xC401,0x02
       +BLEGATTSCHAR:"desc",2,2,1,0x2901
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端发现对端服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTCPRIMSRV=0
  
     响应：
  
     .. code-block:: none
  
       +BLEGATTCPRIMSRV:0,1,0x1801,1
       +BLEGATTCPRIMSRV:0,2,0x1800,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1
  
       OK
  
     说明：
  
     - {IDF_TARGET_NAME} Bluetooth LE 服务端查询服务的结果，比 {IDF_TARGET_NAME} Bluetooth LE 客户端查询服务的结果多两个默认服务（UUID: 0x1800 和 0x1801）。正因如此，对于同一服务，{IDF_TARGET_NAME} Bluetooth LE 服务端查询的 <srv_index> 值等于 {IDF_TARGET_NAME} Bluetooth LE 客户端查询的 <srv_index> 值 + 2。例如，上述示例中的服务 0xA002，当前在 {IDF_TARGET_NAME} Bluetooth LE 服务端查询到的 <srv_index> 为 3，如果在 {IDF_TARGET_NAME} Bluetooth LE 服务端通过 :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` 命令查询，则 <srv_index> 为 1。
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端发现对端特征。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTCCHAR=0,3
  
     响应：
  
     .. code-block:: none
 
       +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
       +BLEGATTCCHAR:"desc",0,3,1,1,0x2901
       +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
       +BLEGATTCCHAR:"desc",0,3,2,1,0x2901
       +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
       +BLEGATTCCHAR:"desc",0,3,3,1,0x2901
       +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
       +BLEGATTCCHAR:"desc",0,3,4,1,0x2901
       +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
       +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
       +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
       +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
       +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
       +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02
       +BLEGATTCCHAR:"desc",0,3,8,1,0x2901
       
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 客户端设置服务特征值。
  
     选择支持写操作的服务特征（characteristic）去设置服务特征值。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSETATTR=1,8,,1
  
     响应：
  
     .. code-block:: none
  
       >
  
     命令：
  
     .. code-block:: none
  
       写入一个字节 ``9``
  
     响应：
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端读服务特征值。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTCRD=0,3,8,
  
     响应：
  
     .. code-block:: none
  
       +BLEGATTCRD:0,1,9
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端写服务特征值。
  
     选择支持写操作的服务特性写入特性。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTCWR=0,3,6,1,2
  
     响应：
  
     .. code-block:: none
  
       >
    
     命令：
  
     .. code-block:: none
  
       写入2个字节 ``12``
  
     响应：
  
     .. code-block:: none
  
       OK
  
     说明：
  
     - 如果 Bluetooth LE 服务端写服务特征值成功后，Bluetooth LE 客户端则会提示 ``+WRITE:0,1,6,1,2,12``。
  
  #. {IDF_TARGET_NAME} Bluetooth LE 客户端 notify 服务特征值
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSNTFY=0,1,6,10
  
     响应：
  
     .. code-block:: none
  
       >
  
     命令：
  
     .. code-block:: none
  
       写入 ``1234567890`` 10个字节
  
     响应：
  
     .. code-block:: none
  
       OK
  
     说明：
  
     - 如果 {IDF_TARGET_NAME} Bluetooth LE 客户端 notify 服务特征值给服务端成功，Bluetooth LE 服务端则会提示 ``+NOTIFY:0,3,6,10,1234567890``。
  
Bluetooth LE 连接加密
----------------------------------

以下示例同时使用两块 {IDF_TARGET_NAME} 开发板，其中一块作为 Bluetooth LE 服务端（只作为 Bluetooth LE 服务端角色），另一块作为 Bluetooth LE 客户端（只作为 Bluetooth LE 客户端角色）。这个例子展示了怎样加密 Bluetooth LE 连接。

.. Important::

  - 在以下步骤中以 ``{IDF_TARGET_NAME} Bluetooth LE 服务端`` 开头的操作只需要在 {IDF_TARGET_NAME} Bluetooth LE 服务端执行即可，以 ``{IDF_TARGET_NAME} Bluetooth LE 客户端`` 开头的操作只需要在 {IDF_TARGET_NAME} Bluetooth LE 客户端执行即可。
  - ``加密`` 和 ``绑定`` 是两个不同的概念。``绑定`` 只是加密成功后在本地存储了一个长期的密钥。
  - ESP-AT 最多允许绑定 ``10`` 个设备。

#. 初始化 Bluetooth LE 功能。

   {IDF_TARGET_NAME} Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLEINIT=2

   响应：
  
   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLEINIT=1

   响应：
  
   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端获取 Bluetooth LE 地址。

   命令：

   .. code-block:: none

     AT+BLEADDR?

   响应：

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   说明：

   - 您查询到的地址可能与上述响应中的不同，请记住您的地址，下面的步骤中会用到。

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE 服务端创建服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSRVCRE
  
     响应：
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端开启服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSRVSTART
  
     响应：
  
     .. code-block:: none
  
       OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端发现服务特征。

   命令：

   .. code-block:: none

     AT+BLEGATTSCHAR?

   响应：

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300,0x02
       +BLEGATTSCHAR:"desc",1,1,1,0x2901
       +BLEGATTSCHAR:"char",1,2,0xC301,0x02
       +BLEGATTSCHAR:"desc",1,2,1,0x2901
       +BLEGATTSCHAR:"char",1,3,0xC302,0x08
       +BLEGATTSCHAR:"desc",1,3,1,0x2901
       +BLEGATTSCHAR:"char",1,4,0xC303,0x04
       +BLEGATTSCHAR:"desc",1,4,1,0x2901
       +BLEGATTSCHAR:"char",1,5,0xC304,0x08
       +BLEGATTSCHAR:"char",1,6,0xC305,0x10
       +BLEGATTSCHAR:"desc",1,6,1,0x2902
       +BLEGATTSCHAR:"char",1,7,0xC306,0x20
       +BLEGATTSCHAR:"desc",1,7,1,0x2902
       +BLEGATTSCHAR:"char",1,8,0xC307,0x02
       +BLEGATTSCHAR:"desc",1,8,1,0x2901
       +BLEGATTSCHAR:"char",2,1,0xC400,0x02
       +BLEGATTSCHAR:"desc",2,1,1,0x2901
       +BLEGATTSCHAR:"char",2,2,0xC401,0x02
       +BLEGATTSCHAR:"desc",2,2,1,0x2901

       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300
       +BLEGATTSCHAR:"char",1,2,0xC301
       +BLEGATTSCHAR:"char",1,3,0xC302
       +BLEGATTSCHAR:"char",1,4,0xC303
       +BLEGATTSCHAR:"char",1,5,0xC304
       +BLEGATTSCHAR:"char",1,6,0xC305
       +BLEGATTSCHAR:"char",1,7,0xC306
       +BLEGATTSCHAR:"char",1,8,0xC307
       +BLEGATTSCHAR:"char",2,1,0xC400
       +BLEGATTSCHAR:"char",2,2,0xC401

       OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端开始广播，之后 {IDF_TARGET_NAME} Bluetooth LE 客户端开始扫描并且持续 3 秒钟。

   {IDF_TARGET_NAME} Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLEADVSTART

   响应：

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLESCAN=1,3

   响应：

   .. code-block:: none

     OK
     +BLESCAN:"5b:3b:6c:51:90:49",-87,02011a020a0c0aff4c001005071c3024dc,,1
     +BLESCAN:"c4:5b:be:93:ec:66",-84,0201060303111809095647543147572d58020a03,,0
     +BLESCAN:"24:0a:c4:d6:e4:46",-29,,,0

   说明：

   - 您的扫描结果可能与上述响应中的不同。

#. 建立 Bluetooth LE 连接。

   {IDF_TARGET_NAME} Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLECONN=0,"24:0a:c4:d6:e4:46"

   响应：

   .. code-block:: none

     +BLECONN:0,"24:0a:c4:d6:e4:46"
     
     OK

   说明：

   - 输入上述命令时，请使用您的 {IDF_TARGET_NAME} Bluetooth LE 服务端地址。
   - 如果 Bluetooth LE 连接成功，则会提示 ``+BLECONN:0,"24:0a:c4:d6:e4:46``。
   - 如果 Bluetooth LE 连接失败，则会提示 ``+BLECONN:0,-1``。

#. {IDF_TARGET_NAME} Bluetooth LE 客户端发现服务。

   命令：

   .. code-block:: none

     AT+BLEGATTCPRIMSRV=0

   响应：

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1801,1
       +BLEGATTCPRIMSRV:0,2,0x1800,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1800,1
       +BLEGATTCPRIMSRV:0,2,0x1801,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   说明：

   - {IDF_TARGET_NAME} Bluetooth LE 客户端查询服务的结果，比 {IDF_TARGET_NAME} Bluetooth LE 服户端查询服务的结果多两个默认服务（UUID: 0x1800 和 0x1801），这是正常现象。正因如此，对于同一服务，{IDF_TARGET_NAME} Bluetooth LE 客户端查询的 <srv_index> 值等于 {IDF_TARGET_NAME} Bluetooth LE 服户端查询的 <srv_index> 值 + 2。例如上述示例中的服务 0xA002，当前在 {IDF_TARGET_NAME} Bluetooth LE 客户端查询到的 <srv_index> 为 3，如果在 {IDF_TARGET_NAME} Bluetooth LE 服户端通过 :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` 命令查询，则 <srv_index> 为 1。

#. {IDF_TARGET_NAME} Bluetooth LE 客户端发现特征值。

   命令：

   .. code-block:: none

     AT+BLEGATTCCHAR=0,3

   响应：

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
       +BLEGATTCCHAR:"desc",0,3,1,1,0x2901
       +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
       +BLEGATTCCHAR:"desc",0,3,2,1,0x2901
       +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
       +BLEGATTCCHAR:"desc",0,3,3,1,0x2901
       +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
       +BLEGATTCCHAR:"desc",0,3,4,1,0x2901
       +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
       +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
       +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
       +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
       +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
       +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02
       +BLEGATTCCHAR:"desc",0,3,8,1,0x2901
      
       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
       +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
       +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
       +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
       +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
       +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
       +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
       +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
       +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
       +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02

       OK

#. 设置加密参数。设置 ``auth_req`` 为 ``SC_MITM_BOND``，服务端的 ``iocap`` 为 ``KeyboardOnly``，客户端的 ``iocap`` 为 ``KeyboardDisplay``，``key_size`` 为 ``16``，``init_key`` 为 ``3``, ``rsp_key`` 为 ``3``。

   {IDF_TARGET_NAME} Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLESECPARAM=13,2,16,3,3

   响应：

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLESECPARAM=13,4,16,3,3

   响应：

   .. code-block:: none

     OK

   说明：

   - 在本例中，{IDF_TARGET_NAME} Bluetooth LE 服务端输入配对码，{IDF_TARGET_NAME} Bluetooth LE 客户端显示配对码。
   - ESP-AT 支持 ``Legacy Pairing`` 和 ``Secure Connections`` 两种加密方式，但后者有更高级别的优先级。如果对端也支持 ``Secure Connections``，则会采用 ``Secure Connections`` 方式加密。

#. {IDF_TARGET_NAME} Bluetooth LE 客户端发起加密请求。

   命令：

   .. code-block:: none

     AT+BLEENC=0,3

   响应：

   .. code-block:: none

     OK

   说明：

   如果 {IDF_TARGET_NAME} Bluetooth LE 服务端成功接收到加密请求，{IDF_TARGET_NAME} Bluetooth LE 服务端则会提示 ``+BLESECREQ:0``。

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE 服务端响应配对请求。

    命令：

    .. code-block:: none

      AT+BLEENCRSP=0,1

    响应：

    .. code-block:: none

      OK

    说明：

    - 如果 {IDF_TARGET_NAME} Bluetooth LE 客户端成功收到配对响应，则 {IDF_TARGET_NAME} Bluetooth LE 客户端将会产生一个 6 位的配对码。
    - 在本例中，{IDF_TARGET_NAME} Bluetooth LE 客户端则会提示 ``+BLESECNTFYKEY:0,793718``。配对码为 ``793718``。

#. {IDF_TARGET_NAME} Bluetooth LE 客户端回复配对码。

   命令：

   .. code-block:: none

     AT+BLEKEYREPLY=0,793718

   响应：

   .. code-block:: none

     OK

   执行这个命令之后，在 {IDF_TARGET_NAME} Bluetooth LE 服务端和 {IDF_TARGET_NAME} Bluetooth LE 客户端会有一些对应信息提示。

   {IDF_TARGET_NAME} Bluetooth LE 服务端：

   .. code-block:: none

     +BLESECKEYTYPE:0,16
     +BLESECKEYTYPE:0,1
     +BLESECKEYTYPE:0,32
     +BLESECKEYTYPE:0,2
     +BLEAUTHCMPL:0,0

   {IDF_TARGET_NAME} Bluetooth LE 客户端：

   .. code-block:: none

     +BLESECNTFYKEY:0,793718
     +BLESECKEYTYPE:0,2
     +BLESECKEYTYPE:0,16
     +BLESECKEYTYPE:0,1
     +BLESECKEYTYPE:0,32
     +BLEAUTHCMPL:0,0

   您可以忽略以 ``+BLESECKEYTYPE`` 开头的信息。信息 ``+BLEAUTHCMPL:0,0`` 中的第二个参数为 ``0`` 表示加密成功，为 ``1`` 表示加密失败。

两个 {IDF_TARGET_NAME} 开发板之间建立 SPP 连接，以及在 UART-Bluetooth LE 透传模式下传输数据
-----------------------------------------------------------------------------------------------------

以下示例同时使用两块 {IDF_TARGET_NAME} 开发板，其中一块作为 Bluetooth LE 服务端（只作为 Bluetooth LE 服务端角色），另一块作为 Bluetooth LE 客户端（只作为 Bluetooth LE 客户端角色）。这个例子展示了应如何建立 Bluetooth LE 连接，以及建立透传通信 Bluetooth LE SPP (Serial Port Profile, UART-Bluetooth LE 透传模式)。

.. Important::

  在以下步骤中以 ``{IDF_TARGET_NAME} Bluetooth LE 服务端`` 开头的操作只需要在 {IDF_TARGET_NAME} Bluetooth LE 服务端执行即可，以 ``{IDF_TARGET_NAME} Bluetooth LE 客户端`` 开头的操作只需要在 {IDF_TARGET_NAME} Bluetooth LE 客户端执行即可。

#. 初始化 Bluetooth LE 功能。

   {IDF_TARGET_NAME} Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLEINIT=2

   响应：

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE 客户端：
  
   命令：

   .. code-block:: none

     AT+BLEINIT=1

   响应：

   .. code-block:: none

     OK

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE 服务端创建服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSRVCRE
  
     响应：
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端开启服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSRVSTART
  
     响应：
  
     .. code-block:: none
  
       OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端获取其 MAC 地址。

   命令：

   .. code-block:: none

     AT+BLEADDR?

   响应：

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   说明：

   - 您查询到的地址可能与上述响应中的不同，请记住您的地址，下面的步骤中会用到。

#. {IDF_TARGET_NAME} Bluetooth LE 服务端设置广播参数。

   命令：

   .. code-block:: none

     AT+BLEADVPARAM=50,50,0,0,7,0,,

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端设置广播数据。

   命令：

   .. code-block:: none

     AT+BLEADVDATA="0201060A09457370726573736966030302A0"

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端开始广播。

   命令：

   .. code-block:: none

     AT+BLEADVSTART

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 客户端开始扫描，持续 3 秒。

   命令：

   .. code-block:: none

     AT+BLESCAN=1,3

   响应：

   .. code-block:: none

     OK
     +BLESCAN:"24:0a:c4:d6:e4:46",-78,0201060a09457370726573736966030302a0,,0
     +BLESCAN:"45:03:cb:ac:aa:a0",-62,0201060aff4c001005441c61df7d,,1
     +BLESCAN:"24:0a:c4:d6:e4:46",-26,0201060a09457370726573736966030302a0,,0

   说明：

   - 您的扫描结果可能与上述响应中的不同。

#. 建立 the Bluetooth LE 连接。

   {IDF_TARGET_NAME} Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLECONN=0,"24:0a:c4:d6:e4:46"

   响应：

   .. code-block:: none

     +BLECONN:0,"24:0a:c4:d6:e4:46"
     
     OK

   说明：

   - 输入上述命令时，请使用您的 {IDF_TARGET_NAME} Bluetooth LE 服务端地址。
   - 如果 Bluetooth LE 连接成功，则会提示 ``+BLECONN:0,"24:0a:c4:d6:e4:46``。
   - 如果 Bluetooth LE 连接失败，则会提示 ``+BLECONN:0,-1``。

#. {IDF_TARGET_NAME} Bluetooth LE 服务端查询服务。

   命令：

   .. code-block:: none

     AT+BLEGATTSSRV?

   响应：

   .. code-block:: none

     +BLEGATTSSRV:1,1,0xA002,1
     +BLEGATTSSRV:2,1,0xA003,1
     
     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端发现特征。

   命令：

   .. code-block:: none

     AT+BLEGATTSCHAR?

   响应：

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300,0x02
       +BLEGATTSCHAR:"desc",1,1,1,0x2901
       +BLEGATTSCHAR:"char",1,2,0xC301,0x02
       +BLEGATTSCHAR:"desc",1,2,1,0x2901
       +BLEGATTSCHAR:"char",1,3,0xC302,0x08
       +BLEGATTSCHAR:"desc",1,3,1,0x2901
       +BLEGATTSCHAR:"char",1,4,0xC303,0x04
       +BLEGATTSCHAR:"desc",1,4,1,0x2901
       +BLEGATTSCHAR:"char",1,5,0xC304,0x08
       +BLEGATTSCHAR:"char",1,6,0xC305,0x10
       +BLEGATTSCHAR:"desc",1,6,1,0x2902
       +BLEGATTSCHAR:"char",1,7,0xC306,0x20
       +BLEGATTSCHAR:"desc",1,7,1,0x2902
       +BLEGATTSCHAR:"char",1,8,0xC307,0x02
       +BLEGATTSCHAR:"desc",1,8,1,0x2901
       +BLEGATTSCHAR:"char",2,1,0xC400,0x02
       +BLEGATTSCHAR:"desc",2,1,1,0x2901
       +BLEGATTSCHAR:"char",2,2,0xC401,0x02
       +BLEGATTSCHAR:"desc",2,2,1,0x2901

       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300
       +BLEGATTSCHAR:"char",1,2,0xC301
       +BLEGATTSCHAR:"char",1,3,0xC302
       +BLEGATTSCHAR:"char",1,4,0xC303
       +BLEGATTSCHAR:"char",1,5,0xC304
       +BLEGATTSCHAR:"char",1,6,0xC305
       +BLEGATTSCHAR:"char",1,7,0xC306
       +BLEGATTSCHAR:"char",1,8,0xC307
       +BLEGATTSCHAR:"char",2,1,0xC400
       +BLEGATTSCHAR:"char",2,2,0xC401

       OK

#. {IDF_TARGET_NAME} Bluetooth LE 客户端发现服务。

   命令：

   .. code-block:: none

     AT+BLEGATTCPRIMSRV=0

   响应：

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1801,1
       +BLEGATTCPRIMSRV:0,2,0x1800,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1800,1
       +BLEGATTCPRIMSRV:0,2,0x1801,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   说明：

   - {IDF_TARGET_NAME} Bluetooth LE 客户端查询服务的结果，比 {IDF_TARGET_NAME} Bluetooth LE 服户端查询服务的结果多两个默认服务（UUID: 0x1800 和 0x1801），这是正常现象。正因如此，对于同一服务，{IDF_TARGET_NAME} Bluetooth LE 客户端查询的 <srv_index> 值等于 {IDF_TARGET_NAME} Bluetooth LE 服户端查询的 <srv_index> 值 + 2。例如，上述示例中的服务 0xA002，当前在 {IDF_TARGET_NAME} Bluetooth LE 客户端查询到的 <srv_index> 为 3，如果在 {IDF_TARGET_NAME} Bluetooth LE 服户端通过 :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` 命令查询，则 <srv_index> 为 1。

#. {IDF_TARGET_NAME} Bluetooth LE 客户端发现特征。

   命令：

   .. code-block:: none

     AT+BLEGATTCCHAR=0,3

   响应：

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
       +BLEGATTCCHAR:"desc",0,3,1,1,0x2901
       +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
       +BLEGATTCCHAR:"desc",0,3,2,1,0x2901
       +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
       +BLEGATTCCHAR:"desc",0,3,3,1,0x2901
       +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
       +BLEGATTCCHAR:"desc",0,3,4,1,0x2901
       +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
       +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
       +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
       +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
       +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
       +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02
       +BLEGATTCCHAR:"desc",0,3,8,1,0x2901
      
       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
       +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
       +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
       +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
       +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
       +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
       +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
       +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
       +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
       +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02

       OK

#. {IDF_TARGET_NAME} Bluetooth LE 客户端配置 Bluetooth LE SPP。

   选择支持写操作的服务特征（characteristic）作为写通道发送数据，选择支持 notify 或者 indicate 的 characteristic 作为读通道接收数据。

   命令：

   .. code-block:: none

     AT+BLESPPCFG=1,3,5,3,7

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 客户端使能 Bluetooth LE SPP。

   命令：

   .. code-block:: none

     AT+BLESPP

   响应：

   .. code-block:: none

     OK

     >

   上述响应表示 AT 已经进入 Bluetooth LE SPP 模式，可以进行数据的发送和接收。

   说明：

   - {IDF_TARGET_NAME} Bluetooth LE 客户端开启 Bluetooth LE SPP 透传模式后，串口收到的数据会通过 Bluetooth LE 传输到 {IDF_TARGET_NAME} Bluetooth LE 服务端。

#. {IDF_TARGET_NAME} Bluetooth LE 服务端配置 Bluetooth LE SPP。

   选择支持 notify 或者 indicate 的 characteristic 作为写通道发送数据，选择支持写操作的 characteristic 作为读通道接收数据。

   命令：

   .. code-block:: none

     AT+BLESPPCFG=1,1,7,1,5

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端使能 Bluetooth LE SPP。

   命令：

   .. code-block:: none

     AT+BLESPP

   响应：

   .. code-block:: none

     OK

     >

   上述响应表示 AT 已经进入 Bluetooth LE SPP 模式，可以进行数据的发送和接收。

   说明：

   - {IDF_TARGET_NAME} Bluetooth LE 服户端开启 Bluetooth LE SPP 透传模式后，串口收到的数据会通过 Bluetooth LE 传输到 {IDF_TARGET_NAME} Bluetooth LE 客户端。
   - 如果 {IDF_TARGET_NAME} Bluetooth LE 客户端端没有先开启 Bluetooth LE SPP 透传，或者使用其他设备作为 Bluetooth LE 客户端，则 {IDF_TARGET_NAME} Bluetooth LE 客户端需要先开启侦听 Notify 或者 Indicate。例如，{IDF_TARGET_NAME} Bluetooth LE 客户端如果未开启透传，则应先调用 `AT+BLEGATTCWR=0,3,7,1,1` 开启侦听，{IDF_TARGET_NAME} Bluetooth LE 服务端 才能成功实现透传。
   - 对于同一服务，{IDF_TARGET_NAME} Bluetooth LE 客户端的 <srv_index> 值等于 {IDF_TARGET_NAME} Bluetooth LE 服务端的 <srv_index> 值 + 2，这是正常现象。

{IDF_TARGET_NAME} 与手机建立 SPP 连接，以及在 UART-Bluetooth LE 透传模式下传输数据 
--------------------------------------------------------------------------------------

该示例展示了如何在 {IDF_TARGET_NAME} 开发板（仅作为低功耗蓝牙服务器角色）和手机（仅作为低功耗蓝牙客户端角色）之间建立 SPP 连接，以及如何在 UART-Bluetooth LE 透传模式下传输数据。

.. Important::
  步骤中以 ``{IDF_TARGET_NAME} Bluetooth LE 服务端`` 开头的操作只需要在 {IDF_TARGET_NAME} Bluetooth LE 服务端执行即可，而以 ``Bluetooth LE 客户端`` 开头的操作只需要在手机的蓝牙调试助手中执行即可。

#. 在手机端下载 Bluetooth LE 调试助手，例如 LightBlue。

#. 初始化 Bluetooth LE 功能。

   {IDF_TARGET_NAME} Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLEINIT=2

   响应：

   .. code-block:: none

     OK

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE 服务端创建服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSRVCRE
  
     响应：
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端开启服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSRVSTART
  
     响应：
  
     .. code-block:: none
  
       OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端获取其 MAC 地址。

   命令：

   .. code-block:: none

     AT+BLEADDR?

   响应：

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   说明：

   - 您查询到的地址可能与上述响应中的不同，请记住您的地址，下面的步骤中会用到。

#. {IDF_TARGET_NAME} Bluetooth LE 服务端设置广播参数。

   命令：

   .. code-block:: none

     AT+BLEADVPARAM=50,50,0,0,7,0,,

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端设置广播数据。

   命令：

   .. code-block:: none

     AT+BLEADVDATA="0201060A09457370726573736966030302A0"

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端开始广播。

   命令：

   .. code-block:: none

     AT+BLEADVSTART

   响应：

   .. code-block:: none

     OK

#. 创建 Bluetooth LE 连接。

   手机打开 LightBlue 应用程序，并打开 SCAN 开始扫描，找到 {IDF_TARGET_NAME} Bluetooth LE 服务端的 MAC 地址，点击 ``CONNECT`` 进行连接。此时 {IDF_TARGET_NAME} 端应该会打印类似于 ``+BLECONN:0,"60:51:42:fe:98:aa"`` 的日志，这表示已经建立了 Bluetooth LE 连接。

#. {IDF_TARGET_NAME} Bluetooth LE 服务端查询服务。

   命令：

   .. code-block:: none

     AT+BLEGATTSSRV?

   响应：

   .. code-block:: none

     +BLEGATTSSRV:1,1,0xA002,1
     +BLEGATTSSRV:2,1,0xA003,1
     
     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端发现特征。

   命令：

   .. code-block:: none

     AT+BLEGATTSCHAR?

   响应：

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300,0x02
       +BLEGATTSCHAR:"desc",1,1,1,0x2901
       +BLEGATTSCHAR:"char",1,2,0xC301,0x02
       +BLEGATTSCHAR:"desc",1,2,1,0x2901
       +BLEGATTSCHAR:"char",1,3,0xC302,0x08
       +BLEGATTSCHAR:"desc",1,3,1,0x2901
       +BLEGATTSCHAR:"char",1,4,0xC303,0x04
       +BLEGATTSCHAR:"desc",1,4,1,0x2901
       +BLEGATTSCHAR:"char",1,5,0xC304,0x08
       +BLEGATTSCHAR:"char",1,6,0xC305,0x10
       +BLEGATTSCHAR:"desc",1,6,1,0x2902
       +BLEGATTSCHAR:"char",1,7,0xC306,0x20
       +BLEGATTSCHAR:"desc",1,7,1,0x2902
       +BLEGATTSCHAR:"char",1,8,0xC307,0x02
       +BLEGATTSCHAR:"desc",1,8,1,0x2901
       +BLEGATTSCHAR:"char",2,1,0xC400,0x02
       +BLEGATTSCHAR:"desc",2,1,1,0x2901
       +BLEGATTSCHAR:"char",2,2,0xC401,0x02
       +BLEGATTSCHAR:"desc",2,2,1,0x2901

       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300
       +BLEGATTSCHAR:"char",1,2,0xC301
       +BLEGATTSCHAR:"char",1,3,0xC302
       +BLEGATTSCHAR:"char",1,4,0xC303
       +BLEGATTSCHAR:"char",1,5,0xC304
       +BLEGATTSCHAR:"char",1,6,0xC305
       +BLEGATTSCHAR:"char",1,7,0xC306
       +BLEGATTSCHAR:"char",1,8,0xC307
       +BLEGATTSCHAR:"char",2,1,0xC400
       +BLEGATTSCHAR:"char",2,2,0xC401

       OK

#. Bluetooth LE 客户端发现特征。

   此时在手机 LightBlue 客户端选择点击 Properties 为 NOTIFY 或者 INDICATE 的服务特征（这里 ESP-AT 默认 Properties 为 NOTIFY 或者 INDICATE 的服务特征是 0xC305 和 0xC306），开始侦听 Properties 为 NOTIFY 或者 INDICATE 的服务特征。

#. {IDF_TARGET_NAME} Bluetooth LE 服务端配置 Bluetooth LE SPP。

   选择支持 notify 或者 indicate 的 characteristic 作为写通道发送数据，选择支持写操作的 characteristic 作为读通道接收数据。

   命令：

   .. code-block:: none

     AT+BLESPPCFG=1,1,7,1,5

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端使能 Bluetooth LE SPP。

   命令：

   .. code-block:: none

     AT+BLESPP

   响应：

   .. code-block:: none

     OK

     >

   上述响应表示 AT 已经进入 Bluetooth LE SPP 模式，可以进行数据的发送和接收。

#. Bluetooth LE 客户端发送数据。

   在 LightBlue 客户端选择 0xC304 服务特征值发送数据 ``test`` 给 {IDF_TARGET_NAME} Bluetooth LE 服务端，此时 {IDF_TARGET_NAME} Bluetooth LE 服务端可以收到 ``test``。

#. {IDF_TARGET_NAME} Bluetooth LE 服务端发送数据。
    
   在 {IDF_TARGET_NAME} Bluetooth LE 服务端直接发送 ``test``，此时 LightBlue 客户端可以收到 ``test``。

{IDF_TARGET_NAME} 和手机之间建立 Bluetooth LE 连接并配对
--------------------------------------------------------------------------------------

该示例展示了如何在 {IDF_TARGET_NAME} 开发板（仅作为低功耗蓝牙服务器角色）和手机（仅作为低功耗蓝牙客户端角色）之间建立 Bluetooth LE 连接并输入密钥完成配对。

.. Important::
  步骤中以 ``{IDF_TARGET_NAME} Bluetooth LE 服务端`` 开头的操作只需要在 {IDF_TARGET_NAME} Bluetooth LE 服务端执行即可，而以 ``Bluetooth LE 客户端`` 开头的操作只需要在手机的蓝牙调试助手中执行即可。 

#. 在手机端下载 Bluetooth LE 调试助手，例如 LightBlue 应用程序。

#. 初始化 Bluetooth LE 功能。

   {IDF_TARGET_NAME} Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLEINIT=2

   响应：

   .. code-block:: none

     OK

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE 服务端创建服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSRVCRE
  
     响应：
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE 服务端开启服务。
  
     命令：
  
     .. code-block:: none
  
       AT+BLEGATTSSRVSTART
  
     响应：
  
     .. code-block:: none
  
       OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端获取其 MAC 地址。

   命令：

   .. code-block:: none

     AT+BLEADDR?

   响应：

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   说明：

   - 您查询到的地址可能与上述响应中的不同，请记住您的地址，下面的步骤中会用到。

#. {IDF_TARGET_NAME} Bluetooth LE 服务端设置广播参数。

   命令：

   .. code-block:: none

     AT+BLEADVPARAM=50,50,0,0,7,0,,

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端设置广播数据。

   命令：

   .. code-block:: none

     AT+BLEADVDATA="0201060A09457370726573736966030302A0"

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端设置加密参数。

   命令：

   .. code-block:: none

     AT+BLESECPARAM=13,2,16,3,3

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE 服务端开始广播。

   命令：

   .. code-block:: none

     AT+BLEADVSTART

   响应：

   .. code-block:: none

     OK

#. Bluetooth LE 客户端创建连接。

   手机打开 LightBlue 应用程序，并打开 SCAN 开始扫描，找到 {IDF_TARGET_NAME} Bluetooth LE 服务端的 MAC 地址，点击 ``CONNECT`` 进行连接。此时 {IDF_TARGET_NAME} 端应该会打印类似于 ``+BLECONN:0,"60:51:42:fe:98:aa"`` 的日志，这表示已经建立了 Bluetooth LE 连接。

#. {IDF_TARGET_NAME} Bluetooth LE 服务端发起加密请求。

   命令：

   .. code-block:: none

     AT+BLEENC=0,3

   响应：

   .. code-block:: none

     OK

#. Bluetooth LE 客户端同意配对。

   手机 LightBlue 应用程序刚刚创建成功的 Bluetooth LE 连接的页面会弹出配对信息（包含配对密钥，例如密钥为：231518），点击 ``配对``。此时 {IDF_TARGET_NAME} 端应该会打印类似于 ``+BLESECKEYREQ:0`` 的日志，这表示手机已经响应配对。

#. {IDF_TARGET_NAME} Bluetooth LE 服务端回复配对密钥。

   此时 Bluetooth LE 服务端回复的密钥即为上一步骤中手机 LightBlue 应用程序 弹出的配对信息中的密钥：231518。

   命令：

   .. code-block:: none

     AT+BLEKEYREPLY=0,231518

   响应：

   .. code-block:: none

     OK
  
   此时 {IDF_TARGET_NAME} Bluetooth LE 服务端会打印类似于以下日志，这表示 {IDF_TARGET_NAME} Bluetooth LE 服务端与手机 Bluetooth LE 客户端完成了配对。

   .. code-block:: none

     +BLESECKEYTYPE:0,16
     +BLESECKEYTYPE:0,1
     +BLESECKEYTYPE:0,32
     +BLESECKEYTYPE:0,2
     +BLEAUTHCMPL:0,0
