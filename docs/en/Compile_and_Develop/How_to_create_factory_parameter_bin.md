How To Create Factory Parameter Bin
===================================

## Overview
In order to adapt the AT firmware to different requirements, for example, different development board, different country code, different RF restriction, we make a table to configure those parameters.

## Factory param type 

The origin table is `components/customized_partitions/raw_data/factory_param/factory_param_type.csv`, and the factory parameter type is as the following table:

| param_name    | offset |  type   | size |
| ------------- | ------ | ------- | ---- |
| platform      |   -1   | String  |   0  |
| module_name   |   -1   | String  |   0  |
| description   |   -1   | String  |   0  |
| magic_flag    |    0   | integer |   2  |
| version       |    2   | integer |   1  |
| reserved1     |    3   | integer |   1  |
| tx_max_power  |    4   | integer |   1  |
| uart_port     |    5   | integer |   1  |
| start_channel |    6   | integer |   1  |
| channel_num   |    7   | integer |   1  |
| country_code  |    8   | String  |   4  |
| uart_baudrate |   12   | integer |   4  |
| uart_tx_pin   |   16   | integer |   1  |
| uart_rx_pin   |   17   | integer |   1  |
| uart_cts_pin  |   18   | integer |   1  |
| uart_rts_pin  |   19   | integer |   1  |
| tx_control_pin|   20   | integer |   1  |
| rx_control_pin|   21   | integer |   1  |
| reserved2     |   22   | String  |   2  |
| platform      |   24   | String  |   32 |
| module_name   |   56   | String  |   32 |

- description:
  - prompt information when build the project

 - version:
   - the version of factory param mangement
   
 - reserved1
   - reserved
     
 - tx\_max_power
   - Wi-Fi maximum tx power
     - For ESP32 Series chip, range[8, 78], more details please refer to [ESP32 tx power setting range](https://docs.espressif.com/projects/esp-idf/en/release-v4.0/api-reference/network/esp_wifi.html#_CPPv425esp_wifi_set_max_tx_power6int8_t)
     - For ESP8266 Series chip, range[-128, 127], more details please refer to [ESP8266 tx power setting range](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/release-v3.3/api-reference/wifi/esp_wifi.html#_CPPv425esp_wifi_set_max_tx_power6int8_t)
     - For ESP32S2 Series chip, range[8, 78], more details please refer to [ESP32S2 tx power setting range](https://docs.espressif.com/projects/esp-idf/en/release-v4.2/esp32/api-reference/network/esp_wifi.html#_CPPv425esp_wifi_set_max_tx_power6int8_t)
     - For ESP32-C3 Series chip, range[8, 78], more details please refer to [ESP32-C3 tx power setting range](https://docs.espressif.com/projects/esp-idf/en/release-v4.2/esp32/api-reference/network/esp_wifi.html#_CPPv425esp_wifi_set_max_tx_power6int8_t)
 - start_chanel
   - Wi-Fi start channel
   
 - channel_num
   - the total channel number of Wi-Fi
   
 - country_code
   - Country code
   
 - uart_baudrate
   - uart baudrate
   
 - uart\_tx_pin
   - uart tx pin
   
 - uart\_rx_pin
   - uart rx pin
   
 - uart\_cts_pin
   - uart cts pin, it can be configured -1, if the pin is not used
   
 - uart\_rts_pin
   - uart rts pin, it can be configured -1, if the pin is not used

 - tx\_control\_pin 
   - for some board, tx pin need to be separated from mcu when power on. It can be configured -1, if the pin is not used

 - rx\_control\_pin 
   - for some board, rx pin need to be separated from mcu when power on. It can be configured -1, if the pin is not used

- reserved2
   - reserved

- platform
   - Which platform the current firmware runs on

- module_name
   - Which module the current firmware runs on

## Factory param data 

The origin table is `components/customized_partitions/raw_data/factory_param/factory_param_data.csv`, and the information each row contains is about one module. The factory parameter data is as the following table:

platform|module_name|description|magic_flag|version|reserved1|tx_max_power|uart_port|start_channel|channel_num|country_code|uart_baudrate|uart_tx_pin|uart_rx_pin|uart_cts_pin|uart_rts_pin|tx_control_pin|rx_control_pin|
|---|---|---|---|---|---|---| ---|---|---|---|---|---|---|---|---|---|---|
PLATFORM_ESP32|WROOM-32||0xfcfc|3|0|78|1|1|13|CN|115200|17|16|15|14|-1|-1
PLATFORM_ESP32|WROVER-32||0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP32|PICO-D4||0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP32|SOLO-1||0xfcfc|3|0|78|1|1|13|CN|115200|17|16|15|14|-1|-1
PLATFORM_ESP32|MINI-1|ESP32-U4WDH chip inside|0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP32|ESP32-D2WD|"2MB flash, No OTA"|0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP8266|WROOM-02|TX:15 RX:13|0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|-1|-1
PLATFORM_ESP8266|WROOM-5V2L|5V UART level|0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|5|-1
PLATFORM_ESP8266|ESP8266_1MB|No OTA|0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|-1|-1
PLATFORM_ESP8266|WROOM-02-N|TX:1 RX:3|0xfcfc|3|0|78|0|1|13|CN|115200|1|3|-1|-1|-1|-1
PLATFORM_ESP8266|WROOM-S2||0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|-1|-1
PLATFORM_ESP32S2|WROOM||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32S2|WROVER||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32S2|SOLO||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32S2|MINI||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32C3|MINI-1|TX:7 RX:6|0xfcfc|3|0|78|1|1|13|CN|115200|7|6|5|4|-1|-1

<a name="Add_Customized_Module"></a>
## Add customized module

if you want to add a module named as "MY_MODULE", of which country code is JP, and Wi-Fi channel is from 1 to 14, the table should be as the following one:

platform|module_name|description|magic_flag|version|reserved1|tx_max_power|uart_port|start_channel|channel_num|country_code|uart_baudrate|uart_tx_pin|uart_rx_pin|uart_cts_pin|uart_rts_pin|tx_control_pin|rx_control_pin|
|---|---|---|---|---|---|---| ---|---|---|---|---|---|---|---|---|---|---|
PLATFORM_ESP32|WROOM-32||0xfcfc|3|0|78|1|1|13|CN|115200|17|16|15|14|-1|-1
PLATFORM_ESP32|WROVER-32||0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP32|PICO-D4||0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP32|SOLO-1||0xfcfc|3|0|78|1|1|13|CN|115200|17|16|15|14|-1|-1
PLATFORM_ESP32|MINI-1|ESP32-U4WDH chip inside|0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP32|ESP32-D2WD|"2MB flash, No OTA"|0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP8266|WROOM-02|TX:15 RX:13|0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|-1|-1
PLATFORM_ESP8266|WROOM-5V2L|5V UART level|0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|5|-1
PLATFORM_ESP8266|ESP8266_1MB|No OTA|0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|-1|-1
PLATFORM_ESP8266|WROOM-02-N|TX:1 RX:3|0xfcfc|3|0|78|0|1|13|CN|115200|1|3|-1|-1|-1|-1
PLATFORM_ESP8266|WROOM-S2||0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|-1|-1
PLATFORM_ESP32S2|WROOM||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32S2|WROVER||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32S2|SOLO||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32S2|MINI||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32C3|MINI-1|TX:7 RX:6|0xfcfc|3|0|78|1|1|13|CN|115200|7|6|5|4|-1|-1
MY_PLATFORM|MY_MODULE|MY_DESCRIPTION|0xfcfc|3|0|78|1|1|14|JP|115200|17|16|15|14|-1|-1|

Then add module information in `esp_at_module_info` in `at_default_config.c`, like

```
static const esp_at_module_info_t esp_at_module_info[] = {
#if defined(CONFIG_IDF_TARGET_ESP32)
    {"WROOM-32",        CONFIG_ESP_AT_OTA_TOKEN_WROOM32,       CONFIG_ESP_AT_OTA_SSL_TOKEN_WROOM32 },        // default:ESP32-WROOM-32
    {"WROOM-32",        CONFIG_ESP_AT_OTA_TOKEN_WROOM32,       CONFIG_ESP_AT_OTA_SSL_TOKEN_WROOM32 },        // ESP32-WROOM-32
    {"WROVER-32",       CONFIG_ESP_AT_OTA_TOKEN_WROVER32,      CONFIG_ESP_AT_OTA_SSL_TOKEN_WROVER32 },       // ESP32-WROVER
    {"PICO-D4",         CONFIG_ESP_AT_OTA_TOKEN_ESP32_PICO_D4, CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32_PICO_D4},   // ESP32-PICO-D4
    {"SOLO-1",          CONFIG_ESP_AT_OTA_TOKEN_ESP32_SOLO_1,  CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32_SOLO_1 },   // ESP32-SOLO-1
    {"MINI-1",          CONFIG_ESP_AT_OTA_TOKEN_ESP32_MINI_1,  CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32_MINI_1 },   // ESP32-MINI-1
#endif

#if defined(CONFIG_IDF_TARGET_ESP8266)
    {"WROOM-02",        CONFIG_ESP_AT_OTA_TOKEN_WROOM_02,       CONFIG_ESP_AT_OTA_SSL_TOKEN_WROOM_02 },
    {"WROOM-S2",        CONFIG_ESP_AT_OTA_TOKEN_WROOM_S2,       CONFIG_ESP_AT_OTA_SSL_TOKEN_WROOM_S2 },
#endif

#if defined(CONFIG_IDF_TARGET_ESP32S2)
    {"WROOM",        CONFIG_ESP_AT_OTA_TOKEN_ESP32S2_WROOM,       CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32S2_WROOM },
    {"WROVER",       CONFIG_ESP_AT_OTA_TOKEN_ESP32S2_WROVER,      CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32S2_WROVER },
    {"SOLO",         CONFIG_ESP_AT_OTA_TOKEN_ESP32S2_SOLO,        CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32S2_SOLO },
    {"MINI",         CONFIG_ESP_AT_OTA_TOKEN_ESP32S2_MINI,        CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32S2_MINI },
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C3)
    {"MINI-1",         CONFIG_ESP_AT_OTA_TOKEN_ESP32C3_MINI,        CONFIG_ESP_AT_OTA_SSL_TOKEN_ESP32C3_MINI },
#endif
};
```
## Add customized data

If you want to add more parameter, for example, add a string "20181225" as the date, you need to add the type of date in the `factory_param_type.csv`, as the following table.

| param_name    | offset |  type   | size |
| ------------- | ------ | ------- | ---- |
| platform      |   -1   | String  |   0  |
| module_name   |   -1   | String  |   0  |
| description   |   -1   | String  |   0  |
| magic_flag    |    0   | integer |   2  |
| version       |    2   | integer |   1  |
| reserved1     |    3   | integer |   1  |
| tx_max_power  |    4   | integer |   1  |
| uart_port     |    5   | integer |   1  |
| start_channel |    6   | integer |   1  |
| channel_num   |    7   | integer |   1  |
| country_code  |    8   | String  |   4  |
| uart_baudrate |   12   | integer |   4  |
| uart_tx_pin   |   16   | integer |   1  |
| uart_rx_pin   |   17   | integer |   1  |
| uart_cts_pin  |   18   | integer |   1  |
| uart_rts_pin  |   19   | integer |   1  |
| tx_control_pin|   20   | integer |   1  |
| rx_control_pin|   21   | integer |   1  |
| reserved2     |   22   | String  |   2  |
| platform      |   24   | String  |   32 |
| module_name   |   56   | String  |   32 |
| date          |   88   | String  |   8  |

Edit `factory_param_data.csv` with reference to 
[Add customized module](#Add_Customized_Module), and add the date into the last column, as the following table,

platform|module_name|description|magic_flag|version|reserved1|tx_max_power|uart_port|start_channel|channel_num|country_code|uart_baudrate|uart_tx_pin|uart_rx_pin|uart_cts_pin|uart_rts_pin|tx_control_pin|rx_control_pin|
|---|---|---|---|---|---|---| ---|---|---|---|---|---|---|---|---|---|---|
PLATFORM_ESP32|WROOM-32||0xfcfc|3|0|78|1|1|13|CN|115200|17|16|15|14|-1|-1
PLATFORM_ESP32|WROVER-32||0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP32|PICO-D4||0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP32|SOLO-1||0xfcfc|3|0|78|1|1|13|CN|115200|17|16|15|14|-1|-1
PLATFORM_ESP32|MINI-1|ESP32-U4WDH chip inside|0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP32|ESP32-D2WD|"2MB flash, No OTA"|0xfcfc|3|0|78|1|1|13|CN|115200|22|19|15|14|-1|-1
PLATFORM_ESP8266|WROOM-02|TX:15 RX:13|0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|-1|-1
PLATFORM_ESP8266|WROOM-5V2L|5V UART level|0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|5|-1
PLATFORM_ESP8266|ESP8266_1MB|No OTA|0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|-1|-1
PLATFORM_ESP8266|WROOM-02-N|TX:1 RX:3|0xfcfc|3|0|78|0|1|13|CN|115200|1|3|-1|-1|-1|-1
PLATFORM_ESP8266|WROOM-S2||0xfcfc|3|0|78|0|1|13|CN|115200|15|13|3|1|-1|-1
PLATFORM_ESP32S2|WROOM||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32S2|WROVER||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32S2|SOLO||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32S2|MINI||0xfcfc|3|0|78|1|1|13|CN|115200|17|21|20|19|-1|-1
PLATFORM_ESP32C3|MINI-1|TX:7 RX:6|0xfcfc|3|0|78|1|1|13|CN|115200|7|6|5|4|-1|-1
MY_PLATFORM|MY_MODULE|MY_DESCRIPTION|0xfcfc|2|5|78|1|1|14|JP|115200|17|16|15|14|-1|-1|20181225|

It is important to know that the total size of the AT factory parameter is controlled by the `ESP_AT_FACTORY_PARAMETER_SIZE` in `at_default_config.h`, and can be adjusted as needed

**Notes: It's recommended that do not change the first 2048 bytes, which may be used by Espressif.**

Then, you can add code to parse `date` in `esp_at_factory_parameter_init` or other api.

## Modify Factory param data
If you simply need to modify factory_param on an existing module, the following three methods are recommended:

- method one
   - Premise: you need to have the entire esp-at project.
1. Find the factory_param_data.csv file through the following path: `components/customized_partitions/raw_data/factory_param/factory_param_data.csv`, and modify the parameters.
2. Recompile the ``esp-at`` project, download the new ``factory_param.bin`` into flash.

- method two
   - Premise: you need to have the entire esp-at project.
1. Find the factory_param_data.csv file through the following path: `components/customized_partitions/raw_data/factory_param/factory_param_data.csv`, and modify the parameters.
2. Open the terminal in the following path: `esp-at`, execute the following command.
   - Commandline: `python tools/factory_param_generate.py --platform PLATFORM_ESP32S2 --module WROVER --define_file components/customized_partitions/raw_data/factory_param/factory_param_type.csv --module_file components/customized_partitions/raw_data/factory_param/factory_param_data.csv --bin_name factory_param.bin --log_file ./factory_parameter.log`
   - The value of the `-- platform -- module` parameter in the command needs to be changed as the case may be.
3. It will generate factory_param.bin at esp-at folder, download the new ``factory_param.bin`` into flash.
4. If you want to know how to use the commands in step 2, you can study the factory_param_generate.py file in the 'esp-at/tools''.
  
- method three
   - Premise:  you need to have the factory_param.bin file.
1. Open this file directly with a binary tool, and directly modify the parameters in the corresponding position according to the parameters offset in factory_param_type.csv. 
2. Download the new ``factory_param.bin`` into flash.
