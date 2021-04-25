file(GLOB_RECURSE srcs *.c)

set(components_requires platform sdmmc)

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES ${components_requires})
