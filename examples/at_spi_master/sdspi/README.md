# Introduction

The SDSPI AT uses "SDIO-SPI" mode to communicate with MCU which is as an SPI host. The communication process is shown as the following picture:

![dataflow](res/data_flow.png)

 * ESP32 as slave uses SDIO hardware and SDIO driver to transmit data.
 * The MCU as host uses SPI hardware to transmit SDIO data.

Since the SDIO protocol is a pure software part, which has low coupling degree with the SPI hardware, we encapsulated the SDIO software protocol and make it platform-independent. In this case, users only need to implement the SPI hardware interfaces of the MCU, and can use the upper application directly.

### SPEED

Herein we used one ESP32 as the host MCU, another ESP32 running SDIO AT as the slave. And we tested its TCP throughput.  
Test environment: SPI clock 20 MHz, in shielding box, [optimized WiFi configuration](https://github.com/espressif/esp-idf/blob/master/examples/wifi/iperf/sdkconfig.defaults) for high throughput.  

 * When make the MCU send data 4096 bytes per time, the TCP throughput is about 1.2MB/s in the shielding box.  

# Hardware interface

For the hardware interface, it is through 5-channel signal line, including SCLK, MOSI, MISO, CS line in SPI protocol and 1-channel GPIO interrupt signal. The wiring diagram of ESP32 and MCU is shown as following table: 

| ESP32 |   MCU    |
| :---: | :------: |
|  CLK  |   SCLK   |
|  CMD  |   MOSI   |
| DAT0  |   MISO   |
| DAT1  | GPIO INT |
| DAT3  |    CS    |

In the SPI data transmission, DAT1, DAT2 and DAT3 are not used. DAT1 is reused as the interrupt pin to monitor the data from the SDIO slave. After receiving data, the MCU needs to clear the interruption.

DAT3 is reused as the CS line. For each command, SDIO slave is activated by pulling down the CS line. During SPI transmission (including command, response, data reading and writing), it is necessary to keep the CS line in the activated (low level) state.

# Usage

The SDIO SPI communication can be tested  according to the signal line of the hardware interface.
- MCU needs to run the `ESP32` or  `STM32` program 
- A ESP32 as slave run SDIO AT program ( `./build.py menuconfig` --> `Component config` --> `AT` --> `communicate method for AT command` --> `AT through SDIO`)

It should be noted that CMD and DATA lines D0-D3 of the slave should be pulled up by 50KOhm resistor even when there is no connection. In addition, the voltage selection efuse should be written as 3.3v. For specific notes, please refer to [SD Pullup Requirements](https://espressif-docs.readthedocs-hosted.com/projects/esp-idf/en/latest/api-reference/peripherals/sd_pullup_requirements.html)

# How do I configure the MCU

We offered ESP32 & STM32 (Model: STM32F103ZET6) as the MCU solution. This section use ESP32 as MCU illustrate how to port to your own platform.

Herein we used one ESP32 as the host MCU, running the example `components/platform/esp32`, another ESP32 running the SDIO AT as the slave. Users can refer to our ESP32 MCU example to implement their own host MCU. The directory structure of ESP32 which runs as the MCU is as follows:

```
.
├── components
│   ├── platform
│   │   ├── component.mk
│   │   └── esp32
│   │       ├── include
│   │       │   └── port.h
│   │       └── port.c
│   └── sdspi_host
│       ├── component.mk
│       ├── include
│       │   └── sdspi_host.h
│       └── sdspi_host.c
├── main
│   ├── app_main.c
│   └── component.mk
├── Makefile
├── README.md
└── sdkconfig.defaults

```

- **components**：SDIO SPI universal features and code for ESP32 adapter layer
  - **platform**：The platform-specific part needs to implement the interface according to its own platform
  - **sdio_host**：The driver part of SDIO SPI is platform independent and can be used directly
- **main**：Application software

When porting to your own MCU, the most important part is to implement the file port.c, which including SPI, GPIO and some system-related interfaces.
You can refer to the function definition and description in the port.h file.

The app\_main.c file is mainly the usage of SDIO SPI, including two threads (the receiving thread and the sending thread).
For example, when an ESP32 running the at\_sdspi_host as the host MCU, its sending thread will send the data (received from UART) via SDIO-SPI to the slave (another ESP32 running SDIO AT); and its receiving thread will wait for receiving data from the slave (ESP32 SDIO AT).

***Note:***   
It is strongly recommended to **make sure the hardware works well firstly** before starting integrating to your own MCU platform.
Use two ESP32s, one downloads the SDIO AT running as the slave, and another downloads the at\_sdspi_host running as the host MCU, to troubleshoot  potential hardware issues and also as a reference of future migration.  

# Possible problems

**Q** : Fail to initialize, keep printing `E sdspi_transaction: CMD0 response error, expect 0x1, response 0  E sdspi_transaction: Please restart slave and test again,error code:264`

**A** :   
Step 1: According to the SDIO protocol, SDIO host needs to send CMD0 command to make SDIO slave enter idle state firstly, and then re-send CMD0 and lower the CS line to set SDIO slave into SPI mode, and then R1 should be responded.  
Step 2: It will print these log if read R1 respond fail after Step 1, then there may be a hardware problem between the MCU and SDIO slave. Please check the hardware firstly. Note whether the pull up resistor is added and efuse is burned.

# Next action items

- [x] MCU adds support for STM32 (ARM) (Supported)
- [ ] MCU adds support for Raspberry Pi 3 (Linux )


# 简介

SDSPI AT 是 MCU 通过 SDIO 协议中的 SPI 模式与 ESP32 侧的 SDIO 进行通信，通信流程如下图所示：

![dataflow](res/data_flow.png)

- 作为 Slave 的 ESP32 使用 SDIO 硬件接口，通过 SDIO 协议和 host 进行通信.
- 作为 host 的 MCU 使用 SPI 接口，通过传输 SDIO 协议数据，和 Slave 进行 SDIO 通信.

考虑到 SDIO 协议是纯软件的部分，且与 SPI 硬件耦合程度较低，因此将这部分进行封装，转换为与平台无关的形式。在此 example 中，用户只需要基于 MCU，实现 SPI、GPIO 等硬件相关操作，适配此 example 即可。 

### 速率

我们使用一个 ESP32 作为 host MCU， 另一个 ESP32 在 slave 侧运行 SDIO AT， 测试 TCP 吞吐如下：

测试环境： SPI clock 20 MHz, 在屏蔽箱使用优化过 WiFi 配置项的 SDIO AT 固件（可以参考 iperf 的[优化配置](https://github.com/espressif/esp-idf/blob/master/examples/wifi/iperf/sdkconfig.defaults)）

- MCU 每次发送 4096 bytes， 在屏蔽箱中测试 TCP 吞吐率可以达到 1.2MB/s

# 硬件接口

在硬件接口方面，协议通过 5 路信号线实现，包括 SPI 协议中的 SCLK，MOSI，MISO， CS 线以及1路中断信号，其中，ESP32 与 MCU 的接线图如下所示：

| ESP32 |   MCU    |
| :---: | :------: |
|  CLK  |   SCLK   |
|  CMD  |   MOSI   |
| DAT0  |   MISO   |
| DAT1  | GPIO INT |
| DAT3  |    CS    |

在 SDIO SPI 数据传输中，并不会用到 DAT1，DAT2 和 DAT3 ； 在 SDIO 协议的 SPI 模式中，DAT1 被复用做中断线，用于监测 SDIO slave 端的数据，在监测到数据后，MCU 侧需要清除中断。
DAT3 被复用作 CS 线，对于每一个命令， SDIO slave 均是通过拉低 CS 线来激活的，而在 SPI 传输过程中（包括命令，响应以及读写数据），均需要保持 CS 线处于激活（低电平）状态。

# 使用方法

按照硬件接口的信号线对接即可测试 SDIO SPI 通信

- MCU 需要运行  `at_sdspi_host` 示例程序
- ESP32 在 slave 侧运行 SDIO AT 程序 （在 ESP32-AT 目录下配置  `./build.py menuconfig` --> `Component config` --> `AT` --> `communicate method for AT command` --> `AT through SDIO`）

 需要注意的是 SDIO slave 的 CMD 和 D0-D3 即使在没有连接时都应该连接一个 50K 的上拉电阻，另外需要将电压选择 efuse 烧写为 3.3v，具体注意事项请参考 [SD Pullup Requirements](https://espressif-docs.readthedocs-hosted.com/projects/esp-idf/en/latest/api-reference/peripherals/sd_pullup_requirements.html)

# 如何移植到自己的 MCU

我们提供了 ESP32 & STM32 （型号：STM32F103ZET6）作为 MCU 的方案，本章节以 ESP32 为例介绍如何移植到自己的 MCU，用户可以参照  ESP32 `components/platform/esp32`示例代码在自己的 host MCU 上实现。

其中ESP32 MCU 侧示例代码的目录结构如下：

```
.
├── components
│   ├── platform
│   │   ├── component.mk
│   │   └── esp32
│   │       ├── include
│   │       │   └── port.h
│   │       └── port.c
│   └── sdspi_host
│       ├── component.mk
│       ├── include
│       │   └── sdspi_host.h
│       └── sdspi_host.c
├── main
│   ├── app_main.c
│   └── component.mk
├── Makefile
├── README.md
└── sdkconfig.defaults

```

- **components**： SDIO SPI 通用功能和针对 ESP32 适配层的代码
  - **platform**：平台相关的接口，需要根据自己的 MCU 进行实现
  - **sdio_host**：SDIO SPI 的驱动部分，与平台无关，可以直接使用
- **main**：应用程序

当移植到自己的 MCU 时，最重要的部分是实现 port.c 文件，这个文件包含了 SPI， GPIO 和一些系统相关的接口。可以参考 port.h 中的一些函数声明。

app_main.c 文件主要是 sdio spi 的使用，包含了两个线程（接收和发送线程），在 ESP32 作为 host MCU 的实现中， 发送线程将会通过 SDIO-SPI 将数据（通过 UART 接收）发送给 slave （使用另一个 ESP32 运行 SDIO AT）；接收线程则一直等待接收 slave （ESP32 SDIO AT）发送的数据并将其打印出来。 

**注意：**

**强烈建议**在开始移植到自己的 MCU 之前确保 SDIO 硬件是没有问题的，这个测试可以排查可能出现的 SDIO 硬件问题。 

# 可能遇到的问题

**Q** : 初始化不成功，一直打印`E sdspi_transaction: CMD0 response error, expect 0x1, response 0   E sdspi_transaction: Please restart slave and test again,error code:264`

**A** : 根据 SDIO 协议规定，Host 需要发送 CMD0 使得 SDIO slave 进入 idle 状态，重新发送 CMD0 并拉低 CS 线会设置 SDIO slave 进入 SPI 模式，此时会回应 R1。出现上述问题，代表没有收到 SDIO slave 回复的 R1，这个说明  MCU 与 SDIO slave 的硬件存在问题，请注意是否加入了上拉电阻并烧写 efuse。

# 接下来的工作

- [x] MCU 增加 STM32 的支持 (ARM)
- [ ] MCU 增加树莓派 3 的支持 (Linux)
