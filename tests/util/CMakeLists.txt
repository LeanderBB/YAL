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
        PRIVATE GTest::GTest GTest::Main yal::frontend gtest-util
        PRIVATE compile_options compile_options_werror
    )

add_test(NAME "utilities" COMMAND "test_util")
