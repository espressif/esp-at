How to add user-defined AT commands
===================================

AT firmware is based on the Espressif IoT Development Framework (ESP-IDF). Espressif Systems' AT commands are provided in ``libat_core.a``, which is included in the :doc:`AT BIN firmware <../AT_Binary_Lists/index>`.

Examples of implementing user-defined AT commands are provided in :at_file:`main/interface/uart/at_uart_task.c`.

-  The total length of an AT command cannot be longer than 256 bytes.
-  Only alphabetic characters (A~Z, a~z), numeric characters (0~9), and some other characters (``!``, ``%``, ``-``, ``.``, ``/``, ``:``, ``_``) are supported when naming user-defined AT commands.
-  The structure, :cpp:type:`esp_at_cmd_struct`, is used to define :ref:`four types <at-command-types>` of a command.
