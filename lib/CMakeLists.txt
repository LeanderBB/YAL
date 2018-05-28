#
# YAL Libraries
# 


option(ENABLE_BACKEND_C "Enabled the C transpiler backend" ON)

add_subdirectory(yal-core)
add_subdirectory(yal-frontend)

if (${ENABLE_BACKEND_C})
    add_subdirectory(yal-backend-c)
endif()
