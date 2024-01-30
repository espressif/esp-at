
set(require_components nvs_flash mqtt mdns esp_http_client esp_https_ota json freertos spiffs
    bootloader_support app_update wpa_supplicant spi_flash esp_http_server bt fatfs esp_websocket_client main)

if ("${IDF_TARGET}" STREQUAL "esp32")
    list(APPEND require_components esp_eth)
endif()

set(srcs "")
list(APPEND srcs "src/at_default_config.c")
list(APPEND srcs "src/at_init.c")
list(APPEND srcs "src/at_workaround.c")
list(APPEND srcs "src/at_cmd_register.c")
if (CONFIG_AT_UART_COMMAND_SUPPORT)
    list(APPEND srcs "src/at_uart_cmd.c")
endif()
if (CONFIG_BOOTLOADER_COMPRESSED_ENABLED)
    list(APPEND srcs "src/at_compress_ota.c")
endif()
if (CONFIG_AT_ETHERNET_SUPPORT)
    list(APPEND srcs "src/at_eth_init.c")
endif()
if (CONFIG_AT_SIGNALING_COMMAND_SUPPORT)
    list(APPEND srcs "src/at_fact_cmd.c")
endif()
if (CONFIG_AT_BT_A2DP_COMMAND_SUPPORT)
    list(APPEND srcs "src/at_i2s.c")
endif()
if (CONFIG_AT_OTA_SUPPORT)
    list(APPEND srcs "src/at_ota_cmd.c")
endif()
if (CONFIG_AT_USER_COMMAND_SUPPORT)
    list(APPEND srcs "src/at_user_cmd.c")
endif()
if (CONFIG_AT_WEB_SERVER_SUPPORT)
    list(APPEND srcs "src/at_web_dns_server.c")
    list(APPEND srcs "src/at_web_server_cmd.c")
endif()
if (CONFIG_AT_BASE_ON_UART)
    list(APPEND srcs "src/at_uart_cmd.c")
endif()

if (CONFIG_AT_WEB_SERVER_SUPPORT)
    if(NOT CONFIG_AT_WEB_USE_FATFS)
        set(embed_txt_files ../fs_image/index.html)
    endif()
endif()

set(includes "include")

idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS ${includes}
    PRIV_INCLUDE_DIRS private_include
    REQUIRES ${require_components}
    LDFRAGMENTS at_linker.lf
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

# force the referencing of some symbols
include (force_symbol_ref.cmake)
