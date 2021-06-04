.. _HTTP-AT:

HTTP AT 命令集
================

:link_to_translation:`en:[English]`

-  :ref:`AT+HTTPCLIENT <cmd-HTTPCLIENT>`：发送 HTTP 客户端请求
-  :ref:`AT+HTTPGETSIZE <cmd-HTTPGETSIZE>`：获取 HTTP 资源大小
-  :ref:`AT+HTTPCPOST <cmd-HTTPCPOST>`：Post 指定长度的 HTTP 数据
-  :ref:`HTTP AT 错误码 <cmd-HTTPErrCode>`

.. _cmd-HTTPCLIENT:

:ref:`AT+HTTPCLIENT <HTTP-AT>`：发送 HTTP 客户端请求
------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+HTTPCLIENT=<opt>,<content-type>,<"url">,[<"host">],[<"path">],<transport_type>[,<"data">][,<"http_req_header">][,<"http_req_header">][...]

**响应：**

::

    +HTTPCLIENT:<size>,<data>

    OK

参数
^^^^

-  **<opt>**：HTTP 客户端请求方法：
   
   -  1: HEAD
   -  2: GET
   -  3: POST
   -  4: PUT
   -  5: DELETE

-  **<content-type>**：客户端请求数据类型：

   -  0: ``application/x-www-form-urlencoded``
   -  1: ``application/json``
   -  2: ``multipart/form-data``
   -  3: ``text/xml``

-  **<"url">**：HTTP URL，当后面的 ``<host>`` 和 ``<path>`` 参数为空时，本参数会自动覆盖这两个参数。
-  **<"host">**：域名或 IP 地址。
-  **<"path">**：HTTP 路径。
-  **<transport_type>**：HTTP 客户端传输类型，默认值为 1：

   -  1: ``HTTP_TRANSPORT_OVER_TCP``
   -  2: ``HTTP_TRANSPORT_OVER_SSL``

-  **<"data">**: 当 ``<opt>`` 是 POST 请求时，本参数为发送给 HTTP 服务器的数据。当 ``<opt>`` 不是 POST 请求时，这个参数不存在（也就是，不需要输入逗号来表示有这个参数）。
-  **<"http_req_header">**: 可发送多个请求头给服务器。

说明
^^^^

-  如果 ``url`` 参数不为空，HTTP 客户端将使用它并忽略 ``host`` 参数和 ``path`` 参数；如果 ``url`` 参数被省略或字符串为空，HTTP 客户端将使用 ``host`` 参数和 ``path`` 参数。
-  某些已发布的固件默认不支持 HTTP 客户端命令（详情请见 :doc:`../Compile_and_Develop/How_to_understand_the_differences_of_each_type_of_module`），但是可通过以下方式使其支持该命令：``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT http command support``，然后编译项目（详情请见 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`）。

示例
^^^^

::

    // HEAD 请求
    AT+HTTPCLIENT=1,0,"http://httpbin.org/get","httpbin.org","/get",1

    // GET 请求
    AT+HTTPCLIENT=2,0,"http://httpbin.org/get","httpbin.org","/get",1

    // POST 请求
    AT+HTTPCLIENT=3,0,"http://httpbin.org/post","httpbin.org","/post",1,"field1=value1&field2=value2"


.. _cmd-HTTPGETSIZE:

:ref:`AT+HTTPGETSIZE <HTTP-AT>`：获取 HTTP 资源大小
-----------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+HTTPGETSIZE=<url>

**响应：**

::

    +HTTPGETSIZE:<size>

    OK

参数
^^^^
- **<url>**：HTTP URL。
- **<size>**：HTTP 资源大小。

说明
^^^^

-  某些已发布的固件默认不支持 HTTP 客户端命令（详情请见 :doc:`../Compile_and_Develop/How_to_understand_the_differences_of_each_type_of_module`），但是可通过以下方式使其支持该命令：``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT http command support``，然后编译项目（详情请见 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`）。

示例
^^^^

::

    AT+HTTPGETSIZE="http://www.baidu.com/img/bdlogo.gif"

.. _cmd-HTTPCPOST:

:ref:`AT+HTTPCPOST <HTTP-AT>`：Post 指定长度的 HTTP 数据
------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+HTTPCPOST=<url>,<length>[,<http_req_header_cnt>][,<http_req_header>..<http_req_header>]

**响应：**

::

    OK
    >

符号 ``>`` 表示 AT 准备好接收串口数据，此时您可以输入数据，当数据长度达到参数 ``<length>`` 的值时，传输开始。

若传输成功，则返回：

::

    SEND OK

若传输失败，则返回：

::

    SEND FAIL

参数
^^^^
- **<url>**：HTTP URL。
- **<length>**：需 POST 的 HTTP 数据长度。最大长度等于系统可分配的堆空间大小。
- **<http_req_header_cnt>**：``<http_req_header>`` 参数的数量。
- **[<http_req_header>]**：可发送多个请求头给服务器。

.. _cmd-HTTPErrCode:

:ref:`HTTP AT 错误码 <HTTP-AT>`
-------------------------------------

-  HTTP 客户端：

   .. list-table::          
      :header-rows: 1         
          
      * - HTTP 客户端错误码
        - 说明     
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

-  HTTP 服务器：

   .. list-table::          
      :header-rows: 1 

      * - HTTP 服务器错误码
        - 说明
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

-  HTTP AT：
   
   - ``AT+HTTPCLIENT`` 命令的错误码为 ``0x7000+Standard HTTP Error Code`` （更多有关 Standard HTTP/1.1 Error Code 的信息，请参考 `RFC 2616 <https://tools.ietf.org/html/rfc2616>`_）。
   - 例如，若 AT 在调用 ``AT+HTTPCLIENT`` 命令时收到 HTTP error 404，则会返回 ``0x7194`` 错误码 (``hex(0x7000+404)=0x7194``)。