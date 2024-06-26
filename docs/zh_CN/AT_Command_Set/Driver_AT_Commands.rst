.. _Driver-AT:

驱动 AT 命令
================================================

:link_to_translation:`en:[English]`

- :ref:`介绍 <cmd-driver-intro>`
-  :ref:`AT+DRVADC <cmd-DRVADC>`：读取 ADC 通道值
-  :ref:`AT+DRVPWMINIT <cmd-DRVPWMINIT>`：初始化 PWM 驱动器
-  :ref:`AT+DRVPWMDUTY <cmd-DRVPWMDUTY>`：设置 PWM 占空比
-  :ref:`AT+DRVPWMFADE <cmd-DRVPWMFADE>`：设置 PWM 渐变
-  :ref:`AT+DRVI2CINIT <cmd-DRVI2CINIT>`：初始化 I2C 主机驱动
-  :ref:`AT+DRVI2CRD <cmd-DRVI2CRD>`：读取 I2C 数据
-  :ref:`AT+DRVI2CWRDATA <cmd-DRVI2CWRDATA>`：写入 I2C 数据
-  :ref:`AT+DRVI2CWRBYTES <cmd-DRVI2CWRBYTES>`：写入不超过 4 字节的 I2C 数据
-  :ref:`AT+DRVSPICONFGPIO <cmd-DRVSPICONFGPIO>`：配置 SPI GPIO
-  :ref:`AT+DRVSPIINIT <cmd-DRVSPIINIT>`：初始化 SPI 主机驱动
-  :ref:`AT+DRVSPIRD <cmd-DRVSPIRD>`：读取 SPI 数据
-  :ref:`AT+DRVSPIWR <cmd-DRVSPIWR>`：写入 SPI 数据

.. _cmd-driver-intro:

介绍
------

.. important::
  默认的 AT 固件不支持此页面下的 AT 命令。如果您需要 {IDF_TARGET_NAME} 支持驱动命令，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：

  - 启用 ``Component config`` -> ``AT`` -> ``AT driver command support``

.. _cmd-DRVADC:

:ref:`AT+DRVADC <Driver-AT>`：读取 ADC 通道值
--------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+DRVADC=<channel>,<atten>

**响应：**

::

    +DRVADC:<raw data>

    OK

参数
^^^^

-  **<channel>**：ADC1 通道。

  .. only:: esp32 or esp32s2

    - {IDF_TARGET_NAME} 设备的取值范围为 [0,7]。

      .. list-table::
        :header-rows: 1

        * - 通道
          - 管脚
        * - 0
          - GPIO36
        * - 1
          - GPIO37
        * - 2
          - GPIO38
        * - 3
          - GPIO39
        * - 4
          - GPIO32
        * - 5
          - GPIO33
        * - 6
          - GPIO34
        * - 7
          - GPIO35

  .. only:: esp32c3 or esp32c2 or esp32c6

    - {IDF_TARGET_NAME} 设备的取值范围为 [0,4]。

      .. list-table::
        :header-rows: 1

        * - 通道
          - 管脚
        * - 0
          - GPIO0
        * - 1
          - GPIO1
        * - 2
          - GPIO2
        * - 3
          - GPIO3
        * - 4
          - GPIO4

-  **<atten>**：衰减值。

  .. only:: esp32 or esp32s2

    - 0: 0 dB 衰减，有效测量范围为 [100, 950] mV。
    - 1: 2.5 dB 衰减，有效测量范围为 [100, 1250] mV。
    - 2: 6 dB 衰减，有效测量范围为 [150, 1750] mV。
    - 3: 11 dB 衰减，有效测量范围为 [150, 2450] mV。

  .. only:: esp32c3 or esp32c2 or esp32c6

    - 0: 0 dB 衰减，有效测量范围为 [0, 750] mV。
    - 1: 2.5 dB 衰减，有效测量范围为 [0, 1050] mV。
    - 2: 6 dB 衰减，有效测量范围为 [0, 1300] mV。
    - 3: 11 dB 衰减，有效测量范围为 [0, 2500] mV。

- **<raw data>**：ADC 通道值。

说明
^^^^

-  ESP-AT 只支持 ADC1。
-  {IDF_TARGET_NAME} 支持 12 位宽度。
-  对于如何将通道值转换为电压，可以参考 `ADC 转换 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-reference/peripherals/adc_continuous.html#adc-continuous-read-conversion-result>`__。

示例
^^^^

.. only:: esp32 or esp32s2

  ::

    // {IDF_TARGET_NAME} 设备设置为 0 dB 衰减，有效测量范围为 [100, 950] mV
    // 电压为 2048 / 4095 * 950 = 475.12 mV
    AT+DRVADC=0,0
    +DRVADC:2048

    OK

.. only:: esp32c3 or esp32c2 or esp32c6

  ::

    // {IDF_TARGET_NAME} 设备设置为 0 dB 衰减，有效测量范围为 [0, 750] mV
    // 电压为 2048 / 4095 * 750 = 375.09 mV
    AT+DRVADC=0,0
    +DRVADC:2048

    OK

.. _cmd-DRVPWMINIT:

:ref:`AT+DRVPWMINIT <Driver-AT>`：初始化 PWM 驱动器
----------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+DRVPWMINIT=<freq>,<duty_res>,<ch0_gpio>[,...,<ch3_gpio>]

**响应：**

::

    OK

参数
^^^^

-  **<freq>**：LEDC 定时器频率，单位：Hz，范围：1 Hz ~ 8 MHz。
-  **<duty_res>**：LEDC 通道占空比分辨率，范围：0 ~ 20 位。
-  **<chx_gpio>**：LEDC 通道 x 的输出 GPIO。例如，如果您想将 GPIO16 作为通道 0，需设置 ``<ch0_gpio>`` 为 16。

说明
^^^^

-  ESP-AT 最多能支持 4 个通道。
-  使用本命令初始化的通道数量直接决定了其它 PWM 命令（如 :ref:`AT+DRVPWMDUTY <cmd-DRVPWMDUTY>` 和 :ref:`AT+DRVPWMFADE <cmd-DRVPWMFADE>`）能够设置的通道。例如，如果您只初始化了两个通道，那么 ``AT+DRVPWMDUTY`` 命令只能用来更改这两个通道的 PWM 占空比。
-  频率和占空比分辨率相互影响。更多信息请见 `频率和占空比分辨率支持范围 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-reference/peripherals/ledc.html#ledc-api-supported-range-frequency-duty-resolution>`_。

示例
^^^^

::

    AT+DRVPWMINIT=5000,13,17,16,18,19  // 设置 4 个通道，频率为 5 kHz，占空比分辨率为 13 位
    AT+DRVPWMINIT=10000,10,17          // 只初始化通道 0，频率为 10 kHz，占空比分辨率为 10 位，其它 PWM 相关命令只能设置一个通道

.. _cmd-DRVPWMDUTY:

:ref:`AT+DRVPWMDUTY <Driver-AT>`：设置 PWM 占空比
-------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+DRVPWMDUTY=<ch0_duty>[,...,<ch3_duty>]

**响应：**

::

    OK

参数
^^^^

-  **<duty>**：LEDC 通道占空比，范围：[0,2 :sup:`占空比分辨率`]。

说明
^^^^

-  ESP-AT 最多能支持 4 个通道。
-  若某个通道无需设置占空比，直接省略该参数。

示例
^^^^

::

    AT+DRVPWMDUTY=255,512   // 设置通道 0 的占空比为 255，设置通道 1 的占空比为 512
    AT+DRVPWMDUTY=,,0       // 只设置通道 2 的占空比为 0

.. _cmd-DRVPWMFADE:

:ref:`AT+DRVPWMFADE <Driver-AT>`：设置 PWM 渐变
-----------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

     AT+DRVPWMFADE=<ch0_target_duty>,<ch0_fade_time>[,...,<ch3_target_duty>,<ch3_fade_time>]

**响应：**

::

    OK

参数
^^^^

-  **<target_duty>**：目标渐变占空比，范围：[0,2 :sup:`duty_resolution`–1]。
-  **<fade_time>**：渐变的最长时间，单位：毫秒。

说明
^^^^

-  ESP-AT 最多能支持 4 个通道。
-  若某个通道无需设置 ``<target_duty>`` 和 ``<fade_time>``，直接省略即可。

示例
^^^^

::

    AT+DRVPWMFADE=,,0,1000           // 使用一秒的时间将通道 1 的占空比设置为 0
    AT+DRVPWMFADE=1024,1000,0,2000,  // 使用一秒的时间将通道 0 的占空比设置为 1024、两秒的时间将通道 1 的占空比设为 0

.. _cmd-DRVI2CINIT:

:ref:`AT+DRVI2CINIT <Driver-AT>`：初始化 I2C 主机驱动
---------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

     AT+DRVI2CINIT=<num>,<scl_io>,<sda_io>,<clock>

**响应：**

::

    OK

参数
^^^^

-  **<num>**：I2C 端口号，范围：0 ~ 1。如果未设置后面的参数，AT 将不初始化该 I2C 端口。 
-  **<scl_io>**：I2C SCL 信号的 GPIO 号。
-  **<sda_io>**：I2C SDA 信号的 GPIO 号。
-  **<clock>**：主机模式下的 I2C 时钟频率，单位：Hz，最大值：1 MHz。

说明
^^^^

-  本命令只支持 I2C 主机。

示例
^^^^

::

    AT+DRVI2CINIT=0,25,26,1000  // 初始化 I2C0，SCL：GPIO25，SDA：GPIO26，I2C 时钟频率：1 kHz
    AT+DRVI2CINIT=0             // 取消 I2C0 初始化

.. _cmd-DRVI2CRD:

:ref:`AT+DRVI2CRD <Driver-AT>`：读取 I2C 数据
-------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

     AT+DRVI2CRD=<num>,<address>,<length>

**响应：**

::

    +DRVI2CRD:<read data>
    OK

参数
^^^^

-  **<num>**：I2C 端口号，范围：0 ~ 1。
-  **<address>**：I2C 从机设备地址：

   -  7 位地址：0 ~ 0x7F；
   -  10 位地址：第一个字节的前七个位是 1111 0XX，其中最后两位 XX 是 10 位地址的最高两位。例如，如果 10 位地址为 0x2FF (b'1011111111)，那么输入的地址为 0x7AFF (b'111101011111111)。

-  **<length>**：I2C 数据长度，范围：1 ~ 2048。
-  **<read data>**：I2C 数据。

说明
^^^^

-  I2C 传输超时时间为一秒。

示例
^^^^

::

    AT+DRVI2CRD=0,0x34,1     // I2C0 从地址 0x34 处读取 1 字节的数据
    AT+DRVI2CRD=0,0x7AFF,1   // I2C0 从 10 位地址 0x2FF 处读取 1 字节的数据

    // I2C0 读地址 0x34，寄存器地址 0x27，读 2 字节
    AT+DRVI2CWRBYTES=0,0x34,1,0x27     // I2C0 先写设备地址 0x34、寄存器地址 0x27
    AT+DRVI2CRD=0,0x34,2               // I2C0 读地址 2 字节

.. _cmd-DRVI2CWRDATA:

:ref:`AT+DRVI2CWRDATA <Driver-AT>`：写入 I2C 数据
------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

     AT+DRVI2CWRDATA=<num>,<address>,<length>

**响应：**

::

    OK
    >

收到上述响应后，请输入您想写入的数据，当数据达到参数指定长度后，数据传输开始。

若数据传输成功，则返回：

::

    OK 

若数据传输失败，则返回：

::

    ERROR

参数
^^^^

-  **<num>**：I2C 端口号，范围：0 ~ 1。
-  **<address>**：I2C 从机设备地址：

   -  7 位地址：0 ~ 0x7F；
   -  10 位地址：第一个字节的前七个位是 1111 0XX，其中最后两位 XX 是 10 位地址的最高两位。例如，如果 10 位地址为 0x2FF (b'1011111111)，那么输入的地址为 0x7AFF (b'111101011111111)。

-  **<length>**：I2C 数据长度，范围：1 ~ 2048。

说明
^^^^

-  I2C 传输超时时间为一秒。

示例
^^^^

::

    AT+DRVI2CWRDATA=0,0x34,10   // I2C0 写入 10 字节数据至地址 0x34

.. _cmd-DRVI2CWRBYTES:

:ref:`AT+DRVI2CWRBYTES <Driver-AT>`：写入不超过 4 字节的 I2C 数据
---------------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

     AT+DRVI2CWRBYTES=<num>,<address>,<length>,<data>

**响应：**

::

    OK

参数
^^^^

-  **<num>**：I2C 端口号，范围：0 ~ 1。
-  **<address>**：I2C 从机设备地址。

   -  7 位地址：0 ~ 0x7F。
   -  10 位地址：第一个字节的前七个位是 1111 0XX，其中最后两位 XX 是 10 位地址的最高两位。例如，如果 10 位地址为 0x2FF (b'1011111111)，那么输入的地址为 0x7AFF (b'111101011111111)。

-  **<length>**：待写入的 I2C 数据长度，范围：1 ~ 4 字节。
-  **<data>**：参数 ``<length>`` 指定长度的数据，范围：0 ~ 0xFFFFFFFF。

说明
^^^^

-  I2C 传输超时时间为一秒。

示例
^^^^

::

    AT+DRVI2CWRBYTES=0,0x34,2,0x1234     // I2C0 写入 2 字节数据 0x1234 至地址 0x34
    AT+DRVI2CWRBYTES=0,0x7AFF,2,0x1234   // I2C0 写入 2 字节数据 0x1234 至 10 位地址 0x2FF

    // I2C0 写地址 0x34、寄存器地址 0x27，数据为 c0xFF
    AT+DRVI2CWRBYTES=0,0x34,2,0x27FF

.. _cmd-DRVSPICONFGPIO:

:ref:`AT+DRVSPICONFGPIO <Driver-AT>`：配置 SPI GPIO
---------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

     AT+DRVSPICONFGPIO=<mosi>,<miso>,<sclk>,<cs>

**响应：**

::

    OK

参数
^^^^

-  **<mosi>**：主出从入信号对应的 GPIO 管脚。
-  **<miso>**：主入从出信号对应 GPIO 管脚，若不使用，置位 -1。
-  **<sclk>**：SPI 时钟信号对应的 GPIO 管脚。
-  **<cs>**：选择从机的信号对应 GPIO 管脚，若不使用，置位 -1。

.. _cmd-DRVSPIINIT:

:ref:`AT+DRVSPIINIT <Driver-AT>`：初始化 SPI 主机驱动
----------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+DRVSPIINIT=<clock>,<mode>,<cmd_bit>,<addr_bit>,<dma_chan>[,bits_msb]

**响应：**

::

    OK

参数
^^^^

-  **<clock>**：时钟速度，分频数为 80 MHz，单位：Hz，最大值：40 MHz。
-  **<mode>**：SPI 模式，范围：0 ~ 3。
-  **<cmd_bit>**：命令阶段的默认位数，范围：0 ~ 16。
-  **<addr_bit>**：地址阶段的默认位数，范围：0 ~ 64。
-  **<dma_chan>**：通道 1 或 2，不需要 DMA 时也可为 0。
-  **<bits_msb>**：SPI 数据格式：
   
   - bit0:
    
     - 0: 先传输 MSB（默认）；
     - 1: 先传输 LSB。
   
   - bit1:

     - 0: 先接收 MSB（默认）；
     - 1: 先接收 LSB。

说明
^^^^

- 请在 SPI 初始化前配置 SPI GPIO。

示例
^^^^

::

    AT+DRVSPIINIT=102400,0,0,0,0,3 // SPI 时钟：100 kHz；模式：0；命令阶段和地址阶段默认位数均为 0；不使用 DMA；先传输和接收 LSB
    OK
    AT+DRVSPIINIT=0   // 删除 SPI 驱动
    OK 

.. _cmd-DRVSPIRD:

:ref:`AT+DRVSPIRD <Driver-AT>`：读取 SPI 数据
-------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

     AT+DRVSPIRD=<data_len>[,<cmd>,<cmd_len>][,<addr>,<addr_len>]

**响应：**

::

    +DRVSPIRD:<read data>
    OK

参数
^^^^

-  **<data_len>**：待读取的 SPI 数据长度，范围：1 ~ 4092 字节。
-  **<cmd>**：命令数据，数据长度由 ``<cmd_len>`` 参数设定。
-  **<cmd_len>**：本次传输中的命令长度，范围：0 ~ 2 字节。
-  **<addr>**：命令地址，地址长度由 ``<addr_len>`` 参数设定。
-  **<addr_len>**：本次传输中地址长度，范围：0 ~ 4 字节。

说明
^^^^

-  若不使用 DMA，``<data_len>`` 参数每次能够设定的最大值为 64 字节。

示例
^^^^

::

    AT+DRVSPIRD=2  // 读取 2 字节数据
    +DRVI2CREAD:ffff
    OK

    AT+DRVSPIRD=2,0x03,1,0x001000,3  // 读取 2 字节数据，<cmd> 为 0x03，<cmd_len> 为 1 字节，<addr> 为 0x1000，<addr_len> 为 3 字节
    +DRVI2CREAD:ffff
    OK

.. _cmd-DRVSPIWR:

:ref:`AT+DRVSPIWR <Driver-AT>`：写入 SPI 数据
--------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+DRVSPIWR=<data_len>[,<cmd>,<cmd_len>][,<addr>,<addr_len>]

**响应：**

当 ``<data_len>`` 参数值大于 0，AT 返回：

::

    OK
    >

收到上述响应后，请输入您想写入的数据，当数据达到参数指定长度后，数据传输开始。

若数据传输成功，AT 返回：

::

    OK

当 ``<data_len>`` 参数值为 0 时，也即 AT 只传输命令和地址，不传输 SPI 数据，此时 AT 返回：

::

    OK 

参数
^^^^

-  **<data_len>**：SPI 数据长度，范围：0 ~ 4092。
-  **<cmd>**：命令数据，数据长度由 ``<cmd_len>`` 参数设定。
-  **<cmd_len>**：本次传输中的命令长度，范围：0 ~ 2 字节。
-  **<addr>**：命令地址，地址长度由 ``<addr_len>`` 参数设定。
-  **<addr_len>**：本次传输中地址长度，范围：0 ~ 4 字节。

说明
^^^^

-  若不使用 DMA，``<data_len>`` 参数每次能够设定的最大值为 64 字节。

示例
^^^^

::

    AT+DRVSPIWR=2  // 写入 2 字节数据
    OK
    >              // 开始接收串行数据
    OK

    AT+DRVSPIWR=0,0x03,1,0x001000,3  // 写入 0 字节数据，<cmd> 为 0x03，<cmd_len> 为 1 字节，<addr> 为 0x1000，<addr_len> 为 3 字节
    OK