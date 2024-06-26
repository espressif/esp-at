**************
AT Command Set
**************

{IDF_TARGET_VER: default="undefined", esp32="5.0", esp32c2="5.0", esp32c3="5.0", esp32c6="5.1", esp32s2="5.0"}

:link_to_translation:`zh_CN:[中文]`

Here is a list of AT commands.

.. toctree::
   :maxdepth: 1

   Basic AT Commands <Basic_AT_Commands>
   Wi-Fi AT Commands <Wi-Fi_AT_Commands>
   TCP-IP AT Commands <TCP-IP_AT_Commands>
   :esp32 or esp32c2 or esp32c3 or esp32c6: Bluetooth® Low Energy AT Commands <BLE_AT_Commands>
   :esp32: Bluetooth® AT Commands <BT_AT_Commands>
   MQTT AT Commands <MQTT_AT_Commands>
   HTTP AT Commands <HTTP_AT_Commands>
   FileSystem AT Commands <FS_AT_Commands>
   WebSocket AT Commands <websocket_at_commands>
   :esp32: Ethernet AT Commands <Ethernet_AT_Commands>
   Signaling Test AT Commands <Signaling_Test_AT_Commands>
   Web server AT Commands <Web_server_AT_Commands>
   Driver AT Commands <Driver_AT_Commands>
   User AT Commands <user_at_commands>

It is strongly recommended to read the following sections for some common information on AT commands before you dive into the details of each command.   

.. contents::
   :local:
   :depth: 1

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
     - AT+<CommandName>=?
     - Query the Set Commands' internal parameters and their range of values.
   * - Query Command 
     - AT+<CommandName>?
     - Return the current value of parameters.
   * - Set Command 
     - AT+<CommandName>=<...> 
     - Set the value of user-defined parameters in commands, and run these commands.
   * - Execute Command 
     - AT+<CommandName>
     - Run commands with no user-defined parameters.

-  Not all AT commands support all of the four types mentioned above.
-  Currently, only strings and integer numbers are supported as input parameters in AT commands.
-  Angle brackets < > designate parameters that can not be omitted.
-  Square brackets [ ] designate optional parameters that can be omitted. The default value of the parameter will be used instead when you omit it. Below are examples of entering the command :ref:`AT+CWJAP <cmd-JAP>` with some parameters omitted.
   ::

      AT+CWJAP="ssid","password"
      AT+CWJAP="ssid","password","11:22:33:44:55:66"

-  If the parameter you want to omit is followed by a parameter(s), you must give a ``,`` to indicate it.
   ::
   
      AT+CWJAP="ssid","password",,1

-  String values need to be included in double quotation marks.
   ::

      AT+CWSAP="ESP756290","21030826",1,4

-  Escape character syntax is needed if a string contains special characters. The characters that need to be escaped are ``,``, ``"``, and ``\``:

   -  ``\\``: escape the backslash itself
   -  ``\,``: escape comma which is not used to separate each parameter
   -  ``\"``: escape double quotation mark which is not used to mark string input
   -  ``\<any>``: escape ``<any>`` character means that drop backslash symbol and only use ``<any>`` character

-  Escape is needed in AT commands only, not elsewhere. For example, when AT command port prints ``>`` and wait for your input, the input does not need to be escaped.
   ::
      
      AT+CWJAP="comma\,backslash\\ssid","1234567890"
      AT+MQTTPUB=0,"topic","\"{\"sensor\":012}\"",1,0

-  The default baud rate of AT command is 115200.
-  The length of each AT command should be no more than 256 bytes.
-  AT commands end with a new-line (CR-LF), so the serial tool should be set to "New Line Mode".
-  Definitions of AT command error codes are provided in :doc:`../Compile_and_Develop/AT_API_Reference`:

   - :cpp:type:`esp_at_error_code`
   - :cpp:type:`esp_at_para_parse_result_type`
   - :cpp:type:`esp_at_result_code_string_index`

AT Commands with Configuration Saved in the Flash
=================================================

Configuration settings entered by the following AT Commands will always be saved in the flash NVS Area, so they can be automatically restored on reset:

-  :ref:`AT+UART_DEF <cmd-UARTD>`: ``AT+UART_DEF=115200,8,1,0,3``
-  :ref:`AT+SAVETRANSLINK <cmd-SAVET>`: ``AT+SAVETRANSLINK=1,"192.168.6.10",1001``
-  :ref:`AT+CWAUTOCONN <cmd-AUTOC>`: ``AT+CWAUTOCONN=1``

Saving of configuration settings by several other commands can be switched on or off with :ref:`AT+SYSSTORE <cmd-SYSSTORE>` command, which is mentioned in the Note section of these commands.

.. note::
  The parameters of AT commands are saved based on `NVS <https://docs.espressif.com/projects/esp-idf/en/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-reference/storage/nvs_flash.html>`_ library. Therefore, if the command is configured with the same parameter value, flash will not be written; If the command is configured with the different parameter value, flash will not be erased frequently.

.. _at-messages:

AT Messages
===========

There are two types of ESP-AT messages returned from the ESP-AT command port:

.. _at-messages-response:

- ESP-AT Response Messages (passive)
  
  Each ESP-AT command input returns response messages to tell the sender the result of the ESP-AT command. The last message in the response is either ``OK`` or ``ERROR``.
    
  .. list-table:: ESP-AT Response Messages
      :header-rows: 1
      :widths: 40 60
  
      * - AT Response Messages
        - Description
      * - OK
        - AT command process done and return OK
      * - ERROR
        - AT command error or error occurred during the execution
      * - SEND OK
        - Data has been sent to the protocol stack (specific to :ref:`AT+CIPSEND <cmd-SEND>` and :ref:`AT+CIPSENDEX <cmd-SENDEX>` command). It doesn't mean that the data has been sent to the opposite end
      * - SEND FAIL
        - Error occurred during sending the data to the protocol stack (specific to :ref:`AT+CIPSEND <cmd-SEND>` and :ref:`AT+CIPSENDEX <cmd-SENDEX>` command
      * - SET OK
        - The URL has been set successfully (specific to :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` command)
      * - +<Command Name>:``...``
        - Response to the sender that describes AT command process results in details   

.. _at-messages-report:

- ESP-AT Message Reports (active)

  ESP-AT will report important state changes or messages in the system.

  .. list-table:: ESP-AT Message Reports
     :header-rows: 1
     :widths: 60 60
  
     * - ESP-AT Message Report
       - Description
     * - ready
       - The ESP-AT firmware is ready
     * - busy p...
       - Busy processing. The system is in process of handling the previous command, thus CANNOT accept the new input
     * - ERR CODE:``<0x%08x>``
       - Error code for different commands
     * - Will force to restart!!!
       - Module restart right now
     * - smartconfig type:``<xxx>``
       - Smartconfig type
     * - Smart get wifi info
       - Smartconfig has got the SSID and PASSWORD information
     * - +SCRD:``<length>``,``<reserved data>``
       - ESP-Touch v2 has got the reserved information
     * - smartconfig connected wifi
       - Smartconfig done. ESP-AT has connected to the Wi-Fi
     * - WIFI CONNECTED
       - Wi-Fi station interface has connected to an AP
     * - WIFI GOT IP
       - Wi-Fi station interface has got the IPv4 address
     * - WIFI GOT IPv6 LL
       - Wi-Fi station interface has got the IPv6 LinkLocal address
     * - WIFI GOT IPv6 GL
       - Wi-Fi station interface has got the IPv6 Global address
     * - WIFI DISCONNECT
       - Wi-Fi station interface has disconnected from an AP
     * - +ETH_CONNECTED
       - Ethernet interface has connected
     * - +ETH_GOT_IP
       - Ethernet interface has got the IPv4 address
     * - +ETH_DISCONNECTED
       - Ethernet interface has disconnected
     * - [<conn_id>,]CONNECT
       - A network connection of which ID is ``<conn_id>`` is established (ID=0 by default)
     * - [<conn_id>,]CLOSED
       - A network connection of which ID is ``<conn_id>`` ends (ID=0 by default)
     * - +LINK_CONN
       - Detailed connection information of TCP/UDP/SSL
     * - +STA_CONNECTED: <sta_mac>
       - A station has connected to the Wi-Fi softAP interface of ESP-AT
     * - +DIST_STA_IP: <sta_mac>,<sta_ip>
       - The Wi-Fi softAP interface of ESP-AT distributes an IP address to the station
     * - +STA_DISCONNECTED: <sta_mac> 
       - A station disconnected from the Wi-Fi softAP interface of ESP-AT
     * - >
       - ESP-AT is waiting for more data to be received
     * - Recv ``<xxx>`` bytes
       - ESP-AT has already received ``<xxx>`` bytes from the ESP-AT command port
     * - +IPD
       - ESP-AT received the data from the network when AT is not in :term:`Passthrough Mode`. The message formats are as follows:

         - ``+IPD,<length>`` will be outputted if AT+CIPMUX=0 and AT+CIPRECVTYPE=1.
         - ``+IPD,<link_id>,<length>`` will be outputted if AT+CIPMUX=1 and AT+CIPRECVTYPE=<link_id>,1.
         - ``+IPD,<length>:<data>`` will be outputted if AT+CIPMUX=0, AT+CIPRECVTYPE=0, and AT+CIPDINFO=0.
         - ``+IPD,<link_id>,<length>:<data>`` will be outputted if AT+CIPMUX=1, AT+CIPRECVTYPE=<link_id>,0, and AT+CIPDINFO=0.
         - ``+IPD,<length>,<"remote_ip">,<remote_port>:<data>`` will be outputted if AT+CIPMUX=0, AT+CIPRECVTYPE=0, and AT+CIPDINFO=1.
         - ``+IPD,<link_id>,<length>,<"remote_ip">,<remote_port>:<data>`` will be outputted if AT+CIPMUX=1, AT+CIPRECVTYPE=<link_id>,0, and AT+CIPDINFO=1.

         The ``link_id`` represents the connection ID, ``length`` indicates the data length, ``remote_ip`` refers to the remote IP address, ``remote_port`` represents the remote port number, and ``data`` denotes the actual data.

         Note: In the case of a SSL connection, in passive receive mode (AT+CIPRECVTYPE=1), the ``length`` in the AT command response may not match the actual readable SSL data length. This is because AT prioritizes returning the readable data length at the SSL layer. If the SSL layer has no readable data, AT will return the readable data length at the socket layer.

     * - The Data in :term:`Passthrough Mode`
       - ESP-AT received the data from the network or Bluetooth when AT is in :term:`Passthrough Mode`
     * - SEND Canceled
       - Cancel to send in :ref:`Wi-Fi normal sending mode <cmd-SEND>`
     * - Have ``<xxx>`` Connections
       - Has reached the maximum connection counts for server
     * - +QUITT
       - ESP-AT quits from the Wi-Fi :term:`Passthrough Mode`
     * - NO CERT FOUND
       - No valid device certificate found in custom partition
     * - NO PRVT_KEY FOUND
       - No valid private key found in custom partition
     * - NO CA FOUND
       - No valid CA certificate found in custom partition
     * - +TIME_UPDATED
       - The system time has been updated. Only after sending the :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>` command or power on will this message be outputted if the module fetches a new time from the SNTP server.
     * - +MQTTCONNECTED
       - MQTT connected to the broker
     * - +MQTTDISCONNECTED
       - MQTT disconnected from the broker
     * - +MQTTSUBRECV
       - MQTT received the data from the broker
     * - +MQTTPUB:FAIL
       - MQTT failed to publish data
     * - +MQTTPUB:OK
       - MQTT publish data done
     * - +BLECONN
       - A Bluetooth LE connection established
     * - +BLEDISCONN 
       - A Bluetooth LE connection ends
     * - +READ
       - A read operation from Bluetooth LE connection
     * - +WRITE
       - A write operation from Bluetooth LE connection
     * - +NOTIFY
       - A notification from Bluetooth LE connection
     * - +INDICATE
       - An indication from Bluetooth LE connection
     * - +BLESECNTFYKEY
       - Bluetooth LE SMP key
     * - +BLESECREQ:<conn_index>
       - Received encryption request which index is <conn_index>
     * - +BLEAUTHCMPL:<conn_index>,<enc_result>
       - Bluetooth LE SMP pairing completed
     * - +BLUFIDATA:<len>,<data>
       - The ESP device received customized data from the phone over BluFi
     * - +WS_DISCONNECTED:<link_id>
       - The WebSocket connection of ID <link_id> is disconnected.
     * - +WS_CONNECTED:<link_id>
       - The WebSocket connection of ID ``<link_id>`` is established.
     * - +WS_DATA:<link_id>,<data_len>,<data>
       - The Websocket connection of ID ``<link_id>`` has received the data.
     * - +WS_CLOSED:<link_id>
       - The WebSocket connection of ID ``<link_id>`` is closed.
     * - +BLESCANDONE
       - Scan finished
     * - +BLESECKEYREQ:<conn_index>
       - The peer has accepted the pairing request, and the ESP device can enter the key.

  .. only:: esp32c3

    If third-party cloud commands are used, ESP-AT will report cloud important state changes or messages in the system.

    .. list-table:: ESP-AT Message Report for Third-Party Cloud
      :header-rows: 1
      :widths: 60 60

      * - ESP-AT Message Report
        - Description
      * - RainMaker AT Messages
        - Please refer to :ref:`ESP-AT RainMaker Message Reports (active) <rm-at-messages>`
