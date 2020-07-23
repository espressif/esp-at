How to add user-defined AT commands
===================================

AT firmware is based on the Espressif IoT Development Framework (ESP-IDF). Espressif Systems' AT commands are provided in `libat_core.a`, which is included in the AT BIN firmware.   
Examples of implementing user-defined AT commands are provided in `/esp-at/main/interface/uart/at_uart_task.c`.  

 * The total length of an AT command cannot be longer than 256 Bytes.  
 * Use only English letters or an underscore (_), when naming user-defined AT commands. 
 * The AT command name must NOT contain characters or numbers.
 * The structure, `at_cmd_struct`, is used to define four types of a command. 

