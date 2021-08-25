自定义 Bluetooth LE Services 工具
======================================

:link_to_translation:`en:[English]`

ESP-AT 提供 python 脚本 ``BLEService.py`` 将自定义 Bluetooth LE 服务转换为 ``ble_data.bin``。关于自定义 Bluetooth LE 服务，请参考文档 :doc:`How_to_customize_BLE_services`。

.. contents::
   :local:
   :depth: 1

生成二进制文件
-------------------------------------

请选择以下方式之一生成 ``ble_data.bin``。

.. contents::
   :local:
   :depth: 1

脚本生成
^^^^^^^^^^^^^^^^^^^^

脚本 ``BLEService.py`` 的路径是 :at:`tools/BLEService.py`。您可以通过 ``-h`` 选项获取脚本的帮助信息。您也可以直接通过以下命令生成 ``ble_data.bin`` 二进制文件。

.. code-block:: none

    python <SCRIPT_PATH> [-t TARGET_FILE_NAME] <source_file>

- ``SCRIPT_PATH``：脚本路径。如果您正处于 “tools” 目录下，则 ``SCRIPT_PATH`` 为 ``BLEService.py``。
- ``TARGET_FILE_NAME``：要保存生成的目标文件名（目标文件名的绝对地址或者相对地址）；如果没有指定，则会在 ``source_file`` 所在目录下生成 ``ATBleService.bin`` 二进制文件。
- ``source_file``：您想转换的 Bluetooth LE GATT 服务源文件（源文件的绝对地址或者相对地址）。

例如，您可以执行以下命令在 tools 目录下生成 ``ble_data.bin`` 二进制文件。

.. code-block:: none

    python BLEService.py -t ble_data.bin ../components/customized_partitions/raw_data/ble_data/example.csv

编译期间生成
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ESP-AT 中 Bluetooth LE GATT service 文件的存储路径为 :at:`components/customized_partitions/raw_data`。

对于怎样自定义 Bluetooth LE 服务，请参考文档 :doc:`How_to_customize_BLE_services`。

对于怎样编译 ESP-AT 工程，请参考文档 :doc:`How_to_clone_project_and_compile_it`。

下载或者更新二进制文件
-------------------------------------

脚本 ``BLEService.py`` 仅仅负责将证书文件转换为二进制文件。您可以通过以下方式将二进制文件烧录到 flash 中：

1. 通过烧录工具烧录二进制文件

  #. Windows

     请下载 Windows `Flash 下载工具 <https://www.espressif.com/en/support/download/other-tools>`_。

     请参考 zip 文件夹中 ``readme.pdf`` 或者 ``doc`` 目录获取更多有关该工具的信息。

  #. Linux or macOS

     请使用 `esptool.py <https://github.com/espressif/esptool>`_。

     您可以参考 :doc:`How_to_customize_BLE_services` 获取更多信息。

     .. code-block:: none

       esptool.py --chip auto --port PORTNAME --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB ADDRESS FILEDIRECTORY

     将 ``PORTNAME`` 替换为您的串口。将 ``ADDRESS`` 替换为烧录地址。将 ``FILEDIRECTORY`` 替换为二进制文件所在的目录。

2. 通过命令更新证书二进制文件

  #. :ref:`AT+SYSFLASH <cmd-SYSFLASH>` 命令

     以 ``ESP32`` 模组为例，您可以执行以下命令来更新 ``ble_data`` 分区。请参考 :ref:`AT+SYSFLASH <cmd-SYSFLASH>` 获取更多信息。

     1. 查询 flash 用户分区

        命令：

        .. code-block:: none

          AT+SYSFLASH?

        响应：

        .. code-block:: none

          +SYSFLASH:"ble_data",64,1,0x21000,0x3000
          +SYSFLASH:"server_cert",64,2,0x24000,0x2000
          +SYSFLASH:"server_key",64,3,0x26000,0x2000
          +SYSFLASH:"server_ca",64,4,0x28000,0x2000
          +SYSFLASH:"client_cert",64,5,0x2a000,0x2000
          +SYSFLASH:"client_key",64,6,0x2c000,0x2000
          +SYSFLASH:"client_ca",64,7,0x2e000,0x2000
          +SYSFLASH:"factory_param",64,8,0x30000,0x1000
          +SYSFLASH:"wpa2_cert",64,9,0x31000,0x2000
          +SYSFLASH:"wpa2_key",64,10,0x33000,0x2000
          +SYSFLASH:"wpa2_ca",64,11,0x35000,0x2000
          +SYSFLASH:"mqtt_cert",64,12,0x37000,0x2000
          +SYSFLASH:"mqtt_key",64,13,0x39000,0x2000
          +SYSFLASH:"mqtt_ca",64,14,0x3b000,0x2000
          +SYSFLASH:"fatfs",1,129,0x70000,0x90000

          OK

     2. 擦除 ``ble_data`` 分区

        命令：

        .. code-block:: none

          AT+SYSFLASH=0,"ble_data"

        响应：

        .. code-block:: none

          OK

     3. 更新 ``ble_data`` 分区

        命令：

        .. code-block:: none

          AT+SYSFLASH=1,"ble_data",0,2344

        响应：

        .. code-block:: none

          >

        当 ``<operator>`` 为 ``write`` 时，系统收到此命令后先换行返回 ``>``，此时您可以输入要写的数据，数据长度应与 ``<length>`` 一致。当写入操作完成之后，系统会提示以下信息。

        .. code-block:: none

          OK

  #. :ref:`AT+CIUPDATE <cmd-UPDATE>`

     例如，您可以执行以下命令来更新 ``ble_data`` 分区（前提是您必须使用 Wi-Fi 功能）。请参考 :ref:`AT+CIUPDATE <cmd-UPDATE>` 获取更多信息。

     .. Important::
       如果您想通过这种方式更新 ``ble_data`` 分区，您必须实现自己的 OTA 设备，请参考文档 :doc:`How_to_implement_OTA_update`。

     .. code-block:: none

       AT+CIUPDATE=1,"v2.2.0.0","ble_data"

.. note::

  您必须确保烧录的地址是正确的，否则 ESP-AT 固件可能不能工作。查看烧录地址的最简单方法是执行命令 **AT+SYSFLASH?**。
