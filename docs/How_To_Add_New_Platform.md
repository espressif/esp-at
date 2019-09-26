# How to Add a New Platform

  
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



## 如何增加一个新的平台支持

# 简介

当前工程根据不同的模组采用了不同的配置方式，具体配置信息在 `module_config` 目录下，如果未指定对应的模组配置信息，将采用平台默认的配置信息，现在已支持 esp32 和 esp8266 平台。工程默认为 `PLATFORM_ESP32` 平台的 `WROOM-32` 模组。
对于同款芯片不同通讯接口的模组，由于在 esp-at 中编译的代码不同，我们不能采用默认的 module_espxxxx_default 配置。

假设新的平台为 ESP32 SDIO AT，我们需要使用 SDIO 作为通讯介质，我们以此为例，进行阐述如何添加新的平台设备。

## 1. 创建模块信息
假设平台名称为 `PLATFORM_ESP32`，模块名称为 `WROOM32-SDIO`，打开 `components/customized_partitions/raw_data/factory_param/factory_param_data.csv`,按照标题

| platform | module_name | magic_flag | version | module_id | tx_max_power | start_channel | channel_num | country_code | uart_baudrate | uart_tx_pin | uart_rx_pin | uart_ctx_pin | uart_rts_pin | tx_control_pin | rx_control_pin
|---|---|---|---|---|---|---| ---|---|---|---|---|---|---|---|---|

在最后添加

| PLATFORM_ESP32 | WROOM32-SDIO | 0xfcfc | 1 | 1 | 1 | 1 | 13 | CN | -1 | -1 | -1 | -1 | -1 | -1 | -1|
|---|---|---|---|---|---|---| ---|---|---|---|---|---|---|---|---|

## 2. 修改工程模块信息
打开 `Makefile`，修改平台名称和模块名称，对于英文字母，请使用大写格式

```
export ESP_AT_PROJECT_PLATFORM ?= PLATFORM_ESP32
export ESP_AT_MODULE_NAME ?= WROOM32-SDIO
```
**
## 3. 创建平台相关的配置
- 3.1 进入 `module_config`, 将同款芯片的默认配置 `module_esp32_default` 拷贝一份为 `module_esp32-sdio`  
- 3.2 此处我们不需要修改 partition 分区表和 IDF 版本，所以 `at_customize.csv`、`IDF_VERSION` 和 `partitions_at.csv` 都不做修改  
- 3.3 修改 `sdkconfig.defaults` 文件
	- 配置使用 `module_esp32-sdio` 目录下的分区表文件，需要修改如下配置：
	
	```
	CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="module_config/module_esp32-sdio/partitions_at.csv"

	CONFIG_PARTITION_TABLE_FILENAME="module_config/module_esp32-sdio/partitions_at.csv"

	CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_FILE="module_config/module_esp32-sdio/at_customize.csv"
	```
	- 使用 sdio 配置，由于工程中已经包含选择 sdio 的配置，所以我们只需要将选择 sdio 的配置加入到 `sdkconfig.defaults` 文件即可
		- 移除 UART AT 相关配置
		
		```
		CONFIG_AT_BASE_ON_UART=y
		```
		
		并新增
		
		```
		CONFIG_AT_BASE_ON_SDIO=y
		```

## 4. 修改链接的库文件

由于 ESP32 SDIO AT 和 ESP32 UART AT 是同一个平台，使用的是相同的 at core 库，所以我们不需要再新增库文件。
若需要使用新的 lib, 则将 lib 复制到 `components/at/lib` 目录下,并将 lib 命名为 `libxxxx_at_core.a`, 其中 `xxxx` 为平台名称。假如根目录下的 `Makefile` 设置的 `ESP_AT_PROJECT_PLATFORM ?= PLATFORM_ESP8848`，那么库的名称就要命名为 `libesp8848_at_core.a`。
