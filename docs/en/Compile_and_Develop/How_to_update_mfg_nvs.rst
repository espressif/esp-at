How to Update mfg_nvs Partition
=================================

:link_to_translation:`zh_CN:[中文]`

.. _mfg-nvs-intro:

mfg_nvs Partition
-----------------

mfg_nvs (:term:`manufacturing nvs`) partition is defined in ``esp-at/module_config/{module_name}/at_customize.csv`` file. This partition stores the following configurations of factory firmware:

.. list::

  - Factory parameter configurations (Wi-Fi configurations, UART configurations, module configurations): Please refer to :ref:`factory-param-intro`.
  - PKI configurations (various certificate and key configurations): Please refer to :ref:`factory-pki-intro`.
  :esp32 or esp32c2 or esp32c3 or esp32c6: - GATTS configuration (Bluetooth LE services): Please refer to :ref:`factory-gatts-intro`.

When you need to modify the factory parameter configurations, PKI configurations, or GATTS configurations, you can recompile the ESP-AT project to generate a new mfg_nvs.bin file; Alternatively, through :doc:`../Compile_and_Develop/tools_at_py` modify the firmware to generate a new firmware for your module. This article introduces the former.

.. _mfg-nvs-generate:

Generate mfg_nvs.bin
--------------------

After the configuration modifications are completed, :ref:`re-build the project <esp-at-project-build>`. During the compilation stage, the :component_file:`mfg_nvs.py <customized_partitions/generation_tools/mfg_nvs.py>` script is automatically used to generate the ``build/customized_partitions/mfg_nvs.csv`` file (which contains the namespace, key, type, and value information for all configurations). Then, the ``nvs_partition_gen.py`` script in esp-idf is used to generate the ``build/customized_partitions/mfg_nvs.bin`` file (which represents the structure of the NVS library (`NVS flash API reference <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-reference/storage/nvs_flash.html>`_)). Then, you can download the mfg_nvs.bin file to the device.

.. _mfg-nvs-download:

Download mfg_nvs.bin
^^^^^^^^^^^^^^^^^^^^^

You can download the mfg_nvs.bin in one of the following ways.

- Download the recompiled ESP-AT firmware. See :ref:`esp-at-project-flash` for more information.
- Download mfg_nvs.bin only. This way only updates the mfg_nvs area in the device.

  - Windows

    Please download the Windows `Flash Download Tool <https://www.espressif.com/en/support/download/other-tools>`_. Refer to the ``readme.pdf`` or the documentation in the ``doc`` directory for instructions. Download the ``build/customized_partitions/mfg_nvs.bin`` file to {IDF_TARGET_NAME}. You can use the :ref:`AT+SYSFLASH? <cmd-SYSFLASH>` command to query the download address of mfg_nvs.bin.

  - Linux or macOS

    Please use `esptool.py <https://github.com/espressif/esptool>`_.

    You can execute the following command in the ESP-AT root directory to download the mfg_nvs.bin file.

    .. code-block:: none

        esptool.py --chip auto --port PORTNAME --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB ADDRESS mfg_nvs.bin

    Replace ``PORTNAME`` with your serial port name. Replace ``ADDRESS`` with the address for downloading the mfg_nvs.bin file. You can use the :ref:`AT+SYSFLASH? <cmd-SYSFLASH>` command to query the download address.

- MCU sends the :ref:`AT+SYSFLASH <cmd-SYSFLASH>` command to update the mfg_nvs partition of {IDF_TARGET_NAME}.

  .. code-block:: none

    # Erase the mfg_nvs partition
    AT+SYSFLASH=0,"mfg_nvs",0,MFG_NVS_SIZE

    # Write the mfg_nvs.bin file
    AT+SYSFLASH=1,"mfg_nvs",0,MFG_NVS_SIZE

  Replace ``MFG_NVS_SIZE`` with the size of the downloaded mfg_nvs.bin file. Different modules have different partition sizes. You can use the :ref:`AT+SYSFLASH? <cmd-SYSFLASH>` command to query the partition size.
