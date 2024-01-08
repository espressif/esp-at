AT 固件
=================

:link_to_translation:`en:[English]`

.. toctree::
  :hidden:
  :maxdepth: 1

  {IDF_TARGET_NAME} AT 发布版固件 <esp_at_binaries>

本文档包含以下小节：

.. list::

  - :doc:`下载 {IDF_TARGET_NAME} AT 发布版固件 <esp_at_binaries>`
  - :ref:`brief-intro-firmware`：AT 固件包含哪些二进制文件及其作用
  - :ref:`firmware-selection`：不同类型的 AT 固件及其获取方式、适用情况、优缺点等
  - :ref:`what-next-after-you-get-firmware`

.. note::
  若需下载其他芯片系列的发布版固件，请在页面左上方的下拉菜单栏选择相应的芯片，即可跳转至该芯片的文档进行下载。

.. _brief-intro-firmware:

AT 固件简介
--------------

ESP-AT 固件包含了若干个特定功能的二进制文件： 

- ``factory/factory_xxx.bin`` 是这些特定功能的二进制文件的合集。您可以仅烧录 ``factory/factory_xxx.bin`` 到起始地址为 0 的 flash 空间中，或者根据 ``download.config`` 文件中的信息将若干个二进制文件烧录到 flash 中对应起始地址的空间中。
-  ``at_customize.bin`` 提供了用户分区表，该表列出了 ``mfg_nvs`` 分区、以及可能的 ``fatfs`` 分区的起始地址和分区大小。您可以通过 AT 命令 :ref:`AT+FS <cmd-FS>` 和 :ref:`AT+SYSFLASH <cmd-SYSFLASH>` 来读和写该文件中罗列的分区里的内容。
-  ``mfg_nvs.bin`` 指明了不同 {IDF_TARGET_NAME} 模组之间的硬件配置（见下表）。请确保您的模组使用了正确的固件。更多有关该参数文件的信息请参考 :ref:`firmware-modify-paras-not-source-code`。

  .. only:: esp32

    .. list-table:: {IDF_TARGET_NAME} 模组的硬件配置
       :header-rows: 1

       * - 模组
         - UART 管脚（TX、RX、CTS、RTS）
       * - ESP32-WROOM-32 系列（ESP32 默认模组）
         - - GPIO17
           - GPIO16
           - GPIO15
           - GPIO14
       * - ESP32-WROVER 系列（支持经典蓝牙）
         - - GPIO22
           - GPIO19
           - GPIO15
           - GPIO14
       * - ESP32-PICO 系列
         - - GPIO22
           - GPIO19
           - GPIO15
           - GPIO14
       * - ESP32-SOLO 系列
         - - GPIO17
           - GPIO16
           - GPIO15
           - GPIO14

  .. only:: esp32c2

      .. list-table:: {IDF_TARGET_NAME} 模组的硬件配置
        :header-rows: 1

        * - 模组
          - UART 管脚（TX、RX、CTS、RTS）
        * - ESP32C2-4MB 系列
          - - GPIO7
            - GPIO6
            - GPIO5
            - GPIO4

  .. only:: esp32c3

      .. list-table:: {IDF_TARGET_NAME} 模组的硬件配置
        :header-rows: 1

        * - 模组
          - UART 管脚（TX、RX、CTS、RTS）
        * - ESP32-C3-MINI-1 系列
          - - GPIO7
            - GPIO6
            - GPIO5
            - GPIO4

  .. only:: esp32c6

      .. list-table:: {IDF_TARGET_NAME} 模组的硬件配置
        :header-rows: 1

        * - 模组
          - UART 管脚（TX、RX、CTS、RTS）
        * - ESP32C6-4MB 系列
          - - GPIO7
            - GPIO6
            - GPIO5
            - GPIO4

.. _firmware-selection:

我该选哪种类型的固件？
-----------------------

ESP-AT 固件有以下几种类型，其中下载或准备固件的工作量自上而下依次递增，支持的模组类型也自上而下依次递增。

.. contents::
   :local:
   :depth: 1

.. _official-released-firmware:

官方发布版固件（推荐）
^^^^^^^^^^^^^^^^^^^^^^^^

**官方发布版固件** 又称“发布版固件”、“官方固件”、“默认固件”，为乐鑫官方团队测试并发布的固件，固件会根据内部开发计划周期性发布，此种固件可直接基于乐鑫 OTA 服务器升级固件。如果 **官方发布版固件** 完全满足您的项目需求，建议您优先选择 **官方发布版固件**。如果官方固件不支持您的模组，您可以根据 :doc:`硬件差异 <../Compile_and_Develop/esp-at_firmware_differences>`，选择和您的模组硬件配置相近的固件进行测试验证。

.. list::

  - 获取途径：:doc:`{IDF_TARGET_NAME} AT 固件 <esp_at_binaries>`
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
^^^^^^^^^^^^^^^^^^^^^^

**GitHub 临时固件** 为每次将代码推送到 GiHub 时都会生成但并未达到固件发布周期条件的固件，或者说是开发中的固件，包括 **官方发布版固件** 的临时版本和适配过但是不计划正式发布的固件，其中前者可直接基于乐鑫 OTA 服务器升级固件。
  
- 获取途径：请参考 :doc:`../Compile_and_Develop/How_to_download_the_latest_temporary_version_of_AT_from_github`。
- 优点：

  - 实时性强，新的特性和漏洞修补都会实时同步出来。
  - 包含一些非正式发布的固件，如基于 SDIO 通讯的固件、基于 SPI 通讯的固件。
  - 获取固件工作量小。

- 缺点：基于非正式发布的 commit 生成的固件未经过完整的测试，可能会存在一些风险，需要您自己做完整的测试。

.. _firmware-modify-paras-not-source-code:

修改参数的固件
^^^^^^^^^^^^^^^^^^^^^^

**修改参数的固件** 指的是只修改参数区域而并不需要重新编译的固件，适用于固件功能满足项目要求、但只有某些参数不满足的情况下，如出厂波特率、UART IO 管脚的等参数的变更，此种固件可直接基于乐鑫 OTA 服务器升级固件。
  
- 关于如何修改参数文件，请参考 :doc:`../Compile_and_Develop/tools_at_py`。
- 优点：

  - 不需要重新编译固件。
  - 固件稳定、可靠。

- 缺点：需要基于发布版的固件修改，更新周期长，覆盖的模组有限。

.. _self-compiled-firmware:

自行编译的固件
^^^^^^^^^^^^^^^^^

当您需要进行二次开发时可采用此种方式。需要自己部署 OTA 服务器以支持 OTA 功能。

- 关于如何自行编译固件，请参考 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`。
- 优点：功能、周期自己可控。
- 缺点：需要自己搭建环境编译。

.. _what-next-after-you-get-firmware:

获取固件后，接来下做什么？
----------------------------------

当您获取到固件后，请参考 :doc:`硬件连接 <../Get_Started/Hardware_connection>` 与 :doc:`固件下载及烧录指南 <../Get_Started/Downloading_guide>` 连接 PC 和 ESP 设备、并将固件烧录至设备。
