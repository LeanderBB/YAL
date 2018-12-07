#
# YAL Lexer Unit test
#

add_executable(test_yal_lexer  main.cpp)

target_link_libraries(test_yal_lexer
        PRIVATE GTest::GTest yal::frontend
        PRIVATE compile_options compile_options_werror
    )


add_test(NAME "lexer" COMMAND test_yal_lexer)
