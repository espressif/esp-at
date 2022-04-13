AT Binary Lists
=================

:link_to_translation:`zh_CN:[中文]`

.. toctree::
  :maxdepth: 1

  :esp32: ESP32 AT binaries <ESP32_AT_binaries>
  :esp32c3: ESP32-C3 AT binaries <ESP32-C3_AT_binaries>

Each of the linked above ESP-AT firmware contains several binaries dedicated to some specific functions, and the ``factory/factory_xxx.bin`` is the combination of all binaries. So you can either download the ``factory/factory_xxx.bin`` to address 0, or several binaries to different addresses according to ``download.config``. Please refer to :ref:`Download AT Firmware <download-at-firmware>` for how to download.

-   ``at_customize.bin`` provides a user partition table, which lists the starting address and partition size for the ``ble_data.bin``, SSL certificates, MQTT certificates, ``factory_param_XXX.bin``, and so on. You can read and write contents of the partition listed in this file with the command :ref:`AT+FS <cmd-FS>` and :ref:`AT+SYSFLASH <cmd-SYSFLASH>`.
-   ``factory_param_XXX.bin`` indicates the hardware configurations for different {IDF_TARGET_NAME} modules (see the table below). Please make sure the correct bin is used for your specific module.

    .. note::

      If you design your own module, please configure and compile with reference to :doc:`../Compile_and_Develop/How_to_create_factory_parameter_bin`, and the binaries will be automatically generated after compilation. Or you can select firmware with similar configuration according to the configuration of ``UART pins,PSRAM,Flash`` (The premise is to ensure that the hardware meets the requirements. Please refer to :doc:`../Compile_and_Develop/esp-at_firmware_differences` for the firmware applicable to your module).

      When you flash the firmware into module according to the ``download.config``, the ``customized_partitions/factory_param.bin`` should be replaced with the actual module-specific ``customized_partitions/factory_param_XXX.bin``. UART CTS and RTS pins are optional.

.. only:: esp32

    - **ESP32 Series**

    .. list-table::
       :header-rows: 1

       * - Modules
         - UART Pins (TX, RX, CTS, RTS)
         - Factory Parameter Bin 
       * - ESP32-WROOM-32 Series (ESP32 Default Module)
         - - GPIO17
           - GPIO16
           - GPIO15
           - GPIO14
         - ``factory_param_WROOM-32.bin``
       * - ESP32-WROVER Series (Supports Classic Bluetooth)
         - - GPIO22
           - GPIO19
           - GPIO15
           - GPIO14
         - ``factory_param_WROVER-32.bin``
       * - ESP32-PICO Series
         - - GPIO22
           - GPIO19
           - GPIO15
           - GPIO14
         - ``factory_param_PICO-D4.bin``
       * - ESP32-SOLO Series
         - - GPIO17
           - GPIO16
           - GPIO15
           - GPIO14
         - ``factory_param_SOLO-1.bin``

.. only:: esp32c3

    - **ESP32-C3 Series**

    .. list-table::
       :header-rows: 1

       * - Modules
         - UART Pins (TX, RX, CTS, RTS)
         - Factory Parameter Bin 
       * - ESP32-C3-MINI Series
         - - GPIO7
           - GPIO6
           - GPIO5
           - GPIO4
         - ``factory_param_MINI-1.bin``

-   ``ble_data.bin`` provides Bluetooth LE services when the {IDF_TARGET_NAME} works as a Bluetooth LE server;
-   ``server_cert.bin``, ``server_key.bin`` and ``server_ca.bin`` are examples of SSL server's certificate;
-   ``client_cert.bin``, ``client_key.bin`` and ``client_ca.bin`` are examples of SSL client's certificate;
-   ``mqtt_cert.bin``, ``mqtt_key.bin`` and ``mqtt_ca.bin`` are examples of MQTT SSL client's certificate;

If some of the functions are not used, then the corresponding binaries need not to be downloaded into flash.
