How to Set AT Port Pins
==========================

:link_to_translation:`zh_CN:[中文]`

This document introduces how to modify :term:`AT port` pins in the firmware for {IDF_TARGET_NAME} series of products. By default, ESP-AT uses two UART interfaces as AT ports: one is to output logs (named as log port below) and the other to send AT commands and receive responses (named as command port below). 

If you already have the {IDF_TARGET_NAME} AT firmware and only need to modify the command port pins, you can directly modify the firmware using the :doc:`tools_at_py` to generate a new firmware for your module. Otherwise, if you need to modify the AT port pins of your {IDF_TARGET_NAME} device, you need to complete the following steps:

- :doc:`clone the ESP-AT project <How_to_clone_project_and_compile_it>`;
- modify the pins either in the menuconfig utility or the factory_param_data.csv table;
- :doc:`compile the project <How_to_clone_project_and_compile_it>` to generate the new bin in ``build/customized_partitions/factory_param.bin``;
- :ref:`flash the new bin to your device <flash-at-firmware-into-your-device>`.

This document focuses on modifying the pins. Click the links above for details of other steps.

.. note::
  To use other interfaces as the AT command port, please refer to :project_file:`AT through SDIO <main/interface/sdio/README.md>`, :project_file:`AT through SPI <main/interface/spi/README.md>`, or :project_file:`AT through socket <main/interface/socket/README.md>` for more details.

{IDF_TARGET_NAME} Series
------------------------

The log port and command port pins of {IDF_TARGET_NAME} AT firmware can be user-defined to other pins. Refer to `{IDF_TARGET_NAME} Technical Reference Manual <{IDF_TARGET_TRM_EN_URL}>`_ for the pins you can use.

Modify Log Port Pins
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, the {IDF_TARGET_NAME} AT firmware provided by Espressif uses the following UART0 pins to output log:

.. only:: esp32

  - TX: GPIO1
  - RX: GPIO3

.. only:: esp32c2

  - TX: GPIO20
  - RX: GPIO19

.. only:: esp32c3

  - TX: GPIO21
  - RX: GPIO20

.. only:: esp32c6

  - TX: GPIO17
  - RX: GPIO16

.. only:: esp32s2

  - TX: GPIO17
  - RX: GPIO21

When compiling your ESP-AT project, you can modify them to other pins with the menuconfig utitlity:

* ``./build.py menuconfig`` --> ``Component config`` --> ``ESP System Settings`` --> ``Channel for console output`` --> ``Custom UART``
* ``./build.py menuconfig`` --> ``Component config`` --> ``ESP System Settings`` --> ``UART TX on GPIO#``
* ``./build.py menuconfig`` --> ``Component config`` --> ``ESP System Settings`` --> ``UART RX on GPIO#``

Modify Command Port Pins
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, UART1 is used to send AT commands and receive AT responses, and its pins are defined in Column ``uart_port``, ``uart_tx_pin``, ``uart_rx_pin``, ``uart_cts_pin``, and ``uart_rts_pin`` of the :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`.

You can change them directly in your factory_param_data.csv table:

- Open your local factory_param_data.csv file.
- Locate the row of your module.
- Set ``uart_port`` as needed. (If you want to use the AT log port as the AT command port as well, you need to modify this line, and ensure that the ``uart_tx_pin`` and ``uart_rx_pin`` below have the same pins as the AT log port)
- Set ``uart_tx_pin`` and ``uart_rx_pin`` as needed. (Make sure that the pins you are going to modify are not being used by other functions, including the AT log port)
- Set ``uart_cts_pin`` and ``uart_rts_pin`` to be -1 if you do not use the hardware flow control function.
- Save the table.
