[ESP32 Only] Classic Bluetooth AT Examples
=============================================

:link_to_translation:`zh_CN:[中文]`

This document provides detailed command examples to illustrate how to utilize :doc:`../AT_Command_Set/BT_AT_Commands` on ESP devices.

.. contents::
   :local:
   :depth: 1

.. Important::
  
  The default firmware does not support Classic Bluetooth commands, please refer to :doc:`../Compile_and_Develop/how_to_enable_at_classic_bluetooth` to enable Classic Bluetooth commands.

Establish SPP connection between phone (or PC) and ESP32 in Normal Transmission mode with IO capability set to NoInputNoOutput
------------------------------------------------------------------------------------------------------------------------------------------

In this example, mobile phone or PC is master and ESP32 is slave. The example shows how to establish SPP connection.

#. Classic Bluetooth initialization.
   
   Command:

   .. code-block:: none

     AT+BTINIT=1

   Response:
  
   .. code-block:: none

     OK

#. Classic Bluetooth SPP profile initialization, and the role is set to slave.
   
   Command:

   .. code-block:: none

     AT+BTSPPINIT=2

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth device name.
   
   Command:

   .. code-block:: none

     AT+BTNAME="EXAMPLE"

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth scan mode to discoverable and connectable.
   
   Command:

   .. code-block:: none

     AT+BTSCANMODE=2

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth security parameters. Set ``io_cap`` to ``NoInputNoOutput``, ``pin_type`` to ``fixed``, ``pin_code`` to ``9527``.
   
   Command:

   .. code-block:: none

     AT+BTSECPARAM=3,1,"9527"

   Response:
  
   .. code-block:: none

     OK

#. Start Classic Bluetooth SPP profile.
   
   Command:

   .. code-block:: none

     AT+BTSPPSTART

   Response:
  
   .. code-block:: none

     OK

#. The mobile phone or PC initiates the connection.

   The mobile phone or PC should be able to find the Bluetooth device with name "EXAMPLE". If the mobile phone or PC initiates a connection and the connection is established successfully, ESP32 will prompt:

   .. code-block:: none

     +BTSPPCONN:0,"e0:24:81:47:90:bc"

   Note:

   - The address you obtained may be different from that in the above response.

#. Send 4 bytes of data.

   Command:

   .. code-block:: none

     AT+BTSPPSEND=0,4

   Response:

   .. code-block:: none

     >

   The symbol ``>`` indicates that AT is ready for receiving serial data and you can enter data now. When the requirement of data length determined by the parameter ``<data_len>`` is met, the writing starts.

   Input 4 bytes, for example, ``test``, then AT will respond the following message.

   .. code-block:: none

     OK

   Note:

   - If the number of bytes inputted are more than the length (n) set by ``AT+BTSPPSEND``, the system will reply ``busy p...``, and send the first n bytes. And after sending the first n bytes, the system will reply ``OK``.
   - After AT outputs the ``>`` character, the special characters in the data does not need to be escaped through the escape character, and it does not need to end with a new line (CR-LF).

#. Receive 4 bytes of data.

   Assume that mobile phone or PC sends 4 bytes of data (data is ``test``), the system will prompt:

   .. code-block:: none

     +BTDATA:4,test

#. Terminate Classic Bluetooth SPP connection.

   Command:

   .. code-block:: none

     AT+BTSPPDISCONN=0

   Response:

   .. code-block:: none

     +BTSPPDISCONN:0,"e0:24:81:47:90:bc"

     OK

   Note:

   - The address you obtained may be different from that in the above response.

Establish SPP connection between phone (or PC) and ESP32 in Passthrough mode with IO capability set to NoInputNoOutput
-------------------------------------------------------------------------------------------------------------------------------

In this example, mobile phone or PC is master and ESP32 is slave. The example shows how to establish SPP connection.

#. Classic Bluetooth initialization.
   
   Command:

   .. code-block:: none

     AT+BTINIT=1

   Response:
  
   .. code-block:: none

     OK

#. Classic Bluetooth SPP profile initialization, and the role is set to slave.
   
   Command:

   .. code-block:: none

     AT+BTSPPINIT=2

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth device name.
   
   Command:

   .. code-block:: none

     AT+BTNAME="EXAMPLE"

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth scan mode to discoverable and connectable.
   
   Command:

   .. code-block:: none

     AT+BTSCANMODE=2

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth security parameters. Set ``io_cap`` to ``NoInputNoOutput``, ``pin_type`` to ``fixed``, ``pin_code`` to ``9527``.
   
   Command:

   .. code-block:: none

     AT+BTSECPARAM=3,1,"9527"

   Response:
  
   .. code-block:: none

     OK

#. Start Classic Bluetooth SPP profile.
   
   Command:

   .. code-block:: none

     AT+BTSPPSTART

   Response:
  
   .. code-block:: none

     OK

#. The mobile phone or PC initiates the connection.

   The mobile phone or PC should be able to find the Bluetooth device with name "EXAMPLE". If the mobile phone or PC initiates a connection and the connection is established successfully, ESP32 will prompt:

   .. code-block:: none

     +BTSPPCONN:0,"e0:24:81:47:90:bc"

   Note:

   - The address you obtained may be different from that in the above response.

#. Send data in Passthrough Mode.

   Command:

   .. code-block:: none

     AT+BTSPPSEND

   Response:

   .. code-block:: none

     OK

     >

   This response indicates that AT has entered Passthrough Mode.

   Note:

   - After the AT enters Passthrough Mode, data received from serial port will be transmitted to the mobile phone or PC.

#. Stop sending data.

   When receiving a packet that contains only ``+++``, the Passthrough Mode will be stopped. Then please wait at least 1 second before sending next AT command. Please be noted that if you input ``+++`` directly by typing, the ``+++`` may not be recognised as three consecutive ``+`` because of the prolonged typing duration. For more details, please refer to :ref:`AT+BTSPPSEND <cmd-BTSPPSEND>`.

   .. Important::

     The aim of ending the packet with ``+++`` is to exit Passthrough Mode and to accept normal AT commands. However, you can also use command ``AT+BTSPPSEND`` to go back into Passthrough Mode.

#. Terminate Classic Bluetooth SPP connection.

   Command:

   .. code-block:: none

     AT+BTSPPDISCONN=0

   Response:

   .. code-block:: none

     +BTSPPDISCONN:0,"e0:24:81:47:90:bc"

     OK

   Note:

   - The address you obtained may be different from that in the above response.

Establish SPP connection between phone (or PC) and ESP32 with IO capability set to KeyboardOnly
-------------------------------------------------------------------------------------------------

The process is basically the same as in the `Establish SPP connection between phone (or PC) and ESP32 in Normal Transmission mode with IO capability set to NoInputNoOutput`_. The only difference lies in security parameters settings.

#. Classic Bluetooth initialization.
   
   Command:

   .. code-block:: none

     AT+BTINIT=1

   Response:
  
   .. code-block:: none

     OK

#. Classic Bluetooth SPP profile initialization, and the role is set to slave.
   
   Command:

   .. code-block:: none

     AT+BTSPPINIT=2

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth device name.
   
   Command:

   .. code-block:: none

     AT+BTNAME="EXAMPLE"

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth scan mode to discoverable and connectable.
   
   Command:

   .. code-block:: none

     AT+BTSCANMODE=2

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth security parameters. Set ``io_cap`` to ``KeyboardOnly``, ``pin_type`` to ``variable``, ``pin_code`` to ``9527``.
   
   Command:

   .. code-block:: none

     AT+BTSECPARAM=2,0,"9527"

   Response:
  
   .. code-block:: none

     OK

#. Start Classic Bluetooth SPP profile.
   
   Command:

   .. code-block:: none

     AT+BTSPPSTART

   Response:
  
   .. code-block:: none

     OK

#. The mobile phone or PC initiates the connection.

   The mobile phone or PC can initiate a connection and generate a PIN code, then you can enter the PIN code on the ESP32.

   .. code-block:: none

     AT+BTKEYREPLY=0,676572

   If the connection is established successfully, the system will prompt:

   .. code-block:: none

     +BTSPPCONN:0,"e0:24:81:47:90:bc"

   Note:

   - The PIN code you entered may be different from those in the above command. Please use the real PIN instead.
   - The address you obtained may be different from that in the above response.

#. Terminate Classic Bluetooth SPP connection.

   Command:

   .. code-block:: none

     AT+BTSPPDISCONN=0

   Response:

   .. code-block:: none

     +BTSPPDISCONN:0,"e0:24:81:47:90:bc"

     OK

   Note:

   - The address you obtained may be different from that in the above response.

Establish SPP connection between two ESP32 development boards
---------------------------------------------------------------

Below is an example of using two ESP32 development boards, one as master, the other one as slave.

.. Important::

  In the following steps, the operations starting with ``Master`` only need to be executed at master, and the operations starting with ``Slave`` only need to be executed at slave. If the operation is not specified on which side it is executed, it needs to be executed on both the master side and the slave side.

#. Classic Bluetooth initialization.
   
   Command:

   .. code-block:: none

     AT+BTINIT=1

   Response:
  
   .. code-block:: none

     OK

#. Classic Bluetooth SPP profile initialization.

   Master:

   Command:

   .. code-block:: none

     AT+BTSPPINIT=1

   Response:
  
   .. code-block:: none

     OK

   Slave:

   Command:

   .. code-block:: none

     AT+BTSPPINIT=2

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth device name.

   Slave:

   Command:

   .. code-block:: none

     AT+BTNAME="EXAMPLE"

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth scan mode to discoverable and connectable.

   Slave:

   Command:

   .. code-block:: none

     AT+BTSCANMODE=2

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth security parameters. Set ``io_cap`` to ``NoInputNoOutput``, ``pin_type`` to ``fixed``, ``pin_code`` to ``9527``.

   Slave:

   Command:

   .. code-block:: none

     AT+BTSECPARAM=3,1,"9527"

   Response:
  
   .. code-block:: none

     OK

#. Start Classic Bluetooth SPP profile.

   Slave:

   Command:

   .. code-block:: none

     AT+BTSPPSTART

   Response:
  
   .. code-block:: none

     OK

#. Start Classic Bluetooth discovery. Set inquiry duration to 10 s, number of inquiry responses to 10.

   Master:

   Command:

   .. code-block:: none

     AT+BTSTARTDISC=0,10,10

   Response:
  
   .. code-block:: none

     +BTSTARTDISC:"10:f6:05:f9:bc:4f",realme V11 5G,0x2,0x3,0x2d0,-34
     +BTSTARTDISC:"24:0a:c4:d6:e4:46",EXAMPLE,,,,-27
     +BTSTARTDISC:"10:f6:05:f9:bc:4f",realme V11 5G,0x2,0x3,0x2d0,-33
     +BTSTARTDISC:"24:0a:c4:d6:e4:46",EXAMPLE,,,,-25
     +BTSTARTDISC:"ac:d6:18:47:0c:ae",,0x2,0x3,0x2d0,-72
     +BTSTARTDISC:"24:0a:c4:d6:e4:46",EXAMPLE,,,,-26
     +BTSTARTDISC:"10:f6:05:f9:bc:4f",,0x2,0x3,0x2d0,-41
     +BTSTARTDISC:"24:0a:c4:2c:a8:a2",,,,,-50
     +BTSTARTDISC:"24:0a:c4:d6:e4:46",EXAMPLE,,,,-26
     +BTSTARTDISC:"10:f6:05:f9:bc:4f",realme V11 5G,0x2,0x3,0x2d0,-39
     +BTSTARTDISC:"24:0a:c4:d6:e4:46",EXAMPLE,,,,-23
     +BTSTARTDISC:"10:f6:05:f9:bc:4f",realme V11 5G,0x2,0x3,0x2d0,-36
     +BTSTARTDISC:"10:f6:05:f9:bc:4f",realme V11 5G,0x2,0x3,0x2d0,-41
     +BTSTARTDISC:"b4:a5:ac:16:14:8c",,0x2,0x3,0x2d0,-57
     +BTSTARTDISC:"24:0a:c4:2c:a8:a2"
     +BTSTARTDISC:"b4:a5:ac:16:14:8c"

     OK

   Note:

   - The discovery results you obtain may be different from those in the above response.

#. Establish SPP connection.

   Master:

   Command:

   .. code-block:: none

     AT+BTSPPCONN=0,0,"24:0a:c4:d6:e4:46"

   Response:
  
   .. code-block:: none

     +BTSPPCONN:0,"24:0a:c4:d6:e4:46"

     OK

   Note:

   - When entering the above command, replace the address with slave address.
   - If the SPP connection is established successfully, message ``+BTSPPCONN:0,"30:ae:a4:80:06:8e"`` will be prompted on the slave.

#. Terminate Classic Bluetooth SPP connection.

   Slave:

   Command:

   .. code-block:: none

     AT+BTSPPDISCONN=0

   Response:

   .. code-block:: none

     +BTSPPDISCONN:0,"30:ae:a4:80:06:8e"

     OK

   Note:

   - Both master and slave can actively terminate the SPP connection.
   - If the SPP connection is ended successfully, message ``+BTSPPDISCONN:0,"24:0a:c4:d6:e4:46"`` will be prompted on the master.

Establish A2DP connection and enable A2DP Sink to play music
-------------------------------------------------------------

.. Important::
  
  - To use ``A2DP Sink``, you need to add the code of the ``I2S`` part by yourself. For the code to initialize the ``I2S`` part, please refer to `a2dp sink example <https://github.com/espressif/esp-idf/blob/master/examples/bluetooth/bluedroid/classic_bt/a2dp_sink/main/main.c>`__.
  - The driver code of the ``decoder`` chip part also needs to be added by yourself or use an off-the-shelf development board.

#. Classic Bluetooth initialization.

   Command:

   .. code-block:: none

     AT+BTINIT=1

   Response:
  
   .. code-block:: none

     OK

#. Classic Bluetooth A2DP profile initialization, and the role is set to sink.

   Command:

   .. code-block:: none

     AT+BTA2DPINIT=2

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth device name.
   
   Command:

   .. code-block:: none

     AT+BTNAME="EXAMPLE"

   Response:
  
   .. code-block:: none

     OK

#. Set Classic Bluetooth scan mode to discoverable and connectable.

   Command:

   .. code-block:: none

     AT+BTSCANMODE=2

   Response:
  
   .. code-block:: none

     OK

#. Establish connection.

   The source role should be able to find the Bluetooth device with name "EXAMPLE". In this example, you can use your mobile phone to initiate a connection. If the connection is established successfully, ESP32 will prompt:

   .. code-block:: none

     +BTA2DPCONN:0,"e0:24:81:47:90:bc"

   Note:

   - The address you obtained may be different from that in the above response.

#. Start playing music.

   Command:

   .. code-block:: none

     AT+BTA2DPCTRL=0,1

   Response:
  
   .. code-block:: none

     OK

   Note:

   - For more types of control, please refer to :ref:`AT+BTA2DPCTRL <cmd-BTA2DPCTRL>`.

#. Stop playing music.

   Command:

   .. code-block:: none

     AT+BTA2DPCTRL=0,0

   Response:
  
   .. code-block:: none

     OK

   Note:

   - For more types of control, please refer to :ref:`AT+BTA2DPCTRL <cmd-BTA2DPCTRL>`.

#. Terminate A2DP connection.

   Command:

   .. code-block:: none

     AT+BTA2DPDISCONN=0

   Response:
  
   .. code-block:: none

     OK
     +BTA2DPDISCONN:0,"e0:24:81:47:90:bc"

Query and clear Classic Bluetooth encryption device list
----------------------------------------------------------

#. Get the encryption device list.

   Command:

   .. code-block:: none

     AT+BTENCDEV?

   Response:
  
   .. code-block:: none

     +BTA2DPDISCONN:0,"e0:24:81:47:90:bc"
     OK

   Note:

   - If no device has been successfully bound before, AT will only prompt ``OK``.

#. Clear Classic Bluetooth encryption device list.

   There are two ways to clear encryption device list.

   1. Remove a device from the encryption device list with a specific index.

      Command:

      .. code-block:: none

        AT+BTENCCLEAR=0

      Response:
     
      .. code-block:: none

        OK

   2. Remove all devices from the encryption device list.

      Command:

      .. code-block:: none

        AT+BTENCCLEAR

      Response:
     
      .. code-block:: none

        OK
