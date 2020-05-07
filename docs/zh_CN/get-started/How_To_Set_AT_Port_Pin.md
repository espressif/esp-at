如何修改 `AT port` 管脚
=============

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
