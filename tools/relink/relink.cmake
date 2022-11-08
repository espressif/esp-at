idf_build_get_property(link_depends __LINK_DEPENDS)

set(link_src_file "${CMAKE_BINARY_DIR}/esp-idf/esp_system/ld/sections.ld")
set(customer_sections_file "${CMAKE_BINARY_DIR}/esp-idf/esp_system/ld/customer_sections.ld")
set(library_file "${CMAKE_CURRENT_LIST_DIR}/library.csv")
set(object_file "${CMAKE_CURRENT_LIST_DIR}/object.csv")
set(function_file "${CMAKE_CURRENT_LIST_DIR}/function.csv")

add_custom_command(OUTPUT ${customer_sections_file}
                COMMAND ${python} ${CMAKE_CURRENT_LIST_DIR}/relink.py
                        --input  ${link_src_file}
                        --output ${customer_sections_file}
                        --library ${library_file}
                        --object ${object_file}
                        --function ${function_file}
                        --sdkconfig ${sdkconfig}
                        --objdump "${CMAKE_OBJDUMP}"
                COMMAND ${CMAKE_COMMAND} -E copy
                        ${customer_sections_file}
                        ${link_src_file}
                COMMAND echo /*relink*/ >>
                        ${customer_sections_file}
                DEPENDS "${link_depends}"
                        "${library_file}"
                        "${object_file}"
                        "${function_file}"
                VERBATIM)
add_custom_target(customer_sections DEPENDS ${customer_sections_file})
add_dependencies(${project_elf} customer_sections)
