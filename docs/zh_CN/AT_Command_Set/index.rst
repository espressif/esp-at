**************
AT 命令集
**************

{IDF_TARGET_VER: default="undefined", esp32="5.0", esp32c2="5.0", esp32c3="5.0", esp32c6="5.1", esp32s2="5.0"}

:link_to_translation:`en:[English]`

本章将具体介绍如何使用各类 AT 命令。

.. toctree::
   :maxdepth: 1

   基础 AT 命令集 <Basic_AT_Commands>
   Wi-Fi AT 命令集 <Wi-Fi_AT_Commands>
   TCP-IP AT 命令集 <TCP-IP_AT_Commands>
   :esp32 or esp32c2 or esp32c3 or esp32c6: Bluetooth® Low Energy 命令集 <BLE_AT_Commands>
   :esp32: Bluetooth® AT 命令集 <BT_AT_Commands>
   MQTT AT 命令集 <MQTT_AT_Commands>
   HTTP AT 命令集 <HTTP_AT_Commands>
   文件系统 AT 命令集 <FS_AT_Commands>
   WebSocket AT 命令集 <websocket_at_commands>
   :esp32: 以太网 AT 命令集 <Ethernet_AT_Commands>
   信令测试 AT 命令集 <Signaling_Test_AT_Commands>
   驱动 AT 命令集 <Driver_AT_Commands>
   Web 服务器 AT 命令集 <Web_server_AT_Commands>
   用户 AT 命令集 <user_at_commands>

强烈建议在使用命令之前先阅读以下内容，了解 AT 命令的一些基本信息。  

.. contents::
   :local:
   :depth: 1

.. _at-command-types:

AT 命令分类
================

通用 AT 命令有四种类型：

.. list-table::
   :header-rows: 1
   :widths: 20 30 55

   * - 类型 
     - 命令格式 
     - 说明
   * - 测试命令
     - AT+<命令名称>=?
     - 查询设置命令的内部参数及其取值范围
   * - 查询命令
     - AT+<命令名称>?
     - 返回当前参数值
   * - 设置命令
     - AT+<命令名称>=<...> 
     - 设置用户自定义的参数值，并运行命令
   * - 执行命令
     - AT+<命令名称>
     - 运行无用户自定义参数的命令

-  不是每条 AT 命令都具备上述四种类型的命令。
-  命令里输入参数，当前只支持字符串参数和整形数字参数。
-  尖括号 < > 内的参数不可以省略。
-  方括号 [ ] 内的参数可以省略，省略时使用默认值。例如，运行 :ref:`AT+CWJAP <cmd-JAP>` 命令时省略某些参数：
   ::

         AT+CWJAP="ssid","password"
         AT+CWJAP="ssid","password","11:22:33:44:55:66"

-  当省略的参数后仍有参数要填写时，必须使用 ``,``，以示分隔，如：
   ::
   
      AT+CWJAP="ssid","password",,1

-  使用双引号表示字符串参数，如：
   ::

      AT+CWSAP="ESP756290","21030826",1,4

-  特殊字符需作转义处理，当前需要转义的字符有 ``,``、``"``、``\``。

   -  ``\\``：转义反斜杠。
   -  ``\,``：转义逗号，分隔参数的逗号无需转义。
   -  ``\"``：转义双引号，表示字符串参数的双引号无需转义。
   -  ``\<any>``：转义 ``<any>`` 字符，即只使用 ``<any>`` 字符，不使用反斜杠。

-  只有 AT **命令** 中的特殊字符需要转义，其它地方无需转义。例如，AT 命令口打印 ``>`` 等待输入数据时，该数据不需要转义。
   ::
      
      AT+CWJAP="comma\,backslash\\ssid","1234567890"
      AT+MQTTPUB=0,"topic","\"{\"sensor\":012}\"",1,0

-  AT 命令的默认波特率为 115200。
-  每条 AT 命令的长度不应超过 256 字节。
-  AT 命令以新行 (CR-LF) 结束，所以串口工具应设置为“新行模式”。
-  AT 命令错误代码的定义请见 :doc:`../Compile_and_Develop/AT_API_Reference`：

   - :cpp:type:`esp_at_error_code`
   - :cpp:type:`esp_at_para_parse_result_type`
   - :cpp:type:`esp_at_result_code_string_index`

参数信息保存在 flash 中的 AT 命令
=================================================

以下 AT 命令的参数更改将始终保存在 flash 的 NVS 区域中，因此重启后，会直接使用。

-  :ref:`AT+UART_DEF <cmd-UARTD>`: ``AT+UART_DEF=115200,8,1,0,3``
-  :ref:`AT+SAVETRANSLINK <cmd-SAVET>`: ``AT+SAVETRANSLINK=1,"192.168.6.10",1001``
-  :ref:`AT+CWAUTOCONN <cmd-AUTOC>`: ``AT+CWAUTOCONN=1``

其它一些命令的参数更改是否保存到 flash 可以通过 :ref:`AT+SYSSTORE <cmd-SYSSTORE>` 命令来配置，具体请参见命令的详细说明。

.. note::
  AT 命令里的参数保存，是通过 `NVS <https://docs.espressif.com/projects/esp-idf/zh_CN/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-reference/storage/nvs_flash.html>`_ 库实现的。因此，如果命令配置相同的参数值，则不会写入 flash；如果命令配置不同的参数值，flash 也不会被频繁擦除。

.. _at-messages:

AT 消息
===========

从 ESP-AT 命令端口返回的 ESP-AT 消息有两种类型：ESP-AT 响应（被动）和 ESP-AT 消息报告（主动）。

.. _at-messages-response:

- ESP-AT 响应（被动）
  
  每个输入的 ESP-AT 命令都会返回响应，告诉发送者 ESP-AT 命令的执行结果。响应的最后一条消息必然是 ``OK`` 或者 ``ERROR``。

  .. list-table:: ESP-AT 响应
      :header-rows: 1
      :widths: 40 60
  
      * - AT 响应
        - 说明
      * - OK
        - AT 命令处理完毕，返回 OK
      * - ERROR
        - AT 命令错误或执行过程中发生错误
      * - SEND OK
        - 数据已发送到协议栈（针对于 :ref:`AT+CIPSEND <cmd-SEND>` 和 :ref:`AT+CIPSENDEX <cmd-SENDEX>` 命令），但不代表数据已经发到对端
      * - SEND FAIL
        - 向协议栈发送数据时发生错误（针对于 :ref:`AT+CIPSEND <cmd-SEND>` 和 :ref:`AT+CIPSENDEX <cmd-SENDEX>` 命令）
      * - SET OK
        - URL 已经成功设置（针对于 :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` 命令）
      * - +<Command Name>:``...``
        - 详细描述 AT 命令处理结果

.. _at-messages-report:

- ESP-AT 消息报告（主动）

  ESP-AT 会报告系统中重要的状态变化或消息。

  .. list-table:: ESP-AT 消息报告
     :header-rows: 1
     :widths: 60 60
  
     * - ESP-AT 消息报告
       - 说明
     * - ready
       - ESP-AT 固件已经准备就绪
     * - busy p...
       - 系统繁忙，正在处理上一条命令，无法处理新的命令
     * - ERR CODE:``<0x%08x>``
       - 不同命令的错误代码
     * - Will force to restart!!!
       - 立即重启模块
     * - smartconfig type:``<xxx>``
       - Smartconfig 类型
     * - Smart get wifi info
       - Smartconfig 已获取 SSID 和 PASSWORD
     * - +SCRD:``<length>``,``<reserved data>``
       - ESP-Touch v2 已获取自定义数据
     * - smartconfig connected wifi
       - Smartconfig 完成，ESP-AT 已连接到 Wi-Fi
     * - WIFI CONNECTED
       - Wi-Fi station 接口已连接到 AP
     * - WIFI GOT IP
       - Wi-Fi station 接口已获取 IPv4 地址
     * - WIFI GOT IPv6 LL
       - Wi-Fi station 接口已获取 IPv6 链路本地地址
     * - WIFI GOT IPv6 GL
       - Wi-Fi station 接口已获取 IPv6 全局地址
     * - WIFI DISCONNECT
       - Wi-Fi station 接口已与 AP 断开连接
     * - +ETH_CONNECTED
       - 以太网接口已连接
     * - +ETH_GOT_IP
       - 以太网接口已获取 IPv4 地址
     * - +ETH_DISCONNECTED
       - 以太网接口已断开
     * - [<conn_id>,]CONNECT
       - ID 为 ``<conn_id>`` 的网络连接已建立（默认情况下，ID 为 0）
     * - [<conn_id>,]CLOSED
       - ID 为 ``<conn_id>`` 的网络连接已断开（默认情况下，ID 为 0）
     * - +LINK_CONN
       - TCP/UDP/SSL 连接的详细信息
     * - +STA_CONNECTED: <sta_mac>
       - station 已连接到 ESP-AT 的 Wi-Fi softAP 接口
     * - +DIST_STA_IP: <sta_mac>,<sta_ip>
       - ESP-AT 的 Wi-Fi softAP 接口给 station 分配 IP 地址
     * - +STA_DISCONNECTED: <sta_mac> 
       - station 与 ESP-AT 的 Wi-Fi softAP 接口的连接断开
     * - >
       - ESP-AT 正在等待用户输入数据
     * - Recv ``<xxx>`` bytes
       - ESP-AT 从命令端口已接收到 ``<xxx>`` 字节
     * - +IPD
       - ESP-AT 在非透传模式下，已收到来自网络的数据。有以下的消息格式：

         - 如果 AT+CIPMUX=0，AT+CIPRECVTYPE=1，打印：``+IPD,<length>``
         - 如果 AT+CIPMUX=1，AT+CIPRECVTYPE=<link_id>,1，打印：``+IPD,<link_id>,<length>``
         - 如果 AT+CIPMUX=0，AT+CIPRECVTYPE=0，AT+CIPDINFO=0，打印：``+IPD,<length>:<data>``
         - 如果 AT+CIPMUX=1，AT+CIPRECVTYPE=<link_id>,0，AT+CIPDINFO=0，打印：``+IPD,<link_id>,<length>:<data>``
         - 如果 AT+CIPMUX=0，AT+CIPRECVTYPE=0，AT+CIPDINFO=1，打印：``+IPD,<length>,<"remote_ip">,<remote_port>:<data>``
         - 如果 AT+CIPMUX=1，AT+CIPRECVTYPE=<link_id>,0，AT+CIPDINFO=1，打印：``+IPD,<link_id>,<length>,<"remote_ip">,<remote_port>:<data>``

         其中的 ``link_id`` 为连接 ID，``length`` 为数据长度，``remote_ip`` 为远端 IP 地址，``remote_port`` 为远端端口号，``data`` 为数据。

         注意：当这是个 SSL 连接时，在被动接收模式下（AT+CIPRECVTYPE=1），AT 命令口回复的 ``length`` 可能和实际可读的 SSL 数据长度不一致。因为 AT 会优先返回 SSL 层可读的数据长度，如果 SSL 层可读的数据长度为 0，AT 会返回套接字层可读的数据长度。

     * - :term:`透传模式` 下的数据
       - ESP-AT 在透传模式下，已收到来自网络或蓝牙的数据
     * - SEND Canceled
       - 取消在 Wi-Fi :term:`普通传输模式` 下发送数据
     * - Have ``<xxx>`` Connections
       - 已达到服务器的最大连接数
     * - +QUITT
       - ESP-AT 退出 Wi-Fi :term:`透传模式`
     * - NO CERT FOUND
       - 在自定义分区中没有找到有效的设备证书
     * - NO PRVT_KEY FOUND
       - 在自定义分区中没有找到有效的私钥
     * - NO CA FOUND
       - 在自定义分区中没有找到有效的 CA 证书
     * - +TIME_UPDATED
       - 系统时间已更新。只在发送 :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>` 命令后或者掉电重启后，系统从 SNTP 服务器获取到新的时间，才会打印此消息。
     * - +MQTTCONNECTED
       - MQTT 已连接到 broker
     * - +MQTTDISCONNECTED
       - MQTT 与 broker 已断开连接
     * - +MQTTSUBRECV
       - MQTT 已从 broker 收到数据
     * - +MQTTPUB:FAIL
       - MQTT 发布数据失败
     * - +MQTTPUB:OK
       - MQTT 发布数据完成
     * - +BLECONN
       - Bluetooth LE 连接已建立
     * - +BLEDISCONN 
       - Bluetooth LE 连接已断开
     * - +READ
       - 通过 Bluetooth LE 连接进行读取操作
     * - +WRITE
       - 通过 Bluetooth LE 进行写入操作
     * - +NOTIFY
       - 来自 Bluetooth LE 连接的 notification
     * - +INDICATE
       - 来自 Bluetooth LE 连接的 indication
     * - +BLESECNTFYKEY
       - Bluetooth LE SMP 密钥
     * - +BLESECREQ:<conn_index>
       - 收到来自 Bluetooth LE 连接的加密配对请求
     * - +BLEAUTHCMPL:<conn_index>,<enc_result>
       - Bluetooth LE SMP 配对完成
     * - +BLUFIDATA:<len>,<data>
       - ESP 设备收到从手机端发送的 BluFi 用户自定义数据
     * - +WS_DISCONNECTED:<link_id>
       - 连接 ID 为 <link_id> 的 WebSocket 连接已断开
     * - +WS_CONNECTED:<link_id>
       - 连接 ID 为 <link_id> 的 WebSocket 连接已建立
     * - +WS_DATA:<link_id>,<data_len>,<data>
       - 连接 ID 为 <link_id> 的 WebSocket 连接收到数据
     * - +WS_CLOSED:<link_id>
       - 连接 ID 为 <link_id> 的 WebSocket 连接已关闭
     * - +BLESCANDONE
       - 扫描结束
     * - +BLESECKEYREQ:<conn_index>
       - 对端已经接受配对请求，ESP 设备可以输入密钥了

  .. only:: esp32c3

    如果使用第三方云命令，ESP-AT 会在系统中报告云重要状态变化或消息。

    .. list-table:: ESP-AT 第三方云消息报告
      :header-rows: 1
      :widths: 60 60

      * - ESP-AT 消息报告
        - 说明
      * - RainMaker AT 消息
        - 请参考 :ref:`ESP-AT RainMaker 消息报告（主动） <rm-at-messages>`
