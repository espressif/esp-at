How to implement OTA update
===========================

The following steps guide the users in creating a device on iot.espressif.cn and updating the OTA BIN on it.

1. Open the website http://iot.espressif.cn. If using SSL OTA, it should be https://iot.espressif.cn.


   .. figure:: ../../_static/OTA-1.png
    :align: center
    :alt: Open iot.espressif.cn website
    :figclass: align-center

    Open iot.espressif.cn website

2. Click "Join" in the upper right corner of the webpage, and enter your name, email address, and password.

   .. figure:: ../../_static/OTA-2.png
    :align: center
    :alt: Join iot.espressif.cn website
    :figclass: align-center

    Join iot.espressif.cn website

3. Click on "Device" in the upper right corner of the webpage, and click on "Create" to create a device.

   .. figure:: ../../_static/OTA-3.png
    :align: center
    :alt: Click on "Device"
    :figclass: align-center

    Click on "Device"

   .. figure:: ../../_static/OTA-4.png
    :align: center
    :alt: Click on "Create" 
    :figclass: align-center

    Click on "Create" 

4. A key is generated when the device is successfully created, as the figure below shows.

   .. figure:: ../../_static/OTA-5.png
    :align: center
    :alt: A key has been generated
    :figclass: align-center

    A key has been generated

5. Use the key to compile your own OTA BIN. The process of configuring the AT OTA token key is as follows:

   .. figure:: ../../_static/OTA-6.png
    :align: center
    :alt: Configuring the AT OTA token key - Step 1
    :figclass: align-center

    Configuring the AT OTA token key - Step 1

   .. figure:: ../../_static/OTA-7.png
    :align: center
    :alt: Configuring the AT OTA token key - Step 2 and 3
    :figclass: align-center

    Configuring the AT OTA token key - Step 2 and 3

   .. note::

      If using SSL OTA, the option "OTA based upon ssl" should be selected.

6. Click on "Product" to enter the webpage, as shown below. Click on the device created. Enter version and corename under "ROM Deploy". Rename the BIN compiled in Step 5 as "ota.bin" and save the configuration.

   .. figure:: ../../_static/OTA-8.png
    :align: center
    :alt: Enter version and corename
    :figclass: align-center

    Enter version and corename

7. Click on the ota.bin to save it as the current version.

   .. figure:: ../../_static/OTA-9.png
    :align: center
    :alt: Save the current version of ota.bin
    :figclass: align-center

    Save the current version of ota.bin

8. Run the command AT+CIUPDATE on the ESP device. If the network is connected, OTA update will be done.

