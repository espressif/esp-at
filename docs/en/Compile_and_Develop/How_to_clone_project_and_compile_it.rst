Compile ESP-AT Project Locally
==============================

{IDF_TARGET_COMPILE_MNAME: default="undefined", esp32="WROOM-32", esp32c2="ESP32C2-4MB", esp32c3="MINI-1", esp32c6="ESP32C6-4MB", esp32s2="MINI"}
{IDF_TARGET_PRODUCT_NAME: default="undefined", esp32="ESP32-WROOM-32", esp32c2="ESP8684-MINI-1 4MB", esp32c3="ESP32-C3-MINI-1", esp32c6="ESP32-C6-MINI-1", esp32s2="ESP32-S2-MINI"}
{IDF_TARGET_VER: default="undefined", esp32="5.0", esp32c2="5.0", esp32c3="5.0", esp32c6="5.1", esp32s2="5.0"}

:link_to_translation:`zh_CN:[中文]`

.. only:: esp32 or esp32c3 or esp32c6

  This document details how to build your own ESP-AT project locally and flash the generated firmware into your {IDF_TARGET_NAME}. It comes in handy when the :doc:`official released fimware <../AT_Binary_Lists/index>` cannot meet your needs, for example, to customize the :doc:`AT port pins <How_to_set_AT_port_pin>`, :doc:`Bluetooth LE services <How_to_customize_BLE_services>`, and :doc:`partitions <How_to_customize_partitions>`, and so on.

.. only:: esp32c2 or esp32s2

  This document details how to build your own ESP-AT project locally and flash the generated firmware into your {IDF_TARGET_NAME}. It comes in handy when the :doc:`official released fimware <../AT_Binary_Lists/index>` cannot meet your needs, for example, to customize the :doc:`AT port pins <How_to_set_AT_port_pin>` and :doc:`partitions <How_to_customize_partitions>`, and so on.

If you have difficulties in compiling ESP-AT project locally, or you only need to modify a small amount of code, we recommend that you use :doc:`Compile ESP-AT Project on the GitHub Webpage <How_to_build_project_with_web_page>`.

.. _esp-at-started-steps:

Detailed Steps
^^^^^^^^^^^^^^

Please follow the detailed steps below to set up your environment and build the project. **It is recommended that you develop the esp-at project on Linux system.**

* :ref:`esp-at-idf-prerequisites`
* :ref:`esp-at-get-at-sdk`
* :ref:`esp-at-install-env`
* :ref:`esp-at-hardware-connection`
* :ref:`esp-at-project-config`
* :ref:`esp-at-project-build`
* :ref:`esp-at-project-flash`
* :ref:`esp-at-project-advanced-usage`

.. _esp-at-idf-prerequisites:

Step 1. Get Started with ESP-IDF
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Get started with ESP-IDF before compiling an ESP-AT project, because ESP-AT is developed based on ESP-IDF.

Please follow `ESP-IDF v{IDF_TARGET_VER} Get Started <https://docs.espressif.com/projects/esp-idf/en/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/get-started/index.html>`_ guide, configure, build, flash onto the {IDF_TARGET_NAME} device of the ``hello_world`` example.

.. note::

  This step is not a must, but if you are a beginner, you are strongly recommended to complete it in order to familiarize yourself with ESP-IDF and ensure smooth proceeding with the following steps.

After you complete the ESP-IDF get started in the step above, now you can proceed to compile an ESP-AT project according to the following steps.

.. _esp-at-get-at-sdk:

Step 2. Get ESP-AT
^^^^^^^^^^^^^^^^^^

To compile an ESP-AT project, you need the software libraries provided by Espressif in the ESP-AT repository.

To get ESP-AT, navigate to your installation directory and clone the repository with ``git clone``, following instructions below specific to your operating system.

- Linux or macOS
  
  ::

    cd ~/esp
    git clone --recursive https://github.com/espressif/esp-at.git

- Windows

  For {IDF_TARGET_NAME} series of modules, it is recommended that you run `ESP-IDF {IDF_TARGET_VER} CMD <https://dl.espressif.com/dl/esp-idf/?idf={IDF_TARGET_VER}>`__ as an administrator first.

  ::

    cd %userprofile%\esp
    git clone --recursive https://github.com/espressif/esp-at.git

If you are located in China or have difficulties to access GitHub, you can also use ``git clone https://jihulab.com/esp-mirror/espressif/esp-at.git`` or ``git clone https://gitee.com/EspressifSystems/esp-at.git`` to get ESP-AT, which may be faster.

ESP-AT will be downloaded into ``~/esp/esp-at`` on Linux or macOS, or ``%userprofile%\esp\esp-at`` on Windows.

.. note::

    This guide uses the directory ``~/esp`` on Linux or macOS, or ``%userprofile%\esp`` on Windows as an installation folder for ESP-AT. You can use any directory, but you will need to adjust paths for the commands respectively. Keep in mind that ESP-AT does not support spaces in paths.

.. _esp-at-install-env:

Step 3. Install Environment
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Run the project tool ``install`` to install the environment. This tool will automatically install Python packages, ESP-IDF repository, and the compiler and tools used by ESP-IDF.

- Linux or macOS
  
  ::
    
    ./build.py install

- Windows

  ::
    
    python build.py install

Select the following configuration options for your {IDF_TARGET_NAME} if it is your first time.

  - Select the ``Platform name`` for your {IDF_TARGET_NAME}. For example, select ``PLATFORM_{IDF_TARGET_CFG_PREFIX}`` for {IDF_TARGET_NAME} series of products. ``Platform name`` is defined in :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`.
  - Select the ``Module name`` for your {IDF_TARGET_NAME}. For example, select ``{IDF_TARGET_COMPILE_MNAME}`` for the {IDF_TARGET_PRODUCT_NAME} module. ``Module name`` is defined in :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>`.

  .. _esp-at_silence_mode_cfg:

  - Before selecting to enable or disable silence mode, please read the :doc:`documentation <../Compile_and_Develop/How_to_configure_silence_mode>` to understand silence mode. Generally, it should be disabled.
  - The above three option items will not appear if the file ``build/module_info.json`` exists. So please delete it if you want to reconfigure the module information.

  .. only:: esp32

    For example, set ``Platform name`` to ``ESP32``, ``Module name`` to ``WROOM-32``, and enable silence mode as follows:

    .. code-block:: none

        $ ./build.py install
        Ready to install ESP-IDF prerequisites..
    
        ... (more lines of install ESP-IDF prerequisites)

        Ready to install ESP-AT prerequisites..

        ... (more lines of install ESP-IDF prerequisites)

        Platform name:
        1. PLATFORM_ESP32
        2. PLATFORM_ESP32C3
        3. PLATFORM_ESP32C2
        4. PLATFORM_ESP32C6
        5. PLATFORM_ESP32S2
        choose(range[1,5]):1

        Module name:
        1. WROOM-32 (Firmware description: TX:17 RX:16)
        2. WROVER-32 (Firmware description: need PSRAM, TX:22 RX:19)
        3. PICO-D4 (Firmware description: TX:22 RX:19)
        4. SOLO-1 (Firmware description: not recommended for new design, TX:17 RX:16)
        5. MINI-1 (Firmware description: TX:17 RX:16, ESP32-U4WDH chip inside)
        6. ESP32-SDIO (Firmware description: communicate with MCU via SDIO)
        7. ESP32-D2WD (Firmware description: 2MB flash, No OTA)
        choose(range[1,7]):1

        Enable silence mode to remove some logs and reduce the firmware size?
        0. No
        1. Yes
        choose(range[0,1]):1
        Platform name:ESP32 Module name:WROOM-32 Silence:1
        Cloning into 'esp-idf'...

        ... (more lines of clone esp-idf)

        Ready to set up ESP-IDF tools..

        ... (more lines of set up ESP-IDF tools)

        All done! You can now run:

        ./build.py build

  .. only:: esp32c2

    For example, set ``Platform name`` to ``ESP32C2``, ``Module name`` to ``ESP32C2-4MB``, and disable silence mode as follows:

    .. code-block:: none

        $ ./build.py install
        Ready to install ESP-IDF prerequisites..
    
        ... (more lines of install ESP-IDF prerequisites)

        Ready to install ESP-AT prerequisites..

        ... (more lines of install ESP-IDF prerequisites)

        Platform name:
        1. PLATFORM_ESP32
        2. PLATFORM_ESP32C3
        3. PLATFORM_ESP32C2
        4. PLATFORM_ESP32C6
        5. PLATFORM_ESP32S2
        choose(range[1,5]):3

        Module name:
        1. ESP32C2-2MB (Firmware description: single Wi-Fi, 2MB, TX:7 RX:6)
        2. ESP32C2-4MB (Firmware description: Wi-Fi + BluFi, 4MB, TX:7 RX:6)
        3. ESP32C2-BLE-2MB (Firmware description: single BLE, 2MB, TX:7 RX:6)
        choose(range[1,3]):2

        Enable silence mode to remove some logs and reduce the firmware size?
        0. No
        1. Yes
        choose(range[0,1]):0
        Platform name:ESP32C2 Module name:ESP32C2-4MB Silence:0
        Cloning into 'esp-idf'...

        ... (more lines of clone esp-idf)

        Ready to set up ESP-IDF tools..

        ... (more lines of set up ESP-IDF tools)

        All done! You can now run:

        ./build.py build

  .. only:: esp32c3

    For example, set ``Platform name`` to ``ESP32C3``, ``Module name`` to ``MINI-1``, and disable silence mode as follows:

    .. code-block:: none

        $ ./build.py install
        Ready to install ESP-IDF prerequisites..
    
        ... (more lines of install ESP-IDF prerequisites)

        Ready to install ESP-AT prerequisites..

        ... (more lines of install ESP-IDF prerequisites)

        Platform name:
        1. PLATFORM_ESP32
        2. PLATFORM_ESP32C3
        3. PLATFORM_ESP32C2
        4. PLATFORM_ESP32C6
        5. PLATFORM_ESP32S2
        choose(range[1,5]):2

        Module name:
        1. MINI-1 (Firmware description: TX:7 RX:6)
        2. ESP32C3-SPI (Firmware description: communicate with MCU via SPI)
        3. ESP32C3_RAINMAKER (Firmware description: support rainmaker cloud, TX:7 RX:6)
        choose(range[1,3]):1

        Enable silence mode to remove some logs and reduce the firmware size?
        0. No
        1. Yes
        choose(range[0,1]):0
        Platform name:ESP32C3 Module name:MINI-1 Silence:0
        Cloning into 'esp-idf'...

        ... (more lines of clone esp-idf)

        Ready to set up ESP-IDF tools..

        ... (more lines of set up ESP-IDF tools)

        All done! You can now run:

        ./build.py build

  .. only:: esp32c6

    For example, set ``Platform name`` to ``ESP32C6``, ``Module name`` to ``ESP32C6-4MB``, and disable silence mode as follows:

    .. code-block:: none

        $ ./build.py install
        Ready to install ESP-IDF prerequisites..
    
        ... (more lines of install ESP-IDF prerequisites)

        Ready to install ESP-AT prerequisites..

        ... (more lines of install ESP-IDF prerequisites)

        Platform name:
        1. PLATFORM_ESP32
        2. PLATFORM_ESP32C3
        3. PLATFORM_ESP32C2
        4. PLATFORM_ESP32C6
        5. PLATFORM_ESP32S2
        choose(range[1,5]):4

        Module name:
        1. ESP32C6-4MB (Firmware description: TX:7 RX:6)
        choose(range[1,1]):1

        Enable silence mode to remove some logs and reduce the firmware size?
        0. No
        1. Yes
        choose(range[0,1]):0
        Platform name:ESP32C6 Module name:ESP32C6-4MB Silence:0

        Cloning into 'esp-idf'...

        ... (more lines of clone esp-idf)

        Ready to set up ESP-IDF tools..

        ... (more lines of set up ESP-IDF tools)

        All done! You can now run:

        ./build.py build

  .. only:: esp32s2

    For example, set ``Platform name`` to ``ESP32S2``, ``Module name`` to ``MINI``, and disable silence mode as follows:

    .. code-block:: none

        $ ./build.py install
        Ready to install ESP-IDF prerequisites..
    
        ... (more lines of install ESP-IDF prerequisites)

        Ready to install ESP-AT prerequisites..

        ... (more lines of install ESP-IDF prerequisites)

        Platform name:
        1. PLATFORM_ESP32
        2. PLATFORM_ESP32C3
        3. PLATFORM_ESP32C2
        4. PLATFORM_ESP32C6
        5. PLATFORM_ESP32S2
        choose(range[1,5]):5

        Module name:
        1. MINI (Firmware description: TX:17 RX:21)
        choose(range[1,1]):1

        Enable silence mode to remove some logs and reduce the firmware size?
        0. No
        1. Yes
        choose(range[0,1]):0
        Platform name:ESP32S2 Module name:MINI Silence:0

        Cloning into 'esp-idf'...

        ... (more lines of clone esp-idf)

        Ready to set up ESP-IDF tools..

        ... (more lines of set up ESP-IDF tools)

        All done! You can now run:

        ./build.py build

.. _esp-at-hardware-connection:

Step 4. Connect Your Device
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Connect your {IDF_TARGET_NAME} device to the PC with a USB cable to download firmware and print log. See :doc:`../Get_Started/Hardware_connection` for more information. Note that you do not need to set up the "AT command/response" connection if you do not send AT commands and receive AT responses during the compiling process. You can change default port pins referring to :doc:`How_to_set_AT_port_pin`.

.. _esp-at-project-config:

Step 5. Configure
^^^^^^^^^^^^^^^^^

Run the project configuration utility ``menuconfig`` to configure.

- Linux or macOS

  ::
    
    ./build.py menuconfig

- Windows

  ::
    
    python build.py menuconfig

If the previous steps have been done correctly, the following menu pops up:

.. figure:: ../../_static/project-configuration.png
   :align: center
   :alt: Project configuration - Home window
   :figclass: align-center

   Project configuration - Home window

You are using this menu to set up project-specific configuration, e.g. changing AT port pins, enabling Classic Bluetooth function, etc. If you made no changes, it will run with the default configuration.

.. _esp-at-project-build:

Step 6. Build the Project
^^^^^^^^^^^^^^^^^^^^^^^^^

Build the project by running:

- Linux or macOS

  ::

    ./build.py build

- Windows

  ::

    python build.py build

If Bluetooth feature is enabled, the firmware size will be much larger. Please make sure it does not exceed the OTA partition size.

After compiled, the combined factory bin will be created in ``build/factory``. See :doc:`esp-at_firmware_differences` for more information.

.. _esp-at-project-flash:

Step 7. Flash onto the Device
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Flash the firmware that you just compiled onto your {IDF_TARGET_NAME} by running:

- Linux or macOS

  ::

    ./build.py -p (PORT) flash

- Windows

  ::

    python build.py -p (PORT) flash

Note that you need to replace ``(PORT)`` with your {IDF_TARGET_NAME}'s serial port name. Or you can follow the printed instructions to download the bin files into flash. Note that you also need to replace the ``(PORT)``.

If the ESP-AT bin fails to boot and prints "ota data partition invalid", you should run ``python build.py erase_flash`` to erase the entire flash, and then reflash the AT firmware.

.. _esp-at-project-advanced-usage:

build.py Advanced Usage
^^^^^^^^^^^^^^^^^^^^^^^

The script ``build.py`` is based on `idf.py <https://docs.espressif.com/projects/esp-idf/en/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-guides/build-system.html#idf-py>`__, which means that all ``idf.py <cmd>`` features should be included in ``build.py <cmd>``. You can run the following command for more details.

- Linux or macOS

::

  ./build.py --help

- Windows

::

  python build.py --help
