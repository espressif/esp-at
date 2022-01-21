# QCloud AT 自定义分区表

* [English Version](./README.md)

QCloud AT 命令集有专属的分区表：

1. 由于应用程序二进制大小超过默认值，所以将 partitions_at.csv 中的应用的应用分区大小修改为 0xD0000。
2. QCloud 应用程序需要几个自定义分区，`qcloud_modinfo` 分区、`qcloud_devinfo` 分区、`qcloud_prdinfo` 分区和 `qcloud_errlog` 分区，这些分区都可以在 at_customize.csv 中找到。请确保这些分区的存在。
3. QCloud 应用程序还需要一个 flash 区域用来为 MCU 端执行固件 OTA。该 OTA 区域的起始地址和大小取决于所使用的模组，可以通过命令 `AT+TCMODINFOSET` 定义。

# 注意事项

对于 **ESP8266 系列** QCloud AT 固件，从 `QCloud_AT_ESP8266_v2.2.0 (AT version:2.2.1.0)` 版本开始，由于 flash 大小限制的原因，`NET` 命令（即 [TCP-IP AT 命令集](https://espressif-docs.readthedocs-hosted.com/projects/esp-at/zh_CN/release-v2.2.0.0_esp8266/AT_Command_Set/TCP-IP_AT_Commands.html)）不再提供支持。如果您想使用该类命令，考虑使用 [ESP32-C3](https://docs.espressif.com/projects/esp-at/zh_CN/release-v2.3.0.0_esp32c3/AT_Binary_Lists/ESP32-C3_AT_binaries.html) 系列代替。
