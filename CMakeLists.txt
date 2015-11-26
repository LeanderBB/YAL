#
# YAL - Yet Another Language
#
project(yal)
cmake_minimum_required(VERSION 2.8)
enable_testing()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (NOT MSVC)
    set(YAL_C_WERROR "-Werror")
    set(YAL_C_FLAGS "-std=c99 -Wall -Wextra -Werror -pedantic")
    set(YAL_C_FLAGS_DEBUG "-DYAL_DEBUG")
    set(YAL_CXX_WERROR "-Werror")
    set(YAL_CXX_FLAGS "-Wall -Wextra -std=c++11 -fexceptions -fno-rtti")
    set(YAL_CXX_FLAGS_DEBUG "-DYAL_DEBUG -fno-inline")
else()
    add_definitions( "/D WIN32" "/D _WINDOWS" "/D _CRT_SECURE_NO_WARNINGS")
    set(YAL_C_WERROR "/WX")
    set(YAL_C_FLAGS "/Wall")
    set(YAL_C_WARNING_FLAGS "/W2")
    set(YAL_C_FLAGS_DEBUG "/DYAL_DEBUG /D _DEBUG /Oy- /Od /Ob0 /Gm- /RTC1 /Zi")
    set(YAL_C_FLAGS_RELEASE "/O2 /Oy- /Gm- /GS /MD /Ob2 /Gd  /D NDEBUG")
    set(YAL_CXX_WERROR "/WX")
    set(YAL_CXX_FLAGS "/W2 /EHsc")
    set(YAL_CXX_FLAGS_DEBUG "/DYAL_DEBUG /D _DEBUG /Oy- /Od /Ob0 /Gm- /RTC1 /Zi")
    set(YAL_CXX_FLAGS_RELEASE "/O2 /Oy- /Gm- /GS /MD /Ob2 /Gd  /D NDEBUG")
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${YAL_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${YAL_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${YAL_CXX_FLAGS_DEBUG}")


find_package(Threads REQUIRED)

include_directories(include)

add_subdirectory(lib)
add_subdirectory(src/yalc)
add_subdirectory(src/yaldisass)
add_subdirectory(src/yale)
add_subdirectory(tests)
