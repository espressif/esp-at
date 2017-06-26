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
    * optional. if not specified, it will generate ATBleService.bin in the same folder with SOURCE_FILE_NAME

for example:
```commandline
cd esp-at/tools
python BLEService.py GattServiceExample.csv
python BLEService.py -t GattServiceExample.bin GattServiceExample.csv
```

1. go to tools folder
2. will generate `ATBleService.bin` from file `GattServiceExample.csv`
3. will generate `GattServiceExample.bin` from file `GattServiceExample.csv`


