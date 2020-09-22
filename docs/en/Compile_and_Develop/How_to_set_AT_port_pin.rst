How to Set ``AT Port`` Pin
==========================

In the project ``esp-at``, UART0 and UART1 are used by default. And users can change those UART pins according to their actual hardware design. Since the ``esp-at`` supports both ESP32 and ESP8266, there are some differences between the configurations.

ESP32 AT
--------

The UART pin of ESP32 can be user-defined to other pins, refer to `ESP32 Technical Reference Manual <https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf>`__. In the official Espressif ESP32 AT bin, UART0 is the default port to print log, using the following pins:

::

    TX ---> GPIO1  
    RX ---> GPIO3  

The log pins can be set in ``make menuconfig`` > ``Component config`` > ``Common ESP-related`` > ``UART for console output``.
UART1 is for sending AT commands and receiving response, but its pins can be changed. The pins of UART1 is in the ``factory_param.bin``, they can be changed in the component file :component:`customized_partitions/raw_data/factory_param/factory_param_data.csv`. The UART1 pins may be different for different ESP modules. More details of ``factory_param_data.csv`` are in the ``How_to_create_factory_parameter_bin.md``.

For example, the configuration of the ``ESP32-WROOM-32`` is as the following table.

+----------------+----------------+
| Parameter      | Value          |
+================+================+
| platform       | PLATFORM_ESP32 |
+----------------+----------------+
| module_name    | WROOM-32       |
+----------------+----------------+
| magic_flag     | 0xfcfc         |
+----------------+----------------+
| version        | 2              |
+----------------+----------------+
| module_id      | 1              |
+----------------+----------------+
| tx_max_power   | 78             |
+----------------+----------------+
| uart_port      | 1              |
+----------------+----------------+
| start_channel  | 1              |
+----------------+----------------+
| channel_num    | 13             |
+----------------+----------------+
| country_code   | CN             |
+----------------+----------------+
| uart_baudrate  | 115200         |
+----------------+----------------+
| uart_tx_pin    | 17             |
+----------------+----------------+
| uart_rx_pin    | 16             |
+----------------+----------------+
| uart_cts_pin   | 15             |
+----------------+----------------+
| uart_rts_pin   | 14             |
+----------------+----------------+
| tx_control_pin | -1             |
+----------------+----------------+
| rx_control_pin | -1             |
+----------------+----------------+

In this case, the pins of ``ESP32-WROOM-32`` AT port is:

::

    TX ---> GPIO17  
    RX ---> GPIO16  
    CTS ---> GPIO15  
    RTS ---> GPIO14  

For example, if you need to set GPIO1 (TX) and GPIO3 (RX) to be both the log pin and AT port pin, then you can set it as the following steps.

1.  Open component file :component:`customized_partitions/raw_data/factory_param/factory_param_data.csv`.
2.  Choose the line of ``WROOM-32``, set ``uart_port`` to be 0, ``uart_tx_pin`` to be 1 and ``uart_rx_pin`` to be 3, and then save it.

    +----------------+----------------+
    | Parameter      | Value          |
    +================+================+
    | platform       | PLATFORM_ESP32 |
    +----------------+----------------+
    | module_name    | WROOM-32       |
    +----------------+----------------+
    | magic_flag     | 0xfcfc         |
    +----------------+----------------+
    | version        | 2              |
    +----------------+----------------+
    | module_id      | 1              |
    +----------------+----------------+
    | tx_max_power   | 78             |
    +----------------+----------------+
    | uart_port      | 0              |
    +----------------+----------------+
    | start_channel  | 1              |
    +----------------+----------------+
    | channel_num    | 13             |
    +----------------+----------------+
    | country_code   | CN             |
    +----------------+----------------+
    | uart_baudrate  | 115200         |
    +----------------+----------------+
    | uart_tx_pin    | 1              |
    +----------------+----------------+
    | uart_rx_pin    | 3              |
    +----------------+----------------+
    | uart_cts_pin   | -1             |
    +----------------+----------------+
    | uart_rts_pin   | -1             |
    +----------------+----------------+
    | tx_control_pin | -1             |
    +----------------+----------------+
    | rx_control_pin | -1             |
    +----------------+----------------+

3. Recompile the ``esp-at`` project, download the new ``factory_param.bin`` and AT bin into flash.

ESP8266 AT
----------

ESP8266 has two UART ports, UART0 and UART1. UART1 only supports TX pin to print debug log. UART0 has both TX and RX pin, to send AT commands and receive response. Unlike ESP32, UART0 pins of ESP8266 cannot be set to any pins, there are only two choice, ``GPIO15 as TX pin, GPIO13 as RX`` or ``GPIO1 as TX、GPIO3 as RX``.

The default setting of ESP8266 AT UART is：

-  Use UART0 is the AT port to send/receive AT commands/responses. GPIO15 is the UART0 TX, GPIO13 is the UART0 RX.
-  Use UART1 to print debug log, GPIO2 is the UART1 TX pin.

For example, if you need to set GPIO1 (TX) and GPIO3 (RX) of ESP-WROOM-02 to be both the log pin and AT port pin, then you can set it as the following steps.

1.  ``make menuconfig`` > ``Component config`` > ``ESP8266-specific`` > ``UART for console output`` > ``Default: UART0``
2.  Open component file :component:`customized_partitions/raw_data/factory_param/factory_param_data.csv`, choose the line of ``WROOM-02``, set ``uart_tx_pin`` to be 1 and ``uart_rx_pin`` to be 3, and then save it.

    +----------------+------------------+
    | Parameter      | Value            |
    +================+==================+
    | platform       | PLATFORM_ESP8266 |
    +----------------+------------------+
    | module_name    | WROOM-02         |
    +----------------+------------------+
    | magic_flag     | 0xfcfc           |
    +----------------+------------------+
    | ...            | ...              |
    +----------------+------------------+
    | uart_baudrate  | 115200           |
    +----------------+------------------+
    | uart_tx_pin    | 1                |
    +----------------+------------------+
    | uart_rx_pin    | 3                |
    +----------------+------------------+
    | uart_cts_pin   | -1               |
    +----------------+------------------+
    | uart_rts_pin   | -1               |
    +----------------+------------------+
    | ...            | ...              |
    +----------------+------------------+

3.  Recompile the ``esp-at`` project, download the new ``factory_param.bin`` and AT bin into flash.

ESP32S2 AT
----------
The UART pin of ESP32S2 can be user-defined to other pins, refer to `ESP32S2 Technical Reference Manual <https://www.espressif.com/sites/default/files/documentation/esp32-s2_technical_reference_manual_en.pdf>`__. In the official Espressif ESP32S2 AT bin, UART0 is the default port to print log, using the following pins:

::

    TX ---> GPIO43  
    RX ---> GPIO44 

The log pins can be set in ``make menuconfig`` > ``Component config`` > ``Common ESP-related`` > ``UART for console output``.
UART1 is for sending AT commands and receiving response, but its pins can be changed. The pins of UART1 are configured in the ``factory_param.bin``, they can be changed in the component file :component:`customized_partitions/raw_data/factory_param/factory_param_data.csv`. The UART1 pins may be different for different ESP modules. More details of ``factory_param_data.csv`` are in the ``How_to_create_factory_parameter_bin.md``.

For example, the configuration of the ``ESP32S2-WROVER`` is as the following table.

+----------------+------------------+
| Parameter      | Value            |
+================+==================+
| platform       | PLATFORM_ESP32S2 |
+----------------+------------------+
| module_name    | WROVER           |
+----------------+------------------+
| magic_flag     | 0xfcfc           |
+----------------+------------------+
| version        | 2                |
+----------------+------------------+
| module_id      | 0                |
+----------------+------------------+
| tx_max_power   | 78               |
+----------------+------------------+
| uart_port      | 1                |
+----------------+------------------+
| start_channel  | 1                |
+----------------+------------------+
| channel_num    | 13               |
+----------------+------------------+
| country_code   | CN               |
+----------------+------------------+
| uart_baudrate  | 115200           |
+----------------+------------------+
| uart_tx_pin    | 17               |
+----------------+------------------+
| uart_rx_pin    | 21               |
+----------------+------------------+
| uart_cts_pin   | 20               |
+----------------+------------------+
| uart_rts_pin   | 19               |
+----------------+------------------+
| tx_control_pin | -1               |
+----------------+------------------+
| rx_control_pin | -1               |
+----------------+------------------+

In this case, the pins of ``ESP32S2-WROVER`` AT port is:

::

    TX ---> GPIO17  
    RX ---> GPIO21  
    CTS ---> GPIO20  
    RTS ---> GPIO19  

For example, if you need to set GPIO43 (TX) and GPIO44 (RX) to be both the log pin and AT port pin, then you can set it as the following steps.

1.  Open component file :component:`customized_partitions/raw_data/factory_param/factory_param_data.csv`.
2.  Choose the line of ``WROVER``, set ``uart_port`` to be 0, ``uart_tx_pin`` to be 43 and ``uart_rx_pin`` to be 44, and then save it.

    +----------------+------------------+
    | Parameter      | Value            |
    +================+==================+
    | platform       | PLATFORM_ESP32S2 |
    +----------------+------------------+
    | module_name    | WROVER           |
    +----------------+------------------+
    | magic_flag     | 0xfcfc           |
    +----------------+------------------+
    | version        | 2                |
    +----------------+------------------+
    | module_id      | 0                |
    +----------------+------------------+
    | tx_max_power   | 78               |
    +----------------+------------------+
    | uart_port      | 0                |
    +----------------+------------------+
    | start_channel  | 1                |
    +----------------+------------------+
    | channel_num    | 13               |
    +----------------+------------------+
    | country_code   | CN               |
    +----------------+------------------+
    | uart_baudrate  | 115200           |
    +----------------+------------------+
    | uart_tx_pin    | 43               |
    +----------------+------------------+
    | uart_rx_pin    | 44               |
    +----------------+------------------+
    | uart_cts_pin   | -1               |
    +----------------+------------------+
    | uart_rts_pin   | -1               |
    +----------------+------------------+
    | tx_control_pin | -1               |
    +----------------+------------------+
    | rx_control_pin | -1               |
    +----------------+------------------+

3. Recompile the ``esp-at`` project, download the new ``factory_param.bin`` and AT bin into flash.
4. If you don't want to compile the entire project in the third step, you can refer to ``How_to_create_factory_parameter_bin.md``.
