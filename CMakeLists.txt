set(include_dirs port wolfssl)
set(lib_path ${CMAKE_CURRENT_SOURCE_DIR}/lib)

idf_component_register(INCLUDE_DIRS "${include_dirs}"
                    REQUIRES lwip
                    )

target_compile_definitions(${COMPONENT_LIB} INTERFACE WOLFSSL_USER_SETTINGS)
target_link_libraries(${COMPONENT_LIB} INTERFACE "-L ${lib_path}")
target_link_libraries(${COMPONENT_LIB} INTERFACE wolfssl)
