如何自定义分区
===========================

{IDF_TARGET_AT_SECOND_PARTITION_ADDR: default="undefined", esp32="0x20000", esp32c2="0x1E000", esp32c3="0x1E000", esp32c6="0x1E000", esp32s2="0x20000"}
{IDF_TARGET_PRODUCT_NAME: default="undefined", esp32="ESP32-WROOM-32", esp32c2="ESP8684-MINI-1 4MB", esp32c3="ESP32-C3-MINI-1", esp32c6="ESP32-C6-MINI-1", esp32s2="ESP32-S2-MINI"}

:link_to_translation:`en:[English]`

本文档介绍了如何通过修改 ESP-AT 提供的 at_customize.csv 表来自定义 {IDF_TARGET_NAME} 设备中的分区。共有两个分区表：一级分区表和二级分区表。

一级分区表 partitions_at.csv 供系统使用，在此基础上生成 partitions_at.bin 文件。如果一级分区表出错，系统将无法启动。因此，不建议修改 partitions_at.csv。

ESP-AT 提供了二级分区表 at_customize.csv 供您存储自定义数据块。它基于一级分区表。

要修改 {IDF_TARGET_NAME} 设备中的分区，请按照前三个步骤操作。第四部分举例说明了前三个步骤。

- :ref:`modify-at-customize-csv`
- :ref:`generate-at-customize-bin`
- :ref:`flash-at-customize-bin-into-ESP`
- :ref:`customize-partition-example`

.. _modify-at-customize-csv:

修改 at_customize.csv
-----------------------

请参考下表找到模组的 at_customize.csv。

.. only:: esp32

  .. list-table:: at_customize.csv 路径
    :header-rows: 1

    * - 平台
      - 模组
      - 路径
    * - ESP32
      - - WROOM-32
        - PICO-D4
        - SOLO-1
        - MINI-1
      - :project_file:`module_config/module_esp32_default/at_customize.csv`
    * - ESP32
      - WROVER-32
      - :project_file:`module_config/module_wrover-32/at_customize.csv`
    * - ESP32
      - ESP32-D2WD
      - :project_file:`module_config/module_esp32-d2wd/at_customize.csv`

.. only:: esp32c2

  .. list-table:: at_customize.csv 路径
    :header-rows: 1

    * - 平台
      - 模组
      - 路径
    * - ESP32-C2
      - ESP32C2-2MB（所有带 2 MB flash 的 ESP32-C2（ESP8684） 系列）
      - :project_file:`module_config/module_esp32c2-2mb/at_customize.csv`
    * - ESP32-C2
      - ESP32C2-4MB（所有带 4 MB flash 的 ESP32-C2（ESP8684） 系列）
      - :project_file:`module_config/module_esp32c2_default/at_customize.csv`

.. only:: esp32c3

  .. list-table:: at_customize.csv 路径
    :header-rows: 1

    * - 平台
      - 模组
      - 路径
    * - ESP32-C3
      - MINI-1
      - :project_file:`module_config/module_esp32c3_default/at_customize.csv`

.. only:: esp32c6

  .. list-table:: at_customize.csv 路径
    :header-rows: 1

    * - 平台
      - 模组
      - 路径
    * - ESP32-C6
      - ESP32C6-4MB（所有带 4 MB flash 的 ESP32-C6 系列）
      - :project_file:`module_config/module_esp32c6_default/at_customize.csv`

.. only:: esp32s2

  .. list-table:: at_customize.csv 路径
    :header-rows: 1

    * - 平台
      - 模组
      - 路径
    * - ESP32-S2
      - MINI
      - :project_file:`module_config/module_esp32s2_default/at_customize.csv`

然后，在修改 at_customize.csv 时遵循以下规则。

- 已定义的用户分区的 ``Name`` 和 ``Type`` 不可更改，但 ``SubType``、``Offset`` 和 ``Size`` 可以更改。
- 如果您需要添加一个新的用户分区，请先检查它是否已经在 ESP-IDF (``esp_partition.h``) 中定义。

    - 如果已定义，请保持 ``Type`` 值与 ESP-IDF 的相同。
    - 如果未定义，请将 ``Type`` 设置为 ``0x40``。
- 用户分区的 ``Name`` 不应超过 ``16`` 字节。
- ``at_customize`` 分区的默认大小定义在 partitions_at.csv 表中，添加新用户分区时请不要超出范围。

.. _generate-at-customize-bin:

生成 at_customize.bin
--------------------------

修改 at_customize.csv 后，您可以重新编译 ESP-AT 工程或使用 python 脚本 ``gen_esp32part.py`` 来生成 at_customize.bin 文件。

如果使用脚本，在 ESP-AT 工程根目录下执行以下命令，并替换 ``INPUT`` 和 ``OUTPUT``。

::

    python esp-idf/components/partition_table/gen_esp32part.py <INPUT> [OUTPUT]

- ``INPUT`` 替换为待解析的 at_customize.csv 或二进制文件的路径。
- ``OUTPUT`` 替换为生成的二进制或 CSV 文件的路径，如果省略，将使用标准输出。

.. _flash-at-customize-bin-into-ESP:

烧录 at_customize.bin 至 {IDF_TARGET_NAME} 设备
-----------------------------------------------------

将 at_customize.bin 下载到 flash 中。关于如何将二进制文件烧录至 {IDF_TARGET_NAME} 设备，请参考 :ref:`flash-at-firmware-into-your-device`。下表为不同模组 at_customize.bin 文件的下载地址。

.. only:: esp32

  .. list-table:: 不同模组 at_customize.bin 的下载地址
    :header-rows: 1

    * - 平台
      - 模组
      - 地址
      - 大小
    * - ESP32
      - - WROOM-32
        - WROVER-32
        - PICO-D4
        - SOLO-1
        - MINI-1
        - ESP32-D2WD
      - 0x20000
      - 0xE0000

.. only:: esp32c2

  .. list-table:: 不同模组 at_customize.bin 的下载地址
    :header-rows: 1

    * - 平台
      - 模组
      - 地址
      - 大小
    * - ESP32-C2
      - ESP32C2-2MB（所有带 2 MB flash 的 ESP32-C2（ESP8684） 系列）
      - 0x1A000
      - 0x26000
    * - ESP32-C2
      - ESP32C2-4MB（所有带 4 MB flash 的 ESP32-C2（ESP8684） 系列）
      - {IDF_TARGET_AT_SECOND_PARTITION_ADDR}
      - 0x42000

.. only:: esp32c3

  .. list-table:: 不同模组 at_customize.bin 的下载地址
    :header-rows: 1

    * - 平台
      - 模组
      - 地址
      - 大小
    * - ESP32-C3
      - MINI-1
      - 0x1E000
      - 0x42000

.. only:: esp32c6

  .. list-table:: 不同模组 at_customize.bin 的下载地址
    :header-rows: 1

    * - 平台
      - 模组
      - 地址
      - 大小
    * - ESP32-C6
      - ESP32C6-4MB（所有带 4 MB flash 的 ESP32-C6 系列）
      - 0x1E000
      - 0x42000

.. only:: esp32s2

  .. list-table:: 不同模组 at_customize.bin 的下载地址
    :header-rows: 1

    * - 平台
      - 模组
      - 地址
      - 大小
    * - ESP32-S2
      - MINI
      - 0x20000
      - 0xE0000

在某些情况下，必须将 at_customize.bin 下载到 flash 后才能使用一些 AT 命令：

- :ref:`cmd-SYSFLASH`
- :ref:`cmd-FS`
- ``SSL`` 服务器相关命令
- ``BLE`` 服务器相关命令

.. _customize-partition-example:

示例
-------

本节介绍如何将名为 ``test`` 的 4 KB 分区添加到 {IDF_TARGET_PRODUCT_NAME} 模组中。

首先找到 {IDF_TARGET_PRODUCT_NAME} 的 at_customize.csv 表，设置新分区的 ``Name``、``Type``、``SubType``、``Offset`` 和 ``Size``。

.. only:: esp32 or esp32s2

  ::

      # Name,Type,SubType,Offset,Size
      ... ...
      test,0x40,15,0x3D000,4K
      fatfs,data,fat,0x70000,576K

.. only:: esp32c2 or esp32c3 or esp32c6

  ::

      # Name,Type,SubType,Offset,Size
      ... ...
      test,0x40,15,0x3E000,4K
      fatfs,data,fat,0x47000,100K

第二步，重新编译 ESP-AT 工程，或者在 ESP-AT 根目录下执行 python 脚本生成 at_customize.bin。

::

    python esp-idf/components/partition_table/gen_esp32part.py -q ./module_config/module_{IDF_TARGET_PATH_NAME}_default/at_customize.csv at_customize.bin

然后，ESP-AT 根目录中会生成 at_customize.bin。

第三步，下载 at_customize.bin 至 flash。

在 ESP-AT 工程根目录下执行以下命令，并替换 ``PORT`` 和 ``BAUD``。

::

    python esp-idf/components/esptool_py/esptool/esptool.py -p PORT -b BAUD --before default_reset --after hard_reset --chip auto  write_flash --flash_mode dio --flash_size detect --flash_freq 40m {IDF_TARGET_AT_SECOND_PARTITION_ADDR} ./at_customize.bin

- ``PORT`` 替换为端口名称。
- ``BAUD`` 替换为波特率。
