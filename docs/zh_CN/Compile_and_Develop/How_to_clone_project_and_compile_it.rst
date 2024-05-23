本地编译 ESP-AT 工程
=============================

{IDF_TARGET_COMPILE_MNAME: default="undefined", esp32="WROOM-32", esp32c2="ESP32C2-4MB", esp32c3="MINI-1", esp32c6="ESP32C6-4MB", esp32s2="MINI"}
{IDF_TARGET_PRODUCT_NAME: default="undefined", esp32="ESP32-WROOM-32D", esp32c2="ESP8684-MINI-1 4MB", esp32c3="ESP32-C3-MINI-1", esp32c6="ESP32-C6-MINI-1", esp32s2="ESP32-S2-MINI"}
{IDF_TARGET_VER: default="undefined", esp32="5.0", esp32c2="5.0", esp32c3="5.0", esp32c6="5.1", esp32s2="5.0"}

:link_to_translation:`en:[English]`

.. only:: esp32 or esp32c3 or esp32c6

  本文档详细介绍了如何本地编译 ESP-AT 工程，并将生成的固件烧录到 {IDF_TARGET_NAME} 设备中。当默认的 :doc:`官方发布的固件 <../AT_Binary_Lists/index>` 无法满足需求时，如您需要自定义 :doc:`AT 端口管脚 <How_to_set_AT_port_pin>`、:doc:`低功耗蓝牙服务 <How_to_customize_BLE_services>` 以及 :doc:`分区 <How_to_customize_partitions>` 等，那么就需要编译 ESP-AT 工程。

.. only:: esp32c2 or esp32s2

  本文档详细介绍了如何本地编译 ESP-AT 工程，并将生成的固件烧录到 {IDF_TARGET_NAME} 设备中。当默认的 :doc:`官方发布的固件 <../AT_Binary_Lists/index>` 无法满足需求时，如您需要自定义 :doc:`AT 端口管脚 <How_to_set_AT_port_pin>` 以及 :doc:`分区 <How_to_customize_partitions>` 等，那么就需要编译 ESP-AT 工程。

如果本地编译 ESP-AT 工程有困难，或者需要修改的代码量较少，推荐您通过 :doc:`如何在 GitHub 网页上编译 ESP-AT 工程 <How_to_build_project_with_web_page>`。

.. _esp-at-started-steps:

详细步骤
^^^^^^^^^^^^^

请根据下方详细步骤，完成 ESP-AT 工程的克隆、环境安装、配置、编译以及烧录。**推荐您优先选择 Linux 系统开发。**

* :ref:`esp-at-idf-prerequisites`
* :ref:`esp-at-get-at-sdk`
* :ref:`esp-at-install-env`
* :ref:`esp-at-hardware-connection`
* :ref:`esp-at-project-config`
* :ref:`esp-at-project-build`
* :ref:`esp-at-project-flash`
* :ref:`esp-at-project-advanced-usage`

.. _esp-at-idf-prerequisites:

第一步：ESP-IDF 快速入门
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

在编译 ESP-AT 工程之前，请先学习使用 ESP-IDF，因为 ESP-AT 是基于 ESP-IDF 开发的。

请您根据 `ESP-IDF v{IDF_TARGET_VER} 快速入门文档 <https://docs.espressif.com/projects/esp-idf/zh_CN/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/get-started/index.html>`__ 的指导，完成 ``hello_world`` 工程的配置、编译以及下载固件至 {IDF_TARGET_NAME} 开发板等步骤。

.. note::

  此步骤不是必须的，但如果您是初学者，强烈建议您完成此步骤，以熟悉 ESP-IDF 并确保顺利进行以下步骤。

完成上一步的 ESP-IDF 快速入门后，便可以开始下面的 ESP-AT 工程的编译。

.. _esp-at-get-at-sdk:

第二步：获取 ESP-AT
^^^^^^^^^^^^^^^^^^^^^^^

编译 ESP-AT 工程需下载乐鑫提供的软件库文件 ESP-AT 仓库。

打开终端，切换到您要保存 ESP-AT 的工作目录，使用 ``git clone`` 命令克隆远程仓库，获取 ESP-AT 的本地副本。以下是不同操作系统的获取方式。

- Linux 或 macOS

  ::

    cd ~/esp
    git clone --recursive https://github.com/espressif/esp-at.git

- Windows

  对于 {IDF_TARGET_NAME} 系列模组，推荐您以管理员权限运行 `ESP-IDF {IDF_TARGET_VER} CMD <https://dl.espressif.com/dl/esp-idf/?idf={IDF_TARGET_VER}>`__。

  ::

    cd %userprofile%\esp
    git clone --recursive https://github.com/espressif/esp-at.git

如果您位于中国或访问 GitHub 有困难，也可以使用 ``git clone https://jihulab.com/esp-mirror/espressif/esp-at.git`` 或者 ``git clone https://gitee.com/EspressifSystems/esp-at.git`` 来获取 ESP-AT，可能会更快。

ESP-AT 将下载至 Linux 和 macOS 的 ``~/esp/esp-at``、Windows 的 ``%userprofile%\esp\esp-at``。

.. note::

    在本文档中，Linux 和 macOS 操作系统中 ESP-AT 的默认安装路径为 ``~/esp``；Windows 操作系统的默认路径为 ``%userprofile%\esp``。您也可以将 ESP-AT 安装在任何其它路径下，但请注意在使用命令行时进行相应替换。注意，ESP-AT 不支持带有空格的路径。

.. _esp-at-install-env:

第三步：安装环境
^^^^^^^^^^^^^^^^^^^^^^^

运行项目工具 ``install`` 来安装环境。此安装工具将自动安装依赖的 Python 包、ESP-IDF 仓库以及 ESP-IDF 依赖的编译器、工具等。

- Linux 或 macOS
  
  ::
    
    ./build.py install

- Windows

  ::
    
    python build.py install

如果是第一次安装环境，请为 {IDF_TARGET_NAME} 设备选择以下配置选项。

  - 选择 ``Platform name``，例如 {IDF_TARGET_NAME} 系列设备选择 ``PLATFORM_{IDF_TARGET_CFG_PREFIX}``。``Platform name`` 由 :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` 定义。
  - 选择 ``Module name``，例如 {IDF_TARGET_PRODUCT_NAME} 模组选择 ``{IDF_TARGET_COMPILE_MNAME}``。``Module name`` 由 :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` 定义。
  - 启用或禁用 ``silence mode``，启用时将删除一些日志并减少固件的大小。一般情况下请禁用。
  - 如果 ``build/module_info.json`` 文件存在，上述三个配置选项将不会出现。因此，如果您想重新配置模组信息，请删除该文件。

.. _esp-at-hardware-connection:

第四步：连接设备
^^^^^^^^^^^^^^^^

使用 USB 线将您的 {IDF_TARGET_NAME} 设备连接到 PC 上，以下载固件和输出日志，详情请见 :doc:`../Get_Started/Hardware_connection`。注意，如果您在编译过程中不发送 AT 命令和接收 AT 响应，则不需要建立 "AT 命令/响应" 连接。关于更改默认端口管脚的信息请参考 :doc:`How_to_set_AT_port_pin`。

.. _esp-at-project-config:

第五步：配置工程
^^^^^^^^^^^^^^^^

运行项目工具 ``menuconfig`` 来配置。

- Linux 或 macOS
  
  ::
    
    ./build.py menuconfig

- Windows

  ::
    
    python build.py menuconfig

如果以上所有步骤都正确，则会弹出下面的菜单：

.. figure:: ../../_static/project-configuration.png
   :align: center
   :alt: 工程配置 - 主窗口
   :figclass: align-center

   工程配置 - 主窗口

此菜单可以用来配置每个工程，如更改 AT 端口管脚、启用经典蓝牙功能等，如果不修改配置，那么就会按照默认配置编译工程。

.. _esp-at-project-build:

第六步：编译工程
^^^^^^^^^^^^^^^^

运行以下命令编译工程。

- Linux 或 macOS

::

  ./build.py build

- Windows

::

  python build.py build

如果启用了蓝牙功能，固件尺寸会大大增加。请确保它不超过 ota 分区的大小。

编译完成后会在 ``build/factory`` 路径下生成打包好的量产固件。更多信息请参见 :doc:`esp-at_firmware_differences`。

.. _esp-at-project-flash:

第七步：烧录到设备
^^^^^^^^^^^^^^^^^^^^^^

运行以下命令将生成的固件烧录到 {IDF_TARGET_NAME} 设备上。

- Linux 或 macOS

::

  ./build.py -p (PORT) flash

- Windows

::

  python build.py -p (PORT) flash

注意请用 {IDF_TARGET_NAME} 设备的串口名称替换 ``(PORT)``。或者按照提示信息将固件烧录到 flash 中。仍然需要注意替换 ``(PORT)``。

如果 ESP-AT bin 不能启动，并且打印出 "ota data partition invalid"，请运行 ``python build.py erase_flash`` 来擦除整个 flash，然后重新烧录 AT 固件。

.. _esp-at-project-advanced-usage:

build.py 进阶用法
^^^^^^^^^^^^^^^^^^^^^^

``build.py`` 脚本是基于 `idf.py <https://docs.espressif.com/projects/esp-idf/zh_CN/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-guides/build-system.html#idf-py>`__ 封装的工具（即 ``idf.py <cmd>`` 功能均包含在 ``build.py <cmd>`` 里），您可以运行以下命令查看更多用法。

- Linux 或 macOS

::

  ./build.py --help

- Windows

::

  python build.py --help
