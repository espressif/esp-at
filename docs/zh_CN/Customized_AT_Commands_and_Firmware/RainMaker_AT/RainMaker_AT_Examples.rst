RainMaker AT 示例
================================

:link_to_translation:`en:[English]`

本文档主要介绍 :doc:`./RainMaker_AT_Command_Set` 的使用方法，并提供在 {IDF_TARGET_NAME} 设备上运行这些命令的详细示例。

.. contents::
   :local:
   :depth: 1

简介
------------

**乐鑫** 提供了一个官方应用 `Nova Home <https://rainmaker.espressif.com/zh-hans/index.html>`__ 来和设备进行交互。您可以通过 `Google Play (Android) <https://play.google.com/store/apps/details?id=com.espressif.novahome>`__ 或者 `App Store (iOS) <https://apps.apple.com/cn/app/nova-home/id1563728960>`__ 下载。在文档的示例中，默认使用 Nova Home（版本：1.6.2）作为客户端应用。

.. _simple-communi:

在普通传输模式下连接到 RainMaker 云进行简单的通信
-----------------------------------------------------------------------------

.. _init-node:

1. 初始化节点。

   {IDF_TARGET_NAME} 初始化节点：

   命令：

   .. code-block:: none

     AT+RMNODEINIT

   响应：

   .. code-block:: none

     OK

   说明：

   该命令执行成功之后，节点配置将在内部保存。初始化的节点配置如下。

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

2. 属性信息操作。（可选）

   例如，{IDF_TARGET_NAME} 添加名字为 "serial_num"，值为 "123abc" 的属性到节点中。

   命令：

   .. code-block:: none

     AT+RMNODEATTR="serial_num","123abc"

   响应：

   .. code-block:: none

     OK

   说明：

   该命令执行成功后，新的属性被添加到节点配置中。

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

3. 添加一个设备。

   例如，{IDF_TARGET_NAME} 添加一个设备，设备唯一标识名为 "Light"，设备名称为 "Light"，设备类型为 "esp.device.light"。

   命令：

   .. code-block:: none

     AT+RMDEV=0,"Light","Light","esp.device.light"

   响应：

   .. code-block:: none

     OK

   说明：

   该命令执行成功后，设备被添加到节点配置中。

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

4. 向设备中添加参数。

   例如，{IDF_TARGET_NAME} 添加 "Power" 和 "Brightness" 到 "Light" 设备中。

   参数名为 "Power"，参数类型为 "esp.param.power"，数据类型为 bool，权限为读和写，UI 类型为 "esp.ui.toggle"，默认值为 false ("0")。

   命令：

   .. code-block:: none

     AT+RMPARAM="Light","Power","esp.param.power",1,3,"esp.ui.toggle","0"

   响应：

   .. code-block:: none

     OK

   参数名为 "Brightness"，参数类型为 "esp.param.brightness"，数据类型为 int，权限为读和写，UI 类型为 "esp.ui.slider"，默认值为 "50"。

   命令：

   .. code-block:: none

     AT+RMPARAM="Light","Brightness","esp.param.brightness",2,3,"esp.ui.slider","50"

   响应：

   .. code-block:: none

     OK

   说明：

   该命令执行成功后，"Power" 参数和 "Brightness" 参数被加入到设备中。

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

5. 执行配网并完成用户和节点之间的映射。

   使用 Nova Home app 作为客户端。

   命令：

   .. code-block:: none

     AT+RMPROV=0

   响应：

   .. code-block:: none

     WIFI DISCONNECT

     OK

   如果 {IDF_TARGET_NAME} 之前已经连接一个 AP，则设备首先会断开连接。之后在 app 右上角，您可以点击 "+" 按钮 > ``Add Device``，然后 app 通过 Bluetooth LE 扫描并显示发现的设备。

   .. figure:: ../../../_static/RainMaker/nova_add_device.png
      :align: center
      :scale: 30 %
      :alt: Nova Home 添加设备
      :figclass: align-center

      Nova Home 添加设备

   .. figure:: ../../../_static/RainMaker/nova_found_devices.png
      :align: center
      :scale: 30 %
      :alt: Nova Home 发现设备
      :figclass: align-center

      Nova Home 发现设备

   点击设备进行 Bluetooth LE 配网，在 ``Connecting Device`` 屏幕中输入 SSID 和密码。之后配网开始。

   .. figure:: ../../../_static/RainMaker/nova_provisioning_starts.png
      :align: center
      :scale: 30 %
      :alt: Nova Home 开始配网
      :figclass: align-center

      Nova Home 开始配网

   在配网的过程中和配网成功后，系统返回：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     +RMCONNECTED
     +RMMAPPINGDONE

   之后您可以在 app 上设置设备的名字和房间。

   .. figure:: ../../../_static/RainMaker/nova_device.png
      :align: center
      :scale: 30 %
      :alt: Nova Home 设备
      :figclass: align-center

      Nova Home 设备

6. 更新参数。

   在 app 上显示灯的状态为关闭状态。通过修改 "Power" 参数将值改为 true ("1")。

   命令：

   .. code-block:: none

     AT+RMPARAMUPDATE="Light","Power","1"

   响应：

   .. code-block:: none

     OK

   之后您可以在 app 上看到灯的状态从关闭状态切换到打开状态。

   .. figure:: ../../../_static/RainMaker/nova_change_light_status.png
      :align: center
      :scale: 30 %
      :alt: Nova Home 修改灯的状态
      :figclass: align-center

      Nova Home 修改灯的状态

7. 远程控制设备。

   您可以在 app 上控制灯的状态。例如，您可以将灯的状态由打开状态切换到关闭状态。当 {IDF_TARGET_NAME} 接收到控制消息时，系统返回：

   .. code-block:: none

     +RMRECV:Local,Light,Power:0

.. _close-agent:

8. 主动断开与 ESP RainMaker 云的连接。

   命令：

   .. code-block:: none

     AT+RMCLOSE

   响应：

   .. code-block:: none

     OK

在透传模式下连接到 RainMaker 云进行简单的通信
-----------------------------------------------------------------------------

1. 初始化节点。

   请参考 :ref:`在普通传输模式下连接到 RainMaker 云进行简单的通信 <simple-communi>` 示例中的 :ref:`初始化节点 <init-node>`。

2. 属性信息操作。（可选）

   请参考 :ref:`在普通传输模式下连接到 RainMaker 云进行简单的通信 <simple-communi>` 示例中的 :ref:`属性信息操作 <add-attr>`。

3. 添加一个设备。

   请参考 :ref:`在普通传输模式下连接到 RainMaker 云进行简单的通信 <simple-communi>` 示例中的 :ref:`添加一个设备 <add-device>`。

4. 向设备中添加参数。

   在 :term:`RainMaker 透传模式` 中，只允许存在一个参数 （不包含命令 :ref:`AT+RMDEV <cmd-RMDEV>` 添加的节点默认参数）。如果在设备下存在多个参数，则无法进入 :term:`RainMaker 透传模式`。

   例如，{IDF_TARGET_NAME} 添加 "Power" 参数到 "Light" 设备中。参数名为 "Power"，参数类型为 "esp.param.power"，数据类型为 bool，权限为读和写，UI 类型为 "esp.ui.toggle"，默认值为 false ("0")。

   命令：

   .. code-block:: none

     AT+RMPARAM="Light","Power","esp.param.power",1,3,"esp.ui.toggle","0"

   响应：

   .. code-block:: none

     OK

   说明：

   该命令执行成功后，"Power" 参数被添加到设备中。

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

5. 执行配网并完成用户和节点之间的映射。

   请参考 :ref:`在普通传输模式下连接到 RainMaker 云进行简单的通信 <simple-communi>` 示例中的 :ref:`执行配网并完成用户和节点之间的映射 <prov-and-mapping>`。

6. 进入透传模式。

   命令：

   .. code-block:: none

     AT+RMMODE=1

   响应：

   .. code-block:: none

     OK

7. 发送数据

   命令：

   .. code-block:: none

     AT+RMSEND

   响应：

   .. code-block:: none

     OK

     >

   之后您可以直接输入数据，输入 ``1`` 可以将灯切换到开灯状态，输入 ``0`` 可以将灯切换到关灯状态。

8. 停止发送数据。

   当 AT 识别到单独的一包数据 ``+++`` 时，系统会退出 :term:`RainMaker 透传模式`。此时请至少等待一秒，再发下一条 AT 命令。请注意，如果直接用键盘输入 ``+++``，有可能因为时间太慢而不能被识别为连续的三个 ``+``。

   .. Important::

     使用 :ref:`+++ <cmd-PLUS>` 可退出 :term:`RainMaker 透传模式`，回到 :term:`RainMaker 普通传输模式`。您可以再次使用 ``AT+RMSEND`` 命令进入到 :term:`RainMaker 透传模式`。

9. 主动断开与 ESP RainMaker 云的连接。

   请参考 :ref:`在普通传输模式下连接到 RainMaker 云进行简单的通信 <simple-communi>` 示例中的 :ref:`主动断开与 ESP RainMaker 云的连接 <close-agent>`。

执行用户和节点之间的映射
----------------------------

1. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

2. 连接到路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

3. 初始化节点。

   请参考 :ref:`在普通传输模式下连接到 RainMaker 云进行简单的通信 <simple-communi>` 示例中的 :ref:`初始化节点 <init-node>`。

4. 属性信息操作。（可选）

   请参考 :ref:`在普通传输模式下连接到 RainMaker 云进行简单的通信 <simple-communi>` 示例中的 :ref:`属性信息操作 <add-attr>`。

5. 添加一个设备。

   请参考 :ref:`在普通传输模式下连接到 RainMaker 云进行简单的通信 <simple-communi>` 示例中的 :ref:`添加一个设备 <add-device>`。

6. 向设备中添加参数。

   请参考 :ref:`在普通传输模式下连接到 RainMaker 云进行简单的通信 <simple-communi>` 示例中的 :ref:`向设备中添加参数 <add-params>`。

7. 连接到 ESP RainMaker 云。

   命令：

   .. code-block:: none

     AT+RMCONN

   响应：

   .. code-block:: none

     OK

   如果设备成功连接到云，系统返回：

   .. code-block:: none

     +RMCONNECTED

8. 开启用户和节点之间的映射。

   **乐鑫** 提供了 `REST APIs <https://swaggerapis.rainmaker.espressif.com/>`__ 操作 RainMaker 后台服务。

   首先您可以通过 API `/{version}/login <https://swaggerapis.rainmaker.espressif.com/#/User/login>`__ 输入 ``user_name`` 和 ``password`` 来获取 ``accesstoken``。之后可以点击 ``Authorize`` 按钮来输入刚获取到的 ``accesstoken``。

   .. figure:: ../../../_static/RainMaker/rest_api_login.png
      :align: center
      :scale: 50 %
      :alt: REST API 登陆
      :figclass: align-center

      REST API 登陆

   .. figure:: ../../../_static/RainMaker/rest_api_login_accesstoken.png
      :align: center
      :scale: 50 %
      :alt: REST API 访问令牌
      :figclass: align-center

      REST API 访问令牌

   .. figure:: ../../../_static/RainMaker/rest_api_login_authorize.png
      :align: center
      :scale: 50 %
      :alt: REST API 登陆授权
      :figclass: align-center

      REST API 登陆授权

   接着使用 API `/{version}/user <https://swaggerapis.rainmaker.espressif.com/#/User/getUser>`__ 获取 ``user_id``。

   .. figure:: ../../../_static/RainMaker/rest_api_get_user_details.png
      :align: center
      :scale: 50 %
      :alt: REST API 获取用户细节
      :figclass: align-center

      REST API 获取用户细节

   接着通过 API `/{version}/user/nodes/mapping <https://swaggerapis.rainmaker.espressif.com/#/User%20Node%20Association/addRemoveUserNodeMapping>`__ 来获取 ``request_id``。如下图所示，{IDF_TARGET_NAME} 添加 ``node_id`` 为 "84F70308E0E8"，``secret_key`` 为 "test"。

   .. figure:: ../../../_static/RainMaker/rest_api_get_request_id.png
      :align: center
      :scale: 50 %
      :alt: REST API 获取 Request id
      :figclass: align-center

      REST API 获取 Request id

   最后，使用 :ref:`AT+RMUSERMAPPING <cmd-RMUSERMAPPING>` 开启用户和节点之间的映射。

   命令：

   .. code-block:: none

     AT+RMUSERMAPPING="GitHub_xxxxxxxxxxxxxxxxxxxxxx","test"

   响应：

   .. code-block:: none

     OK

   如果映射完成，系统返回：

   .. code-block:: none

     +RMMAPPINGDONE

   之后，您可以在 app 上刷新就可以看到刚添加的设备。

   说明：

   该命令不保证映射成功。映射结果需要由客户端单独检查 (Phone app/CLI)。

9. 清除用户和节点之间的映射。(可选)

   命令：

   .. code-block:: none

     AT+RMUSERUNMAPPING

   响应：

   .. code-block:: none

     OK
