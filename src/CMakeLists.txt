#
# Exec sources
#



# Yal Compiler

set(YALC_BIN yalc)
set(YALC_SOURCES
      yalc/main_yalc.cpp
)

add_executable(${YALC_BIN} ${YALC_SOURCES})
YAL_APPLY_COMPILER_FLAGS_WERROR(${YALC_BIN})

target_link_libraries(${YALC_BIN}
        PRIVATE yal
    )
