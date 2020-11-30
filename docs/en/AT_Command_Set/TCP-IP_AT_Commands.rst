
.. _TCPIP-AT:

TCP/IP AT Commands
==================

-  :ref:`AT+CIPSTATUS <cmd-STATUS>`: Obtain the TCP/UDP/SSL connection status and information.
-  :ref:`AT+CIPDOMAIN <cmd-DOMAIN>`: Resolve a Domain Name.
-  :ref:`AT+CIPSTART <cmd-START>`: Establish TCP connection, UDP transmission, or SSL connection.
-  :ref:`AT+CIPSTARTEX <cmd-STARTEX>`: Establish TCP connection, UDP transmission, or SSL connection with an automatically assigned ID.
-  :ref:`AT+CIPSEND <cmd-SEND>`: Send data in the normal or Wi-Fi passthrough modes.
-  :ref:`AT+CIPSENDEX <cmd-SENDEX>`: Send data of designated length in the normal transmission mode.
-  :ref:`AT+CIPCLOSE <cmd-CLOSE>`: Close TCP/UDP/SSL connection.
-  :ref:`AT+CIFSR <cmd-IFSR>`: Obtain the local IP address.
-  :ref:`AT+CIPMUX <cmd-MUX>`: Enable/disable the multiple connections mode.
-  :ref:`AT+CIPSERVER <cmd-SERVER>`: Delete/create a TCP or SSL server.
-  :ref:`AT+CIPSERVERMAXCONN <cmd-SERVERMAX>`: Set the maximum connections allowed by a server.
-  :ref:`AT+CIPMODE <cmd-IPMODE>`: Configure the transmission mode.
-  :ref:`AT+SAVETRANSLINK <cmd-SAVET>`: Save the Wi-Fi passthrough connection in flash.
-  :ref:`AT+CIPSTO <cmd-STO>`: Set the TCP server timeout.
-  :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>`: Set the time zone and SNTP server.
-  :ref:`AT+CIPSNTPTIME <cmd-SNTPT>`: Query the SNTP time.
-  :ref:`AT+CIUPDATE <cmd-UPDATE>`: Upgrade the firmware through Wi-Fi.
-  :ref:`AT+CIPDINFO <cmd-IPDINFO>`: Show the remote IP and remote port with "+IPD".
-  :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`: Configure SSL clients.
-  :ref:`AT+CIPSSLCCN <cmd-SSLCCN>`: Configure the Common Name of the SSL client.
-  :ref:`AT+CIPSSLCSNI <cmd-SSLCSNI>`: Configure SSL client Server Name Indication (SNI).
-  :ref:`AT+CIPSSLCALPN <cmd-SSLCALPN>`: Configure SSL client Application Layer Protocol Negotiation (ALPN).
-  :ref:`AT+CIPSSLCPSK <cmd-SSLCPSK>`: Configure SSL client Pre-shared Key (PSK).
-  :ref:`AT+CIPRECONNINTV <cmd-AUTOCONNINT>`: Set the TCP reconnection interval for the Wi-Fi passthrough mode.
-  :ref:`AT+CIPRECVMODE <cmd-CIPRECVMODE>`: Set socket receiving mode.
-  :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>`: Obtain socket data in passive receiving mode.
-  :ref:`AT+CIPRECVLEN <cmd-CIPRECVLEN>`: Obtain socket data length in passive receiving mode.
-  :ref:`AT+PING <cmd-CIPPING>`: Send ICMP ECHO_REQUEST to network hosts.
-  :ref:`AT+CIPDNS <cmd-DNS>`: Configure Domain Name System (DNS).
-  :ref:`AT+CIPTCPOPT <cmd-TCPOPT>`: Configure the socket options.

.. _cmd-STATUS:

:ref:`AT+CIPSTATUS <TCPIP-AT>`: Obtain the TCP/UDP/SSL Connection Status and Information
-----------------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+CIPSTATUS

**Response:**

::

    STATUS:<stat>
    +CIPSTATUS:<link ID>,<type>,<remote IP>,<remote port>,<local port>,<tetype>
    OK

Parameters
^^^^^^^^^^

-  **<stat>**: status of the ESP station interface.

   -  0: The ESP station is inactive.
   -  1: The ESP station is idle.
   -  2: The ESP station is connected to an AP and its IP is obtained.
   -  3: The ESP station has created a TCP, UDP, or SSL transmission.
   -  4: All of the TCP, UDP, and SSL connections of the ESP device station are disconnected.
   -  5: The ESP station is not connected to an AP.

-  **<link ID>**: ID of the connection (0~4), used for multiple connections.
-  **<type>**: string parameter showing the type of transmission: "TCP", "UDP", or "SSL".
-  **<remote IP>**: string parameter showing the remote IP address.
-  **<remote port>**: the remote port number.
-  **<local port>**: the local port number.
-  **<tetype>**:

   -  0: ESP device runs as a client.
   -  1: ESP device runs as a server.

.. _cmd-DOMAIN:

:ref:`AT+CIPDOMAIN <TCPIP-AT>`: Resolve a Domain Name
------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+CIPDOMAIN=<domain name>

**Response:**

::

    +CIPDOMAIN:<IP address>
    OK

Parameter
^^^^^^^^^^

-  **<domain name>**: the domain name. 
-  **<IP address>**: the resolved IP address.

Example
^^^^^^^^

::

    AT+CWMODE=1                       // set the station mode
    AT+CWJAP="SSID","password"        // access to the internet
    AT+CIPDOMAIN="iot.espressif.cn"   // Domain Name Resolution function

.. _cmd-START:

:ref:`AT+CIPSTART <TCPIP-AT>`: Establish TCP Connection, UDP Transmission, or SSL Connection
---------------------------------------------------------------------------------------------

Establish TCP Connection
^^^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
""""""""""""

**Command:**

::

    // Single TCP connection (AT+CIPMUX=0):
    AT+CIPSTART=<type>,<remote IP>,<remote port>[,<TCP keep alive>][,<local IP>]
    // Multiple TCP Connections (AT+CIPMUX=1):
    AT+CIPSTART=<link ID>,<type>,<remote IP>,<remote port>[,<TCP keep alive>][,<local IP>]

**Response:**

::

    OK

If the TCP connection is already established, the response is:

::

    ALREADY CONNECTED
    ERROR

Parameters
"""""""""""

-  **<link ID>**: ID of network connection (0~4), used for multiple connections.
-  **<type>**: string parameter showing the connection type: "TCP", "UDP", or "SSL".
-  **<remote IP>**: string parameter showing the remote IP address.
-  **<remote port>**: the remote port number.
-  **[<TCP keep alive>]**: TCP keep-alive interval. Default: 0.

   -  0: disable TCP keep-alive function.
   -  1 ~ 7200: detection interval. Unit: second.

-  **[<local IP>]**: the local IP you want to connect. This parameter is useful when you are using both Ethernet and Wi-Fi. By default, it is disabled. If you want to use it, you should specify it first. Null is also valid.

Notes
""""""

- If the remote IP address over the UDP is a multicast address (224.0.0.0 ~ 239.255.255.255), the ESP device will send and receive the UDP multicast.
- If the remote IP address over the UDP is a broadcast address (255.255.255.255), the ESP device will send and receive the UDP broadcast.

Example
"""""""""

::

    AT+CIPSTART="TCP","iot.espressif.cn",8000
    AT+CIPSTART="TCP","192.168.101.110",1000
    AT+CIPSTART="TCP","192.168.101.110",1000,,"192.168.101.100"

Establish UDP Transmission
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
""""""""""""

**Command:**

::

    // Single connection (AT+CIPMUX=0): 
    AT+CIPSTART=<type>,<remote IP>,<remote port>[,(<UDP local port>),(<UDP mode>)][,<local IP>]
    // Multiple connections (AT+CIPMUX=1): 
    AT+CIPSTART=<link ID>,<type>,<remote IP>,<remote port>[,(<UDP local port>),(<UDP mode>)][,<local IP>]

**Response:**

::

    OK

If the UDP transmission is already established, the response is:

::

    ALREADY CONNECTED
    ERROR

Parameters
"""""""""""

-  **<link ID>**: ID of network connection (0~4), used for multiple connections.
-  **<type>**: string parameter showing the connection type: "TCP", "UDP", or "SSL".
-  **<remote IP>**: string parameter showing the remote IP address.
-  **<remote port>**: remote port number.
-  **[<UDP local port>]**: UDP port of ESP devices.
-  **[<UDP mode>]**: In the UDP Wi-Fi passthrough, the value of this parameter has to be 0.

   -  0: the destination peer entity of UDP will not change (default).
   -  1: the destination peer entity of UDP can change once.
   -  2: the destination peer entity of UDP is allowed to change.

-  **[<local IP>]**: the local IP you want to connect. This parameter is useful when you are using both Ethernet and Wi-Fi. By default, it is disabled. If you want to use it, you should specify it first. Null is also valid.

Note
"""""

-  To use the parameter <UDP mode> , parameter <UDP local port> must be set first.

Example
"""""""""

::

    AT+CIPSTART="UDP","192.168.101.110",1000,1002,2
    AT+CIPSTART="UDP","192.168.101.110",1000,,,"192.168.101.100"

Establish SSL Connection
^^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
"""""""""""""

**Command:**

::

    AT+CIPSTART=[<link ID>,]<type>,<remote IP>,<remote port>[,<TCP keep alive>][,<local IP>]    

**Response:**

::

    OK

If the TCP connection is already established, the response is:

::

    ALREADY CONNECTED
    ERROR

Parameters
"""""""""""

-  **<link ID>**: ID of network connection (0~4), used for multiple connections.
-  **<type>**: string parameter showing the connection type: "TCP", "UDP", or "SSL".
-  **<remote IP>**: string parameter showing the remote IP address.
-  **<remote port>**: the remote port number.
-  **[<TCP keep alive>]**: TCP keep-alive interval. Default: 0.

   -  0: disable TCP keep-alive function.
   -  1 ~ 7200: detection interval. Unit: second.

-  **[<local IP>]**: the local IP you want to connect. This parameter is useful when you are using both Ethernet and Wi-Fi. By default, it is disabled. If you want to use it, you should specify it first. Null is also valid.

Notes
""""""

-  The number of SSL connections depends on available memory and the maximum number of connections.
-  SSL connection does not support Wi-Fi passthrough mode.
-  SSL connection needs a large amount of memory. Insufficient memory may cause the system reboot.
-  If the ``AT+CIPSTART`` is based on an SSL connection and the timeout of each packet is 10 s, the total timeout will be much longer depending on the number of handshake packets.

Example
""""""""

::

    AT+CIPSTART="SSL","iot.espressif.cn",8443
    AT+CIPSTART="SSL","192.168.101.110",1000,,"192.168.101.100" 

.. _cmd-STARTEX:

:ref:`AT+CIPSTARTEX <TCPIP-AT>`: Establish TCP connection, UDP transmission, or SSL connection with an Automatically Assigned ID
----------------------------------------------------------------------------------------------------------------------------------

This command is similar to :ref:`AT+CIPSTART <cmd-START>` except that you don't need to assign an ID by yourself in multiple connections mode (:ref:`AT+CIPMUX=1 <cmd-MUX>`). The system will assign an ID to the new connection automatically.

.. _cmd-SEND:

:ref:`AT+CIPSEND <TCPIP-AT>`: Send Data in the Normal or Wi-Fi Passthrough Modes
-----------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Configure the data length in the normal transmission mode.

**Command:**

::

    // Single connection: (AT+CIPMUX=0)
    AT+CIPSEND=<length>
    // Multiple connections: (AT+CIPMUX=1)
    AT+CIPSEND=<link ID>,<length>
    // Remote IP and ports can be set in UDP transmission: 
    AT+CIPSEND=[<link ID>,]<length>[,<remote IP>,<remote port>]

**Response:**

::

    OK
    >

This response indicates that AT is ready for receiving serial data. You should enter the data, and when the requirement of data length is met, the transmission of data starts.

If the connection cannot be established or is disrupted during data transmission, the system returns:

::

    ERROR

If data is transmitted successfully, the system returns:

::

    SEND OK 

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Start to send data in the Wi-Fi passthrough mode.

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

Enter the Wi-Fi passthrough mode. AT will send a packet every 20 ms or when the data reaches 2048 bytes. When a single packet containing ``+++`` is received, the ESP device returns to the normal command mode. Please wait for at least one second before sending the next AT command.

This command can only be used for single connection in the Wi-Fi passthrough mode. For UDP Wi-Fi passthrough, the <UDP mode> parameter has to be 0 when using ``AT+CIPSTART``.

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~4), for multiple connections.
-  **<length>**: data length. Maximum: 2048 bytes.
-  **[<remote IP>]**: remote IP can be set in UDP transmission.
-  **[<remote port>]**: remote port can be set in UDP transmission.

.. _cmd-SENDEX:

:ref:`AT+CIPSENDEX <TCPIP-AT>`: Send Data of Designated Length in the Normal Transmission Mode
-----------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Configure the data length in normal transmission mode.

**Command:**

::

    // Single connection: (AT+CIPMUX=0)
    AT+CIPSENDEX=<length>
    // Multiple connections: (AT+CIPMUX=1)
    AT+CIPSENDEX=<link ID>,<length>
    // Remote IP and ports can be set in UDP transmission:
    AT+CIPSENDEX=[<link ID>,]<length>[,<remote IP>,<remote port>]

**Response:**

::

    OK
    >

This response indicates that AT is ready for receiving data. You should enter the data of designated length. When the requirement of data length is met, or when the string ``\0`` appears in the data, the transmission starts.

If the connection cannot be established or gets disconnected during transmission, the system returns:

::

    ERROR

If the data are successfully transmitted, the system returns:

::

    SEND OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~4), for multiple connections.
-  **<length>**: data length. Maximum: 2048 bytes.

Notes
^^^^^^

-  When the requirement of data length is met, or when the string ``\0`` appears, the transmission of data starts. Go back to the normal command mode and wait for the next AT command.
-  When sending ``\0``, please use a backslash to escape it as ``\\0``.

.. _cmd-CLOSE:

:ref:`AT+CIPCLOSE <TCPIP-AT>`: Close TCP/UDP/SSL Connection
----------------------------------------------------------------

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

:ref:`AT+CIFSR <TCPIP-AT>`: Obtain the Local IP Address
---------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+CIFSR    

**Response:**

::

    +CIFSR:<SoftAP IP address>
    +CIFSR:<Station IP address>
    OK

Parameter
^^^^^^^^^^

-  **<IP address>**:

   -  IP address of the ESP SoftAP.
   -  IP address of the ESP station.

Note
^^^^^

-  Only when the ESP station is connected to an AP can you query its IP address.

.. _cmd-MUX:

:ref:`AT+CIPMUX <TCPIP-AT>`: Enable/disable Multiple Connections
----------------------------------------------------------------------

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
-  If you want to set the multiple connections mode, the Wi-Fi passthrough mode should be disabled (:ref:`AT+CIPMODE=0 <cmd-IPMODE>`).  
-  If you want to set the single connection mode when the TCP server is running, you should delete the server first. (:ref:`AT+CIPSERVER=0 <cmd-SERVER>`).

Example
^^^^^^^^

::

    AT+CIPMUX=1 

.. _cmd-SERVER:

:ref:`AT+CIPSERVER <TCPIP-AT>`: Delete/create a TCP or SSL Server
---------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+CIPSERVER=<mode>[,<port>][,<SSL>,<SSL CA enable>]    

**Response:**

::

    OK  

Parameters
^^^^^^^^^^

-  **<mode>**:

   -  0: delete a server.
   -  1: create a server.

-  **<port>**: port number; Default: 333.
-  **[<SSL>]**: the string "SSL", which means configuring an SSL server. If you omit this parameter, you will delete or create a **TCP** server. This parameter is applicable to ESP32 and ESP32-S2 only.
-  **[<SSL CA enable>]**: applicable to ESP32 and ESP32-S2 only.

   -  0: disable CA.
   -  1: enable CA.

Notes
^^^^^

-  A TCP server can only be created when multiple connections are activated (:ref:`AT+CIPMUX=1 <cmd-MUX>`).
-  A server monitor will be created automatically when the TCP server is created. Only one server can be created at most.
-  When a client is connected to the server, it will take up one connection and be assigned an ID.

Example
^^^^^^^^

::

    // To create a TCP server
    AT+CIPMUX=1
    AT+CIPSERVER=1,80
    // To create an SSL server
    AT+CIPMUX=1
    AT+CIPSERVER=1,443,"SSL",1

.. _cmd-SERVERMAX:

:ref:`AT+CIPSERVERMAXCONN <TCPIP-AT>`: Set the Maximum Connections Allowed by a Server
----------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Obtain the maximum number of clients allowed to connect to the TCP or SSL server.

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

Set the maximum number of clients allowed to connect to the TCP or SSL server.

**Command:**

::

    AT+CIPSERVERMAXCONN=<num>

**Response:**

::

    OK  

Parameter
^^^^^^^^^^

-  **<num>**: the maximum number of clients allowed to connect to the TCP or SSL server.

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

:ref:`AT+CIPMODE <TCPIP-AT>`: Configure the Transmission Mode
------------------------------------------------------------------

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

   -  0: normal transmission mode.
   -  1: Wi-Fi passthrough mode, or called transparent transmission, which can only be enabled in TCP single connection mode or in UDP mode when the remote IP and port do not change.

Notes
^^^^^

-  The configuration changes will NOT be saved in flash.
-  During the Wi-Fi passthrough transmission, if the TCP connection breaks, ESP devices will keep trying to reconnect until ``+++`` is input to exit the transmission.
-  During a normal TCP transmission, if the TCP connection breaks, ESP devices will give a prompt and will not attempt to reconnect.

Example
^^^^^^^^

::

    AT+CIPMODE=1    

.. _cmd-SAVET:

:ref:`AT+SAVETRANSLINK <TCPIP-AT>`: Save the Wi-Fi Passthrough Connection in Flash
------------------------------------------------------------------------------------------

Save TCP Single Connection in Flash
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
""""""""""""""

**Command:**

::

    AT+SAVETRANSLINK=<mode>,<remote IP or domain name>,<remote port>[,<type>,<TCP keep alive>]  

**Response:**

::

    OK

Parameters
""""""""""""""

-  **<mode>**:

   -  0: normal mode. ESP devices will NOT enter Wi-Fi passthrough mode on power-up.
   -  1: ESP devices will enter Wi-Fi passthrough mode on power-up.

-  **<remote IP>**: remote IP or domain name.
-  **<remote port>**: remote port.
-  **[<type>]**: TCP or UDP. Default: TCP.
-  **[<TCP keep alive>]**: TCP keep-alive interval. Default: 0.

   -  0: disable the TCP keep-alive function.
   -  1 ~ 7200: detection interval. Unit: second.

Notes
"""""""

-  This command will save the Wi-Fi passthrough mode and its connection in the NVS area. The ESP devices will enter the Wi-Fi passthrough mode in any subsequent power cycles.
-  As long as the remote IP or domain name and port are valid, the configuration will be saved in flash.

Example
""""""""

::

    AT+SAVETRANSLINK=1,"192.168.6.110",1002,"TCP"   

Save UDP Transmission in Flash
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
""""""""""""""

**Command:**

::

    AT+SAVETRANSLINK=<mode>,<remote IP>,<remote port>,<type>[,<UDP local port>] 

**Response:**

::

    OK

Parameters
""""""""""""""

-  **<mode>**:

   -  0: normal mode; ESP will NOT enter Wi-Fi passthrough mode on power-up.
   -  1: ESP devices enter Wi-Fi passthrough mode on power-up.

-  **<remote IP>**: remote IP or domain name.
-  **<remote port>**: remote port.
-  **[<type>]**: UDP. Default: TCP.
-  **[<UDP local port>]**: local port when UDP Wi-Fi passthrough is enabled on power-up.

Notes
"""""""

-  This command will save the Wi-Fi passthrough mode and its connections in the NVS area. The ESP device will enter the Wi-Fi passthrough mode in any subsequent power cycles.
-  As long as the remote IP (or domain name) and port are valid, the configuration will be saved in flash.

Example
"""""""""

::

    AT+SAVETRANSLINK=1,"192.168.6.110",1002,"UDP",1005  

.. _cmd-STO:

:ref:`AT+CIPSTO <TCPIP-AT>`: Set the TCP Server Timeout
------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the TCP server timeout.

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

Set the TCP server timeout.

**Command:**

::

    AT+CIPSTO=<time>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<time>**: TCP server timeout. Unit second. Range: 0 ~ 7200.

Notes
^^^^^

-  When a timeout occurs, the ESP device configured as a TCP server will terminate the connection from the TCP client that does not respond.
-  If you set <time> to 0, the connection will never timeout. This configuration is not recommended.

Example
^^^^^^^^

::

    AT+CIPMUX=1
    AT+CIPSERVER=1,1001
    AT+CIPSTO=10

.. _cmd-SNTPCFG:

:ref:`AT+CIPSNTPCFG <TCPIP-AT>`: Set the Time Zone and the SNTP Server
--------------------------------------------------------------------------

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

   -  The first format range is [-12,14]. It marks most of the time zones by offset from Coordinated Universal Time (UTC) in **whole hours** (`UTC−12:00 <https://en.wikipedia.org/wiki/UTC%E2%88%9212:00>`__ to `UTC+14:00 <https://en.wikipedia.org/wiki/UTC%2B14:00>`_).
   -  The second format is ``UTC offset``. The ``UTC offset`` specifies the time value you must add to the UTC time to get a local time value. It has syntax like ``[+|-]hh[mm]``. This is negative if the local time zone is on the west of the Prime Meridian and positive if it is on the east. The hour(hh) must be between -12 and 14, and the minute(mm) between 0 and 59. For example, if you want to set the timezone to New Zealand (Chatham Islands) which is in ``UTC+12:45``, you should set the parameter ``<timezone>`` to ``1245``. Please refer to `UTC offset wiki <https://en.wikipedia.org/wiki/Time_zone#List_of_UTC_offsets>`_ for more information.

-  **[<SNTP server1>]**: the first SNTP server.
-  **[<SNTP server2>]**: the second SNTP server.
-  **[<SNTP server3>]**: the third SNTP server.

Note
^^^^^

-  If the three SNTP servers are not configured, one of the following default servers will be used: "cn.ntp.org.cn", "ntp.sjtu.edu.cn", "us.pool.ntp.org".

Example
^^^^^^^^

::

    // Enable SNTP server, set timezone to China (UTC+08:00)
    AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"
    or
    AT+CIPSNTPCFG=1,800,"cn.ntp.org.cn","ntp.sjtu.edu.cn"

    // Enable SNTP server, set timezone to New York of the United States (UTC−05:00)
    AT+CIPSNTPCFG=1,-5,"0.pool.ntp.org","time.google.com"
    or
    AT+CIPSNTPCFG=1,-500,"0.pool.ntp.org","time.google.com"

    // Enable SNTP server, set timezone to New Zealand (Chatham Islands, UTC+12:45)
    AT+CIPSNTPCFG=1,1245,"0.pool.ntp.org","time.google.com"

.. _cmd-SNTPT:

:ref:`AT+CIPSNTPTIME <TCPIP-AT>`: Query the SNTP Time
-----------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+CIPSNTPTIME? 

**Response:**

::

    +CIPSNTPTIME:<asctime style time>
    OK

Example
^^^^^^^^

::

    AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"
    OK
    AT+CIPSNTPTIME?
    +CIPSNTPTIME:Mon Dec 12 02:33:32 2016
    OK  

Note
^^^^^

-  The asctime style time is defined at `asctime man page <https://linux.die.net/man/3/asctime>`_.

.. _cmd-UPDATE:

:ref:`AT+CIUPDATE <TCPIP-AT>`: Upgrade Firmware Through Wi-Fi
---------------------------------------------------------------------

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

Upgrade OTA the latest version of firmware via TCP from the server.

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
    
    - 0: OTA via TCP.
    - 1: OTA via SSL. If it does not work, please check whether ``make menuconfig`` > ``Component config`` > ``AT`` > ``OTA based upon ssl`` is enabled. For more information, please refer to :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`.

- **<version>**: AT version, such as, ``v1.2.0.0``, ``v1.1.3.0``, ``v1.1.2.0``.
- **<firmware name>**: firmware to upgrade, such as, ``ota``, ``mqtt_ca``, ``client_ca`` or other custom partition in ``at_customize.csv``.
- **<nonblocking>**:

    - 0: OTA by blocking mode (In this mode, user can not send AT command until OTA completes successfully or fails.)
    - 1: OTA by non-blocking mode（Users need to manually restart after update done(+CIPUPDATE:4).)

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
-  If you use Espressif's AT `BIN <https://www.espressif.com/zh-hans/support/download/at>`_, ``AT+CIUPDATE`` will download a new AT BIN from the Espressif Cloud.
-  If you use a user-compiled AT BIN, you need to implement their own AT+CIUPDATE FOTA function. ESP-AT project provides an example of `FOTA <https://github.com/espressif/esp-at/blob/master/main/at_upgrade.c>`_.
-  After you upgrade the AT firmware, you are suggested to call the command :ref:`AT+RESTORE <cmd-RESTORE>` to restore the factory default settings.

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

:ref:`AT+CIPDINFO <TCPIP-AT>`: Show the Remote IP and Port with "+IPD"
------------------------------------------------------------------------

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

   -  0: does not show the remote IP and port with "+IPD" and "+CIPRECVDATA".
   -  1: shows the remote IP and port with "+IPD" and "+CIPRECVDATA".

Example
^^^^^^^^

::

    AT+CIPDINFO=1

.. _cmd-SSLCCONF:

:ref:`AT+CIPSSLCCONF <TCPIP-AT>`: Configure SSL Clients
--------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Obtain the configuration of each connection where the ESP device runs as an SSL client.

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

    // Single connection: (+CIPMUX=0)
    AT+CIPSSLCCONF=<auth_mode>[,<pki_number>][,<ca_number>]
    // Multiple connections: (+CIPMUX=1)
    AT+CIPSSLCCONF=<link ID>,<auth_mode>[,<pki_number>][,<ca_number>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~max). For multiple connections, if the value is max, it means all connections. By default, max is 5.
-  **<auth_mode>**:

   -  0: no authentication. In this case, <pki_number> and <ca_number> are not required.
   -  1: the server verifies the client's certificate and private key.
   -  2: the client loads CA to verify the server's certificate and private key.
   -  3: mutual authentication.

-  **<pki_number>**: the index of certificate and private key. If there is only one certificate and private key, the value should be 0.
-  **<ca_number>**: the index of CA. If there is only one CA, the value should be 0.

Notes
^^^^^

-  If you want this configuration to take effect immediately, run this command before establishing an SSL connection.
-  The configuration changes will be saved in the NVS area. If you run :ref:`AT+SAVETRANSLINK <cmd-SAVET>` to enter SSL Wi-Fi passthrough mode, the ESP device will establish an SSL connection based on this configuration when powered on next time.

.. _cmd-SSLCCN:

:ref:`AT+CIPSSLCCN <TCPIP-AT>`: Configure the Common Name of the SSL Client
-----------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Obtain the common name of the SSL client of each connection.

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

    // Single connection: (+CIPMUX=0)
    AT+CIPSSLCCN=<"common name">
    // Multiple connections: (+CIPMUX=1)
    AT+CIPSSLCCN=<link ID>,<"common name">

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~max). For the single connection, the link ID is 0. For multiple connections, if the value is max, it means all connections. Max is 5 by default.
-  **<"common name">**: this parameter is used to verify the Common Name in the certificate sent by the server.

Note
^^^^^

-  If you want this configuration to take effect immediately, run this command before establishing the SSL connection.

.. _cmd-SSLCSNI:

:ref:`AT+CIPSSLCSNI <TCPIP-AT>`: Configure SSL Client Server Name Indication (SNI)
-----------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Obtain the SNI configuration of each connection.

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

    Single connection: (+CIPMUX=0)
    AT+CIPSSLCSNI=<"sni">
    Multiple connections: (+CIPMUX=1)
    AT+CIPSSLCSNI=<link ID>,<"sni">

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~max). For the single connection, the link ID is 0. For multiple connections, if the value is max, it means all connections. Max is 5 by default.
-  **<"sni">**: the Server Name Indication in ClientHello.

Notes
^^^^^

-  If you want this configuration to take effect immediately, run this command before establishing the SSL connection.

.. _cmd-SSLCALPN:

:ref:`AT+CIPSSLCALPN <TCPIP-AT>`: Configure SSL Client Application Layer Protocol Negotiation (ALPN)
-----------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Obtain the ALPN configuration of each connection where the ESP device runs as an SSL client.

**Command:**

::

    AT+CIPSSLCALPN?

**Response:**

::

    +CIPSSLCALPN:<link ID>,<"alpn">[,<"alpn">[,<"alpn">]]
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    // Single connection: (+CIPMUX=0)
    AT+CIPSSLCALPN=<counts>,<"alpn">[,<"alpn">[,<"alpn">]]
    // Multiple connections: (+CIPMUX=1)
    AT+CIPSSLCALPN=<link ID>,<counts>,<"alpn">[,<"alpn">[,<"alpn">]]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~max). For the single connection, the link ID is 0. For multiple connections, if the value is max, it means all connections. Max is 5 by default.
-  **<counts>**: the number of ALPNs.
-  **<"alpn">**: a string paramemter showing the ALPN in ClientHello.

Note
^^^^^

-  If you want this configuration to take effect immediately, run this command before establishing the SSL connection.

.. _cmd-SSLCPSK:

:ref:`AT+CIPSSLCPSK <TCPIP-AT>`: Configure SSL Client Pre-shared Key (PSK)
---------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Obtain the PSK configuration of each connection where the ESP device runs as an SSL client.

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

    // Single connection: (+CIPMUX=0)
    AT+CIPSSLCPSK=<"psk">,<"hint">
    // Multiple connections: (+CIPMUX=1)
    AT+CIPSSLCPSK=<link ID>,<"psk">,<"hint">

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<link ID>**: ID of the connection (0~max). For single connection, <link ID> is 0.
   for multiple connections, if the value is max, it means all connections, max is 5 by default.
-  **<"psk">**: PSK identity. Maximum length: 32.
-  **<"hint">**: PSK hint. Maximum length: 32.

Notes
^^^^^
-  If you want this configuration to take effect immediately, run this command before establishing the SSL connection.
-  This command is currently supported on ESP8266 and ESP32-S2, but not on ESP32 devices.

.. _cmd-AUTOCONNINT:

:ref:`AT+CIPRECONNINTV <TCPIP-AT>`: Set the TCP reconnection Interval for the Wi-Fi Passthrough Mode
-----------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Obtain the automatic connect interval for the Wi-Fi passthrough mode.

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

Set the automatic reconnecting interval when TCP/UDP/SSL transmission breaks in the Wi-Fi passthrough mode.

**Command:**

::

    AT+CIPRECONNINTV=<interval>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<interval>**: the duration between automatic reconnections. Unit: 100 milliseconds. Default: 1. Range: 1 ~ 36000. 

Example
^^^^^^^^

::

    AT+CIPRECONNINTV=10  

Note
^^^^^

-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.

.. _cmd-CIPRECVMODE:

:ref:`AT+CIPRECVMODE <TCPIP-AT>`: Set Socket Receiving Mode
-------------------------------------------------------------

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
   
   - 0: active mode. ESP-AT will send all the received socket data instantly to the host MCU through UART with header “+IPD".
   - 1: passive mode. ESP-AT will keep the received socket data in an internal buffer (5744 bytes by default), and wait for the host MCU to read it. If the buffer is full, the socket transmission will be blocked.

Example
^^^^^^^^

::

    AT+CIPRECVMODE=1   

Notes
^^^^^

-  The configuration is for TCP and SSL transmission only, and can not be used in the Wi-Fi passthrough mode. If it is a UDP transmission in passive mode, data will be lost when the buffer is full.

-  When ESP-AT receives socket data in passive mode, it will prompt the following messages in different scenarios:

   -  For multiple connections mode (AT+CIPMUX=1), the message is ``+IPD,<link ID>,<len>``.
   -  For single connection mode (AT+CIPMUX=0), the message is ``+IPD,<len>``.

-  ``<len>`` is the total length of socket data in the buffer.
-  You should read data by running ``AT+CIPRECVDATA`` once there is a ``+IPD`` reported. Otherwise, the next ``+IPD`` will not be reported to the host MCU until the previous ``+IPD`` has been read.
-  In case of disconnection, the buffered socket data will still be there and can be read by the MCU until you send ``AT+CIPCLOSE``. In other words, if ``+IPD`` has been reported, the message ``CLOSED`` of this connection will never come until you send ``AT+CIPCLOSE`` or read all data by command ``AT+CIPRECVDATA``.

.. _cmd-CIPRECVDATA:

:ref:`AT+CIPRECVDATA <TCPIP-AT>`: Obtain Socket Data in Passive Receiving Mode
-------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    // Single connection: (+CIPMUX=0)
    AT+CIPRECVDATA=<len>
    // Multiple connections: (+CIPMUX=1)
    AT+CIPRECVDATA=<link_id>,<len>

**Response:**

::

    +CIPRECVDATA:<actual_len>,<data>
    OK

or

::

    +CIPRECVDATA:<actual_len>,<remote IP>,<remote port>,<data>
    OK

Parameters
^^^^^^^^^^

-  **<link_id>**: connection ID in multiple connections mode.
-  **<len>**: the max value is 0x7fffffff. If the actual length of the received data is less than ``len``, the actual length will be returned.
-  **<actual_len>**: length of the data you actually obtain.
-  **<data>**: the data you want to obtain.
-  **[<remote IP>]**: string parameter showing the remote IP, enabled by the command :ref:`AT+CIPDINFO=1 <cmd-IPDINFO>`.
-  **[<remote port>]**: remote port, enabled by the command :ref:`AT+CIPDINFO=1 <cmd-IPDINFO>`.

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

:ref:`AT+PING <TCPIP-AT>`: Send ICMP ECHO_REQUEST to Network Hosts
--------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Ping packets.

**Command:**

::

    AT+PING=<IP>

**Response:**

::

    +PING:<time>

    OK

or

::

    +timeout

    ERROR

Parameters
^^^^^^^^^^

- **<IP>**: string parameter showing the host IP or domain name.
- **<time>**: the response time of ping. Unit: millisecond.

Example
^^^^^^^^

::

    AT+PING="192.168.1.1"
    AT+PING="www.baidu.com"

.. _cmd-DNS:

:ref:`AT+CIPDNS <TCPIP-AT>`: Configure Domain Name System (DNS)
---------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Obtain the current DNS information.

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

Configure the Domain Name System.

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

   -  0: Enable automatic DNS settings from DHCP. The DNS will be restored to ``222.222.67.208``. Only when DHCP is updated will it take effect.
   -  1: Enable manual DNS settings. If you do not set a value for ``DNS IP``, it will use ``222.222.67.208`` by default.

-  **<DNS IP1>**: the first DNS IP. For the set command, this parameter only works when you set <enable> to 1, i.e. enable manual DNS settings. If you set <enable> to 1 and a value for this parameter, the ESP-AT will return this parameter as the current DNS setting when you run the query command.
-  **<DNS IP2>**: the second DNS IP. For the set command, this parameter only works when you set <enable> to 1, i.e. enable manual DNS settings. If you set <enable> to 1 and a value for this parameter, the ESP-AT will return this parameter as the current DNS setting when you run the query command.
-  **<DNS IP3>**: the third DNS IP. For the set command, this parameter only works when you set <enable> to 1, i.e. enable manual DNS settings. If you set <enable> to 1 and a value for this parameter, the ESP-AT will return this parameter as the current DNS setting when you run the query command.

Example
^^^^^^^^

::

    AT+CIPDNS=0
    AT+CIPDNS=1,"222.222.67.208","114.114.114.114","8.8.8.8"

Notes
^^^^^

-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  The three parameters cannot be set to the same server.
-  The DNS server may change according to the configuration of the router which the ESP device is connected to.

.. _cmd-TCPOPT:

:ref:`AT+CIPTCPOPT <TCPIP-AT>`: Configure the Socket Options
---------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Obtain current socket options.

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

-  **<link_id>**: ID of the connection (0~max). For multiple connections, if the value is max, it means all connections. By default, max is 5.
-  **<so_linger>**: configure the SO_LINGER options for the socket. Unit: second. Default: -1.

   -  = -1: off
   -  = 0: on, linger time = 0
   -  > 0: on, linger time = <so_linger>

-  **<tcp_nodelay>**: configure the ``TCP_NODELAY`` option for the socket. Default: 0.

   -  0: disable TCP_NODELAY
   -  1: enable TCP_NODELAY

-  **<so_sndtimeo>**: configure the ``SO_SNDTIMEO`` option for socket. Unit: millisecond. Default: 0.
