MQTT AT 连接云示例
===========================

:link_to_translation:`en:[English]`

本文档主要介绍您的设备如何通过 AT 命令对接 AWS IoT。

.. Important::
    有关如何使用 MQTT AT 命令的详细信息，请参阅 :doc:`../AT_Command_Set/MQTT_AT_Commands`。
    您需要通过阅读 `AWS IoT 开发指南 <https://docs.aws.amazon.com/zh_cn/iot/latest/developerguide/iot-gs.html>`_ 来熟悉 AWS IoT。

请按照以下步骤使用 ESP-AT 将您的 {IDF_TARGET_NAME} 设备连接到 AWS IoT。

.. contents::
   :local:
   :depth: 1

从 AWS IoT 获取证书以及 endpoint
-----------------------------------------

1. 登录您的 AWS IoT 控制台帐号，以及切换至 IoT Core services。

2. 按照 `创建 AWS IoT 资源 <https://docs.aws.amazon.com/zh_cn/iot/latest/developerguide/create-iot-resources.html>`_ 中的说明创建 AWS IoT 策略、事物和证书。

  确保您已获得以下证书和密钥文件：

  - device.pem.crt（设备证书）
  - private.pem.key（私有密钥）
  - Amazon-root-CA-1.pem（根 CA 证书）

3. 根据文档 `设置策略 <https://docs.aws.amazon.com/zh_cn/iot/latest/developerguide/using-laptop-as-device.html>`_ 获取端点 (endpoint) 以及了解如何通过证书将事物绑定到策略。

    端点的格式为 “xxx-ats.iot.us-east-2.amazonaws.com”。

.. note::
  强烈建议您熟悉 `AWS IoT 开发人员指南 <https://docs.aws.amazon.com/zh_cn/iot/latest/developerguide/what-is-aws-iot.html>`_ 以下是本指南中值得注意的一些要点。

  - AWS IoT 需要所有设备必须有事物证书、事物私钥、和根证书。
  - 有关如何激活证书。
  - 区域建议选择俄亥俄州 (Ohio)。

使用 MQTT AT 命令基于双向认证连接 AWS IoT
----------------------------------------------

替换证书
^^^^^^^^^^^^^^

打开本地 ESP-AT 工程，并执行如下操作：

- 使用 ``Amazon-root-CA-1.pem`` 替换 :component_file:`customized_partitions/raw_data/mqtt_ca/mqtt_ca.crt`。
- 使用 ``device.pem.crt`` 替换 :component_file:`customized_partitions/raw_data/mqtt_cert/mqtt_client.crt`。
- 使用 ``private.pem.key`` 替换 :component_file:`customized_partitions/raw_data/mqtt_key/mqtt_client.key`。

编译和烧录 AT 固件
^^^^^^^^^^^^^^^^^^^^

编译 ESP-AT 项目以构建 AT 固件，并将固件烧录到您的 {IDF_TARGET_NAME} 设备。欲了解更多信息，请参阅 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`。

.. note::
  若不想编译 ESP-AT 工程替换证书，可直接使用 AT 命令替换固件中的证书，具体请参阅 :doc:`../Compile_and_Develop/How_to_update_pki_config`。

使用 AT 命令连接到 AWS IoT
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：
  
   .. code-block:: none

     OK

#. 连接 AP。

   命令：

   .. code-block:: none

     AT+CWJAP=<ssid>,<password>

   响应：
  
   .. code-block:: none

     OK

#. 设置 SNTP Server。

   命令：

   .. code-block:: none

     AT+CIPSNTPCFG=1,8,"pool.ntp.org"

   响应：
  
   .. code-block:: none

     OK

#. 查询 SNTP 时间。

   命令：

   .. code-block:: none

     AT+CIPSNTPTIME?

   响应：
  
   .. code-block:: none

     +CIPSNTPTIME:<asctime style time>
     OK

   说明：

   - 此时获得的 <asctime style time> 必须是设置时区的实时时间，否则会因为证书有效期而导致连接失败。

#. 设置 MQTT 用户属性。

   命令：

   .. code-block:: none

     AT+MQTTUSERCFG=0,5,"esp32","espressif","1234567890",0,0,""

   响应：
  
   .. code-block:: none

     OK

   说明：

   - AT+MQTTUSERCFG 中第二参数为 5，即双向认证，不可更改。

#. 连接 AWS IoT。

   命令：

   .. code-block:: none

     AT+MQTTCONN=0,"<endpoint>",8883,1

   响应：
  
   .. code-block:: none

     +MQTTCONNECTED:0,5,<endpoint>,"8883","",1
     OK

   说明：

   - 请在 `<endpoint>` 参数中填写您的 <endpoint> 值。
   - 无法更改端口 8883。

#. 订阅消息。

   命令：

   .. code-block:: none

     AT+MQTTSUB=0,"topic/esp32at",1

   响应：
  
   .. code-block:: none

     OK

#. 发布消息。

   命令：

   .. code-block:: none

     AT+MQTTPUB=0,"topic/esp32at","hello aws!",1,0

   响应：
  
   .. code-block:: none

     +MQTTSUBRECV:0,"topic/esp32at",10,hello aws!

     OK

示例日志
^^^^^^^^^^^^^^^^^

正常交互日志如下:

#. ESP32 端日志

   .. figure:: ../../img/esp32at-log.png
       :scale: 100 %
       :align: center
       :alt: ESP32 端连接 AWS IoT 日志

#. AWS 端日志

   .. figure:: ../../img/aws-log.png
       :scale: 100 %
       :align: center
       :alt: AWS 端连接 AWS IoT 日志
