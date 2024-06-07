Technology Selection
====================

:link_to_translation:`zh_CN:[中文]`

This document mainly introduces how to choose the right Espressif hardware product, AT software solution, and the initial preparation for your project.

.. important::
    If you have any questions regarding the selection of Espressif hardware products or AT software solutions, please contact `Espressif Sales <https://www.espressif.com/en/contact-us/sales-questions>`_ or `Technical Support <https://www.espressif.com/en/contact-us/technical-inquiries>`_.

Hardware Selection
------------------

Before using ESP-AT, you need to select a suitable Espressif chip to integrate into your product, empowering it with wireless capabilities. Hardware selection is a complex process that requires consideration of various factors such as functionality, power consumption, cost, size, etc. Please read the following content to help you choose the hardware.

.. list::

    - `Product Selector Tool <https://products.espressif.com/#/product-selector?language=en&names=>`_ can help you understand the hardware differences of different Espressif products.
    - `Datasheets <https://www.espressif.com/en/support/documents/technical-documents?keys=&field_download_document_type_tid%5B%5D=510>`_ can help you understand the hardware capabilities supported by the chip/module.
    - `Board Selection Guide <https://docs.espressif.com/projects/esp-techpedia/en/latest/esp-friends/get-started/board-selection.html>`_ can help you compare the differences between chips, modules, and development boards and provide selection guidance.

.. note::
    **The datasheets indicate the maximum hardware capabilities and do not represent the capabilities of the AT software**. For example, the ESP32-C6 chip supports Zigbee 3.0 and Thread 1.3, but the existing AT software solutions do not support these two wireless protocols.

.. _at-solution-selection:

AT Software Solution Selection
------------------------------

AT software solution is the AT firmwares provided by Espressif for different chips, which can help you quickly implement wireless functionality.

- If you want to know the detailed AT software capabilities of {IDF_TARGET_NAME} chip, please refer to the :doc:`AT Command Set <../AT_Command_Set/index>`.
- If you want to compare the different AT firmware functionalities of {IDF_TARGET_NAME} chip, please refer to the :doc:`ESP-AT Firmware Differences <../Compile_and_Develop/esp-at_firmware_differences>`.

The table below shows a brief comparison of the AT firmware for different chips.

.. list-table::
  :header-rows: 1

  * - Chip
    - Wireless
    - AT Firmware
    - Description
  * - ESP32-C6
    - **Wi-Fi 6** + BLE 5.0
    - `v4.0.0.0 <https://github.com/espressif/esp-at/releases/tag/v4.0.0.0>`_
    -
  * - ESP32-C3
    - Wi-Fi 4 + BLE 5.0
    - `v3.3.0.0 <https://github.com/espressif/esp-at/releases/tag/v3.3.0.0>`_
    -
  * - ESP32-C2
    - Wi-Fi 4 (or BLE 5.0)
    - `v3.3.0.0 <https://github.com/espressif/esp-at/releases/tag/v3.3.0.0>`_
    -
  * - ESP32
    - Wi-Fi 4 + BLE v4.2 (+ **BT**)
    - `v3.4.0.0 <https://github.com/espressif/esp-at/releases/tag/v3.4.0.0>`_
    -
  * - ESP32-S2
    - Wi-Fi 4
    - `v3.4.0.0 <https://github.com/espressif/esp-at/releases/tag/v3.4.0.0>`_
    - It is recommended to use the more cost-effective ESP32-C series.

- ``(or BLE 5.0)`` indicates that Bluetooth LE functionality is supported in the AT software solution, but it is not included in the released firmware.
- ``(+ BT)`` indicates that Classic Bluetooth functionality is supported in the AT software solution, but it is not included in the released firmware.

.. note::
  The modules or chips from the factory are not preloaded with AT firmware. If you have mass production requirements, please contact the corresponding business personnel or sales@espressif.com in a timely manner, and we will provide customized production.

.. _firmware-selection:

Which Type of AT Firmware Shall I Choose?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ESP-AT firmware has the following types. Among them, the workload of downloading or preparing firmware increases from top to bottom, and so does the number of supported module types.

.. contents::
   :local:
   :depth: 1

.. _official-released-firmware:

Officially Released Firmware (Recommended)
""""""""""""""""""""""""""""""""""""""""""

**Officially released firmware**, also known as "released firmware", "official firmware" or "default firmware", has passed testing and periodically released by Espressif's official team according to the internal development plan. It can be upgraded directly based on the Espressif OTA server. If it fully matches your project needs, it is recommended to choose it over the other types of firmware. If it does not support your module, you can select a firmware that has a similar hardware configuration to your module according to :doc:`Hardware Differences <../Compile_and_Develop/esp-at_firmware_differences>` for testing and verification.

.. list::

  - How to obtain firmware: :doc:`{IDF_TARGET_NAME} AT firmware <../AT_Binary_Lists/esp_at_binaries>`
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
"""""""""""""""""""""""""

**GitHub temporary firmware** is compiled by GitHub whenever code is pushed to GitHub and yet does not reach the firmware release cycle. In other words, it is the firmware in development, including the temporary versions of **officially released firmware** and supported firmware that is not planned for release. The former can be upgraded directly based on the Espressif OTA server.
  
- How to obtain firmware: Please refer to :doc:`../Compile_and_Develop/How_to_download_the_latest_temporary_version_of_AT_from_github`.
- Pros: 

  - Real-time integration of new features and bug fixes.
  - Covering some unofficially released firmware, such as firmware based on SDIO communication and firmware based on SPI communication.
  - Small workload of obtaining firmware.

- Cons: As the firmware compiled based on commits that are not for official release has not been thoroughly tested, there may be some risks. You need to do a complete test by yourself.

.. _firmware-modify-paras-not-source-code:

Firmware with Updated Parameters
""""""""""""""""""""""""""""""""

The **firmware with updated parameters** is generated by updating the parameter area and you do not need to compile the firmware. It is suitable for the case where the firmware function meets the project requirements, but only some parameters do not, such as the UART baud rate and UART GPIO pins. This type of firmware can be directly upgraded based on the Espressif OTA server.
  
- For how to modify those parameters, please refer to :doc:`../Compile_and_Develop/tools_at_py`.
- Pros:

  - No need to recompile the firmware.
  - The firmware is stable and reliable.

- Cons: Requiring modification to released firmware, long update cycle, and a limited number of supported modules.

.. _self-compiled-firmware:

Self-Compiled Firmware
""""""""""""""""""""""

When you need to conduct secondary development, you can compile the firmware by yourself. To support the OTA function, you need to deploy your own OTA server.

- For how to compile the firmware by yourself, please refer to :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`.
- Pros: You can control functions and cycles.
- Cons: You need to set up the compilation environment.

If stability is your priority, it is recommended to develop your AT firmware based on the latest released version corresponding to the chip. If you want more new features, it is recommended to develop your AT firmware based on the `master branch <https://github.com/espressif/esp-at/tree/master>`_.

Initial Project Preparation
---------------------------

During the initial project preparation phase, it is **strongly recommended** that you choose `Espressif development boards <https://www.espressif.com/products/devkits>`_ to start your project. In the early stages of the project, it can help you quickly validate prototypes, evaluate hardware and software capabilities, and reduce project risks. In the middle stages of the project, it can help you quickly integrate and verify functions, optimize performance, and improve development efficiency. In the later stages of the project, it can help you quickly simulate and locate issues, and achieve rapid product iteration.

If you are using :ref:`self-compiled-firmware`, it is recommended to prioritize using Linux as the development environment.
