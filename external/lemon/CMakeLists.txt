#
# Lemon Parser Tool
# Taken from SQLlite source tree
#

project(Ext_Lemon C)

set(LEMON_VERSION_DIR 3.24)

add_executable(lemon
    ${LEMON_VERSION_DIR}/lemon.c)

set(LEMON_TEMPLATE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${LEMON_VERSION_DIR}/lempar.c"
    CACHE STRING "Location of lemon template file" FORCE)

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
        DEPENDS lemon makeheaders
        MAIN_DEPENDENCY "${WKDIR}/${INPUT}"
        COMMAND $<TARGET_FILE:lemon> -m "${INPUT}" "-T${LEMON_TEMPLATE_FILE}"
        COMMAND $<TARGET_FILE:makeheaders> "${LEMON-OUTPUT-SRC}"
        COMMAND ${CMAKE_COMMAND} -E rename "${LEMON-OUTPUT-SRC}" "${OUTPUT}"
        COMMENT "Generating lemon source file for ${INPUT}..."
        WORKING_DIRECTORY "${WKDIR}"
        COMMAND_EXPAND_LISTS)
endfunction()
