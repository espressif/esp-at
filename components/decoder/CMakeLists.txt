
# Prebuilt MP3 decoder is ESP32-only (A2DP). Other targets leave this
# component unregistered.
if(NOT "${IDF_TARGET}" STREQUAL "esp32")
    return()
endif()

idf_component_register(REQUIRES log esp_hw_support)

set(LIB_NAME _mp3)

string(SUBSTRING "$ENV{ESP_AT_PROJECT_PLATFORM}" 9 31 PLATFORM_NAME) # remove PLATFORM_
string(STRIP ${PLATFORM_NAME} PLATFORM_NAME)
string(CONCAT FULL_NAME ${PLATFORM_NAME} ${LIB_NAME})
string(TOLOWER ${FULL_NAME} LIBS)

add_library(${LIBS} STATIC IMPORTED)
set_property(TARGET ${LIBS} PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/lib${LIBS}.a)
target_link_libraries(${COMPONENT_LIB} INTERFACE ${LIBS})
set_property(TARGET ${LIBS} APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${COMPONENT_LIB})
