How to Customize Partitions
===========================

This document describes how to customize the partitions in your ESP device by modifying the at_customize.csv table provided by ESP-AT. There are two partition tables: the primary partition and the secondary partition table.

The primary partition table partitions_at.csv is for system usage, based on which the partitions_at.bin file is generated. If the primary partition table goes wrong, the system will fail to startup. Therefore, it is not recommended to modify the partitions_at.csv.

ESP-AT provides a secondary partition table at_customize.csv that you can customize to store self-defined blocks of data. It is based on the primary partition table.

To modify the partition in your ESP device, please follow the first three steps. The fourth section illustrates the three steps with an example.

- :ref:`modify-at-customize-csv`
- :ref:`generate-at-customize-bin`
- :ref:`flash-at-customize-bin-into-ESP`
- :ref:`customize-partition-example`

.. _modify-at-customize-csv:

Modify at_customize.csv
-----------------------

Find the at_customize.csv for your module with reference to the following table.

.. list-table:: at_customize.csv paths
   :header-rows: 1

   * - Platform
     - Module
     - Paths
   * - ESP32
     - - WROOM-32
       - PICO-D4
       - SOLO-1
       - MINI-1
     - :module_config:`module_esp32_default/at_customize.csv`
   * - ESP32
     - WROVER-32
     - :module_config:`module_wrover-32/at_customize.csv`
   * - ESP32
     - ESP32-D2WD
     - :module_config:`module_esp32-d2wd/at_customize.csv`
   * - ESP32
     - ESP32_QCLOUD
     - :module_config:`module_esp32_qcloud/at_customize.csv`
   * - ESP32-C3
     - MINI-1
     - :module_config:`module_esp32c3_default/at_customize.csv`
   * - ESP32-C3
     - ESP32C3_QCLOUD
     - :module_config:`module_esp32c3_qcloud/at_customize.csv`

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

- Replace ``INPUT`` with the path to at_customize.csv or binary file to parse.
- Replace ``OUTPUT`` with the path to output converted binary or CSV file. Stdout will be used if omitted.

.. _flash-at-customize-bin-into-ESP:

Flash at_customize.bin into ESP
-------------------------------

Download the at_customize.bin into flash. Please refer to :ref:`flash-at-firmware-into-your-device` for how to flash bin files into ESP and the following table for the download address for your module.

.. list-table:: at_customize.bin download address of modules
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
       - ESP32_QCLOUD
     - 0x20000
     - 0xE0000
   * - ESP32-C3
     - MINI-1
     - 0x1E000
     - 0x42000
   * - ESP32-C3
     - ESP32C3_QCLOUD
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

The section demonstrates how to add a 4 KB partition named ``test`` into the ESP32-WROOM-32 module.

Firstly, find the at_customize.csv table for ESP32-WROOM-32 and set the Name, Type, Subtype. Offset, and Size of the new partition:

::

    # Name,Type,SubType,Offset,Size
    ... ...
    test,0x40,15,0x3D000,4K
    fatfs,data,fat,0x70000,576K

Secondly, recompile the ESP-AT project, or execute the python script in the ESP-AT root directory to generate at_customize.bin.

::

    python esp-idf/components/partition_table/gen_esp32part.py -q ./module_config/module_esp32_default/at_customize.csv at_customize.bin

Then, the at_customize.bin will be generated in the ESP-AT root directory.

Thirdly, download the at_customize.bin to flash.

Execute the following command under the root directory of ESP-AT project and replace ``PORT`` and ``BAUD``.

::

    python esp-idf/components/esptool_py/esptool/esptool.py -p PORT -b BAUD --before default_reset --after hard_reset --chip auto  write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x20000 ./at_customize.bin

- Replace ``PORT`` with your port name.
- Replace ``BAUD`` with the baud rate.