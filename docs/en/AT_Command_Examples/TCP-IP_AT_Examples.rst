TCP/IP AT Examples
==========================

:link_to_translation:`zh_CN:[中文]`

This document provides detailed command examples to illustrate how to utilize :doc:`../AT_Command_Set/TCP-IP_AT_Commands` on {IDF_TARGET_NAME}.

.. contents::
   :local:
   :depth: 1

{IDF_TARGET_NAME} as a TCP client in single connection
--------------------------------------------------

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

#. Query the device's IP address.

   Command:

   .. code-block:: none

     AT+CIPSTA?

   Response:

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   Note:

   - The query results you obtained may be different from those in the above response.

#. Connect the PC to the same router which {IDF_TARGET_NAME} is connected to.

   Use a network tool on the PC to create a TCP server. For example, the TCP server on PC is ``192.168.3.102``, and the port is ``8080``.

#. Connect {IDF_TARGET_NAME} to the TCP server as a client over TCP. The server's IP address is ``192.168.3.102``, and the port is ``8080``.

   Command:

   .. code-block:: none

     AT+CIPSTART="TCP","192.168.3.102",8080

   Response:

   .. code-block:: none

     CONNECT

     OK

#. Send 4 bytes of data.

   Command:

   .. code-block:: none

     AT+CIPSEND=4

   Response:

   .. code-block:: none

     OK

     >

   Input 4 bytes, for example, ``test``, then AT will respond the following message.

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   Note:

   - If the number of bytes inputted are more than the length (n) set by ``AT+CIPSEND``, the system will reply ``busy p...``, and send the first n bytes. And after sending the first n bytes, the system will reply ``SEND OK``.

#. Receive 4 bytes of data.

   Assume that the TCP server sends 4 bytes of data (data is ``test``), the system will prompt:

   .. code-block:: none

     +IPD,4:test

{IDF_TARGET_NAME} as a TCP server in multiple connections
----------------------------------------------------

When {IDF_TARGET_NAME} works as a TCP server, multiple connections should be enabled by :ref:`AT+CIPMUX=1 <cmd-MUX>` command, because in most cases more than one client needs to be connected to the {IDF_TARGET_NAME} server.

Below is an example showing how a TCP server is established when {IDF_TARGET_NAME} works in the softAP mode. If {IDF_TARGET_NAME} works as a station, you can set up a server in the same way mentioned above after connecting {IDF_TARGET_NAME} to the router.

#. Set the Wi-Fi mode to softAP.

   Command:

   .. code-block:: none

     AT+CWMODE=2

   Response:

   .. code-block:: none

     OK

#. Enable multiple connections.

   Command:

   .. code-block:: none

     AT+CIPMUX=1

   Response:

   .. code-block:: none

     OK

#. Set softAP.

   Command:

   .. code-block:: none

     AT+CWSAP="ESP32_softAP","1234567890",5,3

   Response:

   .. code-block:: none

     OK

#. Query softAP information.

   Command:

   .. code-block:: none

     AT+CIPAP?

   Response:

   .. code-block:: none

     AT+CIPAP?
     +CIPAP:ip:"192.168.4.1"
     +CIPAP:gateway:"192.168.4.1"
     +CIPAP:netmask:"255.255.255.0"

     OK

   Note:

   - The address you obtained may be different from that in the above response.

#. Set up a TCP server, the default port is ``333``.

   Command:

   .. code-block:: none

     AT+CIPSERVER=1

   Response:

   .. code-block:: none

     OK

#. Connect the PC to the {IDF_TARGET_NAME} softAP.

   .. figure:: ../../img/Connect-SoftAP.png
       :scale: 100 %
       :align: center
       :alt: Connect SoftAP

#. Use a network tool on PC to create a TCP client and connect it to the TCP server that {IDF_TARGET_NAME} has created.

#. Send 4 bytes of data to connection link 0.

   Command:

   .. code-block:: none

     AT+CIPSEND=0,4

   Response:

   .. code-block:: none

     OK

     >

   Input 4 bytes, for example, ``test``, then AT will respond the following messages.

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   Note:

   - If the number of bytes inputted are more than the length (n) set by ``AT+CIPSEND``, the system will reply ``busy p...``, and send the first n bytes. And after sending the first n bytes, the system will reply ``SEND OK``.

#. Receive 4 bytes of data from connection link 0.

   Assume that the TCP server sends 4 bytes of data (data is ``test``), the system will prompt:

   .. code-block:: none

     +IPD,0,4:test

#. Close TCP connection.

   Command:

   .. code-block:: none

     AT+CIPCLOSE=0

   Response:

   .. code-block:: none

     0,CLOSED

     OK

UDP transmission with fixed remote IP address and port
--------------------------------------------------------

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

#. Query the device's IP address.

   Command:

   .. code-block:: none

     AT+CIPSTA?

   Response:

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   Note:

   - The query results you obtained may be different from those in the above response.

#. Connect the PC to the same router which {IDF_TARGET_NAME} is connected to.

   Use a network tool on the PC to create UDP transmission. For example, the PC's IP address is ``192.168.3.102``, and the port is ``8080``.

#. Enable multiple connections.

   Command:

   .. code-block:: none

     AT+CIPMUX=1

   Response:

   .. code-block:: none

     OK

#. Create a UDP transmission. The connection link is 4, the remote host's IP address is ``192.168.3.102``, the remote port is ``8080``, the local port is ``1112``, and the mode is ``0``.

   .. Important::

     In UDP transmission, whether the remote IP address and port are fixed or not is determined by the ``mode`` parameter of `AT+CIPSTART`. If the parameter is 0, a specific connection link ID will be given to ensure that the remote IP address and port are fixed and the data sender and receiver will not be replaced by other devices.

   Command:

   .. code-block:: none

     AT+CIPSTART=4,"UDP","192.168.3.102",8080,1112,0

   Response:

   .. code-block:: none

     4,CONNECT

     OK

   Note:

   - ``"192.168.3.102"`` and ``8080`` are the remote IP address and port of UDP transmission on the remote side, i.e., the UDP configuration set by PC.
   - ``1112`` is the local port number of {IDF_TARGET_NAME}. You can define this port number, or else, a random port will be used.
   - ``0`` means that the remote IP address and port are fixed and cannot be changed. For example, when there is another PC creating a UDP entity and sending data to {IDF_TARGET_NAME} port 1112, {IDF_TARGET_NAME} will still receive the data from UDP port 1112, and if the AT command ``AT+CIPSEND=4,X`` is used, the data will still be sent to the first PC end. However, if the parameter is not set as ``0``, the data will be sent to the new PC.

#. Send 7 bytes of data to connection link 4.

   Command:

   .. code-block:: none

     AT+CIPSEND=4,7

   Response:

   .. code-block:: none

     OK

     >

   Input 7 bytes, for example, ``abcdefg``, then AT will respond the following messages.

   .. code-block:: none

     Recv 7 bytes

     SEND OK

   Note:

   - If the number of bytes inputted are more than the length (n) set by ``AT+CIPSEND``, the system will reply ``busy p...``, and send the first n bytes. And after sending the first n bytes, the system will reply ``SEND OK``.

#. Receive 4 bytes of data from connection link 4.

   Assume that the PC sends 4 bytes of data (data is ``test``), the system will prompt:

   .. code-block:: none

     +IPD,4,4:test

#. Close UDP connection link 4.

   Command:

   .. code-block:: none

     AT+CIPCLOSE=4

   Response:

   .. code-block:: none

     4,CLOSED

     OK

UDP transmission with changeable remote IP address and port
------------------------------------------------------------

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

#. Query the device's IP address.

   Command:

   .. code-block:: none

     AT+CIPSTA?

   Response:

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   Note:

   - The query results you obtained may be different from those in the above response.

#. Connect the PC to the same router which {IDF_TARGET_NAME} is connected to.

   Use a network tool on the PC to create UDP transmission. For example, the PC's IP address is ``192.168.3.102``, and the port is ``8080``.

#. Enable single connections.

   Command:

   .. code-block:: none

     AT+CIPMUX=0

   Response:

   .. code-block:: none

     OK

#. Create a UDP transmission. The remote host's IP address is ``192.168.3.102``, the remote port is ``8080``, the local port is ``1112``, and the mode is ``2``.

   Command:

   .. code-block:: none

     AT+CIPSTART="UDP","192.168.3.102",8080,1112,2

   Response:

   .. code-block:: none

     CONNECT

     OK

   Note:

   - ``"192.168.3.102"`` and `8080` are the remote IP address and port of UDP transmission on the remote side, i.e., the UDP configuration set by PC.
   - ``1112`` is the local port number of {IDF_TARGET_NAME}. You can define this port number, or else, a random port will be used.
   - ``2`` means the opposite terminal of UDP transmission can be changed. The remote IP address and port will be automatically changed to those of the last UDP connection to {IDF_TARGET_NAME}.

#. Send 4 bytes of data.

   Command:

   .. code-block:: none

     AT+CIPSEND=4

   Response:

   .. code-block:: none

     OK

     >

   Input 4 bytes, for example, ``test``, then AT will respond the following messages.

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   Note:

   - If the number of bytes inputted are more than the length (n) set by ``AT+CIPSEND``, the system will reply ``busy p...``, and send the first n bytes. And after sending the first n bytes, the system will reply ``SEND OK``.

#. Send data to any other UDP terminal. For example, you can send 4 bytes of data with the remote host's IP address as ``192.168.3.103`` and the remote port as ``1000``.

   If you want to send data to any other UDP terminal, please designate the IP address and port of the target terminal in the command.

   Command:

   .. code-block:: none

     AT+CIPSEND=4,"192.168.3.103",1000

   Response:

   .. code-block:: none

     OK

     >

   Input 4 bytes, for example, ``test``, then AT will respond the following messages.

   .. code-block:: none

     Recv 4 bytes

     SEND OK

#. Receive 4 bytes of data.

   Assume that the PC sends 4 bytes of data (data is ``test``), the system will prompt:

   .. code-block:: none

     +IPD,4:test

#. Close UDP connection.

   Command:

   .. code-block:: none

     AT+CIPCLOSE

   Response:

   .. code-block:: none

     CLOSED

     OK

{IDF_TARGET_NAME} as an SSL client in single connection
--------------------------------------------------

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

#. Query the device's IP address.

   Command:

   .. code-block:: none

     AT+CIPSTA?

   Response:

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   Note:

   - The query results you obtained may be different from those in the above response.

#. Connect the PC to the same router which {IDF_TARGET_NAME} is connected to.

#. Use the OpenSSL command on the PC to create an SSL server. For example, the SSL server on PC is ``192.168.3.102``, and the port is ``8070``.

   Command:

   .. code-block:: none

     openssl s_server -cert /home/esp-at/components/customized_partitions/raw_data/server_cert/server_cert.crt -key /home/esp-at/components/customized_partitions/raw_data/server_key/server.key -port 8070

   Response:

   .. code-block:: none

     ACCEPT

#. Connect the {IDF_TARGET_NAME} to the SSL server as a client over SSL. The server's IP address is ``192.168.3.102``, and the port is ``8070``.

   Command:

   .. code-block:: none

     AT+CIPSTART="SSL","192.168.3.102",8070

   Response:

   .. code-block:: none

     CONNECT

     OK

#. Send 4 bytes of data.

   Command:

   .. code-block:: none

     AT+CIPSEND=4

   Response:

   .. code-block:: none

     OK

     >

   Input 4 bytes, for example, ``test``, then AT will respond the following message.

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   Note:

   - If the number of bytes inputted are more than the length (n) set by ``AT+CIPSEND``, the system will reply ``busy p...``, and send the first n bytes. And after sending the first n bytes, the system will reply ``SEND OK``.

#. Receive 4 bytes of data.

   Assume that the SSL server sends 4 bytes of data (data is ``test``), the system will prompt:

   .. code-block:: none

     +IPD,4:test

{IDF_TARGET_NAME} as an SSL server in multiple connections
-----------------------------------------------------

When {IDF_TARGET_NAME} works as an SSL server, multiple connections should be enabled by :ref:`AT+CIPMUX=1 <cmd-MUX>` command, because in most cases more than one client needs to be connected to the {IDF_TARGET_NAME} server.

Below is an example showing how an SSL server is established when {IDF_TARGET_NAME} works in the softAP mode. If {IDF_TARGET_NAME} works as a station, after connecting to the router, follow the steps for establishing a connection to an SSL server in this example.

#. Set the Wi-Fi mode to softAP.

   Command:

   .. code-block:: none

     AT+CWMODE=2

   Response:

   .. code-block:: none

     OK

#. Enable multiple connections.

   Command:

   .. code-block:: none

     AT+CIPMUX=1

   Response:

   .. code-block:: none

     OK

#. Configure the {IDF_TARGET_NAME} softAP.

   Command:

   .. code-block:: none

     AT+CWSAP="ESP32_softAP","1234567890",5,3

   Response:

   .. code-block:: none

     OK

#. Query softAP information.

   Command:

   .. code-block:: none

     AT+CIPAP?

   Response:

   .. code-block:: none

     AT+CIPAP?
     +CIPAP:ip:"192.168.4.1"
     +CIPAP:gateway:"192.168.4.1"
     +CIPAP:netmask:"255.255.255.0"

     OK

   Note:

   - The address you obtained may be different from that in the above response.

#. Set up an SSL server.

   Command:

   .. code-block:: none

     AT+CIPSERVER=1,8070,"SSL"

   Response:

   .. code-block:: none

     OK

#. Connect the PC to the {IDF_TARGET_NAME} softAP.

   .. figure:: ../../img/Connect-SoftAP.png
       :scale: 100 %
       :align: center
       :alt: Connect SoftAP

#. Use the OpenSSL command on PC to create an SSL client and connect it to the SSL server that {IDF_TARGET_NAME} has created.

   Command:

   .. code-block:: none

     openssl s_client -host 192.168.4.1 -port 8070

   Response on the {IDF_TARGET_NAME}:

   .. code-block:: none

     CONNECT

#. Send 4 bytes of data to connection link 0.

   Command:

   .. code-block:: none

     AT+CIPSEND=0,4

   Response:

   .. code-block:: none

     OK

     >

   Input 4 bytes, for example, ``test``, then AT will respond the following messages.

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   Note:

   - If the number of bytes inputted are more than the length (n) set by ``AT+CIPSEND``, the system will reply ``busy p...``, and send the first n bytes. And after sending the first n bytes, the system will reply ``SEND OK``.

#. Receive 4 bytes of data from connection link 0.

   Assume that the SSL server sends 4 bytes of data (data is ``test``), the system will prompt:

   .. code-block:: none

     +IPD,0,4:test

#. Close SSL connection.

   Command:

   .. code-block:: none

     AT+CIPCLOSE=0

   Response:

   .. code-block:: none

     0,CLOSED

     OK

{IDF_TARGET_NAME} as an SSL client to create a single connection with mutual authentication
---------------------------------------------------------------------------------------

In this example, the certificate used is the default certificate in esp-at. You can also use your own certificate:

  - To use your own SSL client certificate, replace the default certificate according to the :doc:`../Compile_and_Develop/How_to_update_pki_config` document.
  - To use your own SSL server certificate, replace the SSL server certificate path below with your own certificate path.

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

   - You can check whether the SNTP time matches the real-time time to determine whether the SNTP server you set takes effect.

#. Query the device's IP address.

   Command:

   .. code-block:: none

     AT+CIPSTA?

   Response:

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   Note:

   - The query results you obtained may be different from those in the above response.

#. Connect the PC to the same router which {IDF_TARGET_NAME} is connected to.

#. Use the OpenSSL command on the PC to create an SSL server. For example, the SSL server on PC is ``192.168.3.102``, and the port is ``8070``.

   Command:

   .. code-block:: none

     openssl s_server -CAfile /home/esp-at/components/customized_partitions/raw_data/server_ca/server_ca.crt -cert /home/esp-at/components/customized_partitions/raw_data/server_cert/server_cert.crt -key /home/esp-at/components/customized_partitions/raw_data/server_key/server.key -port 8070 -verify_return_error -verify_depth 1 -Verify 1

   Response on the {IDF_TARGET_NAME}:

   .. code-block:: none

     CONNECT

   Note:

   - The certificate path in the command can be adjusted according to the location of your certificate.

#. The {IDF_TARGET_NAME} sets up the SSL client mutual authentication configuration.

   Command:

   .. code-block:: none

     AT+CIPSSLCCONF=3,0,0

   Response:

   .. code-block:: none

     OK

#. Connect the {IDF_TARGET_NAME} to the SSL server as a client over SSL. The server's IP address is ``192.168.3.102``, and the port is ``8070``.

   Command:

   .. code-block:: none

     AT+CIPSTART="SSL","192.168.3.102",8070

   Response:

   .. code-block:: none

     CONNECT

     OK

#. Send 4 bytes of data.

   Command:

   .. code-block:: none

     AT+CIPSEND=4

   Response:

   .. code-block:: none

     OK

     >

   Input 4 bytes, for example, ``test``, then AT will respond the following message.

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   Note:

   - If the number of bytes inputted are more than the length (n) set by ``AT+CIPSEND``, the system will reply ``busy p...``, and send the first n bytes. And after sending the first n bytes, the system will reply ``SEND OK``.

#. Receive 4 bytes of data.

   Assume that the SSL server sends 4 bytes of data (data is ``test``), the system will prompt:

   .. code-block:: none

     +IPD,4:test

{IDF_TARGET_NAME} as an SSL server to create multiple connections with mutual authentication
--------------------------------------------------------------------------------------------

When {IDF_TARGET_NAME} works as an SSL server, multiple connections should be enabled by :ref:`AT+CIPMUX=1 <cmd-MUX>` command, because in most cases more than one client needs to be connected to the {IDF_TARGET_NAME} server.

Below is an example showing how an SSL server is established when {IDF_TARGET_NAME} works in the station mode. If {IDF_TARGET_NAME} works as a softAP, refer to the example of `{IDF_TARGET_NAME} as an SSL server in multiple connections`_.

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

#. Query the device's IP address.

   Command:

   .. code-block:: none

     AT+CIPSTA?

   Response:

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   Note:

   - The query results you obtained may be different from those in the above response.

#. Enable multiple connections.

   Command:

   .. code-block:: none

     AT+CIPMUX=1

   Response:

   .. code-block:: none

     OK

#. Set up an SSL server.

   Command:

   .. code-block:: none

     AT+CIPSERVER=1,8070,"SSL",1

   Response:

   .. code-block:: none

     OK

#. Connect the PC to the {IDF_TARGET_NAME} softAP.

   .. figure:: ../../img/Connect-SoftAP.png
       :scale: 100 %
       :align: center
       :alt: Connect SoftAP

#. Use the OpenSSL command on PC to create an SSL client and connect it to the SSL server that {IDF_TARGET_NAME} has created.

   Command:

   .. code-block:: none

     openssl s_client -CAfile /home/esp-at/components/customized_partitions/raw_data/client_ca/client_ca_00.crt -cert /home/esp-at/components/customized_partitions/raw_data/client_cert/client_cert_00.crt -key /home/esp-at/components/customized_partitions/raw_data/client_key/client_key_00.key -host 192.168.3.112 -port 8070

   Response on the {IDF_TARGET_NAME}:

   .. code-block:: none

     0,CONNECT

#. Send 4 bytes of data to connection link 0.

   Command:

   .. code-block:: none

     AT+CIPSEND=0,4

   Response:

   .. code-block:: none

     OK

     >

   Input 4 bytes, for example, ``test``, then AT will respond the following messages.

   .. code-block:: none

     Recv 4 bytes

     SEND OK

   Note:

   - If the number of bytes inputted are more than the length (n) set by ``AT+CIPSEND``, the system will reply ``busy p...``, and send the first n bytes. And after sending the first n bytes, the system will reply ``SEND OK``.

#. Receive 4 bytes of data from connection link 0.

   Assume that the SSL server sends 4 bytes of data (data is ``test``), the system will prompt:

   .. code-block:: none

     +IPD,0,4:test

#. Close SSL connection.

   Command:

   .. code-block:: none

     AT+CIPCLOSE=0

   Response:

   .. code-block:: none

     0,CLOSED

     OK

#. Close SSL server.

   Command:

   .. code-block:: none

     AT+CIPSERVER=0

   Response:

   .. code-block:: none

     OK

UART Wi-Fi passthrough transmission when the {IDF_TARGET_NAME} works as a TCP client in single connection
---------------------------------------------------------------------------------------------------

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

#. Query the device's IP address.

   Command:

   .. code-block:: none

     AT+CIPSTA?

   Response:

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   Note:

   - The query results you obtained may be different from those in the above response.

#. Connect the PC to the same router which {IDF_TARGET_NAME} is connected to.

   Use a network tool on the PC to create a TCP server. For example, the TCP server on PC is ``192.168.3.102``, and the port is ``8080``.

#. Connect the {IDF_TARGET_NAME} to the TCP server as a TCP client over TCP. The server's IP address is ``192.168.3.102``, and the port is ``8080``.

   Command:

   .. code-block:: none

     AT+CIPSTART="TCP","192.168.3.102",8080

   Response:

   .. code-block:: none

     CONNECT

     OK

#. Enable the UART Wi-Fi :term:`Passthrough Receiving Mode`.

   Command:

   .. code-block:: none

     AT+CIPMODE=1

   Response:

   .. code-block:: none

     OK

#. Enter the UART Wi-Fi :term:`Passthrough Mode` and send data.

   Command:

   .. code-block:: none

     AT+CIPSEND

   Response:

   .. code-block:: none

     OK

     >

#. Stop sending data.

   When receiving a packet that contains only ``+++``,  the UART Wi-Fi passthrough transmission process will be stopped. Then please wait at least 1 second before sending the next AT command. Please note that if you input ``+++`` directly by typing, the ``+++`` may not be recognized as three consecutive ``+`` because of the prolonged typing duration. For more details, please refer to :ref:`[Passthrough Mode Only] +++ <cmd-PLUS>`.

   .. Important::

     The aim of ending the packet with ``+++`` is to exit :term:`Passthrough Mode` and to accept normal AT commands, while TCP still remains connected. However, you can also use command ``AT+CIPSEND`` to go back into :term:`Passthrough Mode`.

#. Exit the UART Wi-Fi :term:`Passthrough Receiving Mode`.

   Command:

   .. code-block:: none

     AT+CIPMODE=0

   Response:

   .. code-block:: none

     OK

#. Close TCP connection.

   Command:

   .. code-block:: none

     AT+CIPCLOSE

   Response:

   .. code-block:: none

     CLOSED

     OK

UART Wi-Fi passthrough transmission when the {IDF_TARGET_NAME} works as a TCP server
------------------------------------------------------------------------------------

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

#. Enable multiple connections.

   Command:

   .. code-block:: none

     AT+CIPMUX=1

   Response:

   .. code-block:: none

     OK

   Note:

   - TCP server can be created only in multiple connections.

#. Set the maximum number of TCP server connections to 1.

   Command:

   .. code-block:: none

     AT+CIPSERVERMAXCONN=1

   Response:

   .. code-block:: none

     OK

   Note:

   - The passthrough mode is point-to-point, so the maximum number of connections to the TCP server can only be 1.

#. Create TCP server.

   Command:

   .. code-block:: none

     AT+CIPSERVER=1,8080

   Response:

   .. code-block:: none

     OK

   Note:

   - Set the TCP server port to 8080. You can also set it to other port.

#. Query the device's IP address.

   Command:

   .. code-block:: none

     AT+CIPSTA?

   Response:

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   Note:

   - The query results you obtained may be different from those in the above response.

#. Connect the PC to the {IDF_TARGET_NAME} TCP server

   Connect the PC to the same router which {IDF_TARGET_NAME} is connected to.

   Use a network tool on the PC to create a TCP client and connect to the {IDF_TARGET_NAME} TCP server. The remote address is ``192.168.3.112``, and the port is ``8080``.

   AT Response:

   .. code-block:: none

     0,CONNECT

#. Enable the UART Wi-Fi :term:`Passthrough Receiving Mode`.

   Command:

   .. code-block:: none

     AT+CIPMODE=1

   Response:

   .. code-block:: none

     OK

#. Enter the UART Wi-Fi :term:`Passthrough Mode` and send data.

   Command:

   .. code-block:: none

     AT+CIPSEND

   Response:

   .. code-block:: none

     OK

     >

#. Stop sending data.

   When receiving a packet that contains only ``+++``,  the UART Wi-Fi passthrough transmission process will be stopped. Then please wait at least 1 second before sending the next AT command. Please note that if you input ``+++`` directly by typing, the ``+++`` may not be recognized as three consecutive ``+`` because of the prolonged typing duration. For more details, please refer to :ref:`[Passthrough Mode Only] +++ <cmd-PLUS>`.

   .. Important::

     The aim of ending the packet with ``+++`` is to exit :term:`Passthrough Mode` and to accept normal AT commands, while TCP still remains connected. However, you can also use command ``AT+CIPSEND`` to go back into :term:`Passthrough Mode`.

#. Exit the UART Wi-Fi :term:`Passthrough Receiving Mode`.

   Command:

   .. code-block:: none

     AT+CIPMODE=0

   Response:

   .. code-block:: none

     OK

#. Close TCP connection.

   Command:

   .. code-block:: none

     AT+CIPCLOSE

   Response:

   .. code-block:: none

     CLOSED

     OK

UART Wi-Fi passthrough transmission when the {IDF_TARGET_NAME} works as a softAP in UDP transparent transmission
---------------------------------------------------------------------------------------------------------

#. Set the Wi-Fi mode to softAP.

   Command:

   .. code-block:: none

     AT+CWMODE=2

   Response:

   .. code-block:: none

     OK

#. Set softAP.

   Command:

   .. code-block:: none

     AT+CWSAP="ESP32_softAP","1234567890",5,3

   Response:

   .. code-block:: none

     OK

#. Connect the PC to the {IDF_TARGET_NAME} softAP.

   .. figure:: ../../img/Connect-SoftAP.png
       :scale: 100 %
       :align: center
       :alt: Connect SoftAP

#. Create a UDP endpoint.

   Use a network tool on PC to create a UDP endpoint. For example, the PC's IP address is ``192.168.4.2`` and the port is ``8080``.

#. Create a UDP transmission between {IDF_TARGET_NAME} and the PC with a fixed remote IP address and port. The remote host's IP address is ``192.168.4.2``, the remote port is ``8080``, the local port is ``2233``, and the mode is ``0``.

   Command:

   .. code-block:: none

     AT+CIPSTART="UDP","192.168.4.2",8080,2233,0

   Response:

   .. code-block:: none

     CONNECT

     OK

#. Enter the UART Wi-Fi :term:`Passthrough Receiving Mode`.

   Command:

   .. code-block:: none

     AT+CIPMODE=1

   Response:

   .. code-block:: none

     OK

#. Enter the UART Wi-Fi :term:`Passthrough Mode` and send data.

   Command:

   .. code-block:: none

     AT+CIPSEND

   Response:

   .. code-block:: none

     OK

     >

#. Stop sending data.

   When receiving a packet that contains only ``+++``,  the UART Wi-Fi passthrough transmission process will be stopped. Then please wait at least 1 second before sending the next AT command. Please note that if you input ``+++`` directly by typing, the ``+++`` may not be recognized as three consecutive ``+`` because of the prolonged typing duration. For more details, please refer to :ref:`[Passthrough Mode Only] +++ <cmd-PLUS>`.

   .. Important::

     The aim of ending the packet with ``+++`` is to exit :term:`Passthrough Mode` and to accept normal AT commands, while TCP still remains connected. However, you can also use command ``AT+CIPSEND`` to go back into :term:`Passthrough Mode`.

#. Exit the UART Wi-Fi :term:`Passthrough Receiving Mode`.

   Command:

   .. code-block:: none

     AT+CIPMODE=0

   Response:

   .. code-block:: none

     OK

#. Close TCP connection.

   Command:

   .. code-block:: none

     AT+CIPCLOSE

   Response:

   .. code-block:: none

     CLOSED

     OK

.. _example-passive_recv:

{IDF_TARGET_NAME} obtains socket data in passive receiving mode
-----------------------------------------------------------------------

When a large amount of network data is expected to be received and the MCU cannot process it timely, you can refer to this example and use the passive receive data mode.

.. _using-passive-mode:

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

#. Query the device's IP address.

   Command:

   .. code-block:: none

     AT+CIPSTA?

   Response:

   .. code-block:: none

    +CIPSTA:ip:"192.168.3.112"
    +CIPSTA:gateway:"192.168.3.1"
    +CIPSTA:netmask:"255.255.255.0"

    OK

   Note:

   - The query results you obtained may be different from those in the above response.

#. Connect the PC to the same router which {IDF_TARGET_NAME} is connected to.

   Use a network tool on the PC to create a TCP server. For example, the TCP server on PC is ``192.168.3.102``, and the port is ``8080``.

#. Connect {IDF_TARGET_NAME} to the TCP server as a client over TCP. The server's IP address is ``192.168.3.102``, and the port is ``8080``.

   Command:

   .. code-block:: none

     AT+CIPSTART="TCP","192.168.3.102",8080

   Response:

   .. code-block:: none

     CONNECT

     OK

#. {IDF_TARGET_NAME} sets the socket receiving mode to passive mode.

   Command:

   .. code-block:: none

     AT+CIPRECVTYPE=1

   Response:

   .. code-block:: none

     OK

#. The TCP server sends 4 bytes of data (data is ``test``).

   Note:

   - The device replies with ``+IPD,4``. If it receives server data again later, please refer to the note section of :ref:`AT+CIPRECVTYPE <cmd-CIPRECVTYPE>` for whether it will reply with ``+IPD,``.

#. {IDF_TARGET_NAME} obtains socket data length in passive receiving mode.

   Command:

   .. code-block:: none

     AT+CIPRECVLEN?

   Response:

   .. code-block:: none

     +CIPRECVLEN:4
     OK

#. {IDF_TARGET_NAME} obtains socket data in passive receiving mode.

   Command:

   .. code-block:: none

     AT+CIPRECVDATA=4

   Response:

   .. code-block:: none

     +CIPRECVDATA:4,test
     OK

.. _example-mdns:

{IDF_TARGET_NAME} enables mDNS function, PC connects to the device's TCP server using a domain name
-----------------------------------------------------------------------------------------------------

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

   PC can discover the {IDF_TARGET_NAME} device only when they are in the same LAN.

#. Use an mDNS tool on the PC to enable service discovery. For example, use `avahi-browse <https://linux.die.net/man/1/avahi-browse>`_ on linux (use `Bonjour <https://developer.apple.com/bonjour/>`_ on macOS or Windows).

   Command:

   .. code-block:: none

     sudo avahi-browse -a -r

#. {IDF_TARGET_NAME} device enables mDNS function.

   Command:

   .. code-block:: none

     AT+MDNS=1,"espressif","_printer",35,"my_instance","_tcp",2,"product","my_printer","firmware_version","AT-V3.4.1.0"

   Response:

   .. code-block:: none

     OK

   Note:

   - This command enables mDNS function and specifies the device instance name as ``my_instance``, service type as ``_printer``, port as ``35``, product as ``my_printer``, and firmware version as ``AT-V3.4.1.0``.

#. (Optional) PC discovers the {IDF_TARGET_NAME} device.

   The PC's ``avahi-browse`` tool will display:

   .. code-block:: none

     ...
     + enx000ec6dd4ebf IPv4 my_instance                                   UNIX Printer         local
     = enx000ec6dd4ebf IPv4 my_instance                                   UNIX Printer         local
       hostname = [espressif.local]
       address = [192.168.200.90]
       port = [35]
       txt = ["product=my_printer" "firmware_version=AT-V3.4.1.0"]

   Note:

   - This step is not necessary, it is just to verify the mDNS function of the {IDF_TARGET_NAME} device.

#. {IDF_TARGET_NAME} device enables multiple connections.

   Command:

   .. code-block:: none

     AT+CIPMUX=1

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} device runs a TCP server on port ``35``.

   Command:

   .. code-block:: none

     AT+CIPSERVER=1,35

   Response:

   .. code-block:: none

     OK

#. Use a TCP tool (For example, use `nc <https://netcat.sourceforge.net/>`_ on Linux or macOS, use `ncat <https://nmap.org/ncat/>`_ on Windows) on the PC to connect to {IDF_TARGET_NAME} device's TCP server using the domain name.

   Command:

   .. code-block:: none

     nc espressif.local 35

   {IDF_TARGET_NAME} device responds:

   .. code-block:: none

     0,CONNECT

   Note:

   - After the connection is established, data transmission can immediately take place between PC and {IDF_TARGET_NAME} device.

#. {IDF_TARGET_NAME} device disables mDNS function.

   Command:

   .. code-block:: none

     AT+MDNS=0

   Response:

   .. code-block:: none

     OK

   Note:

   - Disabling mDNS function can reduce the device's power consumption to some extent.
