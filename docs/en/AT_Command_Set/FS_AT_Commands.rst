.. _FS-AT:

FileSystem AT Commands
======================

:link_to_translation:`zh_CN:[中文]`

.. list::

  - :ref:`Introduction <cmd-fs-intro>`
  - :ref:`AT+FS <cmd-FS>`: Filesystem Operations.
  - :ref:`AT+FSMOUNT <cmd-FSMOUNT>`: Mount/Unmount Filesystem.

.. _cmd-fs-intro:

Introduction
------------

.. important::
  The default AT firmware does not support the AT commands listed on this page. If you need {IDF_TARGET_NAME} to support FileSystem commands, you can compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections:

  - Enable ``Component config`` > ``AT`` > ``AT FS command support``

.. _cmd-FS:

:ref:`AT+FS <FS-AT>`: Filesystem Operations
---------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+FS=<type>,<operation>,<filename>,<offset>,<length>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

- **<type>**: Filesystem type, LittleFS is recommended. You can select it in ``Component config`` > ``AT`` > ``AT FS command support`` > ``Filesystem type for AT FS command``.

  - 0: FATFS
  - 1: LittleFS

- **<operation>**:

   -  0: delete file.
   -  1: write file.
   -  2: read file.
   -  3: query the size of the file.
   -  4: list files in a specific directory. Only root directory is currently supported.
   -  5: calculate the MD5 hash of a file for verifying file integrity.
   -  6: query the free space and total space of the filesystem.

-  **<offset>**: apply to writing and reading operations only.
-  **<length>**: data length, applying to writing and reading operations only.

Notes
^^^^^

-  This command will automatically mount the filesystem. After the :ref:`AT+FS <cmd-FS>` filesystem operation is all done, it is strongly recommended to use the :ref:`AT+FSMOUNT=0 <cmd-FSMOUNT>` command to unmount the filesystem to free a large amount of RAM space.
-  Please make sure that you have downloaded at_customize.bin before using this command. For more details, refer to `ESP-IDF Partition Tables <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/partition-tables.html>`_ and :doc:`../Compile_and_Develop/How_to_customize_partitions`.
-  If the length of the read data is greater than the actual file length, only the actual data length of the file will be returned.
-  If the operator is ``write``, wrap return ``>`` after the write command, then you can send the data that you want to write. The length should be parameter ``<length>``.
-  If you need to ensure that the AT firmware is compatible with the legacy FATFS partition after an OTA upgrade, please select ``FatFS (legacy partition, upgrade only, READ HELP FIRST)`` at the following configuration path: ``Component config`` > ``AT`` > ``AT FS command support`` > ``Filesystem type for AT FS command``.

Example
^^^^^^^^

::

    // delete a file.
    AT+FS=1,0,"filename"

    // write 10 bytes to offset 100 of a file.
    AT+FS=1,1,"filename",100,10

    // read 100 bytes from offset 0 of a file.
    AT+FS=1,2,"filename",0,100

    // list all files in the root directory.
    AT+FS=1,4,"."

    // calculate the MD5 hash of a file.
    AT+FS=1,5,"filename"

    // query the free space and total space of the filesystem.
    AT+FS=1,6

.. _cmd-FSMOUNT:

:ref:`AT+FSMOUNT <FS-AT>`: Mount/Unmount Filesystem
------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+FSMOUNT=<mount>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<mount>**:

   -  0: Unmount filesystem
   -  1: Mount filesystem

Notes
^^^^^

-  After the :ref:`AT+FS <cmd-FS>` filesystem operation is all done, it is strongly recommended to use the :ref:`AT+FSMOUNT=0 <cmd-FSMOUNT>` command to unmount the filesystem to free a large amount of RAM space.

Example
^^^^^^^^

::

    // unmount the filesystem manually
    AT+FSMOUNT=0

    // mount the filesystem manually
    AT+FSMOUNT=1
