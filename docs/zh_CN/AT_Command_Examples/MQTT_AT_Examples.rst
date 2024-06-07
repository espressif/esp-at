MQTT AT 示例
==========================

:link_to_translation:`en:[English]`

本文档主要提供在 {IDF_TARGET_NAME} 设备上运行 :doc:`../AT_Command_Set/MQTT_AT_Commands` 命令的详细示例。

.. contents::
   :local:
   :depth: 1

.. Important::
  文档中所描述的例子均基于设备已连接 Wi-Fi 的前提。

基于 TCP 的 MQTT 连接（需要本地创建 MQTT 代理）（适用于数据量少）
------------------------------------------------------------------------

以下示例同时使用两块 {IDF_TARGET_NAME} 开发板，其中一块作为 MQTT 发布者（只作为 MQTT 发布者角色），另一块作为 MQTT 订阅者（只作为 MQTT 订阅者角色）。

示例介绍了如何基于 TCP 创建 MQTT 连接。首先您需要创建一个本地 MQTT 代理，假设 MQTT 代理的 IP 地址为 ``192.168.3.102``，端口为 ``8883``。

.. Important::
  步骤中以 ``{IDF_TARGET_NAME} MQTT 发布者`` 开头的操作只需要在 {IDF_TARGET_NAME} MQTT 发布者端执行即可，以 ``{IDF_TARGET_NAME} MQTT 订阅者`` 开头的操作只需要在 {IDF_TARGET_NAME} MQTT 订阅者端执行即可。如果操作没有特别指明在哪端操作，则需要在发布者端和订阅者端都执行。

#. 设置 MQTT 用户属性。

   {IDF_TARGET_NAME} MQTT 发布者：

   命令：

   .. code-block:: none

     AT+MQTTUSERCFG=0,1,"publisher","espressif","123456789",0,0,""

   响应：

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} MQTT 订阅者：

   命令：

   .. code-block:: none

     AT+MQTTUSERCFG=0,1,"subscriber","espressif","123456789",0,0,""

   响应：

   .. code-block:: none

     OK

#. 连接 MQTT 代理。

   命令：

   .. code-block:: none

     AT+MQTTCONN=0,"192.168.3.102",8883,1

   响应：
  
   .. code-block:: none

     +MQTTCONNECTED:0,1,"192.168.3.102","8883","",1

     OK

   说明：

   - 您输入的 MQTT 代理域名或 MQTT 代理 IP 地址可能与上述命令中的不同。

#. 订阅 MQTT 主题。

   {IDF_TARGET_NAME} MQTT 订阅者：

   命令：

   .. code-block:: none

     AT+MQTTSUB=0,"topic",1

   响应：
  
   .. code-block:: none

     OK

#. 发布 MQTT 消息（字符串）。

   {IDF_TARGET_NAME} MQTT 发布者：

   命令：

   .. code-block:: none

     AT+MQTTPUB=0,"topic","test",1,0

   响应：
  
   .. code-block:: none

     OK

   说明：

   - 如果 {IDF_TARGET_NAME} MQTT 发布者成功发布消息，以下信息将会在 {IDF_TARGET_NAME} MQTT 订阅者端提示。

     .. code-block:: none

       +MQTTSUBRECV:0,"topic",4,test

#. 关闭 MQTT 连接。

   命令：

   .. code-block:: none

     AT+MQTTCLEAN=0

   响应：
  
   .. code-block:: none

     OK

基于 TCP 的 MQTT 连接（需要本地创建 MQTT 代理）（适用于数据量多）
---------------------------------------------------------------------------

以下示例同时使用两块 {IDF_TARGET_NAME} 开发板，其中一块作为 MQTT 发布者（只作为 MQTT 发布者角色），另一块作为 MQTT 订阅者（只作为 MQTT 订阅者角色）。

示例介绍了如何基于 TCP 创建 MQTT 连接。首先您需要创建一个本地 MQTT 代理，假设 MQTT 代理的 IP 地址为 ``192.168.3.102``，端口为 ``8883``。

如果您发布消息的数据量相对较多，已经超过了单条 AT 命令的长度阈值 ``256``，则您可以使用 :ref:`AT+MQTTPUBRAW <cmd-MQTTPUBRAW>` 命令。

.. Important::
  步骤中以 ``{IDF_TARGET_NAME} MQTT 发布者`` 开头的操作只需要在 {IDF_TARGET_NAME} MQTT 发布者端执行即可，以 ``{IDF_TARGET_NAME} MQTT 订阅者`` 开头的操作只需要在 {IDF_TARGET_NAME} MQTT 订阅者端执行即可。如果操作没有特别指明在哪端操作，则需要在发布者端和订阅者端都执行。

#. 设置 MQTT 用户属性。

   {IDF_TARGET_NAME} MQTT 发布者：

   命令：

   .. code-block:: none

     AT+MQTTUSERCFG=0,1,"publisher","espressif","123456789",0,0,""

   响应：

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} MQTT 订阅者：

   命令：

   .. code-block:: none

     AT+MQTTUSERCFG=0,1,"subscriber","espressif","123456789",0,0,""

   响应：

   .. code-block:: none

     OK

#. 连接 MQTT 代理。

   命令：

   .. code-block:: none

     AT+MQTTCONN=0,"192.168.3.102",8883,1

   响应：

   .. code-block:: none

     +MQTTCONNECTED:0,1,"192.168.3.102","8883","",1

     OK

   说明：

   - 您输入的 MQTT 代理域名或 MQTT 代理 IP 地址可能与上述命令中的不同。

#. 订阅 MQTT 主题。

   {IDF_TARGET_NAME} MQTT 订阅者：

   命令：

   .. code-block:: none

     AT+MQTTSUB=0,"topic",1

   响应：

   .. code-block:: none

     OK

#. 发布 MQTT 消息（字符串）。

   假设你想要发布消息的数据如下，长度为 427 字节。

   .. code-block:: none

     {"headers": {"Accept": "application/json","Accept-Encoding": "gzip, deflate","Accept-Language": "en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7","Content-Length": "0","Host": "httpbin.org","Origin": "http://httpbin.org","Referer": "http://httpbin.org/","User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36","X-Amzn-Trace-Id": "Root=1-6150581e-1ad4bd5254b4bf5218070413"}}

   {IDF_TARGET_NAME} MQTT 发布者：

   命令：

   .. code-block:: none

     AT+MQTTPUBRAW=0,"topic",427,0,0

   响应：

   .. code-block:: none

     OK

     >

   上述响应表示 AT 已准备好接收串行数据，此时您可以输入数据，当 AT 接收到的数据长度达到 ``<length>`` 后，数据传输开始。

   .. code-block:: none

     +MQTTPUB:OK

   说明：

   - ``AT`` 输出 ``>`` 字符后，数据中的特殊字符不需要转义字符进行转义，也不需要以新行结尾（CR-LF）。
   - 如果 {IDF_TARGET_NAME} MQTT 发布者成功发布消息，以下信息将会在 {IDF_TARGET_NAME} MQTT 订阅者端提示。

     .. code-block:: none

       +MQTTSUBRECV:0,"topic",427,{"headers": {"Accept": "application/json","Accept-Encoding": "gzip, deflate","Accept-Language": "en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7","Content-Length": "0","Host": "httpbin.org","Origin": "http://httpbin.org","Referer": "http://httpbin.org/","User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36","X-Amzn-Trace-Id": "Root=1-6150581e-1ad4bd5254b4bf5218070413"}}

#. 关闭 MQTT 连接。

   命令：

   .. code-block:: none

     AT+MQTTCLEAN=0

   响应：

   .. code-block:: none

     OK

基于 TLS 的 MQTT 连接（需要本地创建 MQTT 代理）
-------------------------------------------------

以下示例同时使用两块 {IDF_TARGET_NAME} 开发板，其中一块作为 MQTT 发布者（只作为 MQTT 发布者角色），另一块作为 MQTT 订阅者（只作为 MQTT 订阅者角色）。

示例介绍了如何基于 TLS 创建 MQTT 连接。首先您需要创建一个本地 MQTT 代理，假设 MQTT 代理的 IP 地址为 ``192.168.3.102``，端口为 ``8883``。

.. Important::
  步骤中以 ``{IDF_TARGET_NAME} MQTT 发布者`` 开头的操作只需要在 {IDF_TARGET_NAME} MQTT 发布者端执行即可，以 ``{IDF_TARGET_NAME} MQTT 订阅者`` 开头的操作只需要在 {IDF_TARGET_NAME} MQTT 订阅者端执行即可。如果操作没有特别指明在哪端操作，则需要在发布者端和订阅者端都执行。

#. 设置时区和 SNTP 服务器。

   命令：

   .. code-block:: none

     AT+CIPSNTPCFG=1,8,"ntp1.aliyun.com"

   响应：
  
   .. code-block:: none

     OK

#. 查询 SNTP 时间。

   命令：

   .. code-block:: none

     AT+CIPSNTPTIME?

   响应：
  
   .. code-block:: none

     +CIPSNTPTIME:Thu Sep  2 18:57:03 2021
     OK

   说明：

   - 您的查询 SNTP 结果可能与上述响应中的不同。
   - 请确保 SNTP 时间一定是真实有效的时间，不能是 1970 年及之前的时间。
   - 设置时间是为了在 TLS 认证时校验证书的有效期。

#. 设置 MQTT 用户属性。

   {IDF_TARGET_NAME} MQTT 发布者：

   命令：

   .. code-block:: none

     AT+MQTTUSERCFG=0,4,"publisher","espressif","123456789",0,0,""

   响应：

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} MQTT 订阅者：

   命令：

   .. code-block:: none

     AT+MQTTUSERCFG=0,4,"subscriber","espressif","123456789",0,0,""

   响应：
  
   .. code-block:: none

     OK

#. 设置 MQTT 连接属性。

   命令：

   .. code-block:: none

     AT+MQTTCONNCFG=0,0,0,"lwtt","lwtm",0,0

   响应：
  
   .. code-block:: none

     OK

#. 连接 MQTT 代理。

   命令：

   .. code-block:: none

     AT+MQTTCONN=0,"192.168.3.102",8883,1

   响应：
  
   .. code-block:: none

     +MQTTCONNECTED:0,4,"192.168.3.102","8883","",1

     OK

   说明：

   - 您输入的 MQTT 代理域名或 MQTT 代理 IP 地址可能与上述命令中的不同。

#. 订阅 MQTT 主题。

   {IDF_TARGET_NAME} MQTT 订阅者：

   命令：

   .. code-block:: none

     AT+MQTTSUB=0,"topic",1

   响应：
  
   .. code-block:: none

     OK

#. 发布 MQTT 消息（字符串）。

   {IDF_TARGET_NAME} MQTT 发布者：

   命令：

   .. code-block:: none

     AT+MQTTPUB=0,"topic","test",1,0

   响应：
  
   .. code-block:: none

     OK

   说明：

   - 如果 {IDF_TARGET_NAME} MQTT 发布者成功发布消息，以下信息将会在 {IDF_TARGET_NAME} MQTT 订阅者端提示。

     .. code-block:: none

       +MQTTSUBRECV:0,"topic",4,test

#. 关闭 MQTT 连接。

   命令：

   .. code-block:: none

     AT+MQTTCLEAN=0

   响应：
  
   .. code-block:: none

     OK

基于 WSS 的 MQTT 连接
-------------------------------------------------

以下示例同时使用两块 {IDF_TARGET_NAME} 开发板，其中一块作为 MQTT 发布者（只作为 MQTT 发布者角色），另一块作为 MQTT 订阅者（只作为 MQTT 订阅者角色）。

示例介绍了如何基于 WSS 创建 MQTT 连接。MQTT 代理域名为 ``mqtt.eclipseprojects.io``，资源路径为 ``mqtt``，端口为 ``443``。

.. Important::
  步骤中以 ``{IDF_TARGET_NAME} MQTT 发布者`` 开头的操作只需要在 {IDF_TARGET_NAME} MQTT 发布者端执行即可，以 ``{IDF_TARGET_NAME} MQTT 订阅者`` 开头的操作只需要在 {IDF_TARGET_NAME} MQTT 订阅者端执行即可。如果操作没有特别指明在哪端操作，则需要在发布者端和订阅者端都执行。

#. 设置时区和 SNTP 服务器。

   命令：

   .. code-block:: none

     AT+CIPSNTPCFG=1,8,"ntp1.aliyun.com"

   响应：
  
   .. code-block:: none

     OK

#. 查询 SNTP 时间。

   命令：

   .. code-block:: none

     AT+CIPSNTPTIME?

   响应：
  
   .. code-block:: none

     +CIPSNTPTIME:Thu Sep  2 18:57:03 2021
     OK

   说明：

   - 您的查询 SNTP 结果可能与上述响应中的不同。
   - 请确保 SNTP 时间一定是真实有效的时间，不能是 1970 年及之前的时间。
   - 设置时间是为了在 TLS 认证时校验证书的有效期。

#. 设置 MQTT 用户属性。

   {IDF_TARGET_NAME} MQTT 发布者：

   命令：

   .. code-block:: none

     AT+MQTTUSERCFG=0,7,"publisher","espressif","1234567890",0,0,"mqtt"

   响应：

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} MQTT 订阅者：

   命令：

   .. code-block:: none

     AT+MQTTUSERCFG=0,7,"subscriber","espressif","1234567890",0,0,"mqtt"

   响应：
  
   .. code-block:: none

     OK

#. 连接 MQTT 代理。

   命令：

   .. code-block:: none

     AT+MQTTCONN=0,"mqtt.eclipseprojects.io",443,1

   响应：
  
   .. code-block:: none

     +MQTTCONNECTED:0,7,"mqtt.eclipseprojects.io","443","/mqtt",1

     OK

   说明：

   - 您输入的 MQTT 代理域名或 MQTT 代理 IP 地址可能与上述命令中的不同。

#. 订阅 MQTT 主题。

   {IDF_TARGET_NAME} MQTT 订阅者：

   命令：

   .. code-block:: none

     AT+MQTTSUB=0,"topic",1

   响应：
  
   .. code-block:: none

     OK

#. 发布 MQTT 消息（字符串）。

   {IDF_TARGET_NAME} MQTT 发布者：

   命令：

   .. code-block:: none

     AT+MQTTPUB=0,"topic","test",1,0

   响应：
  
   .. code-block:: none

     OK

   说明：

   - 如果 {IDF_TARGET_NAME} MQTT 发布者成功发布消息，以下信息将会在 {IDF_TARGET_NAME} MQTT 订阅者端提示。

     .. code-block:: none

       +MQTTSUBRECV:0,"topic",4,test

#. 关闭 MQTT 连接。

   命令：

   .. code-block:: none

     AT+MQTTCLEAN=0

   响应：
  
   .. code-block:: none

     OK
