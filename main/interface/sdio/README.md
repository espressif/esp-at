# AT through SDIO

## Overview
This demo is based on libat_core.a, and it's a replacement for AT through UART. Since SDIO AT  is similar to [SDIO demo](https://github.com/espressif/esp-idf/tree/master/examples/peripherals/sdio) , it is highly recommended to first read SDIO demo README carefully and run the basic demo of SDIO before using this program.

More details about SDIO slave are in documentation [SDIO slave](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/sdio_slave.html)

## Hardware Introduction
ESP32 AT through SDIO use the following pins to communicate with the MCU:  

- GPIO14 is CLK
- GPIO15 is CMD
- GPIO2  is DAT0
- GPIO4  is DAT1
- GPIO12 is DAT2
- GPIO13 is DAT3

For more details, please refer to [sdio_AT_User_Guide.md](https://github.com/espressif/esp32-at/tree/master/docs/sdio_AT_User_Guide.md) in the docs directory.

***Note*:** If you're using a board (e.g. WroverKit v2 and before, PICO, DevKitC) which is not able to drive GPIO2 low on downloading, be sure to do some preprocessing with [README](https://github.com/espressif/esp-idf/blob/master/examples/peripherals/sdio/README.md) firstly.
