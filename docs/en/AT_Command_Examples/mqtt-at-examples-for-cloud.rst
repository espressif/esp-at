MQTT AT Examples for Cloud
===========================================

:link_to_translation:`zh_CN:[中文]`

This document mainly describes how to connect your {IDF_TARGET_NAME} to AWS IoT with MQTT AT commands.

.. Important::
    For details on how to use MQTT AT commands, please refer to :doc:`../AT_Command_Set/MQTT_AT_Commands`.
    You need to become familiar with the AWS IoT by reading the `AWS IoT Development Guide <https://docs.aws.amazon.com/iot/latest/developerguide/iot-gs.html>`_.

Please follow the steps below to connect your {IDF_TARGET_NAME} to AWS IoT with ESP-AT.

.. contents::
   :local:
   :depth: 1

Obtain certificates and endpoints from AWS IoT
---------------------------------------------------------------------------

1. Sign in to your AWS IoT Console account and switch to the IoT Core services.

2. Create an AWS IoT policy, thing, and certificates following the instructions in `Create AWS IoT Resources <https://docs.aws.amazon.com/en_us/iot/latest/developerguide/create-iot-resources.html>`_.
  
  Make sure you have got the following certificate and key files:

  - device.pem.crt (Device certificate)
  - private.pem.key (Private key)
  - Amazon-root-CA-1.pem (Root CA certificate)

3. Get the endpoint and bind the thing to the policy through the certificate according to the documentation `Set up the policy <https://docs.aws.amazon.com/en_us/iot/latest/developerguide/using-laptop-as-device.html>`_.

  The endpoint value has the format of `xxx-ats.iot.us-east-2.amazonaws.com`.

.. note::
  It is strongly recommended to familiarize yourself with the `AWS IoT Developer Guide <https://docs.aws.amazon.com/iot/latest/developerguide/what-is-aws-iot.html>`_. Below are some key points from this Guide that are worth noting.

  - All devices must have a device certificate, private key, and root CA certificate installed in order to communicate with AWS IoT.
  - Information on how to activate certificates.
  - Select Ohio as your region.

Connect to AWS IoT based on mutual authentication with MQTT AT commands
---------------------------------------------------------------------------

Replace certificates
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Open your local ESP-AT project and do the following:

- Replace :component_file:`customized_partitions/raw_data/mqtt_ca/mqtt_ca.crt` with ``Amazon-root-CA-1.pem``.
- Replace :component_file:`customized_partitions/raw_data/mqtt_cert/mqtt_client.crt` with ``device.pem.crt``.
- Replace :component_file:`customized_partitions/raw_data/mqtt_key/mqtt_client.key` with ``private.pem.key``.

Compile and flash the AT firmware
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Compile the ESP-AT project to build the AT firmware, and flash the firmware to your {IDF_TARGET_NAME}. For more information, please refer to :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`.

.. note::
  If you do not want to compile the ESP-AT project to replace certificates, you can directly use the AT command to replace certificates in the firmware. For more information, please refer to :doc:`../Compile_and_Develop/How_to_update_pki_config`.

Use AT commands to connect to AWS IoT
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Set the Wi-Fi mode to station.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:
  
   .. code-block:: none

     OK

#. Connect to the AP.

   Command:

   .. code-block:: none

     AT+CWJAP=<ssid>,<password>

   Response:
  
   .. code-block:: none

     OK

#. Set the SNTP server.

   Command:

   .. code-block:: none

     AT+CIPSNTPCFG=1,8,"pool.ntp.org"

   Response:
  
   .. code-block:: none

     OK

#. Query the SNTP time.

   Command:

   .. code-block:: none

     AT+CIPSNTPTIME?

   Response:
  
   .. code-block:: none

     +CIPSNTPTIME:<asctime style time>
     OK

   Note:

   - The <asctime style time> obtained at this time must be the real-time time of the set time zone, otherwise the connection will fail due to the validity period of the certificate.

#. Set MQTT user properties.

   Command:

   .. code-block:: none

     AT+MQTTUSERCFG=0,5,"esp32","espressif","1234567890",0,0,""

   Response:
  
   .. code-block:: none

     OK

   Note:

   - If the second parameter of AT+MQTTUSERCFG is 5, it is authenticated by both sides and cannot be changed.

#. Connect to AWS IoT.

   Command:

   .. code-block:: none

     AT+MQTTCONN=0,"<endpoint>",8883,1

   Response:
  
   .. code-block:: none

     +MQTTCONNECTED:0,5,<endpoint>,"8883","",1
     OK

   Note:

   - Please fill in your endpoint value in the `<endpoint>` parameter.
   - The port 8883 cannot be changed.

#. Subscribe to messages.

   Command:

   .. code-block:: none

     AT+MQTTSUB=0,"topic/esp32at",1

   Response:
  
   .. code-block:: none

     OK

#. Publish a message.

   Command:

   .. code-block:: none

     AT+MQTTPUB=0,"topic/esp32at","hello aws!",1,0

   Response:
  
   .. code-block:: none

     +MQTTSUBRECV:0,"topic/esp32at",10,hello aws!

     OK

Example log
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Log for normal interaction is as follows:

#. Log on the ESP32 side

   .. figure:: ../../img/esp32at-log.png
       :scale: 100 %
       :align: center
       :alt: Log of Connecting to AWS IoT on ESP32 Side

#. Log on the AWS side

   .. figure:: ../../img/aws-log.png
       :scale: 100 %
       :align: center
       :alt: Log of Connecting to AWS IoT on AWS Side
