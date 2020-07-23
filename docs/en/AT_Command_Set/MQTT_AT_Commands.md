<a name="MQTT-AT"></a>
## MQTT AT Commands

 * [AT+MQTTUSERCFG](#cmd-MQTTUSERCFG) : Set MQTT User Config
 * [AT+MQTTCLIENTID](#cmd-MQTTCLIENTID) : Set MQTT Client ID
 * [AT+MQTTUSERNAME](#cmd-MQTTUSERNAME) : Set MQTT Username
 * [AT+MQTTPASSWORD](#cmd-MQTTPASSWORD) : Set MQTT Password
 * [AT+MQTTCONNCFG](#cmd-MQTTCONNCFG) : Set configuration of MQTT Connection
 * [AT+MQTTCONN](#cmd-MQTTCONN) : Connect to MQTT Broker
 * [AT+MQTTPUB](#cmd-MQTTPUB) : Publish MQTT Data in string
 * [AT+MQTTPUBRAW](#cmd-MQTTPUBRAW) : Publish MQTT message in binary
 * [AT+MQTTSUB](#cmd-MQTTSUB) : Subscribe to MQTT Topic
 * [AT+MQTTUNSUB](#cmd-MQTTUNSUB) : Unsubscribe from MQTT Topic
 * [AT+MQTTCLEAN](#cmd-MQTTCLEAN) : Close the MQTT Connection
 * [MQTT Error Codes](#MQTTErrCod)
 * [MQTT Notes](#MQTTNote)
 
<a name="cmd-MQTTUSERCFG"></a>
### [AT+MQTTUSERCFG](#MQTT-AT)  - Set MQTT User Config
**Set Command:**  
  
    AT+MQTTUSERCFG=<LinkID>,<scheme>,<"client_id">,<"username">,<"password">,<cert_key_ID>,<CA_ID>,<"path">

**Function:**  

    Set MQTT User Config  

**Response:**    

    OK 

**Query Command:**  

```
AT+MQTTUSERCFG?
```
**Function:**  

    Get the MQTT user configuration.  

**Response:**  

```
+MQTTUSERCFG:<LinkID>,<scheme>,<"client_id">,<"username">,<"password">,<cert_key_ID>,<CA_ID>,<"path">
OK
```

**Parameters:**  

- **\<LinkID>**: only supports link ID 0 for now 
- **\<scheme>**:
  - 1: `MQTT over TCP`
  - 2: `MQTT over TLS(no certificate verify)`
  - 3: `MQTT over TLS(verify server certificate)`
  - 4: `MQTT over TLS(provide client certificate)`
  - 5: `MQTT over TLS(verify server certificate and provide client certificate)`
  - 6: `MQTT over WebSocket(based on TCP)`
  - 7: `MQTT over WebSocket Secure(based on TLS, no certificate verify)`
  - 8: `MQTT over WebSocket Secure(based on TLS, verify server certificate)`
  - 9: `MQTT over WebSocket Secure(based on TLS, provide client certificate)`
  - 10: `MQTT over WebSocket Secure(based on TLS, verify server certificate and provide client certificate)`
- **\<client_id>**: MQTT client ID, max length 256Bytes
- **\<username>**: the user name to login to the MQTT broker, max length 64Bytes
- **\<password>**: the password to login to the MQTT broker, max length 64Bytes
- **\<cert\_key_ID>**: certificate ID, only supports one certificate of ID 0 for now
- **\<CA_ID>**: CA ID, only supports one CA of ID 0 for now
- **\<path>**: path of the resource, max length 32Bytes

**Note:**  

- The total length of the entire AT command should be less than 256Bytes.

<a name="cmd-MQTTCLIENTID"></a>
### [AT+MQTTCLIENTID](#MQTT-AT)  - Set MQTT Client ID
**Set Command:**  
  
    AT+MQTTCLIENTID=<LinkID><"client_id">

**Function:**  

    Set MQTT Client ID, will cover the parameter client_id in AT+MQTTUSERCFG   
    User can set a long client id by AT+MQTTCLIENTID.   

**Response:**    

    OK 

**Query Command:**  

```
AT+MQTTCLIENTID?
```
**Function:**  

    Get the MQTT client ID.  

**Response:**  

```
+MQTTCLIENTID:<LinkID>,<"client_id">
OK
```

**Parameters:**  

- **\<LinkID>**: only supports link ID 0 for now 
- **\<client_id>**: MQTT client ID, max length 256Bytes

**Note:**  

- The total length of the entire AT command should be less than 256Bytes.
- AT+MQTTCLIENTID command only could be set after AT+MQTTUSERCFG command

<a name="cmd-MQTTUSERNAME"></a>
### [AT+MQTTUSERNAME](#MQTT-AT)  - Set MQTT Username
**Set Command:**  
  
    AT+MQTTUSERNAME=<LinkID><"username">

**Function:**  

    Set MQTT Username, will cover the parameter username in AT+MQTTUSERCFG   
    User can set a long username by AT+MQTTUSERNAME.     

**Response:**    

    OK 

**Query Command:**  

```
AT+MQTTUSERNAME?
```
**Function:**  

    Get the MQTT client username.  

**Response:**  

```
+MQTTUSERNAME:<LinkID>,<"username">
OK
```

**Parameters:**  

- **\<LinkID>**: only supports link ID 0 for now 
- **\<username>**: the user name to login to the MQTT broker, max length 256Bytes

**Note:**  

- The total length of the entire AT command should be less than 256Bytes.
- AT+MQTTUSERNAME command only could be set after AT+MQTTUSERCFG command

<a name="cmd-MQTTPASSWORD"></a>
### [AT+MQTTPASSWORD](#MQTT-AT)  - Set MQTT Password
**Set Command:**  
  
    AT+MQTTPASSWORD=<LinkID><"password">

**Function:**  

    Set MQTT Password, will cover the parameter password in AT+MQTTUSERCFG   
    User can set a long password by AT+MQTTPASSWORD.  

**Response:**    

    OK 

**Query Command:**  

```
AT+MQTTPASSWORD?
```
**Function:**  

    Get the MQTT client password.  

**Response:**  

```
+MQTTPASSWORD:<LinkID>,<"password">
OK
```

**Parameters:**  

- **\<LinkID>**: only supports link ID 0 for now 
- **\<password>**: the password to login to the MQTT broker, max length 256Bytes

**Note:**  

- The total length of the entire AT command should be less than 256Bytes.
- AT+MQTTPASSWORD command only could be set after AT+MQTTUSERCFG command
<a name="cmd-MQTTCONNCFG"></a>
### [AT+MQTTCONNCFG](#MQTT-AT)  - Set configuration of MQTT Connection
**Set Command:**

```
AT+MQTTCONNCFG=<LinkID>,<keepalive>,<disable_clean_session>,<"lwt_topic">,<"lwt_msg">,<lwt_qos>,<lwt_retain>
```

**Function:**  

    Set configuration of MQTT Connection 

**Response:**  

    OK

**Query Command:**  

```
AT+MQTTCONNCFG?
```
**Function:**  

    Get configuration of MQTT Connection  

**Response:**  

```
+MQTTCONNCFG:<LinkID>,<keepalive>,<disable_clean_session>,<"lwt_topic">,<"lwt_msg">,<lwt_qos>,<lwt_retain>
OK
```

**Parameters:**  
  
- **\<LinkID>**: only supports link ID 0 for now
- **\<keepalive>**: timeout of MQTT ping, range [0, 7200], unit:second. 0 means default, default is 120s.
- **\<disable\_clean_session>**: set MQTT clean session
    - 0: enable clean session
    - 1: disable clean session
- **\<lwt_topic>**: LWT (Last Will and Testament) message topic, max length 64Bytes
- **\<lwt_msg>**: LWT message, max length 64Bytes
- **\<lwt_qos>**: LWT QoS, can be set to 0, or 1, or 2. Default is 0.
- **\<lwt_retain>**: LWT retain, can be set to 0 or 1. Default is 0.

<a name="cmd-MQTTCONN"></a>
### [AT+MQTTCONN](#MQTT-AT)  - Connect to MQTT Broker

**Set Command:**  

```
AT+MQTTCONN=<LinkID>,<"host">,<port>,<reconnect>
```

**Function:**    
  
    Connect to a MQTT broker.  

**Response:**    

```
OK
```

**Query Command:**  

```
AT+MQTTCONN?
```
**Function:**  

    Get the MQTT broker that the ESP chip connected to.  

**Response:**  

```
+MQTTCONN:<LinkID>,<state>,<scheme><"host">,<port>,<"path">,<reconnect>
OK
```

**Parameters:**  

- **\<LinkID>**: only supports link ID 0 for now  
- **\<host>**: MQTT broker domain, max length 128Bytes  
- **\<port>**: MQTT broker port, max is port 65535  
- **\<path>**: path, max length 32Bytes
- **\<reconnect>**: 
  - 0: MQTT will not auto-reconnect
  - 1: MQTT will auto-reconnect, it will take more resource
- **\<state>**: MQTT states
  - 0: MQTT uninitialized
  - 1: already set `AT+MQTTUSERCFG`
  - 2: already set `AT+MQTTCONNCFG`
  - 3: connection disconnected
  - 4: connection established
  - 5: connected, but did not subscribe to any topic
  - 6: connected, and subscribed to MQTT topic
- **\<scheme>**:
  - 1: MQTT over TCP`
  - 2: MQTT over TLS(no certificate verify)
  - 3: MQTT over TLS(verify server certificate)
  - 4: MQTT over TLS(provide client certificate)
  - 5: MQTT over TLS(verify server certificate and provide client certificate)`
  - 6: MQTT over WebSocket(based on TCP)
  - 7: MQTT over WebSocket Secure(based on TLS, no certificate verify)
  - 8: MQTT over WebSocket Secure(based on TLS, verify server certificate)
  - 9: MQTT over WebSocket Secure(based on TLS, provide client certificate)
  - 10: MQTT over WebSocket Secure(based on TLS, verify server certificate and provide client certificate)

<a name="cmd-MQTTPUB"></a>
### [AT+MQTTPUB](#MQTT-AT) - Publish MQTT message in string
**Set Command:**  

```
AT+MQTTPUB=<LinkID>,<"topic">,<"data">,<qos>,<retain>
```

**Function:**  

    Publish MQTT message in string to defined topic. If you need to publish message in binary, please use command `AT+MQTTPUBRAW` instead.

**Response:**    

```
OK
```
**Parameters:**  

- **\<LinkID>**: only supports link ID 0 for now
- **\<topic>**: MQTT topic, max length 64Bytes
- **\<data>**: MQTT message in string.  
- **\<qos>**: qos of publish message, can be set to 0, or 1, or 2. Default is 0.
- **\<retain>**: retain flag

**Note:**  

- The total length of the entire AT command should be less than 256Bytes.  
- This command cannot send data `\0`, if you need to send `\0`, please use command `AT+MQTTPUBRAW` instead.

<a name="cmd-MQTTPUBRAW"></a>
### [AT+MQTTPUBRAW](#MQTT-AT) - Publish MQTT message in binary
**Set Command:**   
```
AT+MQTTPUBRAW=<LinkID>,<"topic">,<length>,<qos>,<retain>
```

**Function:**  

    Publish MQTT message in binary to defined topic.

**Response:**  

```
OK
> 
```
Wrap return > after the Set Command. Begin receiving serial data. The AT firmware will keep waiting until the data length defined by <length> is met, all data received will be considered as the MQTT publish message. When the data <length> is met, the transmission of data starts.
And then it will respond as the following message.  

```
+MQTTPUB:FAIL    
```
Or  

```
+MQTTPUB:OK  
```
  
**Parameters:**  

- **\<LinkID>**: only supports link ID 0 for now
- **\<topic>**: MQTT topic, max length 64Bytes
- **\<length>**: length of MQTT message, max length depends on platform.
  - max length on ESP32 is limited by available memory
  - max length on ESP8266 is limited by marco `MQTT_BUFFER_SIZE_BYTE` and available memory  
  > Default value of `MQTT_BUFFER_SIZE_BYTE` is `512`  
  > Users can change the max length limitation by setting `MQTT_BUFFER_SIZE_BYTE` in `make menuconfig`  
  > max publish data length plus the MQTT header length(depends on topic name length) is equal to `MQTT_BUFFER_SIZE_BYTE`  
- **\<qos>**: qos of publish message, can be set to 0, or 1, or 2. Default is 0.  
- **\<retain>**: retain flag

<a name="cmd-MQTTSUB"></a>
### [AT+MQTTSUB](#MQTT-AT) - Subscribe to MQTT Topic
**Set Command:**  
```
AT+MQTTSUB=<LinkID>,<"topic">,<qos>
```

**Function:**  

    Subscribe to defined MQTT topic with defined QoS. It supports subscribing to multiple topics.

**Response:**  

```
OK
```
When received MQTT message of the subscribed topic, it will prompt:
```
+MQTTSUBRECV:<LinkID>,<"topic">,<data_length>,data    
```
  
If the topic has been subscribed before, it will prompt:  
`ALREADY SUBSCRIBE`

**Query Command:**    

```
AT+MQTTSUB?    
```
**Function:**  

    Get all MQTT topics that already subscribed.

**Response:**  

```
+MQTTSUB:<LinkID>,<state>,<"topic1">,<qos>  
+MQTTSUB:<LinkID>,<state>,<"topic2">,<qos>
+MQTTSUB:<LinkID>,<state>,<"topic3">,<qos>
...
OK
```

**Parameters:**  

- **\<LinkID>**: only supports link ID 0 for now  
- **\<state>**: MQTT states  
  - 0: MQTT uninitialized  
  - 1: already set `AT+MQTTUSERCFG`
  - 2: already set `AT+MQTTCONNCFG`
  - 3: connection disconnected
  - 4: connection established
  - 5: connected, but did not subscribe to any topic
  - 6: connected, and subscribed to MQTT topic
- **\<topic>**: the topic that subscribed to
- **\<qos>**: the QoS that subscribed to

<a name="cmd-MQTTUNSUB"></a>
### [AT+MQTTUNSUB](#MQTT-AT) - Unsubscribe from MQTT Topic
**Set Command:**   

```
AT+MQTTUNSUB=<LinkID>,<"topic">   
```

**Function:**    
 
     Unsubscribe the client from defined topic. This command can be called multiple times to unsubscribe from differrent topics.

**Response:**  

```
OK
```

**Parameters:**  

- **\<LinkID>**: only supports link ID 0 for now
- **\<topic>**: MQTT topic, max length 64Bytes

**Note:**  

- If the topic has not been subscribed, then the AT log will prompt `NO UNSUBSCRIBE`. And the AT command will still respond `OK`.

<a name="cmd-MQTTCLEAN"></a>
### [AT+MQTTCLEAN](#MQTT-AT) - Close the MQTT Connection
**Set Command:**  

```
AT+MQTTCLEAN=<LinkID>  
```

**Function:**  

    Close the MQTT connection, and release the resource.

**Response:**   

```
OK
```

**Parameters:**  

- **\<LinkID>**: only supports link ID 0 for now

<a name="MQTTErrCod"></a>
### [MQTT Error Codes](#MQTT-AT)
The MQTT Error code will be prompt as `ERR CODE:0x<%08x>`.

```
    AT_MQTT_NO_CONFIGURED,                              // 0x6001
    AT_MQTT_NOT_IN_CONFIGURED_STATE,                    // 0x6002
    AT_MQTT_UNINITIATED_OR_ALREADY_CLEAN,               // 0x6003
    AT_MQTT_ALREADY_CONNECTED,                          // 0x6004
    AT_MQTT_MALLOC_FAILED,                              // 0x6005
    AT_MQTT_NULL_LINK,                                  // 0x6006
    AT_MQTT_NULL_PARAMTER,                              // 0x6007
    AT_MQTT_PARAMETER_COUNTS_IS_WRONG,                  // 0x6008
    AT_MQTT_TLS_CONFIG_ERROR,                           // 0x6009
    AT_MQTT_PARAM_PREPARE_ERROR,                        // 0x600A
    AT_MQTT_CLIENT_START_FAILED,                        // 0x600B
    AT_MQTT_CLIENT_PUBLISH_FAILED,                      // 0x600C
    AT_MQTT_CLIENT_SUBSCRIBE_FAILED,                    // 0x600D
    AT_MQTT_CLIENT_UNSUBSCRIBE_FAILED,                  // 0x600E
    AT_MQTT_CLIENT_DISCONNECT_FAILED,                   // 0x600F
    AT_MQTT_LINK_ID_READ_FAILED,                        // 0x6010
    AT_MQTT_LINK_ID_VALUE_IS_WRONG,                     // 0x6011
    AT_MQTT_SCHEME_READ_FAILED,                         // 0x6012
    AT_MQTT_SCHEME_VALUE_IS_WRONG,                      // 0x6013
    AT_MQTT_CLIENT_ID_READ_FAILED,                      // 0x6014
    AT_MQTT_CLIENT_ID_IS_NULL,                          // 0x6015
    AT_MQTT_CLIENT_ID_IS_OVERLENGTH,                    // 0x6016
    AT_MQTT_USERNAME_READ_FAILED,                       // 0x6017
    AT_MQTT_USERNAME_IS_NULL,                           // 0x6018
    AT_MQTT_USERNAME_IS_OVERLENGTH,                     // 0x6019
    AT_MQTT_PASSWORD_READ_FAILED,                       // 0x601A
    AT_MQTT_PASSWORD_IS_NULL,                           // 0x601B
    AT_MQTT_PASSWORD_IS_OVERLENGTH,                     // 0x601C
    AT_MQTT_CERT_KEY_ID_READ_FAILED,                    // 0x601D
    AT_MQTT_CERT_KEY_ID_VALUE_IS_WRONG,                 // 0x601E
    AT_MQTT_CA_ID_READ_FAILED,                          // 0x601F
    AT_MQTT_CA_ID_VALUE_IS_WRONG,                       // 0x6020
    AT_MQTT_CA_LENGTH_ERROR,                            // 0x6021
    AT_MQTT_CA_READ_FAILED,                             // 0x6022
    AT_MQTT_CERT_LENGTH_ERROR,                          // 0x6023
    AT_MQTT_CERT_READ_FAILED,                           // 0x6024
    AT_MQTT_KEY_LENGTH_ERROR,                           // 0x6025
    AT_MQTT_KEY_READ_FAILED,                            // 0x6026
    AT_MQTT_PATH_READ_FAILED,                           // 0x6027
    AT_MQTT_PATH_IS_NULL,                               // 0x6028
    AT_MQTT_PATH_IS_OVERLENGTH,                         // 0x6029
    AT_MQTT_VERSION_READ_FAILED,                        // 0x602A
    AT_MQTT_KEEPALIVE_READ_FAILED,                      // 0x602B
    AT_MQTT_KEEPALIVE_IS_NULL,                          // 0x602C
    AT_MQTT_KEEPALIVE_VALUE_IS_WRONG,                   // 0x602D
    AT_MQTT_DISABLE_CLEAN_SESSION_READ_FAILED,          // 0x602E
    AT_MQTT_DISABLE_CLEAN_SESSION_VALUE_IS_WRONG,       // 0x602F
    AT_MQTT_LWT_TOPIC_READ_FAILED,                      // 0x6030
    AT_MQTT_LWT_TOPIC_IS_NULL,                          // 0x6031
    AT_MQTT_LWT_TOPIC_IS_OVERLENGTH,                    // 0x6032
    AT_MQTT_LWT_MSG_READ_FAILED,                        // 0x6033
    AT_MQTT_LWT_MSG_IS_NULL,                            // 0x6034
    AT_MQTT_LWT_MSG_IS_OVERLENGTH,                      // 0x6035
    AT_MQTT_LWT_QOS_READ_FAILED,                        // 0x6036
    AT_MQTT_LWT_QOS_VALUE_IS_WRONG,                     // 0x6037
    AT_MQTT_LWT_RETAIN_READ_FAILED,                     // 0x6038
    AT_MQTT_LWT_RETAIN_VALUE_IS_WRONG,                  // 0x6039
    AT_MQTT_HOST_READ_FAILED,                           // 0x603A
    AT_MQTT_HOST_IS_NULL,                               // 0x603B
    AT_MQTT_HOST_IS_OVERLENGTH,                         // 0x603C
    AT_MQTT_PORT_READ_FAILED,                           // 0x603D
    AT_MQTT_PORT_VALUE_IS_WRONG,                        // 0x603E
    AT_MQTT_RECONNECT_READ_FAILED,                      // 0x603F
    AT_MQTT_RECONNECT_VALUE_IS_WRONG,                   // 0x6040
    AT_MQTT_TOPIC_READ_FAILED,                          // 0x6041
    AT_MQTT_TOPIC_IS_NULL,                              // 0x6042
    AT_MQTT_TOPIC_IS_OVERLENGTH,                        // 0x6043
    AT_MQTT_DATA_READ_FAILED,                           // 0x6044
    AT_MQTT_DATA_IS_NULL,                               // 0x6045
    AT_MQTT_DATA_IS_OVERLENGTH,                         // 0x6046
    AT_MQTT_QOS_READ_FAILED,                            // 0x6047
    AT_MQTT_QOS_VALUE_IS_WRONG,                         // 0x6048
    AT_MQTT_RETAIN_READ_FAILED,                         // 0x6049
    AT_MQTT_RETAIN_VALUE_IS_WRONG,                      // 0x604A
    AT_MQTT_PUBLISH_LENGTH_READ_FAILED,                 // 0x604B
    AT_MQTT_PUBLISH_LENGTH_VALUE_IS_WRONG,              // 0x604C
    AT_MQTT_RECV_LENGTH_IS_WRONG,                       // 0x604D
    AT_MQTT_CREATE_SEMA_FAILED,                         // 0x604E
    AT_MQTT_CREATE_EVENT_GROUP_FAILED,                  // 0x604F
    AT_MQTT_URI_PARSE_FAILED,                           // 0x6050
    AT_MQTT_IN_DISCONNECTED_STATE,                      // 0x6051
```

<a name="MQTTNote"></a>
### [MQTT Notes](#MQTT-AT)
- In general, AT MQTT commands will be responded within 10s, except command `AT+MQTTCONN`.For example, if the router fails to access to the internet, the command `AT+MQTTPUB` will respond within 10s. But the command `AT+MQTTCONN` may need more time due to the packet retransmission in bad network environment.
- If the `AT+MQTTCONN` is based on a TLS connection, the timeout of each packet is 10s, then the total timeout will be much longer depending on the handshake packets count. 
- When the MQTT connection ends, it will prompt message `+MQTTDISCONNECTED:<LinkID>`
- When the MQTT connection established, it will prompt message `+MQTTCONNECTED:<LinkID>,<scheme>,<"host">,port,<"path">,<reconnect>`

