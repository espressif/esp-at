Downloading Guide
=================

{IDF_TARGET_MODULE_NAME: default="undefined", esp32="ESP32-WROOM-32", esp32c2="ESP8684-MINI-1", esp32c3="ESP32-C3-MINI-1"}
{IDF_TARGET_FACTORY_BIN: default="undefined", esp32="ESP32-WROOM-32_AT_Bin_V2.2.0.0", esp32c2="ESP32C2-4MB_AT_Bin_V2.5.0.0", esp32c3="ESP32-C3-MINI-1_AT_Bin_V2.3.0.0"}

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

.. only:: esp32

   .. code-block:: none

      .
      ├── at_customize.bin                 // secondary partition table
      ├── bootloader                       // bootloader
      │   └── bootloader.bin
      ├── customized_partitions            // AT customized binaries
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
      ├── download.config                  // configuration of downloading
      ├── esp-at.bin                       // AT application binary
      ├── esp-at.elf
      ├── esp-at.map
      ├── factory                          // A combined bin for factory downloading
      │   ├── factory_WROOM-32.bin
      │   └── factory_parameter.log
      ├── flasher_args.json                // flasher arguments
      ├── ota_data_initial.bin             // ota data parameters
      ├── partition_table                  // primary partition table
      │   └── partition-table.bin
      ├── phy_init_data.bin                // phy parameters
      └── sdkconfig                        // compilation configuration for AT firmware

.. only:: esp32c3 or esp32c2

   .. code-block:: none

      .
      ├── at_customize.bin                 // secondary partition table
      ├── bootloader                       // bootloader
      │   └── bootloader.bin
      ├── customized_partitions            // AT customized binaries
      │   ├── ble_data.bin
      │   ├── client_ca.bin
      │   ├── client_cert.bin
      │   ├── client_key.bin
      │   ├── factory_param.bin
      │   ├── factory_param_MINI-1.bin
      │   ├── mqtt_ca.bin
      │   ├── mqtt_cert.bin
      │   ├── mqtt_key.bin
      │   ├── server_ca.bin
      │   ├── server_cert.bin
      │   └── server_key.bin
      ├── download.config                  // configuration of downloading
      ├── esp-at.bin                       // AT application binary
      ├── esp-at.elf
      ├── esp-at.map
      ├── factory                          // A combined bin for factory downloading
      │   ├── factory_MINI-1.bin
      │   └── factory_parameter.log
      ├── flasher_args.json                // flasher arguments
      ├── ota_data_initial.bin             // ota data parameters
      ├── partition_table                  // primary partition table
      │   └── partition-table.bin
      ├── phy_init_data.bin                // phy parameters
      └── sdkconfig                        // compilation configuration for AT firmware

The file ``download.config`` contains the configuration to flash the firmware into multiple addresses: 

.. only:: esp32

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

.. only:: esp32c2

   .. code-block:: none

      --flash_mode dio --flash_freq 60m --flash_size 4MB
      0x0 bootloader/bootloader.bin
      0x60000 esp-at.bin
      0x8000 partition_table/partition-table.bin
      0xd000 ota_data_initial.bin
      0xf000 phy_init_data.bin
      0x1e000 at_customize.bin
      0x1F000 customized_partitions/server_cert.bin
      0x21000 customized_partitions/server_key.bin
      0x23000 customized_partitions/server_ca.bin
      0x25000 customized_partitions/client_cert.bin
      0x27000 customized_partitions/client_key.bin
      0x29000 customized_partitions/client_ca.bin
      0x32000 customized_partitions/mqtt_cert.bin
      0x34000 customized_partitions/mqtt_key.bin
      0x36000 customized_partitions/mqtt_ca.bin
      0x2B000 customized_partitions/factory_param.bin

.. only:: esp32c3

   .. code-block:: none

      --flash_mode dio --flash_freq 40m --flash_size 4MB
      0x8000 partition_table/partition-table.bin
      0xd000 ota_data_initial.bin
      0xf000 phy_init_data.bin
      0x0 bootloader/bootloader.bin
      0x60000 esp-at.bin
      0x1e000 at_customize.bin
      0x1F000 customized_partitions/ble_data.bin
      0x3a000 customized_partitions/mqtt_key.bin
      0x27000 customized_partitions/server_key.bin
      0x3c000 customized_partitions/mqtt_ca.bin
      0x2d000 customized_partitions/client_key.bin
      0x2b000 customized_partitions/client_cert.bin
      0x31000 customized_partitions/factory_param.bin
      0x2f000 customized_partitions/client_ca.bin
      0x38000 customized_partitions/mqtt_cert.bin
      0x29000 customized_partitions/server_ca.bin
      0x25000 customized_partitions/server_cert.bin

.. list::

   - ``--flash_mode dio`` means the firmware is compiled with flash DIO mode.
   :esp32 or esp32c3: - ``--flash_freq 40m`` means the firmware's flash frequency is 40 MHz.
   :esp32c2: - ``--flash_freq 60m`` means the firmware's flash frequency is 60 MHz.
   - ``--flash_size 4MB`` means the firmware is using flash size 4 MB.
   :esp32: - ``0x10000 ota_data_initial.bin`` means downloading ``ota_data_initial.bin`` into the address ``0x10000``.
   :esp32c2 or esp32c3: - ``0xd000 ota_data_initial.bin`` means downloading ``ota_data_initial.bin`` into the address ``0xd000``.

.. _flash-at-firmware-into-your-device:

Flash AT Firmware into Your Device
-----------------------------------

Follow the instructions below for your operating system.

Windows
^^^^^^^^

Before starting to flash, you need to download `Flash Download Tools for Windows <https://www.espressif.com/en/support/download/other-tools>`_. For more details about the tools, please see the  ``doc`` folder in the zip folder.

- Open the {IDF_TARGET_NAME} Flash Download Tool.
- Select chipType. (Here, we select ``{IDF_TARGET_CFG_PREFIX}``.)
- Select a workMode according to your need. (Here, we select ``Developer Mode``.)
- Select a loadMode according to your need. (Here, we select ``uart``.)

.. figure:: ../../_static/download_tool_{IDF_TARGET_PATH_NAME}.png
   :align: center
   :alt: Firmware Download Configurations
   :figclass: align-center

   Firmware Download Configurations

- Flash AT firmware into your device. You can select either of the two ways below.

   - To download one combined factory bin to address 0, select "DoNotChgBin" to use the default configuration of the factory bin.

     .. figure:: ../../_static/download_one_bin_{IDF_TARGET_PATH_NAME}.png
        :align: center
        :scale: 70%
        :alt: Download to One Address

        Download to One Address (click to enlarge)

   - To download multiple bins separately to different addresses, set up the configurations according to the file ``download.config`` and do NOT select "DoNotChgBin".

     .. figure:: ../../_static/download_multi_bin_{IDF_TARGET_PATH_NAME}.png
        :align: center
        :scale: 60%
        :alt: Download to Multiple Addresses

        Download to Multiple Addresses (click to enlarge)

In case of flashing issues, please verify what the COM port number of download interface of the {IDF_TARGET_NAME} board is and select it from "COM:" dropdown list. If you don't know the port number, you can refer to `Check port on Windows <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/get-started/establish-serial-connection.html#check-port-on-windows>`_ for details.

When you finish flashing, please `Check Whether AT Works`_.

Linux or macOS
^^^^^^^^^^^^^^^

Before you start to flash, you need to install `esptool.py <https://github.com/espressif/esptool>`_.

You can select either of the two ways below to flash AT firmware into your device.

- To download the bins separately into multiple addresses, enter the following command and replace ``PORTNAME`` and ``download.config``:

  .. code-block:: none

       esptool.py --chip auto --port PORTNAME --baud 115200 --before default_reset --after hard_reset write_flash -z download.config

  Replace ``PORTNAME`` with your port name. If you don't know it, you can refer to `Check port on Linux and macOS <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/get-started/establish-serial-connection.html#check-port-on-linux-and-macos>`_ for details.

  Replace ``download.config`` with the content inside the file.

  Below is the example command for {IDF_TARGET_MODULE_NAME}.

   .. only:: esp32

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x8000 partition_table/partition-table.bin 0x10000 ota_data_initial.bin 0xf000 phy_init_data.bin 0x1000 bootloader/bootloader.bin 0x100000 esp-at.bin 0x20000 at_customize.bin 0x24000 customized_partitions/server_cert.bin 0x39000 customized_partitions/mqtt_key.bin 0x26000 customized_partitions/server_key.bin 0x28000 customized_partitions/server_ca.bin 0x2e000 customized_partitions/client_ca.bin 0x30000 customized_partitions/factory_param.bin 0x21000 customized_partitions/ble_data.bin 0x3B000 customized_partitions/mqtt_ca.bin 0x37000 customized_partitions/mqtt_cert.bin 0x2a000 customized_partitions/client_cert.bin 0x2c000 customized_partitions/client_key.bin

   .. only:: esp32c2

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 60m --flash_size 4MB 0x0 bootloader/bootloader.bin 0x60000 esp-at.bin 0x8000 partition_table/partition-table.bin 0xd000 ota_data_initial.bin 0xf000 phy_init_data.bin 0x1e000 at_customize.bin 0x1F000 customized_partitions/server_cert.bin 0x21000 customized_partitions/server_key.bin 0x23000 customized_partitions/server_ca.bin 0x25000 customized_partitions/client_cert.bin 0x27000 customized_partitions/client_key.bin 0x29000 customized_partitions/client_ca.bin 0x32000 customized_partitions/mqtt_cert.bin 0x34000 customized_partitions/mqtt_key.bin 0x36000 customized_partitions/mqtt_ca.bin 0x2B000 customized_partitions/factory_param.bin

   .. only:: esp32c3

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x8000 partition_table/partition-table.bin 0xd000 ota_data_initial.bin 0xf000 phy_init_data.bin 0x0 bootloader/bootloader.bin 0x60000 esp-at.bin 0x1e000 at_customize.bin 0x1F000 customized_partitions/ble_data.bin 0x3a000 customized_partitions/mqtt_key.bin 0x27000 customized_partitions/server_key.bin 0x3c000 customized_partitions/mqtt_ca.bin 0x2d000 customized_partitions/client_key.bin 0x2b000 customized_partitions/client_cert.bin 0x31000 customized_partitions/factory_param.bin 0x2f000 customized_partitions/client_ca.bin 0x38000 customized_partitions/mqtt_cert.bin 0x29000 customized_partitions/server_ca.bin 0x25000 customized_partitions/server_cert.bin

- To download the bins together to one address, enter the following command and replace ``PORTNAME`` and ``FILEDIRECTORY``:

  .. code-block:: none

        esptool.py --chip auto --port PORTNAME --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 FILEDIRECTORY

  Replace ``PORTNAME`` with your port name. If you don't know it, you can refer to `Check port on Linux and macOS <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/get-started/establish-serial-connection.html#check-port-on-linux-and-macos>`_ for details.

  Replace ``FILEDIRECTORY`` with the file directory you would flash to the address ``0x0``. It is normally factory/XXX.bin.

  Below is the example command for {IDF_TARGET_MODULE_NAME}.

   .. only:: esp32

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 factory/factory_WROOM-32.bin

   .. only:: esp32c2

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 60m --flash_size 4MB 0x0 factory/factory_MINI-1.bin

   .. only:: esp32c3

      .. code-block:: none

         esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 factory/factory_MINI-1.bin

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

If the response is OK as the picture below shows, AT works.

.. figure:: ../../_static/at_version_{IDF_TARGET_PATH_NAME}.png
   :align: center
   :alt: Response from AT
   :figclass: align-center

   Response from AT

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

      ets Jun  8 2016 00:22:57

      rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
      configsip: 0, SPIWP:0xee
      clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
      mode:DIO, clock div:2
      load:0x3fff0030,len:7168
      load:0x40078000,len:13200
      load:0x40080400,len:4564
      0x40080400: _init at ??:?

      entry 0x400806f4
      I (26) boot: ESP-IDF v4.2.2-76-gefa6eca 2nd stage bootloader
      I (26) boot: compile time 11:54:30
      I (26) boot: chip revision: 1
      I (30) boot_comm: chip revision: 1, min. bootloader chip revision: 0
      I (37) boot.esp32: SPI Speed      : 40MHz
      I (42) boot.esp32: SPI Mode       : DIO
      I (46) boot.esp32: SPI Flash Size : 4MB
      I (51) boot: Enabling RNG early entropy source...
      I (56) boot: Partition Table:
      I (60) boot: ## Label            Usage          Type ST Offset   Length
      I (67) boot:  0 phy_init         RF data          01 01 0000f000 00001000
      I (75) boot:  1 otadata          OTA data         01 00 00010000 00002000
      I (82) boot:  2 nvs              WiFi data        01 02 00012000 0000e000
      I (90) boot:  3 at_customize     unknown          40 00 00020000 000e0000
      I (97) boot:  4 ota_0            OTA app          00 10 00100000 00180000
      I (105) boot:  5 ota_1            OTA app          00 11 00280000 00180000
      I (112) boot: End of partition table
      I (117) boot_comm: chip revision: 1, min. application chip revision: 0
      I (124) esp_image: segment 0: paddr=0x00100020 vaddr=0x3f400020 size=0x285f8 (165368) map
      I (196) esp_image: segment 1: paddr=0x00128620 vaddr=0x3ffbdb60 size=0x03934 ( 14644) load
      I (202) esp_image: segment 2: paddr=0x0012bf5c vaddr=0x40080000 size=0x040bc ( 16572) load
      I (210) esp_image: segment 3: paddr=0x00130020 vaddr=0x400d0020 size=0x109f40 (1089344) map
      I (626) esp_image: segment 4: paddr=0x00239f68 vaddr=0x400840bc size=0x1aa04 (109060) load
      I (674) esp_image: segment 5: paddr=0x00254974 vaddr=0x400c0000 size=0x00064 (   100) load
      I (691) boot: Loaded app from partition at offset 0x100000
      I (691) boot: Disabling RNG early entropy source...
      module_name:WROOM-32
      max tx power=78,ret=0
      2.2.0

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
      I (21) boot: ESP-IDF v5.0-dev-3424-gbb23d783c0 2nd stage bootloader
      I (21) boot: compile time 19:44:11
      I (21) boot: chip revision: 0
      I (24) boot.esp32c2: MMU Page Size  : 64K
      I (29) boot.esp32c2: SPI Speed      : 60MHz
      I (34) boot.esp32c2: SPI Mode       : DIO
      I (38) boot.esp32c2: SPI Flash Size : 4MB
      I (43) boot: Enabling RNG early entropy source...
      I (49) boot: Partition Table:
      I (52) boot: ## Label            Usage          Type ST Offset   Length
      I (59) boot:  0 otadata          OTA data         01 00 0000d000 00002000
      I (67) boot:  1 phy_init         RF data          01 01 0000f000 00001000
      I (74) boot:  2 nvs              WiFi data        01 02 00010000 0000e000
      I (82) boot:  3 at_customize     unknown          40 00 0001e000 00042000
      I (89) boot:  4 ota_0            OTA app          00 10 00060000 001d0000
      I (97) boot:  5 ota_1            OTA app          00 11 00230000 001d0000
      I (104) boot: End of partition table
      I (108) esp_image: segment 0: paddr=00060020 vaddr=3c0d0020 size=279d8h (162264) map
      I (153) esp_image: segment 1: paddr=00087a00 vaddr=3fcab2b0 size=018a0h (  6304) load
      I (155) esp_image: segment 2: paddr=000892a8 vaddr=40380000 size=06d70h ( 28016) load
      I (166) esp_image: segment 3: paddr=00090020 vaddr=42000020 size=c10f8h (790776) map
      I (344) esp_image: segment 4: paddr=00151120 vaddr=40386d70 size=04534h ( 17716) load
      I (353) boot: Loaded app from partition at offset 0x60000
      I (353) boot: Disabling RNG early entropy source...
      module_name:MINI-1
      max tx power=78,ret=0
      2.5.0

.. only:: esp32c3

   {IDF_TARGET_NAME} startup log:

   .. code-block:: none

      ESP-ROM:esp32c3-api1-20210207
      Build:Feb  7 2021
      rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
      SPIWP:0xee
      mode:DIO, clock div:2
      load:0x3fcd6100,len:0x1948
      load:0x403ce000,len:0x8dc
      load:0x403d0000,len:0x2de0
      entry 0x403ce000
      I (31) boot: ESP-IDF v4.3.1-354-g98d34e5 2nd stage bootloader
      I (32) boot: compile time 11:21:56
      I (32) boot: chip revision: 3
      I (34) boot.esp32c3: SPI Speed      : 40MHz
      I (39) boot.esp32c3: SPI Mode       : DIO
      I (44) boot.esp32c3: SPI Flash Size : 4MB
      I (49) boot: Enabling RNG early entropy source...
      I (54) boot: Partition Table:
      I (58) boot: ## Label            Usage          Type ST Offset   Length
      I (65) boot:  0 otadata          OTA data         01 00 0000d000 00002000
      I (72) boot:  1 phy_init         RF data          01 01 0000f000 00001000
      I (80) boot:  2 nvs              WiFi data        01 02 00010000 0000e000
      I (87) boot:  3 at_customize     unknown          40 00 0001e000 00042000
      I (95) boot:  4 ota_0            OTA app          00 10 00060000 001d0000
      I (102) boot:  5 ota_1            OTA app          00 11 00230000 001d0000
      I (110) boot: End of partition table
      I (114) esp_image: segment 0: paddr=00060020 vaddr=3c140020 size=29060h (168032) map
      I (158) esp_image: segment 1: paddr=00089088 vaddr=3fc90800 size=03dfch ( 15868) load
      I (162) esp_image: segment 2: paddr=0008ce8c vaddr=40380000 size=0318ch ( 12684) load
      I (167) esp_image: segment 3: paddr=00090020 vaddr=42000020 size=130764h (1247076) map
      I (433) esp_image: segment 4: paddr=001c078c vaddr=4038318c size=0d588h ( 54664) load
      I (446) esp_image: segment 5: paddr=001cdd1c vaddr=50000000 size=00004h (     4) load
      I (447) esp_image: segment 6: paddr=001cdd28 vaddr=50000008 size=00010h (    16) load
      I (457) boot: Loaded app from partition at offset 0x60000
      I (458) boot: Disabling RNG early entropy source...
      module_name:MINI-1
      max tx power=78,ret=0
      2.3.0
