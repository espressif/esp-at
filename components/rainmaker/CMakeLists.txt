if (DEFINED ENV{AT_RAINMAKER_SUPPORT})
    set(require_components nvs_flash at esp_rainmaker esp_schedule json_generator json_parser rmaker_common wifi_provisioning bootloader_support_plus)

    idf_component_register(SRCS at_rainmaker.c
                        PRIV_REQUIRES ${require_components})

    target_link_libraries(${COMPONENT_LIB} INTERFACE "-u esp_at_rainmaker_cmd_regist")
endif()
