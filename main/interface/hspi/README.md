# AT through SPI

## Overview
AT through SPI is a replacement for AT through UART. If using AT through SPI, you must choose the MCU with hardware or software simulated spi interface.  
Because SPI is a protocal that controlled by master device, so slave must notify master to read or write, if there are some data to be sent by slave. In this demo, we use one more pin to handshake the MCU. 

If you want to use ESP32 AT through SPI, More details about AT through SPI are in documentation [ESP32 SPI demo](https://github.com/espressif/esp-at/tree/master/examples/at_spi_master/spi/esp32).
If you use ESP32-C or ESP32-S AT through SPI, please Refer to the [ESP32 series demo](https://gitlab.espressif.cn:6688/application/esp-at/-/tree/master/examples/at_spi_master/spi/esp32_c_series).
