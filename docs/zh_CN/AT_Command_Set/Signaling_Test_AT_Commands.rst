.. _FACT-AT:

[ESP8266 Only] 信令测试 AT 命令
==========================================

:link_to_translation:`en:[English]`

-  :ref:`AT+FACTPLCP <cmd-FACTPLCP>`：发送长 PLCP 或短 PLCP

.. _cmd-FACTPLCP:

[ESP8266 Only] :ref:`AT+FACTPLCP <FACT-AT>`：发送长 PLCP 或短 PLCP
--------------------------------------------------------------------------

设置命令
^^^^^^^^

**命令：**

::

    AT+FACTPLCP=<enable>,<tx_with_long>

**响应：**

::

    OK

参数
^^^^

-  **<enable>**：启用/禁用手动配置：

   -  0: 禁用手动配置，将使用 ``<tx_with_long>`` 参数的默认值；
   -  1: 启用手动配置，AT 发送的 PLCP 类型取决于 ``<tx_with_long>`` 参数。

-  **<tx_with_long>**：发送长 PLCP 或短 PLCP：

   -  0: 发送短 PLCP（默认）；
   -  1: 发送长 PLCP。
