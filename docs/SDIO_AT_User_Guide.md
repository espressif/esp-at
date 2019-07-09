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


ESP32 SDIO AT 指南

# 简介
SDIO AT 基于 ESP32 AT，使用 SDIO 协议进行通讯，其中 ESP32 作为 SDIO slave 与 MCU 进行通信。SDIO 协议需要至少 4 根线：CMD，CLK，DAT0 和 DAT1；

* 对于一线模式，DAT1 作为中断线；
* 对于四线模式，需要增加 DAT2 和 DAT3。  

SDIO slave 管脚如下所示：

* CLK GPIO14
* CMD GPIO15
* DAT0 GPIO2
* DAT1 GPIO4
* DAT2 GPIO12 （四线）
* DAT3 GPIO13 （四线）

# SDIO 下载

## ESP-SDIO-TESTBOARD-V1 流程

1. 开关 1、2、3、4，5 拨至 ON，其他均为 OFF。
2. PC 为 master 烧录固件。烧录完成后，slave 侧 ESP32 模组的灯自动亮起，表示 master 成功运行，为 slave 供电。
3. PC 烧写 SDIO AT 程序到 slave。

**注意**：  
如果你使用 ESP32-DevKitC 或者 ESP-WROVER-KIT V2（或更早之前的板子）来验证 SDIO AT，首先请参照 SDIO demo 中的 [board-compatibility](https://github.com/espressif/esp-idf/tree/master/examples/peripherals/sdio#board-compatibility) 对 strapping 管脚进行处理，在此之后，强烈建议先运行 [SDIO demo](https://github.com/espressif/esp-idf/tree/master/examples/peripherals/sdio) 保证 SDIO 数据传输正常，再测试 SDIO AT。

# SDIO 交互流程

## Host 侧

0. SDIO slave 模组上电（此步骤仅针对 ESP-SDIO-TESTBOARD-V1 开发板）  
    * ESP-SDIO-TESTBOARD-V1 包含了一个 master 和 3个 slave（ESP32,ESP8266以及ESP8089）
    * 如果使用 ESP32 作为 SDIO slave，需要将 GPIO5 拉低，参见 `slave_power_on`。 
1. 初始化 SDIO host  
    * SDIO host 初始化主要是 SDIO 协议的初始化，包括设置 1 线或者 4 线，SDIO 频率，初始化 SD mode。
2. 协商 SDIO 通讯   
    * 这部分主要按照 SDIO spec 的要求，跟 SDIO slave 协商参数。
    * 特别需要注意的是，如果 SDIO host 和 slave 同时重启，那么，协商需要等待 slave 初始化完成后再开始。一般 host 会在启动时添加延时，等待 slave 启动完成，再开始协商 SDIO 通信。
3. 接收数据  
    * 接收数据主要依靠监测 DAT1 的中断信号。当接收到中断信号后，host 读取中断源并判断中断信号，如果中断是 slave 有数据要发送， host 会调用 CMD53 读取 slave 的数据。
4. 发送数据  
    * SDIO AT DEMO 中，发送数据通过串口输入，然后 host 调用 CMD53 将数据发送过去。
    * 需要注意的是，如果发送超时，那么有可能 slave 侧出现异常，此时 host 和 slave 需要重新初始化 SDIO。
    * 在调用 AT+RST 或者 AT+RESTORE 命令后，host 和 slave 也同样需要重新初始化 SDIO。

## Slave 侧

​SDIO slave 的处理与 SDIO host 类似，slave 在接收到 SDIO host 发送的数据后，通知 AT core 并将数据发送给 AT core 进行处理，在 AT core 处理完成后，再发送数据给 SDIO host。

1. 初始化 SDIO slave  
    * 调用 `sdio_slave_initialize` 初始化 SDIO slave driver
    * 调用 `sdio_slave_recv_register_buf` 注册接收用的 buffer，为了加快接收速度，此处注册了多个接收 buffer。
    * 调用 `sdio_slave_recv_load_buf` 加载刚刚注册的 buffer，准备接收数据
    * `sdio_slave_set_host_intena` 用于设置 host 可用中断，主要用到的是新数据包发送中断 `SDIO_SLAVE_HOSTINT_SEND_NEW_PACKET` 
    * 调用 `sdio_slave_start` 在硬件上开始接收和发送
2. 发送数据  
    * 因为 SDIO  slave 发送的数据需要保证能被 DMA 访问，所以需要先把 AT 中的数据拷贝到可被 DMA 访问的内存中，然后调用 sdio_slave_transmit 进行发送。
3. 接收数据  
    * 为了优化接收 SDIO 数据传输给 AT core 的速率，在调用 `sdio_slave_recv` 接收 SDIO 数据后，使用了循环链表将接收到的数据传输到 AT core。
