How to Set `AT Port` Pin
=============

In the project `esp-at`, UART0 and UART1 are used by default. And users can change those UART pins according to their actual hardware design. Since the `esp-at` supports both ESP32 and ESP8266, there are some differences between the configurations.


## ESP32 AT
The UART pin of ESP32 can be user-defined to other pins, refer to [`esp32_technical_reference_manual_en.pdf`](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf).   
In the official Espressif ESP32 AT bin, UART0 is the default port to print log, using the following pins:  

```
TX ---> GPIO1  
RX ---> GPIO3  
```
The log pins can be set in `make menuconfig` > `Component config` > `ESP32-specific` > `UART for console output`.   
UART1 is for sending AT commands and receiving response, but its pins can be changed. The pins of UART1 is in the `factory_param.bin`, they can be changed in the `esp-at/components/customized_partitions/raw_data/factory_param/factory_param_data.csv`. The UART1 pins may be different for different ESP modules. More details of `factory_param_data.csv` are in the [`ESP_AT_Factory_Parameter_Bin.md`](ESP_AT_Factory_Parameter_Bin.md).    
  
For example, the configuration of the `ESP32-WROOM-32` is as the following table.
 
| platform | module_name | magic_flag | version | module_id | tx_max_power | uart_port | start_channel | channel_num | country_code | uart_baudrate | uart\_tx_pin | uart\_rx_pin | uart\_ctx_pin | uart\_rts_pin | tx\_control_pin | rx\_control_pin
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:| :---:|:---:|:---:|:---:|:---:|:---:|
PLATFORM_ESP32 |	WROOM-32|	0xfcfc	|1|	1|78|1|1|13|CN|115200|17|16|15|14|-1|-1

In this case, the pins of `ESP32-WROOM-32` AT port is:  

```
TX ---> GPIO17  
RX ---> GPIO16  
CTS ---> GPIO15  
RTX ---> GPIO14  
```
For example, if you need to set GPIO1(TX) and GPIO3(RX) to be both the log pin and AT port pin, then you can set it as the following steps.  

1. Open `esp-at/components/customized_partitions/raw_data/factory_param/factory_param_data.csv`
2. Choose the line of `WROOM-32`, set `uart_port` to be 0, `uart_tx_pin` to be 1 and `uart_rx_pin` to be 3, and then save it.   

| platform | module_name | magic_flag | version | module_id | tx_max_power | uart_port | start_channel | channel_num | country_code | uart_baudrate | uart\_tx_pin | uart\_rx_pin | uart\_ctx_pin | uart\_rts_pin | tx\_control_pin | rx\_control_pin
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:| :---:|:---:|:---:|:---:|:---:|:---:|
PLATFORM_ESP32 |	WROOM-32|	0xfcfc	|1|	1|78|0|1|13|CN|115200|1|3|-1|-1|-1|-1

6. Recompile the `esp-at` project, download the new `factory_param.bin` and AT bin into flash.

## ESP8266 AT

ESP8266 has two UART ports, UART0 and UART1. UART1 only supports TX pin to print debug log. UART0 has both TX and RX pin, to send AT commands and receive response. Unlike ESP32, UART0 pins of ESP8266 cannot be set to any pins, there are only two choice, `GPIO15 as TX pin, GPIO13 as RX` or `GPIO1 as TX、GPIO3 as RX`.   
The default setting of ESP8266 AT UART is：

- Use UART0 is the AT port to send/receive AT commands/responses. GPIO15 is the UART0 TX, GPIO13 is the UART0 RX.
- Use UART1 to print debug log, GPIO2 is the UART1 TX pin.

For example, if you need to set GPIO1(TX) and GPIO3(RX) of ESP-WROOM-02 to be both the log pin and AT port pin, then you can set it as the following steps.
 
1. `make menuconfig` > `Component config` > `ESP8266-specific` > `UART for console output` > `Default: UART0`
2. Open `esp-at/components/customized_partitions/raw_data/factory_param/factory_param_data.csv`, choose the line of `WROOM-02`, set `uart_tx_pin` to be 1 and `uart_rx_pin` to be 3, and then save it.

| platform | module_name | magic_flag | ... | uart_baudrate | uart\_tx_pin | uart\_rx_pin | uart\_ctx_pin | uart\_rts_pin |...
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
PLATFORM_ESP8266 |	WROOM-02|	0xfcfc	|...|115200|1|3|-1|-1|...

3. Recompile the `esp-at` project, download the new `factory_param.bin` and AT bin into flash.
