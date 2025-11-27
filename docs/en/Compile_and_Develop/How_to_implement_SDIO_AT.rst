{IDF_TARGET_NAME} SDIO AT Guide
===============================

:link_to_translation:`zh_CN:[中文]`

Introduction
------------

{IDF_TARGET_NAME} SDIO AT uses SDIO rather than UART as the communication interface between {IDF_TARGET_NAME} (SDIO slave) and the host MCU.

SDIO can use either 1-bit or 4-bit data transfer mode.

-  For 1-bit mode at least 4 pins are required: CMD, CLK, DAT0, DAT1; where DAT1 runs as the interrupt pin.
-  For 4-bit mode, two more pins (DAT2 and DAT3) are needed.

The SDIO slave pins are as below:

.. only:: esp32

   -  CLK is GPIO14
   -  CMD is GPIO15
   -  DAT0 is GPIO2
   -  DAT1 is GPIO4
   -  DAT2 is GPIO12 (for 4-bit mode only)
   -  DAT3 is GPIO13 (for 4-bit mode only)

.. only:: esp32c6 or esp32c61

   -  CLK is GPIO19
   -  CMD is GPIO18
   -  DAT0 is GPIO20
   -  DAT1 is GPIO21
   -  DAT2 is GPIO22 (for 4-bit mode only)
   -  DAT3 is GPIO23 (for 4-bit mode only)

.. only:: esp32c5

   -  CLK is GPIO9
   -  CMD is GPIO10
   -  DAT0 is GPIO8
   -  DAT1 is GPIO7
   -  DAT2 is GPIO14 (for 4-bit mode only)
   -  DAT3 is GPIO13 (for 4-bit mode only)

Implement SDIO AT
-----------------

Before testing SDIO AT communication, please configure {IDF_TARGET_NAME} hardware according to `SD Pull-up Requirements <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-reference/peripherals/sd_pullup_requirements.html#solutions>`_, otherwise, exceptions may occur during SDIO communication.

SDIO Slave
^^^^^^^^^^^

By default, the ESP-AT project uses UART as the the communication interface. When {IDF_TARGET_NAME} serves as the SDIO slave, please go to ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``communicate method for AT command`` > ``AT via SDIO`` to switch to the SDIO interface. Then, recompile the ESP-AT project, flash and run the new bin.


SDIO MCU
^^^^^^^^

ESP-AT provides the :example:`at_sdio_host` example for {IDF_TARGET_NAME} or STM32 serving as the SDIO host. For {IDF_TARGET_NAME} serving as the host, you can use the esp-idf of the AT project to compile and flash; for STM32, as the provided solution is based on STM32F103ZET, please use Keil5 to compile and flash as the application.

For other devices working as SDIO hosts, you can develop based on the above example.

SDIO Data Transimission
-----------------------

SDIO Host
^^^^^^^^^

1. Initialize the SDIO host

   -  The SDIO host initialization is mainly the initialization of the SDIO protocol, including setting 1-bit or 4-bit transfer mode, SDIO frequency, and SD mode.

2. Negotiate SDIO communication

   -  Negotiate SDIO parameters with the slave according to the SDIO specification. Please note that if the host and slave reboot at the same time, the host must wait until the slave startup is done to start the negotiation. Usually, the host needs to add a delay at boot time to achieve it.

3. Receive data

   -  When the SDIO host detects an interrupt from DAT1 and determines that the slave sends new packets, the host will read those data by CMD53.

4. Send data

   -  In SDIO AT demo, the data inputs from a serial port. When the SDIO host gets the data, it will transfer it to the slave through SDIO by CMD53.
   -  Please note that if the sending time out, there may be something wrong with the slave, please re-initiate both the host and slave.
   -  After :ref:`AT+RST <cmd-RST>` or :ref:`AT+RESTORE <cmd-RESTORE>` is called, both the host and slave should be initialized again.

SDIO Slave
^^^^^^^^^^^

When the SDIO slave receives data from the SDIO host, it will inform the AT core and give the data to the AT core to handle. After the AT core finishes the work, the slave will send data to the host as feedback.

1. Initialize the SDIO slave

   -  Call ``sdio_slave_initialize`` to initialize the SDIO slave driver.
   -  Call ``sdio_slave_recv_register_buf`` to register receiving buffer. To make it faster to receive data, you can register multiple buffers.
   -  Call ``sdio_slave_recv_load_buf`` to load the receiving buffer to receive data.
   -  Call ``sdio_slave_set_host_intena`` to set host interrupts, of which the  ``SDIO_SLAVE_HOSTINT_SEND_NEW_PACKET`` is used to notify that a new packet is sent from the host.
   -  Call ``sdio_slave_start`` to start SDIO hardware transmission.

2. Send data

   -  Since the SDIO slave data transmission is via DMA, you need to copy the data from AT core to the memory which DMA can read firstly. Call ``sdio_slave_transmit`` to send the data.

3. Receive data

   -  To speed up the data transmission, after receiving data by ``sdio_slave_recv``, a circular linked list is used to transmit the received data to the AT core.

.. only:: esp32c5

  Test Results
  ------------

  The table below shows the communication speed results we obtained in the shielded box:

  .. list-table:: SDIO AT Wi-Fi TCP Communication Speed
     :header-rows: 1

     * - Transfer Mode
       - TCP Send (Mbps)
       - TCP Receive (Mbps)
     * - 1-bit
       - 17.4
       - 15.2

  Note: The SDIO HOST hardware used for testing is ESP32. SDIO operates in 1-bit 40MHz mode, and Wi-Fi operates in 802.11ac mode.