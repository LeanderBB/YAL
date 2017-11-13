#
# Test cases for util classes
#


set (SOURCES
    allocatorstack.cpp
    cast.cpp
    format.cpp
    )

add_executable(test_util ${SOURCES})

target_link_libraries(test_util
        PRIVATE GTest::GTest GTest::Main yal gtest-util
    )

YAL_APPLY_COMPILER_FLAGS_WERROR(test_util)
