#
# Dump tokens in the stream
#

add_executable(yal_lexer_dump lexerdump.cpp)

target_link_libraries(yal_lexer_dump
        PRIVATE  yal
    )

YAL_APPLY_COMPILER_FLAGS_WERROR(yal_lexer_dump)
