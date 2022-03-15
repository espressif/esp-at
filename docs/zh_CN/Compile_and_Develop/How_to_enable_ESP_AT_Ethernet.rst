如何启用 ESP-AT 以太网功能
===============================

:link_to_translation:`en:[English]`

概述
----------

本文档旨在指导用户启用 ESP-AT Ethernet，并通过简单的测试，展示如何确认是否启用成功。

第一步：配置并烧录
-----------------------

1. ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``AT ethernet support`` 启用以太网功能。
2. ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``Ethernet PHY`` 选择以太网 PHY。选择以太网 PHY 的说明请参考 `PHY 配置`_。
3. 重新编译 ``esp-at`` 工程 (参考 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`)，将生成的固件烧录到开发板中运行。

PHY 配置
^^^^^^^^^^

使用 ``./build.py menuconfig`` 配置以太网接口要使用的 PHY 模块，配置选项将根据你使用不同的硬件而有所不同。

Espressif 的官方硬件以太网开发板默认的配置是使用 IP101 PHY 模块。{IDF_TARGET_NAME} AT 最多支持四种以太网 PHY：``LAN8720``，``IP101``，``DP83848`` 和 ``RTL8201``。
由于 TI 停止生产，``TLK110`` PHY 已经不再支持。
如果你想使用其他 PHY，请参考 `{IDF_TARGET_NAME}-Ethernet-Kit V1.2 入门指南 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/hw-reference/{IDF_TARGET_PATH_NAME}/get-started-ethernet-kit.html>`__ 进行设计。

第二步：连接开发板并测试
------------------------------------------

将开发板通过网线连接到路由器，开发板将自动发送 DHCP 请求并尝试获取 IP 地址。如果设备获取 IP 地址成功，那么你可以使用连接到该路由器的 PC ping 通开发板。
