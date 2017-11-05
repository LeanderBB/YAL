#
# Test cases for util classes
#


add_executable(test_allocatorstack allocatorstack.cpp)

target_link_libraries(test_allocatorstack
        PRIVATE GTest::GTest yal
    )

YAL_APPLY_COMPILER_FLAGS_WERROR(test_allocatorstack)
