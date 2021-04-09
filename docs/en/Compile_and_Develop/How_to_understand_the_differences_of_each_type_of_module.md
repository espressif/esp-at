How to understand the differences of each type of module
========================================================
  
Since the esp-at project supports different module, for example, WROOM-32, WROVER-32, ESP8266_1MB, even supports WROOM-5V2L, when compiling the esp-at project, users can set different configurations to generate AT firmware for different ESP modules. The detailed information are in the `esp-at/module_config` directory.  
In order to facilitate customers to understand the differences between different modules of the same platform, they will be summarized in the following list.

## 1. ESP32 Platform

### 1. Command differences supported

| command | ESP32-WROOM-32_AT_Bin | ESP32-WROVER-32_AT_Bin | ESP32-PICO-D4_AT_Bin | ESP32-SOLO-1_AT_Bin | ESP32-D2WD_AT_Bin | 
|---|---|---|---|---|---|
| AT base command support | √ | √ | √ | √ | √
| AT wifi command support | √ | √ | √ | √ | √
| AT MDNS command support | √ | √ | √ | √ | √
| AT net command support | √ | √ | √ | √ | √ 
| AT WPS command support | √ | √ | √ | √ | √ 
| AT ping command support | √ | √ | √ | √ | √ 
| AT smartconfig command support | √ | √ | √ | √ | √ 
| AT MQTT command support | √ | √ | √ | √ | √ 
| AT http command support | √ | √ | √ | √ | √ 
| AT ble command support | √ | √ | √ | √ | √ 
| AT ble hid command support | √ | √ | √ | √ | √
| AT blufi command support | × | × | × | × | × 
| AT bt spp command support | × | √ | × | × | × 
| AT bt a2dp command support | × | × | × | × | × 
| AT ethernet support | × | × | × | × | × 
| AT FS command support | × | × | × | × | × 
| AT driver command support | × | × | × | × | × 
| AT WPA2 Enterprise command support | × | × | × | × | × 
| AT OTA command support | √ | √ | √ | √ | × 

### 2. Hardware differences

|  | ESP32-WROOM-32_AT_Bin | ESP32-WROVER-32_AT_Bin | ESP32-PICO-D4_AT_Bin | ESP32-SOLO-1_AT_Bin | ESP32-D2WD_AT_Bin | 
|---|---|---|---|---|---|
| flash_size(MB) | 4 | 4 | 4 | 4 | 2 
| PSRAM(MB) | × | 8 | × | × | × 
| UART level(Volt) | 3.3 | 3.3 | 3.3 | 3.3 | 3.3 
| UART pin(TX,RX,CTS,RTS) | 17, 16, 15, 14 | 22, 19, 15, 14 | 22, 19, 15, 14 | 17, 16, 15, 14 | 22, 19, 15, 14 

For factory param, see: `components/customized_partitions/raw_data/factory_param/factory_param_data.csv`

### 3. AT pin
See: https://docs.espressif.com/projects/esp-at/en/latest/AT_Binary_Lists/index.html

### 4. Firmware supported modules 

| module/chip | ESP32-WROOM-32_AT_Bin | ESP32-WROVER-32_AT_Bin | ESP32-PICO-D4_AT_Bin | ESP32-SOLO-1_AT_Bin | ESP32-D2WD_AT_Bin | 
|---|---|---|---|---|---|
| ESP32-WROOM-32E | √ | × | √ | √ | √ 
| ESP32-WROOM-32UE | √ | × | √ | √ | √ 
| ESP32-WROOM-32D | √ | × | √ | √ | √ 
| ESP32-WROOM-32U | √ | × | √ | √ | √ 
| ESP32-WROOM-32 | √ | × | √ | √ | √ 
| ESP32-WROOM-32SE | × | × | × | × | × 
| ESP32-WROVER-E | × | √ | √ | × | √ 
| ESP32-WROVER-IE | × | √ | √ | × | √ 
| ESP32-WROVER-B | × | √ | √ | × | √ 
| ESP32-WROVER-IB | × | √ | √ | × | √ 
| ESP32-WROVER | × | √ | √ | × | √ 
| ESP32-WROVER-I | × | √ | √ | × | √ 
| ESP32-SOLO-1 | √ | × | √ | √ | √ 
| ESP32-D2WD | × | × | × | × | √ 
| ESP32-MINI-1 | × | × | √ | × | √ 
| ESP32-PICO-D4 | × | × | √ | × | √ 

## 2. ESP8266 Platform

### 1. Command differences supported

| command | ESP8266-WROOM-02_AT_Bin | ESP8266-WROOM-5V2L_AT_Bin | ESP8266_1MB_AT_Bin | ESP8266-WROOM-02-N_AT_Bin |  
|---|---|---|---|---|
| AT base command support | √ | √ | √ | √
| AT wifi command support | √ | √ | √ | √
| AT MDNS command support | √ | √ | √ | √
| AT WPS command support | √ | √ | √ | √ 
| AT ping command support | √ | √ | √ | √ 
| AT smartconfig command support | √ | √ | √ | √ 
| AT MQTT command support | √ | √ | √ | √ 
| AT http command support | × | × | × | × 
| AT signaling test command support | √ | √ | √ | √ 
| AT WPA2 Enterprise command support | × | × | × | × 
| AT OTA command support | √ | √ | × | √ 

### 2. Hardware differences

|  | ESP8266-WROOM-02_AT_Bin | ESP8266-WROOM-5V2L_AT_Bin | ESP8266_1MB_AT_Bin | ESP8266-WROOM-02-N_AT_Bin | 
|---|---|---|---|---|
| flash_size(MB) | 2 | 2 | 1 | 2 
| PSRAM(MB) | × | × | × | × 
| UART level(Volt) | 3.3 | 5 | 3.3 | 3.3 
| UART pin(TX,RX,CTS,RTS) | 15, 13, 3, 1 | 15, 13, 3, 1 | 15, 13, 3, 1 | 1, 3, -1, -1 

For factory param, see: `components/customized_partitions/raw_data/factory_param/factory_param_data.csv`

### 3. AT pin
See: https://docs.espressif.com/projects/esp-at/en/latest/AT_Binary_Lists/index.html

### 4. Firmware supported modules

| module/chip | ESP8266-WROOM-02_AT_Bin | ESP8266-WROOM-5V2L_AT_Bin | ESP8266_1MB_AT_Bin | ESP8266-WROOM-02-N_AT_Bin | 
|---|---|---|---|---|
| ESP-WROOM-02D | √ | × | √ | √ 
| ESP-WROOM-02U  | √ | × | √ | √ 
| ESP-WROOM-02 | √ | × | √ | √ 
| ESP-WROOM-5V2L | × | √ | × | ×  
| ESP-WROOM-S2 | × | × | × | × 
| ESP8285N08 | × | × | √ | × 
| ESP8285H08 | × | × | √ | × 
| ESP8285H16 | √ | × | √ | √ 

## 3. ESP32S2 Platform

### 1. Command differences supported

| command | ESP32-S2-WROOM_AT_Bin | ESP32-S2-WROVER_AT_Bin | ESP32-S2-SOLO_AT_Bin | ESP32-S2-MINI_AT_Bin | 
|---|---|---|---|---|
| AT base command support | √ | √ | √ | √ 
| AT wifi command support | √ | √ | √ | √ 
| AT net command support | √ | √ | √ | √ 
| AT MDNS command support | √ | √ | √ | √ 
| AT WPS command support | √ | √ | √ | √ 
| AT ping command support | √ | √ | √ | √ 
| AT smartconfig command support | √ | √ | √ | √ 
| AT MQTT command support | √ | √ | √ | √ 
| AT http command support | √ | √ | √ | √ 
| AT FS command support | × | × | × | × 
| AT driver command support | × | × | × | × 
| AT WPA2 Enterprise command support | × | × | × | × 
| AT OTA command support | √ | √ | √ | √ 

### 2. Hardware differences

|  | ESP32-S2-WROOM_AT_Bin | ESP32-S2-WROVER_AT_Bin | ESP32-S2-SOLO_AT_Bin | ESP32-S2-MINI_AT_Bin | 
|---|---|---|---|---|
| flash_size(MB) | 2 | 2 | 2 | 2 
| PSRAM(MB) | × | × | × | × 
| UART level(Volt) | 3.3 | 3.3 | 3.3 | 3.3 
| UART pin(TX,RX,CTS,RTS) | 17, 21, 20, 19 | 17, 21, 20, 19 | 17, 21, 20, 19 | 17, 21, 20, 19 

For factory param, see: `components/customized_partitions/raw_data/factory_param/factory_param_data.csv`

### 3. AT pin
See: https://docs.espressif.com/projects/esp-at/en/latest/AT_Binary_Lists/index.html


### 4. Firmware supported modules 

| module/chip | ESP32-S2-WROOM_AT_Bin | ESP32-S2-WROVER_AT_Bin | ESP32-S2-SOLO_AT_Bin | ESP32-S2-MINI_AT_Bin | 
|---|---|---|---|---|
| ESP32-S2-WROOM | √ | √ | √ | √ 
| ESP32-S2-WROOM-I | √ | √ | √ | √ 
| ESP32-S2-WROVER | √ | √ | √ | √ 
| ESP32-S2-WROVER-I | √ | √ | √ | √ 
| ESP32-S2-SOLO | √ | √ | √ | √ 
| ESP32-S2-SOLO-U | √ | √ | √ | √ 
| ESP32-S2-MINI-1 | √ | √ | √ | √ 
| ESP32-S2-MINI-1U | √ | √ | √ | √ 

## 4. ESP32-C3 Platform

### 1. Command differences supported

| command | ESP32-C3-MINI-1_AT_Bin |
|---|---|
| AT base command support | √ |
| AT wifi command support | √ |
| AT MDNS command support | √ |
| AT net command support | √ |
| AT WPS command support | √ |
| AT ping command support | √ |
| AT smartconfig command support | √ |
| AT MQTT command support | √ |
| AT http command support | √ |
| AT ble command support | × |
| AT ble hid command support | × |
| AT blufi command support | × |
| AT bt spp command support | × |
| AT bt a2dp command support | × |
| AT ethernet support | × |
| AT FS command support | × |
| AT driver command support | × |
| AT WPA2 Enterprise command support | × |
| AT OTA command support | √ |
| AT WEB Server command support | × |

### 2. Hardware differences

|  | ESP32-C3-MINI-1_AT_Bin | 
|---|---|
| flash_size(MB) | 4 | 
| PSRAM(MB) | × |
| UART level(Volt) | 3.3 |
| UART pin(TX,RX,CTS,RTS) | 7, 6, 5, 4 |

For factory param, see: `components/customized_partitions/raw_data/factory_param/factory_param_data.csv`

### 3. AT pin
See: https://docs.espressif.com/projects/esp-at/en/latest/AT_Binary_Lists/index.html


### 4. Firmware supported modules 

| module/chip | ESP32-C3-MINI-1_AT_Bin |
|---|---|
| ESP32-C3-MINI-1 | √ |