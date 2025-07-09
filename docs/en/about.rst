About
=====

:link_to_translation:`zh_CN:[中文]`

This is documentation of `ESP-AT <https://github.com/espressif/esp-at>`_, a solution developed by Espressif to quickly and easily interface with {IDF_TARGET_NAME} products.

Espressif Wi-Fi and Bluetooth chipsets are often used as add-on modules to seamlessly integrate wireless connectivity features into new and existing products. In an effort to facilitate this and cut down on engineering costs, Espressif Systems has developed :doc:`AT firmware <AT_Binary_Lists/index>` as well as a rich set of :doc:`AT commands <AT_Command_Set/index>` that can be used to interface with Espressif products.

.. figure:: ../_static/about-esp-at-solution.png
   :align: center
   :alt: ESP-AT Solution
   :figclass: align-center

   ESP-AT Solution

The AT firmware allows for rapid integration by providing:

- In-built TCP/IP stack and data buffering
- Easy integration with resource-constrained host platforms
- Easy-to-parse command-response protocols
- Customized, user-defined AT commands

.. figure:: ../_static/about-esp-at-commands.png
   :align: center
   :alt: ESP-AT Commands
   :figclass: align-center

   ESP-AT Commands

Other Espressif Connectivity Solutions
---------------------------------------

In addition to ESP-AT, Espressif also provides other solutions to meet different integration needs:

- **ESP-Hosted** is a software solution that enables Espressif series chips to act as wireless communication co-processors for external host systems. It allows host devices (Linux-based systems or microcontrollers, MCUs) to add Wi-Fi and Bluetooth/BLE capabilities via standard interfaces like SPI, SDIO, or UART. You can refer to `ESP-Hosted GitHub <https://github.com/espressif/esp-hosted>`_ for details.
- **ESP-IoT-Bridge Solution** focuses on connectivity and communication between various network interfaces in IoT application scenarios, such as SPI, SDIO, USB, Wi-Fi, Ethernet and other network interfaces. In this solution, the bridge device can not only provide net access for other devices, but also be a separate equipment to connect the remote server. You can refer to `ESP-IoT-Bridge GitHub <https://github.com/espressif/esp-iot-bridge>`_ for details.

These solutions target different system architectures and requirements. For users who are considering alternatives to the AT firmware for integrating wireless connectivity, these options may provide a better fit depending on host platform capabilities and design goals.
