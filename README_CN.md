# ESP-AT
[![alt text][doc-latest]](https://docs.espressif.com/projects/esp-at/en/latest/)

[doc-latest]: https://img.shields.io/badge/docs-latest-blue

- [English Version](./README.md)

esp-at 作为由 Espressif Systems (@[espressif](https://github.com/espressif/)) 发起和提供技术支持的官方项目，适用于 Windows、Linux、macOS 上的 **ESP32-C2**、**ESP32-C3**、**ESP32-C6**、**ESP32**、和 **ESP32-S2** 系列芯片。  
当前该项目由 Espressif esp-at 团队 (@[esp-at](https://github.com/espressif/esp-at)) 负责技术支持和维护。  

# 简介
乐鑫 SoC 芯片可以用作附加模块，完美集成在其他现有产品上，提供丰富的无线功能。为降低客户开发成本，乐鑫开发了一套 [AT 命令集](https://docs.espressif.com/projects/esp-at/zh_CN/latest/esp32/AT_Command_Set/index.html)（请在页面左上方的下拉菜单中选择您的目标芯片），方便客户简单快速地使用 AT 命令来控制芯片。

乐鑫提供的 AT 方案具有以下特色，利于芯片集成到应用中：

- 内置丰富的 AT 命令集
- 能便捷地集成到资源受限的主机平台中
- 主机对命令的回应易于解析
- 用户可自定义 AT 命令

# ESP-AT 与乐鑫芯片
下表总结了乐鑫芯片在 ESP-AT 主要版本中的支持状态。

| 芯片 | v3.2.0.0 | v3.3.0.0 | v3.4.0.0 | v4.0.0.0 | master |
|:- |:-:| :-:| :-:| :-:| :-:|
| ESP32 | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.2.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.3.0.0) | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.4.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v4.0.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/master) |
|ESP32-C3 | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.2.0.0) | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.3.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.4.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v4.0.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/master) |
|ESP32-C2 | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.2.0.0) | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.3.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.4.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v4.0.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/master) |
|ESP32-C6 | | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.3.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/release/v3.4.0.0) | [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v4.0.0.0) | [![alt text][supported]](https://github.com/espressif/esp-at/tree/master) |
|ESP32-S2 | | |  [![alt text][released]](https://github.com/espressif/esp-at/releases/tag/v3.4.0.0) | | [![alt text][supported]](https://github.com/espressif/esp-at/tree/master) |

- ![alt text][released]：已发布的版本，您可以点击 ![alt text][released] 前往对应的发布页下载 AT 固件。
- ![alt text][supported]：已支持的版本，您可以点击 ![alt text][supported] 前往对应的分支开发您自己的 AT 固件。
- ![alt text][pre-release]：在测试的版本，预计近期发布对应芯片的 AT 固件。

请根据您的需求，从以下选项中任选其一：
- **默认 AT 固件已满足您的需求**：推荐选用该芯片最新已发布的版本对应的 AT 固件。
- **默认 AT 固件不满足您的需求且希望稳定性优先**：推荐基于该芯片最新已发布的版本对应的分支开发您的 AT 固件。
- **默认 AT 固件不满足您的需求且希望更多新功能**：推荐基于 [master 分支](https://github.com/espressif/esp-at/tree/master) 开发您的 AT 固件。

[released]: https://img.shields.io/badge/-已发布-4CBB17
[supported]: https://img.shields.io/badge/-已支持-1F51FF
[pre-release]: https://img.shields.io/badge/-待发布-FFC000

**其它系列芯片：**
- ESP-AT 将计划支持 ESP32-C5 芯片。
- ESP-AT 无计划支持 ESP32-S3、ESP32-H 系列芯片。

# ESP-AT 版本支持期限

![support periods](docs/_static/at-support-periods.png)

- 请参考 [ESP-AT 版本简介](https://docs.espressif.com/projects/esp-at/zh_CN/latest/esp32/versions.html) 了解更多关于 ESP-AT 版本的信息。

# 资源
- 这里为开发者和用户提供了一些指南，这些指南可以以多种格式呈现，比如 HTML 和 PDF。  
  最新文档见：[https://docs.espressif.com/projects/esp-at/zh_CN/latest/index.html](https://docs.espressif.com/projects/esp-at/zh_CN/latest/index.html)。该文档是由本仓库 [docs 目录](https://github.com/espressif/esp-at/tree/master/docs) 自动编译构建的。

- 已发布版本的修改记录见：https://github.com/espressif/esp-at/releases

- 如果你有 bug 上报或者功能需求，可以在 [GitHub Issues](https://github.com/espressif/esp-at/issues) 里提交。请在提交前检索下是否有相同的 Issue。

- [esp-at 社区](https://www.esp32.com/viewforum.php?f=34) 可以用来询问问题或寻找一些社区资源。

- [产品选型工具](https://products.espressif.com/#/product-selector?language=zh&names=) 可以帮助你对比不同的芯片或模组。
