#
# YAL Lexer Unit test
#

add_executable(test_yal_lexer main.cpp)

target_link_libraries(test_yal_lexer
        PRIVATE gtest
    )

YAL_APPLY_COMPILER_FLAGS(test_yal_lexer)
