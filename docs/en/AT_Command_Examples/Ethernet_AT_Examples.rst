{IDF_TARGET_NAME} Ethernet AT Examples
======================================

:link_to_translation:`zh_CN:[中文]`

This document provides an introduction and detailed command examples to illustrate how to utilize :doc:`../AT_Command_Set/Ethernet_AT_Commands` on {IDF_TARGET_NAME}.

.. contents::
   :local:
   :depth: 1

.. Important::
  - Before you run any Ethernet AT commands, please make sure you have followed the :ref:`Prerequisite <cmd-ETHPRE>`.
  - The examples described in this document are based on the situation that network cable has been plugged in.

Establish a TCP connection on Ethernet network
---------------------------------------------------------------

#. Enable multiple connections.

   Command:

   .. code-block:: none

     AT+CIPMUX=1

   Response:

   .. code-block:: none

     OK

#. Create a TCP server.

   Command:

   .. code-block:: none

     AT+CIPSERVER=1,8081

   Response:

   .. code-block:: none

     OK

#. Obtain the IP address of the server.
  
   Command:

   .. code-block:: none

     AT+CIPETH?

   Response:

   .. code-block:: none

     +CIPETH:ip:192.168.105.24
     +CIPETH:gateway:192.168.105.1
     +CIPETH:netmask:255.255.255.0
     OK

   Note:

   - The address you obtain may be different from that in the above response.

#. Use a network tool on PC to create a TCP client and connect to the TCP server by the step 2, which IP address is ``192.168.105.24``, port is ``8081``.

#. Send 4 bytes of data to transmission link 0 in :term:`Normal Transmission Mode`.

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

   - If the number of bytes input exceeds the length (n) set by ``AT+CIPSEND``, the system will reply ``busy p...``, and send the first n bytes. After sending the first n bytes, the system will reply ``SEND OK``.

#. Receive 4 bytes of data from transmission link 0 in :term:`Normal Transmission Mode`.

   Assume that the TCP server received 4 bytes of data (data is ``test``), the system would be prompt as:

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

#. Delete the TCP server.

   Command:

   .. code-block:: none

     AT+CIPSERVER=0

   Response:

   .. code-block:: none

     OK

   Note:

   - The ``AT+CIPSERVER=0`` command will only shutdown the server, but will keep the existing connection. If you want to close all client connections to the server at the same time, please execute the command ``AT+CIPSERVER=0,1``.
