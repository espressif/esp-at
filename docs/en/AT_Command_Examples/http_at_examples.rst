HTTP AT Examples
==========================

:link_to_translation:`zh_CN:[中文]`

This document provides detailed command examples to illustrate how to utilize :doc:`../AT_Command_Set/HTTP_AT_Commands` on ESP devices.

.. contents::
   :local:
   :depth: 1

.. Important::
  Currently ESP-AT only supports some HTTP client functions.

The HEAD method of HTTP client
---------------------------------------------------

In this example, the HTTP server is http://httpbin.org.

#. Restore factory default settings of the module.

   Command:

   .. code-block:: none

    AT+RESTORE

   Response:
  
   .. code-block:: none

     OK

#. Set the Wi-Fi mode to station.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:
  
   .. code-block:: none

     OK

#. Connect to the router.

   Command:

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   Response:

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   Note:

   - The SSID and password you entered may be different from those in the above command. Please replace the SSID and password with those of your router settings.

#. Send an HTTP HEAD request. Set ``opt`` to ``1`` (HEAD method), ``url`` to ``http://httpbin.org/get`` and ``transport_type`` to ``1`` (HTTP_TRANSPORT_OVER_TCP).

   Command:

   .. code-block:: none

     AT+HTTPCLIENT=1,0,"http://httpbin.org/get",,,1

   Response:

   .. code-block:: none

     +HTTPCLIENT:35, Date: Sun, 26 Sep 2021 06:59:13 GMT
     +HTTPCLIENT:30, Content-Type: application/json
     +HTTPCLIENT:19, Content-Length: 329
     +HTTPCLIENT:22, Connection: keep-alive
     +HTTPCLIENT:23, Server: gunicorn/19.9.0
     +HTTPCLIENT:30, Access-Control-Allow-Origin: *
     +HTTPCLIENT:38, Access-Control-Allow-Credentials: true

     OK

   Note:

   - The HTTP header information you obtain may be different from those in the above response.

The GET method of HTTP client
---------------------------------------------------

This example describes how to download an image file in JPG format. The image link is https://www.espressif.com/sites/all/themes/espressif/images/about-us/solution-platform.jpg.

#. Restore factory default settings of the module.

   Command:

   .. code-block:: none

     AT+RESTORE

   Response:
  
   .. code-block:: none

     OK

#. Set the Wi-Fi mode to station.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:
  
   .. code-block:: none

     OK

#. Connect to the router.

   Command:

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   Response:

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   Note:

   - The SSID and password you entered may be different from those in the above command. Please replace the SSID and password with those of your router settings.

#. Send an HTTP GET request. Set ``opt`` to ``2`` (GET method), ``url`` to ``https://www.espressif.com/sites/all/themes/espressif/images/about-us/solution-platform.jpg`` and ``transport_type`` to ``2`` (HTTP_TRANSPORT_OVER_SSL).

   Command:

   .. code-block:: none

     AT+HTTPCLIENT=2,0,"https://www.espressif.com/sites/all/themes/espressif/images/about-us/solution-platform.jpg",,,2

   Response:

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

The POST method of HTTP client (suitable for POST small amount of data)
-------------------------------------------------------------------------------------

In this example, the HTTP server is http://httpbin.org and the data type is ``application/json``.

#. Restore factory default settings of the module.

   Command:

   .. code-block:: none

     AT+RESTORE

   Response:
  
   .. code-block:: none

     OK

#. Set the Wi-Fi mode to station.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:
  
   .. code-block:: none

     OK

#. Connect to the router.

   Command:

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   Response:

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   Note:

   - The SSID and password you entered may be different from those in the above command. Please replace the SSID and password with those of your router settings.

#. Send an HTTP POST request. Set ``opt`` to ``3`` (POST method), ``url`` to ``http://httpbin.org/post``, ``content-type`` to ``1`` (application/json) and ``transport_type`` to ``1`` (HTTP_TRANSPORT_OVER_TCP).

   Command:

   .. code-block:: none

     AT+HTTPCLIENT=3,1,"http://httpbin.org/post",,,1,"{\"form\":{\"purpose\":\"test\"}}"

   Response:

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

   Note:

   - The results you obtain may be different from those in the above response.

The POST method of HTTP client (recommended method)
-------------------------------------------------------------------------------------

If the amount of data you post is relatively large, and the length of a single AT command has exceeded the threshold of ``256``, it is recommended that you use the :ref:`AT+HTTPCPOST <cmd-HTTPCPOST>` command.

In this example, the HTTP server is http://httpbin.org and the data type is ``application/json``.

#. Restore factory default settings of the module.

   Command:

   .. code-block:: none

     AT+RESTORE

   Response:
  
   .. code-block:: none

     OK

#. Set the Wi-Fi mode to station.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:
  
   .. code-block:: none

     OK

#. Connect to the router.

   Command:

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   Response:

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   Note:

   - The SSID and password you entered may be different from those in the above command. Please replace the SSID and password with those of your router settings.

#. Post HTTP data of specified length. The command specifies that the number of HTTP header fields is 2, which are ``connection`` field and ``content-type`` field respectively. ``connection`` is ``keep-alive`` and ``content-type`` is ``application/json``.

   Assume the JSON data you want to post is as follows, length is 472 bytes.

   .. code-block:: none

     {"headers": {"Accept": "application/json","Accept-Encoding": "gzip, deflate","Accept-Language": "en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7","Content-Length": "0","Host": "httpbin.org","Origin": "http://httpbin.org","Referer": "http://httpbin.org/","User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36","X-Amzn-Trace-Id": "Root=1-6150581e-1ad4bd5254b4bf5218070413"}}

   Command:

   .. code-block:: none

     AT+HTTPCPOST="http://httpbin.org/post",427,2,"connection: keep-alive","content-type: application/json"

   Response:

   .. code-block:: none

     OK

     >

   This response indicates that AT is ready for receiving serial data. You should enter the data, and when the data length reaches the ``<length>`` value, the transmission of data starts. 

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

   Note:

   - After AT outputs the ``>`` character, the special characters in the HTTP body does not need to be escaped through the escape character, and it does not need to end with a new line(CR-LF).

The PUT method of HTTP client
---------------------------------------------------

In this example, the HTTP server is http://httpbin.org. PUT request supports `Query String Parameters <https://www.postman.com/postman/workspace/published-postman-templates/documentation/631643-f695cab7-6878-eb55-7943-ad88e1ccfd65?ctx=documentation>`__ mode.

#. Restore factory default settings of the module.

   Command:

   .. code-block:: none

     AT+RESTORE

   Response:
  
   .. code-block:: none

     OK

#. Set the Wi-Fi mode to station.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:
  
   .. code-block:: none

     OK

#. Connect to the router.

   Command:

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   Response:

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   Note:

   - The SSID and password you entered may be different from those in the above command. Please replace the SSID and password with those of your router settings.

#. Send an HTTP PUT request. Set ``opt`` to ``4`` (PUT method), ``url`` to ``http://httpbin.org/put``, and ``transport_type`` to ``1`` (HTTP_TRANSPORT_OVER_TCP).

   Command:

   .. code-block:: none

     AT+HTTPCLIENT=4,0,"http://httpbin.org/put?user=foo",,,1

   Response:

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

   Note:

   - The results you obtain may be different from those in the above response.

The DELETE method of HTTP client
---------------------------------------------------

In this example, the HTTP server is http://httpbin.org. The DELETE method is used to delete resources on a server. The exact use of DELETE requests depends on the server implementation.

#. Restore factory default settings of the module.

   Command:

   .. code-block:: none

     AT+RESTORE

   Response:
  
   .. code-block:: none

     OK

#. Set the Wi-Fi mode to station.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:
  
   .. code-block:: none

     OK

#. Connect to the router.

   Command:

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   Response:

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   Note:

   - The SSID and password you entered may be different from those in the above command. Please replace the SSID and password with those of your router settings.

#. Send an HTTP DELETE request. Set ``opt`` to ``5`` (DELETE method), ``url`` to ``http://httpbin.org/delete``, and ``transport_type`` to ``1`` (HTTP_TRANSPORT_OVER_TCP).

   Command:

   .. code-block:: none

     AT+HTTPCLIENT=5,0,"https://httpbin.org/delete",,,1

   Response:

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

   Note:

   - The results you obtain may be different from those in the above response.
