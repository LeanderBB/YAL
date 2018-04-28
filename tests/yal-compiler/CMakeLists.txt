#
# Simple Parser test
#
add_executable(test_yal_compiler
    fixture.h
    #test_general.cpp
    #test_stageexprtype.cpp
    #test_stagemovecheck.cpp
    #test_stagereturncheck.cpp
    #test_stagesizecalc.cpp


    passes/test_passparser.cpp
    passes/decl/test_predecl.cpp
    passes/decl/test_astbuilder.cpp
    main_test_compiler.cpp)

target_include_directories(test_yal_compiler
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    )

target_link_libraries(test_yal_compiler
        PRIVATE GTest::GTest yal
    )
YAL_APPLY_COMPILER_FLAGS_WERROR(test_yal_compiler)
