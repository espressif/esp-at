# ESP-AT
[![alt text][doc-latest]](https://docs.espressif.com/projects/esp-at/en/latest/)

[doc-latest]: https://img.shields.io/badge/docs-latest-blue

- [中文版](./README_CN.md)

esp-at project was started and powered by Espressif Systems (@[espressif](https://github.com/espressif/)) as an official project, for the **ESP32-C2**, **ESP32-C3**, **ESP32-C6**, **ESP32**, and **ESP32-S2** Series SoCs provided for Windows, Linux, and macOS.  
It is now supported and maintained by Espressif esp-at team (@[esp-at](https://github.com/espressif/esp-at)).

# Introduction
Espressif SoC chipsets are often used as add-on modules to seamlessly integrate wireless connectivity features into new and existing products. In an effort to facilitate this and cut down on engineering costs, Espressif Systems has developed a set of [AT commands](https://docs.espressif.com/projects/esp-at/en/latest/esp32/AT_Command_Set/index.html) (Please select your target chip from the dropdown menu in the top left corner of the page) that can be used to interface with Espressif products.

The AT solution provided by Espressif allows for rapid integration by providing:

- In-built a large number of AT commands
- Easy integration with resource-constrained host platforms
- Easy-to-parse command-response protocols
- Customized, user-defined AT commands

# ESP-AT Release and SoC Compatibility
The following table shows ESP-AT support status of Espressif SoCs.

| Chip | v3.2.0.0 | v3.3.0.0 | v3.4.0.0 | v4.0.0.0 | master |
|:- |:-:| :-:| :-:| :-:| :-:|
| ESP32 | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.2.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.3.0.0) | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.4.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v4.0.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/master) |
|ESP32-C3 | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.2.0.0) | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.3.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.4.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v4.0.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/master) |
|ESP32-C2 | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.2.0.0) | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.3.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.4.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v4.0.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/master) |
|ESP32-C6 | | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.3.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.4.0.0) | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v4.0.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/master) |
|ESP32-S2 | | |  [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.4.0.0) | | [![alt text][supported]](https://github.com/espressif/esp-at/tree/master) |

- ![alt text][released]: Released versions, you can click ![alt text][released] to go to the corresponding release page and download the AT firmware.
- ![alt text][supported]: Supported versions, you can click ![alt text][supported] to go to the corresponding branch and develop your own AT firmware.
- ![alt text][pre-release]: Pre-release versions, expected to release AT firmware for the corresponding chip soon.

Please choose one of the following options based on your needs:
- **The default AT firmware meets your requirements**: It is recommended to use the latest released version of the firmware corresponding to the chip.
- **The default AT firmware does not meet your requirements and stability is a priority**: It is recommended to develop your AT firmware based on the latest released version of the firmware corresponding to the chip.
- **The default AT firmware does not meet your requirements and you want more new features**: It is recommended to develop your AT firmware based on the [master branch](https://github.com/espressif/esp-at/tree/master).

[released]: https://img.shields.io/badge/-released-4CBB17
[supported]: https://img.shields.io/badge/-supported-1F51FF
[pre-release]: https://img.shields.io/badge/-pre--release-FFC000

**Other series of chips:**
- ESP-AT will support ESP32-C5 series of chips.
- ESP-AT will not support ESP32-S3 and ESP32-H series of chips.

# Support Periods of ESP-AT

![support periods](docs/_static/at-support-periods.png)

- Please refer to [ESP-AT Versions](https://docs.espressif.com/projects/esp-at/en/latest/esp32/versions.html) for more details。

# Resources
- There are several guides for esp-at developers and users. These guides can be rendered in a number of formats, like HTML and PDF.  
  Documentation for the latest version: [https://docs.espressif.com/projects/esp-at/en/latest/index.html](https://docs.espressif.com/projects/esp-at/en/latest/index.html). This documentation is built from the [docs directory](https://github.com/espressif/esp-at/tree/master/docs) of this repository.

- The Changelogs of historic released versions: https://github.com/espressif/esp-at/releases

- [Check the Issues section on GitHub](https://github.com/espressif/esp-at/issues) if you find a bug or have a feature request. Please check existing Issues before opening a new one.

- The [esp-at forum](https://www.esp32.com/viewforum.php?f=42) is a place to ask questions and find community resources.

- The [ESP Product Selector](https://products.espressif.com/#/product-selector?language=en&names=) is a tool to help you compare different chips or modules.
