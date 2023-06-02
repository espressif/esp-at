.. _FACT-AT:

信令测试 AT 命令
==========================================

:link_to_translation:`en:[English]`

-  :ref:`介绍 <cmd-signal-test-intro>`
-  :ref:`AT+FACTPLCP <cmd-FACTPLCP>`：发送长 PLCP 或短 PLCP

.. _cmd-signal-test-intro:

介绍
------

.. important::
  默认的 AT 固件支持此页面下的所有 AT 命令。如果您不需要 {IDF_TARGET_NAME} 支持信令测试命令，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：

  - 禁用 ``Component config`` -> ``AT`` -> ``AT signaling test command support``

.. _cmd-FACTPLCP:

:ref:`AT+FACTPLCP <FACT-AT>`：发送长 PLCP 或短 PLCP
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
