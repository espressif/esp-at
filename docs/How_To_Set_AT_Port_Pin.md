# How to Set `AT Port` Pin

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


# 如何修改 `AT port` 管脚

在 `esp-at` 工程中，默认使用了两个 UART: UART0 和 UART1. 在有些情况下，用户可能想要修改管脚配置已满足自己的产品需求. 由于 `esp-at` 当前可支持 ESP8266 和 ESP32 两个平台，另个平台硬件有些差异，所以 UART 的配置方式也有少许差异.

## ESP32 平台
ESP32 的 UART 管脚可以通过管脚映射的方式进行修改, 具体请参见 [`esp32_technical_reference_manual_cn.pdf`](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_cn.pdf), 在官方 release 固件中，UART0 作为 Log 的打印，默认管脚为  

```
TX ---> GPIO1  
RX ---> GPIO3  
```
可以通过 `make menuconfig` > `Component config` > `ESP32-specific` > `UART for console output` 进行修改. 
UART1 作为 AT 命令通讯使用(只能为 UART1, 但管脚可修改)，默认管脚配置在 `factory_param.bin` 中, 可以在 `esp-at/components/customized_partitions/raw_data/factory_param/factory_param_data.csv` 文件中修改,不同的模组固件可能管脚不同，关于 `factory_param_data.csv` 的含义描述，可参阅 [`ESP_AT_Factory_Parameter_Bin.md`](ESP_AT_Factory_Parameter_Bin.md).  
比如 `WROOM-32` 模组
 
| platform | module_name | magic_flag | version | module_id | tx_max_power | uart_port | start_channel | channel_num | country_code | uart_baudrate | uart\_tx_pin | uart\_rx_pin | uart\_ctx_pin | uart\_rts_pin | tx\_control_pin | rx\_control_pin
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:| :---:|:---:|:---:|:---:|:---:|:---:|
PLATFORM_ESP32 |	WROOM-32|	0xfcfc	|1|	1|78|1|1|13|CN|115200|17|16|15|14|-1|-1

发送命令的 AT port 管脚分别为  

```
TX ---> GPIO17  
RX ---> GPIO16  
CTS ---> GPIO15  
RTX ---> GPIO14  
```

如果想要使用 GPIO1(TX)、GPIO3(RX) 同时作为 Log 打印和 AT 命令输入，可以采用如下操作:  

1. 打开 `esp-at/components/customized_partitions/raw_data/factory_param/factory_param_data.csv` 文件
2. 修改 `WROOM-32` 模组的 `uart_port` 为 0，`uart_tx_pin` 为 1 以及 `uart_rx_pin` 为 3，如下  

| platform | module_name | magic_flag | version | module_id | tx_max_power | uart_port | start_channel | channel_num | country_code | uart_baudrate | uart\_tx_pin | uart\_rx_pin | uart\_ctx_pin | uart\_rts_pin | tx\_control_pin | rx\_control_pin
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:| :---:|:---:|:---:|:---:|:---:|:---:|
PLATFORM_ESP32 |	WROOM-32|	0xfcfc	|1|	1|78|0|1|13|CN|115200|1|3|-1|-1|-1|-1

然后保存，重新编译固件，并完全烧录固件即可.注意：一定要同时烧录对应的 `factory_param.bin`.

## ESP8266 平台

`ESP8266` 共有两组 `UART` 口，分别为：`UART0` 和 `UART1`，其中，`UART1` 只有`TX`功能（`GPIO2`)。所以只能使用 UART0 作为命令输入口. 由于 ESP8266 UART pin 并不能像 ESP32 那样任意映射，只能使用 GPIO15 作为 TX、GPIO13 作为 RX, 或者使用 GPIO1 作为 TX、GPIO3 作为 RX. 默认 LOG UART 为 UART1, TX 为 GPIO2; AT port UART 为 UART0, TX 为 GPIO15, RX 为 GPIO13. 
 
如果想要使用 GPIO1(TX)、GPIO3(RX) 同时作为 Log 打印和 AT 命令输入，可以采用如下操作(WROOM-02 为例):  
1. `make menuconfig` > `Component config` > `ESP8266-specific` > `UART for console output` > `Default: UART0`
2. 修改 `esp-at/components/customized_partitions/raw_data/factory_param/factory_param_data.csv` 文件中 `WROOM-02` 模组的 `uart_tx_pin ` 和 `uart_rx_pin `分别为 1 和 3，如下  

| platform | module_name | magic_flag | ... | uart_baudrate | uart_tx_pin | uart_rx_pin | uart_ctx_pin | uart_rts_pin |...
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
PLATFORM_ESP8266 |	WROOM-02|	0xfcfc	|...|115200|1|3|-1|-1|...
