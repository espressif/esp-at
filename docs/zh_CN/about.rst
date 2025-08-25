关于 ESP-AT
=============

:link_to_translation:`en:[English]`

这是 `ESP-AT <https://github.com/espressif/esp-at>`_ 的文档，ESP-AT 是乐鑫开发的可与乐鑫产品快速简单交互的解决方案。

乐鑫 Wi-Fi 和蓝牙芯片可以用作附加模块，可以完美集成在其他新产品或现有产品上，提供无线通讯功能。为降低客户开发成本，乐鑫开发了 :doc:`AT 固件 <AT_Binary_Lists/index>` 和各类 :doc:`AT 命令 <AT_Command_Set/index>`，方便客户简单快速地使用 AT 命令来控制芯片。

.. figure:: ../_static/about-esp-at-solution.png
   :align: center
   :alt: ESP-AT 方案
   :figclass: align-center

   ESP-AT 方案

乐鑫提供的 AT 固件具有以下特色，利于芯片快速集成到应用中：

- 内置 TCP/IP 堆栈和数据缓冲
- 能便捷地集成到资源受限的主机平台中
- 主机对命令的回应易于解析
- 用户可自定义 AT 命令

.. figure:: ../_static/about-esp-at-commands.png
   :align: center
   :alt: ESP-AT 命令
   :figclass: align-center

   ESP-AT 命令

其他 Espressif 连接方案
---------------------------

除了 ESP-AT 之外，乐鑫还提供了其他连接方案，以满足不同的集成需求：

- **ESP-Hosted 方案** 可使乐鑫系列芯片作为外部主机系统的无线通信协处理器。允许主机设备（如基于 Linux 的系统或微控制器 MCU）通过标准接口（如 SPI、SDIO 或 UART）添加 Wi-Fi 和 Bluetooth/BLE 功能。详情请见 `ESP-Hosted GitHub <https://github.com/espressif/esp-hosted>`_。
- **ESP-IoT-Bridge 方案** 主要针对 IoT 应用场景下的各种网络接口之间的连接与通信，如 SPI、SDIO、USB、Wi-Fi、以太网等网络接口。在该解决方案中，Bridge 设备既可以为其它设备提供上网功能，又可以作为连接远程服务器的独立设备。详情请见 `ESP-IoT-Bridge GitHub <https://github.com/espressif/esp-iot-bridge>`_。

上述方案面向不同的系统架构和应用需求。对于希望在集成无线连接时寻找 AT 固件以外替代方案的用户，可以根据主机平台能力和系统设计目标选择更合适的解决方案。
