# QCloud AT Customized partition table

QCloud AT command set use its own partition table :
1. The Size of App in `partitions_at.csv` is 0x1D0000.
2. The QCloud App needs to use the QCloud partition `qcloud_errlog` defined in `at_customize.csv`. Please make sure the partition exists.
3. QCloud App also need a FLASH area to do firmware OTA for MCU side. The start address and size of the OTA area is module dependant and can be defined by `AT+TCMODINFOSET` command.

For QCloud IoT AT commands, please refer to [Customized AT Commands and Firmware](../../docs/en/Customized_AT_Commands_and_Firmware/index.rst)

## BIN file generation tool

To facilitate factory production, BIN file generation tools in `bin_gen_tool` folder are provided to convert readable `csv` files into binary files. `qcloud_iot_bin_tool_linux` for Linux and `qcloud_iot_bin_tool_win.exe` for Windows platform ('-h' for the tool help message). Please also check each `sample.csv` file for the parameters:

1. `ModInfo` bin file is stored in the `qcloud_modinfo` partition which defined in at_customize.csv. And can be query by `AT+TCMODINFOSET?` command.
2. `DevInfo` bin file is stored in the `qcloud_devinfo` partition which defined in at_customize.csv. And can be query by `AT+TCDEVINFOSET?` command.
3. `PrdInfo` bin file is stored in the `qcloud_prdinfo` partition which defined in at_customize.csv. And can be query by `AT+TCPRDINFOSET?` command.

To prevent security risk, please note the tools are not for public usage and only for dedicated customers. If you want to use it, please contact [Espressif](https://www.espressif.com/en/contact-us/sales-questions).

# QCloud AT 自定义分区表

QCloud AT 命令集使用自定义的分区表：
1. `partitions_at.csv` 中定义的 App 大小为 0x180000。
2. QCloud App 需要使用在 `at_customize.csv` 中定义的 QCloud 分区 `qcloud_errlog`。 请确保该分区存在。
4. QCloud App 需要一个 FLASH 区域来为 MCU 端执行 OTA。 OTA 的起始地址和大小取决于所用模块，可以通过 `AT+TCMODINFOSET` 命令定义。

对于 QCloud IoT AT 命令集，请参考 [第三方定制化 AT 命令和固件](../../docs/zh_CN/Customized_AT_Commands_and_Firmware/index.rst)

## 二进制文件生成工具

为了方便工厂生产，提供了 `bin_gen_tool` 目录中的二进制文件生成工具，用于将可读的 `csv` 文件转换成二进制文件。`qcloud_iot_bin_tool_linux` 适用于 Linux 平台，`qcloud_iot_bin_tool_win.exe` 适用于 Windows 平台（'-h' 选项用于工具帮助消息）。请检查每个 `sample.csv` 文件的参数：

1. `ModInfo` 二进制文件存放在 at_customize.csv 中定义的 `qcloud_modinfo` 分区中。可以通过命令 `AT+TCMODINFOSET?` 查询。
2. `DevInfo` 二进制文件存放在 at_customize.csv 中定义的 `qcloud_devinfo` 分区中。可以通过命令 `AT+TCDEVINFOSET?` 查询。
3. `PrdInfo` 二进制文件存放在 at_customize.csv 中定义的 `qcloud_prdinfo` 分区中。可以通过命令 `AT+TCPRDINFOSET?` 查询。

为防止安全风险，请注意这些工具不提供给公众使用，仅供专门客户使用。 如需使用，请联系[乐鑫](https://www.espressif.com/zh-hans/contact-us/sales-questions)。