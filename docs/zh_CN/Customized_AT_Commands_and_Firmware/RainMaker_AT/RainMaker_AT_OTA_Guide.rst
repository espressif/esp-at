RainMaker AT OTA 指南
================================

:link_to_translation:`en:[English]`

本文档提供了详细的 :doc:`RainMaker_AT_Command_Set` 示例来说明如何在 {IDF_TARGET_NAME} 上进行 OTA 升级。

.. contents::
   :local:
   :depth: 1

简介
------------

当前 `ESP RainMaker Dashboard <https://dashboard.rainmaker.espressif.com/login>`__ 支持 2 种 OTA 触发方式：

  - **Force Push**：直接升级。ESP RainMaker 云立即下发一条 OTA 消息。
  - **User Approval**：用户确认。用户需要在手机应用上确认后才能接收到 ESP RainMaker 云下发的 OTA 消息。（推荐）

当前主控 MCU 和 Wi-Fi MCU 均支持通过 ESP RainMaker 云进行 OTA 升级：

  - **主控 MCU OTA**：OTA 固件用来升级主控 MCU。
  - **Wi-Fi MCU OTA**：OTA 固件用来升级 {IDF_TARGET_NAME}。

在 OTA 过程中，存在以下 5 种状态：

   - OTA 进行中。
   - OTA 成功。
   - OTA 失败。
   - OTA 被应用程序延迟。
   - OTA 由于某种原因被拒绝。

.. Important::

  - :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>` 命令默认开启 OTA 服务。
  - 只有当设备上报的 OTA 状态为 ``OTA 成功`` 或者 ``OTA 由于某种原因被拒绝`` 时，云端才会将 OTA job 状态设置为 ``完成``。

主控 MCU OTA
---------------

主控 MCU 完全控制其 OTA 的升级过程。

整个 OTA 流程可以分为以下三个阶段：

.. contents::
   :local:
   :depth: 1

获取固件信息
^^^^^^^^^^^^^

ESP-AT 默认会被动的接收 OTA 信息。你也可以主动执行 :ref:`AT+RMOTAFETCH <cmd-RMOTAFETCH>` 命令来获取 OTA 信息。

#. 获取 OTA 信息。

   命令：

   .. code-block:: none

     AT+RMOTAFETCH

   响应：

   .. code-block:: none

     OK

   如果云端存在未完成的 OTA job，ESP-AT 会立即将接收到的 OTA 信息发送到主控 MCU，格式为 ``+RMFWNOTIFY:<type>,<size>,<url>,<fw_version>,<ota_job_id>``。请参考 :ref:`AT+RMOTAFETCH <cmd-RMOTAFETCH>` 获取更多 OTA 通知信息。

   .. code-block:: none

     +RMFWNOTIFY:0,1740880,https://esp-rainmaker-ota-315787942180-prod.s3.us-east-1.amazonaws.com/users/GitHub_TMo6AB9Y96HjWnRkYdWfNK/firmwareimages/fnwH9nS4g/AT_OTA_TEST_2.bin?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=ASIAUTBTNTESPB4WFGEF%2F20221114%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20221114T114202Z&X-Amz-Expires=86400&X-Amz-Security-Token=FwoGZXIvYXdzEO3%2F%2F%2F%2F%2F%2F%2F%2F%2F%2FwEaDHkEHIt8cAKM633AFCLRAqKj%2F6Mb0%2FUTH%2BjJdcp%2FNPuCgDF3xDY0u9kZ1LKrL9oXCmFPTMn1ZQRRELIkKxsiPwW7Y7Wwr9h6iYrtUyhWpPoapDZYNKyesfQST%2BfFnWiq7nDYMyaqYfezIO2e60b50IEqW4aE58krGc0F4TuM1eG4ZkLX7skw%2BuSRkhVmMo4YmlrO%2Bbi49%2Fe12DmETnX69inFXvdZqyQ4ws2T7YmDu%2BC%2FSZZwoYkWnCBphMR%2BsORfE5SCpg9VAlJfEt56NmkDTUU875zIzw9IgR1pr4MQV0E3C3KCCy7qZGTSGdss9r4Qy783WpQcT%2FPfta4%2FR7fPzChbyPawAqQFDlkqjBAdANNbx3bSyDN%2BDyk%2BKXJVRXgtNTp3GQS8zp7%2FGcYakd8%2FXMMqDEWJjlKjO%2F%2Bt4Gyw5iLaHwNRzeyvzm6uDYf3%2F%2F2cPhvqHtPE5%2BXNU9wm%2Fv0FDpooitPImwYyLUyTk7gw1QQ%2FbAhlxffQbpHJLyDqGraMyZF8KtAHnv5WeCuvs0rFt1dhmq0ePg%3D%3D&X-Amz-SignedHeaders=host&X-Amz-Signature=fa5e04cd9eb8d010a954ccacba12d4fccc019853b34f60ebba82b212b0080561,d486921-dirty,cpuJraj3cxmAXuw8ER2GAe

下载固件
^^^^^^^^^^^^^

#. 设置长 HTTP URL。

   命令：

   .. code-block:: none

     AT+HTTPURLCFG=1083

   响应：

   .. code-block:: none

     OK

     >

   符号 ``>`` 标识 AT 准备好接收串口数据，此时你可以输入 URL，当数据长度达到参数 ``<url length>`` 的值时，系统返回：

   .. code-block:: none

     SET OK

#. 下载固件

   命令：

   .. code-block:: none

     AT+HTTPCGET=""

   响应：

   .. code-block:: none

     +HTTPCGET:<size>,<data>
     OK

   在该步中，主控 MCU 必须自己处理接收到的固件数据。请参考 :ref:`AT+HTTPCGET <cmd-HTTPCGET>` 命令获取更多信息。

上报 OTA 结果
^^^^^^^^^^^^^

#. 上报 OTA 结果。

   命令：

   .. code-block:: none

     AT+RMOTARESULT=0,"cpuJraj3cxmAXuw8ER2GAe",2,"OTA success"

   响应：

   .. code-block:: none

     OK

Wi-Fi MCU OTA
---------------

{IDF_TARGET_NAME} 完全控制其 OTA 的升级过程。

在 OTA 过程中，ESP-AT 会自动将 OTA 状态上报云端，并以 ``+RMOTA:<status>`` 格式向主控 MCU 输出状态。请参考 :ref:`AT+RMOTAFETCH <cmd-RMOTAFETCH>` 命令获取 ``<status>`` 信息。

Wi-Fi MCU OTA 成功后，ESP-AT 将向主控 MCU 发送 ``+RMOTA:1``。在此之后，当主控 MCU 发送 :ref:`AT+RST <cmd-RST>` 或者 :ref:`AT+RESTORE <cmd-RESTORE>` 命令重启 {IDF_TARGET_NAME}，或者 {IDF_TARGET_NAME} 因为其它原因重启后，均会运行新的固件。推荐主控 MCU 接收到 ``+RMOTA:1`` 后在合适的时间执行 :ref:`AT+RST <cmd-RST>` 命令重启 {IDF_TARGET_NAME} 来切换到新固件。
