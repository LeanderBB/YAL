#
# YAL - Yet Another Language
#
project(yal)
cmake_minimum_required(VERSION 3.9)
enable_testing()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

include_directories(include)
