Sleep AT 示例
==================================

:link_to_translation:`en:[English]`

本文档简要介绍并举例说明如何在 {IDF_TARGET_NAME} 系列产品上使用 AT 命令设置睡眠模式。

.. contents::
   :local:
   :depth: 1

简介
----

{IDF_TARGET_NAME} 系列采用先进的电源管理技术，可以在不同的电源模式之间切换。当前，ESP-AT 支持以下四种功耗模式（更多休眠模式请参考技术规格书）：

1. ``Active`` 模式：芯片射频处于工作状态。芯片可以接收、发射和侦听信号。
2. ``Modem-sleep`` 模式：CPU 可运行，时钟可被配置。Wi-Fi 基带、蓝牙基带和射频关闭。
3. ``Light-sleep`` 模式：CPU 暂停运行。RTC 存储器和外设以及 ULP 协处理器运行。任何唤醒事件（MAC、主机、RTC 定时器或外部中断）都会唤醒芯片。
4. ``Deep-sleep`` 模式：CPU 和大部分外设都会掉电，只有 RTC 存储器和 RTC 外设处于工作状态。

默认情况下，{IDF_TARGET_NAME} 会在系统复位后进入 ``Active`` 模式。当 CPU 不需要一直工作时，例如等待外部活动唤醒时，系统可以进入低功耗模式。

{IDF_TARGET_NAME} 的功耗，请参考 `{IDF_TARGET_NAME} 系列芯片技术规格书 <{IDF_TARGET_DATASHEET_CN_URL}>`__。

.. note::

  .. list::

    - 将分别描述在 Wi-Fi 模式和蓝牙模式下将 {IDF_TARGET_NAME} 设置为睡眠模式。
    - 在单 Wi-Fi 模式下，只有 ``station`` 模式支持 ``Modem-sleep`` 模式和 ``Light-sleep`` 模式。
    - 对于蓝牙模式下的 ``Light-sleep`` 模式，请确保外部存在 32 KHz 晶振。如果外部不存在 32 KHz 晶振，ESP-AT 将工作在 Modem-sleep 模式。

测量方法
^^^^^^^^^^^^^^^^^^^^

为避免功耗测试过程中出现一些不必要的干扰，建议使用集成芯片的乐鑫模组进行测试。

硬件连接可参考下图。（注意，图中开发板只保留了 {IDF_TARGET_NAME} ，外围元器件均已移除。）

.. figure:: ../../_static/at_command_examples/esp-power-consumption-hardware-connection.png
    :align: center
    :alt: {IDF_TARGET_NAME} 硬件连接

    {IDF_TARGET_NAME} 硬件连接

在 Wi-Fi 模式下设置为 Modem-sleep 模式
-----------------------------------------

#. 设置 Wi-Fi 为 station 模式。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接路由器。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 设置休眠模式为 Modem-sleep 模式。

   命令：

   .. code-block:: none

     AT+SLEEP=1

   响应：

   .. code-block:: none

     OK

.. note::

  * RF 将根据 AP 的 DTIM 定期关闭（路由器一般设置 DTIM 为 1）。

在 Wi-Fi 模式下设置为 Light-sleep 模式
-----------------------------------------

#. 设置 Wi-Fi 为 station 模式。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. 连接路由器。设置监听间隔为 3。

   命令：

   .. code-block:: none

     AT+CWJAP="espressif","1234567890",,,,3

   响应：

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   说明：

   - 您输入的 SSID 和密码可能跟上述命令中的不同。请使用您的路由器的 SSID 和密码。

#. 设置休眠模式为 Light-sleep 模式。

   命令：

   .. code-block:: none

     AT+SLEEP=2

   响应：

   .. code-block:: none

     OK

.. note::

  * CPU 将会自动休眠，RF 则会根据 :ref:`AT+CWJAP <cmd-JAP>` 设置的监听间隔定期关闭。

.. only:: esp32 or esp32c3

  在蓝牙广播态下设置为 Modem-sleep 模式
  ------------------------------------------------------

  #. 初始化为角色为蓝牙服务端。

    命令：

    .. code-block:: none

      AT+BLEINIT=2

    响应：

    .. code-block:: none

      OK

  #. 设置蓝牙广播参数。设置蓝牙广播间隔为 1 s。

    命令：

    .. code-block:: none

      AT+BLEADVPARAM=1600,1600,0,0,7,0,0,"00:00:00:00:00:00"

    响应：

    .. code-block:: none

      OK

  #. 开始广播

    命令：

    .. code-block:: none

      AT+BLEADVSTART

    响应：

    .. code-block:: none

      OK

  #. 禁用 Wi-Fi。

    命令：

    .. code-block:: none

      AT+CWMODE=0

    响应：

    .. code-block:: none

      OK

  #. 设置休眠模式为 Modem-sleep 模式。

    命令：

    .. code-block:: none

      AT+SLEEP=1

    响应：

    .. code-block:: none

      OK

  在蓝牙连接态下设置为 Modem-sleep 模式
  ------------------------------------------------------

  #. 初始化为角色为蓝牙服务端。

    命令：

    .. code-block:: none

      AT+BLEINIT=2

    响应：

    .. code-block:: none

      OK

  #. 开启蓝牙广播。

    命令：

    .. code-block:: none

      AT+BLEADVSTART

    响应：

    .. code-block:: none

      OK

  #. 等待连接。

    如果连接建立成功，则 AT 将会提示：

    .. code-block:: none

      +BLECONN:0,"47:3f:86:dc:e4:7d"
      +BLECONNPARAM:0,0,0,6,0,500
      +BLECONNPARAM:0,0,0,24,0,500

      OK

    说明：

    - 在这个示例中，蓝牙客户端的地址为 47:3f:86:dc:e4:7d。
    - 对于提示信息（+BLECONN and +BLECONNPARAM），请参考 :ref:`AT+BLECONN <cmd-BCONN>` 和 :ref:`AT+BLECONNPARAM <cmd-BCONNP>` 获取更多信息。

  #. 更新蓝牙连接参数。设置蓝牙连接间隔为 1 s。

    命令：

    .. code-block:: none

      AT+BLECONNPARAM=0,800,800,0,500

    响应：

    .. code-block:: none

      OK

    如果连接参数更新成功，则 AT 将会提示：

    .. code-block:: none

        +BLECONNPARAM:0,800,800,800,0,500

    说明：

    - 对于提示信息（+BLECONNPARAM），请参考 :ref:`AT+BLECONNPARAM <cmd-BCONNP>` 获取更多信息。

  #. 禁用 Wi-Fi。

    命令：

    .. code-block:: none

      AT+CWMODE=0

    响应：

    .. code-block:: none

      OK

  #. 设置休眠模式为 Modem-sleep 模式。

    命令：

    .. code-block:: none

      AT+SLEEP=1

    响应：

    .. code-block:: none

      OK

  在蓝牙广播态下设置为 Light-sleep 模式
  -------------------------------------------------------

  #. 初始化为角色为蓝牙服务端。

    命令：

    .. code-block:: none

      AT+BLEINIT=2

    响应：

    .. code-block:: none

      OK

  #. 设置蓝牙广播参数。设置蓝牙广播间隔为 1 s。

    命令：

    .. code-block:: none

      AT+BLEADVPARAM=1600,1600,0,0,7,0,0,"00:00:00:00:00:00"

    响应：

    .. code-block:: none

      OK

  #. 开始广播。

    命令：

    .. code-block:: none

      AT+BLEADVSTART

    响应：

    .. code-block:: none

      OK

  #. 禁用 Wi-Fi。

    命令：

    .. code-block:: none

      AT+CWMODE=0

    响应：

    .. code-block:: none

      OK

  #. 设置休眠模式为 Light-sleep 模式。

    命令：

    .. code-block:: none

      AT+SLEEP=2

    响应：

    .. code-block:: none

      OK

  在蓝牙连接态下设置为 Light-sleep 模式
  -----------------------------------------------------

  #. 初始化为角色为蓝牙服务端。

    命令：

    .. code-block:: none

      AT+BLEINIT=2

    响应：

    .. code-block:: none

      OK

  #. 开始广播。

    命令：

    .. code-block:: none

      AT+BLEADVSTART

    响应：

    .. code-block:: none

      OK

  #. 等待连接。

    如果连接建立成功，则 AT 将会提示：

    .. code-block:: none

      +BLECONN:0,"47:3f:86:dc:e4:7d"
      +BLECONNPARAM:0,0,0,6,0,500
      +BLECONNPARAM:0,0,0,24,0,500

      OK

    说明：

    - 在这个示例中，蓝牙客户端的地址为 47:3f:86:dc:e4:7d。
    - 对于提示信息（+BLECONN and +BLECONNPARAM），请参考 :ref:`AT+BLECONN <cmd-BCONN>` 和 :ref:`AT+BLECONNPARAM <cmd-BCONNP>` 获取更多信息。

  #. 更新蓝牙连接参数。设置蓝牙连接间隔为 1 s。

    命令：

    .. code-block:: none

      AT+BLECONNPARAM=0,800,800,0,500

    响应：

    .. code-block:: none

      OK

    如果连接参数更新成功，则 AT 将会提示：

    .. code-block:: none

        +BLECONNPARAM:0,800,800,800,0,500

    说明：

    - 对于提示信息（+BLECONNPARAM），请参考 :ref:`AT+BLECONNPARAM <cmd-BCONNP>` 获取更多信息。

  #. 禁用 Wi-Fi。

    命令：

    .. code-block:: none

      AT+CWMODE=0

    响应：

    .. code-block:: none

      OK

  #. 设置休眠模式为 Light-sleep 模式。

    命令：

    .. code-block:: none

      AT+SLEEP=2

    响应：

    .. code-block:: none

      OK

设置为 Deep-sleep 模式
-----------------------

#. 设置休眠模式为 Deep-sleep 模式。设置 deep-sleep 时间为 3600000 ms。

   命令：

   .. code-block:: none

     AT+GSLP=3600000

   响应：

   .. code-block:: none

     OK

   说明：

   - 设定时间到后，设备自动唤醒，调用深度睡眠唤醒桩，然后加载应用程序。
   - 对于 Deep-sleep 模式，唯一的唤醒方法是定时唤醒。
