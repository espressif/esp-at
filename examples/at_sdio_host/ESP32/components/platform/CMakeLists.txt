file(GLOB_RECURSE srcs *.c)

set(components_requires sdmmc)

if("${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}" VERSION_GREATER_EQUAL "5.4")
    list(APPEND components_requires esp_driver_sdmmc)
endif()

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "esp32/include" "../sdio_host/include"
                    PRIV_REQUIRES ${components_requires})
