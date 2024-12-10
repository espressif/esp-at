WebSocket AT Examples
==========================

:link_to_translation:`en:[English]`

This document shows how to use :doc:`../AT_Command_Set/websocket_at_commands` on {IDF_TARGET_NAME} with detailed examples.

.. _example-websocket:

.. contents::
   :local:
   :depth: 1

.. Important::
  The default AT firmware does not support the WebSocket functionality. If you need {IDF_TARGET_NAME} to support WebSocket functionality, please refer to :ref:`introduction <cmd-ws-intro>`.

.. _example-websocket-tcp:

WebSocket Connection over TCP
-----------------------------

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

#. Connect the PC to the same router which {IDF_TARGET_NAME} is connected to.

#. Create a WebSocket server over TCP on PC using python. The example code is as follows:

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

   Please modify the code above by replacing the ``host`` with the IP address of your PC, and save it as ``ws-server.py`` file. Then run the program.

   .. code-block:: python

      python ws-server.py

   Note:

   - If you do not have the ``websockets`` library installed on your PC, please install it using ``pip install websockets``.
   - The WebSocket server port is set to ``8765`` by default, but you can also set it to a different port.

#. Connect {IDF_TARGET_NAME} device as a client to the WebSocket server.

   Command:

   .. code-block:: none

     AT+WSOPEN=0,"ws://192.168.200.249:8765"

   Response:

   .. code-block:: none

     +WS_CONNECTED:0

     OK

#. Send 4 bytes of data.

   Command:

   .. code-block:: none

     AT+WSSEND=0,4

   Response:

   .. code-block:: none

     OK

     >

   Input 4 bytes (e.g., ``test``), and then AT will respond with the following message.

   .. code-block:: none

     SEND OK

   Note:

   - If the input exceeds the length (n) specified by ``AT+CIPSEND``, the system will respond with ``busy p...``, and send the first n bytes. Once those bytes are sent, the system will respond with ``SEND OK``.

#. Receive 4 bytes of data.

   Since the above WebSocket server is an echo server, after the data is sent, the server will also return the data as it is, so AT will output:

   .. code-block:: none

     +WS_DATA:0,4,test

.. _example-websocket-tls:

WebSocket Connection over TLS (Mutual Authentication)
-----------------------------------------------------

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

#. Set the SNTP server.

   Command:

   .. code-block:: none

     AT+CIPSNTPCFG=1,8,"cn.ntp.org.cn","ntp.sjtu.edu.cn"

   Response:

   .. code-block:: none

     OK

   Note:

   - You can set the SNTP server according to your country's time zone.

#. Query the SNTP time.

   Command:

   .. code-block:: none

     AT+CIPSNTPTIME?

   Response:

   .. code-block:: none

     +CIPSNTPTIME:Mon Oct 18 20:12:27 2021 
     OK

   Note:

   - You can verify if the SNTP server is functioning correctly by checking whether the SNTP-synchronized time matches the actual current time.

#. Connect the PC to the same router which {IDF_TARGET_NAME} is connected to.

#. Get the CA, certificate, and private key of the WebSocket server.

   You can use the ``openssl`` tool to generate the CA, certificate, and private key. If you encounter any difficulties, consider using the following configuration for testing:

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

#. Create a WebSocket server over TLS on PC using python. The example code is as follows:

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

   Please modify the code above by replacing the ``host`` with the IP address of your PC, and the ``server_ca``, ``server_cert``, and ``server_key`` with the paths to the CA, certificate, and private key of the server. Save the modified code as ``wss-server.py`` and run the program.

   .. code-block:: python

      python wss-server.py

   Note:

   - If you do not have the ``websockets`` library installed on your PC, please install it using ``pip install websockets``.
   - The WebSocket server port is set to ``8766``, but you can also set it to a different port.

#. Configure the WebSocket connection for mutual authentication.

   Command:

   .. code-block:: none

     AT+WSCFG=0,30,60,4096,3,0,0

   Response:

   .. code-block:: none

     OK

   Note:

   - If you want to use your own certificate or use multiple sets of certificates, please refer to :doc:`../Compile_and_Develop/How_to_update_pki_config`.

#. Connect {IDF_TARGET_NAME} device as a client to the WebSocket server.

   Command:

   .. code-block:: none

     AT+WSOPEN=0,"wss://192.168.200.249:8766"

   Response:

   .. code-block:: none

     +WS_CONNECTED:0

     OK

#. Send 4 bytes of data.

   Command:

   .. code-block:: none

     AT+WSSEND=0,4

   Response:

   .. code-block:: none

     OK

     >

   Input 4 bytes, for example, ``test``, then AT will respond the following message.

   .. code-block:: none

     SEND OK

   Note:

   - If the number of bytes inputted are more than the length (n) set by ``AT+CIPSEND``, the system will reply ``busy p...``, and send the first n bytes. And after sending the first n bytes, the system will reply ``SEND OK``.

#. Receive 4 bytes of data.

   Since the above WebSocket server is an echo server, after the data is sent, the server will also return the data as it is, so AT will output:

   .. code-block:: none

     +WS_DATA:0,4,test
