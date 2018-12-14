# Overview
There are some differences between different development boards, and different countries also have different restrictions on RF. So we need a table to manage these differences.

## Factory param type
The factory param type is shown in the following table:

| param_name    | offset |  type   | size |
| ------------- | ------ | ------- | ---- |
| module_name   |   -1   | String  |   0  |
| magic_flag    |    0   | integer |   2  |
| version       |    2   | integer |   1  |
| module_id     |    3   | integer |   1  |
| tx_max_power  |    4   | integer |   1  |
| start_channel |    6   | integer |   1  |
| channel_num   |    7   | integer |   1  |
| country_code  |    8   | String  |   4  |
| uart_baudrate |   12   | integer |   4  |
| uart_tx_pin   |   16   | integer |   1  |
| uart_rx_pin   |   17   | integer |   1  |
| uart_ctx_pin  |   18   | integer |   1  |
| uart_rts_pin  |   19   | integer |   1  |

 - version:
   - the version of factory param mangement
   
 - module_id
   - the index of development boards
     - 1 - WROOM32
     - 2 - WROVER32
     - 3 - PICO-D4
     - 4 - SOLO
     
 - tx\_max_power
   - Wi-Fi maximum tx power
   
 - start_chanel
   - Wi-Fi start channel
   
 - channel_num
   - the total channel number of Wi-Fi
   
 - country_code
   - Country code
   
 - uart_baudrate
   - uart baudrate
   
 - uart\_tx_pin
   - tx pin
   
 - uart\_rx_pin
   - rx pin
   
 - uart\_ctx_pin
   - ctx pin
   
 - uart\_rts_pin
   - rts pin
   
## Factory param data

Save the factory param date for each development boards in a table, it was organized as bellow:

| module_name | magic_flag | version | module_id | tx_max_power | start_channel | channel_num | country_code | uart_baudrate | uart_tx_pin | uart_rx_pin | uart_ctx_pin | uart_rts_pin |
|---|---|---|---|---|---|---| ---|---|---|---|---|---|
| WROOM-32 |0xfcfc|1|1|1|1|13|CN|115200|17|16|15|14|
| WROVER-32|0xfcfc|1|2|1|1|13|CN|115200|22|19|15|14|
| PICO-D4  |0xfcfc|1|3|1|1|13|CN|115200|22|19|15|14|
| SOLO-1   |0xfcfc|1|4|1|1|13|CN|115200|17|16|15|14|