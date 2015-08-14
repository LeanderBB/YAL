#
# YAL VM - Virtual Machine
#

set(YALVM_LIB_NAME yalvm)


set(YALVM_VERSION_MAJOR 0)
set(YALVM_VERSION_MINOR 3)
set(YALVM_VERSION_PATCH 0)
set(YALVM_VERSION_STR "\"${YALVM_VERSION_MAJOR}.${YALVM_VERSION_MINOR}.${YALVM_VERSION_PATCH}\"")

set(YALVM_CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/)
set(YALVM_CONFIG_FILE ${YALVM_CONFIG_INCLUDE_DIR}/yalvm/yalvm_config.h)
configure_file(yalvm_config.h.cmake ${YALVM_CONFIG_FILE})

include_directories(${YALVM_CONFIG_INCLUDE_DIR})

set(YALVM_HDR
    yalvm.h
    yalvm_bytecode.h
    yalvm_binary.h
    yalvm_hashing.h
    yalvm_register.h
    yalvm_ctx.h
    yalvm_stack.h
    yalvm_external.h
    yalvm_error.h
    ${YALVM_CONFIG_FILE}
)

set(YALVM_SRC
    yalvm_bytecode.c
    yalvm_binary.c
    yalvm_hashing.c
    yalvm_ctx.c
    yalvm_stack.c
    yalvm_error.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${YAL_C_FLAGS} ${YAL_C_WERROR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${YAL_CXX_WERROR}")

add_library(${YALVM_LIB_NAME} ${YALVM_HDR} ${YALVM_SRC})

set(YALVM_INCLUDE_DIRS ${YALVM_CONFIG_INCLUDE_DIR})
set(YALVM_INCLUDE_DIRS ${YALVM_CONFIG_INCLUDE_DIR} PARENT_SCOPE)
