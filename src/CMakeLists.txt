#
# YAL Source Files
#

include(lexerparsertools)

set(YAL_VERSION_MAJOR 0)
set(YAL_VERSION_MINOR 1)
set(YAL_VERSION_PATCH 0)
set(YAL_VERSION_STR "\"${YAL_VERSION_MAJOR}.${YAL_VERSION_MINOR}.${YAL_VERSION_PATCH}\"")
set(YAL_CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/)
set(YAL_CONFIG_FILE ${YAL_CONFIG_INCLUDE_DIR}/yal/yalconfig.h)
configure_file(yal/yalconfig.h.cmake ${YAL_CONFIG_FILE})


set(YAL_LIB_NAME yal)


set(YAL_SOURCES
    yal/yal.h
    ${YAL_CONFIG_FILE}

    yal/ast/module.h
    yal/ast/module.cpp
    yal/ast/modulemanager.h
    yal/ast/modulemanager.cpp
    yal/ast/astcontext.h
    yal/ast/astcontext.cpp
    yal/ast/declbase.h
    yal/ast/declbase.cpp
    yal/ast/declbasetypes.def
    yal/ast/asttypes.h
    yal/ast/astnodes.def
    yal/ast/declfunction.h
    yal/ast/declfunction.cpp
    yal/ast/type.h
    yal/ast/type.cpp
    yal/ast/typelist.def
    yal/ast/typebuiltin.cpp
    yal/ast/typebuiltin.h
    yal/ast/typecontext.cpp
    yal/ast/typecontext.h
    yal/ast/typefunction.cpp
    yal/ast/typefunction.h
    yal/ast/declmodule.cpp
    yal/ast/declmodule.h
    yal/ast/refbase.h
    yal/ast/refbase.cpp
    yal/ast/refbasetypes.def
    yal/ast/reftype.cpp
    yal/ast/reftype.h
    yal/ast/reftypebuiltin.cpp
    yal/ast/reftypebuiltin.h

    yal/io/memorystream.cpp
    yal/io/memorystream.h
    yal/io/bytestream.cpp
    yal/io/bytestream.h
    yal/io/filestream.cpp
    yal/io/filestream.h
    yal/io/sourcemanager.cpp
    yal/io/sourcemanager.h
    yal/io/sourceitems.cpp
    yal/io/sourceitems.h

    yal/lexer/lexer.h
    yal/lexer/lexer.cpp
    yal/lexer/tokens.h
    yal/lexer/tokens.cpp
    yal/lexer/lexer_re2c.in
    yal/lexer/lexer_re2c.cpp

    yal/parser/parser.cpp
    yal/parser/parser.h
    yal/parser/parsertokens.h
    yal/parser/parserimpl.lemon
    yal/parser/parserimpl.cpp
    yal/parser/parserimpl.h

    yal/util/log.h
    yal/util/log.cpp
    yal/util/prettyprint.cpp
    yal/util/prettyprint.h
    yal/util/allocatorstack.cpp
    yal/util/allocatorstack.h
    yal/util/stringref.cpp
    yal/util/stringref.h
)

# include parser / lexer DSL's
set_source_files_properties(yal/lexer/lexer_re2c.in
    PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(yal/parser/parser.lemon
    PROPERTIES HEADER_FILE_ONLY TRUE)

# include .def files
set_source_files_properties(yal/parser/astnodes.def
    PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(yal/parser/declbasetypes.def
    PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(yal/parser/typelist.def
    PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(yal/parser/refbasetypes.def
    PROPERTIES HEADER_FILE_ONLY TRUE)




#if re2c is availabe use custom targets
if (${RE2C_TOOLS_FOUND})
    RE2C_GEN(
        "${CMAKE_CURRENT_SOURCE_DIR}/yal/lexer/lexer_re2c.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/yal/lexer/lexer_re2c.cpp")
endif()

#if lemon is availabe use custom targets
if (${LEMON_TOOLS_FOUND})
    LEMON_GEN(
        "${CMAKE_CURRENT_SOURCE_DIR}/yal/parser/"
        parserimpl.lemon
        parserimpl.cpp)
endif()

add_library(${YAL_LIB_NAME} STATIC
    ${YAL_SOURCES}
    )


target_include_directories( ${YAL_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${YAL_CONFIG_INCLUDE_DIR}
    )

YAL_APPLY_COMPILER_FLAGS_WERROR(${YAL_LIB_NAME})
YAL_REMOVE_WERROR_FROM_SOURCE(yal/parser/parserimpl.cpp)
