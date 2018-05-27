#
# Small exec to test process calling
#


set (SOURCES
    invoke.cpp
    )

set(EXEC_NAME "invoke")

add_executable(${EXEC_NAME} ${SOURCES})

add_executable(redirect redirect.cpp)

target_link_libraries(${EXEC_NAME}
        PRIVATE yal::libyal
        PRIVATE compile_options compile_options_werror
    )

