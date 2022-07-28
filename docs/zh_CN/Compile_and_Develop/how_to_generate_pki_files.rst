如何生成 :term:`PKI` 文件
=============================================

:link_to_translation:`en:[English]`

ESP-AT 提供 python 脚本 ``AtPKI.py`` 将 SSL 服务器证书文件，SSL 客户端证书文件，MQTT 证书文件 和 WPA2 证书文件（包括 ``CA`` 证书， ``cert`` 证书和私钥）转换为二进制文件。

.. contents::
   :local:
   :depth: 1

证书二进制文件格式
-------------------------------------

除了将证书文件转换为二进制文件外，脚本 ``AtPKI.py`` 还会向二进制文件中添加一些额外信息。

转换单个证书文件时，将会在文件头中以小端模式添加 12 字节数据，并在文件末尾进行 4 字节对齐。

   .. list-table::
      :header-rows: 1
      :widths: 40 80

      * - 字段
        - 描述
      * - magic code（2 字节）
        - 0xF1 0xF1
      * - 列表大小（2 字节）
        - 证书文件的数量
      * - 长度（4 字节）
        - 剩余文件长度 = 总的文件大小 - magic code 大小 - 列表大小 - 长度大小
      * - 类型（1 字节）
        - | ca: 0x01
          | certificate: 0x02
          | key: 0x03
      * - ID（1 字节）
        - 用于匹配证书文件
      * - 内容长度（2 字节）
        - 转换为二进制文件的证书文件的大小

以 `生成证书二进制文件`_ 小节中生成的 ``client_cert.bin`` 文件为例，二进制文件开头的小端模式 12 字节数据如下所示：

   .. list-table::
      :header-rows: 1
      :widths: 40 80

      * - 字段
        - 描述
      * - magic code（2 字节）
        - 0xF1 0xF1
      * - 列表大小（2 字节）
        - 0x02 0x00
      * - 长度（4 字节）
        - 0x20 0x09 0x00 0x00
      * - 类型（1 字节）
        - 0x02
      * - ID（1 字节）
        - 0x00
      * - 内容长度（2 字节）
        - 0x8C 0x04

转换多个证书文件时，脚本 ``AtPKI.py`` 还会在除第一个证书文件外的每个证书文件的开头插入 4 字节数据。文件开头的小端模式 4 字节数据如下：

   .. list-table::
      :header-rows: 1
      :widths: 40 80

      * - 字段
        - 描述
      * - 类型（1 字节）
        - | ca: 0x01
          | certificate: 0x02
          | key: 0x03
      * - ID（1 字节）
        - 用于匹配证书文件
      * - 内容长度（2 字节）
        - 转换为二进制文件的证书文件的大小

以 `生成证书二进制文件`_ 小节中生成的 ``client_cert.bin`` 文件为例，文件开头的小端模式 4 字节数据如下所示：

   .. list-table::
      :header-rows: 1
      :widths: 40 80

      * - 字段
        - 描述
      * - 类型（1 字节）
        - certificate: 0x02
      * - ID（1 字节）
        - 0x01
      * - 内容长度（2 字节）
        - 0x8C 0x04

生成证书二进制文件
-------------------------------------

请选择以下方式之一生成证书二进制文件。

.. contents::
   :local:
   :depth: 1

脚本生成
^^^^^^^^^^^^^^^^^^^^

脚本 ``AtPKI.py`` 的路径是 :project_file:`tools/AtPKI.py`。您可以通过 ``-h`` 选项获取脚本的帮助信息。您也可以直接通过以下命令生成二进制文件。

.. code-block:: none

    python <SCRIPT_PATH> generate_bin [-b OUTPUT_BIN_NAME] <PKI_LIST> <source_file>

- ``SCRIPT_PATH``：脚本路径。如果您正处于 “tools” 目录下，则 ``SCRIPT_PATH`` 为 ``AtPKI.py``。
- ``OUTPUT_BIN_NAME``：要保存生成的目标文件名（目标文件名的绝对地址或者相对地址）；如果 ``-b OUTPUT_BIN_NAME`` 被省略，则脚本将会在当前目录下生成 ``PKI.bin`` 文件。
- ``PKI_LIST``：必须等于 ``ca``、``cert``、``key`` 中的一个。
- ``source_file``：您想转换的证书源文件（证书源文件的绝对地址或者相对地址）。

以 ESP-AT 的 SSL 客户端证书文件为例，您可以执行下面的命令在 tools 目录下生成 ``client_cert.bin`` 二进制文件。

.. code-block:: none

    python AtPKI.py generate_bin -b ./client_cert.bin  cert ../components/customized_partitions/raw_data/client_cert/client_cert_00.crt cert ../components/customized_partitions/raw_data/client_cert/client_cert_01.crt

编译期间生成
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ESP-AT 中证书文件的存储路径为 :project:`components/customized_partitions/raw_data`。

以 ESP-AT 的 SSL 客户端证书文件为例。如果您想生成自己的 SSL 客户端证书二进制文件，您必须将目录 ``client_ca`` 下的 ``CA`` 证书替换为自己的 ``CA`` 证书，将目录 ``client_cert`` 目录下的 ``cert`` 证书替换为自己的 ``cert`` 证书，将 ``client_ key`` 目录下的私钥替换为自己的私钥。

如果您有多套证书文件，请按照您的证书链依次放置于对应的目录下。建议您可以将文件名以数字结尾以确保证书文件的解析顺序。

替换完成之后，您可以参考 :doc:`How_to_clone_project_and_compile_it` 来编译 ESP-AT 工程。

下载或者更新证书二进制文件
--------------------------------------------

脚本 ``AtPKI.py`` 仅仅负责将证书文件转换为二进制文件。您可以通过以下方式将二进制文件烧录到 flash 中：

通过烧录工具烧录
^^^^^^^^^^^^^^^^

- Windows

  请下载 Windows `Flash 下载工具 <https://www.espressif.com/zh-hans/support/download/other-tools>`_。

  请参考 zip 文件夹中 ``readme.pdf`` 或者 ``doc`` 目录获取更多有关该工具的信息。

- Linux or macOS

  请使用 `esptool.py <https://github.com/espressif/esptool>`_。

  您可以在 ESP-AT 根目录下执行下面的命令烧录二进制文件。

  .. code-block:: none

    esptool.py --chip auto --port PORTNAME --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB ADDRESS FILEDIRECTORY

  将 ``PORTNAME`` 替换为您的串口。将 ``ADDRESS`` 替换为烧录地址。将 ``FILEDIRECTORY`` 替换为二进制文件所在的目录。

通过命令更新
^^^^^^^^^^^^^^^^

- :ref:`AT+SYSFLASH <cmd-SYSFLASH>` 命令

  以 ``{IDF_TARGET_NAME}`` 模组为例，您可以执行以下命令来更新 ``client_cert`` 分区。请参考 :ref:`AT+SYSFLASH <cmd-SYSFLASH>` 获取更多信息。

  1. 查询 flash 用户分区

    命令：

    .. code-block:: none

      AT+SYSFLASH?

    响应：

    .. only:: esp32

      .. code-block:: none

        +SYSFLASH:"ble_data",64,1,0x21000,0x3000
        +SYSFLASH:"server_cert",64,2,0x24000,0x2000
        +SYSFLASH:"server_key",64,3,0x26000,0x2000
        +SYSFLASH:"server_ca",64,4,0x28000,0x2000
        +SYSFLASH:"client_cert",64,5,0x2a000,0x2000
        +SYSFLASH:"client_key",64,6,0x2c000,0x2000
        +SYSFLASH:"client_ca",64,7,0x2e000,0x2000
        +SYSFLASH:"factory_param",64,8,0x30000,0x1000
        +SYSFLASH:"wpa2_cert",64,9,0x31000,0x2000
        +SYSFLASH:"wpa2_key",64,10,0x33000,0x2000
        +SYSFLASH:"wpa2_ca",64,11,0x35000,0x2000
        +SYSFLASH:"mqtt_cert",64,12,0x37000,0x2000
        +SYSFLASH:"mqtt_key",64,13,0x39000,0x2000
        +SYSFLASH:"mqtt_ca",64,14,0x3b000,0x2000
        +SYSFLASH:"fatfs",1,129,0x70000,0x90000

        OK

    .. only:: esp32c2

      .. code-block:: none

          +SYSFLASH:"server_cert",64,1,0x1f000,0x2000
          +SYSFLASH:"server_key",64,2,0x21000,0x2000
          +SYSFLASH:"server_ca",64,3,0x23000,0x2000
          +SYSFLASH:"client_cert",64,4,0x25000,0x2000
          +SYSFLASH:"client_key",64,5,0x27000,0x2000
          +SYSFLASH:"client_ca",64,6,0x29000,0x2000
          +SYSFLASH:"factory_param",64,7,0x2b000,0x1000
          +SYSFLASH:"wpa2_cert",64,8,0x2c000,0x2000
          +SYSFLASH:"wpa2_key",64,9,0x2e000,0x2000
          +SYSFLASH:"wpa2_ca",64,10,0x30000,0x2000
          +SYSFLASH:"mqtt_cert",64,11,0x32000,0x2000
          +SYSFLASH:"mqtt_key",64,12,0x34000,0x2000
          +SYSFLASH:"mqtt_ca",64,13,0x36000,0x2000
          +SYSFLASH:"ble_data",64,14,0x38000,0x6000
          +SYSFLASH:"fatfs",1,129,0x47000,0x19000

          OK

    .. only:: esp32c3

      .. code-block:: none

          +SYSFLASH:"ble_data",64,1,0x1f000,0x6000
          +SYSFLASH:"server_cert",64,2,0x25000,0x2000
          +SYSFLASH:"server_key",64,3,0x27000,0x2000
          +SYSFLASH:"server_ca",64,4,0x29000,0x2000
          +SYSFLASH:"client_cert",64,5,0x2b000,0x2000
          +SYSFLASH:"client_key",64,6,0x2d000,0x2000
          +SYSFLASH:"client_ca",64,7,0x2f000,0x2000
          +SYSFLASH:"factory_param",64,8,0x31000,0x1000
          +SYSFLASH:"wpa2_cert",64,9,0x32000,0x2000
          +SYSFLASH:"wpa2_key",64,10,0x34000,0x2000
          +SYSFLASH:"wpa2_ca",64,11,0x36000,0x2000
          +SYSFLASH:"mqtt_cert",64,12,0x38000,0x2000
          +SYSFLASH:"mqtt_key",64,13,0x3a000,0x2000
          +SYSFLASH:"mqtt_ca",64,14,0x3c000,0x2000
          +SYSFLASH:"fatfs",1,129,0x47000,0x19000

          OK

  2. 擦除 ``client_cert`` 分区

    命令：

    .. code-block:: none

      AT+SYSFLASH=0,"client_cert"

    响应：

    .. code-block:: none

      OK

  3. 更新 ``client_cert`` 分区

    命令：

    .. code-block:: none

      AT+SYSFLASH=1,"client_cert",0,2344

    响应：

    .. code-block:: none

      >

    当 ``<operator>`` 为 ``write`` 时，系统收到此命令后先换行返回 ``>``，此时您可以输入要写的数据，数据长度应与 ``<length>`` 一致。当写入操作完成之后，系统会提示以下信息。

    .. code-block:: none

      OK

- :ref:`AT+CIUPDATE <cmd-UPDATE>` 命令

  例如，您可以执行以下命令来更新 ``client_ca`` 分区。请参考 :ref:`AT+CIUPDATE <cmd-UPDATE>` 获取更多信息。

  .. Important::
    如果您想通过这种方式更新 ``client_ca`` 分区，您必须实现自己的 OTA 设备，请参考文档 :doc:`How_to_implement_OTA_update`。

  .. code-block:: none

    AT+CIUPDATE=1,"v2.2.0.0","client_ca"

.. note::

  您必须确保烧录的地址是正确的，否则 ESP-AT 固件可能不能工作。查看烧录地址的最简单方法是执行命令 **AT+SYSFLASH?**。
