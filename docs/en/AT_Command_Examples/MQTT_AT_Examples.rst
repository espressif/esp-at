MQTT AT Examples
==========================

:link_to_translation:`zh_CN:[中文]`

This document provides detailed command examples to illustrate how to utilize :doc:`../AT_Command_Set/MQTT_AT_Commands` on {IDF_TARGET_NAME}.

.. contents::
   :local:
   :depth: 1

.. Important::
  * The examples described in this document are based on the situation that Wi-Fi has been connected.

MQTT over TCP (with a local MQTT broker)(suitable for a small amount of data)
------------------------------------------------------------------------------

Below is an example of using two {IDF_TARGET_NAME} development boards, one as a MQTT publisher (only as MQTT publisher role), the other one as a MQTT subscriber (only as MQTT subscriber role).

The example shows how to establish MQTT connections over TCP. You need to first create a local MQTT broker. For example, the MQTT broker's IP address is ``192.168.3.102``, and the port is ``8883``.

.. Important::
  In the step, the operations starting with ``{IDF_TARGET_NAME} MQTT publisher`` only need to be executed at {IDF_TARGET_NAME} MQTT publisher, and the operations starting with ``{IDF_TARGET_NAME} MQTT subscriber`` only need to be executed at {IDF_TARGET_NAME} MQTT subscriber. If the operation is not specified on which side it is executed, it needs to be executed on both the publisher side and the subscriber side.

#. Set MQTT user configuration.

   {IDF_TARGET_NAME} MQTT publisher:

   Command:

   .. code-block:: none

     AT+MQTTUSERCFG=0,1,"publisher","espressif","123456789",0,0,""

   Response:

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} MQTT subscriber:

   Command:

   .. code-block:: none

     AT+MQTTUSERCFG=0,1,"subscriber","espressif","123456789",0,0,""

   Response:

   .. code-block:: none

     OK

#. Connect to MQTT brokers.

   Command:

   .. code-block:: none

     AT+MQTTCONN=0,"192.168.3.102",8883,1

   Response:
  
   .. code-block:: none

     +MQTTCONNECTED:0,1,"192.168.3.102","8883","",1

     OK

   Note:

   - The MQTT broker domain or MQTT broker IP address you enter may be different from those in the above command.

#. Subscribe to MQTT topics.

   {IDF_TARGET_NAME} MQTT subscriber:

   Command:

   .. code-block:: none

     AT+MQTTSUB=0,"topic",1

   Response:
  
   .. code-block:: none

     OK

#. Publish MQTT messages in string.

   {IDF_TARGET_NAME} MQTT publisher:

   Command:

   .. code-block:: none

     AT+MQTTPUB=0,"topic","test",1,0

   Response:
  
   .. code-block:: none

     OK

   Note:

   - If the {IDF_TARGET_NAME} MQTT publisher successfully publishes the message, following message will be prompted on the {IDF_TARGET_NAME} MQTT subscriber.

     .. code-block:: none

       +MQTTSUBRECV:0,"topic",4,test

#. Close MQTT connections.

   Command:

   .. code-block:: none

     AT+MQTTCLEAN=0

   Response:
  
   .. code-block:: none

     OK

MQTT over TCP (with a local MQTT broker)(suitable for large amounts of data)
------------------------------------------------------------------------------

Below is an example of using two {IDF_TARGET_NAME} development boards, one as a MQTT publisher (only as MQTT publisher role), the other one as a MQTT subscriber (only as MQTT subscriber role).

The example shows how to establish MQTT connections over TCP. You need to first create a local MQTT broker. For example, the MQTT broker's IP address is ``192.168.3.102``, and the port is ``8883``.

If the amount of data you publish is relatively large, and the length of a single AT command has exceeded the threshold of ``256``, it is recommended that you use the :ref:`AT+MQTTPUBRAW <cmd-MQTTPUBRAW>` command.

.. Important::
  In the step, the operations starting with ``{IDF_TARGET_NAME} MQTT publisher`` only need to be executed at {IDF_TARGET_NAME} MQTT publisher, and the operations starting with ``{IDF_TARGET_NAME} MQTT subscriber`` only need to be executed at {IDF_TARGET_NAME} MQTT subscriber. If the operation is not specified on which side it is executed, it needs to be executed on both the publisher side and the subscriber side.

#. Set MQTT user configuration.

   {IDF_TARGET_NAME} MQTT publisher:

   Command:

   .. code-block:: none

     AT+MQTTUSERCFG=0,1,"publisher","espressif","123456789",0,0,""

   Response:

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} MQTT subscriber:

   Command:

   .. code-block:: none

     AT+MQTTUSERCFG=0,1,"subscriber","espressif","123456789",0,0,""

   Response:

   .. code-block:: none

     OK

#. Connect to MQTT brokers.

   Command:

   .. code-block:: none

     AT+MQTTCONN=0,"192.168.3.102",8883,1

   Response:

   .. code-block:: none

     +MQTTCONNECTED:0,1,"192.168.3.102","8883","",1

     OK

   Note:

   - The MQTT broker domain or MQTT broker IP address you enter may be different from those in the above command.

#. Subscribe to MQTT topics.

   {IDF_TARGET_NAME} MQTT subscriber:

   Command:

   .. code-block:: none

     AT+MQTTSUB=0,"topic",1

   Response:

   .. code-block:: none

     OK

#. Publish MQTT messages in string.

   Assume the data you want to publish is as follows, length is 427 bytes.

   .. code-block:: none

     {"headers": {"Accept": "application/json","Accept-Encoding": "gzip, deflate","Accept-Language": "en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7","Content-Length": "0","Host": "httpbin.org","Origin": "http://httpbin.org","Referer": "http://httpbin.org/","User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36","X-Amzn-Trace-Id": "Root=1-6150581e-1ad4bd5254b4bf5218070413"}}

   {IDF_TARGET_NAME} MQTT publisher:

   Command:

   .. code-block:: none

     AT+MQTTPUBRAW=0,"topic",427,0,0

   Response:

   .. code-block:: none

     OK

     >

   This response indicates that AT is ready for receiving serial data. You should enter the data, and when the data length reaches the ``<length>`` value, the transmission of data starts.

   .. code-block:: none

     +MQTTPUB:OK

   Note:

   - After AT outputs the ``>`` character, the special characters in the data does not need to be escaped through the escape character, and it does not need to end with a new line(CR-LF).
   - If the {IDF_TARGET_NAME} MQTT publisher successfully publishes the message, following message will be prompted on the {IDF_TARGET_NAME} MQTT subscriber.

     .. code-block:: none

       +MQTTSUBRECV:0,"topic",427,{"headers": {"Accept": "application/json","Accept-Encoding": "gzip, deflate","Accept-Language": "en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7","Content-Length": "0","Host": "httpbin.org","Origin": "http://httpbin.org","Referer": "http://httpbin.org/","User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36","X-Amzn-Trace-Id": "Root=1-6150581e-1ad4bd5254b4bf5218070413"}}

#. Close MQTT connections.

   Command:

   .. code-block:: none

     AT+MQTTCLEAN=0

   Response:

   .. code-block:: none

     OK

MQTT over TLS (with a local MQTT broker)
------------------------------------------

Below is an example of using two {IDF_TARGET_NAME} development boards, one as a MQTT publisher (only as MQTT publisher role), the other one as a MQTT subscriber (only as MQTT subscriber role).

The example shows how to establish MQTT connections over TLS. You need to first create a local MQTT broker. For example, the MQTT broker's IP address is ``192.168.3.102``, and port is ``8883``.

.. Important::
  In the step, the operations starting with ``{IDF_TARGET_NAME} MQTT publisher`` only need to be executed at {IDF_TARGET_NAME} MQTT publisher, and the operations starting with ``{IDF_TARGET_NAME} MQTT subscriber`` only need to be executed at {IDF_TARGET_NAME} MQTT subscriber. If the operation is not specified on which side it is executed, it needs to be executed on both the publisher side and the subscriber side.

#. Set the time zone and the SNTP server.

   Command:

   .. code-block:: none

     AT+CIPSNTPCFG=1,8,"ntp1.aliyun.com"

   Response:
  
   .. code-block:: none

     OK

#. Query the SNTP time.

   Command:

   .. code-block:: none

     AT+CIPSNTPTIME?

   Response:
  
   .. code-block:: none

     +CIPSNTPTIME:Thu Sep  2 18:57:03 2021
     OK

   Note:

   - The time you obtained may be different from that in the above response.
   - Please make sure that the SNTP time must be a real and valid time and cannot be the time in 1970 or before.
   - The purpose of setting the time is to verify the validity period of the certificates during TLS authentication.

#. Set MQTT user configuration.

   {IDF_TARGET_NAME} MQTT publisher:

   Command:

   .. code-block:: none

     AT+MQTTUSERCFG=0,4,"publisher","espressif","123456789",0,0,""

   Response:

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} MQTT subscriber:

   Command:

   .. code-block:: none

     AT+MQTTUSERCFG=0,4,"subscriber","espressif","123456789",0,0,""

   Response:
  
   .. code-block:: none

     OK

#. Set configuration of MQTT connection.

   Command:

   .. code-block:: none

     AT+MQTTCONNCFG=0,0,0,"lwtt","lwtm",0,0

   Response:
  
   .. code-block:: none

     OK

#. Connect to MQTT brokers.

   Command:

   .. code-block:: none

     AT+MQTTCONN=0,"192.168.3.102",8883,1

   Response:
  
   .. code-block:: none

     +MQTTCONNECTED:0,4,"192.168.3.102","8883","",1

     OK

   Note:

   - The MQTT broker domain or MQTT broker IP address you enter may be different from those in the above command.

#. Subscribe to MQTT topics.

   {IDF_TARGET_NAME} MQTT subscriber:

   Command:

   .. code-block:: none

     AT+MQTTSUB=0,"topic",1

   Response:
  
   .. code-block:: none

     OK

#. Publish MQTT messages in string.

   {IDF_TARGET_NAME} MQTT publisher:

   Command:

   .. code-block:: none

     AT+MQTTPUB=0,"topic","test",1,0

   Response:
  
   .. code-block:: none

     OK

   Note:

   - If the {IDF_TARGET_NAME} MQTT publisher successfully publishes the message, following message will be prompted on the {IDF_TARGET_NAME} MQTT subscriber.

     .. code-block:: none

       +MQTTSUBRECV:0,"topic",4,test

#. Close MQTT connections.

   Command:

   .. code-block:: none

     AT+MQTTCLEAN=0

   Response:
  
   .. code-block:: none

     OK

MQTT over WSS
------------------------------------------

Below is an example of using two {IDF_TARGET_NAME} development boards, one as a MQTT publisher (only as MQTT publisher role), the other one as a MQTT subscriber (only as MQTT subscriber role).

The example shows how to establish MQTT connections over WSS and how to communicate with a MQTT broker. For example, the MQTT broker's domain name is ``mqtt.eclipseprojects.io``, the path is ``mqtt``, and the port is ``443``.

.. Important::
  In the step, the operations starting with ``{IDF_TARGET_NAME} MQTT publisher`` only need to be executed at {IDF_TARGET_NAME} MQTT publisher, and the operations starting with ``{IDF_TARGET_NAME} MQTT subscriber`` only need to be executed at {IDF_TARGET_NAME} MQTT subscriber. If the operation is not specified on which side it is executed, it needs to be executed on both the publisher side and the subscriber side.

#. Set the time zone and the SNTP server.

   Command:

   .. code-block:: none

     AT+CIPSNTPCFG=1,8,"ntp1.aliyun.com"

   Response:
  
   .. code-block:: none

     OK

#. Query the SNTP time.

   Command:

   .. code-block:: none

     AT+CIPSNTPTIME?

   Response:
  
   .. code-block:: none

     +CIPSNTPTIME:Thu Sep  2 18:57:03 2021
     OK

   Note:

   - The time you obtained may be different from that in the above response.
   - Please make sure that the SNTP time must be a real and valid time and cannot be the time in 1970 or before.
   - The purpose of setting the time is to verify the validity period of the certificates during TLS authentication.

#. Set MQTT user configuration.

   {IDF_TARGET_NAME} MQTT publisher:

   Command:

   .. code-block:: none

     AT+MQTTUSERCFG=0,7,"publisher","espressif","1234567890",0,0,"mqtt"

   Response:

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} MQTT subscriber:

   Command:

   .. code-block:: none

     AT+MQTTUSERCFG=0,7,"subscriber","espressif","1234567890",0,0,"mqtt"

   Response:
  
   .. code-block:: none

     OK

#. Connect to MQTT brokers.

   Command:

   .. code-block:: none

     AT+MQTTCONN=0,"mqtt.eclipseprojects.io",443,1

   Response:
  
   .. code-block:: none

     +MQTTCONNECTED:0,7,"mqtt.eclipseprojects.io","443","/mqtt",1

     OK

   Note:

   - The MQTT broker domain or MQTT broker IP address you enter may be different from those in the above command.

#. Subscribe to MQTT topics.

   {IDF_TARGET_NAME} MQTT subscriber:

   Command:

   .. code-block:: none

     AT+MQTTSUB=0,"topic",1

   Response:
  
   .. code-block:: none

     OK

#. Publish MQTT messages in string.

   {IDF_TARGET_NAME} MQTT publisher:

   Command:

   .. code-block:: none

     AT+MQTTPUB=0,"topic","test",1,0

   Response:
  
   .. code-block:: none

     OK

   Note:

   - If the {IDF_TARGET_NAME} MQTT publisher successfully publishes the message, following message will be prompted on the {IDF_TARGET_NAME} MQTT subscriber.

     .. code-block:: none

       +MQTTSUBRECV:0,"topic",4,test

#. Close MQTT connections.

   Command:

   .. code-block:: none

     AT+MQTTCLEAN=0

   Response:
  
   .. code-block:: none

     OK
