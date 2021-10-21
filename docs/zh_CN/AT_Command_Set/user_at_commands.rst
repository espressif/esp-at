.. _User-AT:

用户 AT 命令
=================

:link_to_translation:`en:[English]`

-  :ref:`AT+USERRAM <cmd-USERRAM>`：操作用户的空闲 RAM
-  :ref:`AT+USEROTA <cmd-USEROTA>`：根据指定 URL 升级固件

.. _cmd-USERRAM:

:ref:`AT+USERRAM <User-AT>`: 操作用户的空闲 RAM
----------------------------------------------------------
查询命令
^^^^^^^^

**功能：**

查询用户当前可用的空闲 RAM 大小

**命令：**

::

    AT+USERRAM?

**响应：**

::

    +USERRAM:<size>

    OK

设置命令
^^^^^^^^

**功能：**

分配、读、写、擦除、释放用户 RAM 空间

**命令：**

::

    AT+USERRAM=<operation>,<size>[,<offset>]

**响应：**

::

    +USERRAM:<length>,<data>    // 只有是读操作时，才会有这个回复

    OK

参数
^^^^

-  **<operation>**：

   -  0：释放用户 RAM 空间
   -  1：分配用户 RAM 空间
   -  2：向用户 RAM 写数据
   -  3：从用户 RAM 读数据
   -  4：清除用户 RAM 上的数据

-  **<size>**: 分配/读/写的用户 RAM 大小
-  **<offset>**: 读/写 RAM 的偏移量。默认：0

说明
^^^^

- 请在执行任何其他操作之前分配用户 RAM 空间。
- 当 ``<operator>`` 为 ``write`` 时，系统收到此命令后先换行返回 ``>``，此时您可以输入要写的数据，数据长度应与 ``<length>`` 一致。
- 当 ``<operator>`` 为 ``read`` 时并且长度大于 1024，ESP-AT 会以同样格式多次回复，每次回复最多携带 1024 字节数据，最终以 ``\r\nOK\r\n`` 结束。

示例
^^^^

::

    // 分配 1 KB 用户 RAM 空间
    AT+USERRAM=1,1024

    // 向 RAM 空间开始位置写入 500 字节数据
    AT+USERRAM=2,500

    // 从 RAM 空间偏移 100 位置读取 64 字节数据
    AT+USERRAM=3,64,100

    // 释放用户 RAM 空间
    AT+USERRAM=0

.. _cmd-USEROTA:

:ref:`AT+USEROTA <User-AT>`：根据指定 URL 升级固件
---------------------------------------------------------------------

ESP-AT 在运行时，升级到指定 URL 上的新固件。

设置命令
^^^^^^^^

**功能：**

升级到 URL 指定版本的固件

**命令：**

::

    AT+USEROTA=<url len>

**响应：**

::

    OK

    >

上述响应表示 AT 已准备好接收 URL，此时您可以输入 URL，当 AT 接收到的 URL 长度达到 ``<url len>`` 后，返回：

::

    Recv <url len> bytes

AT 输出上述信息之后，升级过程开始。如果升级完成，返回：

::

    OK

如果参数错误或者固件升级失败，返回：

::

    ERROR

参数
^^^^

-  **<url len>**：URL 长度。最大值：8192 字节

说明
^^^^

-  升级速度取决于网络状况。
-  如果网络条件不佳导致升级失败，AT 将返回 ``ERROR``，请等待一段时间再试。
-  建议升级 AT 固件后，调用 :ref:`AT+RESTORE <cmd-RESTORE>` 恢复出厂设置。
-  ``AT+USEROTA`` 支持 ``HTTP`` 和 ``HTTPS``。
-  AT 输出 ``>`` 字符后，数据中的特殊字符不需要转义字符进行转义，也不需要以新行结尾（CR-LF）。
-  当 URL 为 ``HTTPS`` 时，不建议 SSL 认证。如果要求 SSL 认证，您必须自行生成 PKI 文件然后将它们下载到对应的分区中，之后在 ``AT+USEROTA`` 命令的实现代码中加载证书。对于 PKI 文件请参考 :doc:`../Compile_and_Develop/how_to_generate_pki_files`。对于 ``AT+USEROTA`` 命令，可参考 ESP-AT 工程提供的示例 `USEROTA <https://github.com/espressif/esp-at/blob/master/components/at/src/at_user_cmd.c>`_。

示例
^^^^

::

    AT+USEROTA=36

    OK

    >
    Recv 36 bytes

    OK
