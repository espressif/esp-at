硬件连接
========

:link_to_translation:`en:[English]`

本文档主要介绍下载和烧录 AT 固件、发送 AT 命令和接收 AT 响应所需要的硬件以及硬件之间该如何连接，主要涉及以下 ESP 系列的模组：

- `ESP32 系列`_
- `ESP32-S2 系列`_
- `ESP32-C3 系列`_
- `ESP8266 系列`_

对于不同系列的模组，AT 默认固件所支持的命令会有所差异。具体可参考 :doc:`/Compile_and_Develop/How_to_understand_the_differences_of_each_type_of_module`。

硬件准备
------------

.. list-table:: ESP-AT 测试所需硬件
   :header-rows: 1

   * - 硬件
     - 功能
   * - ESP 开发板
     - 从机
   * - USB 数据线（连接 ESP 开发板和 PC）
     - 下载固件、输出日志数据连接
   * - PC
     - 主机，将固件下载至从机
   * - USB 数据线（连接 PC 和 USB 转 UART 串口模块）
     - 发送 AT 命令、接收 AT 响应数据连接
   * - USB 转 UART 串口模块
     - 转换 USB 信号和 TTL 信号
   * - 杜邦线（连接 USB 转 UART 串口模块和 ESP 开发板）
     - 发送 AT 命令、接收 AT 响应数据连接

.. figure:: ../../_static/hw-connection-what-you-need.png
   :align: center
   :alt: ESP-AT 测试硬件连接示意图
   :figclass: align-center

   ESP-AT 测试硬件连接示意图

注意：上图使用 4 根杜邦线连接 ESP 开发板和 USB 转 UART 串口模块，但如果您不使用硬件流控功能，只需 2 根杜邦线连接 TX/RX 即可。

ESP32 系列
-------------

ESP32 AT 采用两个 UART 接口：UART0 用于下载固件和输出日志，UART1 用于发送 AT 命令和接收 AT 响应。

所有 ESP32 模组均连接 GPIO1 和 GPIO3 作为 UART0，但连接不同的 GPIO 作为 UART1，下文将详细介绍如何连接 ESP32 系列模组。

更多有关 ESP32 模组和开发板的信息可参考 `ESP32 系列模组和开发板 <https://docs.espressif.com/projects/esp-idf/zh_CN/stable/hw-reference/modules-and-boards.html>`_。

ESP32-WROOM-32 系列
^^^^^^^^^^^^^^^^^^^^^^

.. list-table:: ESP32-WROOM-32 系列硬件连接管脚分配
   :header-rows: 1

   * - 功能
     - ESP 开发板管脚
     - 其它设备管脚
   * - 下载固件/输出日志 :sup:`1`
     - UART0
         * GPIO3 (RX)
         * GPIO1 (TX)
     - PC
         * TX
         * RX
   * - AT 命令/响应 :sup:`2`
     - UART1
         * GPIO16 (RX)
         * GPIO17 (TX)
         * GPIO15 (CTS)
         * GPIO14 (RTS)
     - USB 转 UART 串口模块
         * TX
         * RX
         * RTS
         * CTS

**说明** 1：ESP 开发板和 PC 之间的管脚连接已内置在 ESP 开发板上，您只需使用 USB 数据线连接开发板和 PC 即可。

**说明** 2：CTS/RTS 管脚只有在使用硬件流控功能时才需连接。

.. figure:: ../../_static/esp32-wroom-hw-connection.png
   :align: center
   :alt: ESP32-WROOM-32 系列硬件连接示意图
   :figclass: align-center

   ESP32-WROOM-32 系列硬件连接示意图

如果需要直接基于 ESP32-WROOM-32 模组进行连接，请参考 `《ESP32-WROOM-32 技术规格书》 <https://www.espressif.com/sites/default/files/documentation/esp32_wrover_datasheet_cn.pdf>`_。

ESP32-WROVER 系列
^^^^^^^^^^^^^^^^^^^^^^^^
.. list-table:: ESP32-WROVER 系列硬件连接管脚分配
   :header-rows: 1

   * - 功能
     - ESP 开发板管脚
     - 其它设备管脚
   * - 下载固件/输出日志 :sup:`1`
     - UART0
         * GPIO3 (RX)
         * GPIO1 (TX)
     - PC
         * TX
         * RX
   * - AT 命令/响应 :sup:`2`
     - UART1
         * GPIO19 (RX)
         * GPIO22 (TX)
         * GPIO15 (CTS)
         * GPIO14 (RTS)
     - USB 转 UART 串口模块
         * TX
         * RX
         * RTS
         * CTS

**说明** 1：ESP 开发板和 PC 之间的管脚连接已内置在 ESP 开发板上，您只需使用 USB 数据线连接开发板和 PC 即可。

**说明** 2：CTS/RTS 管脚只有在使用硬件流控功能时才需连接。

.. figure:: ../../_static/esp32-wrover-hw-connection.png
   :align: center
   :alt: ESP32-WROVER 系列硬件连接示意图
   :figclass: align-center

   ESP32-WROVER 系列硬件连接示意图

如果需要直接基于 ESP32-WROVER 模组进行连接，请参考 `《ESP32-WROVER 技术规格书》 <https://www.espressif.com/sites/default/files/documentation/esp32_wrover_datasheet_cn.pdf>`_。

ESP32-PICO 系列
^^^^^^^^^^^^^^^^^^

.. list-table:: ESP32-PICO 系列硬件连接管脚分配
   :header-rows: 1

   * - 功能
     - ESP 开发板管脚
     - 其它设备管脚
   * - 下载固件/输出日志 :sup:`1`
     - UART0
         * GPIO3 (RX)
         * GPIO1 (TX)
     - PC
         * TX
         * RX
   * - AT 命令/响应 :sup:`2`
     - UART1
         * GPIO19 (RX)
         * GPIO22 (TX)
         * GPIO15 (CTS)
         * GPIO14 (RTS)
     - USB 转 UART 串口模块
         * TX
         * RX
         * RTS
         * CTS

**说明** 1：ESP 开发板和 PC 之间的管脚连接已内置在 ESP 开发板上，您只需使用 USB 数据线连接开发板和 PC 即可。

**说明** 2：CTS/RTS 管脚只有在使用硬件流控功能时才需连接。

.. figure:: ../../_static/esp32-pico-hw-connection.png
   :align: center
   :alt: ESP32-PICO 系列硬件连接示意图
   :figclass: align-center

   ESP32-PICO 系列硬件连接示意图

如果需要直接基于 ESP32-PICO-D4 进行连接，请参考 `《ESP32-PICO-D4 技术规格书》 <https://www.espressif.com/sites/default/files/documentation/esp32-pico-d4_datasheet_cn.pdf>`_。

ESP32-SOLO 系列
^^^^^^^^^^^^^^^^^^

.. list-table:: ESP32-SOLO 系列硬件连接管脚分配
   :header-rows: 1

   * - 功能
     - ESP 开发板管脚
     - 其它设备管脚
   * - 下载固件/输出日志 :sup:`1`
     - UART0
         * GPIO3 (RX)
         * GPIO1 (TX)
     - PC
         * TX
         * RX
   * - AT 命令/响应 :sup:`2`
     - UART1
         * GPIO16 (RX)
         * GPIO17 (TX)
         * GPIO15 (CTS)
         * GPIO14 (RTS)
     - USB 转 UART 串口模块
         * TX
         * RX
         * RTS
         * CTS

**说明** 1：ESP 开发板和 PC 之间的管脚连接已内置在 ESP 开发板上，您只需使用 USB 数据线连接开发板和 PC 即可。

**说明** 2：CTS/RTS 管脚只有在使用硬件流控功能时才需连接。

.. figure:: ../../_static/esp32-solo-hw-connection.png
   :align: center
   :alt: ESP32-SOLO 系列硬件连接示意图
   :figclass: align-center

   ESP32-SOLO 系列硬件连接示意图

如果需要直接基于 ESP32-SOLO-1 进行连接，请参考 `《ESP32-SOLO-1 技术规格书》 <https://www.espressif.com/sites/default/files/documentation/esp32-solo-1_datasheet_cn.pdf>`_。

ESP32-S2 系列
----------------

ESP32-S2 AT 采用两个 UART 接口：UART0 用于下载固件和输出日志，UART1 用于发送 AT 命令和接收 AT 响应。

.. list-table:: ESP32-S2 Series 系列硬件连接管脚分配
   :header-rows: 1

   * - 功能
     - ESP 开发板管脚
     - 其它设备管脚
   * - 下载固件/输出日志 :sup:`1`
     - UART0
         * GPIO44 (RX)
         * GPIO43 (TX)
     - PC
         * TX
         * RX
   * - AT 命令/响应 :sup:`2`
     - UART1
         * GPIO21 (RX)
         * GPIO17 (TX)
         * GPIO20 (CTS)
         * GPIO19 (RTS)
     - USB 转 UART 串口模块
         * TX
         * RX
         * RTS
         * CTS

**说明** 1：ESP 开发板和 PC 之间的管脚连接已内置在 ESP 开发板上，您只需使用 USB 数据线连接开发板和 PC 即可。

**说明** 2：CTS/RTS 管脚只有在使用硬件流控功能时才需连接。

.. figure:: ../../_static/esp32-s2-hw-connection.png
   :align: center
   :alt: ESP32-S2 系列硬件连接示意图
   :figclass: align-center

   ESP32-S2 系列硬件连接示意图

如果需要直接基于 ESP32-S2-WROOM 模组进行连接，请参考 `《ESP32-S2-WROOM & ESP32-S2-WROOM-I 技术规格书》 <https://www.espressif.com/sites/default/files/documentation/esp32-s2-wroom_esp32-s2-wroom-i_datasheet_cn.pdf>`_。

ESP32-C3 系列
----------------

ESP32-C3 AT 采用两个 UART 接口：UART0 用于下载固件和输出日志，UART1 用于发送 AT 命令和接收 AT 响应。

.. list-table:: ESP32-C3 Series 系列硬件连接管脚分配
   :header-rows: 1

   * - 功能
     - ESP 开发板管脚
     - 其它设备管脚
   * - 下载固件/输出日志 :sup:`1`
     - UART0
         * GPIO20 (RX)
         * GPIO21 (TX)
     - PC
         * TX
         * RX
   * - AT 命令/响应 :sup:`2`
     - UART1
         * GPIO6 (RX)
         * GPIO7 (TX)
         * GPIO5 (CTS)
         * GPIO4 (RTS)
     - USB 转 UART 串口模块
         * TX
         * RX
         * RTS
         * CTS

**说明** 1：ESP 开发板和 PC 之间的管脚连接已内置在 ESP 开发板上，您只需使用 USB 数据线连接开发板和 PC 即可。

**说明** 2：CTS/RTS 管脚只有在使用硬件流控功能时才需连接。

.. figure:: ../../_static/esp32-c3-hw-connection.png
   :align: center
   :alt: ESP32-C3 系列硬件连接示意图
   :figclass: align-center

   ESP32-C3 系列硬件连接示意图

如果需要直接基于 ESP32­-C3-­MINI-­1 模组进行连接，请参考 `《ESP32­-C3-­MINI-­1 技术规格书》 <https://www.espressif.com/sites/default/files/documentation/esp32-c3-mini-1_datasheet_cn.pdf>`_。

ESP8266 系列
---------------

ESP8266 AT 采用两个 UART 接口：UART0 用于下载固件、发送 AT 命令以及接收 AT 响应；UART1 用于输出日志。

.. list-table:: ESP8266 系列硬件连接管脚分配
   :header-rows: 1

   * - 功能
     - ESP 开发板管脚
     - 其它设备管脚
   * - 下载固件
     - UART0
         * GPIO3 (RX)
         * GPIO1 (TX)
     - PC
         * TX
         * RX
   * - AT 命令/响应 :sup:`2`
     - UART0
         * GPIO13 (RX)
         * GPIO15 (TX)
         * GPIO3 (CTS)
         * GPIO1 (RTS)
     - USB 转 UART 串口模块
         * TX
         * RX
         * RTS
         * CTS 
   * - 输出日志
     - UART1
         * GPIO2 (TX)
     - USB 转 UART 串口模块
         * RX

**说明** 1：ESP 开发板和 PC 之间的管脚连接已内置在 ESP 开发板上，您只需使用 USB 数据线连接开发板和 PC 即可。

**说明** 2：CTS/RTS 管脚只有在使用硬件流控功能时才需连接。

.. figure:: ../../_static/esp8266-hw-connection.png
    :align: center
    :alt: ESP8266 系列硬件连接示意图
    :figclass: align-center

    ESP8266 系列硬件连接示意图

.. 注意::

    默认配置下，基于 ESP8266_RTOS_SDK 的 ESP-WROOM-02 AT 固件具有 swap 功能，会将 RX/TX 与 CTS/RTS 进行交换。若想使用硬件流控功能，您需要断开 UART1, 从 ESP 开发板拆焊 CP2102N 芯片，并将开发板与 USB 转 UART 串口模块的 3.3 V 和 GND 相连进行供电。

如果需要直接基于 ESP-WROOM-02 或 ESP-WROOM-02D/02U 模组进行连接, 请参考 `《ESP-WROOM-02 技术规格书》 <https://www.espressif.com/sites/default/files/documentation/0c-esp-wroom-02_datasheet_cn.pdf>`_ 或 `《ESP-WROOM-02D/02U 技术规格书》 <https://www.espressif.com/sites/default/files/documentation/esp-wroom-02d_esp-wroom-02u_datasheet_cn.pdf>`_。

更多有关 ESP8266 模组的信息可参考 `ESP8266 文档 <https://www.espressif.com/zh-hans/products/socs/esp8266>`_。
