#
# Simple Parser test
#
add_executable(test_yal_parser main.cpp)

target_link_libraries(test_yal_parser
        PRIVATE yal
    )

YAL_APPLY_COMPILER_FLAGS_WERROR(test_yal_parser)
