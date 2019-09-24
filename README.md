# Overview
libat_core.a is AT Command Core, and it is the core of AT command, including the default AT instruction set, the AT command parsing, execution and responding. The lib contains 4 kinds of command, such as AT+TEST=?, AT+TEST?, AT+TEST=“abc” and AT+TEST. It supports custom AT commands based on the lib and related APIs, and ones can also define input and output medium, like uart, spi, socket, bt, etc.

The demo is the AT command set based on uart. You can replace the uart driver with other drivers whichever you want to use. But you have to make some changes in at_task.c. In addition, you can add some custom AT commands in at_custom_cmd like AT+CIUPDATE if necessary.

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