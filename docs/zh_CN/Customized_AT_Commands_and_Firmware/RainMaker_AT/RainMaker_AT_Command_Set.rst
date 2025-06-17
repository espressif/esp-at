.. _RainMaker-AT:

RainMaker AT 命令集
================================

:link_to_translation:`en:[English]`

.. important::
  默认的 AT 固件不支持此页面下的 AT 命令。如果您需要 {IDF_TARGET_NAME} 支持 RainMaker 命令，请任选下面一种方式：

  - 参考 :doc:`如何从 GitHub 下载最新临时版本 AT 固件 <../../Compile_and_Develop/How_to_download_the_latest_temporary_version_of_AT_from_github>` 文档，下载 esp32c3-rainmaker-at 固件
  - 自行 :doc:`编译 ESP-AT 工程 <../../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第三步安装环境里 ``Platform name`` 选择 ``PLATFORM_ESP32C3``，``Module name`` 选择 ESP32C3_RAINMAKER。

-  :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>`：初始化节点
-  :ref:`AT+RMNODEATTR <cmd-RMNODEATTR>`：属性信息操作
-  :ref:`AT+RMDEV <cmd-RMDEV>`：设备操作
-  :ref:`AT+RMPARAM <cmd-RMPARAM>`：设备参数操作
-  :ref:`AT+RMPARAMBOUNDS <cmd-RMPARAMBOUNDS>`：向数字参数添加范围
-  :ref:`AT+RMPARAMSTRLIST <cmd-RMPARAMSTRLIST>`：向字符参数添加字符串列表
-  :ref:`AT+RMPARAMCOUNT <cmd-RMPARAMCOUNT>`：向数组参数添加最大元素数
-  :ref:`AT+RMUSERMAPPING <cmd-RMUSERMAPPING>`：开启用户和节点之间的映射
-  :ref:`AT+RMUSERUNMAPPING <cmd-RMUSERUNMAPPING>`：清除用户和节点之间的映射
-  :ref:`AT+RMPROV <cmd-RMPROV>`：配网并完成用户和节点之间的映射
-  :ref:`AT+RMCONN <cmd-RMCONN>`：连接到 ESP RainMaker 云
-  :ref:`AT+RMCLOSE <cmd-RMCLOSED>`：主动断开与 ESP RainMaker 云的连接
-  :ref:`AT+RMPARAMUPDATE <cmd-RMPARAMUPDATE>`：参数更新
-  :ref:`AT+RMMODE <cmd-RMMODE>`：设置传输模式
-  :ref:`AT+RMSEND <cmd-RMSEND>`：在 :term:`RainMaker 普通传输模式` 或 :term:`RainMaker 透传模式` 下发送数据
-  :ref:`AT+RMOTARESULT <cmd-RMOTARESULT>`：发送 OTA 结果
-  :ref:`AT+RMOTAFETCH <cmd-RMOTAFETCH>`：获取 OTA 信息

.. _cmd-RMNODEINIT:

:ref:`AT+RMNODEINIT <RainMaker-AT>`: 初始化节点
------------------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    AT+RMNODEINIT

**响应：**

::

    OK

该命令执行成功之后，节点配置会以如下所示 JSON 格式保存在内部。

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

说明
^^^^

-  在执行其它 ESP RainMaker AT 命令之前应该先执行该命令。
-  该命令默认开启了系统管理服务、OTA 服务、时区服务、定时和倒计时服务。
-  该命令首先会获取存储在量产分区 ``rmaker_mfg`` 中的认证信息。如果没有获取到，则会在系统 NVS 分区中获取认证信息。如果都没有获取到，则设备会执行 claiming。
-  该命令会加载存储在量产分区 ``rmaker_mfg`` 中的参数。如果参数不存在，则默认配置信息将用于自动创建节点。
-  节点配置中有一些默认的键值对。

   -  node_id：源自证书，唯一标识符，不可更改。
   -  config_version：暂时无实际用途，无需更改。
   -  name：固定为 "ESP RainMaker AT Node"。
   -  fw_version：RainMaker AT 版本信息。
   -  type：固定为 "AT Node"。
   -  model：固定为 "esp-at"。
   -  project_name：固定为 "esp-at"。
   -  platform：固定为 "{IDF_TARGET_NAME}"。
   -  services：系统管理服务、OTA 服务、时区服务、定时和倒计时服务。

.. _cmd-RMNODEATTR:

:ref:`AT+RMNODEATTR <RainMaker-AT>`：属性信息操作
-------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+RMNODEATTR=<"name1">,<"value1">[<"name2">,<"value2">,<"name3">,<"value3">,...,<"name8">,<"value8">]

**响应：**

::

    OK

参数
^^^^

-  **<"name">**：节点属性键名。
-  **<"value">**：节点属性值。

说明
^^^^

-  该命令应该在设备连接上 RainMaker 云之前执行（请参考 :ref:`AT+RMPROV <cmd-RMPROV>` 或者 :ref:`AT+RMCONN <cmd-RMCONN>`）。

示例
^^^^

::

    AT+RMNODEATTR="serial_num","123abc"

.. _cmd-RMDEV:

:ref:`AT+RMDEV <RainMaker-AT>`：设备操作
--------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+RMDEV=<dev_opt>,<"unique_name">,<"device_name">,<"device_type">

**响应：**

::

    OK

参数
^^^^

-  **<"dev_opt">**：设备操作。

   -  0：添加一个设备。
   -  1：删除一个设备

-  **<"unique_name">**：设备唯一标识名。
-  **<"device_name">**：设备名称，将作为应用上显示的默认设备名称。
-  **<"device_type">**：设备类型。请参考 `Devices <https://rainmaker.espressif.com/docs/standard-types.html#devices>`__。

说明
^^^^

-  该命令应该在设备连接上 RainMaker 云之前执行（请参考 :ref:`AT+RMPROV <cmd-RMPROV>` 或者 :ref:`AT+RMCONN <cmd-RMCONN>`）。
-  目前一个节点只能添加一个设备。
-  该命令执行成功后，设备被添加到节点中。默认在 params 中类型的值为 "esp.param.name"，数据类型的值为 "string"，权限为 "read" 和 "write"。

示例
^^^^

::

    AT+RMDEV=0,"Light","Light","esp.device.light"

该命令执行成功之后，设备 "Light" 会被添加到节点配置中，并在内部以如下所示 JSON 格式保存（节点配置请参考 :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>`）。

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

:ref:`AT+RMPARAM <RainMaker-AT>`：设备参数操作
------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

向设备添加参数

**命令：**

::

    AT+RMPARAM=<"unique_name">,<"param_name">,<"param_type">,<data_type>,<properties>,<"ui_type">,<"def">

**响应：**

::

    OK

参数
^^^^

-  **<"unique_name">**：设备唯一标识名。
-  **<"param_name">**：参数名称。
-  **<"param_type">**：参数类型。请参考 `Parameters <https://rainmaker.espressif.com/docs/standard-types.html#parameters>`__。
-  **<data_type>**：数据类型。

   -  bit 0：boolean。
   -  bit 1：integer。
   -  bit 2：floating-point number。
   -  bit 3：string。
   -  bit 4：object。
   -  bit 5：array。

-  **<properties>**：数据权限。

   -  bit 0：read。
   -  bit 1：write。
   -  bit 2：time_series。
   -  bit 3：persist。

-  **<"ui_type">**：UI 类型。请参考 `UI Elements <https://rainmaker.espressif.com/docs/standard-types.html#ui-elements>`__。
-  **<"def">**：默认值。

说明
^^^^

-  该命令应该在设备连接上 RainMaker 云之前执行（请参考 :ref:`AT+RMPROV <cmd-RMPROV>` 或者 :ref:`AT+RMCONN <cmd-RMCONN>`）。
-  请确保参数 ``<def>`` 匹配参数 ``<data_type>``。AT 不会做内部检查。
-  在 :term:`RainMaker 透传模式` 中，只允许存在一个参数（不包含命令 :ref:`AT+RMDEV <cmd-RMDEV>` 添加的节点默认参数）。如果在设备下存在多个参数，则无法进入 :term:`RainMaker 透传模式`。

示例
^^^^

::

    AT+RMPARAM="Light","Brightness","esp.param.brightness",2,3,"esp.ui.slider","50"

.. _cmd-RMPARAMBOUNDS:

:ref:`AT+RMPARAMBOUNDS <RainMaker-AT>`：向数字参数添加范围
----------------------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+RMPARAMBOUNDS=<"unique_name">,<"param_name">,<"min">,<"max">,<"step">

**响应：**

::

    OK

参数
^^^^

-  **<"unique_name">**：设备唯一标识名。
-  **<"param_name">**：参数名称。
-  **<"min">**：最小值。
-  **<"max">**：最大值。
-  **<"step">**：步进值。

说明
^^^^

-  该命令应该在设备连接上 RainMaker 云之前执行（请参考 :ref:`AT+RMPROV <cmd-RMPROV>` 或者 :ref:`AT+RMCONN <cmd-RMCONN>`）。
-  该命令仅针对 ``<data_type>`` （请参考 :ref:`AT+RMPARAM <RainMaker-AT>` 中的 ``<data_type>`` 参数）为 integer 或者 floating-point number 的参数。请确保参数 ``<"min">``、``<"max">`` 和 ``<"step">`` 匹配 ``<data_type>``，AT 不会做内部检查。

示例
^^^^

::

    AT+RMPARAMBOUNDS="Switch","brightness","0","100","1"

该命令执行成功之后，"bounds" 会被加入设备 "Switch" 中，并在内部以如下所示 JSON 格式保存（节点配置请参考 :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>`）。

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

:ref:`AT+RMPARAMSTRLIST <RainMaker-AT>`：向字符参数添加字符串列表
------------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+RMPARAMSTRLIST=<"unique_name">,<"param_name">,<"str1">[,<"str2">,<"str3">,...,<"str14">]

**响应：**

::

    OK

参数
^^^^

-  **<"unique_name">**：设备唯一标识名。
-  **<"param_name">**：参数名称。
-  **<"str">**：字符串类表中的字符串。

说明
^^^^

-  该命令应该在设备连接上 RainMaker 云之前执行（请参考 :ref:`AT+RMPROV <cmd-RMPROV>` 或者 :ref:`AT+RMCONN <cmd-RMCONN>`）。
-  该命令仅针对 ``<data_type>`` （请参考 :ref:`AT+RMPARAM <RainMaker-AT>` 中的 ``<data_type>`` 参数）为 string 的参数。请确保参数 ``<"str">`` 匹配 ``<data_type>``，AT 不会做内部检查。

示例
^^^^

::

    AT+RMPARAM="Light","Color","esp.param.color",4,3,"esp.ui.dropdown","white"

    AT+RMPARAMSTRLIST="Light","Color","white","red","blue","yellow"

该命令执行成功之后，"valid_strs" 会被加入设备 "Light" 中，并在内部以如下所示 JSON 格式保存（节点配置请参考 :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>`）。

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

:ref:`AT+RMPARAMCOUNT <RainMaker-AT>`：向数组参数添加最大元素数
--------------------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+RMPARAMCOUNT=<"unique_name">,<"param_name">,<array_count>

**响应：**

::

    OK

参数
^^^^

-  **<"unique_name">**：设备唯一标识名。
-  **<"param_name">**：参数名称。
-  **<array_count>**：数组中最大元素数。

说明
^^^^

-  该命令应该在设备连接上 RainMaker 云之前执行（请参考 :ref:`AT+RMPROV <cmd-RMPROV>` 或者 :ref:`AT+RMCONN <cmd-RMCONN>`）。
-  该命令仅针对 ``<data_type>`` （请参考 :ref:`AT+RMPARAM <RainMaker-AT>` 中的 ``<data_type>`` 参数）为 array 的参数。请确保参数 ``<array_count>`` 匹配 ``<data_type>``，AT 不会做内部检查。

示例
^^^^

::

    AT+RMPARAM="Light","Color","esp.param.color",6,3,"esp.ui.hidden",""

    AT+RMPARAMCOUNT="Light","Color",5

该命令执行成功之后，"bounds" 会被加入设备 "Light" 中，并在内部以如下所示 JSON 格式保存（节点配置请参考 :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>`）。

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

:ref:`AT+RMUSERMAPPING <RainMaker-AT>`：开启用户和节点之间的映射
-----------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+RMUSERMAPPING=<"user_id">,<"secret_key">

**响应：**

::

    OK

如果用户和节点之间的映射完成，AT 返回：

::

  +RMMAPPINGDONE

参数
^^^^

-  **<"user_id">**：用户标识符。
-  **<"secret_key">**：密钥。

说明
^^^^

-  请确认在执行该命令之前设备已经连接到 ESP RainMaker 云，请参考 :ref:`AT+RMCONN <cmd-RMCONN>`。
-  该命令不保证映射成功。映射结果需要由客户端单独检查 (Phone app/CLI)。

.. _cmd-RMUSERUNMAPPING:

:ref:`AT+RMUSERUNMAPPING <RainMaker-AT>`：清除用户和节点之间的映射
-----------------------------------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    AT+RMUSERUNMAPPING

**响应：**

::

    OK

.. _cmd-RMPROV:

:ref:`AT+RMPROV <RainMaker-AT>`：配网并完成用户和节点之间的映射
-----------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+RMPROV=<mode>[,<customer_id>,<device_extra_code>,<"broadcast_name">]

**响应：**

::

    OK

参数
^^^^

-  **<mode>**：模式。

   -  0：开始配网，并在配网后开启用户和节点之间的映射。
   -  1：停止配网。

-  **<customer_id>**：客户标识符，用于区分不同的客户。范围：[0,65535]。如果你想使用 `Nova Home <https://rainmaker.espressif.com>`__，请 `联系我们 <https://www.espressif.com/zh-hans/contact-us/sales-questions>`__。
-  **<device_extra_code>**：设备编码，用于 app 配网时标识设备图标。范围：[0,255]。
-  **<"broadcast_name">**：自定义蓝牙广播时设备的名称。范围：[0,12]。单位：字节。

.. _cmd-RMCONN:

:ref:`AT+RMCONN <RainMaker-AT>`：连接到 ESP RainMaker 云
-----------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+RMCONN=<conn_timeout>

**响应：**

如果设备成功连接到 ESP RainMaker 云，AT 返回：

::

    +RMCONNECTED
    OK

如果设备连接 ESP RainMaker 云失败，AT 返回：

::

    ERROR

执行命令
^^^^^^^^

**命令：**

::

    AT+RMCONN

**响应：**

如果设备成功连接到 ESP RainMaker 云，AT 返回：

::

    +RMCONNECTED
    OK

如果设备连接 ESP RainMaker 云失败，AT 返回：

::

    ERROR

参数
^^^^

-  **<conn_timeout>**：连接最大超时时间。范围：[3,600]。单位：秒。默认值：15。

.. _cmd-RMCLOSED:

:ref:`AT+RMCLOSE <RainMaker-AT>`：主动断开与 ESP RainMaker 云的连接
-------------------------------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    AT+RMCLOSE

**响应：**

::

    OK

说明
^^^^

-  当设备主动调用该命令断开与云的连接时，不会主动报 `+RMDISCONNECTED` 的消息，只有设备被动的与云断开连接时，AT 才会报 `+RMDISCONNECTED` 的消息。

.. _cmd-RMPARAMUPDATE:

:ref:`AT+RMPARAMUPDATE <RainMaker-AT>`：参数更新
---------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+RMPARAMUPDATE=<"unique_name">,<"param_name1">,<"param_value1">[,<"param_name2">,<"param_value2">,...,<"param_name7">,<"param_value7">]

**响应：**

::

    OK

参数
^^^^

-  **<"unique_name">**：设备唯一标识名。
-  **<"param_name">**：参数名。
-  **<"param_value">**：参数值。

说明
^^^^

-  参数 ``<"param_value">`` 必须匹配命令 :ref:`AT+RMPARAM <RainMaker-AT>` 中参数 ``<data_type>`` 设置的类型。
-  该命令最多支持 15 个参数，即 1 个 ``<"unique_name">`` + 7 个 ``<"param_name">`` + 7 个 ``<"param_value">``。
-  整条 AT 命令的长度应小于 ``256`` 字节。如果你想更新的数据量较大，请使用 :ref:`AT+RMSEND <cmd-RMSEND>` 命令。

示例
^^^^

::

    AT+RMPARAMUPDATE="Light","Power","1"

.. _cmd-RMMODE:

:ref:`AT+RMMODE <RainMaker-AT>`：设置传输模式
-------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+RMMODE=<mode>

**响应：**

::

    OK

参数
^^^^

-  **<mode>**：传输模式。

   -  0：:term:`RainMaker 普通传输模式`。
   -  1：:term:`RainMaker 透传模式`。

说明
^^^^

-  在 :term:`RainMaker 透传模式` 中，只允许存在一个参数（不包含命令 :ref:`AT+RMDEV <cmd-RMDEV>` 添加的节点默认参数）。如果在设备下存在多个参数，则无法进入 :term:`RainMaker 透传模式`。

.. _cmd-RMSEND:

:ref:`AT+RMSEND <RainMaker-AT>`：在 :term:`RainMaker 普通传输模式` 或 :term:`RainMaker 透传模式` 下发送数据
----------------------------------------------------------------------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

在 :term:`RainMaker 普通传输模式` 中传输指定长度的数据。

**命令：**

::

    AT+RMSEND=<"unique_name">,<"param_name">,<len>

**响应：**

::

    OK

    >

上述响应表示 AT 已经准备好接收串行数据，此时你可以输入数据，当 AT 接收到的数据长度达到 `<len>` 后，返回：

::

    Recv <len> bytes

如果所有数据没有被完全发出去，系统最终返回：

::

    SEND FAIL

如果所有数据被成功发送，系统最终返回：

::

    SEND OK

执行命令
^^^^^^^^

**功能：**

进入 :term:`RainMaker 透传模式`。

**命令：**

::

    AT+RMSEND

**响应：**

::

    OK

    >

或

::

    ERROR

进入 :term:`RainMaker 透传模式`。当输入单独一包 ``+++`` 时，{IDF_TARGET_NAME} 将会退出 :term:`RainMaker 透传模式` 下的数据发送模式。请至少间隔 1 秒在发下一条 AT 命令。

参数
^^^^

-  **<"unique_name">**：设备唯一标识名。
-  **<"param_name">**：参数名。
-   **<len>**：数据长度。长度值取决于 RAM 大小。你可以使用 :ref:`AT+SYSRAM <cmd-SYSRAM>` 命令来查询剩余可用 RAM 大小。

说明
^^^^

-  在 :term:`RainMaker 透传模式` 中，只允许存在一个参数（不包含命令 :ref:`AT+RMDEV <cmd-RMDEV>` 添加的节点默认参数）。如果在设备下存在多个参数，则无法进入 :term:`RainMaker 透传模式`。

-  如果你想同时更新多个参数，请参考 :ref:`AT+RMPARAMUPDATE <cmd-RMPARAMUPDATE>` 命令。

.. _cmd-RMOTARESULT:

:ref:`AT+RMOTARESULT <RainMaker-AT>`：上报 OTA 结果
----------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+RMOTARESULT=<type>,<"ota_job_id">,<result>,<"additional_info">

**响应：**

::

    OK

参数
^^^^

-  **<type>**：保留。
-  **<"ota_job_id">**：OTA job ID.
-  **<result>**：OTA 结果。

   -  1：OTA 进行中。
   -  2：OTA 成功。
   -  3：OTA 失败。
   -  4：OTA 被应用程序延迟。
   -  5：OTA 由于某种原因被拒绝。

-  **<"additional_info">**：OTA 状态的附加信息。

说明
^^^^

-  此命令只适用于主控 MCU OTA。对于 {IDF_TARGET_NAME} Wi-Fi OTA，系统会自动上报 OTA 状态。

.. _cmd-RMOTAFETCH:

:ref:`AT+RMOTAFETCH <RainMaker-AT>`：获取 OTA 信息
----------------------------------------------------------

执行命令
^^^^^^^^

**命令：**

::

    AT+RMOTAFETCH

**响应：**

::

    OK

说明
^^^^

-  对于主控 MCU OTA，ESP-AT 会立即将接收到的 OTA 信息发送到主控 MCU，格式为 ``+RMFWNOTIFY:<type>,<size>,<url>,<fw_version>,<ota_job_id>``。

    -  **<type>**：保留。ESP-AT 总是设置为 0。
    -  **<size>**：主控 MCU OTA 固件大小。单位：字节。
    -  **<url>**：主控 MCU OTA 固件下载 URI。你可以执行 :ref:`AT+HTTPCGET <cmd-HTTPCGET>` 命令来下载固件。
    -  **<fw_version>**：主控 MCU OTA 固件版本。
    -  **<ota_job_id>**：主控 MCU OTA job ID. 你可以执行 :ref:`AT+RMOTARESULT <cmd-RMOTARESULT>` 命令上报 OTA 结果。

-  对于 {IDF_TARGET_NAME} Wi-Fi OTA，系统会自动执行 OTA。ESP-AT 会将 OTA 状态发送到主控 MCU，格式为 ``+RMOTA:<status>``。

    -  1： OTA 进行中。
    -  2： OTA 成功。
    -  3： OTA 失败。
    -  4： OTA 被应用程序延迟。
    -  5： OTA 由于某种原因被拒绝。

- 请参考 :doc:`RainMaker_AT_OTA_Guide` 了解如何通过 ESP RainMaker 云实现 OTA。
