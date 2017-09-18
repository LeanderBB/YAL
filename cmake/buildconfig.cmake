#
# Build Configuration settings
#

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (NOT MSVC) # Clang and GCC
    # Common flags
    set(YAL_C_FLAGS -std=c99 -Wall -Wextra -Werror -pedantic)
    set(YAL_CXX_FLAGS -Wall -Wextra -std=c++14 -fexceptions -fno-rtti)

    set(YAL_DEFINITIONS)
    # Release flags
     set(YAL_DEFINITIONS_RELEASE "YAL_RELEASE")
    # Debug flags
    set(YAL_CXX_FLAGS_DEBUG -fno-inline)
    set(YAL_C_FLAGS_DEBUG "")
    set(YAL_DEFINITIONS_DEBUG "YAL_DEBUG")

else() # MSVC
    # Common flags
    set(YAL_DEFINITIONS WIN32 _WINDOWS _CRT_SECURE_NO_WARNINGS)
    set(YAL_C_FLAGS /Wall /WX)
    set(YAL_C_WARNING_FLAGS /W2)
    set(YAL_CXX_FLAGS /W2 /WX /EHsc)

     # Release flags
     set(YAL_DEFINITIONS_RELEASE NDEBUG)
    set(YAL_CXX_FLAGS_RELEASE /O2 /Oy- /Gm- /GS /MD /Ob2 /Gd )
    set(YAL_C_FLAGS_RELEASE /O2 /Oy- /Gm- /GS /MD /Ob2 /Gd )

    # Debug flags
    set(YAL_DEFINITIONS_DEBUG DYAL_DEBUG _DEBUG)
    set(YAL_C_FLAGS_DEBUG /Oy- /Od /Ob0 /Gm- /RTC1 /Zi)
    set(YAL_CXX_FLAGS_DEBUG /Oy- /Od /Ob0 /Gm- /RTC1 /Zi)

endif()



macro(YAL_APPLY_COMPILER_FLAGS TARGET)

    target_compile_definitions( ${TARGET}
        PRIVATE ${YAL_DEFINITIONS}
        PRIVATE $<$<CONFIG:DEBUG>:${YAL_DEFINITIONS_DEBUG}>
        PRIVATE $<$<CONFIG:RELEASE>:${YAL_DEFINITIONS_RELEASE}>
        )

    target_compile_options(${TARGET}
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${YAL_CXX_FLAGS}>
        PRIVATE $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>:${YAL_CXX_FLAGS_DEBUG}>
        PRIVATE $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>:${YAL_CXX_FLAGS_RELEASE}>

        PRIVATE $<$<COMPILE_LANGUAGE:C>:${YAL_C_FLAGS}>
        PRIVATE $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>:${YAL_C_FLAGS_DEBUG}>
        PRIVATE $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>:${YAL_C_FLAGS_RELEASE}>
        )

endmacro()
