# YAL - Library directory

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


set(YAL_LIB_CORE yalcore)
set(YAL_LIB_PARSER yalparser)
set(YAL_LIB_VM yalvm)

#FIXME: Move this into yal subdir at some point
set(YAL_VERSION_MAJOR 0)
set(YAL_VERSION_MINOR 1)
set(YAL_VERSION_PATCH 0)
set(YAL_CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/)
set(YAL_CONFIG_FILE ${YAL_CONFIG_INCLUDE_DIR}/yal/config.h)
configure_file(yal/config.h.cmake ${YAL_CONFIG_FILE})
include_directories(${YAL_CONFIG_INCLUDE_DIR})


add_subdirectory(yalvm)

#FIXME: Move this into yal subdir at some point
add_subdirectory(parser)
add_subdirectory(yal)


set(YAL_CORE_LIBRARIES  ${YAL_LIB_PARSER} ${YAL_LIB_CORE} ${YAL_LIB_VM})
set(YAL_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${YAL_CONFIG_INCLUDE_DIR}
    ${YAL_PARSER_INCLUDE_DIRS}
    ${YALVM_INCLUDE_DIRS}
)

set(YAL_INCLUDE_DIRS ${YAL_INCLUDE_DIRS} PARENT_SCOPE)
set(YAL_CORE_LIBRARIES ${YAL_CORE_LIBRARIES} PARENT_SCOPE)
