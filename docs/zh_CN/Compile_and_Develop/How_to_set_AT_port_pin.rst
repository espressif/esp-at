如何设置 AT 端口管脚
==========================

{IDF_TARGET_HYPHEN_LOWERCASE_NAME: default="undefined", esp32="esp32", esp32c2="esp32-c2", esp32c3="esp32-c3"}

:link_to_translation:`en:[English]`

本文档介绍了如何修改 {IDF_TARGET_NAME} 系列固件中的 :term:`AT port` 管脚。默认情况下，ESP-AT 使用两个 UART 接口作为 AT 端口：一个用于输出日志（以下称为日志端口），另一个用于发送 AT 命令和接收响应（以下称为命令端口）。

要修改 {IDF_TARGET_NAME} 设备的 AT 端口管脚，应该：

- :doc:`克隆 ESP-AT 工程 <How_to_clone_project_and_compile_it>`。
- 在 menuconfig 配置工具或 factory_param_data.csv 表格中修改对应管脚。
- :doc:`编译工程 <How_to_clone_project_and_compile_it>`，在 ``build/customized_partitions/factory_param.bin`` 生成新的 bin 文件。
- :ref:`将新的 bin 文件烧录进设备 <flash-at-firmware-into-your-device>`。

本文档重点介绍如何修改管脚，点击上面的链接了解其它步骤的详细信息。

.. note::
  使用其它接口作为 AT 命令接口请参考 :project_file:`使用 AT SPI 接口 <main/interface/sdio/README.md>`, :project_file:`AT through SPI <main/interface/hspi/README.md>` 和 :project_file:`使用 AT 套接字接口 <main/interface/socket/README.md>`。

{IDF_TARGET_NAME} 系列
------------------------

{IDF_TARGET_NAME} AT 固件的日志端口和命令端口管脚可以自定义为其它管脚，请参阅 `《{IDF_TARGET_NAME} 技术参考手册》 <https://www.espressif.com/sites/default/files/documentation/{IDF_TARGET_HYPHEN_LOWERCASE_NAME}_technical_reference_manual_cn.pdf>`_ 查看可使用的管脚。

修改日志端口管脚
^^^^^^^^^^^^^^^^^

默认情况下，乐鑫提供的 {IDF_TARGET_NAME} AT 固件使用以下 UART0 管脚输出日志：

.. only:: esp32

  - TX：GPIO1
  - RX：GPIO3

.. only:: esp32c3 or esp32c2

  - TX：GPIO21
  - RX：GPIO20

在编译 ESP-AT 工程时，可使用 menuconfig 配置工具将其修改为其它管脚：

* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART for console output``
* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART TX on GPIO#``
* ``./build.py menuconfig`` --> ``Component config`` --> ``Common ESP-related`` --> ``UART RX on GPIO#``

修改命令端口管脚
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

默认情况下，UART1 用于发送 AT 命令和接收 AT 响应，其管脚定义在 :component_file:`factory_param_data.csv <customized_partitions/raw_data/factory_param/factory_param_data.csv>` 表格中的 uart_port、uart_tx_pin、uart_rx_pin、uart_cts_pin 和 uart_rts_pin 列。

您可以直接在 factory_param_data.csv 表中修改端口管脚：
  
- 打开您本地的 factory_param_data.csv。
- 找到模组所在的行。
- 根据需要设置 ``uart_port``。
- 根据需要设置 ``uart_tx_pin`` 和 ``uart_rx_pin``。
- 若不需要使用硬件流控功能，请将 ``uart_cts_pin`` 和 ``uart_rts_pin`` 设置为 -1。
- 保存表格。
