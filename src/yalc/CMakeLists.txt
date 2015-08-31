#
# YALC - Yet Another Language Compiler
#

set(YALC_NAME yalc)


set(YALC_SRC
    main.cpp
)

include_directories(${YAL_INCLUDE_DIRS})
add_executable(${YALC_NAME} ${YALC_SRC})
target_link_libraries(${YALC_NAME} ${YAL_CORE_LIBRARIES} ${YAL_PARSER_LIB} ${YAL_CORE_LIBRARIES})

install(TARGETS ${YALC_NAME} DESTINATION bin)
