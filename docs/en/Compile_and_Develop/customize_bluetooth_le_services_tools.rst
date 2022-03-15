Customize Bluetooth LE Services Tools
======================================

:link_to_translation:`zh_CN:[中文]`

ESP-AT provides python script ``BLEService.py`` to convert customized Bluetooth LE services into ``ble_data.bin``. For more information on customizing Bluetooth LE services, please refer to :doc:`How_to_customize_BLE_services`.

.. contents::
   :local:
   :depth: 1

Generate Bin File
-------------------------------------

Select one of the following ways to generate ``ble_data.bin``.

.. contents::
   :local:
   :depth: 1

Script Generation
^^^^^^^^^^^^^^^^^^^^

The path of ``BLEService.py`` is :project_file:`tools/BLEService.py`. You can get help information of the script through the ``-h`` option. You can also generate ``ble_data.bin`` directly through the following commands.

.. code-block:: none

    python <SCRIPT_PATH> [-t TARGET_FILE_NAME] <source_file>

- ``SCRIPT_PATH``: the path the script. If you're in "tools" folder of ESP-AT project, ``SCRIPT_PATH`` is ``BLEService.py``.
- ``TARGET_FILE_NAME``: the target file which you want to save the generated bin (absolute address or relative address of the target file); if not specified, it will generate ``ATBleService.bin`` in the same directory with ``source_file``.
- ``source_file``: the source file which you defines your Bluetooth LE GATT services (absolute address or relative address of the source file).

For example, you can execute the following directory to generate ``ble_data.bin`` in the tools directory:

.. code-block:: none

    python BLEService.py -t ble_data.bin ../components/customized_partitions/raw_data/ble_data/example.csv

Generation During Compilation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The storage path of Bluetooth LE GATT service files in ESP-AT is :project_file:`components/customized_partitions/raw_data/ble_data/example.csv`.

For how to customize Bluetooth LE services, you can refer to :doc:`How_to_customize_BLE_services`.

For how to compile ESP-AT project, you can refer to :doc:`How_to_clone_project_and_compile_it`.

Download or Update Bin File
-------------------------------------

The script ``BLEService.py`` is only responsible for converting ``example.csv`` file to bin file. You can download bin files to the corresponding flash partition in the following ways:

1. Download bin file with tools

  #. Windows

     Please download `Flash Download Tools for Windows <https://www.espressif.com/en/support/download/other-tools>`_.

     For more details about the Tools, please see ``readme.pdf`` or the ``doc`` folder in the zip folder.

  #. Linux or macOS

     Please use `esptool.py <https://github.com/espressif/esptool>`_.

     You can refer to :doc:`How_to_customize_BLE_services` for more details.

     .. code-block:: none

       esptool.py --chip auto --port PORTNAME --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB ADDRESS FILEDIRECTORY

     Replace ``PORTNAME`` with your port name. Replace ``ADDRESS`` with the the download address. Replace ``FILEDIRECTORY`` with the file directory of the bin.

2. Update bin file with commands

  #. :ref:`AT+SYSFLASH <cmd-SYSFLASH>`

     Taking ``{IDF_TARGET_NAME}`` module as an example, you can execute the following command to upgrade the ``ble_data`` partition. Please refer to :ref:`AT+SYSFLASH <cmd-SYSFLASH>` for more details.

     1. Query user partitions in flash

       Command:

       .. code-block:: none

         AT+SYSFLASH?

       Response:

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

     2. Erase ``ble_data`` sector

       Command:

       .. code-block:: none

         AT+SYSFLASH=0,"ble_data"

       Response:

       .. code-block:: none

         OK

     3. Update ``ble_data`` sector

       Command:

       .. code-block:: none

         AT+SYSFLASH=1,"ble_data",0,6487

       Response:

       .. code-block:: none

         >

       If the ``operator`` is ``write``, wrap return ``>`` after the write command, then you can send the data that you want to write. The length should be parameter ``<length>``. When the write operation is completed, the system will prompt the following information.

       .. code-block:: none

         OK

  #. :ref:`AT+CIUPDATE <cmd-UPDATE>`

     For example, you can execute the following command to upgrade the ``ble_data`` partition (The premise is that you must use the Wi-Fi function). Please refer to :ref:`AT+CIUPDATE <cmd-UPDATE>` for more details.

     .. Important::
       If you want to update the ``ble_data`` partition in this way, you must implement your own OTA device, please refer to :doc:`How_to_implement_OTA_update`.

     .. code-block:: none

       AT+CIUPDATE=1,"v2.2.0.0","ble_data"

.. note::

  You must ensure that the download address is correct, otherwise the ESP-AT firmware may not work. The simplest way to view the download address is to execute the command **AT+SYSFLASH?**.
