# Overview
libat_core.a is AT Command Core, and it is the core of AT command, including the default AT instruction set, the AT command parsing, execution and responding. The lib contains 4 kinds of command, such as AT+TEST=?, AT+TEST?, AT+TEST=“abc” and AT+TEST. It supports custom AT commands based on the lib and related APIs, and ones can also define input and output medium, like uart, spi, socket, bt, etc.

The demo is the AT command set based on uart. You can replace the uart driver with other drivers whichever you want to use. But you have to make some changes in at_task.c. In addition, you can add some custom AT commands in at_custom_cmd like AT+CIUPDATE if necessary.

More details are in documentation [esp32_at_instruction_set_and_examples_en.pdf](http://espressif.com/sites/default/files/documentation/esp32_at_instruction_set_and_examples_en.pdf) or 
[esp32_at_instruction_set_and_examples_cn.pdf](http://espressif.com/sites/default/files/documentation/esp32_at_instruction_set_and_examples_cn.pdf).
  
# Hardware Introduction
The ESP32 Board sends AT commands through UART1 by default. 

* GPIO16 is RXD
* GPIO17 is TXD
* GPIO14 is RTS
* GPIO15 is CTS

The debug log will output through UART0 by default, but user can change it in menuconfig if needed.  

* `make menuconfig` --> `Component config` --> `ESP32-specific` --> `UART for console output`

# Compiling and flashing the project
Compiling the esp32-at is the same as compiling any other project based on the ESP-IDF:

1. You can clone the project into an empty directory using command:
```
git clone --recursive https://github.com/espressif/esp32-at.git
```
  * Note the `--recursive` option! The esp32-at project already has an ESP-IDF as a submodule, which should be used to compile the project, in order to avoid any compatibility issues (e.g. compile failure or running problems, etc.). 
  * If you have already cloned the esp32-at without this option, run another command to get all the submodules:
```shell
git submodule update --init --recursive
```  
2. Set `IDF_PATH` environment variable, for example:
```
export IDF_PATH=/path_of_your_esp32-at/esp-idf
```
3. `rm sdkconfig` to remove the old configuration.
4. Set the latest default configuration by `make defconfig`. 
5. `make menuconfig` -> `Serial flasher config` to configure the serial port for downloading.
6. `make flash` to compile the project and download it into the flash.
  * Or you can call `make` to compile it, and follow the printed instructions to download the bin files into flash by yourself.
  * `make print_flash_cmd` can be used to print the addresses of downloading.
  * More details are in the [esp-idf README](https://github.com/espressif/esp-idf/blob/master/README.md).
7. If the ESP32-AT bin fails to boot, and prints "ota data partition invalid", you should run `make erase_flash` to erase the entire flash.

