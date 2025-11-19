
file(GLOB_RECURSE srcs *.c)

set(includes "include")

# Add more required components you need here, separated by spaces
set(require_components at freertos nvs_flash)

idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS ${includes}
    REQUIRES ${require_components})

idf_component_set_property(${COMPONENT_NAME} WHOLE_ARCHIVE TRUE)

target_link_options(${COMPONENT_LIB} INTERFACE "-Wl,--wrap=nvs_set_blob")
target_link_options(${COMPONENT_LIB} INTERFACE "-Wl,--wrap=nvs_get_blob")
target_link_options(${COMPONENT_LIB} INTERFACE "-Wl,--wrap=nvs_set_str")
target_link_options(${COMPONENT_LIB} INTERFACE "-Wl,--wrap=nvs_get_str")
