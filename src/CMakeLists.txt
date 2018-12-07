#
# Exec sources
#

set(YAL_VERSION_MAJOR 0)
set(YAL_VERSION_MINOR 1)
set(YAL_VERSION_PATCH 0)

project(yalc
    VERSION ${YAL_VERSION_MAJOR}.${YAL_VERSION_MINOR}.${YAL_VERSION_PATCH}
    LANGUAGES CXX)

# Yal Compiler

set(YALC_BIN yalc)
set(YALC_SOURCES
      yalc/main_yalc.cpp
)

add_executable(${YALC_BIN} ${YALC_SOURCES})

add_executable(yalc::yalc ALIAS yalc)

target_link_libraries(${YALC_BIN}
        PRIVATE yal::frontend yal::backend::c
        PRIVATE compile_options compile_options_werror
    )
