.. _MQTT-AT:

MQTT AT Commands
================

:link_to_translation:`zh_CN:[中文]`

-  :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`: Set MQTT user configuration
-  :ref:`AT+MQTTCLIENTID <cmd-MQTTCLIENTID>`: Set MQTT client ID
-  :ref:`AT+MQTTUSERNAME <cmd-MQTTUSERNAME>`: Set MQTT username
-  :ref:`AT+MQTTPASSWORD <cmd-MQTTPASSWORD>`: Set MQTT password
-  :ref:`AT+MQTTCONNCFG <cmd-MQTTCONNCFG>`: Set configuration of MQTT connection
-  :ref:`AT+MQTTCONN <cmd-MQTTCONN>`: Connect to MQTT Brokers
-  :ref:`AT+MQTTPUB <cmd-MQTTPUB>`: Publish MQTT Messages in string
-  :ref:`AT+MQTTPUBRAW <cmd-MQTTPUBRAW>`: Publish MQTT messages in binary
-  :ref:`AT+MQTTSUB <cmd-MQTTSUB>`: Subscribe to MQTT topics
-  :ref:`AT+MQTTUNSUB <cmd-MQTTUNSUB>`: Unsubscribe from MQTT topics
-  :ref:`AT+MQTTCLEAN <cmd-MQTTCLEAN>`: Close MQTT connections
-  :ref:`MQTT AT Error Codes <MQTTErrCod>`
-  :ref:`MQTT AT Notes <MQTTNote>`

.. _cmd-MQTTUSERCFG:

:ref:`AT+MQTTUSERCFG <MQTT-AT>`: Set MQTT User Configuration
-------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set MQTT User Configuration.

**Command:**

::

    AT+MQTTUSERCFG=<LinkID>,<scheme>,<"client_id">,<"username">,<"password">,<cert_key_ID>,<CA_ID>,<"path">

**Response:**

::

   OK

Parameters
^^^^^^^^^^

-  **<LinkID>**: currently only supports link ID 0.
-  **<scheme>**:

   -  1: MQTT over TCP.
   -  2: MQTT over TLS (no certificate verify).
   -  3: MQTT over TLS (verify server certificate).
   -  4: MQTT over TLS (provide client certificate).
   -  5: MQTT over TLS (verify server certificate and provide client certificate).
   -  6: MQTT over WebSocket (based on TCP).
   -  7: MQTT over WebSocket Secure (based on TLS, no certificate verify).
   -  8: MQTT over WebSocket Secure (based on TLS, verify server certificate).
   -  9: MQTT over WebSocket Secure (based on TLS, provide client certificate).
   -  10: MQTT over WebSocket Secure (based on TLS, verify server certificate and provide client certificate).

-  **<client_id>**: MQTT client ID. Maximum length: 256 bytes.
-  **<username>**: the username to login to the MQTT broker. Maximum length: 64 bytes.
-  **<password>**: the password to login to the MQTT broker. Maximum length: 64 bytes.
-  **<cert_key_ID>**: certificate ID. Currently, ESP-AT only supports one certificate for ID 0.
-  **<CA_ID>**: CA ID. Currently, ESP-AT only supports one CA for ID 0.
-  **<path>**: the path of the resource. Maximum length: 32 bytes.

Note
^^^^^

-  The length of the entire AT command should be less than 256 bytes.

.. _cmd-MQTTCLIENTID:

:ref:`AT+MQTTCLIENTID <MQTT-AT>`: Set MQTT Client ID
--------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set MQTT Client ID.  

**Command:**

::

    AT+MQTTCLIENTID=<LinkID>,<"client_id">

**Response:**

::

   OK

Parameters
^^^^^^^^^^

-  **<LinkID>**: currently only supports link ID 0.
-  **<client_id>**: MQTT client ID.

Notes
^^^^^

-  The length of the entire AT command should be less than 256 bytes.
-  The command :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>` can also set MQTT client ID. The differences between the two commands include:

   - You can use ``AT+MQTTCLIENTID`` to set a relatively long client ID since there is a limitation on the length of the ``AT+MQTTUSERCFG`` command.
   - You should set ``AT+MQTTCLIENTID`` after setting the ``AT+MQTTUSERCFG`` command.

.. _cmd-MQTTUSERNAME:

:ref:`AT+MQTTUSERNAME <MQTT-AT>`: Set MQTT Username
-------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set MQTT username.

**Command:**

::

    AT+MQTTUSERNAME=<LinkID>,<"username">

**Response:**

::

   OK

Parameters
^^^^^^^^^^

-  **<LinkID>**: only supports link ID 0 currently.
-  **<username>**: the username to login to the MQTT broker.

Notes
^^^^^

-  The length of the entire AT command should be less than 256 bytes.
-  The command :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>` can also set MQTT username. The differences between the two commands include:

   - You can use ``AT+MQTTUSERNAME`` to set a relatively long username since there is a limitation on the length of the ``AT+MQTTUSERCFG`` command.
   - You should set ``AT+MQTTUSERNAME`` after setting the command ``AT+MQTTUSERCFG``.

.. _cmd-MQTTPASSWORD:

:ref:`AT+MQTTPASSWORD <MQTT-AT>`: Set MQTT Password
-------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set MQTT password. 

**Command:**

::

    AT+MQTTPASSWORD=<LinkID>,<"password">

**Response:**

::

   OK

Parameters
^^^^^^^^^^

-  **<LinkID>**: only supports link ID 0 currently.
-  **<password>**: the password to login to the MQTT broker.

Notes
^^^^^

-  The length of the entire AT command should be less than 256 bytes.
-  The command :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>` can also set MQTT password. The differences between the two commands include:

   - You can use ``AT+MQTTPASSWORD`` to set a relatively long password since there is a limitation on the length of the ``AT+MQTTUSERCFG`` command.
   - You should set ``AT+MQTTPASSWORD`` after setting the command ``AT+MQTTUSERCFG``.

.. _cmd-MQTTCONNCFG:

:ref:`AT+MQTTCONNCFG <MQTT-AT>`: Set Configuration of MQTT Connection
-------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Set configuration of MQTT Connection.

**Command:**

::

    AT+MQTTCONNCFG=<LinkID>,<keepalive>,<disable_clean_session>,<"lwt_topic">,<"lwt_msg">,<lwt_qos>,<lwt_retain>

**Response:**

::

   OK

Parameters
^^^^^^^^^^

-  **<LinkID>**: only supports link ID 0 currently.
-  **<keepalive>**: timeout of MQTT ping. Unit: second. Range [0,7200]. The default value is 0, which will be force-changed to 120 s.
-  **<disable_clean_session>**: set MQTT clean session. For more details about this parameter, please refer to the section `Clean Session <http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.pdf>`_ in *MQTT Version 3.1.1*.

   -  0: enable clean session.
   -  1: disable clean session.

-  **<lwt_topic>**: LWT (Last Will and Testament) message topic. Maximum length: 128 bytes.
-  **<lwt_msg>**: LWT message. Maximum length: 64 bytes.
-  **<lwt_qos>**: LWT QoS, which can be set to 0, 1, or 2. Default: 0.
-  **<lwt_retain>**: LWT retain, which can be set to 0 or 1. Default: 0.

.. _cmd-MQTTCONN:

:ref:`AT+MQTTCONN <MQTT-AT>`: Connect to MQTT Brokers
--------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the MQTT broker that ESP devices are connected to.

**Command:**

::

    AT+MQTTCONN?

**Response:**

::

    +MQTTCONN:<LinkID>,<state>,<scheme><"host">,<port>,<"path">,<reconnect>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Connect to an MQTT broker.  

**Command:**

::

    AT+MQTTCONN=<LinkID>,<"host">,<port>,<reconnect>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<LinkID>**: only supports link ID 0 currently.
-  **<host>**: MQTT broker domain. Maximum length: 128 bytes.
-  **<port>**: MQTT broker port. Maximum: port 65535.
-  **<path>**: path. Maximum length: 32 bytes.
-  **<reconnect>**:

   -  0: MQTT will not reconnect automatically.
   -  1: MQTT will reconnect automatically. It takes more resources.

-  **<state>**: MQTT state.

   -  0: MQTT uninitialized.
   -  1: already set ``AT+MQTTUSERCFG``.
   -  2: already set ``AT+MQTTCONNCFG``.
   -  3: connection disconnected.
   -  4: connection established.
   -  5: connected, but did not subscribe to any topic.
   -  6: connected, and subscribed to MQTT topics.

-  **<scheme>**:

   -  1: MQTT over TCP.
   -  2: MQTT over TLS (no certificate verify).
   -  3: MQTT over TLS (verify server certificate).
   -  4: MQTT over TLS (provide client certificate).
   -  5: MQTT over TLS (verify server certificate and provide client certificate).
   -  6: MQTT over WebSocket (based on TCP).
   -  7: MQTT over WebSocket Secure (based on TLS, verify no certificate).
   -  8: MQTT over WebSocket Secure (based on TLS, verify server certificate).
   -  9: MQTT over WebSocket Secure (based on TLS, provide client certificate).
   -  10: MQTT over WebSocket Secure (based on TLS, verify server certificate and provide client certificate).

.. _cmd-MQTTPUB:

:ref:`AT+MQTTPUB <MQTT-AT>`: Publish MQTT Messages in String
---------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Publish MQTT messages in string to a defined topic. If you need to publish messages in binary, please use the :ref:`AT+MQTTPUBRAW <cmd-MQTTPUBRAW>` command.

**Command:**

::

    AT+MQTTPUB=<LinkID>,<"topic">,<"data">,<qos>,<retain>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<LinkID>**: only supports link ID 0 currently.
-  **<topic>**: MQTT topic. Maximum length: 128 bytes.
-  **<data>**: MQTT message in string.
-  **<qos>**: QoS of message, which can be set to 0, 1, or 2. Default: 0.
-  **<retain>**: retain flag.

Notes
^^^^^

-  The length of the entire AT command should be less than 256 bytes.
-  This command cannot send data ``\0``. If you need to send ``\0``, please use the command :ref:`AT+MQTTPUBRAW <cmd-MQTTPUBRAW>` instead.

.. _cmd-MQTTPUBRAW:

:ref:`AT+MQTTPUBRAW <MQTT-AT>`: Publish MQTT Messages in Binary
------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Publish MQTT messages in binary to a defined topic.

**Command:**

::

    AT+MQTTPUBRAW=<LinkID>,<"topic">,<length>,<qos>,<retain>

**Response:**

::

    OK
    > 

The symbol ``>`` indicates that AT is ready for receiving serial data, and you can enter the data now. When the requirement of message length determined by the parameter ``<length>`` is met, the transmission starts.

If the transmission is successful, AT returns:

::

    +MQTTPUB:OK  

Otherwise, it returns:

::

    +MQTTPUB:FAIL    

Parameters
^^^^^^^^^^

-  **<LinkID>**: only supports link ID 0 currently.
-  **<topic>**: MQTT topic. Maximum length: 128 bytes.
-  **<length>**: length of MQTT message. The maximum length varies on different ESP devices.

   -  For ESP32 devices, the maximum length is limited by available memory.
   -  For ESP8266 devices, the maximum is limited by available memory and the marco ``MQTT_BUFFER_SIZE_BYTE``. The default value of the macro is ``512``. You can change the max length limitation by setting it in ``build.py menuconfig``. ``MQTT_BUFFER_SIZE_BYTE`` equals maximum published data length plus the MQTT header length (depends on topic name length).

-  **<qos>**: QoS of the published message, which can be set to 0, 1, or 2. Default is 0.
-  **<retain>**: retain flag.

.. _cmd-MQTTSUB:

:ref:`AT+MQTTSUB <MQTT-AT>`: Subscribe to MQTT Topics
--------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

List all MQTT topics that have been already subscribed.

**Command:**

::

    AT+MQTTSUB?    


**Response:**

::

    +MQTTSUB:<LinkID>,<state>,<"topic1">,<qos>  
    +MQTTSUB:<LinkID>,<state>,<"topic2">,<qos>
    +MQTTSUB:<LinkID>,<state>,<"topic3">,<qos>
    ...
    OK

Set Command
^^^^^^^^^^^

**Function:**

Subscribe to defined MQTT topics with defined QoS. It supports subscribing to multiple topics.

**Command:**

::

    AT+MQTTSUB=<LinkID>,<"topic">,<qos>


**Response:**

::

    OK

When AT receives MQTT messages of the subscribed topic, it will prompt:

::

    +MQTTSUBRECV:<LinkID>,<"topic">,<data_length>,data    

If the topic has been subscribed before, it will prompt:

::

   ALREADY SUBSCRIBE

Parameters
^^^^^^^^^^

-  **<LinkID>**: only supports link ID 0 currently.
-  **<state>**: MQTT state.

   -  0: MQTT uninitialized.
   -  1: already set ``AT+MQTTUSERCFG``.
   -  2: already set ``AT+MQTTCONNCFG``.
   -  3: connection disconnected.
   -  4: connection established.
   -  5: connected, but subscribe to no topic.
   -  6: connected, and subscribed to MQTT topics.

-  **<topic>**: the topic that is subscribed to.
-  **<qos>**: the QoS that is subscribed to.

.. _cmd-MQTTUNSUB:

:ref:`AT+MQTTUNSUB <MQTT-AT>`: Unsubscribe from MQTT Topics
--------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Unsubscribe the client from defined topics. This command can be called multiple times to unsubscribe from different topics.

**Command:**

::

    AT+MQTTUNSUB=<LinkID>,<"topic">   


**Response:**

::

    OK

If the topic has not been subscribed, AT will prompt:

::

  NO UNSUBSCRIBE 
  
  OK

Parameters
^^^^^^^^^^

-  **<LinkID>**: only supports link ID 0 currently.
-  **<topic>**: MQTT topic. Maximum length: 128 bytes.

.. _cmd-MQTTCLEAN:

:ref:`AT+MQTTCLEAN <MQTT-AT>`: Close MQTT Connections
------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Function:**

Close the MQTT connection and release the resource.

**Command:**

::

    AT+MQTTCLEAN=<LinkID>  

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<LinkID>**: only supports link ID 0 currently。

.. _MQTTErrCod:

:ref:`MQTT AT Error Codes <MQTT-AT>`
--------------------------------------

The MQTT Error code will be prompted as ``ERR CODE:0x<%08x>``.

.. list-table::
   :header-rows: 1
   :widths: 15 5

   * - Error Type
     - Error Code
   * - AT_MQTT_NO_CONFIGURED
     - 0x6001
   * - AT_MQTT_NOT_IN_CONFIGURED_STATE
     - 0x6002
   * - AT_MQTT_UNINITIATED_OR_ALREADY_CLEAN
     - 0x6003
   * - AT_MQTT_ALREADY_CONNECTED
     - 0x6004
   * - AT_MQTT_MALLOC_FAILED
     - 0x6005
   * - AT_MQTT_NULL_LINK
     - 0x6006
   * - AT_MQTT_NULL_PARAMTER
     - 0x6007
   * - AT_MQTT_PARAMETER_COUNTS_IS_WRONG
     - 0x6008
   * - AT_MQTT_TLS_CONFIG_ERROR
     - 0x6009
   * - AT_MQTT_PARAM_PREPARE_ERROR
     - 0x600A
   * - AT_MQTT_CLIENT_START_FAILED
     - 0x600B
   * - AT_MQTT_CLIENT_PUBLISH_FAILED
     - 0x600C
   * - AT_MQTT_CLIENT_SUBSCRIBE_FAILED
     - 0x600D
   * - AT_MQTT_CLIENT_UNSUBSCRIBE_FAILED
     - 0x600E
   * - AT_MQTT_CLIENT_DISCONNECT_FAILED
     - 0x600F
   * - AT_MQTT_LINK_ID_READ_FAILED
     - 0x6010
   * - AT_MQTT_LINK_ID_VALUE_IS_WRONG
     - 0x6011
   * - AT_MQTT_SCHEME_READ_FAILED
     - 0x6012
   * - AT_MQTT_SCHEME_VALUE_IS_WRONG
     - 0x6013
   * - AT_MQTT_CLIENT_ID_READ_FAILED
     - 0x6014
   * - AT_MQTT_CLIENT_ID_IS_NULL
     - 0x6015
   * - AT_MQTT_CLIENT_ID_IS_OVERLENGTH
     - 0x6016
   * - AT_MQTT_USERNAME_READ_FAILED
     - 0x6017
   * - AT_MQTT_USERNAME_IS_NULL
     - 0x6018
   * - AT_MQTT_USERNAME_IS_OVERLENGTH
     - 0x6019
   * - AT_MQTT_PASSWORD_READ_FAILED
     - 0x601A
   * - AT_MQTT_PASSWORD_IS_NULL
     - 0x601B
   * - AT_MQTT_PASSWORD_IS_OVERLENGTH
     - 0x601C
   * - AT_MQTT_CERT_KEY_ID_READ_FAILED
     - 0x601D
   * - AT_MQTT_CERT_KEY_ID_VALUE_IS_WRONG
     - 0x601E
   * - AT_MQTT_CA_ID_READ_FAILED
     - 0x601F
   * - AT_MQTT_CA_ID_VALUE_IS_WRONG
     - 0x6020
   * - AT_MQTT_CA_LENGTH_ERROR
     - 0x6021
   * - AT_MQTT_CA_READ_FAILED
     - 0x6022
   * - AT_MQTT_CERT_LENGTH_ERROR
     - 0x6023
   * - AT_MQTT_CERT_READ_FAILED
     - 0x6024
   * - AT_MQTT_KEY_LENGTH_ERROR
     - 0x6025
   * - AT_MQTT_KEY_READ_FAILED
     - 0x6026
   * - AT_MQTT_PATH_READ_FAILED
     - 0x6027
   * - AT_MQTT_PATH_IS_NULL
     - 0x6028
   * - AT_MQTT_PATH_IS_OVERLENGTH
     - 0x6029
   * - AT_MQTT_VERSION_READ_FAILED
     - 0x602A
   * - AT_MQTT_KEEPALIVE_READ_FAILED
     - 0x602B
   * - AT_MQTT_KEEPALIVE_IS_NULL
     - 0x602C
   * - AT_MQTT_KEEPALIVE_VALUE_IS_WRONG
     - 0x602D
   * - AT_MQTT_DISABLE_CLEAN_SESSION_READ_FAILED
     - 0x602E
   * - AT_MQTT_DISABLE_CLEAN_SESSION_VALUE_IS_WRONG
     - 0x602F
   * - AT_MQTT_LWT_TOPIC_READ_FAILED
     - 0x6030
   * - AT_MQTT_LWT_TOPIC_IS_NULL
     - 0x6031
   * - AT_MQTT_LWT_TOPIC_IS_OVERLENGTH
     - 0x6032
   * - AT_MQTT_LWT_MSG_READ_FAILED
     - 0x6033
   * - AT_MQTT_LWT_MSG_IS_NULL
     - 0x6034
   * - AT_MQTT_LWT_MSG_IS_OVERLENGTH
     - 0x6035
   * - AT_MQTT_LWT_QOS_READ_FAILED
     - 0x6036
   * - AT_MQTT_LWT_QOS_VALUE_IS_WRONG
     - 0x6037
   * - AT_MQTT_LWT_RETAIN_READ_FAILED
     - 0x6038
   * - AT_MQTT_LWT_RETAIN_VALUE_IS_WRONG
     - 0x6039
   * - AT_MQTT_HOST_READ_FAILED
     - 0x603A
   * - AT_MQTT_HOST_IS_NULL
     - 0x603B
   * - AT_MQTT_HOST_IS_OVERLENGTH
     - 0x603C
   * - AT_MQTT_PORT_READ_FAILED
     - 0x603D
   * - AT_MQTT_PORT_VALUE_IS_WRONG
     - 0x603E
   * - AT_MQTT_RECONNECT_READ_FAILED
     - 0x603F
   * - AT_MQTT_RECONNECT_VALUE_IS_WRONG
     - 0x6040
   * - AT_MQTT_TOPIC_READ_FAILED
     - 0x6041
   * - AT_MQTT_TOPIC_IS_NULL
     - 0x6042
   * - AT_MQTT_TOPIC_IS_OVERLENGTH
     - 0x6043
   * - AT_MQTT_DATA_READ_FAILED
     - 0x6044
   * - AT_MQTT_DATA_IS_NULL
     - 0x6045
   * - AT_MQTT_DATA_IS_OVERLENGTH
     - 0x6046
   * - AT_MQTT_QOS_READ_FAILED
     - 0x6047
   * - AT_MQTT_QOS_VALUE_IS_WRONG
     - 0x6048
   * - AT_MQTT_RETAIN_READ_FAILED
     - 0x6049
   * - AT_MQTT_RETAIN_VALUE_IS_WRONG
     - 0x604A
   * - AT_MQTT_PUBLISH_LENGTH_READ_FAILED
     - 0x604B
   * - AT_MQTT_PUBLISH_LENGTH_VALUE_IS_WRONG
     - 0x604C
   * - AT_MQTT_RECV_LENGTH_IS_WRONG
     - 0x604D
   * - AT_MQTT_CREATE_SEMA_FAILED
     - 0x604E
   * - AT_MQTT_CREATE_EVENT_GROUP_FAILED
     - 0x604F
   * - AT_MQTT_URI_PARSE_FAILED
     - 0x6050
   * - AT_MQTT_IN_DISCONNECTED_STATE
     - 0x6051
   * - AT_MQTT_HOSTNAME_VERIFY_FAILED
     - 0x6052

.. _MQTTNote:

:ref:`MQTT AT Notes <MQTT-AT>`
-------------------------------

-  In general, AT MQTT commands responds within 10 s, except the command ``AT+MQTTCONN``. For example, if the router fails to access the Internet, the command ``AT+MQTTPUB`` will respond within 10 s. But the command ``AT+MQTTCONN`` may need more time due to packet retransmission in a bad network environment.
-  If the ``AT+MQTTCONN`` is based on a TLS connection, the timeout of each packet is 10 s, and the total timeout will be much longer depending on the handshake packets count.
-  When the MQTT connection ends, it will prompt the message ``+MQTTDISCONNECTED:<LinkID>``.
-  When the MQTT connection established, it will prompt the message ``+MQTTCONNECTED:<LinkID>,<scheme>,<"host">,port,<"path">,<reconnect>``.
