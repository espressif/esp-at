# QCloud AT Customized partition table

* [中文版](./README_CN.md)

QCloud AT command set uses its own partition table:

1. As the `app` size is larger than the default size, it is changed to 0xD0000 in partitions_at.csv.
2. The QCloud app needs several customized partitions in at_customize section. They are `qcloud_modinfo`, `qcloud_devinfo`, `qcloud_prdinfo`, and `qcloud_errlog`, as can be found in at_customize.csv. Please make sure these partitions exist.
3. The QCloud app also needs a flash area to do firmware OTA for MCU side. The start address and size of the OTA area is module dependant and can be defined by `AT+TCMODINFOSET` command.

# Note

For **ESP8266 series** QCloud AT firmware, starting from `QCloud_AT_ESP8266_v2.2.0 (AT version:2.2.1.0)`, `NET` commands (namely [TCP/IP AT Commands](https://docs.espressif.com/projects/esp-at/en/release-v2.2.0.0_esp8266/AT_Command_Set/TCP-IP_AT_Commands.html)) are no longer supported due to flash size limitation. If you want to use these commands, consider switching to the [ESP32-C3](https://github.com/espressif/esp-at/tree/release/v2.3.0.0_esp32c3) series instead.
