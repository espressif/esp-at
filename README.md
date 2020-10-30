# Overview
libat_core.a is AT Command Core, and it is the core of AT command, including the default AT instruction set, the AT command parsing, execution and responding. The lib contains 4 kinds of command, such as `AT+TEST=?`, `AT+TEST?`, `AT+TEST=“abc”` and `AT+TEST`. It supports custom AT commands based on the lib and related APIs, and ones can also define input and output medium, like uart, spi, socket, bt, etc.

The demo is the AT command set based on uart. You can replace the uart driver with other drivers whichever you want to use. But maybe you have to make some changes to run the new driver. In addition, you can add some custom AT commands in `at_custom_cmd` like `AT+CIUPDATE` if necessary.

- Documentation for the latest version: https://docs.espressif.com/projects/esp-at/. This documentation is built from the [docs directory](docs) of this repository
- [How_to_clone_project_and_compile_it.rst](docs/en/Compile_and_Develop/How_to_clone_project_and_compile_it.rst): How to compile ESP AT  
- [ESP_AT_Commands_Set.md](https://docs.espressif.com/projects/esp-at/en/latest/index.html): ESP AT Command Set  
- [How_to_set_AT_port_pin.md](docs/en/Compile_and_Develop/How_to_set_AT_port_pin.md): How to modify AT Port pin  
- [How_to_create_factory_parameter_bin.md](docs/en/Compile_and_Develop/How_to_create_factory_parameter_bin.md): How to add factory parameter 
- [How_to_customize_partitions.md](docs/en/Compile_and_Develop/How_to_customize_partitions.md): What is `The Secondary Partitions Table`    
- [How_to_add_new_platform.md](docs/en/Compile_and_Develop/How_to_add_new_platform.md): How to add new configuration for your module  
- [How_to_update_IDF.md](docs/en/Compile_and_Develop/How_to_update_IDF.md): How to update idf version  
- [How_to_enable_ESP32_AT_Classic_Bluetooth.md](docs/en/Compile_and_Develop/How_to_enable_ESP32_AT_Classic_Bluetooth.md): The example for ESP AT classic bluetooth command  
- [How_to_customize_BLE_services.md](docs/en/Compile_and_Develop/How_to_customize_BLE_services.md): How to customize ble services  
- [How_to_enable_ESP32_AT_Ethernet.md](docs/en/Compile_and_Develop/How_to_enable_ESP32_AT_Ethernet.md): How to use ethernet  
- [How_to_implement_ESP32_SDIO_AT.md](docs/en/Compile_and_Develop/How_to_implement_ESP32_SDIO_AT.md): SDIO AT user guide
- [tools](tools/README.md): Some tools ESP AT used  
- virtual machine (for windows 64bits user): [`http://download.espressif.com/esp_share/env/ubuntu1804.ova`](http://download.espressif.com/esp_share/env/ubuntu1804.ova)
- https://blog.csdn.net/espressif/article/details/79316992: CSDN espressif official blog in Chinese

# 概述
libat_core.a 是 AT 命令集的核心部分, 包含默认的 AT 指令集, 具有 AT 命令解析、 执行和回复功能. 在此库中，可以处理 `AT+TEST=?`、 `AT+TEST?`、 `AT+TEST=“abc”` 和 `AT+TEST` 4 种格式的指令集. 可以根据相关 API 自定义命令, 并且还可以定义输入输出介质, 比如 uart, spi, socket, bt 等等.

此工程是基于 uart 实现的命令集，如果需要，你可以使用其它驱动程序替换掉 uart 驱动，为了使用新的驱动程序，可能必须做一些必要的代码改动. 另外，如果需要，你可以仿照 `AT+CIUPDATE` 在 `at_custom_cmd` 中添加自定义命令.

- 最新版的文档：https://docs.espressif.com/projects/esp-at/ ，该文档是由本仓库 [docs 目录](docs) 构建得到
- [How_to_clone_project_and_compile_it.rst](docs/zh_CN/Compile_and_Develop/How_to_clone_project_and_compile_it.rst): 如何编译 ESP AT 工程  
- [ESP_AT_Commands_Set.md](https://docs.espressif.com/projects/esp-at/zh_CN/latest/): ESP AT 指令集  
- [How_to_set_AT_port_pin.md](docs/zh_CN/Compile_and_Develop/How_to_set_AT_port_pin.md): 如何修改 AT 发送命令的管脚  
- [How_to_create_factory_parameter_bin.md](docs/zh_CN/Compile_and_Develop/How_to_create_factory_parameter_bin.md): 如何创建默认出厂参数  
- [How_to_customize_partitions.md](docs/zh_CN/Compile_and_Develop/How_to_customize_partitions.md): 如何客制化分区  
- [How_to_add_new_platform.md](docs/zh_CN/Compile_and_Develop/How_to_add_new_platform.md): 如何针对自己的模组添加新的平台支持  
- [How_to_update_IDF.md](docs/zh_CN/Compile_and_Develop/How_to_update_IDF.md): 如何更新 IDF 版本  
- [How_to_enable_ESP32_AT_Classic_Bluetooth.md](docs/zh_CN/Compile_and_Develop/How_to_enable_ESP32_AT_Classic_Bluetooth.md): 如何使能 ESP32 AT 经典蓝牙  
- [How_to_customize_BLE_services.md](docs/zh_CN/Compile_and_Develop/How_to_customize_BLE_services.md): 如何自定义 ble services  
- [How_to_enable_ESP32_AT_Ethernet.md](docs/zh_CN/Compile_and_Develop/How_to_enable_ESP32_AT_Ethernet.md): 如何使能 ESP32 AT 网口  
- [How_to_implement_ESP32_SDIO_AT.md](docs/zh_CN/Compile_and_Develop/How_to_implement_ESP32_SDIO_AT.md): SDIO AT 用法指南
- [tools](tools/README.md): 其它一些 ESP AT 相关工具  
- 虚拟机下载地址(供 64 位 windows 虚拟机客户使用): [`http://download.espressif.com/esp_share/env/ubuntu1804.ova`](http://download.espressif.com/esp_share/env/ubuntu1804.ova)
- https://blog.csdn.net/espressif/article/details/79316992: CSDN espressif 中文官方博客