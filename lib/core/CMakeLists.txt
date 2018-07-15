#
# YAL Backend C - C Transpiler
#

set(YAL_CORE_VERSION_MAJOR 0)
set(YAL_CORE_VERSION_MINOR 1)
set(YAL_CORE_VERSION_PATCH 0)

project(yal-core
    VERSION ${YAL_CORE_VERSION_MAJOR}.${YAL_CORE_VERSION_MINOR}.${YAL_CORE_VERSION_PATCH}
    LANGUAGES CXX)


set(YAL_CORE_VERSION_STR
    "\"${YAL_CORE_VERSION_MAJOR}.${YAL_CORE_VERSION_MINOR}.${YAL_CORE_VERSION_PATCH}\"")

set(YAL_CORE_CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/)
set(YAL_CORE_CONFIG_FILE ${YAL_CORE_CONFIG_INCLUDE_DIR}/yal/yalconfig.h)
configure_file(yal/yalconfig.h.cmake ${YAL_CORE_CONFIG_FILE})

set(LIB_NAME yal-core)


set(LIB_SOURCES
    yal/yal.h
    ${YAL_CORE_CONFIG_FILE}

    yal/error/error.h
    yal/error/error.cpp
    yal/error/errorreporter.cpp
    yal/error/errorreporter.h
    yal/error/errorprinter.cpp
    yal/error/errorprinter.h

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
    yal/io/ansicolorcodes.h

    yal/os/process.h
    yal/os/process.cpp
    yal/os/path.h
    yal/os/path.cpp

    yal/util/log.h
    yal/util/log.cpp
    yal/util/prettyprint.cpp
    yal/util/prettyprint.h
    yal/util/allocator/allocatorstack.cpp
    yal/util/allocator/allocatorstack.h
    yal/util/stringref.cpp
    yal/util/stringref.h
    yal/util/cast.h
    yal/util/format.h
    yal/util/format.cpp
    yal/util/formattypes.h
    yal/util/formathex.h
    yal/util/formathex.cpp
    yal/util/strconversions.h
    yal/util/strconversions.cpp
    yal/util/iteratorrange.h
    yal/util/stackjump.h
    yal/util/codewriter.cpp
    yal/util/codewriter.h
    yal/util/scopedstackelem.h
    yal/util/hash.cpp
    yal/util/hash.h
    yal/util/stringpool.cpp
    yal/util/stringpool.h
)


if (${YAL_BUILD_UNIX})
    set(LIB_SOURCES ${LIB_SOURCES}
        yal/os/unix/processunix.cpp
        yal/os/unix/processunix.h
        yal/os/unix/bytestreamunixfd.cpp
        yal/os/unix/bytestreamunixfd.h
        )
endif()

add_library(${LIB_NAME} STATIC
    ${LIB_SOURCES})

add_library(yal::core ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${YAL_CORE_CONFIG_INCLUDE_DIR})

target_link_libraries(${LIB_NAME}
    PUBLIC compile_options compile_options_werror)
