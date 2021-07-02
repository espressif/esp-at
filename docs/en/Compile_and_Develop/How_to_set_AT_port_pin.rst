How to Set AT Port Pins
==========================

This document introduces how to modify :term:`AT port` pins in the firmware for ESP32 and ESP32-C3 series of products. By default, ESP-AT uses two UART interfaces as AT ports: one is to output logs (named as log port below) and the other to send AT commands and receive responses (named as command port below). 

To modify the AT port pins of your ESP device, you should:

- :doc:`clone the esp-at project <How_to_clone_project_and_compile_it>`;
- modify the pins either in the menuconfig utility or the factory_param_data.csv table;
- :doc:`compile the project <How_to_clone_project_and_compile_it>` to generate the new bin in ``build/customized_partitions/factory_param.bin``;
- :ref:`flash the new bin to your device <flash-at-firmware-into-your-device>`.

This document focuses on modifying the pins. Click the links above for details of other steps. Below is the document structure:

- `ESP32 Series`_
- `ESP32-C3 Series`_

.. note::
  To use other interfaces as the AT command port, please refer to :at_file:`AT through SDIO <main/interface/sdio/README.md>`, :at_file:`AT through SPI <main/interface/hspi/README.md>`, or :at_file:`AT through socket <main/interface/socket/README.md>` for more details.

ESP32 Series
-------------

The log port and command port pins of ESP32 AT firmware can be user-defined to other pins. Refer to `ESP32 Technical Reference Manual <https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf>`_ for the pins you can use.

Modify Log Port Pins
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, the ESP32 AT firmware provided by Espressif uses the following UART0 pins to output log:

- TX: GPIO1
- RX: GPIO3

When compiling your esp-at project, you can modify them to other pins with the menuconfig utitlity:

* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART for console output``
* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART TX on GPIO#``
* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART RX on GPIO#``

Modify Command Port Pins
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, UART1 is used to send AT commands and receive AT responses, and its pins are defined in Column ``uart_port``, ``uart_tx_pin``, ``uart_rx_pin``, ``uart_cts_pin``, and ``uart_rts_pin`` of the :component:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`.

You can change them directly in your factory_param_data.csv table:
  
- Open your local factory_param_data.csv file.
- Locate the row of your module.
- Set ``uart_port`` as needed.
- Set ``uart_tx_pin`` and ``uart_rx_pin`` as needed.
- Set ``uart_cts_pin`` and ``uart_rts_pin`` to be -1 if you do not use the hardware flow control function.
- Save the table.

ESP32-C3 Series
---------------

The log port and command port pins of ESP32-C3 AT firmware can be user-defined to other pins. `ESP32-C3 Technical Reference Manual <https://www.espressif.com/sites/default/files/documentation/esp32-c3_technical_reference_manual_en.pdf>`_ for the pins you can use.

Modify Log Port Pins
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, the ESP32-C3 AT firmware provided by Espressif uses the following UART0 pins to output log:

- TX: GPIO21
- RX: GPIO20

When compiling your esp-at project, you can modify them to other pins with the menuconfig utitlity:

* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART for console output``
* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART TX on GPIO#``
* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART RX on GPIO#``

Modify Command Port Pins
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, UART1 is used to send AT commands and receive AT responses, and its pins are defined in Column ``uart_port``, ``uart_tx_pin``, ``uart_rx_pin``, ``uart_cts_pin``, and ``uart_rts_pin`` of the :component:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`.

You can change them directly in your factory_param_data.csv table:
  
- Open your local factory_param_data.csv file.
- Locate the row of your module.
- Set ``uart_port`` as needed.
- Set ``uart_tx_pin`` and ``uart_rx_pin`` as needed.
- Set ``uart_cts_pin`` and ``uart_rts_pin`` to be -1 if you do not use the hardware flow control function.
- Save the table.
