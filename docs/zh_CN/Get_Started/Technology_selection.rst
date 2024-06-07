技术选型
========

:link_to_translation:`en:[English]`

本文档主要介绍如何选择合适的乐鑫硬件产品、AT 软件方案、以及项目初期的准备。

.. important::
  如果您在选择乐鑫硬件产品、AT 软件方案中有任何问题，请联系 `乐鑫商务 <https://www.espressif.com/zh-hans/contact-us/sales-questions>`_ 或者 `技术支持 <https://www.espressif.com/zh-hans/contact-us/technical-inquiries>`_。

硬件选型
------------

在开始使用 ESP-AT 之前，您需要选择一款合适的乐鑫芯片集成到您的产品中，为您的产品赋能无线功能。硬件选型是一个复杂的过程，需要考虑多方面的因素，如功能、功耗、成本、尺寸等。请阅读下面内容帮助您选型硬件。

.. list::

  - `产品选型工具 <https://products.espressif.com/#/product-selector?language=zh&names=>`_ 可以帮助您了解不同乐鑫产品的硬件区别。
  - `技术规格书 <https://www.espressif.com/zh-hans/support/documents/technical-documents?keys=&field_download_document_type_tid%5B%5D=510>`_ 可以帮助您了解该芯片/模组所支持的硬件能力。
  - `硬件选型入门指导 <https://docs.espressif.com/projects/esp-techpedia/zh_CN/latest/esp-friends/get-started/board-selection.html>`_ 可以帮助您简要对比芯片差别，了解芯片、模组、和开发板的差别以及选择指南。

.. note::
  **技术规格书中载明的是硬件最大能力，不代表 AT 软件能力**。例如，ESP32-C6 芯片支持 Zigbee 3.0 及 Thread 1.3，但是现有的 AT 软件方案暂未支持这两种无线协议。

.. _at-solution-selection:

AT 软件方案选型
--------------------

AT 软件方案是乐鑫针对不同芯片提供的 AT 固件，可以帮助您快速实现无线功能。

- 如果您想了解 {IDF_TARGET_NAME} 芯片详细的 AT 软件能力，请参考 :doc:`AT 命令集 <../AT_Command_Set/index>`。
- 如果您想对比 {IDF_TARGET_NAME} 芯片不同的 AT 固件功能，请参考 :doc:`ESP-AT 固件差异 <../Compile_and_Develop/esp-at_firmware_differences>`。

下表列出了不同芯片对应的 AT 固件简要对比图。

.. list-table::
  :header-rows: 1

  * - 芯片
    - 无线功能
    - 推荐的 AT 固件
    - 说明
  * - ESP32-C6
    - **Wi-Fi 6** + BLE 5.0
    - `v4.0.0.0 <https://github.com/espressif/esp-at/releases/tag/v4.0.0.0>`_
    -
  * - ESP32-C3
    - Wi-Fi 4 + BLE 5.0
    - `v3.3.0.0 <https://github.com/espressif/esp-at/releases/tag/v3.3.0.0>`_
    -
  * - ESP32-C2
    - Wi-Fi 4 (或 BLE 5.0)
    - `v3.3.0.0 <https://github.com/espressif/esp-at/releases/tag/v3.3.0.0>`_
    -
  * - ESP32
    - Wi-Fi 4 + BLE v4.2 (+ **BT**)
    - `v3.4.0.0 <https://github.com/espressif/esp-at/releases/tag/v3.4.0.0>`_
    -
  * - ESP32-S2
    - Wi-Fi 4
    - `v3.4.0.0 <https://github.com/espressif/esp-at/releases/tag/v3.4.0.0>`_
    - 推荐使用性价比更高的 ESP32-C 系列

- ``(或 BLE 5.0)`` 表示 AT 软件方案中支持低功耗蓝牙功能，但发布的固件中未包含此功能。
- ``(+ BT)`` 表示 AT 软件方案中支持经典蓝牙功能，但发布的固件中未包含此功能。

.. note::
  出厂的模组或芯片中，均未烧录 AT 固件。若您有量产需求，请及时联系对接的商务人员或 sales@espressif.com，我们将提供定制生产。

.. _firmware-selection:

我该选哪种类型的 AT 固件？
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ESP-AT 固件有以下几种类型，其中下载或准备固件的工作量自上而下依次递增，支持的模组类型也自上而下依次递增。

.. contents::
   :local:
   :depth: 1

.. _official-released-firmware:

官方发布版固件（推荐）
""""""""""""""""""""""""

**官方发布版固件** 又称“发布版固件”、“官方固件”、“默认固件”，为乐鑫官方团队测试通过并发布的固件，固件会根据内部开发计划周期性发布，此种固件可直接基于乐鑫 OTA 服务器升级固件。如果 **官方发布版固件** 完全满足您的项目需求，建议您优先选择 **官方发布版固件**。如果官方固件不支持您的模组，您可以根据 :doc:`硬件差异 <../Compile_and_Develop/esp-at_firmware_differences>`，选择和您的模组硬件配置相近的固件进行测试验证。

.. list::

  - 获取途径：:doc:`{IDF_TARGET_NAME} AT 固件 <../AT_Binary_Lists/esp_at_binaries>`
  - 优点：
  
    - 稳定
    - 可靠
    - 获取固件工作量小
  
  - 缺点：
    
    - 更新周期长
    - 覆盖的模组有限
  
  - 参考文档：
    
    - :doc:`硬件连接 <../Get_Started/Hardware_connection>`
    - :doc:`固件下载及烧录指南 <../Get_Started/Downloading_guide>`
    - 有关 ESP-AT 固件支持/不支持哪些芯片系列，请参考 ESP-AT GitHub 首页 `readme.md <https://github.com/espressif/esp-at>`_

.. _github-temporary-firmware:

GitHub 临时固件
""""""""""""""""""""""""

**GitHub 临时固件** 为每次将代码推送到 GiHub 时都会生成但并未达到固件发布周期条件的固件，或者说是开发中的固件，包括 **官方发布版固件** 的临时版本和适配过但是不计划正式发布的固件，其中前者可直接基于乐鑫 OTA 服务器升级固件。
  
- 获取途径：请参考 :doc:`../Compile_and_Develop/How_to_download_the_latest_temporary_version_of_AT_from_github`。
- 优点：

  - 实时性强，新的特性和漏洞修补都会实时同步出来。
  - 包含一些非正式发布的固件，如基于 SDIO 通讯的固件、基于 SPI 通讯的固件。
  - 获取固件工作量小。

- 缺点：基于非正式发布的 commit 生成的固件未经过完整的测试，可能会存在一些风险，需要您自己做完整的测试。

.. _firmware-modify-paras-not-source-code:

修改参数的固件
""""""""""""""""""""""""

**修改参数的固件** 指的是只修改参数区域而并不需要重新编译的固件，适用于固件功能满足项目要求、但只有某些参数不满足的情况下，如 UART 波特率、UART GPIO 管脚等参数的变更，此种固件可直接基于乐鑫 OTA 服务器升级固件。
  
- 关于如何修改参数文件，请参考 :doc:`../Compile_and_Develop/tools_at_py`。
- 优点：

  - 不需要重新编译固件。
  - 固件稳定、可靠。

- 缺点：需要基于发布版的固件修改，更新周期长，覆盖的模组有限。

.. _self-compiled-firmware:

自行编译的固件
""""""""""""""""""""""""

当您需要进行二次开发时可采用此种方式。需要自己部署 OTA 服务器以支持 OTA 功能。

- 关于如何自行编译固件，请参考 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`。
- 优点：功能、周期自己可控。
- 缺点：需要自己搭建环境编译。

如果您希望稳定性优先，推荐基于该芯片最新已发布的版本对应的分支开发您的 AT 固件。如果您希望更多新功能，推荐基于 `master 分支 <https://github.com/espressif/esp-at/tree/master>`_ 开发您的 AT 固件。

项目初期准备
------------

项目初期准备阶段， **强烈建议** 您选择 `乐鑫开发板 <https://www.espressif.com/zh-hans/products/devkits>`_ 开始您的项目。在项目初期，能够帮助您快速原型验证，评估硬件和软件能力，减少项目风险；在项目中期，能够帮助您快速功能集成和验证，性能优化，提高开发效率；在项目后期，能够帮助您快速模拟和定位问题，实现产品快速迭代。

如果您是 :ref:`self-compiled-firmware`，建议您优先选择 Linux 系统作为开发环境。
