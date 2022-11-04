RainMaker AT Examples
================================

:link_to_translation:`zh_CN:[中文]`

This document provides an introduction and detailed command examples to illustrate how to utilize :doc:`./RainMaker_AT_Command_Set` on {IDF_TARGET_NAME}.

.. contents::
   :local:
   :depth: 1

Introduction
-------------

**Espressif** provides an official application `Nova Home <https://rainmaker.espressif.com>`__ to interact with the devices. You can download it through the `Google Play (Android) <https://play.google.com/store/apps/details?id=com.espressif.novahome&hl=en_US&gl=US>`__ or `App Store (iOS) <https://apps.apple.com/us/app/nova-home/id1563728960>`__. The examples in the document use Nova Home (version: 1.6.2) as the client application by default.

.. _simple-communi:

Connect to the RainMaker cloud for simple communications in normal transmission transmode
-------------------------------------------------------------------------------------------

.. _init-node:

1. Initialize the node.

   {IDF_TARGET_NAME} initializes the node:

   Command:

   .. code-block:: none

     AT+RMNODEINIT

   Response:

   .. code-block:: none

     OK

   Note:

   After the command is executed successfully, the node configuration will be saved internally. The node configuration that has been initialized is as follows.

   .. code-block:: none

     {
         "node_id":"xxxxxxxxxxxx",
         "config_version":"2020-03-20",
         "info":{
             "name":"ESP RainMaker AT Node",
             "fw_version":"2.4.0(esp32c3_MINI-1_4b42408):Sep 20 2022 11:58:48",
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

.. _add-attr:

2. Attributes operations. (optional)

   For example, {IDF_TARGET_NAME} adds metadata with name "serial_num" and value "123abc" to the node.

   Command:

   .. code-block:: none

     AT+RMNODEATTR="serial_num","123abc"

   Response:

   .. code-block:: none

     OK

   Note:

   After the command is executed successfully, the new attributes are added to the node configuration.

   .. code-block:: none

     {
         "node_id":"xxxxxxxxxxxx",
         "config_version":"2020-03-20",
         "info":Object{...},
         "attributes":[
             {
                 "name":"serial_num",
                 "value":"123abc"
             }
         ],
         "devices":[

         ],
         "services":Array[3]
     }

.. _add-device:

3. Add a device.

   For example, {IDF_TARGET_NAME} adds a device with the unique name "Light" and device name "Light", and the device type is "esp.device.light".

   Command:

   .. code-block:: none

     AT+RMDEV=0,"Light","Light","esp.device.light"

   Response:

   .. code-block:: none

     OK

   Note:

   After the command is executed successfully, the device is added to the node configuration.

   .. code-block:: none

     {
         "node_id":"xxxxxxxxxxxx",
         "config_version":"2020-03-20",
         "info":Object{...},
         "attributes":Array[1],
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
         "services":Array[3]
     }

.. _add-params:

4. Add parameters to the device.

   For example, {IDF_TARGET_NAME} adds the "Power" and "Brightness" parameters to the "Light" device.

   The parameter name is "Power", parameter type is "esp.param.power", data type is boolean, properties are read and write, UI type is "esp.ui.toggle", and default value is false ("0").

   Command:

   .. code-block:: none

     AT+RMPARAM="Light","Power","esp.param.power",1,3,"esp.ui.toggle","0"

   Response:

   .. code-block:: none

     OK

   The parameter name is "Brightness", parameter type is "esp.param.brightness", data type is integer, properties are read and write, UI type is "esp.ui.slider", and default value is "50".

   Command:

   .. code-block:: none

     AT+RMPARAM="Light","Brightness","esp.param.brightness",2,3,"esp.ui.slider","50"

   Response:

   .. code-block:: none

     OK

   Note:

   After the above two commands are executed successfully, "Power" and "Brightness" parameters are added to the device.

   .. code-block:: none

     {
         "node_id":"XXXXXXXXXXXX",
         "config_version":"2020-03-20",
         "info":Object{...},
         "attributes":Array[1],
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
                     },
                     {
                         "name":"Brightness",
                         "type":"esp.param.brightness",
                         "data_type":"int",
                         "properties":[
                             "read",
                             "write"
                         ],
                         "ui_type":"esp.ui.slider"
                     },
                     {
                         "name":"Power",
                         "type":"esp.param.power",
                         "data_type":"bool",
                         "properties":[
                             "read",
                             "write"
                         ],
                         "ui_type":"esp.ui.toggle"
                     }
                 ]
             }
         ],
         "services":Array[3]
     }

.. _prov-and-mapping:

5. Perform provisioning and user-node mapping.

   The Nova Home app is used as the client.

   Command:

   .. code-block:: none

     AT+RMPROV=0

   Response:

   .. code-block:: none

     WIFI DISCONNECT

     OK

   If {IDF_TARGET_NAME} has been connected to an AP before, it will be disconnected first. Then, in the upper right corner of the app, you can tap the "+" button > ``Add Device``, and then the app scans via Bluetooth LE and displays the found devices.

   .. figure:: ../../../_static/RainMaker/nova_add_device.png
      :align: center
      :scale: 30 %
      :alt: Nova Home Adds Device
      :figclass: align-center

      Nova Home Adds Device

   .. figure:: ../../../_static/RainMaker/nova_found_devices.png
      :align: center
      :scale: 30 %
      :alt: Nova Home Found Devices
      :figclass: align-center

      Nova Home Found Devices

   Tap the device for Bluetooth LE provisioning, and enter the SSID and PASSWORD in the ``Connecting Device`` screen. Then, provisioning starts.

   .. figure:: ../../../_static/RainMaker/nova_provisioning_starts.png
      :align: center
      :scale: 30 %
      :alt: Nova Home Starts Provisioning
      :figclass: align-center

      Nova Home Starts Provisioning

   In the provisioning process and after the provisioning is successful, the system will prompts:

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     +RMCONNECTED
     +RMMAPPINGDONE

   After that, you can set the name and room of the device displayed on the app. After completing the above operations, you can see the added device on the app.

   .. figure:: ../../../_static/RainMaker/nova_device.png
      :align: center
      :scale: 30 %
      :alt: Nova Home Device
      :figclass: align-center

      Nova Home Device

6. Update parameters.

   The light displayed on the app is OFF. Modify the value of the "Power" parameter to true ("1").

   Command:

   .. code-block:: none

     AT+RMPARAMUPDATE="Light","Power","1"

   Response:

   .. code-block:: none

     OK

   After that, you can see that the status of the light on the app is switched from OFF to ON.

   .. figure:: ../../../_static/RainMaker/nova_change_light_status.png
      :align: center
      :scale: 30 %
      :alt: Nova Home Changes Light Status
      :figclass: align-center

      Nova Home Changes Light Status

7. Control the device remotely.

   You can control the device with the app. For example, you can change the status of the light from ON to OFF. When {IDF_TARGET_NAME} receives a control message, the system will prompts:

   .. code-block:: none

     +RMRECV:Local,Light,Power:0

.. _close-agent:

8. Actively disconnect from ESP RainMaker cloud.

   Command:

   .. code-block:: none

     AT+RMCLOSE

   Response:

   .. code-block:: none

     OK

Connect to the RainMaker cloud for simple communications in passthrough mode
------------------------------------------------------------------------------

1. Initialize the node.

   Please refer to :ref:`Initialize the node <init-node>` in the example :ref:`Connect to the RainMaker cloud for simple communications in normal transmission transmode <simple-communi>`.

2. Attributes operations. (optional)

   Please refer to :ref:`Attributes operations (optional) <add-attr>` in the example :ref:`Connect to the RainMaker cloud for simple communications in normal transmission transmode <simple-communi>`.

3. Add a device.

   Please refer to :ref:`Add a device <add-device>` in the example :ref:`Connect to the RainMaker cloud for simple communications in normal transmission transmode <simple-communi>`.

4. Add parameters to the device.

   In the :term:`RainMaker Passthrough Mode`, only one parameter is allowed in the devices (the default parameter created by the :ref:`AT+RMDEV <cmd-RMDEV>` command is not included). If there are multiple parameters, the device cannot enter the :term:`RainMaker Passthrough Mode`.
   
   For example, {IDF_TARGET_NAME} adds the "Power" parameter to the "Light" device. The parameter name is "Power", parameter type is "esp.param.power", data type is bool, properties are read and write, UI type is "esp.ui.toggle", and default value is false ("0").

   Command:

   .. code-block:: none

     AT+RMPARAM="Light","Power","esp.param.power",1,3,"esp.ui.toggle","0"

   Response:

   .. code-block:: none

     OK

   Note:

   After the command is executed successfully, the "Power" parameter is added to the device.

   .. code-block:: none

     {
         "node_id":"XXXXXXXXXXXX",
         "config_version":"2020-03-20",
         "info":Object{...},
         "attributes":Array[1],
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
                     },
                     {
                         "name":"Power",
                         "type":"esp.param.power",
                         "data_type":"bool",
                         "properties":[
                             "read",
                             "write"
                         ],
                         "ui_type":"esp.ui.toggle"
                     }
                 ]
             }
         ],
         "services":Array[3]
     }

5. Perform provisioning and user-node mapping.

   Please refer to :ref:`Perform provisioning and user-node mapping <prov-and-mapping>` in the example :ref:`Connect to the RainMaker cloud for simple communications in normal transmission transmode <simple-communi>`.

6. Enter passthrough mode.

   Command:

   .. code-block:: none

     AT+RMMODE=1

   Response:

   .. code-block:: none

     OK

7. Send data.

   Command:

   .. code-block:: none

     AT+RMSEND

   Response:

   .. code-block:: none

     OK

     >

   Then you can directly enter the data, enter ``1`` to switch the light to ON, and ``0`` to switch the light to OFF.

8. Stop sending data.

   When AT receives a packet that contains only ``+++``, the :term:`RainMaker Passthrough Mode` will be stopped. Then please wait at least one second before sending the next AT command. Please note that if you input ``+++`` directly by typing, the ``+++`` may not be recognized as three consecutive ``+`` because of the prolonged typing duration.

   .. Important::

     The aim of ending the packet with :ref:`+++ <cmd-PLUS>` is to exit :term:`RainMaker Passthrough Mode` and to accept normal AT commands. However, you can also use command ``AT+RMSEND`` to go back into :term:`RainMaker Passthrough Mode`.

9. Actively disconnect from ESP RainMaker cloud.

   Please refer to :ref:`Actively disconnect from ESP RainMaker cloud <close-agent>` in the example :ref:`Connect to the RainMaker cloud for simple communications in normal transmission transmode <simple-communi>`.

Perform user-node mapping.
----------------------------

1. Set the Wi-Fi mode to station.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:

   .. code-block:: none

     OK

2. Connect to the router.

   Command:

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   Response:

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   Note:

   The SSID and password you entered may be different from those in the above command. Please replace the SSID and password with those of your router settings.

3. Initialize the node.

   Please refer to :ref:`Initialize the node <init-node>` in the example :ref:`Connect to the RainMaker cloud for simple communications in normal transmission transmode <simple-communi>`.

4. Attributes Operations. (optional)

   Please refer to :ref:`Attributes operations (optional) <add-attr>` in the example :ref:`Connect to the RainMaker cloud for simple communications in normal transmission transmode <simple-communi>`.

5. Add a device.

   Please refer to :ref:`Add a device <add-device>` in the example :ref:`Connect to the RainMaker cloud for simple communications in normal transmission transmode <simple-communi>`.

6. Add parameters to the device.

   Please refer to :ref:`Add parameters to the device <add-params>` in the example :ref:`Connect to the RainMaker cloud for simple communications in normal transmission transmode <simple-communi>`.

7. Connect to ESP RainMaker cloud.

   Command:

   .. code-block:: none

     AT+RMCONN

   Response:

   .. code-block:: none

     OK

   If the device connects to the cloud successfully, the system returns:

   .. code-block:: none

     +RMCONNECTED

8. Start user-node mapping.

   **Espressif** provides `REST APIs <https://swaggerapis.rainmaker.espressif.com/>`__ for RainMaker backend service.

   Firstly, you can enter ``user_name`` and ``password`` through API `/{version}/login <https://swaggerapis.rainmaker.espressif.com/#/User/login>`__ to get ``accesstoken``. Then, you can click the ``Authorize`` button to enter the obtained ``accesstoken``.

   .. figure:: ../../../_static/RainMaker/rest_api_login.png
      :align: center
      :scale: 50 %
      :alt: REST API Login
      :figclass: align-center

      REST API Login

   .. figure:: ../../../_static/RainMaker/rest_api_login_accesstoken.png
      :align: center
      :scale: 50 %
      :alt: REST API Login Accesstoken
      :figclass: align-center

      REST API Login Accesstoken

   .. figure:: ../../../_static/RainMaker/rest_api_login_authorize.png
      :align: center
      :scale: 50 %
      :alt: REST API Login Authorize
      :figclass: align-center

      REST API Login Authorize

   Secondly, use API `/{version}/user <https://swaggerapis.rainmaker.espressif.com/#/User/getUser>`__ to get ``user_id``.

   .. figure:: ../../../_static/RainMaker/rest_api_get_user_details.png
      :align: center
      :scale: 50 %
      :alt: REST API Get User Details
      :figclass: align-center

      REST API Get User Details

   Thirdly, enter ``node_id`` and ``secret_key`` through API `/{version}/user/nodes/mapping <https://swaggerapis.rainmaker.espressif.com/#/User%20Node%20Association/addRemoveUserNodeMapping>`__ to get ``request_id``. As shown in the figure below, {IDF_TARGET_NAME} adds "84F70308E0E8" as ``node_id`` and "test" as ``secret_key``.

   .. figure:: ../../../_static/RainMaker/rest_api_get_request_id.png
      :align: center
      :scale: 50 %
      :alt: REST API Get request id
      :figclass: align-center

      REST API Get request id

   Finally, use :ref:`AT+RMUSERMAPPING <cmd-RMUSERMAPPING>` to start user-node mapping.

   Command:

   .. code-block:: none

     AT+RMUSERMAPPING="GitHub_xxxxxxxxxxxxxxxxxxxxxx","test"

   Response:

   .. code-block:: none

     OK

   If the mapping completes, the system returns:

   .. code-block:: none

     +RMMAPPINGDONE

   Then, refresh the app and you should see the newly added device.

   Note:

   This command does not guarantee success of the actual mapping. The mapping status needs to be checked separately by the clients (Phone app/CLI).

9. Clear user-node mapping. (optional)

   Command:

   .. code-block:: none

     AT+RMUSERUNMAPPING

   Response:

   .. code-block:: none

     OK
