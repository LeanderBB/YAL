#
# YAL C Backend Tests
#



set(BACKEND_C_BUILD_DIR "\"${CMAKE_CURRENT_BINARY_DIR}/build_dir\"")

if (MSVC)
    set(BACKED_C_ISMSVC 1)
else()
    set(BACKEND_C_COMPILER_BIN "\"/usr/bin/clang\"")
endif()
set(BACKEND_C_PROJ_ROOT_DIR "\"${CMAKE_CURRENT_SOURCE_DIR}\"")

set(CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/build_config/)
set(CONFIG_FILE ${CONFIG_INCLUDE_DIR}/config.h)
configure_file(config.h.cmake ${CONFIG_FILE})


add_executable(test_yal_backend_c
    fixture.h
    ${CONFIG_FILE}
    main_test_backendc.cpp
    test_backendc_import.cpp
)

target_include_directories(test_yal_backend_c
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CONFIG_INCLUDE_DIR}"
    )

target_link_libraries(test_yal_backend_c
        PRIVATE GTest::GTest yal::frontend yal::backend::c
        PRIVATE compile_options compile_options_werror
        )

add_test(NAME "backend-c" COMMAND test_yal_backend_c)
