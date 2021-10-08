Bluetooth LE AT 示例
=================================

:link_to_translation:`en:[English]`

本文档主要介绍 :doc:`../AT_Command_Set/BLE_AT_Commands` 的使用方法，并提供在 ESP 设备上运行这些命令的详细示例。

.. contents::
   :local:
   :depth: 1

Bluetooth LE AT 示例
------------------------

以下示例同时使用两块 ESP32 开发板，其中一块作为 Bluetooth LE 服务端（只作为 Bluetooth LE 服务端角色），另一块作为 Bluetooth LE 客户端（只作为 Bluetooth LE 客户端角色）。这个例子展示了应如何使用 AT 命令建立 Bluetooth LE 连接，完成数据通信。

.. Important::
  * ESP32 Bluetooth LE 服户端需烧录 ``ble_data.bin`` 文件到 Flash 中，用以提供 Bluetooth LE 服务。

    * 如何生成 ``ble_data.bin`` 文件，请参考 ``tools/README.md`` 。
    * ``ble_data.bin`` 文件的烧录地址，见 ``at_customize.csv`` 中 ``ble_data`` 对应的地址。

  * 步骤中以 ``ESP32 Bluetooth LE 服务端`` 开头的操作只需要在 ESP32 Bluetooth LE 服务端执行即可，以 ``ESP32 Bluetooth LE 客户端`` 开头的操作只需要在 ESP32 Bluetooth LE 客户端执行即可。

#. 初始化 Bluetooth LE 功能。

   ESP32 Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLEINIT=2

   响应：
  
   .. code-block:: none

     OK

   ESP32 Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLEINIT=1

   响应：
  
   .. code-block:: none

     OK

#. ESP32 Bluetooth LE 服务端获取 Bluetooth LE 地址。

   命令：

   .. code-block:: none

     AT+BLEADDR?

   响应：

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   说明：

   - 您查询到的地址可能与上述响应中的不同，请记住您的地址，下面的步骤中会用到。

#. ESP32 Bluetooth LE 服务端创建服务。

   命令：

   .. code-block:: none

     AT+BLEGATTSSRVCRE

   响应：

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE 服务端开启服务。

   命令：

   .. code-block:: none

     AT+BLEGATTSSRVSTART

   响应：

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE 服务端发现服务特征。

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

#. ESP32 Bluetooth LE 服务端开始广播，之后 ESP32 Bluetooth LE 客户端开始扫描并且持续 3 秒钟。

   ESP32 Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLEADVSTART

   响应：

   .. code-block:: none

     OK

   ESP32 Bluetooth LE 客户端：

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

   ESP32 Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLECONN=0,"24:0a:c4:d6:e4:46"

   响应：

   .. code-block:: none

     +BLECONN:0,"24:0a:c4:d6:e4:46"
     
     OK

   说明：

   - 输入上述命令时，请使用您的 ESP Bluetooth LE 服务端地址。
   - 如果 Bluetooth LE 连接成功，则会提示 ``+BLECONN:0,"24:0a:c4:d6:e4:46``。
   - 如果 Bluetooth LE 连接失败，则会提示 ``+BLECONN:0,-1``。

#. ESP32 Bluetooth LE 客户端发现服务。

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

   - ESP32 Bluetooth LE 客户端查询服务的结果，比 ESP32 Bluetooth LE 服户端查询服务的结果多两个默认服务（UUID: 0x1800 和 0x1801），这是正常现象。
   - 正因如此，对于同一服务，ESP32 Bluetooth LE 客户端查询的 <srv_index> 值等于 ESP32 Bluetooth LE 服户端查询的 <srv_index> 值 + 2。
   - 例如上述示例中的服务 0xA002，当前在 ESP32 Bluetooth LE 客户端查询到的 <srv_index> 为 3，如果在 ESP32 Bluetooth LE 服户端通过 :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` 命令查询，则 <srv_index> 为 1。

#. ESP32 Bluetooth LE 客户端发现特征值。

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

#. ESP32 Bluetooth LE 客户端读取一个特征值。

   命令：

   .. code-block:: none

     AT+BLEGATTCRD=0,3,1

   响应：

   .. code-block:: none

     +BLEGATTCRD:0,1,0

     OK

   说明：

   - 请注意目标特征值必须要有读权限。
   - 如果 ESP32 Bluetooth LE 客户端读取特征成功，ESP32 Bluetooth LE 服务端则会提示 ``+READ:0,"7c:df:a1:b3:8d:de"``。

#. ESP32 Bluetooth LE 客户端写一个特征值。

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

   - 如果 ESP32 Bluetooth LE 客户端写特征描述符成功，ESP32 Bluetooth LE 服务端则会提示 ``+WRITE:<conn_index>,<srv_index>,<char_index>,[<desc_index>],<len>,<value>``。

#. Indicate 一个特征值。

   ESP32 Bluetooth LE 服务端：

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

   - 如果 ESP32 Bluetooth LE 客户端接收到 indication, 则会提示 ``+INDICATE:<conn_index>,<srv_index>,<char_index>,<len>,<value>``。
   - 对于同一服务，ESP32 Bluetooth LE 客户端的 <srv_index> 值等于 ESP32 Bluetooth LE 服户端的 <srv_index> 值 + 2，这是正常现象。

   ESP32 Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLEGATTCWR=0,3,7,1,2 

   响应：

   .. code-block:: none

     >

   符号 ``>`` 表示 AT 准备好接收串口数据，此时您可以输入数据，当数据长度达到参数 ``<length>`` 的值时，执行写入操作。

   .. code-block:: none

     OK

   说明：

   - 如果 ESP32 Bluetooth LE 客户端写特征描述符成功，ESP32 Bluetooth LE 服务端则会提示 ``+WRITE:<conn_index>,<srv_index>,<char_index>,<desc_index>,<len>,<value>``。

UART-Bluetooth LE 透传模式示例
-----------------------------------------------

以下示例同时使用两块 ESP32 开发板，其中一块作为 Bluetooth LE 服务端（只作为 Bluetooth LE 服务端角色），另一块作为 Bluetooth LE 客户端（只作为 Bluetooth LE 客户端角色）。这个例子展示了应如何建立 Bluetooth LE 连接，以及建立透传通信 Bluetooth LE SPP (Serial Port Profile, UART-Bluetooth LE 透传模式)。

.. Important::
  步骤中以 ``ESP32 Bluetooth LE 服务端`` 开头的操作只需要在 ESP32 Bluetooth LE 服务端执行即可，以 ``ESP32 Bluetooth LE 客户端`` 开头的操作只需要在 ESP32 Bluetooth LE 客户端执行即可。

#. 初始化 Bluetooth LE 功能。

   ESP32 Bluetooth LE 服务端：

   命令：

   .. code-block:: none

     AT+BLEINIT=2

   响应：

   .. code-block:: none

     OK

   ESP32 Bluetooth LE 客户端：
  
   命令：

   .. code-block:: none

     AT+BLEINIT=1

   响应：

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE 服务端创建服务。

   命令：

   .. code-block:: none

     AT+BLEGATTSSRVCRE

   响应：

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE 服务端开启服务。

   命令：

   .. code-block:: none

     AT+BLEGATTSSRVSTART

   响应：

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE 服务端获取 Bluetooth LE 地址。

   命令：

   .. code-block:: none

     AT+BLEADDR?

   响应：

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   说明：

   - 您查询到的地址可能与上述响应中的不同，请记住您的地址，下面的步骤中会用到。

#. 设置 Bluetooth LE 广播数据。

   命令：

   .. code-block:: none

     AT+BLEADVDATA="0201060A09457370726573736966030302A0"

   响应：

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE 服务端开始广播。

   命令：

   .. code-block:: none

     AT+BLEADVSTART

   响应：

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE 客户端开始扫描，持续 3 秒。

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

   ESP32 Bluetooth LE 客户端：

   命令：

   .. code-block:: none

     AT+BLECONN=0,"24:0a:c4:d6:e4:46"

   响应：

   .. code-block:: none

     +BLECONN:0,"24:0a:c4:d6:e4:46"
     
     OK

   说明：

   - 输入上述命令时，请使用您的 ESP Bluetooth LE 服务端地址。
   - 如果 Bluetooth LE 连接成功，则会提示 ``+BLECONN:0,"24:0a:c4:d6:e4:46``。
   - 如果 Bluetooth LE 连接失败，则会提示 ``+BLECONN:0,-1``。

#. ESP32 Bluetooth LE 服务端查询服务。

   命令：

   .. code-block:: none

     AT+BLEGATTSSRV?

   响应：

   .. code-block:: none

     +BLEGATTSSRV:1,1,0xA002,1
     +BLEGATTSSRV:2,1,0xA003,1
     
     OK

#. ESP32 Bluetooth LE 服务端发现特征。

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

#. ESP32 Bluetooth LE 客户端发现服务。

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

   - ESP32 Bluetooth LE 客户端查询服务的结果，比 ESP32 Bluetooth LE 服户端查询服务的结果多两个默认服务（UUID: 0x1800 和 0x1801），这是正常现象。
   - 正因如此，对于同一服务，ESP32 Bluetooth LE 客户端查询的 <srv_index> 值等于 ESP32 Bluetooth LE 服户端查询的 <srv_index> 值 + 2。
   - 例如，上述示例中的服务 0xA002，当前在 ESP32 Bluetooth LE 客户端查询到的 <srv_index> 为 3，如果在 ESP32 Bluetooth LE 服户端通过 :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` 命令查询，则 <srv_index> 为 1。

#. ESP32 Bluetooth LE 客户端发现特征。

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

#. ESP32 Bluetooth LE 客户端配置 Bluetooth LE SPP。

   选择支持写操作的服务特征（characteristic）作为写通道发送数据，选择支持 notify 或者 indicate 的 characteristic 作为读通道接收数据。

   命令：

   .. code-block:: none

     AT+BLESPPCFG=1,3,5,3,7

   响应：

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE 客户端使能 Bluetooth LE SPP。

   命令：

   .. code-block:: none

     AT+BLESPP

   响应：

   .. code-block:: none

     OK

     >

   上述响应表示 AT 已经进入 Bluetooth LE SPP 模式，可以进行数据的发送和接收。

   说明：

   - ESP32 Bluetooth LE 客户端开启 Bluetooth LE SPP 透传模式后，串口收到的数据会通过 Bluetooth LE 传输到 ESP32 Bluetooth LE 服务端。

#. ESP32 Bluetooth LE 服务端配置 Bluetooth LE SPP。

   选择支持 notify 或者 indicate 的 characteristic 作为写通道发送数据，选择支持写操作的 characteristic 作为读通道接收数据。

   命令：

   .. code-block:: none

     AT+BLESPPCFG=1,1,7,1,5

   响应：

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE 服务端使能 Bluetooth LE SPP。

   命令：

   .. code-block:: none

     AT+BLESPP

   响应：

   .. code-block:: none

     OK

     >

   上述响应表示 AT 已经进入 Bluetooth LE SPP 模式，可以进行数据的发送和接收。

   说明：

   - ESP32 Bluetooth LE 服户端开启 Bluetooth LE SPP 透传模式后，串口收到的数据会通过 Bluetooth LE 传输到 ESP32 Bluetooth LE 客户端。
   - 如果 ESP32 Bluetooth LE 客户端端没有先开启 Bluetooth LE SPP 透传，或者使用其他设备作为 Bluetooth LE 客户端，则 ESP32 Bluetooth LE 客户端需要先开启侦听 Notify 或者 Indicate。例如，ESP32 Bluetooth LE 客户端如果未开启透传，则应先调用 `AT+BLEGATTCWR=0,3,7,1,1` 开启侦听，ESP32 Bluetooth LE 服务端 才能成功实现透传。
   - 对于同一服务，ESP32 Bluetooth LE 客户端的 <srv_index> 值等于 ESP32 Bluetooth LE 服务端的 <srv_index> 值 + 2，这是正常现象。
