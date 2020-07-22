ESP-AT Bin Introduction
=======================

The ESP-AT-Bin contains several binaries for some specific functions, and the factory/factory_xxx.bin is the combination of all binaries. So user can only download the `factory/factory_xxx.bin` to address 0, or user can download several binaries to different addresses according to `ESP-AT-Bin/download.config`.  

 * `at_customize.bin` is to provide a user partition table, which lists different partitions for the `ble_data.bin`, SSL certificates, and `factory_param_XXX.bin`. Furthermore, users can add their own users partitions, and read/write the user partitions with the command `AT+FS` and `AT+SYSFLASH`.
 * `factory_param_XXX.bin` indicates the hardware configurations for different ESP modules. Please make sure the correct bin is used for your specific module. If users design their own module, they can configure it with reference to the `esp-at/docs/ESP32_AT_Factory_Parameter_Bin.md`, and the binaries will be automatically generated after compilation. When users flash the firmware into module according to the `download.config`, the `customized_partitions/factory_param.bin` should be replaced with the actual module-specific `customized_partitions/factory_param_XXX.bin`. UART CTS and RTS are optional pins, not compulsive.

|Modules| UART Pins（TX, RX, CTS, RTS） |Factory Parameter Bin
|---|---|---|
| ESP32-WROOM-32 Series (ESP32 Default Value)  | GPIO17, GPIO16, GPIO15, GPIO14  | `customized_partitions/factory_param_WROOM-32.bin` |  
| ESP32-WROVER Series(Support Classic Bluetooth)  | GPIO22, GPIO19, GPIO15, GPIO14 | `customized_partitions/factory_param_WROVER-32.bin` |  
| ESP32-PICO Series  | GPIO22, GPIO19, GPIO15, GPIO14  | `customized_partitions/factory_param_PICO-D4.bin` |
|ESP32-SOLO Series  | GPIO17, GPIO16, GPIO15, GPIO14 | `customized_partitions/factory_param_SOLO-1.bin` |
| ESP-WROOM-02 Series (ESP8266 Default Value)  | GPIO15, GPIO13, GPIO3, GPIO1 | `customized_partitions/factory_param_WROOM-02.bin` | 

 * `ble_data.bin` is to provide BLE services when the ESP32 works as a BLE server;
 * `server_cert.bin`, `server_key.bin` and `server_ca.bin` are examples of SSL server‘s certificate; `client_cert.bin`, `client_key.bin` and `client_ca.bin` are examples of SSL client‘s certificate.  

If some of the functions are not used, then the corresponding binaries need not to be downloaded into flash.  
