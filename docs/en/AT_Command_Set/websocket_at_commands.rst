.. _WS-AT:

WebSocket AT Commands
=====================

:link_to_translation:`zh_CN:[中文]`

- :ref:`Introduction <cmd-ws-intro>`
- :ref:`AT+WSCFG <cmd-WSCFG>`: Set the WebSocket configuration.
- :ref:`AT+WSHEAD <cmd-WSHEAD>`: Set/Query WebSocket request headers.
- :ref:`AT+WSOPEN <cmd-WSOPEN>`: Query/Open a WebSocket connection.
- :ref:`AT+WSSEND <cmd-WSSEND>`: Send data to a WebSocket connection.
- :ref:`AT+WSCLOSE <cmd-WSCLOSE>`: Close a WebSocket connection.

.. _cmd-ws-intro:

Introduction
------------

.. important::
  The default AT firmware does not support the AT commands listed on this page. If you need {IDF_TARGET_NAME} to support WebSocket commands, you can compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections:

  - Enable ``Component config`` -> ``AT`` -> ``AT WebSocket command support``

.. _cmd-WSCFG:

:ref:`AT+WSCFG <WS-AT>`: Set the WebSocket Configuration
---------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+WSCFG=<link_id>,<ping_intv_sec>,<ping_timeout_sec>[,<buffer_size>][,<auth_mode>,<pki_number>,<ca_number>]

**Response:**

::

    OK

or

::

    ERROR

Parameters
^^^^^^^^^^

- **<link_id>**: ID of the WebSocket connection. Range: [0,2], which means that AT can support up to three WebSocket connections.
- **<ping_intv_sec>**: WebSocket Ping interval. Unit: second. Range: [1,7200]. Default: 10, which means that WebSocket Ping packets are sent every 10 seconds by default.
- **<ping_timeout_sec>**: WebSocket Ping timeout. Unit: second. Range: [1,7200]. Default: 120, which means that by default, if the WebSocket Pong packet is not received within 120 seconds, the connection will be closed.
- **<buffer_size>**: WebSocket buffer size. Unit: byte. Range: [1,8192]. Default: 1024.
- **<auth_mode>**:

  - 0: no authentication. In this case ``<pki_number>`` and ``<ca_number>`` are not required.
  - 1: the client provides the client certificate for the server to verify.
  - 2: the client loads CA certificate to verify the server's certificate.
  - 3: mutual authentication.

- **<pki_number>**: the index of certificate and private key. If there is only one certificate and private key, the value should be 0.
- **<ca_number>**: the index of CA. If there is only one CA, the value should be 0.

Notes
^^^^^
- This command should be configured before :ref:`AT+WSOPEN <cmd-WSOPEN>` command. Otherwise, it will not take effect.
- If you want to use your own certificate or use multiple sets of certificates, please refer to :doc:`../Compile_and_Develop/How_to_update_pki_config`.
- If ``<auth_mode>`` is configured to 2 or 3, in order to check the server certificate validity period, please make sure {IDF_TARGET_NAME} has obtained the current time before sending the :ref:`AT+WSOPEN <cmd-WSOPEN>` command. (You can send :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>` command to configure SNTP and obtain the current time, and send :ref:`AT+CIPSNPTIME? <cmd-SNTPT>` command to query the current time.)
- Mutual authentication example: :ref:`WebSocket Connection over TLS (Mutual Authentication) <example-websocket-tls>`.

Example
^^^^^^^^

::

    // Set the ping interval to 30 seconds, ping timeout to 60 seconds, and buffer size to 4096 bytes for link_id: 0.
    AT+WSCFG=0,30,60,4096

.. _cmd-WSHEAD:

:ref:`AT+WSHEAD <HTTP-AT>`: Set/Query WebSocket Request Headers
---------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+WSHEAD?

**Response:**

::

    +WSHEAD:<index>,<"req_header">

    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+WSHEAD=<req_header_len>

**Response:**

::

    OK

    >

The ``>`` symbol indicates that AT is ready to receive AT command data. At this point, you can enter the WebSocket request header (in the format of ``key: value``). When the data length reaches the value of parameter ``<req_header_len>``, AT returns:

::

    OK

Parameters
^^^^^^^^^^
- **<index>**: Index value of WebSocket request header.
- **<"req_header">**: WebSocket request header.
- **<req_header_len>**: WebSocket request header length. Unit: byte.

  - 0: Clear all set WebSocket request headers.
  - Other values: Set a new WebSocket request header.

Notes
^^^^^

- This command can only set one WebSocket request header at a time, but it can be set multiple times to support multiple different WebSocket request headers.
- The WebSocket request headers configured by this command are global. Once set, all WebSocket commands will carry these request headers.

Example
^^^^^^^

::

    // Set the request header
    AT+WSHEAD=49

    // After receiving the ">" symbol, enter the authorization request header below.
    AUTHORIZATION: Basic QTIzMzIyMDE5OTk6MTIzNDU2Nzg=

    // Open a WebSocket connection
    AT+WSOPEN=0,"wss://demo.piesocket.com/v3/channel_123?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_self"

.. _cmd-WSOPEN:

:ref:`AT+WSOPEN <WS-AT>`: Query/Open a WebSocket Connection
-----------------------------------------------------------

Query Command
^^^^^^^^^^^^^^^

**Command:**

::

    AT+WSOPEN?

**Response:**

When there is a connection, AT returns:

::

    +WSOPEN:<link_id>,<state>,<"uri">

    OK

When there is no connection, AT returns:

::

    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+WSOPEN=<link_id>,<"uri">[,<"subprotocol">][,<timeout_ms>][,<"auth">]

**Response:**

::

    +WS_CONNECTED:<link_id>

    OK

or

::

    ERROR

Parameters
^^^^^^^^^^

- **<link_id>**: ID of the WebSocket connection. Range: [0,2], which means that AT can support up to three WebSocket connections.
- **<state>**: The state of WebSocket connections.

   - 0: The WebSocket connection is closed.
   - 1: The WebSocket connection is reconnecting.
   - 2: The WebSocket connection is established.
   - 3: Receiving WebSocket Pong timeout or reading connection data error, waiting for reconnection.
   - 4: The WebSocket connection Received close frame from the server side and is sending close frame to the server.

- **<"uri">**: Uniform resource identifier of WebSocket server.
- **<"subprotocol">**: The subprotocol of WebSocket (refer to `Section: RFC6455 1.9 <https://www.rfc-editor.org/rfc/rfc6455#section-1.9>`_ for more details).
- **<timeout_ms>**: Timeout for establishing a WebSocket connection. Unit: millisecond. Range: [0,180000]. Default: 15000.
- **<"auth">**: The authorization of WebSocket (refer to `Section: RFC6455 4.1.12 <https://www.rfc-editor.org/rfc/rfc6455#section-4.1>`_ for more details).

Example
^^^^^^^

::

    // uri parameter comes from https://www.piesocket.com/websocket-tester
    AT+WSOPEN=0,"wss://demo.piesocket.com/v3/channel_123?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_self"

Detailed examples refer to: :ref:`WebSocket Example <example-websocket>`.

.. _cmd-WSSEND:

:ref:`AT+WSSEND <WS-AT>`: Send Data to a WebSocket Connection
-------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+WSSEND=<link_id>,<length>[,<opcode>][,<timeout_ms>]

**Response:**

::

    OK

    >

This response indicates that AT is ready for receiving data from AT port. You should enter the data, and when the data length reaches the ``<length>`` value, the transmission of data starts.

If the connection cannot be established or is disrupted during data transmission, the system returns:

::

    ERROR

If data is transmitted successfully, the system returns:

::

    SEND OK

Parameters
^^^^^^^^^^

- **<link_id>**: ID of the WebSocket connection. Range: [0,2].
- **<length>**: Length of data to send. Unit: byte. The maximum length that can be sent is determined by subtracting the value of ``<buffer_size>`` in `AT+WSCFG <cmd-WSCFG>` by 10 and the size of the heap space that the system can allocate (taking the smaller value of the two).
- **<opcode>**: The opcode in the WebSocket frame sent. Range: [0,0xF]. Default: 1, which means text frame. For details about opcode, please refer to `Section: RFC6455 5.2 <https://www.rfc-editor.org/rfc/rfc6455#section-5.2>`_.

   - 0x0: continuation frame
   - 0x1: text frame
   - 0x2: binary frame
   - 0x3 - 0x7: reserved for further non-control frames
   - 0x8: connection close frame
   - 0x9: ping frame
   - 0xA: pong frame
   - 0xB - 0xF: reserved for further control frames

- **<timeout_ms>**: Send timeout. Unit: millisecond. Range: [0,60000]. Default: 10000.

.. _cmd-WSCLOSE:

:ref:`AT+WSCLOSE <WS-AT>`: Close a WebSocket Connection
-------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+WSCLOSE=<link_id>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

- **<link_id>**: ID of the WebSocket connection. Range: [0,2].

Example
^^^^^^^^

::

    // Close the WebSocket connection whose link_id is 0
    AT+WSCLOSE=0
