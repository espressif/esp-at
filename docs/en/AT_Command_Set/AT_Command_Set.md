ESP-AT Commands Set
=============

Here is a list of AT commands. Some of the AT commands can only work on the ESP32, which will be marked as [ESP32 Only]; others can work on both the ESP8266 and ESP32.  

**ESP-AT Commands List**:  

* [Basic AT Commands](Basic_AT_Commands.md)
* [Wi-Fi AT Commands](Wi-Fi_AT_Commands.md)
* [TCP/IP AT Commands](TCP-IP_AT_Commands.md)
* [[ESP32 Only] BLE AT Commands](BLE_AT_Commands.md)
* [[ESP32 Only] BT AT Commands](BT_AT_Commands.md)
* [[ESP32 Only] Ethernet AT Commands](Ethernet_AT_Commands.md)
* [MQTT AT Commands](MQTT_AT_Commands.md)
* [HTTP AT Commands](HTTP_AT_Commands.md)
* [Signaling Test AT Commands](Signaling_Test_AT_Commands.md)


**Appendix**:  

 * [AT Command Description](#Appendix-ATDescrpt)
 * [AT Commands with Configuration Saved in the NVS Area](#Appendix-ATSaveNVS)
 * [AT Messages](#Appendix-ATmsg)
 
 


<a name="Appendix-ATDescrpt"></a>
## Appendix 1. [AT Command Description](#Begin-AT)
Each command can support four types of AT commands.

|Type | Command Format | Description|  
|---|---|---|  
|Test Command	 | AT+<x>=? | Queries the Set Commands' internal parameters and their range of values.|
|Query Command | AT+<x>? | Returns the current value of parameters.|
|Set Command | AT+<x>=<…> | Sets the value of user-defined parameters in commands, and runs these commands.|
|Execute Command | AT+<x> | Runs commands with no user-defined parameters.|

 * Not all AT commands support all four variations mentioned above.
 * Square brackets [ ] designate the default value; it is either not required or may not appear.
 * String values need to be included in double quotation marks, for example: `AT+CWSAP="ESP756290","21030826",1,4`.
 * Escape sensitive delimiters is necessary on string parameters in AT command.
   * `\\`: escape backslash itself
   * `\,`: escape comma which used to separate each parameter
   * `\"`: escape double quotation marks which used to mark string input
   * `\<any>`: escape `<any>` character means that drop backslash symbol and only use `<any>` character
   > for example:  
   > `AT+CWJAP="comma\,backslash\\ssid","1234567890"`  
   > `AT+MQTTPUB=0,"topic","\"{\"sensor\":012}\"",1,0`
 * The default baud rate of AT command is 115200.
 * AT commands are ended with a new-line (CR-LF), so the serial tool should be set into "New Line Mode".
 * Definitions of AT command error codes are in `esp-at/components/at/include/esp_at.h`.
 
<a name="Appendix-ATSave"></a>
## Appendix 2. [AT Commands with Configuration Saved in the Flash](#Begin-AT)
The following AT Commands with configuration will be saved in the flash NVS Area.

 *  [AT+SYSSTORE](#cmd-SYSSTORE): some AT configurations will be saved into flash after command `AT+SYSSTORE=1`.
 *  [AT+UART_DEF](#cmd-UARTD): for example, `AT+UART_DEF=115200,8,1,0,3`
 *  [AT+SAVETRANSLINK](#cmd-SAVET) : for example, `AT+SAVETRANSLINK=1,"192.168.6.10",1001`
 *  [AT+CWAUTOCONN](#cmd-AUTOC): for example, `AT+CWAUTOCONN=1`

<a name="Appendix-ATmsg"></a>
## Appendix 3. [AT Messages](#Begin-AT)

|Messages | Description|
|---|---|
|ready	| The AT firmware is ready.|
|ERROR | AT command error, or error occurred during execution.|
|WIFI CONNECTED | ESP station connected to an AP.|
|WIFI GOT IP	| ESP station got IP address.|
|WIFI DISCONNECT	| ESP station disconnected from an AP.|
|busy p... | Busy processing. The system is in process of handling the previous command, cannot accept the newly input.|
|\<conn_id>,CONNECT | A network connection of which ID is <conn_id> is established.|
|\<conn_id>,CLOSED | A network connection of which ID is <conn_id> ends.|
|+IPD | Network data received.|
|+STA\_CONNECTED: \<sta_mac> | A station connects to the ESP softAP.|
|+DIST\_STA\_IP: \<sta\_mac>,\<sta_ip> | ESP softAP distributes an IP address to the station connected.|
|+STA\_DISCONNECTED: \<sta_mac> | A station disconnects from the ESP softAP.|
|+BLECONN	| A BLE connection established.|
|+BLEDISCONN | A BLE connection ends.|
|+READ | A read operation from BLE connection.|
|+WRITE | A write operation from BLE connection.|
|+NOTIFY | A notification from BLE connection.|
|+INDICATE | An indication from BLE connection.|
|+BLESECNTFYKEY | BLE SMP key|
|+BLEAUTHCMPL	 | BLE SMP pairing completed.|




