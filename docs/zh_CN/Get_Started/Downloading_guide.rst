下载指导
==========

:link_to_translation:`en:[English]`

本文档以 ESP32-WROOM-32 模组为例，介绍如何下载 ESP 模组对应的 AT 固件，以及如何将固件烧录到模组上，其它 ESP 系列模组也可参考本文档。

下载和烧录 AT 固件之前，请确保您已正确连接所需硬件，具体可参考 :doc:`Hardware_connection`。

对于不同系列的模组，AT 默认固件所支持的命令会有所差异。具体可参考 :doc:`/Compile_and_Develop/How_to_understand_the_differences_of_each_type_of_module`。

下载 AT 固件
-------------

请按照以下步骤将 AT 固件下载至 PC：

- 前往 :doc:`../AT_Binary_Lists/index`
- 找到您的模组所对应的 AT 固件
- 点击相应链接进行下载

此处，我们下载了 ESP32-WROOM-32 对应的 ``ESP32-WROOM-32_AT_Bin_V2.1`` 固件，该固件的目录结构及其中各个 bin 文件介绍如下，其它 ESP 系列模组固件的目录结构及 bin 文件也可参考如下介绍：

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
   ├── factory                          // 量产所需打包好的 bin 文件
   │   ├── factory_WROOM-32.bin
   │   └── factory_parameter.log
   ├── flasher_args.json                // 下载参数信息新的格式
   ├── ota_data_initial.bin             // ota data 区初始值
   ├── partition_table                  // 一级分区列表
   │   └── partition-table.bin
   └── phy_init_data.bin                // phy 初始值信息

其中，``download.config`` 文件包含烧录固件的参数：

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

- ``--flash_mode dio`` 代表此固件采用的 flash dio 模式进行编译；
- ``--flash_freq 40m`` 代表此固件采用的 flash 通讯频率为 40 MHz；
- ``--flash_size 4MB`` 代表此固件适用的 flash 最小为 4 MB；
- ``0x10000 ota_data_initial.bin`` 代表在 ``0x10000`` 地址烧录 ``ota_data_initial.bin`` 文件。

烧录 AT 固件至设备
-------------------

请根据您的操作系统选择对应的烧录方法。

Windows 
^^^^^^^^

开始烧录之前，请下载 `Flash 下载工具 <https://www.espressif.com/zh-hans/support/download/other-tools>`_。更多有关 Flash 下载工具的介绍，请参考压缩包中 ``readme.pdf`` 文件或 ``doc`` 文件夹。

- 打开 Flash 下载工具；
- 根据您的需求选择一种模式；（此处，我们选择 ``Developer Mode``。)

.. figure:: ../../_static/Download_tool.png
   :align: center
   :alt: Flash 下载工具的模式
   :figclass: align-center

   Flash 下载工具的模式

- 选择对应的 ESP 下载工具，如 ESP8266 设备应选择 "ESP8266 DownloadTool"，ESP32-S2 设备应选择 "ESP32-S2 DownloadTool"；（此处，我们选择 "ESP32 DownloadTool"。）

.. figure:: ../../_static/esp32_wroom32_download_select.png
   :align: center
   :alt: Flash 下载工具选择
   :figclass: align-center

   Flash 下载工具选择

- 将 AT 固件烧录至设备，以下两种方式任选其一：

   - 直接下载打包好的量产固件至 ``0x0`` 地址：勾选 "DoNotChgBin"，使用量产固件的默认配置；

     .. figure:: ../../_static/esp32_wroom32_download_one_bin.png
        :align: center
        :scale: 70%
        :alt: 下载至单个地址界面图

        下载至单个地址界面图

   - 分开下载多个 bin 文件至不同的地址：根据 ``download.config`` 文件进行配置，请勿勾选 "DoNotChgBin"；

     .. figure:: ../../_static/esp32_wroom32_download_multi_bin.png
        :align: center
        :scale: 60%
        :alt: 下载至多个地址界面图

        下载至多个地址界面图

为了避免烧录出现问题，请查看开发板的下载接口的 COM 端口号，并从 "COM:" 下拉列表中选择该端口号。有关如何查看端口号的详细介绍请参考 `在 Windows 上查看端口 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/get-started/establish-serial-connection.html#windows>`_。 

烧录完成后, 请 `检查 AT 固件是否烧录成功`_。

Linux 或 macOS
^^^^^^^^^^^^^^^

开始烧录之前，请安装 `esptool.py <https://github.com/espressif/esptool>`_。

以下两种方式任选其一，将 AT 固件烧录至设备：

- 分开下载多个 bin 文件至不同的地址：输入以下命令，替换 ``PORTNAME`` 和 ``download.config`` 参数；

  .. code-block:: none

       esptool.py --chip auto --port PORTNAME --baud 115200 --before default_reset --after hard_reset write_flash -z download.config

  将 ``PORTNAME`` 替换成开发板的下载接口名称，若您无法确定该接口名称，请参考 `在 Linux 和 macOS 上查看端口 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/get-started/establish-serial-connection.html#linux-macos>`_。

  将 ``download.config`` 替换成该文件里的参数列表。

  以下是将固件烧录至 ESP32-WROOM-32 模组输入的命令：

  .. code-block:: none

        esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x8000 partition_table/partition-table.bin 0x10000 ota_data_initial.bin 0xf000 phy_init_data.bin 0x1000 bootloader/bootloader.bin 0x100000 esp-at.bin 0x20000 at_customize.bin 0x24000 customized_partitions/server_cert.bin 0x39000 customized_partitions/mqtt_key.bin 0x26000 customized_partitions/server_key.bin 0x28000 customized_partitions/server_ca.bin 0x2e000 customized_partitions/client_ca.bin 0x30000 customized_partitions/factory_param.bin 0x21000 customized_partitions/ble_data.bin 0x3B000 customized_partitions/mqtt_ca.bin 0x37000 customized_partitions/mqtt_cert.bin 0x2a000 customized_partitions/client_cert.bin 0x2c000 customized_partitions/client_key.bin

- 直接下载打包好的量产固件至 ``0x0`` 地址：输入以下命令，替换 ``PORTNAME`` 和 ``FILEDIRECTORY`` 参数；

  .. code-block:: none

        esptool.py --chip auto --port PORTNAME --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 FILEDIRECTORY

  将 ``PORTNAME`` 替换成开发板的下载接口名称，若您无法确定该接口名称，请参考 `在 Linux 和 macOS 上查看端口 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/get-started/establish-serial-connection.html#linux-macos>`_。

  将 ``FILEDIRECTORY`` 替换成打包好的量产固件的文件路径，通常情况下，文件路径是 ``factory/XXX.bin``。

  以下是将固件烧录至 ESP32-WROOM-32 模组输入的命令：

  .. code-block:: none

        esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 factory/factory_WROOM-32.bin

烧录完成后, 请 `检查 AT 固件是否烧录成功`_。

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

.. figure:: ../../_static/esp32_wroom32_version.png
   :align: center
   :alt: AT 响应
   :figclass: align-center

   AT 响应

否则，您需要检查 ESP 设备开机日志，可以通过“下载/输出日志”串口在电脑上查看。若日志和下面的日志相似，则说明 ESP-AT 固件已经正确初始化了。

ESP32 开机日志：

.. code-block:: none

    ets Jun  8 2016 00:22:57
    rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
    configsip: 0, SPIWP:0xee
    clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
    mode:DIO, clock div:2
    load:0x3fff0030,len:4
    load:0x3fff0034,len:7184
    ho 0 tail 12 room 4
    load:0x40078000,len:13200
    load:0x40080400,len:4564
    entry 0x400806f4
    I (30) boot: ESP-IDF v4.2 2nd stage bootloader
    I (31) boot: compile time 11:23:19
    I (31) boot: chip revision: 0
    I (33) boot.esp32: SPI Speed      : 40MHz
    I (38) boot.esp32: SPI Mode       : DIO
    I (42) boot.esp32: SPI Flash Size : 4MB
    I (47) boot: Enabling RNG early entropy source...
    I (52) boot: Partition Table:
    I (56) boot: ## Label            Usage          Type ST Offset   Length
    I (63) boot:  0 phy_init         RF data          01 01 0000f000 00001000
    I (71) boot:  1 otadata          OTA data         01 00 00010000 00002000
    I (78) boot:  2 nvs              WiFi data        01 02 00012000 0000e000
    I (86) boot:  3 at_customize     unknown          40 00 00020000 000e0000
    I (93) boot:  4 ota_0            OTA app          00 10 00100000 00180000
    I (101) boot:  5 ota_1            OTA app          00 11 00280000 00180000
    I (108) boot: End of partition table
    I (112) esp_image: segment 0: paddr=0x00100020 vaddr=0x3f400020 size=0x2a300 (172800) map
    I (187) esp_image: segment 1: paddr=0x0012a328 vaddr=0x3ffbdb60 size=0x039e8 ( 14824) load
    I (194) esp_image: segment 2: paddr=0x0012dd18 vaddr=0x40080000 size=0x00404 (  1028) load
    I (194) esp_image: segment 3: paddr=0x0012e124 vaddr=0x40080404 size=0x01ef4 (  7924) load
    I (206) esp_image: segment 4: paddr=0x00130020 vaddr=0x400d0020 size=0x10a470 (1090672) map
    I (627) esp_image: segment 5: paddr=0x0023a498 vaddr=0x400822f8 size=0x1c3a0 (115616) load
    I (678) esp_image: segment 6: paddr=0x00256840 vaddr=0x400c0000 size=0x00064 (   100) load
    I (695) boot: Loaded app from partition at offset 0x100000
    I (695) boot: Disabling RNG early entropy source...
    max tx power=78,ret=0
    2.1.0

ESP32-S2 开机日志：

.. code-block:: none

    ESP-ROM:esp32s2-rc4-20191025
    Build:Oct 25 2019
    rst:0x1 (POWERON),boot:0x8 (SPI_FAST_FLASH_BOOT)
    SPIWP:0xee
    mode:DIO, clock div:1
    load:0x3ffe6100,len:0x4
    load:0x3ffe6104,len:0x1a24
    load:0x4004c000,len:0x1a6c
    load:0x40050000,len:0x20fc
    entry 0x4004c35c
    I (46) boot: ESP-IDF v4.2 2nd stage bootloader
    I (46) boot: compile time 11:24:34
    I (46) boot: chip revision: 0
    I (47) qio_mode: Enabling default flash chip QIO
    I (53) boot.esp32s2: SPI Speed      : 80MHz
    I (57) boot.esp32s2: SPI Mode       : QIO
    I (62) boot.esp32s2: SPI Flash Size : 4MB
    I (67) boot: Enabling RNG early entropy source...
    I (72) boot: Partition Table:
    I (76) boot: ## Label            Usage          Type ST Offset   Length
    I (83) boot:  0 phy_init         RF data          01 01 0000f000 00001000
    I (91) boot:  1 otadata          OTA data         01 00 00010000 00002000
    I (98) boot:  2 nvs              WiFi data        01 02 00012000 0000e000
    I (106) boot:  3 at_customize     unknown          40 00 00020000 000e0000
    I (113) boot:  4 ota_0            OTA app          00 10 00100000 00180000
    I (121) boot:  5 ota_1            OTA app          00 11 00280000 00180000
    I (128) boot: End of partition table
    I (133) esp_image: segment 0: paddr=0x00100020 vaddr=0x3f000020 size=0x21bec (138220) map
    I (167) esp_image: segment 1: paddr=0x00121c14 vaddr=0x3ffc9330 size=0x02fe0 ( 12256) load
    I (169) esp_image: segment 2: paddr=0x00124bfc vaddr=0x40024000 size=0x00404 (  1028) load
    I (173) esp_image: segment 3: paddr=0x00125008 vaddr=0x40024404 size=0x0b010 ( 45072) load
    I (193) esp_image: segment 4: paddr=0x00130020 vaddr=0x40080020 size=0xb0784 (722820) map
    I (324) esp_image: segment 5: paddr=0x001e07ac vaddr=0x4002f414 size=0x09f18 ( 40728) load
    I (334) esp_image: segment 6: paddr=0x001ea6cc vaddr=0x40070000 size=0x0001c (    28) load
    I (346) boot: Loaded app from partition at offset 0x100000
    I (346) boot: Disabling RNG early entropy source...
    max tx power=78,ret=0
    2.1.0

ESP32-C3 开机日志:

.. code-block:: none

    ESP-ROM:esp32c3-20200918
    Build:Sep 18 2020
    rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
    SPIWP:0xee
    mode:DIO, clock div:2
    load:0x3fcd6100,len:0x14
    load:0x3fcd6114,len:0x179c
    load:0x403ce000,len:0x894
    load:0x403d0000,len:0x2bf8
    entry 0x403ce000
    I (54) boot: ESP-IDF v4.3-beta1 2nd stage bootloader
    I (55) boot: compile time 12:09:42
    I (55) boot: chip revision: 1
    I (57) boot_comm: chip revision: 1, min. bootloader chip revision: 0
    I (64) boot.esp32c3: SPI Speed      : 40MHz
    I (68) boot.esp32c3: SPI Mode       : DIO
    I (73) boot.esp32c3: SPI Flash Size : 4MB
    I (78) boot: Enabling RNG early entropy source...
    I (83) boot: Partition Table:
    I (87) boot: ## Label            Usage          Type ST Offset   Length
    I (94) boot:  0 phy_init         RF data          01 01 0000f000 00001000
    I (102) boot:  1 otadata          OTA data         01 00 00010000 00002000
    I (109) boot:  2 nvs              WiFi data        01 02 00012000 0000e000
    I (117) boot:  3 at_customize     unknown          40 00 00020000 000e0000
    I (124) boot:  4 ota_0            OTA app          00 10 00100000 00180000
    I (132) boot:  5 ota_1            OTA app          00 11 00280000 00180000
    I (139) boot: End of partition table
    I (144) boot: No factory image, trying OTA 0
    I (149) boot_comm: chip revision: 1, min. application chip revision: 0
    I (156) esp_image: segment 0: paddr=00100020 vaddr=3c140020 size=29cc8h (171208) map
    I (201) esp_image: segment 1: paddr=00129cf0 vaddr=3fc8f000 size=03be8h ( 15336) load
    I (205) esp_image: segment 2: paddr=0012d8e0 vaddr=40380000 size=02738h ( 10040) load
    I (210) esp_image: segment 3: paddr=00130020 vaddr=42000020 size=135bf0h (1268720) map
    I (489) esp_image: segment 4: paddr=00265c18 vaddr=40382738 size=0c778h ( 51064) load
    I (502) esp_image: segment 5: paddr=00272398 vaddr=50000000 size=00004h (     4) load
    I (508) boot: Loaded app from partition at offset 0x100000
    I (544) boot: Set actual ota_seq=1 in otadata[0]
    I (544) boot: Disabling RNG early entropy source...
    max tx power=78,ret=0
    2.1.0

ESP8266 开机日志：

.. code-block:: none

    ...
    boot: ESP-IDF v3.4-rc 2nd stage bootloader
    I (54) boot: compile time 11:18:21
    I (54) boot: SPI Speed      : 80MHz
    I (57) boot: SPI Mode       : DIO
    I (61) boot: SPI Flash Size : 2MB
    I (65) boot: Partition Table:
    I (68) boot: ## Label            Usage          Type ST Offset   Length
    I (75) boot:  0 otadata          OTA data         01 00 00009000 00002000
    I (83) boot:  1 phy_init         RF data          01 01 0000f000 00001000
    I (90) boot:  2 ota_0            OTA app          00 10 00010000 000e0000
    I (98) boot:  3 at_customize     unknown          40 00 000f0000 00020000
    I (105) boot:  4 ota_1            OTA app          00 11 00110000 000e0000
    I (112) boot:  5 nvs              WiFi data        01 02 001f0000 00010000
    I (120) boot: End of partition table
    I (124) boot: No factory image, trying OTA 0
    I (129) esp_image: segment 0: paddr=0x00010010 vaddr=0x40210010 size=0xac0d0 (704720) map
    I (138) esp_image: segment 1: paddr=0x000bc0e8 vaddr=0x402bc0e0 size=0x1aba8 (109480) map
    I (146) esp_image: segment 2: paddr=0x000d6c98 vaddr=0x3ffe8000 size=0x00788 (  1928) load
    I (155) esp_image: segment 3: paddr=0x000d7428 vaddr=0x40100000 size=0x00080 (   128) load
    I (164) esp_image: segment 4: paddr=0x000d74b0 vaddr=0x40100080 size=0x059c4 ( 22980) load
    I (173) boot: Loaded app from partition at offset 0x10000
    phy_version: 1163.0, 665d56c, Jun 24 2020, 10:00:08, RTOS new
    max tx power=78,ret=0
    2.0.0