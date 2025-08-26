Bluetooth LE AT Examples
==========================

:link_to_translation:`zh_CN:[中文]`

This document provides an introduction and detailed command examples to illustrate how to utilize :doc:`../AT_Command_Set/BLE_AT_Commands` on {IDF_TARGET_NAME}.

.. contents::
   :local:
   :depth: 1

Introduction
------------

ESP-AT currently only supports **Bluetooth LE 4.2 protocol specification**, and the description in this document is only for **Bluetooth LE 4.2 protocol specification**. Please refer to `Core Specification 4.2 <https://www.bluetooth.com/specifications/specs/core-specification-4-2/>`__ for more details.

Bluetooth LE protocol architecture
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Bluetooth LE protocol stack is divided into several layers from bottom to top: ``Physical Layer (PHY)``, ``Link Layer (LL)``, ``Host Controller Interface (HCI)``, ``Logical Link Control and Adaptation Protocol Layer (L2CAP)``, ``Attribute Protocol (ATT)``, ``Security Manager Protocol (SMP)``, ``Generic Attribute Profile (GATT)``, ``Generic Access Profile (GAP)``.

- ``PHY``: the PHY layer is responsible for transmitting and receiving packets of information on the physical channel. Bluetooth LE uses ``40`` RF channels. Frequency Range: 2402 MHz to 2480 MHz.
- ``LL``: the LL layer is responsible for the creation, modification and release of logical links (and, if required, their associated logical transports), as well as the update of parameters related to physical links between devices. It controls the Link Layer state machine in one of the five states of ``standby``, ``advertising``, ``scanning``, ``initiating`` and ``connected``.
- ``HCI``: the HCI layer provides a standardized interface to host and controller. This layer can be implemented by software ``API`` or controlled by hardware interfaces ``UART``, ``SPI`` and ``USB``.
- ``L2CAP``: the L2CAP layer is responsible for protocol multiplexing capability, segmentation, and reassembly operation for data exchanged between the host and the protocol stack.
- ``ATT``: the ATT layer implements the peer-to-peer protocol between an attribute server and an attribute client. The ATT client sends commands, requests, and confirmations to the ATT server. The ATT server sends responses, notifications and indications to the client.
- ``SMP``: the SMP layer is the peer-to-peer protocol used to generate encryption keys and identity keys. The SMP also manages storage of the encryption keys and identity keys and is responsible for generating random addresses and resolving random addresses to known device identities.
- ``GATT``: the GATT layer represents the functionality of the attribute server and, optionally, the attribute client. The profile describes the hierarchy of services, characteristics and attributes used in the attribute server. The layer provides interfaces for discovering, reading, writing and indicating of service characteristics and attributes.
- ``GAP``: the GAP represents the base functionality common to all Bluetooth devices such as modes and access procedures used by the transports, protocols and application profiles. GAP services include device discovery, connection modes, security, authentication, association models and service discovery.

Bluetooth LE role division
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There are different roles in different levels of the Bluetooth LE protocol architecture. These divisions are not affected by each other.

- ``LL``: devices can be divided into ``master`` and ``slave``, the slave advertises, and the master initiates a connection.
- ``GAP``: GAP defines four specific roles: ``broadcaster``, ``observer``, ``peripheral``, and ``central``.
- ``GATT``: devices can be divided into ``server`` and ``client``.

.. Important::

  - The ``Bluetooth LE server`` and ``Bluetooth LE client`` described in this document are both ``GATT`` layer roles.
  - At present, ESP-AT supports the existence of ``Bluetooth LE server`` and ``Bluetooth LE client`` at the same time.
  - No matter ESP-AT is initialized as Bluetooth LE server or Bluetooth LE client, the maximum number of devices connected at the same time is ``3``.

GATT is actually an attribute transmission protocol, which can be regarded as an application layer protocol for attribute transmission. The structure of this attribute is very simple. It is composed of ``services``, each ``service`` is composed of different numbers of ``characteristics``, and each ``characteristic`` is composed of many other elements.

``GATT server`` and ``GATT client`` roles exist after the Bluetooth LE connection is established. The GATT server stores the data transported over the Attribute Protocol and accepts Attribute Protocol requests, commands and confirmations from the GATT client. In short, the end that provides data is called ``GATT server``, and the end that accesses data is called ``GATT client``.

Bluetooth LE client reads and write services
----------------------------------------------

Below is an example of using two {IDF_TARGET_NAME} development boards, one as a Bluetooth LE server (only as Bluetooth LE server role), the other one as a Bluetooth LE client (only as Bluetooth LE client role). The example shows how to use Bluetooth LE functions with AT commands.

.. Important::

   In the following steps, the operations starting with ``{IDF_TARGET_NAME} Bluetooth LE server`` only need to be executed at {IDF_TARGET_NAME} Bluetooth LE server, and the operations starting with ``{IDF_TARGET_NAME} Bluetooth LE client`` only need to be executed at {IDF_TARGET_NAME} Bluetooth LE client.

#. Bluetooth LE initialization.

   {IDF_TARGET_NAME} Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLEINIT=2

   Response:

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE client:
  
   Command:

   .. code-block:: none

     AT+BLEINIT=1

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE server gets Bluetooth LE address.

   Command:

   .. code-block:: none

     AT+BLEADDR?

   Response:

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   Note:

   - The address you obtain may be different from that in the above response. Keep yours handy as you will need it in one of the following steps.

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE server creates services.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSRVCRE
  
     Response:
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE server starts services.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSRVSTART
  
     Response:
  
     .. code-block:: none
  
       OK

#. {IDF_TARGET_NAME} Bluetooth LE server discovers characteristics.

   Command:

   .. code-block:: none

     AT+BLEGATTSCHAR?

   Response:

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

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

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300
       +BLEGATTSCHAR:"char",1,2,0xC301
       +BLEGATTSCHAR:"char",1,3,0xC302
       +BLEGATTSCHAR:"char",1,4,0xC303
       +BLEGATTSCHAR:"char",1,5,0xC304
       +BLEGATTSCHAR:"char",1,6,0xC305
       +BLEGATTSCHAR:"char",1,7,0xC306
       +BLEGATTSCHAR:"char",1,8,0xC307
       +BLEGATTSCHAR:"char",2,1,0xC400
       +BLEGATTSCHAR:"char",2,2,0xC401

       OK

#. {IDF_TARGET_NAME} Bluetooth LE server starts advertising, then the {IDF_TARGET_NAME} Bluetooth LE client starts scanning and lasts for 3 s. 

   {IDF_TARGET_NAME} Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLEADVSTART

   Response:

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE client:

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
  
   {IDF_TARGET_NAME} Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLECONN=0,"24:0a:c4:d6:e4:46"

   Response:

   .. code-block:: none

     +BLECONN:0,"24:0a:c4:d6:e4:46"
     
     OK

   Note:

   - When entering the above command, replace the address with your {IDF_TARGET_NAME} Bluetooth LE server address.
   - If the Bluetooth LE connection is established successfully, message ``+BLECONN:0,"24:0a:c4:d6:e4:46`` will be prompted.
   - If the Bluetooth LE connection is broken, message ``+BLECONN:0,-1`` will be prompted.

#. {IDF_TARGET_NAME} Bluetooth LE client discovers services.
  
   Command:

   .. code-block:: none

     AT+BLEGATTCPRIMSRV=0

   Response:

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1801,1
       +BLEGATTCPRIMSRV:0,2,0x1800,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1800,1
       +BLEGATTCPRIMSRV:0,2,0x1801,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   Note:

   - When discovering services, the {IDF_TARGET_NAME} Bluetooth LE client will get two more default services (UUID: 0x1800 and 0x1801) than what the {IDF_TARGET_NAME} Bluetooth LE server will get. So, for the same service, the <srv_index> received by the {IDF_TARGET_NAME} Bluetooth LE client equals the <srv_index> received by the {IDF_TARGET_NAME} Bluetooth LE server + 2. For example, for service 0xA002, the <srv_index> queried on the {IDF_TARGET_NAME} Bluetooth LE client is 3, if the {IDF_TARGET_NAME} Bluetooth LE server is queried through the command :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>`, then <srv_index> is 1.

#. {IDF_TARGET_NAME} Bluetooth LE client discovers characteristics.

   Command:

   .. code-block:: none

     AT+BLEGATTCCHAR=0,3

   Response:

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

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

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
       +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
       +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
       +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
       +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
       +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
       +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
       +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
       +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
       +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02

       OK

#. {IDF_TARGET_NAME} Bluetooth LE client reads a characteristic.

   Command:

   .. code-block:: none

     AT+BLEGATTCRD=0,3,1

   Response:

   .. code-block:: none

     +BLEGATTCRD:0,1,0

     OK

   Note:

   - Please note that the target characteristic's property has to support the read operation.
   - If the {IDF_TARGET_NAME} Bluetooth LE client reads the characteristic successfully, message ``+READ:0,"7c:df:a1:b3:8d:de"`` will be prompted on the {IDF_TARGET_NAME} Bluetooth LE Server side.

#. {IDF_TARGET_NAME} Bluetooth LE client writes a characteristic.

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

   - If the {IDF_TARGET_NAME} Bluetooth LE client writes the characteristic successfully, message ``+WRITE:<conn_index>,<srv_index>,<char_index>,[<desc_index>],<len>,<value>`` will be prompted on the {IDF_TARGET_NAME} Bluetooth LE server side.

#. Indicate a characteristic.

   {IDF_TARGET_NAME} Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLEGATTCWR=0,3,7,1,2

   Response:

   .. code-block:: none

     >

   The symbol ``>`` indicates that AT is ready for receiving serial data and you can enter data now. When the requirement of data length determined by the parameter ``<length>`` is met, the writing starts.

   To receive data from {IDF_TARGET_NAME} Bluetooth LE server (through the ``notify`` or the ``indicate`` method), the {IDF_TARGET_NAME} Bluetooth LE client needs to register with the server in advance. Write the value ``0x0001`` to use the ``notify`` method, and ``0x0002`` to use the ``indicate`` method. This example writes the ``0x0002`` to use the ``indicate`` method.

   .. code-block:: none

     OK

   Note:

   - If the {IDF_TARGET_NAME} Bluetooth LE client writes the descriptor successfully, message ``+WRITE:<conn_index>,<srv_index>,<char_index>,<desc_index>,<len>,<value>`` will be prompted on the {IDF_TARGET_NAME} Bluetooth LE server side.

   {IDF_TARGET_NAME} Bluetooth LE server:

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

   - If the {IDF_TARGET_NAME} Bluetooth LE client receives the indication, message ``+INDICATE:<conn_index>,<srv_index>,<char_index>,<len>,<value>`` will be prompted.
   - For the same service, the <srv_index> on the {IDF_TARGET_NAME} Bluetooth LE client side equals the <srv_index> on the {IDF_TARGET_NAME} Bluetooth LE server side + 2.
   - For the permissions of the characteristics in the services, please refer to :doc:`../Compile_and_Develop/How_to_customize_BLE_services`.

.. only:: esp32 or esp32c3 or esp32c5 or esp32c6

  Bluetooth LE server read and write services
  ---------------------------------------------
  
  Below is an example of using two {IDF_TARGET_NAME} development boards, one as a Bluetooth LE server (only as Bluetooth LE server role), the other one as a Bluetooth LE client (only as Bluetooth LE client role). The example shows how to establish a Bluetooth LE connection, as well as the read and write characteristics of the server and client settings, and notification characteristics.
  
  .. Important::
    In the step, the operations starting with ``{IDF_TARGET_NAME} Bluetooth LE server`` only need to be executed at {IDF_TARGET_NAME} Bluetooth LE server, and the operations starting with ``{IDF_TARGET_NAME} Bluetooth LE client`` only need to be executed at {IDF_TARGET_NAME} Bluetooth LE client.
  
  #. Bluetooth LE initialization.
  
     {IDF_TARGET_NAME} Bluetooth LE server:
  
     Command:
  
     .. code-block:: none
  
       AT+BLEINIT=2
  
     Response:
  
     .. code-block:: none
  
       OK
  
     {IDF_TARGET_NAME} Bluetooth LE client:
  
     Command:
  
     .. code-block:: none
  
       AT+BLEINIT=1
  
     Response:
  
     .. code-block:: none
  
       OK

  .. only:: esp32 or esp32c3

    #. {IDF_TARGET_NAME} Bluetooth LE server creates services.
    
       Command:
    
       .. code-block:: none
    
         AT+BLEGATTSSRVCRE
    
       Response:
    
       .. code-block:: none
    
         OK
    
    #. {IDF_TARGET_NAME} Bluetooth LE server starts services.
    
       Command:
    
       .. code-block:: none
    
         AT+BLEGATTSSRVSTART
    
       Response:
    
       .. code-block:: none
    
         OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE server gets its MAC address.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEADDR?
  
     Response:
  
     .. code-block:: none
  
       +BLEADDR:"24:0a:c4:d6:e4:46"
       OK
  
     Note:
  
     - The address you obtain may be different from that in the above response. Keep yours handy as you will need it in one of the following steps.
  
  #. {IDF_TARGET_NAME} Bluetooth LE server sets advertising parameters.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEADVPARAM=50,50,0,0,7,0,,
  
     Response:
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE server sets advertising data.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEADVDATA="0201060A09457370726573736966030302A0"
  
     Response:
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE server starts advertising.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEADVSTART
  
     Response:
  
     .. code-block:: none
  
       OK

  .. only:: esp32 or esp32c3

    #. {IDF_TARGET_NAME} Bluetooth LE client creates services.
    
       Command:
    
       .. code-block:: none
    
         AT+BLEGATTSSRVCRE
    
       Response:
    
       .. code-block:: none
    
         OK
    
    #. {IDF_TARGET_NAME} Bluetooth LE client starts services.
    
       Command:
    
       .. code-block:: none
    
         AT+BLEGATTSSRVSTART
    
       Response:
    
       .. code-block:: none
    
         OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE client gets Bluetooth LE address.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEADDR?
  
     Response:
  
     .. code-block:: none
  
       +BLEADDR:"24:0a:c4:03:a7:4e"
       OK
  
     Note:
  
     - The address you obtain may be different from that in the above response. Keep yours handy as you will need it in one of the following steps.
  
  #. {IDF_TARGET_NAME} Bluetooth LE client enables a scanning for three seconds.
  
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
    
     {IDF_TARGET_NAME} Bluetooth LE client:
  
     Command:
  
     .. code-block:: none
  
       AT+BLECONN=0,"24:0a:c4:d6:e4:46"
  
     Response:
  
     .. code-block:: none
  
       +BLECONN:0,"24:0a:c4:d6:e4:46"
  
       OK
  
     Note:
  
     - When entering the above command, replace the address with your {IDF_TARGET_NAME} Bluetooth LE server address.
     - If the Bluetooth LE connection is established successfully, message ``+BLECONN:0,"24:0a:c4:d6:e4:46`` will be prompted.
     - If the Bluetooth LE connection is broken, message ``+BLECONN:0,-1`` will be prompted.
  
     {IDF_TARGET_NAME} Bluetooth LE server:
  
     Command:
  
     .. code-block:: none
  
       AT+BLECONN=0,"24:0a:c4:03:a7:4e"
  
     Response:
  
     .. code-block:: none
  
       +BLECONN:0,"24:0a:c4:03:a7:4e"
  
       OK
  
     Note:
  
     - When entering the above command, replace the address with your {IDF_TARGET_NAME} Bluetooth LE server address.
     - If the Bluetooth LE connection is established successfully, the message ``OK`` will be prompted and the message ``+BLECONN:0,"24:0a:c4:03:a7:4e`` will not be prompted.
     - If the Bluetooth LE connection is broken, the message ``ERROR`` will be prompted and the message ``+BLECONN:0,-1`` will not be prompted.
  
  #. {IDF_TARGET_NAME} Bluetooth LE client discovers local services.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSRV?
  
     Response:
  
     .. code-block:: none
  
       +BLEGATTSSRV:1,1,0xA002,1
       +BLEGATTSSRV:2,1,0xA003,1
       
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE client discovers local characteristics.
  
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
  
  #. {IDF_TARGET_NAME} Bluetooth LE server discovers primary services.
  
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
  
     - When discovering services, the {IDF_TARGET_NAME} Bluetooth LE server will get two more default services (UUID: 0x1800 and 0x1801) than what the {IDF_TARGET_NAME} Bluetooth LE client will get. So, for the same service, the <srv_index> received by the {IDF_TARGET_NAME} Bluetooth LE server equals the <srv_index> received by the {IDF_TARGET_NAME} Bluetooth LE client + 2. For example, for service 0xA002, the <srv_index> queried on the {IDF_TARGET_NAME} Bluetooth LE client is 3, if the {IDF_TARGET_NAME} Bluetooth LE server is queried through the command :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>`, then <srv_index> is 1.
  
  #. {IDF_TARGET_NAME} Bluetooth LE server discovers  primary characteristics.
  
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
  
  #. {IDF_TARGET_NAME} Bluetooth LE client sets characteristics.
  
     Select the service characteristic that supports the write operation (characteristic) to set the characteristic.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSETATTR=1,8,,1
  
     Response:
  
     .. code-block:: none
  
       >
  
     Command:
  
     .. code-block:: none
  
       Write 1 byte ``9``
  
     Response:
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE server reads characteristics.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTCRD=0,3,8,
  
     Response:
  
     .. code-block:: none
  
       +BLEGATTCRD:0,1,9
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE client write characteristics.
  
     Select the service characteristic that supports the write operation to write the characteristics.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTCWR=0,3,6,1,2
  
     Response:
  
     .. code-block:: none
  
       >
  
     Command:
  
     .. code-block:: none
  
       Write 2 bytes ``12``
  
     Response:
  
     .. code-block:: none
  
       OK
  
     Note:
  
     - If the Bluetooth LE server successfully writes the service characteristic value, the Bluetooth LE client will prompt ``+WRITE:0,1,6,1,2,12``.
  
  #. {IDF_TARGET_NAME} Bluetooth LE client notify characteristics.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSNTFY=0,1,6,10
  
     Response:
  
     .. code-block:: none
  
       >
  
     Command:
  
     .. code-block:: none
  
       Write 10 bytes ``1234567890``
  
     Response:
  
     .. code-block:: none
  
       OK
  
     Note:
  
     - If the {IDF_TARGET_NAME} Bluetooth LE client's notify characteristic is successfully sent to the server, the Bluetooth LE server ``+NOTIFY:0,3,6,10,1234567890`` will be prompt.

Encrypt Bluetooth LE connection
----------------------------------

Below is an example of using two {IDF_TARGET_NAME} development boards, one as a Bluetooth LE server (only as Bluetooth LE server role), the other one as a Bluetooth LE client (only as Bluetooth LE client role). The example shows how to encrypt Bluetooth LE connection.

.. Important::

  - In the following steps, the operations starting with ``{IDF_TARGET_NAME} Bluetooth LE server`` only need to be executed at {IDF_TARGET_NAME} Bluetooth LE server, and the operations starting with ``{IDF_TARGET_NAME} Bluetooth LE client`` only need to be executed at {IDF_TARGET_NAME} Bluetooth LE client.
  - ``Encryption`` and ``bonding`` are two different concepts. ``bonding`` is just a long-term key stored locally after successful encryption.
  - ESP-AT allows a maximum of ``10`` devices to be bonded.

#. Bluetooth LE initialization.

   {IDF_TARGET_NAME} Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLEINIT=2

   Response:

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLEINIT=1

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE server gets Bluetooth LE address.

   Command:

   .. code-block:: none

     AT+BLEADDR?

   Response:

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   Note:

   - The address you obtain may be different from that in the above response. Keep yours handy as you will need it in one of the following steps.

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE server creates services.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSRVCRE
  
     Response:
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE server starts services.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSRVSTART
  
     Response:
  
     .. code-block:: none
  
       OK

#. {IDF_TARGET_NAME} Bluetooth LE server discovers characteristics.

   Command:

   .. code-block:: none

     AT+BLEGATTSCHAR?

   Response:

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

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

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300
       +BLEGATTSCHAR:"char",1,2,0xC301
       +BLEGATTSCHAR:"char",1,3,0xC302
       +BLEGATTSCHAR:"char",1,4,0xC303
       +BLEGATTSCHAR:"char",1,5,0xC304
       +BLEGATTSCHAR:"char",1,6,0xC305
       +BLEGATTSCHAR:"char",1,7,0xC306
       +BLEGATTSCHAR:"char",1,8,0xC307
       +BLEGATTSCHAR:"char",2,1,0xC400
       +BLEGATTSCHAR:"char",2,2,0xC401

       OK

#. {IDF_TARGET_NAME} Bluetooth LE server starts advertising, then the {IDF_TARGET_NAME} Bluetooth LE client starts scanning and lasts for 3 s. 

   {IDF_TARGET_NAME} Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLEADVSTART

   Response:

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE client:

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

   {IDF_TARGET_NAME} Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLECONN=0,"24:0a:c4:d6:e4:46"

   Response:

   .. code-block:: none

     +BLECONN:0,"24:0a:c4:d6:e4:46"

     OK

   Note:

   - When entering the above command, replace the address with your {IDF_TARGET_NAME} Bluetooth LE server address.
   - If the Bluetooth LE connection is established successfully, message ``+BLECONN:0,"24:0a:c4:d6:e4:46`` will be prompted.
   - If the Bluetooth LE connection is broken, message ``+BLECONN:0,-1`` will be prompted.

#. {IDF_TARGET_NAME} Bluetooth LE client discovers services.

   Command:

   .. code-block:: none

     AT+BLEGATTCPRIMSRV=0

   Response:

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1801,1
       +BLEGATTCPRIMSRV:0,2,0x1800,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1800,1
       +BLEGATTCPRIMSRV:0,2,0x1801,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   Note:

   - When discovering services, the {IDF_TARGET_NAME} Bluetooth LE client will get two more default services (UUID: 0x1800 and 0x1801) than what the {IDF_TARGET_NAME} Bluetooth LE server will get. So, for the same service, the <srv_index> received by the {IDF_TARGET_NAME} Bluetooth LE client equals the <srv_index> received by the {IDF_TARGET_NAME} Bluetooth LE server + 2. For example, for service 0xA002, the <srv_index> queried on the {IDF_TARGET_NAME} Bluetooth LE client is 3, if the {IDF_TARGET_NAME} Bluetooth LE server is queried through the command :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>`, then <srv_index> is 1.

#. {IDF_TARGET_NAME} Bluetooth LE client discovers characteristics.

   Command:

   .. code-block:: none

     AT+BLEGATTCCHAR=0,3

   Response:

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

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

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
       +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
       +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
       +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
       +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
       +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
       +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
       +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
       +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
       +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02

       OK

#. Set Bluetooth LE encryption parameters. Set ``auth_req`` to ``SC_MITM_BOND``, server's ``iocap`` to ``KeyboardOnly``, client's ``iocap`` to ``KeyboardDisplay``, ``key_size`` to ``16``, ``init_key`` to ``3``, ``rsp_key`` to ``3``.

   {IDF_TARGET_NAME} Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLESECPARAM=13,2,16,3,3

   Response:

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLESECPARAM=13,4,16,3,3

   Response:

   .. code-block:: none

     OK

   Note:

   - In this example, {IDF_TARGET_NAME} Bluetooth LE server enters the pairing code and {IDF_TARGET_NAME} Bluetooth LE client displays the pairing code.
   - ESP-AT supports ``Legacy Pairing`` and ``Secure Connections`` encryption methods, but the latter has a higher priority. If the peer also supports ``Secure Connections``, then ``Secure Connections`` will be used for encryption.

#. {IDF_TARGET_NAME} Bluetooth LE client initiates encryption request.

   Command:

   .. code-block:: none

     AT+BLEENC=0,3

   Response:

   .. code-block:: none

     OK

   Note:

   If the {IDF_TARGET_NAME} Bluetooth LE server successfully receives the encryption request, message ``+BLESECREQ:0`` will be prompted on the {IDF_TARGET_NAME} Bluetooth LE server side.

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE server responds to the pairing request.

    Command:

    .. code-block:: none

      AT+BLEENCRSP=0,1

    Response:

    .. code-block:: none

      OK

    Note:

    - If the {IDF_TARGET_NAME} Bluetooth LE client successfully receives the pairing response, a 6-digit pairing code will generate on the {IDF_TARGET_NAME} Bluetooth LE client side. 
    - In this example, message ``+BLESECNTFYKEY:0,793718`` will be prompted on the {IDF_TARGET_NAME} Bluetooth LE client side. Pairing code is ``793718``.

#. {IDF_TARGET_NAME} Bluetooth LE server replies pairing code.

   Command:

   .. code-block:: none

     AT+BLEKEYREPLY=0,793718

   Response:

   .. code-block:: none

     OK

   After running this command, there will be corresponding messages prompt on both the {IDF_TARGET_NAME} Bluetooth LE server and the {IDF_TARGET_NAME} Bluetooth LE client.

   {IDF_TARGET_NAME} Bluetooth LE server:

   .. code-block:: none

     +BLESECKEYTYPE:0,16
     +BLESECKEYTYPE:0,1
     +BLESECKEYTYPE:0,32
     +BLESECKEYTYPE:0,2
     +BLEAUTHCMPL:0,0

   {IDF_TARGET_NAME} Bluetooth LE client:

   .. code-block:: none

     +BLESECNTFYKEY:0,793718
     +BLESECKEYTYPE:0,2
     +BLESECKEYTYPE:0,16
     +BLESECKEYTYPE:0,1
     +BLESECKEYTYPE:0,32
     +BLEAUTHCMPL:0,0

   You can ignore the message starting with ``+BLESECKEYTYPE``. In terms of the second parameter in the message ``+BLEAUTHCMPL:0,0``, ``0`` means encryption is successful, and ``1`` means encryption fails.

Establish SPP connection between two {IDF_TARGET_NAME} development boards and transmit data in UART-Bluetooth LE Passthrough Mode
-----------------------------------------------------------------------------------------------------------------------------

Below is an example of using two {IDF_TARGET_NAME} development boards, one as a Bluetooth LE server (only as Bluetooth LE server role), the other one as a Bluetooth LE client (only as Bluetooth LE client role). The example shows how to build Bluetooth LE SPP (Serial Port Profile, UART-Bluetooth LE passthrough mode) with AT commands.

.. Important::

  In the following steps, the operations starting with ``{IDF_TARGET_NAME} Bluetooth LE server`` only need to be executed at {IDF_TARGET_NAME} Bluetooth LE server, and the operations starting with ``{IDF_TARGET_NAME} Bluetooth LE client`` only need to be executed at {IDF_TARGET_NAME} Bluetooth LE client.

#. Bluetooth LE initialization.

   {IDF_TARGET_NAME} Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLEINIT=2

   Response:

   .. code-block:: none

     OK

   {IDF_TARGET_NAME} Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLEINIT=1

   Response:

   .. code-block:: none

     OK

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE server creates services.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSRVCRE
  
     Response:
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE server starts services.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSRVSTART
  
     Response:
  
     .. code-block:: none
  
       OK

#. {IDF_TARGET_NAME} Bluetooth LE server gets Bluetooth LE address.

   Command:

   .. code-block:: none

     AT+BLEADDR?

   Response:

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   Note:

   - The address you obtain may be different from that in the above response. Keep yours handy as you will need it in one of the following steps.

#. {IDF_TARGET_NAME} Bluetooth LE server sets advertising parameters.

   Command:

   .. code-block:: none

     AT+BLEADVPARAM=50,50,0,0,7,0,,

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE server sets advertising data.

   Command:

   .. code-block:: none

     AT+BLEADVDATA="0201060A09457370726573736966030302A0"

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE server starts advertising.

   Command:

   .. code-block:: none

     AT+BLEADVSTART

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE client enables a scanning for three seconds.

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

   {IDF_TARGET_NAME} Bluetooth LE client:

   Command:

   .. code-block:: none

     AT+BLECONN=0,"24:0a:c4:d6:e4:46"

   Response:

   .. code-block:: none

     +BLECONN:0,"24:0a:c4:d6:e4:46"
     
     OK

   Note:

   - When entering the above command, replace the address your {IDF_TARGET_NAME} Bluetooth LE server address.
   - If the Bluetooth LE connection is established successfully, message ``+BLECONN:0,"24:0a:c4:d6:e4:46`` will be prompted.
   - If the Bluetooth LE connection is broken, message ``+BLECONN:0,-1`` will be prompted.

#. {IDF_TARGET_NAME} Bluetooth LE server discovers local services.

   Command:

   .. code-block:: none

     AT+BLEGATTSSRV?

   Response:

   .. code-block:: none

     +BLEGATTSSRV:1,1,0xA002,1
     +BLEGATTSSRV:2,1,0xA003,1
     
     OK

#. {IDF_TARGET_NAME} Bluetooth LE server discovers local characteristics.

   Command:

   .. code-block:: none

     AT+BLEGATTSCHAR?

   Response:

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

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

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300
       +BLEGATTSCHAR:"char",1,2,0xC301
       +BLEGATTSCHAR:"char",1,3,0xC302
       +BLEGATTSCHAR:"char",1,4,0xC303
       +BLEGATTSCHAR:"char",1,5,0xC304
       +BLEGATTSCHAR:"char",1,6,0xC305
       +BLEGATTSCHAR:"char",1,7,0xC306
       +BLEGATTSCHAR:"char",1,8,0xC307
       +BLEGATTSCHAR:"char",2,1,0xC400
       +BLEGATTSCHAR:"char",2,2,0xC401

       OK

#. {IDF_TARGET_NAME} Bluetooth LE client discovers services.

   Command:

   .. code-block:: none

     AT+BLEGATTCPRIMSRV=0

   Response:

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1801,1
       +BLEGATTCPRIMSRV:0,2,0x1800,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCPRIMSRV:0,1,0x1800,1
       +BLEGATTCPRIMSRV:0,2,0x1801,1
       +BLEGATTCPRIMSRV:0,3,0xA002,1
       +BLEGATTCPRIMSRV:0,4,0xA003,1

       OK

   Note:

   - When discovering services, the {IDF_TARGET_NAME} Bluetooth LE client will get two more default services (UUID: 0x1800 and 0x1801) than what the {IDF_TARGET_NAME} Bluetooth LE server will get. So, for the same service, the <srv_index> received by the {IDF_TARGET_NAME} Bluetooth LE client equals the <srv_index> received by the {IDF_TARGET_NAME} Bluetooth LE server + 2. For example, for service 0xA002, the <srv_index> queried on the {IDF_TARGET_NAME} Bluetooth LE client is 3, if the {IDF_TARGET_NAME} Bluetooth LE server is queried through the command :ref:`AT+BLEGATTSSRV? <cmd-GSSRV>`, then <srv_index> is 1.

#. {IDF_TARGET_NAME} Bluetooth LE client discovers characteristics.

   Command:

   .. code-block:: none

     AT+BLEGATTCCHAR=0,3

   Response:

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

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

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTCCHAR:"char",0,3,1,0xC300,0x02
       +BLEGATTCCHAR:"char",0,3,2,0xC301,0x02
       +BLEGATTCCHAR:"char",0,3,3,0xC302,0x08
       +BLEGATTCCHAR:"char",0,3,4,0xC303,0x04
       +BLEGATTCCHAR:"char",0,3,5,0xC304,0x08
       +BLEGATTCCHAR:"char",0,3,6,0xC305,0x10
       +BLEGATTCCHAR:"desc",0,3,6,1,0x2902
       +BLEGATTCCHAR:"char",0,3,7,0xC306,0x20
       +BLEGATTCCHAR:"desc",0,3,7,1,0x2902
       +BLEGATTCCHAR:"char",0,3,8,0xC307,0x02

       OK

#. {IDF_TARGET_NAME} Bluetooth LE client Configures Bluetooth LE SPP.

   Set a characteristic that enables writing permission to TX channel for sending data. Set another characteristic that supports notification or indication to RX channel for receiving data.

   Command:

   .. code-block:: none

     AT+BLESPPCFG=1,3,5,3,7

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE client enables Bluetooth LE SPP.

   Command:

   .. code-block:: none

     AT+BLESPP

   Response:

   .. code-block:: none

     OK

     >

   This response indicates that AT has entered Bluetooth LE SPP mode and can send and receive data.

   Note:

   - After the {IDF_TARGET_NAME} Bluetooth LE client enables Bluetooth LE SPP, data received from serial port will be transmitted to the Bluetooth LE server directly.

#. {IDF_TARGET_NAME} Bluetooth LE server Configures Bluetooth LE SPP.

   Set a characteristic that supports notification or indication to TX channel for sending data. Set another characteristic that enables writing permission to RX channel for receiving data.

   Command:

   .. code-block:: none

     AT+BLESPPCFG=1,1,7,1,5

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE server enables Bluetooth LE SPP.

   Command:

   .. code-block:: none

     AT+BLESPP

   Response:

   .. code-block:: none

     OK

     >

   This response indicates that AT has entered Bluetooth LE SPP mode and can send and receive data.

   Note:

   - After the {IDF_TARGET_NAME} Bluetooth LE server enables Bluetooth LE SPP, the data received from serial port will be transmitted to the Bluetooth LE client directly.
   - If the {IDF_TARGET_NAME} Bluetooth LE client does not enable Bluetooth LE SPP first, or uses other device as Bluetooth LE client, then the Bluetooth LE client needs to listen to the notification or indication first. For example, if the {IDF_TARGET_NAME} Bluetooth LE client does not enable Bluetooth LE SPP first, then it should use command ``AT+BLEGATTCWR=0,3,7,1,1`` to enable listening function first, so that the {IDF_TARGET_NAME} Bluetooth LE server can transmit successfully.
   - For the same service, the <srv_index> on the {IDF_TARGET_NAME} Bluetooth LE client side equals the <srv_index> on the {IDF_TARGET_NAME} Bluetooth LE server side + 2.

Establish SPP connection between {IDF_TARGET_NAME} and mobile phone and transmit data in UART-Bluetooth LE passthrough mode
---------------------------------------------------------------------------------------------------------------------------

The example shows how to establish SPP connection between an {IDF_TARGET_NAME} development board (only serving as the Bluetooth LE server role) and a mobile phone (only serve as the Bluetooth LE client role) and how to transmit data between them in UART-Bluetooth LE passthrough mode.

.. Important::
  In the following steps, the operations starting with ``{IDF_TARGET_NAME} Bluetooth LE server`` only need to be executed at {IDF_TARGET_NAME} Bluetooth LE server, and those starting with ``{IDF_TARGET_NAME} Bluetooth LE client`` only need to be executed on the Bluetooth debugging assistant of the mobile phone.

#. First, you need to download the Bluetooth LE debugging assistant on the mobile phone, such as LightBlue.

#. Bluetooth LE initialization.

   {IDF_TARGET_NAME} Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLEINIT=2

   Response:

   .. code-block:: none

     OK

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE server creates services.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSRVCRE
  
     Response:
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE server starts services.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSRVSTART
  
     Response:
  
     .. code-block:: none
  
       OK

#. {IDF_TARGET_NAME} Bluetooth LE server gets its MAC address.

   Command:

   .. code-block:: none

     AT+BLEADDR?

   Response:

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   Note:

   - The address you obtain may be different from that in the above response. Keep yours handy as you will need it in one of the following steps.

#. {IDF_TARGET_NAME} Bluetooth LE server sets advertising parameters.

   Command:

   .. code-block:: none

     AT+BLEADVPARAM=50,50,0,0,7,0,,

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE server sets advertising data.

   Command:

   .. code-block:: none

     AT+BLEADVDATA="0201060A09457370726573736966030302A0"

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE server starts advertising.

   Command:

   .. code-block:: none

     AT+BLEADVSTART

   Response:

   .. code-block:: none

     OK

#. Establish the Bluetooth LE connection.

   Open the LightBlue application on your mobile phone, and click ``SCAN`` to start scanning. When you find the MAC address of the {IDF_TARGET_NAME} Bluetooth LE server, click ``CONNECT``. Then, {IDF_TARGET_NAME} should print the log similar to ``+BLECONN:0,"60:51:42:fe:98:aa"``, which indicates that Bluetooth LE connection has been established.

#. {IDF_TARGET_NAME} Bluetooth LE server discovers local services.

   Command:

   .. code-block:: none

     AT+BLEGATTSSRV?

   Response:

   .. code-block:: none

     +BLEGATTSSRV:1,1,0xA002,1
     +BLEGATTSSRV:2,1,0xA003,1
     
     OK

#. {IDF_TARGET_NAME} Bluetooth LE server discovers local characteristics.

   Command:

   .. code-block:: none

     AT+BLEGATTSCHAR?

   Response:

   .. only:: esp32 or esp32c3 or esp32c5 or esp32c6

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

   .. only:: esp32c2

     .. code-block:: none

       +BLEGATTSCHAR:"char",1,1,0xC300
       +BLEGATTSCHAR:"char",1,2,0xC301
       +BLEGATTSCHAR:"char",1,3,0xC302
       +BLEGATTSCHAR:"char",1,4,0xC303
       +BLEGATTSCHAR:"char",1,5,0xC304
       +BLEGATTSCHAR:"char",1,6,0xC305
       +BLEGATTSCHAR:"char",1,7,0xC306
       +BLEGATTSCHAR:"char",1,8,0xC307
       +BLEGATTSCHAR:"char",2,1,0xC400
       +BLEGATTSCHAR:"char",2,2,0xC401

       OK

#. {IDF_TARGET_NAME} Bluetooth LE client discovers characteristics.

   Click the button of the service feature whose Properties is NOTIFY or INDICATE in the mobile phone LightBlue client (here ESP-AT default Properties The service characteristics of NOTIFY or INDICATE are 0xC305 and 0xC306) and start to listen for the service characteristics of NOTIFY or INDICATE.

#. {IDF_TARGET_NAME} Bluetooth LE server configures Bluetooth LE SPP.

   Set a characteristic that supports notification or indication to TX channel for sending data. Set another characteristic that enables writing permission to RX channel for receiving data.

   Command:

   .. code-block:: none

     AT+BLESPPCFG=1,1,7,1,5

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE server enables Bluetooth LE SPP.

   Command:

   .. code-block:: none

     AT+BLESPP

   Response:

   .. code-block:: none

     OK

     >

   This response indicates that AT has entered Bluetooth LE SPP mode and can send and receive data.

#. Bluetooth LE client sends data.

   In the LightBlue client, select the 0xC304 service characteristic value and send the data ``test`` to the {IDF_TARGET_NAME} Bluetooth LE server. Then, the {IDF_TARGET_NAME} Bluetooth LE server can receive the ``test``.

#. {IDF_TARGET_NAME} Bluetooth LE server sends data.

   The {IDF_TARGET_NAME} Bluetooth LE server sends ``test``, and then the LightBlue client can receive ``test``.

Establish Bluetooth LE connection and pairing between {IDF_TARGET_NAME} and mobile phone
-------------------------------------------------------------------------------------------

This example shows how to establish a Bluetooth LE connection between the {IDF_TARGET_NAME} development board (only as a Bluetooth LE server role) and a mobile phone (only as a Bluetooth LE client role) and enter the passkey to complete the pairing.

.. Important::
  In the following steps, the operations starting with ``{IDF_TARGET_NAME} Bluetooth LE server`` only need to be executed at {IDF_TARGET_NAME} Bluetooth LE server, and those starting with ``{IDF_TARGET_NAME} Bluetooth LE client`` only need to be executed on the Bluetooth debugging assistant of the mobile phone.

#. First, you need to download the Bluetooth LE debugging assistant on the mobile phone, such as LightBlue.

#. Bluetooth LE initialization.

   {IDF_TARGET_NAME} Bluetooth LE server:

   Command:

   .. code-block:: none

     AT+BLEINIT=2

   Response:

   .. code-block:: none

     OK

.. only:: esp32 or esp32c3

  #. {IDF_TARGET_NAME} Bluetooth LE server creates services.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSRVCRE
  
     Response:
  
     .. code-block:: none
  
       OK
  
  #. {IDF_TARGET_NAME} Bluetooth LE server starts services.
  
     Command:
  
     .. code-block:: none
  
       AT+BLEGATTSSRVSTART
  
     Response:
  
     .. code-block:: none
  
       OK

#. {IDF_TARGET_NAME} Bluetooth LE server gets its MAC address.

   Command:

   .. code-block:: none

     AT+BLEADDR?

   Response:

   .. code-block:: none

     +BLEADDR:"24:0a:c4:d6:e4:46"
     OK

   Note:

   - The address you obtain may be different from that in the above response. Keep yours handy as you will need it in one of the following steps.

#. {IDF_TARGET_NAME} Bluetooth LE server sets advertising parameters.

   Command:

   .. code-block:: none

     AT+BLEADVPARAM=50,50,0,0,7,0,,

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE server sets advertising data.

   Command:

   .. code-block:: none

     AT+BLEADVDATA="0201060A09457370726573736966030302A0"

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE server sets encryption parameters.

   Command:

   .. code-block:: none

     AT+BLESECPARAM=13,2,16,3,3

   Response:

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} Bluetooth LE server starts advertising.

   Command:

   .. code-block:: none

     AT+BLEADVSTART

   Response:

   .. code-block:: none

     OK

#. Establish the Bluetooth LE connection.

   Open the LightBlue application on your mobile phone, and click ``SCAN`` to start scanning. When you find the MAC address of the {IDF_TARGET_NAME} Bluetooth LE server, click ``CONNECT``. Then, {IDF_TARGET_NAME} should print the log similar to ``+BLECONN:0,"60:51:42:fe:98:aa"``, which indicates that Bluetooth LE connection has been established.

#. {IDF_TARGET_NAME} Bluetooth LE server initiates encryption request.

   Command:

   .. code-block:: none

     AT+BLEENC=0,3

   Response:

   .. code-block:: none

     OK

#. Bluetooth LE client accepts pairing.

   The pairing information (including the pairing key, for example: 231518) will pop up on the Bluetooth LE connection page that has just been successfully created on the LightBlue, and then click "Pairing". At this time, the {IDF_TARGET_NAME} Bluetooth LE server should print a log similar to ``+BLESECKEYREQ:0``, indicating that the phone has responded to pairing, and the {IDF_TARGET_NAME} Bluetooth LE server can enter the key.

#. {IDF_TARGET_NAME} Bluetooth LE server replies with the pairing key.

   At this time, the key that the Bluetooth LE server should reply with is the key (231518) contained in the pairing information that popped up on LightBlue in the previous step.

   Command:

   .. code-block:: none

     AT+BLEKEYREPLY=0,231518

   Response:

   .. code-block:: none

     OK
  
   At this time, the {IDF_TARGET_NAME} Bluetooth LE server will print a log similar to the following, which means that the {IDF_TARGET_NAME} Bluetooth LE server has successfully paired with the phone Bluetooth LE client.

   .. code-block:: none

     +BLESECKEYTYPE:0,16
     +BLESECKEYTYPE:0,1
     +BLESECKEYTYPE:0,32
     +BLESECKEYTYPE:0,2
     +BLEAUTHCMPL:0,0
