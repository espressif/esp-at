
What is ESP-AT?
=================

ESP-AT is an IoT solution developed by Espressif, which can be directly used in mass production.  It aims to reduce customers' development costs and quickly form products. Through the ESP AT command, you can quickly join the wireless network, connect to the cloud platform, realize data transmission and remote control functions, and realize the interconnection of everything through wireless communication easily.

ESP-AT is a project based on ESP-IDF/ESP8266-RTOS-SDK. It makes ESP board work as slave, and needs another MCU to work as host. Host MCU sends AT commands to ESP chip, and receives AT response.   
ESP-AT provides many AT commands with different functions. For example, Wi-Fi commands, TCPIP commands, BLE commands, BT commands, MQTT commands, HTTP commands, Ethernet commands, etc. In this case, host MCU can implement those functions through ESP board.

![avatar](../../_static/ESP-AT.jpg)


“AT” is short of “Attention”. AT command starts with “AT”, ends with a new line (CR LF).  
With the default configuration, host MCU connects to ESP board via UART, and sends/receives AT commands/responses through UART. But you can also change to use other peripheral, such as SDIO, according to your actual use scenario.  
And also, you can develop your own AT commands based on our ESP-AT project, to implement more features according to your actual use scenario.


