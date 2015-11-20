#
# YALE - Yet Another Language vmbinary executor
#

set(YALE_NAME yale)


set(YALE_SRC
    main.cpp
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${YALVM_LINKER_FLAGS}")

get_target_property(YALVM_INCLUDE_DIRS yalvm INCLUDE_DIRECTORIES)
include_directories(${YAL_INCLUDE_DIRS} ${YALVM_INCLUDE_DIRS})

add_executable(${YALE_NAME} ${YALE_SRC})
target_link_libraries(${YALE_NAME} ${YAL_CORE_LIBRARIES} ${YALVM_LIBRARIES})

install(TARGETS ${YALE_NAME} DESTINATION bin)
