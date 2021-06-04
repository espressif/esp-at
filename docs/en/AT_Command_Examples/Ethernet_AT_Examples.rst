[ESP32 Only] Ethernet AT Examples
==================================

.. note::
    Before you run any Ethernet AT commands, please make sure you have followed the :ref:`Prerequisite <cmd-ETHPRE>`.

- `Establish a TCP Connection on Ethernet Network`_


Establish a TCP Connection on Ethernet Network
-----------------------------------------------

#. Enable multiple connections.


   - Command
   
     ::
 
       AT+CIPMUX=1

   - Response

     ::

       OK

#. Create a TCP server.

   - Command
  
     ::

       AT+CIPSERVER=1,8081

   - Response

     ::

       OK

#. Obtain the IP address of the server.

   - Command
  
     ::

       AT+CIPETH?

   - Response

     ::

       +CIPETH:ip:192.168.105.24
       +CIPETH:gateway:192.168.105.1
       +CIPETH:netmask:255.255.255.0
       OK
   
#. Use a network tool on PC to create a TCP client and connect to the TCP server that ESP created. (IP: 192.168.105.24, Port: 8081).

#. Send data in :term:`Normal Transmission Mode`.

   - Command
  
     ::

       AT+CIPSEND=0,4    // send 4 bytes to connection ID 0

   - Response

     ::

       OK
       >

   - Enter the data.
   - Response

     ::

       SEND OK
  
   Note: If the length of the data you entered is more than the value of <length> (n) set by ``AT+CIPSEND``, the system will prompt ``busy``, and send the first n bytes. After sending the first n bytes, the system will reply ``SEND OK``.

#. Receive data in :term:`Normal Transmission Mode`.

   When the ESP server receives data, AT will prompt:

   ::

     +IPD,n:xxxxxxxxxx    // received n bytes, data=xxxxxxxxxxx

#. Close TCP Connection.

   - Command
  
     ::

       AT+CIPCLOSE=0

   - Response

     ::

       OK

#. Delete the TCP server.

   - Command
  
     ::

       AT+CIPSERVER=0

   - Response

     ::

       OK

