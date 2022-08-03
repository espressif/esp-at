******************************************
RainMaker AT 命令和固件
******************************************

.. toctree::
   :maxdepth: 1
   
   RainMaker AT 命令集 <RainMaker_AT_Command_Set>
   RainMaker AT 示例 <RainMaker_AT_Examples>
   缩写词索引 <index_of_abbreviations>

.. _rm-at-messages:

RainMaker AT 消息
==================

.. _rm-at-messages-report:

- ESP-AT RainMaker 消息报告（主动）

  ESP-AT 会报告系统中重要的 RainMaker 状态变化或消息。

  .. list-table:: ESP-AT 消息报告
     :header-rows: 1
     :widths: 60 60

     * - ESP-AT 消息报告
       - 说明
     * - 通用 AT 消息
       - 请参考 :ref:`ESP-AT 消息报告（主动） <at-messages-report>`
     * - +RMRECV:<src_name>,<device_name>,<param_name>:<param_value>
       - ESP-AT 收到来自 RainMaker 云端的消息，参数 ``<param_value>`` 的格式为字符串
     * - +RMCONNECTED
       - RainMaker 设备连接到云
     * - +RMDISCONNECTED
       - RainMaker 设备被动断开了与云的连接
     * - +RMRESET
       - RainMaker 设备接收到 reset 消息。Host MCU 在接收到消息后应主动执行命令 :ref:`AT+RST <cmd-RST>` 来重启模块
     * - +RMREBOOT
       - RainMaker 设备接收到 reboot 消息。Host MCU 在接收到消息后应主动执行命令 :ref:`AT+RST <cmd-RST>` 来重启模块
     * - +RMTIMEZONE
       - RainMaker 设备接收到 timezone 消息
     * - +RMMAPPINGDONE
       - RainMaker 设备完成用户和节点之间的映射
