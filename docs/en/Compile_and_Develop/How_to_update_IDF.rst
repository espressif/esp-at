How to Update the ESP-IDF Version
=================================

{IDF_TARGET_COMPILE_MNAME: default="undefined", esp32="WROOM-32", esp32c2="ESP32C2-4MB", esp32c3="MINI-1", esp32c6="ESP32C6-4MB", esp32s2="MINI"}

:link_to_translation:`zh_CN:[中文]`

ESP-AT firmware is based on the Espressif IoT Development Framework (ESP-IDF), and each version of ESP-AT firmware corresponds to a specific ESP-IDF version. It is strongly recommended to use the default ESP-IDF version of the ESP-AT project, and **NOT** recommended to update it, because the inconsistency between the underlying ESP-IDF versions of lib{IDF_TARGET_PATH_NAME}_at_core.a and the ESP-AT project may cause incorrect operation of the firmware.

However, in some special cases, a minor updated ESP-IDF version may be also able to work with the ESP-AT project. In case you should update it, this document can be a reference.

The ESP-IDF version that a specific ESP-AT firmware corresponds to is recorded in the ``IDF_VERSION`` files, which are distributed in different module folders under the :project:`module_config` folder. The file describes the branch, commit ID, and repository URL of the ESP-IDF that a module firmware is based on. For example, the ``IDF_VERSION`` of the ``{IDF_TARGET_COMPILE_MNAME}`` module of ``PLATFORM_{IDF_TARGET_CFG_PREFIX}`` platform is located at :project_file:`module_config/module_{IDF_TARGET_PATH_NAME}_default/IDF_VERSION`.

If you want to update the ESP-IDF version for your ESP-AT firmware, please follow the steps:

- Find the ``IDF_VERSION`` file for your module.
- Update the branch, commit ID, repository as needed.
- Delete the original ``esp-idf`` under the ``esp-at`` root directory, so that the ESP-IDF of the version specified in ``IDF_VERSION`` will be cloned first in the next compilation.
- Recompile the ESP-AT project.

Note that when the ESP-AT version and the ESP-IDF version do not match, the following error will be reported when compiling:

::

    Please wait for the update to complete, which will take some time
