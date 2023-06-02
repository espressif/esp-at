.. _WEB-AT:

Web 服务器 AT 命令
==========================================

:link_to_translation:`en:[English]`

-  :ref:`介绍 <cmd-web-server-intro>`
-  :ref:`AT+WEBSERVER <cmd-WEBSERVER>`: 启用/禁用通过 Web 服务器配置 Wi-Fi 连接

.. _cmd-web-server-intro:

介绍
------

.. important::
  默认的 AT 固件不支持此页面下的 AT 命令。如果您需要 {IDF_TARGET_NAME} 支持 Web 服务器命令，请自行 :doc:`编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`，在第五步配置工程里选择：

  - 启用 ``Component config`` -> ``AT`` -> ``AT Web Server command support``

.. _cmd-WEBSERVER:

:ref:`AT+WEBSERVER <WEB-AT>`: 启用/禁用通过 Web 服务器配置 Wi-Fi 连接
-------------------------------------------------------------------------------------------

设置命令
^^^^^^^^^^^

**命令：**

::

    AT+WEBSERVER=<enable>,<server_port>,<connection_timeout>

**响应：**

::

    OK

参数
^^^^^^^^^^

-  **<enable>**: 启用/禁用 Web 服务器。

   -  0: 禁用 Web 服务器并释放相关资源。 
   -  1: 启用 Web 服务器，您可以通过微信或者浏览器配置 Wi-Fi 连接信息。

-  **<server_port>**: Web 服务器端口号。
-  **<connection_timeout>**: 每个连接的超时时间。单位：秒。范围：[21,60]。

说明
^^^^^

-  有两种方法可以提供 Web 服务器所需的 HTML 文件。一种是使用 FAT 文件系统，此时需要启用 AT FS 命令。另一种是使用嵌入文件来存储 HTML 文件（默认设置）。
-  请确保开放的 socket 的最大数目不能小于 12，您可以在 menuconfig 中设置此项 ``./build.py menuconfig`` > ``Component config`` > ``LWIP`` > ``Max number of open sockets``，然后重新编译工程（参考文档 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`）。
-  AT 固件默认不支持 Web 服务器 AT 命令（参考文档 see :doc:`../Compile_and_Develop/esp-at_firmware_differences`），但您可以在 menuconfig 中设置支持 Web 服务器 AT 命令 ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT WEB Server command support``，然后重新编译工程（参考文档 :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`）。
-  ESP-AT 在 {IDF_TARGET_NAME} 系列设备中支持强制门户 (captive portal)，可参考 :ref:`示例 <using-captive-portal>`。
-  更多示例可参考文档 :doc:`../AT_Command_Examples/Web_server_AT_Examples`。
-  该命令的实现开源，源码请参考 :component_file:`at/src/at_web_server_cmd.c`。
-  请参考 :doc:`../Compile_and_Develop/How_to_implement_OTA_update` 获取更多 OTA 命令。

示例
^^^^

::

    // 启用 Web 服务器，端口 80，每个连接的超时时间 50 秒
    AT+WEBSERVER=1,80,50

    // 禁用 Web 服务器
    AT+WEBSERVER=0