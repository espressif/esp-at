# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# add cmake flags
### (unused typedefs, unused variable, unused function, unused-but-set-variable) should not be treated as warnings and errors
add_compile_options("-Wno-unused-local-typedefs")
add_compile_options("-Wno-unused-variable")
add_compile_options("-Wno-unused-function")
add_compile_options("-Wno-unused-but-set-variable")
add_compile_options("-Wno-format")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(sdspi_host)
