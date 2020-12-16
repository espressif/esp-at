.. _ETH-AT:

[ESP32 Only] Ethernet AT Commands
=================================

-  [ESP32 Only] :ref:`AT+CIPETHMAC <cmd-ETHMAC>`: Query/Set the MAC address of the ESP Ethernet.
-  [ESP32 Only] :ref:`AT+CIPETH <cmd-ETHIP>`: Query/Set the IP address of the ESP Ethernet.

.. _cmd-ETHMAC:

[ESP32 Only] :ref:`AT+CIPETHMAC <ETH-AT>`: Query/Set the MAC Address of the ESP Ethernet
--------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^
**Function:**

Obtain the MAC address of the ESP Ethernet.

**Command:**

::

    AT+CIPETHMAC?

**Response:**

::

    +CIPETHMAC:<mac>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the MAC address of the ESP Ethernet.

**Command:**

::

    AT+CIPETHMAC=<mac>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<mac>**: string parameter showing the MAC address of the Ethernet interface.

Notes
^^^^^

-  The default firmware does not support Ethernet AT commands (see :doc:`../Compile_and_Develop/How_to_understand_the_differences_of_each_type_of_module`), but you can enable it by ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT ethernet support`` and compile the project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`).
-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  Please make sure the MAC address of Ethernet interface you set is different from those of other interfaces.
-  Bit 0 of the ESP MAC address CANNOT be 1.

   -  For example, a MAC address can be "1a:…" but not "15:…".

-  ``FF:FF:FF:FF:FF:FF`` and ``00:00:00:00:00:00`` are invalid MAC addresses and cannot be set.

Example
^^^^^^^^

::

    AT+CIPETHMAC="1a:fe:35:98:d4:7b"

.. _cmd-ETHIP:

[ESP32 Only] :ref:`AT+CIPETH <ETH-AT>`: Query/Set the IP Address of the the ESP Ethernet
-------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Obtain the IP address of the ESP Ethernet.

**Command:**

::

    AT+CIPETH?

**Response:**

::

    +CIPETH:ip:<ip>
    +CIPETH:gateway:<gateway>
    +CIPETH:netmask:<netmask>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the IP address of the ESP Ethernet.

**Command:**

::

    AT+CIPETH=<ip>[,<gateway>,<netmask>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<ip>**: string parameter showing the IP address of the ESP Ethernet.
-  **[<gateway>]**: gateway.
-  **[<netmask>]**: netmask.

Notes
^^^^^

-  The default firmware does not support Ethernet AT commands (see :doc:`../Compile_and_Develop/How_to_understand_the_differences_of_each_type_of_module`), but you can enable it by ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT ethernet support`` and compile the project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`).
- Only after calling esp_at_eth_cmd_regist can its IP address be queried.
- The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
- This Set Command correlates with DHCP commands, such as :ref:`AT+CWDHCP <cmd-DHCP>`:

   -  If static IP is enabled, DHCP will be disabled.
   -  If DHCP is enabled, static IP will be disabled.
   -  The last configuration overwrites the previous configuration.

Example
^^^^^^^^

::

    AT+CIPETH="192.168.6.100","192.168.6.1","255.255.255.0"
