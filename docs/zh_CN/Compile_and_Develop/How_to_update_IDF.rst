如何更新 ESP-IDF 版本
=================================

{IDF_TARGET_COMPILE_MNAME: default="undefined", esp32="WROOM-32", esp32c3="MINI-1", esp32c2="ESP32C2-4MB", esp32c6="ESP32C6-4MB", esp32s2="MINI"}

:link_to_translation:`en:[English]`

ESP-AT 固件基于乐鑫物联网开发框架（ESP-IDF），每个版本的 ESP-AT 固件对应某个特定的 ESP-IDF 版本。强烈建议使用 ESP-AT 工程默认的 ESP-IDF 版本，**不建议** 更新 ESP-IDF 版本，因为 lib{IDF_TARGET_PATH_NAME}_at_core.a 底层的 ESP-IDF 版本与 ESP-AT 工程的 IDF 版本不一致可能会导致固件的错误操作。

但是，在某些特殊情况下，ESP-IDF 的小版本更新也可能适用于 ESP-AT 工程。如果您需要更新，本文档可作为参考。

ESP-AT 固件对应的 ESP-IDF 版本记录在 ``IDF_VERSION`` 文件中，这些文件分布在 :project:`module_config` 文件夹下的不同模组文件夹中。该文件描述了模组固件所基于的 ESP-IDF 的分支、提交 ID 和仓库地址。例如，``PLATFORM_{IDF_TARGET_CFG_PREFIX}`` 平台的 ``{IDF_TARGET_COMPILE_MNAME}`` 模组的 ``IDF_VERSION`` 位于 :project_file:`module_config/module_{IDF_TARGET_PATH_NAME}_default/IDF_VERSION`。

如果您想为 ESP-AT 固件更新 ESP-IDF 版本，请按照以下步骤操作。

- 找到模组的 ``IDF_VERSION`` 文件。
- 根据需要更新其中的分支、提交 ID 和仓库地址。
- 删除 ``esp-at`` 根目录下原有的 ``esp-idf``，以便下次编译时首先克隆 ``IDF_VERSION`` 中指定的 ESP-IDF 版本。
- 重新编译 ESP-AT 工程。

注意，当 ESP-AT 版本和 ESP-IDF 版本不匹配，编译时会报如下错误。

::

    Please wait for the update to complete, which will take some time
