.. _TCPIP-AT:

TCP/IP AT 命令
==================

:link_to_translation:`en:[English]`

-  :ref:`介绍 <cmd-tcpip-intro>`
-  :ref:`AT+CIPV6 <cmd-IPV6>`: 启用/禁用 IPv6 网络 (IPv6)
-  :ref:`AT+CIPSTATE <cmd-IPSTATE>`：查询 TCP/UDP/SSL 连接信息
-  :ref:`AT+CIPSTATUS (弃用) <cmd-STATUS>`：查询 TCP/UDP/SSL 连接状态和信息
-  :ref:`AT+CIPDOMAIN <cmd-DOMAIN>`：域名解析
-  :ref:`AT+CIPSTART <cmd-START>`：建立 TCP 连接、UDP 传输或 SSL 连接
-  :ref:`AT+CIPSTARTEX <cmd-STARTEX>`：建立自动分配 ID 的 TCP 连接、UDP 传输或 SSL 连接
-  :ref:`[仅适用数据模式] +++ <cmd-PLUS>`: 退出 :term:`数据模式`
-  :ref:`AT+SAVETRANSLINK <cmd-SAVET>`：设置 Wi-Fi 开机 :term:`透传模式` 信息
-  :ref:`AT+CIPSEND <cmd-SEND>`：在 :term:`普通传输模式` 或 Wi-Fi :term:`透传模式` 下发送数据
-  :ref:`AT+CIPSENDL <cmd-SENDL>`：在 :term:`普通传输模式` 下并行发送长数据
-  :ref:`AT+CIPSENDLCFG <cmd-SENDLCFG>`：设置 :ref:`AT+CIPSENDL <cmd-SENDL>` 命令的属性
-  :ref:`AT+CIPSENDEX <cmd-SENDEX>`：在 :term:`普通传输模式` 下采用扩展的方式发送数据
-  :ref:`AT+CIPCLOSE <cmd-CLOSE>`：关闭 TCP/UDP/SSL 连接
-  :ref:`AT+CIFSR <cmd-IFSR>`：查询本地 IP 地址和 MAC 地址
-  :ref:`AT+CIPMUX <cmd-MUX>`：启用/禁用多连接模式
-  :ref:`AT+CIPSERVER <cmd-SERVER>`：建立/关闭 TCP 或 SSL 服务器
-  :ref:`AT+CIPSERVERMAXCONN <cmd-SERVERMAX>`：查询/设置服务器允许建立的最大连接数
-  :ref:`AT+CIPMODE <cmd-IPMODE>`：查询/设置传输模式
-  :ref:`AT+CIPSTO <cmd-STO>`：查询/设置本地 TCP 服务器超时时间
-  :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>`：查询/设置时区和 SNTP 服务器
-  :ref:`AT+CIPSNTPTIME <cmd-SNTPT>`：查询 SNTP 时间
-  :ref:`AT+CIPSNTPINTV <cmd-SNTPINTV>`：查询/设置 SNTP 时间同步的间隔
-  :ref:`AT+CIPFWVER <cmd-FWVER>`：查询服务器已有的 AT 固件版本
-  :ref:`AT+CIUPDATE <cmd-UPDATE>`：通过 Wi-Fi 升级固件
-  :ref:`AT+CIPDINFO <cmd-IPDINFO>`：设置 +IPD 消息详情
-  :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`：查询/设置 SSL 客户端配置
-  :ref:`AT+CIPSSLCCN <cmd-SSLCCN>`：查询/设置 SSL 客户端的公用名 (common name)
-  :ref:`AT+CIPSSLCSNI <cmd-SSLCSNI>`：查询/设置 SSL 客户端的 SNI
-  :ref:`AT+CIPSSLCALPN <cmd-SSLCALPN>`：查询/设置 SSL 客户端 ALPN
-  :ref:`AT+CIPSSLCPSK <cmd-SSLCPSK>`：查询/设置 SSL 客户端的 PSK (字符串格式)
-  :ref:`AT+CIPSSLCPSKHEX <cmd-SSLCPSKHEX>`：查询/设置 SSL 客户端的 PSK (十六进制格式)
-  :ref:`AT+CIPRECONNINTV <cmd-AUTOCONNINT>`：查询/设置 Wi-Fi :term:`透传模式` 下的 TCP/UDP/SSL 重连间隔
-  :ref:`AT+CIPRECVTYPE <cmd-CIPRECVTYPE>`：查询/设置套接字接收模式
-  :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>`：获取被动接收模式下的套接字数据
-  :ref:`AT+CIPRECVLEN <cmd-CIPRECVLEN>`：查询被动接收模式下套接字数据的长度
-  :ref:`AT+PING <cmd-CIPPING>`：ping 对端主机
-  :ref:`AT+CIPDNS <cmd-DNS>`：查询/设置 DNS 服务器信息
-  :ref:`AT+MDNS <cmd-MDNS>`：设置 mDNS 功能
-  :ref:`AT+CIPTCPOPT <cmd-TCPOPT>`：查询/设置套接字选项

.. _cmd-tcpip-intro:

介绍
------

.. important::
  默认的 AT 固件支持此页面下的所有 AT 命令。如果您需要修改 {IDF_TARGET_NAME} 默认支持的命令，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择（下面每项是独立的，根据您的需要选择）：

  - 禁用 OTA 命令（:ref:`AT+CIUPDATE <cmd-UPDATE>`、:ref:`AT+CIPFWVER <cmd-FWVER>`）：``Component config`` -> ``AT`` -> ``AT OTA command support``
  - 禁用 PING 命令（:ref:`AT+PING <cmd-CIPPING>`）：``Component config`` -> ``AT`` -> ``AT ping command support``
  - 禁用 mDNS 命令（:ref:`AT+MDNS <cmd-MDNS>`）：``Component config`` -> ``AT`` -> ``AT MDNS command support``
  - 禁用 TCP/IP 命令（不推荐。一旦禁用，所有 TCP/IP 功能将无法使用，您需要自行实现这些 AT 命令）： ``Component config`` -> ``AT`` -> ``AT net command support``

.. _cmd-IPV6:

:ref:`AT+CIPV6 <TCPIP-AT>`: 启用/禁用 IPv6 网络 (IPv6)
------------------------------------------------------------------

查询命令
^^^^^^^^^^^^

**功能：**

查询 IPv6 网络是否使能

**命令：**

::

    AT+CIPV6?

**响应：**

::

    +CIPV6:<enable>

    OK

设置命令
^^^^^^^^^^^^^^

**功能：**

启用/禁用 IPv6 网络

**命令：**

::

    AT+CIPV6=<enable>

**响应：**

::

    OK

参数
^^^^

-  **<enable>**: IPv6 网络使能状态。 默认值：0

   -  0: 禁用 IPv6 网络
   -  1: 启用 IPv6 网络

说明
^^^^

-  在使用基于 IPv6 网络的上层应用前，需要先启用 IPv6 网络。（例如：基于 IPv6 网络使用 TCP/UDP/SSL/PING/DNS，也称为 TCP6/UDP6/SSL6/PING6/DNS6 或 TCPv6/UDPv6/SSLv6/PINGv6/DNSv6）

.. _cmd-IPSTATE:

:ref:`AT+CIPSTATE <TCPIP-AT>`：查询 TCP/UDP/SSL 连接信息
-----------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**命令：**

::

    AT+CIPSTATE?

**响应：**

当有连接时，AT 返回：

::

    +CIPSTATE:<link ID>,<"type">,<"remote IP">,<remote port>,<local port>,<tetype>

    OK

当没有连接时，AT 返回：

::

    OK

参数
^^^^

-  **<link ID>**：网络连接 ID (0 ~ 4)，用于多连接的情况
-  **<"type">**：字符串参数，表示传输类型："TCP"、"UDP"、"SSL"、"TCPv6"、"UDPv6" 或 "SSLv6"
-  **<"remote IP">**：字符串参数，表示远端 IPv4 地址或 IPv6 地址
-  **<remote port>**：远端端口值
-  **<local port>**：{IDF_TARGET_NAME} 本地端口值
-  **<tetype>**:

   -  0: {IDF_TARGET_NAME} 设备作为客户端
   -  1: {IDF_TARGET_NAME} 设备作为服务器

.. _cmd-STATUS:

:ref:`AT+CIPSTATUS (弃用) <TCPIP-AT>`：查询 TCP/UDP/SSL 连接状态和信息
-----------------------------------------------------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    AT+CIPSTATUS

**响应：**

::

    STATUS:<stat>
    +CIPSTATUS:<link ID>,<"type">,<"remote IP">,<remote port>,<local port>,<tetype>
    OK

参数
^^^^

-  **<stat>**：{IDF_TARGET_NAME} station 接⼝的状态

   -  0: {IDF_TARGET_NAME} station 为未初始化状态
   -  1: {IDF_TARGET_NAME} station 为已初始化状态，但还未开始 Wi-Fi 连接
   -  2: {IDF_TARGET_NAME} station 已连接 AP，获得 IP 地址
   -  3: {IDF_TARGET_NAME} station 已建立 TCP、UDP 或 SSL 传输
   -  4: {IDF_TARGET_NAME} 设备所有的 TCP、UDP 和 SSL 均断开
   -  5: {IDF_TARGET_NAME} station 开始过 Wi-Fi 连接，但尚未连接上 AP 或从 AP 断开

-  **<link ID>**：网络连接 ID (0 ~ 4)，用于多连接的情况
-  **<"type">**：字符串参数，表示传输类型："TCP"、"UDP"、"SSL"、"TCPv6"、"UDPv6" 或 "SSLv6"
-  **<"remote IP">**：字符串参数，表示远端 IPv4 地址或 IPv6 地址
-  **<remote port>**：远端端口值
-  **<local port>**：{IDF_TARGET_NAME} 本地端口值
-  **<tetype>**:

   -  0: {IDF_TARGET_NAME} 设备作为客户端
   -  1: {IDF_TARGET_NAME} 设备作为服务器

说明
^^^^

- 建议您使用 :ref:`AT+CWSTATE <cmd-WSTATE>` 命令查询 Wi-Fi 状态，使用 :ref:`AT+CIPSTATE <cmd-IPSTATE>` 命令查询 TCP/UDP/SSL 状态。

.. _cmd-DOMAIN:

:ref:`AT+CIPDOMAIN <TCPIP-AT>`：域名解析
------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+CIPDOMAIN=<"domain name">[,<ip network>]

**响应：**

::

    +CIPDOMAIN:<"IP address">

    OK

参数
^^^^

-  **<"domain name">**：待解析的域名
-  **<ip network>**：首选 IP 网络。默认值：1

   - 1：首选解析为 IPv4 地址
   - 2：只解析为 IPv4 地址
   - 3：只解析为 IPv6 地址

-  **<"IP address">**：解析出的 IP 地址

示例
^^^^

::

    AT+CWMODE=1                       // 设置 station 模式
    AT+CWJAP="SSID","password"        // 连接网络
    AT+CIPDOMAIN="iot.espressif.cn"   // 域名解析

    // 域名解析，只解析为 IPv4 地址
    AT+CIPDOMAIN="iot.espressif.cn",2

    // 域名解析，只解析为 IPv6 地址
    AT+CIPDOMAIN="ipv6.test-ipv6.com",3

    // 域名解析，首选解析为 IPv4 地址
    AT+CIPDOMAIN="ds.test-ipv6.com",1

.. _cmd-START:

:ref:`AT+CIPSTART <TCPIP-AT>`：建立 TCP 连接、UDP 传输或 SSL 连接
------------------------------------------------------------------------------------------------------

* :ref:`esp-at-cipstart-tcp`
* :ref:`esp-at-cipstart-udp`
* :ref:`esp-at-cipstart-ssl`

.. _esp-at-cipstart-tcp:

建立 TCP 连接
^^^^^^^^^^^^^^^^^^^^^^^^

设置命令
""""""""""""""

**命令：**

::

    // 单连接 (AT+CIPMUX=0):
    AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<keep_alive>][,<"local IP">]

    // 多连接 (AT+CIPMUX=1):
    AT+CIPSTART=<link ID>,<"type">,<"remote host">,<remote port>[,<keep_alive>][,<"local IP">]

**响应：**

单连接模式下，返回：

::

    CONNECT

    OK

多连接模式下，返回：

::

    <link ID>,CONNECT

    OK

参数
""""

-  **<link ID>**：网络连接 ID (0 ~ 4)，用于多连接的情况。该参数范围取决于 ``menuconfig`` 中的两个配置项。一个是 ``AT`` 组件中的配置项 ``AT_SOCKET_MAX_CONN_NUM`` ，默认值为 5。另一个是 ``LWIP`` 组件中的配置项 ``LWIP_MAX_SOCKETS`` ，默认值为 10。要修改该参数的范围，您需要修改配置项 ``AT_SOCKET_MAX_CONN_NUM`` 的值并确保该值不大于 ``LWIP_MAX_SOCKETS`` 的值。（请参考 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>` 获取更多信息。）
-  **<"type">**：字符串参数，表示网络连接类型，"TCP" 或 "TCPv6"。默认值："TCP"
-  **<"remote host">**：字符串参数，表示远端 IPv4 地址、IPv6 地址，或域名。最长为 64 字节。如果您需要使用域名且域名长度超过 64 字节，请使用 :ref:`AT+CIPDOMAIN <cmd-DOMAIN>` 命令获取域名对应的 IP 地址，然后使用 IP 地址建立连接。
-  **<remote port>**：远端端口值
-  **<keep_alive>**：配置套接字的 ``SO_KEEPALIVE`` 选项（参考：`SO_KEEPALIVE 介绍 <https://man7.org/linux/man-pages/man7/socket.7.html#SO_KEEPALIVE>`_），单位：秒。

  - 范围：[0,7200]。

    - 0：禁用 keep-alive 功能；（默认）
    - 1 ~ 7200：开启 keep-alive 功能。`TCP_KEEPIDLE <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPIDLE>`_ 值为 **<keep_alive>**，`TCP_KEEPINTVL <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPINTVL>`_ 值为 1，`TCP_KEEPCNT <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPCNT>`_ 值为 3。

  -  本命令中的 ``<keep_alive>`` 参数与 :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` 命令中的 ``<keep_alive>`` 参数相同，最终值由后设置的命令决定。如果运行本命令时不设置 ``<keep_alive>`` 参数，则默认使用上次配置的值。

-  **<"local IP">**：连接绑定的本机 IPv4 地址或 IPv6 地址，该参数在本地多网络接口时和本地多 IP 地址时非常有用。默认为禁用，如果您想使用，需自行设置，空值也为有效值

说明
""""""

- 如果您想基于 IPv6 网络建立一个 TCP 连接，请执行以下操作：

  - 确保 AP 支持 IPv6
  - 设置 :ref:`AT+CIPV6=1 <cmd-IPV6>`
  - 通过 :ref:`AT+CWJAP <cmd-JAP>` 命令获取到一个 IPv6 地址
  - （可选）通过 :ref:`AT+CIPSTA? <cmd-IPSTA>` 命令检查 {IDF_TARGET_NAME} 是否获取到 IPv6 地址

示例
""""

::

    AT+CIPSTART="TCP","iot.espressif.cn",8000
    AT+CIPSTART="TCP","192.168.101.110",1000
    AT+CIPSTART="TCP","192.168.101.110",2500,60
    AT+CIPSTART="TCP","192.168.101.110",1000,,"192.168.101.100"
    AT+CIPSTART="TCPv6","test-ipv6.com",80
    AT+CIPSTART="TCPv6","fe80::860d:8eff:fe9d:cd90",1000,,"fe80::411c:1fdb:22a6:4d24"

    // esp-at 已通过 AT+CWJAP 获取到 IPv6 全局地址
    AT+CIPSTART="TCPv6","2404:6800:4005:80b::2004",80,,"240e:3a1:2070:11c0:32ae:a4ff:fe80:65ac"

.. _esp-at-cipstart-udp:

建立 UDP 传输
^^^^^^^^^^^^^^^^^^^^^^^^

设置命令
""""""""""""""

**命令：**

::

    // 单连接：(AT+CIPMUX=0)
    AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<local port>,<mode>,<"local IP">]

    // 多连接：(AT+CIPMUX=1)
    AT+CIPSTART=<link ID>,<"type">,<"remote host">,<remote port>[,<local port>,<mode>,<"local IP">]

**响应：**

单连接模式下，返回：

::

    CONNECT

    OK

多连接模式下，返回：

::

    <link ID>,CONNECT

    OK

参数
"""""""""""

-  **<link ID>**：网络连接 ID (0 ~ 4)，用于多连接的情况
-  **<"type">**：字符串参数，表示网络连接类型，"UDP" 或 "UDPv6"。默认值："TCP"
-  **<"remote host">**：字符串参数，表示远端 IPv4 地址、IPv6 地址，或域名。最长为 64 字节。如果您需要使用域名且域名长度超过 64 字节，请使用 :ref:`AT+CIPDOMAIN <cmd-DOMAIN>` 命令获取域名对应的 IP 地址，然后使用 IP 地址建立连接。
-  **<remote port>**：远端端口值
-  **<local port>**：{IDF_TARGET_NAME} 设备的 UDP 端口值
-  **<mode>**：在 UDP Wi-Fi 透传下，本参数的值必须设为 0

   -  0: 接收到 UDP 数据后，不改变对端 UDP 地址信息（默认）
   -  1: 仅第一次接收到与初始设置不同的对端 UDP 数据时，改变对端 UDP 地址信息为发送数据设备的 IP 地址和端口
   -  2: 每次接收到 UDP 数据时，都改变对端 UDP 地址信息为发送数据的设备的 IP 地址和端口

-  **<"local IP">**：连接绑定的本机 IPv4 地址或 IPv6 地址，该参数在本地多网络接口时和本地多 IP 地址时非常有用。默认为禁用，如果您想使用，需自行设置，空值也为有效值

说明
"""""
- 如果 UDP 连接中的远端 IP 地址是 IPv4 组播地址 (224.0.0.0 ~ 239.255.255.255)，{IDF_TARGET_NAME} 设备将发送和接收 UDPv4 组播
- 如果 UDP 连接中的远端 IP 地址是 IPv4 广播地址 (255.255.255.255)，{IDF_TARGET_NAME} 设备将发送和接收 UDPv4 广播
- 如果 UDP 连接中的远端 IP 地址是 IPv6 组播地址 (FF00:0:0:0:0:0:0:0 ~ FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF)，{IDF_TARGET_NAME} 设备将基于 IPv6 网络，发送和接收 UDP 组播
- 使用参数 ``<mode>`` 前，需先设置参数 ``<local port>``

- 如果您想基于 IPv6 网络建立一个 UDP 传输，请执行以下操作：

  - 确保 AP 支持 IPv6
  - 设置 :ref:`AT+CIPV6=1 <cmd-IPV6>`
  - 通过 :ref:`AT+CWJAP <cmd-JAP>` 命令获取到一个 IPv6 地址
  - （可选）通过 :ref:`AT+CIPSTA? <cmd-IPSTA>` 命令检查 {IDF_TARGET_NAME} 是否获取到 IPv6 地址

- 如果想接收长度大于 1460 字节的 UDP 包，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：``Component config`` -> ``LWIP`` -> ``Enable reassembly incoming fragmented IP4 packets``

示例
"""""""""

::

    // UDPv4 单播
    AT+CIPSTART="UDP","192.168.101.110",1000,1002,2
    AT+CIPSTART="UDP","192.168.101.110",1000,,,"192.168.101.100"

    // 基于 IPv6 网络的 UDP 单播
    AT+CIPSTART="UDPv6","fe80::32ae:a4ff:fe80:65ac",1000,,,"fe80::5512:f37f:bb03:5d9b"

    // 基于 IPv6 网络的 UDP 多播
    AT+CIPSTART="UDPv6","FF02::FC",1000,1002,0

.. _esp-at-cipstart-ssl:

建立 SSL 连接
^^^^^^^^^^^^^^^^^^^^^^^^

设置命令
""""""""""""""

**命令：**

::

    // 单连接：(AT+CIPMUX=0)
    AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<keep_alive>,<"local IP">]

    // 多连接：(AT+CIPMUX=1)
    AT+CIPSTART=<link ID>,<"type">,<"remote host">,<remote port>[,<keep_alive>,<"local IP">]

**响应：**

单连接模式下，返回：

::

    CONNECT

    OK

多连接模式下，返回：

::

    <link ID>,CONNECT

    OK

参数
"""""""""""

-  **<link ID>**：网络连接 ID (0 ~ 4)，用于多连接的情况
-  **<"type">**：字符串参数，表示网络连接类型，"SSL" 或 "SSLv6"。默认值："TCP"
-  **<"remote host">**：字符串参数，表示远端 IPv4 地址、IPv6 地址，或域名。最长为 64 字节。如果您需要使用域名且域名长度超过 64 字节，请使用 :ref:`AT+CIPDOMAIN <cmd-DOMAIN>` 命令获取域名对应的 IP 地址，然后使用 IP 地址建立连接。
-  **<remote port>**：远端端口值
-  **<keep_alive>**：配置套接字的 ``SO_KEEPALIVE`` 选项（参考：`SO_KEEPALIVE 介绍 <https://man7.org/linux/man-pages/man7/socket.7.html#SO_KEEPALIVE>`_），单位：秒。

  - 范围：[0,7200]。

    - 0：禁用 keep-alive 功能；（默认）
    - 1 ~ 7200：开启 keep-alive 功能。`TCP_KEEPIDLE <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPIDLE>`_ 值为 **<keep_alive>**，`TCP_KEEPINTVL <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPINTVL>`_ 值为 1，`TCP_KEEPCNT <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPCNT>`_ 值为 3。

  -  本命令中的 ``<keep_alive>`` 参数与 :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` 命令中的 ``<keep_alive>`` 参数相同，最终值由后设置的命令决定。如果运行本命令时不设置 ``<keep_alive>`` 参数，则默认使用上次配置的值。

-  **<"local IP">**：连接绑定的本机 IPv4 地址或 IPv6 地址，该参数在本地多网络接口时和本地多 IP 地址时非常有用。默认为禁用，如果您想使用，需自行设置，空值也为有效值

说明
""""""

- SSL 连接数量取决于可用内存和最大连接数量
- SSL 连接需占用大量内存，内存不足会导致系统重启
- 如果 ``AT+CIPSTART`` 命令是基于 SSL 连接，且每个数据包的超时时间为 10 秒，则总超时时间会变得更长，具体取决于握手数据包的个数

- 如果您想基于 IPv6 网络建立一个 SSL 连接，请执行以下操作：

  - 确保 AP 支持 IPv6
  - 设置 :ref:`AT+CIPV6=1 <cmd-IPV6>`
  - 通过 :ref:`AT+CWJAP <cmd-JAP>` 命令获取到一个 IPv6 地址
  - （可选）通过 :ref:`AT+CIPSTA? <cmd-IPSTA>` 命令检查 {IDF_TARGET_NAME} 是否获取到 IPv6 地址

示例
""""""""

::

    AT+CIPSTART="SSL","iot.espressif.cn",8443
    AT+CIPSTART="SSL","192.168.101.110",1000,,"192.168.101.100" 

    // esp-at 已通过 AT+CWJAP 获取到 IPv6 全局地址
    AT+CIPSTART="SSLv6","240e:3a1:2070:11c0:6972:6f96:9147:d66d",1000,,"240e:3a1:2070:11c0:55ce:4e19:9649:b75"

.. _cmd-STARTEX:

:ref:`AT+CIPSTARTEX <TCPIP-AT>`：建立自动分配 ID 的 TCP 连接、UDP 传输或 SSL 连接
------------------------------------------------------------------------------------------------------------------

本命令与 :ref:`AT+CIPSTART <cmd-START>` 相似，不同点在于：在多连接的情况下 (:ref:`AT+CIPMUX=1 <cmd-MUX>`) 无需手动分配 ID，系统会自动为新建的连接分配 ID。

.. _cmd-PLUS:

:ref:`[仅适用数据模式] +++ <TCPIP-AT>`：退出 :term:`数据模式`
--------------------------------------------------------------------------

特殊执行命令
^^^^^^^^^^^^^^^^^^^^^^^^

**功能：**

退出 :term:`数据模式`，进入 :term:`命令模式`

**Command:**

::

    // 仅适用数据模式
    +++

说明
""""""

-  此特殊执行命令包含有三个相同的 ``+`` 字符（即 ASCII 码：0x2b），同时命令结尾没有 CR-LF 字符
-  确保第一个 ``+`` 字符前至少有 20 ms 时间间隔内没有其他输入，第三个 ``+`` 字符后至少有 20 ms 时间间隔内没有其他输入，三个 ``+`` 字符之间至多有 20 ms 时间间隔内没有其他输入。否则，``+`` 字符会被当做普通数据发送出去
-  本条特殊执行命令没有命令回复
-  请至少间隔 1 秒再发下一条 AT 命令

.. _cmd-SEND:

:ref:`AT+CIPSEND <TCPIP-AT>`：在 :term:`普通传输模式` 或 Wi-Fi :term:`透传模式` 下发送数据
------------------------------------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

:term:`普通传输模式` 下，指定长度发送数据。如果您要发送的数据长度大于 8192 字节，请使用 :ref:`AT+CIPSENDL <cmd-SENDL>` 命令发送。

**命令：**

::

    // 单连接：(AT+CIPMUX=0)
    AT+CIPSEND=<length>

    // 多连接：(AT+CIPMUX=1)
    AT+CIPSEND=<link ID>,<length>

    // UDP 传输可指定对端主机和端口
    AT+CIPSEND=[<link ID>,]<length>[,<"remote host">,<remote port>]

**响应：**

::

    OK

    >

上述响应表示 AT 已准备好接收串行数据，此时您可以输入数据，当 AT 接收到的数据长度达到 ``<length>`` 后，数据传输开始。

如果未建立连接或数据传输时连接被断开，返回：

::

    ERROR

如果所有数据被成功发往协议栈（不代表数据已经发送到对端），返回：

::

    SEND OK

执行命令
^^^^^^^^

**功能：**

进入 Wi-Fi :term:`透传模式`

**命令：**

::

    AT+CIPSEND

**响应：**

::

    OK
    >

或

::

    ERROR

进入 Wi-Fi :term:`透传模式`，{IDF_TARGET_NAME} 设备每次最大接收 8192 字节，最大发送 2920 字节。如果收到的数据长度大于等于 2920 字节，数据会立即被分为每 2920 字节一组的块进行发送，否则会等待 20 毫秒或等待收到的数据大于等于 2920 字节再发送数据（您可以通过 :ref:`AT+TRANSINTVL <cmd-TRANSINTVL>` 命令配置此间隔）。当输入单独一包 :ref:`+++ <cmd-PLUS>` 时，退出 :term:`透传模式` 下的数据发送模式，请至少间隔 1 秒再发下一条 AT 命令。

本命令必须在开启 :term:`透传模式` 以及单连接下使用。若为 Wi-Fi UDP 透传，:ref:`AT+CIPSTART <cmd-START>` 命令的参数 ``<mode>`` 必须设置为 0。

参数
^^^^

-  **<link ID>**：网络连接 ID (0 ~ 4)，用于多连接的情况
-  **<length>**：数据长度，最大值：8192 字节
-  **<"remote host">**：UDP 传输可以指定对端主机：IPv4 地址、IPv6 地址，或域名
-  **<remote port>**：UDP 传输可以指定对端端口

说明
""""""

- 您可以使用 :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` 命令来为每个 TCP 连接配置套接字选项。例如：设置 <so_sndtimeo> 为 5000，则 TCP 发送操作会在 5 秒内返回结果，无论成功还是失败。这可以节省 MCU 等待 AT 命令回复的时间。

.. _cmd-SENDL:

:ref:`AT+CIPSENDL <TCPIP-AT>`：在 :term:`普通传输模式` 下并行发送长数据
----------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

:term:`普通传输模式` 下，指定长度，并行发送数据（AT 命令端口接收数据和 AT 往对端发送数据是并行的）。您可以使用 :ref:`AT+CIPSENDLCFG <cmd-SENDLCFG>` 命令配置本条命令。如果您要发送的数据长度小于 8192 字节，您也可以使用 :ref:`AT+CIPSEND <cmd-SEND>` 命令发送。

**命令：**

::

    // 单连接：(AT+CIPMUX=0)
    AT+CIPSENDL=<length>

    // 多连接：(AT+CIPMUX=1)
    AT+CIPSENDL=<link ID>,<length>

    // UDP 传输可指定对端主机和端口
    AT+CIPSENDL=[<link ID>,]<length>[,<"remote host">,<remote port>]

**响应：**

::

    OK

    >

上述响应表示 AT 进入 :term:`数据模式` 并且已准备好接收 AT 命令端口的数据，此时您可以输入数据，一旦 AT 命令端口接收到数据，数据就会被发往底层协议，数据传输开始。

如果传输已开始，系统会根据 :ref:`AT+CIPSENDLCFG <cmd-SENDLCFG>` 配置上报消息：

::

    +CIPSENDL:<had sent len>,<port recv len>

如果传输被 :ref:`+++ <cmd-PLUS>` 命令取消，系统返回：

::

    SEND CANCELLED

如果所有数据没有被完全发出去，系统最终返回：

::

    SEND FAIL

如果所有数据被成功发往协议栈（不代表数据已经发送到对端），系统最终返回：

::

    SEND OK 

当连接断开时，您可以发送 :ref:`+++ <cmd-PLUS>` 命令取消传输，同时 {IDF_TARGET_NAME} 设备会从 :term:`数据模式` 退出。否则，AT 命令端口会一直接收数据，直到收到指定的 ``<length>`` 长度数据后，才会退出 :term:`数据模式`。

参数
^^^^

-  **<link ID>**：网络连接 ID (0 ~ 4)，用于多连接的情况
-  **<length>**：数据长度，最大值：2 :sup:`31` - 1 字节
-  **<"remote host">**：UDP 传输可以指定对端主机：IPv4 地址、IPv6 地址，或域名
-  **<remote port>**：UDP 传输可以指定对端端口
-  **<had sent len>**：成功发到底层协议栈的数据长度
-  **<port recv len>**：AT 命令端口收到的数据总长度

说明
""""""

- 建议您使用 UART 流控。否则，如果 UART 接收速度大于网络发送速度时，将会导致数据丢失。
- 您可以使用 :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` 命令来为每个 TCP 连接配置套接字选项。例如：设置 <so_sndtimeo> 为 5000，则 TCP 发送操作会在 5 秒内返回结果，无论成功还是失败。这可以节省 MCU 等待 AT 命令回复的时间。

.. _cmd-SENDLCFG:

:ref:`AT+CIPSENDLCFG <TCPIP-AT>`: 设置 :ref:`AT+CIPSENDL <cmd-SENDL>` 命令的属性
------------------------------------------------------------------------------------------------------

查询命令
^^^^^^^^^^^^^

**功能：**

查询 :ref:`AT+CIPSENDL <cmd-SENDL>` 命令的配置

**命令：**

::

    AT+CIPSENDLCFG?

**响应：**

::

    +CIPSENDLCFG:<report size>,<transmit size>

    OK

设置命令
^^^^^^^^^^^

**功能：**

设置 :ref:`AT+CIPSENDL <cmd-SENDL>` 命令的配置

**命令：**

::

    AT+CIPSENDLCFG=<report size>[,<transmit size>]

**响应：**

::

    OK

参数
^^^^^^^^^^

-  **<report size>**: :ref:`AT+CIPSENDL <cmd-SENDL>` 命令中的上报块大小。默认值：1024。范围：[100,2 :sup:`20`]。例如：设置 ``<report size>`` 值为 100，则 :ref:`AT+CIPSENDL <cmd-SENDL>` 命令回复里的 ``<had sent len>`` 上报序列为（100，200，300，400，……）。最后的 ``<had sent len>`` 上报值总是等于实际传输的数据长度。
-  **<transmit size>**: :ref:`AT+CIPSENDL <cmd-SENDL>` 命令中的传输块大小，它指定了数据发往协议栈的数据块大小。默认值：2920。范围：[100,2920]。如果收到的数据长度大于等于 ``<transmit size>``，数据会立即被分为每 ``<transmit size>`` 字节一组的块进行发送，否则会等待 20 毫秒或等待收到的数据大于等于 ``<transmit size>`` 字节再发送数据（您可以通过 :ref:`AT+TRANSINTVL <cmd-TRANSINTVL>` 命令配置此间隔）。

说明
""""""

- 对于吞吐量小但对实时性要求高的设备，推荐您设置较小的 ``<transmit size>``。也推荐您通过 :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` 命令设置 ``TCP_NODELAY`` 属性。
- 对于吞吐量大的设备，推荐您设置较大的 ``<transmit size>``。也推荐您阅读 :doc:`如何提高 ESP-AT 吞吐性能 <../Compile_and_Develop/How_to_optimize_throughput>`。

.. _cmd-SENDEX:

:ref:`AT+CIPSENDEX <TCPIP-AT>`：在 :term:`普通传输模式` 下采用扩展的方式发送数据
----------------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

:term:`普通传输模式` 下，指定长度发送数据，或者使用字符串 ``\0`` (0x5c, 0x30 ASCII) 触发数据发送

**命令：**

::

    // 单连接：(AT+CIPMUX=0)
    AT+CIPSENDEX=<length>

    // 多连接：(AT+CIPMUX=1)
    AT+CIPSENDEX=<link ID>,<length>

    // UDP 传输可指定对端 IP 地址和端口：
    AT+CIPSENDEX=[<link ID>,]<length>[,<"remote host">,<remote port>]

**响应：**

::

    OK

    >

上述响应表示 AT 已准备好接收串行数据，此时您可以输入指定长度的数据，当 AT 接收到的数据长度达到 ``<length>`` 后或数据中出现 ``\0`` 字符时，数据传输开始。

如果未建立连接或数据传输时连接被断开，返回：

::

    ERROR

如果所有数据被成功发往协议栈（不代表数据已经发送到对端），返回：

::

    SEND OK

参数
^^^^

-  **<link ID>**：网络连接 ID (0 ~ 4)，用于多连接的情况
-  **<length>**：数据长度，最大值：8192 字节
-  **<"remote host">**：UDP 传输可以指定对端主机：IPv4 地址、IPv6 地址，或域名
-  **<remote port>**：UDP 传输可以指定对端端口

说明
^^^^^

-  当数据长度满足要求时，或数据中出现 ``\0`` 字符时 (0x5c，0x30 ASCII)，数据传输开始，系统返回普通命令模式，等待下一条 AT 命令
-  如果数据中包含 ``\<any>``，则会去掉反斜杠，只使用 ``<any>`` 符号
-  如果需要发送 ``\0``，请转义为 ``\\0``
- 您可以使用 :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` 命令来为每个 TCP 连接配置套接字选项。例如：设置 <so_sndtimeo> 为 5000，则 TCP 发送操作会在 5 秒内返回结果，无论成功还是失败。这可以节省 MCU 等待 AT 命令回复的时间。

.. _cmd-CLOSE:

:ref:`AT+CIPCLOSE <TCPIP-AT>`：关闭 TCP/UDP/SSL 连接
----------------------------------------------------------------------------

设置命令
^^^^^^^^^^

**功能：**

关闭多连接模式下的 TCP/UDP/SSL 连接

**命令：**

::

    AT+CIPCLOSE=<link ID>

**响应：**

::

    <link ID>,CLOSED

    OK

执行命令
^^^^^^^^^^

**功能：**

关闭单连接模式下的 TCP/UDP/SSL 连接

::

    AT+CIPCLOSE

**响应：**

::

    CLOSED

    OK

参数
^^^^

-  **<link ID>**：需关闭的网络连接 ID，如果设为 5，则表示关闭所有连接

.. _cmd-IFSR:

:ref:`AT+CIFSR <TCPIP-AT>`：查询本地 IP 地址和 MAC 地址
--------------------------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    AT+CIFSR

**响应：**

::

    +CIFSR:APIP,<"APIP">
    +CIFSR:APIP6LL,<"APIP6LL">
    +CIFSR:APIP6GL,<"APIP6GL">
    +CIFSR:APMAC,<"APMAC">
    +CIFSR:STAIP,<"STAIP">
    +CIFSR:STAIP6LL,<"STAIP6LL">
    +CIFSR:STAIP6GL,<"STAIP6GL">
    +CIFSR:STAMAC,<"STAMAC">
    +CIFSR:ETHIP,<"ETHIP">
    +CIFSR:ETHIP6LL,<"ETHIP6LL">
    +CIFSR:ETHIP6GL,<"ETHIP6GL">
    +CIFSR:ETHMAC,<"ETHMAC">

    OK

参数
^^^^

- **<"APIP">**: {IDF_TARGET_NAME} SoftAP 的 IPv4 地址
- **<"APIP6LL">**: {IDF_TARGET_NAME} SoftAP 的 IPv6 本地链路地址
- **<"APIP6GL">**: {IDF_TARGET_NAME} SoftAP 的 IPv6 全局地址
- **<"APMAC">**: {IDF_TARGET_NAME} SoftAP 的 MAC 地址
- **<"STAIP">**: {IDF_TARGET_NAME} station 的 IPv4 地址
- **<"STAIP6LL">**: {IDF_TARGET_NAME} station 的 IPv6 本地链路地址
- **<"STAIP6GL">**: {IDF_TARGET_NAME} station 的 IPv6 全局地址
- **<"STAMAC">**: {IDF_TARGET_NAME} station 的 MAC 地址
- **<"ETHIP">**: {IDF_TARGET_NAME} ethernet 的 IPv4 地址
- **<"ETHIP6LL">**: {IDF_TARGET_NAME} ethernet 的 IPv6 本地链路地址
- **<"ETHIP6GL">**: {IDF_TARGET_NAME} ethernet 的 IPv6 全局地址
- **<"ETHMAC">**: {IDF_TARGET_NAME} ethernet 的 MAC 地址

说明
^^^^

-  只有当 {IDF_TARGET_NAME} 设备获取到有效接口信息后，才能查询到它的 IP 地址和 MAC 地址

.. _cmd-MUX:

:ref:`AT+CIPMUX <TCPIP-AT>`：启用/禁用多连接模式
---------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询连接模式

**命令：**

::

    AT+CIPMUX?

**响应：**

::

    +CIPMUX:<mode>
    OK

设置命令
^^^^^^^^

**功能：**

设置连接模式

**命令：**

::

    AT+CIPMUX=<mode>

**响应：**

::

    OK

参数
^^^^

-  **<mode>**：连接模式，默认值：0

   -  0: 单连接
   -  1: 多连接

说明
^^^^

-  只有当所有连接都断开时才可更改连接模式
-  只有 :term:`普通传输模式` (:ref:`AT+CIPMODE=0 <cmd-IPMODE>`)，才能设置为多连接 
-  如果建立了 TCP/SSL 服务器，想切换为单连接，必须关闭服务器 (:ref:`AT+CIPSERVER=0 <cmd-SERVER>`)

示例
^^^^

::

    AT+CIPMUX=1 

.. _cmd-SERVER:

:ref:`AT+CIPSERVER <TCPIP-AT>`：建立/关闭 TCP 或 SSL 服务器
------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 TCP/SSL 服务器状态

**命令：**

::

    AT+CIPSERVER?

**响应：**

::

    +CIPSERVER:<mode>[,<port>,<"type">][,<CA enable>]

    OK

设置命令
^^^^^^^^

**命令：**

::

    AT+CIPSERVER=<mode>[,<param2>][,<"type">][,<CA enable>]

**响应：**

::

    OK  

参数
^^^^

-  **<mode>**：

   -  0: 关闭服务器
   -  1: 建立服务器

-  **<param2>**：参数 ``<mode>`` 不同，则此参数意义不同：

  - 如果 ``<mode>`` 是 1，``<param2>`` 代表端口号。默认值：333
  - 如果 ``<mode>`` 是 0，``<param2>`` 代表服务器是否关闭所有客户端。默认值：0

    - 0：关闭服务器并保留现有客户端连接
    - 1：关闭服务器并关闭所有连接

-  **<"type">**：服务器类型："TCP"，"TCPv6"，"SSL"，或 "SSLv6". 默认值："TCP"
-  **<CA enable>**：

   -  0：不使用 CA 认证
   -  1：使用 CA 认证

说明
^^^^

- 多连接情况下 (:ref:`AT+CIPMUX=1 <cmd-MUX>`)，才能开启服务器。
- 创建服务器后，自动建立服务器监听，最多只允许创建一个服务器。
- 当有客户端接入，会自动占用一个连接 ID。
- 如果您想基于 IPv6 网络创建一个 TCP/SSL 服务器，请首先设置 :ref:`AT+CIPV6=1 <cmd-IPV6>`，并获取一个IPv6地址。
- 关闭服务器时参数 ``<"type">`` 和 ``<CA enable>`` 必须省略。

示例
^^^^

::

    // 建立 TCP 服务器
    AT+CIPMUX=1
    AT+CIPSERVER=1,80

    // 建立 SSL 服务器
    AT+CIPMUX=1
    AT+CIPSERVER=1,443,"SSL",1

    // 基于 IPv6 网络，创建 SSL 服务器
    AT+CIPMUX=1
    AT+CIPSERVER=1,443,"SSLv6",0

    // 关闭服务器并且关闭所有连接
    AT+CIPSERVER=0,1

.. _cmd-SERVERMAX:

:ref:`AT+CIPSERVERMAXCONN <TCPIP-AT>`：查询/设置服务器允许建立的最大连接数
--------------------------------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 TCP 或 SSL 服务器允许建立的最大连接数

**命令：**

::

    AT+CIPSERVERMAXCONN?

**响应：**

::

    +CIPSERVERMAXCONN:<num>
    OK  

设置命令
^^^^^^^^

**功能：**

设置 TCP 或 SSL 服务器允许建立的最大连接数

**命令：**

::

    AT+CIPSERVERMAXCONN=<num>

**响应：**

::

    OK  

参数
^^^^

-  **<num>**：TCP 或 SSL 服务器允许建立的最大连接数，范围：[1,5]。如果您想修改该参数的上限阈值，请参考 :ref:`AT+CIPSTART <cmd-START>` 命令中参数 ``<link ID>`` 的描述。

说明
^^^^

-  如需设置最大连接数 (``AT+CIPSERVERMAXCONN=<num>``)，请在创建服务器之前设置。

示例
^^^^

::

    AT+CIPMUX=1
    AT+CIPSERVERMAXCONN=2
    AT+CIPSERVER=1,80

.. _cmd-IPMODE:

:ref:`AT+CIPMODE <TCPIP-AT>`：查询/设置传输模式
------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询传输模式

**命令：**

::

    AT+CIPMODE?

**响应：**

::

    +CIPMODE:<mode>
    OK

设置命令
^^^^^^^^

**功能：**

设置传输模式

**命令：**

::

    AT+CIPMODE=<mode>

**响应：**

::

    OK

参数
^^^^

-  **<mode>**:

   -  0: :term:`普通传输模式`
   -  1: Wi-Fi :term:`透传接收模式`，仅支持 TCP 单连接、UDP 固定通信对端、SSL 单连接的情况

说明
^^^^

-  配置更改不保存到 flash。
-  在 {IDF_TARGET_NAME} 进入 Wi-Fi :term:`透传接收模式` 后，任何蓝牙功能将无法使用。

示例
^^^^

::

    AT+CIPMODE=1

.. _cmd-STO:

:ref:`AT+CIPSTO <TCPIP-AT>`：查询/设置本地 TCP/SSL 服务器超时时间
----------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询本地 TCP/SSL 服务器超时时间

**命令：**

::

    AT+CIPSTO?

**响应：**

::

    +CIPSTO:<time>
    OK

设置命令
^^^^^^^^

**功能：**

设置本地 TCP/SSL 服务器超时时间

**命令：**

::

    AT+CIPSTO=<time>

**响应：**

::

    OK

参数
^^^^

-  **<time>**：本地 TCP/SSL 服务器超时时间，单位：秒，取值范围：[0,7200]

说明
^^^^

-  当 TCP/SSL 客户端在 ``<time>`` 时间内未发生数据通讯时，{IDF_TARGET_NAME} 服务器会断开此连接。
-  如果设置参数 ``<time>`` 为 0，则连接永远不会超时，不建议这样设置。
-  在设定的时间内，当客户端发起与服务器的通信或者服务器发起与客户端的通信时，计时器将重新计时。超时后，客户端被关闭。

示例
^^^^

::

    AT+CIPMUX=1
    AT+CIPSERVER=1,1001
    AT+CIPSTO=10

.. _cmd-SNTPCFG:

:ref:`AT+CIPSNTPCFG <TCPIP-AT>`：查询/设置时区和 SNTP 服务器
------------------------------------------------------------------------------

查询命令
^^^^^^^^

**命令：**

::

    AT+CIPSNTPCFG?

**响应：**

::

    +CIPSNTPCFG:<enable>,<timezone>,<SNTP server1>[,<SNTP server2>,<SNTP server3>]
    OK

设置命令
^^^^^^^^

**命令：**

::

    AT+CIPSNTPCFG=<enable>,<timezone>[,<SNTP server1>,<SNTP server2>,<SNTP server3>]

**响应：**

::

    OK

参数
^^^^

-  **<enable>**：设置 SNTP 服务器：

   -  1: 设置 SNTP 服务器；
   -  0: 不设置 SNTP 服务器。

-  **<timezone>**：支持以下两种格式：

   -  第一种格式的范围：[-12,14]，它以小时为单位，通过与协调世界时 (UTC) 的偏移来标记大多数时区（`UTC−12:00 <https://en.wikipedia.org/wiki/UTC%E2%88%9212:00>`_ 至 `UTC+14:00 <https://en.wikipedia.org/wiki/UTC%2B14:00>`_）；
   -  第二种格式为 ``UTC 偏移量``， ``UTC 偏移量`` 指定了你需要加多少时间到 UTC 时间上才能得到本地时间，通常显示为 ``[+|-][hh]mm``。如果当地时区在本初子午线以西，则为负数，如果在东边，则为正数。小时 (hh) 必须在 -12 到 14 之间，分钟 (mm) 必须在 0 到 59 之间。例如，如果您想把时区设置为新西兰查塔姆群岛，即 ``UTC+12:45``，您应该把 ``<timezone>`` 参数设置为 ``1245``，更多信息请参考 `UTC 偏移量 <https://en.wikipedia.org/wiki/Time_zone#List_of_UTC_offsets>`_。

-  **[<SNTP server1>]**：第一个 SNTP 服务器。
-  **[<SNTP server2>]**：第二个 SNTP 服务器。
-  **[<SNTP server3>]**：第三个 SNTP 服务器。

说明
^^^^

-  设置命令若未填写以上三个 SNTP 服务器参数，则默认使用 "cn.ntp.org.cn"、"ntp.sjtu.edu.cn" 和 "us.pool.ntp.org" 其中之一。
-  对于查询命令，查询的 ``<timezone>`` 参数可能会和设置的 ``<timezone>`` 参数不一样。因为 ``<timezone>`` 参数支持第二种 UTC 偏移量格式，例如：设置 ``AT+CIPSNTPCFG=1,015``，那么查询时，ESP-AT 会忽略时区参数的前导 0，即设置值是 ``15``。不属于第一种格式，所以按照第二种 UTC 偏移量格式解析，也就是 ``UTC+00:15``，也就是查询出来的是 0 时区。
-  由于 SNTP 是基于 UDP 协议发送请求和接收回复，当网络丢包时，会导致 {IDF_TARGET_NAME} 的时间无法及时同步。一旦 AT 命令口输出 :ref:`+TIME_UPDATED <at-messages-report>`，代表时间已同步，此时您可以发送 :ref:`AT+CIPSNTPTIME? <cmd-SNTPT>` 命令查询当前时间。

示例
^^^^

::

    // 使能 SNTP 服务器，设置中国时区 (UTC+08:00)
    AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"
    或
    AT+CIPSNTPCFG=1,800,"cn.ntp.org.cn","ntp.sjtu.edu.cn"

    // 使能 SNTP 服务器，设置美国纽约的时区 (UTC−05:00)
    AT+CIPSNTPCFG=1,-5,"0.pool.ntp.org","time.google.com"
    或
    AT+CIPSNTPCFG=1,-500,"0.pool.ntp.org","time.google.com"

    // 使能 SNTP 服务器，设置新西兰时区查塔姆群岛的时区 (Chatham Islands, UTC+12:45)
    AT+CIPSNTPCFG=1,1245,"0.pool.ntp.org","time.google.com"

.. _cmd-SNTPT:

:ref:`AT+CIPSNTPTIME <TCPIP-AT>`：查询 SNTP 时间
-----------------------------------------------------------

查询命令
^^^^^^^^

**命令：**

::

    AT+CIPSNTPTIME? 

**响应：**

::

    +CIPSNTPTIME:<asctime style time>
    OK

说明
^^^^

-  有关 asctime 时间的定义请见 `asctime man page <https://linux.die.net/man/3/asctime>`_。
-  在 {IDF_TARGET_NAME} 进入 Light-sleep 或 Deep-sleep 后再唤醒，系统时间可能会不准。建议您重新发送 :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>` 命令，从 NTP 服务器获取新的时间。

.. only:: esp32 or esp32c3 or esp32c6 or esp32s2

  - SNTP 获取到的时间存储在 RTC 区域，因此在软重启（芯片不掉电）后，时间不会丢失。

.. only:: esp32c2

  - SNTP 获取到的时间暂不支持存储在 RTC 区域，因此在软重启（芯片不掉电）后，时间会重置到 1970 年。

示例
^^^^

::

    AT+CWMODE=1
    AT+CWJAP="1234567890","1234567890"
    AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"
    AT+CIPSNTPTIME?
    +CIPSNTPTIME:Tue Oct 19 17:47:56 2021
    OK

    或

    AT+CWMODE=1
    AT+CWJAP="1234567890","1234567890"
    AT+CIPSNTPCFG=1,530
    AT+CIPSNTPTIME?
    +CIPSNTPTIME:Tue Oct 19 15:17:56 2021
    OK

.. _cmd-SNTPINTV:

:ref:`AT+CIPSNTPINTV <TCPIP-AT>`：查询/设置 SNTP 时间同步的间隔
----------------------------------------------------------------------------------

查询命令
^^^^^^^^^^^^

**命令：**

::

    AT+CIPSNTPINTV? 

**响应：**

::

    +CIPSNTPINTV:<interval second>

    OK

设置命令
^^^^^^^^^^^^^^

**命令：**

::

    AT+CIPSNTPINTV=<interval second>

**响应：**

::

    OK

参数
^^^^

-  **<interval second>**：SNTP 时间同步间隔。单位：秒。范围：[15,4294967]。

说明
^^^^

- 配置了时间同步间隔，意味着 {IDF_TARGET_NAME} 多久一次向 NTP 服务器获取新的时间。

示例
^^^^

::

    AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"

    OK

    // 每小时同步一次时间
    AT+CIPSNTPINTV=3600

    OK

.. _cmd-FWVER:

:ref:`AT+CIPFWVER <TCPIP-AT>`：查询服务器已有的 AT 固件版本
---------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询服务器已有的 {IDF_TARGET_NAME} AT 固件版本

**命令：**

::

    AT+CIPFWVER?

**响应：**

::

    +CIPFWVER:<"version">

    OK

参数
^^^^
- **<"version">**：{IDF_TARGET_NAME} AT 固件版本

说明
^^^^

- 在选择要升级的 OTA 版本时，强烈不建议从高版本向低版本升级。

.. _cmd-UPDATE:

:ref:`AT+CIUPDATE <TCPIP-AT>`：通过 Wi-Fi 升级固件
---------------------------------------------------------------------

ESP-AT 在运行时，通过 Wi-Fi 从指定的服务器上下载新固件到某些分区，从而升级固件。

查询命令
^^^^^^^^

**功能：**

查询 {IDF_TARGET_NAME} 设备的升级状态

**命令：**

::

    AT+CIUPDATE?

**响应：**

::

    +CIPUPDATE:<state>

    OK

执行命令
^^^^^^^^

**功能：**

在阻塞模式下通过 OTA 升级到 TCP 服务器上最新版本的固件

**命令：**

::

    AT+CIUPDATE  

**响应：**

请参考设置命令中的 :ref:`响应 <cmd-UPDATE-RESPONSE>`

设置命令
^^^^^^^^

**功能：**

升级到服务器上指定版本的固件

.. _cmd-UPDATE-RESPONSE:

**命令：**

::

    AT+CIUPDATE=<ota mode>[,<version>][,<firmware name>][,<nonblocking>]

**响应：**

如果 OTA 在阻塞模式下成功，返回：

::

    +CIPUPDATE:1
    +CIPUPDATE:2
    +CIPUPDATE:3
    +CIPUPDATE:4
    
    OK

如果 OTA 在非阻塞模式下成功，返回：

::

    OK
    +CIPUPDATE:1
    +CIPUPDATE:2
    +CIPUPDATE:3
    +CIPUPDATE:4

如果在阻塞模式下 OTA 失败，返回：

::

    +CIPUPDATE:<state>

    ERROR

如果在非阻塞模式下 OTA 失败，返回：

::

    OK
    +CIPUPDATE:<state>
    +CIPUPDATE:-1

参数
^^^^
- **<ota mode>**:
    
    - 0: 通过 HTTP OTA；
    - 1: 通过 HTTPS OTA，如果无效，请检查 ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``OTA based upon ssl`` 是否使能，更多信息请见 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`。

- **<version>**：AT 版本，如 ``v1.2.0.0``、``v1.1.3.0`` 或 ``v1.1.2.0``。
- **<firmware name>**：升级的固件，如 ``ota``、``mqtt_ca``、``client_ca`` 或其它 ``at_customize.csv`` 中自定义的分区。
- **<nonblocking>**:

    - 0: 阻塞模式的 OTA（此模式下，直到 OTA 升级成功或失败后才可以发送 AT 命令）；
    - 1: 非阻塞模式的 OTA（此模式下，升级完成后 (+CIPUPDATE:4) 需手动重启)。

- **<state>**:

    - 1: 找到服务器；
    - 2: 连接至服务器；
    - 3: 获得升级版本；
    - 4: 完成升级；
    - -1: 非阻塞模式下 OTA 失败。

说明
^^^^

-  升级速度取决于网络状况。
-  如果网络条件不佳导致升级失败，AT 将返回 ``ERROR``，请等待一段时间再试。 
-  如果您直接使用乐鑫提供的 AT `BIN <https://www.espressif.com/zh-hans/support/download/at>`_, 本命令将从 Espressif Cloud 下载 AT 固件升级。
-  如果您使用的是自行编译的 AT BIN，请自行实现 AT+CIUPDATE FOTA 功能或者使用 :ref:`AT+USEROTA <cmd-USEROTA>` 或者 :ref:`AT+WEBSERVER <cmd-WEBSERVER>` 命令，可参考 ESP-AT 工程提供的示例 `FOTA <https://github.com/espressif/esp-at/blob/master/components/at/src/at_ota_cmd.c>`_。
-  建议升级 AT 固件后，调用 :ref:`AT+RESTORE <cmd-RESTORE>` 恢复出厂设置。
-  OTA 过程的超时时间为 ``3`` 分钟。
-  非阻塞模式响应中的 ``OK`` 和 ``+CIPUPDATE:<state>`` 在输出顺序上没有严格意义上的先后顺序。OK 可能在 ``+CIPUPDATE:<state>`` 之前输出，也有可能在 ``+CIPUPDATE:<state>`` 之后输出。
-  不建议升级到旧版本。降到旧版本会存在一定的兼容性问题，甚至无法运行，如果您坚持要升级到旧版本，请根据自己的产品自行测试验证功能。
-  请参考 :doc:`../Compile_and_Develop/How_to_implement_OTA_update` 获取更多 OTA 命令。

示例
^^^^

::

    AT+CWMODE=1
    AT+CWJAP="1234567890","1234567890"
    AT+CIUPDATE
    AT+CIUPDATE=1
    AT+CIUPDATE=1,"v1.2.0.0"
    AT+CIUPDATE=1,"v2.2.0.0","mqtt_ca"
    AT+CIUPDATE=1,"v2.2.0.0","ota",1
    AT+CIUPDATE=1,,,1
    AT+CIUPDATE=1,,"ota",1
    AT+CIUPDATE=1,"v2.2.0.0",,1

.. _cmd-IPDINFO:

:ref:`AT+CIPDINFO <TCPIP-AT>`：设置 +IPD 消息详情
----------------------------------------------------------------

查询命令
^^^^^^^^

**命令：**

::

    AT+CIPDINFO?

**响应：**

::

    +CIPDINFO:true
    OK

或

::
    
    +CIPDINFO:false
    OK

设置命令
^^^^^^^^

**命令：**

::

    AT+CIPDINFO=<mode>  

**响应：**

::

    OK  

参数
^^^^

-  **<mode>**:

   -  0: 在 "+IPD" 和 "+CIPRECVDATA" 消息中，不提示对端 IP 地址和端口信息
   -  1: 在 "+IPD" 和 "+CIPRECVDATA" 消息中，提示对端 IP 地址和端口信息

示例
^^^^

::

    AT+CIPDINFO=1

.. _cmd-SSLCCONF:

:ref:`AT+CIPSSLCCONF <TCPIP-AT>`：查询/设置 SSL 客户端配置
-------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 {IDF_TARGET_NAME} 作为 SSL 客户端时每个连接的配置信息

**命令：**

::

    AT+CIPSSLCCONF?

**响应：**

::

    +CIPSSLCCONF:<link ID>,<auth_mode>,<pki_number>,<ca_number>
    OK

设置命令
^^^^^^^^

**命令：**

::

    // 单连接：(AT+CIPMUX=0)
    AT+CIPSSLCCONF=<auth_mode>[,<pki_number>][,<ca_number>]

    // 多连接：(AT+CIPMUX=1)
    AT+CIPSSLCCONF=<link ID>,<auth_mode>[,<pki_number>][,<ca_number>]

**响应：**

::

    OK

参数
^^^^

-  **<link ID>**：网络连接 ID (0 ~ max)，在多连接的情况下，若参数值设为 max，则表示所有连接，本参数默认值为 5。
-  **<auth_mode>**:

   -  0: 不认证，此时无需填写 ``<pki_number>`` 和 ``<ca_number>`` 参数；
   -  1: ESP-AT 提供客户端证书供服务器端 CA 证书校验；
   -  2: ESP-AT 客户端载入 CA 证书来校验服务器端的证书；
   -  3: 相互认证。

-  **<pki_number>**：证书和私钥的索引，如果只有一个证书和私钥，其值应为 0。
-  **<ca_number>**：CA 的索引，如果只有一个 CA，其值应为 0。

说明
^^^^

-  如果想要本配置立即生效，请在建立 SSL 连接前运行本命令。
-  配置更改将保存在 NVS 区，如果您使用 :ref:`AT+SAVETRANSLINK <cmd-SAVET>` 命令设置开机进入 Wi-Fi SSL :term:`透传模式`，{IDF_TARGET_NAME} 将在下次上电时基于本配置建立 SSL 连接。
-  如果您想使用自己的证书或者使用多套证书，请参考 :doc:`../Compile_and_Develop/How_to_update_pki_config`。
-  如果 ``<auth_mode>`` 配置为 2 或者 3，为了校验服务器的证书有效期，请在发送 :ref:`AT+CIPSTART <cmd-START>` 命令前确保 {IDF_TARGET_NAME} 已获取到当前时间。（您可以发送 :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>` 命令来配置 SNTP，获取当前时间，发送 :ref:`AT+CIPSNTPTIME? <cmd-SNTPT>` 命令查询当前时间。）

.. _cmd-SSLCCN:

:ref:`AT+CIPSSLCCN <TCPIP-AT>`：查询/设置 SSL 客户端的公用名 (common name)
-------------------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询每个 SSL 连接中客户端的通用名称

**命令：**

::

    AT+CIPSSLCCN?

**响应：**

::

    +CIPSSLCCN:<link ID>,<"common name">
    OK

设置命令
^^^^^^^^

**命令：**

::

    // 单连接：(AT+CIPMUX=0)
    AT+CIPSSLCCN=<"common name">

    // 多连接：(AT+CIPMUX=1)
    AT+CIPSSLCCN=<link ID>,<"common name">

**响应：**

::

    OK

参数
^^^^

-  **<link ID>**：网络连接 ID (0 ~ max)，在单连接的情况下，本参数值为 0；在多连接的情况下，若参数值设为 max，则表示所有连接；本参数默认值为 5。
-  **<"common name">**：本参数用来认证服务器发送的证书中的公用名。公用名最大长度为 64 字节。

说明
^^^^

-  如果想要本配置立即生效，请在建立 SSL 连接前运行本命令。

.. _cmd-SSLCSNI:

:ref:`AT+CIPSSLCSNI <TCPIP-AT>`：查询/设置 SSL 客户端的 SNI
---------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询每个连接的 SNI 配置

**命令：**

::

    AT+CIPSSLCSNI?

**响应：**

::

    +CIPSSLCSNI:<link ID>,<"sni">
    OK

设置命令
^^^^^^^^

**命令：**

::

    单连接：(AT+CIPMUX=0)
    AT+CIPSSLCSNI=<"sni">

    多连接：(AT+CIPMUX=1)
    AT+CIPSSLCSNI=<link ID>,<"sni">

**响应：**

::

    OK

参数
^^^^

-  **<link ID>**：网络连接 ID (0 ~ max)，在单连接的情况下，本参数值为 0；在多连接的情况下，若参数值设为 max，则表示所有连接；本参数默认值为 5。
-  **<"sni">**：ClientHello 里的 SNI。SNI 最大长度为 64 字节。

说明
^^^^

-  如果想要本配置立即生效，请在建立 SSL 连接前运行本命令。

.. _cmd-SSLCALPN:

:ref:`AT+CIPSSLCALPN <TCPIP-AT>`：查询/设置 SSL 客户端 ALPN
-----------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 {IDF_TARGET_NAME} 作为 SSL 客户端时每个连接的 ALPN 配置

**命令：**

::

    AT+CIPSSLCALPN?

**响应：**

::

    +CIPSSLCALPN:<link ID>,<"alpn">[,<"alpn">][,<"alpn">]

    OK

设置命令
^^^^^^^^

**命令：**

::

    // 单连接：(AT+CIPMUX=0)
    AT+CIPSSLCALPN=<counts>[,<"alpn">][,<"alpn">][,<"alpn">]

    // 多连接：(AT+CIPMUX=1)
    AT+CIPSSLCALPN=<link ID>,<counts>[,<"alpn">][,<"alpn">[,<"alpn">]

**响应：**

::

    OK

参数
^^^^

-  **<link ID>**：网络连接 ID (0 ~ max)，在单连接的情况下，本参数值为 0；在多连接的情况下，若参数值设为 max，则表示所有连接；本参数默认值为 5。
-  **<counts>**：ALPN 的数量。范围：[0,5]。

  - 0: 清除 ALPN 配置。
  - [1,5]: 设置 ALPN 配置。

-  **<"alpn">**：字符串参数，表示 ClientHello 中的 ALPN。ALPN 最大长度受限于命令的最大长度。

说明
^^^^

-  如果想要本配置立即生效，请在建立 SSL 连接前运行本命令。

.. _cmd-SSLCPSK:

:ref:`AT+CIPSSLCPSK <TCPIP-AT>`：查询/设置 SSL 客户端的 PSK (字符串格式)
---------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 {IDF_TARGET_NAME} 作为 SSL 客户端时每个连接的 PSK 配置

**命令：**

::

    AT+CIPSSLCPSK?

**响应：**

::

    +CIPSSLCPSK:<link ID>,<"psk">,<"hint">
    OK

设置命令
^^^^^^^^

**命令：**

::

    // 单连接：(AT+CIPMUX=0)
    AT+CIPSSLCPSK=<"psk">,<"hint">

    // 多连接：(AT+CIPMUX=1)
    AT+CIPSSLCPSK=<link ID>,<"psk">,<"hint">

**响应：**

::

    OK

参数
^^^^

-  **<link ID>**：网络连接 ID (0 ~ max)，在单连接的情况下，本参数值为 0；在多连接的情况下，若参数值设为 max，则表示所有连接；本参数默认值为 5。
-  **<"psk">**：PSK identity (字符串格式)，最大长度：32。如果您的 ``<"psk">`` 参数包含 ``\0``，请使用 :ref:`AT+CIPSSLCPSKHEX <cmd-SSLCPSKHEX>` 命令。
-  **<"hint">**：PSK hint，最大长度：32。

说明
^^^^
-  如果想要本配置立即生效，请在建立 SSL 连接前运行本命令。

.. _cmd-SSLCPSKHEX:

:ref:`AT+CIPSSLCPSKHEX <TCPIP-AT>`：查询/设置 SSL 客户端的 PSK (十六进制格式)
-----------------------------------------------------------------------------------------

说明
^^^^
- 类似于 :ref:`AT+CIPSSLCPSK <cmd-SSLCPSK>` 命令，该命令也用于设置或查询 SSL 客户端的预共享密钥（PSK），但其 ``<"psk">`` 参数使用十六进制格式而不是字符串格式。因此， ``<"psk">`` 参数中的 ``\0`` 表示为 ``00``。

示例
^^^^

::

    // 单连接：(AT+CIPMUX=0), PSK identity 为 "psk"，PSK hint 为 "myhint"
    AT+CIPSSLCPSKHEX="70736b","myhint"

    // 多连接：(AT+CIPMUX=1), PSK identity 为 "psk"，PSK hint 为 "myhint"
    AT+CIPSSLCPSKHEX=0,"70736b","myhint"

.. _cmd-AUTOCONNINT:

:ref:`AT+CIPRECONNINTV <TCPIP-AT>`：查询/设置 Wi-Fi :term:`透传模式` 下的 TCP/UDP/SSL 重连间隔
-----------------------------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 Wi-Fi :term:`透传模式` 下的自动重连间隔

**命令：**

::

    AT+CIPRECONNINTV?

**响应：**

::

    +CIPRECONNINTV:<interval>
    OK

设置命令
^^^^^^^^

**功能：**

设置 Wi-Fi :term:`透传模式` 下 TCP/UDP/SSL 传输断开后自动重连的间隔

**命令：**

::

    AT+CIPRECONNINTV=<interval>

**响应：**

::

    OK

参数
^^^^

-  **<interval>**：自动重连间隔时间，单位：100 毫秒，默认值：1，范围：[1,36000]。

说明
^^^^

-  若 :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>` 时，配置更改将保存在 NVS 区。

示例
^^^^

::

    AT+CIPRECONNINTV=10

.. _cmd-CIPRECVTYPE:

:ref:`AT+CIPRECVTYPE <TCPIP-AT>`：查询/设置套接字接收模式
-----------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询套接字接收模式

**命令：**

::

    AT+CIPRECVTYPE?

**响应：**

::

    +CIPRECVTYPE:<link ID>,<mode>

    OK

设置命令
^^^^^^^^

**命令：**

::

    // 单连接：(AT+CIPMUX=0)
    AT+CIPRECVTYPE=<mode>

    // 多连接：(AT+CIPMUX=1)
    AT+CIPRECVTYPE=<link ID>,<mode>

**响应：**

::

    OK

参数
^^^^

- **<link ID>**：网络连接 ID (0 ~ max)，在单连接的情况下，本参数值为 0；在多连接的情况下，若参数值设为 max，则表示所有连接；本参数默认值为 5。
- **<mode>**：套接字数据接收模式，默认值：0。
   
   - 0: 主动模式，ESP-AT 将所有接收到的套接字数据立即发送给主机 MCU，头为 "+IPD"（套接字接收窗口为 5760 字节，每次向 MCU 最大发送 2920 字节有效数据）。
   - 1: 被动模式，ESP-AT 将所有接收到的套接字数据保存到内部缓存区（套接字接收窗口，默认值为 5760 字节），等待 MCU 读取。对于 TCP 和 SSL 连接，如果缓存区满了，将阻止套接字传输；对于 UDP 传输，如果缓存区满了，则会发生数据丢失。

说明
^^^^

-  该配置不能用于 Wi-Fi :term:`透传模式`。

-  当 ESP-AT 在被动模式下收到套接字数据时，会根据情况的不同提示不同的信息：

   -  多连接时 (AT+CIPMUX=1)，提示 ``+IPD,<link ID>,<len>``；
   -  单连接时 (AT+CIPMUX=0)，提示 ``+IPD,<len>``。

-  ``<len>`` 表示缓存区中套接字数据的总长度。
-  一旦有 ``+IPD`` 报出，应该运行 :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>` 来读取数据。否则，在前一个 ``+IPD`` 被读取之前，下一个 ``+IPD`` 将不会被报告给主机 MCU。
-  在断开连接的情况下，缓冲的套接字数据仍然存在，MCU 仍然可以读取，直到发送 :ref:`AT+CIPCLOSE <cmd-CLOSE>` （AT 作为客户端）或 :ref:`AT+CIPSERVER=0,1 <cmd-SERVER>` （AT 作为服务器）。换句话说，如果 ``+IPD`` 已经被报告，那么在你发送 :ref:`AT+CIPCLOSE <cmd-CLOSE>` 或发送 :ref:`AT+CIPSERVER=0,1 <cmd-SERVER>` 或通过 :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>` 命令读取所有数据之前，这个连接的 ``CLOSED`` 信息永远不会出现。
-  预计设备将接收大量网络数据并且 MCU 端来不及处理时，可以参考 :ref:`示例 <using-passive-mode>`，使用被动接收数据模式。

示例
^^^^

::

    // 单连接模式下，设置被动接收模式
    AT+CIPRECVTYPE=1

    // 多连接模式下，设置所有连接为被动接收模式
    AT+CIPRECVTYPE=5,1

.. _cmd-CIPRECVDATA:

:ref:`AT+CIPRECVDATA <TCPIP-AT>`：获取被动接收模式下的套接字数据
-------------------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    // 单连接：(AT+CIPMUX=0)
    AT+CIPRECVDATA=<len>

    // 多连接：(AT+CIPMUX=1)
    AT+CIPRECVDATA=<link_id>,<len>

**响应：**

::

    +CIPRECVDATA:<actual_len>,<data>
    OK

或

::

    +CIPRECVDATA:<actual_len>,<remote IP>,<remote port>,<data>
    OK

参数
^^^^

-  **<link_id>**：多连接模式下的连接 ID。
-  **<len>**：最大值为：0x7fffffff，如果实际收到的数据长度比本参数值小，则返回实际长度的数据。
-  **<actual_len>**：实际获取的数据长度。
-  **<data>**：获取的数据。
-  **[<remote IP>]**：字符串参数，表示对端 IP 地址，通过 :ref:`AT+CIPDINFO=1 <cmd-IPDINFO>` 命令使能。
-  **[<remote port>]**：对端端口，通过 :ref:`AT+CIPDINFO=1 <cmd-IPDINFO>` 命令使能。

说明
^^^^

- 该命令需要在被动接收模式下执行，否则会直接返回 ERROR，可以通过 :ref:`AT+CIPRECVTYPE? <cmd-CIPRECVTYPE>` 命令确认是否是在被动接收模式。
- 该命令在没有数据可读的情况下执行时会直接返回 ERROR，可以通过 :ref:`AT+CIPRECVLEN? <cmd-CIPRECVLEN>` 命令确认此时是否有可读数据。
- 执行 ``AT+CIPRECVDATA=<len>`` 命令时，至少需要 ``<len> + 128`` 字节的内存，您可以使用命令 :ref:`AT+SYSRAM? <Basic-AT>` 查询当前可用内存情况。当内存不足导致内存申请失败时此命令也会返回 ERROR。你可以通过 :doc:`AT 输出日志口 </Get_Started/Hardware_connection>` 查看是否有类似 ``alloc fail`` 的打印信息，以确认是否出现了内存分配失败的情况。

示例
^^^^

::

    AT+CIPRECVTYPE=1

    // 例如，如果主机 MCU 从 0 号连接中收到 100 字节的数据，
    // 则会提示消息 "+IPD,0,100"，
    // 然后，您可以通过运行以下命令读取这 100 字节的数据：
    AT+CIPRECVDATA=0,100

.. _cmd-CIPRECVLEN:

:ref:`AT+CIPRECVLEN <TCPIP-AT>`：查询被动接收模式下套接字数据的长度
-------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询某一连接中缓存的所有的数据长度

**命令：**

::

    AT+CIPRECVLEN?

**响应：**

::

    +CIPRECVLEN:<data length of link0>,<data length of link1>,<data length of link2>,<data length of link3>,<data length of link4>
    OK

参数
^^^^

- **<data length of link>**：某一连接中缓冲的所有的数据长度。

说明
^^^^

-  SSL 连接中，ESP-AT 返回的数据长度可能会小于真实数据的长度。

示例
^^^^

::

    AT+CIPRECVLEN?
    +CIPRECVLEN:100,,,,,
    OK

.. _cmd-CIPPING:

:ref:`AT+PING <TCPIP-AT>`：ping 对端主机
----------------------------------------------------

设置命令
^^^^^^^^

**功能：**

ping 对端主机

**命令：**

::

    AT+PING=<"host">

**响应：**

::

    +PING:<time>

    OK

或

::

    +PING:TIMEOUT   // 只有在域名解析失败或 PING 超时情况下，才会有这个回复

    ERROR

参数
^^^^

- **<"host">**：字符串参数，表示对端主机的 IPv4 地址，IPv6 地址，或域名。
- **<time>**：ping 的响应时间，单位：毫秒。

说明
^^^^

- 如果您想基于 IPv6 网络 Ping 对端主机，请执行以下操作：

  - 确保 AP 支持 IPv6
  - 设置 :ref:`AT+CIPV6=1 <cmd-IPV6>`
  - 通过 :ref:`AT+CWJAP <cmd-JAP>` 命令获取到一个 IPv6 地址
  - （可选）通过 :ref:`AT+CIPSTA? <cmd-IPSTA>` 命令检查 {IDF_TARGET_NAME} 是否获取到 IPv6 地址

- 如果远端主机是域名字符串，则 ping 将先通过 DNS 进行域名解析（优先解析 IPv4 地址），再 ping 对端主机 IP 地址

示例
^^^^

::

    AT+PING="192.168.1.1"
    AT+PING="www.baidu.com"

    // 下一代互联网国家工程中心
    AT+PING="240c::6666"

.. _cmd-DNS:

:ref:`AT+CIPDNS <TCPIP-AT>`：查询/设置 DNS 服务器信息
------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询当前 DNS 服务器信息

**命令：**

::

    AT+CIPDNS?

**响应：**

::

    +CIPDNS:<enable>[,<"DNS IP1">][,<"DNS IP2">][,<"DNS IP3">]
    OK

设置命令
^^^^^^^^

**功能：**

设置 DNS 服务器信息

**命令：**

::

    AT+CIPDNS=<enable>[,<"DNS IP1">][,<"DNS IP2">][,<"DNS IP3">]

**响应：**

::

    OK

或

::

    ERROR

参数
^^^^

-  **<enable>**：设置 DNS 服务器

   -  0: 启用自动获取 DNS 服务器设置，DNS 服务器将会恢复为 ``208.67.222.222`` 和 ``8.8.8.8``，只有当 {IDF_TARGET_NAME} station 完成了 DHCP 过程，DNS 服务器才有可能会更新。
   -  1: 启用手动设置 DNS 服务器信息，如果不设置参数 ``<DNS IPx>`` 的值，则使用默认值 ``208.67.222.222`` 和 ``8.8.8.8``。

-  **<DNS IP1>**：第一个 DNS 服务器 IP 地址，对于设置命令，只有当 <enable> 参数为 1 时，也就是启用手动 DNS 设置，本参数才有效；如果设置 <enable> 为 1，并为本参数设置一个值，当您运行查询命令时，ESP-AT 将把该参数作为当前的 DNS 设置返回。
-  **<DNS IP2>**：第二个 DNS 服务器 IP 地址，对于设置命令，只有当 <enable> 参数为 1 时，也就是启用手动 DNS 设置，本参数才有效；如果设置 <enable> 为 1，并为本参数设置一个值，当您运行查询命令时，ESP-AT 将把该参数作为当前的 DNS 设置返回。
-  **<DNS IP3>**：第三个 DNS 服务器 IP 地址，对于设置命令，只有当 <enable> 参数为 1 时，也就是启用手动 DNS 设置，本参数才有效；如果设置 <enable> 为 1，并为本参数设置一个值，当您运行查询命令时，ESP-AT 将把该参数作为当前的 DNS 设置返回。

说明
^^^^

-  若 :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`，配置更改将保存在 NVS 区。
-  这三个参数不能设置在同一个服务器上。
-  当 ``<enable>`` 为 0 时，DNS 服务器可能会根据 {IDF_TARGET_NAME} 设备所连接的路由器的配置而改变。

示例
^^^^

::

    AT+CIPDNS=0
    AT+CIPDNS=1,"208.67.222.222","114.114.114.114","8.8.8.8"

    // 第一个基于 IPv6 的 DNS 服务器：下一代互联网国家工程中心
    // 第二个基于 IPv6 的 DNS 服务器：google-public-dns-a.google.com
    // 第三个基于 IPv6 的 DNS 服务器：江苏省主 DNS 服务器
    AT+CIPDNS=1,"240c::6666","2001:4860:4860::8888","240e:5a::6666"

.. _cmd-MDNS:

:ref:`AT+MDNS <WiFi-AT>`：设置 mDNS 功能
------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+MDNS=<enable>[,<"hostname">,<"service_type">,<port>][,<"instance">][,<"proto">][,<txt_number>][,<"key">,<"value">][...]

**响应：**

::

    OK

参数
^^^^

- **<enable>**：

   - 1：开启 mDNS 功能，后续参数需要填写
   - 0：关闭 mDNS 功能，后续参数无需填写

- **<"hostname">**：mDNS 主机名称。
- **<"service_type">**：mDNS 服务类型。
- **<port>**：mDNS 服务端口。
- **<"instance">**：mDNS 实例名称。默认值：``<"hostname">``。
- **<"proto">**：mDNS 服务协议。建议值：``_tcp`` 或 ``_udp``，默认值：``_tcp``。
- **<txt_number>**：mDNS TXT 记录的数量。范围：[1,10]。
- **<"key">**：TXT 记录的键。
- **<"value">**：TXT 记录的值。
- **[...]**：根据 ``<txt_number>`` 继续填写 TXT 记录的键值对。

示例
^^^^

::

    // 开启 mDNS 功能，主机名为 "espressif"，服务类型为 "_iot"，端口为 8080
    AT+MDNS=1,"espressif","_iot",8080

    // 关闭 mDNS 功能
    AT+MDNS=0

详细示例参考： :ref:`mDNS 示例 <example-mdns>`。

.. _cmd-TCPOPT:

:ref:`AT+CIPTCPOPT <TCPIP-AT>`：查询/设置套接字选项
---------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询当前套接字选项

**命令：**

::

    AT+CIPTCPOPT?

**响应：**

::

    +CIPTCPOPT:<link_id>,<so_linger>,<tcp_nodelay>,<so_sndtimeo>,<keep_alive>
    OK

设置命令
^^^^^^^^

**命令：**

::

    // 单连接：(AT+CIPMUX=0):
    AT+CIPTCPOPT=[<so_linger>],[<tcp_nodelay>],[<so_sndtimeo>][,<keep_alive>]

    // 多连接：(AT+CIPMUX=1):
    AT+CIPTCPOPT=<link ID>,[<so_linger>],[<tcp_nodelay>],[<so_sndtimeo>][,<keep_alive>]

**响应：**

::

    OK

或

::

    ERROR

参数
^^^^

-  **<link_id>**：网络连接 ID (0 ~ max)，在多连接的情况下，若参数值设为 max，则表示所有连接；本参数默认值为 5。
-  **<so_linger>**：配置套接字的 ``SO_LINGER`` 选项（参考：`SO_LINGER 介绍 <https://man7.org/linux/man-pages/man7/socket.7.html#SO_LINGER>`_），单位：秒，默认值：-1。

   -  = -1: 关闭；
   -  = 0: 开启，linger time = 0；
   -  > 0: 开启，linger time = <so_linger>；

-  **<tcp_nodelay>**：配置套接字的 ``TCP_NODELAY`` 选项（参考：`TCP_NODELAY 介绍 <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_NODELAY>`_），默认值：0。

   -  0: 禁用 TCP_NODELAY
   -  1: 启用 TCP_NODELAY

-  **<so_sndtimeo>**：配置套接字的 ``SO_SNDTIMEO`` 选项（参考：`SO_SNDTIMEO 介绍 <https://man7.org/linux/man-pages/man7/socket.7.html#SO_SNDTIMEO>`_），单位：毫秒，默认值：0。

-  **<keep_alive>**：配置套接字的 ``SO_KEEPALIVE`` 选项（参考：`SO_KEEPALIVE 介绍 <https://man7.org/linux/man-pages/man7/socket.7.html#SO_KEEPALIVE>`_），单位：秒。

  - 范围：[0,7200]。

    - 0：禁用 keep-alive 功能；（默认）
    - 1 ~ 7200：开启 keep-alive 功能。`TCP_KEEPIDLE <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPIDLE>`_ 值为 **<keep_alive>**，`TCP_KEEPINTVL <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPINTVL>`_ 值为 1，`TCP_KEEPCNT <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPCNT>`_ 值为 3。

  -  本命令中的 ``<keep_alive>`` 参数与 :ref:`AT+CIPSTART <cmd-START>` 命令中的 ``<keep_alive>`` 参数相同，最终值由后设置的命令决定。如果运行本命令时不设置 ``<keep_alive>`` 参数，则默认使用上次配置的值。

说明
^^^^

-  在配置套接字选项前，**请充分了解该选项功能，以及配置后可能的影响**。
-  SO_LINGER 选项不建议配置较大的值。例如配置 SO_LINGER 值为 60，则 :ref:`AT+CIPCLOSE <cmd-CLOSE>` 命令在收不到对端 TCP FIN 包情况下，会导致 AT 阻塞 60 秒，从而无法响应其它命令。因此，SO_LINGER 建议保持默认值。
-  TCP_NODELAY 选项适用于吞吐量小但对实时性要求高的场景。开启后，:term:`LwIP` 会加快 TCP 的发送，但如果网络环境较差，会由于重传而导致吞吐降低。因此，TCP_NODELAY 建议保持默认值。
-  SO_SNDTIMEO 选项适用于 :ref:`AT+CIPSTART <cmd-START>` 命令未配置 keepalive 参数的应用场景。配置本选项后，:ref:`AT+CIPSEND <cmd-SEND>`、:ref:`AT+CIPSENDL <cmd-SENDL>`、:ref:`AT+CIPSENDEX <cmd-SENDEX>` 命令将会在该超时内退出，无论是否发送成功。这里，SO_SNDTIMEO 建议配置为 5 ~ 10 秒。
-  SO_KEEPALIVE 选项适用于主动定时检测连接是否断开的应用场景，通常 AT 作为 TCP 服务器时建议配置该选项。配置本选项后，会增加额外的网络带宽。SO_KEEPALIVE 建议配置值不小于 60 秒。
