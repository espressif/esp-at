Downloading Guide
=================

{IDF_TARGET_MODULE_NAME: default="undefined", esp32="ESP32-WROOM-32", esp32c2="ESP8684-MINI-1", esp32c3="ESP32-C3-MINI-1", esp32c6="ESP32-C6-MINI-1", esp32s2="ESP32-S2-MINI"}
{IDF_TARGET_FACTORY_BIN: default="undefined", esp32="ESP32-WROOM-32-AT-V3.2.0.0", esp32c2="ESP32C2-4MB-AT-V3.3.0.0", esp32c3="ESP32-C3-MINI-1-AT-V3.3.0.0", esp32c6="ESP32C6-4MB-AT-V4.0.0.0", esp32s2="ESP32-S2-MINI-AT-V3.4.0.0"}

:link_to_translation:`zh_CN:[中文]`

This Guide demonstrates how to download AT firmware and flash it into an {IDF_TARGET_MODULE_NAME} device by taking {IDF_TARGET_MODULE_NAME} as an example. The Guide is also applicable to other {IDF_TARGET_NAME} modules.

Before you start, please make sure you have already connected your hardware. For more details, see :doc:`Hardware_connection`.

For different series of modules, the commands supported by AT firmware are different. Please refer to :doc:`/Compile_and_Develop/esp-at_firmware_differences` for more details.

.. _download-at-firmware:

Download AT Firmware
---------------------

To download AT firmware to your computer, please do as follows:

- Navigate to :doc:`../AT_Binary_Lists/index`
- Find the firmware for your device
- Click the link to download it

Here, we download ``{IDF_TARGET_FACTORY_BIN}`` for {IDF_TARGET_MODULE_NAME}. The list below describes the structure of this firmware and what each bin file contains. Other AT firmware has similar structure and bin files.


.. code-block:: none

   .
   ├── at_customize.bin                 // secondary partition table
   ├── bootloader                       // bootloader
   │   └── bootloader.bin
   ├── customized_partitions            // AT customized binaries
         ├── mfg_nvs.csv                  // raw data of manufacturing nvs partition
   │   └── mfg_nvs.bin                  // manufacturing nvs partition binary
   ├── download.config                  // configuration of downloading
   ├── esp-at.bin                       // AT application binary
   ├── esp-at.elf
   ├── esp-at.map
   ├── factory                          // A combined bin for factory downloading
   │   └── factory_XXX.bin
   ├── flasher_args.json                // flasher arguments
   ├── ota_data_initial.bin             // ota data parameters
   ├── partition_table                  // primary partition table
   │   └── partition-table.bin
   └── sdkconfig                        // compilation configuration for AT firmware

The file ``download.config`` contains the configuration to flash the firmware into multiple addresses: 

.. only:: esp32

   .. code-block:: none

      --flash_mode dio --flash_freq 40m --flash_size 4MB
      0x1000 bootloader/bootloader.bin
      0x8000 partition_table/partition-table.bin
      0x10000 ota_data_initial.bin
      0x20000 at_customize.bin
      0x21000 customized_partitions/mfg_nvs.bin
      0x100000 esp-at.bin

.. only:: esp32c2

   .. code-block:: none

      --flash_mode dio --flash_freq 60m --flash_size 4MB
      0x0 bootloader/bootloader.bin
      0x8000 partition_table/partition-table.bin
      0xd000 ota_data_initial.bin
      0x1e000 at_customize.bin
      0x1f000 customized_partitions/mfg_nvs.bin
      0x60000 esp-at.bin

.. only:: esp32c3

   .. code-block:: none

      --flash_mode dio --flash_freq 40m --flash_size 4MB
      0x0 bootloader/bootloader.bin
      0x8000 partition_table/partition-table.bin
      0xd000 ota_data_initial.bin
      0x1e000 at_customize.bin
      0x1f000 customized_partitions/mfg_nvs.bin
      0x60000 esp-at.bin

.. only:: esp32c6

   .. code-block:: none

      --flash_mode dio --flash_freq 80m --flash_size 4MB
      0x0 bootloader/bootloader.bin
      0x8000 partition_table/partition-table.bin
      0xd000 ota_data_initial.bin
      0x1e000 at_customize.bin
      0x1f000 customized_partitions/mfg_nvs.bin
      0x60000 esp-at.bin

.. only:: esp32s2

   .. code-block:: none

      --flash_mode dio --flash_freq 80m --flash_size 4MB
      0x1000 bootloader/bootloader.bin
      0x8000 partition_table/partition-table.bin
      0x10000 ota_data_initial.bin
      0x20000 at_customize.bin
      0x21000 customized_partitions/mfg_nvs.bin
      0x100000 esp-at.bin

.. list::

   - ``--flash_mode dio`` means the firmware is compiled with flash DIO mode.
   :esp32 or esp32c3: - ``--flash_freq 40m`` means the firmware's flash frequency is 40 MHz.
   :esp32c2: - ``--flash_freq 60m`` means the firmware's flash frequency is 60 MHz.
   :esp32c6 or esp32s2: - ``--flash_freq 80m`` means the firmware's flash frequency is 80 MHz.
   - ``--flash_size 4MB`` means the firmware is using flash size 4 MB.
   :esp32 or esp32s2: - ``0x10000 ota_data_initial.bin`` means downloading ``ota_data_initial.bin`` into the address ``0x10000``.
   :esp32c2 or esp32c3 or esp32c6: - ``0xd000 ota_data_initial.bin`` means downloading ``ota_data_initial.bin`` into the address ``0xd000``.

.. _flash-at-firmware-into-your-device:

Flash AT Firmware into Your Device
-----------------------------------

Follow the instructions below for your operating system.

Windows
^^^^^^^^

Before starting to flash, you need to download `Flash Download Tools for Windows <https://www.espressif.com/en/support/download/other-tools>`_. For more details about the tools, please see the  ``doc`` folder in the zip folder.

- Open the {IDF_TARGET_NAME} Flash Download Tool.
- Select chipType. (Here, we select ``{IDF_TARGET_NAME}``.)
- Select a workMode according to your need. (Here, we select ``Developer Mode``.)
- Select a loadMode according to your need. (Here, we select ``uart``.)

.. figure:: ../../_static/get_started/download_guide/download_tool_{IDF_TARGET_PATH_NAME}.png
   :align: center
   :alt: Firmware Download Configurations
   :figclass: align-center

   Firmware Download Configurations

- Flash AT firmware into your device. You can select either of the two ways below.

   - To download one combined factory bin (namely, ``factory_XXX.bin`` in the ``build/factory`` directory) to address 0, select "DoNotChgBin" to use the default configuration of the factory bin.

     .. figure:: ../../_static/get_started/download_guide/download_one_bin_{IDF_TARGET_PATH_NAME}.png
        :align: center
        :scale: 70%
        :alt: Download to One Address

        Download to One Address (click to enlarge)

   - To download multiple bins separately to different addresses, set up the configurations according to the file ``download.config`` and do NOT select "DoNotChgBin".

     .. figure:: ../../_static/get_started/download_guide/download_multi_bin_{IDF_TARGET_PATH_NAME}.png
        :align: center
        :scale: 60%
        :alt: Download to Multiple Addresses

        Download to Multiple Addresses (click to enlarge)

In case of flashing issues, please verify what the COM port number of download interface of the {IDF_TARGET_NAME} board is and select it from "COM:" dropdown list. If you do not know the port number, you can refer to `Check port on Windows <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/get-started/establish-serial-connection.html#check-port-on-windows>`_ for details.

When you finish flashing, please `Check Whether AT Works`_.

Linux or macOS
^^^^^^^^^^^^^^^

Before you start to flash, you need to install `esptool.py <https://github.com/espressif/esptool>`_.

You can select either of the two ways below to flash AT firmware into your device.

- To download the bins separately into multiple addresses, enter the following command and replace ``PORTNAME`` and ``download.config``:

  .. code-block:: none

       esptool.py --chip auto --port PORTNAME --baud 115200 --before default_reset --after hard_reset write_flash -z download.config

  Replace ``PORTNAME`` with your port name. If you do not know it, you can refer to `Check port on Linux and macOS <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/get-started/establish-serial-connection.html#check-port-on-linux-and-macos>`_ for details.

  Replace ``download.config`` with the content inside the file.

  Below is the example command for {IDF_TARGET_MODULE_NAME}.

   .. only:: esp32

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x8000 partition_table/partition-table.bin 0x10000 ota_data_initial.bin 0x1000 bootloader/bootloader.bin 0x100000 esp-at.bin 0x20000 at_customize.bin 0x21000 customized_partitions/mfg_nvs.bin

   .. only:: esp32c2

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 60m --flash_size 4MB 0x0 bootloader/bootloader.bin 0x60000 esp-at.bin 0x8000 partition_table/partition-table.bin 0xd000 ota_data_initial.bin 0x1e000 at_customize.bin 0x1f000 customized_partitions/mfg_nvs.bin

   .. only:: esp32c3

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x8000 partition_table/partition-table.bin 0xd000 ota_data_initial.bin 0x0 bootloader/bootloader.bin 0x60000 esp-at.bin 0x1e000 at_customize.bin 0x1f000 customized_partitions/mfg_nvs.bin

   .. only:: esp32c6

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x8000 partition_table/partition-table.bin 0xd000 ota_data_initial.bin 0x0 bootloader/bootloader.bin 0x60000 esp-at.bin 0x1e000 at_customize.bin 0x1f000 customized_partitions/mfg_nvs.bin

   .. only:: esp32s2

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 bootloader/bootloader.bin 0x60000 esp-at.bin 0x8000 partition_table/partition-table.bin 0xd000 ota_data_initial.bin 0x1e000 at_customize.bin 0x1f000 customized_partitions/mfg_nvs.bin

- To download the bins together to one address, enter the following command and replace ``PORTNAME`` and ``FILEDIRECTORY``:

  .. code-block:: none

     esptool.py --chip auto --port PORTNAME --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 FILEDIRECTORY

  Replace ``PORTNAME`` with your port name. If you do not know it, you can refer to `Check port on Linux and macOS <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/get-started/establish-serial-connection.html#check-port-on-linux-and-macos>`_ for details.

  Replace ``FILEDIRECTORY`` with the file directory you would flash to the address ``0x0``. It is normally factory/XXX.bin.

  Below is the example command for {IDF_TARGET_MODULE_NAME}.

   .. only:: esp32

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 factory/factory_WROOM-32.bin

   .. only:: esp32c2

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 60m --flash_size 4MB 0x0 factory/factory_ESP32C2-4MB.bin

   .. only:: esp32c3

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 factory/factory_MINI-1.bin

   .. only:: esp32c6

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0 factory/factory_ESP32C6-4MB.bin

   .. only:: esp32s2

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0 factory/factory_MINI-1.bin

When you finish flashing, please `Check Whether AT Works`_.

.. _check-whether-at-works:

Check Whether AT Works
-----------------------

To check whether AT works, do as follows:

- Open a serial port tool, such as SecureCRT;
- Select the Port attached to "AT command/response" line (see :doc:`Hardware_connection` for details);
- Set Baudrate to 115200;
- Set Data Bits to 8;
- Set Parity to None;
- Set Stop Bits to 1;
- Set Flow Type to None;
- Enter the command "AT+GMR" with a new line (CR LF). 

If the response is OK as shown below, AT works.

.. only:: esp32c2

   .. code-block:: none

      AT+GMR
      AT version:3.3.0.0(3b13d04 - ESP32C2 - May  8 2024 08:21:45)
      SDK version:v5.0.6-dirty
      compile time(be332568):May  8 2024 08:50:59
      Bin version:v3.3.0.0(ESP32C2-4MB)

      OK

.. only:: esp32c3

   .. code-block:: none

      AT+GMR
      AT version:3.3.0.0(3b13d04 - ESP32C3 - May  8 2024 08:21:54)
      SDK version:v5.0.6-dirty
      compile time(be332568):May  8 2024 08:51:33
      Bin version:v3.3.0.0(MINI-1)

      OK

.. only:: esp32c6

   .. code-block:: none

      AT+GMR
      AT version:4.0.0.0(3fe3806 - ESP32C6 - Dec 29 2023 11:10:21)
      SDK version:v5.1.2-dirty
      compile time(89040be7):Jan  2 2024 05:53:07
      Bin version:v4.0.0.0(ESP32C6-4MB)

      OK

.. only:: esp32

   .. code-block:: none

      AT+GMR
      AT version:3.2.0.0(s-ec2dec2 - ESP32 - Jul 28 2023 07:05:28)
      SDK version:v5.0.2-376-g24b9d38a24-dirty
      compile time(6118fc22):Jul 28 2023 09:47:28
      Bin version:v3.2.0.0(WROOM-32)

      OK

.. only:: esp32s2

   .. code-block:: none

      AT+GMR
      AT version:3.4.0.0-dev(ca45add - ESP32S2 - May  9 2024 08:00:07)
      SDK version:v5.0.6-dirty
      compile time(877c7e69):May 10 2024 06:47:54
      Bin version:v3.4.0.0-dev(MINI)

      OK

Otherwise, you need to check your {IDF_TARGET_NAME} startup log in one of the following ways:

**Method 1:**

- Open a serial port tool, such as SecureCRT;
- Select the port attached to the "Download/Log output" line. For more information on this line, see :doc:`Hardware_connection`.
- Set Baudrate to 115200;
- Set Data Bits to 8;
- Set Parity to None;
- Set Stop Bits to 1;
- Set Flow Type to None;
- Press the RST key of the board directly. If it is like the log below, it means that ESP-AT firmware have been initalized correctly.

**Method 2:**

- Open two serial port tools, such as SecureCRT;
- In one serial port tool, select the port attached to the "AT command/response" line. In the other tool, select the port attached to the "Download/Log output" line. For more information on these lines, see :doc:`Hardware_connection`.
- Set Baudrate to 115200;
- Set Data Bits to 8;
- Set Parity to None;
- Set Stop Bits to 1;
- Set Flow Type to None;
- Enter the command :ref:`AT+RST <cmd-RST>` with a new line (CR LF) to the "AT command/response" line. If the serial log from the "Download/Output log" line is like the log below, it means that ESP-AT firmware have been initialized correctly.

.. only:: esp32

   {IDF_TARGET_NAME} startup log:

   .. code-block:: none

      rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
      configsip: 0, SPIWP:0xee
      clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
      mode:DIO, clock div:2
      load:0x3fff0030,len:5884
      ho 0 tail 12 room 4
      load:0x40078000,len:15844
      load:0x40080400,len:3560
      entry 0x40080604
      I (29) boot: ESP-IDF v5.0-541-g885e501d99-dirty 2nd stage bootloader
      I (29) boot: compile time 08:40:13
      I (29) boot: chip revision: v1.0
      I (34) boot.esp32: SPI Speed      : 40MHz
      I (38) boot.esp32: SPI Mode       : DIO
      I (43) boot.esp32: SPI Flash Size : 4MB
      I (47) boot: Enabling RNG early entropy source...
      I (53) boot: Partition Table:
      I (56) boot: ## Label            Usage          Type ST Offset   Length
      I (64) boot:  0 phy_init         RF data          01 01 0000f000 00001000
      I (71) boot:  1 otadata          OTA data         01 00 00010000 00002000
      I (78) boot:  2 nvs              WiFi data        01 02 00012000 0000e000
      I (86) boot:  3 at_customize     unknown          40 00 00020000 000e0000
      I (93) boot:  4 ota_0            OTA app          00 10 00100000 00180000
      I (101) boot:  5 ota_1            OTA app          00 11 00280000 00180000
      I (108) boot: End of partition table
      I (113) esp_image: segment 0: paddr=00100020 vaddr=3f400020 size=1a854h (108628) map
      I (161) esp_image: segment 1: paddr=0011a87c vaddr=3ff80063 size=00008h (     8) load
      I (161) esp_image: segment 2: paddr=0011a88c vaddr=3ffbdb60 size=04d5ch ( 19804) load
      I (174) esp_image: segment 3: paddr=0011f5f0 vaddr=40080000 size=00a28h (  2600) load
      I (176) esp_image: segment 4: paddr=00120020 vaddr=400d0020 size=11f5c0h (1177024) map
      I (609) esp_image: segment 5: paddr=0023f5e8 vaddr=40080a28 size=1e948h (125256) load
      I (660) esp_image: segment 6: paddr=0025df38 vaddr=400c0000 size=00064h (   100) load
      I (676) boot: Loaded app from partition at offset 0x100000
      I (676) boot: Disabling RNG early entropy source...
      no external 32k oscillator, disable it now.
      at param mode: 1
      AT cmd port:uart1 tx:17 rx:16 cts:15 rts:14 baudrate:115200
      module_name: WROOM-32
      max tx power=78, ret=0
      2.5.0

.. only:: esp32c2

   {IDF_TARGET_NAME} startup log:

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
      I (32) boot: ESP-IDF v5.0-dev-5949-g885e501d99-dirty 2nd stage bootloader
      I (32) boot: compile time 11:05:11
      I (32) boot: chip revision: v1.0
      I (36) boot.esp32c2: MMU Page Size  : 64K
      I (41) boot.esp32c2: SPI Speed      : 60MHz
      I (46) boot.esp32c2: SPI Mode       : DIO
      I (50) boot.esp32c2: SPI Flash Size : 4MB
      I (55) boot: Enabling RNG early entropy source...
      I (61) boot: Partition Table:
      I (64) boot: ## Label            Usage          Type ST Offset   Length
      I (71) boot:  0 otadata          OTA data         01 00 0000d000 00002000
      I (79) boot:  1 phy_init         RF data          01 01 0000f000 00001000
      I (86) boot:  2 nvs              WiFi data        01 02 00010000 0000e000
      I (94) boot:  3 at_customize     unknown          40 00 0001e000 00042000
      I (101) boot:  4 ota_0            OTA app          00 10 00060000 001d0000
      I (109) boot:  5 ota_1            OTA app          00 11 00230000 001d0000
      I (116) boot: End of partition table
      I (121) esp_image: segment 0: paddr=00060020 vaddr=3c0e0020 size=288c8h (166088) map
      I (167) esp_image: segment 1: paddr=000888f0 vaddr=3fca6010 size=02c18h ( 11288) load
      I (170) esp_image: segment 2: paddr=0008b510 vaddr=40380000 size=04b08h ( 19208) load
      I (178) esp_image: segment 3: paddr=00090020 vaddr=42000020 size=d444ch (869452) map
      I (378) esp_image: segment 4: paddr=00164474 vaddr=40384b08 size=01508h (  5384) load
      I (382) boot: Loaded app from partition at offset 0x60000
      I (383) boot: Disabling RNG early entropy source...
      at param mode: 1
      AT cmd port:uart1 tx:7 rx:6 cts:5 rts:4 baudrate:115200
      module_name: ESP32C2-4MB
      max tx power=78, ret=0
      3.0.0

.. only:: esp32c3

   {IDF_TARGET_NAME} startup log:

   .. code-block:: none

      ESP-ROM:esp32c3-api1-20210207
      Build:Feb  7 2021
      rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
      SPIWP:0xee
      mode:DIO, clock div:2
      load:0x3fcd5820,len:0x16b4
      load:0x403cc710,len:0x970
      load:0x403ce710,len:0x2e90
      entry 0x403cc710
      I (31) boot: ESP-IDF v5.0-541-g885e501d99-dirty 2nd stage bootloader
      I (31) boot: compile time 14:34:08
      I (32) boot: chip revision: v0.3
      I (35) boot.esp32c3: SPI Speed      : 40MHz
      I (40) boot.esp32c3: SPI Mode       : DIO
      I (45) boot.esp32c3: SPI Flash Size : 4MB
      I (49) boot: Enabling RNG early entropy source...
      I (55) boot: Partition Table:
      I (58) boot: ## Label            Usage          Type ST Offset   Length
      I (66) boot:  0 otadata          OTA data         01 00 0000d000 00002000
      I (73) boot:  1 phy_init         RF data          01 01 0000f000 00001000
      I (81) boot:  2 nvs              WiFi data        01 02 00010000 0000e000
      I (88) boot:  3 at_customize     unknown          40 00 0001e000 00042000
      I (95) boot:  4 ota_0            OTA app          00 10 00060000 001d0000
      I (103) boot:  5 ota_1            OTA app          00 11 00230000 001d0000
      I (110) boot: End of partition table
      I (115) esp_image: segment 0: paddr=00060020 vaddr=3c170020 size=3bd30h (245040) map
      I (175) esp_image: segment 1: paddr=0009bd58 vaddr=3fc95400 size=03884h ( 14468) load
      I (178) esp_image: segment 2: paddr=0009f5e4 vaddr=40380000 size=00a34h (  2612) load
      I (181) esp_image: segment 3: paddr=000a0020 vaddr=42000020 size=167a10h (1473040) map
      I (497) esp_image: segment 4: paddr=00207a38 vaddr=40380a34 size=1486ch ( 84076) load
      I (518) esp_image: segment 5: paddr=0021c2ac vaddr=50000000 size=00018h (    24) load
      I (525) boot: Loaded app from partition at offset 0x60000
      I (525) boot: Disabling RNG early entropy source...
      no external 32k oscillator, disable it now.
      at param mode: 1
      AT cmd port:uart1 tx:7 rx:6 cts:5 rts:4 baudrate:115200
      module_name: MINI-1
      max tx power=78, ret=0
      2.5.0

.. only:: esp32c6

   {IDF_TARGET_NAME} startup log:

   .. code-block:: none

      ESP-ROM:esp32c6-20220919
      Build:Sep 19 2022
      rst:0xc (SW_CPU),boot:0x6c (SPI_FAST_FLASH_BOOT)
      Saved PC:0x4001975a
      SPIWP:0xee
      mode:DIO, clock div:2
      load:0x4086c410,len:0xd50
      load:0x4086e610,len:0x2d74
      load:0x40875720,len:0x1800
      entry 0x4086c410
      I (27) boot: ESP-IDF v5.0-dev-9643-g4bc762621d-dirty 2nd stage bootloader
      I (27) boot: compile time Jul  5 2023 11:12:16
      I (29) boot: chip revision: v0.1
      I (32) boot.esp32c6: SPI Speed      : 40MHz
      I (37) boot.esp32c6: SPI Mode       : DIO
      I (41) boot.esp32c6: SPI Flash Size : 4MB
      I (46) boot: Enabling RNG early entropy source...
      I (52) boot: Partition Table:
      I (55) boot: ## Label            Usage          Type ST Offset   Length
      I (62) boot:  0 otadata          OTA data         01 00 0000d000 00002000
      I (70) boot:  1 phy_init         RF data          01 01 0000f000 00001000
      I (77) boot:  2 nvs              WiFi data        01 02 00010000 0000e000
      I (85) boot:  3 at_customize     unknown          40 00 0001e000 00042000
      I (92) boot:  4 ota_0            OTA app          00 10 00060000 001d0000
      I (100) boot:  5 ota_1            OTA app          00 11 00230000 001d0000
      I (107) boot: End of partition table
      I (112) esp_image: segment 0: paddr=00060020 vaddr=42140020 size=30628h (198184) map
      I (198) esp_image: segment 1: paddr=00090650 vaddr=40800000 size=0f9c8h ( 63944) load
      I (228) esp_image: segment 2: paddr=000a0020 vaddr=42000020 size=13c688h (1296008) map
      I (740) esp_image: segment 3: paddr=001dc6b0 vaddr=4080f9c8 size=05bf4h ( 23540) load
      I (752) esp_image: segment 4: paddr=001e22ac vaddr=408155c0 size=03c54h ( 15444) load
      I (760) esp_image: segment 5: paddr=001e5f08 vaddr=50000000 size=00068h (   104) load
      I (771) boot: Loaded app from partition at offset 0x60000
      I (772) boot: Disabling RNG early entropy source...
      no external 32k oscillator, disable it now.
      at param mode: 1
      AT cmd port:uart1 tx:7 rx:6 cts:5 rts:4 baudrate:115200
      module_name: ESP32C6-4MB
      max tx power=78, ret=0
      4.0.0

.. only:: esp32s2

   {IDF_TARGET_NAME} startup log:

   .. code-block:: none

      ESP-ROM:esp32s2-rc4-20191025
      Build:Oct 25 2019
      rst:0x1 (POWERON),boot:0x8 (SPI_FAST_FLASH_BOOT)
      SPIWP:0xee
      mode:DIO, clock div:1
      load:0x3ffe6108,len:0x17d4
      load:0x4004c000,len:0xa9c
      load:0x40050000,len:0x3204
      entry 0x4004c1b8
      I (21) boot: ESP-IDF v5.0.6-dirty 2nd stage bootloader
      I (21) boot: compile time 06:47:54
      I (21) boot: chip revision: v0.0
      I (24) boot.esp32s2: SPI Speed      : 80MHz
      I (29) boot.esp32s2: SPI Mode       : DIO
      I (34) boot.esp32s2: SPI Flash Size : 4MB
      I (39) boot: Enabling RNG early entropy source...
      I (44) boot: Partition Table:
      I (48) boot: ## Label            Usage          Type ST Offset   Length
      I (55) boot:  0 phy_init         RF data          01 01 0000f000 00001000
      I (62) boot:  1 otadata          OTA data         01 00 00010000 00002000
      I (70) boot:  2 nvs              WiFi data        01 02 00012000 0000e000
      I (77) boot:  3 at_customize     unknown          40 00 00020000 000e0000
      I (85) boot:  4 ota_0            OTA app          00 10 00100000 00180000
      I (92) boot:  5 ota_1            OTA app          00 11 00280000 00180000
      I (100) boot: End of partition table
      I (104) esp_image: segment 0: paddr=00100020 vaddr=3f000020 size=28958h (166232) map
      I (146) esp_image: segment 1: paddr=00128980 vaddr=3ff9e02c size=00004h (     4) load
      I (146) esp_image: segment 2: paddr=0012898c vaddr=3ffc2f70 size=036f4h ( 14068) load
      I (155) esp_image: segment 3: paddr=0012c088 vaddr=40022000 size=03f90h ( 16272) load
      I (164) esp_image: segment 4: paddr=00130020 vaddr=40080020 size=d2214h (860692) map
      I (340) esp_image: segment 5: paddr=0020223c vaddr=40025f90 size=0cfdch ( 53212) load
      I (354) esp_image: segment 6: paddr=0020f220 vaddr=40070000 size=0002ch (    44) load
      I (363) boot: Loaded app from partition at offset 0x100000
      I (363) boot: Disabling RNG early entropy source...
      at param mode: 1
      AT cmd port:uart1 tx:17 rx:21 cts:20 rts:19 baudrate:115200
      module_name: MINI
      max tx power=78, ret=0
      v3.4.0.0-dev
      negotiated phy mode: 3
