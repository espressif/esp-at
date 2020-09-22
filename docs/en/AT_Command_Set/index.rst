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

.. list-table::
   :header-rows: 1
   :widths: 40 60

   * - Messages 
     - Description
   * - ready 
     - The AT firmware is ready.
   * - ERROR 
     - AT command error, or error occurred during execution.
   * - WIFI CONNECTED 
     - ESP station connected to an AP.
   * - WIFI GOT IP 
     - ESP station got IP address.
   * - WIFI DISCONNECT 
     - ESP station disconnected from an AP.
   * - busy p... 
     - Busy processing. The system is in process of handling the previous command, cannot accept the newly input.
   * - <conn_id>,CONNECT 
     - A network connection of which ID is <conn_id> is established.
   * - <conn_id>,CLOSED 
     - A network connection of which ID is <conn_id> ends.
   * - +IPD 
     - Network data received.
   * - +STA_CONNECTED: <sta_mac> 
     - A station connects to the ESP softAP.
   * - +DIST_STA_IP: <sta_mac>,<sta_ip> 
     - ESP softAP distributes an IP address to the station connected.
   * - +STA_DISCONNECTED: <sta_mac> 
     - A station disconnects from the ESP softAP.
   * - +BLECONN 
     - A BLE connection established.
   * - +BLEDISCONN 
     - A BLE connection ends.
   * - +READ 
     - A read operation from BLE connection.
   * - +WRITE 
     - A write operation from BLE connection.
   * - +NOTIFY 
     - A notification from BLE connection.
   * - +INDICATE 
     - An indication from BLE connection.
   * - +BLESECNTFYKEY 
     - BLE SMP key
   * - +BLEAUTHCMPL 
     - BLE SMP pairing completed.
