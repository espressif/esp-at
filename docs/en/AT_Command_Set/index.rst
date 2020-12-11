**************
AT Command Set
**************

Here is a list of AT commands. Some of the AT commands can only work on the ESP32, which is marked as [ESP32 Only]; others can work on both the ESP8266 and ESP32.

.. toctree::
   :maxdepth: 1
   
   Basic AT Commands <Basic_AT_Commands>
   Wi-Fi AT Commands <Wi-Fi_AT_Commands>
   TCP-IP AT Commands <TCP-IP_AT_Commands>
   [ESP32 Only] BLE AT Commands <BLE_AT_Commands>
   [ESP32 Only] BT AT Commands <BT_AT_Commands>
   MQTT AT Commands <MQTT_AT_Commands>
   HTTP AT Commands <HTTP_AT_Commands>
   [ESP32 Only] Ethernet AT Commands <Ethernet_AT_Commands>
   Signaling Test AT Commands <Signaling_Test_AT_Commands>
   [ESP32 & ESP32S2 Only] Driver AT Commands <Driver_AT_Commands>
   AT Command Set Comparison <AT_Command_Set_Comparison>

Before checking the command set details, please review some common information on command types, configurations that can be saved in the flash, as well as messages returned after entering commands.   

- `AT Command Types`_
- `AT Commands with Configuration Saved in the Flash`_
- `AT Messages`_


.. _at-command-types:

AT Command Types
================

Generic AT command has four types:

.. list-table::
   :header-rows: 1
   :widths: 25 20 55

   * - Type 
     - Command Format 
     - Description
   * - Test Command 
     - AT+=? 
     - Queries the Set Commands' internal parameters and their range of values.
   * - Query Command 
     - AT+? 
     - Returns the current value of parameters.
   * - Set Command 
     - AT+=<…> 
     - Sets the value of user-defined parameters in commands, and runs these commands.
   * - Execute Command 
     - AT+ 
     - Runs commands with no user-defined parameters.

-  Not all AT commands support all four types mentioned above.
-  Square brackets [ ] designate parameters that may be omitted; default value of the parameter will be used instead. 

   Below are examples of entering command `AT+CWJAP <#cmd-JAP>`__ with some parameters omitted::

      AT+CWJAP="ssid","password"
      AT+CWJAP="ssid","password","11:22:33:44:55:66"

-  If the parameter which is not the last one is omitted, you can give a ``,`` to indicate it.

   Example::
   
      AT+CWJAP="ssid","password",,1

-  String values need to be included in double quotation marks, for example: ``AT+CWSAP="ESP756290","21030826",1,4``.
-  Escape character syntax is needed if a string contains any special characters, such as ``,``, ``"`` or ``\``:

   -  ``\\``: escape backslash itself
   -  ``\,``: escape comma which is used to separate each parameter
   -  ``\"``: escape double quotation marks which used to mark string input
   -  ``\<any>``: escape ``<any>`` character means that drop backslash symbol and only use ``<any>`` character

      Example::
      
         AT+CWJAP="comma\,backslash\\ssid","1234567890"
         AT+MQTTPUB=0,"topic","\"{\"sensor\":012}\"",1,0

-  The default baud rate of AT command is 115200.
-  AT commands are ended with a new-line (CR-LF), so the serial tool should be set into "New Line Mode".
-  Definitions of AT command error codes are provided in :doc:`../Compile_and_Develop/AT_API_Reference`:

   - :cpp:type:`esp_at_error_code`
   - :cpp:type:`esp_at_para_parse_result_type`
   - :cpp:type:`esp_at_result_code_string_index`


AT Commands with Configuration Saved in the Flash
=================================================

Configuration settings entered by the following AT Commands will always be saved in the flash NVS Area, so they can be automatically restored on reset:

-  `AT+UART\_DEF <#cmd-UARTD>`__: for example, ``AT+UART_DEF=115200,8,1,0,3``
-  `AT+SAVETRANSLINK <#cmd-SAVET>`__ : for example, ``AT+SAVETRANSLINK=1,"192.168.6.10",1001``
-  `AT+CWAUTOCONN <#cmd-AUTOC>`__: for example, ``AT+CWAUTOCONN=1``

Saving of configuration settings by several other commands can be switched on or off with `AT+SYSSTORE <#cmd-SYSSTORE>`__ command. Please see description of `AT+SYSSTORE <#cmd-SYSSTORE>`__ for details.


AT Messages
===========

There are two types of ESP-AT messages from ESP-AT command port. One is ESP-AT command response Messages passively, the other is ESP-AT message report actively.

ESP-AT Response Messages
  Any ESP-AT command input will return the response by mandatory, it tells the sender the result of the ESP-AT command.

ESP-AT Message Report
  ESP-AT will report the system important state change or message.

.. list-table:: ESP-AT Response Messages
    :header-rows: 1
    :widths: 40 60

    * - AT Response Messages
      - Description
    * - OK
      - AT command process done and return OK
    * - ERROR
      - AT command error or error occurred during execution
    * - SEND OK
      - Data has been sent to the protocol stack (Unique to AT+CIPSEND and AT+CIPSENDEX command)
    * - SEND FAIL
      - Error occurred during sending the data to the protocol stack (Unique to AT+CIPSEND and AT+CIPSENDEX command)
    * - +<Command Name>:``...``
      - Response to the sender that AT command process result in detail

.. list-table:: ESP-AT Message Report
   :header-rows: 1
   :widths: 40 60

   * - ESP-AT Message Report
     - Description
   * - ready
     - The ESP-AT firmware is ready
   * - busy p...
     - Busy processing. The system is in process of handling the previous command, CAN NOT accept the newly input
   * - ERR CODE:``<0x%08x>``
     - Error code for different command
   * - Will force to restart!!!
     - Module restart right now
   * - smartconfig type:``<xxx>``
     - Smartconfig type
   * - Smart get wifi info
     - Smartconfig has got the SSID and PASSWORD information
   * - smartconfig connected wifi
     - Smartconfig done, ESP-AT has connected to the WiFi
   * - WIFI CONNECTED
     - WiFi station interface has connected to an AP
   * - WIFI GOT IP
     - WiFi station interface has got the IPv4 address
   * - WIFI DISCONNECT
     - WiFi station interface has disconnected from an AP
   * - +ETH_CONNECTED
     - Ethernet station interface has connected
   * - +ETH_GOT_IP
     - Ethernet station interface has got the IPv4 address
   * - +ETH_DISCONNECTED
     - Ethernet station interface has disconnected
   * - [<conn_id>,]CONNECT
     - A network connection of which ID is ``<conn_id>`` is established (ID=0 by default)
   * - [<conn_id>,]CLOSED
     - A network connection of which ID is ``<conn_id>`` ends (ID=0 by default)
   * - +LINK_CONN
     - Detailed connection information of TCP/UDP/SSL
   * - +STA_CONNECTED: <sta_mac>
     - A station has connected to the WiFi softAP interface of ESP-AT
   * - +DIST_STA_IP: <sta_mac>,<sta_ip>
     - The WiFi softAP interface of ESP-AT distributes an IP address to the station
   * - +STA_DISCONNECTED: <sta_mac> 
     - A station disconnected from the WiFi softAP interface of ESP-AT
   * - >
     - ESP-AT is waiting for more data to be received
   * - Recv ``<xxx>`` bytes
     - ESP-AT has already received ``<xxx>`` bytes from the ESP-AT command port
   * - +IPD
     - ESP-AT received the data from the network
   * - SEND Canceled
     - Cancel to send in transparent transmission
   * - Have ``<xxx>`` Connections
     - Has reached the maximum connection counts for server
   * - +QUITT
     - ESP-AT quits from the transparent transmission mode
   * - NO CERT FOUND
     - No a valid device certificate found in custom partition
   * - NO PRVT_KEY FOUND
     - No a valid private key found in custom partition
   * - NO CA FOUND
     - No a valid CA certificate found in custom partition
   * - +MQTTCONNECTED
     - MQTT connected to the broker
   * - +MQTTDISCONNECTED
     - MQTTT disconnected from the broker
   * - +MQTTSUBRECV
     - MQTT received the data from the broker
   * - +MQTTPUB:FAIL
     - MQTT failed to publish data
   * - +MQTTPUB:OK
     - MQTT publish data done
   * - +BLECONN
     - A BLE connection established
   * - +BLEDISCONN 
     - A BLE connection ends
   * - +READ
     - A read operation from BLE connection
   * - +WRITE
     - A write operation from BLE connection
   * - +NOTIFY
     - A notification from BLE connection
   * - +INDICATE
     - An indication from BLE connection
   * - +BLESECNTFYKEY
     - BLE SMP key
   * - +BLEAUTHCMPL 
     - BLE SMP pairing completed
