Bluetooth LE AT Examples
==========================

:link_to_translation:`zh_CN:[中文]`

This document provides an introduction and detailed command examples to illustrate how to utilize :doc:`../AT_Command_Set/BLE_AT_Commands` on ESP devices.

.. contents::
   :local:
   :depth: 1

Bluetooth LE AT Example
----------------------------------

Below is an example of using two ESP32 development boards, one as a Bluetooth LE server (only as Bluetooth LE server role), the other one as a Bluetooth LE client (only as Bluetooth LE client role). The example shows how to use Bluetooth LE functions with AT commands.

.. Important::
  * The ESP32 Bluetooth LE server needs to download a ``ble_data.bin`` into Flash to provide Bluetooth LE services.

    * To learn how to generate a ``ble_data.bin``, please refer to ``tools/README.md``.
    * The download address of the ``ble_data.bin`` is the address of ``ble_data`` in ``at_customize.csv``.

  * In the step, the operations starting with ``ESP32 Bluetooth LE server`` only need to be executed at ESP32 Bluetooth LE server, and the operations starting with ``ESP32 Bluetooth LE client`` only need to be executed at ESP32 Bluetooth LE client.

#. Bluetooth LE initialization.

   ESP32 Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLEINIT=2

   Response:

   .. code-block:: none

     OK

   ESP32 Bluetooth LE client:
  
   Command:

   .. code-block:: none

     AT+BLEINIT=1

   Response:

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE server gets Bluetooth LE address.

   Command:

   .. code-block:: none

     AT+BLEADDR?

   Response:

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   Note:

   - The address you obtain may be different from that in the above response. Keep yours handy as you will need it in one of the following steps.

#. ESP32 Bluetooth LE server creates services.

   Command:

   .. code-block:: none

     AT+BLEGATTSSRVCRE

   Response:

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE server starts services.

   Command:

   .. code-block:: none

     AT+BLEGATTSSRVSTART

   Response:

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE server discovers characteristics.

   Command:

   .. code-block:: none

     AT+BLEGATTSCHAR?

   Response:

   .. code-block:: none

     +BLEGATTSCHAR:"char",1,1,0xC300,0x02
     +BLEGATTSCHAR:"desc",1,1,1,0x2901
     +BLEGATTSCHAR:"char",1,2,0xC301,0x02
     +BLEGATTSCHAR:"desc",1,2,1,0x2901
     +BLEGATTSCHAR:"char",1,3,0xC302,0x08
     +BLEGATTSCHAR:"desc",1,3,1,0x2901
     +BLEGATTSCHAR:"char",1,4,0xC303,0x04
     +BLEGATTSCHAR:"desc",1,4,1,0x2901
     +BLEGATTSCHAR:"char",1,5,0xC304,0x08
     +BLEGATTSCHAR:"char",1,6,0xC305,0x10
     +BLEGATTSCHAR:"desc",1,6,1,0x2902
     +BLEGATTSCHAR:"char",1,7,0xC306,0x20
     +BLEGATTSCHAR:"desc",1,7,1,0x2902
     +BLEGATTSCHAR:"char",1,8,0xC307,0x02
     +BLEGATTSCHAR:"desc",1,8,1,0x2901
     +BLEGATTSCHAR:"char",2,1,0xC400,0x02
     +BLEGATTSCHAR:"desc",2,1,1,0x2901
     +BLEGATTSCHAR:"char",2,2,0xC401,0x02
     +BLEGATTSCHAR:"desc",2,2,1,0x2901

     OK

#. ESP32 Bluetooth LE server starts advertising, then the ESP32 Bluetooth LE client starts scanning and lasts for 3 s. 

   ESP32 Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLEADVSTART

   Response:

   .. code-block:: none

     OK

   ESP32 Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLESCAN=1,3

   Response:

   .. code-block:: none

     OK
     +BLESCAN:"5b:3b:6c:51:90:49",-87,02011a020a0c0aff4c001005071c3024dc,,1
     +BLESCAN:"c4:5b:be:93:ec:66",-84,0201060303111809095647543147572d58020a03,,0
     +BLESCAN:"24:0a:c4:d6:e4:46",-29,,,0

   Note:

   - The scan results you obtain may be different from those in the above response.

#. Establish the Bluetooth LE connection.
  
   ESP32 Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLECONN=0,"24:0a:c4:d6:e4:46"

   Response:

   .. code-block:: none

     +BLECONN:0,"24:0a:c4:d6:e4:46"
     
     OK

   Note:

   - When entering the above command, replace the address with your ESP Bluetooth LE server address.
   - If the Bluetooth LE connection is established successfully, message ``+BLECONN:0,"24:0a:c4:d6:e4:46`` will be prompted.
   - If the Bluetooth LE connection is broken, message ``+BLECONN:0,-1`` will be prompted.

#. ESP32 Bluetooth LE client discovers services.
  
   Command:

   .. code-block:: none

     AT+BLEGATTCPRIMSRV=0

   Response:

   .. code-block:: none

     +BLEGATTCPRIMSRV:0,1,0x1801,1
     +BLEGATTCPRIMSRV:0,2,0x1800,1
     +BLEGATTCPRIMSRV:0,3,0xA002,1
     +BLEGATTCPRIMSRV:0,4,0xA003,1

    OK

   Note:

   - When discovering services, the ESP32 Bluetooth LE client will get two more default services (UUID: 0x1800 and 0x1801) than what the ESP32 Bluetooth LE server will get.
   - So, for the same service, the <srv_index> received by the ESP32 Bluetooth LE client equals the <srv_index> received by the ESP32 Bluetooth LE server + 2.
   - For example, for service 0xA002, the <srv_index> queried on the ESP32 Bluetooth LE client is 3, if the ESP32 Bluetooth LE server is queried through the command :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>`, then <srv_index> is 1.

#. ESP32 Bluetooth LE client discovers characteristics.

   Command:

   .. code-block:: none

     AT+BLEGATTCCHAR=0,3

   Response:

   .. code-block:: none

     +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
     +BLEGATTCCHAR:"desc",0,3,1,1,0x2901
     +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
     +BLEGATTCCHAR:"desc",0,3,2,1,0x2901
     +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
     +BLEGATTCCHAR:"desc",0,3,3,1,0x2901
     +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
     +BLEGATTCCHAR:"desc",0,3,4,1,0x2901
     +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
     +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
     +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
     +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
     +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
     +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02
     +BLEGATTCCHAR:"desc",0,3,8,1,0x2901
     
     OK

#. ESP32 Bluetooth LE client reads a characteristic.

   Command:

   .. code-block:: none

     AT+BLEGATTCRD=0,3,1

   Response:

   .. code-block:: none

     +BLEGATTCRD:0,1,0

     OK

   Note:

   - Please note that the target characteristic's property has to support the read operation.
   - If the ESP32 Bluetooth LE client reads the characteristic successfully, message ``+READ:0,"7c:df:a1:b3:8d:de"`` will be prompted on the ESP32 Bluetooth LE Server side.

#. ESP32 Bluetooth LE client writes a characteristic.

   Command:

   .. code-block:: none

     AT+BLEGATTCWR=0,3,3,,2

   Response:

   .. code-block:: none

     >

   The symbol ``>`` indicates that AT is ready for receiving serial data and you can enter data now. When the requirement of data length determined by the parameter ``<length>`` is met, the writing starts.

   .. code-block:: none

     OK

   Note:

   - If the ESP32 Bluetooth LE client writes the characteristic successfully, message ``+WRITE:<conn_index>,<srv_index>,<char_index>,[<desc_index>],<len>,<value>`` will be prompted on the ESP32 Bluetooth LE server side.

#. Indicate a characteristic.

   ESP32 Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLEGATTSIND=0,1,7,3

   Response:

   .. code-block:: none

     >

   The symbol ``>`` indicates that AT is ready for receiving serial data and you can enter data now. When the requirement of data length determined by the parameter ``<length>`` is met, the indication starts.

   .. code-block:: none

     OK

   Note:

   - If the ESP32 Bluetooth LE client receives the indication, message ``+INDICATE:<conn_index>,<srv_index>,<char_index>,<len>,<value>`` will be prompted.
   - For the same service, the <srv_index> on the ESP32 Bluetooth LE client side equals the <srv_index> on the ESP32 Bluetooth LE server side + 2.

   ESP32 Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLEGATTCWR=0,3,7,1,2 

   Response:

   .. code-block:: none

     >

   The symbol ``>`` indicates that AT is ready for receiving serial data and you can enter data now. When the requirement of data length determined by the parameter ``<length>`` is met, the writing starts.

   .. code-block:: none

     OK

   Note:

   - If the ESP32 Bluetooth LE client writes the descriptor successfully, message ``+WRITE:<conn_index>,<srv_index>,<char_index>,<desc_index>,<len>,<value>`` will be prompted on the ESP32 Bluetooth LE server side.

UART-Bluetooth LE Passthrough Mode Example
-----------------------------------------------

Below is an example of using two ESP32 development boards, one as a Bluetooth LE server (only as Bluetooth LE server role), the other one as a Bluetooth LE client (only as Bluetooth LE client role). The example shows how to build Bluetooth LE SPP (Serial Port Profile, UART-Bluetooth LE passthrough mode) with AT commands.

.. Important::
  In the step, the operations starting with ``ESP32 Bluetooth LE server`` only need to be executed at ESP32 Bluetooth LE server, and the operations starting with ``ESP32 Bluetooth LE client`` only need to be executed at ESP32 Bluetooth LE client.

#. Bluetooth LE initialization.

   ESP32 Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLEINIT=2

   Response:

   .. code-block:: none

     OK

   ESP32 Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLEINIT=1

   Response:

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE server creates services.

   Command:

   .. code-block:: none

     AT+BLEGATTSSRVCRE

   Response:

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE server starts services.

   Command:

   .. code-block:: none

     AT+BLEGATTSSRVSTART

   Response:

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE server gets Bluetooth LE address.

   Command:

   .. code-block:: none

     AT+BLEADDR?

   Response:

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   Note:

   - The address you obtain may be different from that in the above response. Keep yours handy as you will need it in one of the following steps.

#. Set Bluetooth LE advertising data.

   Command:

   .. code-block:: none

     AT+BLEADVDATA="0201060A09457370726573736966030302A0"

   Response:

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE server starts advertising.

   Command:

   .. code-block:: none

     AT+BLEADVSTART

   Response:

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE client enables a scanning for three seconds.

   Command:

   .. code-block:: none

     AT+BLESCAN=1,3

   Response:

   .. code-block:: none

     OK
     +BLESCAN:"24:0a:c4:d6:e4:46",-78,0201060a09457370726573736966030302a0,,0
     +BLESCAN:"45:03:cb:ac:aa:a0",-62,0201060aff4c001005441c61df7d,,1
     +BLESCAN:"24:0a:c4:d6:e4:46",-26,0201060a09457370726573736966030302a0,,0

   Note:

   - The scan results you obtain may be different from those in the above response.

#. Establish the Bluetooth LE connection.

   ESP32 Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLECONN=0,"24:0a:c4:d6:e4:46"

   Response:

   .. code-block:: none

     +BLECONN:0,"24:0a:c4:d6:e4:46"
     
     OK

   Note:

   - When entering the above command, replace the address your ESP Bluetooth LE server address.
   - If the Bluetooth LE connection is established successfully, message ``+BLECONN:0,"24:0a:c4:d6:e4:46`` will be prompted.
   - If the Bluetooth LE connection is broken, message ``+BLECONN:0,-1`` will be prompted.

#. ESP32 Bluetooth LE server discovers local services.

   Command:

   .. code-block:: none

     AT+BLEGATTSSRV?

   Response:

   .. code-block:: none

     +BLEGATTSSRV:1,1,0xA002,1
     +BLEGATTSSRV:2,1,0xA003,1
     
     OK

#. ESP32 Bluetooth LE server discovers local characteristics.

   Command:

   .. code-block:: none

     AT+BLEGATTSCHAR?

   Response:

   .. code-block:: none

     +BLEGATTSCHAR:"char",1,1,0xC300,0x02
     +BLEGATTSCHAR:"desc",1,1,1,0x2901
     +BLEGATTSCHAR:"char",1,2,0xC301,0x02
     +BLEGATTSCHAR:"desc",1,2,1,0x2901
     +BLEGATTSCHAR:"char",1,3,0xC302,0x08
     +BLEGATTSCHAR:"desc",1,3,1,0x2901
     +BLEGATTSCHAR:"char",1,4,0xC303,0x04
     +BLEGATTSCHAR:"desc",1,4,1,0x2901
     +BLEGATTSCHAR:"char",1,5,0xC304,0x08
     +BLEGATTSCHAR:"char",1,6,0xC305,0x10
     +BLEGATTSCHAR:"desc",1,6,1,0x2902
     +BLEGATTSCHAR:"char",1,7,0xC306,0x20
     +BLEGATTSCHAR:"desc",1,7,1,0x2902
     +BLEGATTSCHAR:"char",1,8,0xC307,0x02
     +BLEGATTSCHAR:"desc",1,8,1,0x2901
     +BLEGATTSCHAR:"char",2,1,0xC400,0x02
     +BLEGATTSCHAR:"desc",2,1,1,0x2901
     +BLEGATTSCHAR:"char",2,2,0xC401,0x02
     +BLEGATTSCHAR:"desc",2,2,1,0x2901

     OK

#. ESP32 Bluetooth LE client discovers services.

   Command:

   .. code-block:: none

     AT+BLEGATTCPRIMSRV=0

   Response:

   .. code-block:: none

     +BLEGATTCPRIMSRV:0,1,0x1801,1
     +BLEGATTCPRIMSRV:0,2,0x1800,1
     +BLEGATTCPRIMSRV:0,3,0xA002,1
     +BLEGATTCPRIMSRV:0,4,0xA003,1

    OK

   Note:

   - When discovering services, the ESP32 Bluetooth LE client will get two more default services (UUID: 0x1800 and 0x1801) than what the ESP32 Bluetooth LE server will get.
   - So, for the same service, the <srv_index> received by the ESP32 Bluetooth LE client equals the <srv_index> received by the ESP32 Bluetooth LE server + 2.
   - For example, for service 0xA002, the <srv_index> queried on the ESP32 Bluetooth LE client is 3, if the ESP32 Bluetooth LE server is queried through the command :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>`, then <srv_index> is 1.

#. ESP32 Bluetooth LE client discovers characteristics.

   Command:

   .. code-block:: none

     AT+BLEGATTCCHAR=0,3

   Response:

   .. code-block:: none

     +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
     +BLEGATTCCHAR:"desc",0,3,1,1,0x2901
     +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
     +BLEGATTCCHAR:"desc",0,3,2,1,0x2901
     +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
     +BLEGATTCCHAR:"desc",0,3,3,1,0x2901
     +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
     +BLEGATTCCHAR:"desc",0,3,4,1,0x2901
     +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
     +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
     +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
     +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
     +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
     +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02
     +BLEGATTCCHAR:"desc",0,3,8,1,0x2901
     
     OK

#. ESP32 Bluetooth LE client Configures Bluetooth LE SPP.

   Set a characteristic that enables writing permission to TX channel for sending data. Set another characteristic that supports notification or indication to RX channel for receiving data.

   Command:

   .. code-block:: none

     AT+BLESPPCFG=1,3,5,3,7

   Response:

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE client enables Bluetooth LE SPP.

   Command:

   .. code-block:: none

     AT+BLESPP

   Response:

   .. code-block:: none

     OK

     >

   This response indicates that AT has entered Bluetooth LE SPP mode and can send and receive data.

   Note:

   - After the ESP32 Bluetooth LE client enables Bluetooth LE SPP, data received from serial port will be transmitted to the Bluetooth LE server directly.

#. ESP32 Bluetooth LE server Configures Bluetooth LE SPP.

   Set a characteristic that supports notification or indication to TX channel for sending data. Set another characteristic that enables writing permission to RX channel for receiving data.

   Command:

   .. code-block:: none

     AT+BLESPPCFG=1,1,7,1,5

   Response:

   .. code-block:: none

     OK

#. ESP32 Bluetooth LE server enables Bluetooth LE SPP.

   Command:

   .. code-block:: none

     AT+BLESPP

   Response:

   .. code-block:: none

     OK

     >

   This response indicates that AT has entered Bluetooth LE SPP mode and can send and receive data.

   Note:

   - After the ESP32 Bluetooth LE server enables Bluetooth LE SPP, the data received from serial port will be transmitted to the Bluetooth LE client directly.
   - If the ESP32 Bluetooth LE client does not enable Bluetooth LE SPP first, or uses other device as Bluetooth LE client, then the Bluetooth LE client needs to listen to the notification or indication first. For example, if the ESP32 Bluetooth LE client does not enable Bluetooth LE SPP first, then it should use command ``AT+BLEGATTCWR=0,3,7,1,1`` to enable listening function first, so that the ESP32 Bluetooth LE server can transmit successfully.
   - For the same service, the <srv_index> on the ESP32 Bluetooth LE client side equals the <srv_index> on the ESP32 Bluetooth LE server side + 2.
