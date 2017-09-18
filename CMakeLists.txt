#
# YAL - Yet Another Language
#
project(yal LANGUAGES CXX C)
cmake_minimum_required(VERSION 3.9)
enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (buildconfig)


add_subdirectory(tests)
