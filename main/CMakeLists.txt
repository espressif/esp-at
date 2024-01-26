
file(GLOB_RECURSE srcs *.c)

set(includes "interface/include")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ${includes})
