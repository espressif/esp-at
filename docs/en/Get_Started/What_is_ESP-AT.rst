What is ESP-AT
===============

ESP-AT is a solution developed by Espressif to integrate connectivity into customers' products, which can be quickly moved to mass production. It aims to reduce software development costs and quickly form products. With ESP-AT commands, you can quickly join the wireless network, connect to the cloud platform, realize data transmission and remote control functions, and realize the interconnection of everything through wireless communication easily.

ESP-AT is a project based on ESP-IDF or ESP8266_RTOS_SDK. It makes an ESP board work as a slave, and an MCU as a host. The host MCU sends AT commands to the ESP chip and receives AT responses back. ESP-AT provides a wide range of AT commands with different functions, such as Wi-Fi commands, TCP/IP commands, Bluetooth LE commands, Bluetooth commands, MQTT commands, HTTP commands, and Ethernet commands.

.. figure:: ../../_static/ESP-AT-overview.jpg
   :align: center
   :alt: ESP-AT Overview
   :figclass: align-center

   ESP-AT Overview

.. note::
    Currently, ESP-AT is based on ESP-IDF or ESP8266_RTOS_SDK, not ESP8266 NonOS SDK.

AT commands start with “AT”, which stand for "Attention", and end with a new line (CR LF). Every time you send a command, you will reveive an ``OK`` or ``ERROR``, which indicates the final execution status of the current command. Please be noted that all commands are executed serially, which means only one AT command can be executed at a time. Therefore, you should wait for the previous command to be executed before sending out the next one. Otherwise, you will receive ``busy P...``. For more details about AT commands, please refer to :doc:`../AT_Command_Set/index`.

By default, the host MCU connects to the ESP board via UART, and sends/receives AT commands/responses through UART. But you can also use other interfaces, such as SDIO, according to your actual use scenario.

You can develop your own AT commands based on our ESP-AT project and implement more features according to your actual use scenario.
