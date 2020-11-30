.. _HTTP-AT:

HTTP AT Commands
================

-  :ref:`AT+HTTPCLIENT <cmd-HTTPCLIENT>`: Send HTTP Client Request
-  :ref:`AT+HTTPGETSIZE <cmd-HTTPGETSIZE>`: Get HTTP Resource Size
-  :ref:`HTTP AT Error Codes <cmd-HTTPErrCode>`

.. _cmd-HTTPCLIENT:

:ref:`AT+HTTPCLIENT <HTTP-AT>`: Send HTTP Client Request
------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+HTTPCLIENT=<opt>,<content-type>,<url>,<host>,<path>,<transport_type>,[<data>][,"http_req_header"][,"http_req_header"][...]

**Response:**

::

    +HTTPCLIENT:<size>,<data>

    OK

Parameters
^^^^^^^^^^

-  **<opt>**: method of HTTP client request.
   
   -  1 : HEAD
   -  2 : GET
   -  3 : POST
   -  4 : PUT
   -  5 : DELETE

-  **<content-type>**: data type of HTTP client request.

   -  0 : ``application/x-www-form-urlencoded``
   -  1 : ``application/json``
   -  2 : ``multipart/form-data``
   -  3 : ``text/xml``

-  **<url>**: HTTP URL. The parameter can override the ``<host>`` and ``<path>`` parameters if they are null.
-  **<host>**: domain name or IP address.
-  **<path>**: HTTP Path.
-  **<transport_type>**: HTTP Client transport type. Default: 1.

   -  1 : ``HTTP_TRANSPORT_OVER_TCP``
   -  2 : ``HTTP_TRANSPORT_OVER_SSL``

-  **[<data>]**: when it is a POST request, this parameter holds the data you want to send to the HTTP server.
-  **[<http_req_header>]**: you can send more than one request header to the server.

Note
^^^^^

-  In some released firmware, HTTP client commands are not supported (see :doc:`../Compile_and_Develop/How_to_understand_the_differences_of_each_type_of_module`), but you can enable it by ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT http command support`` and build the project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`).

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

    AT+HTTPGETSIZE=<url>

**Response:**

::

    +HTTPGETSIZE:<size>

    OK

Parameters
^^^^^^^^^^
- **<url>**: HTTP URL.
- **<size>**: HTTP resource size.

Note
^^^^^

-  In some released firmware, HTTP client commands are not supported (see :doc:`../Compile_and_Develop/How_to_understand_the_differences_of_each_type_of_module`), but you can enable it by ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT http command support`` and build the project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`).

Example
^^^^^^^^

::

    AT+HTTPGETSIZE="http://www.baidu.com/img/bdlogo.gif"

.. _cmd-HTTPErrCode:

:ref:`HTTP Error Codes <HTTP-AT>`
---------------------------------

-  HTTP Client:

   .. list-table::          
      :header-rows: 1         
          
      * - HTTP Client Error Code
        - Description      
      * - 0x7190
        - Bad Request  
      * - 0x7191
        - Unauthorized  
      * - 0x7192
        - Payment Required 
      * - 0x7193
        - Forbidden 
      * - 0x7194
        - Not Found  
      * - 0x7195
        - Method Not Allowed  
      * - 0x7196
        - Not Acceptable 
      * - 0x7197
        - Proxy Authentication Required
      * - 0x7198
        - Request Timeout
      * - 0x7199
        - Conflict
      * - 0x719a
        - Gone
      * - 0x719b
        - Length Required
      * - 0x719c
        - Precondition Failed
      * - 0x719d
        - Request Entity Too Large
      * - 0x719e
        - Request-URI Too Long
      * - 0x719f
        - Unsupported Media Type
      * - 0x71a0
        - Requested Range Not Satisfiable
      * - 0x71a1
        - Expectation Failed

-  HTTP Server:

   .. list-table::          
      :header-rows: 1 

      * - HTTP Server Error Code
        - Description 
      * - 0x71f4
        - Internal Server Error
      * - 0x71f5
        - Not Implemented
      * - 0x71f6
        - Bad Gateway
      * - 0x71f7
        - Service Unavailable
      * - 0x71f8
        - Gateway Timeout
      * - 0x71f9
        - HTTP Version Not Supported

-  HTTP AT:
   
   - The error code of command ``AT+HTTPCLIENT`` will be ``0x7000+Standard HTTP Error Code``.
   - For example, if AT gets the HTTP error 404 when calling command ``AT+HTTPCLIENT``, it will respond with error code of ``0x7194`` (``hex(0x7000+404)=0x7194``).

- For more details about Standard HTTP/1.1 Error Code, please refer to `RFC 2616 <https://tools.ietf.org/html/rfc2616>`_.
