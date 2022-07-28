How to Set AT Port Pins
==========================

{IDF_TARGET_HYPHEN_LOWERCASE_NAME: default="undefined", esp32="esp32", esp32c3="esp32-c3"}

:link_to_translation:`zh_CN:[中文]`

This document introduces how to modify :term:`AT port` pins in the firmware for {IDF_TARGET_NAME} series of products. By default, ESP-AT uses two UART interfaces as AT ports: one is to output logs (named as log port below) and the other to send AT commands and receive responses (named as command port below). 

To modify the AT port pins of your {IDF_TARGET_NAME}, you should:

- :doc:`clone the ESP-AT project <How_to_clone_project_and_compile_it>`;
- modify the pins either in the menuconfig utility or the factory_param_data.csv table;
- :doc:`compile the project <How_to_clone_project_and_compile_it>` to generate the new bin in ``build/customized_partitions/factory_param.bin``;
- :ref:`flash the new bin to your device <flash-at-firmware-into-your-device>`.

This document focuses on modifying the pins. Click the links above for details of other steps.

.. note::
  To use other interfaces as the AT command port, please refer to :project_file:`AT through SDIO <main/interface/sdio/README.md>`, :project_file:`AT through SPI <main/interface/hspi/README.md>`, or :project_file:`AT through socket <main/interface/socket/README.md>` for more details.

{IDF_TARGET_NAME} Series
------------------------

The log port and command port pins of {IDF_TARGET_NAME} AT firmware can be user-defined to other pins. Refer to `{IDF_TARGET_NAME} Technical Reference Manual <https://www.espressif.com/sites/default/files/documentation/{IDF_TARGET_HYPHEN_LOWERCASE_NAME}_technical_reference_manual_en.pdf>`_ for the pins you can use.

Modify Log Port Pins
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, the {IDF_TARGET_NAME} AT firmware provided by Espressif uses the following UART0 pins to output log:

.. only:: esp32

  - TX: GPIO1
  - RX: GPIO3

.. only:: esp32c3 or esp32c2

  - TX: GPIO21
  - RX: GPIO20

When compiling your ESP-AT project, you can modify them to other pins with the menuconfig utitlity:

* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART for console output``
* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART TX on GPIO#``
* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART RX on GPIO#``

Modify Command Port Pins
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, UART1 is used to send AT commands and receive AT responses, and its pins are defined in Column ``uart_port``, ``uart_tx_pin``, ``uart_rx_pin``, ``uart_cts_pin``, and ``uart_rts_pin`` of the :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`.

You can change them directly in your factory_param_data.csv table:
  
- Open your local factory_param_data.csv file.
- Locate the row of your module.
- Set ``uart_port`` as needed.
- Set ``uart_tx_pin`` and ``uart_rx_pin`` as needed.
- Set ``uart_cts_pin`` and ``uart_rts_pin`` to be -1 if you do not use the hardware flow control function.
- Save the table.
