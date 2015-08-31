#
# YALE - Yet Another Language vmbinary executor
#

set(YALE_NAME yale)


set(YALE_SRC
    main.cpp
)

get_target_property(YALVM_INCLUDE_DIRS yalvm INCLUDE_DIRECTORIES)
include_directories(${YAL_INCLUDE_DIRS} ${YALVM_INCLUDE_DIRS})

add_executable(${YALE_NAME} ${YALE_SRC})
target_link_libraries(${YALE_NAME} ${YAL_CORE_LIBRARIES} ${YAL_PARSER_LIB} ${YAL_CORE_LIBRARIES})

install(TARGETS ${YALE_NAME} DESTINATION bin)
