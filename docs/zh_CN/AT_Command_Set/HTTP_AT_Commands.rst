.. _HTTP-AT:

HTTP AT 命令集
================

:link_to_translation:`en:[English]`

-  :ref:`介绍 <cmd-http-intro>`
-  :ref:`AT+HTTPCLIENT <cmd-HTTPCLIENT>`：发送 HTTP 客户端请求
-  :ref:`AT+HTTPGETSIZE <cmd-HTTPGETSIZE>`：获取 HTTP 资源大小
-  :ref:`AT+HTTPCGET <cmd-HTTPCGET>`：获取 HTTP 资源
-  :ref:`AT+HTTPCPOST <cmd-HTTPCPOST>`：Post 指定长度的 HTTP 数据
-  :ref:`AT+HTTPCPUT <cmd-HTTPCPUT>`：Put 指定长度的 HTTP 数据
-  :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>`：设置/获取长的 HTTP URL
-  :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>`：设置/查询 HTTP 请求头
-  :ref:`HTTP AT 错误码 <cmd-HTTPErrCode>`

.. _cmd-http-intro:

介绍
------

.. only:: esp32 or esp32c3 or esp32c6 or esp32s2

  .. important::
    默认的 AT 固件支持此页面下的所有 AT 命令。如果您不需要 {IDF_TARGET_NAME} 支持 HTTP 命令，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：

    - 禁用 ``Component config`` -> ``AT`` -> ``AT http command support``

.. only:: esp32c2

  .. important::
    **默认的 {IDF_TARGET_CFG_PREFIX}-4MB AT 固件支持 HTTP 命令，而 {IDF_TARGET_CFG_PREFIX}-2MB AT 固件不支持**。如果您需要 {IDF_TARGET_CFG_PREFIX}-2MB 支持 HTTP 功能，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：

    - 启用 ``Component config`` -> ``AT`` -> ``AT http command support``

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
   
   -  1：HEAD
   -  2：GET
   -  3：POST
   -  4：PUT
   -  5：DELETE

-  **<content-type>**：客户端请求数据类型：

   -  0：``application/x-www-form-urlencoded``
   -  1：``application/json``
   -  2：``multipart/form-data``
   -  3：``text/xml``

-  **<"url">**：HTTP URL，当后面的 ``<host>`` 和 ``<path>`` 参数为空时，本参数会自动覆盖这两个参数。
-  **<"host">**：域名或 IP 地址。
-  **<"path">**：HTTP 路径。
-  **<transport_type>**：HTTP 客户端传输类型，默认值为 1：

   -  1：``HTTP_TRANSPORT_OVER_TCP``
   -  2：``HTTP_TRANSPORT_OVER_SSL``

-  **<"data">**：当 ``<opt>`` 是 POST 请求时，本参数为发送给 HTTP 服务器的数据。当 ``<opt>`` 不是 POST 请求时，这个参数不存在（也就是，不需要输入逗号来表示有这个参数）。
-  **<"http_req_header">**：可发送多个请求头给服务器。

说明
^^^^
-  如果包含 URL 的整条命令的长度超过了 256 字节，请先使用 :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` 命令预配置 URL，然后本命令里的 ``<"url">`` 参数需要设置为 ``""``。
-  如果 ``url`` 参数不为空，HTTP 客户端将使用它并忽略 ``host`` 参数和 ``path`` 参数；如果 ``url`` 参数被省略或字符串为空，HTTP 客户端将使用 ``host`` 参数和 ``path`` 参数。
-  某些已发布的固件默认不支持 HTTP 客户端命令（详情请见 :doc:`../Compile_and_Develop/esp-at_firmware_differences`），但是可通过以下方式使其支持该命令：``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT http command support``，然后编译项目（详情请见 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`）。
-  该命令不支持 URL 重定向，在获取到服务器返回的状态码 301（永久性重定向）或者 302（临时性重定向）后不会自动跳转到新的 URL 地址。您可以使用某些工具获取要访问的实际 URL，然后通过该命令访问它。
-  如果包含 ``<"data">`` 参数的整条命令的长度超过了 256 字节，请使用 :ref:`AT+HTTPCPOST <cmd-HTTPCPOST>` 命令。
-  要设置更多的 HTTP 请求头，请使用 :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>` 命令。

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

    AT+HTTPGETSIZE=<"url">[,<tx size>][,<rx size>][,<timeout>]

**响应：**

::

    +HTTPGETSIZE:<size>

    OK

参数
^^^^
- **<"url">**：HTTP URL。
- **<tx size>**：HTTP 发送缓存大小。单位：字节。默认值：2048。范围：[0,10240]。
- **<rx size>**：HTTP 接收缓存大小。单位：字节。默认值：2048。范围：[0,10240]。
- **<timeout>**：网络超时。单位：毫秒。默认值：5000。范围：[0,180000]。
- **<size>**：HTTP 资源大小。

说明
^^^^

-  如果包含 URL 的整条命令的长度超过了 256 字节，请先使用 :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` 命令预配置 URL，然后本命令里的 ``<"url">`` 参数需要设置为 ``""``。
-  如果您想设置 HTTP 请求头，请使用 :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>` 命令设置。

示例
^^^^

::

    AT+HTTPGETSIZE="http://www.baidu.com/img/bdlogo.gif"

.. _cmd-HTTPCGET:

:ref:`AT+HTTPCGET <HTTP-AT>`：获取 HTTP 资源
-----------------------------------------------

设置命令
^^^^^^^^^^^

**命令：**

::

    AT+HTTPCGET=<"url">[,<tx size>][,<rx size>][,<timeout>]

**响应：**

::

    +HTTPCGET:<size>,<data>
    OK

参数
^^^^^^^^^^
- **<"url">**：HTTP URL。
- **<tx size>**：HTTP 发送缓存大小。单位：字节。默认值：2048。范围：[0,10240]。
- **<rx size>**：HTTP 接收缓存大小。单位：字节。默认值：2048。范围：[0,10240]。
- **<timeout>**：网络超时。单位：毫秒。默认值：5000。范围：[0,180000]。

说明
^^^^^

-  如果包含 URL 的整条命令的长度超过了 256 字节，请先使用 :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` 命令预配置 URL，然后本命令里的 ``<"url">`` 参数需要设置为 ``""``。
-  如果您想设置 HTTP 请求头，请使用 :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>` 命令设置。

.. _cmd-HTTPCPOST:

:ref:`AT+HTTPCPOST <HTTP-AT>`：Post 指定长度的 HTTP 数据
------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+HTTPCPOST=<"url">,<length>[,<http_req_header_cnt>][,<http_req_header>..<http_req_header>]

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
- **<"url">**：HTTP URL。
- **<length>**：需 POST 的 HTTP 数据长度。最大长度等于系统可分配的堆空间大小。
- **<http_req_header_cnt>**：``<http_req_header>`` 参数的数量。
- **[<http_req_header>]**：HTTP 请求头。可发送多个请求头给服务器。

说明
^^^^^

-  如果包含 URL 的整条命令的长度超过了 256 字节，请先使用 :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` 命令预配置 URL，然后本命令里的 ``<"url">`` 参数需要设置为 ``""``。
-  该命令的 ``content-type`` 默认类型为 ``application/x-www-form-urlencoded``。
-  如果您想设置 HTTP 请求头，请使用 :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>` 命令设置。

.. _cmd-HTTPCPUT:

:ref:`AT+HTTPCPUT <HTTP-AT>`：Put 指定长度的 HTTP 数据
------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+HTTPCPUT=<"url">,<length>[,<http_req_header_cnt>][,<http_req_header>..<http_req_header>]

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
- **<"url">**：HTTP URL。
- **<length>**：需 Put 的 HTTP 数据长度。最大长度等于系统可分配的堆空间大小。
- **<http_req_header_cnt>**：``<http_req_header>`` 参数的数量。
- **[<http_req_header>]**：HTTP 请求头。可发送多个请求头给服务器。

说明
^^^^^

-  如果包含 URL 的整条命令的长度超过了 256 字节，请先使用 :ref:`AT+HTTPURLCFG <cmd-HTTPURLCFG>` 命令预配置 URL，然后本命令里的 ``<"url">`` 参数需要设置为 ``""``。
-  如果您想设置 HTTP 请求头，请使用 :ref:`AT+HTTPCHEAD <cmd-HTTPCHEAD>` 命令设置。

.. _cmd-HTTPURLCFG:

:ref:`AT+HTTPURLCFG <HTTP-AT>`：设置/获取长的 HTTP URL
----------------------------------------------------------

查询命令
^^^^^^^^^^^^^

**命令：**

::

    AT+HTTPURLCFG?

**响应：**

::

    [+HTTPURLCFG:<url length>,<data>]
    OK

设置命令
^^^^^^^^^^^

**命令：**

::

    AT+HTTPURLCFG=<url length>

**响应：**

::

    OK

    >

符号 > 表示 AT 准备好接收串口数据，此时您可以输入 URL，当数据长度达到参数 ``<url length>`` 的值时，系统返回：

::

    SET OK

参数
^^^^^^^^^^
- **<url length>**：HTTP URL 长度。单位：字节。

  - 0：清除 HTTP URL 配置。
  - [8,8192]：设置 HTTP URL 配置。

- **<data>**： HTTP URL 数据。

.. _cmd-HTTPCHEAD:

:ref:`AT+HTTPCHEAD <HTTP-AT>`：设置/查询 HTTP 请求头
----------------------------------------------------------

查询命令
^^^^^^^^^^^^^

**命令：**

::

    AT+HTTPCHEAD?

**响应：**

::

    +HTTPCHEAD:<index>,<"req_header">

    OK

设置命令
^^^^^^^^^^^

**命令：**

::

    AT+HTTPCHEAD=<req_header_len>

**响应：**

::

    OK

    >

符号 ``>`` 表示 AT 准备好接收 AT 命令口数据，此时您可以输入 HTTP 请求头（请求头为 ``key: value`` 形式），当数据长度达到参数 ``<req_header_len>`` 的值时，AT 返回：

::

    OK

参数
^^^^^^^^^^
- **<index>**：HTTP 请求头的索引值。
- **<"req_header">**：HTTP 请求头。
- **<req_header_len>**：HTTP 请求头长度。单位：字节。

  - 0：清除所有已设置的 HTTP 请求头。
  - 其他值：设置一个新的 HTTP 请求头。

说明
^^^^^

- 本命令一次只能设置一个 HTTP 请求头，但可以多次设置，支持多个不同的 HTTP 请求头。
- 本命令配置的 HTTP 请求头是全局性的，一旦设置，所有 HTTP 的命令都会携带这些请求头。
- 本命令设置的 HTTP 请求头中的 ``key`` 如果和其它 HTTP 命令的请求头中的 ``key`` 相同，则会使用本命令中设置的 HTTP 请求头。

.. _cmd-HTTPCHEAD_example:

示例
^^^^

::

    // 设置请求头
    AT+HTTPCHEAD=18

    // 在收到 ">" 符号后，输入以下的 Range 请求头，下载资源的前 256 个字节。
    Range: bytes=0-255

    // 下载 HTTP 资源
    AT+HTTPCGET="https://docs.espressif.com/projects/esp-at/zh_CN/latest/{IDF_TARGET_PATH_NAME}/index.html"

.. _cmd-HTTPErrCode:

:ref:`HTTP AT 错误码 <HTTP-AT>`
-------------------------------------

-  HTTP 客户端：

   .. list-table::          
      :header-rows: 1         
          
      * - HTTP 客户端错误码
        - 说明     
      * - 0x7000
        - 建立连接失败
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
   
   - ``AT+HTTPCLIENT`` 命令的错误码为 ``0x7000+Standard HTTP Error Code`` （更多有关 Standard HTTP/1.1 Error Code 的信息，请参考 `RFC 2616 <https://datatracker.ietf.org/doc/html/rfc2616>`_）。
   - 例如，若 AT 在调用 ``AT+HTTPCLIENT`` 命令时收到 HTTP error 404，则会返回 ``0x7194`` 错误码 (``hex(0x7000+404)=0x7194``)。
