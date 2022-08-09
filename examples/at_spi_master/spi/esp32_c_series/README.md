# AT ESP32 SPI Master Example

## Overview

This code demonstrates how to implement the SPI master to read/write a SPI AT slave. Please refer to [ESP SPI AT](https://docs.espressif.com/projects/esp-at/en/latest/esp32c3/Compile_and_Develop/How_to_implement_SPI_AT.html)  for more information about SPI AT.

To facilitate testing, we added an UART task to receive AT commands and print the AT responses. So the schematic of data transmission is as follows：

```
                    （SPI Master)                                 (SPI Slave)
        +-----------------x-----------------+                  +-------x-------+
     TX |                                   |                  |               |
    ----|--------------     ----------------|                  |               |
        | UART Task   |<--->|    SPI Task   |  <----------->   |    SPI AT     |
    ----|--------------     ----------------|                  |               |
     RX |                                   |                  |               |
        +-----------------x-----------------+                  +-------x-------+

```
Please refer to the example code and set up a serial terminal program to the same settings as of UART in ESP32 board, and then you can send AT commands through the serial terminal.

## Pin Assignment

| Signal           | Master(ESP32) | Master(ESP32C3) |
| ---------------- | ------------- | -------------   |
| SCLK             | GPIO15        | GPIO6           |
| MISO             | GPIO12        | GPIO2           |
| MOSI             | GPIO13        | GPIO7           |
| CS               | GPIO14        | GPIO10          |
| HANDSHAKE        | GPIO5         | GPIO3           |
| GND              | GND           | GND             |
| QUADWP(qio/qout) | GPIO16        | GPIO8           |
| QUADHD(qio/qout) | GPIO17        | GPIO9           |

**Note**： QUADWP and QUADHD only used for 4-bit(qio/qout) transactions.

## How to use example

### Hardware Required

To run this example, you need an ESP32 or ESP32-C3 development board.

### Configure the project

Open the project configuration menu (`idf.py menuconfig`). Then go into `SPI master Configuration` menu.

- Set the transmission mode and change the pin assignment if necessary.

### Build and Flash

Run `idf.py -p PORT flash` to build and flash the project.

**Note**： After flashing the project, please close the current terminal to avoid occupying the current serial port.

### Setup external terminal software
Refer to the example code and set up a serial terminal program to the same settings as of UART in ESP32 board. Open the serial interface in the terminal,  connect to the ESP32 UART port, and send "AT\r\n" command. If you can receive the "OK" response, it means the communication is normal.

For more details about AT commands, please refer to [AT Command Set](https://docs.espressif.com/projects/esp-at/en/latest/AT_Command_Set/index.html) .

## Troubleshooting
If you do not see any output from UART port, please check the pin used by the project.