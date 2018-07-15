#
# Lexer & Parser Tools,
# Try to locate re2c, lemon and makeheaders binaries
#

# How to generate files
# lemon -m -l parserimpl.lemon  && makeheaders parserimpl.c && mv parserimpl.c parserimpl.cpp
# re2c -i -o lexer_re2c.cpp lexer_re2c.in

# locate re2c

set(RE2C_TOOLS_FOUND False)
set(LEMON_TOOLS_FOUND False)

find_program(RE2C_BIN re2c
    HINTS "/usr/bin" "/usr/local/bin"
    DOC "Path to re2c bin")


if (NOT "${RE2C_BIN}" STREQUAL "RE2C_BIN-NOTFOUND")

    # check re2c version
    execute_process(
        COMMAND "${RE2C_BIN}" "-v"
        RESULT_VARIABLE RE2C_VER_RESULT
        OUTPUT_VARIABLE RE2C_VER_OUT
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    if (${RE2C_VER_RESULT} EQUAL 0)
        if ("${RE2C_VER_OUT}" STREQUAL "re2c 1.0.2")
             message(STATUS "Found re2c: ${RE2C_BIN}")
             set(RE2C_TOOLS_FOUND True)
        else()
            message(WARNING "Found ${RE2C_VER_OUT}, but expecting v1.0.2")
        endif()
    else()
        message(WARNING "Failed to get re2c version")
    endif()
endif()


# Generate re2c source file
# INPUT - Full path to input file
# OUTUT - Full path to output file
function(RE2C_GEN INPUT OUTPUT)
    add_custom_command(
        OUTPUT "${OUTPUT}"
        DEPENDS "${INPUT}"
        MAIN_DEPENDENCY "${INPUT}"
        COMMAND ${RE2C_BIN} -i -o "${OUTPUT}" "${INPUT}"
        COMMENT "Generating re2c source file for ${INPUT}...")
endfunction()
