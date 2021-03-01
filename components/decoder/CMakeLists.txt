
set(require_components ${IDF_TARGET} mqtt mdns esp_http_client json freertos spiffs
    bootloader_support app_update openssl wpa_supplicant spi_flash)

if ("${IDF_TARGET}" STREQUAL "esp32")
    list(APPEND require_components bt fatfs)

idf_component_register (
    REQUIRES ${require_components})

set(LIB_NAME _mp3)

string(SUBSTRING "$ENV{ESP_AT_PROJECT_PLATFORM}" 9 31 PLATFORM_NAME) # remove PLATFORM_
string(CONCAT FULL_NAME ${PLATFORM_NAME} ${LIB_NAME})
string(TOLOWER ${FULL_NAME} LIBS)

add_library(${LIBS} STATIC IMPORTED)
set_property(TARGET ${LIBS} PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/lib${LIBS}.a)
target_link_libraries(${COMPONENT_LIB} INTERFACE ${LIBS})
set_property(TARGET ${LIBS} APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${COMPONENT_LIB})
endif()
