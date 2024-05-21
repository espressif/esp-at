.. _Basic-AT:

Basic AT Commands
=================

:link_to_translation:`zh_CN:[中文]`

.. list::

  - :ref:`Introduction <cmd-basic-intro>`
  - :ref:`AT <cmd-AT>`: Test AT startup.
  - :ref:`AT+RST <cmd-RST>`: Restart a module.
  - :ref:`AT+GMR <cmd-GMR>`: Check version information.
  - :ref:`AT+CMD <cmd-cmd>`: List all AT commands and types supported in current firmware.
  - :ref:`AT+GSLP <cmd-GSLP>`: Enter Deep-sleep mode.
  - :ref:`ATE <cmd-ATE>`: Configure AT commands echoing.
  - :ref:`AT+RESTORE <cmd-RESTORE>`: Restore factory default settings of the module.
  - :ref:`AT+SAVETRANSLINK <cmd-SAVET>`: Set whether to enter :term:`Passthrough Mode` on power-up.
  - :ref:`AT+TRANSINTVL <cmd-TRANSINTVL>`: Set the data transmission interval in the :term:`Passthrough Mode`.
  - :ref:`AT+UART_CUR <cmd-UARTC>`: Current UART configuration, not saved in flash.
  - :ref:`AT+UART_DEF <cmd-UARTD>`: Default UART configuration, saved in flash.
  - :ref:`AT+SLEEP <cmd-SLEEP>`: Set the sleep mode.
  - :ref:`AT+SYSRAM <cmd-SYSRAM>`: Query current remaining heap size and minimum heap size.
  - :ref:`AT+SYSMSG <cmd-SYSMSG>`: Query/Set System Prompt Information.
  - :ref:`AT+SYSMSGFILTER <cmd-SYSMSGFILTER>`: Enable or disable the :term:`system message` filter.
  - :ref:`AT+SYSMSGFILTERCFG <cmd-SYSMSGFILTERCFG>`: Query/Set the :term:`system message` filters.
  - :ref:`AT+SYSFLASH <cmd-SYSFLASH>`: Query/Set User Partitions in Flash.
  - :ref:`AT+SYSMFG <cmd-SYSMFG>`: Query/Set :term:`manufacturing nvs` User Partitions.
  - :ref:`AT+RFPOWER <cmd-RFPOWER>`: Query/Set RF TX Power.
  - :ref:`AT+SYSROLLBACK <cmd-SYSROLLBACK>`: Roll back to the previous firmware.
  - :ref:`AT+SYSTIMESTAMP <cmd-SETTIME>`: Query/Set local time stamp.
  - :ref:`AT+SYSLOG <cmd-SYSLOG>`: Enable or disable the AT error code prompt.
  - :ref:`AT+SLEEPWKCFG <cmd-WKCFG>`: Query/Set the light-sleep wakeup source and awake GPIO.
  - :ref:`AT+SYSSTORE <cmd-SYSSTORE>`: Query/Set parameter store mode.
  - :ref:`AT+SYSREG <cmd-SYSREG>`: Read/write the register.
  :esp32c3: - :ref:`AT+SYSTEMP <cmd-SYSTEMP>`: Read the internal chip Celsius temperature value.

.. _cmd-basic-intro:

Introduction
------------

.. important::
  The default AT firmware supports all the AT commands mentioned on this page.

.. _cmd-AT:

:ref:`AT <Basic-AT>`: Test AT Startup
------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT  

**Response:**

::

    OK  

.. _cmd-RST:

:ref:`AT+RST <Basic-AT>`: Restart a Module
-------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+RST  

**Response:**

::

    OK  

.. only:: esp32c2 or esp32c3

    Set Command
    ^^^^^^^^^^^

    **Command:**

    ::

        AT+RST=<mode>

    **Response:**

    ::

        OK

    Parameter
    ^^^^^^^^^^

    - **<mode>**:

      - 0: Restart {IDF_TARGET_NAME} and the chip enters normal running mode.
      - 1: Restart {IDF_TARGET_NAME} and the chip enters firmware downloading mode.

    Note
    ^^^^^

    - If you want to achieve the firmware download, you could send this set command to let {IDF_TARGET_NAME} enter the download mode, so that you can save Boot pins on the hardware.

.. _cmd-GMR:

:ref:`AT+GMR <Basic-AT>`: Check Version Information
--------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+GMR

**Response:**

::

    <AT version info>
    <SDK version info>
    <compile time>
    <Bin version>

    OK

Parameters
^^^^^^^^^^

-  **<AT version info>**: information about the esp-at core library version, which is under the directory: ``esp-at/components/at/lib/``. Code is closed source, no plan to open.
-  **<SDK version info>**: information about the esp-at platform sdk version, which is defined in file: ``esp-at/module_config/module_{platform}_default/IDF_VERSION``
-  **<compile time>**: the time to compile the firmware.
-  **<Bin version>**: esp-at firmware version. Version information can be modified in menuconfig. (``python build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``AT firmware version.``)

Note
^^^^^

-  If you have any issues on esp-at firmware, please provide ``AT+GMR`` version information firstly.

Example
^^^^^^^^

::

    AT+GMR
    AT version:2.2.0.0-dev(ca41ec4 - {IDF_TARGET_NAME} - Sep 16 2020 11:28:17)
    SDK version:v4.0.1-193-ge7ac221b4
    compile time(98b95fc):Oct 29 2020 11:23:25
    Bin version:2.1.0(MINI-1)

    OK

.. _cmd-CMD:

:ref:`AT+CMD <Basic-AT>`: List all AT commands and types supported in current firmware
--------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+CMD?

**Response:**

::

    +CMD:<index>,<AT command name>,<support test command>,<support query command>,<support set command>,<support execute command>

    OK

Parameters
^^^^^^^^^^

-  **<index>**: AT command sequence number.
-  **<AT command name>**: AT command name.
-  **<support test command>**: 0 means not supported, 1 means supported.
-  **<support query command>**: 0 means not supported, 1 means supported.
-  **<support set command>**: 0 means not supported, 1 means supported.
-  **<support execute command>**: 0 means not supported, 1 means supported.

.. _cmd-GSLP:

:ref:`AT+GSLP <Basic-AT>`: Enter Deep-sleep Mode
-----------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+GSLP=<time>

**Response:**

::

    <time>

    OK

Parameter
^^^^^^^^^^

-  **<time>**: The duration when the device stays in Deep-sleep. Unit: millisecond. When the time is up, the device automatically wakes up, calls Deep-sleep wake stub, and then proceeds to load the application.

   .. only:: esp32c3 or esp32c2 or esp32 or esp32s2

       - 0 means restarting right now

   .. only:: esp32c6

       - The minimum Deep-sleep time is 5 milliseconds

    - The maximum Deep-sleep time is about 28.8 days (2 :sup:`31`-1 milliseconds)

Notes
^^^^^^

- The theoretical and actual time of Deep-sleep may be different due to external factors.

.. _cmd-ATE:

:ref:`ATE <Basic-AT>`: Configure AT Commands Echoing
-----------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    ATE0  

or

::

    ATE1  

**Response:**

::

    OK  

Parameters
^^^^^^^^^^

-  **ATE0**: Switch echo off.
-  **ATE1**: Switch echo on.

.. _cmd-RESTORE:

:ref:`AT+RESTORE <Basic-AT>`: Restore Factory Default Settings
-----------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+RESTORE  

**Response:**

::

    OK  

Notes
^^^^^

-  The execution of this command will restore all parameters saved in flash to factory default settings of the module.
-  The device will be restarted when this command is executed.

.. _cmd-SAVET:

:ref:`AT+SAVETRANSLINK <TCPIP-AT>`: Set Whether to Enter Wi-Fi/Bluetooth LE :term:`Passthrough Mode` on Power-up
----------------------------------------------------------------------------------------------------------------

.. list::

    * :ref:`savetrans-tcpssl`
    * :ref:`savetrans-udp`
    :esp32 or esp32c3 or esp32c6 or esp32c2: * :ref:`savetrans-ble`

.. _savetrans-tcpssl:

For TCP/SSL Single Connection
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
""""""""""""""

**Command:**

::

    AT+SAVETRANSLINK=<mode>,<"remote host">,<remote port>[,<"type">,<keep_alive>]

**Response:**

::

    OK

Parameters
""""""""""""""

-  **<mode>**:

   -  0: {IDF_TARGET_NAME} will NOT enter Wi-Fi :term:`Passthrough Mode` on power-up.
   -  1: {IDF_TARGET_NAME} will enter Wi-Fi :term:`Passthrough Mode` on power-up.

-  **<"remote host">**: IPv4 address, IPv6 address, or domain name of remote host. The maximum length is 64 bytes.
-  **<remote port>**: the remote port number.
-  **<"type">**: string parameter showing the type of transmission: "TCP", "TCPv6", "SSL", or "SSLv6". Default: "TCP".
-  **<keep_alive>**: It configures the `SO_KEEPALIVE <https://man7.org/linux/man-pages/man7/socket.7.html#SO_KEEPALIVE>`__ option for socket. Unit: second.

   - Range: [0,7200].

     - 0: disable keep-alive function (default).
     - 1 ~ 7200: enable keep-alive function. `TCP_KEEPIDLE <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPIDLE>`_ value is **<keep_alive>**, `TCP_KEEPINTVL <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPINTVL>`_ value is 1, and `TCP_KEEPCNT <https://man7.org/linux/man-pages/man7/tcp.7.html#TCP_KEEPCNT>`_ value is 3.

   - This parameter of this command is the same as the ``<keep_alive>`` parameter of :ref:`AT+CIPTCPOPT <cmd-TCPOPT>` command. It always takes the value set later by either of the two commands. If it is omitted or not set, the last configured value is used by default.

Notes
"""""""

-  This command will save the Wi-Fi :term:`Passthrough Mode` configuration in the NVS area. If ``<mode>`` is set to 1, {IDF_TARGET_NAME} will enter the Wi-Fi :term:`Passthrough Mode` in the next power on. The configuration will take effect after {IDF_TARGET_NAME} reboots.

Example
""""""""

::

    AT+SAVETRANSLINK=1,"192.168.6.110",1002,"TCP"
    AT+SAVETRANSLINK=1,"www.baidu.com",443,"SSL"
    AT+SAVETRANSLINK=1,"240e:3a1:2070:11c0:55ce:4e19:9649:b75",8080,"TCPv6"
    AT+SAVETRANSLINK=1,"240e:3a1:2070:11c0:55ce:4e19:9649:b75",8080,"SSLv6"

.. _savetrans-udp:

For UDP Transmission
^^^^^^^^^^^^^^^^^^^^

Set Command
""""""""""""""

**Command:**

::

    AT+SAVETRANSLINK=<mode>,<"remote host">,<remote port>,[<"type">,<local port>]

**Response:**

::

    OK

Parameters
""""""""""""""

-  **<mode>**:

   -  0: {IDF_TARGET_NAME} will NOT enter Wi-Fi :term:`Passthrough Mode` on power-up.
   -  1: {IDF_TARGET_NAME} will enter Wi-Fi :term:`Passthrough Mode` on power-up.

-  **<"remote host">**: IPv4 address, IPv6 address, or domain name of remote host. The maximum length is 64 bytes.
-  **<remote port>**: the remote port number.
-  **<"type">**: string parameter showing the type of transmission: "UDP" or "UDPv6". Default: "TCP".
-  **<local port>**: local port when UDP Wi-Fi passthrough is enabled on power-up.

Notes
"""""""

-  This command will save the Wi-Fi :term:`Passthrough Mode` configuration in the NVS area. If ``<mode>`` is set to 1, {IDF_TARGET_NAME} will enter the Wi-Fi :term:`Passthrough Mode` in the next power on. The configuration will take effect after {IDF_TARGET_NAME} reboots.

-  To establish an UDP transmission based on an IPv6 network, do as follows:

  -  Make sure that the AP supports IPv6
  -  Set :ref:`AT+CIPV6=1 <cmd-IPV6>`
  -  Obtain an IPv6 address through the :ref:`AT+CWJAP <cmd-JAP>` command
  - (Optional) Check whether {IDF_TARGET_NAME} has obtained an IPv6 address using the :ref:`AT+CIPSTA? <cmd-IPSTA>` command

Example
"""""""""

::

    AT+SAVETRANSLINK=1,"192.168.6.110",1002,"UDP",1005
    AT+SAVETRANSLINK=1,"240e:3a1:2070:11c0:55ce:4e19:9649:b75",8081,"UDPv6",1005

.. only:: esp32c2 or esp32c3 or esp32c6 or esp32

    .. _savetrans-ble:

    For BLE Connection
    ^^^^^^^^^^^^^^^^^^^^

    Set Command
    """"""""""""""

    **Command:**

    ::

        AT+SAVETRANSLINK=<mode>,<role>,<tx_srv>,<tx_char>,<rx_srv>,<rx_char>,<peer_addr>

    **Response:**

    ::

        OK

    Parameters
    """"""""""""""

    -  **<mode>**:

        -  0: {IDF_TARGET_NAME} will NOT enter BLE :term:`Passthrough Mode` on power-up.
        -  2: {IDF_TARGET_NAME} will enter BLE :term:`Passthrough Mode` on power-up.

    -  **<role>**:

        -  1: client role.
        -  2: server role.

    -  **<tx_srv>**: tx service's index. It can be queried with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> if AT works as GATTC role or with command :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` if AT works as GATTS role.
    -  **<tx_char>**: tx characteristic's index. It can be queried with command :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> if AT works as GATTC role or with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` if AT works as GATTS role.
    -  **<rx_srv>**: rx service's index. It can be queried with command :ref:`AT+BLEGATTCPRIMSRV <cmd-GCPRIMSRV>`\=<conn_index> if AT works as GATTC role or with command :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>` if AT works as GATTS role.
    -  **<rx_char>**: rx characteristic's index. It can be queried with command :ref:`AT+BLEGATTCCHAR <cmd-GCCHAR>`\=<conn_index>,<srv_index> if AT works as GATTC role or with command :ref:`AT+BLEGATTSCHAR? <cmd-GSCHAR>` if AT works as GATTS role.
    -  **<peer_addr>**: remote Bluetooth LE address.

    Notes
    """""""

    -  This command will save the BLE :term:`Passthrough Mode` configuration in the NVS area. If ``<mode>`` is set to 2, {IDF_TARGET_NAME} will enter the Bluetooth LE :term:`Passthrough Mode` in the next power on. The configuration will take effect after {IDF_TARGET_NAME} reboots.

    Example
    """""""""

    ::

        AT+SAVETRANSLINK=2,2,1,7,1,5,"26:a2:11:22:33:88"

.. _cmd-TRANSINTVL:

:ref:`AT+TRANSINTVL <Basic-AT>`: Set the Data Transmission Interval in Passthrough Mode
---------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+TRANSINTVL?

**Response:**

::

    +TRANSINTVL:<interval>

    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+TRANSINTVL=<interval>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

- **<interval>**: Data transmission interval. Unit: milliseconds. Default value: 20. Range: [0,1000].

Note
^^^^^

- In passthrough mode, if the data received by {IDF_TARGET_NAME} reaches or exceeds 2920 bytes, the data will be immediately sent in chunks of 2920 bytes. Otherwise, it will wait for ``<interval>`` milliseconds before being sent.
- To optimize data transmission in cases where the data size is small and the data transmission interval is short, adjusting ``<interval>`` can be useful. A smaller ``<interval>`` reduces the delay in sending data to the protocol stack, but this may increase the number of times the protocol stack sends data to the network, thereby potentially decreasing the throughput performance to some extent.

Example
^^^^^^^^

::

    // Set to send immediately upon receiving data
    AT+TRANSINTVL=0

.. _cmd-UARTC:

:ref:`AT+UART_CUR <Basic-AT>`: Current UART Configuration, Not Saved in Flash
----------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+UART_CUR?

**Response:**

::

    +UART_CUR:<baudrate>,<databits>,<stopbits>,<parity>,<flow control>

    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+UART_CUR=<baudrate>,<databits>,<stopbits>,<parity>,<flow control>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<baudrate>**: UART baud rate

   - For {IDF_TARGET_NAME} devices, the supported range is 80 ~ 5000000.

-  **<databits>**: data bits

   -  5: 5-bit data
   -  6: 6-bit data
   -  7: 7-bit data
   -  8: 8-bit data

-  **<stopbits>**: stop bits

   -  1: 1-bit stop bit
   -  2: 1.5-bit stop bit
   -  3: 2-bit stop bit

-  **<parity>**: parity bit

   -  0: None
   -  1: Odd
   -  2: Even

-  **<flow control>**: flow control

   -  0: flow control is not enabled
   -  1: enable RTS
   -  2: enable CTS
   -  3: enable both RTS and CTS

Notes
^^^^^

-  The Query Command will return actual values of UART configuration parameters, which may have minor differences from the set value because of the clock division.
-  The configuration changes will NOT be saved in flash.
-  To use hardware flow control, you need to connect CTS/RTS pins of your {IDF_TARGET_NAME}. For more details, please refer to :doc:`../Get_Started/Hardware_connection` or ``components/customized_partitions/raw_data/factory_param/factory_param_data.csv``.

Example
^^^^^^^^

::

    AT+UART_CUR=115200,8,1,0,3  

.. _cmd-UARTD:

:ref:`AT+UART_DEF <Basic-AT>`: Default UART Configuration, Saved in Flash
------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+UART_DEF?

**Response:**

::

    +UART_DEF:<baudrate>,<databits>,<stopbits>,<parity>,<flow control>

    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+UART_DEF=<baudrate>,<databits>,<stopbits>,<parity>,<flow control>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<baudrate>**: UART baud rate

   - For {IDF_TARGET_NAME} devices, the supported range is 80 ~ 5000000.

-  **<databits>**: data bits

   -  5: 5-bit data
   -  6: 6-bit data
   -  7: 7-bit data
   -  8: 8-bit data

-  **<stopbits>**: stop bits

   -  1: 1-bit stop bit
   -  2: 1.5-bit stop bit
   -  3: 2-bit stop bit

-  **<parity>**: parity bit

   -  0: None
   -  1: Odd
   -  2: Even

-  **<flow control>**: flow control

   -  0: flow control is not enabled
   -  1: enable RTS
   -  2: enable CTS
   -  3: enable both RTS and CTS

Notes
^^^^^

-  The configuration changes will be saved in the NVS area, and will still be valid when the chip is powered on again.
-  To use hardware flow control, you need to connect CTS/RTS pins of your {IDF_TARGET_NAME}. For more details, please refer to :doc:`../Get_Started/Hardware_connection` or ``components/customized_partitions/raw_data/factory_param/factory_param_data.csv``.

Example
^^^^^^^^

::

    AT+UART_DEF=115200,8,1,0,3  

.. _cmd-SLEEP:

:ref:`AT+SLEEP <Basic-AT>`: Set the Sleep Mode
---------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+SLEEP?

**Response:**

::

    +SLEEP:<sleep mode>

    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+SLEEP=<sleep mode>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<sleep mode>**:

   - 0: Disable the sleep mode.

   - 1: Modem-sleep mode.

     - Only Wi-Fi mode.

       - RF will be periodically closed according to AP ``DTIM``.

     - Only BLE mode.

       - When Bluetooth LE is advertising, RF will be periodically closed according to advertising interval.
       - When Bluetooth LE is connected, RF will be periodically closed according to connection interval.

   - 2: Light-sleep mode.

     - Null Wi-Fi mode.

       - CPU will automatically sleep and RF will be closed.

     - Only Wi-Fi mode.

       - CPU will automatically sleep and RF will be periodically closed according to ``listen interval`` set by :ref:`AT+CWJAP <cmd-JAP>`.

     - Only Bluetooth mode.

       - When Bluetooth LE is advertising, CPU will automatically sleep and RF will be periodically closed according to advertising interval of Bluetooth.
       - When Bluetooth LE is connected, CPU will automatically sleep and RF will be periodically closed according to connection interval of Bluetooth.

    - Wi-Fi and Bluetooth coexistence mode.

        - CPU will automatically sleep and RF will be periodically closed according to power management module.

   - 3: Modem-sleep listen interval mode.

     - Only Wi-Fi mode.

       - RF will be periodically closed according to ``listen interval`` set by :ref:`AT+CWJAP <cmd-JAP>`.

     - Only BLE mode.

       - When Bluetooth LE is advertising, RF will be periodically closed according to advertising interval.
       - When Bluetooth LE is connected, RF will be periodically closed according to connection interval.

Note
^^^^^

-  When sleep mode is disabled, you cannot initialize Bluetooth LE. When Bluetooth LE is initialized, you cannot disable sleep mode.
-  Modem-sleep mode and Light-sleep mode can be set under Wi-Fi mode or BLE mode, but in Wi-Fi mode, these two modes can only be set in ``station`` mode.
-  Before setting the Light-sleep mode, it is recommended to set the wakeup source in advance through the command :ref:`AT+SLEEPWKCFG <cmd-WKCFG>`, otherwise {IDF_TARGET_NAME} can't wake up and will always be in sleep mode.
-  After setting the Light-sleep mode, if the Light-sleep wakeup condition is not met, {IDF_TARGET_NAME} will automatically enter the sleep mode. When the Light-sleep wakeup condition is met, {IDF_TARGET_NAME} will automatically wake up from sleep mode.
-  For Light-sleep mode in BLE mode, users must ensure external 32KHz crystal oscillator, otherwise the Light-sleep mode will work in Modem-sleep mode.
-  For more examples, please refer to :doc:`../AT_Command_Examples/sleep_at_examples`.

Example
^^^^^^^^

::

    AT+SLEEP=0

.. _cmd-SYSRAM:

:ref:`AT+SYSRAM <Basic-AT>`: Query Current Remaining Heap Size and Minimum Heap Size
-----------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+SYSRAM?  

**Response:**

::

    +SYSRAM:<remaining RAM size>,<minimum heap size>
    OK  

Parameters
^^^^^^^^^^

-  **<remaining RAM size>**: current remaining heap size. Unit: byte.
-  **<minimum heap size>**: minimum available heap size in the runtime. Unit: byte. When the parameter's value is less than or close to 10 KB, the Wi-Fi and BLE functions of {IDF_TARGET_NAME} may be affected.

Example
^^^^^^^^

::

    AT+SYSRAM?
    +SYSRAM:148408,84044
    OK

.. _cmd-SYSMSG:

:ref:`AT+SYSMSG <Basic-AT>`: Query/Set System Prompt Information
-----------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the current system prompt information state. 

**Command:**

::

    AT+SYSMSG?

**Response:**

::

    +SYSMSG:<state>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Configure system prompt information. If you need more fine-grained management of AT messages, please use the :ref:`AT+SYSMSGFILTER <cmd-SYSMSGFILTER>` command.

**Command:**

::

    AT+SYSMSG=<state>

**Response:**

::

    OK  

Parameter
^^^^^^^^^^

-  **<state>**:

   - Bit0: Prompt information when quitting Wi-Fi :term:`Passthrough Mode`, Bluetooth LE SPP and Bluetooth SPP.

     - 0: Print no prompt information when quitting Wi-Fi :term:`Passthrough Mode`, Bluetooth LE SPP and Bluetooth SPP.
     - 1: Print ``+QUITT`` when quitting Wi-Fi :term:`Passthrough Mode`, Bluetooth LE SPP and Bluetooth SPP.

   - Bit1: Connection prompt information type.

     - 0: Use simple prompt information, such as ``XX,CONNECT``.
     - 1: Use detailed prompt information, such as ``+LINK_CONN:status_type,link_id,ip_type,terminal_type,remote_ip,remote_port,local_port``.

   - Bit2: Connection status prompt information for Wi-Fi :term:`Passthrough Mode`, Bluetooth LE SPP and Bluetooth SPP.

     - 0: Print no prompt information.
     - 1: Print one of the following prompt information when Wi-Fi, socket, Bluetooth LE or Bluetooth status is changed:

      ::

           - "CONNECT\r\n" or the message prefixed with "+LINK_CONN:"
           - "CLOSED\r\n"
           - "WIFI CONNECTED\r\n"
           - "WIFI GOT IP\r\n"
           - "WIFI GOT IPv6 LL\r\n"
           - "WIFI GOT IPv6 GL\r\n"
           - "WIFI DISCONNECT\r\n"
           - "+ETH_CONNECTED\r\n"
           - "+ETH_DISCONNECTED\r\n"
           - the message prefixed with "+ETH_GOT_IP:"
           - the message prefixed with "+STA_CONNECTED:"
           - the message prefixed with "+STA_DISCONNECTED:"
           - the message prefixed with "+DIST_STA_IP:"
           - the message prefixed with "+BLECONN:"
           - the message prefixed with "+BLEDISCONN:"

Notes
^^^^^

-  The configuration changes will be saved in the NVS area if ``AT+SYSSTORE=1``.
-  If you set Bit0 to 1, it will prompt "+QUITT" when you quit Wi-Fi :term:`Passthrough Mode`.
-  If you set Bit1 to 1, it will impact the information of command :ref:`AT+CIPSTART <cmd-START>` and :ref:`AT+CIPSERVER <cmd-SERVER>`. It will supply "+LINK_CONN:status_type,link_id,ip_type,terminal_type,remote_ip,remote_port,local_port" instead of "XX,CONNECT".

Example
^^^^^^^^

::

    // print no promt info when quitting Wi-Fi passthrough mode
    // print detailed connection prompt info
    // print no prompt info when the connection status is changed
    AT+SYSMSG=2

or

::

    // In the transparent transmission mode, a prompt message will be printed when the Wi-Fi, socket, Bluetooth LE or Bluetooth status changes
    AT+SYSMSG=4

.. _cmd-SYSMSGFILTER:

:ref:`AT+SYSMSGFILTER <Basic-AT>`: Enable or Disable the :term:`system message` Filter
--------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the current :term:`system message` filtering state.

**Command:**

::

    AT+SYSMSGFILTER?

**Response:**

::

    +SYSMSGFILTER:<enable>

    OK

Set Command
^^^^^^^^^^^

**Function:**

Enable or disable the :term:`system message` filter.

**Command:**

::

    AT+SYSMSGFILTER=<enable>

**Response:**

::

    OK

Parameter
^^^^^^^^^

- **<enable>**:

    - 0: Disable system message filtering. System default value. When disabled, system messages will not be filtered by the set filter.
    - 1: Enable system message filtering. When it is enabled, the data matching the regular expression will be filtered out by AT, and MCU will not receive it, whereas the unmatched data will be sent to the MCU as it is.

Notes
^^^^^

- Please use :ref:`AT+SYSMSGFILTERCFG <cmd-SYSMSGFILTERCFG>` to set up system message filters. Then, use this command to enable the system message filtering to achieve more sophisticated system message management.
- Please use the :ref:`AT+SYSMSGFILTER=1 <cmd-SYSMSGFILTER>` command with caution. It is recommended that you disable the system message filtering promptly after enabling it to prevent the over-filtering of AT system messages.
- Before entering the :term:`Passthrough Mode`, it is strongly recommended to use the :ref:`AT+SYSMSGFILTER=0 <cmd-SYSMSGFILTER>` command to disable system message filtering.
- If you are working on secondary development of AT project, please use the following APIs to transmit data via the AT command port.

::

    // Data transmission via native AT command port. Data will not be filtered by AT+SYSMSGFILTER command, and AT will not actively wake up MCU before sending data (MCU wake-up function is set by AT+USERWKMCUCFG).
    int32_t esp_at_port_write_data_without_filter(uint8_t data, int32_t len);

    // Data transmission via AT command port with a filtering function. Data will be filtered by AT+SYSMSGFILTER command (if enabled), and AT will not actively wake up MCU before sending data (MCU wake-up function is set by AT+USERWKMCUCFG command).
    int32_t esp_at_port_write_data(uint8_t data, int32_t len);

    // Data transmission via AT command port with wake-up MCU function. Data will not be filtered by AT+SYSMSGFILTER command, and AT will actively wake up MCU before sending data (MCU wake-up function is set by AT+USERWKMCUCFG command).
    int32_t esp_at_port_active_write_data_without_filter(uint8_t data, int32_t len);

    // Data transmission via AT command port with wake-up MCU function and filtering function. Data will be filtered by AT+SYSMSGFILTER command (if enabled), and AT will actively wake up MCU before sending data (MCU wake-up function is set by AT+USERWKMCUCFG command).
    int32_t esp_at_port_active_write_data(uint8_t data, int32_t len);

Example
"""""""

For detailed examples, refer to :ref:`system message filtering example <example-sysmfgfilter>`.

.. _cmd-SYSMSGFILTERCFG:

:ref:`AT+SYSMSGFILTERCFG <Basic-AT>`: Query/Set the :term:`system message` Filters
----------------------------------------------------------------------------------

* :ref:`sysmsgfiltercfg-query`
* :ref:`sysmsgfiltercfg-clear`
* :ref:`sysmsgfiltercfg-add`
* :ref:`sysmsgfiltercfg-delete`

.. _sysmsgfiltercfg-query:

Query the Filters
^^^^^^^^^^^^^^^^^

Query Command
"""""""""""""

**Command:**

::

    AT+SYSMSGFILTERCFG?

**Response:**

::

    +SYSMSGFILTERCFG:<index>,"<head_regexp>","<tail_regexp>"

    OK

Parameters
""""""""""

- **<index>**: The index of filters.
- **<"head_regexp">**: The regular expression of header.
- **<"tail_regexp">**: The regular expression of tail.

.. _sysmsgfiltercfg-clear:

Clear all the Filters
^^^^^^^^^^^^^^^^^^^^^

Set Command
"""""""""""

**Command:**

::

    AT+SYSMSGFILTERCFG=<operator>

**Response:**

::

    OK

Parameter
"""""""""

- **<operator>**:

    - 0: Clear all the filters. After clearing, you can free some heap size occupied by the filters.

Example
"""""""

::

    // Clear all the filters
    AT+SYSMSGFILTERCFG=0

.. _sysmsgfiltercfg-add:

Add a Filter
^^^^^^^^^^^^

Set Command
"""""""""""

**Command:**

::

    AT+SYSMSGFILTERCFG=<operator>,<head_regexp_len>,<tail_regexp_len>

**Response:**

::

    OK

    >

The symbol ``>`` indicates that AT is ready for receiving regular expressions from AT command port. You should enter the head regular expression and the tail regular expression. When the length reaches the ``<head_regexp_len>`` + ``<tail_regexp_len>`` value, the regular expression integrity check starts.

If the regular expression integrity check fails or the addition of filter fails, AT returns:

::

    ERROR

If the integrity of the regular expression is verified successfully and the filter is added successfully, AT returns:

::

    OK

Parameters
""""""""""

- **<operator>**:

    - 1: Add a filter. A filter contains a header regular expression and a tail regular expression.

- **<head_regexp_len>**: The length of the header regular expression. Range: [0,64]. If it is set to 0, the matching of the regular expression in the header is ignored, and ``<tail_regexp_len>`` cannot be 0.
- **<tail_regexp_len>**: The length of the tail regular expression. Range: [0,64]. If it is set to 0, the matching of the regular expression in the tail is ignored, and ``<head_regexp_len>`` cannot be 0.

Notes
""""""

- Please use this command to set up system message filters. Then, use :ref:`AT+SYSMSGFILTER <cmd-SYSMSGFILTER>` to enable the system message filtering to achieve more sophisticated system message management.
- For more details about header and tail regular expression format, refer to `POSIX Basic Regular Expression (BRE) <https://en.wikipedia.org/wiki/Regular_expression#POSIX_basic_and_extended>`_.
- In order to avoid :term:`system message` (TX data of AT command port) being filtered incorrectly, it is **strongly recommended** that the header regular expression starts with ``^`` and the tail regular expression ends with ``$``.
- Only when the system message matches both the header regular expression and the tail regular expression **at the same time** is the system message filtered. After filtering, the data matching the regular expression will be filtered out by AT, and MCU will not receive it, whereas the unmatched data will be sent to the MCU as it is.
- When the system message matches one filter, it will not continue to match other filters.
- When the system message matches the filter, the system message will not be cached, that is, the previous system message and the current system message will not be combined for matching.
- For devices with large throughput, it is **strongly recommended** that you limit the number of filters and disable system message filtering using the :ref:`AT+SYSMSGFILTER=0 <cmd-SYSMSGFILTER>` command in time.

Example
"""""""

::

    // Set the filter to filter out the "WIFI CONNECTED" system message report
    AT+SYSMSGFILTERCFG=1,17,0
    // After the command returns OK and >, enter "^WIFI CONNECTED\r\n" (Note: \r\n are 2 bytes, corresponding to 0D 0A in ASCII code)

    // Enable system message filtering
    AT+SYSMSGFILTER=1

    // Test filtering function
    AT+CWMODE=1
    AT+CWJAP="ssid","password"
    // AT no longer outputs "WIFI CONNECTED" system message report

For more examples of filtering system messages, refer to :ref:`system message filter example <example-sysmfgfilter>`.

.. _sysmsgfiltercfg-delete:

Delete a Filter
^^^^^^^^^^^^^^^

Set Command
"""""""""""

**Command:**

::

    AT+SYSMSGFILTERCFG=<operator>,<head_regexp_len>,<tail_regexp_len>

**Response:**

::

    OK

    >

The symbol ``>`` indicates that AT is ready for receiving regular expressions from AT command port. You should enter the head regular expression and the tail regular expression. When the length reaches the ``<head_regexp_len>`` + ``<tail_regexp_len>`` value, the regular expression integrity check starts.

If the regular expression integrity check fails or the addition of filter fails, AT returns:

::

    ERROR

If the integrity of the regular expression is verified successfully and the filter is added successfully, AT returns:

::

    OK

Parameters
""""""""""

- **<operator>**:

    - 2: Delete a filter.

- **<head_regexp_len>**: The length of the header regular expression. Range: [0,64]. If it is set to 0, the ``<tail_regexp_len>`` cannot be 0.
- **<tail_regexp_len>**: The length of the header regular expression. Range: [0,64]. If it is set to 0, the ``<head_regexp_len>`` cannot be 0.

Notes
""""""

- The filter to be deleted should be in the added filters.

Example
"""""""

::

    // Delete the filter added above
    AT+SYSMSGFILTERCFG=2,17,0
    // After the command returns OK and >, enter "^WIFI CONNECTED\r\n" (Note: \r\n are 2 bytes, corresponding to 0D 0A in ASCII code)

    // Test filtering function
    AT+CWMODE=1
    AT+CWJAP="ssid","password"
    // AT will output "WIFI CONNECTED" system message report again

.. _cmd-SYSFLASH:

:ref:`AT+SYSFLASH <Basic-AT>`: Query/Set User Partitions in Flash
-------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query user partitions in flash.

**Command:**

::

    AT+SYSFLASH?

**Response:**

::

    +SYSFLASH:<partition>,<type>,<subtype>,<addr>,<size>
    OK  

Set Command
^^^^^^^^^^^

**Function:**

Read/write the user partitions in flash. 

**Command:**

::

    AT+SYSFLASH=<operation>,<partition>,<offset>,<length>

**Response:**

::

    +SYSFLASH:<length>,<data>
    OK  

Parameters
^^^^^^^^^^

-  **<operation>**:

   -  0: erase sector
   -  1: write data into the user partition
   -  2: read data from the user partition

-  **<partition>**: name of user partition
-  **<offset>**: offset of user partition
-  **<length>**: data length
-  **<type>**: type of user partition
-  **<subtype>**: subtype of user partition
-  **<addr>**: address of user partition
-  **<size>**: size of user partition

Notes
^^^^^

-  Please make sure that you have downloaded at_customize.bin before using this command. For more details, please refer to :doc:`../Compile_and_Develop/How_to_customize_partitions`.
-  When erasing a partition, please erase the target partition in its entirety. This can be done by omitting the parameters ``<offset>`` and ``<length>``. For example, command ``AT+SYSFLASH=0,"mfg_nvs"`` can erase the entire "mfg_nvs" user partition.
-  The introduction to partitions is in `ESP-IDF Partition Tables <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/partition-tables.html>`_.
-  If the operator is ``write``, wrap return ``>`` after the write command, then you can send the data that you want to write. The length should be parameter ``<length>``.
-  If the operator is ``write``, please make sure that you have already erased this partition.
-  If you want to modify some data in the "mfg_nvs" partition, please use the :ref:`AT+SYSMFG <cmd-SYSMFG>` command (key-value pairs operation). If you want to modify total "mfg_nvs" partition, please use the :ref:`AT+SYSFLASH <cmd-SYSFLASH>` command (partition operation).

Example
^^^^^^^^

::

    // erase the "mfg_nvs" partition in its entirety.
    AT+SYSFLASH=0,"mfg_nvs",4096,8192

    // write a new "mfg_nvs" partition (size: 0x1C000) at offset 0 of the "mfg_nvs" partition.
    AT+SYSFLASH=1,"mfg_nvs",0,0x1C000

.. _cmd-SYSMFG:

:ref:`AT+SYSMFG <Basic-AT>`: Query/Set :term:`manufacturing nvs` User Partitions
--------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query all namespaces of :term:`manufacturing nvs` user partitions.

**Command:**

::

    AT+SYSMFG?

**Response:**

::

    +SYSMFG:<"namespace">

    OK

Erase a namespace or key-value pair
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
"""""""""""

**Command:**

::

    AT+SYSMFG=<operation>,<"namespace">[,<"key">]

**Response:**

::

    OK

Parameters
"""""""""""

- **<operation>**:

   - 0: erase operation
   - 1: read operation
   - 2: write operation

- **<"namespace">**: namespace name.
- **<"key">**: key name. If this parameter is omitted, all key-value pairs of current ``<"namespace">`` will be erased. Otherwise, only the current key-value pair is erased.

Note
^^^^
- Please refer to the `Non-Volatile Storage (NVS) <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html>`_ documentation to understand the concept of namespace and key-value pairs.

Example
"""""""

::

    // Erase all key-value pairs of client_cert namespace (That is, erase all client certificates)
    AT+SYSMFG=0,"client_cert"

    // Erase the client_cert.0 key-value pair of client_cert namespace (That is, erase the first client certificate)
    AT+SYSMFG=0,"client_cert","client_cert.0"

Read a namespace or key-value pair
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
"""""""""""

**Command:**

::

    AT+SYSMFG=<operation>[,<"namespace">][,<"key">][,<offset>,<length>]

**Response:**

When ``<"namespace">`` and subsequent parameters are omitted, it returns:

::

    +SYSMFG:<"namespace">

    OK

When ``<"key">`` and subsequent parameters are omitted, it returns:

::

    +SYSMFG:<"namespace">,<"key">,<type>

    OK

In other cases, it returns:

::

    +SYSMFG:<"namespace">,<"key">,<type>,<length>,<value>

    OK

Parameters
""""""""""

- **<operation>**:

   - 0: erase operation
   - 1: read operation
   - 2: write operation

- **<"namespace">**: namespace name.
- **<"key">**: key name.
- **<offset>**: The offset of the value.
- **<length>**: The length of the value.
- **<type>**: The type of the value.

  - 1: u8
  - 2: i8
  - 3: u16
  - 4: i16
  - 5: u32
  - 6: i32
  - 7: string
  - 8: binary

- **<value>**: The data of the value.

Note
^^^^
- Please refer to the `Non-Volatile Storage (NVS) <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html>`_ documentation to understand the concept of namespace and key-value pairs.

Example
"""""""

::

    // Read all namespaces
    AT+SYSMFG=1

    // Read all key-value pairs of client_cert namespace
    AT+SYSMFG=1,"client_cert"

    // Read the value of client_cert.0 key in client_cert namespace
    AT+SYSMFG=1,"client_cert","client_cert.0"

    // Read the value of client_cert.0 key in client_cert namespace, from offset: 100 place, read 200 bytes
    AT+SYSMFG=1,"client_cert","client_cert.0",100,200

Write a key-value pair to a namespace
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Set Command
"""""""""""

**Command:**

::

    AT+SYSMFG=<operation>,<"namespace">,<"key">,<type>,<value>

**Response:**

::

    OK

Parameters
""""""""""

- **<operation>**:

   - 0: erase operation
   - 1: read operation
   - 2: write operation

- **<"namespace">**: namespace name.
- **<"key">**: key name.
- **<type>**: The type of the value.

  - 1: u8
  - 2: i8
  - 3: u16
  - 4: i16
  - 5: u32
  - 6: i32
  - 7: string
  - 8: binary

- **<value>**: It means differently depending on the parameter ``<type>``:

  - If ``<type>`` is between 1-6, ``<value>`` represents the real value.
  - If ``<type>`` is between 7-8, ``<value>`` represents the length of the value. After you send the command, AT will return ``>``. This symbol indicates that AT is ready for receiving data. You should enter the data of designated length. When the data length reaches the ``<value>`` value, the key-value pair will be written to the namespace immediately.

Note
^^^^
- Please refer to the `Non-Volatile Storage (NVS) <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html>`_ documentation to understand the concept of namespace and key-value pairs.
- Before writing, you do not need to manually erase namespaces or key-value pairs (NVS will automatically erase key-value pairs as needed).
- If you want to modify some data in the "mfg_nvs" partition, please use the :ref:`AT+SYSMFG <cmd-SYSMFG>` command (key-value pairs operation). If you want to modify total "mfg_nvs" partition, please use the :ref:`AT+SYSFLASH <cmd-SYSFLASH>` command (partition operation).

Example
"""""""

::

    // Write a new value for client_cert.0 key into client_cert namespace (That is, update the 0th client certificate)
    AT+SYSMFG=2,"client_cert","client_cert.0",8,1164

    // Wait until AT command port returns ``>``, and then write 1164 bytes

.. _cmd-RFPOWER:

:ref:`AT+RFPOWER <Basic-AT>`: Query/Set RF TX Power
----------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the RF TX Power.

**Command:**

::

    AT+RFPOWER?

**Response:**

.. only:: esp32 or esp32c3 or esp32c6

  ::

    +RFPOWER:<wifi_power>,<ble_adv_power>,<ble_scan_power>,<ble_conn_power>
    OK

.. only:: esp32c2 or esp32s2

  ::

    +RFPOWER:<wifi_power>
    OK

Set Command
^^^^^^^^^^^

**Command:**

.. only:: esp32 or esp32c3 or esp32c6

  ::

    AT+RFPOWER=<wifi_power>[,<ble_adv_power>,<ble_scan_power>,<ble_conn_power>]

.. only:: esp32c2 or esp32s2

  ::

    AT+RFPOWER=<wifi_power>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

- **<wifi_power>**: the unit is 0.25 dBm. For example, if you set the value to 78, the actual maximum RF Power value is 78 * 0.25 dBm = 19.5 dBm. After you configure it, please confirm the actual value by entering the command ``AT+RFPOWER?``.

  .. only:: esp32

    - For {IDF_TARGET_NAME} devices, the range is [40,84]:

      ========= ============ ============ ==========
      set value   get value  actual value actual dBm
      ========= ============ ============ ==========
      [40,43]   34           34           8.5
      [44,51]   44           44           11
      [52,55]   52           52           13
      [56,59]   56           56           14
      [60,65]   60           60           15
      [66,71]   66           66           16.5
      [72,77]   72           72           18
      [78,84]   78           78           19.5
      ========= ============ ============ ==========

  .. only:: esp32c3 or esp32c6 or esp32c2

    - For {IDF_TARGET_NAME} devices, the range is [40,84]:

      ========= ============ ============ ==========
      set value   get value  actual value actual dBm
      ========= ============ ============ ==========
      [40,80]   <set value>  <set value>  <set value> * 0.25
      [81,84]   <set value>  80           20
      ========= ============ ============ ==========

  .. only:: esp32s2

    - For {IDF_TARGET_NAME} devices, the range is [40,84]:

      ========= ============ ============ ==========
      set value   get value  actual value actual dBm
      ========= ============ ============ ==========
      [40,78]   <set value>  <set value>  <set value> * 0.25
      [79,84]   <set value>  78           19.5
      ========= ============ ============ ==========

.. only:: esp32

  -  **<ble_adv_power>**: RF TX Power of Bluetooth LE advertising. Range: [0,7].

    -  0: 7 dBm
    -  1: 4 dBm
    -  2: 1 dBm
    -  3: -2 dBm
    -  4: -5 dBm
    -  5: -8 dBm
    -  6: -11 dBm
    -  7: -14 dBm

.. only:: esp32c3

  -  **<ble_adv_power>**: RF TX Power of Bluetooth LE advertising. Range: [0,15].

    -  0: -24 dBm
    -  1: -21 dBm
    -  2: -18 dBm
    -  3: -15 dBm
    -  4: -12 dBm
    -  5: -9 dBm
    -  6: -6 dBm
    -  7: -3 dBm
    -  8: -0 dBm
    -  9: 3 dBm
    -  10: 6 dBm
    -  11: 9 dBm
    -  12: 12 dBm
    -  13: 15 dBm
    -  14: 18 dBm
    -  15: 21 dBm

.. only:: esp32c6

  -  **<ble_adv_power>**: RF TX Power of Bluetooth LE advertising. Range: [3,15].

    -  3: -15 dBm
    -  4: -12 dBm
    -  5: -9 dBm
    -  6: -6 dBm
    -  7: -3 dBm
    -  8: -0 dBm
    -  9: 3 dBm
    -  10: 6 dBm
    -  11: 9 dBm
    -  12: 12 dBm
    -  13: 15 dBm
    -  14: 18 dBm
    -  15: 20 dBm

.. only:: esp32 or esp32c3 or esp32c6

  -  **<ble_scan_power>**: RF TX Power of Bluetooth LE scanning. The parameters are the same as ``<ble_adv_power>``.
  -  **<ble_conn_power>**: RF TX Power of Bluetooth LE connecting. The same as ``<ble_adv_power>``.

Note
------

- When Wi-Fi is turned off or not initialized, the :ref:`AT+RFPOWER <cmd-RFPOWER>` command cannot set or query the RF TX Power of Wi-Fi. Similarly, when Bluetooth LE is not initialized, the command cannot set or query that of Bluetooth LE, either.
- Since the RF TX Power is actually divided into several levels, and each level has its own value range, the ``wifi_power`` value queried by the ``esp_wifi_get_max_tx_power`` may differ from the value set by ``esp_wifi_set_max_tx_power`` and is no larger than the set value.
- It is recommended to set the two parameters <ble_scan_power> and <ble_conn_power> to the same value as the <ble_adv_power> parameter. Otherwise, they will be automatically adjusted to the value of <ble_adv_power>.

.. _cmd-SYSROLLBACK:

:ref:`AT+SYSROLLBACK <Basic-AT>`: Roll Back to the Previous Firmware
------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+SYSROLLBACK

**Response:**

::

    OK

Note
^^^^^

.. only:: esp32c2

  - **{IDF_TARGET_CFG_PREFIX}-4MB AT firmware supports this command, but {IDF_TARGET_CFG_PREFIX}-2MB AT firmware does not due to the compressed OTA firmware**.

-  This command will not upgrade via OTA. It only rolls back to the firmware which is in the other OTA partition.

.. _cmd-SETTIME:

:ref:`AT+SYSTIMESTAMP <Basic-AT>`: Query/Set Local Time Stamp
--------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the time stamp.

**Command:**

::

    AT+SYSTIMESTAMP?

**Response:**

::

    +SYSTIMESTAMP:<Unix_timestamp>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set local time stamp. It will be the same as SNTP time when the SNTP time is updated.

**Command:**

::

    AT+SYSTIMESTAMP=<Unix_timestamp>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<Unix-timestamp>**: Unix timestamp. Unit: second.

Example
^^^^^^^^

::

    AT+SYSTIMESTAMP=1565853509    //2019-08-15 15:18:29

.. _cmd-SYSLOG:

:ref:`AT+SYSLOG <Basic-AT>`: Enable or Disable the AT Error Code Prompt
----------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query whether the AT error code prompt is enabled or not.

**Command:**

::

    AT+SYSLOG?  

**Response:**

::

    +SYSLOG:<status>  

    OK  

Set Command
^^^^^^^^^^^

**Function:**

Enable or disable the AT error code prompt.

**Command:**

::

    AT+SYSLOG=<status>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<status>**: enable or disable

   -  0: disable
   -  1: enable

Example
^^^^^^^^

::

   // enable AT error code prompt
   AT+SYSLOG=1

   OK
   AT+FAKE
   ERR CODE:0x01090000

   ERROR


::

   // disable AT error code prompt
   AT+SYSLOG=0

   OK
   AT+FAKE
   // No `ERR CODE:0x01090000` 

   ERROR  

The error code is a 32-bit hexadecimal value and defined as follows:

.. list-table::
   :header-rows: 1

   * - category
     - subcategory
     - extension
   * - bit32 ~ bit24
     - bit23 ~ bit16
     - bit15 ~ bit0

-  **category:** stationary value 0x01.
-  **subcategory:** error type.

   .. list-table:: Subcategory of Error Code  
      :header-rows: 1
       
      * - Error Type
        - Error Code
        - Description
      * - ESP_AT_SUB_OK
        - 0x00
        - OK
      * - ESP_AT_SUB_COMMON_ERROR
        - 0x01
        - reserved  
      * - ESP_AT_SUB_NO_TERMINATOR
        - 0x02
        - terminator character not found ("\r\n" expected)
      * - ESP_AT_SUB_NO_AT
        - 0x03
        - Starting AT not found (or at, At or aT entered)
      * - ESP_AT_SUB_PARA_LENGTH_MISMATCH
        - 0x04
        - parameter length mismatch
      * - ESP_AT_SUB_PARA_TYPE_MISMATCH
        - 0x05
        - parameter type mismatch
      * - ESP_AT_SUB_PARA_NUM_MISMATCH
        - 0x06
        - parameter number mismatch
      * - ESP_AT_SUB_PARA_INVALID
        - 0x07
        - the parameter is invalid
      * - ESP_AT_SUB_PARA_PARSE_FAIL
        - 0x08
        - parse parameter fail
      * - ESP_AT_SUB_UNSUPPORT_CMD
        - 0x09
        - the command is not supported
      * - ESP_AT_SUB_CMD_EXEC_FAIL
        - 0x0A
        - the command execution failed 
      * - ESP_AT_SUB_CMD_PROCESSING
        - 0x0B
        - processing of previous command is in progress
      * - ESP_AT_SUB_CMD_OP_ERROR
        - 0x0C
        - the command operation type is error

-  **extension:** error extension information. There are different extensions for different subcategory. For more information, please see the ``components/at/include/esp_at.h``.

For example, the error code ``ERR CODE:0x01090000`` means the command is not supported.

.. _cmd-WKCFG:

:ref:`AT+SLEEPWKCFG <Basic-AT>`: Set the Light-sleep Wakeup Source and Awake GPIO
-----------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+SLEEPWKCFG=<wakeup source>,<param1>[,<param2>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<wakeup source>**:

   -  0: reserved, not supported now.
   -  1: reserved, not supported now.
   -  2: wakeup by GPIO.

-  **<param1>**:

   -  If the wakeup source is a timer, it means the time before wakeup. Unit: millisecond.
   -  If the wakeup source is GPIO, it means the GPIO number.

-  **<param2>**:

   -  If the wakeup source is GPIO, it means the wakeup level:

     - 0: low level.
     - 1: high level.

Example
^^^^^^^^

::

    // GPIO12 wakeup, low level
    AT+SLEEPWKCFG=2,12,0

.. _cmd-SYSSTORE:

:ref:`AT+SYSSTORE <Basic-AT>`: Query/Set Parameter Store Mode
--------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the AT parameter store mode.  

**Command:**

::

    AT+SYSSTORE?  

**Response:**

::

    +SYSSTORE:<store_mode>  

    OK  

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+SYSSTORE=<store_mode>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<store_mode>**:

   -  0: command configuration is not stored into flash.
   -  1: command configuration is stored into flash. (Default)

Note
^^^^^

- This command affects set commands only. Query commands are always fetched from RAM.
- Affected commands:

.. list::

  - :ref:`AT+SYSMSG <cmd-SYSMSG>`
  - :ref:`AT+CWMODE <cmd-MODE>`
  - :ref:`AT+CIPV6 <cmd-IPV6>`
  - :ref:`AT+CWJAP <cmd-JAP>`
  - :ref:`AT+CWSAP <cmd-SAP>`
  - :ref:`AT+CWRECONNCFG <cmd-RECONNCFG>`
  - :ref:`AT+CIPAP <cmd-IPAP>`
  - :ref:`AT+CIPSTA <cmd-IPSTA>`
  - :ref:`AT+CIPAPMAC <cmd-APMAC>`
  - :ref:`AT+CIPSTAMAC <cmd-STAMAC>`
  - :ref:`AT+CIPDNS <cmd-DNS>`
  - :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
  - :ref:`AT+CIPRECONNINTV <cmd-AUTOCONNINT>`
  - :ref:`AT+CIPTCPOPT <cmd-TCPOPT>`
  - :ref:`AT+CWDHCPS <cmd-DHCPS>`
  - :ref:`AT+CWDHCP <cmd-DHCP>`
  - :ref:`AT+CWSTAPROTO <cmd-STAPROTO>`
  - :ref:`AT+CWAPPROTO <cmd-APPROTO>`
  - :ref:`AT+CWJEAP <cmd-JEAP>`
  :esp32: - :ref:`AT+CIPETH <cmd-ETHIP>`
  :esp32: - :ref:`AT+CIPETHMAC <cmd-ETHMAC>`
  :esp32 or esp32c3: - :ref:`AT+BLENAME <cmd-BNAME>`
  :esp32: - :ref:`AT+BTNAME <cmd-BTNAME>`
  :esp32 or esp32c3: - :ref:`AT+BLEADVPARAM <cmd-BADVP>`
  :esp32 or esp32c3: - :ref:`AT+BLEADVDATA <cmd-BADVD>`
  :esp32 or esp32c3: - :ref:`AT+BLEADVDATAEX <cmd-BADVDEX>`
  :esp32 or esp32c3: - :ref:`AT+BLESCANRSPDATA <cmd-BSCANR>`
  :esp32 or esp32c3: - :ref:`AT+BLESCANPARAM <cmd-BSCANP>`
  :esp32: - :ref:`AT+BTSCANMODE <cmd-BTSCANMODE>`

Examples
^^^^^^^^

::

   AT+SYSSTORE=0
   AT+CWMODE=1  // Not stored into flash
   AT+CWJAP="test","1234567890" // Not stored into flash

   AT+SYSSTORE=1
   AT+CWMODE=3  // Stored into flash
   AT+CWJAP="test","1234567890" // Stored into flash

.. _cmd-SYSREG:

:ref:`AT+SYSREG <Basic-AT>`: Read/Write the Register
--------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+SYSREG=<direct>,<address>[,<write value>]

**Response:**

::

    +SYSREG:<read value>    // Only in read mode
    OK

Parameters
^^^^^^^^^^

-  **<direct>**: read or write register.

   -  0: read register.
   -  1: write register.

-  **<address>**: (uint32) register address. You can refer to Technical Reference Manuals.
-  **<write value>**: (uint32) write value (only in write mode).

Note
^^^^^

- AT does not check address. Make sure that the registers you are operating on are valid.

.. only:: esp32c3

  .. _cmd-SYSTEMP:

  :ref:`AT+SYSTEMP <Basic-AT>`: Read the Internal Chip Celsius Temperature Value
  ------------------------------------------------------------------------------

  **Function:**

  Read data from the internal chip temperature sensor and convert it into values in Celsius degrees.

  Query Command
  ^^^^^^^^^^^^^

  **Command:**

  ::

      AT+SYSTEMP?

  **Response:**

  ::

      +SYSTEMP:<value>
      OK

  Parameter
  ^^^^^^^^^

  - **<value>**: Celsius temperature value. Floating point type with two decimal places.
