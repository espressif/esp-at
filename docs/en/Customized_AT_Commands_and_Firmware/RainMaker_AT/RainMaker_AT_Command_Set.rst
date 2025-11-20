.. _RainMaker-AT:

RainMaker AT Command Set
================================

:link_to_translation:`zh_CN:[中文]`

.. important::
  The default AT firmware does not support the AT commands listed on this page. If you need {IDF_TARGET_NAME} to support RainMaker commands, you have two options:

  - Refer to the :doc:`How to Download the Latest Temporary Version of AT Firmware from GitHub <../../Compile_and_Develop/How_to_download_the_latest_temporary_version_of_AT_from_github>` documentation to download the esp32c3-rainmaker-at firmware.
  - Compile the ESP-AT project yourself by following the steps in :doc:`Compiling the ESP-AT Project </Compile_and_Develop/How_to_clone_project_and_compile_it>`. During the third step (Install Environment), choose ``PLATFORM_ESP32C3`` for the ``Platform name`` and choose ``ESP32C3_RAINMAKER`` for the ``Module name``.

-  :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>`: Initialize the node.
-  :ref:`AT+RMNODEATTR <cmd-RMNODEATTR>`: Attributes operations.
-  :ref:`AT+RMDEV <cmd-RMDEV>`: Device operations.
-  :ref:`AT+RMPARAM <cmd-RMPARAM>`: Device parameters operations.
-  :ref:`AT+RMPARAMBOUNDS <cmd-RMPARAMBOUNDS>`: Add bounds to numeric parameters.
-  :ref:`AT+RMPARAMSTRLIST <cmd-RMPARAMSTRLIST>`: Add a list of strings to a character parameters.
-  :ref:`AT+RMPARAMCOUNT <cmd-RMPARAMCOUNT>`: Add maximum number of elements to array parameters.
-  :ref:`AT+RMUSERMAPPING <cmd-RMUSERMAPPING>`: Start user-node mapping.
-  :ref:`AT+RMUSERUNMAPPING <cmd-RMUSERUNMAPPING>`: Clear user-node mapping.
-  :ref:`AT+RMPROV <cmd-RMPROV>`: Provisioning and user-node mapping.
-  :ref:`AT+RMCONN <cmd-RMCONN>`: Connect to ESP RainMaker cloud.
-  :ref:`AT+RMCLOSE <cmd-RMCLOSED>`: Actively disconnect from ESP RainMaker cloud.
-  :ref:`AT+RMPARAMUPDATE <cmd-RMPARAMUPDATE>`: Update parameters.
-  :ref:`AT+RMMODE <cmd-RMMODE>`: Set the transmission mode.
-  :ref:`AT+RMSEND <cmd-RMSEND>`: Send data in the :term:`RainMaker Normal Transmission Mode` or :term:`RainMaker Passthrough Mode`.
-  :ref:`AT+RMOTARESULT <cmd-RMOTARESULT>`: Send the OTA result.
-  :ref:`AT+RMOTAFETCH <cmd-RMOTAFETCH>`: Fetch OTA information.

.. _cmd-RMNODEINIT:

:ref:`AT+RMNODEINIT <RainMaker-AT>`: Initialize the Node
----------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+RMNODEINIT

**Response:**

::

    OK

After the command is executed successfully, the node configuration will be saved internally in JSON format as follows.

.. code-block:: none

     {
         "node_id":"xxxxxxxxxxxx",
         "config_version":"xxxx-xx-xx",
         "info":{
             "name":"ESP RainMaker AT Node",
             "fw_version":"xxxxxxx",
             "type":"AT Node",
             "model":"esp-at",
             "project_name":"esp-at",
             "platform":"{IDF_TARGET_PATH_NAME}"
         },
         "devices":[

         ],
         "services":[
             {
                 "name":"System",
                 "type":"esp.service.system",
                 "params":[
                     {
                         "name":"Reboot",
                         "type":"esp.param.reboot",
                         "data_type":"bool",
                         "properties":[
                             "read",
                             "write"
                         ]
                     },
                     {
                         "name":"Factory-Reset",
                         "type":"esp.param.factory-reset",
                         "data_type":"bool",
                         "properties":[
                             "read",
                             "write"
                         ]
                     },
                     {
                         "name":"Wi-Fi-Reset",
                         "type":"esp.param.wifi-reset",
                         "data_type":"bool",
                         "properties":[
                             "read",
                             "write"
                         ]
                     }
                 ]
             },
             {
                 "name":"Time",
                 "type":"esp.service.time",
                 "params":[
                     {
                         "name":"TZ",
                         "type":"esp.param.tz",
                         "data_type":"string",
                         "properties":[
                             "read",
                             "write"
                         ]
                     },
                     {
                         "name":"TZ-POSIX",
                         "type":"esp.param.tz_posix",
                         "data_type":"string",
                         "properties":[
                             "read",
                             "write"
                         ]
                     }
                 ]
             },
             {
                 "name":"Schedule",
                 "type":"esp.service.schedule",
                 "params":[
                     {
                         "name":"Schedules",
                         "type":"esp.param.schedules",
                         "data_type":"array",
                         "properties":[
                             "read",
                             "write"
                         ],
                         "bounds":{
                             "max":10
                         }
                     }
                 ]
             }
         ]
     }

Note
^^^^^

-  This command should be executed before any other ESP RainMaker AT commands.
-  This command enables the services of system management, OTA, time, and scheduling by default.
-  This command will first obtain the authentication information from the mass manufacturing partition ``rmaker_mfg``. If the information is not found, it will search the system NVS. If it is still not found, it will perform claiming.
-  This command will load the parameters from the mass manufacturing partition ``rmaker_mfg``. If the parameters do not exist, the default configuration will be used to create a node automatically.
-  After the command is executed successfully, the node configuration will be saved internally. There are some default key-value pairs in the node configuration.

   -  node_id: Unique identifier derived from certificates and cannot be changed.
   -  config_version: Temporarily no practical use. No need to change.
   -  name: Fixed to "ESP RainMaker AT Node".
   -  fw_version: RainMaker AT version information.
   -  type: Fixed to "AT Node".
   -  model: Fixed to "esp-at".
   -  project_name: Fixed to "esp-at".
   -  platform: Fixed to "{IDF_TARGET_NAME}".
   -  services: system, OTA, time, and scheduling services.

.. _cmd-RMNODEATTR:

:ref:`AT+RMNODEATTR <RainMaker-AT>`: Attributes Operations
-------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+RMNODEATTR=<"name1">,<"value1">[<"name2">,<"value2">,<"name3">,<"value3">,...,<"name8">,<"value8">]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<"name">**: node attribute key name.
-  **<"value">**: node attribute value.

Note
^^^^^

-  This command should be used before the device is connected to the RainMaker cloud (please refer to :ref:`AT+RMPROV <cmd-RMPROV>` or :ref:`AT+RMCONN <cmd-RMCONN>`).

Example
^^^^^^^^

::

    AT+RMNODEATTR="serial_num","123abc"

.. _cmd-RMDEV:

:ref:`AT+RMDEV <RainMaker-AT>`: Device Operations
--------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+RMDEV=<dev_opt>,<"unique_name">,<"device_name">,<"device_type">

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<"dev_opt">**: device operations.

   -  0: Add a device.
   -  1: Delete a device.

-  **<"unique_name">**: device unique name.
-  **<"device_name">**: device name, which will be used as the default device name displayed on the app.
-  **<"device_type">**: device type. Please refer to `Devices <https://rainmaker.espressif.com/docs/standard-types.html#devices>`__.

Note
^^^^^

-  This command should be used before the device is connected to the RainMaker cloud (please refer to :ref:`AT+RMPROV <cmd-RMPROV>` or :ref:`AT+RMCONN <cmd-RMCONN>`).
-  Currently, only one device can be added to a node.
-  With the command being executed successfully, the device is added to the node configuration. The default type in the params is "esp.param.name", the data type in the params is "string", and the properties are "read" and "write".

Example
^^^^^^^^

::

    AT+RMDEV=0,"Light","Light","esp.device.light"

After the command is executed successfully, the device "Light" will be added to the node configuration and saved internally in JSON format as follows (For node configuration, please refer to :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>`).

.. code-block:: none

     {
         "node_id":"xxxxxxxxxxxx",
         "config_version":"xxxx-xx-xx",
         "info":{
             "name":"ESP RainMaker AT Node",
             "fw_version":"xxxxxxx",
             "type":"AT Node",
             "model":"esp-at",
             "project_name":"esp-at",
             "platform":"{IDF_TARGET_PATH_NAME}"
         },
         "attributes":[
             {
                 "name":"serial_num",
                 "value":"123abc"
             }
         ],
         "devices":[
             {
                 "name":"Light",
                 "type":"esp.device.light",
                 "params":[
                     {
                         "name":"Name",
                         "type":"esp.param.name",
                         "data_type":"string",
                         "properties":[
                             "read",
                             "write"
                         ]
                     }
                 ]
             }
         ],
         "services":[
             {
                 "name":"System",
                 "type":"esp.service.system",
                 "params":[
                     {
                         "name":"Reboot",
                         "type":"esp.param.reboot",
                         "data_type":"bool",
                         "properties":[
                             "read",
                             "write"
                         ]
                     },
                     {
                         "name":"Factory-Reset",
                         "type":"esp.param.factory-reset",
                         "data_type":"bool",
                         "properties":[
                             "read",
                             "write"
                         ]
                     },
                     {
                         "name":"Wi-Fi-Reset",
                         "type":"esp.param.wifi-reset",
                         "data_type":"bool",
                         "properties":[
                             "read",
                             "write"
                         ]
                     }
                 ]
             },
             {
                 "name":"Time",
                 "type":"esp.service.time",
                 "params":[
                     {
                         "name":"TZ",
                         "type":"esp.param.tz",
                         "data_type":"string",
                         "properties":[
                             "read",
                             "write"
                         ]
                     },
                     {
                         "name":"TZ-POSIX",
                         "type":"esp.param.tz_posix",
                         "data_type":"string",
                         "properties":[
                             "read",
                             "write"
                         ]
                     }
                 ]
             },
             {
                 "name":"Schedule",
                 "type":"esp.service.schedule",
                 "params":[
                     {
                         "name":"Schedules",
                         "type":"esp.param.schedules",
                         "data_type":"array",
                         "properties":[
                             "read",
                             "write"
                         ],
                         "bounds":{
                             "max":10
                         }
                     }
                 ]
             }
         ]
     }

.. _cmd-RMPARAM:

:ref:`AT+RMPARAM <RainMaker-AT>`: Device Parameters Operations
------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Add parameters to the device.

**Command:**

::

    AT+RMPARAM=<"unique_name">,<"param_name">,<"param_type">,<data_type>,<properties>,<"ui_type">,<"def">

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<"unique_name">**: device unique name.
-  **<"param_name">**: parameter name.
-  **<"param_type">**: parameter type. Please refer to `Parameters <https://rainmaker.espressif.com/docs/standard-types.html#parameters>`__.
-  **<data_type>**: data type.

   -  bit 0: boolean.
   -  bit 1: integer.
   -  bit 2: floating-point number.
   -  bit 3: string.
   -  bit 4: object.
   -  bit 5: array.

-  **<properties>**: data properties.

   -  bit 0: read.
   -  bit 1: write.
   -  bit 2: time_series.
   -  bit 3: persist.

-  **<"ui_type">**: UI type. Please refer to `UI Elements <https://rainmaker.espressif.com/docs/standard-types.html#ui-elements>`__.
-  **<"def">**: default value.

Note
^^^^^

-  This command should be used before the device is connected to the RainMaker cloud (please refer to :ref:`AT+RMPROV <cmd-RMPROV>` or :ref:`AT+RMCONN <cmd-RMCONN>`).
-  Please make sure the parameter ``<def>`` matches the parameter ``<data_type>``. AT does not check it internally.
-  In the :term:`RainMaker Passthrough Mode`, only one parameter is allowed in the device (the default parameter created by the :ref:`AT+RMDEV <cmd-RMDEV>` command is not included). If there are multiple parameters, the device cannot enter the :term:`RainMaker Passthrough Mode`.

Example
^^^^^^^^

::

    AT+RMPARAM="Light","Brightness","esp.param.brightness",2,3,"esp.ui.slider","50"

.. _cmd-RMPARAMBOUNDS:

:ref:`AT+RMPARAMBOUNDS <RainMaker-AT>`: Add Bounds to Numeric Parameters
----------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+RMPARAMBOUNDS=<"unique_name">,<"param_name">,<"min">,<"max">,<"step">

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<"unique_name">**: device unique name.
-  **<"param_name">**: parameter name.
-  **<"min">**: minimum value.
-  **<"max">**: maximum value.
-  **<"step">**: step value.

Note
^^^^^

-  This command should be used before the device is connected to the RainMaker cloud (please refer to :ref:`AT+RMPROV <cmd-RMPROV>` or :ref:`AT+RMCONN <cmd-RMCONN>`).
-  This command is only for parameter whose type is an integer or a floating-point number (please refer to the parameter ``<data_type>`` in the :ref:`AT+RMPARAM <RainMaker-AT>`). Please make sure the parameters ``<"min">``, ``<"max">``, and ``<"step">`` match the parameter ``<data_type>``. AT does not check it internally.

Example
^^^^^^^^

::

    AT+RMPARAMBOUNDS="Switch","brightness","0","100","1"

After the command is executed successfully, the "bounds" will be added to the device "Switch" and saved internally in JSON format as follows (For node configuration, please refer to :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>`).

.. code-block:: none

     {
         "name":"Brightness",
         "type":"esp.param.brightness",
         "data_type":"int",
         "properties":[
             "read",
             "write"
         ],
         "bounds":{
             "min":0,
             "max":100,
             "step":1
         },
         "ui_type":"esp.ui.slider"
     }

.. _cmd-RMPARAMSTRLIST:

:ref:`AT+RMPARAMSTRLIST <RainMaker-AT>`: Add a List of Strings to a Character Parameters
------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+RMPARAMSTRLIST=<"unique_name">,<"param_name">,<"str1">[,<"str2">,<"str3">,...,<"str14">]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<"unique_name">**: device unique name.
-  **<"param_name">**: parameter name.
-  **<"str">**: members of a list of strings.

Note
^^^^^

-  This command should be used before the device is connected to the RainMaker cloud (please refer to :ref:`AT+RMPROV <cmd-RMPROV>` or :ref:`AT+RMCONN <cmd-RMCONN>`).
-  This command is only for parameter whose type is a string (please refer to the parameter ``<data_type>`` in the :ref:`AT+RMPARAM <RainMaker-AT>`). Please make sure the parameters ``<"str">`` matches the parameter ``<data_type>``. AT does not check it internally.

Example
^^^^^^^^

::

    AT+RMPARAM="Light","Color","esp.param.color",4,3,"esp.ui.dropdown","white"

    AT+RMPARAMSTRLIST="Light","Color","white","red","blue","yellow"

After the command is executed successfully, the "valid_strs" will be added to the device "Light" and saved internally in JSON format as follows (For node configuration, please refer to :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>`).

.. code-block:: none

     {
         "name":"Color",
         "type":"esp.param.color",
         "data_type":"string",
         "properties":[
             "read",
             "write"
         ],
         "valid_strs":[
             "white",
             "red",
             "blue",
             "yellow"
         ],
         "ui_type":"esp.ui.dropdown"
     }

.. _cmd-RMPARAMCOUNT:

:ref:`AT+RMPARAMCOUNT <RainMaker-AT>`: Add Maximum Number of Elements to Array Parameters
---------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+RMPARAMCOUNT=<"unique_name">,<"param_name">,<array_count>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<"unique_name">**: device unique name.
-  **<"param_name">**: parameter name.
-  **<array_count>**: maximum number of elements in an array.

Note
^^^^^

-  This command should be used before the device is connected to the RainMaker cloud (please refer to :ref:`AT+RMPROV <cmd-RMPROV>` or :ref:`AT+RMCONN <cmd-RMCONN>`).
-  This command is only for parameter whose type is array (please refer to the parameter ``<data_type>`` in the :ref:`AT+RMPARAM <RainMaker-AT>`). Please make sure the parameters ``<array_count>`` matches the parameter ``<data_type>``, AT doesn't check internally.

Example
^^^^^^^^

::

    AT+RMPARAM="Light","Color","esp.param.color",6,3,"esp.ui.hidden",""

    AT+RMPARAMCOUNT="Light","Color",5

After the command is executed successfully, the "bounds" will be added to the device "Light" and saved internally in JSON format as follows (For node configuration, please refer to :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>`).

.. code-block:: none

     {
         "name":"Color",
         "type":"esp.param.color",
         "data_type":"array",
         "properties":[
             "read",
             "write"
         ],
         "bounds":{
             "max":5
         },
         "ui_type":"esp.ui.hidden"
     }

.. _cmd-RMUSERMAPPING:

:ref:`AT+RMUSERMAPPING <RainMaker-AT>`: Start User-Node Mapping
-----------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+RMUSERMAPPING=<"user_id">,<"secret_key">

**Response:**

::

    OK

If the mapping completes, the system returns:

::

  +RMMAPPINGDONE

Parameters
^^^^^^^^^^

-  **<"user_id">**: user identifier.
-  **<"secret_key">**: secret key.

Note
^^^^^

-  Before executing this command, please make sure the device is connected to the cloud. For more information, please refer to :ref:`AT+RMCONN <cmd-RMCONN>`.
-  This command does not guarantee the success of the actual mapping. The mapping status needs to be checked separately by the clients (phone app/CLI).

.. _cmd-RMUSERUNMAPPING:

:ref:`AT+RMUSERUNMAPPING <RainMaker-AT>`: Clear User-Node Mapping
-----------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+RMUSERUNMAPPING

**Response:**

::

    OK

.. _cmd-RMPROV:

:ref:`AT+RMPROV <RainMaker-AT>`: Provisioning and User-Node Mapping
-----------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+RMPROV=<mode>[,<customer_id>,<device_extra_code>,<"broadcast_name">]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<mode>**: mode.

   -  0: start provisioning and user-node mapping.
   -  1: stop provisioning.

-  **<customer_id>**: customer identifier, used to distinguish different customers. Range:[0,65535]. If you want to use the `Nova Home <https://rainmaker.espressif.com>`__, please `contact us <https://www.espressif.com/en/contact-us/sales-questions>`__.
-  **<device_extra_code>**: device code, used to identify the device icon when provisioning. Range:[0,255].
-  **<"broadcast_name">**: device name when customizing the Bluetooth broadcast. Range:[0,12]. Unit: byte.

.. _cmd-RMCONN:

:ref:`AT+RMCONN <RainMaker-AT>`: Connect to ESP RainMaker cloud
-----------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+RMCONN=<conn_timeout>

**Response:**

If the device connects to the cloud successfully, the system returns:

::

    +RMCONNECTED
    OK

If the device fails to connect to the cloud, the system returns:

::

    ERROR

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+RMCONN

**Response:**

If the device connects to the cloud successfully, the system returns:

::

    +RMCONNECTED
    OK

If the device fails to connect to the cloud, the system returns:

::

    ERROR

Parameters
^^^^^^^^^^

-  **<conn_timeout>**: maximum timeout for connection. Range:[3,600]. Unit: second. Default: 15.

.. _cmd-RMCLOSED:

:ref:`AT+RMCLOSE <RainMaker-AT>`: Actively Disconnect from ESP RainMaker Cloud
---------------------------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+RMCLOSE

**Response:**

::

    OK

Note
^^^^^

-  When the device actively calls this command to disconnect from the cloud, AT will not report the `+RMDISCONNECTED` message. Only when the device is passively disconnected from the cloud, AT will report the `+RMDISCONNECTED` message.

.. _cmd-RMPARAMUPDATE:

:ref:`AT+RMPARAMUPDATE <RainMaker-AT>`: Update Parameters
---------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+RMPARAMUPDATE=<"unique_name">,<"param_name1">,<"param_value1">[,<"param_name2">,<"param_value2">,...,<"param_name7">,<"param_value7">]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<"unique_name">**: device unique name.
-  **<"param_name">**: parameter name.
-  **<"param_value">**: parameter value.

Note
^^^^^

-  The parameter ``<"param_value">`` must match the parameter ``<data_type>`` set in :ref:`AT+RMPARAM <cmd-RMPARAM>`.
-  The command supports up to 15 parameters, namely, 1 ``<"unique_name">`` + 7 ``<"param_name">`` + 7 ``<"param_value">``.
-  The length of the entire AT command should be less than ``256`` bytes. If the amount of data you want to update is relatively large, please use the :ref:`AT+RMSEND <cmd-RMSEND>` command.

Example
^^^^^^^^

::

    AT+RMPARAMUPDATE="Light","Power","1"

.. _cmd-RMMODE:

:ref:`AT+RMMODE <RainMaker-AT>`: Set the Transmission Mode
-------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+RMMODE=<mode>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<mode>**: transmission mode.

   -  0: :term:`RainMaker Normal Transmission Mode`.
   -  1: :term:`RainMaker Passthrough Mode`.

Note
^^^^^

-  In the :term:`RainMaker Passthrough Mode`, only one parameter is allowed in the devices (the default parameter created by the :ref:`AT+RMDEV <cmd-RMDEV>` command is not included). If there are multiple parameters, the device cannot enter the :term:`RainMaker Passthrough Mode`.

.. _cmd-RMSEND:

:ref:`AT+RMSEND <RainMaker-AT>`: Send Data in the :term:`RainMaker Normal Transmission Mode` or :term:`RainMaker Passthrough Mode`
----------------------------------------------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Send data of specified length in the :term:`RainMaker Normal Transmission Mode`.

**Command:**

::

    AT+RMSEND=<"unique_name">,<"param_name">,<len>

**Response:**

::

    OK

    >

This response indicates that AT is ready for receiving serial data. You should enter the data, and when the data length reaches the `<len>` value, the system returns:

::

    Recv <len> bytes

If not all the data has been sent out, the system finally returns:

::

    SEND FAIL

If all the data is transmitted successfully, the system finally returns:

::

    SEND OK

Execute Command
^^^^^^^^^^^^^^^

**Function:**

Enter the :term:`RainMaker Passthrough Mode`.

**Command:**

::

    AT+RMSEND

**Response:**

::

    OK

    >

or

::

    ERROR

Enter the :term:`RainMaker Passthrough Mode`. When a single packet containing ``+++`` is received, the {IDF_TARGET_NAME} will exit the data sending mode under the :term:`RainMaker Passthrough Mode`. Please wait for at least one second before sending the next AT command.

Parameters
^^^^^^^^^^

-  **<"unique_name">**: device unique name.
-  **<"param_name">**: parameter name.
-  **<len>**: data length. The length depends on the RAM size. You can use :ref:`AT+SYSRAM <cmd-SYSRAM>` to query the remaining RAM size.

Note
^^^^^

-  In the :term:`RainMaker Passthrough Mode`, only one parameter in the devices is allowed (the default parameter created by the :ref:`AT+RMDEV <cmd-RMDEV>` command is not included). If there are multiple parameters, the device cannot enter the :term:`RainMaker Passthrough Mode`.

-  If you want to update multiple parameters at the same time, please refer to :ref:`AT+RMPARAMUPDATE <cmd-RMPARAMUPDATE>` command.

.. _cmd-RMOTARESULT:

:ref:`AT+RMOTARESULT <RainMaker-AT>`: Send the OTA Result
----------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+RMOTARESULT=<type>,<"ota_job_id">,<result>,<"additional_info">

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<type>**: reserved.
-  **<"ota_job_id">**: OTA job ID.
-  **<result>**: OTA result.

   -  1: OTA in progress.
   -  2: OTA succeeded.
   -  3: OTA failed.
   -  4: OTA delayed by the application.
   -  5: OTA rejected due to some reason.

-  **<"additional_info">**: Additional information for the OTA status.

Note
^^^^^

-  This command is only applicable to host MCU OTA. For {IDF_TARGET_NAME} Wi-Fi OTA, the system will automatically send the OTA status.

.. _cmd-RMOTAFETCH:

:ref:`AT+RMOTAFETCH <RainMaker-AT>`: Fetch OTA Information
-------------------------------------------------------------

Execute Command
^^^^^^^^^^^^^^^^^

**Command:**

::

    AT+RMOTAFETCH

**Response:**

::

    OK

Note
^^^^^

-  For host MCU OTA, ESP-AT will send the received OTA information instantly to the host MCU in the format of ``+RMFWNOTIFY:<type>,<size>,<url>,<fw_version>,<ota_job_id>``.

    -  **<type>**: reserved. It is always set to 0 by ESP-AT.
    -  **<size>**: host MCU OTA firmware size. Unit: byte.
    -  **<url>**: host MCU OTA download URI. You can use the :ref:`AT+HTTPCGET <cmd-HTTPCGET>` command to download firmware.
    -  **<fw_version>**: host MCU OTA firmware version.
    -  **<ota_job_id>**: host MCU OTA job ID. You can use the :ref:`AT+RMOTARESULT <cmd-RMOTARESULT>` command to send the OTA result.

-  For {IDF_TARGET_NAME} Wi-Fi OTA, the system will automatically executes the OTA. ESP-AT will send the OTA status to the host MCU in the format of ``+RMOTA:<status>``.

    -  1: OTA in progress.
    -  2: OTA succeeded.
    -  3: OTA failed.
    -  4: OTA delayed by the application.
    -  5: OTA rejected due to some reason.

-  Please refer to :doc:`RainMaker_AT_OTA_Guide` for how to implement OTA via ESP RainMaker cloud.
