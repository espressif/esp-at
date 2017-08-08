## The Secondary Partitions Table

The primary partition table is for system usage, it will generate a "partitions_at.bin" according to the "partitions_at.csv" in compilation. And if the primary partition table goes wrong, the system will fail to startup. So generally, we should not change the "partitions_at.csv".

In this case, we provide a secondary partition table for custom usage, "at_customize.csv". We have already defined some user partitions in it. Custom can add new partitions in the "at_customize.csv", and generate a new "at_customize.bin" according to it. The partition table can be updated by flashing the new "at_customize.bin" into flash, or be revised by command "AT+SYSFLASH".

## How to use the secondary partition table
Enter the project esp32-at, run command `python esp-idf/components/partition_table/gen_esp32part.py -q at_customize.csv at_customize.bin` to generate a "at_customize.bin".
Download the "at_customize.bin" into flash, the default address is 0x20000.

## Notes of revising at_customize.csv
Users should not change the "name" and "type" of the user partitions which we defined in the "at_customize.csv". But "offset" and "size" can be changed, if necessary.  
If you need to add a new user partition, please check if it has already been defined in the ESP-IDF (esp_partition.h) first.  
If it is defined in the ESP-IDF, you should keep the "type" value to be the same when adding it into the at_customize.csv.  
If it is not defined in the ESP-IDF, please set the "type" to be "0x40" when adding it into the at_customize.csv.  
A user partition's name should NOT be longer than 16 bytes.  
The default size of the entire "at_customize" partition is 0xE0000, which is defined in the first partition. Please do NOT over the range when adding new user partitions.  


## When a "at_customize.bin" is needed
To use below AT commands, the "at_customize.bin" should be downloaded into flash.  
AT+SYSFLASH — Set User Partitions in Flash  
AT+FS — Filesystem Operations   
SSL server relevant commands  
BLE server relevant commands  
