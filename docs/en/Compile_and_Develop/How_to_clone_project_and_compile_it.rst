Compile ESP-AT Project
=============================

{IDF_TARGET_HYPHEN_LOWERCASE_NAME: default="undefined", esp32="esp32", esp32c3="esp32-c3"}
{IDF_TARGET_COMPILE_MNAME: default="undefined", esp32="WROOM-32", esp32c3="MINI-1"}
{IDF_TARGET_PRODUCT_NAME: default="undefined", esp32="ESP32-WROOM-32D", esp32c3="ESP32-C3-MINI-1"}

:link_to_translation:`zh_CN:[中文]`

This document details how to build your own ESP-AT project and flash the generated firmware into your ESP devices. It comes in handy when the :doc:`official released fimware <../AT_Binary_Lists/index>` cannot meet your needs, for example, to customize the :doc:`AT port pins <How_to_set_AT_port_pin>`, :doc:`Bluetooth LE services <How_to_customize_BLE_services>`, and :doc:`partitions <How_to_customize_partitions>`, and so on.

The structure of this document is as follows:

- :ref:`build-project-overview`: Overview of the steps to build an ESP-AT project.
- :ref:`build-project-series-steps`: Details steps to build a project for {IDF_TARGET_NAME} series.

.. _build-project-overview:

Overview
--------

Before compiling an ESP-AT project, you need first get started with ESP-IDF and set up the environment for ESP-IDF, because ESP-AT is based on ESP-IDF. 

After the environment is ready, install the tools and ESP-AT SDK. Then, connect your ESP device to PC. Use ``./build.py menuconfig`` to set up some configuration for the project. Build the project and flash the generated bin files onto your ESP device.

.. note::

  **Please pay attention to possible conflicts of pins**. If choosing ``AT through HSPI``, you can get the information of the HSPI pin by ``./build.py menuconfig`` --> ``Component config`` --> ``AT`` --> ``AT hspi settings``.

.. _build-project-series-steps:

{IDF_TARGET_NAME} Series
-----------------------------------

This section describes how to compile an ESP-AT project for {IDF_TARGET_NAME} series.

Get Started with ESP-IDF
^^^^^^^^^^^^^^^^^^^^^^^^

Get started with ESP-IDF before compiling an ESP-AT project, because ESP-AT is developed based on ESP-IDF, and the supported version varies from series to series:

.. include:: ../../inline_substitutions

.. list-table:: ESP-IDF Versions for Different Series
  :header-rows: 0

  * - Project
    - IDF Version
    - IDF Documentation Version
  * - {IDF_TARGET_NAME} ESP-AT
    - release/v4.3
    - `ESP-IDF Get Started Guide v4.3 <https://docs.espressif.com/projects/esp-idf/en/release-v4.3/{IDF_TARGET_PATH_NAME}/get-started/index.html>`__

First, set up the development environment for ESP-IDF according to Step 1 to 4 of *ESP-IDF Get Started Guide* (click the corresponding link in the table above to navigate to the documentation).

Then, start a simple project of ``hello_world`` according to Step 5 to 10 of *ESP-IDF Get Started Guide* to make sure your environment works and familiarize yourself with the process of starting a new project based on ESP-IDF. It is not a must, but you are strongly recommended to do so.

After finishing all the ten steps (if not, at least the first four steps), you can move onto the following steps that are ESP-AT specific.

.. note::

  Please do not set ``IDF_PATH`` during the process, otherwise, you would encounter some unexpected issues when compiling ESP-AT projects later.

Get ESP-AT
^^^^^^^^^^

To compile an ESP-AT project, you need the software libraries provided by Espressif in the ESP-AT repository.

To get ESP-AT, navigate to your installation directory and clone the repository with git clone, following instructions below specific to your operating system.

- Linux or macOS
  
  ::

    cd ~/esp
    git clone --recursive https://github.com/espressif/esp-at.git

- Windows

  - For {IDF_TARGET_NAME} series of modules, it is recommended that you run `ESP-IDF 4.3 CMD <https://dl.espressif.com/dl/esp-idf/?idf=4.3>`__ as an administrator first.

  ::

    cd %userprofile%\esp
    git clone --recursive https://github.com/espressif/esp-at.git

If you are located in China or have difficulties to access GitHub, you can also use ``git clone https://gitee.com/EspressifSystems/esp-at.git`` to get ESP-AT, which may be faster.

ESP-AT will be downloaded into ``~/esp/esp-at`` on Linux or macOS, or ``%userprofile%\esp\esp-at`` on Windows.

.. note::

    This guide uses the directory ``~/esp`` on Linux or macOS, or ``%userprofile%\esp`` on Windows as an installation folder for ESP-AT. You can use any directory, but you will need to adjust paths for the commands respectively. Keep in mind that ESP-AT does not support spaces in paths.

Connect Your Device
^^^^^^^^^^^^^^^^^^^

Connect your device to the PC with a USB cable to download firmware and log output. See :doc:`../Get_Started/Hardware_connection` for more information. Note that you do not need to set up the "AT command/response" connection if you do not send AT commands and receive AT responses during the compiling process. You can change default port pins referring to :doc:`How_to_set_AT_port_pin`.

Configure
^^^^^^^^^

In this step, you will clone the ``esp-idf`` folder into the ``esp-at`` folder, set up the development environment in the newly cloned folder, and configure your project.

1. Navigate to ``~/esp/esp-at`` directory.

2. Run the project configuration utility ``menuconfig`` to configure.

  ::
    
    ./build.py menuconfig

3. Select the following configuration options for your ESP device if it is your first time.

  - Select the ``Platform name`` for your ESP device. For example, select ``PLATFORM_{IDF_TARGET_CFG_PREFIX}`` for {IDF_TARGET_NAME} series of products. ``Platform name`` is defined in :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`.
  - Select the ``Module name`` for your ESP device. For example, select ``{IDF_TARGET_COMPILE_MNAME}`` for the {IDF_TARGET_PRODUCT_NAME} module. ``Module name`` is defined in :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`.
  - Enable or disable ``silence mode``. If enabled, it will remove some logs and reduce the firmware size. Generally, it should be disabled.
  - The above three option items will not appear if the file ``build/module_info.json`` exists. So please delete it if you want to reconfigure the module information.

4. Now, the ``esp-idf`` folder is created in ``esp-at`` folder. This ``esp-idf`` is different from that in `Get Started with ESP-IDF`_.

  If you encounter any of the cases below, please proceed with the next step to set up the develop environment in the ``esp-at/esp-idf``.

  - The terminal prompt an error message like the following:
    
    ::

      The following Python requirements are not satisfied:
      ...
      Please follow the instructions found in the "Set up the tools" section of ESP-IDF Get Started Guide.

  - If you have compiled an ESP-AT project for an ESP series before and want to switch to another series, you must run ``rm -rf esp-idf`` to remove the old esp-idf and then proceed with the next step.

  - Your esp-idf is upgraded.

5. Set up the development environment in the ``esp-at/esp-idf``.
  
  - Set up the tools in the folder if it is the first time you compile the ESP-AT project. See Step 3 of *ESP-IDF Get Started Guide*.
  - Set up environment variables in the folder **every time** you compile an ESP-AT project. See Step 4 of *ESP-IDF Get Started Guide*.
  - **Install** ``pyyaml`` **and** ``xlrd`` **packages with pip in the folder if you have not done it**.

    ::

      python -m pip install pyyaml xlrd

If the previous steps have been done correctly, the following menu appears after you run ``./build.py menuconfig``:

.. figure:: ../../_static/project-configuration.png
   :align: center
   :alt: Project configuration - Home window
   :figclass: align-center

   Project configuration - Home window

You are using this menu to set up project-specific configuration, e.g. changing AT port pins, enabling Classic Bluetooth function, etc. If you made no changes, it will run with the default configuration.

.. _build-project-build-the-project:

Build the Project
^^^^^^^^^^^^^^^^^

Build the project by running:

::

  ./build.py build

- If Bluetooth feature is enabled, the firmware size will be much larger. Please make sure it does not exceed the ota partition size.
- After compiled, the combined factory bin will be created in ``build/factory``. See :doc:`esp-at_firmware_differences` for more information.

.. _build-project-flash-onto-the-device:

Flash onto the Device
^^^^^^^^^^^^^^^^^^^^^^

Flash the firmware that you just compiled onto your ESP device by running:

::

  ./build.py -p (PORT) flash

- Note that you need to replace ``PORT`` with your ESP device's serial port name.
- Or you can follow the printed instructions to download the bin files into flash. Note that you also need to replace the ``PORT``.
- If the ESP-AT bin fails to boot and prints "ota data partition invalid", you should run ``./build.py erase_flash`` to erase the entire flash, and then re-flash the AT firmware.
