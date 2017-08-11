# Tools

## 1. GATT Service Bin

If you want to use GATT server, you need to define the GATT service.
AT provides a method to allow user to define the GATT service:

1. define the service in xls or csv file
2. use BLEService.py to generate
3. download generated bin to address defined in partition table


#### 1. define service in xls or csv file

define the following attributes for one service:
1. index:
    * GATT database index.
    * format: incremental number start from 0
2. uuid_len:
    * length of UUID.
    * format: 16 or 32 or 128
3. uuid:
    * UUID value
    * format: hex. for example `0x2800` or `2800`.
4. perm:
    * permission (refer to BLE Spec for definition)
    * format: int. for example `0x11` or `17` or `0b10001`.
5. val\_max\_len:
    * max allow value length (the max length when you dynamic set value)
    * format: int.
6. val\_cur\_len:
    * initial value length
    * format: int.
7. value:
    * initial value
    * format: hex string, for example, you need to set "3031323334" ascii value "01234"

###### Example:

You can refer to `GattServiceExample.csv` in the same folder for example.
It creates service with UUID 0xA002. It defines characteristic with UUID 0xC300, 0xC301 and each characteristic has one descriptor with UUID 0x2901.

```csv
index,uuid_len,uuid,perm,val_max_len,val_cur_len,value
# defines service A002
0,16,0x2800,0x01,2,2,A002
# declares a char with read properity
1,16,0x2803,0x01,1,1,2
# defines char C300 with read & write permission
2,16,0xC300,0x11,1,1,1
# defines descriptor 2901 for C300 with read & write permission
3,16,0x2901,0x11,1,1,1
# declares another char with read properity
4,16,0x2803,0x01,1,1,2
5,16,0xC301,0x01,512,512,...
6,16,0x2901,0x11,512,512,...
```

#### 2. use BLEService.py to generate

###### 2.1 Setup Python environment:
1. install python 2.7
2. [install pip](https://pip.pypa.io/en/latest/installing/)
3. install the following python packages with pip:
`pip install pyyaml xlrd`

###### 2.2 Generate bin with BLEService

```commandline
python SCRIPT_PATH -t TARGET_FILE_NAME SOURCE_FILE_NAME
```

* `SCRIPT_PATH`:
    * the path of script. If you're in Tools folder of AT, then SCRIPT_PATH=BLEService.py
* `SOURCE_FILE_NAME`:
    * the source file which you defines your GATT service (abs or relative path of the source file)
* `TARGET_FILE_NAME`:
    * the target file which you want to save the generated bin (abs or relative path of the target file)
    * optional. if not specified, it will generate ATBleService.bin in the same folder with SOURCE\_FILE_NAME

for example:
```commandline
cd esp-at/tools
python BLEService.py GattServiceExample.csv
python BLEService.py -t GattServiceExample.bin GattServiceExample.csv
```

1. go to tools folder
2. will generate `ATBleService.bin` from file `GattServiceExample.csv`
3. will generate `GattServiceExample.bin` from file `GattServiceExample.csv`


## 2. PKI Bin

AT supports or will support some features like TLS connection, WPA2-Enterprise, which will use PKI items.
PKI Bin tool allows users to generate Bin for AT and use the PKI items for those features.

#### 1. Bin file format:
```
    File Format ( unsigned int is little endian )

    | <- 2 bytes -> | <- 2 bytes -> | <- 4 bytes -> | <- total length -> |
    +---------------+---------------+---------------+--------------------+
    |   magic code  |  list size    |     length    |    list items      |
    +---------------+---------------+---------------+--------------------+

        magic code: 0xF1F1
        list size: count of total items
        length: total length of list items
        list items: a list of PKIItems
    
    PKI item:

    | <- 1 byte -> | <- 1 byte -> | <- 2 bytes -> | <- content len -> | <- (4 - len%4) % 4 bytes -> |
    +--------------+--------------+---------------+-------------------+-----------------------------+
    |     type     |     ID       | content len   |    raw data       |  padding for 4 bytes align  |
    +--------------+--------------+---------------+-------------------+-----------------------------+

        type: item type
            0x01: CA
            0x02: certificate
            0x03: key
        ID: used to match cert and key.
        content len: raw data length
```

#### 2. generate PKI bin

```commandline
python SCRIPT_PATH generate_bin -b OUTPUT_BIN_NAME PKI_LIST
```

* `SCRIPT_PATH`:
    * the path of script. If you're in Tools folder of AT, then SCRIPT_PATH=AtPKI.py
* `OUTPUT_BIN_NAME`:
    * the file name for generated PKI bin
* `PKI_LIST`:
    * a list of (type, file_name) pair:
        * type: ca, cert, key
        * file_name: PKI item file name
    * each item will have ID according to its type. the ID is generated as input order. ID will be used to match certificates and keys.

###### Example:

```commandline
python AtPKI.py generate_bin -b at-pki.bin cert Cert1.pem key PrivateKey1.pem ca CA1.pem ca CA2.pem cert Cert2.pem key PrivateKey2.pem ca CA3.pem
```

It will generate at-pki.bin at tools folder. at-pki.bin contains the following PKI items:

1. CA:
    * CA1 with ID 0
    * CA2 with ID 1
    * CA3 with ID 2
2. Certificate:
    * Cert1 with ID 0
    * Cert1 with ID 1
3. Key
    * PrivateKey1 with ID 0
    * PrivateKey2 with ID 1

<b><font color=#DC143C> But for the AT SSL function, the cert, private_key and CA are defined in different addresses in "esp32-at/at_customize.csv", so that we need to generate the bin files seperately. 

* generate at_cert.bin by  ```
python AtPKI.py generate_bin -b at_cert.bin cert Cert.pem```  
* generate at_key.bin by  ```
python AtPKI.py generate_bin -b at_key.bin key PrivateKey.pem```  
* generate at_ca.bin by ```
python AtPKI.py generate_bin -b at_ca.bin ca CA.pem```  
then   
* download the at_cert.bin into address 0x24000.  
* download the at_key.bin into address 0x26000.
* download the at_ca.bin into address 0x28000.</font></b>

### <font color=#DC143C>Notice:   
We provided a group of test bin files for testing in the folder "tools/SSL_test_bin".   
But customers should use their own Cert.pem, PrivateKey.pem and CA.pem to generate them.</font>

#### 3. parse PKI bin

The tool also provide parsing generated PKI bin feature. It will generate PKI items to the specified path.

```commandline
python SCRIPT_PATH parse_bin -b INPUT_BIN_NAME -o OUTPUT_PATH
```

* `SCRIPT_PATH`:
    * the path of script. If you're in Tools folder of AT, then SCRIPT_PATH=AtPKI.py
* `INPUT_BIN_NAME`:
    * the PKI bin file to be parsed
* `OUTPUT_PATH`:
    * the output path of parsed PKI items

###### Example:

```commandline
python AtPKI.py generate_bin -b at-pki.bin -o parsed_items
```

`at-pki.bin` is the bin generated by the example in Step2.

```
Parse Output:
tools --- parsed_items --- ca --- ca_0.cer
      |                |      |-- ca_1.cer
      |                |      |-- ca_2.cer
      |                |
      |                |-- cert --- cert_0.cer
      |                |        |-- cert_1.cer
      |                |
      |                |-- key --- key_0.key
      |                |       |-- key_1.key
```

