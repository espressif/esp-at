# Overview
libat_core.a is AT Command Core,and it is the core of AT command, including the default AT instruction set, the AT command parsing, execution and responding.The lib contains 4 kinds of command, such as AT+TEST=?, AT+TEST?, AT+TEST=“abc” and AT+TEST. It supports custom AT commands based on the lib and related APIs, and ones can also define input and output medium, like uart, spi, socket, bt, etc.

The demo is the AT command set based on uart.You can replace uart driver with other drivers whichever you want to use.But you have to make some changes in at_task.c. In addition，you can add some custom AT command in at_custom_cmd like AT+CIUPDATE if necessary.

If You want to get more information,you can get document from http://espressif.com/sites/default/files/documentation/esp32_at_instruction_set_and_examples_en.pdf,or
http://espressif.com/sites/default/files/documentation/esp32_at_instruction_set_and_examples_cn.pdf .

# Compile the demo and using AT instructions
Building the demo is the same as building any other project:

* Follow the setup instructions in the top-level esp-idf README.
* Set `IDF_PATH` environment variable to point to the path to the esp-idf top-level directory.
* `make menuconfig` to configure the demo.And you can use default configuration by 'make defconfig'.
* `make` to build the demo.
* Follow the printed instructions to flash, or run `make flash`.
* You can use AT instructions referred in the released AT document.

# Released Bin
You can get AT bin from http://espressif.com/zh-hans/support/download/at?keys=&field_type_tid%5B%5D=13




