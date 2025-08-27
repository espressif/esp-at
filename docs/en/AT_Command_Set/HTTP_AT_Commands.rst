.. _HTTP-AT:

HTTP AT Commands
================

:link_to_translation:`zh_CN:[中文]`

-  :ref:`Introduction <cmd-http-intro>`
-  :ref:`AT+HTTPCLIENT <cmd-HTTPCLIENT>`: Send HTTP Client Request
-  :ref:`AT+HTTPGETSIZE <cmd-HTTPGETSIZE>`: Get HTTP Resource Size
-  :ref:`AT+HTTPCGET <cmd-HTTPCGET>`: Get HTTP Resource
-  :ref:`AT+HTTPCPOST <cmd-HTTPCPOST>`: Post HTTP data of specified length
-  :ref:`AT+HTTPCPUT <cmd-HTTPCPUT>`: Put HTTP data of specified length
-  :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>`: Set/Get long HTTP URL
-  :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>`: Set/Query HTTP request headers
-  :ref:`AT+HTTPCFG <cmd-HTTPCFG>`: Set HTTP Client Configuration
-  :ref:`HTTP AT Error Codes <cmd-HTTPErrCode>`

.. _cmd-http-intro:

Introduction
------------

.. important::
  The default AT firmware supports all the AT commands mentioned on this page. If you don't need {IDF_TARGET_NAME} to support HTTP commands, you can compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections:

  - Disable ``Component config`` -> ``AT`` -> ``AT http command support``

.. _cmd-HTTPCLIENT:

:ref:`AT+HTTPCLIENT <HTTP-AT>`: Send HTTP Client Request
------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+HTTPCLIENT=<opt>,<content-type>,<"url">,[<"host">],[<"path">],<transport_type>[,<"data">][,<"http_req_header">][,<"http_req_header">][...]

**Response:**

::

    +HTTPCLIENT:<size>,<data>

    OK

Parameters
^^^^^^^^^^

-  **<opt>**: method of HTTP client request.
   
   -  1: HEAD
   -  2: GET
   -  3: POST
   -  4: PUT
   -  5: DELETE

-  **<content-type>**: data type of HTTP client request.

   -  0: ``application/x-www-form-urlencoded``
   -  1: ``application/json``
   -  2: ``multipart/form-data``
   -  3: ``text/xml``

-  **<"url">**: HTTP URL. The parameter can override the ``<"host">`` and ``<"path">`` parameters if they are null.
-  **<"host">**: domain name or IP address.
-  **<"path">**: HTTP Path.
-  **<transport_type>**: HTTP Client transport type. Default: 1.

   -  1: ``HTTP_TRANSPORT_OVER_TCP``
   -  2: ``HTTP_TRANSPORT_OVER_SSL``

-  **<"data">**: If ``<opt>`` is a POST request, this parameter holds the data you send to the HTTP server. If not, this parameter does not exist, which means there is no need to input a comma to indicate this parameter.
-  **<"http_req_header">**: you can send more than one request header to the server.

Notes
^^^^^
-  If the length of the entire command containing the URL exceeds 256 bytes, please use the :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` command to preset the URL first, and then set the ``<"url">`` parameter of this command to ``""``.
-  If the ``url`` parameter is not null, HTTP client will use it and ignore the ``host`` parameter and ``path`` parameter; If the ``url`` parameter is omited or null string, HTTP client will use ``host`` parameter and ``path`` parameter.
-  In some released firmware, HTTP client commands are not supported (see :doc:`../Compile_and_Develop/esp-at_firmware_differences`), but you can enable it by ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT http command support`` and build the project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`).
-  The command does not support redirection. After getting the status code 301 (permanent redirection) or 302 (temporary redirection) from the server, AT will not automatically redirect to the new URL address. You can use some tools to get the actual URL, and then access it using this command.
-  If the length of the entire command containing the ``<"data">`` exceeds 256 bytes, please use the :ref:`AT+HTTPCPOST <cmd-HTTPCPOST>` command.
-  To set more HTTP request headers, use the :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>` command.

Example
^^^^^^^^

::

    // HEAD Request
    AT+HTTPCLIENT=1,0,"http://httpbin.org/get","httpbin.org","/get",1

    // GET Request
    AT+HTTPCLIENT=2,0,"http://httpbin.org/get","httpbin.org","/get",1

    // POST Request
    AT+HTTPCLIENT=3,0,"http://httpbin.org/post","httpbin.org","/post",1,"field1=value1&field2=value2"


.. _cmd-HTTPGETSIZE:

:ref:`AT+HTTPGETSIZE <HTTP-AT>`: Get HTTP Resource Size
-----------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+HTTPGETSIZE=<"url">[,<tx size>][,<rx size>][,<timeout>]

**Response:**

::

    +HTTPGETSIZE:<size>

    OK

Parameters
^^^^^^^^^^
- **<"url">**: HTTP URL. It is a string parameter and should be enclosed with quotes.
- **<tx size>**: HTTP send buffer size. Unit: byte. Default: 2048. Range: [0,10240].
- **<rx size>**: HTTP receive buffer size. Unit: byte. Default: 2048. Range: [0,10240].
- **<timeout>**: Network timeout. Unit: millisecond. Default: 5000. Range: [0,180000].
- **<size>**: HTTP resource size.

Note
^^^^^

-  If the length of the entire command containing the URL exceeds 256 bytes, please use the :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` command to preset the URL first, and then set the ``<"url">`` parameter of this command to ``""``.
-  To set HTTP request headers, use the :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>` command to set them.

Example
^^^^^^^^

::

    AT+HTTPGETSIZE="http://www.baidu.com/img/bdlogo.gif"

.. _cmd-HTTPCGET:

:ref:`AT+HTTPCGET <HTTP-AT>`: Get HTTP Resource
-----------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+HTTPCGET=<"url">[,<tx size>][,<rx size>][,<timeout>]

**Response:**

::

    +HTTPCGET:<size>,<data>
    OK

Parameters
^^^^^^^^^^
- **<"url">**: HTTP URL. It is a string parameter and should be enclosed with quotes.
- **<tx size>**: HTTP send buffer size. Unit: byte. Default: 2048. Range: [0,10240].
- **<rx size>**: HTTP receive buffer size. Unit: byte. Default: 2048. Range: [0,10240].
- **<timeout>**: Network timeout. Unit: millisecond. Default: 5000. Range: [0,180000].

Note
^^^^^

- If the length of the entire command containing the URL exceeds 256 bytes, please use the :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` command to preset the URL first, and then set the ``<"url">`` parameter of this command to ``""``.
- To set HTTP request headers, use the :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>` command to set them.

.. _cmd-HTTPCPOST:

:ref:`AT+HTTPCPOST <HTTP-AT>`: Post HTTP data of specified length
------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+HTTPCPOST=<"url">,<length>[,<http_req_header_cnt>][,<"http_req_header">..<"http_req_header">]

**Response:**

::

    OK

    >

The symbol ``>`` indicates that AT is ready for receiving serial data, and you can enter the data now. When the requirement of message length determined by the parameter ``<length>`` is met, the transmission starts.

If the transmission is successful, AT returns:

::

    SEND OK

Otherwise, it returns:

::

    SEND FAIL

Parameters
^^^^^^^^^^
- **<"url">**: HTTP URL. It is a string parameter and should be enclosed with quotes.
- **<length>**: HTTP data length to POST. The maximum length is equal to the system allocable heap size.
- **<http_req_header_cnt>**: the number of <"http_req_header"> parameters.
- **[<"http_req_header">]**: HTTP request header. You can send more than one request header to the server.

Note
^^^^^

- If the length of the entire command containing the URL exceeds 256 bytes, please use the :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` command to preset the URL first, and then set the ``<"url">`` parameter of this command to ``""``.
- the default type of ``content-type`` is ``application/x-www-form-urlencoded`` for this command.
- To set HTTP request headers, use the :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>` command to set them.

.. _cmd-HTTPCPUT:

:ref:`AT+HTTPCPUT <HTTP-AT>`: Put HTTP data of specified length
------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+HTTPCPUT=<"url">,<length>[,<http_req_header_cnt>][,<"http_req_header">..<"http_req_header">]

**Response:**

::

    OK

    >

The symbol ``>`` indicates that AT is ready for receiving serial data, and you can enter the data now. When the requirement of message length determined by the parameter ``<length>`` is met, the transmission starts.

If the transmission is successful, AT returns:

::

    SEND OK

Otherwise, it returns:

::

    SEND FAIL

Parameters
^^^^^^^^^^
- **<"url">**: HTTP URL. It is a string parameter and should be enclosed with quotes.
- **<length>**: HTTP data length to PUT. The maximum length is equal to the system allocable heap size.
- **<http_req_header_cnt>**: the number of <"http_req_header"> parameters.
- **[<"http_req_header">]**: HTTP request header. You can send more than one request header to the server.

Note
^^^^^

- If the length of the entire command containing the URL exceeds 256 bytes, please use the :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` command to preset the URL first, and then set the ``<"url">`` parameter of this command to ``""``.
- To set HTTP request headers, use the :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>` command to set them.

.. _cmd-HTTPURLCFG:

:ref:`AT+HTTPURLCFG <HTTP-AT>`: Set/Get long HTTP URL
-----------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+HTTPURLCFG?

**Response:**

::

    [+HTTPURLCFG:<url length>,<data>]
    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+HTTPURLCFG=<url length>

**Response:**

::

    OK

    >

This response indicates that AT is ready for receiving serial data. You should enter the URL now, and when the URL length reaches the ``<url length>`` value, the system returns:

::

    SET OK

Parameters
^^^^^^^^^^
- **<url length>**: HTTP URL length. Unit: byte.

  - 0: clean the HTTP URL configuration.
  - [8,8192]: set the HTTP URL configuration.

- **<data>**: HTTP URL data.

.. _cmd-HTTPCHEAD:

:ref:`AT+HTTPCHEAD <HTTP-AT>`: Set/Query HTTP Request Headers
-------------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Command:**

::

    AT+HTTPCHEAD?

**Response:**

::

    +HTTPCHEAD:<index>,<"req_header">

    OK

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+HTTPCHEAD=<req_header_len>

**Response:**

::

    OK

    >

The ``>`` symbol indicates that AT is ready to receive AT command data. At this point, you can enter the HTTP request header (in the format of ``key: value``). When the data length reaches the value of parameter ``<req_header_len>``, AT returns:

::

    OK

Parameters
^^^^^^^^^^
- **<index>**: Index value of HTTP request header.
- **<"req_header">**: HTTP request header.
- **<req_header_len>**: HTTP request header length. Unit: byte.

  - 0: Clear all set HTTP request headers.
  - Other values: Set a new HTTP request header.

Note
^^^^

- This command can only set one HTTP request header at a time, but it can be set multiple times to support multiple different HTTP request headers.
- The HTTP request headers configured by this command are global. Once set, all HTTP commands will carry these request headers.
- If the ``key`` in the HTTP request header set by this command is the same as that of other HTTP commands, the HTTP request header set by this command will be used.

.. _cmd-HTTPCHEAD_example:

Example
^^^^^^^

::

    // Set the request header
    AT+HTTPCHEAD=18

    // After receiving the ">" symbol, enter the Range request header below to download only the first 256 bytes of the resource
    Range: bytes=0-255

    // Download HTTP resource
    AT+HTTPCGET="https://docs.espressif.com/projects/esp-at/en/latest/{IDF_TARGET_PATH_NAME}/index.html"

.. _cmd-HTTPCFG:

:ref:`AT+HTTPCFG <HTTP-AT>`: Set HTTP Client Configuration
----------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

  AT+HTTPCFG=<auth_mode>[,<pki_number>][,<ca_number>]


**Response:**

::

  OK

Parameters
^^^^^^^^^^

- **<auth_mode>**:

  - 0: No authentication, in this case, ``<pki_number>`` and ``<ca_number>`` parameters are not required;
  - 1: ESP-AT provides HTTP client certificate for HTTP server CA certificate verification;
  - 2: ESP-AT HTTP client loads CA certificate to verify the server's certificate;
  - 3: Mutual authentication.

- **<pki_number>**: Index of certificate and private key. If there is only one certificate and private key, the value should be 0.
- **<ca_number>**: Index of CA. If there is only one CA, the value should be 0.

Notes
^^^^^

- By default, AT firmware does not support HTTP certificate configuration. You can enable it via ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT http command support`` > ``AT HTTP authentication method``.
- The parameters configured by this command are global. Once set, all HTTP commands will share this configuration.
- If you want to use your own certificate at runtime, use the :ref:`AT+SYSMFG <cmd-SYSMFG>` command to update the WebSocket certificate. If you want to pre-burn your own certificate, please refer to :doc:`../Compile_and_Develop/How_to_update_pki_config`.
- If ``<auth_mode>`` is set to 2 or 3, to verify the validity period of the server certificate, please ensure that {IDF_TARGET_NAME} has obtained the current time before sending other HTTP commands. (You can configure SNTP and obtain the current time by sending the :ref:`AT+CIPSNTPCFG <cmd-SNTPCFG>` command, and query the current time by sending the :ref:`AT+CIPSNTPTIME? <cmd-SNTPT>` command.)

.. _cmd-HTTPErrCode:

:ref:`HTTP AT Error Codes <HTTP-AT>`
------------------------------------

When :ref:`AT+SYSLOG=1 <cmd-SYSLOG>` is enabled, if an HTTP client request fails, AT will return an error code. The error code format is:

::

  ERR CODE:0x010a7xxx

Where ``01`` is the module identifier, ``0a`` is the module's response result to the executed AT command, and ``7xxx`` represents an HTTP error code. If ``xxx`` falls within the range of standard HTTP status codes, it indicates a standard HTTP status code; otherwise, it indicates an internal error code specific to AT HTTP. The following table lists some common HTTP status codes. For more details, please refer to `RFC 2616 <https://datatracker.ietf.org/doc/html/rfc2616#section-6.1.1>`_.

.. list-table::
  :header-rows: 1

  * - HTTP Error Code (HTTP Status Code)
    - Description
  * - 0x7000
    - Connection Failed
  * - 0x7190 (400)
    - Bad Request
  * - 0x7191 (401)
    - Unauthorized
  * - 0x7192 (402)
    - Payment Required
  * - 0x7193 (403)
    - Forbidden
  * - 0x7194 (404)
    - Not Found
  * - 0x7195 (405)
    - Method Not Allowed
  * - 0x7196 (406)
    - Not Acceptable
  * - 0x7197 (407)
    - Proxy Authentication Required
  * - 0x7198 (408)
    - Request Timeout
  * - 0x7199 (409)
    - Conflict
  * - 0x719a (410)
    - Gone
  * - 0x719b (411)
    - Length Required
  * - 0x719c (412)
    - Precondition Failed
  * - 0x719d (413)
    - Request Entity Too Large
  * - 0x719e (414)
    - Request-URI Too Long
  * - 0x719f (415)
    - Unsupported Media Type
  * - 0x71a0 (416)
    - Requested Range Not Satisfiable
  * - 0x71a1 (417)
    - Expectation Failed
  * - 0x71f4 (500)
    - Internal Server Error
  * - 0x71f5 (501)
    - Not Implemented
  * - 0x71f6 (502)
    - Bad Gateway
  * - 0x71f7 (503)
    - Service Unavailable
  * - 0x71f8 (504)
    - Gateway Timeout
  * - 0x71f9 (505)
    - HTTP Version Not Supported
