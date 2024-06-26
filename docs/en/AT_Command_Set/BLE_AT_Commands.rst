.. _BLE-AT:

Bluetooth® Low Energy AT Commands
=====================================================

:link_to_translation:`zh_CN:[中文]`

.. list::

    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`Introduction <cmd-ble-intro>`
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEINIT <cmd-BINIT>`: Bluetooth LE initialization.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEADDR <cmd-BADDR>`: Query/Set Bluetooth LE device address.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLENAME <cmd-BNAME>`: Query/Set Bluetooth LE device name.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLESCANPARAM <cmd-BSCANP>`: Query/Set parameters of Bluetooth LE scanning.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLESCAN <cmd-BSCAN>`: Enable Bluetooth LE scanning.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLESCANRSPDATA <cmd-BSCANR>`: Set Bluetooth LE scan response.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEADVPARAM <cmd-BADVP>`: Query/Set parameters of Bluetooth LE advertising.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEADVDATA <cmd-BADVD>`: Set Bluetooth LE advertising data.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEADVDATAEX <cmd-BADVDEX>`: Automatically set Bluetooth LE advertising data.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEADVSTART <cmd-BADVSTART>`: Start Bluetooth LE advertising.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEADVSTOP <cmd-BADVSTOP>`: Stop Bluetooth LE advertising.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLECONN <cmd-BCONN>`: Establish Bluetooth LE connection.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLECONNPARAM <cmd-BCONNP>`: Query/Update parameters of Bluetooth LE connection.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEDISCONN <cmd-BDISC>`: End Bluetooth LE connection.
    :esp32 or esp32c3 : - :ref:`AT+BLEDATALEN <cmd-BDLEN>`: Set Bluetooth LE data packet length.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLECFGMTU <cmd-BMTU>`: Set Bluetooth LE MTU length.
    :esp32 or esp32c3 : - :ref:`AT+BLEGATTSSRVCRE <cmd-GSSRVCRE>`: Generic Attributes Server (GATTS) creates services.
    :esp32 or esp32c3 : - :ref:`AT+BLEGATTSSRVSTART <cmd-GSSRVSTART>`: GATTS starts services.
    :esp32 or esp32c3 : - :ref:`AT+BLEGATTSSRVSTOP <cmd-GSSRVSTOP>`: GATTS Stops Services.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEGATTSSRV <cmd-GSSRV>`: GATTS discovers services.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEGATTSCHAR <cmd-GSCHAR>`: GATTS discovers characteristics.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEGATTSNTFY <cmd-GSNTFY>`: Notify a client of the value of a characteristic value from the server.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEGATTSIND <cmd-GSIND>`: Indicate the characteristic value from the server to a client.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEGATTSSETATTR <cmd-GSSETA>`: GATTS sets characteristics.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`: Generic Attributes Client (GATTC) discovers primary services.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEGATTCINCLSRV <cmd-GCINCLSRV>`: GATTC discovers included services.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`: GATTC discovers characteristics.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEGATTCRD <cmd-GCRD>`: GATTC reads characteristics.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEGATTCWR <cmd-GCWR>`: GATTC writes characteristics.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLESPPCFG <cmd-BLESPPCFG>`: Query/Set Bluetooth LE SPP parameters.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLESPP <cmd-BLESPP>`: Enter Bluetooth LE SPP mode.
    :esp32 or esp32c3 or esp32c6: - :ref:`AT+SAVETRANSLINK <cmd-SAVET>`: Set whether to enter Bluetooth LE :term:`Passthrough Mode` on power-up.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLESECPARAM <cmd-BLESMPPAR>`: Query/Set Bluetooth LE encryption parameters.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEENC <cmd-BLEENC>`: Initiate Bluetooth LE encryption request.
    :esp32 or esp32c3 : - :ref:`AT+BLEENCRSP <cmd-BLEENCRSP>`: Respond to the pairing request from the peer device.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEKEYREPLY <cmd-BLEKEYREPLY>`: Reply the key value to the peer device.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLECONFREPLY <cmd-BLECONFREPLY>`: Reply the confirm value to the peer device in the legacy connection stage.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEENCDEV <cmd-BLEENCDEV>`: Query bonded Bluetooth LE encryption device list.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEENCCLEAR <cmd-BLEENCCLEAR>`: Clear Bluetooth LE encryption device list.
    :esp32 or esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLESETKEY <cmd-BLESETKEY>`: Set Bluetooth LE static pair key.
    :esp32 or esp32c3 : - :ref:`AT+BLEHIDINIT <cmd-BLEHIDINIT>`: Bluetooth LE Human Interface Device (HID) profile initialization.
    :esp32 or esp32c3 : - :ref:`AT+BLEHIDKB <cmd-BLEHIDKB>`: Send Bluetooth LE HID keyboard information.
    :esp32 or esp32c3 : - :ref:`AT+BLEHIDMUS <cmd-BLEHIDMUS>`: Send Bluetooth LE HID mouse information.
    :esp32 or esp32c3 : - :ref:`AT+BLEHIDCONSUMER <cmd-BLEHIDC>`: Send Bluetooth LE HID consumer information.
    - :ref:`AT+BLUFI <cmd-BLUFI>`: Start or Stop BluFi.
    - :ref:`AT+BLUFINAME <cmd-BLUFINAME>`: Query/Set BluFi device name.
    - :ref:`AT+BLUFISEND <cmd-BLUFISEND>`: Send user-customized data over BluFi.
    :esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEPERIODICDATA <cmd-BLEPADATA>`: Set Bluetooth LE periodic advertising data.
    :esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEPERIODICSTART <cmd-BLEPASTART>`: Start Bluetooth LE periodic advertising.
    :esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEPERIODICSTOP <cmd-BLEPASTOP>`: Stop Bluetooth LE periodic advertising.
    :esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLESYNCSTART <cmd-BLESYNCSTART>`: Start to synchronize with periodic advertising.
    :esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLESYNCSTOP <cmd-BLESYNCSTOP>`: Stop synchronizing with periodic advertising.
    :esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLEREADPHY <cmd-BLERDPHY>`: Query the current transmitter PHY.
    :esp32c3 or esp32c6 or esp32c2: - :ref:`AT+BLESETPHY <cmd-BLESETPHY>`: Set the current transmitter PHY.

.. _cmd-ble-intro:

Introduction
------------

 .. only:: esp32

  Currently, AT firmware for {IDF_TARGET_NAME} series supports `Bluetooth® Core Specification Version 4.2 <https://www.bluetooth.com/specifications/specs/core-specification-4-2/>`_.
 
 .. only:: esp32c2 or esp32c3 or esp32c6

  Currently, AT firmware for {IDF_TARGET_NAME} supports `Bluetooth® Core Specification Version 5.0 <https://www.bluetooth.com/specifications/specs/core-specification-5-0/>`_.

.. only:: esp32 or esp32c3 or esp32c6

  .. important::
    The default AT firmware supports all the AT commands mentioned on this page. If you need to modify the commands supported by {IDF_TARGET_NAME} by default, please compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections (Each item below is independent. Choose it according to your needs):

    - Disable BluFi commands: ``Component config`` -> ``AT`` -> ``AT blufi command support``
    - Disable Bluetooth LE commands: ``Component config`` -> ``AT`` -> ``AT ble command support``
    - Disable Bluetooth LE HID commands: ``Component config`` -> ``AT`` -> ``AT ble hid command support``

.. only:: esp32c2

  .. important::
    The default {IDF_TARGET_CFG_PREFIX}-4MB AT firmware supports BluFi functionality, while the {IDF_TARGET_CFG_PREFIX}-2MB AT firmware does not. The lack of BluFi support in the {IDF_TARGET_CFG_PREFIX}-2MB AT firmware is due to the insufficient 2 MB flash size to support Wi-Fi, BluFi, and OTA functionality simultaneously.

  .. important::
    The default {IDF_TARGET_CFG_PREFIX}-4MB AT firmware and {IDF_TARGET_CFG_PREFIX}-2MB AT firmware do not support Bluetooth LE functionality. If you need to support Bluetooth LE functionality, please refer to the :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation and choose ``ESP32C2-BLE-2MB`` during the environment installation in step 3.

    Due to the limited memory of {IDF_TARGET_NAME}, Wi-Fi and Bluetooth LE functionalities cannot be supported simultaneously in a single firmware.

.. only:: esp32 or esp32c3 or esp32c6 or esp32c2

    .. _cmd-BINIT:

    :ref:`AT+BLEINIT <BLE-AT>`: Bluetooth LE Initialization
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

    -  To achieve better performance, it is recommended to disable SoftAP by sending the :ref:`AT+CWMODE=0/1 <cmd-MODE>` command before using Bluetooth LE function. For more details, please refer to the `RF Coexistence <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/coexist.html>`_ documentation.
    -  Before using other Bluetooth LE AT commands, you should run this command first to trigger the initialization process.
    -  After the initialization, the Bluetooth LE role cannot be changed unless you run :ref:`AT+RST <cmd-RST>` to restart the system first and then re-initialize the Bluetooth LE role.
    -  Before you deinitialize the Bluetooth stack, it is recommended to stop broadcasting, stop scanning, and disconnect all existing connections.
    -  If Bluetooth LE is initialized, :ref:`AT+CIPMODE <cmd-IPMODE>` cannot be set to 1.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=1

    .. _cmd-BADDR:

    :ref:`AT+BLEADDR <BLE-AT>`: Query/Set Bluetooth LE Device Address
    -------------------------------------------------------------------------------------

    Query Command
    ^^^^^^^^^^^^^

    **Function:**

    .. only:: esp32 or esp32c3

        ::

            Query the Bluetooth LE Public Address.

    .. only:: esp32c2 or esp32c6

        ::

            Query the Bluetooth LE Random Address.

    **Command:**

    ::

        AT+BLEADDR?

    **Response:**

    .. only:: esp32 or esp32c3

        ::

            +BLEADDR:<BLE_public_addr>
            OK

    .. only:: esp32c2 or esp32c6

        ::

            +BLEADDR:<BLE_random_addr>
            OK

    Set Command
    ^^^^^^^^^^^

    **Function:**

    Set the Bluetooth LE address type.

    **Command:**

    .. only:: esp32 or esp32c3

        ::

            AT+BLEADDR=<addr_type>[,<random_addr>]

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLEADDR=<addr_type>

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

    -  The configuration changes will be saved in the NVS area. 

    Example
    ^^^^^^^^

    .. only:: esp32 or esp32c3

        ::

            AT+BLEADDR=1,"f8:7f:24:87:1c:7b"    // Set Random Device Address, Static Address
            AT+BLEADDR=1                        // Set Random Device Address, Private Address
            AT+BLEADDR=0                        // Set Public Device Address

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLEADDR=1                        // Set Random Device Address, Private Address

    .. _cmd-BNAME:

    :ref:`AT+BLENAME <BLE-AT>`: Query/Set Bluetooth LE Device Name
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

    -  **<device_name>**: the Bluetooth LE device name. The maximum length is 32. Default: "ESP-AT".

    Note
    ^^^^^

    -  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`. 
    -  After setting the device name with this command, it is recommended that you execute the :ref:`AT+BLEADVDATA <cmd-BADVD>` command to add the device name into the advertising data.

    Example
    ^^^^^^^^

    ::

        AT+BLENAME="esp_demo"

    .. _cmd-BSCANP:

    :ref:`AT+BLESCANPARAM <BLE-AT>`: Query/Set Parameters of Bluetooth LE Scanning
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

    -  **<scan_interval>**: scan interval. It should be more than or equal to the value of ``<scan_window>``. The range of this parameter is [0x0004,0x4000]. The scan interval equals this parameter multiplied by ``0.625 ms``, so the range for the actual scan interval is [2.5,10240] ms.
    -  **<scan_window>**: scan window. It should be less than or equal to the value of ``<scan_interval>``. The range of this parameter is [0x0004,0x4000]. The scan window equals this parameter multiplied by ``0.625 ms``, so the range for the actual scan window is [2.5,10240] ms.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=1   // Role: client
        AT+BLESCANPARAM=0,0,0,100,50

    .. _cmd-BSCAN:

    :ref:`AT+BLESCAN <BLE-AT>`: Enable Bluetooth LE Scanning
    ----------------------------------------------------------------------------

    Set Command
    ^^^^^^^^^^^

    **Function:**

    Enable/disable scanning.

    **Command:**

    ::

        AT+BLESCAN=<enable>[,<duration>][,<filter_type>,<filter_param>]

    **Response:**

    ::

        +BLESCAN:<addr>,<rssi>,<adv_data>,<scan_rsp_data>,<addr_type>
        OK
        +BLESCANDONE

    Parameters
    ^^^^^^^^^^

    -  **<enable>**:

    -  1: enable continuous scanning.
    -  0: disable continuous scanning.

    -  **[<duration>]**: optional parameter. Unit: second.

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
    -  To obtain the scan response data, use the :ref:`AT+BLESCANPARAM <BLE-AT>` command to set the scan type to ``active scan (AT+BLESCANPARAM=1,0,0,100,50)``, and the peer device needs to set the ``scan_rsp_data``.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=1    // Role: client
        AT+BLESCAN=1    // start scanning
        AT+BLESCAN=0    // stop scanning
        AT+BLESCAN=1,3,1,"24:0A:C4:96:E6:88"  // start scanning, filter type is MAC address
        AT+BLESCAN=1,3,2,"ESP-AT"  // start scanning, filter type is device name

    .. _cmd-BSCANR:

    :ref:`AT+BLESCANRSPDATA <BLE-AT>`: Set Bluetooth LE Scan Response
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

        AT+BLEINIT=2   // Role: server
        AT+BLESCANRSPDATA="1122334455"

    .. _cmd-BADVP:

    :ref:`AT+BLEADVPARAM <BLE-AT>`: Query/Set Parameters of Bluetooth LE Advertising
    ----------------------------------------------------------------------------------------------------

    Query Command
    ^^^^^^^^^^^^^

    **Function:**

    Query the parameters of advertising.

    **Command:**

    ::

        AT+BLEADVPARAM?

    **Response:**

    .. only:: esp32

        ::

            +BLEADVPARAM:<adv_int_min>,<adv_int_max>,<adv_type>,<own_addr_type>,<channel_map>,<adv_filter_policy>,<peer_addr_type>,<peer_addr>
            OK

    .. only:: esp32c3 or esp32c6 or esp32c2

        ::

            +BLEADVPARAM:<adv_int_min>,<adv_int_max>,<adv_type>,<own_addr_type>,<channel_map>,<adv_filter_policy>,<peer_addr_type>,<peer_addr>,<primary_phy>,<secondary_phy>
            OK

    Set Command
    ^^^^^^^^^^^

    **Function:**

    Set the parameters of advertising.

    **Command:**

    .. only:: esp32

        ::

            AT+BLEADVPARAM=<adv_int_min>,<adv_int_max>,<adv_type>,<own_addr_type>,<channel_map>[,<adv_filter_policy>][,<peer_addr_type>,<peer_addr>]

    .. only:: esp32c3 or esp32c6 or esp32c2

        ::

            AT+BLEADVPARAM=<adv_int_min>,<adv_int_max>,<adv_type>,<own_addr_type>,<channel_map>[,<adv_filter_policy>][,<peer_addr_type>,<peer_addr>][,<primary_phy>,<secondary_phy>]

    **Response:**

    ::

        OK

    Parameters
    ^^^^^^^^^^

    -  **<adv_int_min>**: minimum advertising interval. The range of this parameter is [0x0020,0x4000]. The actual advertising interval equals this parameter multiplied by ``0.625 ms``, so the range for the actual minimum interval is [20, 10240] ms. It should be less than or equal to the value of ``<adv_int_max>``.
    -  **<adv_int_max>**: maximum advertising interval. The range of this parameter is [0x0020,0x4000]. The actual advertising interval equals this parameter multiplied by ``0.625 ms``, so the range for the actual maximum interval is [20, 10240] ms. It should be more than or equal to the value of ``<adv_int_min>``.
    -  **<adv_type>**:

    .. only:: esp32

        -  0: ADV_TYPE_IND
        -  1: ADV_TYPE_DIRECT_IND_HIGH
        -  2: ADV_TYPE_SCAN_IND
        -  3: ADV_TYPE_NONCONN_IND
        -  4: ADV_TYPE_DIRECT_IND_LOW

    .. only:: esp32c3 or esp32c6 or esp32c2

        -  0: ADV_TYPE_IND
        -  1: ADV_TYPE_DIRECT_IND_HIGH
        -  2: ADV_TYPE_SCAN_IND
        -  3: ADV_TYPE_NONCONN_IND
        -  4: ADV_TYPE_DIRECT_IND_LOW
        -  5: ADV_TYPE_EXT_NOSCANNABLE_IND
        -  6: ADV_TYPE_EXT_CONNECTABLE_IND
        -  7: ADV_TYPE_EXT_SCANNABLE_IND
            -  When <adv_type> is set to 0-4, the maximum allowed size for advertising data using the :ref:`AT+BLEADVDATA <cmd-BADVD>` command is 31 bytes. If the advertising data exceeds the maximum limit of this command, use command :ref:`AT+BLESCANRSPDATA <cmd-BSCANR>`.
            -  When <adv_type> is set to 5-7, the maximum allowed size for advertising data using the :ref:`AT+BLEADVDATA <cmd-BADVD>` command is 119 bytes.

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

    .. only:: esp32c3 or esp32c6 or esp32c2

        -  **[<primary_phy>]**: advertising primary PHY. Default: 1M PHY.
        
            -  1: 1M PHY
            -  3: Coded PHY
        
        -  **[<secondary_phy>]**: advertising secondary PHY. Default: 1M PHY.
        
            -  1: 1M PHY
            -  2: 2M PHY
            -  3: Coded PHY

    Note
    ^^^^^

    -  If ``peer_addr`` is not set, the query result will be all zero.

    .. only:: esp32c3 or esp32c6 or esp32c2

        -  The ``primary_phy`` and ``secondary_phy`` must be set together, otherwise, the default 1M PHY is used for unconfigured parameter.

    .. only:: esp32

        Example
        ^^^^^^^^^^

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEADDR=1,"c2:34:45:78:66:89"
            AT+BLEADVPARAM=50,50,0,1,4,0,1,"12:34:45:78:66:88"
            // At this time, the MAC of the ESP device scanned by the BLE client is "c2:34:45:78:66:89".

    .. only:: esp32c2 or esp32c6

        Example
        ^^^^^^^^^^

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEADVPARAM=50,50,0,0,4,0,1,"12:34:45:78:66:88"
            AT+BLEADVPARAM=32,32,6,0,7,0,0,"62:34:45:78:66:88",1,3

    .. only:: esp32c3

        Example 1
        ^^^^^^^^^^

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEADVPARAM=50,50,0,0,4,0,1,"12:34:45:78:66:88"
            AT+BLEADVPARAM=32,32,6,0,7,0,0,"62:34:45:78:66:88",1,3

        Example 2
        ^^^^^^^^^^

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEADDR=1,"c2:34:45:78:66:89"
            AT+BLEADVPARAM=50,50,0,1,4,0,1,"12:34:45:78:66:88"
            // At this time, the MAC of the ESP device scanned by the BLE client is "c2:34:45:78:66:89".

    .. _cmd-BADVD:

    :ref:`AT+BLEADVDATA <BLE-AT>`: Set Bluetooth LE Advertising Data
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

    .. only:: esp32c3 or esp32c6 or esp32c2

        -  **<adv_data>**: advertising data in HEX string. For example, to set the advertising data to "0x11 0x22 0x33 0x44 0x55", the command should be ``AT+BLEADVDATA="1122334455"``. The maximum length is 119 bytes.

    .. only:: esp32

        -  **<adv_data>**: advertising data in HEX string. For example, to set the advertising data to "0x11 0x22 0x33 0x44 0x55", the command should be ``AT+BLEADVDATA="1122334455"``. The maximum length is 31 bytes.

    Note
    ^^^^^

    -  If advertising data is preset by command :ref:`AT+BLEADVDATAEX <cmd-BADVDEX>`\=<dev_name>,<uuid>,<manufacturer_data>,<include_power>, it will be overwritten by this command.
    -  If you run this command to modify the device name, it is recommended to also execute the :ref:`AT+BLENAME <cmd-BNAME>` command to set the same device name afterwards.

    .. only:: esp32

        -  If the advertising data exceeds the maximum limit of this command, use command :ref:`AT+BLESCANRSPDATA <cmd-BSCANR>`.

    .. only:: esp32c3 or esp32c6 or esp32c2

        -  Please set the Bluetooth LE advertising parameters by the :ref:`AT+BLEADVPARAM <cmd-BADVP>` command before you use the :ref:`AT+BLEADVDATA <cmd-BADVD>` command.
        -  When the <adv_type> is set to 0-4 by the :ref:`AT+BLEADVPARAM <cmd-BADVP>` command, the maximum allowed size for advertising data using the AT+BLEADVDATA command is 31 bytes. If the advertising data exceeds the maximum limit of this command, use command :ref:`AT+BLESCANRSPDATA <cmd-BSCANR>`.
        -  When the <adv_type> is set to 5-7 by the :ref:`AT+BLEADVPARAM <cmd-BADVP>` command, the maximum allowed size for advertising data using the AT+BLEADVDATA command is 119 bytes.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=2   // Role: server
        AT+BLEADVDATA="1122334455"

    .. _cmd-BADVDEX:

    :ref:`AT+BLEADVDATAEX <BLE-AT>`: Automatically Set Bluetooth LE Advertising Data
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
    -  This command automatically changes the adv type previously set using :ref:`AT+BLEADVPARAM <cmd-BADVP>` to 0.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=2   // Role: server
        AT+BLEADVDATAEX="ESP-AT","A002","0102030405",1

    .. _cmd-BADVSTART:

    :ref:`AT+BLEADVSTART <BLE-AT>`: Start Bluetooth LE Advertising
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

    .. only:: esp32 or esp32c3

        -  If advertising parameters are NOT set by command :ref:`AT+BLEADVPARAM <cmd-BADVP>`\=<adv_parameter>, the default parameters will be used.

    .. only:: esp32c2 or esp32c6

        -  You must set the advertisement parameters using the :ref:`AT+BLEADVPARAM <cmd-BADVP>`\=<adv_parameter> command before advertisement is started.

    -  If advertising data is NOT set by command :ref:`AT+BLEADVDATA <cmd-BADVD>`\=<adv_data>, the advertising playload will be empty.
    -  If advertising data is preset by command :ref:`AT+BLEADVDATA <cmd-BADVD>`\=<adv_data>, it will be overwritten by :ref:`AT+BLEADVDATAEX <cmd-BADVDEX>`\=<dev_name>,<uuid>,<manufacturer_data>,<include_power> and vice versa.
    -  After the start of Bluetooth LE advertisement, if no connection is established, the advertisement will keep going on; if a connection is established, the advertisement will automatically end.

    Example
    ^^^^^^^^

    .. only:: esp32 or esp32c3

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEADVSTART

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEADVPARAM=50,50,0,0,7,0,,
            AT+BLEADVSTART

    .. _cmd-BADVSTOP:

    :ref:`AT+BLEADVSTOP <BLE-AT>`: Stop Bluetooth LE Advertising
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

    .. only:: esp32 or esp32c3

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEADVSTART
            AT+BLEADVSTOP

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEADVPARAM=50,50,0,0,7,0,,
            AT+BLEADVSTART
            AT+BLEADVSTOP

    .. _cmd-BCONN:

    :ref:`AT+BLECONN <BLE-AT>`: Establish Bluetooth LE Connection
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

    If the connection is established successfully, it will prompt:

    ::

        +BLECONN:<conn_index>,<remote_address>

        OK

    If the connection fails, it will prompt:

    ::

        +BLECONN:<conn_index>,-1

        ERROR

    If the connection fails due to parameters error or other reasons, it will prompt:

    ::

        ERROR

    Parameters
    ^^^^^^^^^^

    .. only:: esp32 or esp32c3 or esp32c6

        - **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].

    .. only:: esp32c2

        - **<conn_index>**: index of Bluetooth LE connection. Range: [0,1].

    - **<remote_address>**: remote Bluetooth LE address.
    - **[<addr_type>]**: the address type of broadcasters:

      - 0: Public Address
      - 1: Random Address

    - **[<timeout>]**: the timeout for the connection command. Unit: second. Range: [3,30].

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
    -  If the :ref:`AT+BLECONN? <cmd-BCONN>` is executed when the Bluetooth LE is not initialized (:ref:`AT+BLEINIT=0 <cmd-BINIT>`), the system will not output ``+BLECONN:<conn_index>,<remote_address>`` .

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=1   // Role: client
        AT+BLECONN=0,"24:0a:c4:09:34:23",0,10

    .. _cmd-BCONNP:

    :ref:`AT+BLECONNPARAM <BLE-AT>`: Query/Update Parameters of Bluetooth LE Connection
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
    -  **<min_interval>**: minimum connecting interval. It should be less than or equal to the value of ``<max_interval>``. The range of this parameter is [0x0006,0x0C80]. The actual connecting interval equals this parameter multiplied by ``1.25 ms``, so the range for the actual minimum interval is [7.5,4000] ms.
    -  **<max_interval>**: maximum connecting interval. It should be more than or equal to the value of ``<min_interval>``. The range of this parameter is [0x0006,0x0C80]. The actual connecting interval equals this parameter multiplied by ``1.25 ms``, so the range for the actual maximum interval is [7.5,4000] ms.
    -  **<cur_interval>**: current connecting interval.
    -  **<latency>**: latency. Range: [0x0000,0x01F3].
    -  **<timeout>**: timeout. The range of this parameter is [0x000A,0x0C80]. The actual timeout equals this parameter multiplied by ``10 ms``, so the range for the actual timeout is [100,32000] ms.

    Note
    ^^^^^

    -  This command supports client or server roles  when updating its connection parameters. Of course, the connection has to be established first.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=1   // Role: client
        AT+BLECONN=0,"24:0a:c4:09:34:23"
        AT+BLECONNPARAM=0,12,14,1,500  

    .. _cmd-BDISC:

    :ref:`AT+BLEDISCONN <BLE-AT>`: End Bluetooth LE Connection
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

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=1   // Role: client
        AT+BLECONN=0,"24:0a:c4:09:34:23"
        AT+BLEDISCONN=0

.. only:: esp32 or esp32c3

    .. _cmd-BDLEN:

    :ref:`AT+BLEDATALEN <BLE-AT>`: Set Bluetooth LE Data Packet Length
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
    -  **<pkt_data_len>**: data packet's length. Range: [0x001B,0x00FB].

    Note
    ^^^^^

    -  The Bluetooth LE connection has to be established first.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=1   // Role: client
        AT+BLECONN=0,"24:0a:c4:09:34:23"
        AT+BLEDATALEN=0,30

.. only:: esp32 or esp32c3 or esp32c6 or esp32c2

    .. _cmd-BMTU:

    :ref:`AT+BLECFGMTU <BLE-AT>`: Set Bluetooth LE MTU Length
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

    .. only:: esp32 or esp32c3

        ::

            AT+BLECFGMTU=<conn_index>,<mtu_size>

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLECFGMTU=<conn_index>

    **Response:**

    ::

        OK  // The command is received.

    Parameters
    ^^^^^^^^^^

    .. only:: esp32 or esp32c3

        ::

            -  **<conn_index>**: index of Bluetooth LE connection. Range: [0,2].
            -  **<mtu_size>**: MTU length. Unit: byte. Range: [23,517].

    .. only:: esp32c2 or esp32c6

        ::

            -  **<conn_index>**: index of Bluetooth LE connection. Range: [0,1].

    Notes
    ^^^^^

    .. only:: esp32 or esp32c3

        ::

            -  Bluetooth LE connection has to be established first.
            -  Only the client can call this command to set the length of MTU.
            -  The actual length of MTU needs to be negotiated. The ``OK`` response only indicates an attempt to negotiate the length. The actual length may not be the value you set. Therefore, it is recommended to run command :ref:`AT+BLECFGMTU? <cmd-BMTU>` to query the actual length.

    .. only:: esp32c2 or esp32c6

        ::

            -  Bluetooth LE connection has to be established first.
            -  Only the client can call this command to set the length of MTU.

    Example
    ^^^^^^^^

    .. only:: esp32 or esp32c3

        ::

            AT+BLEINIT=1   // Role: client
            AT+BLECONN=0,"24:0a:c4:09:34:23"
            AT+BLECFGMTU=0,300

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLEINIT=1   // Role: client
            AT+BLECONN=0,"24:0a:c4:09:34:23"
            AT+BLECFGMTU=0

.. only:: esp32 or esp32c3

    .. _cmd-GSSRVCRE:

    :ref:`AT+BLEGATTSSRVCRE <BLE-AT>`: GATTS Creates Services
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

    -  If you are using an {IDF_TARGET_NAME} as a Bluetooth LE server, the ``mfg_nvs.bin`` with GATTS configuration should be downloaded into flash in order to provide services.
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

        AT+BLEINIT=2   // Role: server
        AT+BLEGATTSSRVCRE

    .. _cmd-GSSRVSTART:

    :ref:`AT+BLEGATTSSRVSTART <BLE-AT>`: GATTS Starts Services
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

        AT+BLEINIT=2   // Role: server
        AT+BLEGATTSSRVCRE
        AT+BLEGATTSSRVSTART

    .. _cmd-GSSRVSTOP:

    :ref:`AT+BLEGATTSSRVSTOP <BLE-AT>`: GATTS Stops Services
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

        AT+BLEINIT=2   // Role: server
        AT+BLEGATTSSRVCRE
        AT+BLEGATTSSRVSTART
        AT+BLEGATTSSRVSTOP

.. only:: esp32 or esp32c3 or esp32c6 or esp32c2

    .. _cmd-GSSRV:

    :ref:`AT+BLEGATTSSRV <BLE-AT>`: GATTS Discovers Services
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

    .. only:: esp32 or esp32c3

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEGATTSSRVCRE
            AT+BLEGATTSSRV?

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEGATTSSRV?

    .. _cmd-GSCHAR:

    :ref:`AT+BLEGATTSCHAR <BLE-AT>`: GATTS Discovers Characteristics
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

    .. only:: esp32 or esp32c3

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEGATTSSRVCRE
            AT+BLEGATTSSRVSTART
            AT+BLEGATTSCHAR?

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLEINIT=2   // Role: server
            AT+BLEGATTSCHAR?

    .. _cmd-GSNTFY:

    :ref:`AT+BLEGATTSNTFY <BLE-AT>`: Notify a Client of the Value of a Characteristic Value from the Server
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
    -  **<srv_index>**: service's index. It can be queried with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
    -  **<char_index>**: characteristic's index. It can be queried with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
    -  **<length>**: data length. The maximum length is ``( :ref:`MTU <cmd-BMTU>` - 3)``.

    Example
    ^^^^^^^^

    .. only:: esp32 or esp32c3

        ::

            AT+BLEINIT=2      // Role: server.
            AT+BLEGATTSSRVCRE
            AT+BLEGATTSSRVSTART
            AT+BLEADVSTART    // Start advertising. After the client is connected, it must be configured to receive notifications.
            AT+BLEGATTSCHAR?  // Query the characteristics which the client will be notified of.
            // For example, to notify of 4-byte data using the 6th characteristic in the 3rd service, use the following command:
            AT+BLEGATTSNTFY=0,3,6,4 
            // After the symbol ">" shows, enter the 4-byte data, such as "1234". Then the data will be transmitted automatically.

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLEINIT=2      // Role: server.
            AT+BLEADVPARAM=50,50,0,0,7,0,,
            AT+BLEADVSTART    // Start advertising. After the client is connected, it must be configured to receive notifications.
            AT+BLEGATTSCHAR?  // Query the characteristics which the client will be notified of.
            // For example, to notify of 4-byte data using the 6th characteristic in the 3rd service, use the following command:
            AT+BLEGATTSNTFY=0,3,6,4 
            // After the symbol ">" shows, enter the 4-byte data, such as "1234". Then the data will be transmitted automatically.

    .. _cmd-GSIND:

    :ref:`AT+BLEGATTSIND <BLE-AT>`: Indicate the Characteristic Value from the Server to a Client
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
    -  **<srv_index>**: service's index. It can be queried with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
    -  **<char_index>**: characteristic's index; it can be fetched with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
    -  **<length>**: data length. The maximum length is (:ref:`MTU <cmd-BMTU>` - 3).

    Example
    ^^^^^^^^

    .. only:: esp32 or esp32c3

        ::

            AT+BLEINIT=2      // Role: server
            AT+BLEGATTSSRVCRE
            AT+BLEGATTSSRVSTART
            AT+BLEADVSTART    // Start advertising. After the client is connected, it must be configured to receive indications.
            AT+BLEGATTSCHAR?  // Query the characteristics which the client can receive indications.
            // For example, to indicate 4 bytes of data using the 7th characteristic in the 3rd service, use the following command:
            AT+BLEGATTSIND=0,3,7,4 
            // After the symbol ">" shows, input 4 bytes of data, such as "1234". Then the data will be transmitted automatically.

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLEINIT=2      // Role: server
            AT+BLEADVPARAM=50,50,0,0,7,0,,
            AT+BLEADVSTART    // Start advertising. After the client is connected, it must be configured to receive indications.
            AT+BLEGATTSCHAR?  // Query the characteristics which the client can receive indications.
            // For example, to indicate 4 bytes of data using the 7th characteristic in the 3rd service, use the following command:
            AT+BLEGATTSIND=0,3,7,4 
            // After the symbol ">" shows, input 4 bytes of data, such as "1234". Then the data will be transmitted automatically.

    .. _cmd-GSSETA:

    :ref:`AT+BLEGATTSSETATTR <BLE-AT>`: GATTS Sets Characteristics
    ----------------------------------------------------------------------------------

    Set Command
    ^^^^^^^^^^^

    **Function:**

    GATTS sets its characteristic (descriptor).

    **Command:**

    .. only:: esp32 or esp32c3

        ::

            AT+BLEGATTSSETATTR=<srv_index>,<char_index>,[<desc_index>],<length>

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLEGATTSSETATTR=<srv_index>,<char_index>,<length>

    **Response:**

    ::

        >

    The symbol ``>`` indicates that AT is ready for receiving serial data and you can enter data now. When the requirement of data length determined by the parameter <length> is met, the setting starts.

    If the setting is successful, AT returns:

    ::

        OK

    Parameters
    ^^^^^^^^^^

    -  **<srv_index>**: service's index. It can be queried with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
    -  **<char_index>**: characteristic's index; it can be fetched with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.

    .. only:: esp32c2 or esp32c6

        -  **[<desc_index>]**: descriptor's index.

    -  If it is set, this command is used to set the value of the descriptor. 
    -  Otherwise, this command is used to set the value of the characteristic.

    -  **<length>**: data length.

    Note
    ^^^^^

    -  If the value of ``<length>`` is larger than the maximum length allowed, the setting will fail. The service table is defined in :project_file:`gatts_data.csv <components/customized_partitions/raw_data/ble_data/gatts_data.csv>`.

    .. only:: esp32c2 or esp32c6

        -  The characteristic descriptor value cannot be set.

    Example
    ^^^^^^^^

    .. only:: esp32 or esp32c3

        ::

            AT+BLEINIT=2   // Role: server.
            AT+BLEGATTSSRVCRE
            AT+BLEGATTSSRVSTART
            AT+BLEGATTSCHAR? 
            // For example, to set 1 byte of data of the 1st characteristic in the 1st service, use the following command:
            AT+BLEGATTSSETATTR=1,1,,1
            // After the symbol ">" shows, input 1 byte of data, such as "8". Then the setting starts.

    .. only:: esp32c2 or esp32c6

        ::

            AT+BLEINIT=2   // Role: server.
            AT+BLEGATTSCHAR? 
            // For example, to set 1 byte of data of the 1st characteristic in the 1st service, use the following command:
            AT+BLEGATTSSETATTR=1,1,1
            // After the symbol ">" shows, input 1 byte of data, such as "8". Then the setting starts.

    .. _cmd-GCPRIMSRV:

    :ref:`AT+BLEGATTCPRIMSRV <BLE-AT>`: GATTC Discovers Primary Services
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

        AT+BLEINIT=1   // Role: client
        AT+BLECONN=0,"24:12:5f:9d:91:98"
        AT+BLEGATTCPRIMSRV=0

    .. _cmd-GCINCLSRV:

    :ref:`AT+BLEGATTCINCLSRV <BLE-AT>`: GATTC Discovers Included Services
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
    -  **<srv_index>**: service's index. It can be queried with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index>.
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

        AT+BLEINIT=1   // Role: client
        AT+BLECONN=0,"24:12:5f:9d:91:98"
        AT+BLEGATTCPRIMSRV=0
        AT+BLEGATTCINCLSRV=0,1  // set a specific index according to the result of the previous command

    .. _cmd-GCCHAR:

    :ref:`AT+BLEGATTCCHAR <BLE-AT>`: GATTC Discovers Characteristics
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
    -  **<srv_index>**: service's index. It can be queried with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index>.
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

        AT+BLEINIT=1   // Role: client
        AT+BLECONN=0,"24:12:5f:9d:91:98"
        AT+BLEGATTCPRIMSRV=0
        AT+BLEGATTCCHAR=0,1 // set a specific index according to the result of the previous command

    .. _cmd-GCRD:

    :ref:`AT+BLEGATTCRD <BLE-AT>`: GATTC Reads Characteristics
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
    -  **<srv_index>**: service's index. It can be queried with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index>.
    -  **<char_index>**: characteristic's index; it can be fetched with command :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index>.
    -  **[<desc_index>]**: descriptor's index.

    -  If it is set, the value of the target descriptor will be read.
    -  if it is not set, the value of the target characteristic will be read.

    -  **<len>**: data length.
    -  **<value>**: ``<char_value>`` or ``<desc_value>``.

    -  **<char_value>**: characteristic's value. String format is read by command :ref:`AT+BLEGATTCRD <cmd-GCRD>`\=<conn_index>,<srv_index>,<char_index>. For example, if the response is ``+BLEGATTCRD:0,1,0``, it means that the value length is 1, and the content is "0".
    -  **<desc_value>**: descriptor's value. String format is read by command :ref:`AT+BLEGATTCRD <cmd-GCRD>`\=<conn_index>,<srv_index>,<char_index>,<desc_index>. For example, if the response is ``+BLEGATTCRD:0,4,0123``, it means that the value length is 4, and the content is "0123".

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

    :ref:`AT+BLEGATTCWR <BLE-AT>`: GATTC Writes Characteristics
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
    -  **<srv_index>**: service's index. It can be queried with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index>.
    -  **<char_index>**: characteristic's index; it can be fetched with command :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index>.
    -  **[<desc_index>]**: descriptor's index.

    -  If it is set, the value of the target descriptor will be written.
    -  If it is not set, the value of the target characteristic will be written.

    -  **<length>**: data length. The value range of this parameter is subject to :project_file:`gatts_data.csv <components/customized_partitions/raw_data/ble_data/gatts_data.csv>` in ``val_max_len`` parameter.

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

    :ref:`AT+BLESPPCFG <BLE-AT>`: Query/Set Bluetooth LE SPP Parameters
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

        +BLESPPCFG:<tx_service_index>,<tx_char_index>,<rx_service_index>,<rx_char_index>,<auto_conn>
        OK

    Set Command
    ^^^^^^^^^^^

    **Function:**

    Set or reset the parameters of Bluetooth LE SPP.

    **Command:**

    ::

        AT+BLESPPCFG=<cfg_enable>[,<tx_service_index>,<tx_char_index>,<rx_service_index>,<rx_char_index>][,<auto_conn>]

    **Response:**

    ::

        OK

    Parameters
    ^^^^^^^^^^

    -  **<cfg_enable>**:

    -  0: all the SPP parameters will be reset, and the following parameters do not need input.
    -  1: you should input the following parameters.

    -  **<tx_service_index>**: tx service's index. It can be queried with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> and :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>`.
    -  **<tx_char_index>**: tx characteristic's index. It can be queried with command :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> and :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
    -  **<rx_service_index>**: rx service's index. It can be queried with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> and :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>`.
    -  **<rx_char_index>**: rx characteristic's index. It can be queried with command :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> and :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>`.
    -  **<auto_conn>**: Bluetooth LE SPP auto-reconnection flag. By default, automatic reconnection is enabled.

    -  0: disable Bluetooth LE SPP automatic reconnection.
    -  1: enable Bluetooth LE SPP automatic reconnection

    Notes
    ^^^^^

    -  In Bluetooth LE client, the property of tx characteristic must be ``write with response`` or ``write without response``, and the property of rx characteristic must be ``indicate`` or ``notify``.
    -  In Bluetooth LE server, the property of tx characteristic must be ``indicate`` or ``notify``, and the property of rx characteristic must be ``write with response`` or ``write without response``.
    -  If the automatic reconnection function is disabled, when the connection is disconnected, a disconnection message is displayed (depending on AT+SYSMSG), you need to send the connection command again; If this function is enabled, the connection will be automatically reconnected after disconnection, and the MCU side will not be aware of the disconnection. If the MAC of the peer end changes, the connection will fail.

    Example
    ^^^^^^^^

    ::

        AT+BLESPPCFG=0          // reset Bluetooth LE SPP parameters
        AT+BLESPPCFG=1,3,5,3,7  // set Bluetooth LE SPP parameters
        AT+BLESPPCFG?           // query Bluetooth LE SPP parameters 

    .. _cmd-BLESPP:

    :ref:`AT+BLESPP <BLE-AT>`: Enter Bluetooth LE SPP Mode
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

        OK

        >

    This response indicates that AT has entered Bluetooth LE SPP mode and can send and receive data.

    If the Bluetooth LE SPP status is wrong ( Notifications are not enabled at the opposite end after the Bluetooth LE connection is established ), the system returns:

    ::

        ERROR

    Notes
    ^^^^^

    -  During the SPP transmission, AT will not prompt any exit the Bluetooth LE SPP passthrough mode information unless Bit0 of :ref:`AT+SYSMSG <cmd-SYSMSG>` is 1.
    -  During the SPP transmission, AT will not prompt any connection status changes unless Bit2 of :ref:`AT+SYSMSG <cmd-SYSMSG>` is 1.
    -  When the packet which only contains +++ is received, the device returns to normal command mode. Please wait for at least one second before sending the next AT command.

    Example
    ^^^^^^^^

    ::

        AT+BLESPP   // enter Bluetooth LE SPP mode

    .. _cmd-BLESMPPAR:

    :ref:`AT+BLESECPARAM <BLE-AT>`: Query/Set Bluetooth LE Encryption Parameters
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

        +BLESECPARAM:<auth_req>,<iocap>,<enc_key_size>,<init_key>,<rsp_key>,<auth_option>
        OK

    Set Command
    ^^^^^^^^^^^

    **Function:**

    Set the parameters of Bluetooth LE SMP.

    **Command:**

    ::

        AT+BLESECPARAM=<auth_req>,<iocap>,<enc_key_size>,<init_key>,<rsp_key>[,<auth_option>]

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

    -  **<enc_key_size>**: encryption key size. Range: [7,16]. Unit: byte.
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

    :ref:`AT+BLEENC <BLE-AT>`: Initiate Bluetooth LE Encryption Request
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

    .. only:: esp32 or esp32c3

        ::

            AT+RESTORE
            AT+BLEINIT=2
            AT+BLEGATTSSRVCRE
            AT+BLEGATTSSRVSTART
            AT+BLEADDR?
            AT+BLESECPARAM=1,0,16,3,3
            AT+BLESETKEY=123456
            AT+BLEADVSTART
            // Use your Bluetooth LE debugging app as a client to establish a Bluetooth LE connection with the {IDF_TARGET_NAME}
            AT+BLEENC=0,3

    .. only:: esp32c2 or esp32c6

        ::

            AT+RESTORE
            AT+BLEINIT=2
            AT+BLEADDR?
            AT+BLESECPARAM=1,0,16,3,3
            AT+BLESETKEY=123456
            AT+BLEADVPARAM=50,50,0,0,7,0,,
            AT+BLEADVSTART
            // Use your Bluetooth LE debugging app as a client to establish a Bluetooth LE connection with the {IDF_TARGET_NAME}
            AT+BLEENC=0,3

.. only:: esp32 or esp32c3

    .. _cmd-BLEENCRSP:

    :ref:`AT+BLEENCRSP <BLE-AT>`: Respond to the Pairing Request from the Peer Device
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

.. only:: esp32 or esp32c3 or esp32c6 or esp32c2

    .. _cmd-BLEKEYREPLY:

    :ref:`AT+BLEKEYREPLY <BLE-AT>`: Reply the Key Value to the Peer Device
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

    :ref:`AT+BLECONFREPLY <BLE-AT>`: Reply the Confirm Value to the Peer Device in the Legacy Connection Stage
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

    :ref:`AT+BLEENCDEV <BLE-AT>`: Query Bonded Bluetooth LE Encryption Device List
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

    -  **<enc_dev_index>**: index of the bonded devices. This parameter is not necessarily equal to the ``conn_index`` parameter in the Bluetooth LE connection list queried by the command :ref:`AT+BLECONN <cmd-BCONN>`. Range: [0,14].
    -  **<mac_address>**: MAC address.

    Note
    ^^^^^

    -  ESP-AT allows a maximum of ``15`` devices to be bonded. If the number of bonded devices exceeds 15, the newly bonded device information will sequentially (from 0 to 14) overwrite the previous device information according to the binding order.

    Example
    ^^^^^^^^

    ::

        AT+BLEENCDEV?

    .. _cmd-BLEENCCLEAR:

    :ref:`AT+BLEENCCLEAR <BLE-AT>`: Clear Bluetooth LE Encryption Device List
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

    :ref:`AT+BLESETKEY <BLE-AT>`: Set Bluetooth LE Static Pair Key
    ----------------------------------------------------------------------------------

    Query Command
    ^^^^^^^^^^^^^

    **Function:**

    .. only:: esp32 or esp32c3 or esp32c6

        ::

            Query the Bluetooth LE static pair key. If it is not set, AT will return -1.

    .. only:: esp32c2

        ::

            Query the Bluetooth LE static pair key.

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

.. only:: esp32 or esp32c3 

    .. _cmd-BLEHIDINIT:

    :ref:`AT+BLEHIDINIT <BLE-AT>`: Bluetooth LE HID Profile Initialization
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

    :ref:`AT+BLEHIDKB <BLE-AT>`: Send Bluetooth LE HID Keyboard Information
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
    - To use this command to interact with iOS products, your devices need to pass `MFI <https://mfi.apple.com/>`_ certification first.

    Example
    ^^^^^^^^

    ::

        AT+BLEHIDKB=0,4,0,0,0,0,0   // input the string "a"

    .. _cmd-BLEHIDMUS:

    :ref:`AT+BLEHIDMUS <BLE-AT>`: Send Bluetooth LE HID Mouse Information
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

    Note
    ^^^^

    - For more information about HID mouse, please refer to the section Generic Desktop Page, and Application Usages of `Universal Serial Bus HID Usage Tables <https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf>`_.
    - To use this command to interact with iOS products, your devices need to pass `MFI <https://mfi.apple.com/>`_ certification first.

    Example
    ^^^^^^^^

    ::

        AT+BLEHIDMUS=0,10,10,0

    .. _cmd-BLEHIDC:

    :ref:`AT+BLEHIDCONSUMER <BLE-AT>`: Send Bluetooth LE HID Consumer Information
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

    Note
    ^^^^

    - To use this command to interact with iOS products, your devices need to pass `MFI <https://mfi.apple.com/>`_ certification first.

    Example
    ^^^^^^^^

    ::

        AT+BLEHIDCONSUMER=233   // volume up

.. _cmd-BLUFI:

:ref:`AT+BLUFI <BLE-AT>`: Start or Stop BluFi
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

Note
^^^^

- You can only start or stop BluFi when Bluetooth LE is not initialized (:ref:`AT+BLEINIT=0 <cmd-BINIT>`).
- To achieve better performance, it is recommended to disable SoftAP by sending the :ref:`AT+CWMODE=0/1 <cmd-MODE>` command before using BluFi function. For more details, please refer to the `RF Coexistence <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/coexist.html>`_ documentation.
- After BluFi network configuration is completed, please send the :ref:`AT+BLUFI=0 <cmd-BLUFI>` command to disable BluFi and release resources.

Example
^^^^^^^^

::

    AT+BLUFI=1

.. _cmd-BLUFINAME:

:ref:`AT+BLUFINAME <BLE-AT>`: Query/Set BluFi Device Name
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
-  The maximum length of BluFi name is 26 bytes.
-  The Blufi APP can be downloaded from the app store.

Example
^^^^^^^^

::

    AT+BLUFINAME="BLUFI_DEV"
    AT+BLUFINAME?

.. _cmd-BLUFISEND:

:ref:`AT+BLUFISEND <BLE-AT>`: Send User-Customized Data over BluFi
-----------------------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**
 
Send user-customized data from the ESP side to a phone over BluFi.

**Command:**

::

    AT+BLUFISEND=<length>

**Response:**

::

    >

The symbol ``>`` indicates that AT is ready for receiving serial data and you can enter data now. When the requirement of data length determined by the parameter <length> is met, the transmission starts.

If the data transmission is successful, AT returns:

::

   OK

Parameters
^^^^^^^^^^

-  **<length>**: customized data length. Unit: byte.

Notes
^^^^^

-  The length of customized data cannot exceed 600 bytes.
-  If your ESP chip receives customized data from the phone, the data will be printed in the form of ``+BLUFIDATA:<len>,<data>``.

Example
^^^^^^^^

::

    AT+BLUFISEND=4 
    // After the symbol ">" shows, input 4 bytes of data, such as "1234". Then the data will be transmitted automatically.

.. only:: esp32c3 or esp32c6 or esp32c2

    .. _cmd-BLEPADATA:

    :ref:`AT+BLEPERIODICDATA <BLE-AT>`: Set Bluetooth LE periodic advertising data
    ------------------------------------------------------------------------------------

    Set Command
    ^^^^^^^^^^^

    **Function:**

    Set periodic advertising data.

    **Command:**

    ::

        AT+BLEPERIODICDATA=<periodic_data>

    **Response:**

    ::

        OK

    Parameter
    ^^^^^^^^^^

    -  **<periodic_data>**: Periodic advertising data in HEX string. For example, to set the periodic advertising data to "0x11 0x22 0x33 0x44 0x55", the command should be ``AT+BLEPERIODICDATA="1122334455"``.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=2   // Role: server
        AT+BLEPERIODICDATA="1122334455"

    .. _cmd-BLEPASTART:

    :ref:`AT+BLEPERIODICSTART <BLE-AT>`: Start Bluetooth LE periodic advertising
    ----------------------------------------------------------------------------------

    Execute Command
    ^^^^^^^^^^^^^^^

    **Function:**

    Start periodic advertising.

    **Command:**

    ::

        AT+BLEPERIODICSTART

    **Response:**

    ::

        OK

    Notes
    ^^^^^

    -  The extended advertising should be started before the start of periodic advertising, and the adv type should be ADV_TYPE_EXT_NOSCANNABLE_IND.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=2   // Role: server
        AT+BLEPERIODICDATA="1122334455" // Set periodic adv data
        AT+BLEADVPARAM=32,32,5,0,7,0   // Set extended advertising parameters
        AT+BLEADVSTART  // Start Extend advertising
        AT+BLEPERIODICSTART  // Start periodic advertising

    .. _cmd-BLEPASTOP:

    :ref:`AT+BLEPERIODICSTOP <BLE-AT>`: Stop Bluetooth LE periodic advertising
    --------------------------------------------------------------------------------

    Execute Command
    ^^^^^^^^^^^^^^^

    **Function:**

    Stop periodic advertising.

    **Command:**

    ::

        AT+BLEPERIODICSTOP

    **Response:**

    ::

        OK

    Example
    ^^^^^^^^

    ::

        AT+BLEPERIODICSTOP   // Stop periodic advertising

    .. _cmd-BLESYNCSTART:

    :ref:`AT+BLESYNCSTART <BLE-AT>`: Start to synchronize with periodic advertising
    ---------------------------------------------------------------------------------

    Set Command
    ^^^^^^^^^^^

    **Function:**

    Synchroize with the target device and get the periodic advertising data.

    **Command:**

    ::

        AT+BLESYNCSTART=<target_address>

    **Response:**

    ::

        +BLESYNC:<addr>,<rssi>,<periodic_adv_data>
        OK

    Parameters
    ^^^^^^^^^^

    -  **<addr>**: Bluetooth LE address.
    -  **<rssi>**: signal strength.
    -  **<periodic_adv_data>**: periodic advertising data.

    Notes
    ^^^^^

    -  Before you start to synchronize with the remote device, the blescan must be start.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=1   // Role: client
        AT+BLESCAN=1   // start Bluetooth LE scan
        AT+BLESYNCSTART="24:0a:c4:09:34:23"  // try to Synchroize with the remote device

    .. _cmd-BLESYNCSTOP:

    :ref:`AT+BLESYNCSTOP <BLE-AT>`: Stop synchronizing with periodic advertising
    ---------------------------------------------------------------------------------

    Execute Command
    ^^^^^^^^^^^^^^^

    **Function:**

    Stop synchronizing with the target device.

    **Command:**

    ::

        AT+BLESYNCSTOP

    **Response:**

    ::

        OK


    Notes
    ^^^^^

    -  If you stop blescan with ``AT+BLESCAN=0`` command, the synchronization will be stopped automatically.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=1   // Role: client
        AT+BLESCAN=1   // start Bluetooth LE scan
        AT+BLESYNCSTART="24:0a:c4:09:34:23"  // try to synchronize with the remote device
        AT+BLESYNCSTOP  // Stop Synchroize with the remote device

    .. _cmd-BLERDPHY:

    :ref:`AT+BLEREADPHY <BLE-AT>`: Query the current transmitter PHY
    -----------------------------------------------------------------------------

    Set Command
    ^^^^^^^^^^^

    **Function:**

    Query the current transmitter PHY.

    **Command:**

    ::

        AT+BLEREADPHY=<conn_index>

    **Response:**

    If the query succeeds, it will return:

    ::

        +BLEREADPHY:<device_addr>,<tx_PHY>,<rx_PHY>
        OK

    If the query fails, it will return:

    ::

        +BLEREADPHY:-1
        OK

    Parameters
    ^^^^^^^^^^

    -  **<device_addr>**: remote device address.

    -  **<tx_PHY>**:

    -  1: 1M PHY.
    -  2: 2M PHY.
    -  3: Coded PHY.

    -  **<rx_PHY>**:

    -  1: 1M PHY.
    -  2: 2M PHY.
    -  3: Coded PHY.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=1   // Role: client
        AT+BLECONN=0,"24:0a:c4:09:34:23"
        AT+BLEREADPHY=0 // query current transmitter PHY

    .. _cmd-BLESETPHY:

    :ref:`AT+BLESETPHY <BLE-AT>`: Set the current transmitter PHY
    -----------------------------------------------------------------------------

    Set Command
    ^^^^^^^^^^^

    **Function:**

    Set the current transmitter PHY.

    **Command:**

    ::

        AT+BLESETPHY=<conn_index>,<tx_rx_phy>

    **Response:**

    If the query succeeds, it will return:

    ::

        +BLESETPHY:<device_addr>,<tx_PHY>,<rx_PHY>
        OK

    If the query fails, it will return:

    ::

        +BLESETPHY:-1
        OK

    Parameters
    ^^^^^^^^^^

    -  **<device_addr>**: remote device address.

    -  **<tx_rx_phy>**:

    -  1: 1M PHY.
    -  2: 2M PHY.
    -  3: Coded PHY.

    Example
    ^^^^^^^^

    ::

        AT+BLEINIT=1   // Role: client
        AT+BLECONN=0,"24:0a:c4:09:34:23"
        AT+BLESETPHY=0,1
