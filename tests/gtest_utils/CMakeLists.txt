#
# Yal utilities for gtest
#


set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/gtest_utils.h
    )

add_library(gtest-util INTERFACE)

target_include_directories(gtest-util INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_sources(gtest-util INTERFACE ${SOURCES})
