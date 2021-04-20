# (Automatically converted from project Makefile by convert_to_cmake.py.)

# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(IDF_PATH ${CMAKE_SOURCE_DIR}/esp-idf)

string(TOLOWER $ENV{ESP_AT_MODULE_NAME} LOWER_NAME)

set(ESP_AT_MODULE_CONFIG_DIR ${CMAKE_SOURCE_DIR}/module_config/module_${LOWER_NAME})
if (NOT EXISTS "${ESP_AT_MODULE_CONFIG_DIR}")
string(SUBSTRING "$ENV{ESP_AT_PROJECT_PLATFORM}" 9 31 PLATFORM_NAME) # remove PLATFORM_
string(STRIP ${PLATFORM_NAME} PLATFORM_NAME)
string(TOLOWER ${PLATFORM_NAME} LOWER_NAME)

set(ESP_AT_MODULE_CONFIG_DIR ${CMAKE_SOURCE_DIR}/module_config/module_${LOWER_NAME}_default )
endif()

SET(ENV{ESP_AT_PROJECT_PATH} ${CMAKE_SOURCE_DIR} )

if (NOT DEFINED SILENCE)
if (DEFINED ENV{SILENCE})
set(SILENCE $ENV{SILENCE})
else()
set(SILENCE 0)
endif()
endif()

if (${SILENCE} EQUAL 1)
set(SDKCONFIG_DEFAULTS ${ESP_AT_MODULE_CONFIG_DIR}/sdkconfig_silence.defaults)
else()
set(SDKCONFIG_DEFAULTS ${ESP_AT_MODULE_CONFIG_DIR}/sdkconfig.defaults)
endif()

message("silence:${SILENCE} sdkconfig:${SDKCONFIG_DEFAULTS}")

if(NOT DEFINED ENV{IDF_PATH})
    SET(ENV{IDF_PATH} ${CMAKE_SOURCE_DIR}/esp-idf )
    message(STATUS $ENV{IDF_PATH})
endif()

execute_process(COMMAND git rev-parse --short HEAD
                OUTPUT_VARIABLE ESP_AT_PROJECT_COMMIT_ID
                OUTPUT_STRIP_TRAILING_WHITESPACE)

add_compile_options("-DESP_AT_PROJECT_COMMIT_ID=\"${ESP_AT_PROJECT_COMMIT_ID}\"")

include(${IDF_PATH}/tools/cmake/project.cmake)

project(esp-at)

if (CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_SUPPORT)
    add_custom_target(factory_bin ALL DEPENDS app)
else()
    # TODO: combine defaults bins into a factory bin
    add_custom_target(factory_bin)
endif()

file(MAKE_DIRECTORY $ENV{ESP_AT_PROJECT_PATH}/build/factory)

add_custom_command(TARGET factory_bin
  POST_BUILD
  COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/tools/esp_at_factory_bin_combine.py
		--module_name $ENV{ESP_AT_MODULE_NAME}
 		--bin_directory $ENV{ESP_AT_PROJECT_PATH}/build
		--flash_mode ${CONFIG_ESPTOOLPY_FLASHMODE}
		--flash_size ${CONFIG_ESPTOOLPY_FLASHSIZE}
		--flash_speed ${CONFIG_ESPTOOLPY_FLASHFREQ}
		--parameter_file $ENV{ESP_AT_PROJECT_PATH}/build/factory/factory_parameter.log
        --download_config $ENV{ESP_AT_PROJECT_PATH}/build/flash_project_args
  COMMENT "Create factory bin..."
)

add_dependencies(factory_bin gen_project_binary)
add_dependencies(flash factory_bin)
