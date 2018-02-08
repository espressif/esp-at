# AT through HSPI

## Overview
This demo is based on libat_core.a, and it's a replacement for AT through UART. If using AT through HSPI, you  must choose the MCU with hardware or software simulated spi interface.  
Because SPI is a protocal that controlled by master device, so slave must notify master to read, if there are some data to be sent by slave. In this demo, we use one more pin to handshake the MCU. 

## Hardware Introduction
By default, ESP32 AT through HSPI use the following pins to communicate with the MCU:  

* GPIO12 is MOSI  
* GPIO13 is MISO  
* GPIO14 is CS  
* GPIO15 is SCLK  
* GPIO2 is handshake


