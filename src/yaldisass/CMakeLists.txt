#
# YALDISASS - Yet Another Language vmbinary disassembler
#

set(YALDISASS_NAME yaldisass)


set(YALDISASS_SRC
    main.cpp
)

include_directories(${YAL_INCLUDE_DIRS})
add_executable(${YALDISASS_NAME} ${YALDISASS_SRC})
target_link_libraries(${YALDISASS_NAME} ${YAL_CORE_LIBRARIES} ${YALVM_LIBRARIES})

install(TARGETS ${YALDISASS_NAME} DESTINATION bin)
