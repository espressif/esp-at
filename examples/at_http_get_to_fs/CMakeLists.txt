
file(GLOB_RECURSE srcs *.c)

set(includes "include")

# Add more required components you need here, separated by spaces
set(require_components at freertos esp_http_client)

idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS ${includes}
    REQUIRES ${require_components})

idf_component_set_property(${COMPONENT_NAME} WHOLE_ARCHIVE TRUE)
