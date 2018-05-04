#
# YAL Lexer Unit test
#

add_executable(test_yal_lexer  main.cpp)

target_link_libraries(test_yal_lexer
        PRIVATE GTest::GTest yal
        PRIVATE compile_options compile_options_werror
    )
