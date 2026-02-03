.. _WS-AT:

WebSocket AT 命令集
===================

:link_to_translation:`en:[English]`

- :ref:`介绍 <cmd-ws-intro>`
- :ref:`AT+WSCFG <cmd-WSCFG>`：配置 WebSocket 参数
- :ref:`AT+WSHEAD <cmd-WSHEAD>`：设置/查询 WebSocket 请求头
- :ref:`AT+WSOPEN <cmd-WSOPEN>`：查询/打开 WebSocket 连接
- :ref:`AT+WSSEND <cmd-WSSEND>`：向 WebSocket 连接发送数据
- :ref:`AT+WSDATAFMT <cmd-WSDATAFMT>`：设置 WebSocket 接收数据格式
- :ref:`AT+WSCLOSE <cmd-WSCLOSE>`：关闭 WebSocket 连接
- :ref:`WebSocket 示例 <example-websocket>`

.. _cmd-ws-intro:

介绍
------

.. important::
  默认的 AT 固件不支持此页面下的 AT 命令。如果您需要 {IDF_TARGET_NAME} 支持 WebSocket 命令，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：

  - 启用 ``Component config`` > ``AT`` > ``AT WebSocket command support``

.. _cmd-WSCFG:

:ref:`AT+WSCFG <WS-AT>`：配置 WebSocket 参数
------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+WSCFG=<link_id>,<ping_intv_sec>,<ping_timeout_sec>[,<buffer_size>][,<auth_mode>,<pki_number>,<ca_number>]

**响应：**

::

    OK

或

::

    ERROR

参数
^^^^

- **<link_id>**：WebSocket 连接 ID。范围：[0,2]，即最大支持三个 WebSocket 连接。
- **<ping_intv_sec>**：发送 WebSocket Ping 间隔。单位：秒。范围：[1,7200]。默认值：10，即：每隔 10 秒发送一次 WebSocket Ping 包。
- **<ping_timeout_sec>**：WebSocket Ping 超时。单位：秒。范围：[1,7200]。默认值：120，即：120 秒未收到 WebSocket Pong 包，则关闭连接。
- **<buffer_size>**：WebSocket 缓冲区大小。单位：字节。范围：[1,16384]。默认值：1024。
- **<auth_mode>**:

  - 0: 不认证，此时无需填写 ``<pki_number>`` 和 ``<ca_number>`` 参数；
  - 1: ESP-AT 提供客户端证书供服务器端 CA 证书校验；
  - 2: ESP-AT 客户端载入 CA 证书来校验服务器端的证书；
  - 3: 相互认证。

- **<pki_number>**：证书和私钥的索引，如果只有一个证书和私钥，其值应为 0。
- **<ca_number>**：CA 的索引，如果只有一个 CA，其值应为 0。

说明
^^^^
- 此命令应在 :ref:`AT+WSOPEN <cmd-WSOPEN>` 之前配置，否则不会生效。
- 如果您想使用自己的证书，运行时请使用 :ref:`AT+SYSMFG <cmd-SYSMFG>` 命令更新 WebSocket 证书。如果您想预烧录自己的证书，请参考 :doc:`../Compile_and_Develop/How_to_update_pki_config`。
- 如果 ``<auth_mode>`` 配置为 2 或者 3，为了校验服务器的证书有效期，请在发送 :ref:`AT+WSOPEN <cmd-WSOPEN>` 命令前确保 {IDF_TARGET_NAME} 已获取到当前时间。（您可以发送 :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>` 命令来配置 SNTP，获取当前时间，发送 :ref:`AT+CIPSNTPTIME? <cmd-SNTPT>` 命令查询当前时间。）
- 相互认证的示例： :ref:`基于 TLS 的 WebSocket 连接（相互鉴权）<example-websocket-tls>`。

示例
^^^^

::

    // 配置 link_id 为 0 的 WebSocket 连接的 Ping 发送间隔为 30 秒，超时 60 秒，缓冲区 4096 字节
    AT+WSCFG=0,30,60,4096

.. _cmd-WSHEAD:

:ref:`AT+WSHEAD <HTTP-AT>`：设置/查询 WebSocket 请求头
----------------------------------------------------------

查询命令
^^^^^^^^^^^^^

**命令：**

::

    AT+WSHEAD?

**响应：**

::

    +WSHEAD:<index>,<"req_header">

    OK

设置命令
^^^^^^^^^^^

**命令：**

::

    AT+WSHEAD=<req_header_len>

**响应：**

::

    OK

    >

符号 ``>`` 表示 AT 准备好接收 AT 命令口数据，此时您可以输入 WebSocket 请求头（请求头为 ``key: value`` 形式，无需以 ``\r\n`` 结尾），当数据长度达到参数 ``<req_header_len>`` 的值时，AT 返回：

::

    OK

参数
^^^^^^^^^^
- **<index>**：WebSocket 请求头的索引值。
- **<"req_header">**：WebSocket 请求头。
- **<req_header_len>**：WebSocket 请求头长度。单位：字节。

  - 0：清除所有已设置的 WebSocket 请求头。
  - 其他值：设置一个新的 WebSocket 请求头。

说明
^^^^^

- 本命令一次只能设置一个 WebSocket 请求头，但可以多次设置，支持多个不同的 WebSocket 请求头。
- 本命令配置的 WebSocket 请求头是全局性的，一旦设置，所有 WebSocket 的命令都会携带这些请求头。

示例
^^^^

::

    // 设置请求头
    AT+WSHEAD=49

    // 在收到 ">" 符号后，输入以下的 authorization 请求头
    AUTHORIZATION: Basic QTIzMzIyMDE5OTk6MTIzNDU2Nzg=

    // 打开一个 WebSocket 连接
    AT+WSOPEN=0,"wss://demo.piesocket.com/v3/channel_123?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_self"

.. _cmd-WSOPEN:

:ref:`AT+WSOPEN <WS-AT>`：查询/打开一个 WebSocket 连接
------------------------------------------------------------

查询命令
^^^^^^^^

**命令：**

::

    AT+WSOPEN?

**响应：**

当有连接时，AT 返回：

::

    +WSOPEN:<link_id>,<state>,<"uri">

    OK

当没有连接时，AT 返回：

::

    OK

设置命令
^^^^^^^^

**命令：**

::

    AT+WSOPEN=<link_id>,<"uri">[,<"subprotocol">][,<timeout_ms>][,<"auth">]

**响应：**

::

    +WS_CONNECTED:<link_id>

    OK

或

::

    ERROR

参数
^^^^

- **<link_id>**：WebSocket 连接 ID。范围：[0,2]，即最大支持三个 WebSocket 连接。
- **<state>**：WebSocket 连接的状态。

   - 0：WebSocket 连接已关闭。
   - 1：WebSocket 连接正在重连。
   - 2：已建立 WebSocket 连接。
   - 3：接收 WebSocket Pong 超时或读取连接数据错误，正在等待重连。
   - 4：已收到服务器端 WebSocket 关闭帧，正在发送关闭帧到服务器。

- **<"uri">**：WebSocket 服务器的统一资源标识符。
- **<"subprotocol">**：WebSocket 子协议（参考 `RFC6455 1.9 章节 <https://www.rfc-editor.org/rfc/rfc6455#section-1.9>`_）。
- **<timeout_ms>**：建立 WebSocket 连接的超时时间。单位：毫秒。范围：[0,180000]。默认值：15000。
- **<"auth">**：WebSocket 鉴权（参考 `RFC6455 4.1.12 章节 <https://www.rfc-editor.org/rfc/rfc6455#section-4.1>`_）。

示例
^^^^

::

    // uri 参数来自于 https://www.piesocket.com/websocket-tester
    AT+WSOPEN=0,"wss://demo.piesocket.com/v3/channel_123?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_self"

详细示例参考： :ref:`WebSocket 示例 <example-websocket>`。

.. _cmd-WSSEND:

:ref:`AT+WSSEND <WS-AT>`：向 WebSocket 连接发送数据
-----------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+WSSEND=<link_id>,<length>[,<opcode>][,<timeout_ms>][,<fin>]

**响应：**

::

    OK

    >

上述响应表示 AT 已准备好从 AT port 接收数据，此时您可以输入数据，当 AT 接收到的数据长度达到 ``<length>`` 后，数据传输开始。

如果未建立连接或数据传输时连接被断开，返回：

::

    ERROR

如果数据传输成功，返回：

::

    SEND OK

参数
^^^^

- **<link_id>**：WebSocket 连接 ID。范围：[0,2]。
- **<length>**：发送的数据长度。单位：字节。最大可发送长度取决于 16 KiB 与系统可分配堆空间大小中的较小者。
- **<opcode>**：发送的 WebSocket 帧中的 opcode。范围：[0,0xF]。默认值：1，即 text 帧。请参考 `RFC6455 5.2 章节 <https://www.rfc-editor.org/rfc/rfc6455#section-5.2>`_ 了解更多的 opcode。

   - 0x0：continuation 帧
   - 0x1：text 帧
   - 0x2：binary 帧
   - 0x3 - 0x7：为其它非控制帧保留
   - 0x8：连接关闭帧
   - 0x9：ping 帧
   - 0xA：pong 帧
   - 0xB - 0xF：为其它控制帧保留

- **<timeout_ms>**：发送超时时间。单位：毫秒。范围：[0,60000]。默认值：10000。
- **<fin>**：表示数据帧是否为最后一个分片。默认值：1。

  - 0：表示数据帧还有后续分片。
  - 1：表示数据帧为最后一个分片。

.. _cmd-WSDATAFMT:

:ref:`AT+WSDATAFMT <WS-AT>`：设置 WebSocket 接收数据格式
-------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

默认情况下，{IDF_TARGET_NAME} 在收到 WebSocket 数据时，会以 ``+WS_DATA:<link_id>,<data_len>,<data>\r\n`` 的格式输出数据。本命令允许您配置接收数据时的元数据输出格式，以满足不同的应用需求。完整的输出格式如下：

::

    +WS_DATA:<link_id>[,<opcode>][,<fin>][,<payload_len>][,<payload_offset>][,<data_len>],<data>[<\r\n>]

**命令：**

::

    AT+WSDATAFMT=<link_id>[,<meta_data_level>][,<meta_data_mask>]

**响应：**

::

    OK

或

::

    ERROR

参数
^^^^

- **<link_id>**：WebSocket 连接 ID。范围：[0,2]。
- **<meta_data_level>**：元数据输出等级，默认值：0。各等级含义如下：

  - 0: chunked 等级。每次从传输层收到数据，都会添加 ``+WS_DATA`` 开头的元数据。
  - 1: frame 等级。仅在完整的 WebSocket 帧前后，会添加 ``+WS_DATA`` 开头的元数据。
  - 2: message 等级。仅在完整的 WebSocket 消息前后，会添加 ``+WS_DATA`` 开头的元数据。

- **<meta_data_mask>**：用于控制 ``+WS_DATA`` 输出格式的位掩码。将相应 bit 设为 1 即可在输出中包含该位所对应的字段。默认值：48（此时仅输出 ``<data_len>`` 和 ``<\r\n>``）。

  - bit 0: ``<opcode>``，WebSocket 帧类型
  - bit 1: ``<fin>``，是否为消息的最后一帧
  - bit 2: ``<payload_len>``，完整负载长度
  - bit 3: ``<payload_offset>``，当前数据在负载中的偏移
  - bit 4: ``<data_len>``，当前输出的数据长度
  - bit 5: ``<\r\n>``，数据末尾的换行符

示例
^^^^

::

    // 设置 link_id 为 0 的 WebSocket 连接，使用 message 等级，仅输出 payload_len。
    AT+WSDATAFMT=0,2,4

.. _cmd-WSCLOSE:

:ref:`AT+WSCLOSE <WS-AT>`：关闭 WebSocket 连接
-----------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+WSCLOSE=<link_id>

**响应：**

::

    OK

参数
^^^^

- **<link_id>**：WebSocket 连接 ID。范围：[0,2]。

说明
^^^^

- 本命令触发 TCP 层的关闭流程，返回 ``OK`` 仅表示关闭指令已传达，不保证对端已完全断开连接。若需确保连接完全关闭，建议在执行本命令前，先使用 :ref:`AT+WSSEND <cmd-WSSEND>` 发送一个 opcode 为 8 (Close Frame) 的 WebSocket 控制帧，等待服务器回应后再关闭。

示例
^^^^

::

    // 关闭 ID 为 0 的 WebSocket 连接
    AT+WSCLOSE=0
