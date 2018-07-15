#
# YAL Libraries
# 


option(ENABLE_BACKEND_C "Enabled the C transpiler backend" ON)

add_subdirectory(core)
add_subdirectory(frontend)

if (${ENABLE_BACKEND_C})
    add_subdirectory(backend-c)
endif()
