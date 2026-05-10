cmake_minimum_required(VERSION 3.16)

add_compile_options(-fno-threadsafe-statics)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(aefera_os)
