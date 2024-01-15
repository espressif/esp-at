register_component()

idf_build_get_property(build_dir BUILD_DIR)
idf_build_get_property(project_dir PROJECT_DIR)

set(flash_customized_partition_args ${build_dir}/flash_customized_partition_args)
set(module_info ${build_dir}/module_info)

if (CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_SUPPORT)
    # generate at_customize.bin and add it to flash target
    STRING(REGEX REPLACE ".*/(.*)\.csv" "\\1" customize_bin ${CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_FILE})
    set(customize_bin_full_name ${customize_bin}.bin)
    execute_process(
        COMMAND ${PYTHON} ${IDF_PATH}/components/partition_table/gen_esp32part.py
            ${project_dir}/${CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_FILE} ${build_dir}/${customize_bin_full_name}
    )
    esptool_py_custom_target(${customize_bin} ${customize_bin} "${customize_bin}")
    esptool_py_flash_target_image(flash "${customize_bin}" "${CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_OFFSET}" "${build_dir}/${customize_bin_full_name}")

    # parse sub-item of at_customize.csv, get necessary item, and add it to flash target
    execute_process(
        COMMAND ${PYTHON} ${COMPONENT_DIR}/at_customized_target_generate.py
            --sdkconfig_file ${project_dir}/sdkconfig
            --dependency_file ${COMPONENT_DIR}/at_customized_config_dependency.txt
            --partition_file ${project_dir}/${CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_FILE}
            --output_dir ${build_dir}/${COMPONENT_NAME}
            --flash_args_file ${flash_customized_partition_args}
    )
    file(STRINGS ${flash_customized_partition_args} customized_partition_args)
    foreach(partition_args ${customized_partition_args})
        string(STRIP ${partition_args} args_value)
        string(REPLACE " " ";" args_lists ${args_value})
        list(GET args_lists 0 offset)
        list(GET args_lists 1 file_path)
        STRING(REGEX REPLACE ".*[/\\](.*)\.bin" "\\1" file_name ${file_path})
        esptool_py_custom_target(${file_name} ${file_name} "${file_name}")
        esptool_py_flash_target_image(flash "${file_name}" "${offset}" "${file_path}")
    endforeach(partition_args)

    file(MAKE_DIRECTORY ${build_dir}/include)
    file(COPY ${build_dir}/config/sdkconfig.h DESTINATION ${build_dir}/include/sdkconfig.h)

    # generate mfg_nvs.bin according the dependency
    add_custom_target(customized_bin ALL)
    add_custom_command(TARGET customized_bin
    POST_BUILD
    COMMAND ${PYTHON} ${COMPONENT_DIR}/at_customized_target_bin_generate.py
                --project_dir ${project_dir}
                --tools_dir ${COMPONENT_DIR}/generation_tools
                --output_dir ${build_dir}/${COMPONENT_NAME}
                --flash_args_file ${flash_customized_partition_args}
    COMMENT "Create customized_bin..."
    )
    add_dependencies(gen_project_binary customized_bin)

endif()
