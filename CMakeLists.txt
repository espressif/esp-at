set(include_dirs port wolfssl)
set(lib_path ${CMAKE_CURRENT_SOURCE_DIR}/wolfssl/lib)

idf_component_register(INCLUDE_DIRS "${include_dirs}"
                    REQUIRES lwip
                    )

target_compile_options(${COMPONENT_LIB} INTERFACE -Wno-cpp)
target_compile_definitions(${COMPONENT_LIB} INTERFACE WOLFSSL_USER_SETTINGS)
target_link_libraries(${COMPONENT_LIB} INTERFACE "-L ${lib_path}")

if(CONFIG_ESP_DEBUG_WOLFSSL)
    if(CONFIG_ESP_WOLFSSL_SMALL_CERT_VERIFY)
        # use debug library with WOLFSSL_SMALL_CERT_VERIFY enabled
        target_link_libraries(${COMPONENT_LIB} INTERFACE wolfssl_small_cert_verify_debug)
    else()
        # use debug library without WOLFSSL_SMALL_CERT_VERIFY enabled
        target_link_libraries(${COMPONENT_LIB} INTERFACE wolfssl_debug)
    endif()
else()
    if(CONFIG_ESP_WOLFSSL_SMALL_CERT_VERIFY)
        # use library with WOLFSSL_SMALL_CERT_VERIFY enabled (without debug)
        target_link_libraries(${COMPONENT_LIB} INTERFACE wolfssl_small_cert_verify)
    else()
        # use library without WOLFSSL_SMALL_CERT_VERIFY enabled (without debug)
        # This Library is USED BY DEFAULT (no wolfssl options enabled)
        target_link_libraries(${COMPONENT_LIB} INTERFACE wolfssl)
    endif()
endif()
