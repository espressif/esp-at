.. _ETH-AT:

{IDF_TARGET_ETH_BOARD_NAME: default="/", esp32="ESP32-Ethernet-Kit"}

{IDF_TARGET_NAME} Ethernet AT Commands
======================================

:link_to_translation:`zh_CN:[中文]`

- :ref:`Introduction <cmd-eth-intro>`
- :ref:`Prerequisite <cmd-ETHPRE>`
- :ref:`AT+CIPETHMAC <cmd-ETHMAC>`: Query/Set the MAC address of the {IDF_TARGET_NAME} Ethernet.
- :ref:`AT+CIPETH <cmd-ETHIP>`: Query/Set the IP address of the {IDF_TARGET_NAME} Ethernet.

.. _cmd-eth-intro:

Introduction
------------

.. important::
  The default AT firmware does not support the AT commands listed on this page. If you need {IDF_TARGET_NAME} to support Ethernet commands, please refer to :doc:`../Compile_and_Develop/How_to_enable_ESP_AT_Ethernet` documentation and :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>`.

.. _cmd-ETHPRE:

:ref:`Prerequisite <ETH-AT>`
------------------------------

Before you run any Ethernet AT Commands, please make the following preparations:

.. note::
    This prerequisite takes `{IDF_TARGET_ETH_BOARD_NAME} <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/hw-reference/{IDF_TARGET_PATH_NAME}/get-started-ethernet-kit.html>`_ as an example. If you use other modules or development boards, please refer to corresponding datasheets for RX/TX pins.

- Change AT UART pins (because default AT UART pins are in conflict with the Ethernet function pins):

  - Open :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` file.
  - In the row of module ``WROVER-32``, change ``uart_tx_pin`` from GPIO22 to GPIO2, ``uart_rx_pin`` from GPIO19 to GPIO4, ``uart_cts_pin`` from GPIO15 to GPIO1, and ``uart_rts_pin`` from GPIO14 to GPIO1 (flow control is optional and is not used here). See :doc:`../Compile_and_Develop/How_to_set_AT_port_pin` for more information.

- Enable ``AT ethernet support``. See :doc:`../Compile_and_Develop/How_to_enable_ESP_AT_Ethernet` for more information.
- Compile and flash the project onto {IDF_TARGET_ETH_BOARD_NAME}.
- Connect your hardware:
 
  - Connect Host MCU (PC with USB to serial converter) to GPIO2 (TX) and GPIO4 (RX) of {IDF_TARGET_ETH_BOARD_NAME} when the flow control function is not enabled.
  - Connect {IDF_TARGET_ETH_BOARD_NAME} with Ethernet network.


.. _cmd-ETHMAC:

:ref:`AT+CIPETHMAC <ETH-AT>`: Query/Set the MAC Address of the {IDF_TARGET_NAME} Ethernet
--------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^
**Function:**

Query the MAC address of the {IDF_TARGET_NAME} Ethernet.

**Command:**

::

    AT+CIPETHMAC?

**Response:**

::

    +CIPETHMAC:<"mac">
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the MAC address of the {IDF_TARGET_NAME} Ethernet.

**Command:**

::

    AT+CIPETHMAC=<"mac">

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<"mac">**: string parameter showing the MAC address of the Ethernet interface.

Notes
^^^^^

-  The default firmware does not support Ethernet AT commands (see :doc:`../Compile_and_Develop/esp-at_firmware_differences`), but you can enable it by ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT ethernet support`` and compile the project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`).
-  The configuration changes will be saved in the NVS partition if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  Please make sure the MAC address of Ethernet interface you set is different from those of other interfaces.
-  Bit0 of the {IDF_TARGET_NAME} MAC address CANNOT be 1. For example, a MAC address can be "1a:…" but not "15:…".
-  ``FF:FF:FF:FF:FF:FF`` and ``00:00:00:00:00:00`` are invalid MAC addresses and cannot be set.

Example
^^^^^^^^

::

    AT+CIPETHMAC="1a:fe:35:98:d4:7b"

.. _cmd-ETHIP:

:ref:`AT+CIPETH <ETH-AT>`: Query/Set the IP Address of the the {IDF_TARGET_NAME} Ethernet
-------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the IP address of the {IDF_TARGET_NAME} Ethernet.

**Command:**

::

    AT+CIPETH?

**Response:**

::

    +CIPETH:ip:<"ip">
    +CIPETH:gateway:<"gateway">
    +CIPETH:netmask:<"netmask">
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the IP address of the {IDF_TARGET_NAME} Ethernet.

**Command:**

::

    AT+CIPETH=<"ip">[,<"gateway">,<"netmask">]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<"ip">**: string parameter showing the IP address of the {IDF_TARGET_NAME} Ethernet.
-  **[<"gateway">]**: gateway.
-  **[<"netmask">]**: netmask.

Notes
^^^^^

-  The default firmware does not support Ethernet AT commands (see :doc:`../Compile_and_Develop/esp-at_firmware_differences`), but you can enable it by ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT ethernet support`` and compile the project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`).
- The configuration changes will be saved in the NVS partition if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
- This Set Command correlates with DHCP commands, such as :ref:`AT+CWDHCP <cmd-DHCP>`:

   -  If static IP is enabled, DHCP will be disabled.
   -  If DHCP is enabled, static IP will be disabled.
   -  The last configuration overwrites the previous configuration.

Example
^^^^^^^^

::

    AT+CIPETH="192.168.6.100","192.168.6.1","255.255.255.0"
