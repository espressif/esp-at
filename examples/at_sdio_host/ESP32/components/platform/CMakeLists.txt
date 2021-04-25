file(GLOB_RECURSE srcs *.c)

set(components_requires sdmmc)

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "esp32/include" "../sdio_host/include"
                    PRIV_REQUIRES ${components_requires})
