.. _ETH-AT:

[ESP32 Only] 以太网 AT 命令
=================================

:link_to_translation:`en:[English]`

-  :ref:`准备工作 <cmd-ETHPRE>`
-  [ESP32 Only] :ref:`AT+CIPETHMAC <cmd-ETHMAC>`：查询/设置 ESP 以太网的 MAC 地址
-  [ESP32 Only] :ref:`AT+CIPETH <cmd-ETHIP>`：查询/设置 ESP 以太网的 IP 地址

.. _cmd-ETHPRE:

:ref:`准备工作 <ETH-AT>`
------------------------------

运行以太网 AT 命令之前，请做好以下准备工作：

.. 注意::
    本节内容以 `ESP32-Ethernet-Kit <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-ethernet-kit.html>`_ 开发板为例介绍运行以太网 AT 命令前的准备工作。如果您使用的是其它模组或开发板，请查阅对应的技术规格书获取 RX/TX 管脚号。

- 修改 AT UART 管脚（因为默认的 AT UART 管脚和以太网功能管脚冲突）：

  - 打开 :component:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` 表格文件；
  - 将 ``WROVER-32`` 的 ``uart_tx_pin`` 从 GPIO22 改为 GPIO2， ``uart_rx_pin`` 从 GPIO19 改为 GPIO4， ``uart_cts_pin`` 从 GPIO15 改为 GPIO1，``uart_rts_pin`` 从 GPIO14 改为 GPIO1 （硬件流控功能可选，这里未使用该功能），更多信息请见 :doc:`../Compile_and_Develop/How_to_set_AT_port_pin`。

- 使能 ``AT ethernet support``，更多信息请见 :doc:`../Compile_and_Develop/How_to_enable_ESP_AT_Ethernet`。
- 编译后将该工程烧录至 ESP32-Ethernet-Kit。
- 连接硬件：
 
  - 连接主机 MCU（如 PC，可使用 USB 转串口模块）至 ESP32-Ethernet-Kit 的 GPIO2 (TX) 和 GPIO4 (RX)，不使用流控功能则无需连接 CTS/RTS；
  - ESP32-Ethernet-Kit 连接以太网网络。


.. _cmd-ETHMAC:

[ESP32 Only] :ref:`AT+CIPETHMAC <ETH-AT>`：查询/设置 ESP 以太网的 MAC 地址
--------------------------------------------------------------------------------------------

查询命令
^^^^^^^^
**功能：**

查询 ESP 以太网的 MAC 地址

**命令：**

::

    AT+CIPETHMAC?

**响应：**

::

    +CIPETHMAC:<"mac">
    OK

设置命令
^^^^^^^^

**功能：**

设置 ESP 以太网的 MAC 地址

**命令：**

::

    AT+CIPETHMAC=<"mac">

**响应：**

::

    OK

参数
^^^^

-  **<"mac">**：字符串参数，表示以太网接口的 MAC 地址。

说明
^^^^

-  固件默认不支持以太网 AT 命令 (详情请见 :doc:`../Compile_and_Develop/How_to_understand_the_differences_of_each_type_of_module`)，但是可通过以下方式使其支持该命令：``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT ethernet support``，然后编译工程（详情请见 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`）。
-  若 :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`，配置更改将保存在 NVS 区。
-  以太网接口的 MAC 地址不能与其他接口的相同。
-  ESP MAC 地址的 bit0 不能设为 1。例如，可设为 "1a:…"，但不可设为 "15:…"。
-  ``FF:FF:FF:FF:FF:FF`` 和 ``00:00:00:00:00:00`` 为无效 MAC 地址，不能设置。

示例
^^^^

::

    AT+CIPETHMAC="1a:fe:35:98:d4:7b"

.. _cmd-ETHIP:

[ESP32 Only] :ref:`AT+CIPETH <ETH-AT>`：查询/设置 ESP 以太网的 IP 地址
-------------------------------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询 ESP 以太网的 IP 地址

**命令：**

::

    AT+CIPETH?

**响应：**

::

    +CIPETH:ip:<ip>
    +CIPETH:gateway:<gateway>
    +CIPETH:netmask:<netmask>
    OK

设置命令
^^^^^^^^

**功能：**

设置 ESP 以太网的 IP 地址

**命令：**

::

    AT+CIPETH=<ip>[,<gateway>,<netmask>]

**响应：**

::

    OK

参数
^^^^

-  **<ip>**：字符串参数，表示 ESP 以太网的 IP 地址。
-  **[<gateway>]**：网关。
-  **[<netmask>]**：网络掩码。

说明
^^^^

-  固件默认不支持以太网 AT 命令 (详情请见 :doc:`../Compile_and_Develop/How_to_understand_the_differences_of_each_type_of_module`)，但是可通过以下方式使其支持该命令：``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT ethernet support``，然后编译工程（详情请见 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`）。
- 若 :ref:`AT+SYSSTORE=1 <cmd-SYSSTORE>`，配置更改将保存在 NVS 区。
- 本命令的设置命令与 DHCP 相互影响，如 :ref:`AT+CWDHCP <cmd-DHCP>`：

   -  若启用静态 IP，则 DHCP 会被禁用； 
   -  若启用 DHCP，则静态 IP 会被禁用； 
   -  最后一次配置会覆盖上一次配置。

示例
^^^^

::

    AT+CIPETH="192.168.6.100","192.168.6.1","255.255.255.0"
