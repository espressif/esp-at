HTTP AT 示例
==========================

:link_to_translation:`en:[English]`

本文档主要提供在 {IDF_TARGET_NAME} 设备上运行 :doc:`../AT_Command_Set/HTTP_AT_Commands` 命令的详细示例。

.. contents::
   :local:
   :depth: 1

.. Important::
  当前 ESP-AT 仅支持部分 HTTP 客户端的功能。

HTTP 客户端 HEAD 请求方法
---------------------------------------------------

该示例以 http://httpbin.org 作为 HTTP 服务器。

#. 恢复出厂设置。

   命令：

   .. code-block:: none

     AT+RESTORE

   响应：
  
   .. code-block:: none

     OK

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：
  
   .. code-block:: none

     OK

#. 连接路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 发送一个 HTTP HEAD 请求。设置 ``opt`` 为 ``1`` （HEAD 方法），``url`` 为 ``http://httpbin.org/get``，``transport_type`` 为 ``1`` （HTTP_TRANSPORT_OVER_TCP）。

   命令：

   .. code-block:: none

     AT+HTTPCLIENT=1,0,"http://httpbin.org/get",,,1

   响应：

   .. code-block:: none

     +HTTPCLIENT:35, Date: Sun, 26 Sep 2021 06:59:13 GMT
     +HTTPCLIENT:30, Content-Type: application/json
     +HTTPCLIENT:19, Content-Length: 329
     +HTTPCLIENT:22, Connection: keep-alive
     +HTTPCLIENT:23, Server: gunicorn/19.9.0
     +HTTPCLIENT:30, Access-Control-Allow-Origin: *
     +HTTPCLIENT:38, Access-Control-Allow-Credentials: true

     OK

   说明：

   - 您获取到的 HTTP 头部信息可能与上述响应中的不同。

HTTP 客户端 GET 请求方法
---------------------------------------------------

本例以下载一个 JPG 格式的图片文件为例。图片链接为 https://www.espressif.com/sites/all/themes/espressif/images/about-us/solution-platform.jpg。

#. 恢复出厂设置。

   命令：

   .. code-block:: none

     AT+RESTORE

   响应：
  
   .. code-block:: none

     OK

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：
  
   .. code-block:: none

     OK

#. 连接路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 发送一个 HTTP GET 请求。设置 ``opt`` 为 ``2`` （GET 方法），``url`` 为 ``https://www.espressif.com/sites/all/themes/espressif/images/about-us/solution-platform.jpg``，``transport_type`` 为 ``2`` （HTTP_TRANSPORT_OVER_SSL）。

   命令：

   .. code-block:: none

     AT+HTTPCLIENT=2,0,"https://www.espressif.com/sites/all/themes/espressif/images/about-us/solution-platform.jpg",,,2

   响应：

   .. code-block:: none

     +HTTPCLIENT:512,<0xff><0xd8><0xff><0xe2><0x0c>XICC_PROFILE<break>
     <0x01><0x01><break>
     <break>
     <0x0c>HLino<0x02><0x10><break>
     <break>
     mntrRGB XYZ <0x07><0xce><break>
     <0x02><break>
     ...
     +HTTPCLIENT:512,<0xeb><0xe2>v<0xcb><0x98>-<0xf8><0x8a><0xae><0xf3><0xc8><0xb6><0xa8><0x86><0x02>j<0x06><0xe2>
     "<0xaa>*p<0x7f>2",h<0x12>N<0xa5><0x1e><0xd2>bp<0xea><0x1e><0xf5><0xa3>x<0xa6>J<0x14>Ti<0xc3>m<0x1a>m<0x94>T<0xe1>I<0xb6><0x90><0xdc>_<0x11>QU;<0x94><0x97><0xcb><0xdd><0xc7><0xc6><0x85><0xd7>U<0x02><0xc9>W<0xa4><0xaa><0xa1><0xa1><0x08>hB<0x1a><0x10><0x86><0x84>!<0xa1><0x08>hB<0x1a><0x10><0x9b><0xb9>K<0xf5>5<0x95>5-=<0x8a><0xcb><0xce><0xe0><0x91><0xf0>m<0xa9><0x04>C<0xde>k<0xe7><0xc2>v<H|<0xaf><0xb8>L<0x91>=<0xda>_<0x94><0xde><0xd0><0xa9><0xc0><0xdd>8<0x9a>B<0xaa><0x1a><0x10><0x86><0x84>$<0xf4><0xd6><0xf2><0xa3><0x92><0xe7><0xa8>I<0xa3>b<0x1f>)<0xe1>z<0xc4>y<0xae><0xca><0xed><0xec><0x1e>|^<0xd7>E<0xa2>_<0x13><0x9e>;{|<0xb5>Q<0x97><0xa5>P<0xdf><0xa1>#3vn<0x1b><0xc3>-<0x92><0xe2>dIn<0x9c><0xb8>
     <0xc7><0xa9><0xc6>(<0xe0><0xd3>i-<0x9e>@<0xbb><0xcc><0x88><0xd5>K<0xe3><0xf0>O<0x9f>Km<0xb3>h<0xa8>omR<0xfe><0x8b><0xf9><0xa4><0xa6><0xff><break>
     aU<0xdf><0xf3><0xa3>:A<0xe2>UG<0x04>k<0xaa>*<0xa1><0xa1><0x0b><0xca><0xec><0xd8>Q<0xfb><0xbc>yqY<0xec><0xfb>?<0x16>CM<0xf6>|}<0xae><0xf3><0x1e><0xdf>%<0xf8><0xe8><0xb1>B<0x8f>[<0xb3>><0x04><0xec><0xeb>f<0x06><0x1c><0xe8><0x92><0xc9><0x8c><0xb0>I<0xd1><0x8b>%<0x99><0x04><0xd0><0xbb>s<0x8b>xj<0xe2>4f<0xa0><0x8e>+E<0xda><0xab><0xc7>=<0xab><0xc7><0xb9>xz1f<0xba><0xfd>_e6<0xff><break>
     (w<0xa7>b<0xe3>m<0xf0>|<0x82><0xc9><0xfb><0x8b><0xac>r<0x95><0x94><0x96><0xd9>i<0xe9>RVA<0x91><0x83><0x8b>M'<0x86><0x8f><0xa3>A<0xd8><0xd8>"r"<0x8a><0xa8><0x9e>zl=<0xcd><0x16><0x07>D<0xa2><0xd0>u(<0xc2><0x8b><0x0b><0xc4><0xf1><0x87><0x9c><0x93><0x8f><0xe3><0xd5>U<0x12>]<0x8e><0x91>]<0x91><0x06>#l<0xbe><0xf4>t0?<0xd7><0x85><GEM<0xb1>%<0xee>UUT<0xe7><0xdf><0xa0><0xb9><0xce><0xe2>U@<0x03><0x82>S<0xe9>*<0xa8>hB<0x1a><0x10><0xa1><0xaf>V<0x19>U<0x9d><0xb3>x<0xa6><0xc7><0xe2><0x86><0x8e>d[<0x89>e<0x05>l<0x80>H<0x91>#<0xd2><0x8c><0xe1>j<0x1b>rH<0x04><0x89><0x98><0xd3>lZW]q><0xc2><';<0x93><0xb4><0xf5>&<0x9d><0xa0>^Wp<0xa9>6`<0xe2>T<0xa2><0xc2><0xb1>*<0xbc><0x13><0x13><0xa0><0xc4>)<0x83><0xb6><0xbe><0x86><0xb9><0x88>-<0x1a>

     OK

HTTP 客户端 POST 请求方法（适用于 POST 少量数据）
-------------------------------------------------------------------------------------

该示例以 http://httpbin.org 作为 HTTP 服务器，数据类型为 ``application/json``。

#. 恢复出厂设置。

   命令：

   .. code-block:: none

     AT+RESTORE

   响应：
  
   .. code-block:: none

     OK

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：
  
   .. code-block:: none

     OK

#. 连接路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 发送一个 HTTP POST 请求。设置 ``opt`` 为 ``3`` （POST 方法），``url`` 为 ``http://httpbin.org/post``，``content-type`` 为 ``1`` （application/json），``transport_type`` 为 ``1`` （HTTP_TRANSPORT_OVER_TCP）。

   命令：

   .. code-block:: none

     AT+HTTPCLIENT=3,1,"http://httpbin.org/post",,,1,"{\"form\":{\"purpose\":\"test\"}}"

   响应：

   .. code-block:: none

     +HTTPCLIENT:282,{
       "args": {}, 
       "data": "{\"form\":{\"purpose\":\"test\"}}", 
       "files": {}, 
       "form": {}, 
       "headers": {
         "Content-Length": "27", 
         "Content-Type": "application/json", 
         "Host": "httpbin.org", 
         "User-Agent": "ESP32 HTTP Client/1.0", 
         "X-Amzn-Trace-Id": "Root=
     +HTTPCLIENT:173,1-61503a3f-4b16b71918855b97614c5dfb"
       }, 
       "json": {
         "form": {
           "purpose": "test"
         }
       }, 
       "origin": "20.187.154.207", 
       "url": "http://httpbin.org/post"
     }


     OK

   说明：

   - 您获取到的结果可能与上述响应中的不同。

HTTP 客户端 POST 请求方法（推荐方式）
-------------------------------------------------------------------------------------

如果您 POST 的数据量相对较多，已经超过了单条 AT 命令的长度阈值 ``256``，则建议您可以使用 :ref:`AT+HTTPCPOST <cmd-HTTPCPOST>` 命令。

该示例以 http://httpbin.org 作为 HTTP 服务器，数据类型为 ``application/json``。

#. 恢复出厂设置。

   命令：

   .. code-block:: none

     AT+RESTORE

   响应：
  
   .. code-block:: none

     OK

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：
  
   .. code-block:: none

     OK

#. 连接路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. Post 指定长度数据。该命令设置 HTTP 头部字段数量为 2，分别是 ``connection`` 字段和 ``content-type`` 字段，``connection`` 字段值为 ``keep-alive``，``connection`` 字段值为 ``application/json``。

   假设你想要 post 的 JSON 数据如下，长度为 427 字节。

   .. code-block:: none

     {"headers": {"Accept": "application/json","Accept-Encoding": "gzip, deflate","Accept-Language": "en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7","Content-Length": "0","Host": "httpbin.org","Origin": "http://httpbin.org","Referer": "http://httpbin.org/","User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36","X-Amzn-Trace-Id": "Root=1-6150581e-1ad4bd5254b4bf5218070413"}}

   命令：

   .. code-block:: none

     AT+HTTPCPOST="http://httpbin.org/post",427,2,"connection: keep-alive","content-type: application/json"

   响应：

   .. code-block:: none

     OK

     >

   上述响应表示 AT 已准备好接收串行数据，此时您可以输入上面提到的 427 字节长的数据，当 AT 接收到的数据长度达到 ``<length>`` 后，数据传输开始。

   .. code-block:: none

     +HTTPCPOST:281,{
       "args": {}, 
       "data": "{\"headers\": {\"Accept\": \"application/json\",\"Accept-Encoding\": \"gzip, deflate\",\"Accept-Language\": \"en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7\",\"Content-Length\": \"0\",\"Host\": \"httpbin.org\",\"Origin\": \"http://httpbin.org\",\"Referer\": \"htt
     +HTTPCPOST:512,p://httpbin.org/\",\"User-Agent\": \"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36\",\"X-Amzn-Trace-Id\": \"Root=1-6150581e-1ad4bd5254b4bf5218070413\"}}", 
       "files": {}, 
       "form": {}, 
       "headers": {
         "Content-Length": "427", 
         "Content-Type": "application/json", 
         "Host": "httpbin.org", 
         "User-Agent": "ESP32 HTTP Client/1.0", 
         "X-Amzn-Trace-Id": "Root=1-61505e76-278b5c267aaf55842bd58b32"
       }, 
       "json": {
         "headers": {
           
     +HTTPCPOST:512,"Accept": "application/json", 
           "Accept-Encoding": "gzip, deflate", 
           "Accept-Language": "en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7", 
           "Content-Length": "0", 
           "Host": "httpbin.org", 
           "Origin": "http://httpbin.org", 
           "Referer": "http://httpbin.org/", 
           "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36", 
           "X-Amzn-Trace-Id": "Root=1-6150581e-1ad4bd5254b4bf5218070413"
         }
       }, 
       "origin": "20.187.154
     +HTTPCPOST:45,.207", 
       "url": "http://httpbin.org/post"
     }


     SEND OK

   说明：

   - ``AT`` 输出 ``>`` 字符后，HTTP body 中的特殊字符不需要转义字符进行转义，也不需要以新行结尾（CR-LF）。

HTTP 客户端 PUT 请求方法（适用于无数据情况）
---------------------------------------------------

该示例以 http://httpbin.org 作为 HTTP 服务器。PUT 请求支持 `查询字符串参数 <https://www.postman.com/postman/workspace/published-postman-templates/documentation/631643-f695cab7-6878-eb55-7943-ad88e1ccfd65?ctx=documentation>`__ 模式。

#. 恢复出厂设置。

   命令：

   .. code-block:: none

     AT+RESTORE

   响应：
  
   .. code-block:: none

     OK

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：
  
   .. code-block:: none

     OK

#. 连接路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 发送一个 HTTP PUT 请求。设置 ``opt`` 为 ``4`` （PUT 方法），``url`` 为 ``http://httpbin.org/put``，``transport_type`` 为 ``1`` （HTTP_TRANSPORT_OVER_TCP）。

   命令：

   .. code-block:: none

     AT+HTTPCLIENT=4,0,"http://httpbin.org/put?user=foo",,,1

   响应：

   .. code-block:: none

     +HTTPCLIENT:282,{
       "args": {
         "user": "foo"
       }, 
       "data": "", 
       "files": {}, 
       "form": {}, 
       "headers": {
         "Content-Length": "0", 
         "Content-Type": "application/x-www-form-urlencoded", 
         "Host": "httpbin.org", 
         "User-Agent": "ESP32 HTTP Client/1.0", 
         "X-Amzn-Trace-Id": "R
     +HTTPCLIENT:140,oot=1-61503d41-1dd8cbe0056190f721ab1912"
       }, 
       "json": null, 
       "origin": "20.187.154.207", 
       "url": "http://httpbin.org/put?user=foo"
     }


     OK

   说明：

   - 您获取到的结果可能与上述响应中的不同。

HTTP 客户端 PUT 请求方法（推荐方式）
-------------------------------------------------------------------------------------

该示例以 http://httpbin.org 作为 HTTP 服务器，数据类型为 ``application/json``。

#. 恢复出厂设置。

   命令：

   .. code-block:: none

     AT+RESTORE

   响应：

   .. code-block:: none

     OK

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. PUT 指定长度数据。该命令设置 HTTP 头部字段数量为 2，分别是 ``connection`` 字段和 ``content-type`` 字段，``connection`` 字段值为 ``keep-alive``，``connection`` 字段值为 ``application/json``。

   假设你想要 put 的 JSON 数据如下，长度为 427 字节。

   .. code-block:: none

     {"headers": {"Accept": "application/json","Accept-Encoding": "gzip, deflate","Accept-Language": "en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7","Content-Length": "0","Host": "httpbin.org","Origin": "http://httpbin.org","Referer": "http://httpbin.org/","User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36","X-Amzn-Trace-Id": "Root=1-6150581e-1ad4bd5254b4bf5218070413"}}

   命令：

   .. code-block:: none

     AT+HTTPCPUT="http://httpbin.org/put",427,2,"connection: keep-alive","content-type: application/json"

   响应：

   .. code-block:: none

     OK

     >

   上述响应表示 AT 已准备好接收串行数据，此时您可以输入上面提到的 427 字节长的数据，当 AT 接收到的数据长度达到 ``<length>`` 后，数据传输开始。

   .. code-block:: none

      +HTTPCPUT:281,{
        "args": {}, 
        "data": "{\"headers\": {\"Accept\": \"application/json\",\"Accept-Encoding\": \"gzip, deflate\",\"Accept-Language\": \"en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7\",\"Content-Length\": \"0\",\"Host\": \"httpbin.org\",\"Origin\": \"http://httpbin.org\",\"Referer\": \"htt
      +HTTPCPUT:512,p://httpbin.org/\",\"User-Agent\": \"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36\",\"X-Amzn-Trace-Id\": \"Root=1-6150581e-1ad4bd5254b4bf5218070413\"}}", 
        "files": {}, 
        "form": {}, 
        "headers": {
          "Content-Length": "427", 
          "Content-Type": "application/json", 
          "Host": "httpbin.org", 
          "User-Agent": "ESP32 HTTP Client/1.0", 
          "X-Amzn-Trace-Id": "Root=1-635f7009-681be2d5478504dc5b83624a"
        }, 
        "json": {
          "headers": {
            
      +HTTPCPUT:512,"Accept": "application/json", 
            "Accept-Encoding": "gzip, deflate", 
            "Accept-Language": "en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7", 
            "Content-Length": "0", 
            "Host": "httpbin.org", 
            "Origin": "http://httpbin.org", 
            "Referer": "http://httpbin.org/", 
            "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36", 
            "X-Amzn-Trace-Id": "Root=1-6150581e-1ad4bd5254b4bf5218070413"
          }
        }, 
        "origin": "52.246.135
      +HTTPCPUT:43,.57", 
        "url": "http://httpbin.org/put"
      }


      SEND OK

   说明：

   - ``AT`` 输出 ``>`` 字符后，HTTP body 中的特殊字符不需要转义字符进行转义，也不需要以新行结尾（CR-LF）。

HTTP 客户端 DELETE 请求方法
---------------------------------------------------

该示例以 http://httpbin.org 作为 HTTP 服务器。DELETE 方法用于删除服务器上的资源。DELETE 请求的实现依赖服务器。

#. 恢复出厂设置。

   命令：

   .. code-block:: none

     AT+RESTORE

   响应：
  
   .. code-block:: none

     OK

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：
  
   .. code-block:: none

     OK

#. 连接路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 发送一个 HTTP DELETE 请求。设置  ``opt`` to ``5`` （DELETE 方法），``url`` 为 ``http://httpbin.org/delete``，``transport_type`` to ``1`` （HTTP_TRANSPORT_OVER_TCP）。

   命令：

   .. code-block:: none

     AT+HTTPCLIENT=5,0,"https://httpbin.org/delete",,,1

   响应：

   .. code-block:: none

     +HTTPCLIENT:282,{
       "args": {}, 
       "data": "", 
       "files": {}, 
       "form": {}, 
       "headers": {
         "Content-Length": "0", 
         "Content-Type": "application/x-www-form-urlencoded", 
         "Host": "httpbin.org", 
         "User-Agent": "ESP32 HTTP Client/1.0", 
         "X-Amzn-Trace-Id": "Root=1-61504289-468a41
     +HTTPCLIENT:114,737b0d251672acec9d"
       }, 
       "json": null, 
       "origin": "20.187.154.207", 
       "url": "https://httpbin.org/delete"
     }


     OK

   说明：

   - 您获取到的结果可能与上述响应中的不同。
