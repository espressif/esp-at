<a name="ETH-AT"></a>
## [ESP32 Only] Ethernet AT Commands 
* [ESP32 Only] [AT+CIPETHMAC](#cmd-ETHMAC) : Sets the MAC address of ESP32 Ethernet.
* [ESP32 Only] [AT+CIPETH](#cmd-ETHIP) : Sets the IP address of ESP32 Ethernet.
 
<a name="cmd-ETHMAC"></a>
### [ESP32 Only] [AT+CIPETHMAC](#ETH-AT)—Sets the MAC Address of the ESP32 Ethernet
Query Command:

    AT+CIPETHMAC?
    Function: to obtain the MAC address of the ESP32 Ethernet.
Response:

    +CIPETHMAC:<mac>
    OK
Set Command: 

    AT+CIPETHMAC =<mac>
    Function: to set the MAC address of the ESP32 Ethernet.
Response:

    OK
Parameters:

- **\<mac>**: string parameter, MAC address of the ESP8266 Ethernet.    

***Notes:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.
* The MAC address of ESP32 SoftAP is different from that of the ESP32 Station. Please make sure that you do not set the same MAC address for both of them.
* Bit 0 of the ESP32 MAC address CANNOT be 1. 
    * For example, a MAC address can be "1a:…" but not "15:…".
* `FF:FF:FF:FF:FF:FF` and `00:00:00:00:00:00` are invalid MAC and cannot be set.

Example:

    AT+CIPETHMAC ="1a:fe:35:98:d4:7b"

<a name="cmd-ETHIP"></a>
### [ESP32 Only] [AT+CIPETH](#ETH-AT)—Sets the IP Address of the ESP32 Ethernet
Query Command:

    AT+CIPETH?
    Function: to obtain the IP address of the ESP32 Ethernet.
    Notice: Only after calling esp_at_eth_cmd_regist can its IP address be queried.
Response:

    +CIPETH:ip:<ip>
    +CIPETH:gateway:<gateway>
    +CIPETH:netmask:<netmask>
    OK
Set Command:

    AT+CIPETH=<ip>[,<gateway>,<netmask>]
    Function: to set the IP address of the ESP32 Ethernet.
Response:

    OK
Parameters:

- **\<ip>**: string parameter, the IP address of the ESP32 Ethernet.
- **\[\<gateway>]**: gateway.
- **\[\<netmask>]**: netmask.

***Notes:***

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.
* The set command interacts with DHCP-related AT commands (AT+CWDHCP-related commands): 
    * If static IP is enabled, DHCP will be disabled;
    * If DHCP is enabled, static IP will be disabled;
    * Whether it is DHCP or static IP that is enabled depends on the last configuration.

Example:

    AT+CIPETH="192.168.6.100","192.168.6.1","255.255.255.0"