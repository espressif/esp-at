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

set(ESP_AT_MODULE_CONFIG_DIR ${CMAKE_SOURCE_DIR}/module_config/module_${LOWER_NAME}_default)
endif()

SET(ENV{ESP_AT_PROJECT_PATH} ${CMAKE_SOURCE_DIR})

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
add_compile_options("-DSNTP_STARTUP_DELAY_FUNC=0")
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
    list(APPEND EXTRA_COMPONENT_DIRS $ENV{AT_CUSTOM_COMPONENTS})
    message("add extra custom components: $ENV{AT_CUSTOM_COMPONENTS}")
endif()

if (EXTRA_COMPONENT_DIRS)
    message("extra component dirs: ${EXTRA_COMPONENT_DIRS}")
else()
    message("no extra component dirs")
endif()

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
    COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/managed_components/espressif__cmake_utilities/scripts/gen_custom_ota.py -hv v2 -i esp-at.bin
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
add_custom_command(TARGET factory_bin
  POST_BUILD
  COMMAND ${PYTHON} $ENV{ESP_AT_PROJECT_PATH}/tools/esp_at_factory_bin_combine.py
		--module_name ${ESP_AT_MODULE_NAME}
 		--bin_directory $ENV{ESP_AT_PROJECT_PATH}/build
		--flash_mode ${CONFIG_ESPTOOLPY_FLASHMODE}
		--flash_size ${CONFIG_ESPTOOLPY_FLASHSIZE}
		--flash_speed ${CONFIG_ESPTOOLPY_FLASHFREQ}
        --download_config $ENV{ESP_AT_PROJECT_PATH}/build/flash_project_args
  COMMENT "Create factory bin..."
)
add_dependencies(factory_bin gen_project_binary)
add_dependencies(flash factory_bin)

# If you need more RAM, you can uncomment the section below.
# However, please note that this may introduce certain potential crash issues.
# You will need to modify the CSV files in 'tools/relink' based on the product's requirements.
# (refer to tools/relink/README.md to move certain APIs from flash to IRAM)
# if (CONFIG_IDF_TARGET_ESP32C2)
#     message("ready to relink..")
#     include(tools/relink/relink.cmake)
# endif()
