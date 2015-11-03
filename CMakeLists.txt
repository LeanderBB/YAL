#
# YAL - Yet Another Language
#
project(yal)
cmake_minimum_required(VERSION 2.8)
enable_testing()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (NOT MVSC)
    set(YAL_C_WERROR "-Werror")
    set(YAL_C_FLAGS "-std=c99 -Wall -Wextra -Werror -pedantic")
    set(YAL_C_FLAGS_DEBUG "-DYAL_DEBUG")
    set(YAL_CXX_WERROR "-Werror")
    set(YAL_CXX_FLAGS "-Wall -Wextra -std=c++11 -fexceptions -fno-rtti")
    set(YAL_CXX_FLAGS_DEBUG "-DYAL_DEBUG -fno-inline")
else()

endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${YAL_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${YAL_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${YAL_CXX_FLAGS_DEBUG}")


find_package(BISON 3 REQUIRED)
find_package(FLEX REQUIRED)
find_package(Threads REQUIRED)

include_directories(include)

add_subdirectory(lib)
add_subdirectory(src/yalc)
add_subdirectory(src/yaldisass)
add_subdirectory(src/yale)
add_subdirectory(src/yalhash)
add_subdirectory(tests)
