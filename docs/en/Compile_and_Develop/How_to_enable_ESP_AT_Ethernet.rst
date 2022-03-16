How to Enable ESP-AT Ethernet
=================================

:link_to_translation:`zh_CN:[中文]`

Overview
------------

This document is intended to help you enable ESP-AT Ethernet. After that, a simple test will show you how to confirm whether the enablement is successful.

Step 1. Configure and Flash
-----------------------------

1. ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``AT ethernet support`` to enable the Ethernet interface.
2. ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``Ethernet PHY`` to choose the PHY model. For more details see `PHY Configuration`_.
3. Recompile the ``esp-at`` project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`), download AT bin into flash.

PHY Configuration
^^^^^^^^^^^^^^^^^^^^

Use ``./build.py menuconfig`` to set the PHY model. These configuration items will vary depending on the hardware configuration you are using.

The default configuration is correct for Espressif's Ethernet board with TP101 PHY. {IDF_TARGET_NAME} AT supports up to four Ethernet PHY: ``LAN8720``, ``IP101``, ``DP83848`` and ``RTL8201``.
``TLK110`` PHY is no longer supported because TI stoped production.
If you want to use other PHY, follow the `{IDF_TARGET_NAME}-Ethernet-Kit V1.2 Getting Started Guide <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/hw-reference/{IDF_TARGET_PATH_NAME}/get-started-ethernet-kit.html>`__ to design.

Step 2. Connect the Board and Test
-------------------------------------------

Now connect the board to the router via a ethernet cable, the board will automatically send a DHCP request and try to obtain an IP address. If the device obtains the IP address successfully, you can use the PC connected to the router to ping the board.
