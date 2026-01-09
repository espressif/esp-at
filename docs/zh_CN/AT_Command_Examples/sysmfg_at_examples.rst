AT+SYSMFG 命令示例
========================================================

:link_to_translation:`en:[English]`

本文档提供使用 :ref:`AT+SYSMFG <cmd-SYSMFG>` 命令操作 :term:`manufacturing nvs` 用户分区的详细示例。

.. list::

   - :ref:`介绍 <sysmfg-intro>`

     - :ref:`概述 <sysmfg-overview>`
     - :ref:`命名空间结构 <sysmfg-namespace-structure>`
     - :ref:`数据类型 <sysmfg-data-types>`
     - :ref:`其他重要说明 <sysmfg-other-notes>`

   - :ref:`出厂参数配置 <sysmfg-factory-param>`

     - :ref:`模组名称操作 <sysmfg-module-name>`
     - :ref:`UART 配置操作 <sysmfg-uart-config>`
     - :ref:`最大发射功率操作 <sysmfg-max-tx-power>`
     - :ref:`国家代码操作 <sysmfg-country-code>`
     - :ref:`系统存储模式操作 <sysmfg-sys-store>`
     - :ref:`添加自定义键值对 <sysmfg-custom-key-value>`

   - :ref:`PKI 配置 <sysmfg-pki>`

     - :ref:`SSL 客户端证书 <sysmfg-ssl-client>`
     - :ref:`SSL 服务器证书 <sysmfg-ssl-server>`

   :not esp32s2: - :ref:`GATTS 配置 <sysmfg-gatts>`
   - :ref:`自定义数据存储 <sysmfg-custom-data>`

.. _sysmfg-intro:

介绍
----

.. _sysmfg-overview:

概述
******

manufacturing NVS 分区包含多个命名空间，每个命名空间包含若干键值对，用于存储不同类型的配置信息。常用的命名空间包括：

.. only:: esp32 or esp32c3

  .. list-table::
     :header-rows: 1
     :widths: 20 45 25

     * - 命名空间
       - 描述
       - 相关 AT 命令
     * - ``factory_param``
       - 出厂参数（模组名称、Wi-Fi 配置、UART 配置等）
       - 参见 :ref:`factory-param-intro`
     * - ``client_cert``
       - SSL 客户端证书
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_key``
       - SSL 客户端私钥
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_ca``
       - SSL 客户端 CA 证书
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``server_cert``
       - SSL 服务器证书
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_key``
       - SSL 服务器私钥
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_ca``
       - SSL 服务器 CA 证书
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``mqtt_cert``
       - MQTT 客户端证书
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_key``
       - MQTT 客户端私钥
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_ca``
       - MQTT 客户端 CA 证书
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``https_cert``
       - HTTPS 客户端证书
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_key``
       - HTTPS 客户端私钥
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_ca``
       - HTTPS 客户端 CA 证书
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``wpa2_cert``
       - WPA2-Enterprise 客户端证书
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_key``
       - WPA2-Enterprise 客户端私钥
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_ca``
       - WPA2-Enterprise 客户端 CA 证书
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wss_cert``
       - WebSocket 客户端证书
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_key``
       - WebSocket 客户端私钥
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_ca``
       - WebSocket 客户端 CA 证书
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``ble_data``
       - 低功耗蓝牙服务配置
       - :ref:`AT+BLEGATTSSRVCRE <cmd-GSSRVCRE>`

.. only:: esp32c2 or esp32c5 or esp32c6 or esp32c61

  .. list-table::
     :header-rows: 1
     :widths: 20 45 25
  
     * - 命名空间
       - 描述
       - 相关 AT 命令
     * - ``factory_param``
       - 出厂参数（模组名称、Wi-Fi 配置、UART 配置等）
       - 参见 :ref:`factory-param-intro`
     * - ``client_cert``
       - SSL 客户端证书
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_key``
       - SSL 客户端私钥
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_ca``
       - SSL 客户端 CA 证书
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``server_cert``
       - SSL 服务器证书
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_key``
       - SSL 服务器私钥
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_ca``
       - SSL 服务器 CA 证书
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``mqtt_cert``
       - MQTT 客户端证书
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_key``
       - MQTT 客户端私钥
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_ca``
       - MQTT 客户端 CA 证书
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``https_cert``
       - HTTPS 客户端证书
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_key``
       - HTTPS 客户端私钥
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_ca``
       - HTTPS 客户端 CA 证书
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``wpa2_cert``
       - WPA2-Enterprise 客户端证书
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_key``
       - WPA2-Enterprise 客户端私钥
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_ca``
       - WPA2-Enterprise 客户端 CA 证书
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wss_cert``
       - WebSocket 客户端证书
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_key``
       - WebSocket 客户端私钥
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_ca``
       - WebSocket 客户端 CA 证书
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``ble_data``
       - 低功耗蓝牙服务配置
       - :ref:`AT+BLEINIT <cmd-BINIT>`

.. only:: esp32s2

  .. list-table::
     :header-rows: 1
     :widths: 20 45 25
  
     * - 命名空间
       - 描述
       - 相关 AT 命令
     * - ``factory_param``
       - 出厂参数（模组名称、Wi-Fi 配置、UART 配置等）
       - 参见 :ref:`factory-param-intro`
     * - ``client_cert``
       - SSL 客户端证书
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_key``
       - SSL 客户端私钥
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``client_ca``
       - SSL 客户端 CA 证书
       -
         * :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>`
         * :ref:`AT+CIPSTART <cmd-START>`
     * - ``server_cert``
       - SSL 服务器证书
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_key``
       - SSL 服务器私钥
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``server_ca``
       - SSL 服务器 CA 证书
       - :ref:`AT+CIPSERVER <cmd-SERVER>`
     * - ``mqtt_cert``
       - MQTT 客户端证书
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_key``
       - MQTT 客户端私钥
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``mqtt_ca``
       - MQTT 客户端 CA 证书
       - :ref:`AT+MQTTUSERCFG <cmd-MQTTUSERCFG>`
     * - ``https_cert``
       - HTTPS 客户端证书
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_key``
       - HTTPS 客户端私钥
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``https_ca``
       - HTTPS 客户端 CA 证书
       - :ref:`AT+HTTPCFG <cmd-HTTPCFG>`
     * - ``wpa2_cert``
       - WPA2-Enterprise 客户端证书
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_key``
       - WPA2-Enterprise 客户端私钥
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wpa2_ca``
       - WPA2-Enterprise 客户端 CA 证书
       - :ref:`AT+CWJEAP <cmd-JEAP>`
     * - ``wss_cert``
       - WebSocket 客户端证书
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_key``
       - WebSocket 客户端私钥
       - :ref:`AT+WSCFG <cmd-WSCFG>`
     * - ``wss_ca``
       - WebSocket 客户端 CA 证书
       - :ref:`AT+WSCFG <cmd-WSCFG>`

查询当前固件实际支持的所有命名空间：

  **命令：**

  .. code-block:: none

    AT+SYSMFG?

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param"
    +SYSMFG:"client_cert"
    +SYSMFG:"client_key"
    +SYSMFG:"client_ca"
    +SYSMFG:"server_cert"
    +SYSMFG:"server_key"
    +SYSMFG:"server_ca"
    ...
    +SYSMFG:"mqtt_cert"
    +SYSMFG:"mqtt_key"
    +SYSMFG:"mqtt_ca"

    OK

.. note::

  - 不同芯片和固件版本支持的命名空间可能不同。
  - 本文档中所有示例的命令响应内容仅供参考，实际输出可能因芯片型号、固件版本和配置而异。

.. _sysmfg-namespace-structure:

命名空间结构
**************

每个命名空间是键值对的集合。不同命名空间的键命名约定不同：

- 证书相关命名空间（``client_cert``、``client_key``、``client_ca``、``server_cert``、``server_key``、``server_ca``）：使用 ``<base_name>.<index>`` 格式（例如 ``client_cert.0``、``client_cert.1``）来存储多套证书。
- ``factory_param`` 命名空间：使用描述性键名（例如 ``module_name``、``uart_port``、``country_code``）。
- ``ble_data`` 命名空间：使用 ``cfg`` 加索引的格式（例如 ``cfg0``、``cfg1``、``cfg2``）。

.. _sysmfg-data-types:

数据类型
***********

``AT+SYSMFG`` 命令中的 ``<type>`` 参数表示数据类型：

- 1：u8（8 位无符号整数，范围：0-255，例如：UART 端口号、信道号）
- 2：i8（8 位有符号整数，范围：-128 至 127，用于有符号配置值）
- 3：u16（16 位无符号整数，范围：0-65535，例如：GPIO 引脚编号）
- 4：i16（16 位有符号整数，范围：-32768 至 32767）
- 5：u32（32 位无符号整数，范围：0-4294967295）
- 6：i32（32 位有符号整数，范围：-2147483648 至 2147483647，例如：UART 波特率 115200）
- 7：string（文本字符串，例如：模组名称 "MINI-1"、国家代码 "CN"、低功耗蓝牙服务配置）
- 8：binary（二进制数据，例如：证书、私钥）

.. _sysmfg-other-notes:

其他重要说明
****************

- **长度参数说明**：响应中的 ``<length>`` 参数含义因数据类型而异：

  - 对于数值类型（类型 1-6）：``<length>`` 表示该数值的字符串表示的字符数。例如，i32 类型的波特率值 115200 显示为 6 个字符，因此 length 为 6。
  - 对于字符串和二进制类型（类型 7-8）：``<length>`` 表示实际的字节数。

- **二进制数据传输**：写入二进制数据（类型 8）时，命令返回 ``>`` 并等待您发送 ``<value>`` 参数中指定的确切字节数。

- **数据持久化**：写入 manufacturing NVS 的数据在设备重启后仍然保留，直到被明确擦除。

- **偏移读取**：您可以使用偏移和长度参数从键中读取部分数据，这对处理大数据块很有用。

- **编译时与运行时**：虽然某些数据（如低功耗蓝牙服务）可以在编译时通过修改源 CSV 文件并重新生成 ``mfg_nvs.bin`` 进行自定义，但 ``AT+SYSMFG`` 命令允许您在运行时更新某些值，无需重新编译。

- **参考资料**：有关非易失性存储 (NVS) 概念和其他 NVS 操作的更多信息，请参考 `ESP-IDF NVS 文档 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-reference/storage/nvs_flash.html>`_。

.. _sysmfg-factory-param:

出厂参数配置
----------------

factory_param 命名空间存储设备的基本出厂参数，如 Wi-Fi 配置、UART 配置和模组信息。ESP-AT 在初始化过程中会读取这些参数，它们影响设备的默认运行行为。详情请参考 :doc:`出厂参数配置介绍 <../Compile_and_Develop/How_to_update_factory_parameters>`。

.. important::

  修改出厂参数后，执行 :ref:`AT+RST <cmd-RST>` 或 :ref:`AT+RESTORE <cmd-RESTORE>` 命令以重启设备，使更改生效。

查询所有出厂参数
**********************

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","version",2
    +SYSMFG:"factory_param","module_name",7
    +SYSMFG:"factory_param","max_tx_power",2
    +SYSMFG:"factory_param","country_code",7
    +SYSMFG:"factory_param","start_channel",1
    +SYSMFG:"factory_param","channel_num",1
    +SYSMFG:"factory_param","uart_port",2
    +SYSMFG:"factory_param","uart_baudrate",6
    +SYSMFG:"factory_param","uart_tx_pin",6
    +SYSMFG:"factory_param","uart_rx_pin",6
    +SYSMFG:"factory_param","uart_cts_pin",6
    +SYSMFG:"factory_param","uart_rts_pin",6
    +SYSMFG:"factory_param","sys_store",1

    OK

.. _sysmfg-module-name:

模组名称操作
**********************

1. 读取模组名称：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","module_name"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","module_name",7,6,MINI-1

    OK

2. 写入新的模组名称：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","module_name",7,<name_length>

  **响应：**

  .. code-block:: none

    OK

    >

  返回 ``>`` 后，发送模组名称字符串。例如，发送 "MY-MODULE-01"。

  **数据发送后的响应：**

  .. code-block:: none

    OK

3. 验证配置是否生效：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","module_name"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","module_name",7,12,MY-MODULE-01

    OK

  模组名称已成功更新为 "MY-MODULE-01"。

.. _sysmfg-uart-config:

UART 配置操作
**********************

1. 读取 UART 端口：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","uart_port"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","uart_port",2,1,1

    OK

2. 读取 UART 波特率：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","uart_baudrate"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","uart_baudrate",6,6,115200

    OK

3. 更新 UART TX 引脚：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","uart_tx_pin",6,17

  **响应：**

  .. code-block:: none

    OK

  .. note::

    重启后，需要切换硬件连接到新引脚或通过新的 UART 配置发送 AT 命令来验证修改是否生效。

.. _sysmfg-max-tx-power:

最大发射功率操作
**********************

1. 读取最大发射功率：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","max_tx_power"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","max_tx_power",2,2,78

    OK

2. 更新最大发射功率：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","max_tx_power",2,60

  **响应：**

  .. code-block:: none

    OK

  关于可以设置的 Wi-Fi 最大发射功率值，请参考 :ref:`AT+RFPOWER <cmd-RFPOWER>` 命令中 ``<wifi_power>`` 参数的取值范围。

3. 重启设备使配置生效：

  **命令：**

  .. code-block:: none

    AT+RST

  **响应：**

  .. code-block:: none

    ready

4. 验证配置是否生效：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","max_tx_power"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","max_tx_power",2,2,60

    OK

  最大发射功率已成功更新。

5. 使用 AT+RFPOWER 命令进一步验证：

  **命令：**

  .. code-block:: none

    AT+RFPOWER?

  **响应：**

  .. code-block:: none

    +RFPOWER:60

    OK

  最大发射功率现在为 60 (60 × 0.25 dBm = 15 dBm)。

.. _sysmfg-country-code:

国家代码操作
**********************

1. 读取国家代码：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","country_code"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","country_code",7,2,CN

    OK

2. 更新国家代码：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","country_code",7,2

  **响应：**

  .. code-block:: none

    OK

    >

  收到 ``>`` 后，发送国家代码（例如 "US"、"JP"、"EU"）。

  **数据传输后的响应：**

  .. code-block:: none

    OK

3. 重启设备使配置生效：

  **命令：**

  .. code-block:: none

    AT+RST

  **响应：**

  .. code-block:: none

    ready

4. 验证配置是否生效：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","country_code"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","country_code",7,2,US

    OK

  国家代码已成功更新为 "US"。

5. 更新信道数量为 11（美国标准）：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","channel_num",1,11

  **响应：**

  .. code-block:: none

    OK

6. 重启设备使配置生效：

  **命令：**

  .. code-block:: none

    AT+RST

  **响应：**

  .. code-block:: none

    ready

7. 使用 AT+CWCOUNTRY 命令验证国家码和信道配置：

  **命令：**

  .. code-block:: none

    AT+CWCOUNTRY?

  **响应：**

  .. code-block:: none

    +CWCOUNTRY:0,"US",1,11

    OK

  **响应说明：**

  - ``0``：国家码策略（0 表示使用默认国家码配置）
  - ``"US"``：国家代码
  - ``1``：起始信道 (start_channel)
  - ``11``：信道数量 (channel_num)

  配置已成功生效，设备现在使用美国的国家代码和信道配置。

.. _sysmfg-sys-store:

系统存储模式操作
**********************

``sys_store`` 参数控制系统是否将配置更改自动存储到 flash。此参数与 :ref:`AT+SYSSTORE <cmd-SYSSTORE>` 命令的功能相关。

1. 读取当前系统存储模式：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","sys_store"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","sys_store",1,1,1

    OK

  ``sys_store`` 的值：

  - ``0``：不自动存储配置到 flash（相当于 ``AT+SYSSTORE=0``）
  - ``1``：自动存储配置到 flash（相当于 ``AT+SYSSTORE=1``，默认值）

2. 更改系统存储模式为不自动存储：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","sys_store",1,0

  **响应：**

  .. code-block:: none

    OK

3. 重启设备使配置生效：

  **命令：**

  .. code-block:: none

    AT+RST

  **响应：**

  .. code-block:: none

    ready

4. 验证配置是否生效：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","sys_store"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","sys_store",1,1,0

    OK

5. 使用 AT+SYSSTORE 命令进一步验证：

  **命令：**

  .. code-block:: none

    AT+SYSSTORE?

  **响应：**

  .. code-block:: none

    +SYSSTORE:0

    OK

  系统存储模式已成功更改为不自动存储（``0`` 表示关闭自动存储）。

**更改后的影响：**

当 ``sys_store`` 设置为 ``0`` 后，配置更改（如 Wi-Fi 设置或其他支持自动存储的命令）不会自动保存到 flash，重启后会恢复为之前保存的配置。受影响的命令详见 :ref:`AT+SYSSTORE <cmd-SYSSTORE>`。

如果需要在 ``sys_store=0`` 模式下保存特定配置，可以使用 :ref:`AT+SYSSTORE <cmd-SYSSTORE>` 命令临时启用存储，执行配置命令后再关闭。

.. _sysmfg-custom-key-value:

添加自定义键值对
**********************

除了预定义的出厂参数外，您还可以在 ``factory_param`` 命名空间中添加自定义键值对来存储应用特定的配置数据。

本示例演示如何添加一个自定义键值对，例如存储设备序列号：

1. 添加自定义字符串类型键值对：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","device_sn",7,16

  **响应：**

  .. code-block:: none

    OK

    >

  返回 ``>`` 后，发送设备序列号（例如 "SN202601050001"）。

  **数据发送后的响应：**

  .. code-block:: none

    OK

2. 读取自定义键值对：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param","device_sn"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","device_sn",7,16,SN202601050001

    OK

3. 添加数值类型的自定义键值对：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"factory_param","hw_version",6,1001

  **响应：**

  .. code-block:: none

    OK

  这将存储硬件版本号 1001。

4. 查询所有键值对（包括自定义的）：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"factory_param"

  **响应：**

  .. code-block:: none

    +SYSMFG:"factory_param","version",2
    +SYSMFG:"factory_param","module_name",7
    ...
    +SYSMFG:"factory_param","device_sn",7
    +SYSMFG:"factory_param","hw_version",6

    OK

5. 删除自定义键值对：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=0,"factory_param","device_sn"

  **响应：**

  .. code-block:: none

    OK

.. note::

  - 自定义键名应避免与预定义的键名冲突（如 ``module_name``、``uart_port`` 等）
  - 建议使用有意义的键名，便于后续维护
  - 自定义键值对在重启后保持持久化
  - 过多的键值对可能影响性能，建议仅存储必要的配置信息
  - 修改 ``factory_param`` 中的值后，建议重启设备以确保应用程序正确读取新值

.. _sysmfg-pki:

PKI 配置
------------

PKI（公钥基础设施）配置用于创建、管理、分配、存储和撤销数字证书，以及管理公钥加密。ESP-AT 支持为多种功能配置证书，包括 SSL 客户端/服务器、MQTT 客户端、HTTPS 客户端等。所有证书的配置方法完全相同，均通过 :ref:`AT+SYSMFG <cmd-SYSMFG>` 命令操作对应的命名空间实现。更多详情请参考 :ref:`PKI 配置介绍 <factory-pki-intro>`。

**不同功能的命名空间映射：**

.. list-table::
   :header-rows: 1
   :widths: 30 35

   * - 功能
     - 证书/密钥命名空间
   * - SSL 客户端
     - ``client_cert``、``client_key``、``client_ca``
   * - SSL 服务器
     - ``server_cert``、``server_key``、``server_ca``
   * - MQTT 客户端
     - ``mqtt_cert``、``mqtt_key``、``mqtt_ca``
   * - HTTPS 客户端
     - ``https_cert``、``https_key``、``https_ca``
   * - WebSocket 客户端
     - ``wss_cert``、``wss_key``、``wss_ca``
   * - WPA2 Enterprise 客户端
     - ``wpa2_cert``、``wpa2_key``、``wpa2_ca``

.. note::
  您可以通过 ``AT+SYSMFG?`` 命令查询当前固件支持的所有命名空间，以确认您需要的证书命名空间是否可用。

本节以 SSL 证书为例说明证书配置的完整流程。其他功能的证书配置（如 MQTT、HTTPS）可参照相同方法，仅需根据上表修改对应的命名空间名称即可。

.. _sysmfg-ssl-client:

SSL 客户端证书
**********************

client_cert、client_key 和 client_ca 命名空间存储用于安全连接的 SSL 证书和密钥。这三个命名空间可以协同工作以实现 SSL 认证（单向或双向认证）。

查询证书信息
^^^^^^^^^^^^^

1. 读取客户端证书命名空间中的所有键值对：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"client_cert"

  **响应：**

  .. code-block:: none

    +SYSMFG:"client_cert","client_cert.0",8
    +SYSMFG:"client_cert","client_cert.1",8

    OK

  同样，您可以查询 client_key 和 client_ca 命名空间：

  .. code-block:: none

    AT+SYSMFG=1,"client_key"
    AT+SYSMFG=1,"client_ca"

2. 读取特定的证书或密钥：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"client_cert","client_cert.0"

  **响应：**

  .. code-block:: none

    +SYSMFG:"client_cert","client_cert.0",8,1164,<value>

    OK

配置证书
^^^^^^^^^

1. 写入客户端证书：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"client_cert","client_cert.0",8,1164

  **响应：**

  .. code-block:: none

    OK

    >

  返回 ``>`` 后，发送证书数据（1164 字节）。数据发送完成后，系统返回：

  .. code-block:: none

    OK

2. 写入客户端密钥：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"client_key","client_key.0",8,<key_length>

  **响应：**

  .. code-block:: none

    OK

    >

  返回 ``>`` 后，发送密钥数据。数据发送完成后，系统返回：

  .. code-block:: none

    OK

3. 写入 CA 证书：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"client_ca","client_ca.0",8,<ca_length>

  **响应：**

  .. code-block:: none

    OK

    >

  返回 ``>`` 后，发送 CA 证书数据。数据发送完成后，系统返回：

  .. code-block:: none

    OK

使用证书
^^^^^^^^^

将客户端证书、密钥（双向认证时还需客户端 CA 证书）写入 manufacturing NVS 分区后，您可以使用它们建立安全的 SSL 连接。以下是示例：

1. 确保证书已写入 manufacturing 分区：

   - 客户端证书存储在 ``client_cert`` 命名空间
   - 客户端密钥存储在 ``client_key`` 命名空间
   - 客户端 CA 证书存储在 ``client_ca`` 命名空间

2. 配置 SSL 连接使用 manufacturing 分区证书：

   **命令：**

   .. code-block:: none

     AT+CIPSSLCCONF=3,0,0

   **响应：**

   .. code-block:: none

     OK

   这将配置 SSL 客户端使用 manufacturing 分区中的证书（索引为 0）。

3. 连接到 Wi-Fi：

   **命令：**

   .. code-block:: none

     AT+CWJAP="your_ssid","your_password"

   **响应：**

   .. code-block:: none

     OK

4. 建立到服务器的 SSL 连接：

   **命令：**

   .. code-block:: none

     AT+CIPSTART="SSL","example.com",8883

   **响应：**

   .. code-block:: none

     CONNECT

     OK

   SSL 连接将使用 manufacturing 分区中的证书进行身份验证。

.. note::

  有关 SSL 配置的更多详细信息，请参阅 :ref:`AT+CIPSSLCCONF <cmd-SSLCCONF>` 命令文档。

擦除证书
^^^^^^^^^

擦除特定的证书：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=0,"client_cert","client_cert.0"

  **响应：**

  .. code-block:: none

    OK

擦除命名空间中的所有键值对：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=0,"client_cert"

  **响应：**

  .. code-block:: none

    OK

您可以使用相同的命令擦除 client_key 和 client_ca 数据。

.. _sysmfg-ssl-server:

SSL 服务器证书
**********************

server_cert、server_key 和 server_ca 命名空间存储设备作为 SSL 服务器所需的证书和密钥。这三个命名空间可以协同工作以启用安全的服务器端 SSL 连接（单向或双向认证）。

查询证书信息
^^^^^^^^^^^^^

1. 读取服务器证书命名空间中的所有键：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"server_cert"

  **响应：**

  .. code-block:: none

    +SYSMFG:"server_cert","server_cert.0",8
    +SYSMFG:"server_cert","server_cert.1",8

    OK

  同样，您可以查询 server_key 和 server_ca 命名空间：

  .. code-block:: none

    AT+SYSMFG=1,"server_key"
    AT+SYSMFG=1,"server_ca"

2. 读取特定的服务器证书或密钥：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"server_cert","server_cert.0"

  **响应：**

  .. code-block:: none

    +SYSMFG:"server_cert","server_cert.0",8,<length>,<value>

    OK

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"server_key","server_key.0"

  **响应：**

  .. code-block:: none

    +SYSMFG:"server_key","server_key.0",8,<length>,<value>

    OK

配置证书
^^^^^^^^^

1. 写入服务器证书：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"server_cert","server_cert.0",8,<cert_length>

  **响应：**

  .. code-block:: none

    OK

    >

  返回 ``>`` 后，发送证书数据。

  **数据传输后的响应：**

  .. code-block:: none

    OK

2. 写入服务器密钥：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"server_key","server_key.0",8,<key_length>

  **响应：**

  .. code-block:: none

    OK

    >

  返回 ``>`` 后，发送密钥数据。

  **数据传输后的响应：**

  .. code-block:: none

    OK

3. 写入 CA 证书：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"server_ca","server_ca.0",8,<ca_length>

  **响应：**

  .. code-block:: none

    OK

    >

  返回 ``>`` 后，发送 CA 证书数据。

  **数据传输后的响应：**

  .. code-block:: none

    OK

使用证书
^^^^^^^^^

将服务器证书、密钥（双向认证时还需服务器 CA 证书）写入 manufacturing NVS 分区后，您可以基于这些证书创建 SSL 服务器。以下是一个完整的示例：

1. 确保服务器凭证已写入 manufacturing 分区：

   - 服务器证书位于 ``server_cert`` 命名空间
   - 服务器密钥位于 ``server_key`` 命名空间
   - 服务器 CA 证书位于 ``server_ca`` 命名空间（可选，仅在需要客户端认证时使用）

2. 启用多连接模式：

   **命令：**

   .. code-block:: none

     AT+CIPMUX=1

   **响应：**

   .. code-block:: none

     OK

3. 启动 SSL 服务器：

   - 如果只需要服务器单向认证（服务器向客户端提供证书）：

     **命令：**

     .. code-block:: none

       AT+CIPSERVER=1,8883,"SSL"

     **响应：**

     .. code-block:: none

       OK

   - 如果需要双向认证（服务器和客户端相互验证证书）：

     **命令：**

     .. code-block:: none

       AT+CIPSERVER=1,8883,"SSL",1

     **响应：**

     .. code-block:: none

       OK

     其中最后一个参数 ``1`` 表示启用 CA 验证，服务器将使用 manufacturing 分区中的 CA 证书验证客户端证书。

   设备现在在端口 8883 上监听 SSL 连接，SSL 服务器将自动使用 manufacturing 分区中的服务器证书和密钥。

4. 当客户端连接时，SSL 握手将使用 manufacturing 分区中的服务器证书和密钥进行身份验证。

.. note::

  - SSL 服务器会自动从 manufacturing 分区（manufacturing partition）读取 ``server_cert`` 和 ``server_key`` 命名空间中的证书和密钥，无需额外配置。
  - 如果在 :ref:`AT+CIPSERVER <cmd-SERVER>` 命令中启用了 CA 验证（``<CA enable>`` = 1），服务器还会从 ``server_ca`` 命名空间读取 CA 证书来验证客户端证书，实现双向认证。
  - 有关 SSL 服务器配置的更多详细信息，请参阅 :ref:`AT+CIPSERVER <cmd-SERVER>` 命令文档。

擦除证书
^^^^^^^^^

擦除特定的服务器证书：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=0,"server_cert","server_cert.0"

  **响应：**

  .. code-block:: none

    OK

擦除命名空间中的所有键：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=0,"server_cert"

  **响应：**

  .. code-block:: none

    OK

您可以使用相同的命令擦除 server_key 和 server_ca 数据。

.. only:: not esp32s2

  .. _sysmfg-gatts:

  GATTS 配置
  -------------

  ble_data 命名空间存储低功耗蓝牙 GATTS（Generic Attribute Profile Server）服务配置。该命名空间包含以 ``cfg0``、``cfg1``、``cfg2`` 等形式组织的各个 GATT 服务配置项，其中每个配置项表示 GATT 表中的一个属性（服务、特征或描述符）。更多详情请参考 :doc:`低功耗蓝牙服务源文件介绍 <../Compile_and_Develop/How_to_customize_BLE_services>`。

  .. important::

    - 使用 ``AT+SYSMFG`` 命令修改 ``ble_data`` 命名空间时，需要在未初始化 BLE 的前提下执行。如果 BLE 已初始化，请先执行 ``AT+BLEINIT=0`` 关闭 BLE。
    - 在使用 ``AT+SYSMFG`` 读取或写入 ``ble_data`` 命名空间之前，请先阅读 :doc:`低功耗蓝牙服务源文件介绍 <../Compile_and_Develop/How_to_customize_BLE_services>` 文档，了解低功耗蓝牙服务配置的结构和格式。

  查询配置信息
  **********************

  读取配置项
  ^^^^^^^^^^^^^

  1. 读取 ble_data 命名空间中的所有配置项：

    **命令：**

    .. code-block:: none

      AT+SYSMFG=1,"ble_data"

    **响应：**

    .. code-block:: none

      +SYSMFG:"ble_data","cfg0",7
      +SYSMFG:"ble_data","cfg1",7
      +SYSMFG:"ble_data","cfg2",7
      +SYSMFG:"ble_data","cfg3",7
      ...
      +SYSMFG:"ble_data","cfg30",7

      OK

  2. 读取特定的 BLE 配置项（例如 ``cfg2`` 表示 GATT 服务表中的一行）：

    **命令：**

    .. code-block:: none

      AT+SYSMFG=1,"ble_data","cfg2"

    **响应：**

    .. code-block:: none

      +SYSMFG:"ble_data","cfg2",7,23,2,16,0xC300,0x01,1,1,30

      OK

    **响应格式：**

    响应数据表示 GATT 表中的以下字段：

    - ``2``: index（索引）
    - ``16``: uuid_len（UUID 长度，以位为单位）
    - ``0xC300``: uuid（UUID）
    - ``0x01``: perm（权限）
    - ``1``: val_max_len（最大值长度）
    - ``1``: val_cur_len（当前值长度）
    - ``30``: value（值）

  配置服务
  **************

  修改现有 BLE 特征的 UUID
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^

  此示例展示如何在运行时将特征的 UUID 从 ``0xC300`` 修改为 ``0xC500``。

  1. 查询当前配置：

    **命令：**

    .. code-block:: none

      AT+SYSMFG=1,"ble_data","cfg2"

    **响应：**

    .. code-block:: none

      +SYSMFG:"ble_data","cfg2",7,23,2,16,0xC300,0x01,1,1,30

      OK

    当前 UUID 为 ``0xC300``。

  2. 写入 UUID 为 ``0xC500`` 的新配置：

    **命令：**

    .. code-block:: none

      AT+SYSMFG=2,"ble_data","cfg2",7,23

    **响应：**

    .. code-block:: none

      OK
   
      >

    **发送数据：**

    .. code-block:: none

      2,16,0xC500,0x01,1,1,30

    **响应：**

    .. code-block:: none

      OK

  3. 查询配置项验证 UUID 是否修改成功：

    **命令：**

    .. code-block:: none

      AT+SYSMFG=1,"ble_data","cfg2"

    **响应：**

    .. code-block:: none

      +SYSMFG:"ble_data","cfg2",7,23,2,16,0xC500,0x01,1,1,30

      OK

    UUID 已成功更改为 ``0xC500``。

  4. 重新初始化低功耗蓝牙并创建服务以应用更改：

    **命令：**

    .. code-block:: none

      AT+BLEINIT=2

    **响应：**

    .. code-block:: none

      OK

    .. only:: esp32 or esp32c3

       **命令：**

       .. code-block:: none

         AT+BLEGATTSSRVCRE

       **响应：**

       .. code-block:: none

         OK

       **命令：**

       .. code-block:: none

         AT+BLEGATTSSRVSTART

       **响应：**

       .. code-block:: none

         OK

  5. 查询服务以验证：

    **命令：**

    .. code-block:: none

      AT+BLEGATTSSRV?

    **响应：**

    .. code-block:: none

      +BLEGATTSSRV:1,1,0xA002,1
      +BLEGATTSSRV:1,2,0xA003,1

      OK

  6. 查询特征以验证 UUID 更改：

    **命令：**

    .. code-block:: none

      AT+BLEGATTSCHAR?

    **响应：**

    .. code-block:: none

      +BLEGATTSCHAR:"char",1,1,0xC500
      +BLEGATTSCHAR:"char",1,2,0xC301
      +BLEGATTSCHAR:"char",1,3,0xC302
      +BLEGATTSCHAR:"char",1,4,0xC303
      +BLEGATTSCHAR:"char",1,5,0xC304
      +BLEGATTSCHAR:"char",1,6,0xC305
      +BLEGATTSCHAR:"char",1,7,0xC306
      +BLEGATTSCHAR:"char",1,8,0xC307
      +BLEGATTSCHAR:"char",2,1,0xC400
      +BLEGATTSCHAR:"char",2,2,0xC401

      OK

    第一个特征现在显示 UUID 为 ``0xC500`` 而不是 ``0xC300``。

  添加新的自定义 BLE 服务
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  此示例展示如何在运行时添加一系列新的 BLE 服务，而不修改现有的服务配置。假设我们要添加一个自定义服务（UUID: 0xA100），包含一个可读写的特征（UUID: 0xB100）。

  1. 查询当前最后一个配置项的索引：

    **命令：**

    .. code-block:: none

      AT+SYSMFG=1,"ble_data"

    **响应示例：**

    .. code-block:: none

      +SYSMFG:"ble_data","cfg0",7
      +SYSMFG:"ble_data","cfg1",7
      ...
      +SYSMFG:"ble_data","cfg30",7

      OK

    假设当前最后一个配置项是 ``cfg30``，我们将从 ``cfg31`` 开始添加新服务。

  2. 添加主服务声明（Primary Service Declaration）：

    **命令：**

    .. code-block:: none

      AT+SYSMFG=2,"ble_data","cfg31",7,26

    **响应：**

    .. code-block:: none

      OK

      >

    **发送数据：**

    .. code-block:: none

      31,16,0x2800,0x01,2,2,A100

    **数据格式说明：**

    - ``31``：配置项索引
    - ``16``：UUID 长度（16 位）
    - ``0x2800``：Primary Service Declaration UUID
    - ``0x01``：权限（可读）
    - ``2``：最大值长度（2 字节，用于存储服务 UUID）
    - ``2``：当前值长度
    - ``A100``：自定义服务的 UUID

    **响应：**

    .. code-block:: none

      OK

  3. 添加特征声明（Characteristic Declaration）：

    **命令：**

    .. code-block:: none

      AT+SYSMFG=2,"ble_data","cfg32",7,24

    **响应：**

    .. code-block:: none

      OK

      >

    **发送数据：**

    .. code-block:: none

      32,16,0x2803,0x01,3,1,02

    **数据格式说明：**

    - ``32``：配置项索引
    - ``16``：UUID 长度
    - ``0x2803``：特征声明
    - ``0x01``：权限（可读）
    - ``3``：最大值长度
    - ``1``：当前值长度
    - ``02``：设置权限为读

    **响应：**

    .. code-block:: none

      OK

  4. 添加特征值（Characteristic Value）：

    **命令：**

    .. code-block:: none

      AT+SYSMFG=2,"ble_data","cfg33",7,31

    **响应：**

    .. code-block:: none

      OK

      >

    **发送数据：**

    .. code-block:: none

      33,16,0xB100,0x11,20,8,12345678

    **数据格式说明：**

    - ``33``：配置项索引
    - ``16``：UUID 长度
    - ``0xB100``：特征值的 UUID（与特征声明中的 UUID 一致）
    - ``0x11``：权限（读+写：ESP_GATT_PERM_READ (0x01) + ESP_GATT_PERM_WRITE (0x10) = 0x11）
    - ``20``：最大值长度（允许最多 20 字节的数据）
    - ``8``：当前值长度（初始值为 8 字节）
    - ``12345678``：初始值（8 字节数据）

    **响应：**

    .. code-block:: none

      OK

  5. 查询新添加的配置项验证：

    **命令：**

    .. code-block:: none

      AT+SYSMFG=1,"ble_data","cfg31"

    **响应：**

    .. code-block:: none

      +SYSMFG:"ble_data","cfg31",7,26,31,16,0x2800,0x01,2,2,A100

      OK

    **命令：**

    .. code-block:: none

      AT+SYSMFG=1,"ble_data","cfg32"

    **响应示例：**

    .. code-block:: none

      +SYSMFG:"ble_data","cfg32",7,24,32,16,0x2803,0x01,3,1,02

      OK

    **命令：**

    .. code-block:: none

      AT+SYSMFG=1,"ble_data","cfg33"

    **响应示例：**

    .. code-block:: none

      +SYSMFG:"ble_data","cfg33",7,31,33,16,0xB100,0x11,20,8,12345678

      OK

  6. 重新初始化低功耗蓝牙并创建服务以应用更改：

    **命令：**

    .. code-block:: none

      AT+BLEINIT=2

    **响应：**

    .. code-block:: none

      OK

    .. only:: esp32 or esp32c3

      **命令：**

      .. code-block:: none

        AT+BLEGATTSSRVCRE

      **响应：**

      .. code-block:: none

        OK

      **命令：**

      .. code-block:: none

        AT+BLEGATTSSRVSTART

      **响应：**

      .. code-block:: none

        OK

  7. 查询服务验证新服务已添加：

    **命令：**

    .. code-block:: none

      AT+BLEGATTSSRV?

    **响应示例：**

    .. code-block:: none

      +BLEGATTSSRV:1,1,0xA002,1
      +BLEGATTSSRV:1,2,0xA003,1
      +BLEGATTSSRV:1,3,0xA100,1

      OK

    新添加的服务 UUID ``0xA100`` 已出现在服务列表中。

  8. 查询特征验证新特征已添加：

    **命令：**

    .. code-block:: none

      AT+BLEGATTSCHAR?

    **响应示例：**

    .. code-block:: none

      +BLEGATTSCHAR:"char",1,1,0xC300
      +BLEGATTSCHAR:"char",1,2,0xC301
      ...
      +BLEGATTSCHAR:"char",3,1,0xB100

      OK

    新添加的特征 UUID ``0xB100`` 已出现在特征列表中（属于服务 3，特征索引 1）。

  .. note::

    **BLE 服务配置要点：**

    - 添加新服务时，确保使用的 ``cfg`` 索引不与现有配置冲突
    - 一个完整的 BLE 服务至少需要三个配置项：主服务声明、特征声明、特征值
    - 如需特征描述符（如 Client Characteristic Configuration Descriptor），需添加额外配置项
    - 特征属性值可组合使用，常用属性包括：0x01（广播）、0x02（读）、0x04（无响应写）、0x08（写）、0x10（通知）、0x20（指示）、0x40（认证签名写）、0x80（扩展属性）

    **数据格式与生效：**

    .. list::

      - 每个 ``cfg`` 项对应 gatts_data.csv 文件中 GATT 服务表的一行
      - 数据格式：``index,uuid_len,uuid,perm,val_max_len,val_cur_len,value``
      :esp32 or esp32c3: - 通过 ``AT+SYSMFG`` 写入的配置需重新初始化 BLE（``AT+BLEINIT``）并重新创建服务（``AT+BLEGATTSSRVCRE``）后生效
      :esp32c2 or esp32c5 or esp32c6 or esp32c61: - 通过 ``AT+SYSMFG`` 写入的配置需重新初始化 BLE（``AT+BLEINIT``）后生效
      - 更多 GATT 表结构详情，请参阅 :doc:`../Compile_and_Develop/How_to_customize_BLE_services`

  .. _sysmfg-custom-data:

.. only:: esp32s2

  .. _sysmfg-custom-data:

自定义数据存储
--------------

除了预定义的命名空间（如 ``factory_param``、``client_cert``、``server_cert`` 等）之外，您还可以创建自定义命名空间来存储自己的应用数据。这对于存储需要在重启后保留的少量持久化数据非常有用，例如自定义配置、用户偏好设置或设备特定的参数。

创建和使用自定义命名空间
************************

本示例演示如何创建一个名为 ``my_app_ca`` 的自定义命名空间来存储应用特定的证书或配置数据。

1. 创建新命名空间并写入数据：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"my_app_ca","my_app_ca.0",8,100

  **响应：**

  .. code-block:: none

    OK

    >

  返回 ``>`` 后，发送您的二进制数据（100 字节）。数据发送完成后，系统返回：

  .. code-block:: none

    OK

  命名空间 ``my_app_ca`` 现已创建，其中包含一个键 ``my_app_ca.0``，存储了您的数据。

2. 查询自定义命名空间：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"my_app_ca"

  **响应：**

  .. code-block:: none

    +SYSMFG:"my_app_ca","my_app_ca.0",8

    OK

3. 从自定义命名空间读取数据：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"my_app_ca","my_app_ca.0"

  **响应：**

  .. code-block:: none

    +SYSMFG:"my_app_ca","my_app_ca.0",8,100,<data>

    OK

  响应包含数据类型（8 表示二进制）、长度（100 字节）和实际数据。

4. 向同一命名空间添加更多键：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"my_app_ca","my_app_ca.1",7,20

  **响应：**

  .. code-block:: none

    OK

    >

  **发送数据：**

  .. code-block:: none

    MyCustomConfigValue1

  **响应：**

  .. code-block:: none

    OK

5. 查询命名空间中的所有键：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"my_app_ca"

  **响应：**

  .. code-block:: none

    +SYSMFG:"my_app_ca","my_app_ca.0",8
    +SYSMFG:"my_app_ca","my_app_ca.1",7

    OK

6. 创建另一个命名空间存储应用配置参数：

  如果您需要存储配置参数（而非证书数据），建议创建单独的命名空间以保持语义清晰。

  **命令：**

  .. code-block:: none

    AT+SYSMFG=2,"my_app_config","timeout",6,5000

  **响应：**

  .. code-block:: none

    OK

  这将在 ``my_app_config`` 命名空间中存储一个 i32 类型的配置值（类型 6），键名为 "timeout"，值为 5000。

7. 读取配置参数：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=1,"my_app_config","timeout"

  **响应：**

  .. code-block:: none

    +SYSMFG:"my_app_config","timeout",6,4,5000

    OK

8. 擦除特定的键：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=0,"my_app_ca","my_app_ca.1"

  **响应：**

  .. code-block:: none

    OK

9. 擦除整个命名空间：

  **命令：**

  .. code-block:: none

    AT+SYSMFG=0,"my_app_ca"

  **响应：**

  .. code-block:: none

    OK

.. note::

  **自定义命名空间最佳实践：**

  - 自定义命名空间可用于存储任何类型的持久化数据
  - 根据需求选择合适的数据类型：数值类型（1-6）用于数字，字符串（7）用于文本，二进制（8）用于证书或二进制数据
  - **建议按用途分离命名空间**：例如将证书数据（``my_app_ca``）与配置参数（``my_app_config``）分开存储，便于管理和维护
  - 命名空间名称应具有描述性，清楚表明其用途（如 ``my_app_certs``、``my_app_settings``）
  - 键名可以是任意字符串，但建议使用有意义的名称以便识别
  - 所有自定义数据在重启后保持持久化，直到被明确擦除
  - 自定义命名空间独立于系统命名空间，不会影响 AT 固件功能
  - 创建自定义命名空间并存储数据后，可以使用 ``AT+SYSMFG?`` 查询，在命名空间列表中看到它

