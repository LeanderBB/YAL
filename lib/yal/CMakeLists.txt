#
# YAL - YAL support lib
#

set(YAL_HDR
    parser/bison_utils.h
    parser/flex_utils.h
    parser/parser_state.h
    memory/memory_utils.h
    util/str_utils.h
    util/stl_utils.h
    util/outputsink.h
    util/inputsink.h
    util/filesink.h
    util/stringinputsink.h
    util/outputformater.h
    util/errorhandler.h
    util/constantvalue.h
    util/memorysink.h
    util/argparser.h
    ast/astbasenode.h
    ast/astnodevisitor.h
    ast/variabledeclnode.h
    ast/constantnode.h
    ast/dualoperatornode.h
    ast/singleoperatornode.h
    ast/compareoperatornode.h
    ast/conditionnode.h
    ast/codebodynode.h
    ast/assignoperatornode.h
    ast/astnodetypes.h
    ast/astnodelist.h
    ast/argumentdeclnode.h
    ast/functionnode.h
    ast/returnnode.h
    ast/printnode.h
    ast/whileloopnode.h
    symbols/symboltable.h
    ast/astprinter.h
    symbols/symboltable.h
    symbols/symboltreebuilder.h
    symbols/type_utils.h
    types/type.h
    types/typelist.h
    types/builtintype.h
    types/functiontype.h
    types/typeregistry.h
    types/undefined.h
    bytecode/bytecodebuffer.h
    bytecode/registerallocator.h
    bytecode/bytecodebuilder.h
    bytecode/bytecodegenerator.h
    bytecode/bytecode_utils.h
    bytecode/bytecodeprinter.h
    compiler/compiler.h
    module/module.h
    module/moduleindexable.h
    module/moduleglobal.h
    module/modulefunction.h
    module/moduleconstant.h
    yal.h
    enums.h
    ${YAL_CONFIG_FILE}
)

set(YAL_SRC
    parser/bison_utils.cpp
    parser/flex_utils.cpp
    memory/memory_utils.cpp
    util/str_utils.cpp
    util/filesink.cpp
    util/outputformater.cpp
    util/stringinputsink.cpp
    util/constantvalue.cpp
    util/memorysink.cpp
    util/argparser.cpp
    ast/astbasenode.cpp
    ast/variabledeclnode.cpp
    ast/constantnode.cpp
    ast/dualoperatornode.cpp
    ast/singleoperatornode.cpp
    ast/compareoperatornode.cpp
    ast/conditionnode.cpp
    ast/codebodynode.cpp
    ast/assignoperatornode.cpp
    ast/astnodetypes.cpp
    ast/argumentdeclnode.cpp
    ast/functionnode.cpp
    ast/returnnode.cpp
    ast/astprinter.cpp
    ast/printnode.cpp
    ast/whileloopnode.cpp
    symbols/symbol.cpp
    symbols/symboltable.cpp
    symbols/symboltreebuilder.cpp
    symbols/type_utils.cpp
    types/type.cpp
    types/builtintype.cpp
    types/functiontype.cpp
    types/typeregistry.cpp
    types/undefined.cpp
    bytecode/bytecodebuffer.cpp
    bytecode/registerallocator.cpp
    bytecode/bytecodebuilder.cpp
    bytecode/bytecodegenerator.cpp
    bytecode/bytecode_utils.cpp
    bytecode/bytecodeprinter.cpp
    compiler/compiler.cpp
    module/module.cpp
    module/moduleconstant.cpp
    module/moduleglobal.cpp
    module/modulefunction.cpp
    enums.cpp
)

include_directories(${YAL_PARSER_INCLUDE_DIRS} ${YALVM_INCLUDE_DIRS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${YAL_C_FLAGS} ${YAL_C_WERROR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${YAL_CXX_WERROR}")

add_library(${YAL_LIB_CORE} STATIC ${YAL_SRC} ${YAL_HDR})

target_link_libraries(${YAL_LIB_CORE} ${YAL_LIB_PARSER})

add_dependencies(${YAL_LIB_CORE} ${YAL_LIB_PARSER})
add_dependencies(${YAL_LIB_CORE} yalvm)

