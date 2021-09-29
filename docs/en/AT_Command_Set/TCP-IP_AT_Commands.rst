.. _TCPIP-AT:

TCP/IP AT Commands
==================

:link_to_translation:`zh_CN:[中文]`

-  :ref:`AT+CIPV6 <cmd-IPV6>`: Enable/disable the network of Internet Protocol Version 6 (IPv6).
-  :ref:`AT+CIPSTATE <cmd-IPSTATE>`: Obtain the TCP/UDP/SSL connection information.
-  :ref:`AT+CIPSTATUS (deprecated) <cmd-STATUS>`: Obtain the TCP/UDP/SSL connection status and information.
-  :ref:`AT+CIPDOMAIN <cmd-DOMAIN>`: Resolve a Domain Name.
-  :ref:`AT+CIPSTART <cmd-START>`: Establish TCP connection, UDP transmission, or SSL connection.
-  :ref:`AT+CIPSTARTEX <cmd-STARTEX>`: Establish TCP connection, UDP transmission, or SSL connection with an automatically assigned ID.
-  :ref:`[Passthrough Mode Only] +++ <cmd-PLUS>`: Exit the :term:`passthrough mode`.
-  :ref:`AT+CIPSEND <cmd-SEND>`: Send data in the :term:`normal transmission mode` or Wi-Fi :term:`passthrough mode`.
-  :ref:`AT+CIPSENDEX <cmd-SENDEX>`: Send data in the :term:`normal transmission mode` in expanded ways.
-  :ref:`AT+CIPCLOSE <cmd-CLOSE>`: Close TCP/UDP/SSL connection.
-  :ref:`AT+CIFSR <cmd-IFSR>`: Obtain the local IP address and MAC address.
-  :ref:`AT+CIPMUX <cmd-MUX>`: Enable/disable the multiple connections mode.
-  :ref:`AT+CIPSERVER <cmd-SERVER>`: Delete/create a TCP/SSL server.
-  :ref:`AT+CIPSERVERMAXCONN <cmd-SERVERMAX>`: Query/Set the maximum connections allowed by a server.
-  :ref:`AT+CIPMODE <cmd-IPMODE>`: Query/Set the transmission mode.
-  :ref:`AT+SAVETRANSLINK <cmd-SAVET>`: Set whether to enter Wi-Fi :term:`passthrough mode` on power-up.
-  :ref:`AT+CIPSTO <cmd-STO>`: Query/Set the local TCP Server Timeout.
-  :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>`: Query/Set the time zone and SNTP server.
-  :ref:`AT+CIPSNTPTIME <cmd-SNTPT>`: Query the SNTP time.
-  :ref:`AT+CIUPDATE <cmd-UPDATE>`: Upgrade the firmware through Wi-Fi.
-  :ref:`AT+CIPDINFO <cmd-IPDINFO>`: Set "+IPD" message mode.
-  :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`: Query/Set SSL clients.
-  :ref:`AT+CIPSSLCCN <cmd-SSLCCN>`: Query/Set the Common Name of the SSL client.
-  :ref:`AT+CIPSSLCSNI <cmd-SSLCSNI>`: Query/Set SSL client Server Name Indication (SNI).
-  :ref:`AT+CIPSSLCALPN <cmd-SSLCALPN>`: Query/Set SSL client Application Layer Protocol Negotiation (ALPN).
-  :ref:`AT+CIPSSLCPSK <cmd-SSLCPSK>`: Query/Set SSL client Pre-shared Key (PSK).
-  :ref:`AT+CIPRECONNINTV <cmd-AUTOCONNINT>`: Query/Set the TCP/UDP/SSL reconnection interval for the Wi-Fi :term:`passthrough mode`.
-  :ref:`AT+CIPRECVMODE <cmd-CIPRECVMODE>`: Query/Set socket receiving mode.
-  :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>`: Obtain socket data in passive receiving mode.
-  :ref:`AT+CIPRECVLEN <cmd-CIPRECVLEN>`: Obtain socket data length in passive receiving mode.
-  :ref:`AT+PING <cmd-CIPPING>`: Ping the remote host.
-  :ref:`AT+CIPDNS <cmd-DNS>`: Query/Set DNS server information.
-  :ref:`AT+CIPTCPOPT <cmd-TCPOPT>`: Query/Set the socket options.

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

   -  0: ESP device runs as a client.
   -  1: ESP device runs as a server.

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

-  **<stat>**: status of the ESP station interface.

   -  0: The ESP station is not initialized.
   -  1: The ESP station is initialized, but not started a Wi-Fi connection yet.
   -  2: The ESP station is connected to an AP and its IP address is obtained.
   -  3: The ESP station has created a TCP/SSL transmission.
   -  4: All of the TCP/UDP/SSL connections of the ESP device station are disconnected.
   -  5: The ESP station started a Wi-Fi connection, but was not connected to an AP or disconnected from an AP.

-  **<link ID>**: ID of the connection (0~4), used for multiple connections.
-  **<"type">**: string parameter showing the type of transmission: "TCP", "TCPv6", "UDP", "UDPv6", "SSL", or "SSLv6".
-  **<"remote IP">**: string parameter showing the remote IPv4 address or IPv6 address.
-  **<remote port>**: the remote port number.
-  **<local port>**: the local port number.
-  **<tetype>**:

   -  0: ESP device runs as a client.
   -  1: ESP device runs as a server.

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

Establish TCP Connection
^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
""""""""""""

**Command:**

::

    // Single connection (AT+CIPMUX=0):
    AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<keep alive>][,<"local IP">]

    // Multiple Connections (AT+CIPMUX=1):
    AT+CIPSTART=<link ID>,<"type">,<"remote host">,<remote port>[,<keep alive>][,<"local IP">]

**Response:**

::

    CONNECT

    OK

Parameters
"""""""""""

-  **<link ID>**: ID of network connection (0~4), used for multiple connections.
-  **<"type">**: string parameter showing the type of transmission: "TCP", or "TCPv6". Default: "TCP".
-  **<"remote host">**: string parameter showing the IPv4 address or IPv6 address or domain name of remote host.
-  **<remote port>**: the remote port number.
-  **<keep alive>**: TCP keep-alive interval. Default: 0.

   -  0: disable TCP keep-alive function.
   -  1 ~ 7200: detection interval. Unit: second.

-  **<"local IP">**: the local IPv4 address or IPv6 address that the connection binds. This parameter is useful when you are using multiple network interfaces or multiple IP addresses. By default, it is disabled. If you want to use it, you should specify it first. Null is also valid.

Notes
""""""

- If you want to establish TCP connection based on IPv6 network, set :ref:`AT+CIPV6=1 <cmd-IPV6>` first, and ensure the connected AP by :ref:`AT+CWJAP <cmd-JAP>` supports IPv6 and esp-at got the IPv6 address which you can check it by AT+CIPSTA.
- ``<keep alive>`` parameter will eventually be configured to the socket option ``TCP_KEEPIDLE``. As for other socket options of keepalive, ``TCP_KEEPINTVL`` will use ``1`` by default, and ``TCP_KEEPCNT`` will use ``3`` by default.

Example
"""""""""

::

    AT+CIPSTART="TCP","iot.espressif.cn",8000
    AT+CIPSTART="TCP","192.168.101.110",1000
    AT+CIPSTART="TCP","192.168.101.110",1000,,"192.168.101.100"
    AT+CIPSTART="TCPv6","test-ipv6.com",80
    AT+CIPSTART="TCPv6","fe80::860d:8eff:fe9d:cd90",1000,,"fe80::411c:1fdb:22a6:4d24"

    // esp-at has obtained an IPv6 global address by AT+CWJAP before
    AT+CIPSTART="TCPv6","2404:6800:4005:80b::2004",80,,"240e:3a1:2070:11c0:32ae:a4ff:fe80:65ac"

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

::

    CONNECT

    OK

Parameters
"""""""""""

-  **<link ID>**: ID of network connection (0~4), used for multiple connections.
-  **<"type">**: string parameter showing the type of transmission: "UDP", or "UDPv6". Default: "TCP".
-  **<"remote host">**: string parameter showing the IPv4 address or IPv6 address or domain name of remote host.
-  **<remote port>**: remote port number.
-  **<local port>**: UDP port of ESP devices.
-  **<mode>**: In the UDP Wi-Fi passthrough, the value of this parameter has to be 0.

   -  0: After UDP data is received, the parameters ``<"remote host">`` and ``<remote port>`` will stay unchanged (default).
   -  1: Only the first time that UDP data is received from an IP address and port that are different from the initially set value of parameters ``<remote host>`` and ``<remote port>``, will they be changed to the IP address and port of the device that sends the data.
   -  2: Each time UDP data is received, the ``<"remote host">`` and ``<remote port>`` will be changed to the IP address and port of the device that sends the data.

-  **<"local IP">**: the local IPv4 address or IPv6 address that the connection binds. This parameter is useful when you are using multiple network interfaces or multiple IP addresses. By default, it is disabled. If you want to use it, you should specify it first. Null is also valid.

Notes
""""""
- If the remote host over the UDP is an IPv4 multicast address (224.0.0.0 ~ 239.255.255.255), the ESP device will send and receive the UDPv4 multicast.
- If the remote host over the UDP is an IPv4 broadcast address (255.255.255.255), the ESP device will send and receive the UDPv4 broadcast.
- If the remote host over the UDP is an IPv6 multicast address (FF00:0:0:0:0:0:0:0 ~ FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF), the ESP device will send and receive the UDP multicast based on IPv6 network.
- To use the parameter ``<mode>``, parameter ``<local port>`` must be set first.
- If you want to establish UDP connection based on IPv6 network, set :ref:`AT+CIPV6=1 <cmd-IPV6>` first, and ensure the connected AP by :ref:`AT+CWJAP <cmd-JAP>` supports IPv6 and esp-at got the IPv6 address which you can check it by AT+CIPSTA.

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

Establish SSL Connection
^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
"""""""""""""

**Command:**

::

    AT+CIPSTART=[<link ID>,]<"type">,<"remote host">,<remote port>[,<keep alive>,<"local IP">]

**Response:**

::

    OK

Parameters
"""""""""""

-  **<link ID>**: ID of network connection (0~4), used for multiple connections.
-  **<"type">**: string parameter showing the type of transmission: "SSL", or "SSLv6". Default: "TCP".
-  **<"remote host">**: string parameter showing the IPv4 address or IPv6 address or domain name of remote host.
-  **<remote port>**: the remote port number.
-  **<keep alive>**: reserved item for SSL. Default: 0.
-  **<"local IP">**: the local IPv4 address or IPv6 address that the connection binds. This parameter is useful when you are using multiple network interfaces or multiple IP addresses. By default, it is disabled. If you want to use it, you should specify it first. Null is also valid.

Notes
""""""

-  The number of SSL connections depends on available memory and the maximum number of connections.
-  SSL connection needs a large amount of memory. Insufficient memory may cause the system reboot.
-  If the ``AT+CIPSTART`` is based on an SSL connection and the timeout of each packet is 10 s, the total timeout will be much longer depending on the number of handshake packets.
-  If you want to establish SSL connection based on IPv6 network, set :ref:`AT+CIPV6=1 <cmd-IPV6>` first, and ensure the connected AP by :ref:`AT+CWJAP <cmd-JAP>` supports IPv6 and esp-at got the IPv6 address which you can check it by AT+CIPSTA.
- ``<keep alive>`` parameter will eventually be configured to the socket option ``TCP_KEEPIDLE``. As for other socket options of keepalive, ``TCP_KEEPINTVL`` will use ``1`` by default, and ``TCP_KEEPCNT`` will use ``3`` by default.

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

This command is similar to :ref:`AT+CIPSTART <cmd-START>` except that you don't need to assign an ID by yourself in multiple connections mode (:ref:`AT+CIPMUX=1 <cmd-MUX>`). The system will assign an ID to the new connection automatically.

.. _cmd-PLUS:

:ref:`[Passthrough Mode Only] +++ <TCPIP-AT>`: Exit from :term:`Passthrough Mode`
-----------------------------------------------------------------------------------

Special Execute Command
^^^^^^^^^^^^^^^^^^^^^^^^

**Function:**

Exit from :term:`Passthrough Mode` and enter the :term:`Passthrough Receiving Mode`.

**Command:**

::

    // Only for passthrough mode
    +++

Notes
""""""

-  This special execution command consists of three identical ``+`` characters (0x2b ASCII), and no CR-LF appends to the command tail.
-  Make sure there is more than 20 ms interval before the first ``+`` character, more than 20 ms interval after the third ``+`` character, less than 20 ms interval among the three ``+`` characters. Otherwise, the ``+`` characters will be sent out as normal passthrough data.
-  This command returns no reply.

.. _cmd-SEND:

:ref:`AT+CIPSEND <TCPIP-AT>`: Send Data in the :term:`Normal Transmission Mode` or Wi-Fi :term:`Passthrough Mode`
-----------------------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set the data length to be send in the :term:`Normal Transmission Mode`.

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

If data is transmitted successfully, the system returns:

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

Enter the Wi-Fi :term:`Passthrough Mode`. The ESP devices can receive 8192 bytes and send 2920 bytes at most each time.
If the length of the currently received data is greater than the maximum number of bytes that can be sent, AT will send the received data immediately; Otherwise, the received data will be sent out within 20 ms.
When a single packet containing :ref:`+++ <cmd-PLUS>` is received, the ESP device will exit the data sending mode under the Wi-Fi :term:`Passthrough Mode`. Please wait for at least one second before sending the next AT command.

This command can only be used for single connection in the Wi-Fi :term:`Passthrough Mode`. For UDP Wi-Fi passthrough, the ``<mode>`` parameter has to be 0 when using :ref:`AT+CIPSTART <cmd-START>`.

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~4), for multiple connections.
-  **<length>**: data length. Maximum: 8192 bytes.
-  **<"remote host">**: IPv4 address or IPv6 address or domain name of remote host, can be set in UDP transmission.
-  **<remote port>**: the remote port number.

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

If the data are successfully transmitted, the system returns:

::

    SEND OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~4), for multiple connections.
-  **<length>**: data length. Maximum: 8192 bytes.
-  **<"remote host">**: IPv4 address or IPv6 address or domain name of remote host, can be set in UDP transmission.
-  **<remote port>**: remote port can be set in UDP transmission.

Notes
^^^^^^

-  When the requirement of data length is met, or when the string ``\0`` (0x5c, 0x30 in ASCII) appears, the transmission of data starts. Go back to the normal command mode and wait for the next AT command.
-  If the data contains the ``\<any>``, it means that drop backslash symbol and only use ``<any>`` character.
-  When sending ``\0``, please use a backslash to escape it as ``\\0``.

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

Execute Command
^^^^^^^^^^^^^^^^^

**Function:**

Close TCP/UDP/SSL connection in the single connection mode.

::

    AT+CIPCLOSE

**Response:**

::

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

-  Only when the ESP device has the valid interface information can you query its IP address and MAC address.

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
-  If you want to set the multiple connections mode, ESP devices should be in the :term:`Normal Transmission Mode` (:ref:`AT+CIPMODE=0 <cmd-IPMODE>`).  
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

-  **<num>**: the maximum number of clients allowed to connect to the TCP/SSL server.

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

Example
^^^^^^^^

::

    AT+CIPMODE=1    

.. _cmd-SAVET:

:ref:`AT+SAVETRANSLINK <TCPIP-AT>`: Set Whether to Enter Wi-Fi :term:`Passthrough Mode` on Power-up
----------------------------------------------------------------------------------------------------

For TCP/SSL Single Connection
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
""""""""""""""

**Command:**

::

    AT+SAVETRANSLINK=<mode>,<"remote host">,<remote port>[,<"type">,<keep alive>]

**Response:**

::

    OK

Parameters
""""""""""""""

-  **<mode>**:

   -  0: ESP will NOT enter Wi-Fi :term:`Passthrough Mode` on power-up.
   -  1: ESP will enter Wi-Fi :term:`Passthrough Mode` on power-up.

-  **<"remote host">**: string parameter showing the IPv4 address or IPv6 address or domain name of remote host.
-  **<remote port>**: the remote port number.
-  **<"type">**: string parameter showing the type of transmission: "TCP", "TCPv6", "SSL", or "SSLv6". Default: "TCP".
-  **<keep alive>**: TCP keep-alive interval. Default: 0.

   -  0: disable the keep-alive function.
   -  1 ~ 7200: detection interval. Unit: second.

Notes
"""""""

-  This command will save the Wi-Fi :term:`Passthrough Mode` configuration in the NVS area. If ``<mode>`` is set to 1, ESP device will enter the Wi-Fi :term:`Passthrough Mode` in any subsequent power cycles. The configuration will take effect after ESP reboots.
-  As long as the remote host and port are valid, the configuration will be saved in flash.
-  If you want to establish TCP/SSL connection based on IPv6 network, set :ref:`AT+CIPV6=1 <cmd-IPV6>` first, and ensure the connected AP by :ref:`AT+CWJAP <cmd-JAP>` supports IPv6 and esp-at got the IPv6 address which you can check it by AT+CIPSTA.

Example
""""""""

::

    AT+SAVETRANSLINK=1,"192.168.6.110",1002,"TCP"
    AT+SAVETRANSLINK=1,"www.baidu.com",443,"SSL"
    AT+SAVETRANSLINK=1,"240e:3a1:2070:11c0:55ce:4e19:9649:b75",8080,"TCPv6"
    AT+SAVETRANSLINK=1,"240e:3a1:2070:11c0:55ce:4e19:9649:b75",8080,"SSLv6"

For UDP Transmission
^^^^^^^^^^^^^^^^^^^^

Set Command
""""""""""""""

**Command:**

::

    AT+SAVETRANSLINK=<mode>,<"remote host">,<remote port>,[<"type">,<local port>]

**Response:**

::

    OK

Parameters
""""""""""""""

-  **<mode>**:

   -  0: ESP will NOT enter Wi-Fi :term:`Passthrough Mode` on power-up.
   -  1: ESP will enter Wi-Fi :term:`Passthrough Mode` on power-up.

-  **<"remote host">**: string parameter showing the IPv4 address or IPv6 address or domain name of remote host.
-  **<remote port>**: the remote port number.
-  **<"type">**: string parameter showing the type of transmission: "UDP" or "UDPv6". Default: "TCP".
-  **<local port>**: local port when UDP Wi-Fi passthrough is enabled on power-up.

Notes
"""""""

-  This command will save the Wi-Fi :term:`Passthrough Mode` configuration in the NVS area. If ``<mode>`` is set to 1, ESP device will enter the Wi-Fi :term:`Passthrough Mode` in any subsequent power cycles. The configuration will take effect after ESP reboots.
-  As long as the remote host and port are valid, the configuration will be saved in flash.
-  If you want to establish UDP transmission based on IPv6 network, set :ref:`AT+CIPV6=1 <cmd-IPV6>` first, and ensure the connected AP by :ref:`AT+CWJAP <cmd-JAP>` supports IPv6 and esp-at got the IPv6 address which you can check it by AT+CIPSTA.

Example
"""""""""

::

    AT+SAVETRANSLINK=1,"192.168.6.110",1002,"UDP",1005
    AT+SAVETRANSLINK=1,"240e:3a1:2070:11c0:55ce:4e19:9649:b75",8081,"UDPv6",1005

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

-  When a TCP/SSL client does not communicate with the ESP server within the ``<time>`` value, the server will terminate this connection.
-  If you set ``<time>`` to 0, the connection will never timeout. This configuration is not recommended.
-  When the client initiates a communication with the server within the set time, the timer will restart. After the timeout expires, the client is closed. During the set time, if the server initiate a communication with the client, the timer will not restart. After the timeout expires, the client is closed.

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

Example
^^^^^^^^

::

    AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"

    OK

    AT+CIPSNTPTIME?
    +CIPSNTPTIME:Mon Dec 12 02:33:32 2016
    OK  

.. _cmd-UPDATE:

:ref:`AT+CIUPDATE <TCPIP-AT>`: Upgrade Firmware Through Wi-Fi
-------------------------------------------------------------

ESP-AT upgrades firmware at runtime by downloading the new firmware from a specific server through Wi-Fi and then flash it into some partitions.

Query Command
^^^^^^^^^^^^^

**Function:**

Query ESP device upgrade status.

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

Upgrade OTA the latest version of firmware via TCP/SSL from the server.

**Command:**

::

    AT+CIUPDATE  

**Response:**

::

    +CIPUPDATE:<state>
    OK

or

::

    ERROR

Set Command
^^^^^^^^^^^

**Function:**

Upgrade the specified version of firmware from the server.

**Command:**

::

    AT+CIUPDATE=<ota mode>[,<version>][,<firmware name>][,<nonblocking>]

**Response:**

::

    +CIPUPDATE:<state>
    OK

or

::

    ERROR

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

    - 0: Idle.
    - 1: Server found.
    - 2: Connected to the server.
    - 3: Got the upgrade version.
    - 4: Upgrade done.
    - -1: Upgrade failed.

Notes
^^^^^

-  The speed of the upgrade depends on the network status.
-  If the upgrade fails due to unfavorable network conditions, AT will return ``ERROR``. Please wait for some time before retrying.
-  If you use Espressif's AT `BIN <https://www.espressif.com/en/support/download/at>`_, ``AT+CIUPDATE`` will download a new AT BIN from the Espressif Cloud.
-  If you use a user-compiled AT BIN, you need to implement your own AT+CIUPDATE FOTA function. ESP-AT project provides an example of `FOTA <https://github.com/espressif/esp-at/blob/master/components/at/src/at_ota_cmd.c>`_.
-  After you upgrade the AT firmware, you are suggested to call the command :ref:`AT+RESTORE <cmd-RESTORE>` to restore the factory default settings.
-  Upgraded to an older version is not recommended.

Example
^^^^^^^^

::

    AT+CIUPDATE  
    AT+CIUPDATE=1
    AT+CIUPDATE=1,"v1.2.0.0"
    AT+CIUPDATE=1,"v2.2.0.0","mqtt_ca"
    AT+CIUPDATE=1,"V2.2.0.0","ota",1
    AT+CIUPDATE=1,,,1
    AT+CIUPDATE=1,,"ota",1
    AT+CIUPDATE=1,"V2.2.0.0",,1

.. _cmd-IPDINFO:

:ref:`AT+CIPDINFO <TCPIP-AT>`: Set "+IPD" Message Mode
------------------------------------------------------

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

Query the configuration of each connection where the ESP device runs as an SSL client.

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
-  The configuration changes will be saved in the NVS area. If you set the command :ref:`AT+SAVETRANSLINK <cmd-SAVET>` to enter SSL Wi-Fi :term:`Passthrough Mode` on power-up, the ESP device will establish an SSL connection based on this configuration when powered up next time.

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

Query the ALPN configuration of each connection where the ESP device runs as an SSL client.

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

:ref:`AT+CIPSSLCPSK <TCPIP-AT>`: Query/Set SSL Client Pre-shared Key (PSK)
--------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the PSK configuration of each connection where the ESP device runs as an SSL client.

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
-  **<"psk">**: PSK identity. Maximum length: 32.
-  **<"hint">**: PSK hint. Maximum length: 32.

Notes
^^^^^
-  If you want this configuration to take effect immediately, run this command before establishing the SSL connection.

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

.. _cmd-CIPRECVMODE:

:ref:`AT+CIPRECVMODE <TCPIP-AT>`: Query/Set Socket Receiving Mode
-----------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the socket receiving mode.

**Command:**

::

    AT+CIPRECVMODE?

**Response:**

::

    +CIPRECVMODE:<mode>
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+CIPRECVMODE=<mode>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

- **<mode>**: the receive mode of socket data. Default: 0.
   
   - 0: active mode. ESP-AT will send all the received socket data instantly to the host MCU with header "+IPD".
   - 1: passive mode. ESP-AT will keep the received socket data in an internal buffer (socket receive window, 5760 bytes by default), and wait for the host MCU to read. If the buffer is full, the socket transmission will be blocked for TCP/SSL connections, or data will be lost for UDP connections.

Notes
^^^^^

-  The configuration can not be used in the Wi-Fi :term:`Passthrough Mode`. If it is a UDP transmission in passive mode, data will be lost when the buffer is full.

-  When ESP-AT receives socket data in passive mode, it will prompt the following messages in different scenarios:

   -  For multiple connections mode (AT+CIPMUX=1), the message is ``+IPD,<link ID>,<len>``.
   -  For single connection mode (AT+CIPMUX=0), the message is ``+IPD,<len>``.

-  ``<len>`` is the total length of socket data in the buffer.
-  You should read data by running :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>` once there is a ``+IPD`` reported. Otherwise, the next ``+IPD`` will not be reported to the host MCU until the previous ``+IPD`` has been read.
-  In case of disconnection, the buffered socket data will still be there and can be read by the MCU until you send :ref:`AT+CIPCLOSE <cmd-CLOSE>`. In other words, if ``+IPD`` has been reported, the message ``CLOSED`` of this connection will never come until you send :ref:`AT+CIPCLOSE <cmd-CLOSE>` or read all data by command :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>`.

Example
^^^^^^^^

::

    AT+CIPRECVMODE=1   

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

Example
^^^^^^^^

::

    AT+CIPRECVMODE=1

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

-  For SSL connections, ESP-AT will return the length of the encrypted data, so the returned length will be larger than the real data length.

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
- If you want to ping a remote host based on IPv6 network, set :ref:`AT+CIPV6=1 <cmd-IPV6>` first, and ensure the connected AP by :ref:`AT+CWJAP <cmd-JAP>` supports IPv6 and esp-at got the IPv6 address which you can check it by AT+CIPSTA.
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

    +CIPDNS:<enable>[,<"DNS IP1">,<"DNS IP2">,<"DNS IP3">]
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set DNS server information.

**Command:**

::

    AT+CIPDNS=<enable>[,<"DNS IP1">,<"DNS IP2">,<"DNS IP3">]

**Response:**

::

    OK

or

::

    ERROR

Parameters
^^^^^^^^^^

-  **<enable>**: configure DNS settings

   -  0: Enable automatic DNS settings from DHCP. The DNS will be restored to ``208.67.222.222``. Only when DHCP is updated will it take effect.
   -  1: Enable manual DNS settings. If you do not set a value for ``<DNS IPx>``, it will use ``208.67.222.222`` by default.

-  **<"DNS IP1">**: the first DNS IP. For the set command, this parameter only works when you set <enable> to 1, i.e. enable manual DNS settings. If you set <enable> to 1 and a value for this parameter, the ESP-AT will return this parameter as the current DNS setting when you run the query command.
-  **<"DNS IP2">**: the second DNS IP. For the set command, this parameter only works when you set <enable> to 1, i.e. enable manual DNS settings. If you set <enable> to 1 and a value for this parameter, the ESP-AT will return this parameter as the current DNS setting when you run the query command.
-  **<"DNS IP3">**: the third DNS IP. For the set command, this parameter only works when you set <enable> to 1, i.e. enable manual DNS settings. If you set <enable> to 1 and a value for this parameter, the ESP-AT will return this parameter as the current DNS setting when you run the query command.

Notes
^^^^^

-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  The three parameters cannot be set to the same server.
-  When ``<enable>`` is set to 1, the DNS server may change according to the configuration of the router which the ESP device is connected to.

Example
^^^^^^^^

::

    AT+CIPDNS=0
    AT+CIPDNS=1,"208.67.222.222","114.114.114.114","8.8.8.8"

    // first DNS Server based on IPv6: China Future Internet Engineering Center
    // second DNS Server based on IPv6: google-public-dns-a.google.com
    // third DNS Server based on IPv6: main DNS Server based on IPv6 in JiangSu Province, China
    AT+CIPDNS=1,"240c::6666","2001:4860:4860::8888","240e:5a::6666"

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

    +CIPTCPOPT:<link_id>,<so_linger>,<tcp_nodelay>,<so_sndtimeo>
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    // Single TCP connection (AT+CIPMUX=0):
    AT+CIPTCPOPT=[<so_linger>],[<tcp_nodelay>],[<so_sndtimeo>]

    // Multiple TCP Connections (AT+CIPMUX=1):
    AT+CIPTCPOPT=<link ID>,[<so_linger>],[<tcp_nodelay>],[<so_sndtimeo>]

**Response:**

::

    OK

or

::

    ERROR

Parameters
^^^^^^^^^^

-  **<link_id>**: ID of the connection (0 ~ max). For multiple connections, if the value is max, it means all connections. By default, max is 5.
-  **<so_linger>**: configure the ``SO_LINGER`` options for the socket. Unit: second. Default: -1.

   -  = -1: off
   -  = 0: on, linger time = 0
   -  > 0: on, linger time = <so_linger>

-  **<tcp_nodelay>**: configure the ``TCP_NODELAY`` option for the socket. Default: 0.

   -  0: disable TCP_NODELAY
   -  1: enable TCP_NODELAY

-  **<so_sndtimeo>**: configure the ``SO_SNDTIMEO`` option for socket. Unit: millisecond. Default: 0.
