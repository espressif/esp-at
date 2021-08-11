ESP32 SDIO AT 指南
==================

:link_to_translation:`en:[English]`

简介
----

ESP32 SDIO AT 使用 SDIO 协议进行通讯，其中 ESP32 作为 SDIO slave 与 MCU 进行通信。

SDIO 可使用 一线或四线模式。至少需要 4 根线：CMD、CLK、DAT0 和 DAT1。

- 对于一线模式，至少需要 4 根线：CMD、CLK、DAT0 和 DAT1，其中 DAT1 作为中断线；
- 对于四线模式，需要增加 DAT2 和 DAT3。

SDIO slave 管脚如下所示：

- CLK：GPIO14
- CMD：GPIO15
- DAT0：GPIO2
- DAT1：GPIO4
- DAT2：GPIO12（四线）
- DAT3：GPIO13（四线）

如何使用 SDIO AT
----------------

在测试 SDIO AT 通信之前，首先请参照 `SD Pull-up Requirements <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-reference/peripherals/sd_pullup_requirements.html#solutions>`_ 的介绍对 ESP32 硬件进行处理，否则 SDIO 通信将会异常。

Slave 侧  
^^^^^^^^

AT 项目默认使用 UART 作为传输介质，你可以通过 ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``communicate method for AT command`` -> ``AT throughSDIO`` 切换为用 SDIO 作为传输介质。之后重新编译 ``esp-at`` 工程，烧录新的固件并运行。

Host 侧
^^^^^^^

ESP-AT 提供了 ESP32 和 STM32 作为 SDIO host 的 :example:`at_sdio_host` 参考示例。对于 ESP32 的参考示例，可以使用 AT 工程的 esp-idf 版本进行编译烧录；对于 STM32，我们提供的方案基于 STM32F103ZET，请使用 Keil5 进行编译烧录。

对于其他平台，可以参照这两个示例进行适配。

SDIO 交互流程
-------------

Host 侧
^^^^^^^

1. 初始化 SDIO host

   - SDIO host 初始化主要是 SDIO 协议的初始化，包括设置 1 线或者 4 线、SDIO 频率、初始化 SD mode。

2. 协商 SDIO 通讯

   - 这部分主要按照 SDIO spec 协议标准的要求，跟 SDIO slave 协商参数。
   - 特别需要注意的是，如果 SDIO host 和 slave 同时重启，那么，协商需要等待 slave 初始化完成后再开始。一般 host 会在启动时添加延时，等待 slave 启动完成，再开始协商 SDIO 通信。

3. 接收数据

   - 接收数据主要依靠监测 DAT1 的中断信号。当接收到中断信号后，host 读取中断源并判断中断信号，如果中断是 slave 有数据要发送，host 会调用 CMD53 读取 slave 的数据。

4. 发送数据

   - SDIO AT DEMO 中，发送数据通过串口输入，然后 host 调用 CMD53 将数据发送过去。
   - 需要注意的是，如果发送超时，那么有可能 slave 侧出现异常，此时 host 和 slave 需要重新初始化 SDIO。
   - 在调用 :ref:`AT+RST <cmd-RST>` 或者 :ref:`AT+RESTORE <cmd-RESTORE>` 命令后，host 和 slave 也同样需要重新初始化 SDIO。

Slave 侧
^^^^^^^^

SDIO slave 的处理与 SDIO host 类似，slave 在接收到 SDIO host 发送的数据后，通知 AT core 并将数据发送给 AT core 进行处理，在 AT core 处理完成后，再发送数据给 SDIO host。

1. 初始化 SDIO slave

   - 调用 ``sdio_slave_initialize`` 初始化 SDIO slave driver。
   - 调用 ``sdio_slave_recv_register_buf`` 注册接收用的 buffer，为了加快接收速度，此处注册了多个接收 buffer。
   - 调用 ``sdio_slave_recv_load_buf`` 加载刚刚注册的 buffer，准备接收数据。
   - ``sdio_slave_set_host_intena`` 用于设置 host 可用中断，主要用到的是新数据包发送中断 ``SDIO_SLAVE_HOSTINT_SEND_NEW_PACKET``。
   - 调用 ``sdio_slave_start`` 在硬件上开始接收和发送。

2. 发送数据

   - 因为 SDIO slave 发送的数据需要保证能被 DMA 访问，所以需要先把 AT 中的数据拷贝到可被 DMA 访问的内存中，然后调用 ``sdio_slave_transmit`` 进行发送。

3. 接收数据

   - 为了优化接收 SDIO 数据传输给 AT core 的速率，在调用 ``sdio_slave_recv`` 接收 SDIO 数据后，使用了循环链表将接收到的数据传输到 AT core。