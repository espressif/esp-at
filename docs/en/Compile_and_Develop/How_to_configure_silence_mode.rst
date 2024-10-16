How to Enable Silence Mode
==========================

:link_to_translation:`zh_CN:[中文]`

Introduction
--------------

.. _silence-mode-usage:

Silence mode is a compilation configuration option for ESP-AT firmware that controls the behavior of AT logs (i.e., logs output from the :term:`AT log port`). It is generally recommended to disable this option. While enabling it may reduce the size of the application firmware, you should **prioritize** the following two methods for reducing the firmware size:

  - In :ref:`python build.py menuconfig <esp-at-project-config>`, navigate to Component config > AT to disable the unnecessary AT features.
  - Refer to the optimization methods outlined in the `Minimizing Binary Size <https://docs.espressif.com/projects/esp-idf/en/latest/{IDF_TARGET_PATH_NAME}/api-guides/performance/size.html#minimizing-binary-size>`_ documentation.

.. list-table:: Advantages and Disadvantages of Enabling and Disabling Silence Mode
   :header-rows: 1
   :widths: 25 25 40

   * - Silence Mode
     - Advantages
     - Disadvantages
   * - Disabled (i.e., set to 0, recommended)
     - Enable logging for some AT functions
     - The size of the application firmware (esp-at.bin) will increase, potentially exceeding the allocated application partition size in the code and resulting in a compilation failure (refer to :ref:`compilation failure log <compilation-failure-log>`).
   * - Enabled (i.e., set to 1, not recommended)
     - Reduced size of the application firmware (esp-at.bin)
     - This mode will remove some AT logs and make the `CONFIG_LOG_DEFAULT_LEVEL <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html?highlight=log%20output#config-log-default-level>`_ configuration ineffective, preventing AT from outputting logs at the corresponding log levels, which will increase the difficulty of debugging AT functions.

.. _compilation-failure-log:

Compilation Failure Log Example:

.. code-block:: none

  $ ./build.py build

  ... (more lines of build ESP-AT)

  Generated /path/to/esp-at/build/esp-at.bin
  [836/838] cd /path/to/esp-at/build/esp-at.bin
  FAILED: esp-idf/esptool_py/CMakeFiles/app_check_size 
  cd /path/to/esp-at/build/esp-idf/esptool_py && [...]/bin/python /path/to/esp-at/esp-idf/components/partition_table/check_sizes.py --offset 0x8000 partition --type app /path/to/esp-at/build/partition_table/partition-table.bin /path/to/esp-at/build/esp-at.bin
  Error: app partition is too small for binary esp-at.bin size 0x135ae0:
    - Part 'ota_0' 0/16 @ 0xd0000 size 0x130000 (overflow 0x5ae0)
  ninja: build stopped: subcommand failed.

Configuration
--------------

- Enable or disable the silence mode configuration:

  - :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>`

    Configure the ``silence mode`` as ``No`` (or ``Yes``) in step 3 Install Environment to disable (or enable) the silence mode.

  .. _silence-mode-usage-1:

  - :doc:`Compile ESP-AT Project on the GitHub Webpage <../Compile_and_Develop/How_to_build_project_with_web_page>`

    Modify the value of ``silence_mode`` in the `esp-at/.github/workflows/build_template_{IDF_TARGET_PATH_NAME}.yml <https://github.com/espressif/esp-at/blob/master/.github/workflows/build_template_{IDF_TARGET_PATH_NAME}.yml>`_ file in step 5.3 Commit changes to enable or disable the silence mode. For example, to enable silence mode, set it to:

    .. only:: esp32

      ::

        - name: Configure prerequisites
          run: |
            # set module information
            silence_mode=1
            # set the silence_sdkconfig.defaults file path
            silence_sdkconfig=module_config/module_<your_module_name>/sdkconfig_silence.defaults
            mkdir build
            echo -e "{\"platform\": \"PLATFORM_{IDF_TARGET_CFG_PREFIX}\", \"module\": \"${{ inputs.module_name }}\", \"silence\": ${silence_mode}}" > build/module_info.json

    .. only:: not esp32

      ::

        - name: Configure prerequisites
          run: |
            # set module information
            silence_mode=1
            mkdir build
            echo -e "{\"platform\": \"PLATFORM_{IDF_TARGET_CFG_PREFIX}\", \"module\": \"${{ inputs.module_name }}\", \"silence\": ${silence_mode}}" > build/module_info.json

- Please flash the compiled AT firmware to the module, and then execute the :ref:`AT+GMR <cmd-GMR>` command to confirm whether the enable or disable of silence mode is successful.

  - If silence mode is enabled, the response of the AT+GMR command should include ``s-``, for example:

    ::

      AT+GMR
      AT version:3.5.0.0-dev(s-88b4ea4...

      ... (more lines of version information)

      OK

  - If silence mode is disabled, the response of the AT+GMR command should **NOT** include ``s-``, for example:

    ::

      AT+GMR
      AT version:3.5.0.0-dev(88b4ea4...

      ... (more lines of version information)

      OK
