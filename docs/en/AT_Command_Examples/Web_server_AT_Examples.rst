Web Server AT Example
=======================

:link_to_translation:`zh_CN:[中文]`

This document mainly introduces the use of AT web server, mainly involving the following applications:

.. contents::
   :local:
   :depth: 1

.. note::

   The default firmware does not support web server AT commands, please refer to :doc:`../AT_Command_Set/Web_server_AT_Commands` to enable the web server.

Wi-Fi Provisioning Using a Browser
-------------------------------------

Introduction
^^^^^^^^^^^^^^

With the web server, mobile phone or PC is able to control ESP device‘s Wi-Fi provisioning service. You can use a mobile phone or computer to connect to the SoftAP of the ESP device, open the web pages via browser, start provisioning service, and then the ESP device can connect to the target router as you set.

Introduction to Operation Steps
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The whole process can be divided into the following three steps:  

.. contents::
   :local:
   :depth: 1

Use STA Device to Connect to ESP Device
""""""""""""""""""""""""""""""""""""""""

Firstly, ESP device needs to be configured to softAP + STA mode, and creates a web server to wait for Wi-Fi provisioning messages. In this case, a mobile phone or a PC can connect to the ESP softAP as a station. The corresponding AT commands are as follows:

#. Clear the previous Wi-Fi provisioning information.


   - Command
   
     ::
 
       AT+RESTORE

#. Set the Wi-Fi mode to Station+SoftAP.


   - Command
   
     ::
 
       AT+CWMODE=3

#. Set the configuration of an ESP SoftAP.（For example, set the default connection ssid to "pos_softap", Wi-Fi without password.）


   - Command
   
     ::
 
       AT+CWSAP="pos_softap","",11,0,3

#. Enable multiple connections.


   - Command
   
     ::
 
       AT+CIPMUX=1

#. Create a web server, port: 80, connection timeout: 25 s (default maximum is 60 s).


   - Command
   
     ::
 
       AT+WEBSERVER=1,80,25

After starting the web sever according to the above commands, you can turn on the Wi-Fi connection function on your STA device, and connect it to the softAP of the ESP device:

.. figure:: ../../_static/Web_server/web_brower_wifi_ap_en.png
   :align: center
   :alt: connects_to_the_ESP_AP
   :figclass: align-center

   The browser connects to the ESP AP

Use the Browser to Send Wi-Fi Connection Information
"""""""""""""""""""""""""""""""""""""""""""""""""""""""

After your STA device connected to the ESP softAP, it can send Wi-Fi connection information to ESP in an HTTP request. Please note that if your target AP is the hotspot of the device which opens the web pages, you may not receive the Wi-Fi connection result.
You can enter the default IP address of the web server in the browser (the default IP is 192.168.4.1, or you can query the current SoftAP IP address by command AT+CIPAP?), open the Wi-Fi provisioning interface, and enter the ssid and password of the router to be connected, click "Connect" to let ESP device start connecting to the router:

.. figure:: ../../_static/Web_server/web_brower_open_html_en.png
   :align: center
   :alt: opens_the_Wi-Fi_configuration_interface
   :figclass: align-center

   The browser opens the Wi-Fi provisioning interface

Or you can click the drop-down box of SSID to list all APs nearby, select the target AP and enter the password, and then click "Connect" to let the ESP device start connecting to the router:

.. figure:: ../../_static/Web_server/web_brower_get_ap_record_en.png
   :align: center
   :alt: Schematic_diagram_of_browser_obtaining_Wi-Fi_AP_list
   :figclass: align-center

   Schematic diagram of browser obtaining Wi-Fi AP list

Get the Result of Wi-Fi Connection
""""""""""""""""""""""""""""""""""""

After the Wi-Fi connection is established successfully, the web page will be displayed as follows:

.. figure:: ../../_static/Web_server/web_brower_wifi_connect_success_en.png
   :align: center
   :alt: Browser_Wi-Fi_configuration_is_successful
   :figclass: align-center

   Wi-Fi connection is established successfully

**Note** 1: After the Wi-Fi connection is established successfully, the webpage will be closed automatically. If you want to continue to access the webpage, please re-enter the IP address of the ESP device and reopen the webpage.

At the same time, the following messages will be returned from the ESP-AT command port:

::

    +WEBSERVERRSP:1      // meaning that ESP device has received Wi-Fi connection information  
    WIFI CONNECTED       // meaning that ESP device is connecting 
    WIFI GOT IP          // meaning that ESP device connect successfully to the destination rounter
    +WEBSERVERRSP:2      // meaning that STA device has received Wi-Fi connection result, and web resources can be released  

If the ESP device fails to connect to the router, the web page will display:

.. figure:: ../../_static/Web_server/web_brower_wifi_connect_fail_en.png
   :align: center
   :alt: ESP_device_connects_fail
   :figclass: align-center

   ESP device fails to connect to the router

At the same time, the following messages will be returned from the ESP-AT command port:

::

    +WEBSERVERRSP:1      // meaning that ESP device has received Wi-Fi connection information, but failed to connect to the rounter.

Troubleshooting
^^^^^^^^^^^^^^^^^^^

**Note** 1: The network configuration page received a prompt "Connection failed". Please check whether the Wi-Fi AP of the ESP module is correctly turned on, and the relevant configuration of the AP, and confirm that the correct AT command has been entered to successfully enable the web server.

OTA Firmware Upgrade Using a Browser
---------------------------------------

Introduction
^^^^^^^^^^^^^^

After the browser opens the web page of the web server, you can choose to enter the OTA upgrade page to upgrade the firmware of the ESP device through the web page.

Introduction to Operation Steps
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. contents::
   :local:
   :depth: 1

Open the OTA Configuration Page
""""""""""""""""""""""""""""""""""""

As shown in the figure, click on the "OTA" option in the lower right corner of the web page, and after opening the OTA configuration page, you can view the current firmware version and AT Core version:

.. figure:: ../../_static/Web_server/web_brower_ota_config_page_en.png
   :align: center
   :alt: OTA_configuration_page
   :figclass: align-center

   OTA configuration page

**Note** 1: The configuration interface can only be opened when the STA device is connected to the AP of the ESP device, or the STA device accessing the OTA configuration page is connected to the ESP device in the same subnet.

**Note** 2: The "current app version" displayed on the webpage is the version number of the application. You can change the version number through ``./build.py menuconfig`` --> ``Component config`` --> ``AT`` --> ``AT firmware version`` (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`). In this case, you can manage your application firmware version.

Select and Send the New Firmware
"""""""""""""""""""""""""""""""""

As shown in the figure, click the "Browse" button on the page and select the new firmware to be sent:

.. figure:: ../../_static/Web_server/web_brower_ota_chose_app_en.png
   :align: center
   :alt: Select_the_new_version_of_firmware_to_be_sent
   :figclass: align-center

   Select the new version of firmware to be sent

**Note** 1: Before sending the new firmware, the system will check the selected firmware. The suffix of the firmware name must be .bin, and its size should not exceed 2M.

Get the Result of OTA
"""""""""""""""""""""""

As shown in the figure, if the ESP device OTA successfully, it will prompt "OTA Succeeded":

.. figure:: ../../_static/Web_server/web_brower_send_app_result_en.png
   :align: center
   :alt: The_new_firmware_was_sent_successfully
   :figclass: align-center

   The new firmware was sent successfully

At the same time, the following messages will be returned from the ESP-AT command port:

::

    +WEBSERVERRSP:3      // meaning that ESP device begin to receive ota data
    +WEBSERVERRSP:4      // meaning that ESP device has received all firmware data，and you can choose to restart the ESP device to apply the new firmware

If the received firmware data verification fails, the following message will be received on the serial port:

::

    +WEBSERVERRSP:3      // meaning that ESP device begin to receive ota data
    +WEBSERVERRSP:5      // meaning that the received OTA data verification failed. You can choose to reopen the OTA configuration interface and follow the above steps to restart the firmware upgrade

Wi-Fi Provisioning Using a WeChat Applet
-------------------------------------------

Introduction
^^^^^^^^^^^^^

The WeChat applet can automatically connect to the WiFi AP of the ESP device, and then send the ssid and password required by the ESP device to connect to the network.

Introduction to Operation Steps
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The whole process can be divided into the following four steps:

.. contents::
   :local:
   :depth: 1

Configure ESP Device Parameters
"""""""""""""""""""""""""""""""""""

Firstly, ESP device needs to be configured to softAP + STA mode, and creates a web server to wait for Wi-Fi provisioning messages. In this case, a mobile phone or a PC can connect to the ESP softAP as a station. The corresponding AT commands are as follows:

#. Clear the previous Wi-Fi provisioning information.


   - Command
   
     ::
 
       AT+RESTORE

#. Set the Wi-Fi mode to Station+SoftAP.


   - Command
   
     ::
 
       AT+CWMODE=3

#. Set the configuration of an ESP SoftAP.（For example, set the default connection ssid to "pos_softap" , and password to "espressif".）


   - Command
   
     ::
 
       AT+CWSAP="pos_softap","espressif",11,3,3

  .. note::

     By default, the WeChat applet initiates a connection to the SoftAP whose ssid is `pos_softap` and password is `espressif`. Please make sure to set the parameters of the ESP device according to the above configuration.

#. Enable multiple connections.


   - Command
   
     ::
 
       AT+CIPMUX=1

#. Create a web server, port: 80, connection timeout: 40 s (default maximum is 60 s).


   - Command
   
     ::
 
       AT+WEBSERVER=1,80,40

Load WeChat Applet
"""""""""""""""""""""""

Open the mobile phone WeChat, scan the following QR code:

.. figure:: ../../_static/Web_server/web_wechat_applet_qr.png
   :align: center
   :alt: Get_the_QR_code_of_the_applet
   :figclass: align-center

   Get the QR code of the applet

Open the WeChat applet and enter the Wi-Fi provisioning interface:

.. figure:: ../../_static/Web_server/web_wechat_open_applet.png
   :align: center
   :alt: Wi-Fi_configuration_interface
   :figclass: align-center

   Wi-Fi provisioning interface

Target AP Selection
"""""""""""""""""""""

After loading the WeChat applet, there are two situations according to different target AP:

Situation 1. If your target AP is the hotspot of the mobile phone which running the WeChat applet, please select the "Local phone hotspot" option box on the WeChat applet page.

Situation 2. If your target AP is just another AP, not as the special situation one as above, then please do not select the "Local phone hotspot" option box.

Use the WeChat Applet to Send Wi-Fi Connection Information
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

The target AP to be accessed is not the hotspot provided by the mobile phone which loading the WeChat applet.
**************************************************************************************************************

Here, take connecting to a router as an example, the process of Wi-Fi Connection configuration is introduced:

1.Turn on the mobile Wi-Fi and connect to the router:

.. figure:: ../../_static/Web_server/web_wechat_connect_rounter.png
   :align: center
   :alt: connect_to_the_rounter
   :figclass: align-center

   connect to the rounter

2.Open the WeChat applet, you can see that the applet page has automatically displayed the ssid of the current router as "FAST_FWR310_02".

.. figure:: ../../_static/Web_server/web_wechat_get_rounter_info.png
   :align: center
   :alt: get_rounter_info
   :figclass: align-center

   The applet obtains the information of the router to be connected

Note: If the ssid of the connected router is not displayed on the current page, please click "Re-enter applet" in the following figure to refresh the current page:

.. figure:: ../../_static/Web_server/web_wechat_update_rounter_info.png
   :align: center
   :alt: Re-enter_the_applet
   :figclass: align-center

   Re-enter the applet

3.After entering the password of the router, click "Connect".

.. figure:: ../../_static/Web_server/web_wechat_rounter_connecting.png
   :align: center
   :alt: wechat_rounter_connecting
   :figclass: align-center

   The applet starts the ESP device to connect to the router

4.After the Wi-Fi connection is established successfully, the web page will be displayed as follows:

.. figure:: ../../_static/Web_server/web_wechat_rounter_connect_success.png
   :align: center
   :alt: wechat_rounter_connect_success
   :figclass: align-center

   The applet Wi-Fi provisioning is successful

At the same time, the following messages will be returned from the ESP-AT command port:

::

    +WEBSERVERRSP:1      // meaning that ESP device has received Wi-Fi connection information  
    WIFI CONNECTED       // meaning that ESP device is connecting 
    WIFI GOT IP          // meaning that ESP device connect successfully to the destination rounter
    +WEBSERVERRSP:2      // meaning that STA device has received Wi-Fi connection result, and web resources can be released  

5.If the ESP device fails to connect to the router, the page will display:

.. figure:: ../../_static/Web_server/web_wechat_rounter_connect_fail.png
   :align: center
   :alt: wechat_rounter_connect_fail
   :figclass: align-center

   The applet Wi-Fi provisioning is failed

At the same time, the following messages will be returned from the ESP-AT command port:

::

    +WEBSERVERRSP:1      // meaning that ESP device has received Wi-Fi connection information, but failed to connect to the rounter.

The target AP to be accessed is the hotspot provided by the mobile phone which loading the WeChat applet.
**************************************************************************************************************

If the target AP to be accessed is the hotspot provided by the mobile phone which loading the WeChat applet, it is not necessary to enter the ssid, but only needs to enter the password of the AP, and turn on the mobile AP in time according to the prompts.

1.Select the "Local phone hotspot" option box on the WeChat applet page, enter the password of the local hotspot, and click "Connect".

.. figure:: ../../_static/Web_server/web_wechat_enter_local_password.png
   :align: center
   :alt: wechat_enter_local_password
   :figclass: align-center

   Enter the password of the AP

2.After receiving the prompt "Connecting to the mobile phone hotspot", please check that the local mobile phone hotspot is turned on. At this time, the ESP device will automatically scan the surrounding hotspots and initiate a connection.

.. figure:: ../../_static/Web_server/web_wechat_start_connect.png
   :align: center
   :alt: wechat_start_connect
   :figclass: align-center

   Start to connect to the AP

3.The display of the WiFi connection results on the applet page and the data output from the serial port are the same as the above-mentioned "The target AP to be accessed is not the hotspot provided by the mobile phone which loading the WeChat applet.", please refer to the above.

Troubleshooting
^^^^^^^^^^^^^^^^^^^
**Note** 1: The Wi-Fi provisioning page received a prompt "Data transmission failed". Please check whether the Wi-Fi AP of the ESP device is correctly turned on, and the relevant configuration of the AP, and confirm that the correct AT command has been entered to successfully enable the web server.

**Note** 2: The Wi-Fi provisioning page receives a prompt "Failed to connect to the AP". Please check whether the Wi-Fi connection function of the mobile phone is turned on, check whether the Wi-Fi AP of the ESP device is correctly turned on, and whether the ssid and password of the AP are configured according to the above steps.

**Note** 3: The Wi-Fi provisioning page receives a prompt "The Wi-Fi provisioning saved by the system expired". Please manually connect the ESP device AP with a mobile phone, and confirm that the ssid and password of the ESP module have been configured according to the above steps.

OTA Firmware Upgrade Using a WeChat Applet
-------------------------------------------
The WeChat applet support online firmware upgrade , please refer to the above-described `Configure ESP Device Parameters`_ specific steps performed ESP device configuration (if the configuration has been completed, do not repeat configuration). Once configured, the device performs OTA firmware upgrade processes is similar as `OTA Firmware Upgrade Using a Browser`_ .

[ESP32][ESP32-S Series][ESP32-C Series] Using Capture Portal
--------------------------------------------------------------

Introduction
^^^^^^^^^^^^^

Captive Portal is commonly used to present a specified page to newly connected devices of a Wi-Fi or wired network. For more information about Captive Portal, please refer to `Captive Portal Wiki <https://en.wikipedia.org/wiki/Captive_portal>`__ .

.. note::

   The default firmware does not support web server Captive Portal, you may enable it by ``./build.py menuconfig`` > ``Component config`` > ``AT`` > ``AT WEB Server command support`` > ``AT WEB captive portal support`` and build the project (see :doc:`../Compile_and_Develop/How_to_clone_project_and_compile_it`). In addition, enabling this feature may cause page skipping when using wechat applet for Wi-Fi provisioning or OTA firmware upgrade. It is recommended that this feature be enabled only when accessing at web using browser.

Introduction to Operation Steps
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

After Enable Captive Portal support, please refer to `Use STA Device to Connect to ESP Device`_ to complete the configuration of the ESP device, and then connect to the AP of the ESP device:

.. figure:: ../../_static/Web_server/captive_portal_auth_pages_en.png
   :align: center
   :alt: auth_pages
   :figclass: align-center

   Connect to the AP with Captive Portal enabled

As shown in the figure above, after the Station device is connected to the AP of the ESP device with the Captive Portal function enabled, it will prompt "requires login/authentication", and then the browser will automatically open and jump to the main interface of AT Web. If it cannot be redirected automatically, please follow the instructions of the Station device, click "Authentication" or click the name of the "pos_softap" hotspot in the figure above to manually trigger the Captive Portal to automatically open the browser and enter the main interface of AT Web.

Troubleshooting
^^^^^^^^^^^^^^^^^^^

**Note** 1: Both Station device and AP device support the Captive Portal function to ensure the normal use of this function. Therefore, if the device is connected to the AP of the ESP device, but it does not prompt "Login/Authentication", it may be that the Station device does not support this function. In this case, please refer to the specific steps of `Use the Browser to Send Wi-Fi Connection Information`_ above to open the main interface of AT Web.
