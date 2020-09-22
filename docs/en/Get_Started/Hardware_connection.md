
Hardware Connection
=================

## ESP32 Series

ESP32 AT uses two UART port, UART0 is to download firmware and print background logs when running, UART1 is to send AT commands and receive AT responses. Different ESP32 modules use different GPIOs as UART1, so please pay attention to the hardware connection, and download the corresponding AT firmware to your specific ESP32 module. 

Follow the guide [Details of establishing serial connection](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/establish-serial-connection.html), to establish your serial connection.  

[More details of ESP32 modules](https://docs.espressif.com/projects/esp-idf/en/stable/hw-reference/modules-and-boards.html#wroom-solo-wrover-and-pico-modules).


### ESP32-WROOM-32 Series

Please refer to the data sheet, to get more details about ESP32-WROOM-32.

![avatar](../../_static/esp32-wroom-devkitc.png)  



### ESP32-WROVER-32 Series

Please refer to the data sheet, to get more details about ESP32-WROVER-32.

![avatar](../../_static/esp32-wrover-devkitc.png)  



### ESP32-PICO Series

Please refer to the data sheet, to get more details about ESP32-PICO.  

![avatar](../../_static/esp32-pico-d4.png) 


### ESP32-SOLO Series

Please refer to the data sheet, to get more details about ESP32-SOLO.  

![avatar](../../_static/esp32-solo-devkitc.png) 


## ESP32S2 Series

ESP32S2 AT uses two UART port, UART0 is to download firmware and print background logs when running, UART1 is to send AT commands and receive AT responses. 

Follow the guide [Details of establishing serial connection](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/get-started/establish-serial-connection.html), to establish your serial connection.  

[More details of ESP32S2 modules](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/hw-reference/index.html).

Please refer to the data sheet, to get more details about ESP32-S2-WROOM. 

![avatar](../../_static/esp32-s2-wroom-devkitc.png)

## ESP8266 Series

ESP8266 AT uses two UART port, UART0 is to download firmware and send AT commands and receive AT responses, UART1 is to print background logs. The official module of ESP8266 is ESP-WROOM-02D。

[More details about ESP8266 module](https://www.espressif.com/en/products/socs/esp8266).

Hardware connection of ESP8266 module.
![avatar](../../_static/esp8266_wroom02_devkitc.png) 

Please refer to the data sheet, to get more details about ESP8266 module.  
