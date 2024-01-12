# AT through SPI

## Overview
AT through SPI is a replacement for AT through UART. If using AT through SPI, you must choose the MCU with hardware or software simulated spi interface.  
Because SPI is a protocal that controlled by master device, so slave must notify master to read or write, if there are some data to be sent by slave. In this demo, we use one more pin to handshake the MCU. 

If you want to use SPI AT on ESP32, SDIO SPI mode is recommended, MCU can also use the SPI peripheral, and ESP32 will use SDIO, the detailed informatio refer to [ESP32 SDIO SPI demo](https://github.com/espressif/esp-at/tree/master/examples/at_spi_master/sdspi).
If you use ESP32-C AT through SPI, please Refer to the [ESP32 series demo](https://gitlab.espressif.cn:6688/application/esp-at/-/tree/master/examples/at_spi_master/spi/esp32_c_series).

