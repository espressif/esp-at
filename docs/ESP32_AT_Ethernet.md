# Overview
Initialises the Ethernet interface and enables it, then sends DHCP requests and tries to obtain a DHCP lease. If successful then you will be able to ping the device.

# PHY Configuration
Use `make menuconfig` to set the PHY model. These configuration items will vary depending on the hardware configuration you are using.

The default configuration is correct for Espressif's Ethernet board with TP101 PHY. ESP32 AT supports up to four Ethernet PHY: `LAN8720`, `IP101`, `DP83848` and `RTL8201`.
`TLK110` PHY is no longer supported because TI stoped production.
If you want to use other phy, follow the [document](https://docs.espressif.com/projects/esp-idf/en/latest/hw-reference/get-started-ethernet-kit.html) to design.

## Compiling 

1. `make menuconfig` -> `Serial flasher config` to configure the serial port for downloading.
2. `make menuconfig` -> `Component config` -> `AT` -> `AT ethernet support` to enable ethernet. 
3. `make menuconfig` -> `Component config` -> `AT` -> `Ethernet PHY` to choose ethernet.
4. Recompile the `esp-at` project, download AT bin into flash.
