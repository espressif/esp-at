.. include:: ../../inline_substitutions

AT Network Provisioning Examples
=================================

:link_to_translation:`zh_CN:[中文]`

This document mainly introduces the following network provisioning methods supported by ESP-AT:

.. list::

  :not esp32s2: * :ref:`BluFi Provisioning <example-at_blufi>`
  * :ref:`SmartConfig Provisioning <example-at_smartconfig>`
  * :ref:`SoftAP Provisioning (WEB Provisioning) <example-at_softap>`
  * :ref:`WPS Provisioning <example-at_wps>`

.. only:: esp32 or esp32c3 or esp32c6

  .. list-table:: Summary of Key Parameters for Provisioning Methods
     :header-rows: 1
     :widths: 45 35 35 35 35
  
     * - Provisioning Method
       - BluFi Provisioning
       - SmartConfig Provisioning
       - SoftAP Provisioning
       - WPS Provisioning
     * - Default Firmware Support
       - |icon-green-check|
       - |icon-green-check|
       - |icon-red-cross| (:ref:`Note 1 <np-note>`)
       - |icon-green-check|
     * - Requires BLE
       - |icon-green-check|
       - |icon-red-cross|
       - |icon-red-cross|
       - |icon-red-cross|
     * - Requires Additional Mobile App
       - |icon-green-check|
       - |icon-green-check| (:ref:`Note 2 <np-note>`)
       - |icon-green-check|
       - |icon-red-cross|
     * - Provisioning Success Rate
       - High
       - Moderate
       - High
       - High
     * - Operation Complexity
       - Simple
       - Simple
       - Complex
       - Complex
     * - Recommendation Level
       - Recommended
       - Moderate
       - Moderate
       - Moderate

.. only:: esp32c2

  .. list-table:: Summary of Key Parameters for Provisioning Methods
     :header-rows: 1
     :widths: 45 35 35 35 35
  
     * - Provisioning Method
       - BluFi Provisioning
       - SmartConfig Provisioning
       - SoftAP Provisioning
       - WPS Provisioning
     * - Default Firmware Support
       - |icon-green-check| (:ref:`Note 3 <np-note>`)
       - |icon-green-check|
       - |icon-red-cross| (:ref:`Note 1 <np-note>`)
       - |icon-green-check|
     * - Requires BLE
       - |icon-green-check|
       - |icon-red-cross|
       - |icon-red-cross|
       - |icon-red-cross|
     * - Requires Additional Mobile App
       - |icon-green-check|
       - |icon-green-check| (:ref:`Note 2 <np-note>`)
       - |icon-green-check|
       - |icon-red-cross|
     * - Provisioning Success Rate
       - High
       - Moderate
       - High
       - High
     * - Operation Complexity
       - Simple
       - Simple
       - Complex
       - Complex
     * - Recommendation Level
       - Recommended
       - Moderate
       - Moderate
       - Moderate

.. only:: esp32s2

  .. list-table:: Summary of Key Parameters for Provisioning Methods
     :header-rows: 1
     :widths: 45 35 35 35

     * - Provisioning Method
       - SmartConfig Provisioning
       - SoftAP Provisioning
       - WPS Provisioning
     * - Default Firmware Support
       - |icon-green-check|
       - |icon-red-cross| (:ref:`Note 1 <np-note>`)
       - |icon-green-check|
     * - Requires BLE
       - |icon-red-cross|
       - |icon-red-cross|
       - |icon-red-cross|
     * - Requires Additional Mobile App
       - |icon-green-check| (:ref:`Note 2 <np-note>`)
       - |icon-green-check|
       - |icon-red-cross|
     * - Provisioning Success Rate
       - Moderate
       - High
       - High
     * - Operation Complexity
       - Simple
       - Complex
       - Complex
     * - Recommendation Level
       - Recommended
       - Moderate
       - Moderate

.. _np-note:

  - Note 1: If you want to enable SoftAP provisioning in the AT firmware, please refer to: :doc:`../AT_Command_Set/Web_server_AT_Commands`.
  - Note 2: The AirKiss provisioning in SmartConfig requires the WeChat app, which may be unavailable to users outside China.

  .. only:: esp32c2

    - Note 3: The default ESP32C2-4MB AT firmware supports BluFi functionality, while the ESP32C2-2MB AT firmware does not. For more details, please refer to :ref:`Bluetooth LE AT Command Introduction <cmd-ble-intro>`.

.. only:: not esp32s2

  .. _example-at_blufi:

  BluFi Provisioning
  -------------------

  BluFi is a Wi-Fi configuration feature based on the Bluetooth channel. For more information, please refer to the `BluFi documentation <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/ble/blufi.html>`_.

  #. Install the EspBluFi app on your mobile device.

     - Android: `Download EspBluFi for Android <https://github.com/EspressifApp/EspBlufiForAndroid/releases>`_ (`Android Source Code <https://github.com/EspressifApp/EspBlufiForAndroid>`_)
     - iOS: `Download EspBluFi for iOS <https://apps.apple.com/cn/app/EspBlufi/id1450614082>`_ (`iOS Source Code <https://github.com/EspressifApp/EspBlufiForiOS>`_)

  #. Set the BluFi name on the {IDF_TARGET_NAME} device.

     Command:

     .. code-block:: none

       AT+BLUFINAME="blufi_test"

     Response:

     .. code-block:: none

       OK

  #. Enable BluFi on the {IDF_TARGET_NAME} device.

     Command:

     .. code-block:: none

       AT+BLUFI=1

     Response:

     .. code-block:: none

       OK

  #. Establish BluFi connection on your mobile device and perform Wi-Fi provisioning.

     4.1 BluFi Connection
    
       Enable the system's Bluetooth and GPS location services on your mobile device, then launch the EspBluFi app. Find the device named ``blufi_test`` and click it to enter. Next, click ``Connect`` to establish a connection. At this point, the ESP device should log something similar to ``+BLUFICONN``, and the app will display the below information, indicating that the BluFi connection has been successfully established.

       .. code-block:: none

         Connected <mac>

         Discover service and characteristics success

         Set notification enable complete

         Set mtu complete, mtu=...

     4.2 BluFi Wi-Fi Provisioning

       After the connection is established, click ``Configure`` button in the EspBluFi app. The page will navigate to ``Configure`` page. Enter the Wi-Fi SSID and password, then click ``OK`` button to start provisioning. After a short wait, the app will display the below information.

       .. code-block:: none
  
         Post configure params complete
  
         Receive device status response:
         OpMode: Station
         Station connect Wi-Fi now
         Station connect Wi-Fi bssid: <mac>
         Station connect Wi-Fi ssid: <ssid>

     4.3 Successful Provisioning

       Please wait for a moment, and the ESP device will output the following logs. At this point, the ESP device's network configuration is successfully completed.

       .. code-block:: none

         WIFI CONNECTED
         WIFI GOT IP

  #. (Optional) Send custom user data via BluFi on the {IDF_TARGET_NAME} device.

     Command:

     .. code-block:: none

       AT+BLUFISEND=4

     Response:

     .. code-block:: none

       >

     Input 4 bytes of data, for example, if the input data is ``test``, then AT will output the following information.

     .. code-block:: none

       OK

     At this point, the app will display the below information:

     .. code-block:: none

       Receive custom data:
       test

  #. Disable BluFi on the {IDF_TARGET_NAME} device.

     Command:

     .. code-block:: none

       AT+BLUFI=0

     Response:

     .. code-block:: none

       +BLUFIDISCONN

       OK

     At this point, the app will display the below information:

     .. code-block:: none

       Disconnect <mac>, status=19

.. _example-at_smartconfig:

SmartConfig Provisioning
------------------------

AT SmartConfig provisioning offers four types: ESP-TOUCH, AirKiss, ESP-TOUCH+AirKiss, and ESP-TOUCH v2. Below are examples of ESP-TOUCH v2 provisioning.

ESP-TOUCH v2 Provisioning Example
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Install the EspTouch app on your mobile device.

   - Android: `Download EspTouch for Android <https://github.com/EspressifApp/EsptouchForAndroid/releases>`_ (`Source Code for Android <https://github.com/EspressifApp/EsptouchForAndroid>`_)
   - iOS: `Download EspTouch for iOS <https://apps.apple.com/in/app/espressif-esptouch/id1071176700>`_ (`Source Code for iOS <https://github.com/EspressifApp/EsptouchForIOS>`_)

#. Set the ESP device to Station mode.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:

   .. code-block:: none

     OK

#. Enable SmartConfig on the {IDF_TARGET_NAME} device.

   Command:

   .. code-block:: none

     AT+CWSTARTSMART=4,0,"1234567890123456"

   Response:

   .. code-block:: none

     OK

#. Perform Wi-Fi provisioning on your mobile device.

   Please follow the steps below: enable Wi-Fi on your mobile device and connect to the target network (e.g., SSID: test, password: 1234567890). Then, launch the EspTouch application and click ``EspTouch V2`` button. On the redirected page, enter the password for the connected Wi-Fi, the number of devices for provisioning, and the AES key, as shown below:

   .. code-block:: none

     Wi-Fi Password: 1234567890
     Device Count for Provisioning: 1
     AES Key: 1234567890123456

#. Confirm the successful provisioning on the ESP device.

   At this point, the ESP device should output the below information:

   .. code-block:: none

     smartconfig type:ESPTOUCH_V2
     Smart get wifi info
     ssid:test
     password:1234567890
     WIFI CONNECTED
     WIFI GOT IP
     smartconfig connected wifi

   At this point, the ESP device's provisioning has been successfully completed.

#. Disable SmartConfig on the {IDF_TARGET_NAME} device.

   Command:

   .. code-block:: none

     AT+CWSTOPSMART

   Response:

   .. code-block:: none

     OK

.. AirKiss Provisioning Example
.. ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. 
.. #. Open the WeChat application on your mobile device, search for and follow the "Espressif Technology" official account.
.. 
.. #. Set the ESP device to Station mode.
.. 
..    Command:
.. 
..    .. code-block:: none
.. 
..      AT+CWMODE=1
.. 
..    Response:
.. 
..    .. code-block:: none
.. 
..      OK
.. 
.. #. Enable SmartConfig on the {IDF_TARGET_NAME} device.
.. 
..    Command:
.. 
..    .. code-block:: none
.. 
..      AT+CWSTARTSMART=2
.. 
..    Response:
.. 
..    .. code-block:: none
.. 
..      OK
.. 
.. #. Perform Wi-Fi provisioning on your mobile device.
.. 
..    Connect your mobile device to the Wi-Fi network that requires provisioning (e.g., SSID: test, password: 1234567890), then open WeChat, and go to the "Espressif Systems" official account. Click ``Product Resources``, find the AirKiss device, and enter its page. On the redirected page, enter the Wi-Fi password ``1234567890``. After a moment, the mobile will display the log message ``Configuration successful......``.
.. 
..    At this point, the ESP device should output the below information:
.. 
..    .. code-block:: none
.. 
..      smartconfig type:AIRKISS
..      Smart get wifi info
..      ssid:test
..      password:1234567890
..      WIFI CONNECTED
..      WIFI GOT IP
..      smartconfig connected wifi
.. 
..    At this point, the ESP device's provisioning has been successfully completed.
.. 
.. #. Disable SmartConfig on the {IDF_TARGET_NAME} device.
.. 
..    Command:
.. 
..    .. code-block:: none
.. 
..      AT+CWSTOPSMART
.. 
..    Response:
.. 
..    .. code-block:: none
.. 
..      OK

.. _example-at_softap:

SoftAP Provisioning
--------------------

AT SoftAP provisioning refers to WEB provisioning. For more details, please refer to :doc:`Web Server AT Examples <../AT_Command_Examples/Web_server_AT_Examples>`.

.. _example-at_wps:

WPS Provisioning
--------------------

#. Prepare a router that supports WPS provisioning (e.g., Wi-Fi SSID: test).

#. Set the ESP device to Station mode.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:

   .. code-block:: none

     OK

#. Enable WPS provisioning on the {IDF_TARGET_NAME} device.

   Command:

   .. code-block:: none

     AT+WPS=1

   Response:

   .. code-block:: none

     OK

   Enable the WPS provisioning according to the router's user manual, and wait for a moment. The ESP device should output the below information, indicating that the provisioning has been successfully completed.

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

#. Query the Wi-Fi information of the connected {IDF_TARGET_NAME} device.

   Command:

   .. code-block:: none

     AT+CWJAP?

   Response:

   .. code-block:: none

     +CWJAP:"test",......
     OK
  
#. Disable WPS provisioning on the {IDF_TARGET_NAME} device.

   Command:

   .. code-block:: none

     AT+WPS=0

   Response:

   .. code-block:: none

     OK
