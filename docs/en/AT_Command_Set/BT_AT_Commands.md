<a name="BT-AT"></a>
## [ESP32 Only] BT AT Commands
[Download BlueTooth Spec (ESP32 supports Core Version 4.2)](https://www.bluetooth.com/specifications/adopted-specifications)  

* [ESP32 Only] [AT+BTINIT](#cmd-BTINIT) : Classic Bluetooth initialization
* [ESP32 Only] [AT+BTNAME](#cmd-BTNAME) : Sets BT device's name
* [ESP32 Only] [AT+BTSCANMODE](#cmd-BTSCANMODE) : Sets BT SCAN mode
* [ESP32 Only] [AT+BTSTARTDISC](#cmd-BTDISC) : Start BT discovery
* [ESP32 Only] [AT+BTSPPINIT](#cmd-BTSPPINIT) : Classic Bluetooth SPP profile initialization
* [ESP32 Only] [AT+BTSPPCONN](#cmd-BTSPPCONN) : Establishes SPP connection
* [ESP32 Only] [AT+BTSPPDISCONN](#cmd-BTSPPDISCONN) : Ends SPP connection
* [ESP32 Only] [AT+BTSPPSTART](#cmd-BTSPPSTART) : Start Classic Bluetooth SPP profile
* [ESP32 Only] [AT+BTSPPSEND](#cmd-BTSPPSEND) : Sends data to remote bt spp device
* [ESP32 Only] [AT+BTA2DPINIT](#cmd-BTA2DPINIT) : Classic Bluetooth A2DP profile initialization
* [ESP32 Only] [AT+BTA2DPCONN](#cmd-BTA2DPCONN) : Establishes A2DP connection
* [ESP32 Only] [AT+BTA2DPDISCONN](#cmd-BTA2DPDISCONN) : Ends A2DP connection
* [ESP32 Only] [AT+BTA2DPSRC](#cmd-BTA2DPSRC) : Set or query the audio file URL
* [ESP32 Only] [AT+BTA2DPCTRL](#cmd-BTA2DPCTRL) : control the audio play
* [ESP32 Only] [AT+BTSECPARAM](#cmd-BTSECPARAM) :Set and query the Classic Bluetooth security parameters
* [ESP32 Only] [AT+BTKEYREPLY](#cmd-BTKEYREPLY) :Input the Simple Pair Key
* [ESP32 Only] [AT+BTPINREPLY](#cmd-BTPINREPLY) :Input the Legacy Pair PIN Code
* [ESP32 Only] [AT+BTSECCFM](#cmd-BTSECCFM): Reply the confirm value to the peer device in the legacy connection stage
* [ESP32 Only] [AT+BTENCDEV](#cmd-BTENCDEV) : Query BT encryption device list
* [ESP32 Only] [AT+BTENCCLEAR](#cmd-BTENCCLEAR) : Clear BT encryption device list
* [ESP32 Only] [AT+BTCOD](#cmd-BTCOD) : Set class of device


<a name="cmd-BTINIT"></a>
### [ESP32 Only] [AT+BTINIT](#BT-AT)—Classic Bluetooth initialization
Query Command:

    AT+BTINIT?
    Function: to check the initialization status of classic bluetooth.
Response:

If classic bluetooth is not initialized, it will return:

    +BTINIT:0
    OK
If classic bluetooth is initialized, it will return:

    +BTINIT:1
    OK
Set Command: 

    AT+BTINIT=<init>
    Function: to init or deinit classic bluetooth.
Response:

    OK
Parameter:

- **\<init>**: 
    - 0: deinit classic bluetooth
    - 1: init classic bluetooth

Example:

    AT+BTINIT=1    

<a name="cmd-BTNAME"></a>
### [ESP32 Only] [AT+BTNAME](#BT-AT)—Sets BT device's name
Query Command:

    AT+BTNAME?
    Function: to get the classic bluetooth device name.
Response:

    +BTNAME:<device_name>
    OK
Set Command:

    AT+BTNAME=<device_name>
    Function: to set the classic bluetooth device name, The maximum length is 32.
Response:

    OK
Parameter:

- **\<device_name>**: the classic bluetooth device name

***Notes:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.
* The default classic bluetooth device name is "ESP32_AT".  

Example:

    AT+BTNAME="esp_demo"    

<a name="cmd-BTSCANMODE"></a>
### [ESP32 Only] [AT+BTSCANMODE](#BT-AT)—Sets BT SCAN mode
Set Command:

    AT+BTSCANMODE=<scan_mode>
    Function: to set the scan mode of classic bluetooth.
Response:

    OK
Parameters:

- **\<scan_mode>**:
    - 0: Neither discoverable nor connectable
    - 1: Connectable but not discoverable
    - 2: both discoverable and connectable
    - 3: discoverable but not Connectable

Example:

    AT+BTSCANMODE=2   // both discoverable and connectable

<a name="cmd-BTDISC"></a>
### [ESP32 Only] [AT+BTSTARTDISC](#BT-AT)—Start BT discovery
Set Command:

    AT+BTSTARTDISC=<inq_mode>,<inq_len>,<inq_num_rsps>
    Function: to set the scan mode of classic bluetooth.
Response:

    +BTSTARTDISC:<bt_addr>,<dev_name>,<major_dev_class>,<minor_dev_class>,<major_srv_class>,<rssi>

    OK
Parameters:

- **\<inq_mode>**:
    - 0: General inquiry mode
    - 1: Limited inquiry mode
- **\<inq_len>**: inquiry duration, ranging from 0x01 to 0x30
- **\<inq_num_rsps>**: number of inquiry responses that can be received, value 0 indicates an unlimited number of responses
- **\<bt_addr>**: bluetooth address
- **\<dev_name>**: device name
- **\<major_dev_class>**: 
    -  0x0: Miscellaneous
    -  0x1: Computer
    -  0x2: Phone(cellular, cordless, pay phone, modem)
    -  0x3: LAN, Network Access Point
    -  0x4: Miscellaneous
    -  0x5: Peripheral(mouse, joystick, keyboard)
    -  0x6: Imaging(printer, scanner, camera, display)
    -  0x7: Wearable
    -  0x8: Toy
    -  0x9: Health
    - 0x1F: Uncategorized: device not specified
- **\<minor_dev_class>**
    - please refer to this [web](https://www.bluetooth.com/specifications/assigned-numbers/baseband)
- **\<major_srv_class>**: 
    - 0x0:   None indicates an invalid value 
    - 0x1:   Limited Discoverable Mode
    - 0x8:   Positioning (Location identification)
    - 0x10:  Networking, e.g. LAN, Ad hoc
    - 0x20:  Rendering, e.g. Printing, Speakers
    - 0x40:  Capturing, e.g. Scanner, Microphone
    - 0x80:  Object Transfer, e.g. v-Inbox, v-Folder
    - 0x100: Audio, e.g. Speaker, Microphone, Headerset service 
    - 0x200: Telephony, e.g. Cordless telephony, Modem, Headset service
    - 0x400: Information, e.g., WEB-server, WAP-server
- **\<rssi>**: signal strength

Example:

    AT+BTINIT=1
    AT+BTSCANMODE=2
    AT+BTSTARTDISC=0,10,10

<a name="cmd-BTSPPINIT"></a>
### [ESP32 Only] [AT+BTSPPINIT](#BT-AT)—Classic Bluetooth SPP profile initialization
Query Command:

    AT+BTSPPINIT?
    Function: to check the initialization status of classic bluetooth SPP profile.
Response:

If classic bluetooth SPP profile is not initialized, it will return:

    +BTSPPINIT:0
    OK
If classic bluetooth SPP profile is initialized, it will return:

    +BTSPPINIT:1
    OK
Set Command: 

    AT+BTSPPINIT=<init>
    Function: to init or deinit classic bluetooth SPP profile.
Response:

    OK
Parameter:

- **\<init>**: 
    - 0: deinit classic bluetooth SPP profile
    - 1: init classic bluetooth SPP profile, the role is master
    - 2: init classic bluetooth SPP profile, the role is slave

Example:

    AT+BTSPPINIT=1    //master    
    AT+BTSPPINIT=2    //slave

<a name="cmd-BTSPPCONN"></a>
### [ESP32 Only] [AT+BTSPPCONN](#BT-AT)—Establishes SPP connection
Query Command: 

    AT+BTSPPCONN?
    Function: to query classic bluetooth SPP connection.
Response:

    +BTSPPCONN:<conn_index>,<remote_address>
    OK
If the connection has not been established, there will be return `+BTSPPCONN:-1`.
Set Command: 

    AT+BTSPPCONN=<conn_index>,<sec_mode>,<remote_address>
    Function: to establish the classic bluetooth SPP connection.
Response:

    OK
It will prompt the following message, if the connection is established successfully:

    +BTSPPCONN:<conn_index>,<remote_address>
It will prompt the following message, if NOT:

    +BTSPPCONN:<conn_index>,-1
Parameters:

- **\<conn_index>**: index of classic bluetooth spp connection; only 0 is supported for the single connection right now.
- **\<sec_mode>**：
    - 0x0000 : No security
    - 0x0001 : Authorization required (only needed for out going connection ) 
    - 0x0012 : Authentication required.
    - 0x0024 : Encryption required. 
    - 0x0040 : Mode 4 level 4 service, i.e. incoming/outgoing MITM and P-256 encryption
    - 0x3000 : Man-In-The-Middle protection 
    - 0x4000 : Min 16 digit for pin code 
- **\<remote_address>**：remote classic bluetooth spp device address

Example:

    AT+BTSPPCONN=0,0,"24:0a:c4:09:34:23"

<a name="cmd-BTSPPDISCONN"></a>
### [ESP32 Only] [AT+BTSPPDISCONN](#BT-AT)—Ends SPP connection
Execute Command: 

    AT+BTSPPDISCONN=<conn_index>
    Function: to end the classic bluetooth SPP connection.
Response:

    OK
If the command is successful, it will prompt:

    +BTSPPDISCONN:<conn_index>,<remote_address>
Parameter:

- **\<conn_index>**: index of classic bluetooth SPP connection; only 0 is supported for the single connection right now.
- **\<remote_address>**：remote classic bluetooth A2DP device address.

Example:

    AT+BTSPPDISCONN=0

<a name="cmd-BTSPPSEND"></a>
### [ESP32 Only] [AT+BTSPPSEND](#BT-AT)—Sends data to remote classic bluetooth spp device
Execute Command: 

    AT+BTSPPSEND
    Function: Enter BT SPP mode.

Response:

    >   

Execute Command: 

    AT+BTSPPSEND=<conn_index>,<data_len>
    Function: send data to the remote classic bluetooth SPP device.
Response:

    OK
Parameter:

- **\<conn_index>**: index of classic bluetooth SPP connection; only 0 is supported for the single connection right now.
- **\<data_len>**: the length of the data which was ready to send.

***Notes:***

* The wrap return is > after this command is executed. Then, ESP32 enters UART-BT passthrough mode. When a single packet containing +++ is received, ESP32 returns to normal command mode. Please wait for at least one second before sending the next AT command.

Example:

    AT+BTSPPSEND=0,100
    AT+BTSPPSEND

<a name="cmd-BTSPPSTART"></a>
### [ESP32 Only] [AT+BTSPPSTART](#BT-AT)—Start the classic bluetooth SPP profile.
Execute Command: 

    AT+BTSPPSTART
    Function: start the classic bluetooth SPP profile.
Response:

    OK

Example:

    AT+BTSPPSTART

<a name="cmd-BTA2DPINIT"></a>
### [ESP32 Only] [AT+BTA2DPINIT](#BT-AT)—Classic Bluetooth A2DP profile initialization
Query Command:

    AT+BTA2DPINIT?
    Function: to check the initialization status of classic bluetooth A2DP profile.
Response:

If classic bluetooth A2DP profile is not initialized, it will return

    +BTA2DPINIT:0
    OK
If classic bluetooth A2DP profile is initialized, it will return

    +BTA2DPINIT:1
    OK
Set Command: 

    AT+BTA2DPINIT=<role>,<init_val>
    Function: to init or deinit classic bluetooth A2DP profile.
Response:

    OK
Parameter:

- **\<role>**: 
    - 0: source
    - 1: sink
- **\<init_val>**: 
    - 0: deinit classic bluetooth A2DP profile
    - 1: init classic bluetooth A2DP profile

Example:

    AT+BTA2DPINIT=0,1

<a name="cmd-BTA2DPCONN"></a>
### [ESP32 Only] [AT+BTA2DPCONN](#BT-AT)—Establishes A2DP connection
Query Command: 

    AT+BTA2DPCONN?
    Function: to query classic bluetooth A2DP connection.
Response:

    +BTA2DPCONN:<conn_index>,<remote_address>
    OK
If the connection has not been established, there will NOT be <conn_index> and <remote_address>
Set Command: 

    AT+BTA2DPCONN=<conn_index>,<remote_address>
    Function: to establish the classic bluetooth A2DP connectionn.
Response:

    OK
It will prompt the message below, if the connection is established successfully:

    +BTA2DPCONN:<conn_index>,<remote_address>
It will prompt the message below, if NOT:

    +BTA2DPCONN:<conn_index>,fail
Parameters:

- **\<conn_index>**: index of classic bluetooth A2DP connection; only 0 is supported for the single connection right now.
- **\<remote_address>**：remote classic bluetooth A2DP device address.

Example:

    AT+BTA2DPCONN=0,0,0,"24:0a:c4:09:34:23"

<a name="cmd-BTA2DPDISCONN"></a>
### [ESP32 Only] [AT+BTA2DPDISCONN](#BT-AT)—Ends A2DP connection
Execute Command: 

    AT+BTA2DPDISCONN=<conn_index>
    Function: to end the classic bluetooth A2DP connection.
Response:

    OK
If the command is successful, it will prompt 
    +BTA2DPDISCONN:<conn_index>,<remote_address>
Parameter:

- **\<conn_index>**: index of classic bluetooth A2DP connection; only 0 is supported for the single connection right now.
- **\<remote_address>**：remote classic bluetooth A2DP device address.

Example:

    AT+BTA2DPDISCONN=0

<a name="cmd-BTA2DPSRC"></a>
### [ESP32 Only] [AT+BTA2DPSRC](#BT-AT)—Set or query the audio file URL
Execute Command: 

    AT+BTA2DPSRC=<conn_index>,<url>
    Function: Set the audio file URL.
Response:

    OK

Query Command:

    AT+BTA2DPSRC?
    Function: to query the audio file URL.
Response:

    +BTA2DPSRC:<url>,<type>
    OK

Parameter:

- **\<conn_index>**: index of classic bluetooth A2DP connection; only 0 is supported for the single connection right now.
- **\<url>**: the path of the source file. HTTP HTTPS and FLASH are currently supported.
- **\<type>**: the type of audio file, such as "mp3".

***Note:***  

* Only mp3 format is currently supported.

Example:

    AT+BTA2DPSRC="https://dl.espressif.com/dl/audio/ff-16b-2c-44100hz.mp3"
    AT+BTA2DPSRC="flash://spiffs/zhifubao.mp3"

<a name="cmd-BTA2DPCTRL"></a>
### [ESP32 Only] [AT+BTA2DPCTRL](#BT-AT)—control the audio play
Execute Command: 

    AT+BTA2DPCTRL=<conn_index>,<ctrl>
    Function: control the audio play
Response:

    OK

Parameter:

- **\<conn_index>**: index of classic bluetooth A2DP connection; only 0 is supported for the single connection right now.
- **\<ctrl>**: types of control.
    - 0 : A2DP Sink, stop play
    - 1 : A2DP Sink, start play
    - 2 : A2DP Sink, forward
    - 3 : A2DP Sink, backward
    - 4 : A2DP Sink, fastward start
    - 5 : A2DP Sink, fastward stop
    - 0 : A2DP Source, stop play
    - 1 : A2DP Source, start play
    - 2 : A2DP Source, suspend

Example:

    AT+BTA2DPCTRL=0,1  // start play audio

<a name="cmd-BTSECPARAM"></a>
### [ESP32 Only] [AT+BTSECPARAM](#BT-AT)—Set and query the Classic Bluetooth security parameters
Query Command: 

    AT+BTSECPARAM?
    Function: to query classic bluetooth security parameters.
Response:

    +BTSECPARAM:<io_cap>,<pin_type>,<pin_code>
    OK
Set Command: 

    AT+BTSECPARAM=<io_cap>,<pin_type>,<pin_code>
    Function: set the Classic Bluetooth security parameters.
Response:

    OK
Parameters:

- **\<io_cap>**: io capability.
    - 0 : DisplayOnly
    - 1 : DisplayYesNo
    - 2 : KeyboardOnly
    - 3 : NoInputNoOutput
- **\<pin_type>**：Use variable or fixed PIN.
    - 0 : variable
    - 1 : fixed
- **\<pin_code>**: Legacy Pair PIN Code (upto 16 bytes).

***Notes:***

* If pin_type is variable, pin_code will be ignored,

Example:

    AT+BTSECPARAM=3,1,"9527"

<a name="cmd-BTKEYREPLY"></a>
### [ESP32 Only] [AT+BTKEYREPLY](#BT-AT)—Input Simple Pair Key
Execute Command: 

    AT+BTKEYREPLY=<conn_index>,<Key>
    Function: Input the Simple Pair Key.
Response:

    OK
Parameter:

- **\<conn_index>**: index of classic bluetooth connection; Currently only 0 is supported for the single connection.
- **\<Key>**: the Simple Pair Key.

Example:

    AT+BTKEYREPLY=0,123456

<a name="cmd-BTPINREPLY"></a>
### [ESP32 Only] [AT+BTPINREPLY](#BT-AT)—Input the Legacy Pair PIN Code
Execute Command: 

    AT+BTPINREPLY=<conn_index>,<Pin>
    Function: Input the Legacy Pair PIN Code.
Response:

    OK
Parameter:

- **\<conn_index>**: index of classic bluetooth connection; Currently only 0 is supported for the single connection.
- **\<Pin>**: the Legacy Pair PIN Code.

Example:

    AT+BTPINREPLY=0,"6688"

<a name="cmd-BTSECCFM"></a>
### [ESP32 Only] [AT+BTSECCFM](#BT-AT)—Reply the confirm value to the peer device in the legacy connection stage
Execute Command: 

    AT+BTSECCFM=<conn_index>,<accept>
    Function: Reply the confirm value to the peer device in the legacy connection stage.
Response:

    OK
Parameter:

- **\<conn_index>**: index of classic bluetooth connection; Currently only 0 is supported for the single connection.
- **\<accept>**: reject or accept.
    - 0 : reject
    - 1 : accept

Example:

    AT+BTSECCFM=0,1

<a name="cmd-BTENCDEV"></a>
### [ESP32 Only] [AT+BTENCDEV](#BT-AT)—Query BT encryption device list
Query Command:

    AT+BTENCDEV?
    Function: to get the bonded devices.
Response:

    +BTENCDEV:<enc_dev_index>,<mac_address>
    OK
Parameters:

- **\<enc_dev_index>**: index of the bonded devices.
- **\<mac_address>**:   Mac address.

Example:

    AT+BTENCDEV?

<a name="cmd-BTENCCLEAR"></a>
### [ESP32 Only] [AT+BTENCCLEAR](#BT-AT)—Clear BT encryption device list
Set Command:

    AT+BTENCCLEAR=<enc_dev_index>
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

    AT+BTENCCLEAR


<a name="cmd-BTCOD"></a>
### [ESP32 Only] [AT+BTCOD](#BT-AT)—Set class of device
Set Command:

    AT+BTCOD=<major>,<minor>,<service>
    Function: set the BT class of device.
Response:

    OK

Parameters:

- **\<major>**: [major class](https://www.bluetooth.com/specifications/assigned-numbers/baseband/).
- **\<minor>**: [minor class](https://www.bluetooth.com/specifications/assigned-numbers/baseband/).
- **\<service>**: [service class](https://www.bluetooth.com/specifications/assigned-numbers/baseband/).

Example:

    AT+BTCOD=6,32,32   //the printer
