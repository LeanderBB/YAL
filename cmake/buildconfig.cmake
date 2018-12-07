#
# Build Configuration settings
#

set(YAL_OS_DEFINES)

if (UNIX)
    set(YAL_OS_DEFINES ${YAL_OS_DEFINES} "YAL_OS_UNIX")
    set(YAL_BUILD_UNIX TRUE)
endif()

if (UNIX AND NOT APPLE)
    set(YAL_BUILD_LINUX TRUE)
    set(YAL_OS_DEFINES ${YAL_OS_DEFINES} "YAL_OS_LINUX")
endif()

if (APPLE)
    set(YAL_OS_DEFINES ${YAL_OS_DEFINES} "YAL_OS_UNIX YAL_OS_APPLE")
    set(YAL_BUILD_APPLE TRUE)
endif()

if (WIN32)
    set(YAL_OS_DEFINES ${YAL_OS_DEFINES} "YAL_OS_WIN32")
    set(YAL_BUILD_WIN32 TRUE)
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (NOT MSVC) # Clang and GCC
    # Common flags
    set(YAL_NO_WERROR_FLAG -Wno-error)
    set(YAL_WERROR_FLAG -Werror)
    set(YAL_DISABLE_WARNING_UNUSED_VAR "-Wno-unused-parameter")
    set(YAL_C_FLAGS -std=c99 -Wall -Wextra -pedantic)
    set(YAL_CXX_FLAGS -Wall -Wextra -std=c++17 -fno-exceptions -fno-rtti)

    set(YAL_DEFINITIONS ${YAL_OS_DEFINES})
    # Release flags
     set(YAL_DEFINITIONS_RELEASE "YAL_RELEASE")
    # Debug flags
    set(YAL_CXX_FLAGS_DEBUG -fno-inline -g -O0 -fno-omit-frame-pointer)
    set(YAL_C_FLAGS_DEBUG -g -O0)
    set(YAL_DEFINITIONS_DEBUG "YAL_DEBUG")

else() # MSVC
    # Common flags
    set(YAL_WERROR_FLAG /WX)
    set(YAL_DEFINITIONS WIN32 _WINDOWS _CRT_SECURE_NO_WARNINGS ${YAL_OS_DEFINES})
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



add_library(compile_options INTERFACE)

target_compile_definitions(compile_options
    INTERFACE ${YAL_DEFINITIONS}
    $<$<CONFIG:DEBUG>:${YAL_DEFINITIONS_DEBUG}>
    $<$<CONFIG:RELEASE>:${YAL_DEFINITIONS_RELEASE}>
    )

target_compile_options(compile_options
    INTERFACE ${YAL_CXX_FLAGS}
    $<$<CONFIG:DEBUG>:${YAL_CXX_FLAGS_DEBUG}>
    $<$<CONFIG:RELEASE>:${YAL_CXX_FLAGS_RELEASE}>
    )

add_library(compile_options_werror INTERFACE)

target_compile_options(compile_options_werror
    INTERFACE ${YAL_WERROR_FLAG}
    )


macro(YAL_APPLY_COMPILER_FLAGS_WERROR TARGET)
    YAL_APPLY_COMPILER_FLAGS_IMPL(${TARGET} ON)
endmacro()

macro(YAL_APPLY_COMPILER_FLAGS_NOWERROR TARGET)
    YAL_APPLY_COMPILER_FLAGS_IMPL(${TARGET} OFF)
endmacro()

macro(YAL_REMOVE_WERROR_FROM_SOURCE SOURCE)
    set_property(SOURCE ${SOURCE} APPEND PROPERTY COMPILE_FLAGS  "${YAL_NO_WERROR_FLAG}")
endmacro()
