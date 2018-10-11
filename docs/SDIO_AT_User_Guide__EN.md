ESP32 SDIO AT Guide 

# Overview
SDIO AT is based on ESP32 AT, using SDIO, instead of UART, to communicate with host MCU. The ESP32 SDIO AT runs as an SDIO slave.   
SDIO communication needs at least 4 pins: CMD, CLK, DAT0, DAT1; 

* for one line mode, DAT1 runs as the interrupt pin; 
* for four lines mode, two more pins (DAT2 and DAT3) are needed.  

SDIO SLAVE pins is as below: 
 
 * CLK is GPIO14
 * CMD is GPIO15
 * DAT0 is GPIO2
 * DAT1 is GPIO4
 * DAT2 is GPIO12 (for four lines mode only)
 * DAT3 is GPIO13 (for four lines mode only)

# Flashing Firmware
## ESP-SDIO-TESTBOARD-V1

1. Turn switch 1, 2, 3, 4, 5 “ON”; and others are “OFF”. 
2. Flashing firmware to the master. After the flashing completed, the light on ESP32 slave will turn on, it means that the master runs successfully, and power on the slave.
3. Flashing the SDIO AT firmware to the slave.

**Note**:  
If you use ESP32-DevKitC or ESP-WROVER-KIT V2 (or earlier versions), please refer to the [board-compatibility](https://github.com/espressif/esp-idf/tree/master/examples/peripherals/sdio#board-compatibility) to set strapping pins, and run the [SDIO demo](https://github.com/espressif/esp-idf/tree/master/examples/peripherals/sdio) firstly, to make sure the SDIO communication works properly. If it is, then you can start your SDIO AT journey.

# SDIO Data Transimission

## SDIO HOST 

1. Power on the SDIO SLAVE (this step is for ESP-SDIO-TESTBOARD-V1 only)  
    * ESP-SDIO-TESTBOARD-V1 contains one master and three slaves (ESP32, ESP8266 and ESP8089). 
    * To use ESP32 as SDIO slave, you need to pull down GPIO5, see `slave_power_on`.
2. Intialize SDIO HOST  
    * To initialize SDIO, including configure one line or four lines mode, SDIO frequency, initialize SD mode.
3. Negotiate SDIO Communication  
    * Negotiate SDIO parameters with the slave according to SDIO spec. Please note that SDIO HOST must wait till the slave startup done, then to start the negotiation. Usually the host needs to delay some time to wait.
4. Receive Data  
    * When the SDIO host detects an interrupt from DAT1, if it is the slave sends new packet to the host, the host will read those data by CMD53.
5. Send Data  
    * In SDIO AT demo, the data inputs from serial port, when the SDIO host gets it, the host will send it to the slave through SDIO by CMD53. 
    * Please note that if the sending time out, there may be something wrong with the slave, then we will re-initiate both SDIO host and slave. 
    * Also, after AT+RST or AT+RESTORE, both SDIO host and slave should be initiated again.

## SDIO SLAVE

​When SDIO slave receives data from SDIO host, the slave will inform the AT core and give the data to the AT core to handle. After the AT core finished the work, the slave will send data to the host as feedback.

1. Initialize SDIO Slave
    * Call `sdio_slave_initialize` to initialize SDIO slave driver
    * Call `sdio_slave_recv_register_buf` to register receiving buffer. To make it receive data faster, you can register multiple buffers.
    * Call `sdio_slave_recv_load_buf` to load the receiving buffer, ready to receive data.
    * Call `sdio_slave_set_host_intena` to set interrupt for host, which mainly used is the `SDIO_SLAVE_HOSTINT_SEND_NEW_PACKET`, to notify that there is a new packet sent from the host.
    * Call `sdio_slave_start` to start SDIO hardware transmission.
2. Send Data
    * Since the SDIO slave data transmission via DMA, you need to copy the data from AT core to the memory which DMA can read firstly.
    * Call `sdio_slave_transmit` to send the data.
3. Receive Data
    * To speed up the data transmission, after receiving data by `sdio_slave_recv`, we use a circular linked list to transmit the received data to the AT core.