如何实现 OTA 升级
=============================================

:link_to_translation:`en:[English]`

本文档指导如何为 {IDF_TARGET_NAME} 系列模块实现 OTA 升级。目前，ESP-AT 针对不同场景提供了以下三种 OTA 命令。您可以根据自己的需求选择适合的 OTA 命令。

  #. :ref:`AT+USEROTA <cmd-USEROTA>`
  #. :ref:`AT+CIUPDATE <cmd-UPDATE>`
  #. :ref:`AT+WEBSERVER <cmd-WEBSERVER>`

文档结构如下所示：

- :ref:`upgrade-comparison`
- :ref:`upgrade-commands-use`

.. _upgrade-comparison:

OTA 命令对比及应用场景
-------------------------------------------------------------------

:ref:`AT+USEROTA <upgrade-comparison>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

此命令通过 URL 实现 OTA 升级。您可以升级到放置在 HTTP 服务器上的固件。目前该命令仅支持应用分区升级。请参考 :ref:`AT+USEROTA <cmd-USEROTA>` 获取更多信息。

由于此命令属于用户自定义命令，您可以通过修改 :component_file:`at/src/at_user_cmd.c` 源代码来实现该命令。

此命令的应用场景如下：

  #. **您有属于自己的 HTTP 服务器。**
  #. **您必须指定 URL。**

.. Important::

  - 如果您升级的固件为非乐鑫正式发布的固件，在升级完成后，您可能无法使用 AT+CIUPDATE 命令升级，除非您按照 `使用 AT+CIUPDATE 进行 OTA 升级`_ 创建了自己的设备。

:ref:`AT+CIUPDATE <upgrade-comparison>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

此命令使用 iot.espressif.cn 作为默认 HTTP 服务器。该命令不仅可以升级应用程序分区，还可以升级 ``at_customize.csv`` 中定义的用户自定义分区。如果您使用的是乐鑫发布的版本，该命令将只会升级到乐鑫发布的版本。请参考 :ref:`AT+CIUPDATE <cmd-UPDATE>` 获取更多信息。

使用该命令升级自定义 bin 文件，请选择以下方式之一。

  #. **将 iot.espressif.cn 替换为您自己的 HTTP 服务器，并实现交互流程。** 如何实现自己的 AT+CIUPDATE 命令，请参考 :component_file:`at/src/at_ota_cmd.c`。
  #. **在 iot.espressif.cn 上创建一个设备，并在其上上传 bin 文件。（前提是模组中运行的固件已经对应您在乐鑫服务器上创建的设备。）** 请参考 `使用 AT+CIUPDATE 进行 OTA 升级`_ 获取更多信息。

此命令的应用场景如下：

  #. **您只使用乐鑫发布的固件，只想升级到乐鑫发布的固件。**
  #. **您希望升级自定义的 bin 文件，但没有自己的 HTTP 服务器。**
  #. **您有自己的 HTTP 服务器。除了升级应用程序分区外，还希望升级在 at_customize.csv 文件中定义的用户自定义分区。**

:ref:`AT+WEBSERVER <upgrade-comparison>`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

此命令通过浏览器或微信小程序升级 AT 固件。目前，该命令仅提供升级应用程序分区的功能。在开始升级之前，请启用 web 服务器命令并提前将 AT 固件复制到电脑或者手机上。您可以参考 :ref:`AT+WEBSERVER <cmd-WEBSERVER>` 或者 :doc:`../AT_Command_Examples/Web_server_AT_Examples` 获取更多信息。

为了实现您自己的 HTML 页面，请参考示例 :component_file:`fs_image/index.html`。为了实现您自己的 AT+WEBSERVER 命令，请参考示例 :component_file:`at/src/at_web_server_cmd.c`。

此命令的应用场景如下：

  #. **您需要更方便快捷的 OTA 升级，不依赖于网络状态。**

.. Important::

  - 如果您升级的固件为非乐鑫正式发布的固件，在升级完成后，您可能无法使用 AT+CIUPDATE 命令升级，除非您按照 `使用 AT+CIUPDATE 进行 OTA 升级`_ 创建了自己的设备。

.. _upgrade-commands-use:

使用 ESP-AT OTA 命令执行 OTA 升级
-----------------------------------------

使用 AT+USEROTA 进行 OTA 升级
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

请参考 :ref:`AT+USEROTA <cmd-USEROTA>` 获取更多信息。

使用 AT+CIUPDATE 进行 OTA 升级
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

通过 :ref:`AT+CIUPDATE <cmd-UPDATE>` 命令升级自定义的 bin 文件，首先要做的就是将 bin 文件上传到 iot.espressif.cn 并且获取到 **token** 值。以下步骤描述了如何在 iot.espressif.cn 上创建设备并上传 bin 文件。

1. 打开网站 http://iot.espressif.cn 或者 https://iot.espressif.cn。

   .. figure:: ../../_static/compile_and_develop/ota/OTA-1.png
    :align: center
    :alt: 打开 iot.espressif.cn 网站
    :figclass: align-center

    打开 iot.espressif.cn 网站

2. 点击网页右上角的 "Join"，输入您的名字，邮箱地址和密码。

   .. figure:: ../../_static/compile_and_develop/ota/OTA-2.png
    :align: center
    :alt: 加入 iot.espressif.cn
    :figclass: align-center

    加入 iot.espressif.cn

.. note::

  - 当前 ``Join`` 功能暂不对新用户开放。如果您想使用该功能，请联系 `乐鑫 <https://www.espressif.com/zh-hans/contact-us/sales-questions>`__。

3. 点击网页左上角的 "Device"，然后点击 "Create" 来创建一个设备。

   .. figure:: ../../_static/compile_and_develop/ota/OTA-3.png
    :align: center
    :alt: 点击 "Device"
    :figclass: align-center

    点击 "Device"

   .. figure:: ../../_static/compile_and_develop/ota/OTA-4.png
    :align: center
    :alt: 点击 "Create"
    :figclass: align-center

    点击 "Create"

4. 当设备创建成功后会生成一个密钥，如下图所示：

   .. figure:: ../../_static/compile_and_develop/ota/OTA-5.png
    :align: center
    :alt: 生成一个密钥
    :figclass: align-center

    生成一个密钥

5. 使用该密钥来编译您的 OTA bin 文件。配置 AT OTA token 密钥的过程如下如所示：

   .. figure:: ../../_static/compile_and_develop/ota/OTA-6.png
    :align: center
    :alt: 配置 AT OTA token 密钥 - 步骤 1
    :figclass: align-center

    配置 AT OTA token 密钥 - 步骤 1

   .. figure:: ../../_static/compile_and_develop/ota/OTA-7.png
    :align: center
    :alt: 配置 AT OTA token 密钥 - 步骤 2 和 3
    :figclass: align-center

    配置 AT OTA token 密钥 - 步骤 2 和 3

   .. note::

      如果使用 SSL OTA，选项 "The SSL token for AT OTA" 也需要配置。

6. 点击 "Product" 进入网页，如下如所示。单击创建的设备，在 "ROM Deploy" 下输入版本和 corename。将步骤 5 中的 bin 文件重命令为 "ota.bin" 并保存。

   .. figure:: ../../_static/compile_and_develop/ota/OTA-8.png
    :align: center
    :alt: 输入版本和 corename
    :figclass: align-center

    输入版本和 corename

   .. note::

      - 如果您想要升级 ``at_customize.csv`` 中定义的用户自定义分区，只需将 ``ota.bin`` 替换为用户自定义分区的 bin 文件即可。
      - 对于 ``corename`` 字段，此字段仅仅用于帮助您区分 bin 文件。

7. 单击 ota.bin 将其保存为当前版本。

   .. figure:: ../../_static/compile_and_develop/ota/OTA-9.png
    :align: center
    :alt: 保存当前版本的 ota.bin
    :figclass: align-center

    保存当前版本的 ota.bin

8. 在设备上运行 :ref:`AT+USEROTA <cmd-USEROTA>` 命令。如果网络已连接，将开始 OTA 升级。

.. Important::

  - 设置上传到 iot.espressif.cn 的 bin 文件名称时，请遵循以下规则：

     - 如果升级 ``app`` 分区，请将 bin 文件名设置为 ``ota.bin``。
     - 如果升级用户自定义的分区，请将 bin 文件名设置为 ``at_customize.csv`` 中的 ``Name`` 字段。 例如，如果升级 ``factory_Param`` 分区，请将其设置为 ``factory_ param.bin``。

  - ESP-AT 将新固件存储在备用 OTA 分区中。这样，即使 OTA 由于意外原因失败，原始 ESP-AT 固件也能正常运行。但对于自定义分区，由于 ESP-AT 没有备份措施，请小心升级。
  - **如果您打算从一开始只升级自定制的 bin 文件，那么在发布初始版本时，就应该将 OTA token 设置为自己的 token 值。**

使用 AT+WEBSERVER 进行 OTA 升级
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

请参考 :ref:`AT+WEBSERVER <cmd-WEBSERVER>` 和 :doc:`../AT_Command_Examples/Web_server_AT_Examples` 获取更多信息。
