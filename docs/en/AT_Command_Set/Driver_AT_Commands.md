<a name="Driver-AT"></a>
## [ESP32 & ESP32S2 Only] Driver AT Commands 
* [AT+DRVADC](#cmd-DRVADC) : Read ADC channel value.
* [AT+DRVPWMINIT](#cmd-DRVPWMINIT) -  Init PWM driver
* [AT+DRVPWMDUTY](#cmd-DRVPWMDUTY) -  Change PWM duty
* [AT+DRVPWMFADE](#cmd-DRVPWMFADE) -  Change PWM gradient
* [AT+DRVI2CINIT](#cmd-DRVI2CINIT) -  Init I2C master driver
* [AT+DRVI2CRD](#cmd-DRVI2CRD) -  Read I2C data
* [AT+DRVI2CWRDATA](#cmd-DRVI2CWRDATA) -  Write I2C data
* [AT+DRVI2CWRBYTES](#cmd-DRVI2CWRBYTES) -  Write no more than 4bytes I2C data
* [AT+DRVSPICONFGPIO](#cmd-DRVSPICONFGPIO) -  Config SPI GPIO 
* [AT+DRVSPIINIT](#cmd-DRVSPIINIT) -  Init SPI master driver
* [AT+DRVSPIRD](#cmd-DRVSPIRD) -  Read SPI data
* [AT+DRVSPIWR](#cmd-DRVSPIWR) -  Write SPI data

<a name="cmd-DRVADC"></a>
### [AT+DRVADC](#Driver-AT)- Read ADC channel value
Set Command:  

    AT+DRVADC=<channel>,<atten>
Response:

    +DRVADC:<raw data>
    OK

Parameters:

- **\<channel>** : ADC1 channel, can be set to 0 ~ 7. Refer to the datasheet for the corresponding pin.
- **\<atten>** : attenuation
  - 0 :  0dB attenuaton gives full-scale voltage 1.1V
  - 1 :  2.5dB attenuation gives full-scale voltage 1.5V
  - 2 :  6dB attenuation gives full-scale voltage 2.2V
  - 3 :  11dB attenuation gives full-scale voltage 3.9V

**Note:**

- Only support ADC1.
- ESP32 only support BIT12 width, and ESP32S2 only support BIT13 width.

**Example:**

    AT+DRVADC=0,0   // ADC1 0 channel, voltage: 0 ~ 1.1V
    +DRVADC:2048    // For ESP32, the voltage is : 2048 / 4096 * 1.1 = 0.55. 
                    // For ESP32S2, the voltage is : 2048 / 8192 * 1.1 = 0.264
    OK

<a name="cmd-DRVPWMINIT"></a>

### [AT+DRVPWMINIT](#Driver-AT) - Init PWM driver

Set Command:  

    AT+DRVPWMINIT=<freq>,<duty_res>,<ch0_gpio>[,...,<ch3_gpio>]

Response:

    OK

Parameters:

- **\<freq>** : LEDC timer frequency (Hz), range : 1Hz ~ 8MHz
- **\<duty_res>** : LEDC channel duty resolution (0 ~ 20). 
- **\<gpio>** : LEDC output gpio num,  if you want to use gpio16, gpio_num = 16

**Note:**

- Max support 4 channel
- Other PWM command rely on the setting channel count
- The frequency and the duty resolution are interdependent, please refer to [Supported Range of Frequency and Duty Resolutions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-reference/peripherals/ledc.html#supported-range-of-frequency-and-duty-resolutions).

**Example:**

    AT+DRVPWMINIT=5000,13,17,16,18,19    // set 4 channel, freq:5k, duty_res:13
    AT+DRVPWMINIT=10000,10,17            // only use channel 0, freq:10k, duty_res:10, other PMW command only can set 1 chaneel

<a name="cmd-DRVPWMDUTY"></a>

### [AT+DRVPWMDUTY](#Driver-AT) - Change channel PWM duty

Set Command:  

    AT+DRVPWMDUTY=<ch0_duty>,[...,<ch3_duty>]

Response:

    OK

Parameters:

- **\<duty>** : LEDC channel duty, the range of duty setting is [0, (2**duty_resolution)]

**Note:**

- Max support 4 channel
- Omit parameter duty if do not config this channel

**Example:**

    AT+DRVPWMDUTY=255,512   // set channel 0 to duty 255, set channel 1 to duty 512
    AT+DRVPWMDUTY=,,0       // set channel 2 to duty 0

<a name="cmd-DRVPWMFADE"></a>

### [AT+DRVPWMFADE](#Driver-AT) - Change channel PWM gradient

Set Command:  

     AT+DRVPWMFADE=<ch0_target_duty>,<ch0_fade_time>,[...,<ch3_target_duty>,<ch3_fade_time>]

Response:

    OK

Parameters:

- **\<target_duty>** : Target duty of fading [0, (2**duty_resolution) - 1]
- **\<fade_time>** : The maximum time of the fading ( ms ).

**Note:**

- Max support 4 channel, 
- Omit target_duty and fade_time if do not config this channel

**Example:**

    AT+DRVPWMFADE=,,0,1000   // Use 1s time change CH1 duty to 0
    AT+DRVPWMFADE=1024,1000,0,2000,   // Use 1s time change CH0 duty to 1024, 2s time change CH1 duty to 0
<a name="cmd-DRVI2CINIT"></a>

### [AT+DRVI2CINIT](#Driver-AT) - Init I2C master driver

Set Command:  

     AT+DRVI2CINIT=<num>,<scl_io>,<sda_io>,<clock>

Response:

    OK

Parameters:

- **\<num>** : I2C port number[0 ~ 1], uninitialize I2C port if the following parameters not set
- **\<scl_io>** : GPIO number for I2C scl signal
- **\<sda_io>** : GPIO number for I2C sda signal
- **\<clock>** : I2C clock frequency for master mode, (no higher than 1MHz for now)

**Note:**

- Only support I2C master 

**Example:**

    AT+DRVI2CINIT=0,25,26,1000   // Init I2C0, GPIO25 is SCL, GPIO26 is SDA, I2C clock is 1K
    AT+DRVI2CINIT=0   // Deinit I2C0

<a name="cmd-DRVI2CRD"></a>

### [AT+DRVI2CRD](#Driver-AT) - Read I2C data

Set Command:  

     AT+DRVI2CRD=<num>,<address>,<length>

Response:

    +DRVI2CRD: <read data>
    OK

Parameters:

- **\<num>** : I2C port number[0 ~ 1]
- **\<address>** : I2C slave device address, 
    - 7bit address [0 ~ 0x7F]
    - 10bit address. The first seven bits are the combination 0x1E(b'11110), 
    for example, if 10 bit address is 0x2ff(b'1011111111), the input address should be 0x7AFF(b'111101011111111)
- **\<length>** : Read I2C data length [1 ~ 2048]

**Note:**

- I2C transmission timeout is 1s

**Example:**

    AT+DRVI2CRD=0,0x34,1   // I2C0 read one byte data from address 0x34
    AT+DRVI2CRD=0,0x7AFF,1   // I2C0 read one byte data from 10bit address 0x2ff

    // I2C0 read address 0x34, register address 0x27, read 2bytes
    AT+DRVI2CWRBYTES=0,0x34,1,0x27     // first writer device address 0x34, register address 0x27
    AT+DRVI2CRD=0,0x34,2               // read 2bytes

<a name="cmd-DRVI2CWRDATA"></a>

### [AT+DRVI2CWRDATA](#Driver-AT) - Write I2C data

Set Command:  

     AT+DRVI2CWRDATA=<num>,<address>,<length>

Response:

    OK
    >
Begin receiving serial data. When the requirement of data length is met, the transmission of data starts.
When the data is transmitted successfully, the system returns: 

    OK 

Parameters:

- **\<num>** : I2C port number[0 ~ 1]
- **\<address>** : I2C slave device address, 
    - 7bit address [0 ~ 0x7F]
    - 10bit address. The first seven bits are the combination 0x1E(b'11110), 
    for example, if 10 bit address is 0x2ff(b'1011111111), the input address should be 0x7AFF(b'111101011111111)
- **\<length>** : Write I2C data length [1 ~ 2048]

**Note:**

- I2C transmission timeout is 1s

**Example:**

    AT+DRVI2CWRDATA=0,0x34,10   // i2c0 Write 10bytes data to address 0x34

<a name="cmd-DRVI2CWRBYTES"></a>

### [AT+DRVI2CWRBYTES](#Driver-AT) - Write no more than 4bytes I2C data

Set Command:  

     AT+DRVI2CWRBYTES=<num>,<address>,<length>,<data>

Response:

    OK

Parameters:

- **\<num>** : I2C port number[0 ~ 1]
- **\<address>** : I2C slave device address, 
    - 7bit address [0 ~ 0x7F]
    - 10bit address. The first seven bits are the combination 0x1E(b'11110), 
    for example, if 10 bit address is 0x2ff(b'1011111111), the input address should be 0x7AFF(b'111101011111111)
- **\<length>** : Write I2C data length [1 ~ 4]
- **\<data>** : Write data[0 ~ 0xFFFFFFFF], depend on length

**Note:**

- I2C transmission timeout is 1s

**Example:**

    AT+DRVI2CWRBYTES=0,0x34,2,0x1234   // I2C0 write 2bytes data 0x1234 to address 0x34
    AT+DRVI2CWRBYTES=0,0x7AFF,2,0x1234   // I2C0 write 2bytes data 0x1234 to 10bit address 0x2ff

    // I2C0 write address 0x34, register address 0x27, data:0xff
    AT+DRVI2CWRBYTES=0,0x34,2,0x27ff

<a name="cmd-DRVSPICONFGPIO"></a>

### [AT+DRVSPICONFGPIO](#Driver-AT) - Config SPI GPIO 

Set Command:  

     AT+DRVSPICONFGPIO =<mosi>, <miso>, <sclk>, <cs>

Response:

    OK

Parameters:

- **\<mosi>** : GPIO pin for Master Out Slave In signal.
- **\<miso>** : GPIO pin for Master In Slave Out signal, or -1 if not used.
- **\<sclk>** : GPIO pin for Spi CLocK signal.
- **\<cs>** : GPIO pin for Master In Slave Out signal, or -1 if not used.

<a name="cmd-DRVSPIINIT"></a>

### [AT+DRVSPIINIT](#Driver-AT) - Init SPI master driver

Set Command:  

     AT+DRVSPIINIT=clock, mode, cmd_bit, addr_bit,dma_chan[,bits_msb]

Response:

    OK

Parameters:

- **\<clock>** : Clock speed, divisors of 80MHz, in Hz, max is 40M
- **\<mode>** : SPI mode [0-3]
- **\<cmd_bit>** : Default amount of bits in command phase [0-16]
- **\<addr_bit>** : Default amount of bits in address phase [0-64]
- **\<dma_chan>** : Either channel 1 or 2, or 0 in the case when no DMA is required
- **\<bits_msb>** : SPI data format
    Bit0: 
        0: Transmit MSB first(default)
        1: Transmit LSB first
    Bit1: 
        0: Receive data MSB first(default)
        1: Receive data LSB first

**Note:**
- Need to config SPI GPIO before SPI initialization  

**Example:**

    AT+DRVSPIINIT=102400,0,0,0,0,3   // SPI clock:100K, mode:0, command and address bit amount all is 0, not use DNA, LSB first both transmit and receive 
    OK
    AT+DRVSPIINIT=0   // Delete SPI Driver
    OK 

<a name="cmd-DRVSPIRD"></a>

### [AT+DRVSPIRD](#Driver-AT) - Read SPI data

Set Command:  

     AT+DRVSPIRD=data_len[,cmd,cmd_len] [,addr,addr_len]

Response:

    +DRVSPIRD: <read data>
    OK

Parameters:

- **\<data_len>** : Read SPI data length [1 ~ 4092], in bytes
- **\<cmd>** : Command data, of which the length is set in the cmd_len
- **\<cmd_len>** : command length in this transaction, in bytes [0 ~ 2]
- **\<addr>** : command address, of which the length is set in the addr_len
- **\<addr_len>** :  The address length in this transaction, in bytes [0~ 4]

**Note:**

- if you don't use DMA, the max data_len you can set is 64 bytes at each time.

**Example:**

    AT+DRVSPIRD=2  // Read 2bytes data
    +DRVI2CREAD:ffff
    OK
    
    AT+DRVSPIRD=2,0x03,1,0x001000,3 // Read 2bytes data,cmd is 0x03,cmd_len is 1byte,addr is 0x1000,addr_len is 3bytes
    +DRVI2CREAD:ffff
    OK

<a name="cmd-DRVSPIWR"></a>

### [AT+DRVSPIWR](#Driver-AT) -  Write SPI data

Set Command:  

     AT+DRVSPIWR=data_len[,cmd,cmd_len] [,addr,addr_len]

Response:

- data_len > 0

    OK
    >
Begin receiving serial data. When the requirement of data length is met, the transmission of data starts.
When the data is transmitted successfully, the system returns: 

    OK 

- data_len = 0

 data_len = 0 means just want to transmite command and address, and no SPI data , so the system just returns:

    OK 

Parameters:

- **\<data_len>** : Read SPI data length [0 ~ 4092]
- **\<cmd>** :  Command data, of which the length is set in the cmd_len
- **\<cmd_len>** : The command length in this transaction, in bytes [0 ~ 2]
- **\<addr>** : command address, of which the length is set in the addr_len
- **\<addr_len>** :  The address length in this transaction, in bytes [0~ 4]

**Note:**

- if you don't use DMA, the max data_len you can set is 64 bytes at each time.

**Example:**

    AT+DRVSPIWR=2  // Read 4bytes data
    OK
    >                //Begin receiving serial data
    OK
    
    AT+DRVSPIWR=0,0x03,1,0x001000,3 // WRITE 0bytes data,cmd is 0x03,cmd_len is 1byte,addr is 0x1000,addr_len is 3bytes
    OK


