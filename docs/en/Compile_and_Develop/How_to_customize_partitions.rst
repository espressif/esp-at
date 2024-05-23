How to Customize Partitions
===========================

{IDF_TARGET_AT_SECOND_PARTITION_ADDR: default="undefined", esp32="0x20000", esp32c2="0x1E000", esp32c3="0x1E000", esp32c6="0x1E000", esp32s2="0x20000"}
{IDF_TARGET_PRODUCT_NAME: default="undefined", esp32="ESP32-WROOM-32", esp32c2="ESP8684-MINI-1 4MB", esp32c3="ESP32-C3-MINI-1", esp32c6="ESP32-C6-MINI-1", esp32s2="ESP32-S2-MINI"}

:link_to_translation:`zh_CN:[中文]`

This document describes how to customize the partitions in your {IDF_TARGET_NAME} by modifying the at_customize.csv table provided by ESP-AT. There are two partition tables: the primary partition and the secondary partition table.

The primary partition table partitions_at.csv is for system usage, based on which the partitions_at.bin file is generated. If the primary partition table goes wrong, the system will fail to startup. Therefore, it is not recommended to modify the partitions_at.csv.

ESP-AT provides a secondary partition table at_customize.csv that you can customize to store self-defined blocks of data. It is based on the primary partition table.

To modify the partition in your {IDF_TARGET_NAME}, please follow the first three steps. The fourth section illustrates the three steps with an example.

- :ref:`modify-at-customize-csv`
- :ref:`generate-at-customize-bin`
- :ref:`flash-at-customize-bin-into-ESP`
- :ref:`customize-partition-example`

.. _modify-at-customize-csv:

Modify at_customize.csv
-----------------------

Find the at_customize.csv for your module with reference to the following table.

.. only:: esp32

  .. list-table:: at_customize.csv paths
    :header-rows: 1

    * - Platform
      - Module
      - Path
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

  .. list-table:: at_customize.csv paths
    :header-rows: 1

    * - Platform
      - Module
      - Path
    * - ESP32-C2
      - ESP32C2-2MB (all ESP32-C2 (ESP8684) series with 2 MB flash)
      - :project_file:`module_config/module_esp32c2-2mb/at_customize.csv`
    * - ESP32-C2
      - ESP32C2-4MB (all ESP32-C2 (ESP8684) series with 4 MB flash)
      - :project_file:`module_config/module_esp32c2_default/at_customize.csv`

.. only:: esp32c3

  .. list-table:: at_customize.csv paths
    :header-rows: 1

    * - Platform
      - Module
      - Path
    * - ESP32-C3
      - MINI-1
      - :project_file:`module_config/module_esp32c3_default/at_customize.csv`

.. only:: esp32c6

  .. list-table:: at_customize.csv paths
    :header-rows: 1

    * - Platform
      - Module
      - Path
    * - ESP32-C6
      - ESP32C6-4MB (all ESP32-C6 series with 4 MB flash)
      - :project_file:`module_config/module_esp32c6_default/at_customize.csv`

.. only:: esp32s2

  .. list-table:: at_customize.csv paths
    :header-rows: 1

    * - Platform
      - Module
      - Path
    * - ESP32-S2
      - MINI (all ESP32-S2 series with 4 MB flash)
      - :project_file:`module_config/module_esp32s2_default/at_customize.csv`

Then, follow the rules below when modifying at_customize.csv.

- Do not change the ``Name`` and ``Type`` of the user partitions that have already been defined in it, while ``SubType``, ``Offset``, and ``Size`` can be changed.
- If you need to add a new user partition, please check if it has already been defined in the ESP-IDF (``esp_partition.h``) first.

    - If yes, you should keep the ``Type`` value the same as that of ESP-IDF.
    - If no, please set the ``Type`` to ``0x40``.
- A user partition's ``Name`` should not be longer than ``16`` bytes.
- The default size of the entire ``at_customize`` partition is defined in the partitions_at.csv table. Please do not exceed the range when adding new user partitions.

.. _generate-at-customize-bin:

Generate at_customize.bin
--------------------------

After having modified the at_customize.csv, you can either recompile the ESP-AT project to generate the at_customize.bin file, or use the python script ``gen_esp32part.py``.

If you use the script, execute the following command under the root directory of ESP-AT project and replace ``INPUT`` and ``OUTPUT``:

::

    python esp-idf/components/partition_table/gen_esp32part.py <INPUT> [OUTPUT]

- Replace ``INPUT`` with the path to at_customize.csv or the binary file to parse.
- Replace ``OUTPUT`` with the path to output converted binary or CSV file. Stdout will be used if omitted.

.. _flash-at-customize-bin-into-ESP:

Flash at_customize.bin into {IDF_TARGET_NAME} Device
----------------------------------------------------

Download the at_customize.bin into flash. Please refer to :ref:`flash-at-firmware-into-your-device` for how to flash bin files into {IDF_TARGET_NAME} device and the following table for the download address for your module.

.. only:: esp32

  .. list-table:: Download Address of at_customize.bin in Modules
    :header-rows: 1

    * - Platform
      - Module
      - Address
      - Size
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

  .. list-table:: Download Address of at_customize.bin in Modules
    :header-rows: 1

    * - Platform
      - Module
      - Address
      - Size
    * - ESP32-C2
      - ESP32C2-2MB (all ESP32-C2 (ESP8684) series with 2 MB flash)
      - 0x1A000
      - 0x26000
    * - ESP32-C2
      - ESP32C2-4MB (all ESP32-C2 (ESP8684) series with 4 MB flash)
      - {IDF_TARGET_AT_SECOND_PARTITION_ADDR}
      - 0x42000

.. only:: esp32c3

  .. list-table:: Download Address of at_customize.bin in Modules
    :header-rows: 1

    * - Platform
      - Module
      - Address
      - Size
    * - ESP32-C3
      - MINI-1
      - 0x1E000
      - 0x42000

.. only:: esp32c6

  .. list-table:: Download Address of at_customize.bin in Modules
    :header-rows: 1

    * - Platform
      - Module
      - Address
      - Size
    * - ESP32-C6
      - ESP32C6-4MB (all ESP32-C6 series with 4 MB flash)
      - 0x1E000
      - 0x42000

.. only:: esp32s2

  .. list-table:: Download Address of at_customize.bin in Modules
    :header-rows: 1

    * - Platform
      - Module
      - Address
      - Size
    * - ESP32-S2
      - MINI
      - 0x20000
      - 0xE0000

There are cases where at_customize.bin must be downloaded to flash in order to use certain AT commands:

- :ref:`cmd-SYSFLASH`
- :ref:`cmd-FS`
- ``SSL`` server relevant commands
- ``BLE`` server relevant commands

.. _customize-partition-example:

Example
-------

The section demonstrates how to add a 4 KB partition named ``test`` into the {IDF_TARGET_PRODUCT_NAME} module.

Firstly, find the at_customize.csv table for {IDF_TARGET_PRODUCT_NAME} and set the ``Name``, ``Type``, ``Subtype``, ``Offset``, and ``Size`` of the new partition:

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

Secondly, recompile the ESP-AT project, or execute the python script in the ESP-AT root directory to generate at_customize.bin.

::

    python esp-idf/components/partition_table/gen_esp32part.py -q ./module_config/module_{IDF_TARGET_PATH_NAME}_default/at_customize.csv at_customize.bin

Then, the at_customize.bin will be generated in the ESP-AT root directory.

Thirdly, download the at_customize.bin to flash.

Execute the following command under the root directory of ESP-AT project and replace ``PORT`` and ``BAUD``.

::

    python esp-idf/components/esptool_py/esptool/esptool.py -p PORT -b BAUD --before default_reset --after hard_reset --chip auto write_flash --flash_mode dio --flash_size detect --flash_freq 40m {IDF_TARGET_AT_SECOND_PARTITION_ADDR} ./at_customize.bin

- Replace ``PORT`` with your port name.
- Replace ``BAUD`` with the baud rate.
