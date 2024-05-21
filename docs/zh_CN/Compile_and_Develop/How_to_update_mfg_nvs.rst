如何更新 mfg_nvs 分区
=================================

:link_to_translation:`en:[English]`

.. _mfg-nvs-intro:

mfg_nvs 分区介绍
--------------------------------

mfg_nvs (:term:`manufacturing nvs`) 分区定义在 ``esp-at/module_config/{module_name}/at_customize.csv`` 文件中。该分区存储了出厂固件的下列配置：

.. list::

    - 出厂参数配置 （Wi-Fi 配置、UART 配置、模组配置）：请参考 :ref:`factory-param-intro`。
    - PKI 配置（各类证书和密钥配置）：请参考 :ref:`factory-pki-intro`。
    :esp32 or esp32c2 or esp32c3 or esp32c6: - GATTS 配置（低功耗蓝牙服务）：请参考 :ref:`factory-gatts-intro`。

当您需要修改出厂参数配置、PKI 配置或者 GATTS 配置时，您可以重新编译 ESP-AT 工程，生成新的 mfg_nvs.bin 文件；或者通过 :doc:`../Compile_and_Develop/tools_at_py` 修改固件，为您的模组生成新的固件。本文介绍前者。

.. _mfg-nvs-generate:

生成 mfg_nvs.bin
--------------------------------

配置修改完成后，:ref:`重新编译 ESP-AT 工程 <esp-at-project-build>`。在编译阶段会自动通过 :component_file:`mfg_nvs.py <customized_partitions/generation_tools/mfg_nvs.py>` 先生成 ``build/customized_partitions/mfg_nvs.csv`` 文件（包含了所有配置的命名空间、键、类型、和键值信息），再由 ``esp-idf`` 中的 ``nvs_partition_gen.py`` 脚本生成 ``build/customized_partitions/mfg_nvs.bin`` 文件（`NVS 库的结构 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-reference/storage/nvs_flash.html>`_）。

.. _mfg-nvs-download:

下载 mfg_nvs.bin
--------------------------------

可采用以下任意一种方式下载 mfg_nvs.bin 文件。

- 下载重新编译过的 ESP-AT 固件，详情请见 :ref:`esp-at-project-flash`。
- 仅下载 mfg_nvs.bin，这种方法只更新 {IDF_TARGET_NAME} 中的 mfg_nvs 分区。

  - Windows

      请下载 Windows `Flash 下载工具 <https://www.espressif.com/en/support/download/other-tools>`_。参考 zip 文件夹中 ``readme.pdf`` 或者 ``doc`` 目录下说明，下载 ``build/customized_partitions/mfg_nvs.bin`` 文件到 {IDF_TARGET_NAME}。您可以通过 :ref:`AT+SYSFLASH? <cmd-SYSFLASH>` 命令查询 mfg_nvs.bin 的下载地址。

  - Linux or macOS

      请使用 `esptool.py <https://github.com/espressif/esptool>`_。

      您可以在 ESP-AT 根目录执行以下命令下载 mfg_nvs.bin 文件。

      .. code-block:: none

          esptool.py --chip auto --port PORTNAME --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB ADDRESS mfg_nvs.bin

      将 ``PORTNAME`` 替换为您的串口名称。``ADDRESS`` 替换为下载 mfg_nvs.bin 文件的地址，您可以通过 :ref:`AT+SYSFLASH? <cmd-SYSFLASH>` 命令查询下载地址。

- MCU 发送 :ref:`AT+SYSFLASH <cmd-SYSFLASH>` 命令更新 {IDF_TARGET_NAME} 中的 mfg_nvs 分区。

  .. code-block:: none

      # 擦除 mfg_nvs 分区
      AT+SYSFLASH=0,"mfg_nvs",0,MFG_NVS_SIZE

      # 写入 mfg_nvs.bin 文件
      AT+SYSFLASH=1,"mfg_nvs",0,MFG_NVS_SIZE

  ``MFG_NVS_SIZE`` 替换为下载 mfg_nvs.bin 文件的大小，不同的模组有不同的分区大小，您可以通过 :ref:`AT+SYSFLASH? <cmd-SYSFLASH>` 命令查询分区大小。
