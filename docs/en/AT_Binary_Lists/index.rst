AT Binary Lists
=================

:link_to_translation:`zh_CN:[中文]`

.. toctree::
  :hidden:
  :maxdepth: 1

  {IDF_TARGET_NAME} AT Released Firmware <esp_at_binaries>

This document covers the following sections:

.. list::

  - :doc:`Download {IDF_TARGET_NAME} AT Released Firmware <esp_at_binaries>`
  - :ref:`brief-intro-firmware`: What binary files the AT firmware contains and their functions

.. note::
  To download AT firmware for other chip series, please go to the drop-down list on the upper left corner of this page and select a chip series to navigate to the documentation of that chip for downloading.

.. _brief-intro-firmware:

Brief Introduction to AT Firmware
----------------------------------

The ESP-AT firmware package contains several binary files for specific functionalities:

.. code-block:: none

  build
  ├── at_customize.bin        // Secondary partition table (user partition table, listing the start address and size of the mfg_nvs partition and fatfs partition)
  ├── bootloader
  │   └── bootloader.bin      // Bootloader
  ├── customized_partitions
  │   └── mfg_nvs.bin         // Factory configuration parameters, parameter values are listed in the mfg_nvs.csv file in the same directory
  ├── esp-at.bin              // AT application firmware
  ├── factory
  │   └── factory_xxx.bin     // Collection of binary files for specific functionalities. You can burn only this file to the flash space with a starting address of 0, or burn several binary files to the flash space corresponding to the starting address according to the information in the download.config file.
  ├── partition_table
  │   └── partition-table.bin // Primary partition table (system partition table)
  └── ota_data_initial.bin    // OTA data initialization file
