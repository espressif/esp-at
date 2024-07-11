TCP-IP AT 示例
==================

:link_to_translation:`en:[English]`

本文档主要介绍在 {IDF_TARGET_NAME} 设备上运行 :doc:`../AT_Command_Set/TCP-IP_AT_Commands` 命令的详细示例。

.. contents::
   :local:
   :depth: 1

{IDF_TARGET_NAME} 设备作为 TCP 客户端建立单连接
--------------------------------------------

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 查询 {IDF_TARGET_NAME} 设备 IP 地址。

   命令：

   .. code-block:: none

     AT+CIPSTA?

   响应：

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   说明：

   - 您的查询结果可能与上述响应中的不同。

#. PC 与 {IDF_TARGET_NAME} 设备连接同一个路由。

   在 PC 上使用网络调试工具，创建一个 TCP 服务器。例如 TCP 服务器的 IP 地址为 ``192.168.3.102``，端口为 ``8080``。

#. {IDF_TARGET_NAME} 设备作为客户端通过 TCP 连接到 TCP 服务器，服务器 IP 地址为 ``192.168.3.102``，端口为 ``8080``。

   命令：

   .. code-block:: none

     AT+CIPSTART="TCP","192.168.3.102",8080

   响应：

   .. code-block:: none

     CONNECT

     OK

#. 发送 4 字节数据。

   命令：

   .. code-block:: none

     AT+CIPSEND=4

   响应：

   .. code-block:: none

     OK

     >

   输入 4 字节数据，例如输入数据是 ``test``，之后 AT 将会输出以下信息。

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   说明：

   - 若输入的字节数目超过 ``AT+CIPSEND`` 命令设定的长度 (n)，则系统会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``SEND OK``。

#. 接收 4 字节数据。

   假设 TCP 服务器发送 4 字节的数据（数据为 ``test``），则系统会提示：

   .. code-block:: none

     +IPD,4:test

{IDF_TARGET_NAME} 设备作为 TCP 服务器建立多连接
--------------------------------------------

当 {IDF_TARGET_NAME} 设备作为 TCP 服务器时，必须通过 :ref:`AT+CIPMUX=1 <cmd-MUX>` 命令使能多连接，因为可能有多个 TCP 客户端连接到 {IDF_TARGET_NAME} 设备。

以下是 {IDF_TARGET_NAME} 设备作为 softAP 建立 TCP 服务器的示例；如果是 {IDF_TARGET_NAME} 设备作为 station，可在连接路由器后按照同样方法建立服务器。

#. 设置 Wi-Fi 模式为 softAP。

   命令：

   .. code-block:: none

     AT+CWMODE=2

   响应：

   .. code-block:: none

     OK

#. 使能多连接。

   命令：

   .. code-block:: none

     AT+CIPMUX=1

   响应：

   .. code-block:: none

     OK

#. 设置 softAP。

   命令：

   .. code-block:: none

     AT+CWSAP="ESP32_softAP","1234567890",5,3

   响应：

   .. code-block:: none

     OK

#. 查询 softAP 信息。

   命令：

   .. code-block:: none

     AT+CIPAP?

   响应：

   .. code-block:: none

     AT+CIPAP?
     +CIPAP:ip:"192.168.4.1"
     +CIPAP:gateway:"192.168.4.1"
     +CIPAP:netmask:"255.255.255.0"

     OK

   说明：

   - 您查询到的地址可能与上述响应中的不同。

#. 建立 TCP 服务器，默认端口为 ``333``。

   命令：

   .. code-block:: none

     AT+CIPSERVER=1

   响应：

   .. code-block:: none

     OK

#. PC 连接到 {IDF_TARGET_NAME} 设备的 softAP。

   .. figure:: ../../img/Connect-SoftAP.png
       :scale: 100 %
       :align: center
       :alt: Connect SoftAP

#. 在 PC 上使用网络调试工具创建一个 TCP 客户端，连接到 {IDF_TARGET_NAME} 设备创建的 TCP 服务器。

#. 发送 4 字节数据到网络连接 ID 为 0 的链路上。

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

   - 若输入的字节数目超过 ``AT+CIPSEND`` 命令设定的长度 (n)，则系统会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``SEND OK``。

#. 从网络连接 ID 为 0 的链路上接收 4 字节数据。

   假设 TCP 服务器发送 4 字节的数据（数据为 ``test``），则系统会提示：

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

远端 IP 地址和端口固定的 UDP 通信
-------------------------------------------------

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 查询 {IDF_TARGET_NAME} 设备 IP 地址。

   命令：

   .. code-block:: none

     AT+CIPSTA?

   响应：

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   说明：

   - 您的查询结果可能与上述响应中的不同。

#. PC 与 {IDF_TARGET_NAME} 设备连接到同一个路由。

   在 PC 上使用网络调试工具，创建一个 UDP 传输。例如 PC 的 IP 地址为 ``192.168.3.102``，端口为 ``8080``。

#. 使能多连接。

   命令：

   .. code-block:: none

     AT+CIPMUX=1

   响应：

   .. code-block:: none

     OK

#. 创建 UDP 传输。分配网络连接 ID 为 4，远程 IP 地址为 ``192.168.3.102``，远端端口为 ``8080``，本地端口为 ``1112``，模式为 ``0``。

   .. Important::

     ``AT+CIPSTART`` 命令的参数 ``mode`` 决定了 UDP 通信的远端 IP 地址和端口是否固定。若参数为 0，则代表系统会分配一个特定网络连接 ID，以确保通信过程中远端的 IP 地址和端口不会被改变，且数据发送端和数据接收端不会被其它设备代替。

   命令：

   .. code-block:: none

     AT+CIPSTART=4,"UDP","192.168.3.102",8080,1112,0

   响应：

   .. code-block:: none

     4,CONNECT

     OK

   说明：

   - ``"192.168.3.102"`` 和 ``8080`` 为 UDP 传输的远端 IP 地址和远端端口，也就是 PC 建立的 UDP 配置。
   - ``1112`` 为 {IDF_TARGET_NAME} 设备的 UDP 本地端口，您可自行设置，如不设置则为随机值。
   - ``0`` 表示 UDP 远端 IP 地址和端口是固定的，不能更改。比如有另外一台 PC 创建了 UDP 端并且向 {IDF_TARGET_NAME} 设备端口 1112 发送数据，{IDF_TARGET_NAME} 设备仍然会接收来自 UDP 端口 1112 的数据，如果使用 AT 命令 ``AT+CIPSEND=4,X``，那么数据仍然只会发送到第一台 PC 端。但是如果这个参数未设置为 ``0``，那么数据将会被发送到新的 PC 端。

#. 发送 7 字节数据到网络连接 ID 为 4 的链路上。

   命令：

   .. code-block:: none

     AT+CIPSEND=4,7

   响应：

   .. code-block:: none

     OK

     >

   输入 7 字节数据，例如输入数据是 ``abcdefg``，之后 AT 将会输出以下信息。

   .. code-block:: none

     Recv 7 bytes

     SEND OK

   说明：

   - 若输入的字节数目超过 ``AT+CIPSEND`` 命令设定的长度 (n)，则系统会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``SEND OK``。

#. 从网络连接 ID 为 4 的链路上接收 4 字节数据。

   假设 PC 发送 4 字节的数据（数据为 ``test``），则系统会提示：

   .. code-block:: none

     +IPD,4,4:test

#. 关闭网络连接 ID 为 4 的 UDP 连接。

   命令：

   .. code-block:: none

     AT+CIPCLOSE=4

   响应：

   .. code-block:: none

     4,CLOSED

     OK

远端 IP 地址和端口可变的 UDP 通信
----------------------------------------------------

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 查询 {IDF_TARGET_NAME} 设备 IP 地址。

   命令：

   .. code-block:: none

     AT+CIPSTA?

   响应：

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   说明：

   - 您的查询结果可能与上述响应中的不同。

#. PC 与 {IDF_TARGET_NAME} 设备连接到同一个路由。

   在 PC 上使用网络调试工具，创建一个 UDP 传输。例如 IP 地址为 ``192.168.3.102``，端口为 ``8080``。

#. 使能单连接。

   命令：

   .. code-block:: none

     AT+CIPMUX=0

   响应：

   .. code-block:: none

     OK

#. 创建 UDP 传输。远程 IP 地址为 ``192.168.3.102``，远端端口为 ``8080``，本地端口为 ``1112``，模式为 ``2``。

   命令：

   .. code-block:: none

     AT+CIPSTART="UDP","192.168.3.102",8080,1112,2

   响应：

   .. code-block:: none

     CONNECT

     OK

   说明：

   - ``"192.168.3.102"`` 和 `8080` 为 UDP 传输的远端 IP 地址和远端端口，也就是 PC 建立的 UDP 配置。
   - ``1112`` 为 {IDF_TARGET_NAME} 设备的 UDP 本地端口，您可自行设置，如不设置则为随机值。
   - ``2`` 表示当前 UDP 传输建立后，UDP 传输远端信息仍然会更改；UDP 传输的远端信息会自动更改为最近一次与 {IDF_TARGET_NAME} 设备 UDP 通信的远端 IP 地址和端口。

#. 发送 4 字节数据。

   命令：

   .. code-block:: none

     AT+CIPSEND=4

   响应：

   .. code-block:: none

     OK

     >

   输入 4 字节数据，例如输入数据是 ``test``，之后 AT 将会输出以下信息。

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   说明：

   - 若输入的字节数目超过 ``AT+CIPSEND`` 命令设定的长度 (n)，则系统会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``SEND OK``。

#. 发送 UDP 包给其它 UDP 远端。例如发送 4 字节数据，远端主机的 IP 地址为 ``192.168.3.103``，远端端口为 ``1000``。

   若需要发 UDP 包给其它 UDP 远端，只需指定对方 IP 地址和端口即可。

   命令：

   .. code-block:: none

     AT+CIPSEND=4,"192.168.3.103",1000

   响应：

   .. code-block:: none

     OK

     >

   输入 4 字节数据，例如输入数据是 ``test``，之后 AT 将会输出以下信息。

   .. code-block:: none

     Recv 4 bytes

     SEND OK

#. 接收 4 字节数据。

   假设 PC 发送 4 字节的数据（数据为 ``test``），则系统会提示：

   .. code-block:: none

     +IPD,4:test

#. 关闭 UDP 连接。

   命令：

   .. code-block:: none

     AT+CIPCLOSE

   响应：

   .. code-block:: none

     CLOSED

     OK

{IDF_TARGET_NAME} 设备作为 SSL 客户端建立单连接
--------------------------------------------

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 查询 {IDF_TARGET_NAME} 设备 IP 地址。

   命令：

   .. code-block:: none

     AT+CIPSTA?

   响应：

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   说明：

   - 您的查询结果可能与上述响应中的不同。

#. PC 与 {IDF_TARGET_NAME} 设备连接同一个路由。

#. 在 PC 上使用 OpenSSL 命令，创建一个 SSL 服务器。例如 SSL 服务器的 IP 地址为 ``192.168.3.102``，端口为 ``8070``。

   命令：

   .. code-block:: none

     openssl s_server -cert /home/esp-at/components/customized_partitions/raw_data/server_cert/server_cert.crt -key /home/esp-at/components/customized_partitions/raw_data/server_key/server.key -port 8070

   响应：

   .. code-block:: none

     ACCEPT

#. {IDF_TARGET_NAME} 设备作为客户端通过 SSL 连接到 SSL 服务器，服务器 IP 地址为 ``192.168.3.102``，端口为 ``8070``。

   命令：

   .. code-block:: none

     AT+CIPSTART="SSL","192.168.3.102",8070

   响应：

   .. code-block:: none

     CONNECT

     OK

#. 发送 4 字节数据。

   命令：

   .. code-block:: none

     AT+CIPSEND=4

   响应：

   .. code-block:: none

     OK

     >

   输入 4 字节数据，例如输入数据是 ``test``，之后 AT 将会输出以下信息。

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   说明：

   - 若输入的字节数目超过 ``AT+CIPSEND`` 命令设定的长度 (n)，则系统会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``SEND OK``。

#. 接收 4 字节数据。

   假设 TCP 服务器发送 4 字节的数据（数据为 ``test``），则系统会提示：

   .. code-block:: none

     +IPD,4:test

{IDF_TARGET_NAME} 设备作为 SSL 服务器建立多连接
--------------------------------------------

当 {IDF_TARGET_NAME} 设备作为 SSL 服务器时，必须通过 :ref:`AT+CIPMUX=1 <cmd-MUX>` 命令使能多连接，因为可能有多个客户端连接到 {IDF_TARGET_NAME} 设备。

以下是 {IDF_TARGET_NAME} 设备作为 softAP 建立 SSL 服务器的示例；如果是 {IDF_TARGET_NAME} 设备作为 station，可在连接路由器后，参照本示例中的建立连接 SSL 服务器的相关步骤。

#. 设置 Wi-Fi 模式为 softAP。

   命令：

   .. code-block:: none

     AT+CWMODE=2

   响应：

   .. code-block:: none

     OK

#. 使能多连接。

   命令：

   .. code-block:: none

     AT+CIPMUX=1

   响应：

   .. code-block:: none

     OK

#. 配置 {IDF_TARGET_NAME} softAP。

   命令：

   .. code-block:: none

     AT+CWSAP="ESP32_softAP","1234567890",5,3

   响应：

   .. code-block:: none

     OK

#. 查询 softAP 信息。

   命令：

   .. code-block:: none

     AT+CIPAP?

   响应：

   .. code-block:: none

     AT+CIPAP?
     +CIPAP:ip:"192.168.4.1"
     +CIPAP:gateway:"192.168.4.1"
     +CIPAP:netmask:"255.255.255.0"

     OK

   说明：

   - 您查询到的地址可能与上述响应中的不同。

#. 建立 SSL 服务器，端口为 ``8070``。

   命令：

   .. code-block:: none

     AT+CIPSERVER=1,8070,"SSL"

   响应：

   .. code-block:: none

     OK

#. PC 连接到 {IDF_TARGET_NAME} 设备的 softAP。

   .. figure:: ../../img/Connect-SoftAP.png
       :scale: 100 %
       :align: center
       :alt: Connect SoftAP

#. 在 PC 上使用 OpenSSL 命令，创建一个 SSL 客户端，连接到 {IDF_TARGET_NAME} 设备创建的 SSL 服务器。

   命令：

   .. code-block:: none

     openssl s_client -host 192.168.4.1 -port 8070

   {IDF_TARGET_NAME} 设备上的响应：

   .. code-block:: none

     CONNECT

#. 发送 4 字节数据到网络连接 ID 为 0 的链路上。

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

   - 若输入的字节数目超过 ``AT+CIPSEND`` 命令设定的长度 (n)，则系统会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``SEND OK``。

#. 从网络连接 ID 为 0 的链路上接收 4 字节数据。

   假设 SSL 服务器发送 4 字节的数据（数据为 ``test``），则系统会提示：

   .. code-block:: none

     +IPD,0,4:test

#. 关闭 SSL 连接。

   命令：

   .. code-block:: none

     AT+CIPCLOSE=0

   响应：

   .. code-block:: none

     0,CLOSED

     OK

{IDF_TARGET_NAME} 设备作为 SSL 客户端建立双向认证单连接
---------------------------------------------------------

本示例中使用的证书是 ESP-AT 中默认的证书，您也可以使用自己的证书：

  * 要使用您自己的 SSL 客户端证书，请根据 :doc:`../Compile_and_Develop/How_to_update_pki_config` 文档替换默认的证书。
  * 要使用您自己的 SSL 服务器证书，请用您自己的证书路径替换下面的 SSL 服务器证书。

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 设置 SNTP 服务器。

   命令：

   .. code-block:: none

     AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"

   响应：

   .. code-block:: none

     OK

   说明：

   - 您可以根据自己国家的时区设置 SNTP 服务器。

#. 查询 SNTP 时间。

   命令：

   .. code-block:: none

     AT+CIPSNTPTIME?

   响应：

   .. code-block:: none

     +CIPSNTPTIME:Mon Oct 18 20:12:27 2021 
     OK

   说明：

   - 您可以查询 SNTP 时间与实时时间是否相符来判断您设置的 SNTP 服务器是否生效。

#. 查询 {IDF_TARGET_NAME} 设备 IP 地址。

   命令：

   .. code-block:: none

     AT+CIPSTA?

   响应：

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   说明：

   - 您的查询结果可能与上述响应中的不同。

#. PC 与 {IDF_TARGET_NAME} 设备连接同一个路由。

#. 在 PC 上使用 OpenSSL 命令，创建一个 SSL 服务器。例如 SSL 服务器的 IP 地址为 ``192.168.3.102``，端口为 ``8070``。

   命令：

   .. code-block:: none

     openssl s_server -CAfile /home/esp-at/components/customized_partitions/raw_data/server_ca/server_ca.crt -cert /home/esp-at/components/customized_partitions/raw_data/server_cert/server_cert.crt -key /home/esp-at/components/customized_partitions/raw_data/server_key/server.key -port 8070 -verify_return_error -verify_depth 1 -Verify 1

   {IDF_TARGET_NAME} 设备上的响应：

   .. code-block:: none

     ACCEPT

   说明：

   - 命令中的证书路径可以根据你的证书位置进行调整。

#. {IDF_TARGET_NAME} 设备设置 SSL 客户端双向认证配置。

   命令：

   .. code-block:: none

     AT+CIPSSLCCONF=3,0,0

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} 设备作为客户端通过 SSL 连接到 SSL 服务器，服务器 IP 地址为 ``192.168.3.102``，端口为 ``8070``。

   命令：

   .. code-block:: none

     AT+CIPSTART="SSL","192.168.3.102",8070

   响应：

   .. code-block:: none

     CONNECT

     OK

#. 发送 4 字节数据。

   命令：

   .. code-block:: none

     AT+CIPSEND=4

   响应：

   .. code-block:: none

     OK

     >

   输入 4 字节数据，例如输入数据是 ``test``，之后 AT 将会输出以下信息。

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   说明：

   - 若输入的字节数目超过 ``AT+CIPSEND`` 命令设定的长度 (n)，则系统会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``SEND OK``。

#. 接收 4 字节数据。

   假设 TCP 服务器发送 4 字节的数据（数据为 ``test``），则系统会提示：

   .. code-block:: none

     +IPD,4:test

{IDF_TARGET_NAME} 设备作为 SSL 服务器建立双向认证多连接
----------------------------------------------------------

当 {IDF_TARGET_NAME} 设备作为 SSL 服务器时，必须通过 :ref:`AT+CIPMUX=1 <cmd-MUX>` 命令使能多连接，因为可能有多个客户端连接到 {IDF_TARGET_NAME} 设备。

以下是 {IDF_TARGET_NAME} 设备作为 station 建立 SSL 服务器的示例；如果是 {IDF_TARGET_NAME} 设备作为 softAP，可参考 ``{IDF_TARGET_NAME} 设备作为 SSL 服务器建立多连接`` 示例。

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 查询 {IDF_TARGET_NAME} 设备 IP 地址。

   命令：

   .. code-block:: none

     AT+CIPSTA?

   响应：

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   说明：

   - 您的查询结果可能与上述响应中的不同。

#. 使能多连接。

   命令：

   .. code-block:: none

     AT+CIPMUX=1

   响应：

   .. code-block:: none

     OK

#. 建立 SSL 服务器，端口为 ``8070``。

   命令：

   .. code-block:: none

     AT+CIPSERVER=1,8070,"SSL",1

   响应：

   .. code-block:: none

     OK

#. PC 与 {IDF_TARGET_NAME} 设备连接同一个路由。

   .. figure:: ../../img/Connect-SoftAP.png
       :scale: 100 %
       :align: center
       :alt: Connect SoftAP

#. 在 PC 上使用 OpenSSL 命令，创建一个 SSL 客户端，连接到 {IDF_TARGET_NAME} 设备创建的 SSL 服务器。

   命令：

   .. code-block:: none

     openssl s_client -CAfile /home/esp-at/components/customized_partitions/raw_data/client_ca/client_ca_00.crt -cert /home/esp-at/components/customized_partitions/raw_data/client_cert/client_cert_00.crt -key /home/esp-at/components/customized_partitions/raw_data/client_key/client_key_00.key -host 192.168.3.112 -port 8070

   {IDF_TARGET_NAME} 设备上的响应：

   .. code-block:: none

     0,CONNECT

#. 发送 4 字节数据到网络连接 ID 为 0 的链路上。

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

   - 若输入的字节数目超过 ``AT+CIPSEND`` 命令设定的长度 (n)，则系统会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``SEND OK``。

#. 从网络连接 ID 为 0 的链路上接收 4 字节数据。

   假设 SSL 服务器发送 4 字节的数据（数据为 ``test``），则系统会提示：

   .. code-block:: none

     +IPD,0,4:test

#. 关闭 SSL 连接。

   命令：

   .. code-block:: none

     AT+CIPCLOSE=0

   响应：

   .. code-block:: none

     0,CLOSED

     OK

#. 关闭 SSL 服务端。

   命令：

   .. code-block:: none

     AT+CIPSERVER=0

   响应：

   .. code-block:: none

     OK

{IDF_TARGET_NAME} 设备作为 TCP 客户端，建立单连接，实现 UART Wi-Fi 透传
-----------------------------------------------------------------------------------------

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 查询 {IDF_TARGET_NAME} 设备 IP 地址。

   命令：

   .. code-block:: none

     AT+CIPSTA?

   响应：

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   说明：

   - 您的查询结果可能与上述响应中的不同。

#. PC 与 {IDF_TARGET_NAME} 设备连接到同一个路由。

   在 PC 上使用网络调试工具，创建一个 TCP 服务器。例如 IP 地址为 ``192.168.3.102``，端口为 ``8080``。

#. {IDF_TARGET_NAME} 设备作为客户端通过 TCP 连接到 TCP 服务器，服务器 IP 地址为 ``192.168.3.102``，端口为 ``8080``。

   命令：

   .. code-block:: none

     AT+CIPSTART="TCP","192.168.3.102",8080

   响应：

   .. code-block:: none

     CONNECT

     OK

#. 进入 UART Wi-Fi :term:`透传接收模式`。

   命令：

   .. code-block:: none

     AT+CIPMODE=1

   响应：

   .. code-block:: none

     OK

#. 进入 UART Wi-Fi :term:`透传模式` 并发送数据。

   命令：

   .. code-block:: none

     AT+CIPSEND

   响应：

   .. code-block:: none

     OK

     >

#. 停止发送数据

   在透传发送数据过程中，若识别到单独的一包数据 ``+++``，则系统会退出透传发送。此时请至少等待 1 秒，再发下一条 AT 命令。请注意，如果直接用键盘打字输入 ``+++``，有可能因时间太慢而不能被识别为连续的三个 ``+``。更多介绍请参考 :ref:`[仅适用透传模式] +++ <cmd-PLUS>`。

   .. Important::

     使用 ``+++`` 可退出 :term:`透传模式`，回到 :term:`透传接收模式`，此时 TCP 连接仍然有效。您也可以使用 ``AT+CIPSEND`` 命令恢复透传。

#. 退出 UART Wi-Fi :term:`透传接收模式`。

   命令：

   .. code-block:: none

     AT+CIPMODE=0

   响应：

   .. code-block:: none

     OK

#. 关闭 TCP 连接。

   命令：

   .. code-block:: none

     AT+CIPCLOSE

   响应：

   .. code-block:: none

     CLOSED

     OK

{IDF_TARGET_NAME} 设备作为 TCP 服务器，实现 UART Wi-Fi 透传
-----------------------------------------------------------------------------------------

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 设置多连接模式。

   命令：

   .. code-block:: none

     AT+CIPMUX=1

   响应：

   .. code-block:: none

     OK

   说明：

   - TCP 服务器必须在多连接模式下才能开启。

#. 设置 TCP 服务器最大连接数为 1。

   命令：

   .. code-block:: none

     AT+CIPSERVERMAXCONN=1

   响应：

   .. code-block:: none

     OK

   说明：

   - 透传模式是点对点的，因此 TCP 服务器的最大连接数只能是 1。

#. 开启 TCP 服务器。

   命令：

   .. code-block:: none

     AT+CIPSERVER=1,8080

   响应：

   .. code-block:: none

     OK

   说明：

   - 设置 TCP 服务器端口为 8080，您也可以设置为其它端口。

#. 查询 {IDF_TARGET_NAME} 设备 IP 地址。

   命令：

   .. code-block:: none

     AT+CIPSTA?

   响应：

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   说明：

   - 您的查询结果可能与上述响应中的不同。

#. PC 连接到 {IDF_TARGET_NAME} TCP 服务器。

   PC 与 {IDF_TARGET_NAME} 设备连接到同一个路由。

   在 PC 上使用网络调试工具，创建一个 TCP 客户端。连接到 {IDF_TARGET_NAME} 的 TCP 服务器。地址为 ``192.168.3.112``，端口为 ``8080``。

   AT 响应：

   .. code-block:: none

    0,CONNECT

#. 进入 UART Wi-Fi :term:`透传接收模式`。

   命令：

   .. code-block:: none

     AT+CIPMODE=1

   响应：

   .. code-block:: none

     OK

#. 进入 UART Wi-Fi :term:`透传模式` 并发送数据。

   命令：

   .. code-block:: none

     AT+CIPSEND

   响应：

   .. code-block:: none

     OK

     >

#. 停止发送数据

   在透传发送数据过程中，若识别到单独的一包数据 ``+++``，则系统会退出透传发送。此时请至少等待 1 秒，再发下一条 AT 命令。请注意，如果直接用键盘打字输入 ``+++``，有可能因时间太慢而不能被识别为连续的三个 ``+``。更多介绍请参考 :ref:`[仅适用透传模式] +++ <cmd-PLUS>`。

   .. Important::

     使用 ``+++`` 可退出 :term:`透传模式`，回到 :term:`透传接收模式`，此时 TCP 连接仍然有效。您也可以使用 ``AT+CIPSEND`` 命令恢复透传。

#. 退出 UART Wi-Fi :term:`透传接收模式`。

   命令：

   .. code-block:: none

     AT+CIPMODE=0

   响应：

   .. code-block:: none

     OK

#. 关闭 TCP 连接。

   命令：

   .. code-block:: none

     AT+CIPCLOSE

   响应：

   .. code-block:: none

     CLOSED

     OK

{IDF_TARGET_NAME} 设备作为 softAP 在 UDP 传输中实现 UART Wi-Fi 透传
---------------------------------------------------------------------------------------------------------

#. 设置 Wi-Fi 模式为 softAP。

   命令：

   .. code-block:: none

     AT+CWMODE=2

   响应：

   .. code-block:: none

     OK

#. 设置 softAP。

   命令：

   .. code-block:: none

     AT+CWSAP="ESP32_softAP","1234567890",5,3

   响应：

   .. code-block:: none

     OK

#. PC 连接到 {IDF_TARGET_NAME} 设备的 softAP。

   .. figure:: ../../img/Connect-SoftAP.png
       :scale: 100 %
       :align: center
       :alt: Connect SoftAP

#. 创建一个 UDP 端点。

   在 PC 上使用网络调试助手，创建一个 UDP 传输。例如 PC 端 IP 地址为 ``192.168.4.2``，端口为 ``8080``。

#. {IDF_TARGET_NAME} 与 PC 对应端口建立固定对端 IP 地址和端口的 UDP 传输。远程 IP 地址为 ``192.168.4.2``，远端端口为 ``8080``，本地端口为 ``2233``，模式为 ``0``。

   命令：

   .. code-block:: none

     AT+CIPSTART="UDP","192.168.4.2",8080,2233,0

   响应：

   .. code-block:: none

     CONNECT

     OK

#. 进入 UART Wi-Fi :term:`透传接收模式`。

   命令：

   .. code-block:: none

     AT+CIPMODE=1

   响应：

   .. code-block:: none

     OK

#. 进入 UART Wi-Fi :term:`透传模式` 并发送数据。

   命令：

   .. code-block:: none

     AT+CIPSEND

   响应：

   .. code-block:: none

     OK

     >

#. 停止发送数据

   在透传发送数据过程中，若识别到单独的一包数据 ``+++``，则系统会退出透传发送。此时请至少等待 1 秒，再发下一条 AT 命令。请注意，如果直接用键盘打字输入 ``+++``，有可能因时间太慢而不能被识别为连续的三个 ``+``。更多介绍请参考 :ref:`[仅适用透传模式] +++ <cmd-PLUS>`。

   .. Important::

     使用 ``+++`` 可退出 :term:`透传模式`，回到 :term:`透传接收模式`，此时 TCP 连接仍然有效。您也可以使用 ``AT+CIPSEND`` 命令恢复透传。

#. 退出 UART Wi-Fi :term:`透传接收模式`。

   命令：

   .. code-block:: none

     AT+CIPMODE=0

   响应：

   .. code-block:: none

     OK

#. 关闭 TCP 连接。

   命令：

   .. code-block:: none

     AT+CIPCLOSE

   响应：

   .. code-block:: none

     CLOSED

     OK

.. _example-passive_recv:

{IDF_TARGET_NAME} 设备获取被动接收模式下的套接字数据
-----------------------------------------------------------

预计设备将接收大量网络数据并且 MCU 端来不及处理时，可以参考该示例，使用被动接收数据模式。

.. _using-passive-mode:

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 查询 {IDF_TARGET_NAME} 设备 IP 地址。

   命令：

   .. code-block:: none

     AT+CIPSTA?

   响应：

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   说明：

   - 您的查询结果可能与上述响应中的不同。

#. PC 与 {IDF_TARGET_NAME} 设备连接同一个路由。

   在 PC 上使用网络调试工具，创建一个 TCP 服务器。例如 TCP 服务器的 IP 地址为 ``192.168.3.102``，端口为 ``8080``。

#. {IDF_TARGET_NAME} 设备作为客户端通过 TCP 连接到 TCP 服务器，服务器 IP 地址为 ``192.168.3.102``，端口为 ``8080``。

   命令：

   .. code-block:: none

     AT+CIPSTART="TCP","192.168.3.102",8080

   响应：

   .. code-block:: none

     CONNECT

     OK

#. {IDF_TARGET_NAME} 设备设置套接字接收模式为被动模式。

   命令：

   .. code-block:: none

     AT+CIPRECVTYPE=1

   响应：

   .. code-block:: none

     OK

#. TCP 服务器发送 4 字节的数据（数据为 ``test``）。

   说明:

   - 此时会回复 ``+IPD,4``，如果后续再接收到服务器数据，是否回复 ``+IPD,``，请阅读 :ref:`AT+CIPRECVTYPE <cmd-CIPRECVTYPE>` 说明部分。

#. {IDF_TARGET_NAME} 设备查询被动接收模式下套接字数据的长度。

   命令：

   .. code-block:: none

     AT+CIPRECVLEN?

   响应：

   .. code-block:: none

     +CIPRECVLEN:4
     OK

#. {IDF_TARGET_NAME} 设备获取被动接收模式下的套接字数据。

   命令：

   .. code-block:: none

     AT+CIPRECVDATA=4

   响应：

   .. code-block:: none

     +CIPRECVDATA:4,test
     OK

.. _example-mdns:

{IDF_TARGET_NAME} 设备开启 mDNS 功能，PC 通过域名连接到设备的 TCP 服务器
-----------------------------------------------------------------------------

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. PC 与 {IDF_TARGET_NAME} 设备连接同一个路由。

   只有在同一个局域网中，PC 才能发现 {IDF_TARGET_NAME} 设备。

#. 在 PC 上使用 mDNS 工具开启服务发现。例如 Linux 上使用 `avahi-browse <https://linux.die.net/man/1/avahi-browse>`_ （macOS 或 Windows 上使用 `Bonjour <https://developer.apple.com/bonjour/>`_）。

   命令：

   .. code-block:: none

     sudo avahi-browse -a -r

#. {IDF_TARGET_NAME} 设备开启 mDNS 功能。

   命令：

   .. code-block:: none

     AT+MDNS=1,"espressif","_printer",35,"my_instance","_tcp",2,"product","my_printer","firmware_version","AT-V3.4.1.0"

   响应：

   .. code-block:: none

     OK

   说明：

   - 此命令开启 mDNS 功能，表明了设备实例名称为 ``my_instance``，服务类型为 ``_printer``，端口为 ``35``，产品为 ``my_printer``，固件版本为 ``AT-V3.4.1.0``。

#. （可选步骤）PC 端发现 {IDF_TARGET_NAME} 设备。

   PC 端的 ``avahi-browse`` 工具会提示：

   .. code-block:: none

     ...
     + enx000ec6dd4ebf IPv4 my_instance                                   UNIX Printer         local
     = enx000ec6dd4ebf IPv4 my_instance                                   UNIX Printer         local
       hostname = [espressif.local]
       address = [192.168.200.90]
       port = [35]
       txt = ["product=my_printer" "firmware_version=AT-V3.4.1.0"]

   说明：

   - 此步骤不是必须的，只是为了验证 {IDF_TARGET_NAME} 设备的 mDNS 功能是否正常。

#. {IDF_TARGET_NAME} 设备使能多连接。

   命令：

   .. code-block:: none

     AT+CIPMUX=1

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} 设备作为 TCP 服务器，端口为 ``35``。

   命令：

   .. code-block:: none

     AT+CIPSERVER=1,35

   响应：

   .. code-block:: none

     OK

#. 在 PC 上使用 TCP 工具（例如，Linux 或 macOS 上使用 `nc <https://netcat.sourceforge.net/>`_，Windows 上使用 `ncat <https://nmap.org/ncat/>`_），通过域名连接到 {IDF_TARGET_NAME} 设备的 TCP 服务器。

   命令：

   .. code-block:: none

     nc espressif.local 35

   {IDF_TARGET_NAME} 设备响应：

   .. code-block:: none

     0,CONNECT

   说明：

   - 连接建立成功后，PC 和 {IDF_TARGET_NAME} 设备之间立即可以进行数据传输。

#. {IDF_TARGET_NAME} 设备关闭 mDNS 功能。

   命令：

   .. code-block:: none

     AT+MDNS=0

   响应：

   .. code-block:: none

     OK

   说明：

   - 关闭 mDNS 功能能一定程度上减少设备的功耗。
