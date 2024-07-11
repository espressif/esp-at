.. _TCPIP-AT:

TCP/IP AT Commands
==================

:link_to_translation:`zh_CN:[中文]`

-  :ref:`Introduction <cmd-tcpip-intro>`
-  :ref:`AT+CIPV6 <cmd-IPV6>`: Enable/disable the network of Internet Protocol Version 6 (IPv6).
-  :ref:`AT+CIPSTATE <cmd-IPSTATE>`: Obtain the TCP/UDP/SSL connection information.
-  :ref:`AT+CIPSTATUS (deprecated) <cmd-STATUS>`: Obtain the TCP/UDP/SSL connection status and information.
-  :ref:`AT+CIPDOMAIN <cmd-DOMAIN>`: Resolve a Domain Name.
-  :ref:`AT+CIPSTART <cmd-START>`: Establish TCP connection, UDP transmission, or SSL connection.
-  :ref:`AT+CIPSTARTEX <cmd-STARTEX>`: Establish TCP connection, UDP transmission, or SSL connection with an automatically assigned ID.
-  :ref:`[Data Mode Only] +++ <cmd-PLUS>`: Exit from the :term:`data mode`.
-  :ref:`AT+SAVETRANSLINK <cmd-SAVET>`: Set whether to enter Wi-Fi :term:`Passthrough Mode` on power-up.
-  :ref:`AT+CIPSEND <cmd-SEND>`: Send data in the :term:`normal transmission mode` or Wi-Fi :term:`normal transmission mode`.
-  :ref:`AT+CIPSENDL <cmd-SENDL>`: Send long data in paraller in the :term:`normal transmission mode`.
-  :ref:`AT+CIPSENDLCFG <cmd-SENDLCFG>`: Set the configuration for the command :ref:`AT+CIPSENDL <cmd-SENDL>`.
-  :ref:`AT+CIPSENDEX <cmd-SENDEX>`: Send data in the :term:`normal transmission mode` in expanded ways.
-  :ref:`AT+CIPCLOSE <cmd-CLOSE>`: Close TCP/UDP/SSL connection.
-  :ref:`AT+CIFSR <cmd-IFSR>`: Obtain the local IP address and MAC address.
-  :ref:`AT+CIPMUX <cmd-MUX>`: Enable/disable the multiple connections mode.
-  :ref:`AT+CIPSERVER <cmd-SERVER>`: Delete/create a TCP/SSL server.
-  :ref:`AT+CIPSERVERMAXCONN <cmd-SERVERMAX>`: Query/Set the maximum connections allowed by a server.
-  :ref:`AT+CIPMODE <cmd-IPMODE>`: Query/Set the transmission mode.
-  :ref:`AT+CIPSTO <cmd-STO>`: Query/Set the local TCP Server Timeout.
-  :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>`: Query/Set the time zone and SNTP server.
-  :ref:`AT+CIPSNTPTIME <cmd-SNTPT>`: Query the SNTP time.
-  :ref:`AT+CIPSNTPINTV <cmd-SNTPINTV>`: Query/Set the SNTP time synchronization interval.
-  :ref:`AT+CIPFWVER <cmd-FWVER>`: Query the existing AT firmware version on the server.
-  :ref:`AT+CIUPDATE <cmd-UPDATE>`: Upgrade the firmware through Wi-Fi.
-  :ref:`AT+CIPDINFO <cmd-IPDINFO>`: Set "+IPD" message mode.
-  :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`: Query/Set SSL clients.
-  :ref:`AT+CIPSSLCCN <cmd-SSLCCN>`: Query/Set the Common Name of the SSL client.
-  :ref:`AT+CIPSSLCSNI <cmd-SSLCSNI>`: Query/Set SSL client Server Name Indication (SNI).
-  :ref:`AT+CIPSSLCALPN <cmd-SSLCALPN>`: Query/Set SSL client Application Layer Protocol Negotiation (ALPN).
-  :ref:`AT+CIPSSLCPSK <cmd-SSLCPSK>`: Query/Set SSL client Pre-shared Key (PSK) in string format.
-  :ref:`AT+CIPSSLCPSKHEX <cmd-SSLCPSKHEX>`: Query/Set SSL client Pre-shared Key (PSK) in hexadecimal format.
-  :ref:`AT+CIPRECONNINTV <cmd-AUTOCONNINT>`: Query/Set the TCP/UDP/SSL reconnection interval for the Wi-Fi :term:`normal transmission mode`.
-  :ref:`AT+CIPRECVTYPE <cmd-CIPRECVTYPE>`: Query/Set socket receiving mode.
-  :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>`: Obtain socket data in passive receiving mode.
-  :ref:`AT+CIPRECVLEN <cmd-CIPRECVLEN>`: Obtain socket data length in passive receiving mode.
-  :ref:`AT+PING <cmd-CIPPING>`: Ping the remote host.
-  :ref:`AT+CIPDNS <cmd-DNS>`: Query/Set DNS server information.
-  :ref:`AT+MDNS <cmd-MDNS>`: Configure the mDNS function.
-  :ref:`AT+CIPTCPOPT <cmd-TCPOPT>`: Query/Set the socket options.

.. _cmd-tcpip-intro:

Introduction
------------

.. important::
  The default AT firmware supports all the AT commands mentioned on this page. If you need to modify the commands supported by {IDF_TARGET_NAME} by default, please compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections (Each item below is independent. Choose it according to your needs):

  - Disable OTA commands (:ref:`AT+CIUPDATE <cmd-UPDATE>` and :ref:`AT+CIPFWVER <cmd-FWVER>`): ``Component config`` -> ``AT`` -> ``AT OTA command support``
  - Disable PING commands (:ref:`AT+PING <cmd-CIPPING>`): ``Component config`` -> ``AT`` -> ``AT ping command support``
  - Disable mDNS commands (:ref:`AT+MDNS <cmd-MDNS>`): ``Component config`` -> ``AT`` -> ``AT MDNS command support``
  - Disable TCP/IP commands (Not recommended. Once disabled, all TCP/IP functions will be unavailable and you will need to implement these AT commands yourself): ``Component config`` -> ``AT`` -> ``AT net command support``

.. _cmd-IPV6:

:ref:`AT+CIPV6 <TCPIP-AT>`: Enable/disable the network of Internet Protocol Version 6 (IPv6)
--------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query whether IPv6 is enabled.

**Command:**

::

    AT+CIPV6?

**Response:**

::

    +CIPV6:<enable>

    OK

Set Command
^^^^^^^^^^^

**Function:**

Enable/Disable IPv6 network.

**Command:**

::

    AT+CIPV6=<enable>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<enable>**: status of IPv6 network. Default: 0.

   -  0: disable IPv6 network.
   -  1: enable IPv6 network.

Notes
^^^^^

-  You should enable IPv6 network before using IPv6 related upper layer AT commands (TCP/UDP/SSL/PING/DNS based on IPv6 network, also known as TCP6/UDP6/SSL6/PING6/DNS6 or TCPv6/UDPv6/SSLv6/PINGv6/DNSv6).

.. _cmd-IPSTATE:

:ref:`AT+CIPSTATE <TCPIP-AT>`: Obtain the TCP/UDP/SSL Connection Information
----------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+CIPSTATE?

**Response:**

When there is a connection, AT returns:

::

    +CIPSTATE:<link ID>,<"type">,<"remote IP">,<remote port>,<local port>,<tetype>

    OK

When there is no connection, AT returns:

::

    OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~4), used for multiple connections.
-  **<"type">**: string parameter showing the type of transmission: "TCP", "TCPv6", "UDP", "UDPv6", "SSL", or "SSLv6".
-  **<"remote IP">**: string parameter showing the remote IPv4 address or IPv6 address.
-  **<remote port>**: the remote port number.
-  **<local port>**: the local port number.
-  **<tetype>**:

   -  0: {IDF_TARGET_NAME} runs as a client.
   -  1: {IDF_TARGET_NAME} runs as a server.

.. _cmd-STATUS:

:ref:`AT+CIPSTATUS (deprecated) <TCPIP-AT>`: Obtain the TCP/UDP/SSL Connection Status and Information
------------------------------------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+CIPSTATUS

**Response:**

::

    STATUS:<stat>
    +CIPSTATUS:<link ID>,<"type">,<"remote IP">,<remote port>,<local port>,<tetype>
    OK

Parameters
^^^^^^^^^^

-  **<stat>**: status of the {IDF_TARGET_NAME} station interface.

   -  0: The {IDF_TARGET_NAME} station is not initialized.
   -  1: The {IDF_TARGET_NAME} station is initialized, but not started a Wi-Fi connection yet.
   -  2: The {IDF_TARGET_NAME} station is connected to an AP and its IP address is obtained.
   -  3: The {IDF_TARGET_NAME} station has created a TCP/SSL transmission.
   -  4: All of the TCP/UDP/SSL connections of the {IDF_TARGET_NAME} station are disconnected.
   -  5: The {IDF_TARGET_NAME} station started a Wi-Fi connection, but was not connected to an AP or disconnected from an AP.

-  **<link ID>**: ID of the connection (0~4), used for multiple connections.
-  **<"type">**: string parameter showing the type of transmission: "TCP", "TCPv6", "UDP", "UDPv6", "SSL", or "SSLv6".
-  **<"remote IP">**: string parameter showing the remote IPv4 address or IPv6 address.
-  **<remote port>**: the remote port number.
-  **<local port>**: the local port number.
-  **<tetype>**:

   -  0: {IDF_TARGET_NAME} runs as a client.
   -  1: {IDF_TARGET_NAME} runs as a server.

Notes
""""""

- It is recommended to use :ref:`AT+CWSTATE <cmd-WSTATE>` command to query Wi-Fi state and :ref:`AT+CIPSTATE <cmd-IPSTATE>` command to query TCP/UDP/SSL state.

.. _cmd-DOMAIN:

:ref:`AT+CIPDOMAIN <TCPIP-AT>`: Resolve a Domain Name
------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+CIPDOMAIN=<"domain name">[,<ip network>]

**Response:**

::

    +CIPDOMAIN:<"IP address">

    OK

Parameter
^^^^^^^^^^

-  **<"domain name">**: the domain name.
-  **<ip network>**: preferred IP network. Default: 1.

   - 1: preferred resolution of IPv4 address
   - 2: resolve IPv4 address only
   - 3: resolve IPv6 address only

-  **<"IP address">**: the resolved IPv4 address or IPv6 address.

Example
^^^^^^^^

::

    AT+CWMODE=1                       // set the station mode
    AT+CWJAP="SSID","password"        // access to the internet
    AT+CIPDOMAIN="iot.espressif.cn"   // Domain Name Resolution function

    // Domain Name Resolution Function for IPv4 address only
    AT+CIPDOMAIN="iot.espressif.cn",2

    // Domain Name Resolution Function for IPv6 address only
    AT+CIPDOMAIN="ipv6.test-ipv6.com",3

    // Domain Name Resolution Function for compatible IP address
    AT+CIPDOMAIN="ds.test-ipv6.com",1

.. _cmd-START:

:ref:`AT+CIPSTART <TCPIP-AT>`: Establish TCP Connection, UDP Transmission, or SSL Connection
--------------------------------------------------------------------------------------------

* :ref:`esp-at-cipstart-tcp`
* :ref:`esp-at-cipstart-udp`
* :ref:`esp-at-cipstart-ssl`

.. _esp-at-cipstart-tcp:

Establish TCP Connection
^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
""""""""""""

**Command:**

::

    // Single connection (AT+CIPMUX=0):
    AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<keep_alive>][,<"local IP">]

    // Multiple Connections (AT+CIPMUX=1):
    AT+CIPSTART=<link ID>,<"type">,<"remote host">,<remote port>[,<keep_alive>][,<"local IP">]

**Response:**

For single connection, it returns:

::

    CONNECT

    OK

For multiple connections, it returns:

::

    <link ID>,CONNECT

    OK

Parameters
"""""""""""

-  **<link ID>**: ID of network connection (0~4), used for multiple connections. The range of this parameter depends on two configuration items in ``menuconfig``. One is ``AT_SOCKET_MAX_CONN_NUM`` of the ``AT`` component, and its default value is 5. The other is ``LWIP_MAX_SOCKETS`` of the ``LWIP`` component, and its default value is 10. To modify the range of this parameter, you need to set ``AT_SOCKET_MAX_CONN_NUM`` and make sure it is no larger than the value of ``LWIP_MAX_SOCKETS``. (See :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it` for details on configuring and build ESP-AT projects.)
-  **<"type">**: string parameter showing the type of transmission: "TCP", or "TCPv6". Default: "TCP".
-  **<"remote host">**: IPv4 address, IPv6 address, or domain name of remote host. The maximum length is 64 bytes. If you need to use a domain name and the length of the domain name exceeds 64 bytes, use the :ref:`AT+CIPDOMAIN <cmd-DOMAIN>` command to obtain the IP address corresponding to the domain name, and then use the IP address to establish a connection.
-  **<remote port>**: the remote port number.
-  **<keep_alive>**: It configures the `SO_KEEPALIVE <https://man7.org/linux/man-pages/man7/socket.7.html#SO_KEEPALIVE>`__ option for socket. Unit: second.

   - Range: [0,7200].

     - 0: disable keep-alive function (default).
     - 1 ~ 7200: enable keep-alive function. `TCP_KEEPIDLE <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPIDLE>`_ value is **<keep_alive>**, `TCP_KEEPINTVL <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPINTVL>`_ value is 1, and `TCP_KEEPCNT <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPCNT>`_ value is 3.

   - This parameter of this command is the same as the ``<keep_alive>`` parameter of :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` command. It always takes the value set later by either of the two commands. If it is omitted or not set, the last configured value is used by default.

-  **<"local IP">**: the local IPv4 address or IPv6 address that the connection binds. This parameter is useful when you are using multiple network interfaces or multiple IP addresses. By default, it is disabled. If you want to use it, you should specify it first. Null is also valid.

Notes
""""""

- To establish a TCP connection based on an IPv6 network, do as follows:

  -  Make sure that the AP supports IPv6
  -  Set :ref:`AT+CIPV6=1 <cmd-IPV6>`
  -  Obtain an IPv6 address through the :ref:`AT+CWJAP <cmd-JAP>` command
  - (Optional) Check whether {IDF_TARGET_NAME} has obtained an IPv6 address using the :ref:`AT+CIPSTA? <cmd-IPSTA>` command

Example
"""""""""

::

    AT+CIPSTART="TCP","iot.espressif.cn",8000
    AT+CIPSTART="TCP","192.168.101.110",1000
    AT+CIPSTART="TCP","192.168.101.110",2500,60
    AT+CIPSTART="TCP","192.168.101.110",1000,,"192.168.101.100"
    AT+CIPSTART="TCPv6","test-ipv6.com",80
    AT+CIPSTART="TCPv6","fe80::860d:8eff:fe9d:cd90",1000,,"fe80::411c:1fdb:22a6:4d24"

    // esp-at has obtained an IPv6 global address by AT+CWJAP before
    AT+CIPSTART="TCPv6","2404:6800:4005:80b::2004",80,,"240e:3a1:2070:11c0:32ae:a4ff:fe80:65ac"

.. _esp-at-cipstart-udp:

Establish UDP Transmission
^^^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
""""""""""""

**Command:**

::

    // Single connection (AT+CIPMUX=0):
    AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<local port>,<mode>,<"local IP">]

    // Multiple connections (AT+CIPMUX=1):
    AT+CIPSTART=<link ID>,<"type">,<"remote host">,<remote port>[,<local port>,<mode>,<"local IP">]

**Response:**

For single connection, it returns:

::

    CONNECT

    OK

For multiple connections, it returns:

::

    <link ID>,CONNECT

    OK

Parameters
"""""""""""

-  **<link ID>**: ID of network connection (0~4), used for multiple connections.
-  **<"type">**: string parameter showing the type of transmission: "UDP", or "UDPv6". Default: "TCP".
-  **<"remote host">**: IPv4 address, IPv6 address, or domain name of remote host. The maximum length is 64 bytes. If you need to use a domain name and the length of the domain name exceeds 64 bytes, use the :ref:`AT+CIPDOMAIN <cmd-DOMAIN>` command to obtain the IP address corresponding to the domain name, and then use the IP address to establish a connection.
-  **<remote port>**: remote port number.
-  **<local port>**: UDP port of {IDF_TARGET_NAME}.
-  **<mode>**: In the UDP Wi-Fi passthrough, the value of this parameter has to be 0.

   -  0: After UDP data is received, the parameters ``<"remote host">`` and ``<remote port>`` will stay unchanged (default).
   -  1: Only the first time that UDP data is received from an IP address and port that are different from the initially set value of parameters ``<remote host>`` and ``<remote port>``, will they be changed to the IP address and port of the device that sends the data.
   -  2: Each time UDP data is received, the ``<"remote host">`` and ``<remote port>`` will be changed to the IP address and port of the device that sends the data.

-  **<"local IP">**: the local IPv4 address or IPv6 address that the connection binds. This parameter is useful when you are using multiple network interfaces or multiple IP addresses. By default, it is disabled. If you want to use it, you should specify it first. Null is also valid.

Notes
""""""
- If the remote host over the UDP is an IPv4 multicast address (224.0.0.0 ~ 239.255.255.255), the {IDF_TARGET_NAME} will send and receive the UDPv4 multicast.
- If the remote host over the UDP is an IPv4 broadcast address (255.255.255.255), the {IDF_TARGET_NAME} will send and receive the UDPv4 broadcast.
- If the remote host over the UDP is an IPv6 multicast address (FF00:0:0:0:0:0:0:0 ~ FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF), the {IDF_TARGET_NAME} will send and receive the UDP multicast based on IPv6 network.
- To use the parameter ``<mode>``, parameter ``<local port>`` must be set first.

- To establish an UDP transmission based on an IPv6 network, do as follows:

  -  Make sure that the AP supports IPv6
  -  Set :ref:`AT+CIPV6=1 <cmd-IPV6>`
  -  Obtain an IPv6 address through the :ref:`AT+CWJAP <cmd-JAP>` command
  - (Optional) Check whether {IDF_TARGET_NAME} has obtained an IPv6 address using the :ref:`AT+CIPSTA? <cmd-IPSTA>` command

- If you want to receive a UDP packet longer than 1460 bytes, please compile the firmware on your own by following :doc:`Compile ESP-AT Project <../Compile_and_Develop/How_to_clone_project_and_compile_it>` and choosing the following configurations in the Step 5. Configure: ``Component config`` -> ``LWIP`` -> ``Enable reassembly incoming fragmented IP4 packets``.

Example
"""""""""

::

    // UDP unicast
    AT+CIPSTART="UDP","192.168.101.110",1000,1002,2
    AT+CIPSTART="UDP","192.168.101.110",1000,,,"192.168.101.100"

    // UDP unicast based on IPv6 network
    AT+CIPSTART="UDPv6","fe80::32ae:a4ff:fe80:65ac",1000,,,"fe80::5512:f37f:bb03:5d9b"

    // UDP multicast based on IPv6 network
    AT+CIPSTART="UDPv6","FF02::FC",1000,1002,0

.. _esp-at-cipstart-ssl:

Establish SSL Connection
^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
"""""""""""""

**Command:**

::

    // Single connection (AT+CIPMUX=0):
    AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<keep_alive>,<"local IP">]

    // Multiple connections (AT+CIPMUX=1):
    AT+CIPSTART=<link ID>,<"type">,<"remote host">,<remote port>[,<keep_alive>,<"local IP">]

**Response:**

For single connection, it returns:

::

    CONNECT

    OK

For multiple connections, it returns:

::

    <link ID>,CONNECT

    OK

Parameters
"""""""""""

-  **<link ID>**: ID of network connection (0~4), used for multiple connections.
-  **<"type">**: string parameter showing the type of transmission: "SSL", or "SSLv6". Default: "TCP".
-  **<"remote host">**: IPv4 address, IPv6 address, or domain name of remote host. The maximum length is 64 bytes. If you need to use a domain name and the length of the domain name exceeds 64 bytes, use the :ref:`AT+CIPDOMAIN <cmd-DOMAIN>` command to obtain the IP address corresponding to the domain name, and then use the IP address to establish a connection.
-  **<remote port>**: the remote port number.
-  **<keep_alive>**: It configures the `SO_KEEPALIVE <https://man7.org/linux/man-pages/man7/socket.7.html#SO_KEEPALIVE>`__ option for socket. Unit: second.

   - Range: [0,7200].

     - 0: disable keep-alive function (default).
     - 1 ~ 7200: enable keep-alive function. `TCP_KEEPIDLE <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPIDLE>`_ value is **<keep_alive>**, `TCP_KEEPINTVL <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPINTVL>`_ value is 1, and `TCP_KEEPCNT <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPCNT>`_ value is 3.

   - This parameter of this command is the same as the ``<keep_alive>`` parameter of :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` command. It always takes the value set later by either of the two commands. If it is omitted or not set, the last configured value is used by default.

-  **<"local IP">**: the local IPv4 address or IPv6 address that the connection binds. This parameter is useful when you are using multiple network interfaces or multiple IP addresses. By default, it is disabled. If you want to use it, you should specify it first. Null is also valid.

Notes
""""""

-  The number of SSL connections depends on available memory and the maximum number of connections.
-  SSL connection needs a large amount of memory. Insufficient memory may cause the system reboot.
-  If the ``AT+CIPSTART`` is based on an SSL connection and the timeout of each packet is 10 s, the total timeout will be much longer depending on the number of handshake packets.

- To establish a SSL connection based on an IPv6 network, do as follows:

  -  Make sure that the AP supports IPv6
  -  Set :ref:`AT+CIPV6=1 <cmd-IPV6>`
  -  Obtain an IPv6 address through the :ref:`AT+CWJAP <cmd-JAP>` command
  - (Optional) Check whether {IDF_TARGET_NAME} has obtained an IPv6 address using the :ref:`AT+CIPSTA? <cmd-IPSTA>` command

Example
""""""""

::

    AT+CIPSTART="SSL","iot.espressif.cn",8443
    AT+CIPSTART="SSL","192.168.101.110",1000,,"192.168.101.100"

    // esp-at has obtained an IPv6 global address by AT+CWJAP before
    AT+CIPSTART="SSLv6","240e:3a1:2070:11c0:6972:6f96:9147:d66d",1000,,"240e:3a1:2070:11c0:55ce:4e19:9649:b75"

.. _cmd-STARTEX:

:ref:`AT+CIPSTARTEX <TCPIP-AT>`: Establish TCP connection, UDP transmission, or SSL connection with an Automatically Assigned ID
--------------------------------------------------------------------------------------------------------------------------------

This command is similar to :ref:`AT+CIPSTART <cmd-START>` except that you do not need to assign an ID by yourself in multiple connections mode (:ref:`AT+CIPMUX=1 <cmd-MUX>`). The system will assign an ID to the new connection automatically.

.. _cmd-PLUS:

:ref:`[Data Mode Only] +++ <TCPIP-AT>`: Exit from :term:`Data Mode`
-------------------------------------------------------------------

Special Execute Command
^^^^^^^^^^^^^^^^^^^^^^^^

**Function:**

Exit from :term:`Data Mode` and enter the :term:`Command Mode`.

**Command:**

::

    // Only for data mode
    +++

Notes
""""""

-  This special execution command consists of three identical ``+`` characters (0x2b ASCII), and no CR-LF appends to the command tail.
-  Make sure there is more than 20 ms interval before the first ``+`` character, more than 20 ms interval after the third ``+`` character, less than 20 ms interval among the three ``+`` characters. Otherwise, the ``+`` characters will be sent out as normal data.
-  This command returns no reply.
-  Please wait for at least one second before sending the next AT command.

.. _cmd-SEND:

:ref:`AT+CIPSEND <TCPIP-AT>`: Send Data in the :term:`Normal Transmission Mode` or Wi-Fi :term:`Passthrough Mode`
-----------------------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set the data length to be send in the :term:`Normal Transmission Mode`. If the length of data you need to send exceeds 8192 bytes, please use the :ref:`AT+CIPSENDL <cmd-SENDL>` command.

**Command:**

::

    // Single connection: (AT+CIPMUX=0)
    AT+CIPSEND=<length>

    // Multiple connections: (AT+CIPMUX=1)
    AT+CIPSEND=<link ID>,<length>

    // Remote host and port can be set for UDP transmission:
    AT+CIPSEND=[<link ID>,]<length>[,<"remote host">,<remote port>]

**Response:**

::

    OK

    >

This response indicates that AT is ready for receiving serial data. You should enter the data, and when the data length reaches the ``<length>`` value, the transmission of data starts.

If the connection cannot be established or is disrupted during data transmission, the system returns:

::

    ERROR

If the data has been successfully sent to the protocol stack (It does not mean that the data has been sent to the opposite end), the system returns:

::

    SEND OK

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Enter the Wi-Fi :term:`Passthrough Mode`.

**Command:**

::

    AT+CIPSEND

**Response:**

::

    OK
    >

or

::

    ERROR

Enter the Wi-Fi :term:`Passthrough Mode`. The {IDF_TARGET_NAME} can receive 8192 bytes and send 2920 bytes at most each time. If the data received by {IDF_TARGET_NAME} reaches or exceeds 2920 bytes, the data will be immediately sent in chunks of 2920 bytes. Otherwise, it will wait for 20 milliseconds before being sent (You can configure this interval using :ref:`AT+TRANSINTVL <cmd-TRANSINTVL>` command). When a single packet containing :ref:`+++ <cmd-PLUS>` is received, the {IDF_TARGET_NAME} will exit the data sending mode under the Wi-Fi :term:`Passthrough Mode`. Please wait for at least one second before sending the next AT command.

This command can only be used for single connection in the Wi-Fi :term:`Passthrough Mode`. For UDP Wi-Fi passthrough, the ``<mode>`` parameter has to be 0 when using :ref:`AT+CIPSTART <cmd-START>`.

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~4), for multiple connections.
-  **<length>**: data length. Maximum: 8192 bytes.
-  **<"remote host">**: IPv4 address, IPv6 address, or domain name of remote host. It can be set in UDP transmission.
-  **<remote port>**: the remote port number.

Notes
^^^^^

- You can use :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` command to configure socket options for each TCP connection. For example, setting <so_sndtimeo> to 5000 will enable TCP send operation to return results within 5 seconds, regardless of success or failure. This can save the time that the MCU waits for AT command response.

.. _cmd-SENDL:

:ref:`AT+CIPSENDL <TCPIP-AT>`: Send Long Data in Paraller in the :term:`Normal Transmission Mode`.
--------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

In the :term:`Normal Transmission Mode`, set the data length to be sent, and then send data to remote host in parallel (the AT command port receives data in parallel with the AT sending data to the remote host). You can use the :ref:`AT+CIPSENDLCFG <cmd-SENDLCFG>` command to configure this command. If the length of data you need to send is less than 8192 bytes, you also can use the :ref:`AT+CIPSEND <cmd-SEND>` command.

**Command:**

::

    // Single connection: (AT+CIPMUX=0)
    AT+CIPSENDL=<length>

    // Multiple connections: (AT+CIPMUX=1)
    AT+CIPSENDL=<link ID>,<length>

    // Remote host and port can be set for UDP transmission:
    AT+CIPSENDL=[<link ID>,]<length>[,<"remote host">,<remote port>]

**Response:**

::

    OK

    >

This response indicates that AT enters the :term:`Data Mode` and AT command port is ready to receive data. You can enter the data now. Once the port receives data, it will be pushed to underlying protocol stack and the transmission starts.

If the transmission starts, the system reports message according to :ref:`AT+CIPSENDLCFG <cmd-SENDLCFG>` configuration:

::

    +CIPSENDL:<had sent len>,<port recv len>

If the transmission is cancelled by :ref:`+++ <cmd-PLUS>` command, the system returns:

::

    SEND CANCELLED

If not all the data has been sent out, the system finally returns:

::

    SEND FAIL

If the data has been successfully sent to the protocol stack (It does not mean that the data has been sent to the opposite end), the system finally returns:

::

    SEND OK 

When the connection is disconnected, you can send :ref:`+++ <cmd-PLUS>` command to cancel the transmission, then the {IDF_TARGET_NAME} will exit from the :term:`Data Mode`, otherwise, the :term:`Data Mode` will not end until the AT command port receives all the data of the specified ``<length>``.

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~4), for multiple connections.
-  **<length>**: data length. Maximum: 2 :sup:`31` - 1 bytes.
-  **<"remote host">**: IPv4 address, IPv6 address, or domain name of remote host. It can be set in UDP transmission.
-  **<remote port>**: the remote port number.
-  **<had sent len>**: the length of data successfully sent to the underlying protocol stack.
-  **<port recv len>**: data length received by AT command port.

Notes
^^^^^

- It is recommended to use UART flow control, because if the UART receives data at a faster rate than the network sends, data loss may occur.
- You can use :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` command to configure socket options for each TCP connection. For example, setting <so_sndtimeo> to 5000 will enable TCP send operation to return results within 5 seconds, regardless of success or failure. This can save the time that the MCU waits for AT command response.

.. _cmd-SENDLCFG:

:ref:`AT+CIPSENDLCFG <TCPIP-AT>`: Set the Configuration for the Command :ref:`AT+CIPSENDL <cmd-SENDL>`
------------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the configuration of :ref:`AT+CIPSENDL <cmd-SENDL>`.

**Command:**

::

    AT+CIPSENDLCFG?

**Response:**

::

    +CIPSENDLCFG:<report size>,<transmit size>

    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the configuration of :ref:`AT+CIPSENDL <cmd-SENDL>`.

**Command:**

::

    AT+CIPSENDLCFG=<report size>[,<transmit size>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<report size>**: report block size for :ref:`AT+CIPSENDL <cmd-SENDL>`. Default: 1024. Range: [100,2 :sup:`20`]. For example, set ``<report size>`` to 100, ``<had sent len>`` report sequence in the response of :ref:`AT+CIPSENDL <cmd-SENDL>` will be (100, 200, 300, 400, ...). The final ``<had sent len>`` report is always equal to the data length that had been sent out.
-  **<transmit size>**: transmit block size of :ref:`AT+CIPSENDL <cmd-SENDL>`. It specifies the size of the data block sent to the underlying protocol stack. Default: 2920. Range: [100,2920]. If the data received by {IDF_TARGET_NAME} reaches or exceeds ``<transmit size>`` bytes, the data will be immediately sent in chunks of ``<transmit size>`` bytes. Otherwise, it will wait for 20 milliseconds before being sent (You can configure this interval using :ref:`AT+TRANSINTVL <cmd-TRANSINTVL>` command).

Note
""""""

- For devices with small throughput but high real-time requirements, it is recommended to set a smaller ``<transmit size>``. It is also recommended to set ``TCP_NODELAY`` by :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` command.
- For devices with large throughput, it is recommended to set a larger ``<transmit size>``. It is also recommended to read :doc:`How to Improve ESP-AT Throughput Performance <../Compile_and_Develop/How_to_optimize_throughput>` first.

.. _cmd-SENDEX:

:ref:`AT+CIPSENDEX <TCPIP-AT>`: Send Data in the :term:`Normal Transmission Mode` in Expanded Ways
----------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set the data length to be send in :term:`Normal Transmission Mode`, or use ``\0`` (0x5c, 0x30 ASCII) to trigger data transmission.

**Command:**

::

    // Single connection: (AT+CIPMUX=0)
    AT+CIPSENDEX=<length>

    // Multiple connections: (AT+CIPMUX=1)
    AT+CIPSENDEX=<link ID>,<length>

    // Remote host and port can be set for UDP transmission:
    AT+CIPSENDEX=[<link ID>,]<length>[,<"remote host">,<remote port>]

**Response:**

::

    OK

    >

This response indicates that AT is ready for receiving data. You should enter the data of designated length. When the data length reaches the ``<length>`` value, or when the string ``\0`` appears in the data, the transmission starts.

If the connection cannot be established or gets disconnected during transmission, the system returns:

::

    ERROR

If the data has been successfully sent to the protocol stack (It does not mean that the data has been sent to the opposite end), the system returns:

::

    SEND OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~4), for multiple connections.
-  **<length>**: data length. Maximum: 8192 bytes.
-  **<"remote host">**: IPv4 address, IPv6 address, or domain name of remote host. It can be set in UDP transmission.
-  **<remote port>**: remote port can be set in UDP transmission.

Notes
^^^^^^

-  When the requirement of data length is met, or when the string ``\0`` (0x5c, 0x30 in ASCII) appears, the transmission of data starts. Go back to the normal command mode and wait for the next AT command.
-  If the data contains the ``\<any>``, it means that drop backslash symbol and only use ``<any>`` character.
-  When sending ``\0``, please use a backslash to escape it as ``\\0``.
- You can use :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` command to configure socket options for each TCP connection. For example, setting <so_sndtimeo> to 5000 will enable TCP send operation to return results within 5 seconds, regardless of success or failure. This can save the time that the MCU waits for AT command response.

.. _cmd-CLOSE:

:ref:`AT+CIPCLOSE <TCPIP-AT>`: Close TCP/UDP/SSL Connection
-----------------------------------------------------------

Set Command
^^^^^^^^^^^^^

**Function:**

Close TCP/UDP/SSL connection in the multiple connections mode.

**Command:**

::

    AT+CIPCLOSE=<link ID>

**Response:**

::

    <link ID>,CLOSED

    OK

Execute Command
^^^^^^^^^^^^^^^^^

**Function:**

Close TCP/UDP/SSL connection in the single connection mode.

::

    AT+CIPCLOSE

**Response:**

::

    CLOSED

    OK

Parameter
^^^^^^^^^^

-  **<link ID>**: ID of the connection that you want to close. If you set it to 5, all connections will be closed.

.. _cmd-IFSR:

:ref:`AT+CIFSR <TCPIP-AT>`: Obtain the Local IP Address and MAC Address
-----------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+CIFSR    

**Response:**

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

Parameters
^^^^^^^^^^

- **<"APIP">**: IPv4 address of Wi-Fi softAP interface
- **<"APIP6LL">**: Linklocal IPv6 address of Wi-Fi softAP interface
- **<"APIP6GL">**: Global IPv6 address of Wi-Fi softAP interface
- **<"APMAC">**: MAC address of Wi-Fi softAP interface
- **<"STAIP">**: IPv4 address of Wi-Fi station interface
- **<"STAIP6LL">**: Linklocal IPv6 address of Wi-Fi station interface
- **<"STAIP6GL">**: Global IPv6 address of Wi-Fi station interface
- **<"STAMAC">**: MAC address of Wi-Fi station interface
- **<"ETHIP">**: IPv4 address of ethernet interface
- **<"ETHIP6LL">**: Linklocal IPv6 address of ethernet interface
- **<"ETHIP6GL">**: Global IPv6 address of ethernet interface
- **<"ETHMAC">**: MAC address of ethernet interface

Note
^^^^^

-  Only when the {IDF_TARGET_NAME} has the valid interface information can you query its IP address and MAC address.

.. _cmd-MUX:

:ref:`AT+CIPMUX <TCPIP-AT>`: Enable/disable Multiple Connections
----------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the connection type.

**Command:**

::

    AT+CIPMUX?

**Response:**

::

    +CIPMUX:<mode>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the connection type.

**Command:**

::

    AT+CIPMUX=<mode>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<mode>**: connection mode. Default: 0.

   -  0: single connection.
   -  1: multiple connections.

Notes
^^^^^

-  This mode can only be changed after all connections are disconnected.
-  If you want to set the multiple connections mode, {IDF_TARGET_NAME} should be in the :term:`Normal Transmission Mode` (:ref:`AT+CIPMODE=0 <cmd-IPMODE>`).  
-  If you want to set the single connection mode when the TCP/SSL server is running, you should delete the server first. (:ref:`AT+CIPSERVER=0 <cmd-SERVER>`).

Example
^^^^^^^^

::

    AT+CIPMUX=1 

.. _cmd-SERVER:

:ref:`AT+CIPSERVER <TCPIP-AT>`: Delete/create a TCP/SSL Server
--------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the TCP/SSL server status.

**Command:**

::

    AT+CIPSERVER?

**Response:**

::

    +CIPSERVER:<mode>[,<port>,<"type">][,<CA enable>]

    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+CIPSERVER=<mode>[,<param2>][,<"type">][,<CA enable>]

**Response:**

::

    OK  

Parameters
^^^^^^^^^^

-  **<mode>**:

   -  0: delete a server.
   -  1: create a server.

-  **<param2>**: It means differently depending on the parameter ``<mode>``:

  - If ``<mode>`` is 1, ``<param2>`` represents the port number. Default: 333.
  - If ``<mode>`` is 0, ``<param2>`` represents whether the server closes all connections. Default: 0.

    - 0: shutdown the server and keep existing connections.
    - 1: shutdown the server and close all connections.

-  **<"type">**: server type: "TCP", "TCPv6", "SSL", or "SSLv6". Default: "TCP".
-  **<CA enable>**:

   -  0: disable CA.
   -  1: enable CA.

Notes
^^^^^

-  A TCP/SSL server can only be created when multiple connections are activated (:ref:`AT+CIPMUX=1 <cmd-MUX>`).
-  A server monitor will be created automatically when the server is created. Only one server can be created at most.
-  When a client is connected to the server, it will take up one connection and be assigned an ID.
-  If you want to create a TCP/SSL server based on IPv6 network, set :ref:`AT+CIPV6=1 <cmd-IPV6>` first, and obtain an IPv6 address.
-  Parameters ``<"type">`` and ``<CA enable>`` must be omitted when delete a server.

Example
^^^^^^^^

::

    // To create a TCP server
    AT+CIPMUX=1
    AT+CIPSERVER=1,80

    // To create an SSL server
    AT+CIPMUX=1
    AT+CIPSERVER=1,443,"SSL",1

    // To create an SSL server based on IPv6 network
    AT+CIPMUX=1
    AT+CIPSERVER=1,443,"SSLv6",0

    // To delete an server and close all clients
    AT+CIPSERVER=0,1

.. _cmd-SERVERMAX:

:ref:`AT+CIPSERVERMAXCONN <TCPIP-AT>`: Query/Set the Maximum Connections Allowed by a Server
---------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Obtain the maximum number of clients allowed to connect to the TCP/SSL server.

**Command:**

::

    AT+CIPSERVERMAXCONN?

**Response:**

::

    +CIPSERVERMAXCONN:<num>
    OK  

Set Command
^^^^^^^^^^^

**Function:**

Set the maximum number of clients allowed to connect to the TCP/SSL server.

**Command:**

::

    AT+CIPSERVERMAXCONN=<num>

**Response:**

::

    OK  

Parameter
^^^^^^^^^^

-  **<num>**: the maximum number of clients allowed to connect to the TCP/SSL server. Range: [1,5]. For how to change the upper limit of this range, please refer to the description of the ``<link ID>`` parameter of the :ref:`AT+CIPSTART <cmd-START>` command.

Note
^^^^^

-  You should call the command ``AT+CIPSERVERMAXCONN=<num>`` before creating a server.

Example
^^^^^^^^

::

    AT+CIPMUX=1
    AT+CIPSERVERMAXCONN=2
    AT+CIPSERVER=1,80

.. _cmd-IPMODE:

:ref:`AT+CIPMODE <TCPIP-AT>`: Query/Set the Transmission Mode
-------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the transmission mode.

**Command:**

::

    AT+CIPMODE?

**Response:**

::

    +CIPMODE:<mode>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the transmission mode.

**Command:**

::

    AT+CIPMODE=<mode>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<mode>**:

   -  0: :term:`Normal Transmission Mode`.
   -  1: Wi-Fi :term:`Passthrough Receiving Mode`, or called transparent receiving transmission, which can only be enabled in TCP single connection mode, UDP mode when the remote host and port do not change, or SSL single connection mode.

Notes
^^^^^

-  The configuration changes will NOT be saved in flash.
-  After the {IDF_TARGET_NAME} enters the Wi-Fi :term:`Passthrough Receiving Mode`, no Bluetooth function can be used.

Example
^^^^^^^^

::

    AT+CIPMODE=1    

.. _cmd-STO:

:ref:`AT+CIPSTO <TCPIP-AT>`: Query/Set the local TCP/SSL Server Timeout
-----------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the local TCP/SSL server timeout.

**Command:**

::

    AT+CIPSTO?

**Response:**

::

    +CIPSTO:<time>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the local TCP/SSL server timeout.

**Command:**

::

    AT+CIPSTO=<time>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<time>**: local TCP/SSL server timeout. Unit: second. Range: [0,7200].

Notes
^^^^^

-  When a TCP/SSL client does not communicate with the {IDF_TARGET_NAME} server within the ``<time>`` value, the server will terminate this connection.
-  If you set ``<time>`` to 0, the connection will never timeout. This configuration is not recommended.
-  When the client initiates a communication with the server or the server initiate a communication with the client within the set time, the timer will restart. After the timeout expires, the client is closed.

Example
^^^^^^^^

::

    AT+CIPMUX=1
    AT+CIPSERVER=1,1001
    AT+CIPSTO=10

.. _cmd-SNTPCFG:

:ref:`AT+CIPSNTPCFG <TCPIP-AT>`: Query/Set the Time Zone and the SNTP Server
----------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+CIPSNTPCFG?

**Response:**

::

    +CIPSNTPCFG:<enable>,<timezone>,<SNTP server1>[,<SNTP server2>,<SNTP server3>]
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+CIPSNTPCFG=<enable>,<timezone>[,<SNTP server1>,<SNTP server2>,<SNTP server3>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<enable>**: configure the SNTP server:

   -  1: the SNTP server is configured.
   -  0: the SNTP server is not configured.

-  **<timezone>**: support the following two formats:

   -  The first format range is [-12,14]. It marks most of the time zones by offset from Coordinated Universal Time (UTC) in **whole hours** (`UTC–12:00 <https://en.wikipedia.org/wiki/UTC%E2%88%9212:00>`__ to `UTC+14:00 <https://en.wikipedia.org/wiki/UTC%2B14:00>`_).
   -  The second format is ``UTC offset``. The ``UTC offset`` specifies the time value you must add to the UTC time to get a local time value. It has syntax like ``[+|-][hh]mm``. This is negative if the local time zone is on the west of the Prime Meridian and positive if it is on the east. The hour(hh) must be between -12 and 14, and the minute(mm) between 0 and 59. For example, if you want to set the timezone to New Zealand (Chatham Islands) which is in ``UTC+12:45``, you should set the parameter ``<timezone>`` to ``1245``. Please refer to `UTC offset wiki <https://en.wikipedia.org/wiki/Time_zone#List_of_UTC_offsets>`_ for more information.

-  **[<SNTP server1>]**: the first SNTP server.
-  **[<SNTP server2>]**: the second SNTP server.
-  **[<SNTP server3>]**: the third SNTP server.

Note
^^^^^

-  If the three SNTP servers are not configured, one of the following default servers will be used: "cn.ntp.org.cn", "ntp.sjtu.edu.cn", and "us.pool.ntp.org".
-  For the query command, ``<timezone>`` parameter in the response may be different from the ``<timezone>`` parameter in set command. Because the ``<timezone>`` parameter supports the second ``UTC offset`` format, for example, set ``AT+CIPSNTPCFG=1,015``, for query command, ESP-AT ignores the leading zero of the ``<timezone>`` parameter, and the valid value is ``15``. It does not belong to the first format, so it is parsed according to the second ``UTC offset`` format, that is, ``UTC+00:15``, that is, ``timezone`` is 0 in the response.
-  Since SNTP operates over the UDP protocol for sending requests and receiving replies, if there is packet loss in the network, the time synchronization of {IDF_TARGET_NAME} may be delayed. Once the AT command output shows :ref:`+TIME_UPDATED <at-messages-report>`, it indicates that the time has been synchronized, and you can then send the :ref:`AT+CIPSNTPTIME? <cmd-SNTPT>` command to query the current time.

Example
^^^^^^^^

::

    // Enable SNTP server, set timezone to China (UTC+08:00)
    AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"
    or
    AT+CIPSNTPCFG=1,800,"cn.ntp.org.cn","ntp.sjtu.edu.cn"

    // Enable SNTP server, set timezone to New York of the United States (UTC–05:00)
    AT+CIPSNTPCFG=1,-5,"0.pool.ntp.org","time.google.com"
    or
    AT+CIPSNTPCFG=1,-500,"0.pool.ntp.org","time.google.com"

    // Enable SNTP server, set timezone to New Zealand (Chatham Islands, UTC+12:45)
    AT+CIPSNTPCFG=1,1245,"0.pool.ntp.org","time.google.com"

.. _cmd-SNTPT:

:ref:`AT+CIPSNTPTIME <TCPIP-AT>`: Query the SNTP Time
-----------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+CIPSNTPTIME? 

**Response:**

::

    +CIPSNTPTIME:<asctime style time>
    OK

Note
^^^^^

-  The asctime style time is defined at `asctime man page <https://linux.die.net/man/3/asctime>`_.
-  When {IDF_TARGET_NAME} enters Light-sleep or Deep-sleep mode and then wakes up, the system time may become inaccurate. It is recommended to resend the :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>` command to obtain the new time from the NTP server.

.. only:: esp32 or esp32c3 or esp32c6 or esp32s2

  - The time obtained from SNTP is stored in the RTC area, so it will not be lost after a software reset (chip does not lose power).

.. only:: esp32c2

  - The time obtained from SNTP is currently not supported to be stored in the RTC area, so it will be reset to 1970 after a software reset (chip does not lose power).

Example
^^^^^^^^

::

    AT+CWMODE=1
    AT+CWJAP="1234567890","1234567890"
    AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"
    AT+CIPSNTPTIME?
    +CIPSNTPTIME:Tue Oct 19 17:47:56 2021
    OK

    or

    AT+CWMODE=1
    AT+CWJAP="1234567890","1234567890"
    AT+CIPSNTPCFG=1,530
    AT+CIPSNTPTIME?
    +CIPSNTPTIME:Tue Oct 19 15:17:56 2021
    OK

.. _cmd-SNTPINTV:

:ref:`AT+CIPSNTPINTV <TCPIP-AT>`: Query/Set the SNTP time synchronization interval
----------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+CIPSNTPINTV? 

**Response:**

::

    +CIPSNTPINTV:<interval second>

    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+CIPSNTPINTV=<interval second>

**Response:**

::

    OK

Parameters
""""""""""

-  **<interval second>**: the SNTP time synchronization interval. Unit: second. Range: [15,4294967].

Notes
^^^^^

- It configures interval for synchronization, which means that it sets interval how often {IDF_TARGET_NAME} connects to NTP servers to get new time.

Example
^^^^^^^^

::

    AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"

    OK

    // synchronize SNTP time every hour
    AT+CIPSNTPINTV=3600

    OK

.. _cmd-FWVER:

:ref:`AT+CIPFWVER <TCPIP-AT>`: Query the Existing AT Firmware Version on the Server
-----------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the existing {IDF_TARGET_NAME} AT firmware version on the server.

**Command:**

::

    AT+CIPFWVER?

**Response:**

::

    +CIPFWVER:<"version">

    OK

Parameters
^^^^^^^^^^
- **<"version">**: {IDF_TARGET_NAME} AT firmware version.

Notes
^^^^^

- When selecting the OTA version to be upgraded, it is strongly not recommended to upgrade from a high version to a low version.

.. _cmd-UPDATE:

:ref:`AT+CIUPDATE <TCPIP-AT>`: Upgrade Firmware Through Wi-Fi
-------------------------------------------------------------

ESP-AT upgrades firmware at runtime by downloading the new firmware from a specific server through Wi-Fi and then flash it into some partitions.

Query Command
^^^^^^^^^^^^^

**Function:**

Query {IDF_TARGET_NAME} upgrade status.

**Command:**

::

    AT+CIUPDATE?

**Response:**

::

    +CIPUPDATE:<state>

    OK

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Upgrade OTA the latest version of firmware via TCP from the server in blocking mode.

**Command:**

::

    AT+CIUPDATE  

**Response:**

Please refer to the :ref:`response <cmd-UPDATE-RESPONSE>` in the set command.

Set Command
^^^^^^^^^^^

**Function:**

Upgrade the specified version of firmware from the server.

.. _cmd-UPDATE-RESPONSE:

**Command:**

::

    AT+CIUPDATE=<ota mode>[,<version>][,<firmware name>][,<nonblocking>]

**Response:**

If OTA succeeds in blocking mode, the system returns:

::

    +CIPUPDATE:1
    +CIPUPDATE:2
    +CIPUPDATE:3
    +CIPUPDATE:4
    
    OK

If OTA succeeds in non-blocking mode, the system returns:

::

    OK
    +CIPUPDATE:1
    +CIPUPDATE:2
    +CIPUPDATE:3
    +CIPUPDATE:4

If OTA fails in blocking mode, the system returns:

::

    +CIPUPDATE:<state>

    ERROR

If OTA fails in non-blocking mode, the system returns:

::

    OK
    +CIPUPDATE:<state>
    +CIPUPDATE:-1

Parameters
^^^^^^^^^^
- **<ota mode>**:
    
    - 0: OTA via HTTP.
    - 1: OTA via HTTPS. If it does not work, please check whether ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``OTA based upon ssl`` is enabled. For more information, please refer to :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`.

- **<version>**: AT version, such as, ``v1.2.0.0``, ``v1.1.3.0``, ``v1.1.2.0``.
- **<firmware name>**: firmware to upgrade, such as, ``ota``, ``mqtt_ca``, ``client_ca`` or other custom partition in ``at_customize.csv``.
- **<nonblocking>**:

    - 0: OTA by blocking mode (In this mode, you can not send AT command until OTA completes successfully or fails.)
    - 1: OTA by non-blocking mode (You need to manually restart after upgrade done (+CIPUPDATE:4).)

- **<state>**:

    - 1: Server found.
    - 2: Connected to the server.
    - 3: Got the upgrade version.
    - 4: Upgrade done.
    - -1: OTA fails in non-blocking mode.

Notes
^^^^^

-  The speed of the upgrade depends on the network status.
-  If the upgrade fails due to unfavorable network conditions, AT will return ``ERROR``. Please wait for some time before retrying.
-  If you use Espressif's AT `BIN <https://www.espressif.com/en/support/download/at>`_, ``AT+CIUPDATE`` will download a new AT BIN from the Espressif Cloud.
-  If you use a user-compiled AT BIN, you need to implement your own AT+CIUPDATE FOTA function or use :ref:`AT+USEROTA <cmd-USEROTA>` or :ref:`AT+WEBSERVER <cmd-WEBSERVER>` command. ESP-AT project provides an example of `FOTA <https://github.com/espressif/esp-at/blob/master/components/at/src/at_ota_cmd.c>`_.
-  After you upgrade the AT firmware, you are suggested to call the command :ref:`AT+RESTORE <cmd-RESTORE>` to restore the factory default settings.
-  The timeout of OTA process is ``3`` minutes.
-  The response ``OK`` in non-blocking mode does not necessarily come before the response ``+CIPUPDATE:<state>``. It may be output before ``+CIPUPDATE:<state>`` or after it.
-  Downgrading to an older version is not recommended due to potential compatibility issues and the risk of operational failure. If you still prefer downgrading to an older version, please test and verify the functionality based on your product.
-  Please refer to :doc:`../Compile_and_Develop/How_to_implement_OTA_update` for more OTA commands.

Example
^^^^^^^^

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

:ref:`AT+CIPDINFO <TCPIP-AT>`: Set "+IPD" Message Mode
------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+CIPDINFO?

**Response:**

::

    +CIPDINFO:true
    OK
    
or

::
    
    +CIPDINFO:false
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+CIPDINFO=<mode>  

**Response:**

::

    OK  

Parameters
^^^^^^^^^^

-  **<mode>**:

   -  0: does not show the remote host and port in "+IPD" and "+CIPRECVDATA" messages.
   -  1: show the remote host and port in "+IPD" and "+CIPRECVDATA" messages.

Example
^^^^^^^^

::

    AT+CIPDINFO=1

.. _cmd-SSLCCONF:

:ref:`AT+CIPSSLCCONF <TCPIP-AT>`: Query/Set SSL Clients
-------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the configuration of each connection where the {IDF_TARGET_NAME} runs as an SSL client.

**Command:**

::

    AT+CIPSSLCCONF?

**Response:**

::

    +CIPSSLCCONF:<link ID>,<auth_mode>,<pki_number>,<ca_number>
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    // Single connection: (AT+CIPMUX=0)
    AT+CIPSSLCCONF=<auth_mode>[,<pki_number>][,<ca_number>]

    // Multiple connections: (AT+CIPMUX=1)
    AT+CIPSSLCCONF=<link ID>,<auth_mode>[,<pki_number>][,<ca_number>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0 ~ max). For multiple connections, if the value is max, it means all connections. By default, max is 5.
-  **<auth_mode>**:

   -  0: no authentication. In this case ``<pki_number>`` and ``<ca_number>`` are not required.
   -  1: the client provides the client certificate for the server to verify.
   -  2: the client loads CA certificate to verify the server's certificate.
   -  3: mutual authentication.

-  **<pki_number>**: the index of certificate and private key. If there is only one certificate and private key, the value should be 0.
-  **<ca_number>**: the index of CA. If there is only one CA, the value should be 0.

Notes
^^^^^

-  If you want this configuration to take effect immediately, run this command before establishing an SSL connection.
-  The configuration changes will be saved in the NVS area. If you set the command :ref:`AT+SAVETRANSLINK <cmd-SAVET>` to enter SSL Wi-Fi :term:`Passthrough Mode` on power-up, the {IDF_TARGET_NAME} will establish an SSL connection based on this configuration when powered up next time.
-  If you want to use your own certificate or use multiple sets of certificates, please refer to :doc:`../Compile_and_Develop/How_to_update_pki_config`.
-  If ``<auth_mode>`` is configured to 2 or 3, in order to check the server certificate validity period, please make sure {IDF_TARGET_NAME} has obtained the current time before sending the :ref:`AT+CIPSTART <cmd-START>` command. (You can send :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>` command to configure SNTP and obtain the current time, and send :ref:`AT+CIPSNPTIME? <cmd-SNTPT>` command to query the current time.)

.. _cmd-SSLCCN:

:ref:`AT+CIPSSLCCN <TCPIP-AT>`: Query/Set the Common Name of the SSL Client
---------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the common name of the SSL client of each connection.

**Command:**

::

    AT+CIPSSLCCN?

**Response:**

::

    +CIPSSLCCN:<link ID>,<"common name">
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    // Single connection: (AT+CIPMUX=0)
    AT+CIPSSLCCN=<"common name">

    // Multiple connections: (AT+CIPMUX=1)
    AT+CIPSSLCCN=<link ID>,<"common name">

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0 ~ max). For the single connection, the link ID is 0. For multiple connections, if the value is max, it means all connections. Max is 5 by default.
-  **<"common name">**: this parameter is used to verify the Common Name in the certificate sent by the server. The maximum length of ``common name`` is 64 bytes.

Note
^^^^^

-  If you want this configuration to take effect immediately, run this command before establishing the SSL connection.

.. _cmd-SSLCSNI:

:ref:`AT+CIPSSLCSNI <TCPIP-AT>`: Query/Set SSL Client Server Name Indication (SNI)
----------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the SNI configuration of each connection.

**Command:**

::

    AT+CIPSSLCSNI?

**Response:**

::

    +CIPSSLCSNI:<link ID>,<"sni">
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    Single connection: (AT+CIPMUX=0)
    AT+CIPSSLCSNI=<"sni">

    Multiple connections: (AT+CIPMUX=1)
    AT+CIPSSLCSNI=<link ID>,<"sni">

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0 ~ max). For the single connection, the link ID is 0. For multiple connections, if the value is max, it means all connections. Max is 5 by default.
-  **<"sni">**: the Server Name Indication in ClientHello. The maximum length of ``sni`` is 64 bytes.

Notes
^^^^^

-  If you want this configuration to take effect immediately, run this command before establishing the SSL connection.

.. _cmd-SSLCALPN:

:ref:`AT+CIPSSLCALPN <TCPIP-AT>`: Query/Set SSL Client Application Layer Protocol Negotiation (ALPN)
----------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the ALPN configuration of each connection where the {IDF_TARGET_NAME} runs as an SSL client.

**Command:**

::

    AT+CIPSSLCALPN?

**Response:**

::

    +CIPSSLCALPN:<link ID>[,<"alpn">][,<"alpn">][,<"alpn">]

    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    // Single connection: (AT+CIPMUX=0)
    AT+CIPSSLCALPN=<counts>[,<"alpn">][,<"alpn">][,<"alpn">]

    // Multiple connections: (AT+CIPMUX=1)
    AT+CIPSSLCALPN=<link ID>,<counts>[,<"alpn">][,<"alpn">][,<"alpn">]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0 ~ max). For the single connection, the link ID is 0. For multiple connections, if the value is max, it means all connections. Max is 5 by default.
-  **<counts>**: the number of ALPNs. Range: [0,5].

  - 0: clean the ALPN configuration.
  - [1,5]: set the ALPN configuration.

-  **<"alpn">**: a string paramemter showing the ALPN in ClientHello. The maximum length of alpn is limited by the command length.

Note
^^^^^

-  If you want this configuration to take effect immediately, run this command before establishing the SSL connection.

.. _cmd-SSLCPSK:

:ref:`AT+CIPSSLCPSK <TCPIP-AT>`: Query/Set SSL Client Pre-shared Key (PSK) in String Format
-------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the PSK configuration of each connection where the {IDF_TARGET_NAME} runs as an SSL client.

**Command:**

::

    AT+CIPSSLCPSK?

**Response:**

::

    +CIPSSLCPSK:<link ID>,<"psk">,<"hint">
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    // Single connection: (AT+CIPMUX=0)
    AT+CIPSSLCPSK=<"psk">,<"hint">

    // Multiple connections: (AT+CIPMUX=1)
    AT+CIPSSLCPSK=<link ID>,<"psk">,<"hint">

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0 ~ max). For single connection, <link ID> is 0. For multiple connections, if the value is max, it means all connections, max is 5 by default.
-  **<"psk">**: PSK identity in string format. Maximum length: 32. Please use :ref:`AT+CIPSSLCPSKHEX <cmd-SSLCPSKHEX>` command if your ``<"psk">`` parameter contains ``\0`` characters.
-  **<"hint">**: PSK hint. Maximum length: 32.

Notes
^^^^^
-  If you want this configuration to take effect immediately, run this command before establishing the SSL connection.

.. _cmd-SSLCPSKHEX:

:ref:`AT+CIPSSLCPSKHEX <TCPIP-AT>`: Query/Set SSL Client Pre-shared Key (PSK) in Hexadecimal Format
---------------------------------------------------------------------------------------------------

Note
^^^^^
- Similar to the :ref:`AT+CIPSSLCPSK <cmd-SSLCPSK>` command, this command also sets or queries the SSL Client PSK, but its ``<"psk">`` is in hexadecimal format rather than in string format. So, ``\0`` in the ``<"psk">`` parameter means ``00``.

Example
^^^^^^^^

::

    // Single connection: (AT+CIPMUX=0), PSK identity is "psk", PSK hint is "myhint".
    AT+CIPSSLCPSKHEX="70736b","myhint"

    // Multiple connections: (AT+CIPMUX=1), PSK identity is "psk", PSK hint is "myhint".
    AT+CIPSSLCPSKHEX=0,"70736b","myhint"

.. _cmd-AUTOCONNINT:

:ref:`AT+CIPRECONNINTV <TCPIP-AT>`: Query/Set the TCP/UDP/SSL reconnection Interval for the Wi-Fi :term:`Passthrough Mode`
--------------------------------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the automatic connect interval for the Wi-Fi :term:`Passthrough Mode`.

**Command:**

::

    AT+CIPRECONNINTV?

**Response:**

::

    +CIPRECONNINTV:<interval>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the automatic reconnecting interval when TCP/UDP/SSL transmission breaks in the Wi-Fi :term:`Passthrough Mode`.

**Command:**

::

    AT+CIPRECONNINTV=<interval>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<interval>**: the duration between automatic reconnections. Unit: 100 milliseconds. Default: 1. Range: [1,36000]. 

Note
^^^^^

-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.

Example
^^^^^^^^

::

    AT+CIPRECONNINTV=10  

.. _cmd-CIPRECVTYPE:

:ref:`AT+CIPRECVTYPE <TCPIP-AT>`: Query/Set Socket Receiving Mode
-----------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the socket receiving mode.

**Command:**

::

    AT+CIPRECVTYPE?

**Response:**

::

    +CIPRECVTYPE:<link ID>,<mode>

    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    // Single connection: (AT+CIPMUX=0)
    AT+CIPRECVTYPE=<mode>

    // Multiple connections: (AT+CIPMUX=1)
    AT+CIPRECVTYPE=<link ID>,<mode>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0 ~ max). For a single connection, <link ID> is 0. For multiple connections, if the value is max, it means all connections. Max is 5 by default.
- **<mode>**: the receive mode of socket data. Default: 0.
   
   - 0: active mode. ESP-AT will send all the received socket data instantly to the host MCU with the header "+IPD". (The socket receive window is 5760 bytes by default. The maximum valid bytes sent to MCU is 2920 bytes each time.)
   - 1: passive mode. ESP-AT will keep the received socket data in an internal buffer (socket receive window, 5760 bytes by default), and wait for the host MCU to read. If the buffer is full, the socket transmission will be blocked for TCP/SSL connections, or data will be lost for UDP connections.

Notes
^^^^^

-  The configuration can not be used in the Wi-Fi :term:`Passthrough Mode`. If it is a UDP transmission in passive mode, data will be lost when the buffer is full.

-  When ESP-AT receives socket data in passive mode, it will prompt the following messages in different scenarios:

   -  For multiple connections mode (AT+CIPMUX=1), the message is ``+IPD,<link ID>,<len>``.
   -  For single connection mode (AT+CIPMUX=0), the message is ``+IPD,<len>``.

-  ``<len>`` is the total length of socket data in the buffer.
-  You should read data by running :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>` once there is a ``+IPD`` reported. Otherwise, the next ``+IPD`` will not be reported to the host MCU until the previous ``+IPD`` has been read.
-  In case of disconnection, the buffered socket data will still be there and can be read by the MCU until you send :ref:`AT+CIPCLOSE <cmd-CLOSE>` (AT as client) or :ref:`AT+CIPSERVER=0,1 <cmd-SERVER>` (AT as server). In other words, if ``+IPD`` has been reported, the message ``CLOSED`` of this connection will never come until you send :ref:`AT+CIPCLOSE <cmd-CLOSE>` or :ref:`AT+CIPSERVER=0,1 <cmd-SERVER>` or read all data by command :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>`.
-  When a large amount of network data is expected to be received and the MCU cannot process it timely, you can refer to :ref:`example <using-passive-mode>` and use the passive receive data mode.

Example
^^^^^^^^

::

    // Set passive mode in single connection mode
    AT+CIPRECVTYPE=1

    // Set all connections to passive mode in multiple connections mode
    AT+CIPRECVTYPE=5,1

.. _cmd-CIPRECVDATA:

:ref:`AT+CIPRECVDATA <TCPIP-AT>`: Obtain Socket Data in Passive Receiving Mode
-------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    // Single connection: (AT+CIPMUX=0)
    AT+CIPRECVDATA=<len>

    // Multiple connections: (AT+CIPMUX=1)
    AT+CIPRECVDATA=<link_id>,<len>

**Response:**

::

    +CIPRECVDATA:<actual_len>,<data>
    OK

or

::

    +CIPRECVDATA:<actual_len>,<"remote IP">,<remote port>,<data>
    OK

Parameters
^^^^^^^^^^

-  **<link_id>**: connection ID in multiple connections mode.
-  **<len>**: the max value is 0x7fffffff. If the actual length of the received data is less than ``len``, the actual length will be returned.
-  **<actual_len>**: length of the data you actually obtain.
-  **<data>**: the data you want to obtain.
-  **<"remote IP">**: string parameter showing the remote IPv4 address or IPv6 address, enabled by the command :ref:`AT+CIPDINFO=1 <cmd-IPDINFO>`.
-  **<remote port>**: the remote port number, enabled by the command :ref:`AT+CIPDINFO=1 <cmd-IPDINFO>`.

Note
^^^^^

- The command needs to be executed in passive receive mode. Otherwise, ERROR is returned. You can verify whether AT is in passive receive mode by using the :ref:`AT+CIPRECVTYPE <cmd-CIPRECVTYPE>` command.
- When this command is executed without any data available to read, it will directly return ERROR. You can verify if there is readable data at that time by using the :ref:`AT+CIPRECVLEN? <cmd-CIPRECVLEN>` command.
- When executing the command ``AT+CIPRECVDATA=<len>``, at least ``<len> + 128`` bytes of memory are required. You can use the command :ref:`AT+SYSRAM? <Basic-AT>` to check the current available memory. When insufficient memory leads to a memory allocation failure, this command will also return ERROR. You can review the :doc:`AT log output </Get_Started/Hardware_connection>` for ``alloc fail`` or similar print messages to confirm whether a memory allocation failure has occurred.

Example
^^^^^^^^

::

    AT+CIPRECVTYPE=1

    // For example, if host MCU gets a message of receiving 100-byte data in connection with No.0, 
    // the message will be "+IPD,0,100".
    // Then you can read those 100-byte data by using the command below.
    AT+CIPRECVDATA=0,100

.. _cmd-CIPRECVLEN:

:ref:`AT+CIPRECVLEN <TCPIP-AT>`: Obtain Socket Data Length in Passive Receiving Mode
-------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the length of the entire data buffered for the connection.

**Command:**

::

    AT+CIPRECVLEN?

**Response:**

::

    +CIPRECVLEN:<data length of link0>,<data length of link1>,<data length of link2>,<data length of link3>,<data length of link4>
    OK

Parameters
^^^^^^^^^^

- **<data length of link>**: length of the entire data buffered for the connection.

Note
^^^^^

-  For SSL connections, the data length returned by ESP-AT may be less than the actual data length.

Example
^^^^^^^^

::

    AT+CIPRECVLEN?
    +CIPRECVLEN:100,,,,,
    OK

.. _cmd-CIPPING:

:ref:`AT+PING <TCPIP-AT>`: Ping the Remote Host
-----------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Ping the remote host.

**Command:**

::

    AT+PING=<"host">

**Response:**

::

    +PING:<time>

    OK

or

::

    +PING:TIMEOUT   // esp-at returns this response only when the domain name resolution failure or ping timeout

    ERROR

Parameters
^^^^^^^^^^

- **<"host">**: string parameter showing the host IPv4 address or IPv6 address or domain name.
- **<time>**: the response time of ping. Unit: millisecond.

Notes
^^^^^

-  To ping a remote host based on an IPv6 network, do as follows:

  -  Make sure that the AP supports IPv6
  -  Set :ref:`AT+CIPV6=1 <cmd-IPV6>`
  -  Obtain an IPv6 address through the :ref:`AT+CWJAP <cmd-JAP>` command
  - (Optional) Check whether {IDF_TARGET_NAME} has obtained an IPv6 address using the :ref:`AT+CIPSTA? <cmd-IPSTA>` command

- If the remote host is a domain name string, ping will first resolve the domain name (IPv4 address preferred) from DNS (domain name server), and then ping the remote IP address.

Example
^^^^^^^^

::

    AT+PING="192.168.1.1"
    AT+PING="www.baidu.com"

    // China Future Internet Engineering Center
    AT+PING="240c::6666"

.. _cmd-DNS:

:ref:`AT+CIPDNS <TCPIP-AT>`: Query/Set DNS Server Information
-------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the current DNS server information.

**Command:**

::

    AT+CIPDNS?

**Response:**

::

    +CIPDNS:<enable>[,<"DNS IP1">][,<"DNS IP2">][,<"DNS IP3">]
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set DNS server information.

**Command:**

::

    AT+CIPDNS=<enable>[,<"DNS IP1">][,<"DNS IP2">][,<"DNS IP3">]

**Response:**

::

    OK

or

::

    ERROR

Parameters
^^^^^^^^^^

-  **<enable>**: configure DNS server settings

   -  0: Enable automatic DNS server settings from DHCP. The DNS will be restored to ``208.67.222.222`` and ``8.8.8.8``. Only when the {IDF_TARGET_NAME} station completes the DHCP process, the DNS server of the {IDF_TARGET_NAME} station could be updated.
   -  1: Enable manual DNS server settings. If you do not set a value for ``<DNS IPx>``, it will use ``208.67.222.222`` and ``8.8.8.8`` by default.

-  **<"DNS IP1">**: the first DNS server IP address. For the set command, this parameter only works when you set <enable> to 1, i.e. enable manual DNS settings. If you set <enable> to 1 and a value for this parameter, the ESP-AT will return this parameter as the current DNS setting when you run the query command.
-  **<"DNS IP2">**: the second DNS server IP address. For the set command, this parameter only works when you set <enable> to 1, i.e. enable manual DNS settings. If you set <enable> to 1 and a value for this parameter, the ESP-AT will return this parameter as the current DNS setting when you run the query command.
-  **<"DNS IP3">**: the third DNS server IP address. For the set command, this parameter only works when you set <enable> to 1, i.e. enable manual DNS settings. If you set <enable> to 1 and a value for this parameter, the ESP-AT will return this parameter as the current DNS setting when you run the query command.

Notes
^^^^^

-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  The three parameters cannot be set to the same server.
-  When ``<enable>`` is set to 1, the DNS server may change according to the configuration of the router which the {IDF_TARGET_NAME} is connected to.

Example
^^^^^^^^

::

    AT+CIPDNS=0
    AT+CIPDNS=1,"208.67.222.222","114.114.114.114","8.8.8.8"

    // first DNS Server based on IPv6: China Future Internet Engineering Center
    // second DNS Server based on IPv6: google-public-dns-a.google.com
    // third DNS Server based on IPv6: main DNS Server based on IPv6 in JiangSu Province, China
    AT+CIPDNS=1,"240c::6666","2001:4860:4860::8888","240e:5a::6666"

.. _cmd-MDNS:

:ref:`AT+MDNS <WiFi-AT>`: Configure the mDNS Function
------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+MDNS=<enable>[,<"hostname">,<"service_type">,<port>][,<"instance">][,<"proto">][,<txt_number>][,<"key">,<"value">][...]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<enable>**:

   -  1: Enable the mDNS function. The following parameters need to be set.
   -  0: Disable the mDNS function. Please do not set the following parameters.

- **<"hostname">**: mDNS host name.
- **<"service_type">**: mDNS service type.
- **<port>**: mDNS service port.
- **<"instance">**: mDNS instance name. Default: ``<"hostname">``.
- **<"proto">**: mDNS service protocol. Recommended values: ``_tcp`` or ``_udp``. Default: ``_tcp``.
- **<txt_number>**: the number of key-value pairs in the TXT record. Range: [1,10].
- **<"key">**: key of the TXT record.
- **<"value">**: value of the TXT record.
- **[...]**: repeat the key-value pairs of TXT record according to the ``<txt_number>``.

Example
^^^^^^^^

::

    // Enable mDNS function. Set the hostname to "espressif", service type to "_iot", and port to 8080.
    AT+MDNS=1,"espressif","_iot",8080  

    // Disable mDNS function
    AT+MDNS=0

Detailed examples can be found in: :ref:`mDNS Example <example-mdns>`.

.. _cmd-TCPOPT:

:ref:`AT+CIPTCPOPT <TCPIP-AT>`: Query/Set the Socket Options
-------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query current socket options.

**Command:**

::

    AT+CIPTCPOPT?

**Response:**

::

    +CIPTCPOPT:<link_id>,<so_linger>,<tcp_nodelay>,<so_sndtimeo>,<keep_alive>
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    // Single TCP connection (AT+CIPMUX=0):
    AT+CIPTCPOPT=[<so_linger>],[<tcp_nodelay>],[<so_sndtimeo>][,<keep_alive>]

    // Multiple TCP Connections (AT+CIPMUX=1):
    AT+CIPTCPOPT=<link ID>,[<so_linger>],[<tcp_nodelay>],[<so_sndtimeo>][,<keep_alive>]

**Response:**

::

    OK

or

::

    ERROR

Parameters
^^^^^^^^^^

-  **<link_id>**: ID of the connection (0 ~ max). For multiple connections, if the value is max, it means all connections. By default, max is 5.
-  **<so_linger>**: configure the ``SO_LINGER`` options for the socket (refer to `SO_LINGER description <https://man7.org/linux/man-pages/man7/socket.7.html#SO_LINGER>`_). Unit: second. Default: -1.

   -  = -1: off
   -  = 0: on, linger time = 0
   -  > 0: on, linger time = <so_linger>

-  **<tcp_nodelay>**: configure the ``TCP_NODELAY`` option for the socket (refer to `TCP_NODELAY description <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_NODELAY>`_). Default: 0.

   -  0: disable TCP_NODELAY
   -  1: enable TCP_NODELAY

-  **<so_sndtimeo>**: configure the ``SO_SNDTIMEO`` option for socket (refer to `SO_SNDTIMEO description <https://man7.org/linux/man-pages/man7/socket.7.html#SO_SNDTIMEO>`_). Unit: millisecond. Default: 0.

-  **<keep_alive>**: configure the `SO_KEEPALIVE <https://man7.org/linux/man-pages/man7/socket.7.html#SO_KEEPALIVE>`__ option for socket. Unit: second.

   - Range: [0,7200].

     - 0: disable keep-alive function (default).
     - 1 ~ 7200: enable keep-alive function. `TCP_KEEPIDLE <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPIDLE>`_ value is **<keep_alive>**, `TCP_KEEPINTVL <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPINTVL>`_ value is 1, and `TCP_KEEPCNT <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPCNT>`_ value is 3.

   - This parameter of this command is the same as the ``<keep_alive>`` parameter of :ref:`AT+CIPSTART <cmd-START>` command. It always takes the value set later by either of the two commands. If it is omitted or not set, the last configured value is used by default.

Notes
^^^^^

-  Before configuring these socket options, **please make sure you fully understand the function of them and the possible impact after configuration**.
-  The SO_LINGER option is not recommended to be set to a large value. For example, if you set SO_LINGER value to 60, then :ref:`AT+CIPCLOSE <cmd-CLOSE>` command will block for 60 seconds if {IDF_TARGET_NAME} cannot receive TCP FIN packet from the remote TCP peer due to network issues, so {IDF_TARGET_NAME} is unable to respond to any other AT commands. Therefore, it is recommended to keep the default value of the SO_LINGER option.
-  The TCP_NODELAY option is used for situations with small throughput but high real-time requirements. If this option is enabled, :term:`LwIP` will speed up TCP transmission, but in a poor network environment, the throughput will be reduced due to retransmission. Therefore, it is recommended to keep the default value of the TCP_NODELAY option.
-  The SO_SNDTIMEO option is used for situations where the keepalive parameter is not configured in :ref:`AT+CIPSTART <cmd-START>` command. After this option is configured, :ref:`AT+CIPSEND <cmd-SEND>`, :ref:`AT+CIPSENDL <cmd-SENDL>`, and :ref:`AT+CIPSENDEX <cmd-SENDEX>` commands will exit within this timeout, regardless of whether data are sent successfully or not. Here, SO_SNDTIMEO is recommended to be set to 5 ~ 10 seconds.
-  The SO_KEEPALIVE option is used for actively and regularly detecting whether the connection is disconnected. It is generally recommended to configure this option when AT is used as a TCP server. After this option is configured, additional network bandwidth will be cost. Recommended value of SO_KEEPALIVE should be not less than 60 seconds.
