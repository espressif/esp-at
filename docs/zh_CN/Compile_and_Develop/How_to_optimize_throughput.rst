如何提高 ESP-AT 吞吐性能
=========================

:link_to_translation:`en:[English]`

默认情况下，ESP-AT 和主机之间使用 UART 进行通信，因此最高吞吐速率不会超过其默认配置，即不会超过 115200 bps。用户如要 ESP-AT 实现较高的吞吐量，需了解本文，并做出有针对性的配置。本文以 {IDF_TARGET_NAME} 为例，介绍如何提高 ESP-AT 吞吐性能。

.. note::

    本文基于 ESP-AT 处于 :term:`透传模式` 下，描述提高 TCP/UDP/SSL 吞吐性能的一般性方法。

用户可以选择下面其中一种方法，提高吞吐性能：

- `[简单] 快速配置`_
- `[推荐] 熟悉数据流、针对性地配置`_

[简单] 快速配置
---------------

**1. 配置系统、LWIP、Wi-Fi 适用于高吞吐的参数**

  将下面代码段拷贝并追加至 :project_file:`module_config/module_{IDF_TARGET_PATH_NAME}_default/sdkconfig.defaults` 文件最后，其它 {IDF_TARGET_NAME} 系列设备请修改对应文件夹下的 sdkconfig.defaults 文件。

  .. only:: esp32

    ::

      # System
      CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE=4096
      CONFIG_FREERTOS_UNICORE=n
      CONFIG_FREERTOS_HZ=1000
      CONFIG_ESP_DEFAULT_CPU_FREQ_240=y
      CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ=240
      CONFIG_ESPTOOLPY_FLASHMODE_QIO=y
      CONFIG_ESPTOOLPY_FLASHFREQ_80M=y

      # LWIP
      CONFIG_LWIP_TCP_SND_BUF_DEFAULT=65534
      CONFIG_LWIP_TCP_WND_DEFAULT=65534
      CONFIG_LWIP_TCP_RECVMBOX_SIZE=12
      CONFIG_LWIP_UDP_RECVMBOX_SIZE=12
      CONFIG_LWIP_TCPIP_RECVMBOX_SIZE=64

      # Wi-Fi
      CONFIG_ESP32_WIFI_STATIC_RX_BUFFER_NUM=16
      CONFIG_ESP32_WIFI_DYNAMIC_RX_BUFFER_NUM=64
      CONFIG_ESP32_WIFI_DYNAMIC_TX_BUFFER_NUM=64
      CONFIG_ESP32_WIFI_TX_BA_WIN=32
      CONFIG_ESP32_WIFI_RX_BA_WIN=32
      CONFIG_ESP32_WIFI_AMPDU_TX_ENABLED=y
      CONFIG_ESP32_WIFI_AMPDU_RX_ENABLED=y

  .. only:: esp32c2

    ::

      # System
      CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE=4096
      CONFIG_FREERTOS_UNICORE=n
      CONFIG_FREERTOS_HZ=1000
      CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_120=y
      CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ=120
      CONFIG_ESPTOOLPY_FLASHMODE_QIO=y
      CONFIG_ESPTOOLPY_FLASHFREQ_60M=y

      # LWIP
      CONFIG_LWIP_TCP_SND_BUF_DEFAULT=65534
      CONFIG_LWIP_TCP_WND_DEFAULT=65534
      CONFIG_LWIP_TCP_RECVMBOX_SIZE=12
      CONFIG_LWIP_UDP_RECVMBOX_SIZE=12
      CONFIG_LWIP_TCPIP_RECVMBOX_SIZE=64

      # Wi-Fi
      CONFIG_ESP32_WIFI_STATIC_RX_BUFFER_NUM=16
      CONFIG_ESP32_WIFI_DYNAMIC_RX_BUFFER_NUM=64
      CONFIG_ESP32_WIFI_DYNAMIC_TX_BUFFER_NUM=64
      CONFIG_ESP32_WIFI_TX_BA_WIN=32
      CONFIG_ESP32_WIFI_RX_BA_WIN=32
      CONFIG_ESP32_WIFI_AMPDU_TX_ENABLED=y
      CONFIG_ESP32_WIFI_AMPDU_RX_ENABLED=y

  .. only:: esp32c3 or esp32s2

    ::

      # System
      CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE=4096
      CONFIG_FREERTOS_UNICORE=n
      CONFIG_FREERTOS_HZ=1000
      CONFIG_ESP_DEFAULT_CPU_FREQ_160=y
      CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ=160
      CONFIG_ESPTOOLPY_FLASHMODE_QIO=y
      CONFIG_ESPTOOLPY_FLASHFREQ_80M=y

      # LWIP
      CONFIG_LWIP_TCP_SND_BUF_DEFAULT=65534
      CONFIG_LWIP_TCP_WND_DEFAULT=65534
      CONFIG_LWIP_TCP_RECVMBOX_SIZE=12
      CONFIG_LWIP_UDP_RECVMBOX_SIZE=12
      CONFIG_LWIP_TCPIP_RECVMBOX_SIZE=64

      # Wi-Fi
      CONFIG_ESP32_WIFI_STATIC_RX_BUFFER_NUM=16
      CONFIG_ESP32_WIFI_DYNAMIC_RX_BUFFER_NUM=64
      CONFIG_ESP32_WIFI_DYNAMIC_TX_BUFFER_NUM=64
      CONFIG_ESP32_WIFI_TX_BA_WIN=32
      CONFIG_ESP32_WIFI_RX_BA_WIN=32
      CONFIG_ESP32_WIFI_AMPDU_TX_ENABLED=y
      CONFIG_ESP32_WIFI_AMPDU_RX_ENABLED=y

  .. only:: esp32c6

    ::

      # System
      CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE=4096
      CONFIG_FREERTOS_UNICORE=n
      CONFIG_FREERTOS_HZ=1000
      CONFIG_ESP_DEFAULT_CPU_FREQ_160=y
      CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ=160
      CONFIG_ESPTOOLPY_FLASHMODE_QIO=y
      CONFIG_ESPTOOLPY_FLASHFREQ_80M=y

      # LWIP
      CONFIG_LWIP_TCP_SND_BUF_DEFAULT=65534
      CONFIG_LWIP_TCP_WND_DEFAULT=65534
      CONFIG_LWIP_TCP_RECVMBOX_SIZE=12
      CONFIG_LWIP_UDP_RECVMBOX_SIZE=12
      CONFIG_LWIP_TCPIP_RECVMBOX_SIZE=64

      # Wi-Fi
      CONFIG_ESP_WIFI_STATIC_RX_BUFFER_NUM=16
      CONFIG_ESP_WIFI_DYNAMIC_RX_BUFFER_NUM=64
      CONFIG_ESP_WIFI_DYNAMIC_TX_BUFFER_NUM=64
      CONFIG_ESP_WIFI_TX_BA_WIN=32
      CONFIG_ESP_WIFI_RX_BA_WIN=32
      CONFIG_ESP_WIFI_AMPDU_TX_ENABLED=y
      CONFIG_ESP_WIFI_AMPDU_RX_ENABLED=y

**2. 提高 UART 缓冲区大小**

  将下面代码段拷贝并替换 :project_file:`at_uart_task.c <main/interface/uart/at_uart_task.c>` 文件中 `uart_driver_install()` 行。

  ::

    uart_driver_install(esp_at_uart_port, 1024 * 16, 1024 * 16, 100, &esp_at_uart_queue, 0);

**3. 删除默认配置、重新编译固件、烧录运行**

  ::

    rm -rf build sdkconfig
    ./build.py build
    ./build.py flash monitor

**4. 透传前提高 UART 波特率**

  典型 AT 命令序列如下：

  ::

    AT+CWMODE=1
    AT+CWJAP="ssid","password"
    AT+UART_CUR=3000000,8,1,0,3
    AT+CIPSTART="TCP","192.168.105.13",3344
    AT+CIPMODE=1
    AT+CIPSEND
    // 传输数据

此快速配置的方法在一定程度上可以提高吞吐，但有时可能不能达到用户的预期。另外有些配置可能不是吞吐的瓶颈，配置较高可能会牺牲内存资源或功耗等。因此，用户也可以熟悉下面推荐的方法，进行有针对性地配置。

[推荐] 熟悉数据流、针对性地配置
------------------------------------------------------

影响 ESP-AT 吞吐的因素大体描述如下图：

.. figure:: ../../_static/optimize-throughput.png
   :align: center
   :alt: 吞吐数据流
   :figclass: align-center

   吞吐数据流

如图中箭头所示：

- ESP-AT 发送 (TX) 的数据流为 S1 -> S2 -> S3 -> S4 -> S5 -> S6 -> S7 -> S8
- ESP-AT 接收 (RX) 的数据流为 R8 -> R7 -> R6 -> R5 -> R4 -> R3 -> R2 -> R1

吞吐的数据流类似于水流，要想提高吞吐，需要考虑在数据流速较低的节点之间进行优化，而不需要在数据流速本就达到预期的节点之间进行额外的配置，以免造成不必要的资源浪费。在实际产品中，往往只需要提高其中一条数据流吞吐即可，用户需要根据下面指导进行对应的配置即可。

.. note::

  下面的配置均以可用内存充足为前提的，用户可以通过 :ref:`AT+SYSRAM <cmd-SYSRAM>` 命令来查询可用内存。

**1. G0 吞吐优化**

  G0 是系统可以优化的部分，建议参考配置如下：

  .. only:: esp32

    ::
      
      CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE=4096
      CONFIG_FREERTOS_UNICORE=n
      CONFIG_FREERTOS_HZ=1000
      CONFIG_ESP_DEFAULT_CPU_FREQ_240=y
      CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ=240
      CONFIG_ESPTOOLPY_FLASHMODE_QIO=y
      CONFIG_ESPTOOLPY_FLASHFREQ_80M=y

  .. only:: esp32c2

    ::
      
      CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE=4096
      CONFIG_FREERTOS_UNICORE=n
      CONFIG_FREERTOS_HZ=1000
      CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_120=y
      CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ=120
      CONFIG_ESPTOOLPY_FLASHMODE_QIO=y
      CONFIG_ESPTOOLPY_FLASHFREQ_60M=y

  .. only:: esp32c3 or esp32c6

    ::
      
      CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE=4096
      CONFIG_FREERTOS_UNICORE=n
      CONFIG_FREERTOS_HZ=1000
      CONFIG_ESP_DEFAULT_CPU_FREQ_160=y
      CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ=160
      CONFIG_ESPTOOLPY_FLASHMODE_QIO=y
      CONFIG_ESPTOOLPY_FLASHFREQ_80M=y

**2. S1、R1 吞吐优化**

  通常情况下，S1 和 R1 是 ESP-AT 吞吐高低的关键。因为默认情况下，ESP-AT 和主机之间使用 UART 进行通信，波特率为 115200，而 UART 硬件上，速率上限为 5 Mbps。因此，用户使用场景吞吐低于 5 Mbps，可以使用默认的 UART 作为和主机之间的通信介质，同时可以进行下面优化。

2.1 提高 UART 缓冲区大小

  将下面代码段拷贝并替换 :project_file:`at_uart_task.c <main/interface/uart/at_uart_task.c>` 文件中 `uart_driver_install()` 行。

  - 提高 UART TX 吞吐

    ::

      uart_driver_install(esp_at_uart_port, 1024 * 16, 8192, 100, &esp_at_uart_queue, 0);

  - 提高 UART RX 吞吐

    ::

      uart_driver_install(esp_at_uart_port, 2048, 1024 * 16, 100, &esp_at_uart_queue, 0);

  - 提高 UART TX 和 RX 吞吐

    ::

      uart_driver_install(esp_at_uart_port, 1024 * 16, 1024 * 16, 100, &esp_at_uart_queue, 0);

2.2 透传前提高 UART 波特率

  典型 AT 命令序列如下：

  ::

    AT+CWMODE=1
    AT+CWJAP="ssid","password"
    AT+UART_CUR=3000000,8,1,0,3
    AT+CIPSTART="TCP","192.168.105.13",3344
    AT+CIPMODE=1
    AT+CIPSEND
    // 传输数据

.. note::
  用户需要确保主机的 UART 可以支持到这么高的速率，并且主机和 ESP-AT 之间的 UART 连线尽可能地短。

.. note::

    如果用户期望吞吐速率大于或接近于 5 Mbps，可以考虑使用 SPI、SDIO、Socket 等方式。具体请参考：

    .. only:: esp32 or esp32c6

      - SDIO： :doc:`SDIO AT 指南 </Compile_and_Develop/How_to_implement_SDIO_AT>`
      - Socket： :project_file:`Socket AT 指南 <main/interface/socket/README.md>`

    .. only:: esp32c2 or esp32c3 or esp32c6

      - SPI： :doc:`SPI AT 指南 </Compile_and_Develop/How_to_implement_SPI_AT>`
      - Socket： :project_file:`Socket AT 指南 <main/interface/socket/README.md>`

**3. S2、R2、R3、S3 吞吐优化**

  通常情况下，S2、R2、R3、S3 不是 ESP-AT 吞吐高低的瓶颈。因为 AT core 在 UART 缓冲区和通信协议的传输层之间传递数据，仅有极少的且不耗时的应用逻辑，无需优化。

**4. S4、R4、S5、R5、S6、R6 吞吐优化**

  ESP-AT 和主机之间使用 UART 进行通信，S4、R4、S5、R5、S6、R6 无需优化。ESP-AT 和主机之间使用其他传输介质进行通信时，S4、R4、S5、R5、S6、R6 是影响吞吐的一个因素。

  S4、R4、S5、R5、S6、R6 是通信协议的传输层、网络层、和数据链路层之间的数据流。用户需要阅读 ESP-IDF 中 `如何提高 Wi-Fi 性能 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#how-to-improve-wi-fi-performance>`_ 文档，了解原理，进行合理配置。这些配置均可以在 ``./build.py menuconfig`` 里进行配置。

  - 优化 S4 -> S5 -> S6：`发送数据方向配置 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#id64>`_
  - 优化 R6 -> R5 -> R4：`接收数据方向配置 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#id64>`_

**5. S6、R6 吞吐优化**

  S6 和 R6 是通信协议的数据链路层，{IDF_TARGET_NAME} 可以使用 Wi-Fi 或者以太网作为传输介质。Wi-Fi 除了上述介绍的优化方法之外，可能还需要用户关心：

  - 提高 RF 发射功率

    默认发射功率通常不是吞吐高低的瓶颈，用户也可以通过 :ref:`AT+RFPOWER <cmd-RFPOWER>` 命令查询和设置 RF 发射功率。

  - 设置 802.11 b/g/n 协议

    默认 Wi-Fi 模式即为 802.11 b/g/n 协议，用户可通过 :ref:`AT+CWSTAPROTO <cmd-STAPROTO>` 命令查询和设置 802.11 b/g/n 协议。配置是双向的，因此建议 AP 端 Wi-Fi 模式配置为 802.11 b/g/n 协议，频宽配置为 HT20/HT40 (20/40 MHz) 模式。

**6. S7、R7、S8、R8 吞吐优化**

  通常情况下，S7、R7、S8、R8 不是 ESP-AT 吞吐优化的范围。因为这和实际网络带宽、网络路由、物理距离等有关。
