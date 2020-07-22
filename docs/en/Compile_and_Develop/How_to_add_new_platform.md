How to Add a New Platform
=========================
  
Since the esp-at project supports different platform, for example, ESP32 UART AT, ESP32 SDIO AT, even supports ESP8266 AT, when compiling the esp-at project, users can set different configurations to generate AT firmware for different ESP modules. The detailed information are in the `esp-at/module_config` directory. Default configuration is the "PLATFORM_ESP32" for ESP-WROOM-32.  
Please note that if you use a different bus (for example, SDIO, SPI or other buses) instead of UART, then you cannot use the default `module_espxxxx_default` directly, you need to re-configure it in the `menuconfig`.  
Herein, we provide an example of the ESP32 SDIO AT to show how to set a new platform for the esp-at project.

## 1. Create a New Platform

For example, to name the platform as "PLATFORM_ESP32", the module as "WROOM32-SDIO", we need to open the `components/customized_partitions/raw_data/factory_param/factory_param_data.csv` and add a new row of the new platform at the end.

| platform | module_name | magic_flag | version | module_id | tx_max_power | start_channel | channel_num | country_code | uart_baudrate | uart_tx_pin | uart_rx_pin | uart_ctx_pin | uart_rts_pin | tx_control_pin | rx_control_pin
|---|---|---|---|---|---|---| ---|---|---|---|---|---|---|---|---|
|-xxx-|-xxx-|-xxx-|-xxx-|-xxx-|-xxx-|-xxx-| -xxx-|-xxx-|-xxx-|-xxx-|-xxx-|-xxx-|-xxx-|-xxx-|-xxx-|
| PLATFORM_ESP32 | ESP32-SDIO | 0xfcfc | 1 | 1 | 1 | 1 | 13 | CN | -1 | -1 | -1 | -1 | -1 | -1 | -1

## 2. Set Makefile
Open the `Makefile` and set to the new platform. Please use capital letters. 

```
export ESP_AT_PROJECT_PLATFORM ?= PLATFORM_ESP32
export ESP_AT_MODULE_NAME ?= ESP32-SDIO
```

## 3. Configure the New Platform
- 3.1. Enter `module_config` folder, copy the `module_esp32_default` to make a new `module_esp32-sdio`.  
- 3.2. In this example, we need not to change the partition table and the ESP-IDF  version, so the `at_customize.csv`, `IDF_VERSION` and `partitions_at.csv` all need not to be changed.  
- 3.3. Revise the `sdkconfig.defaults`
	- Configure to use the partition table in the `module_esp32-sdio` folder, revise the following items：
	
	```
	CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="module_config/module_esp32-sdio/partitions_at.csv"

	CONFIG_PARTITION_TABLE_FILENAME="module_config/module_esp32-sdio/partitions_at.csv"

	CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_FILE="module_config/module_esp32-sdio/at_customize.csv"
	```
	- Since the esp-at project already supports the SDIO configuration, we only need to add it into the `sdkconfig.defaults`.
		- Remove the UART AT configuration in the `sdkconfig.defaults`.
		
		```
		CONFIG_AT_BASE_ON_UART=y
		CONFIG_AT_UART_PORT=1
		CONFIG_AT_UART_PORT_RX_PIN=16
		CONFIG_AT_UART_PORT_TX_PIN=17
		CONFIG_AT_UART_PORT_RTS_PIN=14
		CONFIG_AT_UART_PORT_CTS_PIN=15
		```
		
		And add the following configuration. 
		
		```
		CONFIG_AT_BASE_ON_SDIO=y
		```

## 4. Revise the at_core lib

Since the ESP32 SDIO AT and the ESP32 UART AT are based on the same platform, ESP32, they will share the same at_core.lib. In this case, we need not to add any new at\_core lib.  
If you need to use a new at\_core lib, put the lib into the `components/at/lib`, rename the lib as `libxxxx_at_core.a`, `xxxx` is the platform name. For example, if you set the `ESP_AT_PROJECT_PLATFORM ?= PLATFORM_ESP8848` in the `Makefile` in Step 2, then the lib should be named as `libesp8848_at_core.a`.

