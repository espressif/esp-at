Downloading Guide
=================

This Guide demonstrates how to download AT firmware and flash it into an ESP device by taking ESP32-WROOM-32 as an example. The Guide is also applicable to other ESP modules.

Before you start, please make sure you have already connected your hardware. For more details, see :doc:`Hardware_connection`.

Download AT Firmware
---------------------

To download AT firmware to your computer, please do as follows:

- Navigate to :doc:`../AT_Binary_Lists/index`
- Find the firmware for your device
- Click the link to download it

Here, we download ``ESP32-WROOM-32_AT_Bin_V2.1`` for ESP32-WROOM-32. The list below describes the structure of this firmware and what each bin file contains. Other AT firmware has similar structure and bin files.

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
   ├── factory                          // A combined bin for factory downloading
   │   ├── factory_WROOM-32.bin
   │   └── factory_parameter.log
   ├── flasher_args.json                // flasher arguments
   ├── ota_data_initial.bin             // ota data parameters
   ├── partition_table                  // primary partition table
   │   └── partition-table.bin
   └── phy_init_data.bin                // phy parameters

The file ``download.config`` contains the configuration to flash the firmware into multiple addresses: 

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

- ``--flash_mode dio`` means the firmware is compiled with flash DIO mode.
- ``--flash_freq 40m`` means the firmware’s flash frequency is 40 MHz.
- ``--flash_size 4MB`` means the firmware is using flash size 4 MB.
- ``0x10000 ota_data_initial.bin`` means downloading ``ota_data_initial.bin`` into the address ``0x10000``.

Flash AT Firmware into Your Device
-----------------------------------

Follow the instructions below for your operating system.

Windows 
^^^^^^^^

Before starting to flash, you need to download `Flash Download Tools for Windows <https://www.espressif.com/en/support/download/other-tools>`_. For more details about the tools, please see ``readme.pdf`` or the  ``doc`` folder in the zip folder.

- Open the ESP Flash Download Tool.
- Select a mode according to your need. (Here, we select ``Developer Mode``.)

.. figure:: ../../_static/Download_tool.png
   :align: center
   :alt: Flash Download Tools Modes
   :figclass: align-center

   Flash Download Tools Modes

- Select your target chip. For example, choose “ESP8266 DownloadTool” for ESP8266 chip; choose “ESP32-S2 DownloadTool” for ESP32-S2 chip. (Here, we select ESP32 DownloadTool.)

.. figure:: ../../_static/esp32_wroom32_download_select.png
   :align: center
   :alt: Flash Download Tools Target Chips
   :figclass: align-center

   Flash Download Tools Target Chip

- Flash AT firmware into your device. You can select either of the two ways below.

   - To download one combined factory bin to address 0, select “DoNotChgBin” to use the default configuration of the factory bin.

     .. figure:: ../../_static/esp32_wroom32_download_one_bin.png
        :align: center
        :scale: 70%
        :alt: Download to One Address

        Download to One Address

   - To download multiple bins separately to different addresses, set up the configurations according to the file ``download.config`` and do NOT select “DoNotChgBin”.

     .. figure:: ../../_static/esp32_wroom32_download_multi_bin.png
        :align: center
        :scale: 60%
        :alt: Download to Multiple Addresses

        Download to Multiple Addresses

In case of flashing issues, please verify what the COM port number of download interface of the ESP board is and select it from "COM:" dropdown list. If you don't know the port number, you can refer to `Check port on Windows <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html#check-port-on-windows>`_ for details.

When you finish flashing, please `Check Whether AT works`_. 

Linux or macOS
^^^^^^^^^^^^^^^

Before you start to flash, you need to install `esptool.py <https://github.com/espressif/esptool>`_.

You can select either of the two ways below to flash AT firmware into your device.

- To download the bins separately into multiple addresses, enter the following command and replace ``PORTNAME`` and ``download.config``:

  .. code-block:: none

       esptool.py --chip auto --port PORTNAME --baud 115200 --before default_reset --after hard_reset write_flash -z download.config

  Replace ``PORTNAME`` with your port name. If you don't know it, you can refer to `Check port on Linux and macOS <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html#check-port-on-linux-and-macos>`_ for details.

  Replace ``download.config`` with the content inside the file.

  Below is the example command for ESP32-WROOM-32.

  .. code-block:: none

        esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x8000 partition_table/partition-table.bin 0x10000 ota_data_initial.bin 0xf000 phy_init_data.bin 0x1000 bootloader/bootloader.bin 0x100000 esp-at.bin 0x20000 at_customize.bin 0x24000 customized_partitions/server_cert.bin 0x39000 customized_partitions/mqtt_key.bin 0x26000 customized_partitions/server_key.bin 0x28000 customized_partitions/server_ca.bin 0x2e000 customized_partitions/client_ca.bin 0x30000 customized_partitions/factory_param.bin 0x21000 customized_partitions/ble_data.bin 0x3B000 customized_partitions/mqtt_ca.bin 0x37000 customized_partitions/mqtt_cert.bin 0x2a000 customized_partitions/client_cert.bin 0x2c000 customized_partitions/client_key.bin

- To download the bins together to one address, enter the following command and replace ``PORTNAME`` and ``FILEDIRECTORY``:

  .. code-block:: none

        esptool.py --chip auto --port PORTNAME --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 FILEDIRECTORY

  Replace ``PORTNAME`` with your port name. If you don't know it, you can refer to `Check port on Linux and macOS <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html#check-port-on-linux-and-macos>`_ for details.

  Replace ``FILEDIRECTORY`` with the file directory you would flash to the address ``0x0``. It is normally factory/XXX.bin.

  Below is the example command for ESP32-WROOM-32.

  .. code-block:: none

        esptool.py --chip auto --port /dev/tty.usbserial-0001 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 factory/factory_WROOM-32.bin

When you finish flashing, please `Check Whether AT works`_.

Check Whether AT works
-----------------------
To check whether AT works, do as follows:

- Open a serial port tool, such as SecureCRT;
- Select the Port attached to "AT command/response" line (see :doc:`Hardware_connection` for details);
- Set Baudrate to 115200;
- Set Data Bits to 8;
- Set Parity to None;
- Set Stop Bits to 1;
- Set Flow Type to None;
- Enter the command “AT+GMR” with a new line (CR LF). 

If the response is OK as the picture below shows, it means that AT works.

.. figure:: ../../_static/esp32_wroom32_version.png
   :align: center
   :alt: Response from AT
   :figclass: align-center

   Response from AT