# (Automatically converted from project Makefile by convert_to_cmake.py.)

# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(IDF_PATH ${CMAKE_SOURCE_DIR}/esp-idf)

string(TOLOWER $ENV{ESP_AT_MODULE_NAME} LOWER_NAME)
string(STRIP $ENV{ESP_AT_MODULE_NAME} ESP_AT_MODULE_NAME)

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


# get branch name and set docs version
if(NOT DEFINED ENV{ESP_AT_PROJECT_BRANCH})
execute_process(COMMAND git rev-parse --abbrev-ref HEAD
                OUTPUT_VARIABLE ESP_AT_PROJECT_BRANCH
                OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
### GitLab CI and GitHub Actions should already set it
set(ESP_AT_PROJECT_BRANCH $ENV{ESP_AT_PROJECT_BRANCH})
endif()
message("esp-at branch: ${ESP_AT_PROJECT_BRANCH}")
if(${ESP_AT_PROJECT_BRANCH} STREQUAL "master")
set(ESP_AT_DOCS_VERSION "latest")
else()
string(REPLACE "/" "-" ESP_AT_DOCS_VERSION ${ESP_AT_PROJECT_BRANCH})
endif()
add_compile_options("-DESP_AT_DOCS_VERSION=\"${ESP_AT_DOCS_VERSION}\"")
message("esp-at docs version: ${ESP_AT_DOCS_VERSION}")

# add default fallback dns server
set(FALLBACK_DNS_SERVER "8.8.8.8")
add_compile_options("-DFALLBACK_DNS_SERVER_ADDRESS(p)=ipaddr_aton(\"${FALLBACK_DNS_SERVER}\",(p))")

# add cmake flags
### (unused typedefs, unused variable, unused function, unused-but-set-variable) should not be treated as warnings and errors
add_compile_options("-Wno-unused-local-typedefs")
add_compile_options("-Wno-unused-variable")
add_compile_options("-Wno-unused-function")
add_compile_options("-Wno-unused-but-set-variable")
add_compile_options("-Wno-format")

include(${IDF_PATH}/tools/cmake/project.cmake)

project(esp-at)

if (CONFIG_AT_CUSTOMIZED_PARTITION_TABLE_SUPPORT)
    add_custom_target(factory_bin ALL DEPENDS app)
else()
    # TODO: combine defaults bins into a factory bin
    add_custom_target(factory_bin)
endif()

# Create Compressed image bin
if (CONFIG_BOOTLOADER_COMPRESSED_ENABLED)
    add_custom_target(ota_image ALL DEPENDS app)
    add_custom_command(TARGET ota_image
    POST_BUILD
    COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/bootloader_components/tools/custom_ota_gen.py -hv v2 -i esp-at.bin
    COMMAND ${CMAKE_COMMAND} -E copy_directory custom_ota_binaries compressed_ota_image
    COMMENT "Creating Compressed image..."
    )
    add_dependencies(ota_image gen_project_binary)
    add_dependencies(flash ota_image)
endif()

# Create factory bin
file(MAKE_DIRECTORY $ENV{ESP_AT_PROJECT_PATH}/build/factory)
add_custom_command(TARGET factory_bin
  POST_BUILD
  COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/tools/esp_at_factory_bin_combine.py
		--module_name ${ESP_AT_MODULE_NAME}
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
