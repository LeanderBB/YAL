#
# YAL - Yet Another Language
#
project(yal LANGUAGES CXX C)
cmake_minimum_required(VERSION 3.9)
enable_testing()

option(ENABLE_TEST "Enable compilation of tests" ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (buildconfig)

add_subdirectory(lib)
add_subdirectory(src)

if (${ENABLE_TEST})
    add_subdirectory(tests)
endif()
#add_subdirectory(tools)
