#
# YAL Source Files
#


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

    yal/io/memorystream.cpp
    yal/io/memorystream.h
    yal/io/bytestream.cpp
    yal/io/bytestream.h
    yal/io/filestream.cpp
    yal/io/filestream.h

    yal/lexer/lexer.h
    yal/lexer/lexer.cpp
    yal/lexer/tokens.h
    yal/lexer/tokens.cpp
    yal/lexer/lexer_re2c.cpp
    yal/lexer/lexer_re2c.in
)

set_source_files_properties(yal/lexer/lexer_re2c.in
    PROPERTIES HEADER_FILE_ONLY TRUE)

add_library(${YAL_LIB_NAME} STATIC
    ${YAL_SOURCES}
    )

target_include_directories( ${YAL_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${YAL_CONFIG_INCLUDE_DIR}
    )


YAL_APPLY_COMPILER_FLAGS(${YAL_LIB_NAME})
