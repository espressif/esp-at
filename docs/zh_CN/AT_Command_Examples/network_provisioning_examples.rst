.. include:: ../../inline_substitutions

AT 配网示例
=================================

:link_to_translation:`en:[English]`

本文档主要介绍以下几种 ESP-AT 支持的配网方式：

.. list::

  :not esp32s2: * :ref:`BluFi 配网 <example-at_blufi>`
  * :ref:`SmartConfig 配网 <example-at_smartconfig>`
  * :ref:`SoftAP 配网（WEB 配网） <example-at_softap>`
  * :ref:`WPS 配网 <example-at_wps>`

.. only:: esp32 or esp32c3 or esp32c6

  .. list-table:: 配网方式关键参数总结
     :header-rows: 1
     :widths: 45 35 35 35 35
  
     * - 配网方式
       - BluFi 配网
       - SmartConfig 配网
       - SoftAP 配网
       - WPS 配网
     * - 默认固件支持情况
       - |icon-green-check|
       - |icon-green-check|
       - |icon-red-cross| （:ref:`备注 1 <np-note>`）
       - |icon-green-check|
     * - 是否需要 BLE
       - |icon-green-check|
       - |icon-red-cross|
       - |icon-red-cross|
       - |icon-red-cross|
     * - 是否需要额外的手机 APP
       - |icon-green-check|
       - |icon-green-check| （:ref:`备注 2 <np-note>`）
       - |icon-green-check|
       - |icon-red-cross|
     * - 配网成功率
       - 高
       - 较高
       - 高
       - 高
     * - 操作复杂性
       - 简单
       - 简单
       - 复杂
       - 复杂
     * - 推荐指数
       - 推荐
       - 中等
       - 中等
       - 中等

.. only:: esp32c2

  .. list-table:: 配网方式关键参数总结
     :header-rows: 1
     :widths: 45 35 35 35 35
  
     * - 配网方式
       - BluFi 配网
       - SmartConfig 配网
       - SoftAP 配网
       - WPS 配网
     * - 默认固件支持情况
       - |icon-green-check| （:ref:`备注 3 <np-note>`）
       - |icon-green-check|
       - |icon-red-cross| （:ref:`备注 1 <np-note>`）
       - |icon-green-check|
     * - 是否需要 BLE
       - |icon-green-check|
       - |icon-red-cross|
       - |icon-red-cross|
       - |icon-red-cross|
     * - 是否需要额外的手机 APP
       - |icon-green-check|
       - |icon-green-check| （:ref:`备注 2 <np-note>`）
       - |icon-green-check|
       - |icon-red-cross|
     * - 配网成功率
       - 高
       - 较高
       - 高
       - 高
     * - 操作复杂性
       - 简单
       - 简单
       - 复杂
       - 复杂
     * - 推荐指数
       - 推荐
       - 中等
       - 中等
       - 中等

.. only:: esp32s2

  .. list-table:: 配网方式关键参数总结
     :header-rows: 1
     :widths: 45 35 35 35
  
     * - 配网方式
       - SmartConfig 配网
       - SoftAP 配网
       - WPS 配网
     * - 默认固件支持情况
       - |icon-green-check|
       - |icon-red-cross| （:ref:`备注 1 <np-note>`）
       - |icon-green-check|
     * - 是否需要 BLE
       - |icon-red-cross|
       - |icon-red-cross|
       - |icon-red-cross|
     * - 是否需要额外的手机 APP
       - |icon-green-check| （:ref:`备注 2 <np-note>`）
       - |icon-green-check|
       - |icon-red-cross|
     * - 配网成功率
       - 较高
       - 高
       - 高
     * - 操作复杂性
       - 简单
       - 复杂
       - 复杂
     * - 推荐指数
       - 推荐
       - 中等
       - 中等

.. _np-note:

  - 备注 1：若想 AT 固件中支持 SoftAP 配网，请参考： :doc:`../AT_Command_Set/Web_server_AT_Commands`。
  - 备注 2：使用 SmartConfig 中的 AirKiss 类型配网时需依赖微信 APP，国外用户可能无法使用微信 APP。

  .. only:: esp32c2

    - 备注 3: 默认的 ESP32C2-4MB AT 固件支持 BluFi 功能，而 ESP32C2-2MB AT 固件不支持 BluFi 功能。有关详细信息，请参阅 :ref:`Bluetooth LE AT 命令简介 <cmd-ble-intro>`。

.. only:: not esp32s2

  .. _example-at_blufi:

  BluFi 配网
  ------------

  BluFi 是一款基于蓝牙通道的 Wi-Fi 网络配置功能，更多信息请参考 `BluFi 文档 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/ble/blufi.html>`_。

  #. 在手机端安装 EspBluFi 应用程序。

     - Android：`安卓端 EspBluFi 下载 <https://github.com/EspressifApp/EspBlufiForAndroid/releases>`_ （`安卓端源码 <https://github.com/EspressifApp/EspBlufiForAndroid>`_）
     - iOS：`iOS 端 EspBluFi 下载 <https://apps.apple.com/cn/app/EspBlufi/id1450614082>`_ （`iOS 端源码 <https://github.com/EspressifApp/EspBlufiForiOS>`_）

  #. {IDF_TARGET_NAME} 设备设置 BluFi 名称。

     命令：

     .. code-block:: none

       AT+BLUFINAME="blufi_test"

     响应：

     .. code-block:: none

       OK

  #. 开启 BluFi。

     命令：

     .. code-block:: none

       AT+BLUFI=1

     响应：

     .. code-block:: none

       OK

  #. 手机创建 BluFi 连接并进行配网。

     4.1 BluFi 连接
     
       手机打开系统蓝牙和 GPS 定位功能，启动 EspBluFi 应用程序，找到名为 ``blufi_test`` 的设备并点击进入，再点击 ``连接`` 进行连接。此时，ESP 设备端应打印类似于 ``+BLUFICONN`` 的日志，应用程序页面也会输出类似以下信息，表明 BluFi 连接已成功建立。

       .. code-block:: none

         Connected <mac>

         Discover service and characteristics success

         Set notification enable complete

         Set mtu complete, mtu=...

     4.2 BluFi 配网

       连接完成后，在 EspBluFi 应用程序中点击 ``配网`` 按钮，页面将会跳转至 ``配置`` 页面。在此页面输入 Wi-Fi 的 SSID 和密码，并点击 ``确定`` 按钮开始配网。稍等片刻，应用程序页面也会显示类似以下信息。

       .. code-block:: none
  
         Post configure params complete
  
         Receive device status response:
         OpMode: Station
         Station connect Wi-Fi now
         Station connect Wi-Fi bssid: <mac>
         Station connect Wi-Fi ssid: <ssid>

     4.3 配网成功

       请稍等片刻，ESP 设备将输出以下日志。至此，ESP 设备的配网已成功完成。

       .. code-block:: none

         WIFI CONNECTED
         WIFI GOT IP

  #. （可选步骤）{IDF_TARGET_NAME} 设备发送 BluFi 用户自定义数据。
  
     命令：
  
     .. code-block:: none
  
       AT+BLUFISEND=4
  
     响应：
  
     .. code-block:: none
  
       >
    
     输入 4 字节数据，例如输入数据是 ``test``，之后 AT 将会输出以下信息。
  
     .. code-block:: none
  
       OK
  
     此时手机应用程序端显示了类似以下信息：
  
     .. code-block:: none
  
       Receive custom data:
       test
  
  #. {IDF_TARGET_NAME} 设备关闭 BluFi。
  
     命令：
  
     .. code-block:: none
  
       AT+BLUFI=0
  
     响应：
  
     .. code-block:: none
  
       +BLUFIDISCONN
  
       OK
  
     此时手机应用程序端显示了类似以下信息：
  
     .. code-block:: none
  
       Disconnect <mac>, status=19

.. _example-at_smartconfig:

SmartConfig 配网
--------------------

AT SmartConfig 配网有四种类型，分别是 ESP-TOUCH、AirKiss、ESP-TOUCH+AirKiss 和 ESP-TOUCH v2。以下是 AirKiss 和 ESP-TOUCH v2 类型的配网示例。

ESP-TOUCH v2 配网示例
^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. 在手机端安装 EspTouch 应用程序。

   - Android：`安卓端 EspTouch 下载 <https://github.com/EspressifApp/EsptouchForAndroid/releases>`_ （`安卓端应用程序源码 <https://github.com/EspressifApp/EsptouchForAndroid>`_）
   - iOS：`iOS 端 EspTouch 下载 <https://apps.apple.com/in/app/espressif-esptouch/id1071176700>`_ （`iOS 端应用程序源码 <https://github.com/EspressifApp/EsptouchForIOS>`_）

#. 设置 ESP 设备为 Station 模式。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} 设备开启 SmartConfig。

   命令：

   .. code-block:: none

     AT+CWSTARTSMART=4,0,"1234567890123456"

   响应：

   .. code-block:: none

     OK

#. 在手机端进行配网。

   请按照以下步骤操作：打开手机的 Wi-Fi，连接到目标网络（例如 SSID：test，密码：1234567890），启动 EspTouch 应用程序，点击 ``EspTouch V2`` 按钮。在跳转的页面中输入所连接 Wi-Fi 的密码、待配网的设备数量以及 AES 密钥，如下所示：

   .. code-block:: none

     Wi-Fi 密码：1234567890
     需要配网的设备数量：1
     AES 密钥：1234567890123456

#. 在 ESP 设备端确认配网成功。

   此时 ESP 设备端会输出类似如下信息：

   .. code-block:: none

     smartconfig type:ESPTOUCH_V2
     Smart get wifi info
     ssid:test
     password:1234567890
     WIFI CONNECTED
     WIFI GOT IP
     smartconfig connected wifi

   到此，ESP 设备的配网已成功完成。

#. {IDF_TARGET_NAME} 设备停止 SmartConfig。

   命令：

   .. code-block:: none

     AT+CWSTOPSMART

   响应：

   .. code-block:: none

     OK

AirKiss 配网示例
^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. 打开手机微信，搜索并关注“乐鑫信息科技”公众号。

#. 设置 ESP 设备为 Station 模式。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} 设备开启 SmartConfig。

   命令：

   .. code-block:: none

     AT+CWSTARTSMART=2

   响应：

   .. code-block:: none

     OK

#. 在手机端进行配网。

   手机连接到要配网的 Wi-Fi（例如 SSID：test，密码：1234567890），然后打开微信，进入“乐鑫信息科技”公众号，点击产品资源，找到 AirKiss 设备并进入。在跳转的页面中输入 Wi-Fi 密码 ``1234567890``，稍等片刻，手机页面将显示类似日志 ``配置成功......``。

   此时 ESP 设备端会输出类似如下信息：

   .. code-block:: none

     smartconfig type:AIRKISS
     Smart get wifi info
     ssid:test
     password:1234567890
     WIFI CONNECTED
     WIFI GOT IP
     smartconfig connected wifi

   到此，ESP 设备的配网已成功完成。

#. {IDF_TARGET_NAME} 设备停止 SmartConfig。

   命令：

   .. code-block:: none

     AT+CWSTOPSMART

   响应：

   .. code-block:: none

     OK

.. _example-at_softap:

SoftAP 配网
--------------------

AT SoftAP 配网指的是 WEB 配网。有关详细信息，请参见 :doc:`Web Server AT 示例 <../AT_Command_Examples/Web_server_AT_Examples>`。

.. _example-at_wps:

WPS 配网
--------------------

#. 准备一个支持 WPS 配网的路由器（例如 Wi-Fi SSID: test）。

#. 设置 ESP 设备为 Station 模式。

   命令：

   .. code-block:: none

     AT+CWMODE=1

   响应：

   .. code-block:: none

     OK

#. {IDF_TARGET_NAME} 设备开启 WPS 配网。

   命令：

   .. code-block:: none

     AT+WPS=1

   响应：

   .. code-block:: none

     OK

   此时根据路由器的使用手册，开启 WPS 功能，稍等片刻，ESP 设备端会输出类似如下信息，即设备配网已成功。

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

#. 查询 {IDF_TARGET_NAME} 设备连接上的 Wi-Fi 信息。

   命令：

   .. code-block:: none

     AT+CWJAP?

   响应：

   .. code-block:: none

     +CWJAP:"test",......
     OK
  
#. {IDF_TARGET_NAME} 设备关闭 WPS 配网。

   命令：

   .. code-block:: none

     AT+WPS=0

   响应：

   .. code-block:: none

     OK
