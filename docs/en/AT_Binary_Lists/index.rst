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
  - :ref:`firmware-selection`: Types of AT firmware, ways to access them, their application scenarios, pros, cons, etc.
  - :ref:`what-next-after-you-get-firmware`

.. note::
  To download AT firmware for other chip series, please go to the drop-down list on the upper left corner of this page and select a chip series to navigate to the documentation of that chip for downloading.

.. _brief-intro-firmware:

Brief Introduction to AT Firmware
----------------------------------

ESP-AT firmware contains several binaries, and each of them is dedicated to a specific function:

- ``factory/factory_xxx.bin`` is the combination of all binaries. So, you can either download the ``factory/factory_xxx.bin`` to address 0, or several binaries to different addresses according to ``download.config``.
-   ``at_customize.bin`` provides a user partition table, which lists the starting address and partition size for the ``mfg_nvs`` and possible ``fatfs``. You can read and write the contents of the partition listed in this file with the command :ref:`AT+FS <cmd-FS>` and :ref:`AT+SYSFLASH <cmd-SYSFLASH>`.
-   ``mfg_nvs.bin`` indicates the hardware configurations for different {IDF_TARGET_NAME} modules (see the table below). Please make sure the correct bin is used for your module. For more information on this parameter file, please refer to :ref:`firmware-modify-paras-not-source-code`.

  .. only:: esp32

    .. list-table:: Hardware Configuration of {IDF_TARGET_NAME} Modules
       :header-rows: 1

       * - Modules
         - UART Pins (TX, RX, CTS, RTS)
       * - ESP32-WROOM-32 Series (ESP32 Default Module)
         - - GPIO17
           - GPIO16
           - GPIO15
           - GPIO14
       * - ESP32-WROVER Series (Supports Classic Bluetooth)
         - - GPIO22
           - GPIO19
           - GPIO15
           - GPIO14
       * - ESP32-PICO Series
         - - GPIO22
           - GPIO19
           - GPIO15
           - GPIO14
       * - ESP32-SOLO Series
         - - GPIO17
           - GPIO16
           - GPIO15
           - GPIO14

  .. only:: esp32c2

    .. list-table:: Hardware Configuration of {IDF_TARGET_NAME} Modules
       :header-rows: 1

       * - Modules
         - UART Pins (TX, RX, CTS, RTS)
       * - ESP32C2-4MB Series
         - - GPIO7
           - GPIO6
           - GPIO5
           - GPIO4

  .. only:: esp32c3

    .. list-table:: Hardware Configuration of {IDF_TARGET_NAME} Modules
       :header-rows: 1

       * - Modules
         - UART Pins (TX, RX, CTS, RTS)
       * - ESP32-C3-MINI-1 Series
         - - GPIO7
           - GPIO6
           - GPIO5
           - GPIO4

  .. only:: esp32c6

    .. list-table:: Hardware Configuration of {IDF_TARGET_NAME} Modules
       :header-rows: 1

       * - Modules
         - UART Pins (TX, RX, CTS, RTS)
       * - ESP32C6-4MB Series
         - - GPIO7
           - GPIO6
           - GPIO5
           - GPIO4

.. _firmware-selection:

Which Type of Firmware Shall I Choose?
--------------------------------------

ESP-AT firmware has the following types. Among them, the workload of downloading or preparing firmware increases from top to bottom, and so does the number of supported module types.

.. contents::
   :local:
   :depth: 1

.. _official-released-firmware:

Officially Released Firmware (Recommended)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Officially released firmware**, also known as "released firmware", "official firmware" or "default firmware", is tested and periodically released by Espressif's official team according to the internal development plan. It can be upgraded directly based on the Espressif OTA server. If it fully matches your project needs, it is recommended to choose it over the other types of firmware. If it does not support your module, you can select a firmware that has a similar hardware configuration to your module according to :doc:`Hardware Differences <../Compile_and_Develop/esp-at_firmware_differences>` for testing and verification.

.. list::

  - How to obtain firmware: :doc:`{IDF_TARGET_NAME} AT firmware <esp_at_binaries>`
  - Pros: 
    
    - Stable
    - Reliable
    - Small workload of obtaining firmware
  
  - Cons: 
    
    - Long update cycle
    - A limited number of supported modules
  
  - Reference documentation:
    
    - :doc:`Hardware connection <../Get_Started/Hardware_connection>`
    - :doc:`Firmware Downloading and Flash <../Get_Started/Downloading_guide>`
    - For which chip series are supported and unsupported by ESP-AT firmware, please refer to ESP-AT GitHub home page `readme.md <https://github.com/espressif/esp-at>`_

.. _github-temporary-firmware:

GitHub Temporary Firmware
^^^^^^^^^^^^^^^^^^^^^^^^^

**GitHub temporary firmware** is compiled by GitHub whenever code is pushed to GitHub and yet does not reach the firmware release cycle. In other words, it is the firmware in development, including the temporary versions of **officially released firmware** and supported firmware that is not planned for release. The former can be upgraded directly based on the Espressif OTA server.
  
- How to obtain firmware: Please refer to :doc:`../Compile_and_Develop/How_to_download_the_latest_temporary_version_of_AT_from_github`.
- Pros: 

  - Real-time integration of new features and bug fixes.
  - Covering some unofficially released firmware, such as firmware based on SDIO communication and firmware based on SPI communication.
  - Small workload of obtaining firmware.

- Cons: As the firmware compiled based on commits that are not for official release has not been thoroughly tested, there may be some risks. You need to do a complete test by yourself.

.. _firmware-modify-paras-not-source-code:

Firmware with Updated Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The **firmware with updated parameters** is generated by updating the parameter area and you do not need to compile the firmware. It is suitable for the case where the firmware function meets the project requirements, but only some parameters do not, such as the factory baud rate and UART IO pins. This type of firmware can be directly upgraded based on the Espressif OTA server.
  
- For how to modify those parameters, please refer to :doc:`../Compile_and_Develop/tools_at_py`.
- Pros:

  - No need to recompile the firmware.
  - The firmware is stable and reliable.

- Cons: Requiring modification to released firmware, long update cycle, and a limited number of supported modules.

.. _self-compiled-firmware:

Self-Compiled Firmware
^^^^^^^^^^^^^^^^^^^^^^

When you need to conduct secondary development, you can compile the firmware by yourself. To support the OTA function, you need to deploy your own OTA server.

- For how to compile the firmware by yourself, please refer to :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`.
- Pros: You can control functions and cycles.
- Cons: You need to set up the compilation environment.

.. _what-next-after-you-get-firmware:

After Getting Firmware, What Next?
----------------------------------

After you get the firmware, please refer to :doc:`Hardware Connection <../Get_Started/Hardware_connection>` and :doc:`Firmware Downloading and Flashing Guide <../Get_Started/Downloading_guide>` to connect the PC and ESP device and flash the firmware to the device.
