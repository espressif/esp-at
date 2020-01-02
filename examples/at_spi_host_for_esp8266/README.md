This example contains two SPI master demos for ESP8266 SPI AT, where the difference between the two demo of ESP32 and ESP8266 is that when ESP8266 is SPI master, SPI transmission are placed in the interrupt function, increasing the transmission rate.
If ESP32 is SPI master, the interrupt function will notify SPI to read and write data through semaphore after receiving the data, increasing the context  switching time.

See [SPI demo user guide](https://github.com/espressif/ESP8266_RTOS_SDK/tree/master/examples/peripherals/spi#spi-demo-user-guide) for more SPI usage , ESP32 directory is similar to normal_performance in the SPI demo, and the ESP8266 directory is similar to high_performance in the SPI demo.



本示例包含两个 ESP8266 SPI  AT 的 HOST 侧 demo， 其中 ESP32 和 ESP8266 两个目录的区别在于 ESP8266 作为 SPI master 时将 SPI 的收发放在中断函数中进行，提高了传输的速率。而如果 ESP32 作为 SPI master ，收到数据后中断函数将会通过信号量通知 SPI 读写数据，增加了函数切换时间。

参考 [spi-demo 使用说明](https://github.com/espressif/ESP8266_RTOS_SDK/tree/master/examples/peripherals/spi#spi-demo-%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E) 可以获取 SPI 的更多使用细节， 其中 ESP32 目录类似于 SPI -demo 中的 normal_performance , ESP8266 目录类似于  SPI -demo 中的 high_performance。