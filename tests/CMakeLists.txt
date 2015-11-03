#
# Test directory
#

include(ExternalProject)

# Download and build gtest framework
ExternalProject_Add( gtest
    SVN_REPOSITORY "http://googletest.googlecode.com/svn/tags/release-1.7.0"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gtest"
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
)


ExternalProject_Get_Property(gtest source_dir binary_dir)
include_directories(${source_dir}/include)
link_directories(${binary_dir})
#include_directories(/usr/local/include)
#link_directories(/usr/local/lib/${binary_dir})
set(GTEST_LIBRARIES  libgtest.a libgtest_main.a)

add_definitions("-DYAL_TEST_SOURCES_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/test_files\"")

# test macro_util
macro(YAL_TEST_DEP TARGET)
include_directories(${YAL_INCLUDE_DIRS})
target_link_libraries(${TARGET} ${YAL_CORE_LIBRARIES} ${YALVM_LIBRARIES} ${GTEST_LIBRARIES})
target_link_libraries(${TARGET} ${CMAKE_THREAD_LIBS_INIT} pthread)
add_dependencies(${TARGET} gtest)
add_test(NAME ${TARGET}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endmacro()


add_subdirectory(compile_tests)
add_subdirectory(execution_tests)
