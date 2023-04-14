AT 响应消息格式控制示例
=======================

:link_to_translation:`en:[English]`

.. contents::
   :local:
   :depth: 1

.. _example-sysmfgfilter:

启用 :term:`系统消息` 过滤，实现 HTTP 透传下载功能
-----------------------------------------------------

本例以下载一个 PNG 格式的图片文件为例，图片链接为 https://www.espressif.com/sites/default/files/home/hardware.png。

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

#. 设置系统消息过滤器一：过滤每条 HTTP 数据的头部和尾部。

   命令：

   .. code-block:: none

     AT+SYSMSGFILTERCFG=1,18,3

   响应：

   .. code-block:: none

     OK

     >

   此时输入 ``^+HTTPCGET:[0-9]*,`` （共 18 字节） 和 ``\r\n$`` （共 3 字节，其中 ``\r\n`` 对应 ASCII 码中的换行和回车，即：0D 0A）。响应：

   .. code-block:: none

     OK

   说明：

   - ``^+HTTPCGET:[0-9]*,`` 为头部正则表达式，表示匹配以 ``+HTTPCGET:`` 开头，紧跟着一串数字，最后为逗号的字符串。
   - ``\r\n$`` 为尾部正则表达式，表示匹配以 ``\r\n`` 结尾的字符串。

#. 设置系统消息过滤器二：过滤图片下载完成时的 OK 系统消息。

   命令：

   .. code-block:: none

     AT+SYSMSGFILTERCFG=1,0,7

   响应：

   .. code-block:: none

     OK

     >

   此时输入 ``\r\nOK\r\n$`` （共 7 字节，其中 ``\r\n`` 对应 ASCII 码中的换行和回车，即：0D 0A）。响应：

   .. code-block:: none

     OK

   说明：

   - ``\r\nOK\r\n$`` 为尾部正则表达式，表示匹配以 ``\r\nOK\r\n`` 结尾的字符串。

#. 启用系统消息过滤

   命令：

   .. code-block:: none

     AT+SYSMSGFILTER=1

   响应：

   .. code-block:: none

     OK

   说明：

   - 只有启用系统消息过滤后，上面设置的过滤器才会生效。

#. 关闭回显

   命令：

   .. code-block:: none

     ATE0

   响应：

   .. code-block:: none

     OK

#. 下载图片

   下载图片，设置发送和接收缓存大小为 2048 字节，网络超时为 5000 毫秒（注意：网络超时不是命令超时，此处的 5 秒网络超时指有连续 5 秒未接收到服务器端的数据，则关闭此网络连接。在较差的网络环境中，如果每秒都能接收一点服务器端的数据，则不会关闭此网络连接，这可能导致命令超时很长）。

   命令：

   .. code-block:: none

     AT+HTTPCGET="https://www.espressif.com/sites/default/files/home/hardware.png",2048,2048,5000

   响应：

   .. code-block:: none

     // 此处，MCU 将透传接收到整个 https://www.espressif.com/sites/default/files/home/hardware.png 图片资源。

   说明：

   - 如果图片下载失败，AT 仍然会发送 ``\r\nERROR\r\n`` （共 9 字节）系统消息给 MCU。

#. 清除过滤器

   命令：

   .. code-block:: none

     AT+SYSMSGFILTERCFG=0

   响应：

   .. code-block:: none

     OK

#. 禁用系统消息过滤

   命令：

   .. code-block:: none

     AT+SYSMSGFILTER=0

   响应：

   .. code-block:: none

     OK

#. 开启回显

   命令：

   .. code-block:: none

     ATE1

   响应：

   .. code-block:: none

     OK
