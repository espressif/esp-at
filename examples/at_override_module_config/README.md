### 1. Description
This `at_override_module_config` example works as an external directory, allowing you to override the default module configurations (`esp-at/module_config/<your_module>`) for your module, and you will no longer need to modify the source code of the [esp-at repository](https://github.com/espressif/esp-at). This allows you to just host `at_override_module_config` directory in your git repository in a light-weight way, and it also allows you to freely choose any commit from the esp-at repository snapshot starting from March 2024 as the main project.

### 2. About the Override
Here are five optional configurations that you may want to override, and you can choose to override one or more of them. Below are detailed descriptions of each override.

**Notes:**
- If you don't override a configuration, the native configuration will be used.
- You must use the same filename and directory name to override.

#### 2.1 Override the system configuration
The native system configuration file is `esp-at/module_config/<your_module>/sdkconfig.defaults` (disable silence mode) or `esp-at/module_config/<your_module>/sdkconfig_silence.defaults` (enable silence mode). You may want to enable or disable some features, for example, to enable WebSocket functionality and disable mDNS functionality. You can add a new system configuration file to `at_override_module_config` directory, and add the following lines to `at_override_module_config/sdkconfig.defaults` file:

```
# Enable WebSocket and disable mDNS
CONFIG_AT_WS_COMMAND_SUPPORT=y
CONFIG_AT_MDNS_COMMAND_SUPPORT=n
```

The build system will override `at_override_module_config/sdkconfig.defaults` as your system configuration.

#### 2.2 Override the patch directory
The native patch directory is `esp-at/module_config/<your_module>/patch`. You may want to add more patches, for example, to output one prompt message after esp-at is ready. You can copy the native patch directory to `at_override_module_config` directory, and add the `at_example.patch` file to `at_override_module_config/patch`, then specify this patch by adding the following lines to `at_override_module_config/patch/patch_list.ini` file:

```
[at_example.patch]
    path = .
    note = "This is a simple example to demonstrate how to override the patch directory"
```

The build system will use `at_override_module_config/patch` as your patch configuration.

**Notes:**
- You can create the patch using either the `git format-patch` or `git diff` command.

#### 2.3 Override the system partition table
The native system partition table file is `esp-at/module_config/<your_module>/partitions_at.csv`. You may want to modify it, for example, to enlarge app partition (`ota_0`) size to support more features. You can copy the native system partition table to `at_override_module_config/partitions_at.csv`, and update the system partition table by replacing the following lines to `at_override_module_config/partitions_at.csv` file:

```
# Name, Type, SubType, Offset, Size
otadata, data, ota, 0xd000, 0x2000
phy_init, data, phy, 0xf000, 0x1000
nvs, data, nvs, 0x10000, 0xE000
at_customize, 0x40, 0, 0x1E000, 0x42000
ota_0, app, ota_0, 0x60000, 0x3a0000
```

The build system will use `at_override_module_config/partitions_at.csv` as your system partition table configuration.

**Notes:**
- In order to increase the size of the `ota_0` partition, the `ota_1` partition has been removed in this example. This means that you cannot use the OTA commands (`AT+USEROTA` and `AT+CIUPDATE`) provided by the AT firmware. Therefore, your host MCU needs to support firmware updates of esp-at via the [esptool protocol](https://docs.espressif.com/projects/esptool/en/latest/esp32/advanced-topics/serial-protocol.html). Otherwise, if any issues occur, the AT firmware will be unable to be updated.
- You need to override `sdkconfig.defaults` as you need to specify the relative path of the system partition table in `sdkconfig.defaults` using `CONFIG_PARTITION_TABLE_CUSTOM_FILENAME` and `CONFIG_PARTITION_TABLE_FILENAME`. The relative path is relative to `at_override_module_config` for `partitions_at.csv`.

#### 2.4 Override the secondary partition table
The native secondary partition table (also called user partition table) file is `esp-at/module_config/<your_module>/at_customize.csv`. You may want to modify it, for example, to enlarge manufacturing nvs partition (`mfg_nvs`) size to support more parameters. You can copy the native secondary partition table to `at_override_module_config/at_customize.csv`, and update the secondary partition table by replacing the following lines to `at_override_module_config/at_customize.csv` file:

```
# Name, Type, SubType, Offset, Size
mfg_nvs, data, nvs, 0x1f000, 0x41000
```

The build system will use `at_override_module_config/at_customize.csv` as your secondary partition table configuration.

**Notes:**
- In order to increase the size of the `mfg_nvs` partition, the `fatfs` partition has been removed in this example. This means that you cannot use the FileSystem command (`AT+FS`) provided by the AT firmware.
- You need to override `sdkconfig.defaults` as you need to specify the relative path of the secondary partition table in `sdkconfig.defaults` using `CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_FILE`. The relative path is relative to `at_override_module_config` for `at_customize.csv`.

#### 2.5 Override the esp-idf version
It is strongly recommended to use the default `ESP-IDF` version of the `ESP-AT` project, and **NOT** recommended to update it (because of the potential [application binary interface](https://en.wikipedia.org/wiki/Application_binary_interface) incompatibility between the underlying ESP-IDF versions of `libesp_at_core.a` and the esp-at repository, which may cause incorrect operation of the firmware.), but we still keep the option to update the ESP-IDF for testing purposes. The native esp-idf version file is `esp-at/module_config/<your_module>/IDF_VERSION`. You can copy the native esp-idf version to `at_override_module_config/IDF_VERSION`, and keep it as it is.

```
branch:release/v5.0
commit:bcca689866db3dfda47f77670bf8df2a7ec94721
repository:https://github.com/espressif/esp-idf.git
```

The build system will use `at_override_module_config/IDF_VERSION` as your esp-idf version configuration.

### 3. Usage
3.1 Based on the content of `2. About the Override` above and your needs, modify the configurations.

3.2 Add your `at_override_module_config` directory into the build system of esp-at project.

- Linux or macOS
```
export AT_EXT_MODULE_CFG=(path_of_at_override_module_config)
```

- Windows
```
set AT_EXT_MODULE_CFG=(path_of_at_override_module_config)
```

Notes:
- You need to replace (path_of_at_override_module_config) with your real absolute path of your `at_override_module_config` directory.

3.3 Compile your firmware according to the [Compile ESP-AT Project](https://docs.espressif.com/projects/esp-at/en/latest/esp32/Compile_and_Develop/How_to_clone_project_and_compile_it.html#compile-esp-at-project-locally) guide.
