.. _WEB-AT:

Web Server AT Commands
==========================================

:link_to_translation:`zh_CN:[中文]`

-  :ref:`Introduction <cmd-web-server-intro>`
-  :ref:`AT+WEBSERVER <cmd-WEBSERVER>`: Enable/disable Wi-Fi connection configuration via Web server.

.. _cmd-web-server-intro:

Introduction
------------

.. important::
  The default AT firmware does not support the AT commands listed on this page. If you need {IDF_TARGET_NAME} to support web server commands, you can compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections:

  - Enable ``Component config`` -> ``AT`` -> ``AT Web Server command support``

.. _cmd-WEBSERVER:

:ref:`AT+WEBSERVER <WEB-AT>`: Enable/disable Wi-Fi connection configuration via Web server
-------------------------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+WEBSERVER=<enable>,<server_port>,<connection_timeout>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<enable>**: Enable or disable Web server.

   -  0: Disable the Web server and release related resources. 
   -  1: Enable Web server, which means that you can use WeChat or a browser to configure Wi-Fi connection information.

-  **<server_port>**: The Web server port number.
-  **<connection_timeout>**: The timeout for the every connection. Unit: second. Range:[21,60].

Notes
^^^^^

-  There are two ways to provide the HTML files needed by the Web server. One is to use FAT file system, and you need to enable AT FS command at this time. The other is to use the embedded file to store HTML files (default setting).
-  The default HTML file is :component_file:`index.html <fs_image/index.html>`. If you need to customize the display format or text of the HTML file, you can directly modify this file. If you need to customize other content of the HTML file (e.g., add a field), you need to modify the corresponding source code file :component_file:`at_web_server_cmd.c <at/src/at_web_server_cmd.c>`.
-  Please make sure that the maximum number of open sockets is not less than 12, you may change the number by ``./build.py menuconfig`` > ``Component config`` > ``LWIP`` > ``Max bumber of open sockets`` and compile the project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`).
-  The default firmware does not support Web server AT commands (see :doc:`../Compile_and_Develop/esp-at_firmware_differences`), but you can enable it by ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT WEB Server command support`` and compile the project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`).
-  ESP-AT supports captive portals in {IDF_TARGET_NAME} series of devices. See :ref:`example <using-captive-portal>`.
-  For more examples, please refer to :doc:`../AT_Command_Examples/Web_server_AT_Examples`.
-  The command implementation is open-source. See the source code in :component_file:`at/src/at_web_server_cmd.c`.
-  Please refer to :doc:`../Compile_and_Develop/How_to_implement_OTA_update` for more OTA commands.

Example
^^^^^^^^

::

    // Enable the Web server with port 80, and the timeout for the every connection is 50 seconds
    AT+WEBSERVER=1,80,50

    // Disable the Web server
    AT+WEBSERVER=0