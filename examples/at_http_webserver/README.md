# Overview
HTTP server for AT is like a "customizable router" management page. Config ESP32 such as WiFi and BLE only through the web page. 
The esp32 firstly tries to initialize as a access point with the default SSID 'AT_HTTP_WEBSERVER' and defaut password "12345678", you can modify the initial WiFi mode and the default SSID and password with config option (`make menuconfig` -> `Component config` > `AT HTTP SERVER`).
If connect this network, open a web browser with the url http://192.168.4.1(or IP address assigned by the router if select initial wifi mode is station) to connect the http server. The server will transmit a html web page from the FAT file system and your browser will execute it. From the web page, set some parameter, the server will call AT to compute and return the result to you.

# AT Parse Frame
AT parse frame pack the AT command and parse the result, and then provides the API to user, so the user only needs to get the desired result according to the interface. Compared to AT, it provides a simpler and more convenient way for users to connect to call commands without using UART, nor to care about the implementation of each AT command.

# HTTP server
Libesphttpd is the HTTP server library for the ESP32. Its core is clean and small, but it provides an extensible architecture with plugins to handle a flash-based compressed read-only filesystem for static files. Considing the BIN is already large because of AT, we strip down the own file system and let it work by mount another file system.

Pulls together various bits of code from the net that I modified for this project
1. HTTP server code from https://github.com/Spritetm/libesphttpd

# Compiling and flashing the project
1. `make menuconfig` -> `Serial flasher config` to configure the serial port for downloading.
2. `make flash` to compile the project and download it into the flash.
