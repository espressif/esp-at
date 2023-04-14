AT Response Message Format Control Examples
===========================================

:link_to_translation:`zh_CN:[中文]`

.. contents::
   :local:
   :depth: 1

.. _example-sysmfgfilter:

Enable the :term:`system message` filter to download files in HTTP passthough mode
--------------------------------------------------------------------------------------

This example describes how to download an image file in PNG format. The image link is https://www.espressif.com/sites/default/files/home/hardware.png.

#. Restore your device to factory settings.

   Command:

   .. code-block:: none

     AT+RESTORE

   Response:
  
   .. code-block:: none

     OK

#. Set the Wi-Fi mode to station.

   Command:

   .. code-block:: none

     AT+CWMODE=1

   Response:
  
   .. code-block:: none

     OK

#. Connect to the router.

   Command:

   .. code-block:: none

     AT+CWJAP="espressif","1234567890"

   Response:

   .. code-block:: none

     WIFI CONNECTED
     WIFI GOT IP

     OK

   Note:

   - The SSID and password you entered may be different from those in the above command. Please replace the SSID and password with those of your router settings.

#. Configure the first system message filter to filter out the header and tail from each HTTP data.

   Command:

   .. code-block:: none

     AT+SYSMSGFILTERCFG=1,18,3

   Response:

   .. code-block:: none

     OK

     >

   Enter ``^+HTTPCGET:[0-9]*,`` (18 bytes in total) and ``\r\n$`` (3 bytes in total, the ``\r\n`` character corresponds to carriage return and line feed character, i.e. 0D 0A). Response:

   .. code-block:: none

     OK

   Note:

   - ``^+HTTPCGET:[0-9]*,`` is the regular expression for headers. It means the string starts with ``+HTTPCGET:``, followed by a string of numbers, and ends with a comma.
   - ``\r\n$`` is the regular expression of tail. It means the string ends with ``\r\n``.

#. Configure the second system message filter to filter out the ``OK`` system message returned after the picture is downloaded.

   Command:

   .. code-block:: none

     AT+SYSMSGFILTERCFG=1,0,7

   Response:

   .. code-block:: none

     OK

     >

   At this time, enter ``\r\nOK\r\n$`` (7 bytes in total, where ``\r\n`` corresponds to line feed and carriage return in ASCII code, i.e. 0D 0A). Response:

   .. code-block:: none

     OK

   Note:

   - ``\r\nOK\r\n$`` is the regular expression for tails. It means the string ends with ``\r\nOK\r\n``.

#. Enable system message filter

   Command:

   .. code-block:: none

     AT+SYSMSGFILTER=1

   Response:

   .. code-block:: none

     OK

   Note:

   - The filters set above will take effect only after system message filtering is enabled.

#. Turn off echo

   Command:

   .. code-block:: none

     ATE0

   Response:

   .. code-block:: none

     OK

#. Download the picture

   Set the sending and receiving buffer size to 2048 bytes, and set the network timeout to 5000 ms. Note that the network timeout is not the command timeout. The 5-second network timeout means closing the network connection if no data is received from the server side for 5 consecutive seconds. In a poor network environment where the server-side data is received every second, the network connection will not be closed, resulting in a long command timeout.

   Command:

   .. code-block:: none

     AT+HTTPCGET="https://www.espressif.com/sites/default/files/home/hardware.png",2048,2048,5000

   Response:

   .. code-block:: none

     // Here, the MCU transparently receives the entire picture resource from https://www.espressif.com/sites/default/files/home/hardware.png.

   Note:

   - If the picture download fails, AT will still send a ``\r\nERROR\r\n`` (9 bytes in total) system message to MCU.

#. Clear the filter

   Command:

   .. code-block:: none

     AT+SYSMSGFILTERCFG=0

   Response:

   .. code-block:: none

     OK

#. Disable the system message filter

   Command:

   .. code-block:: none

     AT+SYSMSGFILTER=0

   Response:

   .. code-block:: none

     OK

#. Switch echo on

   Command:

   .. code-block:: none

     ATE1

   Response:

   .. code-block:: none

     OK
