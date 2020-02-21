# (Automatically converted from project Makefile by convert_to_cmake.py.)

# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

if (NOT DEFINED ENV{ESP_AT_PROJECT_PLATFORM})
set(ENV{ESP_AT_PROJECT_PLATFORM} PLATFORM_ESP32)
endif()

if (NOT DEFINED ENV{ESP_AT_MODULE_NAME})
set(ENV{ESP_AT_MODULE_NAME} WROOM-32)
endif()

set(IDF_PATH ${CMAKE_SOURCE_DIR}/esp-idf)

string(TOLOWER $ENV{ESP_AT_MODULE_NAME} LOWER_NAME)

set(ESP_AT_MODULE_CONFIG_DIR ${CMAKE_SOURCE_DIR}/module_config/module_${LOWER_NAME})
if (NOT EXISTS "${ESP_AT_MODULE_CONFIG_DIR}")
string(SUBSTRING "$ENV{ESP_AT_PROJECT_PLATFORM}" 9 31 PLATFORM_NAME) # remove PLATFORM_
string(TOLOWER ${PLATFORM_NAME} LOWER_NAME)

set(ESP_AT_MODULE_CONFIG_DIR ${CMAKE_SOURCE_DIR}/module_config/module_${LOWER_NAME}_default )
endif()

SET(ENV{ESP_AT_PROJECT_PATH} ${CMAKE_SOURCE_DIR} )

if (NOT DEFINED SILENCE)
if (DEFINED ENV{SILENCE})
set(SILENCE 1)
else()
set(SILENCE 0)
endif()
endif()

if (${SILENCE} EQUAL 1)
set(SDKCONFIG_DEFAULTS ${ESP_AT_MODULE_CONFIG_DIR}/sdkconfig_silence.defaults)
else()
set(SDKCONFIG_DEFAULTS ${ESP_AT_MODULE_CONFIG_DIR}/sdkconfig.defaults)
endif()

if(NOT DEFINED ENV{IDF_PATH})
    SET(ENV{IDF_PATH} ${CMAKE_SOURCE_DIR}/esp-idf )
    message(STATUS $ENV{IDF_PATH})
endif()

execute_process(COMMAND git rev-parse --short HEAD
                OUTPUT_VARIABLE ESP_AT_PROJECT_COMMIT_ID
                OUTPUT_STRIP_TRAILING_WHITESPACE)

add_compile_options("-DESP_AT_PROJECT_COMMIT_ID=\"${ESP_AT_PROJECT_COMMIT_ID}\"")

execute_process(COMMAND sed -n /branch:/p ${ESP_AT_MODULE_CONFIG_DIR}/IDF_VERSION
                OUTPUT_VARIABLE idf_branch
                OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REPLACE "branch:" "" esp_at_idf_branch ${idf_branch})

execute_process(COMMAND sed -n /commit:/p ${ESP_AT_MODULE_CONFIG_DIR}/IDF_VERSION
                OUTPUT_VARIABLE idf_commit
                OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REPLACE "commit:" "" esp_at_idf_commit ${idf_commit})

execute_process(COMMAND sed -n /repository:/p ${ESP_AT_MODULE_CONFIG_DIR}/IDF_VERSION
                OUTPUT_VARIABLE idf_repository
                OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REPLACE "repository:" "" esp_at_idf_repository ${idf_repository})

if(NOT IS_DIRECTORY ${IDF_PATH})
    execute_process(COMMAND git clone -b ${esp_at_idf_branch} ${esp_at_idf_repository} ${IDF_PATH})
endif()

execute_process(WORKING_DIRECTORY ${IDF_PATH}
                COMMAND git rev-parse HEAD
                OUTPUT_VARIABLE current_idf_commit
                OUTPUT_STRIP_TRAILING_WHITESPACE)

if(NOT ${current_idf_commit} EQUAL ${esp_at_idf_commit})
    execute_process(WORKING_DIRECTORY ${IDF_PATH}
                    COMMAND git checkout ${esp_at_idf_branch})

    execute_process(WORKING_DIRECTORY ${IDF_PATH}
                    COMMAND git pull)

    execute_process(WORKING_DIRECTORY ${IDF_PATH}
                    COMMAND git checkout ${esp_at_idf_commit})

    execute_process(WORKING_DIRECTORY ${IDF_PATH}
                COMMAND git submodule update --init --recursive )
endif()

include(${IDF_PATH}/tools/cmake/project.cmake)

project(esp-at)

add_custom_target(factory_bin ALL DEPENDS customized_bin)
add_custom_command(TARGET factory_bin
  POST_BUILD
  COMMAND mkdir -p $ENV{ESP_AT_PROJECT_PATH}/build/factory
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