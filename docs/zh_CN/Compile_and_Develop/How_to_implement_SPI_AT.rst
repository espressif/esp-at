SPI AT 指南
=============

本文档主要介绍 SPI AT 的实现与使用，主要涉及以下几个方面：

.. contents::
   :local:
   :depth: 1

简介
------

SPI AT 基于 AT 工程，使用 SPI 协议进行数据通信。在使用 SPI AT 时，MCU 设备作为 SPI master，{IDF_TARGET_NAME} 设备作为 SPI slave，通信双方通过 SPI 协议实现基于 AT 命令的数据交互。

使用 SPI AT 的优势
^^^^^^^^^^^^^^^^^^^

AT 工程默认使用 UART 协议进行数据通信，但是 UART 协议在一些需要高速传输数据的应用场景并不适用，因此，使用支持更高传输速率的 SPI 协议传输数据成为一种较好的选择。

如何启用 SPI AT？
^^^^^^^^^^^^^^^^^^^

您可以通过下述步骤配置并启用 SPI AT：

1. 通过 ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``communicate method for AT command`` -> ``AT through SPI`` 使能 SPI AT。
2. 通过 ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``communicate method for AT command`` -> ``AT SPI Data Transmission Mode`` 选择 SPI 数据传输模式。
3. 通过 ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``communicate method for AT command`` -> ``AT SPI GPIO settings`` 配置 SPI 使用的 GPIO 管脚。
4. 通过 ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``communicate method for AT command`` -> ``AT SPI driver settings`` 选择 SPI 从机的工作模式，并配置相关缓存区的大小。
5. 重新编译 ``esp-at`` 工程（参考 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`），烧录新的固件并运行。

SPI AT 默认管脚
^^^^^^^^^^^^^^^^

下表给出了不同系列的 {IDF_TARGET_NAME} 设备使用 SPI AT 时默认的硬件管脚：

.. only:: esp32c2 or esp32c3

  .. list-table:: SPI AT 默认管脚
    :widths: 10 25
    :header-rows: 1

    * - 信号
      - GPIO 编号
    * - SCLK
      - 6
    * - MISO
      - 2
    * - MOSI
      - 7
    * - CS
      - 10
    * - HANDSHAKE
      - 3
    * - GND
      - GND
    * - QUADWP (qio/qout) :sup:`1`
      - 8
    * - QUADHD (qio/qout) :sup:`1`
      - 9

.. only:: esp32c6

  .. list-table:: SPI AT 默认管脚
    :widths: 10 25
    :header-rows: 1

    * - 信号
      - GPIO 编号
    * - SCLK
      - 19
    * - MISO
      - 20
    * - MOSI
      - 18
    * - CS
      - 23
    * - HANDSHAKE
      - 21
    * - GND
      - GND
    * - QUADWP (qio/qout) :sup:`1`
      - 22
    * - QUADHD (qio/qout) :sup:`1`
      - 2

**说明** 1：QUADWP 引脚和 QUADHD 引脚仅在使用 4 线 SPI 工作时使用。

您可以通过 ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``communicate method for AT command`` > ``AT through SPI`` > ``AT SPI GPIO settings``，然后编译工程来配置 SPI AT 对应的管脚（参考 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`）。

使用 SPI AT
--------------

在使用 SPI AT 时，{IDF_TARGET_NAME} 设备上运行的 SPI slave 工作在半双工通信模式下。

握手线 (handshake line)
^^^^^^^^^^^^^^^^^^^^^^^^

SPI 是一种 master-slave 结构的外设，所有的传输均由 master 发起，slave 无法主动传输数据。但是，使用 AT 命令进行数据交互时，需要 {IDF_TARGET_NAME} 设备主动能够主动上报一些信息。因此，我们在 SPI master 和 slave 之间添加了一个握手线，来实现 slave 主动向 master 上报信息的功能。

当 slave 需要传输数据时，将会把握手管脚主动拉高，这会在 master 侧产生一个上升沿的 GPIO 中断，master 发起与 slave 的通信，传输完成后，slave 将握手管脚拉低，结束此次通信。 

使用握手线的具体方法为：

- Master 向 slave 发送 AT 数据时，使用握手线的方法为：

  1. master 向 slave 发送请求传输数据的请求，然后等待 slave 向握手线发出的允许发送数据的信号。
  2. master 检测到握手线上的 slave 发出的允许发送的信号后，开始发送数据。
  3. master 发送数据后，通知 slave 数据发送结束。

- Master 接收 slave 发送的 AT 数据时，使用握手线的方法为：

  1. slave 通过握手线通知 master 开始接收数据。
  2. master 接收数据，并在接收所有数据后，通知 slave 数据已经全部接收。

通信格式
^^^^^^^^^^^^

SPI AT 的通信格式为 CMD+ADDR+DUMMY+DATA（读/写）。在使用 SPI AT 时，SPI master 使用到的一些通信报文介绍如下：

- Master 向 slave 发送数据时的通信报文：

.. list-table:: 主机向从机发送数据
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD（1 字节）
     - ADDR（1 字节）
     - DUMMY（1 字节）
     - DATA（高达 4092 字节）
   * - 0x3
     - 0x0
     - 0x0
     - data_buffer

- Master 向 slave 发送数据结束后，需要发送一条通知消息来结束本次传输，具体的通信报文为：

.. list-table:: 主机通知从机，主机发送数据已经结束
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD（1 字节）
     - ADDR（1 字节）
     - DUMMY（1 字节）
     - DATA
   * - 0x7
     - 0x0
     - 0x0
     - null

- Master 接收 slave 发送的数据时的通信报文：

.. list-table:: 主机读取从机发送的数据
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD（1 字节）
     - ADDR（1 字节）
     - DUMMY（1 字节）
     - DATA（高达 4092 字节）
   * - 0x4
     - 0x0
     - 0x0
     - data_buffer

- Master 接收 slave 发送的数据后，需要发送一条通知消息来结束本次传输，具体的通信报文为：

.. list-table:: 主机通知从机，主机读取数据已经结束
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD（1 字节）
     - ADDR（1 字节）
     - DUMMY（1 字节）
     - DATA
   * - 0x8
     - 0x0
     - 0x0
     - null

- Master 向 slave 发送请求传输指定大小数据的通信报文：

.. list-table:: 主机向从机发送写数据请求
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD（1 字节）
     - ADDR（1 字节）
     - DUMMY（1 字节）
     - DATA（4 字节）
   * - 0x1
     - 0x0
     - 0x0
     - data_info

其中 4 字节的 data_info 中包含了本次请求传输数据的数据包信息，具体格式如下：

1. Master 向 slave 发送的数据的字节数，长度 0～15 bit。
2. Master 向 slave 发送的数据包的序列号，该序列号在 master 每次发送时递增，长度 16～23 bit。
3. Magic 值，长度 24～31 bit，固定为 0xFE。

- Master 检测到握手线上有 slave 发出的信号后，需要发送一条消息查询 slave 进入接收数据的工作模式，还是进入到发送数据的工作模式，具体的通信报文为：

.. list-table:: 主机发送请求，查询从机的可读/可写状态
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD（1 字节）
     - ADDR（1 字节）
     - DUMMY（1 字节）
     - DATA（4 字节）
   * - 0x2
     - 0x4
     - 0x0
     - slave_status

发送查询请求后，slave 返回的状态信息将存储在 4 字节的 slave_status 中，其具体的格式如下：

1. slave 需要向 master 发送的数据的字节数，长度 0～15 bit；仅当 slave 处于可读状态时，该字段数字有效。
2. 数据包序列号，长度 16～23 bit，当序列号达到最大值 0xFF 时，下一个数据包的序列号重新设置为 0x0。当 slave 处于可写状态时，该字段为 master 需向 slave 发送的下一下数据包的序列号；当 slave 处于可读状态时，该字段为 slave 向 master 发送的下一个数据包的序列号。
3. slave 的可读/可写状态，长度 24～31 bit， 其中，0x1 代表可读， 0x2 代表可写。

SPI AT 数据交互流程
^^^^^^^^^^^^^^^^^^^^^^
SPI AT 数据交互流程主要分为两个方面：

- SPI master 向 slave 发送 AT 命令 ：

.. code-block:: none

         | SPI master |                            | SPI slave |
                |                                        |
                |   -------step 1: request to send---->  |
                |                                        |
                |   <------step 2: GPIO interrupt------  |
                |                                        |
                |   -------step 3: read slave status-->  |
                |                                        |
                |   -------step 4: send data---------->  |
                |                                        |
                |   -------step 5: send done---------->  |

每个步骤具体的说明如下：

1. master 向 slave 发送请求向 slave 写数据的请求。
2. slave 接收 master 的发送请求，若此时 slave 允许接收数据，则向 slave_status 寄存器写入允许 master 写入的标志位，然后通过握手线触发 master 上的 GPIO 中断。
3. master 接收到中断后，读取 slave 的 slave_status 寄存器，检测到 slave 进入接收数据的状态。
4. master 开始向 slave 发送数据。
5. 发送数据结束后，master 向 slave 发送一条代表发送结束的消息。

- SPI slave 向 master 发送 AT 响应：

.. code-block:: none

         | SPI master |                            | SPI slave |
                |                                        |
                |   <------step 1: GPIO interrupt------  |
                |                                        |
                |   -------step 2: read slave status-->  |
                |                                        |
                |   <------step 3: send data-----------  |
                |                                        |
                |   -------step 4: receive done------->  |

每个步骤具体的说明如下：

1. slave 向 slave_status 寄存器写入允许 master 读取来自 slave 的数据的标志位,然后通过握手线触发 master 上的 GPIO 中断。
2. master 接收到中断后，读取 slave 的 slave_status 寄存器，检测到 slave 进入发送数据的状态。 
3. master 开始接收来自 slave 的数据。
4. 数据接收完毕后，master 向 slave 发送一条代表接收数据结束的消息。

**说明** 1. 为了方便理解，我们还以发送 AT 命令为例，提供了通信涉及的所有交互流程和逻辑分析仪数据，请参考 :example_file:`at_spi_master/spi/esp32_c_series/README.md`。

SPI AT 对应的 SPI master 侧示例代码
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SPI AT 本身是作为 SPI slave 使用的，使用 SPI master 与 SPI slave 进行通信的示例代码请参考 :example:`at_spi_master/spi/esp32_c_series`。

**说明** 1. 在使用 MCU 开发之前，强烈建议使用 ESP32-C3 或者 ESP32 模拟 MCU 作为 SPI master 来运行此示例，以方便在出现问题时更容易调试问题。

SPI AT 速率
--------------

测试说明
^^^^^^^^^

- 使用 ESP32 或者 ES32-C3 开发板作为 SPI master，运行 `ESP-AT <https://github.com/espressif/esp-at>`_ 中的 :example:`at_spi_master/spi/esp32_c_series` 目录的代码。其软硬件配置如下：

1. 硬件：CPU 工作频率设置为 240 MHz，flash SPI mode 配置为 QIO 模式，flash 频率设置为 40 MHz。

2. 软件：基于 ESP-IDF v4.3 的编译环境，并将示例代码中的 streambuffer 的大小调整为 8192 字节。

- 使用 {IDF_TARGET_NAME} 作为 SPI slave，编译并烧录 SPI AT 固件（参考 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`），并将 {IDF_TARGET_NAME} 配置工作在 TCP 透传模式。其软硬件配置如下：

1. 硬件：CPU 工作频率设置为 160 MHz。

2. 软件：SPI-AT 的实现代码中，将 streambuffer 的大小设置为 8192 字节，并使用 ESP-IDF 下的 example/wifi/iperf 中的 `sdkconfig.defaults.esp32c3 <https://github.com/espressif/esp-idf/blob/master/examples/wifi/iperf/sdkconfig.defaults.esp32c3>`_ 中的相关配置参数。

测试结果
^^^^^^^^^^^^

下表显示了我们在屏蔽箱中得到的通信速率结果：

.. list-table:: SPI AT Wi-Fi TCP 通信速率
   :header-rows: 1
   :widths: 15 20 25 25

   * - Clock
     - SPI mode
     - master->slave
     - slave->master
   * - 10 M
     - Standard
     - 0.95 MByte/s
     - 1.00 MByte/s
   * - 10 M
     - Dual
     - 1.37 MByte/s
     - 1.29 MByte/s
   * - 10 M
     - Quad
     - 1.43 MByte/s
     - 1.31 MByte/s
   * - 20 M
     - Standard
     - 1.41 MByte/s
     - 1.30 MByte/s
   * - 20 M
     - Dual
     - 1.39 MByte/s
     - 1.30 MByte/s
   * - 20 M
     - Quad
     - 1.39 MByte/s
     - 1.30 MByte/s
   * - 40 M
     - Standard
     - 1.37 MByte/s
     - 1.30 MByte/s
   * - 40 M
     - Dual
     - 1.40 MByte/s
     - 1.31 MByte/s
   * - 40 M
     - Quad
     - 1.48 MByte/s
     - 1.31 MByte/s

**说明** 1：当 SPI 的时钟频率较高时，受限于上层网络组件的限制，使用 Dual 或者 Quad 工作模式的通信速率想比较于 Standard 模式并未显著改善。

**说明** 2：更多关于 SPI 通信的介绍请参考对应模组的 `技术参考手册 <https://www.espressif.com/zh-hans/support/documents/technical-documents>`_。
