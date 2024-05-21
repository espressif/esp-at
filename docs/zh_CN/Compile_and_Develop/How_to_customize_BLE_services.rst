如何自定义低功耗蓝牙服务
========================================

:link_to_translation:`en:[English]`

本文档介绍了如何利用 ESP-AT 提供的低功耗蓝牙服务源文件在 {IDF_TARGET_NAME} 设备上自定义低功耗蓝牙服务。

.. contents::
   :local:
   :depth: 2

低功耗蓝牙服务被定义为 GATT 结构的多元数组，该数组至少包含一个属性类型 (attribute type) 为 0x2800 的首要服务 (primary service)。每个服务总是由一个服务定义和几个特征组成。每个特征总是由一个值和可选的描述符组成。更多相关信息请参阅 `《蓝牙核心规范》 <https://www.bluetooth.com/specifications/specs/core-specification-4-2>`_ 中的 Generic Attribute Profile (GATT) 一节。

.. _factory-gatts-intro:

低功耗蓝牙服务源文件介绍
---------------------------------

低功耗蓝牙服务源文件是 ESP-AT 工程创建低功耗蓝牙服务所依据的文件，文件位于 :component_file:`customized_partitions/raw_data/ble_data/gatts_data.csv`，内容如下表所示。

.. list-table::
   :header-rows: 1

   * - index
     - uuid_len
     - uuid
     - perm
     - val_max_len
     - val_cur_len
     - value
   * - 0
     - 16
     - 0x2800
     - 0x01
     - 2
     - 2
     - A002
   * - 1
     - 16
     - 0x2803
     - 0x01
     - 1
     - 1
     - 2
   * - 2
     - 16
     - 0xC300
     - 0x01
     - 1
     - 1
     - 30
   * - 3
     - 16
     - 0x2901
     - 0x11
     - 1
     - 1
     - 30
   * - ...
     - ...
     - ...
     - ...
     - ...
     - ...
     - ...

以下内容是对上表的说明。

- ``perm`` 字段描述权限，它在 ESP-AT 工程中的定义如下所示。
  
  .. code-block:: c

    /* relate to BTA_GATT_PERM_xxx in bta/bta_gatt_api.h */
    /**
    * @brief Attribute permissions
    */
    #define    ESP_GATT_PERM_READ                  (1 << 0)   /* bit 0 -  0x0001 */    /* relate to BTA_GATT_PERM_READ in bta/bta_gatt_api.h */
    #define    ESP_GATT_PERM_READ_ENCRYPTED        (1 << 1)   /* bit 1 -  0x0002 */    /* relate to BTA_GATT_PERM_READ_ENCRYPTED in bta/bta_gatt_api.h */
    #define    ESP_GATT_PERM_READ_ENC_MITM         (1 << 2)   /* bit 2 -  0x0004 */    /* relate to BTA_GATT_PERM_READ_ENC_MITM in bta/bta_gatt_api.h */
    #define    ESP_GATT_PERM_WRITE                 (1 << 4)   /* bit 4 -  0x0010 */    /* relate to BTA_GATT_PERM_WRITE in bta/bta_gatt_api.h */
    #define    ESP_GATT_PERM_WRITE_ENCRYPTED       (1 << 5)   /* bit 5 -  0x0020 */    /* relate to BTA_GATT_PERM_WRITE_ENCRYPTED in bta/bta_gatt_api.h */
    #define    ESP_GATT_PERM_WRITE_ENC_MITM        (1 << 6)   /* bit 6 -  0x0040 */    /* relate to BTA_GATT_PERM_WRITE_ENC_MITM in bta/bta_gatt_api.h */
    #define    ESP_GATT_PERM_WRITE_SIGNED          (1 << 7)   /* bit 7 -  0x0080 */    /* relate to BTA_GATT_PERM_WRITE_SIGNED in bta/bta_gatt_api.h */
    #define    ESP_GATT_PERM_WRITE_SIGNED_MITM     (1 << 8)   /* bit 8 -  0x0100 */    /* relate to BTA_GATT_PERM_WRITE_SIGNED_MITM in bta/bta_gatt_api.h */
    #define    ESP_GATT_PERM_READ_AUTHORIZATION    (1 << 9)   /* bit 9 -  0x0200 */
    #define    ESP_GATT_PERM_WRITE_AUTHORIZATION   (1 << 10)  /* bit 10 - 0x0400 */

- 上表第一行是 UUID 为 ``0xA002`` 的服务定义。
- 第二行是特征的声明。UUID ``0x2803`` 表示特征声明，数值 (value) ``2`` 设置权限，权限长度为 8 位，每一位代表一个操作的权限，``1`` 表示支持该操作，``0`` 表示不支持。

  .. list-table::
     :header-rows: 1
     :widths: 20 100

     * - 位
       - 权限
     * - 0
       - BROADCAST
     * - 1
       - READ
     * - 2
       - WRITE WITHOUT RESPONSE
     * - 3
       - WRITE
     * - 4
       - NOTIFY
     * - 5
       - INDICATE
     * - 6
       - AUTHENTICATION SIGNED WRITES
     * - 7
       - EXTENDED PROPERTIES
- 第三行定义了服务的特征。该行的 UUID 是特征的 UUID，数值是特征的数值。
- 第四行定义了特征的描述符（可选）。

有关 UUID 的更多信息请参考 `蓝牙技术联盟分配符 <https://www.bluetooth.com/specifications/assigned-numbers/>`_。

如果直接在 {IDF_TARGET_NAME} 设备上使用默认源文件，不做任何修改，并建立低功耗蓝牙连接，那么在客户端查询服务器服务后，会得到如下结果。

.. figure:: ../../_static/compile_and_develop/ble_default_service.png
    :scale: 100 %
    :align: center
    :alt: ESP-AT 默认低功耗蓝牙服务

编译时自定义低功耗蓝牙服务
-------------------------------

请根据以下步骤自定义低功耗蓝牙服务。

.. contents::
   :local:
   :depth: 1

修改低功耗蓝牙服务源文件
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

可定义多个服务，例如，若要定义三个服务（``Server_A``、``Server_B`` 和 ``Server_C``），则需要将这三个服务按顺序排列。由于定义每个服务的操作大同小异，这里我们以定义一个服务为例，其他服务您可以按照此例进行定义。

1. 添加服务定义。

   本例定义了一个值为 0xFF01 的主要服务。

   .. list-table::
      :header-rows: 1
   
      * - index
        - uuid_len
        - uuid
        - perm
        - val_max_len
        - val_cur_len
        - value
      * - 31
        - 16
        - 0x2800
        - 0x01
        - 2
        - 2
        - FF01

2. 添加特征说明和特征值。

   本例定义了一个 UUID 为 0xC300 的可读可写特征，并将其值设置为 0x30。
   
   .. list-table::
      :header-rows: 1
   
      * - index
        - uuid_len
        - uuid
        - perm
        - val_max_len
        - val_cur_len
        - value
      * - 32
        - 16
        - 0x2803
        - 0x11
        - 1
        - 1
        - 0A
      * - 33
        - 16
        - 0xC300
        - 0x11
        - 1
        - 1
        - 30

3. 添加特征描述符（可选）。

   本例添加了客户端特征配置，数字 0x0000 表示通知 (notification) 和指示 (indication) 被禁用。

   .. list-table::
      :header-rows: 1
   
      * - index
        - uuid_len
        - uuid
        - perm
        - val_max_len
        - val_cur_len
        - value
      * - 34
        - 16
        - 0x2902
        - 0x11
        - 2
        - 2
        - 0000

完成以上步骤后，自定义的低功耗蓝牙服务定义如下。

.. list-table::
   :header-rows: 1

   * - index
     - uuid_len
     - uuid
     - perm
     - val_max_len
     - val_cur_len
     - value
   * - 31
     - 16
     - 0x2800
     - 0x01
     - 2
     - 2
     - FF01
   * - 32
     - 16
     - 0x2803
     - 0x11
     - 1
     - 1
     - 0A
   * - 33
     - 16
     - 0xC300
     - 0x11
     - 1
     - 1
     - 30
   * - 34
     - 16
     - 0x2902
     - 0x11
     - 2
     - 2
     - 0000

请根据自己的需求修改 GATTS 配置，然后生成 ``mfg_nvs.bin`` 文件。

生成 mfg_nvs.bin 文件
^^^^^^^^^^^^^^^^^^^^^^^^^^

请参考 :ref:`mfg-nvs-generate` 文档生成带有低功耗蓝牙的服务配置的 ``mfg_nvs.bin``。

下载 mfg_nvs.bin 文件
^^^^^^^^^^^^^^^^^^^^^^^^^^

请参考 :ref:`mfg-nvs-download` 文档。

下载完成后，重新建立低功耗蓝牙连接，在客户端查询的服务器服务如下所示。

.. figure:: ../../_static/compile_and_develop/ble_customize_service.png
    :scale: 100 %
    :align: center
    :alt: ESP-AT 自定义低功耗蓝牙服务
