如何自定义 Ble services
===========================

## BLE services 的文件位置

BLE Service 的源文件路径是： `esp-at/components/customized_partitions/raw_data/ble_data/example.csv`. 如果需要自定义服务，那么需要这么做：

  - 修改 `BLE Service` 文件
  - 使用 esp-at/tools/BLEService.py 重新生成 ble_data.bin
  - 将 ble_data.bin 下载到 ESP32，具体下载的地址在 `module_config/module_esp32_default/partitions_at.csv`（ESP32模块）或者`module_config/module_esp32c3_default/partitions_at.csv`（ESP32-C3模块） 分区表里面有定义。

## 如何修改 BLE services 文件

BLE services 的定义类似于一个多元数组，每一行是一个 GATT 结构体，每个服务都是由 service 定义、 characteristic 定义以及一些可选的 description 组成。

用户可以定义多个服务，比如 Service A、 Service B 和 Service C， 这三个服务就需要依次排序，因为每个服务的定义都是类似的，我们只拿其中一个举例说明。

首先，需要说明的是，所有服务定义的第一行都是固定的，第一行用来定义了服务的 UUID，标志着一个服务定义的开始。例如示例中，0x2800 表示这一行定义了一个 Primary Service, 具体的这个服务的 UUID 在 value 字段(用户也可定义为 SIG 颁布的 UUID，例如 0x180A, 也可以自定义，例如示例中是一个自定义的服务，UUID为 0xA002)。

* 例如:  

|index|uuid_len|uuid|perm|val_max_len|val_cur_len|value|  
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|0|16|0x2800|0x01|2|2|A002|  

从第二行开始就是这个服务所包含的 characteristics 的定义, 每个 characteristic 至少有两行组成，第一行是 characteristic 的申明，它的 UUID 是固定的 0x2803，value 字段表示的是这一行可读可写的属性，这里直接参照示例，不要修改，全部是可读可写。第二行是 characteristic 的本身，UUID 可以用户自己定义，这里 value 字段就是 characteristic 本身的数值。

* 例如:  

|index|uuid_len|uuid|perm|val_max_len|val_cur_len|value|  
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|1|16|0x2803|0x01|1|1|02| 
|2|16|0xC300|0x01|1|1|30| 

某些 characteristic 后面还会跟着 description。比如，如果这个 characteristic 是可 notify 的，后面就必须跟着 UUID 为 0x2902 的 description。

* 例如:  

|index|uuid_len|uuid|perm|val_max_len|val_cur_len|value|  
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|3|16|0x2803|0x01|1|1|02| 
|4|16|0xC306|0x01|1|1|30|
|5|16|0x2902|0x011|2|2|0000|  

定义完所有的 characteristics，就是一个服务的结束，如果还想定义其他的 services ，同样的方法依次排列即可