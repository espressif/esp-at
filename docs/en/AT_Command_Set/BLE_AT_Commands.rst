.. _BLE-AT:

[ESP32 Only] Bluetooth® Low Energy AT Commands
=====================================================

:link_to_translation:`zh_CN:[中文]`

ESP32 AT firmware supports `Bluetooth® Core Specification Version 5.0 <https://www.bluetooth.com/specifications/archived-specifications/>`_.

-  [ESP32 Only] :ref:`AT+BLEINIT <cmd-BINIT>`: Bluetooth LE initialization.
-  [ESP32 Only] :ref:`AT+BLEADDR <cmd-BADDR>`: Query/Set Bluetooth LE device address.
-  [ESP32 Only] :ref:`AT+BLENAME <cmd-BNAME>`: Query/Set Bluetooth LE device name.
-  [ESP32 Only] :ref:`AT+BLESCANPARAM <cmd-BSCANP>`: Query/Set parameters of Bluetooth LE scanning.
-  [ESP32 Only] :ref:`AT+BLESCAN <cmd-BSCAN>`: Enable Bluetooth LE scanning.
-  [ESP32 Only] :ref:`AT+BLESCANRSPDATA <cmd-BSCANR>`: Set Bluetooth LE scan response.
-  [ESP32 Only] :ref:`AT+BLEADVPARAM <cmd-BADVP>`: Query/Set parameters of Bluetooth LE advertising.
-  [ESP32 Only] :ref:`AT+BLEADVDATA <cmd-BADVD>`: Set Bluetooth LE advertising data.
-  [ESP32 Only] :ref:`AT+BLEADVDATAEX <cmd-BADVDEX>`: Automatically set Bluetooth LE advertising data.
-  [ESP32 Only] :ref:`AT+BLEADVSTART <cmd-BADVSTART>`: Start Bluetooth LE advertising.
-  [ESP32 Only] :ref:`AT+BLEADVSTOP <cmd-BADVSTOP>`: Stop Bluetooth LE advertising.
-  [ESP32 Only] :ref:`AT+BLECONN <cmd-BCONN>`: Establish Bluetooth LE connection.
-  [ESP32 Only] :ref:`AT+BLECONNPARAM <cmd-BCONNP>`: Query/Update parameters of Bluetooth LE connection.
-  [ESP32 Only] :ref:`AT+BLEDISCONN <cmd-BDISC>`: End Bluetooth LE connection.
-  [ESP32 Only] :ref:`AT+BLEDATALEN <cmd-BDLEN>`: Set Bluetooth LE data packet length.
-  [ESP32 Only] :ref:`AT+BLECFGMTU <cmd-BMTU>`: Set Bluetooth LE MTU length.
-  [ESP32 Only] :ref:`AT+BLEGATTSSRVCRE <cmd-GSSRVCRE>`: Generic Attributes Server (GATTS) creates services.
-  [ESP32 Only] :ref:`AT+BLEGATTSSRVSTART <cmd-GSSRVSTART>`: GATTS starts services.
-  [ESP32 Only] :ref:`AT+BLEGATTSSRVSTOP <cmd-GSSRVSTOP>`: GATTS Stops Services.
-  [ESP32 Only] :ref:`AT+BLEGATTSSRV <cmd-GSSRV>`: GATTS discovers services.
-  [ESP32 Only] :ref:`AT+BLEGATTSCHAR <cmd-GSCHAR>`: GATTS discovers characteristics.
-  [ESP32 Only] :ref:`AT+BLEGATTSNTFY <cmd-GSNTFY>`: Notify a client of the value of a characteristic value from the server.
-  [ESP32 Only] :ref:`AT+BLEGATTSIND <cmd-GSIND>`: Indicate the characteristic value from the server to a client.
-  [ESP32 Only] :ref:`AT+BLEGATTSSETATTR <cmd-GSSETA>`: GATTS sets characteristics.
-  [ESP32 Only] :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`: Generic Attributes Client (GATTC) discovers primary services.
-  [ESP32 Only] :ref:`AT+BLEGATTCINCLSRV <cmd-GCINCLSRV>`: GATTC discovers included services.
-  [ESP32 Only] :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`: GATTC discovers characteristics.
-  [ESP32 Only] :ref:`AT+BLEGATTCRD <cmd-GCRD>`: GATTC reads characteristics.
-  [ESP32 Only] :ref:`AT+BLEGATTCWR <cmd-GCWR>`: GATTC writes characteristics.
-  [ESP32 Only] :ref:`AT+BLESPPCFG <cmd-BLESPPCFG>`: Query/Set Bluetooth LE SPP parameters.
-  [ESP32 Only] :ref:`AT+BLESPP <cmd-BLESPP>`: Enter Bluetooth LE SPP mode.
-  [ESP32 Only] :ref:`AT+BLESECPARAM <cmd-BLESMPPAR>`: Query/Set Bluetooth LE encryption parameters.
-  [ESP32 Only] :ref:`AT+BLEENC <cmd-BLEENC>`: Initiate Bluetooth LE encryption request.
-  [ESP32 Only] :ref:`AT+BLEENCRSP <cmd-BLEENCRSP>`: Respond to the pairing request from the peer device.
-  [ESP32 Only] :ref:`AT+BLEKEYREPLY <cmd-BLEKEYREPLY>`: Reply the key value to the peer device in the legacy connection stage.
-  [ESP32 Only] :ref:`AT+BLECONFREPLY <cmd-BLECONFREPLY>`: Reply the confirm value to the peer device in the legacy connection stage.
-  [ESP32 Only] :ref:`AT+BLEENCDEV <cmd-BLEENCDEV>`: Query bonded Bluetooth LE encryption device list.
-  [ESP32 Only] :ref:`AT+BLEENCCLEAR <cmd-BLEENCCLEAR>`: Clear Bluetooth LE encryption device list.
-  [ESP32 Only] :ref:`AT+BLESETKEY <cmd-BLESETKEY>`: Set Bluetooth LE static pair key.
-  [ESP32 Only] :ref:`AT+BLEHIDINIT <cmd-BLEHIDINIT>`: Bluetooth LE Human Interface Device (HID) profile initialization.
-  [ESP32 Only] :ref:`AT+BLEHIDKB <cmd-BLEHIDKB>`: Send Bluetooth LE HID keyboard information.
-  [ESP32 Only] :ref:`AT+BLEHIDMUS <cmd-BLEHIDMUS>`: Send Bluetooth LE HID mouse information.
-  [ESP32 Only] :ref:`AT+BLEHIDCONSUMER <cmd-BLEHIDC>`: Send Bluetooth LE HID consumer information.
-  [ESP32 Only] :ref:`AT+BLUFI <cmd-BLUFI>`: Start or Stop BluFi.
-  [ESP32 Only] :ref:`AT+BLUFINAME <cmd-BLUFINAME>`: Query/Set BluFi device name.

.. _cmd-BINIT:

[ESP32 Only] :ref:`AT+BLEINIT <BLE-AT>`: Bluetooth LE Initialization
----------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Check the initialization status of Bluetooth LE.

**Command:**

::

    AT+BLEINIT?

**Response:**

If Bluetooth LE is initialized, AT will return:

::

    +BLEINIT:<role>
    OK

If Bluetooth LE is not initialized, AT will return:

::

    +BLEINIT:0
    OK

Set Command
^^^^^^^^^^^

**Function:**

Initialize the role of Bluetooth LE.

**Command:**

::

    AT+BLEINIT=<init>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<init>**:

   -  0: deinit Bluetooth LE
   -  1: client role
   -  2: server role

Notes
^^^^^

-  The file "at_customize.bin" has to be downloaded, so that the relevant commands can be used. Please refer to :doc:`../Compile_and_Develop/How_to_customize_BLE_services` for more details.
-  Before using other Bluetooth LE AT commands, you should run this command first to trigger the initialization process.
-  After the initialization, the Bluetooth LE role cannot be changed unless you run :ref:`AT+RST <cmd-RST>` to restart the system first and then re-initialize the Bluetooth LE role.
-  If you use an ESP device as a Bluetooth LE server, a service bin should be downloaded into flash.

   -  To learn how to generate a service bin, please refer to esp-at/tools/readme.md.
   -  The download address of the service bin is the "ble_data" address in esp-at/module_config/module_${platform}_default/at_customize.csv.

Example
^^^^^^^^

::

    AT+BLEINIT=1

.. _cmd-BADDR:

[ESP32 Only] :ref:`AT+BLEADDR <BLE-AT>`: Query/Set Bluetooth LE Device Address
-------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the Bluetooth LE Public Address.

**Command:**

::

    AT+BLEADDR?

**Response:**

::

    +BLEADDR:<BLE_public_addr>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the Bluetooth LE address type.

**Command:**

::

    AT+BLEADDR=<addr_type>[,<random_addr>]

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<addr_type>**:

   -  0: Public Address
   -  1: Random Address

Note
^^^^^

-  A Static Address should meet the following requirements:

   -  The two most significant bits of the address should be equal to 1.
   -  At least one bit of the random part of the address should be 0.
   -  At least one bit of the random part of the address should be 1.

Example
^^^^^^^^

::

    AT+BLEADDR=1,"f8:7f:24:87:1c:7b"    // Set Random Device Address, Static Address
    AT+BLEADDR=1                        // Set Random Device Address, Private Address
    AT+BLEADDR=0                        // Set Public Device Address

.. _cmd-BNAME:

[ESP32 Only] :ref:`AT+BLENAME <BLE-AT>`: Query/Set Bluetooth LE Device Name
----------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the Bluetooth LE device name.

**Command:**

::

    AT+BLENAME?

**Response:**

::

    +BLENAME:<device_name>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the Bluetooth LE device name.

**Command:**

::

    AT+BLENAME=<device_name>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<device_name>**: the Bluetooth LE device name. The maximum length is 32. Default: "BLE_AT".

Note
^^^^^

-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`. 

Example
^^^^^^^^

::

    AT+BLENAME="esp_demo"

.. _cmd-BSCANP:

[ESP32 Only] :ref:`AT+BLESCANPARAM <BLE-AT>`: Query/Set Parameters of Bluetooth LE Scanning
--------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the parameters of Bluetooth LE scanning.

**Command:**

::

    AT+BLESCANPARAM?

**Response:**

::

    +BLESCANPARAM:<scan_type>,<own_addr_type>,<filter_policy>,<scan_interval>,<scan_window>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the parameters of Bluetooth LE scanning.

**Command:**

::

    AT+BLESCANPARAM=<scan_type>,<own_addr_type>,<filter_policy>,<scan_interval>,<scan_window>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<scan_type>**:

   -  0: passive scan
   -  1: active scan

-  **<own_addr_type>**:

   -  0: Public Address
   -  1: Random Address
   -  2: RPA Public Address
   -  3: RPA Random Address

-  **<filter_policy>**:

   -  0: BLE_SCAN_FILTER_ALLOW_ALL
   -  1: BLE_SCAN_FILTER_ALLOW_ONLY_WLST
   -  2: BLE_SCAN_FILTER_ALLOW_UND_RPA_DIR
   -  3: BLE_SCAN_FILTER_ALLOW_WLIST_PRA_DIR

-  **<scan_interval>**: scan interval
-  **<scan_window>**: scan window

Note
^^^^^

-  The parameter ``<scan_window>`` CANNOT be larger than ``<scan_interval>``.

Example
^^^^^^^^

::

    AT+BLEINIT=1   // role: client
    AT+BLESCANPARAM=0,0,0,100,50

.. _cmd-BSCAN:

[ESP32 Only] :ref:`AT+BLESCAN <BLE-AT>`: Enable Bluetooth LE Scanning
----------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Enable/disable scanning.

**Command:**

::

    AT+BLESCAN=<enable>[,<interval>][,<filter_type>,<filter_param>]

**Response:**

::

    +BLESCAN:<addr>,<rssi>,<adv_data>,<scan_rsp_data>,<addr_type>
    OK

Parameters
^^^^^^^^^^

-  **<enable>**:

   -  1: enable continuous scanning.
   -  0: disable continuous scanning.

-  **[<interval>]**: optional parameter. Unit: second.

   -  If you want to disable the scanning, this parameter should be omitted.
   -  If you want to enable the scanning, set a value for this parameter:

     - When you set it to 0, it means that scanning is continuous.
     - When set it to a value other than 0, for example, ``AT+BLESCAN=1,3``, it means that scanning will last for 3 seconds and then stop automatically. The scanning results will be returned.

-  **[<filter_type>]**: filtering option.

   -  1: "MAC".
   -  2: "NAME".

-  **<filter_param>**: filtering parameter showing the remote device MAC address or remote device name.
-  **<addr>**: Bluetooth LE address.
-  **<rssi>**: signal strength.
-  **<adv_data>**: advertising data.
-  **<scan_rsp_data>**: scan response data.
-  **<addr_type>**: the address type of broadcasters.

Notes
^^^^^

-  The response ``OK`` does not necessarily come before the response ``+BLESCAN:<addr>,<rssi>,<adv_data>,<scan_rsp_data>,<addr_type>``. It may be output before ``+BLESCAN:<addr>,<rssi>,<adv_data>,<scan_rsp_data>,<addr_type>`` or after it.

Example
^^^^^^^^

::

    AT+BLEINIT=1    // role: client
    AT+BLESCAN=1    // start scanning
    AT+BLESCAN=0    // stop scanning
    AT+BLESCAN=1,3,1,"24:0A:C4:96:E6:88"  // start scanning, filter type is MAC address
    AT+BLESCAN=1,3,2,"ESP-AT"  // start scanning, filter type is device name

.. _cmd-BSCANR:

[ESP32 Only] :ref:`AT+BLESCANRSPDATA <BLE-AT>`: Set Bluetooth LE Scan Response
-------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set scan response.

**Command:**

::

    AT+BLESCANRSPDATA=<scan_rsp_data>

**Response:**

::

    OK  

Parameter
^^^^^^^^^^

-  **<scan_rsp_data>**: scan response data is a HEX string. For example, if you want to set the response data to "0x11 0x22 0x33 0x44 0x55", the command should be ``AT+BLESCANRSPDATA="1122334455"``.

Example
^^^^^^^^

::

    AT+BLEINIT=2   // role: server
    AT+BLESCANRSPDATA="1122334455"

.. _cmd-BADVP:

[ESP32 Only] :ref:`AT+BLEADVPARAM <BLE-AT>`: Query/Set Parameters of Bluetooth LE Advertising
----------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the parameters of advertising.

**Command:**

::

    AT+BLEADVPARAM?

**Response:**

::

    +BLEADVPARAM:<adv_int_min>,<adv_int_max>,<adv_type>,<own_addr_type>,<channel_map>,<adv_filter_policy>,<peer_addr_type>,<peer_addr>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the parameters of advertising.

**Command:**

::

    AT+BLEADVPARAM=<adv_int_min>,<adv_int_max>, <adv_type>,<own_addr_type>,<channel_map>[,<adv_filter_policy>][,<peer_addr_type>] [,<peer_addr>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<adv_int_min>**: minimum advertising interval. It should be less than the value of ``<adv_int_max>``. Range: 0x0020 ~ 0x4000. 
-  **<adv_int_max>**: maximum advertising interval. It should be more than the value of ``<adv_int_min>``. Range: 0x0020 ~ 0x4000. 
-  **<adv_type>**:

   -  0: ADV_TYPE_IND
   -  2: ADV_TYPE_SCAN_IND
   -  3: ADV_TYPE_NONCONN_IND

-  **<own_addr_type>**: own Bluetooth LE address type.

   -  0: BLE_ADDR_TYPE_PUBLIC
   -  1: BLE_ADDR_TYPE_RANDOM

-  **<channel_map>**: channel of advertising.

   -  1: ADV_CHNL_37
   -  2: ADV_CHNL_38
   -  4: ADV_CHNL_39
   -  7: ADV_CHNL_ALL

-  **[<adv_filter_policy>]**: filter policy of advertising.

   -  0: ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY
   -  1: ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY
   -  2: ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST
   -  3: ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST

-  **[<peer_addr_type>]**: remote Bluetooth LE address type.

   -  0: PUBLIC
   -  1: RANDOM

-  **[<peer_addr>]**: remote Bluetooth LE address.

Example
^^^^^^^^

::

    AT+BLEINIT=2   // role: server
    AT+BLEADVPARAM=50,50,0,0,4,0,0,"12:34:45:78:66:88"

.. _cmd-BADVD:

[ESP32 Only] :ref:`AT+BLEADVDATA <BLE-AT>`: Set Bluetooth LE Advertising Data
------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set advertising data.

**Command:**

::

    AT+BLEADVDATA=<adv_data>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<adv_data>**: advertising data in HEX string. For example, to set the advertising data to "0x11 0x22 0x33 0x44 0x55", the command should be ``AT+BLEADVDATA="1122334455"``.

Note
^^^^^

-  If advertising data is preset by command :ref:`AT+BLEADVDATAEX <cmd-BADVDEX>`\=<dev_name>,<uuid>,<manufacturer_data>,<include_power>, it will be overwritten by this command.

Example
^^^^^^^^

::

    AT+BLEINIT=2   // role: server
    AT+BLEADVDATA="1122334455"

.. _cmd-BADVDEX:

[ESP32 Only] :ref:`AT+BLEADVDATAEX <BLE-AT>`: Automatically Set Bluetooth LE Advertising Data
----------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the parameters of advertising data.

**Command:**

::

    AT+BLEADVDATAEX?

**Response:**

::

    +BLEADVDATAEX:<dev_name>,<uuid>,<manufacturer_data>,<include_power>

    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the advertising data and start advertising.

**Command:**

::

    AT+BLEADVDATAEX=<dev_name>,<uuid>,<manufacturer_data>,<include_power>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<dev_name>**: string parameter showing a device name. For example, if you want to set the device name to "just-test", the command should be ``AT+BLEADVSTARTEX="just-test",<uuid>,<manufacturer_data>,<include_power>``.

-  **<uuid>**: string parameter. For example, if you want to set the UUID to "0xA002", the command should be ``AT+BLEADVSTARTEX=<dev_name>,"A002",<manufacturer_data>,<include_power>``.

-  **<manufacturer_data>**: manufacturer data in HEX string. For example, if you set the manufacturer data to "0x11 0x22 0x33 0x44 0x55", the command should be ``AT+BLEADVSTARTEX=<dev_name>,<uuid>,"1122334455",<include_power>``.

-  **<include_power>**: If you need to include the TX power in the advertising data, you should set the parameter to ``1``. Otherwise, set it to ``0``.

Note
^^^^^

-  If advertising data is preset by command :ref:`AT+BLEADVDATA <cmd-BADVD>`\=<adv_data>, it will be overwritten by this command.

Example
^^^^^^^^

::

    AT+BLEINIT=2   // role: server
    AT+BLEADVDATAEX="ESP-AT","A002","0102030405",1

.. _cmd-BADVSTART:

[ESP32 Only] :ref:`AT+BLEADVSTART <BLE-AT>`: Start Bluetooth LE Advertising
----------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Start advertising.

**Command:**

::

    AT+BLEADVSTART

**Response:**

::

    OK

Notes
^^^^^

-  If advertising parameters are NOT set by command :ref:`AT+BLEADVPARAM <cmd-BADVP>`\=<adv_parameter>, the default parameters will be used.
-  If advertising data is NOT set by command :ref:`AT+BLEADVDATA <cmd-BADVD>`\=<adv_data>, the advertising playload will be empty.
-  If advertising data is preset by command :ref:`AT+BLEADVDATA <cmd-BADVD>`\=<adv_data>, it will be overwritten by :ref:`AT+BLEADVDATAEX <cmd-BADVDEX>`\=<dev_name>,<uuid>,<manufacturer_data>,<include_power> and vice versa.

Example
^^^^^^^^

::

    AT+BLEINIT=2   // role: server
    AT+BLEADVSTART

.. _cmd-BADVSTOP:

[ESP32 Only] :ref:`AT+BLEADVSTOP <BLE-AT>`: Stop Bluetooth LE Advertising
--------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Stop advertising.

**Command:**

::

    AT+BLEADVSTOP

**Response:**

::

    OK

Note
^^^^^

-  After the start of advertising, if the Bluetooth LE connection is established successfully, it will stop advertising automatically. In such a case, this command does NOT need to be called.

Example
^^^^^^^^

::

    AT+BLEINIT=2   // role: server
    AT+BLEADVSTART
    AT+BLEADVSTOP

.. _cmd-BCONN:

[ESP32 Only] :ref:`AT+BLECONN <BLE-AT>`: Establish Bluetooth LE Connection
---------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the Bluetooth LE connection.

**Command:**

::

    AT+BLECONN?

**Response:**

::

    +BLECONN:<conn_index>,<remote_address>
    OK

If the connection has not been established, there will be no <conn_index> and <remote_address> in the response.

Set Command
^^^^^^^^^^^

**Function:**

Establish the Bluetooth LE connection.

**Command:**

::

    AT+BLECONN=<conn_index>,<remote_address>[,<addr_type>,<timeout>]

**Response:**

::

    OK

If the connection is established successfully, it will prompt:

::

    +BLECONN:<conn_index>,<remote_address>

Otherwise, it will prompt:

::

    +BLECONN:<conn_index>,-1

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<remote_address>**: remote Bluetooth LE address.
-  **[<addr_type>]**: the address type of broadcasters.
-  **[<timeout>]**: the timeout for the connection command. Unit: second. Range: [3,30].

Notes
^^^^^

-  It is recommended to scan devices by running :ref:`AT+BLESCAN <cmd-BSCAN>` before initiating a new connection to ensure that the target device is in the broadcast state.
-  The maximum timeout for connection is 30 seconds.
-  If the Bluetooth LE server is initialized and the connection is established successfully, you can use this command to discover the services in the peer device (GATTC). The following GATTC commands can also be used:

   -  :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`
   -  :ref:`AT+BLEGATTCINCLSRV <cmd-GCINCLSRV>`
   -  :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`
   -  :ref:`AT+BLEGATTCRD <cmd-GCRD>`
   -  :ref:`AT+BLEGATTCWR <cmd-GCWR>`
   -  :ref:`AT+BLEGATTSIND <cmd-GSIND>`

Example
^^^^^^^^

::

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:0a:c4:09:34:23",0,10

.. _cmd-BCONNP:

[ESP32 Only] :ref:`AT+BLECONNPARAM <BLE-AT>`: Query/Update Parameters of Bluetooth LE Connection
-------------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the parameters of Bluetooth LE connection.

**Command:**

::

    AT+BLECONNPARAM?

**Response:**

::

    +BLECONNPARAM:<conn_index>,<min_interval>,<max_interval>,<cur_interval>,<latency>,<timeout>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Update the parameters of Bluetooth LE connection.

**Command:**

::

    AT+BLECONNPARAM=<conn_index>,<min_interval>,<max_interval>,<latency>,<timeout>

**Response:**

::

    OK

If the setting fails, it will prompt the message below:

::

    +BLECONNPARAM: <conn_index>,-1

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<min_interval>**: minimum connecting interval. Range: 0x0006 ~ 0x0C80.
-  **<max_interval>**: maximum connecting interval. Range: 0x0006 ~ 0x0C80.
-  **<cur_interval>**: current connecting interval.
-  **<latency>**: latency. Range: 0x0000 ~ 0x01F3.
-  **<timeout>**: timeout. Range: 0x000A ~ 0x0C80.

Note
^^^^^

-  This command only supports the client role when updating its connection parameters. Of course, the connection has to be established first.

Example
^^^^^^^^

::

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLECONNPARAM=0,12,14,1,500  

.. _cmd-BDISC:

[ESP32 Only] :ref:`AT+BLEDISCONN <BLE-AT>`: End Bluetooth LE Connection
------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

End the Bluetooth LE connection.

**Command:**

::

    AT+BLEDISCONN=<conn_index>

**Response:**

::

    OK  // The AT+BLEDISCONN command is received.
    +BLEDISCONN:<conn_index>,<remote_address>  // The command is successful. 

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<remote_address>**: remote Bluetooth LE address.

Note
^^^^^

-  Only clients can call this command to terminate the connection.

Example
^^^^^^^^

::

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLEDISCONN=0

.. _cmd-BDLEN:

[ESP32 Only] :ref:`AT+BLEDATALEN <BLE-AT>`: Set Bluetooth LE Data Packet Length
---------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set the length of Bluetooth LE data packet.

**Command:**

::

    AT+BLEDATALEN=<conn_index>,<pkt_data_len>

**Response:**

::

    OK 

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<pkt_data_len>**: data packet's length. Range: 0x001b ~ 0x00fb.

Note
^^^^^

-  The Bluetooth LE connection has to be established first.

Example
^^^^^^^^

::

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLEDATALEN=0,30

.. _cmd-BMTU:

[ESP32 Only] :ref:`AT+BLECFGMTU <BLE-AT>`: Set Bluetooth LE MTU Length
-----------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the length of the maximum transmission unit (MTU).

**Command:**

::

    AT+BLECFGMTU?

**Response:**

::

    +BLECFGMTU:<conn_index>,<mtu_size>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the length of the maximum transmission unit (MTU).

**Command:**

::

    AT+BLECFGMTU=<conn_index>,<mtu_size>

**Response:**

::

    OK  // The command is received.

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<mtu_size>**: MTU length.

Notes
^^^^^

-  Bluetooth LE connection has to be established first.
-  Only the client can call this command to set the length of MTU. 
-  The actual length of MTU needs to be negotiated. The ``OK`` response only indicates an attempt to negotiate the length. The actual length may not be the value you set. Therefore, it is recommended to run command :ref:`AT+BLECFGMTU? <cmd-BMTU>` to query the actual length.

Example
^^^^^^^^

::

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:0a:c4:09:34:23"
    AT+BLECFGMTU=0,300

.. _cmd-GSSRVCRE:

[ESP32 Only] :ref:`AT+BLEGATTSSRVCRE <BLE-AT>`: GATTS Creates Services
------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

The Generic Attributes Server (GATTS) creates Bluetooth LE services.

**Command:**

::

    AT+BLEGATTSSRVCRE

**Response:**

::

    OK

Notes
^^^^^

-  If you are using an ESP device as a Bluetooth LE server, a service bin should be downloaded into flash in order to provide services.

   -  To learn how to generate a service bin, please refer to esp-at/tools/readme.md.
   -  The download address of the service bin is the "ble_data" address in esp-at/module_config/module_${platform}_default/at_customize.csv.

-  This command should be called immediately to create services, right after the Bluetooth LE server is initialized; If a Bluetooth LE connection is established first, the service creation will fail.
-  If the Bluetooth LE client is initialized, you can use this command to create local services. Some GATTS commands can also be used, such as those to start and stop services, set attribute values, and send notifications/indications. See the list below for the specific commands.

   -  :ref:`AT+BLEGATTSSRVCRE <cmd-GSSRVCRE>` (It is recommended to execute this command before the connection is established)
   -  :ref:`AT+BLEGATTSSRVSTART <cmd-GSSRVSTART>` (It is recommended to execute this command before the connection is established)
   -  :ref:`AT+BLEGATTSSRV <cmd-GSSRV>`
   -  :ref:`AT+BLEGATTSCHAR <cmd-GSCHAR>`
   -  :ref:`AT+BLEGATTSNTFY <cmd-GSNTFY>`
   -  :ref:`AT+BLEGATTSIND <cmd-GSIND>`
   -  :ref:`AT+BLEGATTSSETATTR <cmd-GSSETA>`

Example
^^^^^^^^

::

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE

.. _cmd-GSSRVSTART:

[ESP32 Only] :ref:`AT+BLEGATTSSRVSTART <BLE-AT>`: GATTS Starts Services
------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

GATTS starts all services.

**Command:**

::

    AT+BLEGATTSSRVSTART

Set Command
^^^^^^^^^^^

**Function:**

GATTS starts a specific service.

**Command:**

::

    AT+BLEGATTSSRVSTART=<srv_index>

**Response:**

::

    OK  

Parameter
^^^^^^^^^^

-  **<srv_index>**: service's index starting from 1.

Example
^^^^^^^^

::

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART

.. _cmd-GSSRVSTOP:

[ESP32 Only] :ref:`AT+BLEGATTSSRVSTOP <BLE-AT>`: GATTS Stops Services
-----------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

GATTS stops all services.

**Command:**

::

    AT+BLEGATTSSRVSTOP

Set Command
^^^^^^^^^^^

**Function:**

GATTS stops a specific service.

**Command:**

::

    AT+BLEGATTSSRVSTOP=<srv_index>

**Response:**

::

    OK  

Parameter
^^^^^^^^^^

-  **<srv_index>**: service's index starting from 1.

Example
^^^^^^^^

::

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEGATTSSRVSTOP

.. _cmd-GSSRV:

[ESP32 Only] :ref:`AT+BLEGATTSSRV <BLE-AT>`: GATTS Discovers Services
-----------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

GATTS discovers services.

**Command:**

::

    AT+BLEGATTSSRV?

**Response:**

::

    +BLEGATTSSRV:<srv_index>,<start>,<srv_uuid>,<srv_type>
    OK

Parameters
^^^^^^^^^^

-  **<srv_index>**: service's index starting from 1.
-  **<start>**:

   -  0: the service has not started.
   -  1: the service has already started.

-  **<srv_uuid>**: service's UUID.
-  **<srv_type>**: service's type.

   -  0: not primary service.
   -  1: primary service.

Example
^^^^^^^^

::

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRV?

.. _cmd-GSCHAR:

[ESP32 Only] :ref:`AT+BLEGATTSCHAR <BLE-AT>`: GATTS Discovers Characteristics
-------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

GATTS discovers characteristics.

**Command:**

::

    AT+BLEGATTSCHAR?

**Response:**

The response for a characteristic:

::

    +BLEGATTSCHAR:"char",<srv_index>,<char_index>,<char_uuid>,<char_prop>

The response for a descriptor:

::

    +BLEGATTSCHAR:"desc",<srv_index>,<char_index>,<desc_index> 
    OK

Parameters
^^^^^^^^^^

-  **<srv_index>**: service's index starting from 1.
-  **<char_index>**: characteristic's index starting from 1.
-  **<char_uuid>**: characteristic's UUID.
-  **<char_prop>**: characteristic's properties.
-  **<desc_index>**: descriptor's index.
-  **<desc_uuid>**: descriptor's UUID.

Example
^^^^^^^^

::

    AT+BLEINIT=2   // role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEGATTSCHAR?

.. _cmd-GSNTFY:

[ESP32 Only] :ref:`AT+BLEGATTSNTFY <BLE-AT>`: Notify a Client of the Value of a Characteristic Value from the Server
---------------------------------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Notify a client of the value of a characteristic value from the server.

**Command:**

::

    AT+BLEGATTSNTFY=<conn_index>,<srv_index>,<char_index>,<length>

**Response:**

::

    >

The symbol ``>`` indicates that AT is ready for receiving serial data, and you can enter data now. When the requirement of data length determined by the parameter <length> is met, the notification starts.

If the data transmission is successful, AT returns:

::

   OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<srv_index>**: service's index. It can be fetched with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
-  **<char_index>**: characteristic's index. It can be fetched with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
-  **<length>**: data length.

Example
^^^^^^^^

::

    AT+BLEINIT=2      // Role: server.
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEADVSTART    // Start advertising. After the client is connected, it must be configured to receive notifications.
    AT+BLEGATTSCHAR?  // Query the characteristics which the client will be notified of.
    // For example, to notify of 4-byte data using the 6th characteristic in the 3rd service, use the following command:
    AT+BLEGATTSNTFY=0,3,6,4 
    // After the symbol ">" shows, enter the 4-byte data, such as "1234". Then the data will be transmitted automatically.

.. _cmd-GSIND:

[ESP32 Only] :ref:`AT+BLEGATTSIND <BLE-AT>`: Indicate the Characteristic Value from the Server to a Client
-----------------------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**
 
Indicate the characteristic value from the server to a client.

**Command:**

::

    AT+BLEGATTSIND=<conn_index>,<srv_index>,<char_index>,<length>

**Response:**

::

    >

The symbol ``>`` indicates that AT is ready for receiving serial data and you can enter data now. When the requirement of data length determined by the parameter <length> is met, the indication starts.

If the data transmission is successful, AT returns:

::

   OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<srv_index>**: service's index. It can be fetched with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
-  **<char_index>**: characteristic's index; it can be fetched with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
-  **<length>**: data length.

Example
^^^^^^^^

::

    AT+BLEINIT=2      // Role: server
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEADVSTART    // Start advertising. After the client is connected, it must be configured to receive indications.
    AT+BLEGATTSCHAR?  // Query the characteristics which the client can receive indications.
    // For example, to indicate 4 bytes of data using the 7th characteristic in the 3rd service, use the following command:
    AT+BLEGATTSIND=0,3,7,4 
    // After the symbol ">" shows, input 4 bytes of data, such as "1234". Then the data will be transmitted automatically.

.. _cmd-GSSETA:

[ESP32 Only] :ref:`AT+BLEGATTSSETATTR <BLE-AT>`: GATTS Sets Characteristics
----------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

GATTS sets its characteristic (descriptor).

**Command:**

::

    AT+BLEGATTSSETATTR=<srv_index>,<char_index>,[<desc_index>],<length>

**Response:**

::

    >

The symbol ``>`` indicates that AT is ready for receiving serial data and you can enter data now. When the requirement of data length determined by the parameter <length> is met, the setting starts.

If the setting is successful, AT returns:

::

   OK

Parameters
^^^^^^^^^^

-  **<srv_index>**: service's index. It can be fetched with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
-  **<char_index>**: characteristic's index; it can be fetched with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
-  **[<desc_index>]**: descriptor's index.

   -  If it is set, this command is used to set the value of the descriptor. 
   -  Otherwise, this command is used to set the value of the characteristic.

-  **<length>**: data length.

Note
^^^^^

-  If the value of ``<length>`` is larger than the maximum length allowed, the setting will fail. The service table is defined in `components/customized_partitions/raw_data/ble_data`.

Example
^^^^^^^^

::

    AT+BLEINIT=2   // Role: server.
    AT+BLEGATTSSRVCRE
    AT+BLEGATTSSRVSTART
    AT+BLEGATTSCHAR? 
    // For example, to set 1 byte of data of the 1st characteristic in the 1st service, use the following command:
    AT+BLEGATTSSETATTR=1,1,,1
    // After the symbol ">" shows, input 1 byte of data, such as "8". Then the setting starts.

.. _cmd-GCPRIMSRV:

[ESP32 Only] :ref:`AT+BLEGATTCPRIMSRV <BLE-AT>`: GATTC Discovers Primary Services
----------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Generic Attributes Client (GATTC) discovers primary services.

**Command:**

::

    AT+BLEGATTCPRIMSRV=<conn_index>

**Response:**

::

    +BLEGATTCPRIMSRV:<conn_index>,<srv_index>,<srv_uuid>,<srv_type>
    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<srv_index>**: service's index starting from 1.
-  **<srv_uuid>**: service's UUID.
-  **<srv_type>**: service's type.

   -  0: not primary service.
   -  1: primary service.

Note
^^^^^

-  The Bluetooth LE connection has to be established first.

Example
^^^^^^^^

::

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0

.. _cmd-GCINCLSRV:

[ESP32 Only] :ref:`AT+BLEGATTCINCLSRV <BLE-AT>`: GATTC Discovers Included Services
-----------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

GATTC discovers included services.

**Command:**

::

    AT+BLEGATTCINCLSRV=<conn_index>,<srv_index>

**Response:**

::

    +BLEGATTCINCLSRV:<conn_index>,<srv_index>,<srv_uuid>,<srv_type>,<included_srv_uuid>,<included_srv_type>
    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<srv_index>**: service's index. It can be fetched with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index>.
-  **<srv_uuid>**: service's UUID.
-  **<srv_type>**: service's type.

   -  0: not primary service.
   -  1: primary service.

-  **<included_srv_uuid>**: included service's UUID.
-  **<included_srv_type>**: included service's type.

   -  0: not primary service.
   -  1: primary service.

Note
^^^^^

-  The Bluetooth LE connection has to be established first.

Example
^^^^^^^^

::

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCINCLSRV=0,1  // set a specific index according to the result of the previous command

.. _cmd-GCCHAR:

[ESP32 Only] :ref:`AT+BLEGATTCCHAR <BLE-AT>`: GATTC Discovers Characteristics
------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

GATTC discovers characteristics.

**Command:**

::

    AT+BLEGATTCCHAR=<conn_index>,<srv_index>

**Response:**

The response for a characteristic:

::

    +BLEGATTCCHAR:"char",<conn_index>,<srv_index>,<char_index>,<char_uuid>,<char_prop>

The response for a descriptor:

::

    +BLEGATTCCHAR:"desc",<conn_index>,<srv_index>,<char_index>,<desc_index>,<desc_uuid> 
    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<srv_index>**: service's index. It can be fetched with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index>.
-  **<char_index>**: characteristic's index starting from 1.
-  **<char_uuid>**: characteristic's UUID.
-  **<char_prop>**: characteristic's properties.
-  **<desc_index>**: descriptor's index.
-  **<desc_uuid>**: descriptor's UUID.

Note
^^^^^

-  The Bluetooth LE connection has to be established first.

Example
^^^^^^^^

::

    AT+BLEINIT=1   // role: client
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCCHAR=0,1 // set a specific index according to the result of the previous command

.. _cmd-GCRD:

[ESP32 Only] :ref:`AT+BLEGATTCRD <BLE-AT>`: GATTC Reads Characteristics
------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

GATTC reads a characteristic or descriptor.

**Command:**

::

    AT+BLEGATTCRD=<conn_index>,<srv_index>,<char_index>[,<desc_index>]

**Response:**

::

    +BLEGATTCRD:<conn_index>,<len>,<value>
    OK

Parameters
^^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<srv_index>**: service's index. It can be fetched with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index>.
-  **<char_index>**: characteristic's index; it can be fetched with command :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index>.
-  **[<desc_index>]**: descriptor's index.

   -  If it is set, the value of the target descriptor will be read.
   -  if it is not set, the value of the target characteristic will be read.

-  **<len>**: data length.
-  **<char_value>**: characteristic's value. HEX string is read by command :ref:`AT+BLEGATTCRD <cmd-GCRD>`\=<conn_index>,<srv_index>,<char_index>. For example, if the response is ``+BLEGATTCRD:1,30``, it means that the value length is 1, and the content is "0x30".
-  **[<desc_value>]**: descriptor's value. HEX string is read by command :ref:`AT+BLEGATTCRD <cmd-GCRD>`\=<conn_index>,<srv_index>,<char_index>,<desc_index>. For example, if the response is ``+BLEGATTCRD:4,30313233``, it means that the value length is 4, and the content is "0x30 0x31 0x32 0x33".

Notes
^^^^^

-  The Bluetooth LE connection has to be established first.
-  If the target characteristic cannot be read, it will return "ERROR".

Example
^^^^^^^^

::

    AT+BLEINIT=1   // Role: client.
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCCHAR=0,3 // Set a specific index according to the result of the previous command.
    // For example, to read 1st descriptor of the 2nd characteristic in the 3rd service, use the following command:
    AT+BLEGATTCRD=0,3,2,1

.. _cmd-GCWR:

[ESP32 Only] :ref:`AT+BLEGATTCWR <BLE-AT>`: GATTC Writes Characteristics
-------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

GATTC writes characteristics or descriptors.

**Command:**

::

    AT+BLEGATTCWR=<conn_index>,<srv_index>,<char_index>[,<desc_index>],<length>

**Response:**

::

    >

The symbol ``>`` indicates that AT is ready for receiving serial data and you can enter data now. When the requirement of data length determined by the parameter <length> is met, the writing starts.

If the setting is successful, AT returns:

::

   OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<srv_index>**: service's index. It can be fetched with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index>.
-  **<char_index>**: characteristic's index; it can be fetched with command :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index>.
-  **[<desc_index>]**: descriptor's index.

   -  If it is set, the value of the target descriptor will be written.
   -  If it is not set, the value of the target characteristic will be written.

-  **<length>**: data length.

Notes
^^^^^

-  The Bluetooth LE connection has to be established first.
-  If the target characteristic cannot be written, it will return "ERROR".

Example
^^^^^^^^

::

    AT+BLEINIT=1   // Role: client.
    AT+BLECONN=0,"24:12:5f:9d:91:98"
    AT+BLEGATTCPRIMSRV=0
    AT+BLEGATTCCHAR=0,3 // Set a specific index according to the result of the previous command.
    // For example, to write 6 bytes of data to the 4th characteristic in the 3rd service, use the following command:
    AT+BLEGATTCWR=0,3,4,,6 
    // After the symbol ">" shows, input 6 bytes of data, such as "123456". Then the writing starts.

.. _cmd-BLESPPCFG:

[ESP32 Only] :ref:`AT+BLESPPCFG <BLE-AT>`: Query/Set Bluetooth LE SPP Parameters
---------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the parameters of Bluetooth LE Serial Port Profile (SPP).

**Command:**

::

    AT+BLESPPCFG?

**Response:**

::

    +BLESPPCFG:<tx_service_index>,<tx_char_index>,<rx_service_index>,<rx_char_index>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set or reset the parameters of Bluetooth LE SPP.

**Command:**

::

    AT+BLESPPCFG=<cfg_enable>[,<tx_service_index>,<tx_char_index>,<rx_service_index>,<rx_char_index>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<cfg_enable>**:

   -  0: all the SPP parameters will be reset, and the following four parameters don't need input.
   -  1: you should input the following four parameters.

-  **<tx_service_index>**: tx service's index. It can be fetched with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> and :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>`.
-  **<tx_char_index>**: tx characteristic's index. It can be fetched with command :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> and :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
-  **<rx_service_index>**: rx service's index. It can be fetched with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> and :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>`.
-  **<rx_char_index>**: rx characteristic's index. It can be fetched with command :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> and :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.

Notes
^^^^^

-  In Bluetooth LE client, the property of tx characteristic must be ``write with response`` or ``write without response``, and the property of rx characteristic must be ``indicate`` or ``notify``.
-  In Bluetooth LE server, the property of tx characteristic must be ``indicate`` or ``notify``, and the property of rx characteristic must be ``write with response`` or ``write without response``.

Example
^^^^^^^^

::

    AT+BLESPPCFG=0          // reset Bluetooth LE SPP parameters
    AT+BLESPPCFG=1,3,5,3,7  // set Bluetooth LE SPP parameters
    AT+BLESPPCFG?           // query Bluetooth LE SPP parameters 

.. _cmd-BLESPP:

[ESP32 Only] :ref:`AT+BLESPP <BLE-AT>`: Enter Bluetooth LE SPP Mode
---------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Enter Bluetooth LE SPP mode.

**Command:**

::

    AT+BLESPP

**Response:**

::

    >

Notes
^^^^^

-  If the Bluetooth LE SPP parameters are illegal, this command will return ``ERROR``.
-  During the SPP transmission, AT will not prompt any connection status changes unless bit2 of :ref:`AT+SYSMSG <cmd-SYSMSG>` is 1.

Example
^^^^^^^^

::

    AT+BLESPP   // enter Bluetooth LE SPP mode

.. _cmd-BLESMPPAR:

[ESP32 Only] :ref:`AT+BLESECPARAM <BLE-AT>`: Query/Set Bluetooth LE Encryption Parameters
------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the parameters of Bluetooth LE SMP.

**Command:**

::

    AT+BLESECPARAM?

**Response:**

::

    +BLESECPARAM:<auth_req>,<iocap>,<key_size>,<init_key>,<rsp_key>,<auth_option>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the parameters of Bluetooth LE SMP.

**Command:**

::

    AT+BLESECPARAM=<auth_req>,<iocap>,<key_size>,<init_key>,<rsp_key>[,<auth_option>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<auth_req>**: authentication request.

   -  0: NO_BOND
   -  1: BOND
   -  4: MITM
   -  8: SC_ONLY
   -  9: SC_BOND
   -  12: SC_MITM
   -  13: SC_MITM_BOND

-  **<iocap>**: input and output capability.

   -  0: DisplayOnly
   -  1: DisplayYesNo
   -  2: KeyboardOnly
   -  3: NoInputNoOutput
   -  4: Keyboard display

-  **<key_size>**: key length. Range: 7 ~ 16 bytes.
-  **<init_key>**: initial key represented in bit combinations.
-  **<rsp_key>**: response key represented in bit combinations.
-  **<auth_option>**: authentication option of security.

   -  0: Select the security level automatically.
   -  1: If it cannot follow the preset security level, the connection will disconnect.

Note
^^^^^

-  The bit pattern for parameters ``<init_key>`` and ``<rsp_key>`` is:

   -  Bit0: Used to exchange the encryption key in the init key & response key.
   -  Bit1: Used to exchange the IRK key in the init key & response key.
   -  Bit2: Used to exchange the CSRK key in the init key & response key.
   -  Bit3: Used to exchange the link key (only used in the Bluetooth LE & BR/EDR coexist mode) in the init key & response key.

Example
^^^^^^^^

::

    AT+BLESECPARAM=1,4,16,3,3,0

.. _cmd-BLEENC:

[ESP32 Only] :ref:`AT+BLEENC <BLE-AT>`: Initiate Bluetooth LE Encryption Request
---------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Start a pairing request

**Command:**

::

    AT+BLEENC=<conn_index>,<sec_act>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<sec_act>**:

   -  0: SEC_NONE
   -  1: SEC_ENCRYPT
   -  2: SEC_ENCRYPT_NO_MITM
   -  3: SEC_ENCRYPT_MITM

Note
^^^^^

-  Before running this command, please set the security parameters and connection with remote devices.

Example
^^^^^^^^

::

    AT+BLESECPARAM=1,4,16,3,3
    AT+BLEENC=0,3

.. _cmd-BLEENCRSP:

[ESP32 Only] :ref:`AT+BLEENCRSP <BLE-AT>`: Respond to the Pairing Request from the Peer Device
-----------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Respond to the pairing request from the peer device.

**Command:**

::

    AT+BLEENCRSP=<conn_index>,<accept>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<accept>**:

   -  0: reject
   -  1: accept

Note
^^^^^

-  After running this command, AT will output the pairing result at the end of the pairing process.

::

    +BLEAUTHCMPL:<conn_index>,<enc_result>

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<enc_result>**:

   - 0: encryption succeeded
   - 1: encryption failed

Example
^^^^^^^^

::

    AT+BLEENCRSP=0,1

.. _cmd-BLEKEYREPLY:

[ESP32 Only] :ref:`AT+BLEKEYREPLY <BLE-AT>`: Reply the Key Value to the Peer Device in the Legacy Connection Stage
-------------------------------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Reply a pairing key.

**Command:**

::

    AT+BLEKEYREPLY=<conn_index>,<key>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<key>**: pairing key.

Example
^^^^^^^^

::

    AT+BLEKEYREPLY=0,649784

.. _cmd-BLECONFREPLY:

[ESP32 Only] :ref:`AT+BLECONFREPLY <BLE-AT>`: Reply the Confirm Value to the Peer Device in the Legacy Connection Stage
------------------------------------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Reply a pairing result.

**Command:**

::

    AT+BLECONFREPLY=<conn_index>,<confirm>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
-  **<confirm>**:

   -  0: No
   -  1: Yes

Example
^^^^^^^^

::

    AT+BLECONFREPLY=0,1

.. _cmd-BLEENCDEV:

[ESP32 Only] :ref:`AT+BLEENCDEV <BLE-AT>`: Query Bonded Bluetooth LE Encryption Device List
--------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query bonded Bluetooth LE encryption device list.

**Command:**

::

    AT+BLEENCDEV?

**Response:**

::

    +BLEENCDEV:<enc_dev_index>,<mac_address>
    OK

Parameters
^^^^^^^^^^

-  **<enc_dev_index>**: index of the bonded devices.
-  **<mac_address>**: MAC address.

Example
^^^^^^^^

::

    AT+BLEENCDEV?

.. _cmd-BLEENCCLEAR:

[ESP32 Only] :ref:`AT+BLEENCCLEAR <BLE-AT>`: Clear Bluetooth LE Encryption Device List
---------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Remove a device from the security database list with a specific index.

**Command:**

::

    AT+BLEENCCLEAR=<enc_dev_index>

**Response:**

::

    OK

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Remove all devices from the security database.

**Command:**

::

    AT+BLEENCCLEAR

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<enc_dev_index>**: index of the bonded devices.

Example
^^^^^^^^

::

    AT+BLEENCCLEAR

.. _cmd-BLESETKEY:

[ESP32 Only] :ref:`AT+BLESETKEY <BLE-AT>`: Set Bluetooth LE Static Pair Key
----------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the Bluetooth LE static pair key. If it is not set, AT will return -1.

**Command:**

::

    AT+BLESETKEY?

**Response:**

::

    +BLESETKEY:<static_key>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set a Bluetooth LE static pair key for all Bluetooth LE connections.

**Command:**

::

    AT+BLESETKEY=<static_key>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<static_key>**: static Bluetooth LE pair key.

Example
^^^^^^^^

::

    AT+BLESETKEY=123456

.. _cmd-BLEHIDINIT:

[ESP32 Only] :ref:`AT+BLEHIDINIT <BLE-AT>`: Bluetooth LE HID Profile Initialization
-------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the initialization status of Bluetooth LE HID profile.

**Command:**

::

    AT+BLEHIDINIT?

**Response:**

If Bluetooth LE HID device profile is not initialized, AT will return:

::

    +BLEHIDINIT:0
    OK

If Bluetooth LE HID device profile is initialized, AT will return:

::

    +BLEHIDINIT:1
    OK

Set Command
^^^^^^^^^^^

**Function:**

Initialize the Bluetooth LE HID profile.

**Command:**

::

    AT+BLEHIDINIT=<init>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<init>**:

   -  0: deinit Bluetooth LE HID profile
   -  1: init Bluetooth LE HID profile

Note
^^^^^

-  The Bluetooth LE HID command cannot be used at the same time with general GATT/GAP commands.

Example
^^^^^^^^

::

    AT+BLEHIDINIT=1 

.. _cmd-BLEHIDKB:

[ESP32 Only] :ref:`AT+BLEHIDKB <BLE-AT>`: Send Bluetooth LE HID Keyboard Information
-------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Send keyboard information.

**Command:**

::

    AT+BLEHIDKB=<Modifier_keys>,<key_1>,<key_2>,<key_3>,<key_4>,<key_5>,<key_6>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<Modifier_keys>**: modifier keys mask
-  **<key_1>**: key code 1
-  **<key_2>**: key code 2
-  **<key_3>**: key code 3
-  **<key_4>**: key code 4
-  **<key_5>**: key code 5
-  **<key_6>**: key code 6

Note
^^^^

- For more information about key codes, please refer to the chapter Keyboard/Keypad Page of `Universal Serial Bus HID Usage Tables <https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf>`_.

Example
^^^^^^^^

::

    AT+BLEHIDKB=0,4,0,0,0,0,0   // input the string "a"

.. _cmd-BLEHIDMUS:

[ESP32 Only] :ref:`AT+BLEHIDMUS <BLE-AT>`: Send Bluetooth LE HID Mouse Information
------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Send mouse information.

**Command:**

::

    AT+BLEHIDMUS=<buttons>,<X_displacement>,<Y_displacement>,<wheel>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<buttons>**: mouse button
-  **<X_displacement>**: X displacement
-  **<Y_displacement>**: Y displacement
-  **<wheel>**: wheel

Example
^^^^^^^^

::

    AT+BLEHIDMUS=0,10,10,0

.. _cmd-BLEHIDC:

[ESP32 Only] :ref:`AT+BLEHIDCONSUMER <BLE-AT>`: Send Bluetooth LE HID Consumer Information
--------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Send consumer information.

**Command:**

::

    AT+BLEHIDCONSUMER=<consumer_usage_id>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<consumer_usage_id>**: consumer ID, such as power, reset, help, volume and so on. See chapter Consumer Page (0x0C) of `HID Usage Tables for Universal Serial Bus (USB) <https://usb.org/sites/default/files/hut1_21_0.pdf>`_ for more information.

Example
^^^^^^^^

::

    AT+BLEHIDCONSUMER=233   // volume up

.. _cmd-BLUFI:

[ESP32 Only] :ref:`AT+BLUFI <BLE-AT>`: Start or Stop BluFi
-----------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the status of BluFi.

**Command:**

::

    AT+BLUFI?

**Response:**

If BluFi is not started, it will return:

::

    +BLUFI:0
    OK

If BluFi is started, it will return:

::

    +BLUFI:1
    OK

Set Command
^^^^^^^^^^^

**Function:**

Start or stop BluFi.

**Command:**

::

    AT+BLUFI=<option>[,<auth floor>]

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<option>**:

   -  0: stop BluFi
   -  1: start BluFi

-  **<auth floor>**: Wi-Fi authentication mode floor. ESP-AT will not connect to the AP whose authmode is lower than this floor.

   -  0: OPEN (Default)
   -  1: WEP
   -  2: WPA_PSK
   -  3: WPA2_PSK
   -  4: WPA_WPA2_PSK
   -  5: WPA2_ENTERPRISE
   -  6: WPA3_PSK
   -  7: WPA2_WPA3_PSK

Example
^^^^^^^^

::

    AT+BLUFI=1

.. _cmd-BLUFINAME:

[ESP32 Only] :ref:`AT+BLUFINAME <BLE-AT>`: Query/Set BluFi Device Name
------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the BluFi name.

**Command:**

::

    AT+BLUFINAME?

**Response:**

::

    +BLUFINAME:<device_name>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the BluFi device name.

**Command:**

::

    AT+BLUFINAME=<device_name>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<device_name>**: the name of BluFi device.

Notes
^^^^^

-  If you need to set BluFi name, please set it before command :ref:`AT+BLUFI=1 <cmd-BLUFI>`. Otherwise, it will use the default name ``BLUFI_DEVICE``.
-  The maximum length of BluFi name is 29 bytes.

Example
^^^^^^^^

::

    AT+BLUFINAME="BLUFI_DEV"
    AT+BLUFINAME?