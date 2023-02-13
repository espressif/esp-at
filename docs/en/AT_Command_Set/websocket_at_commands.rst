.. _WS-AT:

WebSocket AT Commands
=====================

:link_to_translation:`zh_CN:[中文]`

- :ref:`AT+WSCFG <cmd-WSCFG>`: Set the WebSocket configuration.
- :ref:`AT+WSOPEN <cmd-WSOPEN>`: Query/Open a WebSocket connection.
- :ref:`AT+WSSEND <cmd-WSSEND>`: Send data to a WebSocket connection.
- :ref:`AT+WSCLOSE <cmd-WSCLOSE>`: Close a WebSocket connection.

.. _cmd-WSCFG:

:ref:`AT+WSCFG <WS-AT>`: Set the WebSocket Configuration
---------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+WSCFG=<link_id>,<ping_intv_sec>,<ping_timeout_sec>[,<buffer_size>]

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

Notes
^^^^^
- This command should be configured before :ref:`AT+WSOPEN <cmd-WSOPEN>` command. Otherwise, it will not take effect.

Example
^^^^^^^^

::

    // Set the ping interval to 30 seconds, ping timeout to 60 seconds, and buffer size to 4096 bytes for link_id: 0.
    AT+WSCFG=0,30,60,4096

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

    AT+WSOPEN=<link_id>,<"uri">[,<"subprotocol">][,<timeout_ms>]

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
- **<"subprotocol">**: The subprotocol of WebSocket (refer to `RFC6455 1.9 part <https://www.rfc-editor.org/rfc/rfc6455#section-1.9>`_ for more details).
- **<timeout_ms>**: Timeout for establishing a WebSocket connection. Unit: millisecond. Range: [0,180000]. Default: 15000.

Example
^^^^^^^

::

    // uri parameter comes from https://www.piesocket.com/websocket-tester
    AT+WSOPEN=0,"wss://demo.piesocket.com/v3/channel_123?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_self"

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
- **<length>**: Length of data to send. Unit: byte.
- **<opcode>**: The opcode in the WebSocket frame sent. Range: [0,0xF]. Default: 1, which means text frame. For details about opcode, please refer to `RFC6455 5.2 section <https://www.rfc-editor.org/rfc/rfc6455#section-5.2>`_.

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