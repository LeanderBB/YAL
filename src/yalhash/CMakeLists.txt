#
# YALHASH - Yet Another Language hash generator for variable names
#

set(YALHASH_NAME yalhash)


set(YALHASH_SRC
    main.cpp
)

get_target_property(YALVM_INCLUDE_DIRS yalvm INCLUDE_DIRECTORIES)
include_directories(${YAL_INCLUDE_DIRS} ${YALVM_INCLUDE_DIRS})

add_executable(${YALHASH_NAME} ${YALHASH_SRC})
target_link_libraries(${YALHASH_NAME} ${YAL_CORE_LIBRARIES} ${YALVM_LIBRARIES})

install(TARGETS ${YALHASH_NAME} DESTINATION bin)
