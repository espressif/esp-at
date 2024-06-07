{IDF_TARGET_NAME} Ethernet AT 示例
======================================

:link_to_translation:`en:[English]`

本文档主要介绍 :doc:`../AT_Command_Set/Ethernet_AT_Commands` 的使用方法，并提供在 {IDF_TARGET_NAME} 设备上运行这些命令的详细示例。

.. contents::
   :local:
   :depth: 1

.. Important::
  - 在使用 Ethernet AT 命令之前，请先阅读 :ref:`准备工作 <cmd-ETHPRE>`。
  - 文档中所描述的例子均是基于网线已经插入的情况下。

基于以太网创建 TCP 连接
---------------------------------------------------------------

#. 使能多连接。

   命令：

   .. code-block:: none

     AT+CIPMUX=1

   响应：

   .. code-block:: none

     OK

#. 建立 TCP 服务器。

   命令：

   .. code-block:: none

     AT+CIPSERVER=1,8081

   响应：

   .. code-block:: none

     OK

#. 获取 TCP 服务器的 IP 地址。

   命令：

   .. code-block:: none

     AT+CIPETH?

   响应：

   .. code-block:: none

     +CIPETH:ip:192.168.105.24
     +CIPETH:gateway:192.168.105.1
     +CIPETH:netmask:255.255.255.0
     OK

   说明：

   - 您获取到的地址可能与上述响应中的不同。

#. 在 PC 端使用网络调试工具创建一个 TCP 客户端，并连接到步骤 2 中创建的 TCP 服务端，IP 地址是 ``192.168.105.24``，端口为 ``8081``。

#. 采用 :term:`普通传输模式` 发送 4 字节数据到网络连接 ID 为 0 的链路上。

   命令：

   .. code-block:: none

     AT+CIPSEND=0,4

   响应：

   .. code-block:: none

     OK

     >

   输入 4 字节数据，例如输入数据是 ``test``，之后 AT 将会输出以下信息。

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   说明：

   - 若输入的字节数目超过了 ``AT+CIPSEND`` 命令设定的长度 (n)，则会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``SEND OK``。

#. 采用 :term:`普通传输模式` 从网络连接 ID 为 0 的链路上接收 4 字节数据。

   假设 TCP server 接收到 4 字节的数据 （数据为 ``test``），则系统会提示：

   .. code-block:: none

      +IPD,0,4:test

#. 关闭 TCP 连接。

   命令：

   .. code-block:: none

     AT+CIPCLOSE=0

   响应：

   .. code-block:: none

     0,CLOSED

     OK

#. 删除 TCP 服务端。

   命令：

   .. code-block:: none

     AT+CIPSERVER=0

   响应：

   .. code-block:: none

     OK

   说明：

   - 命令 ``AT+CIPSERVER=0`` 只会关闭服务器，但会保留现有客户端连接。如果您想同时关闭所有的客户端连接，请执行命令 ``AT+CIPSERVER=0,1``。
   