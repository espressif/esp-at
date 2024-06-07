AT 固件
=================

:link_to_translation:`en:[English]`

.. toctree::
  :hidden:
  :maxdepth: 1

  {IDF_TARGET_NAME} AT 发布版固件 <esp_at_binaries>

本文档包含以下小节：

.. list::

  - :doc:`下载 {IDF_TARGET_NAME} AT 发布版固件 <esp_at_binaries>`
  - :ref:`brief-intro-firmware`：AT 固件包含哪些二进制文件及其作用

.. note::
  若需下载其他芯片系列的发布版固件，请在页面左上方的下拉菜单栏选择相应的芯片，即可跳转至该芯片的文档进行下载。

.. _brief-intro-firmware:

AT 固件简介
--------------

ESP-AT 固件包含了若干个特定功能的二进制文件：

.. code-block:: none

  build
  ├── at_customize.bin        // 二级分区表（用户分区表，列出了 mfg_nvs 分区以及 fatfs 分区的起始地址和分区大小）
  ├── bootloader
  │   └── bootloader.bin      // 启动加载器
  ├── customized_partitions
  │   └── mfg_nvs.bin         // 出厂配置参数，参数值见同级目录下的 mfg_nvs.csv
  ├── esp-at.bin              // AT 应用固件
  ├── factory
  │   └── factory_xxx.bin     // 特定功能的二进制文件合集，您可以仅烧录本文件到起始地址为 0 的 flash 空间中，或者根据 download.config 文件中的信息将若干个二进制文件烧录到 flash 中对应起始地址的空间中。
  ├── partition_table
  │   └── partition-table.bin // 一级分区表（系统分区表）
  └── ota_data_initial.bin    // OTA 数据初始化文件
