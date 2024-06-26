.. _BT-AT:

{IDF_TARGET_NAME} Classic Bluetooth® AT Commands
================================================

:link_to_translation:`zh_CN:[中文]`

- :ref:`Introduction <cmd-bt-intro>`
- :ref:`AT+BTINIT <cmd-BTINIT>`: Classic Bluetooth initialization.
- :ref:`AT+BTNAME <cmd-BTNAME>`: Query/Set Classic Bluetooth device name.
- :ref:`AT+BTSCANMODE <cmd-BTSCANMODE>`: Set Classic Bluetooth scan mode.
- :ref:`AT+BTSTARTDISC <cmd-BTDISC>`: Start Classic Bluetooth discovery.
- :ref:`AT+BTSPPINIT <cmd-BTSPPINIT>`: Classic Bluetooth SPP profile initialization.
- :ref:`AT+BTSPPCONN <cmd-BTSPPCONN>`: Query/Establish SPP connection.
- :ref:`AT+BTSPPDISCONN <cmd-BTSPPDISCONN>`: End SPP connection.
- :ref:`AT+BTSPPSTART <cmd-BTSPPSTART>`: Start Classic Bluetooth SPP profile.
- :ref:`AT+BTSPPSEND <cmd-BTSPPSEND>`: Send data to remote Classic Bluetooth SPP device.
- :ref:`AT+BTA2DPINIT <cmd-BTA2DPINIT>`: Classic Bluetooth A2DP profile initialization.
- :ref:`AT+BTA2DPCONN <cmd-BTA2DPCONN>`: Establish A2DP connection.
- :ref:`AT+BTA2DPDISCONN <cmd-BTA2DPDISCONN>`: End A2DP connection.
- :ref:`AT+BTA2DPSRC <cmd-BTA2DPSRC>`: Query/Set the audio file URL.
- :ref:`AT+BTA2DPCTRL <cmd-BTA2DPCTRL>`: Control the audio play.
- :ref:`AT+BTSECPARAM <cmd-BTSECPARAM>`: Query/Set the Classic Bluetooth security parameters.
- :ref:`AT+BTKEYREPLY <cmd-BTKEYREPLY>`: Input the Simple Pair Key.
- :ref:`AT+BTPINREPLY <cmd-BTPINREPLY>`: Input the Legacy Pair PIN Code.
- :ref:`AT+BTSECCFM <cmd-BTSECCFM>`: Reply the confirm value to the peer device in the legacy connection stage.
- :ref:`AT+BTENCDEV <cmd-BTENCDEV>`: Query Classic Bluetooth encryption device list.
- :ref:`AT+BTENCCLEAR <cmd-BTENCCLEAR>`: Clear Classic Bluetooth encryption device list.
- :ref:`AT+BTCOD <cmd-BTCOD>`: Set class of devices.
- :ref:`AT+BTPOWER <cmd-BTPWR>`: Query/Set power of Classic Bluetooth.

.. _cmd-bt-intro:

Introduction
------------

{IDF_TARGET_NAME} AT firmware supports `Bluetooth® Core Specification Version 4.2 <https://www.bluetooth.com/specifications/specs/>`_.

.. important::
  The default AT firmware does not support the AT commands listed on this page. If you need {IDF_TARGET_NAME} to support Classic Bluetooth commands, you can compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections (Each item below is independent. Choose it according to your needs):

  - Enable Generic Bluetooth commands: ``Component config`` -> ``AT`` -> ``AT bt command support``
  - Enable SPP commands: ``Component config`` -> ``AT`` -> ``AT bt command support`` -> ``AT bt spp command support``
  - Enable A2DP commands:``Component config`` -> ``AT`` -> ``AT bt command support`` -> ``AT bt a2dp command support``

.. _cmd-BTINIT:

:ref:`AT+BTINIT <BT-AT>`: Classic Bluetooth Initialization
---------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the initialization status of Classic Bluetooth.

**Command:**

::

    AT+BTINIT?

**Response:**

If Classic Bluetooth is initialized, AT will return:

::

    +BTINIT:1
    OK

If Classic Bluetooth is not initialized, AT will return:

::

    +BTINIT:0
    OK


Set Command
^^^^^^^^^^^

**Function:**

Initialize or deinitialize Classic Bluetooth.

**Command:**

::

    AT+BTINIT=<init>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<init>**:

   -  0: deinitialize Classic Bluetooth.
   -  1: initialize Classic Bluetooth.

Notes
^^^^^

- To achieve better performance, it is recommended to disable SoftAP by sending the :ref:`AT+CWMODE=0/1 <cmd-MODE>` command before using Classic Bluetooth function. For more details, please refer to the `RF Coexistence <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/coexist.html>`_ documentation.
- If Classic Bluetooth is initialized, :ref:`AT+CIPMODE <cmd-IPMODE>` cannot be set to 1.

Example
^^^^^^^^

::

    AT+BTINIT=1    

.. _cmd-BTNAME:

:ref:`AT+BTNAME <BT-AT>`: Query/Set Classic Bluetooth Device Name
-------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the Classic Bluetooth device name.

**Command:**

::

    AT+BTNAME?

**Response:**

::

    +BTNAME:<device_name>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the Classic Bluetooth device name.

**Command:**

::

    AT+BTNAME=<device_name>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<device_name>**: the Classic Bluetooth device name. The maximum length is 32. Default: "ESP32_AT".

Notes
^^^^^

-  The configuration changes will be saved in the NVS area if :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`.
-  The default Classic Bluetooth device name is "{IDF_TARGET_NAME}_AT".

Example
^^^^^^^^

::

    AT+BTNAME="esp_demo"    

.. _cmd-BTSCANMODE:

:ref:`AT+BTSCANMODE <BT-AT>`: Set Classic Bluetooth Scan Mode
---------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set the scan mode of Classic Bluetooth.

**Command:**

::

    AT+BTSCANMODE=<scan_mode>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<scan_mode>**:

   -  0: Neither discoverable nor connectable.
   -  1: Connectable but not discoverable.
   -  2: Both discoverable and connectable.
   -  3: Discoverable but not connectable.

Example
^^^^^^^^

::

    AT+BTSCANMODE=2   // both discoverable and connectable

.. _cmd-BTDISC:

:ref:`AT+BTSTARTDISC <BT-AT>`: Start Classic Bluetooth Discovery
-------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Start Classic Bluetooth discovery.

**Command:**

::

    AT+BTSTARTDISC=<inq_mode>,<inq_len>,<inq_num_rsps>

**Response:**

::

    +BTSTARTDISC:<bt_addr>,<dev_name>,<major_dev_class>,<minor_dev_class>,<major_srv_class>,<rssi>

    OK

Parameters
^^^^^^^^^^

-  **<inq_mode>**:

   -  0: general inquiry mode.
   -  1: limited inquiry mode.

-  **<inq_len>**: inquiry duration. Range: 0x01 ~ 0x30.
-  **<inq_num_rsps>**: number of inquiry responses that can be received. If you set it to 0, AT will receive an unlimited number of responses.
-  **<bt_addr>**: Classic Bluetooth address.
-  **<dev_name>**: device name.
-  **<major_dev_class>**:

   -  0x0: miscellaneous.
   -  0x1: computer.
   -  0x2: phone (cellular, cordless, pay phone, modem).
   -  0x3: LAN, Network Access Point.
   -  0x4: audio/video (headset, speaker, stereo, video display, VCR).
   -  0x5: peripheral (mouse, joystick, keyboard).
   -  0x6: imaging (printer, scanner, camera, display).
   -  0x7: wearable.
   -  0x8: toy.
   -  0x9: health.
   -  0x1F: uncategorized device.

-  **<minor_dev_class>**: please refer to `Minor Device Class field <https://www.bluetooth.com/specifications/assigned-numbers/>`_.

-  **<major_srv_class>**:

   -  0x0: an invalid value.
   -  0x1: Limited Discoverable Mode.
   -  0x8: positioning (location identification).
   -  0x10: networking, such as LAN, Ad hoc.
   -  0x20: rendering, such as printing, speakers.
   -  0x40: capturing, such as scanner, microphone.
   -  0x80: object transfer, such as v-Inbox, v-Folder.
   -  0x100: audio, such as speaker, microphone, headerset service.
   -  0x200: telephony, such as cordless telephony, modem, headset service.
   -  0x400: information, such as WEB-server, WAP-server.

-  **<rssi>**: signal strength.

Example
^^^^^^^^

::

    AT+BTINIT=1
    AT+BTSCANMODE=2
    AT+BTSTARTDISC=0,10,10

.. _cmd-BTSPPINIT:

:ref:`AT+BTSPPINIT <BT-AT>`: Classic Bluetooth SPP Profile Initialization
------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the initialization status of Classic Bluetooth SPP profile.

**Command:**

::

    AT+BTSPPINIT?

**Response:**

If Classic Bluetooth SPP profile is initialized, it will return:

::

    +BTSPPINIT:1
    OK

If Classic Bluetooth SPP profile is not initialized, it will return:

::

    +BTSPPINIT:0
    OK


Set Command
^^^^^^^^^^^

**Function:**

Initialize or deinitialize Classic Bluetooth SPP profile.

**Command:**

::

    AT+BTSPPINIT=<init>

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<init>**:

   -  0: deinitialize Classic Bluetooth SPP profile.
   -  1: initialize Classic Bluetooth SPP profile, the role is master.
   -  2: initialize Classic Bluetooth SPP profile, the role is slave.

Example
^^^^^^^^

::

    AT+BTSPPINIT=1    // master
    AT+BTSPPINIT=2    // slave

.. _cmd-BTSPPCONN:

:ref:`AT+BTSPPCONN <BT-AT>`: Query/Establish SPP Connection
-------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query Classic Bluetooth SPP connection.

**Command:**

::

    AT+BTSPPCONN?

**Response:**

::

    +BTSPPCONN:<conn_index>,<remote_address>
    OK

If the connection has not been established, AT will return:

::

   +BTSPPCONN:-1

Set Command
^^^^^^^^^^^

**Function:**

Establish the Classic Bluetooth SPP connection.

**Command:**

::

    AT+BTSPPCONN=<conn_index>,<sec_mode>,<remote_address>

**Response:**

::

    OK

If the connection is established successfully, AT will return:

::

    +BTSPPCONN:<conn_index>,<remote_address>

Otherwise, AT will return:

::

    +BTSPPCONN:<conn_index>,-1

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Classic Bluetooth SPP connection. Only 0 is supported for the single connection right now.
-  **<sec_mode>**: 

   -  0x0000: no security.
   -  0x0001: authorization required (only needed for out going connection).
   -  0x0036: encryption required.
   -  0x3000: Man-In-The-Middle protection.
   -  0x4000: Min 16 digit for pin code.

-  **<remote_address>**: remote Classic Bluetooth SPP device address.

Example
^^^^^^^^

::

    AT+BTSPPCONN=0,0,"24:0a:c4:09:34:23"

.. _cmd-BTSPPDISCONN:

:ref:`AT+BTSPPDISCONN <BT-AT>`: End SPP Connection
--------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

End the Classic Bluetooth SPP connection.

**Command:**

::

    AT+BTSPPDISCONN=<conn_index>

**Response:**

::

    OK

If the command is successful, it will prompt:

::

    +BTSPPDISCONN:<conn_index>,<remote_address>

If the command is fail, it will prompt:

::

    +BTSPPDISCONN:-1

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Classic Bluetooth SPP connection. Only 0 is supported for the single connection right now.
-  **<remote_address>**: remote Classic Bluetooth A2DP device address.

Example
^^^^^^^^

::

    AT+BTSPPDISCONN=0

.. _cmd-BTSPPSEND:

:ref:`AT+BTSPPSEND <BT-AT>`: Send Data to Remote Classic Bluetooth SPP Device
-----------------------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Enter Classic Bluetooth SPP mode.

**Command:**

::

    AT+BTSPPSEND

**Response:**

::

    >   

Set Command
^^^^^^^^^^^^^^^

**Function:**

Send data to the remote Classic Bluetooth SPP device.

**Command:**

::

    AT+BTSPPSEND=<conn_index>,<data_len>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Classic Bluetooth SPP connection. Only 0 is supported for the single connection right now.
-  **<data_len>**: the length of the data which is ready to be sent.

Notes
^^^^^

-  The wrap return is > after this command is executed. Then, the {IDF_TARGET_NAME} enters UART Bluetooth passthrough mode. When the packet which only contains :ref:`+++ <cmd-PLUS>` is received, the device returns to normal command mode. Please wait for at least one second before sending the next AT command.

Example
^^^^^^^^

::

    AT+BTSPPSEND=0,100
    AT+BTSPPSEND

.. _cmd-BTSPPSTART:

:ref:`AT+BTSPPSTART <BT-AT>`: Start Classic Bluetooth SPP Profile
---------------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Start Classic Bluetooth SPP profile.

**Command:**

::

    AT+BTSPPSTART

**Response:**

::

    OK

Note
^^^^

-  During the SPP transmission, AT will not prompt any connection status changes unless bit2 of :ref:`AT+SYSMSG <cmd-SYSMSG>` is 1.

Example
^^^^^^^^

::

    AT+BTSPPSTART

.. _cmd-BTA2DPINIT:

:ref:`AT+BTA2DPINIT <BT-AT>`: Classic Bluetooth A2DP Profile Initialization
--------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the initialization status of Classic Bluetooth A2DP profile.

**Command:**

::

    AT+BTA2DPINIT?

**Response:**

If Classic Bluetooth A2DP profile is initialized, AT will return:

::

    +BTA2DPINIT:<role>

    OK

Otherwise, AT will return:

::

    +BTA2DPINIT:0

    OK

Set Command
^^^^^^^^^^^

**Function:**

Initialize or deinitialize Classic Bluetooth A2DP profile.

**Command:**

::

    AT+BTA2DPINIT=<role>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<role>**: role

   -  0: deinitialize Classic Bluetooth A2DP profile.
   -  1: source.
   -  2: sink.

Example
^^^^^^^^

::

    AT+BTA2DPINIT=2

.. _cmd-BTA2DPCONN:

:ref:`AT+BTA2DPCONN <BT-AT>`: Query/Establish A2DP Connection
---------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query Classic Bluetooth A2DP connection.

**Command:**

::

    AT+BTA2DPCONN?

**Response:**

::

    +BTA2DPCONN:<conn_index>,<remote_address>
    OK

If the connection has not been established, AT will NOT return the parameter <conn_index> and <remote_address>.

Set Command
^^^^^^^^^^^

**Function:**

Establish the Classic Bluetooth A2DP connection.

**Command:**

::

    AT+BTA2DPCONN=<conn_index>,<remote_address>

**Response:**

::

    OK

If the connection is established successfully, it will prompt the message below:

::

    +BTA2DPCONN:<conn_index>,<remote_address>

Otherwise, it will return:

::

    +BTA2DPCONN:<conn_index>,-1

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Classic Bluetooth A2DP connection. Only 0 is supported for the single connection right now.
-  **<remote_address>**: remote Classic Bluetooth A2DP device address.

Example
^^^^^^^^

::

    AT+BTA2DPCONN=0,0,0,"24:0a:c4:09:34:23"

.. _cmd-BTA2DPDISCONN:

:ref:`AT+BTA2DPDISCONN <BT-AT>`: End A2DP Connection
----------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

End the Classic Bluetooth A2DP connection.

**Command:**

::

    AT+BTA2DPDISCONN=<conn_index>

**Response:**

::

   +BTA2DPDISCONN:<conn_index>,<remote_address>
   OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Classic Bluetooth A2DP connection. Only 0 is supported for the single connection right now.
-  **<remote_address>**: remote Classic Bluetooth A2DP device address.

Example
^^^^^^^^

::

    AT+BTA2DPDISCONN=0

.. _cmd-BTA2DPSRC:

:ref:`AT+BTA2DPSRC <BT-AT>`: Query/Set the Audio File URL
-----------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the audio file URL.

**Command:**

::

    AT+BTA2DPSRC?

**Response:**

::

    +BTA2DPSRC:<url>,<type>
    OK

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Set the audio file URL.

**Command:**

::

    AT+BTA2DPSRC=<conn_index>,<url>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Classic Bluetooth A2DP connection. Only 0 is supported for the single connection right now.
-  **<url>**: the path of the source file. HTTP, HTTPS and FLASH are currently supported.
-  **<type>**: the type of audio file, such as "mp3".

Note
^^^^^

-  Only mp3 format is currently supported.

Example
^^^^^^^^

::

    AT+BTA2DPSRC=0,"https://dl.espressif.com/dl/audio/ff-16b-2c-44100hz.mp3"
    AT+BTA2DPSRC=0,"flash://spiffs/zhifubao.mp3"

.. _cmd-BTA2DPCTRL:

:ref:`AT+BTA2DPCTRL <BT-AT>`: Control the Audio Play
---------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Control the audio play.

**Command:**

::

    AT+BTA2DPCTRL=<conn_index>,<ctrl>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Classic Bluetooth A2DP connection. Only 0 is supported for the single connection right now.
-  **<ctrl>**: types of control.

   -  0: A2DP Sink, stop play.
   -  1: A2DP Sink, start play.
   -  2: A2DP Sink, forward.
   -  3: A2DP Sink, backward.
   -  4: A2DP Sink, fastward start.
   -  5: A2DP Sink, fastward stop.
   -  0: A2DP Source, stop play.
   -  1: A2DP Source, start play.
   -  2: A2DP Source, suspend.

Example
^^^^^^^^

::

    AT+BTA2DPCTRL=0,1  // start play audio

.. _cmd-BTSECPARAM:

:ref:`AT+BTSECPARAM <BT-AT>`: Query/Set the Classic Bluetooth Security Parameters
------------------------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query Classic Bluetooth security parameters.

**Command:**

::

    AT+BTSECPARAM?

**Response:**

::

    +BTSECPARAM:<io_cap>,<pin_type>,<pin_code>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Set the Classic Bluetooth security parameters.

**Command:**

::

    AT+BTSECPARAM=<io_cap>,<pin_type>,<pin_code>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<io_cap>**: input and output capability.

   -  0: DisplayOnly.
   -  1: DisplayYesNo.
   -  2: KeyboardOnly.
   -  3: NoInputNoOutput.

-  **<pin_type>**: use variable or fixed PIN.

   -  0: variable.
   -  1: fixed.

-  **<pin_code>**: Legacy Pair PIN Code. Maximum: 16 bytes.

Note
^^^^^

-  If you set the parameter ``<pin_type>`` to 0, ``<pin_code>`` will be ignored.

Example
^^^^^^^^

::

    AT+BTSECPARAM=3,1,"9527"

.. _cmd-BTKEYREPLY:

:ref:`AT+BTKEYREPLY <BT-AT>`: Input the Simple Pair Key
---------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Input the Simple Pair Key.

**Command:**

::

    AT+BTKEYREPLY=<conn_index>,<Key>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Classic Bluetooth connection. Currently, only 0 is supported for the single connection.
-  **<Key>**: the Simple Pair Key.

Example
^^^^^^^^

::

    AT+BTKEYREPLY=0,123456

.. _cmd-BTPINREPLY:

:ref:`AT+BTPINREPLY <BT-AT>`: Input the Legacy Pair PIN Code
-----------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Input the Legacy Pair PIN Code.

**Command:**

::

    AT+BTPINREPLY=<conn_index>,<Pin>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Classic Bluetooth connection. Currently, only 0 is supported for the single connection.
-  **<Pin>**: the Legacy Pair PIN Code.

Example
^^^^^^^^

::

    AT+BTPINREPLY=0,"6688"

.. _cmd-BTSECCFM:

:ref:`AT+BTSECCFM <BT-AT>`: Reply the Confirm Value to the Peer Device in the Legacy Connection Stage
----------------------------------------------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Reply the confirm value to the peer device in the legacy connection stage.

**Command:**

::

    AT+BTSECCFM=<conn_index>,<accept>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<conn_index>**: index of Classic Bluetooth connection. Currently, only 0 is supported for the single connection.
-  **<accept>**: reject or accept.

   -  0: reject.
   -  1: accept.

Example
^^^^^^^^

::

    AT+BTSECCFM=0,1

.. _cmd-BTENCDEV:

:ref:`AT+BTENCDEV <BT-AT>`: Query Classic Bluetooth Encryption Device List
----------------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the bound devices.

**Command:**

::

    AT+BTENCDEV?

**Response:**

::

    +BTENCDEV:<enc_dev_index>,<mac_address>
    OK

Parameters
^^^^^^^^^^

-  **<enc_dev_index>**: index of the bound devices.
-  **<mac_address>**: MAC address.

Example
^^^^^^^^

::

    AT+BTENCDEV?

.. _cmd-BTENCCLEAR:

:ref:`AT+BTENCCLEAR <BT-AT>`: Clear Classic Bluetooth Encryption Device List
------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Remove a device from the security database list with a specific index.

**Command:**

::

    AT+BTENCCLEAR=<enc_dev_index>

**Response:**

::

    OK

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Remove all devices from the security database.

**Command:**

::

    AT+BTENCCLEAR

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<enc_dev_index>**: index of the bound devices.

Example
^^^^^^^^

::

    AT+BTENCCLEAR

.. _cmd-BTCOD:

:ref:`AT+BTCOD <BT-AT>`: Set Class of Devices
-------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set the Classic Bluetooth class of devices.

**Command:**

::

    AT+BTCOD=<major>,<minor>,<service>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<major>**: `major class <https://btprodspecificationrefs.blob.core.windows.net/assigned-numbers/Assigned%20Number%20Types/Assigned_Numbers.pdf>`_.
-  **<minor>**: `minor class <https://btprodspecificationrefs.blob.core.windows.net/assigned-numbers/Assigned%20Number%20Types/Assigned_Numbers.pdf>`_.
-  **<service>**: `service class <https://btprodspecificationrefs.blob.core.windows.net/assigned-numbers/Assigned%20Number%20Types/Assigned_Numbers.pdf>`_.

Example
^^^^^^^^

::

    AT+BTCOD=6,32,32   // the printer

.. _cmd-BTPWR:

:ref:`AT+BTPOWER <BT-AT>`: Query/Set TX power of Classic Bluetooth
-------------------------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query Classic Bluetooth tx power level.

**Command:**

::

    AT+BTPOWER?

**Response:**

::

    +BTPOWER:<min_tx_power>,<max_tx_power>
    OK


Set Command
^^^^^^^^^^^

**Function:**

Set the Classic Bluetooth tx power.

**Command:**

::

    AT+BTPOWER=<min_tx_power>,<max_tx_power>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<min_tx_power>**: The minimum power level. Range: [0,7].
-  **<max_tx_power>**: The maximum power level. Range: [0,7].

Example
^^^^^^^^

::

    AT+BTPOWER=5,6   // set Classic Bluetooth tx power.
