# Overview
libat_core.a is AT Command Core,and it is the core of AT command, including the default AT instruction set, the AT command parsing, execution and responding.The lib contains 4 kinds of command, such as AT+TEST=?, AT+TEST?, AT+TEST=“abc” and AT+TEST. It supports custom AT commands based on the lib and related APIs, and ones can also define input and output medium, like uart, spi, socket, bt, etc.

The demo is the AT command set based on uart.You can replace uart driver with other drivers whichever you want to use.But you have to make some changes in at_task.c. In addition，you can add some custom AT command in at_custom_cmd like AT+CIUPDATE if necessary.

# Compile AT
Building examples is the same as building any other project:

* Follow the setup instructions in the top-level esp-idf README.

* Set `IDF_PATH` environment variable to point to the path to the esp-idf top-level directory.
* Change into the directory of the example you'd like to build.
* `make menuconfig` to configure the example. Most examples require a simple WiFi SSID & password via this configuration.
* `make` to build the example.
* Follow the printed instructions to flash, or run `make flash`.


