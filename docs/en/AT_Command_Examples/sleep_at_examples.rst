Sleep AT Examples
==========================

:link_to_translation:`zh_CN:[中文]`

This document provides an introduction and detailed command examples to illustrate how to utilize AT commands to set sleep modes on {IDF_TARGET_NAME} series of products.

.. contents::
   :local:
   :depth: 1

Introduction
------------

With the use of advanced power-management technologies, {IDF_TARGET_NAME} series can switch between different power modes. Currently, ESP-AT supports the following four power consumption modes (for more sleep modes, please refer to the datasheet):

1. ``Active`` Mode: CPU and chip radio are powered on. The chip can receive, transmit, or listen.
2. ``Modem-sleep`` Mode: The CPU is operational and the clock speed can be reduced. Wi-Fi baseband, Bluetooth LE baseband, and radio are disabled, but Wi-Fi and Bluetooth LE connection can remain active.
3. ``Light-sleep`` Mode: The CPU is paused. Any wake-up events (MAC, host, RTC timer, or external interrupts) will wake up the chip. Wi-Fi and Bluetooth LE connection can remain active.
4. ``Deep-sleep`` Mode: CPU and most peripherals are powered down. Only the RTC memory is powered on.

By default, {IDF_TARGET_NAME} will enter ``Active`` mode after system reset. When the CPU does not need to work all the time, such as waiting for external activities to wake up, the system can enter low-power modes.

For current consumption of {IDF_TARGET_NAME}, please refer to `{IDF_TARGET_NAME} Series Datasheet <{IDF_TARGET_DATASHEET_EN_URL}>`__.

.. note::

  .. list::

    - Setting {IDF_TARGET_NAME} to sleep modes in Wi-Fi mode and Bluetooth LE mode will be described separately.
    - In single Wi-Fi mode, only ``station`` mode supports ``Modem-sleep`` mode and ``Light-sleep`` mode.
    - For Light-sleep mode in Bluetooth LE mode, please ensure that there is an external 32 KHz crystal oscillator. If there is no external 32 KHz crystal oscillator, ESP-AT will work as the Modem-sleep mode.

Measurement Method
^^^^^^^^^^^^^^^^^^^^

In order to avoid some unnecessary interference during the power consumption test process, it is recommended to use the Espressif modules that integrate the chip for the test.

Refer to the following figure for hardware connection. (Note that the development board below only has the {IDF_TARGET_NAME} module on board, and all other peripheral components have been removed.)

.. figure:: ../../_static/at_command_examples/esp-power-consumption-hardware-connection.png
    :align: center
    :alt: {IDF_TARGET_NAME} Hardware Connection

    {IDF_TARGET_NAME} Hardware Connection

Set Modem-sleep mode in Wi-Fi mode
-----------------------------------

#. Set the Wi-Fi mode to station mode.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:

   .. code-block:: none

     OK

#. Connect to an router.

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

#. Set the sleep mode to Modem-sleep mode.

   Command:

   .. code-block:: none

     AT+SLEEP=1

   Response:

   .. code-block:: none

     OK

.. note::

  * RF will be periodically closed according to AP DTIM (routers generally set DTIM to 1).

Set Light-sleep mode in Wi-Fi mode
-----------------------------------

#. Set the Wi-Fi mode to station mode.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:

   .. code-block:: none

     OK

#. Connect to an router. Set listen interval to 3.

   Command:

   .. code-block:: none

     AT+CWJAP="espressif","1234567890",,,,3

   Response:

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   Note:

   - The SSID and password you entered may be different from those in the above command. Please replace the SSID and password with those of your router settings.

#. Set the sleep mode to Light-sleep mode.

   Command:

   .. code-block:: none

     AT+SLEEP=2

   Response:

   .. code-block:: none

     OK

.. note::

  * CPU will automatically sleep and RF will be periodically closed according to listen interval set by :ref:`AT+CWJAP <cmd-JAP>`.

.. only:: esp32 or esp32c3

  Set Modem-sleep mode in Bluetooth LE advertising mode
  ------------------------------------------------------

  #. Initialize the role of Bluetooth LE as server.

    Command:

    .. code-block:: none

      AT+BLEINIT=2

    Response:

    .. code-block:: none

      OK

  #. Set parameters of Bluetooth LE advertising. Set Bluetooth LE advertising interval to 1 s.

    Command:

    .. code-block:: none

      AT+BLEADVPARAM=1600,1600,0,0,7,0,0,"00:00:00:00:00:00"

    Response:

    .. code-block:: none

      OK

  #. Start Bluetooth LE advertising.

    Command:

    .. code-block:: none

      AT+BLEADVSTART

    Response:

    .. code-block:: none

      OK

  #. Disable Wi-Fi.

    Command:

    .. code-block:: none

      AT+CWMODE=0

    Response:

    .. code-block:: none

      OK

  #. Set the sleep mode to Modem-sleep mode.

    Command:

    .. code-block:: none

      AT+SLEEP=1

    Response:

    .. code-block:: none

      OK

  Set Modem-sleep mode in Bluetooth LE connection mode
  ------------------------------------------------------

  #. Initialize the role of Bluetooth LE as server.

    Command:

    .. code-block:: none

      AT+BLEINIT=2

    Response:

    .. code-block:: none

      OK

  #. Start Bluetooth LE advertising.

    Command:

    .. code-block:: none

      AT+BLEADVSTART

    Response:

    .. code-block:: none

      OK

  #. Waiting for connection.

    If the connection is established successfully, AT will prompt:

    .. code-block:: none

      +BLECONN:0,"47:3f:86:dc:e4:7d"
      +BLECONNPARAM:0,0,0,6,0,500
      +BLECONNPARAM:0,0,0,24,0,500

      OK

    Note:

    - In this example, Bluetooth LE client address is 47:3f:86:dc:e4:7d.
    - For prompt information (+BLECONN and +BLECONNPARAM), please refer to :ref:`AT+BLECONN <cmd-BCONN>` and :ref:`AT+BLECONNPARAM <cmd-BCONNP>` for more details.

  #. Update parameters of Bluetooth LE connection. Set Bluetooth LE connection interval to 1 s.

    Command:

    .. code-block:: none

      AT+BLECONNPARAM=0,800,800,0,500

    Response:

    .. code-block:: none

      OK
    
    If the connection parameters are updated successfully, AT will output:

    .. code-block:: none

        +BLECONNPARAM:0,800,800,800,0,500

    Note:

    - For prompt information (+BLECONNPARAM), please refer to :ref:`AT+BLECONNPARAM <cmd-BCONNP>` for more details. 

  #. Disable Wi-Fi.

    Command:

    .. code-block:: none

      AT+CWMODE=0

    Response:

    .. code-block:: none

      OK

  #. Set the sleep mode to Modem-sleep mode.

    Command:

    .. code-block:: none

      AT+SLEEP=1

    Response:

    .. code-block:: none

      OK

  Set Light-sleep mode in Bluetooth LE advertising mode
  -------------------------------------------------------

  #. Initialize the role of Bluetooth LE as server.

    Command:

    .. code-block:: none

      AT+BLEINIT=2

    Response:

    .. code-block:: none

      OK

  #. Set parameters of Bluetooth LE advertising. Set Bluetooth LE advertising interval to 1 s.

    Command:

    .. code-block:: none

      AT+BLEADVPARAM=1600,1600,0,0,7,0,0,"00:00:00:00:00:00"

    Response:

    .. code-block:: none

      OK

  #. Start Bluetooth LE advertising.

    Command:

    .. code-block:: none

      AT+BLEADVSTART

    Response:

    .. code-block:: none

      OK

  #. Disable Wi-Fi.

    Command:

    .. code-block:: none

      AT+CWMODE=0

    Response:

    .. code-block:: none

      OK

  #. Set the sleep mode to Light-sleep mode.

    Command:

    .. code-block:: none

      AT+SLEEP=2

    Response:

    .. code-block:: none

      OK

  Set Light-sleep mode in Bluetooth LE connection mode
  -----------------------------------------------------

  #. Initialize the role of Bluetooth LE as server.

    Command:

    .. code-block:: none

      AT+BLEINIT=2

    Response:

    .. code-block:: none

      OK

  #. Start Bluetooth LE advertising.

    Command:

    .. code-block:: none

      AT+BLEADVSTART

    Response:

    .. code-block:: none

      OK

  #. Waiting for connection.

    If the connection is established successfully, AT will prompt:

    .. code-block:: none

      +BLECONN:0,"47:3f:86:dc:e4:7d"
      +BLECONNPARAM:0,0,0,6,0,500
      +BLECONNPARAM:0,0,0,24,0,500

      OK

    Note:

    - In this example, Bluetooth LE client address is 47:3f:86:dc:e4:7d.
    - For prompt information (+BLECONN and +BLECONNPARAM), please refer to :ref:`AT+BLECONN <cmd-BCONN>` and :ref:`AT+BLECONNPARAM <cmd-BCONNP>` for more details.

  #. Update parameters of Bluetooth LE connection. Set Bluetooth LE connection interval to 1 s.

    Command:

    .. code-block:: none

      AT+BLECONNPARAM=0,800,800,0,500

    Response:

    .. code-block:: none

      OK
    
    If the connection parameters are updated successfully, AT will output:

    .. code-block:: none

        +BLECONNPARAM:0,800,800,800,0,500

    Note:

    - For prompt information (+BLECONNPARAM), please refer to :ref:`AT+BLECONNPARAM <cmd-BCONNP>` for more details.

  #. Disable Wi-Fi.

    Command:

    .. code-block:: none

      AT+CWMODE=0

    Response:

    .. code-block:: none

      OK

  #. Set the sleep mode to Light-sleep mode.

    Command:

    .. code-block:: none

      AT+SLEEP=2

    Response:

    .. code-block:: none

      OK

Set Deep-sleep mode
--------------------

#. Set the sleep mode to Deep-sleep mode. Set the deep-sleep time to 3600000 ms.

   Command:

   .. code-block:: none

     AT+GSLP=3600000

   Response:

   .. code-block:: none

     OK

   Note:

   - When the time is up, the device automatically wakes up, calls Deep-sleep wake stub, and then proceeds to load the application.
   - For Deep-sleep mode, the only wake-up method is timed wake-up.
