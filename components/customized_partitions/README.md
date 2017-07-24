# AT Customized partition table

Some AT command, like BLE commands, SSL commands, requires to define its own partition in partition table.
AT command is configurable, that means the partition table could change if AT command config changed.
We want to keep the main partition table consistent, as it need to be compatible with new reversion.
Therefore we defined a customized partition in the main partition table, and defines another partition table for customized partition.
We add a component `customized_partitions` and it will process the customized partition table, call tools to generate defined bins.

### component `customized_partitions`

in order to integrate customized partitions to the build systems, we created `customized_partitions`. It has the following features:

1. check if the required partitions are defined in customized partition table
2. generate customized partition bin, an generate customized data bins with tools
3. add all partition to flash args so we can use `make flash` in AT

##### already defined partitions
1. ble_data
2. server_cert
3. server_key
4. server_ca
5. fatfs

##### check if required partition is defined
We need to check if the partition required by enabled AT command is defined in partition table. Therefore we created a file `AT\_partition\_dependency.txt` to config the dependency. Each line of the file defines the AT menuconfig option and the required partition name.

For example, the following contents in AT\_partition\_dependency defines the dependency of current customized partitions.

```
AT_BLE_COMMAND_SUPPORT ble_data
AT_FS_COMMAND_SUPPORT fatfs
AT_NET_COMMAND_SUPPORT server_cert
AT_NET_COMMAND_SUPPORT server_key
AT_NET_COMMAND_SUPPORT server_ca
```

##### generation of customized bins
To provide a generic way of generation customized partitions bins from raw content, the make flow will try to find the generation tool and raw content in pre-defined folder.

```
customized_partitions --- generation_tool --- ble_data.sh
                      |                   |-- xxx.sh
                      |                   |-- server_cert.sh
                      |
                      |-- raw_data --- ble_data --- xxx
                      |            |-- server_cert --- xxx
```

In building customized partition bin stage:

1. parsing all customized partitions from partition table
2. try to find generation tool from `generation_tool` folder
    * if tool is not found, then skip generating this partition bin
3. run generation tool, generate customized bin
    * the generation tool can specify its raw data folder. by default the raw data will be put into `raw_data/partition_name` of component customzied partitions
    * if generation tools did not exit 0, it will report error and stop build
4. copy bin to `build/customized_partitions` folder and update to flash args

##### raw data for customized partition bin

1. ble_data: put one `csv/xls/xlsx` file to `esp-at/components/customized_partitions/raw_data/ble_data/`
2. server_ca: put one `pem/der/cer/crt` file to `esp-at/components/customized_partitions/raw_data/server_ca/`
3. server_cert: put one `pem/der/cer/crt` file to `esp-at/components/customized_partitions/raw_data/server_cert/`
4. server_key: put one `pem/der/key` file to `esp-at/components/customized_partitions/raw_data/server_key/`

For BLE service data definition, please refer to README.md in `esp-at/tools` folder.
