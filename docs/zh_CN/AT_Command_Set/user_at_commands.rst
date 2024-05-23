.. _User-AT:

用户 AT 命令
=================

:link_to_translation:`en:[English]`

-  :ref:`介绍 <cmd-user-intro>`
-  :ref:`AT+USERRAM <cmd-USERRAM>`：操作用户的空闲 RAM
-  :ref:`AT+USEROTA <cmd-USEROTA>`：根据指定 URL 升级固件
-  :ref:`AT+USERWKMCUCFG <cmd-USERWKMCUCFG>`：设置 AT 唤醒 MCU 的配置
-  :ref:`AT+USERMCUSLEEP <cmd-USERMCUSLEEP>`：MCU 指示自己睡眠状态
-  :ref:`AT+USERDOCS <cmd-USERDOCS>`：查询固件对应的用户文档链接

.. _cmd-user-intro:

介绍
------

.. important::
  默认的 AT 固件支持此页面下的所有 AT 命令。如果您不需要 {IDF_TARGET_NAME} 支持用户命令，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：

  - 禁用 ``Component config`` -> ``AT`` -> ``AT user command support``

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

-  您可以 :doc:`从 GitHub Actions 里下载 <../Compile_and_Develop/How_to_download_the_latest_temporary_version_of_AT_from_github>` 所需要的 OTA 固件，也可以自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>` 生成所需要的 OTA 固件。

.. only:: esp32c2

  - 如果您使用的是 ESP32C2-2MB 模组配置，OTA 固件为 ``build/custom_ota_binaries/esp-at.bin.xz.packed``；如果您使用的是 ESP32C2-4MB 模组配置，OTA 固件为 ``build/esp-at.bin``。

.. only:: esp32 or esp32c3 or esp32c6 or esp32s2

  - OTA 固件为 ``build/esp-at.bin``。

-  升级速度取决于网络状况。
-  如果网络条件不佳导致升级失败，AT 将返回 ``ERROR``，请等待一段时间再试。
-  不建议升级到旧版本。降到旧版本会存在一定的兼容性问题，甚至无法运行，如果您坚持要升级到旧版本，请根据自己的产品自行测试验证功能。
-  建议升级 AT 固件后，调用 :ref:`AT+RESTORE <cmd-RESTORE>` 恢复出厂设置。
-  ``AT+USEROTA`` 支持 ``HTTP`` 和 ``HTTPS``。
-  AT 输出 ``>`` 字符后，数据中的特殊字符不需要转义字符进行转义，也不需要以新行结尾（CR-LF）。
-  当 URL 为 ``HTTPS`` 时，不建议 SSL 认证。如果要求 SSL 认证，您必须自行生成 PKI 文件然后将它们下载到对应的分区中，之后在 ``AT+USEROTA`` 命令的实现代码中加载证书。对于 PKI 文件请参考 :doc:`../Compile_and_Develop/How_to_update_pki_config`。对于 ``AT+USEROTA`` 命令，可参考 ESP-AT 工程提供的示例 `USEROTA <https://github.com/espressif/esp-at/blob/master/components/at/src/at_user_cmd.c>`_。
-  请参考 :doc:`../Compile_and_Develop/How_to_implement_OTA_update` 获取更多 OTA 命令。

示例
^^^^

::

    AT+USEROTA=36

    OK

    >
    Recv 36 bytes

    OK

.. _cmd-USERWKMCUCFG:

:ref:`AT+USERWKMCUCFG <User-AT>`：设置 AT 唤醒 MCU 的配置
---------------------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

此命令配置 AT 如何检查 MCU 的唤醒状态，以及 AT 如何唤醒 MCU。

- 当 MCU 是醒来的状态，AT 将直接向 MCU 发送数据，不会发送唤醒信号。
- 当 MCU 是睡眠的状态，AT 准备向 MCU 主动发送数据时（主动发送的数据和 :ref:`ESP-AT 消息报告 <at-messages-report>` 中定义的相同），AT 会先发送唤醒信号再发送数据。MCU 被唤醒或者超时后会清除唤醒信号。

**命令：**

::

    AT+USERWKMCUCFG=<enable>,<wake mode>,<wake number>,<wake signal>,<delay time>[,<check mcu awake method>]

**响应：**

::

    OK

参数
^^^^

- **<enable>**：启用或禁用唤醒配置。

  - 0：禁用唤醒 MCU 配置
  - 1：使能唤醒 MCU 配置

- **<wake mode>**：唤醒模式。

  - 1：GPIO 唤醒
  - 2：UART 唤醒

- **<wake number>**：该参数的意义取决于 ``<wake mode>`` 的值。

  - 如果 ``<wake mode>`` 是 1，``<wake number>`` 代表唤醒管脚 GPIO 编号。用户需要保证配置的唤醒管脚没有用作其它用途，否则需要用户做兼容性处理。
  - 如果 ``<wake mode>`` 是 2，``<wake number>`` 代表唤醒 UART 编号。当前只支持 1，即支持 UART1 唤醒 MCU。

- **<wake signal>**：该参数的意义取决于 ``<wake mode>`` 的值。

  - 如果 ``<wake mode>`` 是 1，``<wake signal>`` 代表唤醒电平。

    - 0：低电平
    - 1：高电平

  - 如果 ``<wake mode>`` 是 2，``<wake signal>`` 代表唤醒字节。范围：[0,255]。

- **<delay time>**：最大等待时间。单位：毫秒。范围：[0,60000]。该参数的意义取决于 ``<wake mode>`` 的值。

  - 如果 ``<wake mode>`` 是 1，则在 ``<delay time>`` 期间内，将一直保持 ``<wake signal>`` 电平。``<delay time>`` 到后，则反转 ``<wake signal>`` 电平。
  - 如果 ``<wake mode>`` 是 2，则立即发送 ``<wake signal>`` 字节，进入等待直到超时。

- **<check mcu awake method>**：AT 检查 MCU 是否处于醒来的状态。

  - Bit 0：是否开启与 :ref:`AT+USERMCUSLEEP <cmd-USERMCUSLEEP>` 命令的关联。默认开启。即：收到 AT+USERMCUSLEEP=0 命令，指示 MCU 醒来；收到 AT+USERMCUSLEEP=1 命令，指示 MCU 睡眠。
  - Bit 1：是否开启与 :ref:`AT+SLEEP=0/1/2/3 <cmd-SLEEP>` 命令的关联。默认禁用。即：收到 AT+SLEEP=0 命令，指示 MCU 醒来；收到 AT+SLEEP=1/2/3 命令，指示 MCU 睡眠。
  - Bit 2：是否开启 ``<delay time>`` 超时后指示 MCU 醒来功能。默认禁用。即：禁用时，delay time 后，指示 MCU 睡眠；使能时，delay time 后，指示 MCU 醒来。
  - Bit 3（暂未实现）：是否开启 GPIO 指示 MCU 醒来功能。默认不支持。

说明
^^^^

- 此命令只需要配置一次。
- 每次 AT 向 MCU 主动发送数据前，会先发送唤醒信号再进入等待，``<delay time>`` 时间到了之后直接发送数据。此超时会降低与 MCU 间的传输效率。
- 如果在 ``<delay time>`` 毫秒之前，AT 收到 ``<check mcu awake method>`` 里的任意唤醒事件，则立即清除唤醒状态；否则会等待 ``<delay time>`` 超时后，会自动清除唤醒状态。

示例
^^^^

::

    // 使能唤醒 MCU 配置。每次 AT 向 MCU 发送数据前，会先使用 Wi-Fi 模块的 GPIO18 管脚，高电平唤醒 MCU，同时保持高电平 10 秒。
    AT+USERWKMCUCFG=1,1,18,1,10000,3

    // 禁用唤醒 MCU 配置
    AT+USERWKMCUCFG=0

.. _cmd-USERMCUSLEEP:

:ref:`AT+USERMCUSLEEP <User-AT>`：MCU 指示自己睡眠状态
-----------------------------------------------------------

设置命令
^^^^^^^^

**功能：**

在 :ref:`AT+USERWKMCUCFG <cmd-USERWKMCUCFG>` 命令的 ``<check mcu awake method>`` Bit 0 配置情况下，此命令才会生效。用于告知 AT 当前 MCU 的睡眠状态。

**命令：**

::

    AT+USERMCUSLEEP=<state>

**响应：**

::

    OK

参数
^^^^

- **<state>**：

  - 0：指示 MCU 醒来。
  - 1：指示 MCU 睡眠。

示例
^^^^

::

    // MCU 告知 AT 当前 MCU 醒来
    AT+USERMCUSLEEP=0

.. _cmd-USERDOCS:

:ref:`AT+USERDOCS <User-AT>`：查询固件对应的用户文档链接
---------------------------------------------------------------------

查询命令
^^^^^^^^

**功能：**

查询当前运行固件对应的中英文用户文档链接。

**命令：**

::

    AT+USERDOCS?

**响应：**

::

    +USERDOCS:<"en url">
    +USERDOCS:<"cn url">

    OK

参数
^^^^

-  **<"en url">**：英文文档链接
-  **<"cn url">**：中文文档链接

示例
^^^^

::

    AT+USERDOCS?
    +USERDOCS:"https://docs.espressif.com/projects/esp-at/en/latest/{IDF_TARGET_PATH_NAME}/index.html"
    +USERDOCS:"https://docs.espressif.com/projects/esp-at/zh_CN/latest/{IDF_TARGET_PATH_NAME}/index.html"

    OK
