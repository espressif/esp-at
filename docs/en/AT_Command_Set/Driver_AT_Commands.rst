.. _Driver-AT:

Driver AT Commands
================================================

:link_to_translation:`zh_CN:[中文]`

- :ref:`Introduction <cmd-driver-intro>`
-  :ref:`AT+DRVADC <cmd-DRVADC>`: Read ADC channel value.
-  :ref:`AT+DRVPWMINIT <cmd-DRVPWMINIT>`: Initialize PWM driver.
-  :ref:`AT+DRVPWMDUTY <cmd-DRVPWMDUTY>`: Set PWM duty.
-  :ref:`AT+DRVPWMFADE <cmd-DRVPWMFADE>`: Set PWM fade.
-  :ref:`AT+DRVI2CINIT <cmd-DRVI2CINIT>`: Initialize I2C master driver.
-  :ref:`AT+DRVI2CRD <cmd-DRVI2CRD>`: Read I2C data.
-  :ref:`AT+DRVI2CWRDATA <cmd-DRVI2CWRDATA>`: Write I2C data.
-  :ref:`AT+DRVI2CWRBYTES <cmd-DRVI2CWRBYTES>`: Write no more than 4 bytes I2C data.
-  :ref:`AT+DRVSPICONFGPIO <cmd-DRVSPICONFGPIO>`: Configure SPI GPIO.
-  :ref:`AT+DRVSPIINIT <cmd-DRVSPIINIT>`: Initialize SPI master driver.
-  :ref:`AT+DRVSPIRD <cmd-DRVSPIRD>`: Read SPI data.
-  :ref:`AT+DRVSPIWR <cmd-DRVSPIWR>`: Write SPI data.

.. _cmd-driver-intro:

Introduction
------------

.. important::
  The default AT firmware does not support the AT commands listed on this page. If you need {IDF_TARGET_NAME} to support driver commands, you can compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections:

  - Enable ``Component config`` -> ``AT`` -> ``AT driver command support``

.. _cmd-DRVADC:

:ref:`AT+DRVADC <Driver-AT>`: Read ADC Channel Value
--------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+DRVADC=<channel>,<atten>

**Response:**

::

    +DRVADC:<raw data>

    OK

Parameters
^^^^^^^^^^

-  **<channel>**: ADC1 channel.

  .. only:: esp32 or esp32s2

    - For {IDF_TARGET_NAME} devices, the range is [0,7].

      .. list-table::
        :header-rows: 1

        * - CHANNEL
          - GPIO
        * - 0
          - GPIO36
        * - 1
          - GPIO37
        * - 2
          - GPIO38
        * - 3
          - GPIO39
        * - 4
          - GPIO32
        * - 5
          - GPIO33
        * - 6
          - GPIO34
        * - 7
          - GPIO35

  .. only:: esp32c3 or esp32c2 or esp32c6

    - For {IDF_TARGET_NAME} devices, the range is [0,4].

      .. list-table::
        :header-rows: 1

        * - CHANNEL
          - GPIO
        * - 0
          - GPIO0
        * - 1
          - GPIO1
        * - 2
          - GPIO2
        * - 3
          - GPIO3
        * - 4
          - GPIO4

-  **<atten>**: attenuation.

  .. only:: esp32 or esp32s2

    -  0: 0 dB attenuation, effective measurement range is [100, 950] mV.
    -  1: 2.5 dB attenuation, effective measurement range is [100, 1250] mV.
    -  2: 6 dB attenuation, effective measurement range is [150, 1750] mV.
    -  3: 11 dB attenuation, effective measurement range is [150, 2450] mV.

  .. only:: esp32c3 or esp32c2 or esp32c6

    -  0: 0 dB attenuation, effective measurement range is [0, 750] mV.
    -  1: 2.5 dB attenuation, effective measurement range is [0, 1050] mV.
    -  2: 6 dB attenuation, effective measurement range is [0, 1300] mV.
    -  3: 11 dB attenuation, effective measurement range is [0, 2500] mV.

- **<raw data>**: ADC channel value. 

Notes
^^^^^

-  ESP-AT only supports ADC1.
-  {IDF_TARGET_NAME} support 12-bit width.
-  For details on how to convert the channel value into voltage, please refer to `ADC Conversion <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-reference/peripherals/adc_continuous.html#adc-continuous-read-conversion-result>`__.

Example
^^^^^^^^

.. only:: esp32 or esp32s2

  ::

    // For {IDF_TARGET_NAME}, 0 dB attenuation, effective measurement range is [100, 950] mV
    // The returned 2048 means the voltage is 2048 / 4095 * 950 = 475.12 mV
    AT+DRVADC=0,0
    +DRVADC:2048

    OK

.. only:: esp32c3 or esp32c2 or esp32c6

  ::

    // For {IDF_TARGET_NAME}, 0 dB attenuation, effective measurement range is [0, 750] mV
    // The returned 2048 means the voltage is 2048 / 4095 * 750 = 375.09 mV
    AT+DRVADC=0,0
    +DRVADC:2048

    OK

.. _cmd-DRVPWMINIT:

:ref:`AT+DRVPWMINIT <Driver-AT>`: Initialize PWM Driver
----------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+DRVPWMINIT=<freq>,<duty_res>,<ch0_gpio>[,...,<ch3_gpio>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<freq>**: LEDC timer frequency. Unit: Hz. Range: 1 Hz ~ 8 MHz.
-  **<duty_res>**: LEDC channel duty resolution. Range: 0 ~ 20 bits.
-  **<chx_gpio>**: LEDC output GPIO number of channel x. For example, if you want to use GPIO16 as channel 0, set <ch0_gpio> to 16.

Notes
^^^^^

-  AT can support a maximum of 4 channels.
-  The number of channels that you initialize using this command will determine how many channels you can set using other PWM commands, including :ref:`AT+DRVPWMDUTY <cmd-DRVPWMDUTY>` and :ref:`AT+DRVPWMFADE <cmd-DRVPWMFADE>`. For example, if you initialize two channels, you can only change the two channels' PWM duty using command ``AT+DRVPWMDUTY``.
-  The frequency and the duty resolution are interdependent. See `Supported Range of Frequency and Duty Resolutions <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-reference/peripherals/ledc.html#supported-range-of-frequency-and-duty-resolutions>`_ for more details.

Example
^^^^^^^^

::

    AT+DRVPWMINIT=5000,13,17,16,18,19  // set 4 channels; frequency: 5 kHz; duty resolution: 13 bits
    AT+DRVPWMINIT=10000,10,17          // only use channel 0, frequency: 10 kHz; duty resolution: 10 bits; other PMW commands can only set one channel

.. _cmd-DRVPWMDUTY:

:ref:`AT+DRVPWMDUTY <Driver-AT>`: Set PWM Duty
-------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+DRVPWMDUTY=<ch0_duty>[,...,<ch3_duty>]

**Response:**

::

    OK

Parameter
^^^^^^^^^^

-  **<duty>**: LEDC channel duty. Range: [0,2 :sup:`duty_resolution`].

Notes
^^^^^

-  AT can support a maximum of 4 channels.
-  If you do not want to set ``<duty>`` for a specific channel, just omit it.

Example
^^^^^^^^

::

    AT+DRVPWMDUTY=255,512   // set channel 0 to duty 255, set channel 1 to duty 512
    AT+DRVPWMDUTY=,,0       // set channel 2 to duty 0

.. _cmd-DRVPWMFADE:

:ref:`AT+DRVPWMFADE <Driver-AT>`: Set PWM Fade
-----------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

     AT+DRVPWMFADE=<ch0_target_duty>,<ch0_fade_time>[,...,<ch3_target_duty>,<ch3_fade_time>]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<target_duty>**: target duty of fading. Range: [0, 2 :sup:`duty_resolution`–1].
-  **<fade_time>**: the maximum time of fading. Unit: millisecond.

Notes
^^^^^

-  AT can support a maximum of 4 channels.
-  If you do not want to set ``<target_duty>`` and ``<fade_time>`` for a specific channel, just omit them.

Example
^^^^^^^^

::

    AT+DRVPWMFADE=,,0,1000           // use one second to change channel 1 duty to 0
    AT+DRVPWMFADE=1024,1000,0,2000,  // use one second time to change channel 0 duty to 1024, two seconds to change channel 1 duty to 0

.. _cmd-DRVI2CINIT:

:ref:`AT+DRVI2CINIT <Driver-AT>`: Initialize I2C Master Driver
---------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

     AT+DRVI2CINIT=<num>,<scl_io>,<sda_io>,<clock>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<num>**: I2C port number. Range: 0 ~ 1. If the following parameters are not set, AT will deinitialize the I2C port. 
-  **<scl_io>**: GPIO number for I2C SCL signal.
-  **<sda_io>**: GPIO number for I2C SDA signal.
-  **<clock>**: I2C clock frequency for master mode. Unit: Hz. Maximum: 1 MHz.

Note
^^^^^

-  This command only supports I2C masters.

Example
^^^^^^^^

::

    AT+DRVI2CINIT=0,25,26,1000  // initialize I2C0; GPIO25 is SCL; GPIO26 is SDA; I2C clock is 1 kHz
    AT+DRVI2CINIT=0             // deinitialize I2C0

.. _cmd-DRVI2CRD:

:ref:`AT+DRVI2CRD <Driver-AT>`: Read I2C Data
-------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

     AT+DRVI2CRD=<num>,<address>,<length>

**Response:**

::

    +DRVI2CRD:<read data>
    OK

Parameters
^^^^^^^^^^

-  **<num>**: I2C port number. Range: 0 ~ 1.
-  **<address>**: I2C slave device address.

   -  7-bit address: 0 ~ 0x7F.
   -  10-bit address: The first seven bits of the first byte are the combination 1111 0XX of which the last two bits (XX) are the two Most Significant Bits (MSBs) of the 10-bit address. For example, if the 10-bit address is 0x2FF (b'1011111111), the input address should be 0x7AFF (b'111101011111111).

-  **<length>**: I2C data length. Range: 1 ~ 2048.
-  **<read data>**: I2C data.

Note
^^^^^

-  I2C transmission timeout is one second.

Example
^^^^^^^^

::

    AT+DRVI2CRD=0,0x34,1     // I2C0 reads one byte data from address 0x34
    AT+DRVI2CRD=0,0x7AFF,1   // I2C0 reads one byte data from 10-bit address 0x2FF

    // I2C0 reads address 0x34, register address 0x27, read 2 bytes
    AT+DRVI2CWRBYTES=0,0x34,1,0x27     // I2C0 first writes device address 0x34, register address 0x27
    AT+DRVI2CRD=0,0x34,2               // I2C0 reads 2 bytes

.. _cmd-DRVI2CWRDATA:

:ref:`AT+DRVI2CWRDATA <Driver-AT>`: Write I2C Data
------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

     AT+DRVI2CWRDATA=<num>,<address>,<length>

**Response:**

::

    OK
    >

This response indicates that you should enter the data you want to write. When the requirement of data length is met, the data transmission starts.

If the data is transmitted successfully, AT returns:

::

    OK 

If the data transmission fails, AT returns:

::

    ERROR

Parameters
^^^^^^^^^^

-  **<num>**: I2C port number. Range: 0 ~ 1.
-  **<address>**: I2C slave device address.

   -  7-bit address: 0 ~ 0x7F.
   -  10-bit address: The first seven bits of the first byte are the combination 1111 0XX of which the last two bits (XX) are the two Most Significant Bits (MSBs) of the 10-bit address. For example, if the 10-bit address is 0x2FF (b'1011111111), the input address should be 0x7AFF (b'111101011111111).

-  **<length>**: I2C data length. Range: 1 ~ 2048.

Note
^^^^^

-  I2C transmission timeout is one second.

Example
^^^^^^^^

::

    AT+DRVI2CWRDATA=0,0x34,10   // I2C0 writes 10 bytes data to address 0x34

.. _cmd-DRVI2CWRBYTES:

:ref:`AT+DRVI2CWRBYTES <Driver-AT>`: Write No More Than 4 Bytes I2C Data
---------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

     AT+DRVI2CWRBYTES=<num>,<address>,<length>,<data>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<num>**: I2C port number. Range: 0 ~ 1.
-  **<address>**: I2C slave device address.

   -  7-bit address: 0 ~ 0x7F.
   -  10-bit address: The first seven bits of the first byte are the combination 1111 0XX of which the last two bits (XX) are the two Most Significant Bits (MSBs) of the 10-bit address. For example, if the 10-bit address is 0x2FF (b'1011111111), the input address should be 0x7AFF (b'111101011111111).

-  **<length>**: the length of the I2C data you want to write. Range: 1 ~ 4 bytes.
-  **<data>**: the data of ``<length>`` long. Range: 0 ~ 0xFFFFFFFF.

Note
^^^^^

-  I2C transmission timeout is one second.

Example
^^^^^^^^

::

    AT+DRVI2CWRBYTES=0,0x34,2,0x1234     // I2C0 writes 2 bytes data 0x1234 to address 0x34
    AT+DRVI2CWRBYTES=0,0x7AFF,2,0x1234   // I2C0 writes 2 bytes data 0x1234 to 10-bit address 0x2FF

    // I2C0 writes address 0x34; register address: 0x27; data: c0xFF
    AT+DRVI2CWRBYTES=0,0x34,2,0x27FF

.. _cmd-DRVSPICONFGPIO:

:ref:`AT+DRVSPICONFGPIO <Driver-AT>`: Configure SPI GPIO
---------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

     AT+DRVSPICONFGPIO=<mosi>,<miso>,<sclk>,<cs>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<mosi>**: GPIO pin for Master Out Slave In signal.
-  **<miso>**: GPIO pin for Master In Slave Out signal, or -1 if not used.
-  **<sclk>**: GPIO pin for SPI Clock signal.
-  **<cs>**: GPIO pin for slave selection signal, or -1 if not used.

.. _cmd-DRVSPIINIT:

:ref:`AT+DRVSPIINIT <Driver-AT>`: Initialize SPI Master Driver
----------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+DRVSPIINIT=<clock>,<mode>,<cmd_bit>,<addr_bit>,<dma_chan>[,bits_msb]

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<clock>**: Clock speed, divisors of 80 MHz. Unit: Hz. Maximum: 40 MHz.
-  **<mode>**: SPI mode. Range: 0 ~ 3.
-  **<cmd_bit>**: Default amount of bits in command phase. Range: 0 ~ 16.
-  **<addr_bit>**: Default amount of bits in address phase. Range: 0 ~ 64.
-  **<dma_chan>**: Either channel 1 or 2, or 0 in the case when no DMA is required.
-  **<bits_msb>**: SPI data format:
   
   - Bit0:
    
     - 0: Transmit MSB first (default).
     - 1: Transmit LSB first.
   
   - Bit1:

     - 0: Receive data MSB first (default).
     - 1: Receive data LSB first.

Note
^^^^^

- You should configure SPI GPIO before SPI initialization.

Example
^^^^^^^^

::

    AT+DRVSPIINIT=102400,0,0,0,0,3 // SPI clock: 100 kHz; mode: 0; both command and address bits are 0; not use DMA; transmit and receive LSB first
    OK
    AT+DRVSPIINIT=0   // delete SPI Driver
    OK 

.. _cmd-DRVSPIRD:

:ref:`AT+DRVSPIRD <Driver-AT>`: Read SPI Data
-------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

     AT+DRVSPIRD=<data_len>[,<cmd>,<cmd_len>][,<addr>,<addr_len>]

**Response:**

::

    +DRVSPIRD:<read data>
    OK

Parameters
^^^^^^^^^^

-  **<data_len>**: length of SPI data you want to read. Range: 1 ~ 4092 bytes.
-  **<cmd>**: command data. The length of the data is set in ``<cmd_len>``.
-  **<cmd_len>**: command length in this transaction. Range: 0 ~ 2 bytes.
-  **<addr>**: command address. The length of the address is set in ``<addr_len>``.
-  **<addr_len>**: The address length in this transaction. Range: 0 ~ 4 bytes.

Note
^^^^^

-  If you do not use DMA, the maximum ``<data_len>`` you can set is 64 bytes each time.

Example
^^^^^^^^

::

    AT+DRVSPIRD=2  // read 2 bytes data
    +DRVI2CREAD:ffff
    OK

    AT+DRVSPIRD=2,0x03,1,0x001000,3  // read 2 bytes data; <cmd> is 0x03; <cmd_len> is 1 byte; <addr> is 0x1000; <addr_len> is 3 bytes
    +DRVI2CREAD:ffff
    OK

.. _cmd-DRVSPIWR:

:ref:`AT+DRVSPIWR <Driver-AT>`: Write SPI Data
--------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+DRVSPIWR=<data_len>[,<cmd>,<cmd_len>][,<addr>,<addr_len>]

**Response:**

When ``<data_len>`` is larger than 0, AT returns:

::

    OK
    >

This response indicates that you should enter the data you want to write. When the requirement of data length is met, the data transmission starts.

If the data is transmitted successfully, AT returns:

::

    OK

When ``<data_len>`` is equal to 0, which means AT transmits commands and addresses only, and no SPI data, AT returns:

::

    OK 

Parameters
^^^^^^^^^^

-  **<data_len>**: SPI data length. Range: 0 ~ 4092.
-  **<cmd>**: command data. The length of the data is set in ``<cmd_len>``.
-  **<cmd_len>**: command length in this transaction. Range: 0 ~ 2 bytes.
-  **<addr>**: command address. The length of the address is set in ``<addr_len>``.
-  **<addr_len>**: The address length in this transaction. Range: 0 ~ 4 bytes.

Note
^^^^^

-  If you do not use DMA, the maximum ``<data_len>`` you can set is 64 bytes each time.

Example
^^^^^^^^

::

    AT+DRVSPIWR=2  // write 2 bytes data
    OK
    >              // begin receiving serial data
    OK

    AT+DRVSPIWR=0,0x03,1,0x001000,3  // write 0 byte data; <cmd> is 0x03; <cmd_len> is 1 byte; <addr> is 0x1000; <addr_len> is 3 bytes
    OK