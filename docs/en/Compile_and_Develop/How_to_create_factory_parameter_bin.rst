How to Generate Factory Parameter Bin
======================================

{IDF_TARGET_COMPILE_MNAME: default="undefined", esp32="WROOM-32", esp32c2="ESP32C2-4MB", esp32c3="MINI-1"}
{IDF_TARGET_AT_PARAMS_ADDR: default="undefined", esp32="0x30000", esp32c2="0x2B000", esp32c3="0x31000"}
{IDF_TARGET_INDEX: default="undefined", esp32="1", esp32c2="2", esp32c3="3"}

:link_to_translation:`zh_CN:[中文]`

This document explains how to generate a customized ESP-AT factory parameter bin file (factory_MODULE_NAME.bin) for your module. For example, you may want to self-define the country code, RF restriction, or UART pins in your ESP-AT firmware. The two tables below allow you to define such parameters.

- :ref:`factory-param-type-csv`
- :ref:`factory-param-data-csv`

The following describes how to generate a customized factory parameter bin by modifying the two tables.

- :ref:`add-a-customized-module`
- :ref:`add-a-customized-parameter`
- :ref:`modify-factory-parameter-data-on-an-existing-module`

.. _factory-param-type-csv:

factory_param_type.csv
-----------------------

The factory_param_type.csv table lists all the parameters that you can define as well as the offset, type, and size of each parameter. It is located in :component_file:`customized_partitions/raw_data/factory_param/factory_param_type.csv`.

The table below provides some information about each parameter.

.. list-table::
   :header-rows: 1
   :widths: 20 100

   * - param_name
     - Note
   * - description
     - Additional information of the module, which is prompted when you are building the ESP-AT project.
   * - magic_flag
     - The value must be ``0xfcfc``.
   * - version
     - The version of factory parameter management for internal use. The current version is ``3``. It is not recommended to modify it.
   * - reserved1
     - Reserved.
   * - tx_max_power
     - Wi-Fi maximum tx power for {IDF_TARGET_NAME}: [40,84]. See `{IDF_TARGET_NAME} tx power <https://docs.espressif.com/projects/esp-idf/en/release-v4.3/{IDF_TARGET_PATH_NAME}/api-reference/network/esp_wifi.html#_CPPv425esp_wifi_set_max_tx_power6int8_t>`_ setting range for details.
   * - uart_port
     - The UART port that is used to send AT commands and receive AT responses.
   * - start_channel
     - Wi-Fi start channel.
   * - channel_num
     - The total channel number of Wi-Fi.
   * - country_code
     - Country code.
   * - uart_baudrate
     - UART baudrate.
   * - uart_tx_pin
     - UART tx pin.
   * - uart_rx_pin
     - UART rx pin.
   * - uart_cts_pin
     - UART cts pin; it should be set to -1 if not used.
   * - uart_rts_pin
     - UART rts pin; it should be set to -1 if not used.
   * - tx_control_pin
     - For some boards, the tx pin needs to be separated from MCU when power on; it should be set to -1 if not used.
   * - rx_control_pin
     - For some boards, the rx pin needs to be separated from MCU when power on; it should be set to -1 if not used.
   * - reserved2
     - Reserved.
   * - platform
     - The platform (or called chip series) that the current firmware runs on.
   * - module_name
     - The module that the current firmware runs on.

.. _factory-param-data-csv:

factory_param_data.csv
-----------------------

This factory_param_data.csv table holds the values for all the parameters defined in :ref:`factory-param-type-csv`. It covers the modules of {IDF_TARGET_NAME} series. You can customize your factory parameter bin by modifying the values in the table. It is located in :component_file:`customized_partitions/raw_data/factory_param/factory_param_data.csv`.

.. _add-a-customized-module:

Add a Customized Module
-----------------------

This section demonstrates how to add a customized module in factory_param_data.csv and generate the factory parameter bin for it with an example. Assuming that you want to generate the factory parameter bin for an {IDF_TARGET_NAME} module named as ``MY_MODULE``, its country code is JP, Wi-Fi channel is from 1 to 14, and other parameters stay the same with ``{IDF_TARGET_COMPILE_MNAME}`` module of ``PLATFORM_{IDF_TARGET_CFG_PREFIX}``, you can do as follows:

.. contents::
  :local:
  :depth: 1

Modify factory_param_data.csv
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Set all parameter values for ``MY_MODULE`` in the factory_param_data.csv table. 

Firstly, insert a row at the bottom of the table, and then enter the following parameter values:

- param_name: value
- platform: PLATFORM_{IDF_TARGET_CFG_PREFIX}
- module_name: ``MY_MODULE``
- description: ``MY_DESCRIPTION``
- magic_flag: 0xfcfc
- version: 3
- reserved1: 0
- tx_max_power: 78
- uart_port: 1
- start_channel: ``1``
- channel_num: ``14``
- country_code: ``JP``
- uart_baudrate: 115200
- uart_tx_pin: 17
- uart_rx_pin: 16
- uart_cts_pin: 15
- uart_rts_pin: 14
- tx_control_pin: -1
- rx_control_pin: -1

The modified factory_param_data.csv file is as follows.

.. code-block:: none

  platform,module_name,description,magic_flag,version,reserved1,tx_max_power,uart_port,start_channel,channel_num,country_code,uart_baudrate,uart_tx_pin,uart_rx_pin,uart_cts_pin,uart_rts_pin,tx_control_pin,rx_control_pin
  PLATFORM_ESP32,WROOM-32,,0xfcfc,3,0,78,1,1,13,CN,115200,17,16,15,14,-1,-1
  ...
  PLATFORM_{IDF_TARGET_CFG_PREFIX},MY_MODULE,MY_DESCRIPTION,0xfcfc,3,0,78,1,1,14,JP,115200,17,16,15,14,-1,-1

.. _modify-esp-at-module-info-structure:

Modify ``esp_at_module_info`` Structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Add customized module information in the ``esp_at_module_info`` structure in :component_file:`at/src/at_default_config.c`.

The ``esp_at_module_info`` structure provides ``OTA`` upgrade verification ``token``:

.. code-block:: c

    typedef struct {
        char* module_name;
        char* ota_token;
        char* ota_ssl_token;
    } esp_at_module_info_t;

If you do not want to use ``OTA`` features, member 2 ``ota_token`` and member 3 ``ota_ssl_token`` should be set to ``NULL``. Member 1 ``module_name`` must correspond to the field ``module_name`` in the factory_param_data.csv file.

The modified ``esp_at_module_info`` structure is as follows:

.. code-block:: c

    static const esp_at_module_info_t esp_at_module_info[] = {
    #if defined(CONFIG_IDF_TARGET_ESP32)
      ...
    #endif

    #if defined(CONFIG_IDF_TARGET_ESP32C3)
      ...
    #endif

    #if defined(CONFIG_IDF_TARGET_ESP32C2)
      ...
    #endif

    #if defined(CONFIG_IDF_TARGET_{IDF_TARGET_CFG_PREFIX})
      {"MY_MODULE",       CONFIG_ESP_AT_OTA_TOKEN_MY_MODULE,      CONFIG_ESP_AT_OTA_SSL_TOKEN_MY_MODULE },     // MY_MODULE
    #endif
    };

Macro ``CONFIG_ESP_AT_OTA_TOKEN_MY_MODULE`` and macro ``CONFIG_ESP_AT_OTA_SSL_TOKEN_MY_MODULE`` are defined in the header file :component_file:`at/private_include/at_ota_token.h`.

.. code-block:: none

    #if defined(CONFIG_IDF_TARGET_{IDF_TARGET_CFG_PREFIX})
    ...
    #define CONFIG_ESP_AT_OTA_TOKEN_MY_MODULE       CONFIG_ESP_AT_OTA_TOKEN_DEFAULT

    ...
    #define CONFIG_ESP_AT_OTA_SSL_TOKEN_MY_MODULE       CONFIG_ESP_AT_OTA_SSL_TOKEN_DEFAULT

Recompile the Project and Select the Customized Module
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

After adding the customized module information, recompile the whole project according to :doc:`How_to_clone_project_and_compile_it` and select the customized module when configuring the project:

::

    Platform name:
    1. PLATFORM_ESP32
    2. PLATFORM_ESP32C3
    2. PLATFORM_ESP32C2
    choose(range[1,3]):{IDF_TARGET_INDEX}

    Module name:
    ...
    x. MY_MODULE (description: MY_DESCRIPTION)
    choose(range[1,x]):x

You can find the factory parameter bin generated in ``esp-at/build/customized_partitions`` folder after the build is completed.

.. _add-a-customized-parameter:

Add a Customized Parameter
--------------------------

This section demonstrates how to add a customized parameter with an example. Assuming that you want to add the parameter ``date`` for ``MY_MODULE`` and set it to ``20210603``, you should do as follows:

.. contents::
  :local:
  :depth: 1

Modify factory_param_type.csv
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Define the parameter ``date`` in the factory_param_type.csv. 

Firstly, insert a row at the end of the table, and then set the name, offset, type, and size of the parameter:

.. list-table::
   :header-rows: 1

   * - param_name
     - offset
     - type
     - size
   * - description
     - -1
     - String
     - 0
   * - ...
     - ...
     - ...
     - ...
   * - ``date``
     - ``88``
     - ``String``
     - ``9``

Modify factory_param_data.csv
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In the factory_param_data.csv, insert a column named as ``date`` to the right of the last column, then set its value to ``20210603`` for ``MY_MODULE``.

The modified CSV table is as follows:

::

    platform,module_name,description,magic_flag,version,reserved1,tx_max_power,uart_port,start_channel,channel_num,country_code,uart_baudrate,uart_tx_pin,uart_rx_pin,uart_cts_pin,uart_rts_pin,tx_control_pin,rx_control_pin,date
    PLATFORM_ESP32,WROOM-32,,0xfcfc,3,0,78,1,1,13,CN,115200,17,16,15,14,-1,-1
    ...
    PLATFORM_{IDF_TARGET_CFG_PREFIX},MY_MODULE,MY_DESCRIPTION,0xfcfc,3,0,78,1,1,14,JP,115200,17,16,15,14,-1,-1,20210603

Process a Customized Parameter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can customize processing functions to process the customized parameter ``date``. This section is just a simple output:

.. code-block:: c

    static void esp_at_factory_parameter_date_init(void)
    {
        const esp_partition_t * partition = esp_at_custom_partition_find(0x40, 0xff, "factory_param");
        char* data = NULL;
        char* str_date = NULL;

        if (!partition) {
            printf("factory_parameter partition missed\r\n");
            return;
        }

        data = (char*)malloc(ESP_AT_FACTORY_PARAMETER_SIZE); // Notes
        assert(data != NULL);
        if(esp_partition_read(partition, 0, data, ESP_AT_FACTORY_PARAMETER_SIZE) != ESP_OK){
            free(data);
            return;
        }

        if ((data[0] != 0xFC) || (data[1] != 0xFC)) { // check magic flag, should be 0xfc 0xfc
            return;
        }

        // sample code
        // users can customize the operation of processing date
        // here is just a simple print out of the date parameter
        str_date = &data[88];   // date field offset address
        printf("date is %s\r\n", str_date);

        free(data);

        return;
    }

Recompile the Project
^^^^^^^^^^^^^^^^^^^^^^

Recompile the whole project according to :doc:`How_to_clone_project_and_compile_it`.

You can find the factory parameter bin generated in ``esp-at/build/customized_partitions`` folder after the build is completed.

.. _modify-factory-parameter-data-on-an-existing-module:

Modify Factory Parameter Data on an Existing Module
---------------------------------------------------

Assuming that you need to modify the factory parameter data of an existing module in factory_param_data.csv, you choose one of the following options:

.. contents::
  :local:
  :depth: 1

Recompile the Whole Project
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Open the factory_param_data.csv and modify the parameters as needed.

Recompile the ESP-AT project according to :doc:`How_to_clone_project_and_compile_it` to generate the factory parameter bin in ``esp-at/build/customized_partitions`` folder.

Only Recompile the Factory Parameter Bin 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Firstly, clone the entire ESP-AT project.

Secondly, navigate to the root directory of ESP-AT project, enter the following command, and replace some parameters:

::

    python tools/factory_param_generate.py --platform PLATFORM --module MODULE --define_file DEFINE_FILE --module_file MODULE_FILE --bin_name BIN_NAME --log_file LOG_FILE

- Replace ``PLATFORM`` with the platform of your module. It must correspond to the ``platform`` in the factory_param_data.csv.

- Replace ``MODULE`` with your module name. It must correspond to the ``module_name`` in the factory_param_data.csv.

- Replace ``DEFINE_FILE`` with the relative path of factory_param_type.csv.

- Replace ``MODULE_FILE`` with the relative path of factory_param_data.csv.

- Replace ``BIN_NAME`` with factory parameter bin file name.

- Replace ``LOG_FILE`` with the file name stored the module name.

Below is the example command for ``MY_MODULE``:

::

    python tools/factory_param_generate.py --platform PLATFORM_{IDF_TARGET_CFG_PREFIX} --module MY_MODULE --define_file components/customized_partitions/raw_data/factory_param/factory_param_type.csv --module_file components/customized_partitions/raw_data/factory_param/factory_param_data.csv --bin_name ./factory_param.bin --log_file ./factory_parameter.log

After the above command is executed, the three files will be generated in the current directory:

- factory_param.bin
- factory_parameter.log
- factory_param_MY_MODULE.bin

Download the new ``factory_param_MY_MODULE.bin`` into flash. ESP-AT provides `esptool.py <https://github.com/espressif/esptool/#readme>`_ to do it. Execute the following command under the root directory of ESP-AT project and replace some parameters:

::

    python esp-idf/components/esptool_py/esptool/esptool.py -p PORT -b BAUD --before default_reset --after hard_reset --chip auto  write_flash --flash_mode dio --flash_size detect --flash_freq 40m ADDRESS FILEDIRECTORY

- Replace ``PORT`` with the port name

- Replace ``BAUD`` with baud rate

- Replace ``ADDRESS`` with the start address in flash. ESP-AT has strict requirements on the ``ADDRESS`` parameter. The address of factory parameter bin varies from firmware to firmware. Please refer to the table below:

  .. only:: esp32

    .. list-table:: factory parameter bin download addresses
      :header-rows: 1

      * - Platform
        - Firmware
        - Address
      * - PLATFORM_ESP32
        - All firmware
        - {IDF_TARGET_AT_PARAMS_ADDR}

  .. only:: esp32c2

    .. list-table:: factory parameter bin download addresses
      :header-rows: 1

      * - Platform
        - Firmware
        - Address
      * - PLATFORM_ESP32C2
        - ESP32C2-4MB Bin
        - {IDF_TARGET_AT_PARAMS_ADDR}

  .. only:: esp32c3

    .. list-table:: factory parameter bin download addresses
      :header-rows: 1

      * - Platform
        - Firmware
        - Address
      * - PLATFORM_ESP32C3
        - MINI-1 Bin
        - {IDF_TARGET_AT_PARAMS_ADDR}
      * - PLATFORM_ESP32C3
        - QCLOUD Bin
        - 0x30000

- Replace ``FILEDIRECTORY`` with the relative path of the factory parameter bin.

Below is the example command to flash the generated factory parameter bin to ``MY_MODULE``:

::

    python esp-idf/components/esptool_py/esptool/esptool.py -p /dev/ttyUSB0 -b 921600 --before default_reset --after hard_reset --chip auto  write_flash --flash_mode dio --flash_size detect --flash_freq 40m {IDF_TARGET_AT_PARAMS_ADDR} ./factory_param_MY_MODULE.bin

Directly Modify Factory Parameter Bin
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Open the factory parameter bin with a binary tool, and directly modify the parameters in the corresponding position according to the parameters offset in factory_param_type.csv.

Download the new factory_param.bin into flash (see :doc:`../Get_Started/Downloading_guide`).
