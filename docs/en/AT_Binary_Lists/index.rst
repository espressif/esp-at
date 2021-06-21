***************
AT Binary Lists
***************

:link_to_translation:`zh_CN:[中文]`

.. toctree::
   :maxdepth: 1
   
   ESP32 AT binaries <ESP32_AT_binaries>
   ESP32-C3 AT binaries <ESP32-C3_AT_binaries>
   ESP32-S2 AT binaries <ESP32-S2_AT_binaries>
   ESP8266 AT binaries <ESP8266_AT_binaries>

Each of the linked above ESP-AT-Bin files contains several binaries for some specific functions, and the factory/factory/xxx.bin is the combination of all binaries. So user can only download the ``factory/factory_xxx.bin`` to address 0, or several binaries to different addresses according to ``ESP-AT-Bin/download.config``.

-   ``at_customize.bin`` is to provide a user partition table, which lists different partitions for the ``ble_data.bin``, SSL certificates, and ``factory_param_XXX.bin``. Furthermore, users can add their own users partitions, and read/write the user partitions with the command ``AT+FS`` and ``AT+SYSFLASH``.
-   ``factory_param_XXX.bin`` indicates the hardware configurations for different ESP modules (see the table below). Please make sure the correct bin is used for your specific module. If users design their own module, they can configure it with reference to the ``esp-at/docs/en/Compile_and_Develop/How_to_create_factory_parameter_bin.md``, and the binaries will be automatically generated after compilation. When users flash the firmware into module according to the ``download.config``, the ``customized_partitions/factory_param.bin`` should be replaced with the actual module-specific ``customized_partitions/factory_param_XXX.bin``. UART CTS and RTS pins are optional.

    - **ESP32 Series**

    .. list-table::
       :header-rows: 1

       * - Modules
         - UART Pins (TX, RX, CTS, RTS)
         - Factory Parameter Bin 
       * - ESP32-WROOM-32 Series (ESP32 Default Value)
         - GPIO17, GPIO16, GPIO15, GPIO14
         - ``factory_param_WROOM-32.bin``
       * - ESP32-WROVER Series (Supports Classic Bluetooth)
         - GPIO22, GPIO19, GPIO15, GPIO14
         - ``factory_param_WROVER-32.bin``
       * - ESP32-PICO Series
         - GPIO22, GPIO19, GPIO15, GPIO14
         - ``factory_param_PICO-D4.bin``
       * - ESP32-SOLO Series
         - GPIO17, GPIO16, GPIO15, GPIO14
         - ``factory_param_SOLO-1.bin``

    - **ESP32S2 Series**

    .. list-table::
       :header-rows: 1

       * - Modules
         - UART Pins (TX, RX, CTS, RTS)
         - Factory Parameter Bin 
       * - ESP32S2-WROOM Series
         - GPIO17, GPIO21, GPIO20, GPIO19
         - ``factory_param_WROOM.bin``
       * - ESP32S2-WROVER Series
         - GPIO17, GPIO21, GPIO20, GPIO19
         - ``factory_param_WROVER.bin``
       * - ESP32S2-SOLO Series
         - GPIO17, GPIO21, GPIO20, GPIO19
         - ``factory_param_SOLO.bin``
       * - ESP32S2-MINI Series
         - GPIO17, GPIO21, GPIO20, GPIO19
         - ``factory_param_MINI.bin``

    - **ESP32-C3 Series**

    .. list-table::
       :header-rows: 1

       * - Modules
         - UART Pins (TX, RX, CTS, RTS)
         - Factory Parameter Bin 
       * - ESP32-C3-MINI Series
         - GPIO7, GPIO6, GPIO5, GPIO4
         - ``factory_param_MINI-1.bin``

    - **ESP8266 Series**

    .. list-table::
       :header-rows: 1

       * - Modules
         - UART Pins (TX, RX, CTS, RTS)
         - Factory Parameter Bin 
       * - ESP-WROOM-02 Series (ESP8266 Default Value)
         - GPIO15, GPIO13, GPIO3, GPIO1
         - ``factory_param_WROOM-02.bin``

-   ``ble_data.bin`` is to provide BLE services when the ESP32 works as a BLE server;
-   ``server_cert.bin``, ``server_key.bin`` and ``server_ca.bin`` are examples of SSL server‘s certificate;
-   ``client_cert.bin``, ``client_key.bin`` and ``client_ca.bin`` are examples of SSL client‘s certificate.

If some of the functions are not used, then the corresponding binaries need not to be downloaded into flash.
