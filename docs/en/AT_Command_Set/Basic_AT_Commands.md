<a name="Basic-AT"></a>
## Basic AT Commands 
* [AT](#cmd-AT) : Tests AT startup.
* [AT+RST](#cmd-RST) : Restarts a module.
* [AT+GMR](#cmd-GMR) : Checks version information.
* [AT+GSLP](#cmd-GSLP) : Enters Deep-sleep mode.
* [ATE](#cmd-ATE) : Configures echoing of AT commands.
* [AT+RESTORE](#cmd-RESTORE) : Restores the factory default settings of the module.
* [AT+UART_CUR](#cmd-UARTC) : Current UART configuration.
* [AT+UART_DEF](#cmd-UARTD) : Default UART configuration, saved in flash.
* [AT+SLEEP](#cmd-SLEEP) : Sets the sleep mode.
* [AT+SYSRAM](#cmd-SYSRAM) : Checks current remaining heap size and minimum heap size.
* [AT+SYSMSG](#cmd-SYSMSG) : Set message format.
* [AT+RFPOWER](#cmd-RFPOWER) : Set RF TX Power.
* [AT+SYSFLASH](#cmd-SYSFLASH) : Set User Partitions in Flash.
* [ESP32 Only] [AT+FS](#cmd-FS) : Filesystem Operations.
* [AT+SYSROLLBACK](#cmd-SYSROLLBACK) : Roll back to the previous firmware.
* [AT+SYSTIMESTAMP](#cmd-SETTIME): Set local time stamp.
* [AT+SYSLOG](#cmd-SYSLOG) : Enable or disable the AT error code prompt.
* [AT+SLEEPWKCFG](#cmd-WKCFG) : Config the light-sleep wakeup source and awake GPIO.
* [AT+SYSSTORE](#cmd-SYSSTORE) : Config parameter store mode.
* [AT+SYSREG](#cmd-SYSREG) : Read/Write the register
* [ESP32S2 Only] [AT+SYSTEMP](#cmd-SYSTEMP) : Read ESP32S2 internal celsius temperature


<a name="cmd-AT"></a>
### [AT](#Basic-AT)—Tests AT Startup
Execute Command: 

    AT  
Response: 

    OK  

<a name="cmd-RST"></a>
### [AT+RST](#Basic-AT)—Restarts the Module
Execute Command: 

    AT+RST  
Response: 

    OK  

<a name="cmd-GMR"></a>
### [AT+GMR](#Basic-AT)—Checks Version Information
Execute Command: 

    AT+GMR  
Response: 

    <AT version info>
    <SDK version info>
    <compile time>
    
    OK
Parameters:  

- **\<AT version info>**: information about the AT version.  
- **\<SDK version info>**: information about the SDK version.
- **\<compile time>**: the duration of time for compiling the BIN.
  
<a name="cmd-GSLP"></a>
### [AT+GSLP](#Basic-AT)—Enters Deep-sleep Mode
Set Command: 

    AT+GSLP=<time>  
Response:  

    <time>
    
    OK
Parameters:  

- **\<time>**: the duration of the device’s deep sleep. Unit: ms.  
    ESP device will automatically wake up after the deep-sleep for as many milliseconds (ms) as \<time> indicates.  
    Upon waking up, the device calls deep sleep wake stub, and then proceeds to load application.

***Note:***  

* On ESP8266 platform, in order to timing wake up, it is necessary to connect GPIO16 to RST pin.
* Moreover, ESP8266 can be waken up from deep sleep externally by directly triggering RST pin low level pulse.

<a name="cmd-ATE"></a>
### [ATE](#Basic-AT)—AT Commands Echoing
Execute Command: 

    ATE  
Response: 

    OK  
Parameters:  

- **ATE0**: Switches echo off.
- **ATE1**: Switches echo on.  

<a name="cmd-RESTORE"></a>
### [AT+RESTORE](#Basic-AT)—Restores the Factory Default Settings
Execute Command: 

    AT+RESTORE  
Response: 

    OK  
***Note:***  

* The execution of this command will reset all parameters saved in flash, and restore the factory default settings of the module. 
* The chip will be restarted when this command is executed.

<a name="cmd-UARTC"></a>
### [AT+UART_CUR](#Basic-AT)—Current UART Configuration, Not Saved in Flash
Query Command:  

    AT+UART_CUR?
Response:  

    +UART_CUR:<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
    
    OK
***Note:***  

* Command `AT+UART_CUR?` will return the actual value of UART configuration parameters, which may have allowable errors compared with the set value because of the clock division.
 
Set Command:  

    AT+UART_CUR=<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
Response:  

    OK
Parameters:  

- **\<baudrate>**: UART baud rate
- **\<databits>**: data bits
    -   5: 5-bit data
    -   6: 6-bit data
    -  7: 7-bit data
    -  8: 8-bit data
- **\<stopbits>**: stop bits
    -  1: 1-bit stop bit
    -  2: 1.5-bit stop bit
    -  3: 2-bit stop bit
- **\<parity>**: parity bit
    -  0: None
    -  1: Odd
    -  2: Even
- **\<flow control>**: flow control
    -  0: flow control is not enabled
    -  1: enable RTS
    -  2: enable CTS
    -  3: enable both RTS and CTS   

***Notes:***

* The configuration changes will NOT be saved in flash.
* The use of flow control requires the support of hardware:
    * IO15 is UART0 CTS
    * IO14 is UART0 RTS
* The range of baud rates supported: 80 ~ 5000000.

Example:    

    AT+UART_CUR=115200,8,1,0,3  

<a name="cmd-UARTD"></a>
### [AT+UART_DEF](#Basic-AT)—Default UART Configuration, Saved in Flash
Query Command:  

    AT+UART_DEF?
Response:  

    +UART_DEF:<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
    
    OK
Set Command:  

    AT+UART_DEF=<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
Response:  

    OK
Parameters:  

- **\<baudrate>**: UART baud rate
- **\<databits>**: data bits
    -  5: 5-bit data
    -  6: 6-bit data
    -  7: 7-bit data
    -  8: 8-bit data
- **\<stopbits>**: stop bits
    -  1: 1-bit stop bit
    -  2: 1.5-bit stop bit
    -  3: 2-bit stop bit
- **\<parity>**: parity bit
    -  0: None
    -  1: Odd
    -  2: Even
- **\<flow control>**: flow control
    -  0: flow control is not enabled
    -  1: enable RTS
    -  2: enable CTS
    -  3: enable both RTS and CTS     

***Notes:***

* The configuration changes will be saved in the NVS area, and will still be valid when the chip is powered on again.
* The use of flow control requires the support of hardware:
    * IO15 is UART0 CTS
    * IO14 is UART0 RTS
* The range of baud rates supported: 80 ~ 5000000.  

Example:    

    AT+UART_DEF=115200,8,1,0,3  

<a name="cmd-SLEEP"></a>
### [AT+SLEEP](#Basic-AT)—Sets the Sleep Mode
Set Command:  

    AT+SLEEP=<sleep mode>
Response:

    OK
Parameters:  

- **\<sleep mode>**: 
    - 0: disable the sleep mode.
    - 1: Modem-sleep DTIM mode. RF will be periodically closed according to AP DTIM.  
    - 2: Light-sleep mode. CPU will automatically sleep and RF will be periodically closed according to `listen interval` set by `AT+CWJAP`.  
    - 3: Modem-sleep listen interval mode. RF will be periodically closed according to `listen interval` set by `AT+CWJAP`.  

Example:

    AT+SLEEP=0

***Notes:***  

* Light sleep is not available for ESP32S2 currently.  

<a name="cmd-SYSRAM"></a>
### [AT+SYSRAM](#Basic-AT)—Checks current remaining heap size and minimum heap size  
Query Command:

    AT+SYSRAM?  
Response:

    +SYSRAM:<remaining RAM size>,<minimum heap size>
    OK  
Parameters:

- **\<remaining RAM size>**: current remaining heap size, unit: byte 
- **\<minimum heap size>**: minimum heap size that has ever been available, unit: byte 

Example:

    AT+SYSRAM?
    +SYSRAM:148408,84044
    OK

<a name="cmd-SYSMSG"></a>
### [AT+SYSMSG](#Basic-AT)—Control to use new or old information
Query Command:

    AT+SYSMSG?
    Function:
    Query the current system message state. 
Response:

    +SYSMSG:<state>
    OK  
        
Set Command:

    AT+SYSMSG=<state>
    Function:
    Control to use new or old information.  
Response:

    OK  
Parameters:

- **\<state>**: 
    - Bit0: Quit transparent transmission  
        0: Quit transparent transmission no information.  
        1: Quit transparent transmission will supply information.  
    - Bit1: Connection information  
        0: Use old connection information.  
        1: Use new connection information.  
    - Bit2: conection status information when in Wi-Fi transparent transmission, Ble SPP and BT SPP  
        0: There is no more prompt information but received data.  
        1: It will print some information if Wi-Fi, socket, ble or bt status is changed, the prompt is as following  

            - "CONNECT\r\n" or the message prefixed with "+LINK_CONN:"  
            - "CLOSED\r\n"  
            - "WIFI CONNECTED\r\n"  
            - "WIFI GOT IP\r\n"  
            - "WIFI DISCONNECT\r\n"  
            - "+ETH_CONNECTED\r\n"  
            - "+ETH_DISCONNECTED\r\n"  
            - the message prefixed with "+ETH_GOT_IP:"  
            - the message prefixed with "+STA_CONNECTED:"  
            - the message prefixed with "+STA_DISCONNECTED:"  
            - the message prefixed with "+DIST_STA_IP:"  
            - the message prefixed with "+BLECONN:"  
            - the message prefixed with "+BLEDISCONN:"  

***Notes:***  

* The configuration changes will be saved in the NVS area if `AT+SYSSTORE=1`.
* If set Bit0 to 1 will supply information "+QUITT" when quit transparent transmission.
* If set Bit1 to 1 will impact the infomation of command `AT+CIPSTART` and `AT+CIPSERVER`,
    * It will supply "+LINK_CONN:status_type,link_id,ip_type,terminal_type,remote_ip,remote_port,local_port" instead of "XX,CONNECT".
Example:

    // Use new connection info and quit transparent transmission no information
    AT+SYSMSG=2

<a name="cmd-SYSFLASH"></a>
### [AT+SYSFLASH](#Basic-AT)—Set User Partitions in Flash  
Query Command:

    AT+SYSFLASH?
    Function:
    Check the user partitions in flash. 
Response:

    +SYSFLASH:<partition>,<type>,<subtype>,<addr>,<size>
    OK  
Set Command:

    AT+SYSFLASH=<operation>,<partition>,<offset>,<length>
    Function:
    Read/write the user partitions in flash.    
Response:

    +SYSFLASH:<length>,<data>
    OK  
Parameters:

- **\<operation>**:
    - 0: erase sector
    - 1: write data into the user partition
    - 2: read data from the user partition
- **\<partition>**: name of user partition
- **\<offset>**: offset of user partition
- **\<length>**: data length
- **\<type>**: type of user partition
- **\<subtype>**: subtype of user partition
- **\<addr>**: address of user partition
- **\<size>**: size of user partition

***Notes:***  

* at_customize.bin has to be downloaded, so that the relevant commands can be used. Please refer to the [ESP_AT_Customize_Partitions](https://github.com/espressif/esp-at/tree/master/docs) for more details.
* Important things to note when erasing user partitions:
    * When erasing the targeted user partition in its entirety, parameters `<offset>` and `<length>` can be omitted. For example, command `AT+SYSFLASH=0,"ble_data"` can erase the entire "ble_data" user partition.
    * If parameters `<offset>` and `<length>` are not omitted when erasing the user partition, they have to be 4KB-aligned.
* The introduction to partitions is in [ESP-IDF Partition Tables](http://esp-idf.readthedocs.io/en/latest/api-guides/partition-tables.html).  
* If the operator is write, wrap return `>` after the write command, then you can send the actual data, which length is parameter `<length>`.

Example:

    // read 100 bytes from the "ble_data" partition offset 0.
    AT+SYSFLASH=2,"ble_data",0,100
    // write 10 bytes to the "ble_data" partition offset 100.
    AT+SYSFLASH=1,"ble_data",100,10
    // erase 8192 bytes from the "ble_data" partition offset 4096.
    AT+SYSFLASH=0,"ble_data",4096,8192

<a name="cmd-FS"></a>
### [ESP32 Only] [AT+FS](#Basic-AT)—Filesystem Operations  
Set Command:

    AT+FS=<type>,<operation>,<filename>,<offset>,<length>
Response:

    OK  
Parameters:

- **\<type>**: only FATFS is currently supported
    - 0: FATFS
- **\<operation>**:
    - 0: delete file
    - 1: write file
    - 2: read file
    - 3: query the size of the file
    - 4: list files in a specific directory, only root directory is currently supported
- **\<offset>**: offset, for writing and reading operations only
- **\<length>**: data length, for writing and reading operations only

***Notes:***  

* at_customize.bin has to be downloaded, so that the relevant commands can be used. The definitions of user partitions are in esp-at/at_customize.csv. Please refer to the [ESP32_Customize_Partitions](https://github.com/espressif/esp-at/tree/master/docs) for more details.
* If the length of the read data is greater than the actual file length, only the actual data length of the file will be returned.
* If the operator is write, wrap return `>` after the write command, then you can send the actual data, which length is parameter `<length>`.

Example:

    // delete a file.
    AT+FS=0,0,"filename"
    // write 10 bytes to offset 100 of a file.
    AT+FS=0,1,"filename",100,10
    // read 100 bytes from offset 0 of a file.
    AT+FS=0,2,"filename",0,100
    // list all files in the root directory.
    AT+FS=0,4,"."

<a name="cmd-RFPOWER"></a>
### [AT+RFPOWER](#Basic-AT)-Set RF TX Power
Query Command: 

    AT+RFPOWER?
    Function: to query the RF TX Power.
Response:

    +RFPOWER:<wifi_power>,<ble_adv_power>,<ble_scan_power>,<ble_conn_power>
    OK

Set Command:

    AT+RFPOWER=<wifi_power>[,<ble_adv_power>,<ble_scan_power>,<ble_conn_power>]
Response:

    OK
Parameters:
- **\<wifi_power>**: the unit is 0.25dBm.
    for example, if set the value is 78, then the actual RF max power value is 78*0.25dBm = 19.5dBm.  
    after configuring it, please confirm the actual value by `AT+RFPOWER?`  

    - range: [40, 78] on ESP32 platform and ESP32S2 platform, please refer to the notes below for details
    - range: [40, 82] on ESP8266 platform, please refer to the notes below for details

- **\<ble_adv_power>**: RF TX Power of BLE advertising, range: [0, 7]
    - 0:7dBm
    - 1:4dBm
    - 2:1dBm
    - 3:-2 dBm
    - 4:-5 dBm
    - 5:-8 dBm
    - 6:-11 dBm
    - 7:-14 dBm
- **\<ble_scan_power>**: RF TX Power of BLE scanning, range:  [0, 7], the same as **\<ble_adv_power>**
- **\<ble_conn_power>**: RF TX Power of BLE connecting, range:  [0, 7], the same as **\<ble_adv_power>**

**Notes:** 
1. Since the RF TX power is actually divided into several levels, and each level has its own value range, so the `wifi_power` value queried by the `esp_wifi_get_max_tx_power` may differ from the value set by `esp_wifi_set_max_tx_power`. And the query value will not be larger than the set one.
2. Relationship between set value and actual value, as following,

ESP32 and ESP32S2 platform:

| set value | actual value | actual dBm |
|  ----  | ----  | ----|
| [34, 43] | 34 | 8.5 |
| [44, 51] | 44 | 11 |
| [52, 55] | 52 | 13 |
| [56, 59] | 56 | 14 |
| [60, 65] | 60 | 15 |
| [66, 71] | 66 | 16.5 |
| [72, 77] | 72 | 18 |
| 78 | 78 | 19.5 

ESP8266 platform:

| set value | actual value | actual dBm |
|  ----  | ----  | ----|
| [33, 48] | 33 | 8 |
| [49, 55] | 49 | 12 |
| [56, 63] | 56 | 14 |
| [64, 67] | 64 | 16 |
| [68, 73] | 68 | 17 |
| [74, 77] | 74 | 18.5 |
| [78, 81] | 78 | 19.5 |
| 82 | 82 | 20.5 |

<a name="cmd-SYSROLLBACK"></a>
### [AT+SYSROLLBACK](#Basic-AT)-Roll back to the previous firmware
Execute Command:

    AT+SYSROLLBACK
Response:

    OK

***Note:***

* This command will not upgrade via OTA, only roll back to the firmware which is in the other ota partition. 

<a name="cmd-SETTIME"></a>
### [AT+SYSTIMESTAMP](#Basic-AT)—Set local time stamp.
Query Command: 

    AT+SYSTIMESTAMP?
    Function: to query the time stamp.
Response:

    +SYSTIMESTAMP:<Unix_timestamp>
    OK
Set Command: 

    AT+SYSTIMESTAMP=<Unix_timestamp>
    Function: to set local time stamp. It will be the same as SNTP time when the SNTP time updated.
Response:

    OK
Parameters:

- **\<Unix_timestamp>**: Unix timestamp, the unit is seconds.

Example:

    AT+SYSTIMESTAMP=1565853509    //2019-08-15 15:18:29

<a name="cmd-SYSLOG"></a>
### [AT+SYSLOG](#Basic-AT) : Enable or disable the AT error code prompt.
Query Command:  

    AT+SYSLOG?  
    Function: to query the AT error code prompt for whether it is enabled or disabled.  
Response:  

    +SYSLOG:<status>  

    OK  

Set Command:  

    AT+SYSLOG=<status>
    Function: Enable or disable the AT error code prompt.
Response:  

    OK
Parameters:  
- **\<status>**: : enable or disable
    - 0: disable
    - 1: enable

Example:  
If enable AT error code prompt:     
    
    AT+SYSLOG=1

    OK
    AT+FAKE
    ERR CODE:0x01090000

    ERROR
If disable AT error code prompt:  

    AT+SYSLOG=0

    OK
    AT+FAKE
    //No `ERR CODE:0x01090000` 

    ERROR  


The error code is 32-bits hexadecimal value and defined as follows:

|  category   | subcategory  | extension
|  ----  | ----  | ----|
| bit32~bit24  | bit23~bit16 |bit15~bit0

- **category:** stationary value 0x01
- **subcategory:** error type
- **extension:** error extension information, there are different extension for different subcategory, the detail is defined in `components/at/include/esp_at.h`

subcategory is defined as follows:

```
    ESP_AT_SUB_OK                       = 0x00,              /*!< OK */
    ESP_AT_SUB_COMMON_ERROR             = 0x01,              /*!< reserved */
    ESP_AT_SUB_NO_TERMINATOR            = 0x02,              /*!< terminator character not found ("\r\n" expected) */
    ESP_AT_SUB_NO_AT                    = 0x03,              /*!< Starting "AT" not found (or at, At or aT entered) */
    ESP_AT_SUB_PARA_LENGTH_MISMATCH     = 0x04,              /*!< parameter length mismatch */
    ESP_AT_SUB_PARA_TYPE_MISMATCH       = 0x05,              /*!< parameter type mismatch */
    ESP_AT_SUB_PARA_NUM_MISMATCH        = 0x06,              /*!< parameter number mismatch */
    ESP_AT_SUB_PARA_INVALID             = 0x07,              /*!< the parameter is invalid */
    ESP_AT_SUB_PARA_PARSE_FAIL          = 0x08,              /*!< parse parameter fail */
    ESP_AT_SUB_UNSUPPORT_CMD            = 0x09,              /*!< the command is not supported */
    ESP_AT_SUB_CMD_EXEC_FAIL            = 0x0A,              /*!< the command execution failed */
    ESP_AT_SUB_CMD_PROCESSING           = 0x0B,              /*!< processing of previous command is in progress */
    ESP_AT_SUB_CMD_OP_ERROR             = 0x0C,              /*!< the command operation type is error */
```

for example, the error code `ERR CODE:0x01090000` means the command is not supported.

<a name="cmd-WKCFG"></a>
### [AT+SLEEPWKCFG](#Basic-AT)—Config the light-sleep wakeup source and awake GPIO.
Set Command:  

    AT+SLEEPWKCFG=<wakeup source>,<param1>[,<param2>]
Response:

    OK
Parameters:  

- **\<wakeup source>**: 
    - 0: Wakeup by timer.
    - 1: Wakeup by UART. (Only Support ESP32)
    - 2: Wakeup by GPIO.

- **\<param1>**:
    - If the wakeup source is timer, this param is time before wakeup, the units is millisecond.
    - If the wakeup source is UART. this param is the Uart number.
    - If the wakeup source is GPIO, the param is the GPIO number.

- **\<param2>**:
    - If the wakeup source is GPIO, the param is the wakeup level, 0: Low level, 1: High level.

Example:

    AT+SLEEPWKCFG=0,1000  // Timer wakeup
    AT+SLEEPWKCFG=1,1     // Uart1 wakeup, Only Support ESP32
    AT+SLEEPWKCFG=2,12,0  // GPIO12 wakeup, low level.

***Notes:***

* GPIO16 as the RTC IO can not be set as GPIO wakeup source on ESP8266 platform for light sleep.

<a name="cmd-SYSSTORE"></a>
### [AT+SYSSTORE](#Basic-AT)— Config parameter store mode
Query Command:  

    AT+SYSSTORE?  
    Function: to query the AT parameter store mode.  
Response:  

    +SYSSTORE:<store_mode>  

    OK  

Set Command:  

    AT+SYSSTORE=<store_mode>
Response:

    OK
Parameters:  

- **\<store_mode>**: 
    - 0: Do not store command configuration into flash.  
    - 1: Store command configuration into flash.  

Affected commands:

    AT+SYSMSG  
    AT+CWMODE  
    AT+CWJAP  
    AT+CWSAP
    AT+CIPAP  
    AT+CIPSTA  
    AT+CIPAPMAC  
    AT+CIPSTAMAC  
    AT+CIPDNS
    AT+CIPSSLCCONF
    AT+CIPRECONNINTV
    AT+CWDHCPS  
    AT+CWDHCP  
    AT+CWSTAPROTO  
    AT+CWAPPROTO  
    AT+CWJEAP
    AT+CIPETH  
    AT+CIPETHMAC  
    AT+BLENAME  
    AT+BTNAME
    AT+BLEADVPARAM
    AT+BLEADVDATA
    AT+BLEADVDATAEX
    AT+BLESCANRSPDATA
    AT+BLESCANPARAM
    AT+BTSCANMODE
    AT+BLECONNPARAM

***Note:***

    * The default value of `store_mode` is 1;  
    * `AT+SYSSTORE` only effects on setup command, query command is always got from ram.  

Example:

    AT+SYSSTORE=0
    AT+CWMODE=1  // Do not store into flash
    AT+CWJAP="test","1234567890" // Do not store into flash

    AT+SYSSTORE=1
    AT+CWMODE=3  // Store into flash
    AT+CWJAP="test","1234567890" // Store into flash

<a name="cmd-SYSREG"></a>

### [AT+SYSREG](#Basic-AT)- Read/Write the register interface
Set Command:  

    AT+SYSREG=<direct>,<address>[,<write value>]
Response:

    +SYSREG:<read value>  (Only in read mode)
    OK

Parameters:

- **\<direct>** : read or write register
  - 0 :  Read register
  - 1 :  Write register
- **\<address>** : (uint32)register address, refer to technical reference manual
- **\<write value>** : (uint32)write value (only in write mode)

***Note:***

    * AT does not check address. Make sure that the registers you are operating on are valid

Example:

    AT+SYSREG=1,0x3f40402c,0x2      // Enable ESP32S2 IO33 output, 0x3f40402c means base address 0x3F404000 add relative address 0x2c(GPIO_ENABLE1_REG)
    AT+SYSREG=1,0x3f404010,0x2      // ESP32S2 IO33 output high
    AT+SYSREG=1,0x3f404010,0x0      // ESP32S2 IO33 output low

<a name="cmd-SYSTEMP"></a>
### [ESP32S2 Only] [AT+SYSTEMP](#Basic-AT)— Read ESP32S2 internal celsius temperature  
Query Command:

    AT+SYSTEMP?  
Response:

    +SYSTEMP:<temperature>
    OK  
Parameters:

- **\<temperature>**: The celsius temperature measure output value.

***Note:***

    * Measure range:-10℃ ~  80℃, error < 1℃.

Example:

    AT+SYSTEMP?
    +SYSTEMP:21.59
    OK
