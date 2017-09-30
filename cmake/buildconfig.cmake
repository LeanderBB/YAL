#
# Build Configuration settings
#

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (NOT MSVC) # Clang and GCC
    # Common flags
    set(YAL_WERROR_FLAG -Werror)
    set(YAL_DISABLE_WARNING_UNUSED_VAR "-Wno-unused-parameter")
    set(YAL_C_FLAGS -std=c99 -Wall -Wextra -pedantic)
    set(YAL_CXX_FLAGS -Wall -Wextra -std=c++14 -fexceptions -fno-rtti)

    set(YAL_DEFINITIONS)
    # Release flags
     set(YAL_DEFINITIONS_RELEASE "YAL_RELEASE")
    # Debug flags
    set(YAL_CXX_FLAGS_DEBUG -fno-inline -g -O0)
    set(YAL_C_FLAGS_DEBUG -g -O0)
    set(YAL_DEFINITIONS_DEBUG "YAL_DEBUG")

else() # MSVC
    # Common flags
    set(YAL_WERROR_FLAG /WX)
    set(YAL_DEFINITIONS WIN32 _WINDOWS _CRT_SECURE_NO_WARNINGS)
    set(YAL_C_FLAGS /Wall)
    set(YAL_C_WARNING_FLAGS /W2)
    set(YAL_CXX_FLAGS /W2 /EHsc)

     # Release flags
     set(YAL_DEFINITIONS_RELEASE NDEBUG)
    set(YAL_CXX_FLAGS_RELEASE /O2 /Oy- /Gm- /GS /MD /Ob2 /Gd )
    set(YAL_C_FLAGS_RELEASE /O2 /Oy- /Gm- /GS /MD /Ob2 /Gd )

    # Debug flags
    set(YAL_DEFINITIONS_DEBUG DYAL_DEBUG _DEBUG)
    set(YAL_C_FLAGS_DEBUG /Oy- /Od /Ob0 /Gm- /RTC1 /Zi)
    set(YAL_CXX_FLAGS_DEBUG /Oy- /Od /Ob0 /Gm- /RTC1 /Zi)

endif()



macro(YAL_APPLY_COMPILER_FLAGS_IMPL TARGET WERROR)

    target_compile_definitions( ${TARGET}
        PUBLIC ${YAL_DEFINITIONS}
        PUBLIC $<$<CONFIG:DEBUG>:${YAL_DEFINITIONS_DEBUG}>
        PUBLIC $<$<CONFIG:RELEASE>:${YAL_DEFINITIONS_RELEASE}>
        )

    target_compile_options(${TARGET}
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${YAL_CXX_FLAGS}>
        PRIVATE $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>:${YAL_CXX_FLAGS_DEBUG}>
        PRIVATE $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>:${YAL_CXX_FLAGS_RELEASE}>

        PRIVATE $<$<COMPILE_LANGUAGE:C>:${YAL_C_FLAGS}>
        PRIVATE $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>:${YAL_C_FLAGS_DEBUG}>
        PRIVATE $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>:${YAL_C_FLAGS_RELEASE}>
        )

    if (${WERROR})
        target_compile_options(${TARGET}
            PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${YAL_WERROR_FLAG}>
            PRIVATE $<$<COMPILE_LANGUAGE:C>:${YAL_WERROR_FLAG}>
            )
    endif()

endmacro()


macro(YAL_APPLY_COMPILER_FLAGS_WERROR TARGET)
    YAL_APPLY_COMPILER_FLAGS_IMPL(${TARGET} ON)
endmacro()

macro(YAL_APPLY_COMPILER_FLAGS_NOWERROR TARGET)
    YAL_APPLY_COMPILER_FLAGS_IMPL(${TARGET} OFF)
endmacro()

macro(YAL_APPEND_SOURCE_FILE_COMPILE_FLAG SOURCE FLAG)
    message(" set_property(SOURCE ${SOURCE} APPEND_STRING PROPERTY COMPILE_FLAGS \" ${FLAG} \")")
    set_property(SOURCE ${SOURCE} APPEND_STRING PROPERTY COMPILE_FLAGS " ${FLAG} ")
endmacro()
