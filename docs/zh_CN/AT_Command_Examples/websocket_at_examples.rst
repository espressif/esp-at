WebSocket AT 示例
==========================

:link_to_translation:`en:[English]`

本文档介绍了在 {IDF_TARGET_NAME} 设备上运行 :doc:`../AT_Command_Set/websocket_at_commands` 命令的详细示例。

.. _example-websocket:

.. contents::
   :local:
   :depth: 1

.. Important::
  默认的 AT 固件不支持 WebSocket 功能。如果您需要 {IDF_TARGET_NAME} 支持 WebSocket 功能，请参考 :ref:`介绍 <cmd-ws-intro>`。

.. _example-websocket-tcp:

基于 TCP 的 WebSocket 连接
--------------------------

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

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

#. PC 与 {IDF_TARGET_NAME} 设备连接同一个路由。

#. 在 PC 上使用 python 创建一个基于 TCP 的 WebSocket 服务器。示例代码如下：

   .. code-block:: python

      #!/usr/bin/env python

      import asyncio
      from websockets.server import serve

      host = "192.168.200.249"
      port = 8765

      async def echo(websocket):
          async for message in websocket:
              await websocket.send(message)

      async def main():
          async with serve(echo, host, port):
              print(f"Server started at ws://{host}:{port}")
              await asyncio.get_running_loop().create_future()  # run forever

      asyncio.run(main())

   请修改上述代码中的 ``host`` 为 PC 的 IP 地址并保存为 ``ws-server.py`` 文件，同时运行该程序。

   .. code-block:: python

      python ws-server.py

   说明：

   - 如果您的 PC 上没有安装 ``websockets`` 库，请使用 ``pip install websockets`` 安装。
   - 默认设置的 WebSocket 服务器端口为 ``8765``，您也可以设置为其它端口。

#. {IDF_TARGET_NAME} 设备作为客户端连接到 WebSocket 服务器。

   命令：

   .. code-block:: none

     AT+WSOPEN=0,"ws://192.168.200.249:8765"

   响应：

   .. code-block:: none

     +WS_CONNECTED:0

     OK

#. 发送 4 字节数据。

   命令：

   .. code-block:: none

     AT+WSSEND=0,4

   响应：

   .. code-block:: none

     OK

     >

   输入 4 字节数据，例如输入数据是 ``test``，之后 AT 将会输出以下信息。

   .. code-block:: none

     SEND OK

   说明：

   - 若输入的字节数目超过 ``AT+WSSEND`` 命令设定的长度 (n)，则系统会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``SEND OK``。

#. 接收 4 字节数据。

   由于上面的 WebSocket 服务器是一个回显服务器，因此在数据发送后，服务器也会原样返回该数据，即 AT 会输出：

   .. code-block:: none

     +WS_DATA:0,4,test

.. _example-websocket-tls:

基于 TLS 的 WebSocket 连接（相互鉴权）
--------------------------------------

#. 设置 Wi-Fi 模式为 station。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接到路由器。

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

#. 设置 SNTP 服务器。

   命令：

   .. code-block:: none

     AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"

   响应：

   .. code-block:: none

     OK

   说明：

   - 您可以根据自己国家的时区设置 SNTP 服务器。

#. 查询 SNTP 时间。

   命令：

   .. code-block:: none

     AT+CIPSNTPTIME?

   响应：

   .. code-block:: none

     +CIPSNTPTIME:Wed Nov 13 17:00:15 2024
     OK

   说明：

   - 您可以查询 SNTP 时间与实时时间是否相符来判断您设置的 SNTP 服务器是否生效。

#. PC 与 {IDF_TARGET_NAME} 设备连接同一个路由。

#. 获取 WebSocket 服务器端的 CA、证书、私钥。

   您可以使用 ``openssl`` 工具生成 CA、证书、和私钥。如果遇到困难，请使用以下配置进行测试：

   ``server_ca.crt``

   .. code-block:: none

      -----BEGIN CERTIFICATE-----
      MIIDNjCCAh6gAwIBAgIUDe6T+Pu0BqmmTjw3s4snVRNFiCMwDQYJKoZIhvcNAQEL
      BQAwNjELMAkGA1UEBhMCQ04xFTATBgNVBAoMDEVTUFJFU1NJRiBBVDEQMA4GA1UE
      AwwHUm9vdCBDQTAeFw0yNDA5MTkwOTMzNDBaFw0zNDA5MTcwOTMzNDBaMDYxCzAJ
      BgNVBAYTAkNOMRUwEwYDVQQKDAxFU1BSRVNTSUYgQVQxEDAOBgNVBAMMB1Jvb3Qg
      Q0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCylQstJGNFHLf7F+gG
      oMSZSrWV4+/5Qxw1Cw3y5N1OVkMSxppYHVxj6MbOwWoCqQcx/WMtqnhg9ATDiZOE
      bXFVB0aZd6EEBz24k7UvQ1ilfIw5tzmjl8SSbe7CiFq302WVokBVFhSeY2jrG+sI
      6PWg0WsvxzierDL9hef708IJERlX0ScBIsLZnVU4UBPtrbG2bgl3L6T5iQ95tSEX
      LsmfZ4l+/Q1xSC7VGH1K1WGBnUzGpv9vLc9uFGZVcAEKx1V9Y7DsyJvTosfOOMmb
      yUIcBk5nVCHRfhmtrfRAWWD9YaNc0gMPVpO6cHzGd/Fgw6vO6QshMYUOE6wxpVjb
      8JYlAgMBAAGjPDA6MAwGA1UdEwQFMAMBAf8wCwYDVR0PBAQDAgGmMB0GA1UdDgQW
      BBRdPJ7Nq+WXSiLN4ZLWJlQQfjrm7zANBgkqhkiG9w0BAQsFAAOCAQEANYpErE2L
      IpISbzJTvG6A67YmYMyadWSNGQ2VjdLK2s3zkggF96bIZziygOa9mgAKD/yTw10t
      V0xF1GUDz43DtJZ1wxo8FPrxH41cP63vjtp28+ZNkylv9Hrx8De2JjiYwRpZmlQY
      EHwDEFpK26LdwPPalwdMZSyMzCtRNJ+o8Bk5kSc3V9QJmh9lLe4PdfCJcfzwPskC
      dNgUMECRa4k3VFOYVUumyFofG8/kINcRogfPzZuUE2Oj1wCHqSpOaP2CJN9QTk0q
      Fn0Itrjikv6z1aTp+SJPOzRPbympL9KhhhhcYJQdPaXHWWYfdhEU/abnnJQpd+1Y
      HPnBCb4tK/pS9w==
      -----END CERTIFICATE-----

   ``server.crt``

   .. code-block:: none

      -----BEGIN CERTIFICATE-----
      MIIDVTCCAj2gAwIBAgIUPLhviJh1UJDQ5Md6tHibK11jP24wDQYJKoZIhvcNAQEL
      BQAwNjELMAkGA1UEBhMCQ04xFTATBgNVBAoMDEVTUFJFU1NJRiBBVDEQMA4GA1UE
      AwwHUm9vdCBDQTAeFw0yNDA5MTkwOTMzNDBaFw0zNDA5MTcwOTMzNDBaMDQxCzAJ
      BgNVBAYTAkNOMRUwEwYDVQQKDAxFU1BSRVNTSUYgQVQxDjAMBgNVBAMMBUwxIENB
      MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAtKALI+zRbUaMjukAi2ai
      dzPdNdy+TUv2K+GyB15yomIr0e1c1pfztk68zdHKcFFt0RQfz1GYtxrZzqvO7o4t
      K9ijFAw+k498SCTmTqHwlUKc7B9mK6UZfZSkXUnufXKE5+N96u+49e3wbk7yoNfQ
      kzdtwwXUtM6ee5w7HvjwdKQcJXYWv/c/zVLWmAUG3AEaUknS8r4LdG/X+L/bxN+9
      zycLL5tGgZg22KENW4QWsSUY6ntuKQDlohiNxi+wXyM3mVNOc94umICj7a1OhPst
      UmuLYD/gUCnM4JRvQYAVmPQCi88KoLj7aIwJedQ7TJwhJGDS6WDVEMeRfJVXswUk
      4QIDAQABo10wWzAMBgNVHRMEBTADAQH/MAsGA1UdDwQEAwIBpjAdBgNVHQ4EFgQU
      e1wZbnvJnZr8js60EyIWiP6hRFAwHwYDVR0jBBgwFoAUNrQtNxD0tapUczc8Mwln
      hPZE84YwDQYJKoZIhvcNAQELBQADggEBAICnosdYBc6enaKB77AWXFzMWyDDLvd5
      JSJa1IRJ+Rhs1gBxjIH66/+6QyZcJu5C/RZ+RZ04Mky3nMndc3kSCFqauBCK9xoG
      /fZ5wrAfH54o4P+3ZliGoK8EltKu1HuQYJTW8JKbLWCRJ8PnGkrDZBgSXgn8tyL4
      U3hu1MdmkH//fiV0z6itfyJcZDu387l8bGBhrAD3Z7gWcdIJbXxmu6m7FJadUe/N
      0vsLtgkOpOyUa2rThOnJpSyXB5QnKifRGYjuPrnOIWtmADYQRUeD2zdgqRUUuYJR
      ee4Vz2BFXhpZdGeD3bVAop+/YEbTa0iDxXSLWkPLQfCyIkdTPXmKQPQ=
      -----END CERTIFICATE-----

   ``server.key``

   .. code-block:: none

      -----BEGIN PRIVATE KEY-----
      MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQC0oAsj7NFtRoyO
      6QCLZqJ3M9013L5NS/Yr4bIHXnKiYivR7VzWl/O2TrzN0cpwUW3RFB/PUZi3GtnO
      q87uji0r2KMUDD6Tj3xIJOZOofCVQpzsH2YrpRl9lKRdSe59coTn433q77j17fBu
      TvKg19CTN23DBdS0zp57nDse+PB0pBwldha/9z/NUtaYBQbcARpSSdLyvgt0b9f4
      v9vE373PJwsvm0aBmDbYoQ1bhBaxJRjqe24pAOWiGI3GL7BfIzeZU05z3i6YgKPt
      rU6E+y1Sa4tgP+BQKczglG9BgBWY9AKLzwqguPtojAl51DtMnCEkYNLpYNUQx5F8
      lVezBSThAgMBAAECggEASkRF4FsWjyJDV91Y4nhsU6vpCCT/wCN8D/XoL9x3MOpB
      jzrUAc4PoIWGXvAkFwN8LkviemlX6+2n4bDF0FN4Ij+caflQ33ZPSRCW+3zdQVnW
      0MVmSorDTN3JqSvlWgI0wG3Kz8cKW2AejBR88YJbGbTgNiBXIZKVGkkWC/maULKa
      L2Omd5ZzF9qkWWoBk0GxrlME+V9726L6SJHIVESkPcbojXZPM06zmz84+Zzf0UdG
      pl+SocmQzo8yzhjXR8UHxtzfZbiiJZGCsWIxvizqM4OTaRfIBPhmmSNwpO7Vk3lD
      t6XISALqMbIHv/co5LOWs5WnyQBud28MV7RpCD/tawKBgQDnGuFSmilpMR6KqRrd
      O8amarseklND9NiWsUcXLYYHQkNClTtr5Gqtm3EbJbR0wLwUzL7PwgeuNmN6e6vz
      IreS7wd+uO3HeVVsFPeOpTlXzALMG6wuYN/Ipfe/T8v/vq9CkoZ6VDaDN6OdEBAq
      g6pAUQFL+uamLfrKcCiZtiOR/wKBgQDIFRhC5MyLaFlw3XumgAsh37tmlGOQ/T+K
      GAWXiRhzXW340EVezNUUXeJcGig4BgNVwXBvjYTk0unyIw72bUNrIeOqp4ZgO2vW
      NUgyDgIGKZuIIGI51FeezG6Qu2s93qZTYtdjH0M4ISKgrKOvY51fuXBNFSuC7BOq
      i5MQamuJHwKBgHGJhiszq6aPSCbtH1KTHGwDwXwqfRfEwWd/HqLnbZJBXpPmhvPh
      mvtBg5bHtlkpmv1I/XFKLMXM2KCDA54Gb1OTdQYvyjmWhX386wY8a+iTRMiLy9JZ
      K3gC+a0Wge1Z+/Zj0AdnOgTLH+l4y8hnOQwx/8YZNJltu2kbIwcpMV53AoGAFMzE
      meepL/DoI2iS+ysifSICHFbexurc2SFIK4mwBgY3cX9NRt6qZBSifIqnlbNiU17p
      rl8a6qLWeTqVyp5vPMroHQyPVp+2xS0C1VlJcpSOu6cKLxLZDQQZlmg1bNghmFeV
      JpPQbBxdujBYT9peON5RQ2IpBNI/9SHPZwx5I2cCgYBdCmby0loBi5lCbCXKV5c3
      mzlPpM6zR3aLYsFyknWRtLv3UPMGzOLsRDkwfQEMZWt/VFjXehajyqd8X7r3V0lX
      cz5LwmH0MSKDYgdzrJjvE3lPxDdKtmyPr5nnF/8/SF6Lawj7v6eGnpiZNqHC/wkK
      9EmCxWXJc/9GVWSZtEOz/Q==
      -----END PRIVATE KEY-----

#. 在 PC 上使用 python 创建一个基于 TLS 的 WebSocket 服务器。示例代码如下：

   .. code-block:: python

      #!/usr/bin/env python

      import asyncio
      import ssl
      import websockets

      host = '192.168.200.249'
      port = 8766
      server_ca = '/your_path/server_ca.crt'
      server_cert = '/your_path/server.crt'
      server_key = '/your_path/server.key'

      async def echo(websocket, path):
          async for message in websocket:
              await websocket.send(message)

      ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
      ssl_context.load_cert_chain(certfile=server_cert, keyfile=server_key)
      ssl_context.verify_mode = ssl.CERT_REQUIRED
      ssl_context.load_verify_locations(server_ca)

      start_server = websockets.serve(echo, host, port, ssl=ssl_context)

      asyncio.get_event_loop().run_until_complete(start_server)
      print(f"Server started at wss://{host}:{port}")

      asyncio.get_event_loop().run_forever()

   请修改上述代码中的 ``host`` 为 PC 的 IP 地址， ``server_ca``、 ``server_cert``、 ``server_key`` 为服务器端的 CA、证书、和私钥路径，并保存为 ``wss-server.py`` 文件，同时运行该程序。

   .. code-block:: python

      python wss-server.py

   说明：

   - 如果您的 PC 上没有安装 ``websockets`` 库，请使用 ``pip install websockets`` 安装。
   - 设置的 WebSocket 服务器端口为 ``8766``，您也可以设置为其它端口。

#. 配置 WebSocket 连接为相互鉴权。

   命令：

   .. code-block:: none

     AT+WSCFG=0,30,60,4096,3,0,0

   响应：

   .. code-block:: none

     OK

   说明：

   - 如果您想使用自己的客户端证书或者使用多套证书，请参考 :doc:`../Compile_and_Develop/How_to_update_pki_config`。

#. {IDF_TARGET_NAME} 设备作为客户端连接到 WebSocket 服务器。

   命令：

   .. code-block:: none

     AT+WSOPEN=0,"wss://192.168.200.249:8766"

   响应：

   .. code-block:: none

     +WS_CONNECTED:0

     OK

#. 发送 4 字节数据。

   命令：

   .. code-block:: none

     AT+WSSEND=0,4

   响应：

   .. code-block:: none

     OK

     >

   输入 4 字节数据，例如输入数据是 ``test``，之后 AT 将会输出以下信息。

   .. code-block:: none

     SEND OK

   说明：

   - 若输入的字节数目超过 ``AT+WSSEND`` 命令设定的长度 (n)，则系统会响应 ``busy p...``，并发送数据的前 n 个字节，发送完成后响应 ``SEND OK``。

#. 接收 4 字节数据。

   由于上面的 WebSocket 服务器是一个回显服务器，因此在数据发送后，服务器也会原样返回该数据，即 AT 会输出：

   .. code-block:: none

     +WS_DATA:0,4,test
