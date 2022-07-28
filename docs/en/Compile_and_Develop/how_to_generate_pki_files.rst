How to Generate :term:`PKI` Files
=============================================

:link_to_translation:`zh_CN:[中文]`

ESP-AT provides python script ``AtPKI.py`` to convert SSL server certificates, SSL client certificates, MQTT certificates, and WPA2 certificates files (including ``CA``, ``cert``, and ``private key`` files) into bin files.

.. contents::
   :local:
   :depth: 1

Certificate Bin Files Format
-------------------------------------

In addition to converting the certificate file into a bin file, the script ``AtPKI.py`` adds some additional information to the bin file.

When converting a single certificate file, the script will add 12 bytes in little-endian format to the header and 4 bytes aligned at the end.

   .. list-table::
      :header-rows: 1
      :widths: 40 80

      * - Field
        - Description
      * - magic code (2 bytes)
        - 0xF1 0xF1
      * - list size (2 bytes)
        - the number of certificate files
      * - length (4 bytes)
        - remaining file size = total file size - magic code size - list size - length size
      * - type (1 bytes)
        - | ca: 0x01
          | certificate: 0x02
          | key: 0x03
      * - ID (1 bytes)
        - used to match certificate files
      * - content len (2 bytes)
        - the size of the certificate file converted to the bin file

Taking the ``client_cert.bin`` generated in the `Generate Certificate Bin Files`_ section as an example, the 12 bytes in little-endian format at the beginning of the file are:

   .. list-table::
      :header-rows: 1
      :widths: 40 80

      * - Field
        - Description
      * - magic code (2 bytes)
        - 0xF1 0xF1
      * - list size (2 bytes)
        - 0x02 0x00
      * - length (4 bytes)
        - 0x20 0x09 0x00 0x00
      * - type (1 bytes)
        - 0x02
      * - ID (1 bytes)
        - 0x00
      * - content len (2 bytes)
        - 0x8C 0x04

When converting multiple certificate files, the script ``AtPKI.py`` inserts 4 bytes at the beginning of every file except the first one. The 4 bytes in little-endian format at the beginning of the file are:

   .. list-table::
      :header-rows: 1
      :widths: 40 80

      * - Field
        - Description
      * - type (1 bytes)
        - | ca: 0x01
          | certificate: 0x02
          | key: 0x03
      * - ID (1 bytes)
        - used to match certificate files
      * - content len (2 bytes)
        - the size of the certificate file converted to the bin file

Taking the ``client_cert.bin`` generated in the `Generate Certificate Bin Files`_ section as an example, the 4 bytes in little-endian format at the beginning of the file are:

   .. list-table::
      :header-rows: 1
      :widths: 40 80

      * - Field
        - Description
      * - type (1 bytes)
        - certificate: 0x02
      * - ID (1 bytes)
        - 0x01
      * - content len (2 bytes)
        - 0x8C 0x04

Generate Certificate Bin Files
-------------------------------------

Select one of the following ways to generate certificate bin files.

.. contents::
   :local:
   :depth: 1

Script Generation
^^^^^^^^^^^^^^^^^^^^

The path of ``AtPKI.py`` is :project_file:`tools/AtPKI.py`. You can get help information of the script through the ``-h`` option. You can also generate bin files directly through the following commands.

.. code-block:: none

    python <SCRIPT_PATH> generate_bin [-b OUTPUT_BIN_NAME] <PKI_LIST> <source_file>

- ``SCRIPT_PATH``: the path the script. If you're in "tools" folder of ESP-AT project, ``SCRIPT_PATH`` is ``AtPKI.py``.
- ``OUTPUT_BIN_NAME``: the target file which you want to save the generated bin (absolute address or relative address of the target file); if ``-b OUTPUT_BIN_NAME`` is omitted, it will generate ``PKI.bin`` in the current directory.
- ``PKI_LIST``: must be equal to one of ``ca``, ``cert``, ``key``.
- ``source_file``: the certificate source file which you want to convert (absolute address or relative address of the source file).

Taking the SSL client certificate files of ESP-AT as an example, you can execute the following command to generate ``client_cert.bin`` in the tools directory:

.. code-block:: none

    python AtPKI.py generate_bin -b ./client_cert.bin  cert ../components/customized_partitions/raw_data/client_cert/client_cert_00.crt cert ../components/customized_partitions/raw_data/client_cert/client_cert_01.crt

Generation During Compilation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The storage path of certificate files in ESP-AT is :project:`components/customized_partitions/raw_data`.

Taking the SSL client certificate files of ESP-AT as an example. If you want to generate your own SSL client certificates files, you must replace the ``CA`` certificate in the ``client_ca`` directory with your own ``CA`` certificate, the cert certificate in the ``client_cert`` directory with your own ``cert`` certificate, and the ``private key`` in the ``client_ key`` directory with your own ``private key``.

If you have multiple sets of certificate files, please place them in the corresponding directory according to your certificate chain. It is recommended that you end the file name with a number to ensure the parsing order of the certificate files.

After replacement, You can refer to :doc:`How_to_clone_project_and_compile_it` to compile the ESP-AT project.

Download or Update Certificate Bin Files
---------------------------------------------------------------

The script ``AtPKI.py`` is only responsible for converting the certificate files to bin files. You can download bin files to the corresponding flash partition in one of the following ways:

Download with Tools
^^^^^^^^^^^^^^^^^^^

- Windows

  Please download `Flash Download Tools for Windows <https://www.espressif.com/en/support/download/other-tools>`_.

  For more details about the Tools, please see ``readme.pdf`` or the ``doc`` folder in the zip folder.

- Linux or macOS

  Please use `esptool.py <https://github.com/espressif/esptool>`_.

  You can execute the following command in the root directory of ESP-AT to download bin files.

  .. code-block:: none

    esptool.py --chip auto --port PORTNAME --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB ADDRESS FILEDIRECTORY

  Replace ``PORTNAME`` with your port name. Replace ``ADDRESS`` with the the download address. Replace ``FILEDIRECTORY`` with the file directory of the bin.

Update with Commands
^^^^^^^^^^^^^^^^^^^^

- :ref:`AT+SYSFLASH <cmd-SYSFLASH>`

  Taking ``{IDF_TARGET_NAME}`` module as an example, you can execute the following command to upgrade the ``client_cert`` partition. Please refer to :ref:`AT+SYSFLASH <cmd-SYSFLASH>` for more details.

  1. Query user partitions in flash

    Command:

    .. code-block:: none

      AT+SYSFLASH?

    Response:

    .. only:: esp32

      .. code-block:: none

        +SYSFLASH:"ble_data",64,1,0x21000,0x3000
        +SYSFLASH:"server_cert",64,2,0x24000,0x2000
        +SYSFLASH:"server_key",64,3,0x26000,0x2000
        +SYSFLASH:"server_ca",64,4,0x28000,0x2000
        +SYSFLASH:"client_cert",64,5,0x2a000,0x2000
        +SYSFLASH:"client_key",64,6,0x2c000,0x2000
        +SYSFLASH:"client_ca",64,7,0x2e000,0x2000
        +SYSFLASH:"factory_param",64,8,0x30000,0x1000
        +SYSFLASH:"wpa2_cert",64,9,0x31000,0x2000
        +SYSFLASH:"wpa2_key",64,10,0x33000,0x2000
        +SYSFLASH:"wpa2_ca",64,11,0x35000,0x2000
        +SYSFLASH:"mqtt_cert",64,12,0x37000,0x2000
        +SYSFLASH:"mqtt_key",64,13,0x39000,0x2000
        +SYSFLASH:"mqtt_ca",64,14,0x3b000,0x2000
        +SYSFLASH:"fatfs",1,129,0x70000,0x90000

        OK

    .. only:: esp32c2

      .. code-block:: none

          +SYSFLASH:"server_cert",64,1,0x1f000,0x2000
          +SYSFLASH:"server_key",64,2,0x21000,0x2000
          +SYSFLASH:"server_ca",64,3,0x23000,0x2000
          +SYSFLASH:"client_cert",64,4,0x25000,0x2000
          +SYSFLASH:"client_key",64,5,0x27000,0x2000
          +SYSFLASH:"client_ca",64,6,0x29000,0x2000
          +SYSFLASH:"factory_param",64,7,0x2b000,0x1000
          +SYSFLASH:"wpa2_cert",64,8,0x2c000,0x2000
          +SYSFLASH:"wpa2_key",64,9,0x2e000,0x2000
          +SYSFLASH:"wpa2_ca",64,10,0x30000,0x2000
          +SYSFLASH:"mqtt_cert",64,11,0x32000,0x2000
          +SYSFLASH:"mqtt_key",64,12,0x34000,0x2000
          +SYSFLASH:"mqtt_ca",64,13,0x36000,0x2000
          +SYSFLASH:"ble_data",64,14,0x38000,0x6000
          +SYSFLASH:"fatfs",1,129,0x47000,0x19000

          OK

    .. only:: esp32c3

      .. code-block:: none

          +SYSFLASH:"ble_data",64,1,0x1f000,0x6000
          +SYSFLASH:"server_cert",64,2,0x25000,0x2000
          +SYSFLASH:"server_key",64,3,0x27000,0x2000
          +SYSFLASH:"server_ca",64,4,0x29000,0x2000
          +SYSFLASH:"client_cert",64,5,0x2b000,0x2000
          +SYSFLASH:"client_key",64,6,0x2d000,0x2000
          +SYSFLASH:"client_ca",64,7,0x2f000,0x2000
          +SYSFLASH:"factory_param",64,8,0x31000,0x1000
          +SYSFLASH:"wpa2_cert",64,9,0x32000,0x2000
          +SYSFLASH:"wpa2_key",64,10,0x34000,0x2000
          +SYSFLASH:"wpa2_ca",64,11,0x36000,0x2000
          +SYSFLASH:"mqtt_cert",64,12,0x38000,0x2000
          +SYSFLASH:"mqtt_key",64,13,0x3a000,0x2000
          +SYSFLASH:"mqtt_ca",64,14,0x3c000,0x2000
          +SYSFLASH:"fatfs",1,129,0x47000,0x19000

          OK

  2. Erase ``client_cert`` sector

    Command:

    .. code-block:: none

      AT+SYSFLASH=0,"client_cert"

    Response:

    .. code-block:: none

      OK

  3. Update ``client_cert`` sector

    Command:

    .. code-block:: none

      AT+SYSFLASH=1,"client_cert",0,2344

    Response:

    .. code-block:: none

      >

    If the ``operator`` is ``write``, wrap return ``>`` after the write command, then you can send the data that you want to write. The length should be parameter ``<length>``. When the write operation is completed, the system will prompt the following information.

    .. code-block:: none

      OK

- :ref:`AT+CIUPDATE <cmd-UPDATE>`

  For example, you can execute the following command to upgrade the ``client_ca`` partition. Please refer to :ref:`AT+CIUPDATE <cmd-UPDATE>` for more details.

  .. Important::
    If you want to update the ``client_ca`` partition in this way, you must implement your own OTA device, please refer to :doc:`How_to_implement_OTA_update`.

  .. code-block:: none

    AT+CIUPDATE=1,"v2.2.0.0","client_ca"

.. note::

  You must ensure that the download address is correct, otherwise the ESP-AT firmware may not work. The simplest way to view the download address is to execute the command **AT+SYSFLASH?**.
