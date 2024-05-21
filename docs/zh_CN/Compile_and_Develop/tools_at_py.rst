at.py 工具
=================

{IDF_TARGET_VER: default="undefined", esp32="5.0", esp32c2="5.0", esp32c3="5.0", esp32c6="5.1", esp32s2="5.0"}

:link_to_translation:`en:[English]`

``at.py`` 工具用于修改 ESP-AT 官方发布版固件、GitHub 临时固件和 2MB/4MB 固件中的多种参数配置。这些配置包括 Wi-Fi 配置、证书和密钥配置、UART 配置、GATTS 配置等。当默认的固件无法满足您的需求时，您可以使用 ``at.py`` 工具修改固件中的这些参数配置。

.. _esp-at-py-steps:

详细步骤
-------------

请根据下方详细步骤，完成 ``at.py`` 工具下载、固件中的配置修改以及固件烧录。**推荐您优先选择 Linux 系统开发**。

* :ref:`esp-at-py-prerequisites`
* :ref:`esp-at-py-download`
* :ref:`esp-at-py-usage`
* :ref:`esp-at-py-modify-bin`
* :ref:`esp-at-firmware-download`

.. _esp-at-py-prerequisites:

第一步：Python 安装
----------------------------

如果您本地已经安装过 Python，则可以跳过此步骤。否则，请根据 `Python 官方文档 <https://www.python.org/downloads/>`_，完成 Python 的下载和安装。**推荐您使用 Python 3.8.0 或更高版本**。

安装完成后，您可以在命令行中输入 ``python --version``，查看 Python 版本。

.. _esp-at-py-download:

第二步：at.py 下载
----------------------------

访问 :project_file:`at.py <tools/at.py>` 页面，点击 ``Download raw file`` 按钮，将 ``at.py`` 文件下载到本地。

.. figure:: ../../_static/compile_and_develop/at-py-download.png
  :align: center
  :alt: 下载 at.py 文件
  :figclass: align-center

  下载 at.py 文件

.. _esp-at-py-usage:

第三步：at.py 用法说明
----------------------------

当前 ``at.py`` 支持修改固件中的参数配置，请在命令行中输入 ``python at.py modify_bin --help``，查看支持的用法以及说明。

.. _esp-at-py-modify-bin:

第四步：at.py 修改固件中的配置示例
---------------------------------------

.. list::

  * :ref:`at-py-modify-wifi`
  * :ref:`at-py-modify-pki`
  * :ref:`at-py-modify-uart`
  :not esp32s2: * :ref:`at-py-modify-gatts`

.. note::

  - 您可以混合修改多种参数配置，例如，您可以同时修改 Wi-Fi 配置和证书和密钥配置。
  - 待修改的参数配置，脚本并不会检查其合法性，请确保您输入的参数配置是合法的。
  - 修改后的参数配置，在当前 mfg_nvs 目录下的 ``mfg_nvs.csv`` 中，会有对应的记录。

.. _at-py-modify-wifi:

修改 Wi-Fi 配置
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

当前可修改的 Wi-Fi 参数配置如下表所示：

.. list-table::
  :header-rows: 1
  :widths: 50 60 100

  * - 参数
    - 功能
    - 说明
  * - \--tx_power
    - Wi-Fi 的最大发射功率
    - 详情见 `{IDF_TARGET_NAME} 发射功率 <https://docs.espressif.com/projects/esp-idf/zh_CN/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-reference/network/esp_wifi.html#_CPPv425esp_wifi_set_max_tx_power6int8_t>`_
  * - \--country_code
    - Wi-Fi 国家代码
    - 详情见 `Wi-Fi 国家/地区代码 <https://docs.espressif.com/projects/esp-idf/zh_CN/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#id46>`_ 中的 cc 字段
  * - \--start_channel
    - Wi-Fi 起始信道
    - 详情见 `Wi-Fi 国家/地区代码 <https://docs.espressif.com/projects/esp-idf/zh_CN/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#id46>`_ 中的 schan 字段
  * - \--channel_number
    - Wi-Fi 的总信道数
    - 详情见 `Wi-Fi 国家/地区代码 <https://docs.espressif.com/projects/esp-idf/zh_CN/release-v{IDF_TARGET_VER}/{IDF_TARGET_PATH_NAME}/api-guides/wifi.html#id46>`_ 中的 nchan 字段

例如，您可以使用以下命令，修改 Wi-Fi 的最大发射功率为 18 dBm，国家代码为 US，起始信道为 1，总信道数为 11：

.. code-block:: none

  python at.py modify_bin --tx_power 72 --country_code "US" --start_channel 1 --channel_number 11 --input factory_XXX.bin

- **\--tx_power 72**：单位是 0.25 dBm，72 表示 18 dBm
- **\--input factory_XXX.bin**：输入的固件文件

.. _at-py-modify-pki:

修改证书和密钥配置
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

当前可修改的证书和密钥配置如下表所示：

.. list-table::
  :header-rows: 1
  :widths: 50 60 70

  * - 参数
    - 功能
    - 原始文件
  * - \--server_ca
    - TLS 服务器的 CA 证书
    - :component_file:`server_ca.crt <customized_partitions/raw_data/server_ca/server_ca.crt>`
  * - \--server_cert
    - TLS 服务器的证书
    - :component_file:`server_cert.crt <customized_partitions/raw_data/server_cert/server_cert.crt>`
  * - \--server_key
    - TLS 服务器的密钥
    - :component_file:`server.key <customized_partitions/raw_data/server_key/server.key>`
  * - \--client_ca0
    - 第 0 套客户端的 CA 证书
    - :component_file:`client_ca_00.crt <customized_partitions/raw_data/client_ca/client_ca_00.crt>`
  * - \--client_cert0
    - 第 0 套客户端的证书
    - :component_file:`client_cert_00.crt <customized_partitions/raw_data/client_cert/client_cert_00.crt>`
  * - \--client_key0
    - 第 0 套客户端的密钥
    - :component_file:`client_key_00.key <customized_partitions/raw_data/client_key/client_key_00.key>`
  * - \--client_ca1
    - 第 1 套客户端的 CA 证书
    - :component_file:`client_ca_01.crt <customized_partitions/raw_data/client_ca/client_ca_01.crt>`
  * - \--client_cert1
    - 第 1 套客户端的证书
    - :component_file:`client_cert_01.crt <customized_partitions/raw_data/client_cert/client_cert_01.crt>`
  * - \--client_key1
    - 第 1 套客户端的密钥
    - :component_file:`client_key_01.key <customized_partitions/raw_data/client_key/client_key_01.key>`
  * - \--mqtt_ca
    - MQTT 客户端的 CA 证书
    - :component_file:`mqtt_ca.crt <customized_partitions/raw_data/mqtt_ca/mqtt_ca.crt>`
  * - \--mqtt_cert
    - MQTT 客户端的证书
    - :component_file:`mqtt_client.crt <customized_partitions/raw_data/mqtt_cert/mqtt_client.crt>`
  * - \--mqtt_key
    - MQTT 客户端的密钥
    - :component_file:`mqtt_client.key <customized_partitions/raw_data/mqtt_key/mqtt_client.key>`
  * - \--wpa2_ca
    - WPA2-Enterprise 客户端的 CA 证书
    - :component_file:`wpa2_ca.pem <customized_partitions/raw_data/wpa2_ca/wpa2_ca.pem>`
  * - \--wpa2_cert
    - WPA2-Enterprise 客户端的证书
    - :component_file:`wpa2_client.crt <customized_partitions/raw_data/wpa2_cert/wpa2_client.crt>`
  * - \--wpa2_key
    - WPA2-Enterprise 客户端的密钥
    - :component_file:`wpa2_client.key <customized_partitions/raw_data/wpa2_key/wpa2_client.key>`

例如，您可以使用以下命令，修改 MQTT 客户端的 CA 证书、客户端的证书和密钥。

.. code-block:: none

  python at.py modify_bin --mqtt_ca mqtt/mqtt_ca.crt --mqtt_cert mqtt/mqtt.crt --mqtt_key mqtt/mqtt.key --input factory_XXX.bin

- **\--input factory_XXX.bin**：输入的固件文件

.. _at-py-modify-uart:

修改 UART 配置
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

可修改的 UART 配置，仅包括 :term:`AT 命令端口` 的 UART 配置。可修改的参数配置如下表所示：

.. list-table::
  :header-rows: 1
  :widths: 40 60 100

  * - 参数
    - 功能
    - 说明
  * - \--uart_num
    - AT 命令口的 UART 号
    - 仅在 AT 日志口同时用作 AT 命令口时，需要修改此参数。同时保证下面的 ``tx_pin`` 和 ``rx_pin`` 和 :term:`AT 日志端口` 的管脚一样。
  * - \--baud
    - AT 命令口的波特率
    - 原始值：115200
  * - \--tx_pin
    - AT 命令口的 TX 管脚
    - 请保证待修改的管脚不会和其他管脚冲突
  * - \--rx_pin
    - AT 命令口的 RX 管脚
    - 请保证待修改的管脚不会和其他管脚冲突
  * - \--cts_pin
    - AT 命令口的 CTS 管脚
    - 请保证待修改的管脚不会和其他管脚冲突。不用流控时，修改此参数为 -1。
  * - \--rts_pin
    - AT 命令口的 RTS 管脚
    - 请保证待修改的管脚不会和其他管脚冲突。不用流控时，修改此参数为 -1。

例如，您可以使用以下命令，修改 AT 命令口的波特率为 921600，TX 管脚为 17，RX 管脚为 16，禁用流控。

.. code-block:: none

  python at.py modify_bin --baud 921600 --tx_pin 17 --rx_pin 16 --cts_pin -1 --rts_pin -1 --input factory_XXX.bin

- **\--input factory_XXX.bin**：输入的固件文件

.. only:: not esp32s2

  .. _at-py-modify-gatts:

  修改 GATTS 配置
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  修改前，请先阅读 :doc:`如何自定义低功耗蓝牙服务 <How_to_customize_BLE_services>` 文档，了解 GATTS 的配置文件 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 中的各个字段的含义。

  当前可修改的 GATTS 配置如下表所示：

  .. list-table::
    :header-rows: 1
    :widths: 20 60

    * - 参数
      - 功能
    * - \--gatts_cfg0
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 0 的一行数据
    * - \--gatts_cfg1
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 1 的一行数据
    * - \--gatts_cfg2
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 2 的一行数据
    * - \--gatts_cfg3
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 3 的一行数据
    * - \--gatts_cfg4
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 4 的一行数据
    * - \--gatts_cfg5
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 5 的一行数据
    * - \--gatts_cfg6
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 6 的一行数据
    * - \--gatts_cfg7
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 7 的一行数据
    * - \--gatts_cfg8
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 8 的一行数据
    * - \--gatts_cfg9
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 9 的一行数据
    * - \--gatts_cfg10
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 10 的一行数据
    * - \--gatts_cfg11
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 11 的一行数据
    * - \--gatts_cfg12
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 12 的一行数据
    * - \--gatts_cfg13
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 13 的一行数据
    * - \--gatts_cfg14
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 14 的一行数据
    * - \--gatts_cfg15
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 15 的一行数据
    * - \--gatts_cfg16
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 16 的一行数据
    * - \--gatts_cfg17
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 17 的一行数据
    * - \--gatts_cfg18
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 18 的一行数据
    * - \--gatts_cfg19
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 19 的一行数据
    * - \--gatts_cfg20
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 20 的一行数据
    * - \--gatts_cfg21
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 21 的一行数据
    * - \--gatts_cfg22
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 22 的一行数据
    * - \--gatts_cfg23
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 23 的一行数据
    * - \--gatts_cfg24
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 24 的一行数据
    * - \--gatts_cfg25
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 25 的一行数据
    * - \--gatts_cfg26
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 26 的一行数据
    * - \--gatts_cfg27
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 27 的一行数据
    * - \--gatts_cfg28
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 28 的一行数据
    * - \--gatts_cfg29
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 29 的一行数据
    * - \--gatts_cfg30
      - 更新 :component_file:`gatts_data.csv <customized_partitions/raw_data/ble_data/gatts_data.csv>` 文件中 index 为 30 的一行数据

  例如，您可以使用以下命令，修改 index 为 0 行的 perm 权限。

  .. code-block:: none

    python at.py modify_bin --gatts_cfg0 "0,16,0x2800,0x011,2,2,A002" --input factory_XXX.bin

  - **\--input factory_XXX.bin**：输入的固件文件

  .. // 以下的小节是一个临时的解决方案，而且无法避免。请参考 https://docs.espressif.com/projects/esp-docs/en/latest/writing-documentation/writing-for-multiple-targets.html#target-specific-paragraph 获取更多信息。

  .. _esp-at-firmware-download:

  第五步：固件烧录
  ----------------------------

  .. attention::
    **修改后的 AT 固件，需要您根据自己的产品自行测试验证功能。**

    **请保存好修改前和修改后的固件以及下载链接**，用于后续可能的问题调试。

  请根据 :ref:`固件烧录指南 <flash-at-firmware-into-your-device>`，完成固件烧录。

.. only:: esp32s2

  .. _esp-at-firmware-download:

  第五步：固件烧录
  ----------------------------

  .. attention::
    **修改后的 AT 固件，需要您根据自己的产品自行测试验证功能。**

    **请保存好修改前和修改后的固件以及下载链接**，用于后续可能的问题调试。

  请根据 :ref:`固件烧录指南 <flash-at-firmware-into-your-device>`，完成固件烧录。
