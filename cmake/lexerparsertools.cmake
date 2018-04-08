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

# locate lemon parser
find_program(LEMON_BIN lemon
    HINTS "/usr/bin" "/usr/local/bin"
    DOC "Path to lemon bin")


if (NOT "${LEMON_BIN}" STREQUAL "LEMON_BIN-NOTFOUND")
    # check re2c version
    execute_process(
        COMMAND "${LEMON_BIN}" "-x"
        RESULT_VARIABLE LEMON_VER_RESULT
        OUTPUT_VARIABLE LEMON_VER_OUT
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    if (${LEMON_VER_RESULT} EQUAL 0)
        if ("${LEMON_VER_OUT}" STREQUAL "Lemon version 1.0")

            # locate makeheaders
            find_program(MKHEADERS_BIN makeheaders
                HINTS "/usr/bin" "/usr/local/bin"
                DOC "Path to makeheaders bin")

            if (NOT "${MKHEADERS_BIN}" STREQUAL "MKHEADERS_BIN-NOTFOUND")
                message(STATUS "Found makeheaders: ${MKHEADERS_BIN}")
                message(STATUS "Found lemon: ${LEMON_BIN}")
                set(LEMON_TOOLS_FOUND True)
            else()
                message(WARNING "Found lemon, but failed to find makeheaders")
            endif()
        else()
            message(WARNING "Found ${LEMON_VER_OUT}, but expecting v1.0")
        endif()
    else()
        message(WARNING "Failed to get lemon version")
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


# Generate Lemon files,
# will also generate header file with INPUT.h
# WKDIR - Source directory for files
# INPUT - Input file with .lemon suffix
# OUTPUT - Output file
function(LEMON_GEN WKDIR INPUT OUTPUT)
    string(REPLACE ".lemon" ".c" LEMON-OUTPUT-SRC "${INPUT}")
    string(REPLACE ".lemon" ".h" LEMON-OUTPUT-HDR "${INPUT}")
    add_custom_command(
        OUTPUT "${WKDIR}/${OUTPUT}" "${WKDIR}/${LEMON-OUTPUT-HDR}"
        DEPENDS "${WKDIR}/${INPUT}"
        MAIN_DEPENDENCY "${WKDIR}/${INPUT}"
        COMMAND ${LEMON_BIN} -m "${INPUT}"
        COMMAND ${MKHEADERS_BIN} "${LEMON-OUTPUT-SRC}"
        COMMAND ${CMAKE_COMMAND} -E rename "${LEMON-OUTPUT-SRC}" "${OUTPUT}"
        COMMENT "Generating lemon source file for ${INPUT}..."
        WORKING_DIRECTORY "${WKDIR}")
endfunction()
