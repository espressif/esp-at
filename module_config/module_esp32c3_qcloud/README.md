# QCloud AT Customized partition table
QCloud AT command set use its own partition table :
1. The Size of App in `partitions_at.csv` is 0x180000 .
2. QCloud App needs several customized partitions in at_customize section , `qcloud_modinfo` , `qcloud_devinfo` , `qcloud_prdinfo` and `qcloud_errlog` , as can be found in at_customize.csv. Please make sure these partitions exist.
3. QCloud App also need a FLASH area to do firmware OTA for MCU side. The start address and size of the OTA area is module dependant and can be defined by `AT+TCMODINFOSET` command.

For QCloud IoT AT commands, please refer to [Customized AT Commands and Firmware](../../docs/en/Customized_AT_Commands_and_Firmware/index.rst)

# QCloud AT 自定义分区表
QCloud AT 命令集使用自定义的分区表：
1. `partitions_at.csv` 中定义的 App 大小为 0x180000 .
2. QCloud App 需要使用在 `at_customize.csv` 中定义的 QCloud 分区 `qcloud_modinfo`, `qcloud_devinfo`, `qcloud_prdinfo` 和 `qcloud_errlog` . 请确保上述分区存在.
3. QCloud App 需要一个 FLASH 区域来为 MCU 端执行 OTA . OTA 的起始地址和大小取决于所用模块，可以通过 `AT+TCMODINFOSET` 命令定义.

对于 QCloud IoT AT 命令集, 请参考 [第三方定制化 AT 命令和固件](../../docs/zh_CN/Customized_AT_Commands_and_Firmware/index.rst)