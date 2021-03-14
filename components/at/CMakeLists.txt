
set(require_components ${IDF_TARGET} mqtt mdns esp_http_client json freertos spiffs
    bootloader_support app_update openssl wpa_supplicant spi_flash esp_http_server)

if ("${IDF_TARGET}" STREQUAL "esp32")
    list(APPEND require_components bt fatfs)
elseif ("${IDF_TARGET}" STREQUAL "esp32s2")
    list(APPEND require_components fatfs)
elseif ("${IDF_TARGET}" STREQUAL "esp32c3")
    list(APPEND require_components bt fatfs)
elseif ("${IDF_TARGET}" STREQUAL "esp8266")
    list(APPEND require_components nvs_flash)
endif()

file(GLOB_RECURSE srcs src/*.c)

if (CONFIG_AT_WEB_SERVER_SUPPORT)
    if(NOT CONFIG_AT_WEB_USE_FATFS)
        set(embed_txt_files ../fs_image/index.html)
    endif()
endif()

idf_component_register (
    SRCS ${srcs}
    INCLUDE_DIRS include
    PRIV_INCLUDE_DIRS private_include
    REQUIRES ${require_components}
    EMBED_TXTFILES ${embed_txt_files})

if (${SILENCE} EQUAL 1)
set(LIB_NAME _at_core_silence)
else()
set(LIB_NAME _at_core)
endif()

string(SUBSTRING "$ENV{ESP_AT_PROJECT_PLATFORM}" 9 31 PLATFORM_NAME) # remove PLATFORM_
string(STRIP ${PLATFORM_NAME} PLATFORM_NAME)
string(CONCAT FULL_NAME ${PLATFORM_NAME} ${LIB_NAME})
string(TOLOWER ${FULL_NAME} LIBS)

message("silence:${SILENCE}, ld core:${CMAKE_CURRENT_SOURCE_DIR}/lib/lib${LIBS}.a")

add_library(${LIBS} STATIC IMPORTED)
set_property(TARGET ${LIBS} PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/lib${LIBS}.a)
target_link_libraries(${COMPONENT_LIB} INTERFACE ${LIBS})
set_property(TARGET ${LIBS} APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${COMPONENT_LIB})
