AT Command Set Comparison
=========================

:link_to_translation:`zh_CN:[中文]`

This document lists the differences between the AT commands supported by the old NONOS-AT version and those by the new ESP-AT version that you should pay attention to before migrating from the old to new.

- NONOS-AT: **Not recommended** version for ESP8266 series of chips since its base SDK, ESP8266_NONOS_SDK, is no longer updated.
- ESP-AT: **Recommended** version. It supports all series of chips and a richer set of commands compared with NONOS-AT, such as Bluetooth commands, Bluetooth Low Energy commands, Ethernet commands, driver commands, and so on. For more details about ESP-AT, please refer to :doc:`../Get_Started/What_is_ESP-AT` and :doc:`../AT_Binary_Lists/index`.

.. note::
  The table below covers all the commands supported by the NONOS-AT, but not all by ESP-AT. See Section :doc:`AT Command Set <index>` for a full list of commands supported by ESP-AT. 

  The links in the first column of the table point to ESP-AT commands.

.. list-table:: AT Command Set Comparison
   :header-rows: 1
   :widths: 25 30 15 30

   * - Command
     - Function
     - NONOS-AT
     - ESP-AT
   * - :ref:`AT <cmd-AT>`
     - Test AT startup
     - ✅
     - ✅
   * - :ref:`AT+RST <cmd-RST>`
     - Restart a module
     - ✅
     - ✅
   * - :ref:`AT+GMR <cmd-GMR>`
     - Check version information
     - ✅
     - ✅
   * - :ref:`AT+GSLP <cmd-GSLP>`
     - Enter Deep-sleep mode
     - ✅
     - ✅
   * - :ref:`ATE <cmd-ATE>`
     - AT commands echoing
     - ✅
     - ✅
   * - :ref:`AT+RESTORE <cmd-RESTORE>`
     - Restore factory default settings
     - ✅
     - ✅
   * - :ref:`AT+UART_CUR <cmd-UARTC>`
     - Current UART configuration, not saved in flash
     - ✅
     - ✅
   * - :ref:`AT+UART_DEF <cmd-UARTD>`
     - Default UART configuration, saved in flash
     - ✅
     - ✅
   * - :ref:`AT+SLEEP <cmd-SLEEP>`
     - Set the sleep mode
     - 
       | ✅
       | 1: light sleep
       | 2: modem sleep
     - | ESP8266 ✅
       | ESP32 ✅
       | ESP32-S2 ❌
       | Note [#one]_
   * - AT+WAKEUPGPIO
     - Wakeup from light sleep on GPIO interrupt
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+SLEEPWKCFG <cmd-WKCFG>`
   * - :ref:`AT+RFPOWER <cmd-RFPOWER>`
     - Set RF TX Power
     - | ✅
       | Range: [0,82]
       | Unit: 0.25 dBm
     - ✅ [#two]_
   * - AT+RFVDD
     - Set RF TX power according to VDD33
     - ✅
     - ❌
   * - :ref:`AT+SYSRAM <cmd-SYSRAM>`
     - Query the remaining space of RAM
     - ✅
     - ✅ Added the parameter of minimum heap size
   * - AT+SYSADC
     - Read ADC value
     - ✅
     - ❌
   * - AT+SYSIOSETCFG
     - Set IO working mode
     - ✅
     - ❌
   * - AT+SYSIOGETCFG
     - Query IO working mode
     - ✅
     - ❌
   * - AT+SYSGPIODIR
     - Set GPIO direction
     - ✅
     - ❌
   * - AT+SYSGPIOWRITE
     - Set GPIO output level
     - ✅
     - ❌
   * - AT+SYSGPIOREAD
     - Read GPIO input level
     - ✅
     - ❌
   * - :ref:`AT+SYSMSG <cmd-SYSMSG>`
     - Set system prompt info
     - ❌
     - ✅
   * - AT+SYSMSG_CUR
     - Set system prompt info, not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+SYSMSG <cmd-SYSMSG>`
   * - AT+SYSMSG_DEF
     - Set default system prompt info, saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+SYSMSG <cmd-SYSMSG>`
   * - :ref:`AT+CWMODE <cmd-MODE>`
     - Set Wi-Fi mode
     - ❌
     - ✅ Added automatic connection after mode switching
   * - AT+CWMODE_CUR
     - Set current Wi-Fi mode, not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWMODE <cmd-MODE>`
   * - AT+CWMODE_DEF
     - Set default Wi-Fi mode, saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWMODE <cmd-MODE>`
   * - :ref:`AT+CWJAP <cmd-JAP>`
     - Connect to an AP
     - ✅
     - ✅ Added more functionality
   * - AT+CWJAP_CUR
     - Connect to an AP, not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWJAP <cmd-JAP>`
   * - AT+CWJAP_DEF
     - Connect to an AP, saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWJAP <cmd-JAP>`
   * - :ref:`AT+CWLAPOPT <cmd-LAPOPT>`
     - Set configuration for :ref:`AT+CWLAP <cmd-LAP>`
     - ✅
     - ✅ Added more functionality
   * - :ref:`AT+CWLAP <cmd-LAP>`
     - List available APs
     - ✅
     - ✅ Response is different
   * - :ref:`AT+CWQAP <cmd-QAP>`
     - Disconnect from an AP
     - ✅
     - ✅
   * - :ref:`AT+CWSAP <cmd-SAP>`
     - Set softAP parameters
     - ❌
     - ✅
   * - AT+CWSAP_CUR
     - Set softAP parameters, not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWSAP <cmd-SAP>`
   * - AT+CWSAP_DEF
     - Set softAP parameters, saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWSAP <cmd-SAP>`
   * - :ref:`AT+CWLIF <cmd-LIF>`
     - Query info of the station that connects to a softAP
     - ✅
     - ✅
   * - :ref:`AT+CWDHCP <cmd-DHCP>`
     - Set DHCP
     - ❌
     - ✅
   * - AT+CWDHCP_CUR
     - Set DHCP, not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWDHCP <cmd-DHCP>`
   * - AT+CWDHCP_DEF
     - Set DHCP, saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWDHCP <cmd-DHCP>`
   * - :ref:`AT+CWDHCPS <cmd-DHCPS>`
     - Set the IP addresses allocated by an ESP softAP DHCP server
     - ❌
     - ✅
   * - AT+CWDHCPS_CUR
     - Same as above, but not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWDHCPS <cmd-DHCPS>`
   * - AT+CWDHCPS_DEF
     - Same as above, but saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWDHCPS <cmd-DHCPS>`
   * - :ref:`AT+CWAUTOCONN <cmd-AUTOC>`
     - Connect to an AP automatically when powered on
     - ✅
     - ✅
   * - :ref:`AT+CIPSTAMAC <cmd-STAMAC>`
     - Set the MAC address of an ESP station
     - ❌
     - ✅
   * - AT+CIPSTAMAC_CUR
     - Same as above, but not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CIPSTAMAC <cmd-STAMAC>`
   * - AT+CIPSTAMAC_DEF
     - Same as above, but saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CIPSTAMAC <cmd-STAMAC>`
   * - :ref:`AT+CIPAPMAC <cmd-APMAC>`
     - Set the IP address of an ESP softAP
     - ❌
     - ✅
   * - AT+CIPAPMAC_CUR
     - Same as above, but not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CIPAPMAC <cmd-APMAC>`
   * - AT+CIPAPMAC_DEF
     - Same as above, but saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CIPAPMAC <cmd-APMAC>`
   * - :ref:`AT+CIPSTA <cmd-IPSTA>`
     - Set the IP address of an ESP station
     - ❌
     - ✅
   * - AT+CIPSTA_CUR
     - Same as above, but not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CIPSTA <cmd-IPSTA>`
   * - AT+CIPSTA_DEF
     - Same as above, but saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CIPSTA <cmd-IPSTA>`
   * - :ref:`AT+CIPAP <cmd-IPAP>`
     - Set the IP address of an ESP softAP
     - ❌
     - ✅
   * - AT+CIPAP_CUR
     - Same as above, but not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CIPAP <cmd-IPAP>`
   * - AT+CIPAP_DEF
     - Same as above, but saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CIPAP <cmd-IPAP>`
   * - :ref:`AT+CWSTARTSMART <cmd-STARTS>`
     - Start SmartConfig
     - ✅
     - ✅
   * - :ref:`AT+CWSTOPSMART <cmd-STOPS>`
     - Stop SmartConfig
     - ✅
     - ✅
   * - AT+CWSTARTDISCOVER
     - Enable the mode that an ESP device can be found by WeChat
     - ✅
     - ❌
   * - AT+CWSTOPDISCOVER
     - Disable the mode that an ESP device can be found by WeChat
     - ✅
     - ❌
   * - :ref:`AT+WPS <cmd-WPS>`
     - Enable the WPS function
     - ✅
     - ✅
   * - :ref:`AT+MDNS <cmd-MDNS>`
     - Configure the mDNS function
     - ✅
     - ✅
   * - :ref:`AT+CWJEAP <cmd-JEAP>`
     - Connect to a WPA2 Enterprise AP
     - ❌
     -
       | ESP8266 ❌
       | ESP32 ✅
       | ESP32-S2 ❌
   * - AT+CWJEAP_CUR
     - Same as above, but not saved in flash
     - ✅
     - ❌
   * - AT+CWJEAP_DEF
     - Same as above, but saved in flash
     - ✅
     - ❌
   * - :ref:`AT+CWHOSTNAME <cmd-HOSTNAME>`
     - Set the host name of an ESP station
     - ✅
     - ✅
   * - :ref:`AT+CWCOUNTRY <cmd-COUNTRY>`
     - Set the Wi-Fi Country Code
     - ❌
     - ✅
   * - AT+CWCOUNTRY_CUR
     - Same as above, but not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWCOUNTRY <cmd-COUNTRY>`
   * - AT+CWCOUNTRY_DEF
     - Same as above, but saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CWCOUNTRY <cmd-COUNTRY>`
   * - :ref:`AT+CIPSTATUS <cmd-STATUS>`
     - Obtain the TCP/UDP/SSL connection status and info
     - ✅
     - ✅
   * - :ref:`AT+CIPDOMAIN <cmd-DOMAIN>`
     - Resolve a domain name
     - ✅
     - ✅
   * - :ref:`AT+CIPSTART <cmd-START>`
     - Establish a TCP/UDP/SSL connection
     - ✅
     - ✅
   * - AT+CIPSSLSIZE
     - Set SSL buffer size
     - ✅
     - ❌
   * - :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
     - Set SSL clients
     - ✅
     - ✅ Parameters are different
   * - :ref:`AT+CIPSEND <cmd-SEND>`
     - Send data
     - ✅
     - ✅
   * - :ref:`AT+CIPSENDEX <cmd-SENDEX>`
     - Send data of specified length, or use ``\0`` to trigger data transmission
     - ✅
     - ✅
   * - AT+CIPSENDBUF
     - Write data into the TCP-Send-Buffer
     - ✅
     - ❌ [#three]_
   * - AT+CIPBUFRESET
     - Reset the segment ID count
     - ✅
     - ❌ [#three]_
   * - AT+CIPBUFSTATUS
     - Query the status of the TCP-Send-Buffer
     - ✅
     - ❌ [#three]_
   * - AT+CIPCHECKSEQ
     - Query if a specific segment was successfully sent
     - ✅
     - ❌ [#three]_
   * - AT+CIPCLOSEMODE
     - Set the close mode of TCP connection
     - ✅
     - ❌ [#three]_
   * - :ref:`AT+CIPCLOSE <cmd-CLOSE>`
     - Close TCP/UDP/SSL connection
     - ✅
     - ✅
   * - :ref:`AT+CIFSR <cmd-IFSR>`
     - Obtain the local IP address
     - ✅
     - ✅
   * - :ref:`AT+CIPMUX <cmd-MUX>`
     - Set multiple connections
     - ✅
     - ✅
   * - :ref:`AT+CIPSERVER <cmd-SERVER>`
     - Create a TCP/SSL server
     - ✅ SSL server not supported
     -
       ✅ SSL server supported for ESP32 and ESP32-S2, not for ESP8266
   * - :ref:`AT+CIPSERVERMAXCONN <cmd-SERVERMAX>`
     - Set the maximum connections allowed by a server
     - ✅
     - ✅
   * - :ref:`AT+CIPMODE <cmd-IPMODE>`
     - Set the transmission mode
     - ✅
     - ✅
   * - :ref:`AT+SAVETRANSLINK <cmd-SAVET>`
     - Set whether to enter Wi-Fi :term:`Passthrough Mode` on power-up
     - ✅
     - ✅
   * - :ref:`AT+CIPSTO <cmd-STO>`
     - Set the local TCP server timeout
     - ✅
     - ✅
   * - :ref:`AT+PING <cmd-CIPPING>`
     - Ping the remote host
     - ✅
     - ✅
   * - :ref:`AT+CIUPDATE <cmd-UPDATE>`
     - Upgrade the firmware through Wi-Fi
     - ✅
     - ✅ More parameters supported
   * - :ref:`AT+CIPDINFO <cmd-IPDINFO>`
     - Show the remote IP and port with +IPD
     - ✅
     - ✅
   * - :ref:`AT+CIPRECVMODE <cmd-CIPRECVMODE>`
     - Set TCP Receive Mode
     - ✅
     - ✅
   * - :ref:`AT+CIPRECVDATA <cmd-CIPRECVDATA>`
     - Get TCP data in passive receive mode
     - ✅
     - ✅ Response is different
   * - :ref:`AT+CIPRECVLEN <cmd-CIPRECVLEN>`
     - Get TCP data length in passive receive mode
     - ✅
     - ✅
   * - :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>`
     - Set the time zone and SNTP server
     - ✅
     - ✅ Added more functionality
   * - :ref:`AT+CIPSNTPTIME <cmd-SNTPT>`
     - Query SNTP time
     - ✅
     - ✅
   * - :ref:`AT+CIPDNS <cmd-DNS>`
     - Set DNS server information
     - ❌
     - ✅
   * - AT+CIPDNS_CUR
     - Same as above, but not saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CIPDNS <cmd-DNS>`
   * - AT+CIPDNS_DEF
     - Same as above, but saved in flash
     - ✅
     - | ❌ [#three]_ Similar command: :ref:`AT+CIPDNS <cmd-DNS>`
   * - :ref:`AT+SYSFLASH <cmd-SYSFLASH>`
     - Set user partitions in flash
     - ❌
     - ✅

Tip: Click the footnote number to jump (back) to where it is marked in the table above.

.. [#one] AT+SLEEP in ESP-AT:

       * ESP8266 and ESP32 ✅
       
         * 1: modem sleep by DTIM
         * 2: light sleep
         * 3: modem sleep by listen interval
       
       * ESP32-S2 ❌
.. [#two] AT+RFPOWER in ESP-AT:

       * ESP8266 ✅. Range: [40,82]. Unit: 0.25 dBm
       * ESP32 ✅. Range: [40,78]. Unit: 0.25 dBm. Support Bluetooth LE.
       * ESP32-S2 ✅. Range: [40,78]. Unit: 0.25 dBm

.. [#three] This command will not be added to the ESP-AT version.