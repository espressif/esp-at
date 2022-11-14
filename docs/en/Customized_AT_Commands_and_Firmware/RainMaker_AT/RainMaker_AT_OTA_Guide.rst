RainMaker AT OTA Guide
================================

:link_to_translation:`zh_CN:[中文]`

This document provides an introduction and detailed command examples to illustrate how to implement OTA upgrade using :doc:`RainMaker_AT_Command_Set` on {IDF_TARGET_NAME}.

.. contents::
   :local:
   :depth: 1

Introduction
------------

Currently, two trigger modes can be set on the `ESP RainMaker Dashboard <https://dashboard.rainmaker.espressif.com/login>`__:

  - **Force Push**: Direct upgrade. ESP RainMaker cloud will send an OTA message immediately.
  - **User Approval**: User confirmation. Users needs to confirm on the application to receive the OTA message sent by the ESP RainMaker cloud. (Recommended)

Currently, both the host MCU and the Wi-Fi MCU support OTA upgrade via the ESP RainMaker cloud:

  - **Host MCU OTA**: upgrade firmware OTA for the host MCU.
  - **Wi-Fi MCU OTA**: upgrade firmware OTA for {IDF_TARGET_NAME}.

In the OTA process, there are the following 5 states:

   - OTA in progress.
   - OTA succeeded.
   - OTA failed.
   - OTA delayed by the application.
   - OTA rejected due to some reason.

.. Important::

  - :ref:`AT+RMNODEINIT <cmd-RMNODEINIT>` command enables the OTA service by default.
  - The cloud will set the OTA job status to ``finished`` only when the OTA status reported by the device is ``OTA succeeded`` or ``OTA rejected due to some reason``.

Host MCU OTA
---------------

The host MCU completely controls the process of upgrading its firmware OTA.

The entire OTA process can be divided into the following three stages:

.. contents::
   :local:
   :depth: 1

Get Firmware Information
^^^^^^^^^^^^^^^^^^^^^^^^^^

ESP-AT automatically passively receives OTA information by default. You can also execute the :ref:`AT+RMOTAFETCH <cmd-RMOTAFETCH>` command to actively obtain OTA information.

#. Fetch OTA information

   Command:

   .. code-block:: none

     AT+RMOTAFETCH

   Response:

   .. code-block:: none

     OK

   If there is an unfinished OTA job on the cloud, ESP-AT will send the received OTA information instantly to the host MCU in the format of ``+RMFWNOTIFY:<type>,<size>,<url>,<fw_version>,<ota_job_id>``. Please refer to :ref:`AT+RMOTAFETCH <cmd-RMOTAFETCH>` command for more about OTA notify information.

   .. code-block:: none

     +RMFWNOTIFY:0,1740880,https://esp-rainmaker-ota-315787942180-prod.s3.us-east-1.amazonaws.com/users/GitHub_TMo6AB9Y96HjWnRkYdWfNK/firmwareimages/fnwH9nS4g/AT_OTA_TEST_2.bin?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=ASIAUTBTNTESPB4WFGEF%2F20221114%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20221114T114202Z&X-Amz-Expires=86400&X-Amz-Security-Token=FwoGZXIvYXdzEO3%2F%2F%2F%2F%2F%2F%2F%2F%2F%2FwEaDHkEHIt8cAKM633AFCLRAqKj%2F6Mb0%2FUTH%2BjJdcp%2FNPuCgDF3xDY0u9kZ1LKrL9oXCmFPTMn1ZQRRELIkKxsiPwW7Y7Wwr9h6iYrtUyhWpPoapDZYNKyesfQST%2BfFnWiq7nDYMyaqYfezIO2e60b50IEqW4aE58krGc0F4TuM1eG4ZkLX7skw%2BuSRkhVmMo4YmlrO%2Bbi49%2Fe12DmETnX69inFXvdZqyQ4ws2T7YmDu%2BC%2FSZZwoYkWnCBphMR%2BsORfE5SCpg9VAlJfEt56NmkDTUU875zIzw9IgR1pr4MQV0E3C3KCCy7qZGTSGdss9r4Qy783WpQcT%2FPfta4%2FR7fPzChbyPawAqQFDlkqjBAdANNbx3bSyDN%2BDyk%2BKXJVRXgtNTp3GQS8zp7%2FGcYakd8%2FXMMqDEWJjlKjO%2F%2Bt4Gyw5iLaHwNRzeyvzm6uDYf3%2F%2F2cPhvqHtPE5%2BXNU9wm%2Fv0FDpooitPImwYyLUyTk7gw1QQ%2FbAhlxffQbpHJLyDqGraMyZF8KtAHnv5WeCuvs0rFt1dhmq0ePg%3D%3D&X-Amz-SignedHeaders=host&X-Amz-Signature=fa5e04cd9eb8d010a954ccacba12d4fccc019853b34f60ebba82b212b0080561,d486921-dirty,cpuJraj3cxmAXuw8ER2GAe

Download Firmware
^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Set long download HTTP URL.

   Command:

   .. code-block:: none

     AT+HTTPURLCFG=1083

   Response:

   .. code-block:: none

     OK

     >

   This response indicates that AT is ready for receiving serial data. You should enter the URL now, and when the URL length reaches the ``<url length>`` value, the system returns:

   .. code-block:: none

     SET OK

#. Get firmware

   Command:

   .. code-block:: none

     AT+HTTPCGET=""

   Response:

   .. code-block:: none

     +HTTPCGET:<size>,<data>
     OK

   In this step, the host MUC must process the received firmware data by itself. Please refer to :ref:`AT+HTTPCGET <cmd-HTTPCGET>` command for more information.

Send the OTA Result
^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Send the OTA result

   Command:

   .. code-block:: none

     AT+RMOTARESULT=0,"cpuJraj3cxmAXuw8ER2GAe",2,"OTA success"

   Response:

   .. code-block:: none

     OK

Wi-Fi MCU OTA
---------------

{IDF_TARGET_NAME} completely controls the process of upgrading its firmware OTA.

In the OTA process, ESP-AT will automatically report the OTA status to the cloud, and will output status to the host MCU in the format of ``+RMOTA:<status>``. Please refer to :ref:`AT+RMOTAFETCH <cmd-RMOTAFETCH>` command for ``<status>`` information.

After the Wi-Fi MCU OTA upgrade is successful, ESP-AT will send ``+RMOTA:1`` to the host MCU. After that, when the host MCU sends :ref:`AT+RST <cmd-RST>` or :ref:`AT+RESTORE <cmd-RESTORE>` command to restart {IDF_TARGET_NAME}, or {IDF_TARGET_NAME} is restarted for other reasons, the new firmware will run. It is recommended that after the host MCU receives ``+RMOTA:1``, execute :ref:`AT+RST <cmd-RST>` at an appropriate time to restart the {IDF_TARGET_NAME} to switch to the new firmware.
