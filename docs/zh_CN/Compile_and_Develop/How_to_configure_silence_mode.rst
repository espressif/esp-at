如何配置静默模式
=================

:link_to_translation:`en:[English]`

简介
--------

.. _silence-mode-usage:

静默模式 (silence mode) 是 ESP-AT 固件的一种编译配置选项，用于控制 AT 的日志（即 :term:`AT 日志端口` 输出的日志）行为，通常建议禁用。如果启用，将减少应用固件的大小。但建议您 **优先考虑** 下面两种方式减少应用固件的大小：

  - 在 :ref:`python build.py menuconfig <esp-at-project-config>` > Component config > AT 路径下禁用不需要的 AT 功能。
  - 参考 `最小化二进制文件大小 <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/{IDF_TARGET_PATH_NAME}/api-guides/performance/size.html#id1>`_ 文档中的优化方法。

.. list-table:: 启用和禁用静默模式的优点和缺点对比
   :header-rows: 1
   :widths: 25 25 40

   * - 静默模式
     - 优点
     - 缺点
   * - 禁用静默模式（即设置为 0），推荐此配置
     - 开启一些 AT 的日志
     - 应用固件 (esp-at.bin) 的大小会增加，这可能会导致编译 AT 固件时，应用固件大小超过了代码中划分的应用分区大小，从而导致编译失败（:ref:`编译失败日志 <compilation-failure-log>`）。
   * - 启用静默模式（即设置为 1），不推荐此配置
     - 应用固件 (esp-at.bin) 的大小会减小
     - 此模式会删除一些 AT 的日志，并会使 `CONFIG_LOG_DEFAULT_LEVEL <https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-reference/kconfig.html?highlight=log%20output#config-log-default-level>`_ 配置失效，从而无法输出相应等级日志，这会增加您调试 AT 功能的难度。

.. _compilation-failure-log:

编译失败日志示例：

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

配置
--------

- 启用或禁用静默模式配置：

  - :doc:`本地编译 ESP-AT 工程 <../Compile_and_Develop/How_to_clone_project_and_compile_it>`

    在第三步安装环境时配置 ``silence mode`` 为 ``No`` （或 ``Yes``）来禁用（或启用）静默模式。

  .. _silence-mode-usage-1:

  - :doc:`网页编译 ESP-AT 工程 <../Compile_and_Develop/How_to_build_project_with_web_page>`

    在执行第 5.3 步提交更改时，修改 `esp-at/.github/workflows/build_template_{IDF_TARGET_PATH_NAME}.yml <https://github.com/espressif/esp-at/blob/master/.github/workflows/build_template_{IDF_TARGET_PATH_NAME}.yml>`_ 文件中 ``silence_mode`` 的值，来启用或禁用静默模式。例如，要开启静默模式，则设置为：

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

- 请将编译好的 AT 固件烧录到模组上，然后执行 :ref:`AT+GMR <cmd-GMR>` 命令，确认是否成功配置了静默模式的启用或禁用。

  - 如果启用静默模式，AT+GMR 命令响应中应包含 ``s-``，例如：

    ::

      AT+GMR
      AT version:3.5.0.0-dev(s-88b4ea4...

      ... (more lines of version information)

      OK

  - 如果禁用静默模式，AT+GMR 命令响应中不包含 ``s-``，例如：

    ::

      AT+GMR
      AT version:3.5.0.0-dev(88b4ea4...

      ... (more lines of version information)

      OK
