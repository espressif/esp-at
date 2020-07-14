<a name="MQTT-AT"></a>
## MQTT AT Examples
 * [Example 1: MQTT over TCP](#MQTTExamp1)
 * [Example 2: MQTT over TLS](#MQTTExamp2)
 * [Example 3: MQTT over WSS](#MQTTExamp3)

<a name="MQTTExamp1"></a>
### [Example 1: MQTT over TCP](#MQTT-AT) (with a Local MQTT Broker)
Create a local MQTT broker. For example, the MQTT broker's IP address is "192.168.31.113", port 1883. Then the example of communicating with the MQTT broker will be as the following steps.  

```
AT+MQTTUSERCFG=0,1,"ESP32","espressif","1234567890",0,0,""
AT+MQTTCONN=0,"192.168.31.113",1883,0
AT+MQTTSUB=0,"topic",1
AT+MQTTPUB=0,"topic","test",1,0
AT+MQTTCLEAN=0
```

<a name="MQTTExamp2"></a>
### [Example 2: MQTT over TLS](#MQTT-AT) (with a Local MQTT Broker)
Create a local MQTT broker. For example, the MQTT broker's IP address is "192.168.31.113", port 1883. Then the example of communicating with the MQTT broker will be as the following steps.    

```
AT+CIPSNTPCFG=1,8,"ntp1.aliyun.com"
AT+CIPSNTPTIME?
AT+MQTTUSERCFG=0,3,"ESP32","espressif","1234567890",0,0,""
AT+MQTTCONNCFG=0,0,0,"lwtt","lwtm",0,0
AT+MQTTCONN=0,"192.168.31.113",1883,0
AT+MQTTSUB=0,"topic",1
AT+MQTTPUB=0,"topic","test",1,0
AT+MQTTCLEAN=0
```

<a name="MQTTExamp3"></a>
### [Example 3: MQTT over WSS](#MQTT-AT) 
This is an example of communicating with MQTT broker: iot.eclipse.org, of which port is 443.  

```
AT+CIPSNTPCFG=1,8,"ntp1.aliyun.com"
AT+CIPSNTPTIME?
AT+MQTTUSERCFG=0,7,"ESP32","espressif","1234567890",0,0,"wss"
AT+MQTTCONN=0,"iot.eclipse.org",443,0
AT+MQTTSUB=0,"topic",1
AT+MQTTPUB=0,"topic","test",1,0
AT+MQTTCLEAN=0
```
