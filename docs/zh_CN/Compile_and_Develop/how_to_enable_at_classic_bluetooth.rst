如何启用 ESP-AT 经典蓝牙
======================================

:link_to_translation:`en:[English]`

默认情况下，ESP32 系列 AT 固件禁用经典蓝牙 AT 命令，若需在 ESP32 设备上使用，请按照以下步骤操作。当前 AT 支持的经典蓝牙命令分为三类，通用蓝牙命令、SPP 命令和 A2DP 命令，每类命令可以单独启用和禁用。

1. :doc:`克隆 ESP-AT 工程 <How_to_clone_project_and_compile_it>`。
2. 执行 ``./build.py menuconfig`` （Linux 或者 macOS） 或者 ``build.py menuconfig`` (Windows) 来运行配置工具。
3. 启用您需要的命令。

  - 启用通用蓝牙命令。

   ``Component config`` -> ``AT`` -> ``AT bt command support``

  - 启用 SPP 命令。

    ``Component config`` -> ``AT`` -> ``AT bt command support`` -> ``AT bt spp command support``

  - 启用 A2DP 命令。

    ``Component config`` -> ``AT`` -> ``AT bt command support`` -> ``AT bt a2dp command support``

4. :doc:`编译工程 <How_to_clone_project_and_compile_it>` ，在 ``build/esp-at.bin`` 中生成新的固件。
5. :ref:`将新固件烧录进您的设备 <flash-at-firmware-into-your-device>`。
