SPI AT Guide
===============

This document mainly introduces the implementation and use of SPI AT, mainly involving the following aspects:

.. contents::
   :local:
   :depth: 1

Overview
--------------

SPI AT is based on ESP-AT project and uses SPI protocol for data communication. When SPI AT is used, MCU works as SPI master and {IDF_TARGET_NAME} AT device works as SPI slave. Both sides of communication exchange data based on AT command through SPI protocol.

Why SPI AT?
^^^^^^^^^^^^^^^^^^^

ESP-AT project uses UART protocol for data communication by default, but UART protocol is not applicable in some application scenarios that need high-speed data transmission. In this case, the SPI protocol which supports a higher transmission rate is a better choice.

How to enable SPI AT?
^^^^^^^^^^^^^^^^^^^^^^^^^

You can configure and enable SPI AT through the following steps:

1. ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``communicate method for AT command`` -> ``AT through HSPI`` to enable SPI AT.
2. ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``communicate method for AT command`` -> ``AT SPI Data Transmission Mode`` to choose the SPI data transmission mode.
3. ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``communicate method for AT command`` -> ``AT SPI GPIO settings`` to change the default pin assignments for SPI AT.
4. ``./build.py menuconfig`` -> ``Component config`` -> ``AT`` -> ``communicate method for AT command`` -> ``AT SPI driver settings`` to choose the SPI slave mode, and config the buffer size for data transmission.
5. Recompile the ``esp-at`` project(see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`), download AT bin into flash.

The Default Pin Assignment
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following pin assignments are used by default:

.. list-table:: The Default Pins for SPI AT
   :widths: 10 25
   :header-rows: 1

   * - Signal
     - GPIO Number
   * - SCLK
     - 6
   * - MISO
     - 2
   * - MOSI
     - 7
   * - CS
     - 10
   * - HANDSHAKE
     - 3
   * - GND
     - GND
   * - QUADWP (qio/qout):sup:`1`
     - 8
   * - QUADHD (qio/qout):sup:`1`
     - 9

**Note** 1: QUADWP and QUADHD signals are only used for 4-bit (qio/qout) transactions.

You can change the default pin assignments by ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``communicate method for AT command`` > ``AT through HSPI`` > ``AT SPI GPIO settings`` and compile the project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`).

How to Use SPI AT?
-----------------------

When SPI AT is used, {IDF_TARGET_NAME} works in SPI half-duplex mode as a slave. 

Handshake Pin
^^^^^^^^^^^^^^^^

Since SPI is a master-slave model, all transmission is initiated by the master, and the slave cannot actively transmit data. However, when AT command is used for data interaction, {IDF_TARGET_NAME} (slave) is required to actively report some information. Therefore, a handshake line is added between SPI master and slave to achieve the purpose.

When the slave needs to transfer data, it pulls up the handshake pin, generating a rising edge GPIO interrupt on the master side. Then, the master initiates communication with the slave. After the data is transferred, the slave pulls down the handshake pin to end this communication.

The specific methods of using the handshake line are as follows:

- When master sends AT commands to slave via SPI, the workflow with an extra handshake line is as follows:

  1. The master sends a request for data transmission to the slave, and then waits for the signal sent by the slave to the handshake line to allow data transmission.
  2. After the master detects the permission signal sent by the slave on the handshake line, it starts to send data.
  3. After sending the data, the master notifies slave that the data transmission is finished.

- When slave sends AT responses to master via SPI, the workflow with an extra handshake line is as follows:

  1. The slave sends a signal through the handshake line to inform the master to start receiving data.
  2. The master receives data and notifies slave that the data transmission is finished after receiving all data.

Communication Formats
^^^^^^^^^^^^^^^^^^^^^^^

The communication format used by SPI AT is CMD+ADDR+DUMMY+DATA (Read or Write). When using SPI AT, some communication messages used by SPI master are described as follows:

- The message used by the master to send data to slave:

.. list-table:: Master write data to slave
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD (1 byte)
     - ADDR (1 byte)
     - DUMMY (1 byte)
     - DATA (Up to 4092 bytes)
   * - 0x3
     - 0x0
     - 0x0
     - data_buffer

- The message used by the master to inform the slave all data has been sent:

.. list-table:: Master write data done
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD (1 byte)
     - ADDR (1 byte)
     - DUMMY (1 byte)
     - DATA
   * - 0x7
     - 0x0
     - 0x0
     - null

- The message used by the master to receive data from the slave:

.. list-table:: Master read data from slave
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD (1 byte)
     - ADDR (1 byte)
     - DUMMY (1 byte)
     - DATA (Up to 4092 bytes)
   * - 0x4
     - 0x0
     - 0x0
     - data_buffer

- The message used by the master to inform the slave all data has been received:

.. list-table:: Master read data done
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD (1 byte)
     - ADDR (1 byte)
     - DUMMY (1 byte)
     - DATA
   * - 0x8
     - 0x0
     - 0x0
     - null

- The message used by the master to send a request to send data:

.. list-table:: Master request to send data
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD (1 byte)
     - ADDR (1 byte)
     - DUMMY (1 byte)
     - DATA (4 bytes)
   * - 0x1
     - 0x0
     - 0x0
     - data_info

The 4-byte length ``data_info`` contains the information about the packet to be sent. The specific format is as follows:

1. 0~15 bits: the length of the data that the master want to send to the slave.
2. 16~23 bits: the sequence number of the packet sent by the master to the slave.
3. 24~31 bits: the magic num, and default value is 0xFE.

- After receiving the signal from the handshake line, the master can send the message to query the read or write status of the slave:

.. list-table:: Master query the read/write status of the slave
   :header-rows: 1
   :widths: 25 25 15 30

   * - CMD (1 byte)
     - ADDR (1 byte)
     - DUMMY (1 byte)
     - DATA (4 bytes)
   * - 0x2
     - 0x4
     - 0x0
     - slave_status

After sending the query request, the slave's status returned will be stored in the 4-byte length ``slave_status``, the specific format is as follows:

1. 0~15 bits: the length of the data the slave wants to send to the master. This field is valid only when the slave is readable.
2. 16~23 bits: the sequence number of the packet to be sent. The maximum sequence number is 0xFF, and if the number is reached, the sequence number is incremented by 1 from 0.
3. 24~31 bits: the slave status (readable/writable). 0x1 means readable, and 0x2 means writable.

SPI AT Workflow
^^^^^^^^^^^^^^^^^^^^^^
The workflows mainly includes two aspects:

- When master sends AT commands to slave, the workflow is as follows: 

.. code-block:: none

         | SPI master |                            | SPI slave |
                |                                        |
                |   -------step 1: request to send---->  |
                |                                        |
                |   <------step 2: GPIO interrupt------  |
                |                                        |
                |   -------step 3: read slave status-->  |
                |                                        |
                |   -------step 4: send data---------->  |
                |                                        |
                |   -------step 5: send done---------->  |

The specific description of each step is as follows:

step 1. The master sends a request for data transmission to the slave.
step 2. The slave receives the request from the master. If the master is allowed to send data, the slave sets the status register, and then triggers the GPIO interrupt on the master through the handshake line.
step 3. After receiving the interrupt, master will query the status register of slave. If the query result shows that the slave is in the writable state, the master starts to send data.
step 4. The master send data to the slave.
step 5. After sending the data, the master notifies slave that the data transmission is finished.

- When master receives AT responses from slave, the workflow is as follows: 

.. code-block:: none

         | SPI master |                            | SPI slave |
                |                                        |
                |   <------step 1: GPIO interrupt------  |
                |                                        |
                |   -------step 2: read slave status-->  |
                |                                        |
                |   <------step 3: send data-----------  |
                |                                        |
                |   -------step 4: receive done------->  |

The specific description of each step is as follows:

step 1. The slave sets the status register, and then triggers the GPIO interrupt on the master through the handshake line.
step 2. After receiving the interrupt, master will query the status register of slave. If the query result shows that the slave is in the readable state, the master starts to receive data.
step 3. The master receives the data send by the slave.
step 4. After receiving all data, the master notifies the slave that the data transmission is finished.

Sample Code of SPI Master for SPI AT
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SPI AT is used as the SPI slave. For a code example of communication between the SPI master and the SPI slave, please refer to :example:`at_spi_master/spi/esp32_c_series`.

**Note** 1. Before developing with MCU, it is highly recommended to run this example using ESP32-C3 or ESP32 emulated MCU as SPI master to make it easier to debug problems.

SPI AT Throughput
----------------------

Introduction of the Test
^^^^^^^^^^^^^^^^^^^^^^^^^^

- An ESP32 or ESP32-C3 development board is been used as SPI master. The application runs in the board can be found under the directory :example:`at_spi_master/spi/esp32_c_series` of the `ESP-AT <https://github.com/espressif/esp-at>`_ project. Some related configurations are described below: 

1. Hardware configuration: The frequency of CPU is 240 MHz, flash SPI mode is in QIO mode with 40 MHz.

2. Software configuration: The `ESP-IDF <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html>`_ version is v4.3. The size of streambuffer is 8192 bytes.

- An {IDF_TARGET_NAME}-DevKitC development board is been used as SPI slave. Please refer to :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it` to build your own ESP-AT project and flash the generated binary files into the board. The board works in the TCP passthrough mode, and some related configurations are described below:

1. Hardware configuration: The frequency of CPU is 160 MHz.

2. Software configuration: The size of streambuffer is 8192 bytes, the sdkconfig is `sdkconfig.defaults.esp32c3 <https://github.com/espressif/esp-idf/blob/master/examples/wifi/iperf/sdkconfig.defaults.esp32c3>`_.

Reference Results
^^^^^^^^^^^^^^^^^^^^

The table below shows the throughput results we got in a shield box.

.. list-table:: SPI AT Wi-Fi TCP Throughput
   :header-rows: 1
   :widths: 15 20 25 25

   * - Clock
     - SPI mode
     - master->slave
     - slave->master
   * - 10 M
     - Standard
     - 0.95 MByte/s
     - 1.00 MByte/s
   * - 10 M
     - Dual
     - 1.37 MByte/s
     - 1.29 MByte/s
   * - 10 M
     - Quad
     - 1.43 MByte/s
     - 1.31 MByte/s
   * - 20 M
     - Standard
     - 1.41 MByte/s
     - 1.30 MByte/s
   * - 20 M
     - Dual
     - 1.39 MByte/s
     - 1.30 MByte/s
   * - 20 M
     - Quad
     - 1.39 MByte/s
     - 1.30 MByte/s
   * - 40 M
     - Standard
     - 1.37 MByte/s
     - 1.30 MByte/s
   * - 40 M
     - Dual
     - 1.40 MByte/s
     - 1.31 MByte/s
   * - 40 M
     - Quad
     - 1.48 MByte/s
     - 1.31 MByte/s

**Note** 1: When SPI clock frequency is high, due to the limitation of upper network components, the communication rate of Dual or Quad mode is not significantly improved compared with Standard mode.

**Note** 2: For more information about SPI communication, please refer to the `Technical Reference Manuals <https://www.espressif.com/en/support/documents/technical-documents>`_.
