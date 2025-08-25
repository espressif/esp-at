# (Automatically converted from project Makefile by convert_to_cmake.py.)

# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

set(IDF_PATH ${CMAKE_SOURCE_DIR}/esp-idf)

# set the module_config dir
string(TOLOWER $ENV{ESP_AT_MODULE_NAME} ESP_AT_MODULE_NAME_LOWER)
string(STRIP ${ESP_AT_MODULE_NAME_LOWER} ESP_AT_MODULE_NAME_LOWER)
string(STRIP $ENV{ESP_AT_MODULE_NAME} ESP_AT_MODULE_NAME)
set(ESP_AT_MODULE_CONFIG_DIR ${CMAKE_SOURCE_DIR}/module_config/module_${ESP_AT_MODULE_NAME_LOWER})
if (NOT EXISTS "${ESP_AT_MODULE_CONFIG_DIR}")
    string(SUBSTRING "$ENV{ESP_AT_PROJECT_PLATFORM}" 9 31 PLATFORM_NAME) # remove PLATFORM_
    string(STRIP ${PLATFORM_NAME} PLATFORM_NAME)
    string(TOLOWER ${PLATFORM_NAME} ESP_AT_PLATFORM_NAME_LOWER)
    set(ESP_AT_MODULE_CONFIG_DIR ${CMAKE_SOURCE_DIR}/module_config/module_${ESP_AT_PLATFORM_NAME_LOWER}_default)
endif()

SET(ENV{ESP_AT_PROJECT_PATH} ${CMAKE_SOURCE_DIR})

# set silence
if (NOT DEFINED SILENCE)
    if (DEFINED ENV{SILENCE})
        set(SILENCE $ENV{SILENCE})
    else()
        set(SILENCE 0)
    endif()
endif()

# set the sdkconfig default file
if (${SILENCE} EQUAL 1)
    set(SDKCONFIG_DEFAULTS ${ESP_AT_MODULE_CONFIG_DIR}/sdkconfig_silence.defaults)
    if(DEFINED ENV{AT_EXT_MODULE_CFG} AND EXISTS "$ENV{AT_EXT_MODULE_CFG}/sdkconfig_silence.defaults")
        list(APPEND SDKCONFIG_DEFAULTS $ENV{AT_EXT_MODULE_CFG}/sdkconfig_silence.defaults)
    endif()
else()
    set(SDKCONFIG_DEFAULTS ${ESP_AT_MODULE_CONFIG_DIR}/sdkconfig.defaults)
    if(DEFINED ENV{AT_EXT_MODULE_CFG} AND EXISTS "$ENV{AT_EXT_MODULE_CFG}/sdkconfig.defaults")
        list(APPEND SDKCONFIG_DEFAULTS $ENV{AT_EXT_MODULE_CFG}/sdkconfig.defaults)
    endif()
endif()
message(STATUS "silence:${SILENCE}, sdkconfig:${SDKCONFIG_DEFAULTS}")

# set the base directory of partition table
if(DEFINED ENV{AT_EXT_MODULE_CFG} AND EXISTS "$ENV{AT_EXT_MODULE_CFG}/partitions_at.csv")
    set(CONFIG_PARTITION_TABLE_BASE_DIR "$ENV{AT_EXT_MODULE_CFG}")
else()
    set(CONFIG_PARTITION_TABLE_BASE_DIR "${CMAKE_SOURCE_DIR}")
endif()
message(STATUS "partition_table_dir: ${CONFIG_PARTITION_TABLE_BASE_DIR}")

if(NOT DEFINED ENV{IDF_PATH})
    SET(ENV{IDF_PATH} ${CMAKE_SOURCE_DIR}/esp-idf)
    message(STATUS $ENV{IDF_PATH})
endif()

execute_process(COMMAND git rev-parse --short HEAD
                OUTPUT_VARIABLE ESP_AT_PROJECT_COMMIT_ID
                OUTPUT_STRIP_TRAILING_WHITESPACE)

add_compile_options("-DESP_AT_PROJECT_COMMIT_ID=\"${ESP_AT_PROJECT_COMMIT_ID}\"")

# set the firmware source
if (DEFINED ENV{ESP_AT_FIRMWARE_FROM})
set(ESP_AT_FIRMWARE_FROM $ENV{ESP_AT_FIRMWARE_FROM})
add_compile_options("-DESP_AT_FIRMWARE_FROM=\"${ESP_AT_FIRMWARE_FROM}\"")
endif()

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

# optimize sntp startup delay and receive timeout to speed up time synchronization
add_compile_options("-DSNTP_RECV_TIMEOUT=3000")

# add cmake flags
### (unused typedefs, unused variable, unused function, unused-but-set-variable) should not be treated as warnings and errors
add_compile_options("-Wno-unused-local-typedefs")
add_compile_options("-Wno-unused-variable")
add_compile_options("-Wno-unused-function")
add_compile_options("-Wno-unused-but-set-variable")
add_compile_options("-Wno-format")

# add rainmaker extra components
if (DEFINED ENV{AT_RAINMAKER_SUPPORT})
    if (NOT DEFINED ENV{RMAKER_PATH})
        set(RMAKER_PATH ${CMAKE_SOURCE_DIR}/components/rainmaker/esp-rainmaker)
        set(EXTRA_COMPONENT_DIRS ${RMAKER_PATH}/components)
    endif()
endif()

# add the extra custom components
if (DEFINED ENV{AT_CUSTOM_COMPONENTS})
    set(AT_CUSTOM_COMPONENTS "$ENV{AT_CUSTOM_COMPONENTS}")
    string(STRIP "${AT_CUSTOM_COMPONENTS}" AT_CUSTOM_COMPONENTS)
    while("${AT_CUSTOM_COMPONENTS}" MATCHES "  ")
        string(REPLACE "  " " " AT_CUSTOM_COMPONENTS "${AT_CUSTOM_COMPONENTS}")
    endwhile()
    string(REPLACE " " ";" AT_CUSTOM_COMPONENTS "${AT_CUSTOM_COMPONENTS}")
    list(APPEND EXTRA_COMPONENT_DIRS ${AT_CUSTOM_COMPONENTS})
endif()

if (EXTRA_COMPONENT_DIRS)
    message("extra component dirs: ${EXTRA_COMPONENT_DIRS}")
else()
    message("no extra component dirs")
endif()

# Apply possible patches before generating sdkconfig
execute_process(
    COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/tools/patch.py $ENV{ESP_AT_PROJECT_PATH}/patches ${IDF_TARGET} before_sdkconfig
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Failed to apply the patch before generating sdkconfig. ${error}")
else()
    message(STATUS "Successfully applied the patch before generating sdkconfig. ${output}")
endif()

include(${IDF_PATH}/tools/cmake/project.cmake)

project(esp-at)

# Sanity checker for the configurations
execute_process(
    COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/tools/at_sanity_checker.py ${CONFIG_PARTITION_TABLE_BASE_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "ESP-AT sanity check failed. ${error}")
else()
    message(STATUS "ESP-AT sanity check passed. ${output}")
endif()

# Apply possible patches after generating sdkconfig
execute_process(
    COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/tools/patch.py $ENV{ESP_AT_PROJECT_PATH}/patches ${IDF_TARGET} after_sdkconfig
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Failed to apply the patch after generating sdkconfig. ${error}")
else()
    message(STATUS "Successfully applied the patch after generating sdkconfig. ${output}")
endif()

# Create Compressed image bin
if (CONFIG_BOOTLOADER_COMPRESSED_ENABLED)
    if (CONFIG_ENABLE_LEGACY_ESP_BOOTLOADER_PLUS_V2_SUPPORT)
        set(COMPRESSED_FIRMWARE_VERSION "v2")
        set(COMPRESSED_FIRMWARE_ADD_APP_HEADER "")
    else()
        set(COMPRESSED_FIRMWARE_VERSION "v3")
        set(COMPRESSED_FIRMWARE_ADD_APP_HEADER "--add_app_header")
    endif()
    add_custom_target(ota_image ALL DEPENDS app)
    add_custom_command(TARGET ota_image
    POST_BUILD
    COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/managed_components/espressif__cmake_utilities/scripts/gen_custom_ota.py -hv ${COMPRESSED_FIRMWARE_VERSION} -i esp-at.bin ${COMPRESSED_FIRMWARE_ADD_APP_HEADER}
    COMMAND ${CMAKE_COMMAND} -E copy_directory custom_ota_binaries compressed_ota_image
    COMMENT "Creating Compressed image..."
    )
    add_dependencies(ota_image gen_project_binary)
    add_dependencies(flash ota_image)
endif()

# Network Debug
add_custom_target(at_net_debug ALL)
if (CONFIG_AT_NET_DEBUG)
    add_custom_command(TARGET at_net_debug
        POST_BUILD
        COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/tools/at_net_debug.py --sdkconfig $ENV{ESP_AT_PROJECT_PATH}/sdkconfig
        DEPENDS $ENV{ESP_AT_PROJECT_PATH}/sdkconfig
        WORKING_DIRECTORY $ENV{ESP_AT_PROJECT_PATH}/esp-idf
        COMMENT "Enabling Network Debug...")
else()
    add_custom_command(TARGET at_net_debug
        POST_BUILD
        COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/tools/at_net_debug.py --restore
        WORKING_DIRECTORY $ENV{ESP_AT_PROJECT_PATH}/esp-idf
        COMMENT "Disabling Network Debug...")
endif()
add_dependencies(gen_project_binary at_net_debug)
add_dependencies(flash at_net_debug)

# Create factory bin
file(MAKE_DIRECTORY $ENV{ESP_AT_PROJECT_PATH}/build/factory)
add_custom_target(esp_at_factory_bin ALL DEPENDS app bootloader gen_project_binary)
add_custom_command(TARGET esp_at_factory_bin
    POST_BUILD
    COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/esp-idf/components/esptool_py/esptool/esptool.py
        --chip ${IDF_TARGET} merge_bin --output factory/factory_${ESP_AT_MODULE_NAME}.bin --fill-flash-size ${CONFIG_ESPTOOLPY_FLASHSIZE} "@flash_project_args"
        WORKING_DIRECTORY $ENV{ESP_AT_PROJECT_PATH}/build
    COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/esp-idf/components/esptool_py/esptool/esptool.py
        --chip ${IDF_TARGET} merge_bin --output factory/factory_${ESP_AT_MODULE_NAME}_unfilled.bin "@flash_project_args"
        WORKING_DIRECTORY $ENV{ESP_AT_PROJECT_PATH}/build
    COMMENT "Creating factory bin..."
)
add_dependencies(flash esp_at_factory_bin)

# If you need more RAM, you can uncomment the section below.
# However, please note that this may introduce certain potential crash issues.
# You will need to modify the CSV files in 'tools/relink' based on the product's requirements.
# (refer to tools/relink/README.md to move certain APIs from flash to IRAM)
# if (CONFIG_IDF_TARGET_ESP32C2)
#     message("ready to relink..")
#     include(tools/relink/relink.cmake)
# endif()
