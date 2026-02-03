.. _FS-AT:

文件系统 AT 命令集
===================

:link_to_translation:`en:[English]`

.. list::

  - :ref:`介绍 <cmd-fs-intro>`
  - :ref:`AT+FS <cmd-FS>`：文件系统操作
  - :ref:`AT+FSMOUNT <cmd-FSMOUNT>`：挂载/卸载文件系统

.. _cmd-fs-intro:

介绍
------

.. important::
  默认的 AT 固件不支持此页面下的 AT 命令。如果您需要 {IDF_TARGET_NAME} 支持文件系统命令，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：

  - 启用 ``Component config`` > ``AT`` > ``AT FS command support``

.. _cmd-FS:

:ref:`AT+FS <FS-AT>`：文件系统操作
---------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+FS=<type>,<operation>,<filename>,<offset>,<length>

**响应：**

::

    OK

参数
^^^^

- **<type>**：文件系统类型，推荐使用 LittleFS。可在 ``Component config`` > ``AT`` > ``AT FS command support`` > ``Filesystem type for AT FS command`` 中选择。

  - 0：FATFS
  - 1：LittleFS

- **<operation>**:

   -  0：删除文件
   -  1：写文件
   -  2：读文件
   -  3：查询文件大小
   -  4：查询路径下文件，目前仅支持根目录
   -  5：计算文件的 MD5 哈希值，用于校验文件完整性
   -  6：查询文件系统的空闲空间和总空间

-  **<offset>**：偏移地址，仅针对读写操作设置
-  **<length>**：长度，仅针对读写操作设置

说明
^^^^

-  本命令会自动挂载文件系统。:ref:`AT+FS <cmd-FS>` 文件系统操作完成后，强烈建议使用 :ref:`AT+FSMOUNT=0 <cmd-FSMOUNT>` 命令卸载文件系统，来释放大量 RAM 空间。
-  使用本命令需烧录 at_customize.bin，详细信息可参考 `ESP-IDF 分区表 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/partition-tables.html>`_ 和 :doc:`../Compile_and_Develop/How_to_customize_partitions`。
-  若读取数据的长度大于实际文件大小，仅返回实际长度的数据。
-  当 ``<operator>`` 为 ``write`` 时，系统收到此命令后先换行返回 ``>``，此时您可以输入要写的数据，数据长度应与 ``<length>`` 一致。
-  若需通过 OTA 升级 AT 固件并确保其兼容旧版 FATFS 分区，请在以下配置路径 ``Component config`` > ``AT`` > ``AT FS command support`` > ``Filesystem type for AT FS command`` 中选择 ``FatFS (legacy partition, upgrade only, READ HELP FIRST)``。

示例
^^^^

::

    // 删除某个文件
    AT+FS=1,0,"filename"

    // 在某个文件偏移地址 100 处写入 10 字节
    AT+FS=1,1,"filename",100,10

    // 从某个文件偏移地址 0 处读取 100 字节
    AT+FS=1,2,"filename",0,100

    // 列出根目录下所有文件
    AT+FS=1,4,"."

    // 计算某个文件的 MD5 哈希值
    AT+FS=1,5,"filename"

    // 查询文件系统的空闲空间和总空间
    AT+FS=1,6

.. _cmd-FSMOUNT:

:ref:`AT+FSMOUNT <FS-AT>`：挂载/卸载 FS 文件系统
---------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+FSMOUNT=<mount>

**响应：**

::

    OK

参数
^^^^

-  **<mount>**：

   -  0：卸载 FS 文件系统
   -  1：挂载 FS 文件系统

说明
^^^^

-  :ref:`AT+FS <cmd-FS>` 文件系统操作完成后，强烈建议使用本命令 :ref:`AT+FSMOUNT=0 <cmd-FSMOUNT>` 命令卸载文件系统，来释放大量 RAM 空间。

示例
^^^^

::

    // 手动卸载文件系统
    AT+FSMOUNT=0

    // 手动挂载文件系统
    AT+FSMOUNT=1
