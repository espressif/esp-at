***********************************
How to clone project and compile it
***********************************

- `ESP32 platform`_
- `ESP32S2 platform`_
- `ESP8266 platform`_

For specific supported modules, please refer to :component:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`


ESP32 platform
==============

Hardware Introduction
~~~~~~~~~~~~~~~~~~~~~

The WROOM32 Board sends AT commands through UART1 by default.

- GPIO16 is RXD
- GPIO17 is TXD
- GPIO14 is RTS
- GPIO15 is CTS

The debug log will be output through UART0 by default, where TXD0 is GPIO1 and RXD0 is GPIO3, but user can change it in menuconfig if needed:

``./build.py menuconfig`` --> ``Component config`` --> 
``Common ESP-related`` --> ``UART for console output``

.. note::

  **Please pay attention to possible conflicts of the pins**

  - If choosing ``AT through HSPI``, you can get the information of the HSPI pin by ``./build.py menuconfig`` --> ``Component config`` --> ``AT`` --> ``AT hspi settings``
  - If enabling ``AT ethernet support``, you can get the information of the ethernet pin from ``ESP32_AT_Ethernet.md``.

Compiling and flashing the project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Suppose you have completed the installation of the compiler environment for ESP-IDF, if not, you should complete it referring to `ESP-IDF Getting Started Guide <https://docs.espressif.com/projects/esp-idf/en/v4.0/get-started/index.html#setup-toolchain>`__. Then, to compile ESP-AT project properly, please do the following additional steps::

    step 1: install python>=3.8
    step 2: [install pip](https://pip.pypa.io/en/latest/installing/)  
    step 3: install the following python packages with pip: python -m pip install pyyaml xlrd

Compiling the ESP-AT is the same as compiling any other project based on the ESP-IDF:

.. note::

  Please do not set ``IDF_PATH`` unless you know ESP-AT project in particular. ESP-IDF will automatically be cloned.

1. You can clone the project into an empty directory using command::

     git clone --recursive https://github.com/espressif/esp-at.git

2. ``rm -rf builld sdkconfig`` to remove the old configuration and ``rm -rf esp-idf`` to remove the old ESP-IDF if you want to compile other esp platform AT.
3. ``./build.py menuconfig``, and then choose the name indexes of platform and your module, then ``Serial flasher config`` to configure the serial port for downloading.

  - platform name is defined in ``components/customized_partitions/raw_data/factory_param/factory_param_data.csv``, here should choose ``PLATFORM_ESP32``
  - module name is defined in ``components/customized_partitions/raw_data/factory_param/factory_param_data.csv``
  - silence mode is to remove some logs and reduce the firmware size, generally, it should be disabled.
  - The choice items will not appear if the file ``build/module_info.json`` exists. So please delete it if you want to reconfigure module information.

4. ``./build.py flash`` to compile the project and download it into the flash.

   -  Or you can call ``./build.py build`` to compile it, and follow the printed instructions to download the bin files into flash by yourself.
   -  If enable BT feature, the firmware size will be much larger, please make sure it does not exceed the ota partition size.
   - After compiled, the combined factory bin will be create in ``build/factory``, by default, the factory bin is 4MB flash size, DIO flash mode and 40MHz flash speed.

5. If the ESP-AT bin fails to boot, and prints "ota data partition invalid", you should run ``./build.py erase_flash`` to erase the entire flash.

ESP32S2 platform
=================

Hardware Introduction
~~~~~~~~~~~~~~~~~~~~~

The WROOM32S2 Board sends AT commands through UART1 by default.

-  GPIO21 is RXD
-  GPIO17 is TXD
-  GPIO19 is RTS
-  GPIO20 is CTS

The debug log will output through UART0 by default, which TXD0 is GPIO43 and RXD0 is GPIO44, but user can change it in menuconfig if needed:

``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART for console output``

Compiling and flashing the project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Suppose you have completed the installation of the compiler environment for ESP-IDF, if not, you should complete it referring to `ESP-IDF Getting Started Guide <https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html#setup-toolchain>`__. If required download the `compiler toolchain <https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/tools/idf-tools.html#list-of-idf-tools>`_. Then, to compile ESP-AT project properly, please do the following additional steps::

  step1:python > 3.8.0 
  step2:[install pip](https://pip.pypa.io/en/latest/installing/)  
  step3:install the following python packages with pip: python -m pip install pyyaml xlrd

Compiling the ESP-AT is the same as compiling any other project based on the ESP-IDF:

.. note::

  Please do not set ``IDF_PATH`` unless you know ESP-AT project in particular. ESP-IDF will automatically be cloned.

1. You can clone the project into an empty directory using command::

     git clone --recursive https://github.com/espressif/esp-at.git

2. ``rm -rf builld sdkconfig`` to remove the old configuration and ``rm -rf esp-idf`` to remove the old ESP-IDF if you want to compile other esp platform AT.
3. ``./build.py menuconfig``, and then choose the name indexes of platform and your module, then ``Serial flasher config`` to configure the serial port for downloading.

  - platform name is defined in ``components/customized_partitions/raw_data/factory_param/factory_param_data.csv``, here should choose ``PLATFORM_ESP32S2``
  - module name is defined in ``components/customized_partitions/raw_data/factory_param/factory_param_data.csv``
  - silence mode is to remove some logs and reduce the firmware size, generally, it should be disabled.
  - The choice items will not appear if the file ``build/module_info.json`` exists. So please delete it if you want to reconfigure module information.

4. ``./build.py flash`` to compile the project and download it into the flash.

   -  Or you can call ``./build.py build`` to compile it, and follow the printed instructions to download the bin files into flash by yourself.
   -  If enable BT feature, the firmware size will be much larger, please make sure it does not exceed the ota partition size.
   - After compiled, the combined factory bin will be create in ``build/factory``, by default, the factory bin is 4MB flash size, QIO flash mode and 80MHz flash speed.

5. If the ESP-AT bin fails to boot, and prints "ota data partition invalid", you should run ``./build.py erase_flash`` to erase the entire flash.


ESP8266 platform
================

Hardware Introduction
~~~~~~~~~~~~~~~~~~~~~

The ESP8266 WROOM 02 Board sends AT commands through UART0 by default.

-  GPIO13 is RXD
-  GPIO15 is TXD
-  GPIO1 is RTS
-  GPIO3 is CTS

The debug log will output through UART1 by default, which TXD0 is GPIO2, but user can change it in menuconfig if needed:

``./build.py menuconfig`` --> ``Component config`` -->
``ESP8266-specific`` --> ``UART for console output``

Compiling and flashing the project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Suppose you have completed the installation of the compiler environment for ESP-IDF, if not, you should complete it referring to `ESP8266 RTOS SDK Getting Started Guide <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/v3.2/get-started/index.html#setup-toolchain>`_. Then, to compile ESP-AT project properly, please do the following additional steps::

  step1:python > 3.8.0 
  step2:[install pip](https://pip.pypa.io/en/latest/installing/)  
  step3:install the following python packages with pip: python -m pip install pyyaml xlrd

Compiling the ESP-AT is the same as compiling any other project based on the ESP-IDF:

.. note::

  Please do not set ``IDF_PATH`` unless you know ESP-AT project in particular. ESP-IDF will automatically be cloned.

1. You can clone the project into an empty directory using command::

     git clone --recursive https://github.com/espressif/esp-at.git

2. ``rm -rf builld sdkconfig`` to remove the old configuration and ``rm -rf esp-idf`` to remove the old ESP-IDF if you want to compile other esp platform AT.
3. ``./build.py menuconfig``, and then choose the name indexes of platform and your module, then ``Serial flasher config`` to configure the serial port for downloading.

  - platform name is defined in ``components/customized_partitions/raw_data/factory_param/factory_param_data.csv``, here should choose ``PLATFORM_ESP8266``
  - module name is defined in ``components/customized_partitions/raw_data/factory_param/factory_param_data.csv``
  - silence mode is to remove some logs and reduce the firmware size, generally, it should be disabled.
  - The choice items will not appear if the file ``build/module_info.json`` exists. So please delete it if you want to reconfigure module information.

4. ``./build.py flash`` to compile the project and download it into the flash.

   -  Or you can call ``./build.py build`` to compile it, and follow the printed instructions to download the bin files into flash by yourself.
   -  If enable BT feature, the firmware size will be much larger, please make sure it does not exceed the ota partition size.
   - After compiled, the combined factory bin will be create in ``build/factory``, by default, the factory bin is 2MB flash size, DIO flash mode and 80MHz flash speed.

5. If the ESP-AT bin fails to boot, and prints "ota data partition invalid", you should run ``./build.py erase_flash`` to erase the entire flash.