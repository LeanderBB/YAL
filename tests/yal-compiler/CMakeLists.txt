#
# Simple Parser test
#
add_executable(test_yal_compiler
    fixture.h
    test_general.cpp
    test_stageexprtype.cpp
    test_stagemovecheck.cpp
    test_stagereturncheck.cpp
    main_test_compiler.cpp)

target_link_libraries(test_yal_compiler
        PRIVATE GTest::GTest yal
    )
YAL_APPLY_COMPILER_FLAGS_WERROR(test_yal_compiler)
