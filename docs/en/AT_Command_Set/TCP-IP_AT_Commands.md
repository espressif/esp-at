<a name="TCPIP-AT"></a>
## TCP/IP AT Commands
* [AT+CIPSTATUS](#cmd-STATUS) : Gets the connection status.
* [AT+CIPDOMAIN](#cmd-DOMAIN) : Domain Name Resolution Function.
* [AT+CIPSTART](#cmd-START) : Establishes TCP connection, UDP transmission or SSL connection.
* [AT+CIPSTARTEX](#cmd-STARTEX) : Establishes TCP connection, UDP transmission or SSL connection with automatically assigned ID.
* [AT+CIPSEND](#cmd-SEND) : Sends data.
* [AT+CIPSENDEX](#cmd-SENDEX) : Sends data when length of data is \<length>, or when \0 appears in the data.
* [AT+CIPCLOSE](#cmd-CLOSE) : Closes TCP/UDP/SSL connection.
* [AT+CIFSR](#cmd-IFSR) : Gets the local IP address.
* [AT+CIPMUX](#cmd-MUX) : Configures the multiple connections mode.
* [AT+CIPSERVER](#cmd-SERVER) : Deletes/Creates TCP or SSL server.
* [AT+CIPSERVERMAXCONN](#cmd-SERVERMAX) : Set the Maximum Connections Allowed by Server.
* [AT+CIPMODE](#cmd-IPMODE) : Configures the transmission mode.
* [AT+SAVETRANSLINK](#cmd-SAVET) : Saves the transparent transmission link in flash.
* [AT+CIPSTO](#cmd-STO) : Sets timeout when ESP32 runs as a TCP server.
* [AT+CIPSNTPCFG](#cmd-SNTPCFG) : Configures the time domain and SNTP server.
* [AT+CIPSNTPTIME](#cmd-SNTPT) : Queries the SNTP time.
* [AT+CIUPDATE](#cmd-UPDATE) : Updates the software through Wi-Fi.
* [AT+CIPDINFO](#cmd-IPDINFO) : Shows remote IP and remote port with +IPD.
* [AT+CIPSSLCCONF](#cmd-SSLCCONF) : Config SSL client.
* [AT+CIPSSLCCN](#cmd-SSLCCN) : Config SSL client common name.
* [AT+CIPSSLCSNI](#cmd-SSLCSNI) : Config SSL client SNI.
* [AT+CIPSSLCALPN](#cmd-SSLCALPN) : Config SSL client ALPN.
* [AT+CIPSSLCPSK](#cmd-SSLCPSK) : Config SSL client PSK.
* [AT+CIPRECONNINTV](#cmd-AUTOCONNINT): Set Wi-Fi transparent transmitting auto-connect interval.
* [AT+CIPRECVMODE](#cmd-CIPRECVMODE): Set Socket Receive Mode.
* [AT+CIPRECVDATA](#cmd-CIPRECVDATA): Get Socket Data in Passive Receive Mode.
* [AT+CIPRECVLEN](#cmd-CIPRECVLEN): Get Socket Data Length in Passive Receive Mode. 
* [AT+PING](#cmd-CIPPING): Ping Packets
* [AT+CIPDNS](#cmd-DNS) : Configures Domain Name System.
* [AT+CIPTCPOPT](#cmd-TCPOPT) : Configurates the socket options.  

 
<a name="cmd-STATUS"></a>
### [AT+CIPSTATUS](#TCPIP-AT)—Gets the Connection Status
Execute Command:

    AT+CIPSTATUS
Response:

    STATUS:<stat>
    +CIPSTATUS:<link ID>,<type>,<remote IP>,<remote port>,<local port>,<tetype>
Parameters:

- **\<stat>**: status of the esp device Station interface.
    - 0: The esp device station is inactive.
    - 1: The esp device station is idle.
    - 2: The esp device Station is connected to an AP and its IP is obtained.
    - 3: The esp device Station has created a TCP or SSL transmission.
    - 4: The TCP or SSL transmission of esp device Station is disconnected.
    - 5: The esp device Station does NOT connect to an AP.
- **\<link ID>**: ID of the connection (0~4), used for multiple connections.
- **\<type>**: string parameter, "TCP" or "UDP".
- **\<remote IP>**: string parameter indicating the remote IP address.
- **\<remote port>**: the remote port number.
- **\<local port>**: the local port number.
- **\<tetype>**:
    - 0: esp device runs as a client.
    - 1: esp device runs as a server.

<a name="cmd-DOMAIN"></a>
### [AT+CIPDOMAIN](#TCPIP-AT)—Domain Name Resolution Function
Execute Command:

    AT+CIPDOMAIN=<domain name>
Response:

    +CIPDOMAIN:<IP address>
Parameter:

- **\<domain name>**: the domain name.

Example:

    AT+CWMODE=1                       // set Station mode
    AT+CWJAP="SSID","password"        // access to the internet
    AT+CIPDOMAIN="iot.espressif.cn"   // Domain Name Resolution function

<a name="cmd-START"></a>
### [AT+CIPSTART](#TCPIP-AT)—Establishes TCP Connection, UDP Transmission or SSL Connection
#### Establish TCP Connection
Set Command:

    Single TCP connection (AT+CIPMUX=0):
    AT+CIPSTART=<type>,<remote IP>,<remote port>[,<TCP keep alive>][,<local IP>]
    Multiple TCP Connections (AT+CIPMUX=1):
    AT+CIPSTART=<link ID>,<type>,<remote IP>,<remote port>[,<TCP keep alive>][,<local IP>]
Response:

    OK
Or if the TCP connection is already established, the response is:
    ALREADY CONNECTTED
    ERROR
Parameters:

- **\<link ID>**: ID of network connection (0~4), used for multiple connections.
- **\<type>**: string parameter indicating the connection type: "TCP", "UDP" or "SSL".
- **\<remote IP>**: string parameter indicating the remote IP address.
- **\<remote port>**: the remote port number.
- **\[\<TCP keep alive>]**(optional parameter): detection time interval when TCP is kept alive; this function is disabled by default.
    - 0: disable TCP keep-alive.
    - 1 ~ 7200: detection time interval; unit: second (s).

- **\[\<local IP>]**(optional parameter): select which IP want to use, this is useful when using both ethernet and WiFi; this parameter is disabled by default. If you want to use this parameter, <TCP keep alive> must be specified firstly, null also is valid.

Notes:

    If the remote IP over the UDP is a multicast address(224.0.0.0 ~ 239.255.255.255), the esp device will send and receive the UDP multicast;
    If the remote IP over the UDP is a broadcast address(255.255.255.255), the esp device will send and receive the UDP broadcast.

Examples:

    AT+CIPSTART="TCP","iot.espressif.cn",8000
    AT+CIPSTART="TCP","192.168.101.110",1000
    AT+CIPSTART="TCP","192.168.101.110",1000,,"192.168.101.100"

#### Establish UDP Transmission
Set Command:

    Single connection (AT+CIPMUX=0): 
    AT+CIPSTART=<type>,<remote IP>,<remote port>[,(<UDP local port>),(<UDP mode>)][,<local IP>]
    Multiple connections (AT+CIPMUX=1): 
    AT+CIPSTART=<link ID>,<type>,<remote IP>,<remote port>[,(<UDP local port>),(<UDP mode>)][,<local IP>]
Response:

    OK
Or if the UDP transmission is already established, the response is:
    ALREADY CONNECTTED
    ERROR
Parameters:

- **\<link ID>**: ID of network connection (0~4), used for multiple connections.
- **\<type>**: string parameter indicating the connection type: "TCP", "UDP" or "SSL".
- **\<remote IP>**: string parameter indicating the remote IP address.
- **\<remote port>**: remote port number.
- **\[\<UDP local port>]**(optional parameter): UDP port of ESP32.
- **\[\<UDP mode>]**(optional parameter): In the UDP transparent transmission, the value of this parameter has to be 0.
    - 0: the destination peer entity of UDP will not change; this is the default setting.
    - 1: the destination peer entity of UDP can change once.
    - 2: the destination peer entity of UDP is allowed to change.
- **\[\<local IP>]**(optional parameter): select which IP want to use, this is useful when using both ethernet and WiFi; this parameter is disabled by default. If you want to use this parameter, <UDP local port> and <UDP mode> must be specified firstly, null also is valid.

***Notice:*** 

* To use parameter \<UDP mode> , parameter \<UDP local port> must be set first.

Example:

    AT+CIPSTART="UDP","192.168.101.110",1000,1002,2
    AT+CIPSTART="UDP","192.168.101.110",1000,,,"192.168.101.100"
#### Establish SSL Connection
Set Command:

    AT+CIPSTART=[<link ID>,]<type>,<remote IP>,<remote port>[,<TCP keep alive>][,<local IP>]    
Response:

    OK
Or if the TCP connection is already established, the response is:
    ALREADY CONNECTTED
    ERROR
Parameters:

- **\<link ID>**: ID of network connection (0~4), used for multiple connections.
- **\<type>**: string parameter indicating the connection type: "TCP", "UDP" or "SSL".
- **\<remote IP>**: string parameter indicating the remote IP address.
- **\<remote port>**: the remote port number.
- **\[\<TCP keep alive>]**(optional parameter): detection time interval when TCP is kept alive; this function is disabled by default.
    - 0: disable the TCP keep-alive function.
    - 1 ~ 7200: detection time interval, unit: second (s).
- **\[\<local IP>]**(optional parameter): select which IP want to use, this is useful when using both ethernet and WiFi; this parameter is disabled by default. If you want to use this parameter, <TCP keep alive> must be specified firstly, null also is valid.  

***Notes:***

* SSL connection count depends on available memory and maximum connection count.
* SSL connection does not support UART-WiFi passthrough mode (transparent transmission).
* SSL connection needs a large amount of memory; otherwise, it may cause system reboot. 
* If the `AT+CIPSTART` is based on a TLS connection, the timeout of each packet is 10s, then the total timeout will be much longer depending on the handshake packets count. 

Example:

    AT+CIPSTART="SSL","iot.espressif.cn",8443
    AT+CIPSTART="SSL","192.168.101.110",1000,,"192.168.101.100" 


<a name="cmd-STARTEX"></a>
### [AT+CIPSTARTEX](#TCPIP-AT)—Establishes TCP connection, UDP transmission or SSL connection with automatically assigned ID

This command is similar to [AT+CIPSTART](#cmd-START), but you need not to assign an ID by yourself when it is the multiple connections mode (AT+CIPMUX=1), the system will assign an ID to the new connection automatically.


<a name="cmd-SEND"></a>
### [AT+CIPSEND](#TCPIP-AT)—Sends Data
Set Command: 

    Single connection: (+CIPMUX=0)
    AT+CIPSEND=<length>
    Multiple connections: (+CIPMUX=1)
    AT+CIPSEND=<link ID>,<length>
    Remote IP and ports can be set in UDP transmission: 
    AT+CIPSEND=[<link ID>,]<length>[,<remote IP>,<remote port>]
    Function: to configure the data length in normal transmission mode.
Response:

    OK
    >
Begin receiving serial data. When the requirement of data length is met, the transmission of data starts.
If the connection cannot be established or gets disrupted during data transmission, the system returns:

    ERROR
If data is transmitted successfully, the system returns: 

    SEND OK 
Execute Command: 

    AT+CIPSEND
    Function: to start sending data in transparent transmission mode.
Response:

    OK
    >
Enter transparent transmission, with a 20-ms interval between each packet, and a maximum of 2048 bytes per packet. 
When a single packet containing +++ is received, ESP32 returns to normal command mode. Please wait for at least one second before sending the next AT command.
This command can only be used in transparent transmission mode which requires single connection.
For UDP transparent transmission, the value of <UDP mode> has to be 0 when using AT+CIPSTART.

Or

    ERROR

Parameters:

- **\<link ID>**: ID of the connection (0~4), for multiple connections.
- **\<length>**: data length, MAX: 2048 bytes.
- **\[\<remote IP>]**(optional parameter): remote IP can be set in UDP transmission.
- **\[\<remote port>]**(optional parameter): remote port can be set in UDP transmission.    

<a name="cmd-SENDEX"></a>
### [AT+CIPSENDEX](#TCPIP-AT)—Sends Data
Set Command: 

    Single connection: (+CIPMUX=0)
    AT+CIPSENDEX=<length>
    Multiple connections: (+CIPMUX=1)
    AT+CIPSENDEX=<link ID>,<length>
    Remote IP and ports can be set in UDP transmission:
    AT+CIPSENDEX=[<link ID>,]<length>[,<remote IP>,<remote port>]
    Function: to configure the data length in normal transmission mode.
Response:

    OK
    >
Send data of designated length.
Wrap return > after the set command. Begin receiving serial data. When the requirement of data length, determined by <length>, is met, or when \0 appears in the data, the transmission starts.
If connection cannot be established or gets disconnected during transmission,  the system returns: 

    ERROR
    
If data are successfully transmitted, the system returns:
    SEND OK
Parameters:

- **\<link ID>**: ID of the connection (0~4), for multiple connections.
- **\<length>**: data length, MAX: 2048 bytes.
    - When the requirement of data length, determined by \<length>, is met, or when string \0 appears, the transmission of data starts. Go back to the normal command mode and wait for the next AT command.
    - When sending \0, please send it as \\\0.

<a name="cmd-CLOSE"></a>
### [AT+CIPCLOSE](#TCPIP-AT)—Closes TCP/UDP/SSL Connection
Set Command (for multiple connections): 

    AT+CIPCLOSE=<link ID>
    Function: to close TCP/UDP connection.
Parameters:

- **\<link ID>**: ID number of connections to be closed; when ID=5, all connections will be closed.

Execute Command (for single connection):

    AT+CIPCLOSE
Response:

    OK  

<a name="cmd-IFSR"></a>
### [AT+CIFSR](#TCPIP-AT)—Gets the Local IP Address
Execute Command:

    AT+CIFSR    
Response:

    +CIFSR:<SoftAP IP address>
    +CIFSR:<Station IP address>
    OK
Parameters:

- **\<IP address>**: 
    - IP address of the ESP32 SoftAP;
    - IP address of the ESP32 Station.

***Notes:***

* Only when the ESP32 Station is connected to an AP can the Station IP can be queried.

<a name="cmd-MUX"></a>
### [AT+CIPMUX](#TCPIP-AT)—Enables/Disables Multiple Connections
Query Command:

    AT+CIPMUX?
    Function: to query the connection type.
Response:

    +CIPMUX:<mode>
    OK
Set Command:

    AT+CIPMUX=<mode>
    Function: to set the connection type.
Response:

    OK
Parameters:

- **\<mode>**: 
    - 0: single connection
    - 1: multiple connections

***Notes:***

* The default mode is single connection mode.
* Multiple connections can only be set when transparent transmission is disabled (`AT+CIPMODE=0`).
* This mode can only be changed after all connections are disconnected.
* If the TCP server is running, it must be deleted (`AT+CIPSERVER=0`) before the single connection mode is activated.

Example:

    AT+CIPMUX=1 

<a name="cmd-SERVER"></a>
### [AT+CIPSERVER](#TCPIP-AT)—Deletes/Creates TCP or SSL Server
Set Command:

    AT+CIPSERVER=<mode>[,<port>][,<SSL>,<SSL CA enable>]    
Response:

    OK  
Parameters:

- **\<mode>**:
    - 0: delete server.
    - 1: create server.
- **\<port>**: port number; 333 by default.
- **\[ESP32 Only] [\<SSL>]**(optional parameter): string "SSL", to set a SSL server
- **\[ESP32 Only] [\<SSL CA enable>]**(optional parameter):
    - 0: disable CA.
    - 1: enable CA.

***Notes:***

* A TCP server can only be created when multiple connections are activated (`AT+CIPMUX=1`).
* A server monitor will automatically be created when the TCP server is created. And only one server is allowed.
* When a client is connected to the server, it will take up one connection and be assigned an ID.

Example:

    // To create a TCP server
    AT+CIPMUX=1
    AT+CIPSERVER=1,80
    // To create a SSL server
    AT+CIPMUX=1
    AT+CIPSERVER=1,443,"SSL",1

<a name="cmd-SERVERMAX"></a>
### [AT+CIPSERVERMAXCONN](#TCPIP-AT)—Set the Maximum Connections Allowed by Server
Query Command:

    AT+CIPSERVERMAXCONN?
    Function: obtain the maximum number of clients allowed to connect to the TCP or SSL server.
Response:

    +CIPSERVERMAXCONN:<num>
    OK  
Set Command:

    AT+CIPSERVERMAXCONN=<num>
    Function: set the maximum number of clients allowed to connect to the TCP or SSL server.    
Response:

    OK  
Parameters:

- **\<num>**:  the maximum number of clients allowed to connect to the TCP or SSL server.

***Notes:***

* To set this configuration, you should call the command `AT+CIPSERVERMAXCONN=<num>` before creating a server.

Example:

    AT+CIPMUX=1
    AT+CIPSERVERMAXCONN=2
    AT+CIPSERVER=1,80

<a name="cmd-IPMODE"></a>
### [AT+CIPMODE](#TCPIP-AT)—Configures the Transmission Mode
Query Command:

    AT+CIPMODE?
    Function: to obtain information about transmission mode.
Response:

    +CIPMODE:<mode>
    OK
Set Command:

    AT+CIPMODE=<mode>
    Function: to set the transmission mode.
Response:

    OK
Parameters:

- **\<mode>**: 
    - 0: normal transmission mode.
    - 1: UART-Wi-Fi passthrough mode (transparent transmission), which can only be enabled in TCP single connection mode or in UDP mode when the remote IP and port do not change.

***Notes:***

* The configuration changes will NOT be saved in flash.
* During the UART-WiFi passthrough transmission, if the TCP connection breaks, ESP32 will keep trying to reconnect until `+++` is input to exit the transmission. 
* If it is a normal TCP transmission and the TCP connection breaks, ESP32 will give a prompt and will not attempt to reconnect.

Example:

    AT+CIPMODE=1    

<a name="cmd-SAVET"></a>
### [AT+SAVETRANSLINK](#TCPIP-AT)—Saves the Transparent Transmission Link in Flash
#### Save TCP Single Connection in Flash
Set Command:

    AT+SAVETRANSLINK=<mode>,<remote IP or domain name>,<remote port>[,<type>,<TCP keep alive>]  
Response:

    OK
Parameters:

- **\<mode>**: 
    - 0: normal mode, ESP32 will NOT enter UART-WiFi passthrough mode on power-up.
    - 1: ESP32 will enter UART-WiFi passthrough mode on power-up.
- **\<remote IP>**: remote IP or domain name.
- **\<remote port>**: remote port.
- **\[\<type>]**(optional parameter): TCP or UDP, TCP by default.
- **\[\<TCP keep alive>]**(optional parameter): TCP is kept alive. This function is disabled by default.
    - 0: disables the TCP keep-alive function.
    - 1 ~ 7200: keep-alive detection time interval; unit: second (s).

***Notes:***

* This command will save the UART-WiFi passthrough mode and its link in the NVS area. ESP32 will enter the UART-WiFi passthrough mode on any subsequent power cycles.
* As long as the remote IP (or domain name) and port are valid, the configuration will be saved in flash.

Example:

    AT+SAVETRANSLINK=1,"192.168.6.110",1002,"TCP"   
#### Save UDP Transmission in Flash
Set Command:

    AT+SAVETRANSLINK=<mode>,<remote IP>,<remote port>,<type>[,<UDP local port>] 
Response:

    OK
Parameters:

- **\<mode>**: 
    - 0: normal mode; ESP32 will NOT enter UART-WiFi passthrough mode on power-up.
    - 1: ESP32 enters UART-WiFi passthrough mode on power-up.
- **\<remote IP>**: remote IP or domain name.
- **\<remote port>**: remote port.
- **\[\<type>]**(optional parameter): UDP, TCP by default.
- **\[\<UDP local port>]**(optional parameter): local port when UDP transparent transmission is enabled on power-up.

***Notes:***

* This command will save the UART-WiFi passthrough mode and its link in the NVS area. ESP32 will enter the UART-WiFi passthrough mode on any subsequent power cycles.
* As long as the remote IP (or domain name) and port are valid, the configuration will be saved in flash.

Example:

    AT+SAVETRANSLINK=1,"192.168.6.110",1002,"UDP",1005  

<a name="cmd-STO"></a>
### [AT+CIPSTO](#TCPIP-AT)—Sets the TCP Server Timeout
Query Command:

    AT+CIPSTO?
    Function: to check the TCP server timeout.
Response:

    +CIPSTO:<time>
    OK
Set Command:

    AT+CIPSTO=<time>
    Function: to set the TCP server timeout.
Response:

    OK
Parameter:

- **\<time>**: TCP server timeout within the range of 0 ~ 7200s.    

***Notes:***

* ESP32 configured as a TCP server will disconnect from the TCP client that does not communicate with it until timeout.
* If `AT+CIPSTO=0`, the connection will never time out. This configuration is not recommended.

Example:

    AT+CIPMUX=1
    AT+CIPSERVER=1,1001
    AT+CIPSTO=10

<a name="cmd-SNTPCFG"></a>
### [AT+CIPSNTPCFG](#TCPIP-AT) — Set the Time Zone and the SNTP Server
Query Command:

    AT+CIPSNTPCFG?
Response:

    +CIPSNTPCFG:<enable>,<timezone>,<SNTP server1>[,<SNTP server2>,<SNTP server3>]
    OK
Set Command:

    AT+CIPSNTPCFG=<enable>,<timezone>[,<SNTP server1>,<SNTP server2>,<SNTP server3>]
Response:

    OK
Parameters:

- **\<enable>**:
    - 1: the SNTP server is configured.
    - 0: the SNTP server is not configured.
- **\<timezone>**: Time zones support two formats.
    - The first format range is [-12,14].  
    It marks most of the time zones on land are offset from Coordinated Universal Time (UTC) by a whole number of hours ([UTC−12:00](https://en.wikipedia.org/wiki/UTC%E2%88%9212:00) to [UTC+14:00](https://en.wikipedia.org/wiki/UTC%2B14:00)).
    - The second format is `UTC offset`.  
    The `UTC offset` specifies the time value you must add to the UTC time to get a local time value. It has syntax like `[+|-]hh[mm]`. This is negative if the local time zone is west of the Prime Meridian and positive if it is east. The hour(hh) must be between -12 and 14, and the minute(mm) between 0 and 59. For example, set `timezone` to `1245`, it marks the local time of New Zealand (Chatham Islands) which in `UTC+12:45`, more UTC offset details see as [wiki](https://en.wikipedia.org/wiki/Time_zone#List_of_UTC_offsets).
- **\<SNTP server1>**: the first SNTP server.
- **\<SNTP server2>**: the second SNTP server.
- **\<SNTP server3>**: the third SNTP server.

***Note:***

* If the three SNTP servers are not configured, the following default configuration is used: "cn.ntp.org.cn", "ntp.sjtu.edu.cn", "us.pool.ntp.org".

Example:

    // enable sntp server, set timezone to China(UTC+08:00)
    AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"
    or
    AT+CIPSNTPCFG=1,800,"cn.ntp.org.cn","ntp.sjtu.edu.cn"

    // enable sntp server, set timezone to New York of the United States(UTC−05:00)
    AT+CIPSNTPCFG=1,-5,"0.pool.ntp.org","time.google.com"
    or
    AT+CIPSNTPCFG=1,-500,"0.pool.ntp.org","time.google.com"

    // enable sntp server, set timezone to New Zealand(Chatham Islands, UTC+12:45)
    AT+CIPSNTPCFG=1,1245,"0.pool.ntp.org","time.google.com"

<a name="cmd-SNTPT"></a>
### [AT+CIPSNTPTIME](#TCPIP-AT)—Queries the SNTP Time
Query Command:

    AT+CIPSNTPTIME? 
Response:

    +CIPSNTPTIME:<asctime style time>
    OK
Example:

    AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"
    OK
    AT+CIPSNTPTIME?
    +CIPSNTPTIME:Mon Dec 12 02:33:32 2016
    OK  

***Note:***

* asctime style time is defined at [asctime man page](https://linux.die.net/man/3/asctime)

<a name="cmd-UPDATE"></a>
### [AT+CIUPDATE](#TCPIP-AT) - Upgrade the Firmware Through Wi-Fi
esp-at upgrades firmware at runtime by downloading new firmware from specific server through Wi-Fi and then flash it into some partitions.

Execute Command:

    AT+CIUPDATE  
    Function: Upgrade OTA the latest version of firmware via TCP from server.
Response:

    +CIPUPDATE:<n>
    OK
Or

    ERROR

Set Command:

    AT+CIUPDATE=<ota mode>[,<version>],[,<firmware name>]
    Function: Upgrade the specified version of firmware from server.
Response:

    +CIPUPDATE:<n>
    OK
Or

    ERROR

Parameters:
- **\<ota mode>**:
    - 0: OTA via TCP
    - 1: OTA via TLS, please ensure `make menuconfig` > `Component config` > `AT` > `OTA based upon ssl` is enabled.
- **\<version>**: AT version, for example, `v1.2.0.0`, `v1.1.3.0`,`v1.1.2.0`
- **\<firmware name>**: Firmware name to upgrade, for example, `ota`, `mqtt_ca`, `client_ca` or other custom partition in `at_customize.csv`
- **\<n>**:
    - 1: find the server.
    - 2: connect to server.
    - 3: get the software version.
    - 4: start updating.

Example:

    AT+CIUPDATE  
    AT+CIUPDATE=1,"v1.2.0.0"
    AT+CIUPDATE=1,"v2.2.0.0","mqtt_ca"

***Notes:***

* The speed of the upgrade is susceptible to the connectivity of the network.
* ERROR will be returned if the upgrade fails due to unfavourable network conditions. Please wait for some time before retrying.

***Notice:***

* If using Espressif's AT [BIN](https://www.espressif.com/zh-hans/support/download/at), `AT+CIUPDATE` will download a new AT BIN from the Espressif Cloud.
* If using a user-compiled AT BIN, users need to implement their own AT+CIUPDATE FOTA function. esp-at project provides an example of [FOTA](https://github.com/espressif/esp-at/blob/master/main/at_upgrade.c).
* It is suggested that users call `AT+RESTORE` to restore the factory default settings after upgrading the AT firmware.

<a name="cmd-IPDINFO"></a>
### [AT+CIPDINFO](#TCPIP-AT)—Shows the Remote IP and Port with "+IPD"
Set Command:

    AT+CIPDINFO=<mode>  
Response:

    OK  
Parameters:

- **\<mode>**: 
    - 0: does not show the remote IP and port with "+IPD" and "+CIPRECVDATA".
    - 1: shows the remote IP and port with "+IPD" and "+CIPRECVDATA".

Example:

    AT+CIPDINFO=1   

### [+IPD](#TCPIP-AT)—Receives Network Data
Command:

    Single connection: 
    (+CIPMUX=0)+IPD,<len>[,<remote IP>,<remote port>]:<data>
    multiple connections: 
    (+CIPMUX=1)+IPD,<link ID>,<len>[,<remote IP>,<remote port>]:<data>
Parameters:

- **\[\<remote IP>]**: remote IP string, enabled by command `AT+CIPDINFO=1`.
- **\[\<remote port>]**: remote port, enabled by command `AT+CIPDINFO=1`.
- **\<link ID>**: ID number of connection.
- **\<len>**: data length.
- **\<data>**: data received.

***Note:***

* The command is valid in normal command mode. When the module receives network data, it will send the data through the serial port using the `+IPD` command.

<a name="cmd-SSLCCONF"></a>
### [AT+CIPSSLCCONF](#TCPIP-AT)—Config SSL client
Query Command:

    AT+CIPSSLCCONF?
    Function: to get the configuration of each link that running as SSL client.
Response:

    +CIPSSLCCONF:<link ID>,<auth_mode>,<pki_number>,<ca_number>
    OK
Set Command:

    Single connection: (+CIPMUX=0)
    AT+CIPSSLCCONF=<auth_mode>[,<pki_number>][,<ca_number>]
    Multiple connections: (+CIPMUX=1)
    AT+CIPSSLCCONF=<link ID>,<auth_mode>[,<pki_number>][,<ca_number>]
Response:

    OK
Parameters:

- **\<link ID>**: ID of the connection (0~max), for multiple connections, if the value is max, it means all connections. By default, max is 5.
- **\<auth_mode>**: 
    - 0: no authorization. In this case, \<pki_number> and \<ca_number> are not required.
    - 1: load cert and private key for server authorization.
    - 2: load CA for client authorize server cert and private key.
    - 3: both authorization.
- **\<pki_number>**: the index of cert and private key, if only one cert and private key, the value should be 0.
- **\<ca_number>**: the index of CA, if only one CA, the value should be 0.

***Notes:***

* Send this command before establish SSL connection if you want configuration take effect immediately.
* The configuration changes will be saved in the NVS area. If you use AT+SAVETRANSLINK to set SSL passthrough mode, the ESP will establish an SSL connection based on this configuration after next power on.

<a name="cmd-SSLCCN"></a>
### [AT+CIPSSLCCN](#TCPIP-AT)—Config SSL client common name
Query Command:

    AT+CIPSSLCCN?
    Function: to get the common name configuration of each link that running as SSL client.
Response:

    +CIPSSLCCN:<link ID>,<"common name">
    OK
Set Command:

    Single connection: (+CIPMUX=0)
    AT+CIPSSLCCN=<"common name">
    Multiple connections: (+CIPMUX=1)
    AT+CIPSSLCCN=<link ID>,<"common name">
Response:

    OK
Parameters:

- **\<link ID>**: ID of the connection (0~max), for single connection, link ID is 0;  
    for multiple connections, if the value is max, it means all connections, max is 5 by default.
- **\<"common name">**: common name is used to verify the commonName in the certificate sent by the server.

***Notes:***

* Send this command before establish SSL connection if you want configuration take effect immediately.

<a name="cmd-SSLCSNI"></a>
### [AT+CIPSSLCSNI](#TCPIP-AT)—Config SSL client server name indication
Query Command:

    AT+CIPSSLCSNI?
    Function: to get the SNI configuration of each link that running as SSL client.
Response:

    +CIPSSLCSNI:<link ID>,<"sni">
    OK
Set Command:

    Single connection: (+CIPMUX=0)
    AT+CIPSSLCSNI=<"sni">
    Multiple connections: (+CIPMUX=1)
    AT+CIPSSLCSNI=<link ID>,<"sni">
Response:

    OK
Parameters:

- **\<link ID>**: ID of the connection (0~max), for single connection, link ID is 0;  
    for multiple connections, if the value is max, it means all connections, max is 5 by default.
- **\<"sni">**: Set TLS extension servername (SNI) in ClientHello

***Notes:***

* Send this command before establish SSL connection if you want configuration take effect immediately.

<a name="cmd-SSLCALPN"></a>
### [AT+CIPSSLCALPN](#TCPIP-AT)—Config SSL client application layer protocol negotiation(ALPN)
Query Command:

    AT+CIPSSLCALPN?
    Function: to get the ALPN configuration of each link that running as SSL client.
Response:

    +CIPSSLCALPN:<link ID>,<"alpn">[,<"alpn">[,<"alpn">]]
    OK
Set Command:

    Single connection: (+CIPMUX=0)
    AT+CIPSSLCALPN=<counts>,<"alpn">[,<"alpn">[,<"alpn">]]
    Multiple connections: (+CIPMUX=1)
    AT+CIPSSLCALPN=<link ID>,<counts>,<"alpn">[,<"alpn">[,<"alpn">]]
Response:

    OK
Parameters:

- **\<link ID>**: ID of the connection (0~max), for single connection, link ID is 0;  
    for multiple connections, if the value is max, it means all connections, max is 5 by default.
- **\<counts>**: ALPN counts
- **\<"alpn">**: Set ALPN in ClientHello

***Notes:***

* Send this command before establish SSL connection if you want configuration take effect immediately.

<a name="cmd-SSLCPSK"></a>
### [AT+CIPSSLCPSK](#TCPIP-AT)—Config SSL client pre-shared key(PSK)
Query Command:

    AT+CIPSSLCPSK?
    Function: to get the PSK configuration of each link that running as SSL client.
Response:

    +CIPSSLCPSK:<link ID>,<"psk">,<"hint">
    OK
Set Command:

    Single connection: (+CIPMUX=0)
    AT+CIPSSLCPSK=<"psk">,<"hint">
    Multiple connections: (+CIPMUX=1)
    AT+CIPSSLCPSK=<link ID>,<"psk">,<"hint">
Response:

    OK
Parameters:

- **\<link ID>**: ID of the connection (0~max), for single connection, link ID is 0;  
    for multiple connections, if the value is max, it means all connections, max is 5 by default.
- **\<"psk">**: PSK identity, maxlen is 32.
- **\<"hint">**: PSK hint, maxlen is 32.

***Notes:***

* Send this command before establish SSL connection if you want configuration take effect immediately.
* This command is not currently supported on ESP32.

<a name="cmd-AUTOCONNINT"></a>
### [AT+CIPRECONNINTV](#TCPIP-AT)—Set Wi-Fi transparent transmitting auto-connect interval
Query Command:

    AT+CIPRECONNINTV?
    Function: to get Wi-Fi transparent transmitting auto-connect interval .
Response:

    +CIPRECONNINTV:<interval>
    OK

Set Command:

    AT+CIPRECONNINTV=<interval>
    Function: to set the interval of auto reconnecting when the TCP/UDP/SSL transmission broke in UART-WiFi transparent mode.
Parameters:

- **\<interval>**: Time interval for automatic reconnection, default is 1, range is 1~36000, unit is 100ms.

Example:

    AT+CIPRECONNINTV=10  

**Notes:** 

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`. 

### [+IPD](#TCPIP-AT)—Receives Network Data
Command:

    Single connection: 
    (+CIPMUX=0)+IPD,<len>[,<remote IP>,<remote port>]:<data>
    multiple connections: 
    (+CIPMUX=1)+IPD,<link ID>,<len>[,<remote IP>,<remote port>]:<data>
Parameters:

- **\[\<remote IP>]**: remote IP, enabled by command `AT+CIPDINFO=1`.
- **\[\<remote port>]**: remote port, enabled by command `AT+CIPDINFO=1`.
- **\<link ID>**: ID number of connection.
- **\<len>**: data length.
- **\<data>**: data received.

***Note:***

* The command is valid in normal command mode. When the module receives network data, it will send the data through the serial port using the `+IPD` command.

<a name="cmd-CIPRECVMODE"></a>
### [AT+CIPRECVMODE](#TCPIP-AT)—Set Socket Receive Mode
Query Command:

    AT+CIPRECVMODE?
    Function: to query socket receive mode.
Response:

    +CIPRECVMODE:<mode>
    OK
Set Command:

    AT+CIPRECVMODE=<mode>
Response:

    OK

Parameters:
- **\<mode>**: the receive mode of socket data is active mode by default.
    - 0: active mode - ESP-AT will send all the received socket data instantly to host MCU through UART with header “+IPD".
    - 1: passive mode - ESP-AT will keep the received socket data in an internal buffer (default is 5744 bytes), and wait for host MCU to read the data. If the buffer is full, the socket transmission will be blocked.

Example:

    AT+CIPRECVMODE=1   

***Notes:***

* The configuration is for TCP and SSL transmission only, and can not be used on WiFi-UART passthrough mode. If it is a UDP transmission in passive mode，data will be missed when buffer full.

* If the passive mode is enabled, when ESP-AT receives socket data, it will prompt the following message in different scenarios: 
    - for multiple connection mode (AT+CIPMUX=1), the message is: `+IPD,<link ID>,<len>`
    - for single connection mode (AT+CIPMUX=0), the message is: `+IPD,<len>`
    - `<len>` is the total length of socket data in buffer
    - User should read data by command `AT+CIPRECVDATA` once there is a `+IPD` reported. Otherwise, the next `+IPD` will not be reported to the host MCU, until the previous `+IPD` be read by command `AT+CIPRECVDATA`.
    - In a case of disconnection, the buffered socket data will still be there and can be read by MCU until you send `AT+CIPCLOSE`. Specially, if `+IPD` has been reported, the message `CLOSED` of this connection will never come until you send `AT+CIPCLOSE` or read all data by command `AT+CIPRECVDATA`.

<a name="cmd-CIPRECVDATA"></a>
### [AT+CIPRECVDATA](#TCPIP-AT)—Get Socket Data in Passive Receive Mode
Set Command:

    Single connection: (+CIPMUX=0)
    AT+CIPRECVDATA=<len>
    Multiple connections: (+CIPMUX=1)
    AT+CIPRECVDATA=<link_id>,<len>
Response:

    +CIPRECVDATA:<actual_len>,<data>
    OK

or

    +CIPRECVDATA:<actual_len>,<remote IP>,<remote port>,<data>
    OK

Parameters:

- **\<link_id>**: connection ID in multiple connection mode.
- **\<len>**: the max value is up to 0x7fffffff, if the actual length of the received data is less than `len`, the actual length will be returned.
- **\<actual_len>**: length of the data you actually get
- **\<data>**: the data you get
- **\[\<remote IP>]**: remote IP string, enabled by command `AT+CIPDINFO=1`.
- **\[\<remote port>]**: remote port, enabled by command `AT+CIPDINFO=1`.

Example:

    AT+CIPRECVMODE=1
    For example, if host MCU gets a message of receiving 100 bytes data in connection with No.0, the message will be as following: +IPD,0,100
    then you can read those 100 bytes by using the command below
    AT+CIPRECVDATA=0,100

<a name="cmd-CIPRECVLEN"></a>
### [AT+CIPRECVLEN](#TCPIP-AT)—Get Socket Data Length in Passive Receive Mode
Query Command:

    AT+CIPRECVLEN?
    Function: query the length of the entire data buffered for the link.
Response:

    +CIPRECVLEN:<data length of link0>,<data length of link1>,<data length of link2>,<data length of link3>,<data length of link4>
    OK

Parameters:
- **\<data length of link>**: length of the entire data buffered for the link

Example:

    AT+CIPRECVLEN?
    +CIPRECVLEN:100,,,,,
    OK

***Notes:***

* For ssl link, it will return the length of encrypted data, so the returned length will be more than the real data length.

<a name="cmd-CIPPING"></a>
### [AT+PING](#TCPIP-AT): Ping Packets
Set Command:

    AT+PING=<IP>
    Function: Ping packets.
Response:

    +PING:<time>

    OK
or

    +timeout

    ERROR

Parameters:
- **\<IP>**: string; host IP or domain name
- **\<time>**: the response time of ping, unit: millisecond.

Example:

    AT+PING="192.168.1.1"
    AT+PING="www.baidu.com"

<a name="cmd-DNS"></a>
### [AT+CIPDNS](#TCPIP-AT) : Configures Domain Name System.
Query Command:

    AT+CIPDNS?
    Function: to obtain current Domain Name System information.
Response:

    +CIPDNS:<enable>[,<"DNS IP1">,<"DNS IP2">,<"DNS IP3">]
    OK

Set Command:

    AT+CIPDNS=<enable>[,<"DNS IP1">,<"DNS IP2">,<"DNS IP3">]
    Function: Configures Domain Name System.
Response:

    OK
or

    ERROR

Parameters:

- **\<enable>**: 
    - 0: Enable automatic DNS settings from DHCP, the DNS will be restore to `222.222.67.208`, only when DHCP is updated will it take effect.
    - 1: Enable manual DNS settings, if not set `DNS IP`, It will use `222.222.67.208` by default.
- **\<DNS IP1>**: the first DNS IP. For set command, only for manual DNS settings; for query command, it is current DNS setting.
- **\<DNS IP2>**: the second DNS IP. For set command, only for manual DNS settings; for query command, it is current DNS setting.
- **\<DNS IP3>**: the third DNS IP. For set command, only for manual DNS settings; for query command, it is current DNS setting.

Example:

    AT+CIPDNS=0
    AT+CIPDNS=1,"222.222.67.208","114.114.114.114","8.8.8.8"

**Notes:** 

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.  
* The three <DNS IP>parameters cannot be set to the same server.  
* The DNS server may change according to the configuration of the router which the ESP chip connected to.  


<a name="cmd-TCPOPT"></a>
### [AT+CIPTCPOPT](#TCPIP-AT) : Configurates the socket options.
Query Command:

    AT+CIPTCPOPT?
    Function: to obtain current socket options information.
Response:

    +CIPTCPOPT:<link_id>,<so_linger>,<tcp_nodelay>,<so_sndtimeo>
    OK

Set Command:

    Single TCP connection (AT+CIPMUX=0):
    AT+CIPTCPOPT=[<so_linger>],[<tcp_nodelay>],[<so_sndtimeo>]
    Multiple TCP Connections (AT+CIPMUX=1):
    AT+CIPTCPOPT=<link ID>,[<so_linger>],[<tcp_nodelay>],[<so_sndtimeo>]
Response:

    OK
or

    ERROR

Parameters:

- **\<link_id>**: ID of the connection(0~max), for multiple connections, if the value is max, it means all connections. By default, max is 5.
- **\<so_linger>**: configurate the SO_LINGER options for socket, in second, default: -1.
    - = -1: off
    - = 0: on, linger time = 0
    - > 0: on, linger time = <so_linger>
- **\<tcp_nodelay>**: configurate the TCP_NODELAY options for socket.
    - 0: disable TCP_NODELAY, default
    - 1: enable TCP_NODELAY
- **\<so_sndtimeo>**: configurate the SO_SNDTIMEO options for socket, in millisecond, default: 0.

