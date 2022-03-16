AT 固件
=================

:link_to_translation:`en:[English]`

.. toctree::
  :maxdepth: 1

  :esp32: ESP32 AT binaries <ESP32_AT_binaries>
  :esp32c3: ESP32-C3 AT binaries <ESP32-C3_AT_binaries>

以上链接中下载的 ESP-AT 固件包含了若干个特定功能的二进制文件，``factory/factory_xxx.bin`` 文件是这些特定功能的二进制文件的合集。您可以仅烧录 ``factory/factory_xxx.bin`` 到起始地址为 0 的 flash 空间中，或者根据 ``download.config`` 文件中的信息将若干个二进制文件烧录到 flash 中对应起始地址的空间中。关于如何下载，请参考 :ref:`下载 AT 固件 <download-at-firmware>`。

-  ``at_customize.bin`` 提供了用户分区表，该表列出了 ``ble_data.bin`` 分区、SSL 证书分区、MQTT 证书分区以及 ``factory_param_XXX.bin`` 分区和其它一些分区的的起始地址和分区大小。您可以通过 AT 命令 :ref:`AT+FS <cmd-FS>` 和 :ref:`AT+SYSFLASH <cmd-SYSFLASH>` 来读和写该文件中罗列的分区里的内容。
-  ``factory_param_XXX.bin`` 指出了不同 ESP 模组之间的硬件配置（见下表）。请确保您的模组使用了正确的固件。

    .. note::

      如果您设计了自己的模组，那么可以参考 :doc:`../Compile_and_Develop/How_to_create_factory_parameter_bin` 对自定义模组进行配置，编译后会自动生成固件。或者，您也可以根据 ``UART 管脚/PSRAM/Flash`` 配置选择相似配置的固件（前提是要确保硬件满足要求，有关哪些固件适用于您的模组，请参考 :doc:`../Compile_and_Develop/esp-at_firmware_differences`）。

      当您根据 ``download.config`` 文件内容将固件下载到自定义模组中时，请使用自定义参数固件 ``customized_partitions/factory_param_XXX.bin`` 来代替 ``customized_partitions/factory_param.bin``。UART CTS 和 RTS 管脚是可选的。

.. only:: esp32

    - **ESP32 系列**

    .. list-table::
       :header-rows: 1

       * - 模组
         - UART 管脚（TX, RX, CTS, RTS）
         - Factory Parameter Bin 
       * - ESP32-WROOM-32 系列（ESP32 默认模组）
         - - GPIO17
           - GPIO16
           - GPIO15
           - GPIO14
         - ``factory_param_WROOM-32.bin``
       * - ESP32-WROVER 系列（支持经典蓝牙）
         - - GPIO22
           - GPIO19
           - GPIO15
           - GPIO14
         - ``factory_param_WROVER-32.bin``
       * - ESP32-PICO 系列
         - - GPIO22
           - GPIO19
           - GPIO15
           - GPIO14
         - ``factory_param_PICO-D4.bin``
       * - ESP32-SOLO 系列
         - - GPIO17
           - GPIO16
           - GPIO15
           - GPIO14
         - ``factory_param_SOLO-1.bin``

.. only:: esp32c3

    - **ESP32-C3 系列**

    .. list-table::
       :header-rows: 1

       * - 模组
         - UART 管脚（TX, RX, CTS, RTS）
         - Factory Parameter Bin 
       * - ESP32-C3-MINI 系列
         - - GPIO7
           - GPIO6
           - GPIO5
           - GPIO4
         - ``factory_param_MINI-1.bin``

-   ``ble_data.bin`` 在 ESP 工作于 Bluetooth LE 服务端的时候提供蓝牙服务；
-   ``server_cert.bin``、``server_key.bin`` 和 ``server_ca.bin`` 是 SSL 服务端示例证书；
-   ``client_cert.bin``、``client_key.bin`` 和 ``client_ca.bin`` 是 SSL 客户端示例证书；
-   ``mqtt_cert.bin``、``mqtt_key.bin`` 和 ``mqtt_ca.bin`` 是 MQTT SSL 客户端示例证书；

如果某些功能没有使用到，则不需要将相应的二进制文件下载到 flash 中。