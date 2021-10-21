.. _User-AT:

User AT Commands
=================

:link_to_translation:`zh_CN:[中文]`

-  :ref:`AT+USERRAM <cmd-USERRAM>`: Operate user's free RAM.
-  :ref:`AT+USEROTA <cmd-USEROTA>`: Upgrade the firmware according to the specified URL.

.. _cmd-USERRAM:

:ref:`AT+USERRAM <User-AT>`: Operate user's free RAM
------------------------------------------------------

Query Command
^^^^^^^^^^^^^

**Function:**

Query the current available user's RAM size.

**Command:**

::

    AT+USERRAM?

**Response:**

::

    +USERRAM:<size>
    OK

Set Command
^^^^^^^^^^^

**Function:**

Operate user's free RAM

**Command:**

::

    AT+USERRAM=<operation>,<size>[,<offset>]

**Response:**

::

    +USERRAM:<length>,<data>    // esp-at returns this response only when the operator is ``read``

    OK

Parameters
^^^^^^^^^^

-  **<operation>**:

   -  0: release user's RAM
   -  1: malloc user's RAM
   -  2: write user's RAM
   -  3: read user's RAM
   -  4: clear user's RAM

-  **<size>**: the size to malloc/read/write
-  **<offset>**: the offset to read/write. Default: 0

Notes
^^^^^

-  Please malloc the RAM size before you perform any other operations.
-  If the operator is ``write``, wrap return ``>`` after the write command, then you can send the data that you want to write. The length should be parameter ``<length>``.
-  If the operator is ``read`` and the length is bigger than 1024, ESP-AT will reply multiple times in the same format, each reply can carry up to 1024 bytes of data, and eventually end up with ``\r\nOK\r\n``.

Example
^^^^^^^^

::

    // malloc 1 KB user's RAM
    AT+USERRAM=1,1024

    // write 500 bytes to RAM (offset: 0)
    AT+USERRAM=2,500

    // read 64 bytes from RAM offset 100
    AT+USERRAM=3,64,100

    // free the user's RAM
    AT+USERRAM=0

.. _cmd-USEROTA:

:ref:`AT+USEROTA <User-AT>`: Upgrade the Firmware According to the Specified URL
-----------------------------------------------------------------------------------------------------

ESP-AT upgrades firmware at runtime by downloading the new firmware from a specific URL.

Set Command
^^^^^^^^^^^

**Function:**

Upgrade to the firmware version specified by the URL.

**Command:**

::

    AT+USEROTA=<url len>

**Response:**

::

    OK

    >

This response indicates that AT is ready for receiving URL. You should enter the URL, and when the URL length reaches the ``<url len>`` value, the system returns:

::

    Recv <url len> bytes

After AT outputs the above information, the upgrade process starts. If the upgrade process is complete, the system return:

::

    OK

If the parameter is wrong or firmware upgrades fails, the system returns:

::

    ERROR

Parameters
^^^^^^^^^^

- **<url len>**: URL length. Maximum: 8192 bytes.

Note
^^^^^

-  The speed of the upgrade depends on the network status.
-  If the upgrade fails due to unfavorable network conditions, AT will return ``ERROR``. Please wait for some time before retrying.
-  After you upgrade the AT firmware, you are suggested to call the command AT+RESTORE to restore the factory default settings.
-  ``AT+USEROTA`` supports ``HTTP`` and ``HTTPS``.
-  After AT outputs the ``>`` character, the special characters in the URL does not need to be escaped through the escape character, and it does not need to end with a new line(CR-LF).
-  When the URL is ``HTTPS``, SSL verification is not recommended. If SSL verification is required, you need to generate your own PKI files and download them into the corresponding partition, and then load the certificates in the code implemented by the ``AT+USEROTA`` command. Please refer to :doc:`../Compile_and_Develop/how_to_generate_pki_files` for PKI files. For ``AT+USEROTA`` command, ESP-AT project provides an example of `USEROTA <https://github.com/espressif/esp-at/blob/master/components/at/src/at_user_cmd.c>`_.

Example
^^^^^^^^

::

    AT+USEROTA=36

    OK

    >
    Recv 36 bytes

    OK
