- [ESP32 platform](#platform-esp32)  
- [ESP32S2 platform](#platform-esp32s2)  
- [ESP8266 platform](#platform-esp8266)  

More supported modules, please referred to [`components/customized_partitions/raw_data/factory_param/factory_param_data.csv`](../components/customized_partitions/raw_data/factory_param/factory_param_data.csv)

<a name="platform-esp32"></a>
# ESP32 platform  

## Hardware Introduction
The WROOM32 Board sends AT commands through UART1 by default. 

* GPIO16 is RXD
* GPIO17 is TXD
* GPIO14 is RTS
* GPIO15 is CTS

The debug log will output through UART0 by default, which TXD0 is GPIO1 and RXD0 is GPIO3, but user can change it in menuconfig if needed.  

* `make menuconfig` --> `Component config` --> `ESP32-specific` --> `UART for console output`

### Notes: Please pay attention to conflict of the pin ##
- If choose `AT through HSPI`, you can get the information of the hspi pin by `make menuconfig` --> `Component config` --> `AT` --> `AT hspi settings`
- If enable `AT ethernet support`, you can get the information of the ethernet pin from `ESP32_AT_Ethernet.md`.

## Compiling and flashing the project

Suppose you have completed the installation of the compiler environment for esp-idf, if not, you should completed it referring to https://docs.espressif.com/projects/esp-idf/en/v3.3/get-started/index.html#setup-toolchain, in order to compile esp-at project properly, please do the following additional steps:

```  
step1:install python 2.7 or python 3.x 
step2:[install pip](https://pip.pypa.io/en/latest/installing/)  
step3:install the following python packages with pip: pip install pyyaml xlrd
```

Compiling the esp32-at is the same as compiling any other project based on the ESP-IDF:

1. You can clone the project into an empty directory using command:
```
git clone --recursive https://github.com/espressif/esp-at.git
```
2. `rm sdkconfig` to remove the old configuration and `rm -rf esp-idf` to remove the old esp-idf if you want to compile other esp platform AT.
3. Set the latest default configuration by `make defconfig`. 
4. `make menuconfig` -> `Serial flasher config` to configure the serial port for downloading.
5. `make flash` or `make flash SILENCE=1` to compile the project and download it into the flash, and `make flash SILENCE=1` will remove some logs to reduce firmware size.
  * Or you can call `make` to compile it, and follow the printed instructions to download the bin files into flash by yourself.
  * `make print_flash_cmd` can be used to print the addresses of downloading.
  * More details are in the [esp-idf README](https://github.com/espressif/esp-idf/blob/master/README.md).
  * If enable BT feature, the firmware size will be much larger, please make sure it does not exceed the ota partition size.  
6. `make factory_bin` to combine factory bin, by default, the factory bin is 4MB flash size, DIO flash mode and 40MHz flash speed. If you want use this command, you must fisrt run `make print_flash_cmd | tail -n 1 > build/download.config` to generate `build/download.config`.
7. If the ESP32-AT bin fails to boot, and prints "ota data partition invalid", you should run `make erase_flash` to erase the entire flash.

<a name="platform-esp32s2"></a>
# ESP32S2 platform  

## Hardware Introduction
The WROOM32S2 Board sends AT commands through UART1 by default. 

* GPIO18 is RXD
* GPIO17 is TXD
* GPIO19 is RTS
* GPIO20 is CTS

The debug log will output through UART0 by default, which TXD0 is GPIO1 and RXD0 is GPIO3, but user can change it in menuconfig if needed.  

* `make menuconfig` --> `Component config` --> `Common ESP-related` --> `UART for console output`

## Compiling and flashing the project

Suppose you have completed the installation of the compiler environment for esp-idf, if not, you should completed it referring to https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html#setup-toolchain, and you can get compiler toolchain from https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/tools/idf-tools.html#list-of-idf-tools, in order to compile esp-at project properly, please do the following additional steps:

```  
step1:python > 3.8.0 
step2:[install pip](https://pip.pypa.io/en/latest/installing/)  
step3:install the following python packages with pip3: pip3 install pyyaml xlrd
```

Compiling the esp32-at is the same as compiling any other project based on the ESP-IDF:

1. You can clone the project into an empty directory using command:
```
git clone --recursive https://github.com/espressif/esp-at.git
```
2. `rm sdkconfig` to remove the old configuration and `rm -rf esp-idf` to remove the old esp-idf if you want to compile other esp platform AT.
3. Set esp module information  
```  
export ESP_AT_PROJECT_PLATFORM=PLATFORM_ESP32S2
export ESP_AT_MODULE_NAME=WROOM
export ESP_AT_PROJECT_PATH=$(pwd)
```  
4. `./esp-idf/tools/idf.py -DIDF_TARGET=esp32s2 build` to compile the project and download it into the flash, and `./esp-idf/tools/idf.py -DIDF_TARGET=esp32s2 -DSILENCE=1 build` will remove some logs to reduce firmware size.
  * Follow the printed instructions to download the bin files into flash by yourself.

<a name="platform-esp8266"></a>
# ESP8266 platform  

## Hardware Introduction
The ESP8266 WROOM 02 Board sends AT commands through UART0 by default. 

* GPIO13 is RXD
* GPIO15 is TXD
* GPIO1  is RTS
* GPIO3  is CTS

The debug log will output through UART1 by default, which TXD0 is GPIO2, but user can change it in menuconfig if needed.  

* `make menuconfig` --> `Component config` --> `ESP8266-specific` --> `UART for console output`


## Compiling and flashing the project
Suppose you have completed the installation of the compiler environment for esp-idf, if not, you should completed  it referring to https://docs.espressif.com/projects/esp8266-rtos-sdk/en/v3.2/get-started/index.html#setup-toolchain, in order to compile esp-at project properly, please do the following additional steps:

```
step1:install python 2.7 or python 3.x  
step2:[install pip](https://pip.pypa.io/en/latest/installing/)  
step3:install the following python packages with pip: pip install pyyaml xlrd
```

Compiling the ESP8266 AT is the same as compiling esp32-at:

1. You can clone the project into an empty directory using command:
```
git clone --recursive https://github.com/espressif/esp-at.git
```
2. Change the Makefile from  
```  
export ESP_AT_PROJECT_PLATFORM ?= PLATFORM_ESP32
export ESP_AT_MODULE_NAME ?= WROOM-32
```    
to be   
```  
export ESP_AT_PROJECT_PLATFORM ?= PLATFORM_ESP8266 
export ESP_AT_MODULE_NAME ?= WROOM-02
```  
3. `rm sdkconfig` to remove the old configuration and `rm -rf esp-idf` to remove the old esp-idf if you want to compile other esp platform AT.
4. Set the latest default configuration by `make defconfig`. 
5. `make menuconfig` -> `Serial flasher config` to configure the serial port for downloading.
6. `make flash` or `make flash SILENCE=1` to compile the project and download it into the flash, and `make flash SILENCE=1` will remove some logs to reduce firmware size.
  * Or you can call `make` to compile it, and follow the printed instructions to download the bin files into flash by yourself.
  * `make print_flash_cmd` can be used to print the addresses of downloading.
  * More details are in the [esp-idf README](https://github.com/espressif/esp-idf/blob/master/README.md).
7. `make factory_bin` to combine factory bin, by default, the factory bin is 4MB flash size, DIO flash mode and 40MHz flash speed. If you want use this command, you must fisrt run `make print_flash_cmd | tail -n 1 > build/download.config` to generate `build/download.config`.
8. If the ESP32-AT bin fails to boot, and prints "ota data partition invalid", you should run `make erase_flash` to erase the entire flash.
