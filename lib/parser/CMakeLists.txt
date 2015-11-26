#
# YAL - Flex/bison support layer
#

if (NOT WIN32)

find_package(BISON 3 REQUIRED)
find_package(FLEX REQUIRED)
BISON_TARGET(yalParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/yal_bison_parser.cpp
COMPILE_FLAGS "--warnings=all -Werror --feature=diagnostics-show-caret --debug --verbose --report=state")
FLEX_TARGET(yalLexer lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/yal_flex_lexer.cpp
COMPILE_FLAGS "--header-file=${CMAKE_CURRENT_BINARY_DIR}/yal_flex_lexer.h")

ADD_FLEX_BISON_DEPENDENCY(yalLexer yalParser)

add_custom_target(ParserSoures SOURCES parser.y lexer.l)

add_library(${YAL_LIB_PARSER} STATIC
    ${BISON_yalParser_OUTPUTS}
    ${FLEX_yalLexer_OUTPUTS}
)

set(YAL_PARSER_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR})
set(YAL_PARSER_INCLUDE_DIRS ${YAL_PARSER_INCLUDE_DIRS} PARENT_SCOPE)

else()

add_library(${YAL_LIB_PARSER} STATIC
    generated/yal_bison_parser.hpp
    generated/yal_bison_parser.cpp
    generated/yal_flex_lexer.h
    generated/yal_flex_lexer.cpp
)

set(YAL_PARSER_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(YAL_PARSER_INCLUDE_DIRS ${YAL_PARSER_INCLUDE_DIRS} PARENT_SCOPE)


endif()
