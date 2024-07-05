.. _WiFi-AT:

Wi-Fi AT Commands
=================

:link_to_translation:`zh_CN:[中文]`

-  :ref:`Introduction <cmd-wifi-intro>`
-  :ref:`AT+CWINIT <cmd-INIT>`: Initialize/Deinitialize Wi-Fi driver.
-  :ref:`AT+CWMODE <cmd-MODE>`: Set the Wi-Fi mode (Station/SoftAP/Station+SoftAP).
-  :ref:`AT+CWSTATE <cmd-WSTATE>`: Query the Wi-Fi state and Wi-Fi information.
-  :ref:`AT+CWJAP <cmd-JAP>`: Connect to an AP.
-  :ref:`AT+CWRECONNCFG <cmd-RECONNCFG>`: Query/Set the Wi-Fi reconnecting configuration.
-  :ref:`AT+CWLAPOPT <cmd-LAPOPT>`: Set the configuration for the command :ref:`AT+CWLAP <cmd-LAP>`.
-  :ref:`AT+CWLAP <cmd-LAP>`: List available APs.
-  :ref:`AT+CWQAP <cmd-QAP>`: Disconnect from an AP.
-  :ref:`AT+CWSAP <cmd-SAP>`: Query/Set the configuration of an {IDF_TARGET_NAME} SoftAP.
-  :ref:`AT+CWLIF <cmd-LIF>`: Obtain IP address of the station that connects to an {IDF_TARGET_NAME} SoftAP.
-  :ref:`AT+CWQIF <cmd-QIF>`: Disconnect stations from an {IDF_TARGET_NAME} SoftAP.
-  :ref:`AT+CWDHCP <cmd-DHCP>`: Enable/disable DHCP.
-  :ref:`AT+CWDHCPS <cmd-DHCPS>`: Query/Set the IPv4 addresses allocated by an {IDF_TARGET_NAME} SoftAP DHCP server.
-  :ref:`AT+CWAUTOCONN <cmd-AUTOC>`: Connect to an AP automatically when powered on.
-  :ref:`AT+CWAPPROTO <cmd-APPROTO>`: Query/Set the 802.11 b/g/n protocol standard of SoftAP mode.
-  :ref:`AT+CWSTAPROTO <cmd-STAPROTO>`: Query/Set the 802.11 b/g/n protocol standard of station mode.
-  :ref:`AT+CIPSTAMAC <cmd-STAMAC>`: Query/Set the MAC address of an {IDF_TARGET_NAME} station.
-  :ref:`AT+CIPAPMAC <cmd-APMAC>`: Query/Set the MAC address of an {IDF_TARGET_NAME} SoftAP.
-  :ref:`AT+CIPSTA <cmd-IPSTA>`: Query/Set the IP address of an {IDF_TARGET_NAME} station.
-  :ref:`AT+CIPAP <cmd-IPAP>`: Query/Set the IP address of an {IDF_TARGET_NAME} SoftAP.
-  :ref:`AT+CWSTARTSMART <cmd-STARTS>`: Start SmartConfig.
-  :ref:`AT+CWSTOPSMART <cmd-STOPS>`: Stop SmartConfig.
-  :ref:`AT+WPS <cmd-WPS>`: Enable the WPS function.
-  :ref:`AT+CWJEAP <cmd-JEAP>`: Connect to a WPA2 Enterprise AP.
-  :ref:`AT+CWHOSTNAME <cmd-HOSTNAME>`: Query/Set the host name of an {IDF_TARGET_NAME} station.
-  :ref:`AT+CWCOUNTRY <cmd-COUNTRY>`: Query/Set the Wi-Fi Country Code.

.. _cmd-wifi-intro:

Introduction
------------

.. important::
  The default AT firmware supports all the AT commands except :ref:`AT+CWJEAP <cmd-JEAP>` mentioned on this page. If you need to modify the commands supported by {IDF_TARGET_NAME} by default, please compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections (Each item below is independent. Choose it according to your needs):

  - Enable EAP commands (:ref:`AT+CWJEAP <cmd-JEAP>`): ``Component config`` -> ``AT`` -> ``AT WPA2 Enterprise command support``
  - Disable WPS commands (:ref:`AT+WPS <cmd-WPS>`): ``Component config`` -> ``AT`` -> ``AT WPS command support``
  - Disable smartconfig commands (:ref:`AT+CWSTARTSMART <cmd-STARTS>` and :ref:`AT+CWSTOPSMART <cmd-STOPS>`): ``Component config`` -> ``AT`` -> ``AT smartconfig command support``
  - Disable all Wi-Fi commands (Not recommended. Once disabled, all Wi-Fi and above functions will be unusable, and you will need to implement these AT commands yourself): ``Component config`` -> ``AT`` -> ``AT wifi command support``

.. _cmd-INIT:

:ref:`AT+CWINIT <WiFi-AT>`: Initialize or Deinitialize Wi-Fi Driver
--------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the Wi-Fi initialization status of {IDF_TARGET_NAME} device.

**Command:**

::

    AT+CWINIT?

**Response:**

::

    +CWINIT:<init>

    OK

Set Command
^^^^^^^^^^^

**Function:**

Initialize or deinitialize Wi-Fi driver of {IDF_TARGET_NAME} device.

**Command:**

::

    AT+CWINIT=<init>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<init>**:

   -  0: Deinitialize Wi-Fi driver of {IDF_TARGET_NAME} device.
   -  1: Initialize Wi-Fi driver of {IDF_TARGET_NAME} device. (Default value)

Note
^^^^^

- This setting is not saved to flash and will revert to the default value of 1 after restarting.
- When you run out of RAM resources and Wi-Fi is not used, you can use this command to clean up the Wi-Fi driver to free up RAM resources.

Example
^^^^^^^^

::

    // Deinitialize Wi-Fi driver
    AT+CWINIT=0

.. _cmd-MODE:

:ref:`AT+CWMODE <WiFi-AT>`: Query/Set the Wi-Fi Mode (Station/SoftAP/Station+SoftAP)
-------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the Wi-Fi mode of {IDF_TARGET_NAME}.

**Command:**

::

    AT+CWMODE?

**Response:**

::

    +CWMODE:<mode>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the Wi-Fi mode of {IDF_TARGET_NAME}.

**Command:**

::

    AT+CWMODE=<mode>[,<auto_connect>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<mode>**:

   -  0: Null mode. Wi-Fi RF will be disabled.
   -  1: Station mode.
   -  2: SoftAP mode.
   -  3: SoftAP+Station mode.

-  **<auto_connect>**: Enable or disable automatic connection to an AP when you change the mode of the {IDF_TARGET_NAME} from the SoftAP mode or null mode to the station mode or the SoftAP+Station mode. Default: 1. If you omit the parameter, the default value will be used, i.e. automatically connecting to an AP. 

   -  0: The {IDF_TARGET_NAME} will not automatically connect to an AP.
   -  1: The {IDF_TARGET_NAME} will automatically connect to an AP if the configuration to connect to the AP has already been saved in flash before.

Note
^^^^^

- The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.

.. only:: esp32 or esp32c2 or esp32c3 or esp32c6

  - If you have previously used the Bluetooth function, it is recommended to send the following commands to deinitialize the initialized functions before using the SoftAP or SoftAP+Station functions:

    .. only:: esp32

        - :ref:`AT+BTINIT=0 <cmd-BTINIT>` (Deinitialize Classic Bluetooth)

    .. only:: esp32 or esp32c2 or esp32c3 or esp32c6

        - :ref:`AT+BLEINIT=0 <cmd-BINIT>` (Deinitialize Bluetooth LE)
        - :ref:`AT+BLUFI=0 <cmd-BLUFI>` (Disable BluFi)

    For more details, please refer to the `RF Coexistence <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/coexist.html>`_ documentation.

Example
^^^^^^^^

::

    AT+CWMODE=3 

.. _cmd-WSTATE:

:ref:`AT+CWSTATE <WiFi-AT>`: Query the Wi-Fi state and Wi-Fi information
------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the Wi-Fi state and Wi-Fi information of {IDF_TARGET_NAME}.

**Command:**

::

    AT+CWSTATE?

**Response:**

::

    +CWSTATE:<state>,<"ssid">

    OK

Parameters
^^^^^^^^^^

-  **<state>**: current Wi-Fi state.

   -  0: {IDF_TARGET_NAME} station has not started any Wi-Fi connection.
   -  1: {IDF_TARGET_NAME} station has connected to an AP, but does not get an IPv4 address yet.
   -  2: {IDF_TARGET_NAME} station has connected to an AP, and got an IPv4 address.
   -  3: {IDF_TARGET_NAME} station is in Wi-Fi connecting or reconnecting state.
   -  4: {IDF_TARGET_NAME} station is in Wi-Fi disconnected state.

-  **<"ssid">**: the SSID of the target AP.

Note
^^^^^

- When {IDF_TARGET_NAME} station is not connected to an AP, it is recommended to use this command to query Wi-Fi information; after {IDF_TARGET_NAME} station is connected to an AP, it is recommended to use :ref:`AT+CWJAP <cmd-JAP>` to query Wi-Fi information.

.. _cmd-JAP:

:ref:`AT+CWJAP <WiFi-AT>`: Connect to an AP
------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the AP to which the {IDF_TARGET_NAME} Station is already connected.

**Command:**

::

    AT+CWJAP?

**Response:**

::

    +CWJAP:<ssid>,<bssid>,<channel>,<rssi>,<pci_en>,<reconn_interval>,<listen_interval>,<scan_mode>,<pmf>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Connect an {IDF_TARGET_NAME} station to a targeted AP.

**Command:**

::

    AT+CWJAP=[<ssid>],[<pwd>][,<bssid>][,<pci_en>][,<reconn_interval>][,<listen_interval>][,<scan_mode>][,<jap_timeout>][,<pmf>]

**Response:**

::

    WIFI CONNECTED
    WIFI GOT IP

    OK
    [WIFI GOT IPv6 LL]
    [WIFI GOT IPv6 GL]

or

::

    +CWJAP:<error code>
    ERROR

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Connect an {IDF_TARGET_NAME} station to a targeted AP with last Wi-Fi configuration.

**Command:**

::

    AT+CWJAP

**Response:**

::

    WIFI CONNECTED
    WIFI GOT IP

    OK
    [WIFI GOT IPv6 LL]
    [WIFI GOT IPv6 GL]

or

::

    +CWJAP:<error code>
    ERROR

Parameters
^^^^^^^^^^

-  **<ssid>**: the SSID of the target AP.

   -  Escape character syntax is needed if SSID or password contains special characters, such as ``,``, ``"``, or ``\``.
   -  Chinese SSID is supported. Chinese SSID of some routers or hotspots is not encoded in UTF-8 encoding format. You can scan SSID first, and then connect using the scanned SSID.

-  **<pwd>**: password, MAX: 63-byte ASCII.
-  **<bssid>**: the MAC address of the target AP. It cannot be omitted when multiple APs have the same SSID.
-  **<channel>**: channel.
-  **<rssi>**: signal strength.
-  **<pci_en>**: PCI Authentication.

   - 0: The {IDF_TARGET_NAME} station will connect APs with all encryption methods, including OPEN and WEP.
   - 1: The {IDF_TARGET_NAME} station will connect APs with all encryption methods, except OPEN and WEP.

-  **<reconn_interval>**: the interval between Wi-Fi reconnections. Unit: second. Default: 1. Maximum: 7200.

   -  0: The {IDF_TARGET_NAME} station will not reconnect to the AP when disconnected.
   -  [1,7200]: The {IDF_TARGET_NAME} station will reconnect to the AP at the specified interval when disconnected.

-  **<listen_interval>**: the interval of listening to the AP's beacon. Unit: AP beacon intervals. Default: 3. Range: [1,100].
-  **<scan_mode>**:

   -  0: fast scan. It will end after finding the targeted AP. The {IDF_TARGET_NAME} station will connect to the first scanned AP.
   -  1: all-channel scan. It will end after all the channels are scanned. The device will connect to the scanned AP with the strongest signal.

-  **<jap_timeout>**: maximum timeout for :ref:`AT+CWJAP <cmd-JAP>` command. Unit: second. Default: 15. Range: [3,600].
-  **<pmf>**: Protected Management Frames. Default: 1.

    - 0 means disable PMF.
    - bit 0: PMF capable, advertizes support for protected management frame. Device will prefer to connect in PMF mode if other device also advertizes PMF capability.
    - bit 1: PMF required, advertizes that protected management frame is required. Device will not associate to non-PMF capable devices.

-  **<error code>**: (for reference only)

   -  1: connection timeout.
   -  2: wrong password.
   -  3: cannot find the target AP.
   -  4: connection failed.
   -  others: unknown error occurred.

Notes
^^^^^

-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  This command requires Station mode to be enabled.
- After {IDF_TARGET_NAME} station is connected to an AP, it is recommended to use this command to query Wi-Fi information; when {IDF_TARGET_NAME} station is not connected to an AP, it is recommended to use :ref:`AT+CWSTATE <cmd-WSTATE>` to query Wi-Fi information.
-  The parameter ``<reconn_interval>`` of this command is the same as ``<interval_second>`` of the command :ref:`AT+CWRECONNCFG <cmd-RECONNCFG>`. Therefore, if you omit ``<reconn_interval>`` when running this command, the interval between Wi-Fi reconnections will use the default value 1.
-  If the ``<ssid>`` and ``<password>`` parameter are omitted, AT will use the last configuration.
-  Execute command has the same maximum timeout to setup command. The default value is 15 seconds, but you can change it by setting the parameter ``<jap_timeout>``.
-  The authentication method via `WAPI <https://en.wikipedia.org/wiki/WLAN_Authentication_and_Privacy_Infrastructure>`_ is not supported for connecting to the router.
-  To get an IPv6 address, you need to set :ref:`AT+CIPV6=1 <cmd-IPV6>`.
-  Response ``OK`` means that the IPv4 network is ready, but not the IPv6 network. At present, ESP-AT is mainly based on IPv4 network, supplemented by IPv6 network.
-  ``WIFI GOT IPv6 LL`` represents that the linklocal IPv6 address has been obtained. This address is calculated locally through EUI-64 and does not require the participation of the AP. Because of the parallel timing, this print may be before or after ``OK``.
-  ``WIFI GOT IPv6 GL`` represents that the global IPv6 address has been obtained. This address is combined by the prefix issued by AP and the suffix calculated internally, which requires the participation of the AP. Because of the parallel timing, this print may be before or after ``OK``, or it may not be printed because the AP does not support IPv6.

Example
^^^^^^^^

::

    // If the target AP's SSID is "abc" and the password is "0123456789", the command should be:
    AT+CWJAP="abc","0123456789"

    // If the target AP's SSID is "ab\,c" and the password is "0123456789"\", the command should be:
    AT+CWJAP="ab\\\,c","0123456789\"\\"

    // If multiple APs all have the SSID of "abc", the target AP can be found by BSSID:
    AT+CWJAP="abc","0123456789","ca:d7:19:d8:a6:44" 

    // If esp-at is required that connect to a AP by protected management frame, the command should be:
    AT+CWJAP="abc","0123456789",,,,,,,3

.. _cmd-RECONNCFG:

:ref:`AT+CWRECONNCFG <WiFi-AT>`: Query/Set the Wi-Fi Reconnecting Configuration
-------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the configuration of Wi-Fi reconnect.

**Command:**

::

    AT+CWRECONNCFG?

**Response:**

::

    +CWRECONNCFG:<interval_second>,<repeat_count>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the configuration of Wi-Fi reconnect.

**Command:**

::

    AT+CWRECONNCFG=<interval_second>,<repeat_count>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<interval_second>**: the interval between Wi-Fi reconnections. Unit: second. Default: 0. Maximum: 7200.

   -  0: The {IDF_TARGET_NAME} station will not reconnect to the AP when disconnected.
   -  [1,7200]: The {IDF_TARGET_NAME} station will reconnect to the AP at the specified interval when disconnected.

-  **<repeat_count>**: the number of attempts the {IDF_TARGET_NAME} makes to reconnect to the AP. This parameter only works when the parameter ``<interval_second>`` is not 0. Default: 0. Maximum: 1000.

   -  0: The {IDF_TARGET_NAME} station will always try to reconnect to AP.
   -  [1,1000]: The {IDF_TARGET_NAME} station will attempt to reconnect to AP for the specified times.

Example
^^^^^^^^

::

    // The {IDF_TARGET_NAME} station tries to reconnect to AP at the interval of one second for 100 times.
    AT+CWRECONNCFG=1,100

    // The {IDF_TARGET_NAME} station will not reconnect to AP when disconnected.
    AT+CWRECONNCFG=0,0

Notes
^^^^^

-  The parameter ``<interval_second>`` of this command is the same as the parameter ``[<reconn_interval>]`` of the command :ref:`AT+CWJAP <cmd-JAP>`.
-  This command works for passive disconnection from APs, Wi-Fi mode switch, and Wi-Fi auto connect after power on.

.. _cmd-LAPOPT:

:ref:`AT+CWLAPOPT <WiFi-AT>`: Set the Configuration for the Command :ref:`AT+CWLAP <cmd-LAP>`
----------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+CWLAPOPT=<reserved>,<print mask>[,<rssi filter>][,<authmode mask>]

**Response:**

::

    OK

or

::

    ERROR

Parameters
^^^^^^^^^^

-  **<reserved>**: reserved item.
-  **<print mask>**: determine whether the following parameters are shown in the result of :ref:`AT+CWLAP <cmd-LAP>`. Default: 0x7FF. If you set them to 1, it means showing the corresponding parameters; if you set them as 0, it means NOT showing the corresponding parameters.  

   -  bit 0: determine whether <ecn> will be shown.
   -  bit 1: determine whether <ssid> will be shown.
   -  bit 2: determine whether <rssi> will be shown.
   -  bit 3: determine whether <mac> will be shown.
   -  bit 4: determine whether <channel> will be shown.
   -  bit 5: determine whether <freq_offset> will be shown.
   -  bit 6: determine whether <freqcal_val> will be shown.
   -  bit 7: determine whether <pairwise_cipher> will be shown.
   -  bit 8: determine whether <group_cipher> will be shown.
   -  bit 9: determine whether <bgn> will be shown.
   -  bit 10: determine whether <wps> will be shown.

-  **[<rssi filter>]**: determine whether the result of the command :ref:`AT+CWLAP <cmd-LAP>` will be filtered according to ``rssi filter``. In other words, the result of the command will **NOT** show the APs whose signal strength is below ``rssi filter``. Unit: dBm. Default: -100. Range: [-100,40]. 
-  **[<authmode mask>]**: determine whether APs with the following authmodes are shown in the result of :ref:`AT+CWLAP <cmd-LAP>`. Default: 0xFFFF. If you set ``bit x`` to 1, the APs with the corresponding authmode will be shown. If you set ``bit x`` to 0, the APs with the corresponding authmode will NOT be shown;

   -  bit 0: determine whether APs with ``OPEN`` authmode will be shown.
   -  bit 1: determine whether APs with ``WEP`` authmode will be shown.
   -  bit 2: determine whether APs with ``WPA_PSK`` authmode will be shown.
   -  bit 3: determine whether APs with ``WPA2_PSK`` authmode will be shown.
   -  bit 4: determine whether APs with ``WPA_WPA2_PSK`` authmode will be shown.
   -  bit 5: determine whether APs with ``WPA2_ENTERPRISE`` authmode will be shown.
   -  bit 6: determine whether APs with ``WPA3_PSK`` authmode will be shown.
   -  bit 7: determine whether AP with ``WPA2_WPA3_PSK`` authmode will be shown.
   -  bit 8: determine whether AP with ``WAPI_PSK`` authmode will be shown.
   -  bit 9: determine whether AP with ``OWE`` authmode will be shown.

   .. only:: esp32c6

     -  bit 10: determine whether AP with ``WPA3_ENT_SUITE_B_192_BIT`` authmode will be shown.

Example
^^^^^^^^

::

    // The first parameter is 1, meaning that the result of the command AT+CWLAP will be ordered according to RSSI;
    // The second parameter is 31, namely 0x1F, meaning that the corresponding bits of <print mask> are set to 1. All parameters will be shown in the result of AT+CWLAP.
    AT+CWLAPOPT=1,31
    AT+CWLAP

    // Just show the AP which authmode is OPEN
    AT+CWLAPOPT=1,31,-100,1
    AT+CWLAP

.. _cmd-LAP:

:ref:`AT+CWLAP <WiFi-AT>`: List Available APs
------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Query the APs with specified parameters, such as the SSID, MAC address, or channel.

**Command:**

::

    AT+CWLAP=[<ssid>,<mac>,<channel>,<scan_type>,<scan_time_min>,<scan_time_max>]

Execute Command
^^^^^^^^^^^^^^^

**Function:**

List all available APs.

**Command:**

::

    AT+CWLAP

**Response:**

::

    +CWLAP:<ecn>,<ssid>,<rssi>,<mac>,<channel>,<freq_offset>,<freqcal_val>,<pairwise_cipher>,<group_cipher>,<bgn>,<wps>
    OK

Parameters
^^^^^^^^^^

-  **<ecn>**: encryption method.

   -  0: OPEN
   -  1: WEP
   -  2: WPA_PSK
   -  3: WPA2_PSK
   -  4: WPA_WPA2_PSK
   -  5: WPA2_ENTERPRISE
   -  6: WPA3_PSK
   -  7: WPA2_WPA3_PSK
   -  8: WAPI_PSK
   -  9: OWE

   .. only:: esp32c6

     -  10: WPA3_ENT_SUITE_B_192_BIT

-  **<ssid>**: string parameter showing SSID of the AP.
-  **<rssi>**: signal strength.
-  **<mac>**: string parameter showing MAC address of the AP.
-  **<channel>**: channel.
-  **<scan_type>**: Wi-Fi scan type. Default: 0.

   -  0: active scan
   -  1: passive scan

-  **<scan_time_min>**: the minimum active scan time per channel. Unit: millisecond. Range [0,1500]. If the scan type is passive, this parameter is invalid.
-  **<scan_time_max>**: the maximum active scan time per channel. Unit: millisecond. Range [0,1500]. If this parameter is 0, the firmware will use the default time: 120 ms for active scan; 360 ms for passive scan.
-  **<freq_offset>**: frequency offset (reserved item).
-  **<freqcal_val>**: frequency calibration value (reserved item).
-  **<pairwise_cipher>**: pairwise cipher type.

   -  0: None
   -  1: WEP40
   -  2: WEP104
   -  3: TKIP
   -  4: CCMP
   -  5: TKIP and CCMP
   -  6: AES-CMAC-128
   -  7: Unknown

-  **<group_cipher>**: group cipher type, same enumerated value to ``<pairwise_cipher>``.
-  **<bgn>**: 802.11 b/g/n. If the corresponding bit is 1, the corresponding mode is enabled; if the corresponding bit is 0, the corresponding mode is disabled.

   -  bit 0: bit to identify if 802.11b mode is enabled or not
   -  bit 1: bit to identify if 802.11g mode is enabled or not
   -  bit 2: bit to identify if 802.11n mode is enabled or not

-  **<wps>**: wps flag.

   - 0: WPS disabled
   - 1: WPS enabled

Example
^^^^^^^^

::

    AT+CWLAP="Wi-Fi","ca:d7:19:d8:a6:44",6,0,400,1000
    
    // Search for APs with a designated SSID: 
    AT+CWLAP="Wi-Fi"

.. _cmd-QAP:

:ref:`AT+CWQAP <WiFi-AT>`: Disconnect from an AP
------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+CWQAP

**Response:**

::

    OK

.. _cmd-SAP:

:ref:`AT+CWSAP <WiFi-AT>`: Query/Set the configuration of an {IDF_TARGET_NAME} SoftAP
-------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the configuration parameters of an {IDF_TARGET_NAME} SoftAP.

**Command:**

::

    AT+CWSAP?

**Response:**

::

    +CWSAP:<ssid>,<pwd>,<channel>,<ecn>,<max conn>,<ssid hidden>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the configuration of an {IDF_TARGET_NAME} SoftAP.

**Command:**

::

    AT+CWSAP=<ssid>,<pwd>,<chl>,<ecn>[,<max conn>][,<ssid hidden>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<ssid>**: string parameter showing SSID of the AP.
-  **<pwd>**: string parameter showing the password. Length: 8 ~ 63 bytes ASCII.
-  **<channel>**: channel ID.
-  **<ecn>**: encryption method; WEP is not supported.

   -  0: OPEN
   -  2: WPA_PSK
   -  3: WPA2_PSK
   -  4: WPA_WPA2_PSK

-  **[<max conn>]**: maximum number of stations that {IDF_TARGET_NAME} SoftAP can connect. Range: refer to `max_connection description <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#ap-basic-configuration>`_.
-  **[<ssid hidden>]**:

   -  0: broadcasting SSID (default).
   -  1: not broadcasting SSID.

Notes
^^^^^

-  This command works only when :ref:`AT+CWMODE=2 <cmd-MODE>` or :ref:`AT+CWMODE=3 <cmd-MODE>`.
-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  The default SSID varies from devices to device as it consists of the MAC address of the device. You can use :ref:`AT+CWSAP? <cmd-SAP>` to query the default SSID.

Example
^^^^^^^^

::

    AT+CWSAP="ESP","1234567890",5,3   

.. _cmd-LIF:

:ref:`AT+CWLIF <WiFi-AT>`: Obtain IP Address of the Station That Connects to an {IDF_TARGET_NAME} SoftAP
--------------------------------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+CWLIF

**Response:**

::

    +CWLIF:<ip addr>,<mac>

    OK

Parameters
^^^^^^^^^^

-  **<ip addr>**: IP address of the station that connects to the {IDF_TARGET_NAME} SoftAP.
-  **<mac>**: MAC address of the station that connects to the {IDF_TARGET_NAME} SoftAP.

Note
^^^^^

-  This command cannot get a static IP. It works only when DHCP of both the {IDF_TARGET_NAME} SoftAP and the connected station are enabled.

.. _cmd-QIF:

:ref:`AT+CWQIF <WiFi-AT>`: Disconnect Stations from an {IDF_TARGET_NAME} SoftAP
-------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Disconnect all stations that are connected to the {IDF_TARGET_NAME} SoftAP.

**Command:**

::

    AT+CWQIF

**Response:**

::

    OK

Set Command
^^^^^^^^^^^

**Function:**

Disconnect a specific station from the {IDF_TARGET_NAME} SoftAP.

**Command:**

::

    AT+CWQIF=<mac>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<mac>**: MAC address of the station to disconnect.

.. _cmd-DHCP:

:ref:`AT+CWDHCP <WiFi-AT>`: Enable/Disable DHCP
-----------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+CWDHCP?

**Response:**

::

  +CWDHCP:<state>
  OK

Set Command
^^^^^^^^^^^

**Function:**
 
Enable/disable DHCP.

**Command:**

::

    AT+CWDHCP=<operate>,<mode>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<operate>**:

   -  0: disable
   -  1: enable

-  **<mode>**:

   -  Bit0: Station DHCP
   -  Bit1: SoftAP DHCP

-  **<state>**: the status of DHCP
   
   - Bit0:

     - 0: Station DHCP is disabled.
     - 1: Station DHCP is enabled.
   
   - Bit1:

     - 0: SoftAP DHCP is disabled.
     - 1: SoftAP DHCP is enabled.

   - Bit2:

     - 0: Ethernet DHCP is disabled.
     - 1: Ethernet DHCP is enabled.

Notes
^^^^^
-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  This Set Command correlates with the commands that set static IP, such as :ref:`AT+CIPSTA <cmd-IPSTA>` and :ref:`AT+CIPAP <cmd-IPAP>`:

   -  If DHCP is enabled, static IPv4 address will be disabled;
   -  If static IPv4 address is enabled, DHCP will be disabled;
   -  The last configuration overwrites the previous configuration.

Example
^^^^^^^^

::

    // Enable Station DHCP. If the last DHCP mode is 2, the current DHCP mode will be 3.
    AT+CWDHCP=1,1

    // Disable SoftAP DHCP. If the last DHCP mode is 3, the current DHCP mode will be 1.    
    AT+CWDHCP=0,2

.. _cmd-DHCPS:

:ref:`AT+CWDHCPS <WiFi-AT>`: Query/Set the IPv4 Addresses Allocated by an {IDF_TARGET_NAME} SoftAP DHCP Server
------------------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+CWDHCPS?

**Response:**

::

    +CWDHCPS:<lease time>,<start IP>,<end IP>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the IPv4 address range of the {IDF_TARGET_NAME} SoftAP DHCP server.

**Command:**

::

    AT+CWDHCPS=<enable>,<lease time>,<start IP>,<end IP>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<enable>**:
   
   -  1: Enable DHCP server settings. The parameters below have to be set.
   -  0: Disable DHCP server settings and use the default IPv4 address range.

-  **<lease time>**: lease time. Unit: minute. Range [1,2880].
-  **<start IP>**: start IPv4 address of the IPv4 address range that can be obtained from {IDF_TARGET_NAME} SoftAP DHCP server.
-  **<end IP>**: end IPv4 address of the IPv4 address range that can be obtained from {IDF_TARGET_NAME} SoftAP DHCP server.

Notes
^^^^^

-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  This AT command works only when both SoftAP and DHCP server are enabled for {IDF_TARGET_NAME}.
-  The IPv4 address should be in the same network segment as the IPv4 address of {IDF_TARGET_NAME} SoftAP.

Example
^^^^^^^^

::

    AT+CWDHCPS=1,3,"192.168.4.10","192.168.4.15"
    
    AT+CWDHCPS=0 // Disable the settings and use the default IPv4 address range.

.. _cmd-AUTOC:

:ref:`AT+CWAUTOCONN <WiFi-AT>`: Query/Set Automatic Connection to an AP When Powered on
-----------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+CWAUTOCONN?

**Response:**

::

    +CWAUTOCONN:<enable>
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+CWAUTOCONN=<enable>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<enable>**:

   -  1: Enable automatic connection to an AP when powered on. (Default)   
   -  0: Disable automatic connection to an AP when powered on.

Note
^^^^^

-  The configuration changes will be saved in the NVS area.

Example
^^^^^^^^

::

    AT+CWAUTOCONN=1

.. _cmd-APPROTO:

:ref:`AT+CWAPPROTO <WiFi-AT>`: Query/Set the 802.11 b/g/n Protocol Standard of SoftAP Mode
-------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+CWAPPROTO?

**Response:**

::

    +CWAPPROTO:<protocol>
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+CWAPPROTO=<protocol>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<protocol>**:

   -  bit0: 802.11b protocol standard.
   -  bit1: 802.11g protocol standard.
   -  bit2: 802.11n protocol standard.

   .. only:: esp32 or esp32c3 or esp32c6

     - bit3: `802.11 LR Espressif-patented protocol standard <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#long-range-lr>`_.

   .. only:: esp32c6

     - bit4: 802.11ax protocol standard.

Note
^^^^^

-  See `Wi-Fi Protocol Mode <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#wi-fi-protocol-mode>`_ for the PHY mode supported by the {IDF_TARGET_NAME} device.

.. only:: esp32 or esp32c3 or esp32c2 or esp32s2

  -  By default, PHY mode of {IDF_TARGET_NAME} is 802.11bgn mode.

.. only:: esp32c6

  -  By default, PHY mode of {IDF_TARGET_NAME} is 802.11bgnax mode.

.. _cmd-STAPROTO:

:ref:`AT+CWSTAPROTO <WiFi-AT>`: Query/Set the 802.11 b/g/n Protocol Standard of Station Mode
--------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+CWSTAPROTO?

**Response:**

::

    +CWSTAPROTO:<protocol>
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+CWSTAPROTO=<protocol>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<protocol>**:

   -  bit0: 802.11b protocol standard.
   -  bit1: 802.11g protocol standard.
   -  bit2: 802.11n protocol standard.

   .. only:: esp32 or esp32c3 or esp32c6

     - bit3: `802.11 LR Espressif-patented protocol standard <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#long-range-lr>`_.

   .. only:: esp32c6

     - bit4: 802.11ax protocol standard.

Note
^^^^^

-  See `Wi-Fi Protocol Mode <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#wi-fi-protocol-mode>`_ for the PHY mode supported by the {IDF_TARGET_NAME} device.

.. only:: esp32 or esp32c3 or esp32c2 or esp32s2

  -  By default, PHY mode of {IDF_TARGET_NAME} is 802.11bgn mode.

.. only:: esp32c6

  -  By default, PHY mode of {IDF_TARGET_NAME} is 802.11bgnax mode.

.. _cmd-STAMAC:

:ref:`AT+CIPSTAMAC <WiFi-AT>`: Query/Set the MAC Address of an {IDF_TARGET_NAME} Station
----------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the MAC address of the {IDF_TARGET_NAME} Station.

**Command:**

::

    AT+CIPSTAMAC?

**Response:**

::

    +CIPSTAMAC:<mac>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the MAC address of an {IDF_TARGET_NAME} station.

**Command:**

::

    AT+CIPSTAMAC=<mac>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<mac>**: string parameter showing MAC address of an {IDF_TARGET_NAME} station.

Notes
^^^^^

.. list::

  - The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
  :esp32: - The MAC address of {IDF_TARGET_NAME} Station is different from that of the {IDF_TARGET_NAME} SoftAP and {IDF_TARGET_NAME} Ethernet. Please make sure that you do not set the same MAC address for both of them.
  :esp32c2 or esp32c3 or esp32c6: - The MAC address of {IDF_TARGET_NAME} Station is different from that of the {IDF_TARGET_NAME} SoftAP. Please make sure that you do not set the same MAC address for both of them.
  - Bit 0 of the {IDF_TARGET_NAME} MAC address CANNOT be 1. For example, a MAC address can be "1a:…" but not "15:…".
  - FF:FF:FF:FF:FF:FF and 00:00:00:00:00:00 are invalid MAC address and cannot be set.

Example
^^^^^^^^

::

    AT+CIPSTAMAC="1a:fe:35:98:d3:7b"

.. _cmd-APMAC:

:ref:`AT+CIPAPMAC <WiFi-AT>`: Query/Set the MAC Address of an {IDF_TARGET_NAME} SoftAP
--------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^
**Function:**

Query the MAC address of the {IDF_TARGET_NAME} SoftAP.

**Command:**

::

    AT+CIPAPMAC?

**Response:**

::

    +CIPAPMAC:<mac>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the MAC address of the {IDF_TARGET_NAME} SoftAP.

**Command:**

::

    AT+CIPAPMAC=<mac>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<mac>**: string parameter showing MAC address of the {IDF_TARGET_NAME} SoftAP.

Notes
^^^^^

.. list::

  - The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
  :esp32: - The MAC address of {IDF_TARGET_NAME} SoftAP is different from that of the {IDF_TARGET_NAME} station and {IDF_TARGET_NAME} Ethernet. Please make sure that you do not set the same MAC address for both of them.
  :esp32c2 or esp32c3 or esp32c6: - The MAC address of {IDF_TARGET_NAME} SoftAP is different from that of the {IDF_TARGET_NAME} station. Please make sure that you do not set the same MAC address for both of them.
  - Bit 0 of the {IDF_TARGET_NAME} MAC address CANNOT be 1. For example, a MAC address can be "18:…" but not "15:…".
  - FF:FF:FF:FF:FF:FF and 00:00:00:00:00:00 are invalid MAC and cannot be set.

Example
^^^^^^^^

::

    AT+CIPAPMAC="18:fe:35:98:d3:7b" 

.. _cmd-IPSTA:

:ref:`AT+CIPSTA <WiFi-AT>`: Query/Set the IP Address of an {IDF_TARGET_NAME} Station
------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the IP address of the {IDF_TARGET_NAME} Station.

**Command:**

::

    AT+CIPSTA?

**Response:**

::

    +CIPSTA:ip:<"ip">
    +CIPSTA:gateway:<"gateway">
    +CIPSTA:netmask:<"netmask">
    +CIPSTA:ip6ll:<"ipv6 addr">
    +CIPSTA:ip6gl:<"ipv6 addr">

    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the IPv4 address of the {IDF_TARGET_NAME} station.

**Command:**

::

    AT+CIPSTA=<"ip">[,<"gateway">,<"netmask">]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<"ip">**: string parameter showing the IPv4 address of the {IDF_TARGET_NAME} station.
-  **<"gateway">**: gateway.
-  **<"netmask">**: netmask.
-  **<"ipv6 addr">**: string parameter showing the IPv6 address of the {IDF_TARGET_NAME} station.

Notes
^^^^^

-  For the query command, only when the {IDF_TARGET_NAME} station is connected to an AP or the static IP address is configured can its IP address be queried.
-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  The Set Command correlates with the commands that set DHCP, such as :ref:`AT+CWDHCP <cmd-DHCP>`.

   -  If static IPv4 address is enabled, DHCP will be disabled;
   -  If DHCP is enabled, static IPv4 address will be disabled;
   -  The last configuration overwrites the previous configuration.

Example
^^^^^^^^

::

    AT+CIPSTA="192.168.6.100","192.168.6.1","255.255.255.0" 

.. _cmd-IPAP:

:ref:`AT+CIPAP <WiFi-AT>`: Query/Set the IP Address of an {IDF_TARGET_NAME} SoftAP
------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the IP address of the {IDF_TARGET_NAME} SoftAP.

**Command:**

::

    AT+CIPAP?

**Response:**

::

    +CIPAP:ip:<"ip">
    +CIPAP:gateway:<"gateway">
    +CIPAP:netmask:<"netmask">
    +CIPAP:ip6ll:<"ipv6 addr">

    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the IPv4 address of the {IDF_TARGET_NAME} SoftAP.

**Command:**

::

    AT+CIPAP=<"ip">[,<"gateway">,<"netmask">]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<"ip">**: string parameter showing the IPv4 address of the {IDF_TARGET_NAME} SoftAP.
-  **<"gateway">**: gateway.
-  **<"netmask">**: netmask.
-  **<"ipv6 addr">**: string parameter showing the IPv6 address of the {IDF_TARGET_NAME} SoftAP.

Notes
^^^^^

-  The set command is just applied to the IPv4 network, but not the IPv6 network.
-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  The set command correlates with the commands that set DHCP, such as :ref:`AT+CWDHCP <cmd-DHCP>`.

   -  If static IPv4 address is enabled, DHCP will be disabled;
   -  If DHCP is enabled, static IPv4 address will be disabled;
   -  The last configuration overwrites the previous configuration.

Example
^^^^^^^^

::

    AT+CIPAP="192.168.5.1","192.168.5.1","255.255.255.0"

.. _cmd-STARTS:

:ref:`AT+CWSTARTSMART <WiFi-AT>`: Start SmartConfig
--------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Start SmartConfig of the type ESP-TOUCH+AirKiss.

**Command:**

::

    AT+CWSTARTSMART

Set Command
^^^^^^^^^^^

**Function:**

Start SmartConfig of a designated type.

**Command:**

::

    AT+CWSTARTSMART=<type>[,<auth floor>][,<"esptouch v2 key">]

**Response:**

::

    OK  

Parameters
^^^^^^^^^^

-  **<type>**:

   -  1: ESP-TOUCH
   -  2: AirKiss
   -  3: ESP-TOUCH+AirKiss
   -  4: ESP-TOUCH v2

-  **<auth floor>**: Wi-Fi authentication mode floor. ESP-AT will not connect to the AP whose authmode is lower than this floor.

   -  0: OPEN (Default)
   -  1: WEP
   -  2: WPA_PSK
   -  3: WPA2_PSK
   -  4: WPA_WPA2_PSK
   -  5: WPA2_ENTERPRISE
   -  6: WPA3_PSK
   -  7: WPA2_WPA3_PSK

- **<"esptouch v2 key">**: ESP-TOUCH v2 decrypt key. It is used to decrypt Wi-Fi password and reserved data. Length: 16 bytes.

Notes
^^^^^

-  For more details on SmartConfig, please see `ESP-TOUCH User Guide <https://www.espressif.com/sites/default/files/documentation/esp-touch_user_guide_en.pdf>`_.
-  SmartConfig is only available in the {IDF_TARGET_NAME} station mode.
-  The message ``Smart get Wi-Fi info`` means that SmartConfig has successfully acquired the AP information. {IDF_TARGET_NAME} will try to connect to the target AP.
-  Message ``+SCRD:<length>,<rvd data>`` means that ESP-Touch v2 has successfully acquired the reserved data information.
-  Message ``Smartconfig connected Wi-Fi`` is printed if the connection is successful.
-  When AT returns ``Smartconfig connected Wi-Fi``, it is recommended to delay more than ``6`` seconds before executing :ref:`AT+CWSTOPSMART <cmd-STOPS>` because the {IDF_TARGET_NAME} needs to synchronize the SmartConfig results to the mobile phone.
-  Use command :ref:`AT+CWSTOPSMART <cmd-STOPS>` to stop SmartConfig before running other commands. Please make sure that you do not execute other commands during SmartConfig.

Example
^^^^^^^^

::

    AT+CWMODE=1
    AT+CWSTARTSMART

.. _cmd-STOPS:

:ref:`AT+CWSTOPSMART <WiFi-AT>`: Stop SmartConfig
------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+CWSTOPSMART

**Response:**

::

    OK

Note
^^^^^

-  Irrespective of whether SmartConfig succeeds or not, please always call :ref:`AT+CWSTOPSMART <cmd-STOPS>` before executing any other AT commands to release the internal memory taken up by SmartConfig.

Example
^^^^^^^^

::

    AT+CWMODE=1
    AT+CWSTARTSMART
    AT+CWSTOPSMART

.. _cmd-WPS:

:ref:`AT+WPS <WiFi-AT>`: Enable the WPS Function
-----------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+WPS=<enable>[,<auth floor>]

**Response:**

::

    OK 

Parameters
^^^^^^^^^^

-  **<enable>**:

   -  1: Enable WPS (Wi-Fi Protected Setup) that uses PBC (Push Button Configuration) mode.
   -  0: Disable WPS that uses PBC mode.

-  **<auth floor>**: Wi-Fi authentication mode floor. ESP-AT will not connect to the AP whose authmode is lower than this floor.

   -  0: OPEN (Default)
   -  1: WEP
   -  2: WPA_PSK
   -  3: WPA2_PSK
   -  4: WPA_WPA2_PSK
   -  5: WPA2_ENTERPRISE
   -  6: WPA3_PSK
   -  7: WPA2_WPA3_PSK

Notes
^^^^^

-  WPS can only be used when the {IDF_TARGET_NAME} station is enabled.
-  WPS does not support WEP (Wired-Equivalent Privacy) encryption.

Example
^^^^^^^^

::

    AT+CWMODE=1
    AT+WPS=1

.. _cmd-JEAP:

:ref:`AT+CWJEAP <WiFi-AT>`: Connect to a WPA2 Enterprise AP
-------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the configuration information of the Enterprise AP to which the {IDF_TARGET_NAME} station is already connected.

**Command:**

::

    AT+CWJEAP?

**Response:**

::

    +CWJEAP:<ssid>,<method>,<identity>,<username>,<password>,<security>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Connect to the targeted Enterprise AP.

**Command:**

::

    AT+CWJEAP=<ssid>,<method>,<identity>,<username>,<password>,<security>[,<jeap_timeout>]

**Response:**

::

    OK

or

::

  +CWJEAP:Timeout
  ERROR

Parameters
^^^^^^^^^^

-  **<ssid>**: the SSID of the Enterprise AP.

   -  Escape character syntax is needed if SSID or password contains any special characters, such as ``,``, ``"``, or ``\\``.

-  **<method>**: WPA2 Enterprise authentication method.

   -  0: EAP-TLS.
   -  1: EAP-PEAP.
   -  2: EAP-TTLS.

-  **<identity>**: identity for phase 1. String limited to 1 ~ 32.
-  **<username>**: username for phase 2. Range: 1 ~ 32 bytes. For the EAP-PEAP and EAP-TTLS method, you must set this parameter. For the EAP-TLS method, you do not need to.
-  **<password>**: password for phase 2. Range: 1 ~ 32 bytes. For the EAP-PEAP and EAP-TTLS method, you must set this parameter. For the EAP-TLS method, you do not need to.
-  **<security>**:

   -  Bit0: Client certificate.
   -  Bit1: Server certificate.

-  **[<jeap_timeout>]**: maximum timeout for :ref:`AT+CWJEAP <cmd-JEAP>` command. Unit: second. Default: 15. Range: [3,600].

Example
^^^^^^^^

::

    // Connect to EAP-TLS mode Enterprise AP, set identity, verify server certificate and load client certificate
    AT+CWJEAP="dlink11111",0,"example@espressif.com",,,3

    // Connect to EAP-PEAP mode Enterprise AP, set identity, username and password, not verify server certificate and not load client certificate
    AT+CWJEAP="dlink11111",1,"example@espressif.com","espressif","test11",0

**Error Code:**

The WPA2 Enterprise error code will be prompt as ``ERR CODE:0x<%08x>``.

.. list-table::
   :header-rows: 1
   :widths: 15 5

   * - AT_EAP_MALLOC_FAILED
     - 0x8001
   * - AT_EAP_GET_NVS_CONFIG_FAILED
     - 0x8002
   * - AT_EAP_CONN_FAILED
     - 0x8003
   * - AT_EAP_SET_WIFI_CONFIG_FAILED
     - 0x8004
   * - AT_EAP_SET_IDENTITY_FAILED
     - 0x8005
   * - AT_EAP_SET_USERNAME_FAILED
     - 0x8006
   * - AT_EAP_SET_PASSWORD_FAILED
     - 0x8007
   * - AT_EAP_GET_CA_LEN_FAILED
     - 0x8008
   * - AT_EAP_READ_CA_FAILED
     - 0x8009
   * - AT_EAP_SET_CA_FAILED
     - 0x800A
   * - AT_EAP_GET_CERT_LEN_FAILED
     - 0x800B
   * - AT_EAP_READ_CERT_FAILED
     - 0x800C
   * - AT_EAP_GET_KEY_LEN_FAILED
     - 0x800D
   * - AT_EAP_READ_KEY_FAILED
     - 0x800E
   * - AT_EAP_SET_CERT_KEY_FAILED
     - 0x800F
   * - AT_EAP_ENABLE_FAILED
     - 0x8010
   * - AT_EAP_ALREADY_CONNECTED
     - 0x8011
   * - AT_EAP_GET_SSID_FAILED
     - 0x8012
   * - AT_EAP_SSID_NULL
     - 0x8013
   * - AT_EAP_SSID_LEN_ERROR
     - 0x8014
   * - AT_EAP_GET_METHOD_FAILED
     - 0x8015
   * - AT_EAP_CONN_TIMEOUT
     - 0x8016
   * - AT_EAP_GET_IDENTITY_FAILED
     - 0x8017
   * - AT_EAP_IDENTITY_LEN_ERROR
     - 0x8018
   * - AT_EAP_GET_USERNAME_FAILED
     - 0x8019
   * - AT_EAP_USERNAME_LEN_ERROR
     - 0x801A
   * - AT_EAP_GET_PASSWORD_FAILED
     - 0x801B
   * - AT_EAP_PASSWORD_LEN_ERROR
     - 0x801C
   * - AT_EAP_GET_SECURITY_FAILED
     - 0x801D
   * - AT_EAP_SECURITY_ERROR
     - 0x801E
   * - AT_EAP_METHOD_SECURITY_UNMATCHED
     - 0x801F
   * - AT_EAP_PARAMETER_COUNTS_ERROR
     - 0x8020
   * - AT_EAP_GET_WIFI_MODE_ERROR
     - 0x8021
   * - AT_EAP_WIFI_MODE_NOT_STA
     - 0x8022
   * - AT_EAP_SET_CONFIG_FAILED
     - 0x8023
   * - AT_EAP_METHOD_ERROR
     - 0x8024

Note
^^^^^

-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  This command requires Station mode to be active.
-  TLS mode will use client certificate. Please make sure it is enabled.

.. _cmd-HOSTNAME:

:ref:`AT+CWHOSTNAME <WiFi-AT>`: Query/Set the Host Name of an {IDF_TARGET_NAME} Station
---------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the host name of {IDF_TARGET_NAME} Station.

**Command:**

::

    AT+CWHOSTNAME?

**Response:**

::

    +CWHOSTNAME:<hostname>

    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the host name of {IDF_TARGET_NAME} Station.

**Command:**

::

    AT+CWHOSTNAME=<hostname>

**Response:**

::

    OK

If the Station mode is not enabled, the command will return:

::

    ERROR

Parameters
^^^^^^^^^^

-  **<hostname>**: the host name of the {IDF_TARGET_NAME} Station. Maximum length: 32 bytes.

Note
^^^^^

-  The configuration changes are not saved in the flash.

Example
^^^^^^^^

::

    AT+CWMODE=3
    AT+CWHOSTNAME="my_test"

.. _cmd-COUNTRY:

:ref:`AT+CWCOUNTRY <WiFi-AT>`: Query/Set the Wi-Fi Country Code
--------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query Wi-Fi country code information.

**Command:**

::

    AT+CWCOUNTRY?

**Response:**

::

    +CWCOUNTRY:<country_policy>,<country_code>,<start_channel>,<total_channel_count>

    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the Wi-Fi country code information.

**Command:**

::

    AT+ CWCOUNTRY=<country_policy>,<country_code>,<start_channel>,<total_channel_count>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<country_policy>**:

   -  0: will change the county code to be the same as the AP that the {IDF_TARGET_NAME} is connected to.
   -  1: the country code will not change, always be the one set by command.

-  **<country_code>**: country code. Maximum length: 3 characters. Refer to `ISO 3166-1 alpha-2 <https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2>`_ for country codes.
-  **<start_channel>**: the channel number to start. Range: [1,14].
-  **<total_channel_count>**: total number of channels.

Note
^^^^^

-  See `Wi-Fi Country Code <https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-guides/wifi.html#wi-fi-country-code>`_ for more details.
-  The configuration changes are not saved in the flash.

Example
^^^^^^^^

::

    AT+CWMODE=3
    AT+CWCOUNTRY=1,"CN",1,13
