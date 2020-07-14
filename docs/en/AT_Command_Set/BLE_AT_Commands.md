<a name="BLE-AT"></a>
## [ESP32 Only] BLE AT Commands
[Download BLE Spec (ESP32 supports Core Version 4.2)](https://www.bluetooth.com/specifications/adopted-specifications)  

* [ESP32 Only] [AT+BLEINIT](#cmd-BINIT) : Bluetooth Low Energy (BLE) initialization
* [ESP32 Only] [AT+BLEADDR](#cmd-BADDR) : Sets BLE device's address
* [ESP32 Only] [AT+BLENAME](#cmd-BNAME) : Sets BLE device's name
* [ESP32 Only] [AT+BLESCANPARAM](#cmd-BSCANP) : Sets parameters of BLE scanning
* [ESP32 Only] [AT+BLESCAN](#cmd-BSCAN) : Enables BLE scanning
* [ESP32 Only] [AT+BLESCANRSPDATA](#cmd-BSCANR) : Sets BLE scan response
* [ESP32 Only] [AT+BLEADVPARAM](#cmd-BADVP) : Sets parameters of BLE advertising
* [ESP32 Only] [AT+BLEADVDATA](#cmd-BADVD) : Sets BLE advertising data
* [ESP32 Only] [AT+BLEADVDATAEX](#cmd-BADVDEX) : Auto sets BLE advertising data
* [ESP32 Only] [AT+BLEADVSTART](#cmd-BADVSTART) : Starts BLE advertising
* [ESP32 Only] [AT+BLEADVSTOP](#cmd-BADVSTOP) : Stops BLE advertising
* [ESP32 Only] [AT+BLECONN](#cmd-BCONN) : Establishes BLE connection
* [ESP32 Only] [AT+BLECONNPARAM](#cmd-BCONNP) : Updates parameters of BLE connection
* [ESP32 Only] [AT+BLEDISCONN](#cmd-BDISC) : Ends BLE connection
* [ESP32 Only] [AT+BLEDATALEN](#cmd-BDLEN) : Sets BLE data length
* [ESP32 Only] [AT+BLECFGMTU](#cmd-BMTU) : Sets BLE MTU length
* [ESP32 Only] [AT+BLEGATTSSRVCRE](#cmd-GSSRVCRE) : Generic Attributes Server (GATTS) creates services
* [ESP32 Only] [AT+BLEGATTSSRVSTART](#cmd-GSSRVSTART) : GATTS starts services
* [ESP32 Only] [AT+BLEGATTSSRVSTOP](#cmd-GSSRVSTOP)—GATTS Stops Services
* [ESP32 Only] [AT+BLEGATTSSRV](#cmd-GSSRV) : GATTS discovers services
* [ESP32 Only] [AT+BLEGATTSCHAR](#cmd-GSCHAR) : GATTS discovers characteristics
* [ESP32 Only] [AT+BLEGATTSNTFY](#cmd-GSNTFY) : GATTS notifies of characteristics
* [ESP32 Only] [AT+BLEGATTSIND](#cmd-GSIND) : GATTS indicates characteristics
* [ESP32 Only] [AT+BLEGATTSSETATTR](#cmd-GSSETA) : GATTS sets attributes
* [ESP32 Only] [AT+BLEGATTCPRIMSRV](#cmd-GCPRIMSRV) : Generic Attributes Client (GATTC) discovers primary services
* [ESP32 Only] [AT+BLEGATTCINCLSRV](#cmd-GCINCLSRV) : GATTC discovers included services
* [ESP32 Only] [AT+BLEGATTCCHAR](#cmd-GCCHAR) : GATTC discovers characteristics
* [ESP32 Only] [AT+BLEGATTCRD](#cmd-GCRD) : GATTC reads characteristics
* [ESP32 Only] [AT+BLEGATTCWR](#cmd-GCWR) : GATTC writes characteristics
* [ESP32 Only] [AT+BLESPPCFG](#cmd-BLESPPCFG) : Sets BLE spp parameters
* [ESP32 Only] [AT+BLESPP](#cmd-BLESPP) : Enter BLE spp mode
* [ESP32 Only] [AT+BLESECPARAM](#cmd-BLESMPPAR) : Set BLE encryption parameters
* [ESP32 Only] [AT+BLEENC](#cmd-BLEENC) : Initiate BLE encryption request
* [ESP32 Only] [AT+BLEENCRSP](#cmd-BLEENCRSP) : Grant security request access.
* [ESP32 Only] [AT+BLEKEYREPLY](#cmd-BLEKEYREPLY) : Reply the key value to the peer device in the lagecy connection stage.
* [ESP32 Only] [AT+BLECONFREPLY](#cmd-BLECOFREPLY) : Reply the comfirm value to the peer device in the lagecy connection stage.
* [ESP32 Only] [AT+BLEENCDEV](#cmd-BLEENCDEV) : Query BLE encryption device list
* [ESP32 Only] [AT+BLEENCCLEAR](#cmd-BLEENCCLEAR) : Clear BLE encryption device list
* [ESP32 Only] [AT+BLESETKEY](#cmd-BLESETKEY) : Set BLE static pair key
* [ESP32 Only] [AT+BLEHIDINIT](#cmd-BLEHIDINIT) : BLE HID device profile initialization
* [ESP32 Only] [AT+BLEHIDKB](#cmd-BLEHIDKB) : Send BLE HID Keyboard information
* [ESP32 Only] [AT+BLEHIDMUS](#cmd-BLEHIDMUS) : Send BLE HID mouse information
* [ESP32 Only] [AT+BLEHIDCONSUMER](#cmd-BLEHIDC) : Send BLE HID consumer information
* [ESP32 Only] [AT+BLUFI](#cmd-BLUFI) : Start or Stop BLUFI
* [ESP32 Only] [AT+BLUFINAME](#cmd-BLUFINAME) : Set BLUFI device name


<a name="cmd-BINIT"></a>
### [ESP32 Only] [AT+BLEINIT](#BLE-AT)—BLE Initialization
Query Command:

    AT+BLEINIT?
    Function: to check the initialization status of BLE.
Response:

If BLE is not initialized, it will return

    +BLEINIT:0
    OK
If BLE is initialized, it will return

    +BLEINIT:<role>
    OK
Set Command: 

    AT+BLEINIT=<init>
    Function: to initialize the role of BLE.
Response:

    OK
Parameter:

- **\<init>**: 
    - 0: deinit ble
    - 1: client role
    - 2: server role

***Notes:***

* at_customize.bin has to be downloaded, so that the relevant commands can be used. Please refer to the [ESP32_Customize_Partitions](https://github.com/espressif/esp-at/tree/master/docs) for more details.
* Before using BLE AT commands, this command has to be called first to trigger the initialization process.
* After being initialized, the BLE role cannot be changed. User needs to call AT+RST to restart the system first, then re-init the BLE role.
* If using ESP32 as a BLE server, a service bin should be downloaded into Flash.
    * To learn how to generate a service bin, please refer to esp-at/tools/readme.md.
    * The download address of the service bin is the "ble_data" address in esp-at/partitions_at.csv.  

Example:

    AT+BLEINIT=1    

<a name="cmd-BADDR"></a>
### [ESP32 Only] [AT+BLEADDR](#BLE-AT)—Sets BLE Device's Address
Query Command:

    AT+BLEADDR?
    Function: to get the BLE public address.
Response:

    +BLEADDR:<BLE_public_addr>
    OK
Set Command:

    AT+BLEADDR=<addr_type>[,<random_addr>]
    Function: to set the BLE address type.
Response:

    OK
Parameter:

- **\<addr_type>**: 
    - 0: public address
    - 1: random address  

***Notes:***

* A static address shall meet the following requirements:
    - The two most significant bits of the address shall be equal to 1
    - At least one bit of the random part of the address shall be 0
    - At least one bit of the random part of the address shall be 1

Example:

    AT+BLEADDR=1,"f8:7f:24:87:1c:7b"    // Set Random Device Address, Static Address
    AT+BLEADDR=1                        // Set Random Device Address, Private Address
    AT+BLEADDR=0                        // Set Public Device Address

<a name="cmd-BNAME"></a>
### [ESP32 Only] [AT+BLENAME](#BLE-AT)—Sets BLE Device's Name
Query Command:

    AT+BLENAME?
    Function: to get the BLE device name.
Response:

    +BLENAME:<device_name>
    OK
Set Command:

    AT+BLENAME=<device_name>
    Function: to set the BLE device name, The maximum length is 32.
Response:

    OK
Parameter:

- **\<device_name>**: the BLE device name

***Notes:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`. 
* The default BLE device name is "BLE_AT".  

Example:

    AT+BLENAME="esp_demo"   

<a name="cmd-BSCANP"></a>
### [ESP32 Only] [AT+BLESCANPARAM](#BLE-AT)—Sets Parameters of BLE Scanning
Query Command:

    AT+BLESCANPARAM?
    Function: to get the parameters of BLE scanning.
Response:

    +BLESCANPARAM:<scan_type>,<own_addr_type>,<filter_policy>,<scan_interval>,<scan_window>
    OK
Set Command:

    AT+BLESCANPARAM=<scan_type>,<own_addr_type>,<filter_policy>,<scan_interval>,<scan_window>
    Function: to set the parameters of BLE scanning.
Response:

    OK
Parameters:

- **\<scan_type>**:
    - 0: passive scan
    - 1: active scan
- **\<own\_addr\_type>**:
    - 0: public address
    - 1: random address
    - 2: RPA public address
    - 3: RPA random address
- **\<filter_policy>**:
    - 0: BLE\_SCAN\_FILTER\_ALLOW\_ALL
    - 1: BLE\_SCAN\_FILTER\_ALLOW\_ONLY\_WLST
    - 2: BLE\_SCAN\_FILTER\_ALLOW\_UND\_RPA\_DIR
    - 3: BLE\_SCAN\_FILTER\_ALLOW\_WLIST\_PRA\_DIR
- **\<scan_interval>**: scan interval
- **\<scan_window>**: scan window

***Notes:***

* \<scan\_window> CANNOT be larger than \<scan\_interval>.  

Example:

    AT+BLEINIT=1   // role: client
    AT+BLESCANPARAM=0,0,0,100,50

<a name="cmd-BSCAN"></a>
### [ESP32 Only] [AT+BLESCAN](#BLE-AT)—Enables BLE Scanning
Set Command: 

    AT+BLESCAN=<enable>[,<interval>][,<filter_type>,<filter_param>]
    Function: to enable/disable scanning.
Response:

    +BLESCAN:<addr>,<rssi>,<adv_data>,<scan_rsp_data>,<addr_type>
    OK
Parameters:

- **\<enable>**:
    - 0: disable continuous scanning
    - 1: enable continuous scanning
- **\[\<interval>]**: optional parameter, unit: second
    - When disabling the scanning, this parameter should be omitted
    - When enabling the scanning, and the `<interval>` is 0, it means that scanning is continuous
    - When enabling the scanning, and the `<interval>` is NOT 0, for example, command `AT+BLESCAN=1,3`, it means that scanning should last for 3 seconds and then stop automatically, so that the scanning results be returned.
- **\<filter_type>**: Filtering option, 1:MAC or 2:"NAME".
- **\<filter_param>**: Filtering param, remote evice mac address or remote device name.
- **\<addr>**: BLE address
- **\<rssi>**: signal strength
- **\<adv_data>**: advertising data
- **\<scan\_rsp_data>**: scan response data
- **\<addr_type>**: the address type of broadcasters

Example:

    AT+BLEINIT=1   // role: client
    AT+BLESCAN=1    // start scanning
    AT+BLESCAN=0     // stop scanning
    AT+BLESCAN=1,3,1,"24:0A:C4:96:E6:88"    // start scanning, filter type is MAC address
    AT+BLESCAN=1,3,2,"ESP-AT"    // start scanning, filter type is device name

<a name="cmd-BSCANR"></a>
### [ESP32 Only] [AT+BLESCANRSPDATA](#BLE-AT)—Sets BLE Scan Response
Set Command: 

    AT+BLESCANRSPDATA=<scan_rsp_data>
    Function: to set scan response.
Response:

    OK  
Parameter:

- **\<scan\_rsp\_data>**: scan response data is a HEX string. 
    - For example, to set the response data as "0x11 0x22 0x33 0x44 0x55", the command should be `AT+BLESCANRSPDATA="1122334455"`.  

Example:

    AT+BLEINIT=2   // role: server
    AT+BLESCANRSPDATA="1122334455"

<a name="cmd-BADVP"></a>
### [ESP32 Only] [AT+BLEADVPARAM](#BLE-AT)—Sets Parameters of Advertising
Query Command: 

    AT+BLEADVPARAM?
    Function: to query the parameters of advertising.
Response:

    +BLEADVPARAM:<adv_int_min>,<adv_int_max>,<adv_type>,<own_addr_type>,<channel_map>,<adv_filter_policy>,<peer_addr_type>,<peer_addr>
    OK
Set Command: 

    AT+BLEADVPARAM=<adv_int_min>,<adv_int_max>, <adv_type>,<own_addr_type>,<channel_map>[,<adv_filter_policy>][,<peer_addr_type>] [,<peer_addr>]
    Function: to set the parameters of advertising.
Response:

    OK
Parameters:

- **\<adv\_int\_min>**: minimum value of advertising interval; range: 0x0020 ~ 0x4000
- **\<adv\_int\_max>**: maximum value of advertising interval; range: 0x0020 ~ 0x4000
- **\<adv_type>**:
    - 0：ADV\_TYPE\_IND
    - 2：ADV\_TYPE\_SCAN\_IND
    - 3：ADV\_TYPE\_NONCONN\_IND
- **\<own\_addr\_type>**：own BLE address type
    - 0：BLE\_ADDR\_TYPE\_PUBLIC
    - 1：BLE\_ADDR\_TYPE\_RANDOM
- **\<channel_map>**：channel of advertising
    - 1：ADV\_CHNL\_37
    - 2：ADV\_CHNL\_38
    - 4：ADV\_CHNL\_39
    - 7：ADV\_CHNL\_ALL
- **\[\<adv\_filter\_policy>]**(optional parameter)：filter policy of advertising
    - 0：ADV\_FILTER\_ALLOW\_SCAN\_ANY\_CON\_ANY
    - 1：ADV\_FILTER\_ALLOW\_SCAN\_WLST\_CON\_ANY
    - 2：ADV\_FILTER\_ALLOW\_SCAN\_ANY\_CON\_WLST
    - 3：ADV\_FILTER\_ALLOW\_SCAN\_WLST\_CON\_WLST
- **\[\<peer\_addr\_type>]**(optional parameter)：remote BLE address type
    - 0：PUBLIC
    - 1：RANDOM
- **\[\<peer_addr>]**(optional parameter)：remote BLE address

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEADVPARAM=50,50,0,0,4,0,0,"12:34:45:78:66:88"

<a name="cmd-BADVD"></a>
### [ESP32 Only] [AT+BLEADVDATA](#BLE-AT)—Sets Advertising Data
Set Command: 

    AT+BLEADVDATA=<adv_data>
    Function: to set advertising data.
Response:

    OK
Parameters:

- **\<adv_data>**: advertising data; this is a HEX string. 
    - For example, to set the advertising data as "0x11 0x22 0x33 0x44 0x55", the command should be `AT+BLEADVDATA="1122334455"`.

***Notes:***

* If advertising data is preset by command `AT+BLEADVDATAEX=<dev_name>,<uuid>,<manufacturer_data>,<include_power>`, it will be over write by `AT+BLEADVDATA=<adv_data>`.

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEADVDATA="1122334455"

<a name="cmd-BADVDEX"></a>
### [ESP32 Only] [AT+BLEADVDATAEX](#BLE-AT)—Auto sets BLE advertising data
Query Command: 

    AT+BLEADVDATAEX?
    Function: to query the parameters of advertising data.
Response:

    +BLEADVDATAEX:<dev_name>,<uuid>,<manufacturer_data>,<include_power>

    OK
Set Command: 

    AT+BLEADVSTART=<dev_name>,<uuid>,<manufacturer_data>,<include_power>
    Function: configurate the adv data and start advertising.
Response:

    OK
Parameters:

- **\<dev_name>**: device name; this is a string. For example:
    - to set the device name "just-test", the command should be `AT+BLEADVSTART="just-test",<uuid>,<manufacturer_data>,<include_power>`

- **\<uuid>**: this is a string. For example:
    - to set the UUID "0xA002", the command should be `AT+BLEADVSTART=<dev_name>,"A002",<manufacturer_data>,<include_power>`.

- **\<manufacturer_data>**: manufacturer data; this is a HEX string, For example:
    - to set the manufacturer data as "0x11 0x22 0x33 0x44 0x55", the command should be `AT+BLEADVSTART=<dev_name>,<uuid>,"1122334455",<include_power>`.

- **\<include_power>**: if User need include the tx power in the advertising data, this param should be set `1`, if not, this param should be set `0`

***Notes:***

* If advertising data is preset by command `AT+BLEADVDATA=<adv_data>`, it will be over write by `AT+BLEADVDATAEX=<dev_name>,<uuid>,<manufacturer_data>,<include_power>`

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEADVDATAEX="ESP-AT","A002","0102030405",1

* [ESP32 Only] [AT+BLEADVDATAEX](#cmd-BADVD) : Auto sets BLE advertising data


<a name="cmd-BADVSTART"></a>
### [ESP32 Only] [AT+BLEADVSTART](#BLE-AT)—Starts Advertising
Execute Command:

    AT+BLEADVSTART
    Function: to start advertising.
Response:

    OK

***Notes:***

* If advertising parameters are NOT set by command `AT+BLEADVPARAM=<adv_parameter>`, the default parameters will be used.
* If advertising data is NOT set by command `AT+BLEADVDATA=<adv_data>`, the all zeros data will be sent.
* If advertising data is preset by command `AT+BLEADVDATA=<adv_data>`, it will be over write by `AT+BLEADVDATAEX=<dev_name>,<uuid>,<manufacturer_data>,<include_power>` and vice versa.

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEADVSTART

<a name="cmd-BADVSTOP"></a>
### [ESP32 Only] [AT+BLEADVSTOP](#BLE-AT)—Stops Advertising
Execute Command: 

    AT+BLEADVSTOP
    Function: to stop advertising.
Response:

    OK
***Notes:***

* After having started advertising, if the BLE connection is established successfully, it will stop advertising automatically. In such a case, this command does NOT need to be called.

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEADVSTART
    AT+BLEADVSTOP

<a name="cmd-BCONN"></a>
### [ESP32 Only] [AT+BLECONN](#BLE-AT)—Establishes BLE connection
Query Command: 

    AT+BLECONN?
    Function: to query the BLE connection.
Response:

    +BLECONN:<conn_index>,<remote_address>
    OK
If the connection has not been established, there will NOT be <conn_index> and <remote_address>
Set Command: 

    AT+BLECONN=<conn_index>,<remote_address>[,<addr_type>,<timeout>]
    Function: to establish the BLE connection, the address_type is an optional parameter.
Response:

    OK
It will prompt the message below, if the connection is established successfully:

    +BLECONN:<conn_index>,<remote_address>
It will prompt the message below, if NOT:

    +BLECONN:<conn_index>,-1
Parameters:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<remote_address>**：remote BLE address
- **\<addr_type>**: the address type of broadcasters
- **\<timeout>**: the timeout for the connection command, range is [3,30] second.

Example:

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:0a:c4:09:34:23",0,10

***Notes:***

* It is recommended to scan devices by "AT+BLESCAN" before initiating a new connection to ensure that the target device is in broadcast state.
* The maximum timeout for connection is 30 seconds.


<a name="cmd-BCONNP"></a>
### [ESP32 Only] [AT+BLECONNPARAM](#BLE-AT)—Updates parameters of BLE connection
Query Command:

    AT+BLECONNPARAM?
    Function: to query the parameters of BLE connection.
Response:  
    
    +BLECONNPARAM:<conn_index>,<min_interval>,<max_interval>,<cur_interval>,<latency>,<timeout>
    OK
Set Command:

    AT+BLECONNPARAM=<conn_index>,<min_interval>,<max_interval>,<latency>,<timeout>
    Function: to update the parameters of BLE connection.
Response:

    OK  // command received
If the setting failed, it will prompt message below:

    +BLECONNPARAM：<conn_index>,-1
Parameters:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<min_interval>**: minimum value of connecting interval; range: 0x0006 ~ 0x0C80
- **\<max_interval>**: maximum value of connecting interval; range: 0x0006 ~ 0x0C80
- **\<cur_interval>**: current connecting interval value
- **\<latency>**: latency; range: 0x0000 ~ 0x01F3
- **\<timeout>**: timeout; range: 0x000A ~ 0x0C80

***Notes:***

* This commands supports the client only when updating its connection parameters. Of course, the connection has to be established first. 

Example:

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLECONNPARAM=0,12,14,1,500  


<a name="cmd-BDISC"></a>
### [ESP32 Only] [AT+BLEDISCONN](#BLE-AT)—Ends BLE connection
Execute Command: 

    AT+BLEDISCONN=<conn_index>
    Function: to end the BLE connection.
Response:

    OK  // the AT+BLEDISCONN command is received
    If the command is successful, it will prompt + BLEDISCONN:<conn_index>,<remote_address>
Parameter:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<remote_address>**: remote BLE address  

***Notes:***

* Only client can call this command to break the connection.

Example:

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLEDISCONN=0

<a name="cmd-BDLEN"></a>
### [ESP32 Only] [AT+BLEDATALEN](#BLE-AT)—Sets BLE Data Packet Length
Set Command: 

    AT+BLEDATALEN=<conn_index>,<pkt_data_len>
    Function: to set the length of BLE data packet.
Response:

    OK 
Parameter:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<pkt\_data\_len>**: data packet's length; range: 0x001b ~ 0x00fb  

***Notes:***

* The BLE connection has to be established first.  

Example:

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLEDATALEN=0,30

<a name="cmd-BMTU"></a>
### [ESP32 Only] [AT+BLECFGMTU](#BLE-AT)—Sets BLE MTU Length
Query Command: 

    AT+BLECFGMTU?
    Function: to query the length of the maximum transmission unit (MTU).
Response:

    +BLECFGMTU:<conn_index>,<mtu_size>
    OK
Set Command: 

    AT+BLECFGMTU=<conn_index>,<mtu_size>
    Function: to set the length of the maximum transmission unit (MTU).
Response:

    OK  // the command is received
Parameter:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<mtu_size>**:  MTU length

***Notes:***

* Only the client can call this command to set the length of MTU. However, the BLE connection has to be established first.
* The actual length of MTU needs to be negotiated. The "OK" response only means that the MTU length must be set. So, the user should use command `AT+BLECFGMTU?` to query the actual MTU length.

Example:

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLECFGMTU=0,300

<a name="cmd-GSSRVCRE"></a>
### [ESP32 Only] [AT+BLEGATTSSRVCRE](#BLE-AT)—GATTS Creates Services
Execute Command: 

    AT+BLEGATTSSRVCRE
    Function: The Generic Attributes Server (GATTS) creates BLE services.
Response:

    OK
***Notes:***

* If using ESP32 as a BLE server, a service bin should be downloaded into Flash in order to provide services.
    * To learn how to generate a service bin, please refer to esp-at/tools/readme.md.
    * The download address of the service bin is the "ble_data" address in esp-at/partitions_at.csv.
* This command should be called immediately to create services, right after the BLE server is initialized. 
* If a BLE connection is established first, the service creation will fail.

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE

<a name="cmd-GSSRVSTART"></a>
### [ESP32 Only] [AT+BLEGATTSSRVSTART](#BLE-AT)—GATTS Starts Services
Execute Command: 

    AT+BLEGATTSSTART
    Function: GATTS starts all services.
Set Command: 

    AT+BLEGATTSSRVSTART=<srv_index>
    Function: GATTS starts a specific service.
Response:

    OK  
Parameter:

- **\<srv_index>**: service's index starting from 1

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART


<a name="cmd-GSSRVSTOP"></a>
### [ESP32 Only] [AT+BLEGATTSSRVSTOP](#BLE-AT)—GATTS Stops Services
Execute Command: 

    AT+BLEGATTSSTOP
    Function: GATTS stops all services.
Set Command: 

    AT+BLEGATTSSRVSTOP=<srv_index>
    Function: GATTS stops a specific service.
Response:

    OK  
Parameter:

- **\<srv_index>**: service's index starting from 1

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEGATTSSRVSTOP


<a name="cmd-GSSRV"></a>
### [ESP32 Only] [AT+BLEGATTSSRV](#BLE-AT)—GATTS Discovers Services
Query Command: 

    AT+BLEGATTSSRV?
    Function: GATTS services discovery.
Response:

    +BLEGATTSSRV:<srv_index>,<start>,<srv_uuid>,<srv_type>
    OK
Parameters:

- **\<srv_index>**: service's index starting from 1
- **\<start>**:
    - 0：the service has not started
    - 1：the service has already started
- **\<srv_uuid>**: service's UUID
- **\<srv_type>**: service's type
    - 0：is not a primary service
    - 1：is a primary service

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRV?

<a name="cmd-GSCHAR"></a>
### [ESP32 Only] [AT+BLEGATTSCHAR](#BLE-AT)—GATTS Discovers Characteristics
Query Command: 

    AT+BLEGATTSCHAR?
    Function: GATTS characteristics discovery.
Response:

When showing a characteristic, it will be as:

    +BLEGATTSCHAR:"char",<srv_index>,<char_index>,<char_uuid>,<char_prop>
When showing a descriptor, it will be as:

    +BLEGATTSCHAR:"desc",<srv_index>,<char_index>,<desc_index> 
    OK
Parameters:

- **\<srv_index>**: service's index starting from 1
- **\<char_index>**: characteristic's index starting from 1
- **\<char_uuid>**: characteristic's UUID
- **\<char_prop>**: characteristic's properties
- **\<desc_index>**: descriptor's index
- **\<desc_uuid>**: descriptor's UUID

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEGATTSCHAR?

<a name="cmd-GSNTFY"></a>
### [ESP32 Only] [AT+BLEGATTSNTFY](#BLE-AT)—GATTS Notifies of Characteristics
Set Command: 

    AT+BLEGATTSNTFY=<conn_index>,<srv_index>,<char_index>,<length>
    Function: GATTS to notify of its characteristics.
Response:

    >
Begin receiving serial data. When the requirement of data length, determined by <length>, is met, the notification starts.
If the data transmission is successful, the system returns:
    OK
Parameters:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<srv_index>**: service's index; it can be fetched with command `AT+BLEGATTSCHAR?`
- **\<char_index>**: characteristic's index; it can be fetched with command `AT+BLEGATTSCHAR?`
- **\<length>**: data length

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEADVSTART // starts advertising. After the client is connected, it must be configured to receive notifications.
    AT+BLEGATTSCHAR?  // check which characteristic the client will be notified of
    // for example, to notify of 4 bytes of data using the 6th characteristic in the 3rd service, use the following command:
    AT+BLEGATTSNTFY=0,3,6,4 
    // after > shows, inputs 4 bytes of data, such as "1234"; then, the data will be transmitted automatically

<a name="cmd-GSIND"></a>
### [ESP32 Only] [AT+BLEGATTSIND](#BLE-AT)—GATTS Indicates Characteristics
Set Command: 

    AT+BLEGATTSIND=<conn_index>,<srv_index>,<char_index>,<length>
    Function: GATTS indicates its characteristics.
Response:

    >
Begin receiving serial data. When the requirement of data length, determined by <length>, is met, the indication starts.
If the data transmission is successful, the system returns:
    OK
Parameters:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<srv_index>**: service's index; it can be fetched with command `AT+BLEGATTSCHAR?`
- **\<char_index>**: characteristic's index; it can be fetched with command `AT+BLEGATTSCHAR?`
- **\<length>**: data length

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEADVSTART // starts advertising. After the client is connected, it must be configured to receive indications.
    AT+BLEGATTSCHAR?  // check for which characteristic the client can receive indications
    // for example, to indicate 4 bytes of data using the 7th characteristic in the 3rd service, use the following command:
    AT+BLEGATTSIND=0,3,7,4 
    // after > shows, inputs 4 bytes of data, such as "1234"; then, the data will be transmitted automatically

<a name="cmd-GSSETA"></a>
### [ESP32 Only] [AT+BLEGATTSSETATTR](#BLE-AT)—GATTS Sets Characteristic
Set Command: 

    AT+BLEGATTSSETATTR=<srv_index>,<char_index>[,<desc_index>],<length>
    Function: GATTS to set its characteristic (descriptor).
Response:

    >
Begin receiving serial data. When the requirement of data length, determined by <length>, is met, the setting starts.
If the setting is successful, the system returns:
    OK
Parameters:

- **\<srv_index>**: service's index; it can be fetched with command `AT+BLEGATTSCHAR?`
- **\<char_index>**: characteristic's index; it can be fetched with command `AT+BLEGATTSCHAR?`
- **\[\<desc_index>]**(Optional parameter): descriptor's index. 
    - If it is set, this command is used to set the value of the descriptor; if it is not, this command is used to set the value of the characteristic.
- **\<length>**: data length

***Note:***

* If the \<value> length is larger than the maximum length allowed, the setting will fail. 

Example:

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEGATTSCHAR? 
    // for example, to set 4 bytes of data of the 1st characteristic in the 1st service, use the following command:
    AT+BLEGATTSSETATTR=1,1,,4
    // after > shows, inputs 4 bytes of data, such as "1234"; then, the setting starts

<a name="cmd-GCPRIMSRV"></a>
### [ESP32 Only] [AT+BLEGATTCPRIMSRV](#BLE-AT)—GATTC Discovers Primary Services
Query Command: 

    AT+BLEGATTCPRIMSRV=<conn_index>
    Function: GATTC to discover primary services.
Response:

    + BLEGATTCPRIMSRV:<conn_index>,<srv_index>,<srv_uuid>,<srv_type>
    OK
Parameters:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<srv_index>**: service's index starting from 1
- **\<srv_uuid>**: service's UUID
- **\<srv_type>**: service's type
    - 0：is not a primary service
    - 1：is a primary service

***Note:***

* The BLE connection has to be established first.

Example:

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0

<a name="cmd-GCINCLSRV"></a>
### [ESP32 Only] [AT+BLEGATTCINCLSRV](#BLE-AT)—GATTC Discovers Included Services
Set Command: 

    AT+BLEGATTCINCLSRV=<conn_index>,<srv_index>
    Function: GATTC to discover included services.
Response:

    + BLEGATTCINCLSRV:<conn_index>,<srv_index>,<srv_uuid>,<srv_type>,<included_srv_uuid>,<included_srv_type>
    OK
Parameters:

- **\<conn_index>**: index of BLE connection; only 0 is supported for the single connection right now, but multiple BLE connections will be supported in the future.
- **\<srv_index>**: service's index; it can be fetched with command `AT+BLEGATTCPRIMSRV=<conn_index>`
- **\<srv_uuid>**: service's UUID
- **\<srv_type>**: service's type
    - 0：is not a primary service
    - 1：is a primary service
- **\<included\_srv\_uuid>**: included service's UUID
- **\<included\_srv\_type>**: included service's type
    - 0：is not a primary service
    - 1：is a primary service

***Note:***

* The BLE connection has to be established first.

Example:

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCINCLSRV=0,1  // set a specific index according to the result of the previous command

<a name="cmd-GCCHAR"></a>
### [ESP32 Only] [AT+BLEGATTCCHAR](#BLE-AT)—GATTC Discovers Characteristics
Set Command: 

    AT+BLEGATTCCHAR=<conn_index>,<srv_index>
    Function: GATTC to discover characteristics.
Response:

When showing a characteristic, it will be as:

    +BLEGATTCCHAR:"char",<conn_index>,<srv_index>,<char_index>,<char_uuid>,<char_prop>
When showing a descriptor, it will be as:

    +BLEGATTCCHAR:"desc",<conn_index>,<srv_index>,<char_index>,<desc_index>,<desc_uuid> 
    OK
Parameters:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<srv_index>**: service's index; it can be fetched with command `AT+BLEGATTCPRIMSRV=<conn_index>`
- **\<char_index>**: characteristic's index starting from 1
- **\<char_uuid>**: characteristic's UUID
- **\<char_prop>**: characteristic's properties
- **\<desc_index>**: descriptor's index
- **\<desc_uuid>**: descriptor's UUID

***Note:***

* The BLE connection has to be established first.

Example:

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCCHAR=0,1 // set a specific index according to the result of the previous command

<a name="cmd-GCRD"></a>
### [ESP32 Only] [AT+BLEGATTCRD](#BLE-AT)—GATTC Reads a Characteristic
Set Command: 

    AT+BLEGATTCRD=<conn_index>,<srv_index>,<char_index>[,<desc_index>]
    Function: GATTC to read a characteristic or descriptor.
Response：

    +BLEGATTCRD:<conn_index>,<len>,<value>
    OK
Parameters：

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<srv_index>**: service's index; it can be fetched with command `AT+BLEGATTCPRIMSRV=<conn_index>`
- **\<char_index>**: characteristic's index; it can be fetched with command `AT+BLEGATTCCHAR=<conn_index>,<srv_index>`
- **\[\<desc_index>]**(Optional parameter): descriptor's index. 
    - If it is set, the value of the target descriptor will be read; 
    - if it is not set, the value of the target characteristic will be read.
- **\<len>**: data length
- **\<char_value>**: characteristic's value. HEX string is read by command `AT+BLEGATTCRD=<conn_index>,<srv_index>,<char_index>`. 
    - For example, if the response is "+BLEGATTCRD:1,30", it means that the value length is 1, and the content is "0x30".
- **\<desc_value>**: descriptor's value. HEX string is read by command `AT+BLEGATTCRD=<conn_index>,<srv_index>,<char_index>,<desc_index>`. 
    - For example, if the response is "+BLEGATTCRD:4,30313233", it means that the value length is 4, and the content is "0x30 0x31 0x32 0x33".

***Note:***

* The BLE connection has to be established first.
* If the target characteristic cannot be read, it will return "ERROR".

Example：

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCCHAR=0,3 // set a specific index according to the result of the previous command
    // for example, to read 1st descriptor of the 2nd characteristic in the 3rd service, use the following command:
    AT+BLEGATTCRD=0,3,2,1

<a name="cmd-GCWR"></a>
### [ESP32 Only] [AT+BLEGATTCWR](#BLE-AT)—GATTC Writes Characteristic
Set Command: 

    AT+BLEGATTCWR=<conn_index>,<srv_index>,<char_index>[,<desc_index>],<length>
    Function: GATTC to write characteristics or descriptor.
Response：

    >
Begin receiving serial data. When the requirement of data length, determined by <length>, is met, the writting starts.
If the setting is successful, the system returns:
    OK  
Parameters：

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<srv_index>**: service's index; it can be fetched with command `AT+BLEGATTCPRIMSRV=<conn_index>`
- **\<char_index>**: characteristic's index; it can be fetched with command `AT+BLEGATTCCHAR=<conn_index>,<srv_index>`
- **\[\<desc_index>]**(Optional parameter): descriptor's index. 
    - If it is set, the value of the target descriptor will be written; 
    - If it is not set, the value of the target characteristic will be written.
- **\<length>**: data length

***Note:***

* The BLE connection has to be established first.
* If the target characteristic cannot be written, it will return "ERROR".

Example：

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCCHAR=0,3 // set a specific index according to the result of the previous command
    // for example, to write 6 bytes of data to the 4th characteristic in the 3rd service, use the following command:
    AT+BLEGATTCWR=0,3,4,,6 
    // after > shows, inputs 6 bytes of data, such as "123456"; then, the writing starts

<a name="cmd-BLESPP"></a>
### [ESP32 Only] [AT+BLESPPCFG](#BLE-AT)—Sets BLE spp parameters
Query Command:

    AT+BLESPPCFG?
    Function: to get the parameters of BLE spp.
Response:

    +BLESPPCFG:<tx_service_index>,<tx_char_index>,<rx_service_index>,<rx_char_index>
    OK
Set Command:

    AT+BLESCANPARAM=<option>[,<tx_service_index>,<tx_char_index>,<rx_service_index>,<rx_char_index>]
    Function: to set or reset the parameters of BLE spp.
Response:

    OK
Parameters:

- **\<option>**: if the option is 0, it means all the spp parametersthe will be reset, and the next four parameters don't need input. if the option is 1, the user must input all the parameters.
- **\<tx_service_index>**: tx service's index; it can be fetched with command `AT+BLEGATTCPRIMSRV=<conn_index>` and `AT+BLEGATTSSRVCRE?`
- **\<tx_char_index>**: tx characteristic's index; it can be fetched with command `AT+BLEGATTCCHAR=<conn_index>,<srv_index>` and `AT+BLEGATTSCHAR?`
- **\<rx_service_index>**: rx service's index; it can be fetched with command `AT+BLEGATTCPRIMSRV=<conn_index>` and `AT+BLEGATTSSRVCRE?`
- **\<rx_char_index>**: rx characteristic's index; it can be fetched with command `AT+BLEGATTCCHAR=<conn_index>,<srv_index>` and `AT+BLEGATTSCHAR?`

***Note:***

* In BLE client, the property of tx characteristic must be write with response or write without response, the property of rx characteristic must be indicate or notify.
* In BLE server, the property of tx characteristic must be indicate or notify, the property of rx characteristic must be write with response or write without response.

Example:

    AT+BLESPPCFG=0          // reset ble spp parameters
    AT+BLESPPCFG=1,3,5,3,7  // set ble spp parameters
    AT+BLESPPCFG?           // query ble spp parameters 

<a name="cmd-BLESPP"></a>
### [ESP32 Only] [AT+BLESPP](#BLE-AT)—Enter BLE spp mode
Execute Command: 

    AT+BLESPP
    Function: Enter BLE spp mode.

Response:

    >   

***Note:***

* If the ble spp parameters is illegal, this command will return ERROR.

Example:

    AT+BLESPP   // enter ble spp mode

<a name="cmd-BLESMPPAR"></a>
### [ESP32 Only] [AT+BLESECPARAM](#BLE-AT)—Set BLE encryption parameters
Query Command:

    AT+BLESECPARAM?
    Function: to get the parameters of BLE smp.
Response:

    +BLESECPARAM:<auth_req>,<iocap>,<key_size>,<init_key>,<rsp_key>,<auth_option>
    OK
Set Command:

    AT+BLESECPARAM=<auth_req>,<iocap>,<key_size>,<init_key>,<rsp_key>[,<auth_option>]
    Function: to set the parameters of BLE smp.
Response:

    OK
Parameters:

- **\<auth_req>**: 
    - 0 : NO_BOND
    - 1 : BOND
    - 4 : MITM
    - 8 : SC_ONLY
    - 9 : SC_BOND
    - 12 : SC_MITM
    - 13 : SC_MITM_BOND
- **\<iocap>**:
    - 0 : DisplayOnly 
    - 1 : DisplayYesNo
    - 2 : KeyboardOnly
    - 3 : NoInputNoOutput
    - 4 : Keyboard displa
- **\<key_size>**: the key size should be 7~16 bytes.
- **\<init_key>**: combination of the bit pattern.
- **\<rsp_key>**: combination of the bit pattern.
- **\<auth_option>**: auth option of security.
    - 0 : Select the security level automaticly.
    - 1 : If cannot follow the preset security level, the connection will disconnect.

***Note:***

* The bit pattern for init_key&rsp_key is:
    - (1<<0) Used to exchange the encrytyption key in the init key & response key
    - (1<<1) Used to exchange the IRK key in the init key & response key
    - (1<<2) Used to exchange the CSRK key in the init key & response key
    - (1<<3) Used to exchange the link key(this key just used in the BLE & BR/EDR coexist mode) in the init key & response key

Example:

    AT+BLESECPARAM=1,4,16,3,3,0

<a name="cmd-BLEENC"></a>
### [ESP32 Only] [AT+BLEENC](#BLE-AT)—Initiate BLE encryption request
Set Command:

    AT+BLEENC=<conn_index>,<sec_act>
    Function: to start a pairing request
Response:

    OK
Parameters:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<sec_act>**:
    - 0 : SEC_NONE
    - 1 : SEC_ENCRYPT
    - 2 : SEC_ENCRYPT_NO_MITM
    - 3 : SEC_ENCRYPT_MITM

***Note:***

* Before ipput this command, user must set the security paramsters and connection with remote device.

Example:

    AT+BLESECPARAM=1,4,16,3,3
    AT+BLEENC=0,3

<a name="cmd-BLEENCRSP"></a>
### [ESP32 Only] [AT+BLEENCRSP](#BLE-AT)—Grant security request access
Set Command:

    AT+BLEENCRSP=<conn_index>,<accept>
    Function: to set a pairing response.
Response:

    OK
Parameters:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<accept>**:
    - 0 : reject
    - 1 : accept; 

Example:

    AT+BLEENCRSP=0,1

<a name="cmd-BLEKEYREPLY"></a>
### [ESP32 Only] [AT+BLEKEYREPLY](#BLE-AT)—Reply the key value to the peer device in the lagecy connection stage
Set Command:

    AT+BLEKEYREPLY=<conn_index>,<key>
    Function: to reply a pairing key.
Response:

    OK
Parameters:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<key>**:    pairing key

Example:

    AT+BLEKEYREPLY=0,649784

<a name="cmd-BLECONFREPLY"></a>
### [ESP32 Only] [AT+BLECONFREPLY](#BLE-AT)—Reply the comfirm value to the peer device in the lagecy connection stage
Set Command:

    AT+BLECONFREPLY=<conn_index>,<confirm>
    Function: to reply to a pairing result.
Response:

    OK
Parameters:

- **\<conn_index>**: index of BLE connection, range [0~2].
- **\<confirm>**:
    - 0 : NO
    - 1 : Yes

Example:

    AT+BLECONFREPLY=0,1

<a name="cmd-BLEENCDEV"></a>
### [ESP32 Only] [AT+BLEENCDEV](#BLE-AT)—Query BLE encryption device list
Query Command:

    AT+BLEENCDEV?
    Function: to get the bounded devices.
Response:

    +BLEENCDEV:<enc_dev_index>,<mac_address>
    OK
Parameters:

- **\<enc_dev_index>**: index of the bonded devices.
- **\<mac_address>**:   Mac address.

Example:

    AT+BLEENCDEV?

<a name="cmd-BLEENCCLEAR"></a>
### [ESP32 Only] [AT+BLEENCCLEAR](#BLE-AT)—Clear BLE encryption device list
Set Command:

    AT+BLEENCCLEAR=<enc_dev_index>
    Function: remove a device from the security database list with a specific index.
Response:

    OK
Execute Command:

    AT+BLEENCCLEAR
    Function: remove all devices from the security database.
Response:

    OK
Parameters:

- **\<enc_dev_index>**: index of the bonded devices.

Example:

    AT+BLEENCCLEAR

<a name="cmd-BLESETKEY"></a>
### [ESP32 Only][AT+BLESETKEY](#BLE-AT)—Set BLE static pair key
Query Command:

    AT+BLESETKEY?
    Function: to query the ble static pair key, If it's not set, it will returns -1.
Response:

    +BLESETKEY:<static_key>
    OK
Set Command:

    AT+BLESETKEY=<static_key>
    Function: to set a BLE static pair key for all BLE connections.
Response:

    OK
Parameters:

- **\<static_key>**: static BLE pair key.

Example:

    AT+BLESETKEY=123456

<a name="cmd-BLEHIDINIT"></a>
### [ESP32 Only][AT+BLEHIDINIT](#BLE-AT)—BLE HID device profile initialization
Query Command:

    AT+BLEHIDINIT?
    Function: to check the initialization status of BLE HID profile.
Response:

If BLE HID device profile is not initialized, it will return:

    +BLEHIDINIT:0
    OK
If BLE HID device profile is initialized, it will return:

    +BLEHIDINIT:1
    OK
Set Command: 

    AT+BLEHIDINIT=<init>
    Function: to initialize the BLE HID device profile.
Response:

    OK
Parameter:

- **\<init>**: 
    - 0: deinit ble hid device profile
    - 1: init ble hid device profile

***Notes:***

* The BLE HID command cannot be used at the same time with general GATT/GAP commands.

Example:

    AT+BLEHIDINIT=1 

<a name="cmd-BLEHIDKB"></a>
### [ESP32 Only][AT+BLEHIDKB](#BLE-AT)—Send BLE HID Keyboard information
Set Command: 

    AT+BLEHIDKB=<Modifier_keys>,<key_1>,<key_2>,<key_3>,<key_4>,<key_5>,<key_6>
    Function: to send keyboard information.
Response:

    OK
Parameter:

- **\<Modifier_keys>**: Modifier keys mask
- **\<key_1>**: key code 1
- **\<key_2>**: key code 2
- **\<key_3>**: key code 3
- **\<key_4>**: key code 4
- **\<key_5>**: key code 5
- **\<key_6>**: key code 6

Example:

    AT+BLEHIDKB=0,4,0,0,0,0,0   // input a

<a name="cmd-BLEHIDMUS"></a>
### [ESP32 Only][AT+BLEHIDMUS](#BLE-AT)—Send BLE HID mouse information
Set Command: 

    AT+BLEHIDMUS=<buttons>,<X_displacement>,<Y_displacement>,<wheel>
    Function: to send mouse information.
Response:

    OK
Parameter:

- **\<buttons>**: mouse button
- **\<X_displacement>**: X displacement
- **\<Y_displacement>**: Y displacement
- **\<wheel>**: Wheel

Example:

    AT+BLEHIDMUS=0,10,10,0

<a name="cmd-BLEHIDC"></a>
### [ESP32 Only][AT+BLEHIDCONSUMER](#BLE-AT)—Send BLE HID consumer information
Set Command: 

    AT+BLEHIDCONSUMER=<consumer_usage_id>
    Function: to send consumer information.
Response:

    OK
Parameter:

- **\<consumer_usage_id>**: consumer id, such as power, reset, help, volume and so on.

Example:

    AT+BLEHIDCONSUMER=233   // volume up

<a name="cmd-BLUFI"></a>
### [ESP32 Only] [AT+BLUFI](#BLE-AT)—Start or Stop BLUFI
Query Command:

    AT+BLUFI?
    Function: to check the status of BLUFI.
Response:

If BLUFI is not started, it will return

    +BLUFI:0
    OK
If BLUFI is started, it will return

    +BLUFI:1
    OK
Set Command: 

    AT+BLUFI=<option>
    Function: start or stop blufi.
Response:

    OK
Parameter:

- **\<option>**: 
    - 0: stop blufi
    - 1: start blufi

Example:

    AT+BLUFI=1

[ESP32 Only] [AT+BLUFINAME](#cmd-BLUFINAME) : Set BLUFI device name
<a name="cmd-BLUFINAME"></a>
### [ESP32 Only] [AT+BLUFINAME](#BLE-AT)—Set BLUFI device name
Query Command:

    AT+BLUFINAME?
    Function: to query the BLUFI name.
Response:

    +BLUFINAME:<device_name>
    OK
Set Command: 

    AT+BLUFINAME=<device_name>
    Function: set the BLUFI device name.
Response:

    OK
Parameter:

- **\<device_name>**: the name of blufi device

***Notes:***

* If you need to set BLUFI name, please set it before command `AT+BLUFI=1`, Otherwise, it will use the default name `BLUFI_DEVICE`.
* The max length of BLUFI name is 29 bytes.

Example:

    AT+BLUFINAME="BLUFI_DEV"
    AT+BLUFINAME?
