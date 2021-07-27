How to Enable ESP-AT Classic Bluetooth
======================================

By default, Classic Bluetooth AT commands are disabled in the ESP32 series AT firmware. If you want to use them on ESP devices, please follow the steps below. The commands are classified into three categories, general Bluetooth commands, :term:`SPP` commands, and :term:`A2DP` commands and each category may be enabled and disabled separately.

1. :doc:`Clone the esp-at project <How_to_clone_project_and_compile_it>`.
2. Run the configuration utility by executing ``./build.py menuconfig``.
3. Enable the commands you need.

  - Enable general Bluetooth commands.

   ``Component config`` -> ``AT`` -> ``AT bt command support``.
 
  - Enable SPP commands.

    ``Component config`` -> ``AT`` -> ``AT bt command support`` -> ``AT bt spp command support``.

  - Enable A2DP commands.

    ``Component config`` -> ``AT`` -> ``AT bt command support`` -> ``AT bt a2dp command support``.

4. :doc:`Compile the project <How_to_clone_project_and_compile_it>` to generate the new bin in ``build/esp-at.bin``.
5. :ref:`Flash the new bin to your device <flash-at-firmware-into-your-device>`.