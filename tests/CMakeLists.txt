#
# YAL Unit Tests
#

enable_testing()
include(gtest)
add_subdirectory(gtest_utils)
add_subdirectory(yal-lexer)
add_subdirectory(yal-frontend)
add_subdirectory(yal-backend-c)
add_subdirectory(util)
add_subdirectory(process)
