# AT through socket

## Overview
This demo is based on libat_core.a, and it's a replacement for AT through UART. If using AT through socket, you must create a socket to connect the server of  ESP32. And then you can send the AT commands via the socket.

## Steps:
Here is an example of using a PC as the client:
* PC connects to ESP_xxxxxx through WiFi.
    - ESP_xxxxxx is the ESP32's default SSID, xxxxxx means the last three bytes of MAC address.
* Create a TCP client on PC to connect to IP 192.168.4.1, port 3333.
    - 192.168.4.1 is the default IP of the ESP32 softAP.
    - port 3333 is the default port, you can change it in the menuconfig before compiling.
* After the TCP connection is established, the PC can send AT commands to the ESP32 through socket.
