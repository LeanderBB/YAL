#
# Test cases for util classes
#


set (SOURCES
    test_allocatorstack.cpp
    test_cast.cpp
    test_format.cpp
    test_strconversions.cpp
    )

add_executable(test_util ${SOURCES})

target_link_libraries(test_util
        PRIVATE GTest::GTest GTest::Main yal gtest-util
    )

YAL_APPLY_COMPILER_FLAGS_WERROR(test_util)
