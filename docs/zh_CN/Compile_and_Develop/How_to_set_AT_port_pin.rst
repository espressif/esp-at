如何修改 ``AT port`` 管脚
=========================

在 ``esp-at`` 工程中，默认使用了两个 UART: UART0 和 UART1. 在有些情况下，用户可能想要修改管脚配置已满足自己的产品需求. 由于 ``esp-at`` 当前可支持 ESP8266 和 ESP32 两个平台，另个平台硬件有些差异，所以 UART 的配置方式也有少许差异.

ESP32 平台
----------

ESP32 的 UART 管脚可以通过管脚映射的方式进行修改, 具体请参见 `ESP32 技术参考手册 <https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_cn.pdf>`__, 在官方 release 固件中，UART0 作为 Log 的打印，默认管脚为

::

    TX ---> GPIO1  
    RX ---> GPIO3  

可以通过 ``./build.py menuconfig`` > ``Component config`` > ``Common ESP-related`` > ``UART for console output`` 进行修改.
UART1 作为 AT 命令通讯使用(只能为 UART1, 但管脚可修改)，默认管脚配置在 ``factory_param.bin`` 中, 可以在 :component:`customized_partitions/raw_data/factory_param/factory_param_data.csv` 文件中修改,不同的模组固件可能管脚不同，关于 ``factory_param_data.csv`` 的含义描述，可参阅 ``ESP_AT_Factory_Parameter_Bin.md``.
比如 ``WROOM-32`` 模组

+----------------+----------------+
| Parameter      | Value          |
+================+================+
| platform       | PLATFORM_ESP32 |
+----------------+----------------+
| module_name    | WROOM-32       |
+----------------+----------------+
| magic_flag     | 0xfcfc         |
+----------------+----------------+
| version        | 1              |
+----------------+----------------+
| reserved1      | 0              |
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
| uart_ctx_pin   | 15             |
+----------------+----------------+
| uart_rts_pin   | 14             |
+----------------+----------------+
| tx_control_pin | -1             |
+----------------+----------------+
| rx_control_pin | -1             |
+----------------+----------------+

发送命令的 AT port 管脚分别为

::

    TX ---> GPIO17  
    RX ---> GPIO16  
    CTS ---> GPIO15  
    RTX ---> GPIO14  

如果想要使用 GPIO1 (TX)、GPIO3 (RX) 同时作为 Log 打印和 AT 命令输入，可以采用如下操作:

1.  打开 :component:`customized_partitions/raw_data/factory_param/factory_param_data.csv` 文件
2.  修改 ``WROOM-32`` 模组的 ``uart_port`` 为 0，\ ``uart_tx_pin`` 为 1 以及 ``uart_rx_pin`` 为 3，如下

    +----------------+----------------+
    | Parameter      | Value          |
    +================+================+
    | platform       | PLATFORM_ESP32 |
    +----------------+----------------+
    | module_name    | WROOM-32       |
    +----------------+----------------+
    | magic_flag     | 0xfcfc         |
    +----------------+----------------+
    | version        | 1              |
    +----------------+----------------+
    | reserved1      | 0              |
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
    | uart_ctx_pin   | -1             |
    +----------------+----------------+
    | uart_rts_pin   | -1             |
    +----------------+----------------+
    | tx_control_pin | -1             |
    +----------------+----------------+
    | rx_control_pin | -1             |
    +----------------+----------------+

3.  然后保存，重新编译固件，并完全烧录固件即可.注意：一定要同时烧录对应的 ``factory_param.bin``.

ESP8266 平台
------------

``ESP8266`` 共有两组 ``UART`` 口，分别为：\ ``UART0`` 和 ``UART1``\ ，其中，\ ``UART1`` 只有\ ``TX``\ 功能（\ ``GPIO2``)。所以只能使用 UART0 作为命令输入口. 由于 ESP8266 UART pin 并不能像 ESP32 那样任意映射，只能使用 GPIO15 作为 TX、GPIO13 作为 RX, 或者使用 GPIO1 作为 TX、GPIO3 作为 RX. 默认 LOG UART 为 UART1, TX 为 GPIO2; AT port UART 为 UART0, TX 为 GPIO15, RX 为 GPIO13.

如果想要使用 GPIO1 (TX)、GPIO3 (RX) 同时作为 Log 打印和 AT 命令输入，可以采用如下操作(WROOM-02 为例):

1.  ``./build.py menuconfig`` > ``Component config`` > ``ESP8266-specific`` > ``UART for console output`` > ``Default: UART0``
2.  修改 :component:`customized_partitions/raw_data/factory_param/factory_param_data.csv` 文件中 ``WROOM-02`` 模组的 ``uart_tx_pin`` 和 ``uart_rx_pin``\ 分别为 1 和 3，如下

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
    | uart_ctx_pin   | -1               |
    +----------------+------------------+
    | uart_rts_pin   | -1               |
    +----------------+------------------+
    | ...            | ...              |
    +----------------+------------------+

3.  然后保存，重新编译固件，并完全烧录固件即可.注意：一定要同时烧录对应的 ``factory_param.bin``.

ESP32-C3 平台
--------------

ESP32-C3 的 UART 管脚可以通过管脚映射的方式进行修改, 具体请参见 `ESP32-C3 系列芯片技术规格书 <https://www.espressif.com/sites/default/files/documentation/esp32-c3_datasheet_cn.pdf>`__, 在官方 release 固件中，UART0 作为 Log 的打印，默认管脚为

::

    TX ---> GPIO21  
    RX ---> GPIO20 

可以通过 ``./build.py menuconfig`` > ``Component config`` > ``Common ESP-related`` > ``Channel for console output`` 进行修改.
UART1 作为 AT 命令通讯使用(只能为 UART1, 但管脚可修改)，默认管脚配置在 ``factory_param.bin`` 中, 可以在 :component:`customized_partitions/raw_data/factory_param/factory_param_data.csv` 文件中修改,不同的模组固件可能管脚不同，关于 ``factory_param_data.csv`` 的含义描述，可参阅 ``ESP_AT_Factory_Parameter_Bin.md``.
比如 ``MINI-1`` 模组

+----------------+------------------+
| Parameter      | Value            |
+================+==================+
| platform       | PLATFORM_ESP32C3 |
+----------------+------------------+
| module_name    | MINI-1           |
+----------------+------------------+
| magic_flag     | 0xfcfc           |
+----------------+------------------+
| version        | 2                |
+----------------+------------------+
| module_id      | 1                |
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
| uart_tx_pin    | 7                |
+----------------+------------------+
| uart_rx_pin    | 6                |
+----------------+------------------+
| uart_cts_pin   | 5                |
+----------------+------------------+
| uart_rts_pin   | 4                |
+----------------+------------------+
| tx_control_pin | -1               |
+----------------+------------------+
| rx_control_pin | -1               |
+----------------+------------------+

发送命令的 AT port 管脚分别为

::

    TX ---> GPIO7  
    RX ---> GPIO6  
    CTS ---> GPIO5  
    RTX ---> GPIO4  

如果想要使用 GPIO21 (TX)、GPIO20 (RX) 同时作为 Log 打印和 AT 命令输入，可以采用如下操作:

1.  打开 :component:`customized_partitions/raw_data/factory_param/factory_param_data.csv` 文件
2.  修改 ``MINI-1`` 模组的 ``uart_port`` 为 0，\ ``uart_tx_pin`` 为 21 以及 ``uart_rx_pin`` 为 20，如下

    +----------------+------------------+
    | Parameter      | Value            |
    +================+==================+
    | platform       | PLATFORM_ESP32C3 |
    +----------------+------------------+
    | module_name    | MINI-1           |
    +----------------+------------------+
    | magic_flag     | 0xfcfc           |
    +----------------+------------------+
    | version        | 2                |
    +----------------+------------------+
    | module_id      | 1                |
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
    | uart_tx_pin    | 21               |
    +----------------+------------------+
    | uart_rx_pin    | 20               |
    +----------------+------------------+
    | uart_cts_pin   | -1               |
    +----------------+------------------+
    | uart_rts_pin   | -1               |
    +----------------+------------------+
    | tx_control_pin | -1               |
    +----------------+------------------+
    | rx_control_pin | -1               |
    +----------------+------------------+

3.  然后保存，重新编译固件，并完全烧录固件即可.注意：一定要同时烧录对应的 ``factory_param.bin``.