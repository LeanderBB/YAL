#
# YAL VM - Virtual Machine
#

set(YALVM_LIB_NAME yalvm)


set(YALVM_VERSION_MAJOR 0)
set(YALVM_VERSION_MINOR 4)
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
    yalvm_object.h
    yalvm_array.h
    yalvm_string.h
    yalvm_builtin.h
    ${YALVM_CONFIG_FILE}
)

set(YALVM_SRC
    yalvm_bytecode.c
    yalvm_binary.c
    yalvm_hashing.c
    yalvm_ctx.c
    yalvm_stack.c
    yalvm_error.c
    yalvm_object.c
    yalvm_array.c
    yalvm_string.c
    yalvm_builtin.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${YAL_C_FLAGS} ${YAL_C_WERROR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${YAL_CXX_WERROR}")

add_library(${YALVM_LIB_NAME} STATIC ${YALVM_HDR} ${YALVM_SRC})

if (UNIX AND NOT APPLE)
    target_link_libraries(${YALVM_LIB_NAME} "dl")
endif()

set(YALVM_INCLUDE_DIRS ${YALVM_CONFIG_INCLUDE_DIR})
set(YALVM_INCLUDE_DIRS ${YALVM_CONFIG_INCLUDE_DIR} PARENT_SCOPE)

if (UNIX AND NOT APPLE)
    set(YALVM_LIBRARIES -Wl,--whole-archive  ${YALVM_LIB_NAME} -Wl,--no-whole-archive)
elseif(APPLE)
    set(YALVM_LIBRARIES -all_load  ${YALVM_LIB_NAME})
else()
    set(YALVM_LIBRARIES ${YALMV_LIB_NAME})
endif()
set(YALVM_LIBRARIES ${YALVM_LIBRARIES} PARENT_SCOPE)
#set(YALVM_LINKER_FLAGS "-u yal_native_array_any_length_uint32")
#set(YALVM_LINKER_FLAGS ${YALVM_LINKER_FLAGS} PARENT_SCOPE)
