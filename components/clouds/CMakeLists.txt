idf_component_register (
    INCLUDE_DIRS qcloud/include)

if (CONFIG_AT_QCLOUD_IOT_COMMAND_SUPPORT)
    if (${SILENCE} EQUAL 1)
    set(LIB_NAME _qcloud_core_silence)
    else()
    set(LIB_NAME _qcloud_core)
    endif()

    string(SUBSTRING "$ENV{ESP_AT_PROJECT_PLATFORM}" 9 31 PLATFORM_NAME) # remove PLATFORM_
    string(STRIP ${PLATFORM_NAME} PLATFORM_NAME)
    string(CONCAT FULL_NAME ${PLATFORM_NAME} ${LIB_NAME})
    string(TOLOWER ${FULL_NAME} LIBS)

	message("silence:${SILENCE}, ld core:${CMAKE_CURRENT_SOURCE_DIR}/qcloud/lib/lib${LIBS}.a")

    add_library(${LIBS} STATIC IMPORTED)
    set_property(TARGET ${LIBS} PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/qcloud/lib/lib${LIBS}.a)
    target_link_libraries(${COMPONENT_LIB} INTERFACE ${LIBS})
    set_property(TARGET ${LIBS} APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${COMPONENT_LIB})
endif()
