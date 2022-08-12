下载指导
==========

{IDF_TARGET_MODULE_NAME: default="undefined", esp32="ESP32-WROOM-32", esp32c2="ESP8684-MINI-1", esp32c3="ESP32-C3-MINI-1"}
{IDF_TARGET_FACTORY_BIN: default="undefined", esp32="ESP32-WROOM-32_AT_Bin_V2.2.0.0", esp32c2="ESP32C2-4MB_AT_Bin_V2.5.0.0", esp32c3="ESP32-C3-MINI-1_AT_Bin_V2.3.0.0"}

:link_to_translation:`en:[English]`

本文档以 {IDF_TARGET_MODULE_NAME} 模组为例，介绍如何下载 {IDF_TARGET_MODULE_NAME} 模组对应的 AT 固件，以及如何将固件烧录到模组上，其它 {IDF_TARGET_NAME} 系列模组也可参考本文档。

下载和烧录 AT 固件之前，请确保您已正确连接所需硬件，具体可参考 :doc:`Hardware_connection`。

对于不同系列的模组，AT 默认固件所支持的命令会有所差异。具体可参考 :doc:`/Compile_and_Develop/esp-at_firmware_differences`。

.. _download-at-firmware:

下载 AT 固件
-------------

请按照以下步骤将 AT 固件下载至 PC：

- 前往 :doc:`../AT_Binary_Lists/index`
- 找到您的模组所对应的 AT 固件
- 点击相应链接进行下载

此处，我们下载了 {IDF_TARGET_MODULE_NAME} 对应的 ``{IDF_TARGET_FACTORY_BIN}`` 固件，该固件的目录结构及其中各个 bin 文件介绍如下，其它 {IDF_TARGET_NAME} 系列模组固件的目录结构及 bin 文件也可参考如下介绍：

.. only:: esp32

   .. code-block:: none

      .
      ├── at_customize.bin                 // 二级分区表
      ├── bootloader                       // bootloader
      │   └── bootloader.bin
      ├── customized_partitions            // AT 自定义 bin 文件
      │   ├── ble_data.bin
      │   ├── client_ca.bin
      │   ├── client_cert.bin
      │   ├── client_key.bin
      │   ├── factory_param.bin
      │   ├── factory_param_WROOM-32.bin
      │   ├── mqtt_ca.bin
      │   ├── mqtt_cert.bin
      │   ├── mqtt_key.bin
      │   ├── server_ca.bin
      │   ├── server_cert.bin
      │   └── server_key.bin
      ├── download.config                  // 烧录固件的参数
      ├── esp-at.bin                       // AT 应用固件
      ├── esp-at.elf
      ├── esp-at.map
      ├── factory                          // 量产所需打包好的 bin 文件
      │   ├── factory_WROOM-32.bin
      │   └── factory_parameter.log
      ├── flasher_args.json                // 下载参数信息新的格式
      ├── ota_data_initial.bin             // ota data 区初始值
      ├── partition_table                  // 一级分区列表
      │   └── partition-table.bin
      ├── phy_init_data.bin                // phy 初始值信息
      └── sdkconfig                        // AT 固件对应的编译配置

.. only:: esp32c3 or esp32c2

   .. code-block:: none

      .
      ├── at_customize.bin                 // 二级分区表
      ├── bootloader                       // bootloader
      │   └── bootloader.bin
      ├── customized_partitions            // AT 自定义 bin 文件
      │   ├── ble_data.bin
      │   ├── client_ca.bin
      │   ├── client_cert.bin
      │   ├── client_key.bin
      │   ├── factory_param.bin
      │   ├── factory_param_MINI-1.bin
      │   ├── mqtt_ca.bin
      │   ├── mqtt_cert.bin
      │   ├── mqtt_key.bin
      │   ├── server_ca.bin
      │   ├── server_cert.bin
      │   └── server_key.bin
      ├── download.config                  // 烧录固件的参数
      ├── esp-at.bin                       // AT 应用固件
      ├── esp-at.elf
      ├── esp-at.map
      ├── factory                          // 量产所需打包好的 bin 文件
      │   ├── factory_MINI-1.bin
      │   └── factory_parameter.log
      ├── flasher_args.json                // 下载参数信息新的格式
      ├── ota_data_initial.bin             // ota data 区初始值
      ├── partition_table                  // 一级分区列表
      │   └── partition-table.bin
      ├── phy_init_data.bin                // phy 初始值信息
      └── sdkconfig                        // AT 固件对应的编译配置

其中，``download.config`` 文件包含烧录固件的参数：

.. only:: esp32

   .. code-block:: none

      --flash_mode dio --flash_freq 40m --flash_size 4MB
      0x8000 partition_table/partition-table.bin
      0x10000 ota_data_initial.bin
      0xf000 phy_init_data.bin
      0x1000 bootloader/bootloader.bin
      0x100000 esp-at.bin
      0x20000 at_customize.bin
      0x24000 customized_partitions/server_cert.bin
      0x39000 customized_partitions/mqtt_key.bin
      0x26000 customized_partitions/server_key.bin
      0x28000 customized_partitions/server_ca.bin
      0x2e000 customized_partitions/client_ca.bin
      0x30000 customized_partitions/factory_param.bin
      0x21000 customized_partitions/ble_data.bin
      0x3B000 customized_partitions/mqtt_ca.bin
      0x37000 customized_partitions/mqtt_cert.bin
      0x2a000 customized_partitions/client_cert.bin
      0x2c000 customized_partitions/client_key.bin

.. only:: esp32c2

   .. code-block:: none

      --flash_mode dio --flash_freq 60m --flash_size 4MB
      0x0 bootloader/bootloader.bin
      0x60000 esp-at.bin
      0x8000 partition_table/partition-table.bin
      0xd000 ota_data_initial.bin
      0xf000 phy_init_data.bin
      0x1e000 at_customize.bin
      0x1F000 customized_partitions/server_cert.bin
      0x21000 customized_partitions/server_key.bin
      0x23000 customized_partitions/server_ca.bin
      0x25000 customized_partitions/client_cert.bin
      0x27000 customized_partitions/client_key.bin
      0x29000 customized_partitions/client_ca.bin
      0x32000 customized_partitions/mqtt_cert.bin
      0x34000 customized_partitions/mqtt_key.bin
      0x36000 customized_partitions/mqtt_ca.bin
      0x2B000 customized_partitions/factory_param.bin

.. only:: esp32c3

   .. code-block:: none

      --flash_mode dio --flash_freq 40m --flash_size 4MB
      0x8000 partition_table/partition-table.bin
      0xd000 ota_data_initial.bin
      0xf000 phy_init_data.bin
      0x0 bootloader/bootloader.bin
      0x60000 esp-at.bin
      0x1e000 at_customize.bin
      0x1F000 customized_partitions/ble_data.bin
      0x3a000 customized_partitions/mqtt_key.bin
      0x27000 customized_partitions/server_key.bin
      0x3c000 customized_partitions/mqtt_ca.bin
      0x2d000 customized_partitions/client_key.bin
      0x2b000 customized_partitions/client_cert.bin
      0x31000 customized_partitions/factory_param.bin
      0x2f000 customized_partitions/client_ca.bin
      0x38000 customized_partitions/mqtt_cert.bin
      0x29000 customized_partitions/server_ca.bin
      0x25000 customized_partitions/server_cert.bin

.. list::

   - ``--flash_mode dio`` 代表此固件采用的 flash dio 模式进行编译；
   :esp32 or esp32c3: - ``--flash_freq 40m`` 代表此固件采用的 flash 通讯频率为 40 MHz；
   :esp32c2: - ``--flash_freq 60m`` 代表此固件采用的 flash 通讯频率为 60 MHz；
   - ``--flash_size 4MB`` 代表此固件适用的 flash 最小为 4 MB；
   :esp32: - ``0x10000 ota_data_initial.bin`` 代表在 ``0x10000`` 地址烧录 ``ota_data_initial.bin`` 文件。
   :esp32c2 or esp32c3: - ``0xd000 ota_data_initial.bin`` 代表在 ``0xd000`` 地址烧录 ``ota_data_initial.bin`` 文件。

.. _flash-at-firmware-into-your-device:

烧录 AT 固件至设备
-------------------

请根据您的操作系统选择对应的烧录方法。

Windows 
^^^^^^^^

开始烧录之前，请下载 `Flash 下载工具 <https://www.espressif.com/zh-hans/support/download/other-tools>`_。更多有关 Flash 下载工具的介绍，请参考压缩包中 ``doc`` 文件夹。

- 打开 Flash 下载工具；
- 选择芯片类型；（此处，我们选择 ``{IDF_TARGET_CFG_PREFIX}``。）
- 根据您的需求选择一种工作模式；（此处，我们选择 ``develop``。)
- 根据您的需求选择一种下载接口；（此处，我们选择 ``uart``。)

.. figure:: ../../_static/download_tool_{IDF_TARGET_PATH_NAME}.png
   :align: center
   :alt: 固件下载配置选择
   :figclass: align-center

   固件下载配置选择

- 将 AT 固件烧录至设备，以下两种方式任选其一：

   - 直接下载打包好的量产固件至 ``0x0`` 地址：勾选 "DoNotChgBin"，使用量产固件的默认配置；

     .. figure:: ../../_static/download_one_bin_{IDF_TARGET_PATH_NAME}.png
        :align: center
        :scale: 70%
        :alt: 下载至单个地址界面图

        下载至单个地址界面图（点击放大）

   - 分开下载多个 bin 文件至不同的地址：根据 ``download.config`` 文件进行配置，请勿勾选 "DoNotChgBin"；

     .. figure:: ../../_static/download_multi_bin_{IDF_TARGET_PATH_NAME}.png
        :align: center
        :scale: 60%
        :alt: 下载至多个地址界面图

        下载至多个地址界面图（点击放大）

为了避免烧录出现问题，请查看开发板的下载接口的 COM 端口号，并从 "COM:" 下拉列表中选择该端口号。有关如何查看端口号的详细介绍请参考 `在 Windows 上查看端口 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/get-started/establish-serial-connection.html#windows>`_。 

烧录完成后, 请 `检查 AT 固件是否烧录成功`_。

Linux 或 macOS
^^^^^^^^^^^^^^^

开始烧录之前，请安装 `esptool.py <https://github.com/espressif/esptool>`_。

以下两种方式任选其一，将 AT 固件烧录至设备：

- 分开下载多个 bin 文件至不同的地址：输入以下命令，替换 ``PORTNAME`` 和 ``download.config`` 参数；

  .. code-block:: none

      esptool.py --chip auto --port PORTNAME --baud 115200 --before default_reset --after hard_reset write_flash -z download.config

  将 ``PORTNAME`` 替换成开发板的下载接口名称，若您无法确定该接口名称，请参考 `在 Linux 和 macOS 上查看端口 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/get-started/establish-serial-connection.html#linux-macos>`_。

  将 ``download.config`` 替换成该文件里的参数列表。

  以下是将固件烧录至 {IDF_TARGET_MODULE_NAME} 模组输入的命令：

   .. only:: esp32

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x8000 partition_table/partition-table.bin 0x10000 ota_data_initial.bin 0xf000 phy_init_data.bin 0x1000 bootloader/bootloader.bin 0x100000 esp-at.bin 0x20000 at_customize.bin 0x24000 customized_partitions/server_cert.bin 0x39000 customized_partitions/mqtt_key.bin 0x26000 customized_partitions/server_key.bin 0x28000 customized_partitions/server_ca.bin 0x2e000 customized_partitions/client_ca.bin 0x30000 customized_partitions/factory_param.bin 0x21000 customized_partitions/ble_data.bin 0x3B000 customized_partitions/mqtt_ca.bin 0x37000 customized_partitions/mqtt_cert.bin 0x2a000 customized_partitions/client_cert.bin 0x2c000 customized_partitions/client_key.bin

   .. only:: esp32c2

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 60m --flash_size 4MB 0x0 bootloader/bootloader.bin 0x60000 esp-at.bin 0x8000 partition_table/partition-table.bin 0xd000 ota_data_initial.bin 0xf000 phy_init_data.bin 0x1e000 at_customize.bin 0x1F000 customized_partitions/server_cert.bin 0x21000 customized_partitions/server_key.bin 0x23000 customized_partitions/server_ca.bin 0x25000 customized_partitions/client_cert.bin 0x27000 customized_partitions/client_key.bin 0x29000 customized_partitions/client_ca.bin 0x32000 customized_partitions/mqtt_cert.bin 0x34000 customized_partitions/mqtt_key.bin 0x36000 customized_partitions/mqtt_ca.bin 0x2B000 customized_partitions/factory_param.bin

   .. only:: esp32c3

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x8000 partition_table/partition-table.bin 0xd000 ota_data_initial.bin 0xf000 phy_init_data.bin 0x0 bootloader/bootloader.bin 0x60000 esp-at.bin 0x1e000 at_customize.bin 0x1F000 customized_partitions/ble_data.bin 0x3a000 customized_partitions/mqtt_key.bin 0x27000 customized_partitions/server_key.bin 0x3c000 customized_partitions/mqtt_ca.bin 0x2d000 customized_partitions/client_key.bin 0x2b000 customized_partitions/client_cert.bin 0x31000 customized_partitions/factory_param.bin 0x2f000 customized_partitions/client_ca.bin 0x38000 customized_partitions/mqtt_cert.bin 0x29000 customized_partitions/server_ca.bin 0x25000 customized_partitions/server_cert.bin

- 直接下载打包好的量产固件至 ``0x0`` 地址：输入以下命令，替换 ``PORTNAME`` 和 ``FILEDIRECTORY`` 参数；

  .. code-block:: none

      esptool.py --chip auto --port PORTNAME --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 FILEDIRECTORY

  将 ``PORTNAME`` 替换成开发板的下载接口名称，若您无法确定该接口名称，请参考 `在 Linux 和 macOS 上查看端口 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/get-started/establish-serial-connection.html#linux-macos>`_。

  将 ``FILEDIRECTORY`` 替换成打包好的量产固件的文件路径，通常情况下，文件路径是 ``factory/XXX.bin``。

  以下是将固件烧录至 {IDF_TARGET_MODULE_NAME} 模组输入的命令：

   .. only:: esp32

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 factory/factory_WROOM-32.bin

   .. only:: esp32c2

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 60m --flash_size 4MB 0x0 factory/factory_MINI-1.bin

   .. only:: esp32c3

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 factory/factory_MINI-1.bin

烧录完成后，请 `检查 AT 固件是否烧录成功`_。

.. _check-whether-at-works:

检查 AT 固件是否烧录成功
-------------------------

请按照以下步骤检查 AT 固件是否烧录成功：

- 打开串口工具，如 SecureCRT；
- 串口：选择用于发送或接收“AT 命令/响应”的串口（详情请见 :doc:`Hardware_connection`）；
- 波特率：115200；
- 数据位：8；
- 检验位：None；
- 停止位：1；
- 流控：None；
- 输入 "AT+GMR" 命令，并且换行 (CR LF)；

若如下图所示，响应是 ``OK``, 则表示 AT 固件烧录成功。

.. figure:: ../../_static/at_version_{IDF_TARGET_PATH_NAME}.png
   :align: center
   :alt: AT 响应
   :figclass: align-center

   AT 响应

否则，您需要通过以下方式之一检查 {IDF_TARGET_NAME} 设备开机日志：
  
**方法 1：**

- 打开串口工具，如 SecureCRT；
- 串口：选择用于“下载固件/输出日志”的串口，串口详情请参阅 :doc:`Hardware_connection`。
- 波特率：115200；
- 数据位：8；
- 检验位：None；
- 停止位：1；
- 流控：None；
- 直接按开发板的 RST 键，若日志和下面的日志相似，则说明 ESP-AT 固件已经正确初始化了。

**方法 2：**

- 打开两个串口工具，如 SecureCRT；
- 串口：分别选择用于发送或接收“AT 命令/响应”的串口以及用于“下载固件/输出日志”的串口，串口详情请参阅 :doc:`Hardware_connection`。
- 波特率：115200；
- 数据位：8；
- 检验位：None；
- 停止位：1；
- 流控：None；
- 在发送或接收“AT 命令/响应”的串口输入 :ref:`AT+RST <cmd-RST>` 命令，并且换行 (CR LF)，若“下载固件/输出日志”的串口日志和下面的日志相似，则说明 ESP-AT 固件已经正确初始化了。

.. only:: esp32

   {IDF_TARGET_NAME} 开机日志：

   .. code-block:: none

      ets Jun  8 2016 00:22:57

      rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
      configsip: 0, SPIWP:0xee
      clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
      mode:DIO, clock div:2
      load:0x3fff0030,len:7168
      load:0x40078000,len:13200
      load:0x40080400,len:4564
      0x40080400: _init at ??:?

      entry 0x400806f4
      I (26) boot: ESP-IDF v4.2.2-76-gefa6eca 2nd stage bootloader
      I (26) boot: compile time 11:54:30
      I (26) boot: chip revision: 1
      I (30) boot_comm: chip revision: 1, min. bootloader chip revision: 0
      I (37) boot.esp32: SPI Speed      : 40MHz
      I (42) boot.esp32: SPI Mode       : DIO
      I (46) boot.esp32: SPI Flash Size : 4MB
      I (51) boot: Enabling RNG early entropy source...
      I (56) boot: Partition Table:
      I (60) boot: ## Label            Usage          Type ST Offset   Length
      I (67) boot:  0 phy_init         RF data          01 01 0000f000 00001000
      I (75) boot:  1 otadata          OTA data         01 00 00010000 00002000
      I (82) boot:  2 nvs              WiFi data        01 02 00012000 0000e000
      I (90) boot:  3 at_customize     unknown          40 00 00020000 000e0000
      I (97) boot:  4 ota_0            OTA app          00 10 00100000 00180000
      I (105) boot:  5 ota_1            OTA app          00 11 00280000 00180000
      I (112) boot: End of partition table
      I (117) boot_comm: chip revision: 1, min. application chip revision: 0
      I (124) esp_image: segment 0: paddr=0x00100020 vaddr=0x3f400020 size=0x285f8 (165368) map
      I (196) esp_image: segment 1: paddr=0x00128620 vaddr=0x3ffbdb60 size=0x03934 ( 14644) load
      I (202) esp_image: segment 2: paddr=0x0012bf5c vaddr=0x40080000 size=0x040bc ( 16572) load
      I (210) esp_image: segment 3: paddr=0x00130020 vaddr=0x400d0020 size=0x109f40 (1089344) map
      I (626) esp_image: segment 4: paddr=0x00239f68 vaddr=0x400840bc size=0x1aa04 (109060) load
      I (674) esp_image: segment 5: paddr=0x00254974 vaddr=0x400c0000 size=0x00064 (   100) load
      I (691) boot: Loaded app from partition at offset 0x100000
      I (691) boot: Disabling RNG early entropy source...
      module_name:WROOM-32
      max tx power=78,ret=0
      2.2.0

.. only:: esp32c2

   {IDF_TARGET_NAME} 开机日志:

   .. code-block:: none

      ESP-ROM:esp8684-api2-20220127
      Build:Jan 27 2022
      rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
      SPIWP:0xee
      mode:DIO, clock div:1
      load:0x3fcd6108,len:0x18b0
      load:0x403ae000,len:0x854
      load:0x403b0000,len:0x2724
      entry 0x403ae000
      I (21) boot: ESP-IDF v5.0-dev-3424-gbb23d783c0 2nd stage bootloader
      I (21) boot: compile time 19:44:11
      I (21) boot: chip revision: 0
      I (24) boot.esp32c2: MMU Page Size  : 64K
      I (29) boot.esp32c2: SPI Speed      : 60MHz
      I (34) boot.esp32c2: SPI Mode       : DIO
      I (38) boot.esp32c2: SPI Flash Size : 4MB
      I (43) boot: Enabling RNG early entropy source...
      I (49) boot: Partition Table:
      I (52) boot: ## Label            Usage          Type ST Offset   Length
      I (59) boot:  0 otadata          OTA data         01 00 0000d000 00002000
      I (67) boot:  1 phy_init         RF data          01 01 0000f000 00001000
      I (74) boot:  2 nvs              WiFi data        01 02 00010000 0000e000
      I (82) boot:  3 at_customize     unknown          40 00 0001e000 00042000
      I (89) boot:  4 ota_0            OTA app          00 10 00060000 001d0000
      I (97) boot:  5 ota_1            OTA app          00 11 00230000 001d0000
      I (104) boot: End of partition table
      I (108) esp_image: segment 0: paddr=00060020 vaddr=3c0d0020 size=279d8h (162264) map
      I (153) esp_image: segment 1: paddr=00087a00 vaddr=3fcab2b0 size=018a0h (  6304) load
      I (155) esp_image: segment 2: paddr=000892a8 vaddr=40380000 size=06d70h ( 28016) load
      I (166) esp_image: segment 3: paddr=00090020 vaddr=42000020 size=c10f8h (790776) map
      I (344) esp_image: segment 4: paddr=00151120 vaddr=40386d70 size=04534h ( 17716) load
      I (353) boot: Loaded app from partition at offset 0x60000
      I (353) boot: Disabling RNG early entropy source...
      module_name:MINI-1
      max tx power=78,ret=0
      2.5.0

.. only:: esp32c3

   {IDF_TARGET_NAME} 开机日志:

   .. code-block:: none

      ESP-ROM:esp32c3-api1-20210207
      Build:Feb  7 2021
      rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
      SPIWP:0xee
      mode:DIO, clock div:2
      load:0x3fcd6100,len:0x1948
      load:0x403ce000,len:0x8dc
      load:0x403d0000,len:0x2de0
      entry 0x403ce000
      I (31) boot: ESP-IDF v4.3.1-354-g98d34e5 2nd stage bootloader
      I (32) boot: compile time 11:21:56
      I (32) boot: chip revision: 3
      I (34) boot.esp32c3: SPI Speed      : 40MHz
      I (39) boot.esp32c3: SPI Mode       : DIO
      I (44) boot.esp32c3: SPI Flash Size : 4MB
      I (49) boot: Enabling RNG early entropy source...
      I (54) boot: Partition Table:
      I (58) boot: ## Label            Usage          Type ST Offset   Length
      I (65) boot:  0 otadata          OTA data         01 00 0000d000 00002000
      I (72) boot:  1 phy_init         RF data          01 01 0000f000 00001000
      I (80) boot:  2 nvs              WiFi data        01 02 00010000 0000e000
      I (87) boot:  3 at_customize     unknown          40 00 0001e000 00042000
      I (95) boot:  4 ota_0            OTA app          00 10 00060000 001d0000
      I (102) boot:  5 ota_1            OTA app          00 11 00230000 001d0000
      I (110) boot: End of partition table
      I (114) esp_image: segment 0: paddr=00060020 vaddr=3c140020 size=29060h (168032) map
      I (158) esp_image: segment 1: paddr=00089088 vaddr=3fc90800 size=03dfch ( 15868) load
      I (162) esp_image: segment 2: paddr=0008ce8c vaddr=40380000 size=0318ch ( 12684) load
      I (167) esp_image: segment 3: paddr=00090020 vaddr=42000020 size=130764h (1247076) map
      I (433) esp_image: segment 4: paddr=001c078c vaddr=4038318c size=0d588h ( 54664) load
      I (446) esp_image: segment 5: paddr=001cdd1c vaddr=50000000 size=00004h (     4) load
      I (447) esp_image: segment 6: paddr=001cdd28 vaddr=50000008 size=00010h (    16) load
      I (457) boot: Loaded app from partition at offset 0x60000
      I (458) boot: Disabling RNG early entropy source...
      module_name:MINI-1
      max tx power=78,ret=0
      2.3.0
