编译 ESP-AT 工程
=============================

{IDF_TARGET_HYPHEN_LOWERCASE_NAME: default="undefined", esp32="esp32", esp32c3="esp32-c3"}
{IDF_TARGET_COMPILE_MNAME: default="undefined", esp32="WROOM-32", esp32c3="MINI-1"}
{IDF_TARGET_PRODUCT_NAME: default="undefined", esp32="ESP32-WROOM-32D", esp32c3="ESP32-C3-MINI-1"}

:link_to_translation:`en:[English]`

本文档详细介绍了如何编译 ESP-AT 工程，并将生成的固件烧录到 ESP 设备中。当默认的 :doc:`官方发布的固件 <../AT_Binary_Lists/index>` 无法满足需求时，如您需要自定义 :doc:`AT 端口管脚 <How_to_set_AT_port_pin>`、:doc:`低功耗蓝牙服务 <How_to_customize_BLE_services>` 以及 :doc:`分区 <How_to_customize_partitions>` 等，那么就需要编译 ESP-AT 工程。

文档结构如下所示。

- :ref:`build-project-overview`：ESP-AT 工程编译步骤概述。
- :ref:`build-project-series-steps`：{IDF_TARGET_NAME} 系列 AT 工程编译的详细步骤。

.. _build-project-overview:

概述
--------

在编译 ESP-AT 工程之前，首先需要学习使用 ESP-IDF，并创建 ESP-IDF 的编译环境，这是因为 ESP-AT 是基于 ESP-IDF 的工程。

环境创建完成后，安装工具和 ESP-AT SDK。然后，将 ESP 设备连接至 PC，使用 ``./build.py menuconfig`` 配置工程。最后，编译工程，并将生成的 bin 文件烧录到 ESP 设备上。

.. note::

  **请注意可能出现管脚冲突**，如果选择 ``AT through HSPI``，那么可通过 ``./build.py menuconfig`` --> ``Component config`` --> ``AT`` --> ``AT hspi settings`` 获取 HSPI 管脚信息。

.. _build-project-series-steps:

{IDF_TARGET_NAME} 系列
-----------------------------------

本节介绍了如何为 {IDF_TARGET_NAME} 系列编译 ESP-AT 工程。

ESP-IDF 快速入门
^^^^^^^^^^^^^^^^^^^^^^^^

在编译 ESP-AT 工程之前，请先学习使用 ESP-IDF，因为 ESP-AT 是基于 ESP-IDF 开发的，不同系列的 AT 工程支持的 ESP-IDF 版本也不尽相同：

.. include:: ../../inline_substitutions

.. list-table:: 不同系列支持的 ESP-IDF 版本
   :header-rows: 1

   * - 工程
     - IDF 版本
     - IDF 文档版本
   * - {IDF_TARGET_NAME} ESP-AT
     - release/v4.3
     - `ESP-IDF 快速入门文档 v4.3 <https://docs.espressif.com/projects/esp-idf/en/release-v4.3/{IDF_TARGET_PATH_NAME}/get-started/index.html>`__

首先，按照 *ESP-IDF 快速入门* 文档中的第 1 至 4 步，为 ESP-IDF 创建开发环境（点击上表中的相应链接跳转至文档）。

然后，根据 *ESP-IDF 快速入门* 文档中的第 5 至 10 步，创建一个简单的 ``hello_world`` 工程，确保环境能够正常工作，并熟悉如何基于 ESP-IDF 创建新工程。这一步并非必要，但是强烈建议您进行这一步。

在完成所有 10 个步骤后（如果没有，至少完成前 4 步），可以进行下面专门针对 ESP-AT 工程的步骤。

.. note::

  在上述过程中，请不要设置 ``IDF_PATH``，否则以后编译 ESP-AT 项目时会遇到一些问题。

获取 ESP-AT
^^^^^^^^^^^^^^^

编译 ESP-AT 工程需下载乐鑫提供的软件库文件 ESP-AT 仓库。

打开终端，切换到您要保存 ESP-AT 的工作目录，使用 git clone 命令克隆远程仓库，获取 ESP-AT 的本地副本。以下是不同操作系统的获取方式。

- Linux 或 macOS
  
  ::

    cd ~/esp
    git clone --recursive https://github.com/espressif/esp-at.git

- Windows

  - 对于 {IDF_TARGET_NAME} 系列模组，推荐您以管理员权限运行 `ESP-IDF 4.3 CMD <https://dl.espressif.com/dl/esp-idf/?idf=4.3>`__。
  
  ::

    cd %userprofile%\esp
    git clone --recursive https://github.com/espressif/esp-at.git

如果您位于中国或访问 GitHub 有困难，也可以使用 ``git clone https://gitee.com/EspressifSystems/esp-at.git`` 来获取 ESP-AT，可能会更快。

ESP-AT 将下载至 Linux 和 macOS 的 ``~/esp/esp-at``、Windows 的 ``%userprofile%\esp\esp-at``。

.. note::

    在本文档中，Linux 和 macOS 操作系统中 ESP-AT 的默认安装路径为 ``~/esp``；Windows 操作系统的默认路径为 ``%userprofile%\esp``。您也可以将 ESP-AT 安装在任何其它路径下，但请注意在使用命令行时进行相应替换。注意，ESP-AT 不支持带有空格的路径。

连接设备
^^^^^^^^

使用 USB 线将您的设备连接到 PC 上，以下载固件和输出日志，详情请见 :doc:`../Get_Started/Hardware_connection`。注意，如果您在编译过程中不发送 AT 命令和接收 AT 响应，则不需要建立 "AT 命令/响应" 连接。关于更改默认端口管脚的信息请参考 :doc:`How_to_set_AT_port_pin`。

配置
^^^^^^^^^

此步骤中，您将在 ``esp-at`` 文件夹下克隆 ``esp-idf`` 文件夹，在这个新克隆的文件夹中设置开发环境，最后配置工程。

1. 前往 ``~/esp/esp-at`` 目录。

2. 运行项目配置工具 ``menuconfig`` 来配置。

- Linux 或 macOS
  
  ::
    
    ./build.py menuconfig

- Windows

  ::
    
    python build.py menuconfig

3. 如果是第一次编译工程，请为 ESP 设备选择以下配置选项。

  - 选择 ``Platform name``，例如 {IDF_TARGET_NAME} 系列设备选择 ``PLATFORM_{IDF_TARGET_CFG_PREFIX}``。``Platform name`` 由 :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` 定义。
  - 选择 ``Module name``，例如 {IDF_TARGET_PRODUCT_NAME} 模组选择 ``{IDF_TARGET_COMPILE_MNAME}``。``Module name`` 由 :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` 定义。
  - 启用或禁用 ``silence mode``，启用时将删除一些日志并减少固件的大小。一般情况下请禁用。
  - 如果 ``build/module_info.json`` 文件存在，上述三个配置选项将不会出现。因此，如果您想重新配置模组信息，请删除该文件。

4. 现在，``esp-idf`` 文件夹已经被创建在 ``esp-at`` 文件夹中。这个 ``esp-idf`` 与 `ESP-IDF 快速入门`_ 中的不同。

  如果您遇到以下任意一种情况，可以继续进行进行下一步，在 ``esp-at/esp-idf`` 中设置开发环境。

  - 终端提示如下错误信息。
    
    ::

      The following Python requirements are not satisfied:
      ...
      Please follow the instructions found in the "Set up the tools" section of ESP-IDF Get Started Guide.

  - 如果您曾经编译过某个 ESP 系列的 AT 工程，而现在想切换到另一个系列进行编译，则必须运行 ``rm -rf esp-idf`` 来删除旧的 esp-idf，然后再进行下一步操作。

  - 您的 esp-idf 已经升级。

5. 在 ``esp-at/esp-idf`` 文件夹中设置开发环境。
  
  - 如果是第一次编译 ESP-AT 工程，按照 *ESP-IDF 快速入门* 文档中的第 3 步在该文件夹下设置工具链。
  - 每次编译 ESP-AT 工程时，请按照 *ESP-IDF 快速入门* 文档中的第 4 步在该文件夹下设置环境变量。
  - **如果未安装 ``pyyaml`` 和 ``xlrd`` 包，请使用 pip 在该文件夹下安装 ``pyyaml`` 和 ``xlrd``**。

    ::

      python -m pip install pyyaml xlrd

如果以上所有步骤都正确，再次运行 ``./build.py menuconfig`` 时则会显示下面的菜单：

.. figure:: ../../_static/project-configuration.png
   :align: center
   :alt: 工程配置 - 主窗口
   :figclass: align-center

   工程配置 - 主窗口

此菜单可以用来配置每个工程，如更改 AT 端口管脚、启用经典蓝牙功能等，如果不修改配置，那么就会按照默认配置编译工程。

.. _build-project-build-the-project:

编译工程
^^^^^^^^

运行以下命令编译工程。

- Linux 或 macOS

::

  ./build.py build

- Windows

::

  python build.py build

- 如果启用了蓝牙功能，固件尺寸会大大增加。请确保它不超过 ota 分区的大小。
- 编译完成后会在 ``build/factory`` 路径下生成打包好的量产固件。更多信息请参见 :doc:`esp-at_firmware_differences`。

.. _build-project-flash-onto-the-device:

烧录到设备
^^^^^^^^^^^^^^^^^^^^^^

运行以下命令将生成的固件烧录到 ESP 设备上。

- Linux 或 macOS

::

  ./build.py -p (PORT) flash

- Windows

::

  python build.py -p (PORT) flash

- 注意，请用 ESP 设备的串口名称替换 ``PORT``。
- 或者按照提示信息将固件烧录到 flash 中。注意，仍然需要替换 ``PORT``。
- 如果 ESP-AT bin 不能启动，并且打印出 "ota data partition invalid"，请运行 ``./build.py erase_flash`` 来擦除整个 flash，然后重新烧录 AT 固件。
