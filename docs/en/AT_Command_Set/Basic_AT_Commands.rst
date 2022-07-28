.. _Basic-AT:

Basic AT Commands
=================

:link_to_translation:`zh_CN:[中文]`

.. list::

  - :ref:`AT <cmd-AT>`: Test AT startup.
  - :ref:`AT+RST <cmd-RST>`: Restart a module.
  - :ref:`AT+GMR <cmd-GMR>`: Check version information.
  - :ref:`AT+CMD <cmd-cmd>`: List all AT commands and types supported in current firmware.
  - :ref:`AT+GSLP <cmd-GSLP>`: Enter Deep-sleep mode.
  - :ref:`ATE <cmd-ATE>`: Configure AT commands echoing.
  - :ref:`AT+RESTORE <cmd-RESTORE>`: Restore factory default settings of the module.
  - :ref:`AT+UART_CUR <cmd-UARTC>`: Current UART configuration, not saved in flash.
  - :ref:`AT+UART_DEF <cmd-UARTD>`: Default UART configuration, saved in flash.
  - :ref:`AT+SLEEP <cmd-SLEEP>`: Set the sleep mode.
  - :ref:`AT+SYSRAM <cmd-SYSRAM>`: Query current remaining heap size and minimum heap size.
  - :ref:`AT+SYSMSG <cmd-SYSMSG>`: Query/Set System Prompt Information.
  - :ref:`AT+SYSFLASH <cmd-SYSFLASH>`: Query/Set User Partitions in Flash.
  - :ref:`AT+FS <cmd-FS>`: Filesystem Operations.
  - :ref:`AT+RFPOWER <cmd-RFPOWER>`: Query/Set RF TX Power.
  - :ref:`AT+SYSROLLBACK <cmd-SYSROLLBACK>`: Roll back to the previous firmware.
  - :ref:`AT+SYSTIMESTAMP <cmd-SETTIME>`: Query/Set local time stamp.
  - :ref:`AT+SYSLOG <cmd-SYSLOG>`: Enable or disable the AT error code prompt.
  - :ref:`AT+SLEEPWKCFG <cmd-WKCFG>`: Query/Set the light-sleep wakeup source and awake GPIO.
  - :ref:`AT+SYSSTORE <cmd-SYSSTORE>`: Query/Set parameter store mode.
  - :ref:`AT+SYSREG <cmd-SYSREG>`: Read/write the register.
  :esp32c3: - :ref:`AT+SYSTEMP <cmd-SYSTEMP>`: Read the internal chip Celsius temperature value.

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
-  **<Bin version>**: esp-at firmware version. Version information can be modified in menuconfig.

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

-  **<time>**: the duration when the device stays in Deep-sleep. Unit: millisecond. When the time is up, the device automatically wakes up, calls Deep-sleep wake stub, and then proceeds to load the application.

    - 0 means restarting right now
    - the maximum Deep-sleep time is about 28.8 days (2 :sup:`31`-1 milliseconds)

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

       - RF will be periodically closed according to advertising interval ( BLE state in advertising ).
       - RF will be periodically closed according to connection interval ( BLE state in connection ).

   - 2: Light-sleep mode.

     - Only Wi-Fi mode.

       - CPU will automatically sleep and RF will be periodically closed according to ``listen interval`` set by :ref:`AT+CWJAP <cmd-JAP>`.

     - Only BLE mode.

       - CPU will automatically sleep and RF will be periodically closed according to advertising interval ( BLE state in advertising ).
       - CPU will automatically sleep and RF will be periodically closed according to connection interval ( BLE state in connection ).

   - 3: Modem-sleep listen interval mode.

     - Only Wi-Fi mode.

       - RF will be periodically closed according to ``listen interval`` set by :ref:`AT+CWJAP <cmd-JAP>`.

     - Only BLE mode.

       - RF will be periodically closed according to advertising interval ( BLE state in advertising ).
       - RF will be periodically closed according to connection interval ( BLE state in connection ).

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
-  **<minimum heap size>**: minimum heap size that has ever been available. Unit: byte.

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

Configure system prompt information.

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
-  Before downloading the secondary user partition, please refer :doc:`../Compile_and_Develop/how_to_generate_pki_files` to generate the binary user partition file.
-  When erasing the targeted user partition in its entirety, you can omit the parameters ``<offset>`` and ``<length>``. For example, command ``AT+SYSFLASH=0,"ble_data"`` can erase the entire "ble_data" user partition. But if you want to keep the two parameters, they have to be 4KB-aligned.
-  The introduction to partitions is in `ESP-IDF Partition Tables <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/partition-tables.html>`_.
-  If the operator is ``write``, wrap return ``>`` after the write command, then you can send the data that you want to write. The length should be parameter ``<length>``.
-  If the operator is ``write``, please make sure that you have already erased this partition.
-  If the operator is ``write`` on a `PKI bin <https://github.com/espressif/esp-at/blob/master/tools/README.md#2-pki-bin>`_, the ``<length>`` should be 4 bytes aligned.

Example
^^^^^^^^

::

    // read 100 bytes from the "ble_data" partition offset 0.
    AT+SYSFLASH=2,"ble_data",0,100

    // write 10 bytes to the "ble_data" partition offset 100.
    AT+SYSFLASH=1,"ble_data",100,10

    // erase 8192 bytes from the "ble_data" partition offset 4096.
    AT+SYSFLASH=0,"ble_data",4096,8192

.. _cmd-FS:

:ref:`AT+FS <Basic-AT>`: Filesystem Operations
---------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+FS=<type>,<operation>,<filename>,<offset>,<length>

**Response:**

::

    OK  

Parameters
^^^^^^^^^^

-  **<type>**: only FATFS is currently supported.

   -  0: FATFS

-  **<operation>**:

   -  0: delete file.
   -  1: write file.
   -  2: read file.
   -  3: query the size of the file.
   -  4: list files in a specific directory. Only root directory is currently supported.

-  **<offset>**: apply to writing and reading operations only.
-  **<length>**: data length, applying to writing and reading operations only.

Notes
^^^^^

-  Please make sure that you have downloaded at_customize.bin before using this command. For more details, refer to `ESP-IDF Partition Tables <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/partition-tables.html>`_ and :doc:`../Compile_and_Develop/How_to_customize_partitions`.
-  If the length of the read data is greater than the actual file length, only the actual data length of the file will be returned.
-  If the operator is ``write``, wrap return ``>`` after the write command, then you can send the data that you want to write. The length should be parameter ``<length>``.

Example
^^^^^^^^

::

    // delete a file.
    AT+FS=0,0,"filename"

    // write 10 bytes to offset 100 of a file.
    AT+FS=0,1,"filename",100,10

    // read 100 bytes from offset 0 of a file.
    AT+FS=0,2,"filename",0,100

    // list all files in the root directory.
    AT+FS=0,4,"."

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

::

    +RFPOWER:<wifi_power>,<ble_adv_power>,<ble_scan_power>,<ble_conn_power>
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+RFPOWER=<wifi_power>[,<ble_adv_power>,<ble_scan_power>,<ble_conn_power>]

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

  .. only:: esp32c3 or esp32c2

    - For {IDF_TARGET_NAME} devices, the range is [40,84]:

      ========= ============ ============ ==========
      set value   get value  actual value actual dBm
      ========= ============ ============ ==========
      [40,80]   <set value>  <set value>  <set value> * 0.25
      [81,84]   <set value>  80           20
      ========= ============ ============ ==========

-  **<ble_adv_power>**: RF TX Power of Bluetooth LE advertising. Range: [0,7].

  .. only:: esp32

    -  0: 7 dBm
    -  1: 4 dBm
    -  2: 1 dBm
    -  3: -2 dBm
    -  4: -5 dBm
    -  5: -8 dBm
    -  6: -11 dBm
    -  7: -14 dBm

  .. only:: esp32c3 or esp32c2

    -  0: -27 dBm
    -  1: -24 dBm
    -  2: -21 dBm
    -  3: -18 dBm
    -  4: -15 dBm
    -  5: -12 dBm
    -  6: -9 dBm
    -  7: -6 dBm
    -  8: -3 dBm
    -  9: -0 dBm
    -  10: 3 dBm
    -  11: 6 dBm
    -  12: 9 dBm
    -  13: 12 dBm
    -  14: 15 dBm
    -  15: 18 dBm

-  **<ble_scan_power>**: RF TX Power of Bluetooth LE scanning. The parameters are the same as ``<ble_adv_power>``.
-  **<ble_conn_power>**: RF TX Power of Bluetooth LE connecting. The same as ``<ble_adv_power>``.

Note
------

- Since the RF TX Power is actually divided into several levels, and each level has its own value range, the ``wifi_power`` value queried by the ``esp_wifi_get_max_tx_power`` may differ from the value set by ``esp_wifi_set_max_tx_power`` and is no larger than the set value.

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

   -  0: wakeup by a timer.
   -  1: reserved.
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

    // Timer wakeup
    AT+SLEEPWKCFG=0,1000

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
