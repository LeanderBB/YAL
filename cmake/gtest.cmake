#
# GTest Config
#
include(ExternalProject)

# Download and build gtest framework
ExternalProject_Add( gtestproj
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG "release-1.8.0"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/external/gtest/1.8.0"
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DBUILD_GMOCK=OFF -DBUILD_GTEST=ON
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(gtestproj source_dir binary_dir)

find_package(THREADS REQUIRED)
add_library(gtest INTERFACE)
add_dependencies(gtest gtestproj)
target_link_libraries(gtest INTERFACE ${binary_dir}/googletest/libgtest.a ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(gtest INTERFACE ${source_dir}/googletest/include)
