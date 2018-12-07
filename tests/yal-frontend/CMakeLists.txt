#
# YAL Frontend Tests
#
add_executable(test_yal_frontend
    fixture.h
    #test_stagesizecalc.cpp
    passes/test_passparser.cpp
    passes/decl/test_predecl.cpp
    passes/decl/test_predecl_alias.cpp
    passes/decl/test_predecl_import.cpp
    passes/decl/test_astbuilder.cpp
    passes/decl/test_astbuilder_import.cpp
    passes/fnret/test_fnret.cpp
    passes/type/test_passtype.cpp
    passes/type/test_passtype_alias.cpp
    passes/move/test_passmove.cpp
    main_test_compiler.cpp)

target_include_directories(test_yal_frontend
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    )

target_link_libraries(test_yal_frontend
        PRIVATE GTest::GTest yal::frontend
        PRIVATE compile_options compile_options_werror
        )

add_test(NAME "frontend" COMMAND test_yal_frontend)
