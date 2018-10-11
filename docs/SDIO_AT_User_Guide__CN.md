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