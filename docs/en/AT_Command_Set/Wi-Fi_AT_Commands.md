<a name="WiFi-AT"></a>
## Wi-Fi AT Commands
* [AT+CWMODE](#cmd-MODE) : Sets the Wi-Fi mode (STA/AP/STA+AP).
* [AT+CWJAP](#cmd-JAP) : Connects to an AP.
* [AT+CWRECONNCFG](#cmd-RECONNCFG) : Configurates WiFi reconnect interval and maximum times.
* [AT+CWLAPOPT](#cmd-LAPOPT) : Sets the configuration of command AT+CWLAP.
* [AT+CWLAP](#cmd-LAP) : Lists available APs.
* [AT+CWQAP](#cmd-QAP) : Disconnects from the AP.
* [AT+CWSAP](#cmd-SAP) : Sets the configuration of the ESP SoftAP.
* [AT+CWLIF](#cmd-LIF) : Gets the Station IP to which the ESP SoftAP is connected.
* [AT+CWQIF](#cmd-QIF) : Disconnect Station from the ESP SoftAP.
* [AT+CWDHCP](#cmd-DHCP) : Enables/disables DHCP.
* [AT+CWDHCPS](#cmd-DHCPS) : Sets the IP range of the ESP SoftAP DHCP server.
* [AT+CWAUTOCONN](#cmd-AUTOC) : Connects to the AP automatically on power-up.
* [AT+CWAPPROTO](#cmd-APPROTO) : Sets the 802.11 b/g/n protocol standard of SoftAP mode.
* [AT+CWSTAPROTO](#cmd-STAPROTO) : Sets the 802.11 b/g/n protocol standard of station mode.
* [AT+CIPSTAMAC](#cmd-STAMAC) : Sets the MAC address of ESP Station.
* [AT+CIPAPMAC](#cmd-APMAC) : Sets the MAC address of ESP SoftAP.
* [AT+CIPSTA](#cmd-IPSTA) : Sets the IP address of ESP Station.
* [AT+CIPAP](#cmd-IPAP) : Sets the IP address of ESP SoftAP.
* [AT+CWSTARTSMART](#cmd-STARTS) : Starts SmartConfig.
* [AT+CWSTOPSMART](#cmd-STOPS) : Stops SmartConfig.
* [AT+WPS](#cmd-WPS) : Enables the WPS function.
* [AT+MDNS](#cmd-MDNS) : Configurates the MDNS function
* [ESP32 Only] [AT+CWJEAP](#cmd-JEAP) : Connects to a WPA2 Enterprise AP.
* [AT+CWHOSTNAME](#cmd-HOSTNAME) : Configures the Name of ESP Station
* [AT+CWCOUNTRY](#cmd-COUNTRY) : Configures the Wi-Fi Country Code

 
<a name="cmd-MODE"></a>
### [AT+CWMODE](#WiFi-AT)—Sets the Wi-Fi Mode (Station/SoftAP/Station+SoftAP)  
Query Command: 

    AT+CWMODE?
    Function: to query the Wi-Fi mode of ESP32.
Response:

    +CWMODE:<mode>
    OK
Set Command: 

    AT+CWMODE=<mode>[,<auto_connect>]
    Function: to set the Wi-Fi mode of ESP32.
Response:

    OK
Parameters:

- **\<mode>**:
    - 0: Null mode, WiFi RF will be disabled
    - 1: Station mode
    - 2: SoftAP mode
    - 3: SoftAP+Station mode    

- **\<auto_connect>**:
    - 0: Do not connect to WiFi when WiFi mode change to Station or Station+SoftAP
    - 1: Connect to WiFi when WiFi mode change to Station or Station+SoftAP (default configuration)

***Note:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.  

Example:

    AT+CWMODE=3 

<a name="cmd-JAP"></a>
### [AT+CWJAP](#WiFi-AT)—Connects to an AP
Query Command: 

    AT+CWJAP?
    Function: to query the AP to which the ESP32 Station is already connected.
Response:

    +CWJAP:<ssid>,<bssid>,<channel>,<rssi>,<pci_en>,<reconn_interval>,<listen_interval>,<scan_mode>
    OK
Parameters:

- **\<ssid>**: a string parameter showing the SSID of the AP.
- **\<bssid>**: the AP’s MAC address.
- **\<channel>**: channel
- **\<rssi>**: signal strength
- **\[\<pci_en>]**: PCI Authentication, which will disable connect OPEN and WEP AP.
- **\[\<reconn_interval>]**: Wi-Fi reconnection, when beacon timeout, ESP32 will reconnect automatically.
- **\[\<listen_interval>]**: the interval of listening to the AP's beacon,the range is (0,100]
- **\[\<scan_mode>]**: 
    - 0: Do fast scan, scan will end after find SSID match AP, Wi-Fi will connect the first scanned AP.
    - 1: All channel scan, scan will end after scan all the channel, Wi-Fi will connect the AP with the strongest signal scanned.

Set Command:

    AT+CWJAP=<ssid>,<pwd>[,<bssid>][,<pci_en>][,<reconn_interval>][,<listen_interval>][,<scan_mode>]
    Function: to set the AP to which the ESP32 Station needs to be connected.
Response:

    OK
or
    +CWJAP:<error code>
    ERROR
Parameters:

- **\<ssid>**: the SSID of the target AP.
    - Escape character syntax is needed if SSID or password contains any special characters, such as , or " or \\.
- **\<pwd>**: password, MAX: 64-byte ASCII.
- **\[\<bssid>]**: the target AP's MAC address, used when multiple APs have the same SSID.
- **\[\<pci_en>]**: enable PCI Authentication, which will disable connect OPEN and WEP AP.
- **\[\<reconn_interval>]**: configurate Wi-Fi reconnect interval, when disconnected for AP, esp-at will reconnect automatically, unit: seconds.
- **\[\<listen_interval>]**: the interval of listening to the AP's beacon,the range is (0,100], by default, the value is 3.
- **\<error code>**: (for reference only)
    - 1: connection timeout.
    - 2: wrong password.
    - 3: cannot find the target AP.
    - 4: connection failed.
    - others: unknown error occurred.
- **\[\<scan_mode>]**: 
    - 0: Do fast scan, scan will end after find SSID match AP, Wi-Fi will connect the first scanned AP.
    - 1: All channel scan, scan will end after scan all the channel, Wi-Fi will connect the AP with the strongest signal scanned.

***Note:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.
* This command requires Station mode to be active.
* When reconn is omitted, the last reconnection mechanism will be adopted; if not, this command will change the <interval_second> parameter of `AT+CWRECONNCFG`.

Examples:

    AT+CWJAP="abc","0123456789"
    For example, if the target AP's SSID is "ab\,c" and the password is "0123456789"\", the command is as follows:
    AT+CWJAP="ab\\\,c","0123456789\"\\"
    If multiple APs have the same SSID as "abc", the target AP can be found by BSSID:
    AT+CWJAP="abc","0123456789","ca:d7:19:d8:a6:44" 

<a name="cmd-RECONNCFG"></a>
### [AT+CWRECONNCFG](#WiFi-AT)—Configurates WiFi reconnect interval and maximum times
Query Command:

    AT+CWRECONNCFG?
    Function: to query the configuration of WiFi reconnect.

Response:

    +CWRECONNCFG:<interval_second>,<repeat_count>
    OK

Set Command:

    AT+CWRECONNCFG=<interval_second>,<repeat_count>
    Function: to set the configuration of WiFi reconnect.

Response:

    OK

Parameters:

- **\<interval_second>**: WiFi reconnect interval second, unit: second, maximum value: 7200, defaut value: 0.
    - 0: esp-at will not reconnect to AP when WiFi disconnected.
    - [1,7200]: esp-at will reconnect to AP after <interval_second> seconds when WiFi disconnected.
- **\<repeat_count>**: WiFi reconnect to AP times, maximum value: 1000, defaut value: 0, this parameter only works when <interval_second> is not 0.
    - 0: esp-at will always try to reconnect to AP.
    - [1,1000]: attempt to reconnect to AP <repeat_count> times.

Examples:

    1. esp-at will try to reconnect to AP 100 times, every reconnection interval is one second.
        AT+CWRECONNCFG=1,100
    2. esp-at will not reconnect to AP when WiFi disconnected.
        AT+CWRECONNCFG=0,0

***Note:***

* The <interval_second> parameter of this command is same to the <reconn_interval> parameter of `AT+CWJAP`.
* This command only works for passive disconnect from AP.

<a name="cmd-LAPOPT"></a>
### [AT+CWLAPOPT](#WiFi-AT) — Set the Configuration for the Command AT+CWLAP
Set Command:

    AT+CWLAPOPT=<sort_enable>,<print mask>[,<rssi filter>][,<authmode mask>]
Response:

    OK
or

    ERROR

Parameters:
- **\<sort_enable>**: determines whether the result of command AT+CWLAP will be listed according to RSSI, default: 0.
    - 0: the result is not ordered according to RSSI.
    - 1: the result is ordered according to RSSI.
- **\<print mask>**: determines the parameters shown in the result of `AT+CWLAP`, default: 0x1F.
    - 0 means not showing the parameter corresponding to the bit, and 1 means showing it.
    - bit 0: determines whether \<ecn> will be shown in the result of `AT+CWLAP`.
    - bit 1: determines whether \<ssid> will be shown in the result of `AT+CWLAP`.
    - bit 2: determines whether \<rssi> will be shown in the result of `AT+CWLAP`.
    - bit 3: determines whether \<mac> will be shown in the result of `AT+CWLAP`.
    - bit 4: determines whether \<channel> will be shown in the result of `AT+CWLAP`.
- **\[\<rssi filter>]**(optional parameter): determines whether the result of command AT+CWLAP will be listed according to `rssi filter`. Valid range: -100 to 40, default value: -100.
    > The result of command AT+CWLAP will `NOT` show the AP which signal strength in dBm below `rssi filter`.
- **\[\<authmode mask>]**(optional parameter): determines the parameters shown in the result of `AT+CWLAP`, default: 0xFF.
    - 0 means not showing the any AP information corresponding to the bit, and 1 means showing it.
    - bit 0: determines whether authmode is `OPEN` AP will be shown in the result of `AT+CWLAP`.
    - bit 1: determines whether authmode is `WEP` AP will be shown in the result of `AT+CWLAP`.
    - bit 2: determines whether authmode is `WPA_PSK` AP will be shown in the result of `AT+CWLAP`.
    - bit 3: determines whether authmode is `WPA2_PSK` AP will be shown in the result of `AT+CWLAP`.
    - bit 4: determines whether authmode is `WPA_WPA2_PSK` AP will be shown in the result of `AT+CWLAP`.
    - bit 5: determines whether authmode is `WPA2_ENTERPRISE` AP will be shown in the result of `AT+CWLAP`.
    - bit 6: determines whether authmode is `WPA3_PSK` AP will be shown in the result of `AT+CWLAP`, just for ESP32 and ESP32S2 platform now.
    - bit 7: determines whether authmode is `WPA2_WPA3_PSK` AP will be shown in the result of `AT+CWLAP`, just for ESP32 and ESP32S2 platform now.

Example:

    // The first parameter is 1, meaning that the result of the command AT+CWLAP will be ordered according to RSSI;
    // The second parameter is 31, namely 0x1F, meaning that the corresponding bits of <mask> are set to 1. All parameters will be shown in the result of AT+CWLAP.
    AT+CWLAPOPT=1,31

    // Just show the AP which authmode is `OPEN` way
    AT+CWLAPOPT=1,31,-100,1

<a name="cmd-LAP"></a>
### [AT+CWLAP](#WiFi-AT)—Lists the Available APs
Set Command: 

    AT+CWLAP=[<ssid>,<mac>,<channel>,<scan_type>,<scan_time_min>,<scan_time_max>]
    Function: to query the APs with specific SSID and MAC on a specific channel.
Execute Command:

    AT+CWLAP
    Function: to list all available APs.
Response:

    +CWLAP:<ecn>,<ssid>,<rssi>,<mac>,<channel>
    OK
Parameters:

- **\<ecn>**: encryption method.
    - 0: OPEN
    - 1: WEP
    - 2: WPA\_PSK
    - 3: WPA2\_PSK
    - 4: WPA\_WPA2\_PSK
    - 5: WPA2\_Enterprise (AT can NOT connect to WPA2\_Enterprise AP for now.)
- **\<ssid>**: string parameter, SSID of the AP.
- **\<rssi>**: signal strength.
- **\<mac>**: string parameter, MAC address of the AP. 
- **\<scan_type>**: Wi-Fi scan type, active or passive.
    - 0: active scan 
    - 1: passive scan
- **\<scan_time_min>**: minimum active scan time per channel, units: millisecond, range [0,1500], if the scan type is passive, this param is invalid.
- **\<scan_time_max>**: maximum active scan time per channel, units: millisecond, range [0,1500]. if this param is zero, the firmware will use the default time, active scan type is 120ms , passive scan type is 360ms.

Examples:

    AT+CWLAP="Wi-Fi","ca:d7:19:d8:a6:44",6,0,400,1000
    Or search for APs with a designated SSID: 
    AT+CWLAP="Wi-Fi"

<a name="cmd-QAP"></a>
### [AT+CWQAP](#WiFi-AT)—Disconnects from the AP
Execute Command:

    AT+CWQAP
Response:

    OK

<a name="cmd-SAP"></a>
### [AT+CWSAP](#WiFi-AT)—Configuration of the ESP32 SoftAP
Query Command: 

    AT+CWSAP?
    Function: to obtain the configuration parameters of the ESP32 SoftAP.
Response:

    +CWSAP:<ssid>,<pwd>,<channel>,<ecn>,<max conn>,<ssid hidden>
    OK
Set Command:

    AT+CWSAP=<ssid>,<pwd>,<chl>,<ecn>[,<max conn>][,<ssid hidden>]
    Function: to set the configuration of the ESP32 SoftAP.
Response:

    OK
Parameters:

- **\<ssid>**: string parameter, SSID of AP.
- **\<pwd>**: string parameter, length of password: 8 ~ 64 bytes ASCII.
- **\<channel>**: channel ID.
- **\<ecn>**: encryption method; WEP is not supported.
    - 0: OPEN
    - 2: WPA\_PSK
    - 3: WPA2\_PSK
    - 4: WPA\_WPA2\_PSK
- **\[\<max conn>]**(optional parameter): maximum number of Stations to which ESP32 SoftAP can be connected; within the range of [1, 10].
- **\[\<ssid hidden>]**(optional parameter):
    - 0: SSID is broadcast. (the default setting)
    - 1: SSID is not broadcast.

***Note:***

* This command is only available when SoftAP is active.
* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.  

Example:

    AT+CWSAP="ESP32","1234567890",5,3   

<a name="cmd-LIF"></a>
### [AT+CWLIF](#WiFi-AT)—IP of Stations to Which the ESP32 SoftAP is Connected
Execute Command:

    AT+CWLIF
Response:

    <ip addr>,<mac>
    OK
Parameters:

- **\<ip addr>**: IP address of Stations to which ESP32 SoftAP is connected.
- **\<mac>**: MAC address of Stations to which ESP32 SoftAP is connected.

***Note:***

* This command cannot get a static IP. It only works when both DHCPs of the ESP32 SoftAP, and of the Station to which ESP32 is connected, are enabled.

<a name="cmd-QIF"></a>
### [AT+CWQIF](#WiFi-AT)—Disconnect Station from the ESP SoftAP
Execute Command:

    AT+CWQIF
    Function: Disconnect all stations that connected to the ESP SoftAP.
Response:

    OK

Set Command:

    AT+CWQIF=<mac>
    Function: Disconnect the station whose mac is "<mac>" from the ESP SoftAP.
Response:

    OK

Parameters:

- **\<mac>**: MAC address of the station to disconnect to.

<a name="cmd-DHCP"></a>
### [AT+CWDHCP](#WiFi-AT)—Enables/Disables DHCP
Query Command: 

    AT+CWDHCP?
Response:
    state
    
Set Command: 

    AT+CWDHCP=<operate>,<mode>
    Function: to enable/disable DHCP.
Response:

    OK
Parameters:

- **\<operate>**: 
    - 0: disable
    - 1: enable
- **\<mode>**: 
    - Bit0: Station DHCP
    - Bit1: SoftAP DHCP
- **\<state>**:DHCP disabled or enabled now?
    Bit0: 
        0: Station DHCP is disabled.
        1: Station DHCP is enabled.
    Bit1: 
        0: SoftAP DHCP is disabled.
        1: SoftAP DHCP is enabled.
***Notes:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.
* This set command interacts with static-IP-related AT commands(AT+CIPSTA-related and AT+CIPAP-related commands): 
    * If DHCP is enabled, static IP will be disabled;
    * If static IP is enabled, DHCP will be disabled;
    * Whether it is DHCP or static IP that is enabled depends on the last configuration.

Examples:

    AT+CWDHCP=1,1    //Enable Station DHCP. If the last DHCP mode is 2, then the current DHCP mode will be 3.
    AT+CWDHCP=0,2    //Disable SoftAP DHCP. If the last DHCP mode is 3, then the current DHCP mode will be 1.   

<a name="cmd-DHCPS"></a>
### [AT+CWDHCPS](#WiFi-AT)—Sets the IP Address Allocated by ESP32 SoftAP DHCP
Query Command:

    AT+CWDHCPS?
Response:

    +CWDHCPS=<lease time>,<start IP>,<end IP>
    OK
Set Command: 

    AT+CWDHCPS=<enable>,<lease time>,<start IP>,<end IP>
    Function: sets the IP address range of the ESP32 SoftAP DHCP server.
Response:

    OK
Parameters:

- **\<enable>**: 
    - 0: Disable the settings and use the default IP range. 
    - 1: Enable setting the IP range, and the parameters below have to be set.
- **\<lease time>**: lease time, unit: minute, range [1, 2880].
- **\<start IP>**: start IP of the IP range that can be obtained from ESP32 SoftAP DHCP server.
- **\<end IP>**: end IP of the IP range that can be obtained from ESP32 SoftAP DHCP server.

***Notes:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.
* This AT command is enabled when ESP8266 runs as SoftAP, and when DHCP is enabled. 
* The IP address should be in the same network segment as the IP address of ESP32 SoftAP.

Examples:

    AT+CWDHCPS=1,3,"192.168.4.10","192.168.4.15"
    or
    AT+CWDHCPS=0 //Disable the settings and use the default IP range.

<a name="cmd-AUTOC"></a>
### [AT+CWAUTOCONN](#WiFi-AT)—Auto-Connects to the AP or Not
Set Command:

    AT+CWAUTOCONN=<enable>
Response:  

    OK
Parameters:

- **\<enable>**: 
    - 0: does NOT auto-connect to AP on power-up.
    - 1: connects to AP automatically on power-up.

***Note:***

* The configuration changes will be saved in the NVS area.
* The ESP32 Station connects to the AP automatically on power-up by default.

Example:

    AT+CWAUTOCONN=1

<a name="cmd-APPROTO"></a>
### [AT+CWAPPROTO](#WiFi-AT)—Sets the 802.11 b/g/n protocol standard of SoftAP mode.
Query Command:

    AT+CWAPPROTO?
Response:

    +CWAPPROTO=<protocol>
    OK
Set Command:

    AT+CWAPPROTO=<protocol>
Response:  

    OK
Parameters:

- **\<protocol>**: 
    - bit0: 802.11b protocol standard.
    - bit1: 802.11g protocol standard.
    - bit2: 802.11n protocol standard.

***Note:***

* ESP8266 Currently only support 802.11b or 802.11bg mode
* ESP32 Currently only support 802.11b or 802.11bg or 802.11bgn mode

<a name="cmd-STAPROTO"></a>
### [AT+CWSTAPROTO](#WiFi-AT)—Sets the 802.11 b/g/n protocol standard of station mode.
Query Command:

    AT+CWSTAPROTO?
Response:

    +CWSTAPROTO=<protocol>
    OK
Set Command:

    AT+CWSTAPROTO=<protocol>
Response:  

    OK
Parameters:

- **\<protocol>**: 
    - bit0: 802.11b protocol standard.
    - bit1: 802.11g protocol standard.
    - bit2: 802.11n protocol standard.

***Note:***

* Currently we only support 802.11b or 802.11bg or 802.11bgn mode

<a name="cmd-STAMAC"></a>
### [AT+CIPSTAMAC](#WiFi-AT)—Sets the MAC Address of the ESP32 Station
Query Command:

    AT+CIPSTAMAC?
    Function: to obtain the MAC address of the ESP32 Station.
Response:

    +CIPSTAMAC:<mac>
    OK
Set Command: 

    AT+CIPSTAMAC=<mac>
    Function: to set the MAC address of the ESP32 Station.
Response:

    OK
Parameters:

- **\<mac>**: string parameter, MAC address of the ESP8266 Station. 

***Notes:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.
* The MAC address of ESP32 SoftAP is different from that of the ESP32 Station. Please make sure that you do not set the same MAC address for both of them.
* Bit 0 of the ESP32 MAC address CANNOT be 1. 
    * For example, a MAC address can be "1a:…" but not "15:…".
* FF:FF:FF:FF:FF:FF and 00:00:00:00:00:00 are invalid MAC and cannot be set.

Example:

    AT+CIPSTAMAC="1a:fe:35:98:d3:7b"    

<a name="cmd-APMAC"></a>
### [AT+CIPAPMAC](#WiFi-AT)—Sets the MAC Address of the ESP32 SoftAP
Query Command:

    AT+CIPAPMAC?
    Function: to obtain the MAC address of the ESP32 SoftAP.
Response:

    +CIPAPMAC:<mac>
    OK
Set Command: 

    AT+CIPAPMAC=<mac>
    Function: to set the MAC address of the ESP32 SoftAP.
Response:

    OK
Parameters:

- **\<mac>**: string parameter, MAC address of the ESP8266 SoftAP.  

***Notes:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.
* The MAC address of ESP32 SoftAP is different from that of the ESP32 Station. Please make sure that you do not set the same MAC address for both of them.
* Bit 0 of the ESP32 MAC address CANNOT be 1. 
    * For example, a MAC address can be "18:…" but not "15:…".
* FF:FF:FF:FF:FF:FF and 00:00:00:00:00:00 are invalid MAC and cannot be set.

Example:

    AT+CIPAPMAC="18:fe:35:98:d3:7b" 

<a name="cmd-IPSTA"></a>
### [AT+CIPSTA](#WiFi-AT)—Sets the IP Address of the ESP32 Station
Query Command:

    AT+CIPSTA?
    Function: to obtain the IP address of the ESP32 Station.
    Notice: Only when the ESP32 Station is connected to an AP can its IP address be queried.
Response:

    +CIPSTA:<ip>
    OK
Set Command:

    AT+CIPSTA=<ip>[,<gateway>,<netmask>]
    Function: to set the IP address of the ESP32 Station.
Response:

    OK
Parameters:

- **\<ip>**: string parameter, the IP address of the ESP32 Station.
- **\[\<gateway>]**: gateway.
- **\[\<netmask>]**: netmask.

***Notes:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.
* The set command interacts with DHCP-related AT commands (AT+CWDHCP-related commands): 
    * If static IP is enabled, DHCP will be disabled;
    * If DHCP is enabled, static IP will be disabled;
    * Whether it is DHCP or static IP that is enabled depends on the last configuration.

Example:

    AT+CIPSTA="192.168.6.100","192.168.6.1","255.255.255.0" 

<a name="cmd-IPAP"></a>
### [AT+CIPAP](#WiFi-AT)—Sets the IP Address of the ESP32 SoftAP
Query Command:

    AT+CIPAP?
    Function: to obtain the IP address of the ESP32 SoftAP.
Response:

    +CIPAP:<ip>,<gateway>,<netmask>
    OK
Set Command:

    AT+CIPAP=<ip>[,<gateway>,<netmask>]
    Function: to set the IP address of the ESP32 SoftAP.
Response:

    OK
Parameters:

- **\<ip>**: string parameter, the IP address of the ESP32 SoftAP.
- **\[\<gateway>]**: gateway.
- **\[\<netmask>]**: netmask.

***Notes:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.
* The set command interacts with DHCP-related AT commands (AT+CWDHCP-related commands): 
    * If static IP is enabled, DHCP will be disabled;
    * If DHCP is enabled, static IP will be disabled;
    * Whether it is DHCP or static IP that is enabled depends on the last configuration.

Example:

    AT+CIPAP="192.168.5.1","192.168.5.1","255.255.255.0"

<a name="cmd-STARTS"></a>
### [AT+CWSTARTSMART](#WiFi-AT)—Starts SmartConfig
Execute Command:

    AT+CWSTARTSMART
    Function: to start SmartConfig. (The type of SmartConfig is ESP-TOUCH + AirKiss.）
Set Command:

    AT+CWSTARTSMART=<type>
    Function: to start SmartConfig of a designated type.
Response:

    OK  
Parameters:

- **\<type>**: 
    - 1: ESP-TOUCH
    - 2: AirKiss
    - 3: ESP-TOUCH+AirKiss

***Notes:***

* For details on SmartConfig please see ESP-TOUCH User Guide.
* SmartConfig is only available in the ESP32 Station mode.
* The message `Smart get Wi-Fi info` means that SmartConfig has successfully acquired the AP information. ESP32 will try to connect to the target AP.
* Message`Smartconfig connected Wi-Fi` is printed if the connection is successful. 
* Use command `AT+CWSTOPSMART` to stop SmartConfig before running other commands. Please make sure that you do not execute other commands during SmartConfig.

Example:

    AT+CWMODE=1
    AT+CWSTARTSMART

<a name="cmd-STOPS"></a>
### [AT+CWSTOPSMART](#WiFi-AT)—Stops SmartConfig
Execute Command:

    AT+CWSTOPSMART
Response:

    OK
***Note:***

* Irrespective of whether SmartConfig succeeds or not, before executing any other AT commands, please always call `AT+CWSTOPSMART` to release the internal memory taken up by SmartConfig.

Example:

    AT+CWMODE=1
    AT+CWSTARTSMART
    AT+CWSTOPSMART

<a name="cmd-WPS"></a>
### [AT+WPS](#WiFi-AT)—Enables the WPS Function
Set Command:

    AT+WPS=<enable>
Response:

    OK 
Parameters:

- **\<enable>**: 
    - 1: enable WPS/Wi-Fi Protected Setup (implemented by PBC/Push Button Configuration).
    - 0: disable WPS (implemented by PBC).

***Notes:***

* WPS must be used when the ESP32 Station is enabled.
* WPS does not support WEP/Wired-Equivalent Privacy encryption.

Example:

    AT+CWMODE=1
    AT+WPS=1
    
<a name="cmd-MDNS"></a>
### [AT+MDNS](#WiFi-AT)—Configurates the MDNS Function
Set Command:

    AT+MDNS=<enable>[,<hostname>,<service_name>,<port>]
Response:

    OK 
Parameters:

- **\<enable>**:
    - 1: enables the MDNS function; the following three parameters need to be set.
    - 0: disables the MDNS function; the following three parameters need not to be set.
- **\<hostname>**: MDNS host name
- **\<service_name>**: MDNS service name
- **\<port>**: MDNS port

Example:

    AT+MDNS=1,"espressif","_iot",8080  
    AT+MDNS=0

<a name="cmd-JEAP"></a>
### [AT+CWJEAP](#WiFi-AT)—Connects to an WPA2 Enterprise AP.
Query Command:

    AT+CWJEAP?
    Function: to query the Enterprise AP to which the ESP32 Station is already connected.
Response:

    +CWJEAP:<ssid>,<method>,<identity>,<username>,<password>,<security>
    OK
Set Command:

    AT+CWJEAP=<ssid>,<method>,<identity>,<username>,<password>,<security>
    Function: to set the Enterprise AP to which the ESP32 Station needs to be connected.
Response:

    OK
or
    +CWJEAP:Timeout
    ERROR
Parameters:

- **\<ssid>**: the SSID of the Enterprise AP.
    - Escape character syntax is needed if SSID or password contains any special characters, such as , or " or \\.
- **\<method>**: wpa2 enterprise authentication method.
    - 0: EAP-TLS.
    - 1: EAP-PEAP.
    - 2: EAP-TTLS.
- **\<identity>**: identity for phase 1, string limited to 1~32.
- **\<username>**: username for phase 2, must set for EAP-PEAP and EAP-TTLS mode, nor care for EAP-TLS, string limited to 1~32.
- **\<password>**: password for phase 2, must set for EAP-PEAP and EAP-TTLS mode, nor care for EAP-TLS, string limited to 1~32.
- **\<security>**:
    - Bit0: Client certificate
    - Bit1: Server certificate

Example:

    1. Connect to EAP-TLS mode enterprise AP, set identity, verify server certificate and load client certificate
    AT+CWJEAP="dlink11111",0,"example@espressif.com",,,3
    2. Connect to EAP-PEAP mode enterprise AP, set identity, username and password, not verify server certificate and not load client certificate
    AT+CWJEAP="dlink11111",1,"example@espressif.com","espressif","test11",0

**Error Code:**
The WPA2 enterprise Error code will be prompt as `ERR CODE:0x<%08x>`.
```
    AT_EAP_MALLOC_FAILED,               // 0x8001
    AT_EAP_GET_NVS_CONFIG_FAILED,       // 0x8002
    AT_EAP_CONN_FAILED,                 // 0x8003
    AT_EAP_SET_WIFI_CONFIG_FAILED,      // 0x8004
    AT_EAP_SET_IDENTITY_FAILED,         // 0x8005
    AT_EAP_SET_USERNAME_FAILED,         // 0x8006
    AT_EAP_SET_PASSWORD_FAILED,         // 0x8007
    AT_EAP_GET_CA_LEN_FAILED,           // 0x8008
    AT_EAP_READ_CA_FAILED,              // 0x8009
    AT_EAP_SET_CA_FAILED,               // 0x800A
    AT_EAP_GET_CERT_LEN_FAILED,         // 0x800B
    AT_EAP_READ_CERT_FAILED,            // 0x800C
    AT_EAP_GET_KEY_LEN_FAILED,          // 0x800D
    AT_EAP_READ_KEY_FAILED,             // 0x800E
    AT_EAP_SET_CERT_KEY_FAILED,         // 0x800F
    AT_EAP_ENABLE_FAILED,               // 0x8010
    AT_EAP_ALREADY_CONNECTED,           // 0x8011
    AT_EAP_GET_SSID_FAILED,             // 0x8012
    AT_EAP_SSID_NULL,                   // 0x8013
    AT_EAP_SSID_LEN_ERROR,              // 0x8014
    AT_EAP_GET_METHOD_FAILED,           // 0x8015
    AT_EAP_CONN_TIMEOUT,                // 0x8016
    AT_EAP_GET_IDENTITY_FAILED,         // 0x8017
    AT_EAP_IDENTITY_LEN_ERROR,          // 0x8018
    AT_EAP_GET_USERNAME_FAILED,         // 0x8019
    AT_EAP_USERNAME_LEN_ERROR,          // 0x801A
    AT_EAP_GET_PASSWORD_FAILED,         // 0x801B
    AT_EAP_PASSWORD_LEN_ERROR,          // 0x801C
    AT_EAP_GET_SECURITY_FAILED,         // 0x801D
    AT_EAP_SECURITY_ERROR,              // 0x801E
    AT_EAP_METHOD_SECURITY_UNMATCHED,   // 0x801F
    AT_EAP_PARAMETER_COUNTS_ERROR,      // 0x8020
    AT_EAP_GET_WIFI_MODE_ERROR,         // 0x8021
    AT_EAP_WIFI_MODE_NOT_STA,           // 0x8022
    AT_EAP_SET_CONFIG_FAILED,           // 0x8023
    AT_EAP_METHOD_ERROR,                // 0x8024
```
***Note:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.	
* This command requires Station mode to be active.
* TLS mode will use client certificate, make sure enabled.

<a name="cmd-HOSTNAME"></a>
### [AT+CWHOSTNAME](#WiFi-AT) : Configures the Name of ESP Station
Query Command:

    AT+CWHOSTNAME?
    Function: Checks the host name of ESP Station.
Response:

    +CWHOSTNAME:<hostname>

    OK
Set Command:

    AT+CWHOSTNAME=<hostname>
    Function: Sets the host name of ESP Station.
Response:

    OK

If the Station mode is not enabled, the command will return:

    ERROR
Parameters:

- **\<hostname>**: the host name of the ESP Station, the maximum length is 32 bytes.

***Note:***

* The configuration changes are not saved in the flash.

Example:

    AT+CWMODE=3
    AT+CWHOSTNAME="my_test"



<a name="cmd-COUNTRY"></a>
### [AT+CWCOUNTRY](#WiFi-AT) : Configures the Wi-Fi Country Code
Query Command:

    AT+CWCOUNTRY?
    Function: Query Wi-Fi country code information.
Response:

    +CWCOUNTRY:<country_policy>,<country_code>,<start_channel>,<total_channel_count>

    OK
Set Command:

    AT+ CWCOUNTRY=<country_policy>,<country_code>,<start_channel>,<total_channel_count>
    Function: Sets the Wi-Fi country code information.
Response:

    OK

Parameters:  

- **\<country_policy>**:  
	- 0: will change the county code to be the same as the AP that ESP is connected to
	- 1: the country code will not change, always be the one set by command.
- **\<country_code>**:  country code, the length can be 3 characters at most; 
- **\<start_channel>**:  the channel number to start, range [1,14]
- **\<total_channel_count>**:  total channel count

***Note:***

* The configuration changes are not saved in the flash.

Example:

    AT+CWMODE=3
    AT+CWCOUNTRY=1,"CN",1,13
