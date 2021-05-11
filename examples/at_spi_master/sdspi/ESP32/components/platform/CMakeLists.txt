file(GLOB_RECURSE srcs *.c)

set(components_requires spi_flash)

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "esp32/include" "../sdspi_host/include"
                    PRIV_REQUIRES ${components_requires})
