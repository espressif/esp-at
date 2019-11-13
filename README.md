# Overview
libat_core.a is AT Command Core, and it is the core of AT command, including the default AT instruction set, the AT command parsing, execution and responding. The lib contains 4 kinds of command, such as `AT+TEST=?`, `AT+TEST?`, `AT+TEST=“abc”` and `AT+TEST`. It supports custom AT commands based on the lib and related APIs, and ones can also define input and output medium, like uart, spi, socket, bt, etc.

The demo is the AT command set based on uart. You can replace the uart driver with other drivers whichever you want to use. But maybe you have to make some changes to run the new driver. In addition, you can add some custom AT commands in `at_custom_cmd` like `AT+CIUPDATE` if necessary.

- [ESP_AT_Get_Started.md](docs/ESP_AT_Get_Started.md): How to compile ESP AT  
- [ESP_AT_Commands_Set.md](docs/ESP_AT_Commands_Set.md): ESP AT Command Set  
- [How_To_Set_AT_Port_Pin.md](docs/How_To_Set_AT_Port_Pin.md): How to modify AT Port pin  
- [ESP_AT_Factory_Parameter_Bin.md](docs/ESP_AT_Factory_Parameter_Bin.md): How to add factory parameter 
- [ESP_AT_Customize_Partitions.md](docs/ESP_AT_Customize_Partitions.md): What is `The Secondary Partitions Table`    
- [How_To_Add_New_Platform.md](docs/How_To_Add_New_Platform.md): How to add new configuration for your module  
- [How_To_Update_IDF.md](docs/How_To_Update_IDF.md): How to update idf version  
- [ESP32_AT_Classic_Bluetooth.md](docs/ESP32_AT_Classic_Bluetooth.md): The example for ESP AT classic bluetooth command  
- [ESP32_AT_Ethernet.md](docs/ESP32_AT_Ethernet.md): How to use ethernet  
- [tools](tools/README.md): Some tools ESP AT used  
- virtual machine (for windows 64bits user): [`http://download.espressif.com/esp_share/env/ubuntu1804.ova`](http://download.espressif.com/esp_share/env/ubuntu1804.ova)
- https://blog.csdn.net/espressif/article/details/79316992: CSDN espressif official blog in Chinese

# 概述
libat_core.a 是 AT 命令集的核心部分, 包含默认的 AT 指令集, 具有 AT 命令解析、 执行和回复功能. 在此库中，可以处理 `AT+TEST=?`、 `AT+TEST?`、 `AT+TEST=“abc”` 和 `AT+TEST` 4 种格式的指令集. 可以根据相关 API 自定义命令, 并且还可以定义输入输出介质, 比如 uart, spi, socket, bt 等等.

此工程是基于 uart 实现的命令集，如果需要，你可以使用其它驱动程序替换掉 uart 驱动，为了使用新的驱动程序，可能必须做一些必要的代码改动. 另外，如果需要，你可以仿照 `AT+CIUPDATE` 在 `at_custom_cmd` 中添加自定义命令.

- [ESP_AT_Get_Started.md](docs/ESP_AT_Get_Started.md): 如何编译 ESP AT 工程  
- [ESP_AT_Commands_Set.md](docs/ESP_AT_Commands_Set.md): ESP AT 指令集  
- [How_To_Set_AT_Port_Pin.md](docs/How_To_Set_AT_Port_Pin.md): 如何修改发送 AT 命令的管脚  
- [ESP_AT_Factory_Parameter_Bin.md](docs/ESP_AT_Factory_Parameter_Bin.md): 如何添加出厂默认参数  
- [ESP_AT_Customize_Partitions.md](docs/ESP_AT_Customize_Partitions.md): 什么是第二分区表 `The Secondary Partitions Table`  
- [How_To_Add_New_Platform.md](docs/How_To_Add_New_Platform.md): 如何针对自己的模组添加新的平台支持  
- [How_To_Update_IDF.md](docs/How_To_Update_IDF.md): 如何更新 IDF 版本  
- [ESP32_AT_Classic_Bluetooth.md](docs/ESP32_AT_Classic_Bluetooth.md): ESP AT 经典蓝牙指令集示例  
- [ESP32_AT_Ethernet.md](docs/ESP32_AT_Ethernet.md): 如何使用 ethernet 接口  
- [tools](tools/README.md): 其它一些 ESP AT 相关工具  
- 虚拟机下载地址(供 64 位 windows 虚拟机客户使用): [`http://download.espressif.com/esp_share/env/ubuntu1804.ova`](http://download.espressif.com/esp_share/env/ubuntu1804.ova)
- https://blog.csdn.net/espressif/article/details/79316992: CSDN espressif 中文官方博客