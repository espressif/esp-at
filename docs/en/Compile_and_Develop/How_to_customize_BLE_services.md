How To Customize ble services
=============================

## Where is the `BLE Services` source file

The path of BLE service source file is `esp-at/components/customized_partitions/raw_data/ble_data/example.csv`. If user wants to customize the BLE services, You need to:

  - Modify the `BLE Service` file
  - use esp-at/tools/BLEService.py to generate ble_data.bin
  - Download generated ble_data.bin to address defined in `module_config/module_esp32_default/partitions_at.csv`(ESP32 modules).

## Description of the structure of the BLE service

The BLE Services are defined as a multivariate array of GATT structures, each element of the array always consist of a service, declarations, characteristics and optional descriptors.

User can define more than one services. For example, if you want to define three services(`Server_A`, `Server_B` and `Server_C`), then these three services need to be arranged in order. Since the definition of each service is similar, here we define one service as example, and then you can define others one by one accordingly.

Each service always consist of a service definition and several characteristics. Each characteristic may be followed by some descriptions.

the service definition must be in the first line, it always be a primary service (UUID 0x2800) which determines with its value which service is described(for example, a predefined one such as 0x180A or a self generated one). 

* For example, the following line defines a primary service with UUID 0xA002.

|index|uuid_len|uuid|perm|val_max_len|val_cur_len|value|  
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|0|16|0x2800|0x01|2|2|A002|  


Definition of characteristics starts from the second line. It contains at least two lines, one is the characteristic declaration, another is to define the characteristic. UUID 0x2803 means the characteristic declaration, value of this line sets its permission, for example, 02 means both readable and writable, user can keep this configuration. Then the next line defines this characteristic, UUID of this line will be the characteristic's UUID, you can define it as you need, value will be the characteristic's value.

* For example, the following lines define a readable and writable characteristic with UUID 0xC300, whose value is 0x30.

|index|uuid_len|uuid|perm|val_max_len|val_cur_len|value|  
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|1|16|0x2803|0x01|1|1|02| 
|2|16|0xC300|0x01|1|1|30| 

The attribute can be described further by descriptors. A special one is the descriptor "Client Characteristic Configuration" (UUID 0x2902) which should be present if the Notify bit has been activated in the Characteristic Declaration (UUID 0x2803). This descriptor should always be writable and readable.

* For example, the following lines define a readable and writable characteristic with UUID 0xC306, and able to notify. 

|index|uuid_len|uuid|perm|val_max_len|val_cur_len|value|  
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|3|16|0x2803|0x01|1|1|02| 
|4|16|0xC306|0x01|1|1|30|
|5|16|0x2902|0x011|2|2|0000|  
