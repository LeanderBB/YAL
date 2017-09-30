#
# YAL Lexer Unit test
#

add_executable(test_yal_lexer main.cpp)

target_link_libraries(test_yal_lexer
        PRIVATE gtest yal
    )

YAL_APPLY_COMPILER_FLAGS_WERROR(test_yal_lexer)
