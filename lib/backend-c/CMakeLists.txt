#
# YAL Backend C - C Transpiler
#

set(YAL_BACKEND_C_VERSION_MAJOR 0)
set(YAL_BACKEND_C_VERSION_MINOR 1)
set(YAL_BACKEND_C_VERSION_PATCH 0)

project(yal-backend-c
    VERSION ${YAL_BACKEND_C_VERSION_MAJOR}.${YAL_BACKEND_C_VERSION_MINOR}.${YAL_BACKEND_C_VERSION_PATCH}
    LANGUAGES CXX)


set(YAL_BACKEND_C_VERSION_STR
    "\"${YAL_BACKEND_C_VERSION_MAJOR}.${YAL_BACKEND_C_VERSION_MINOR}.${YAL_BACKEND_C_VERSION_PATCH}\"")

set(YAL_BACKEND_C_CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/)
set(YAL_BACKEND_C_CONFIG_FILE ${YAL_BACKEND_C_CONFIG_INCLUDE_DIR}/yal/backendc/config.h)
configure_file(yal/backendc/config.h.cmake ${YAL_BACKEND_C_CONFIG_FILE})

set(LIB_NAME yal-backend-c)


set(LIB_SOURCES
    ${YAL_BACKEND_C_CONFIG_FILE}
    yal/backendc/backendc.cpp
    yal/backendc/backendc.h
    yal/backendc/ctype.cpp
    yal/backendc/ctype.h
    yal/backendc/errorsbackendc.cpp
    yal/backendc/errorsbackendc.h
    yal/backendc/astvisitorcheader.cpp
    yal/backendc/astvisitorcheader.h
    yal/backendc/astvisitorcsource.cpp
    yal/backendc/astvisitorcsource.h
    yal/backendc/ctypegen.cpp
    yal/backendc/ctypegen.h
    yal/backendc/ctypebuiltin.cpp
    yal/backendc/ctypebuiltin.h
)


add_library(${LIB_NAME} STATIC
    ${LIB_SOURCES})

add_library(yal::backend::c ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${YAL_BACKEND_C_CONFIG_INCLUDE_DIR})

target_link_libraries(${LIB_NAME}
    PUBLIC yal::frontend)
