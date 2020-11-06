下载指导
==========

:link_to_translation:`en:[English]`

本文档以 ESP32-WROOM-32 模组为例，介绍如何下载 ESP 模组对应的 AT 固件，以及如何将固件烧录到模组上，其它 ESP 系列模组也可参考本文档。

下载和烧录 AT 固件之前，请确保您已正确连接所需硬件，具体可参考 :doc:`Hardware_connection`。

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