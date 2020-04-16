if("${IDF_TARGET}" STREQUAL "esp32s2")
idf_component_register(INCLUDE_DIRS "${include_dirs}"
                    )
else()
set(include_dirs port wolfssl)
set(lib_path ${CMAKE_CURRENT_SOURCE_DIR}/wolfssl/lib/${IDF_TARGET})

idf_component_register(INCLUDE_DIRS "${include_dirs}"
                    REQUIRES lwip
                    REQUIRED_IDF_TARGETS esp32 esp8266)

target_compile_options(${COMPONENT_LIB} INTERFACE -Wno-cpp)
target_compile_definitions(${COMPONENT_LIB} INTERFACE WOLFSSL_USER_SETTINGS)
target_link_libraries(${COMPONENT_LIB} INTERFACE "-L ${lib_path}")

if(CONFIG_ESP_DEBUG_WOLFSSL)
    target_link_libraries(${COMPONENT_LIB} INTERFACE wolfssl_nano_debug)
else()
    target_link_libraries(${COMPONENT_LIB} INTERFACE wolfssl_nano)
endif()
endif()
