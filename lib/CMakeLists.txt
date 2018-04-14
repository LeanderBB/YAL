#
# YAL Source Files
#

include(lexerparsertools)

set(YAL_VERSION_MAJOR 0)
set(YAL_VERSION_MINOR 1)
set(YAL_VERSION_PATCH 0)
set(YAL_VERSION_STR "\"${YAL_VERSION_MAJOR}.${YAL_VERSION_MINOR}.${YAL_VERSION_PATCH}\"")
set(YAL_CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/)
set(YAL_CONFIG_FILE ${YAL_CONFIG_INCLUDE_DIR}/yal/yalconfig.h)
configure_file(yal/yalconfig.h.cmake ${YAL_CONFIG_FILE})


set(YAL_LIB_NAME yal)


set(YAL_SOURCES
    yal/yal.h
    ${YAL_CONFIG_FILE}

    yal/ast/astcontext.h
    yal/ast/astcontext.cpp
    yal/ast/astnodes.h
    yal/ast/declbase.h
    yal/ast/declbase.cpp
    yal/ast/declbasetypes.def
    yal/ast/asttypes.h
    yal/ast/astnodes.def
    yal/ast/declfunctionbase.h
    yal/ast/declfunctionbase.cpp
    yal/ast/declfunction.h
    yal/ast/declfunction.cpp
    yal/ast/decltypefunction.h
    yal/ast/decltypefunction.cpp
    yal/ast/type.h
    yal/ast/type.cpp
    yal/ast/typelist.def
    yal/ast/typebuiltin.cpp
    yal/ast/typebuiltin.h
    yal/ast/typecontext.cpp
    yal/ast/typecontext.h
    yal/ast/declmodule.cpp
    yal/ast/declmodule.h
    yal/ast/reftype.cpp
    yal/ast/reftype.h
    yal/ast/identifier.cpp
    yal/ast/identifier.h
    yal/ast/statement.h
    yal/ast/statement.cpp
    yal/ast/stmtexpression.cpp
    yal/ast/stmtexpression.h
    yal/ast/stmtreturn.cpp
    yal/ast/stmtreturn.h
    yal/ast/exprunaryoperator.cpp
    yal/ast/exprunaryoperator.h
    yal/ast/exprbinaryoperator.cpp
    yal/ast/exprbinaryoperator.h
    yal/ast/declvar.h
    yal/ast/declvar.cpp
    yal/ast/declparamvar.cpp
    yal/ast/declparamvar.h
    yal/ast/nodecontainer.h
    yal/ast/statementlist.h
    yal/ast/statementlist.cpp
    yal/ast/exprboolliteral.cpp
    yal/ast/exprboolliteral.h
    yal/ast/exprintegerliteral.h
    yal/ast/exprintegerliteral.cpp
    yal/ast/stmtdecl.cpp
    yal/ast/stmtdecl.h
    yal/ast/stmtassign.cpp
    yal/ast/stmtassign.h
    yal/ast/exprvarref.cpp
    yal/ast/exprvarref.h
    yal/ast/astvisitor.h
    yal/ast/astvisitor.cpp
    yal/ast/astprinter.cpp
    yal/ast/astprinter.h
    yal/ast/declstruct.h
    yal/ast/declstruct.cpp
    yal/ast/exprstructvarref.cpp
    yal/ast/exprstructvarref.h
    yal/ast/exprfncall.h
    yal/ast/exprfncall.cpp
    yal/ast/exprtypefncall.h
    yal/ast/exprtypefncall.cpp
    yal/ast/exprlist.h
    yal/ast/exprlist.cpp
    yal/ast/exprfloatliteral.cpp
    yal/ast/exprfloatliteral.h
    yal/ast/typedecl.h
    yal/ast/typedecl.cpp
    yal/ast/declscope.cpp
    yal/ast/declscope.h
    yal/ast/exprrangecast.h
    yal/ast/exprrangecast.cpp
    yal/ast/exprstructinit.cpp
    yal/ast/exprstructinit.h
    yal/ast/structmemberinit.h
    yal/ast/structmemberinit.cpp
    yal/ast/exprtypefncallstatic.h
    yal/ast/exprtypefncallstatic.cpp
    yal/ast/astsearch.cpp
    yal/ast/astsearch.h

    yal/compiler/compiler.cpp
    yal/compiler/compiler.h
    yal/compiler/stages/stagedecls.h
    yal/compiler/stages/stagedecls.cpp
    yal/compiler/stages/stageexprtype.cpp
    yal/compiler/stages/stageexprtype.h
    yal/compiler/stages/stagefnreturn.cpp
    yal/compiler/stages/stagefnreturn.h
    yal/compiler/stages/stagemovecheck.cpp
    yal/compiler/stages/stagemovecheck.h
    yal/compiler/stages/stagesizecalc.cpp
    yal/compiler/stages/stagesizecalc.h
    yal/compiler/stages/stageexprlistbreakdown.h
    yal/compiler/stages/stageexprlistbreakdown.cpp
    yal/compiler/stages/stageparser.h
    yal/compiler/stages/stageparser.cpp

    yal/error/error.h
    yal/error/error.cpp
    yal/error/errorreporter.cpp
    yal/error/errorreporter.h
    yal/error/errorprinter.cpp
    yal/error/errorprinter.h

    yal/io/memorystream.cpp
    yal/io/memorystream.h
    yal/io/bytestream.cpp
    yal/io/bytestream.h
    yal/io/filestream.cpp
    yal/io/filestream.h
    yal/io/sourcemanager.cpp
    yal/io/sourcemanager.h
    yal/io/sourceitems.cpp
    yal/io/sourceitems.h
    yal/io/ansicolorcodes.h

    yal/frontend/lexer/lexer.h
    yal/frontend/lexer/lexer.cpp
    yal/frontend/lexer/tokens.h
    yal/frontend/lexer/tokens.cpp
    yal/frontend/lexer/lexer_re2c.in
    yal/frontend/lexer/lexer_re2c.cpp
    yal/frontend/lexer/errorlexer.cpp
    yal/frontend/lexer/errorlexer.h

    yal/parser/parser.cpp
    yal/parser/parser.h
    yal/parser/parsertokens.h
    yal/parser/parserimpl.lemon
    yal/parser/parserimpl.cpp
    yal/parser/parserimpl.h
    yal/parser/parserhelper.h
    yal/parser/parserhelper.cpp

    yal/frontend/parser/syntaxtree.h
    yal/frontend/parser/syntaxtree.cpp
    yal/frontend/parser/syntaxtreetypes.def
    yal/frontend/parser/syntaxtreevisitor.h
    yal/frontend/parser/stdeclmodule.cpp
    yal/frontend/parser/stdeclmodule.h
    yal/frontend/parser/stdeclfunction.h
    yal/frontend/parser/stdeclfunction.cpp
    yal/frontend/parser/stdeclstruct.cpp
    yal/frontend/parser/stdeclstruct.h
    yal/frontend/parser/stdeclvar.h
    yal/frontend/parser/stdeclvar.cpp
    yal/frontend/parser/ststatement.h
    yal/frontend/parser/ststmtexpression.h
    yal/frontend/parser/ststmtreturn.h
    yal/frontend/parser/ststmtreturn.cpp
    yal/frontend/parser/ststmtdecl.cpp
    yal/frontend/parser/ststmtdecl.h
    yal/frontend/parser/ststmtassign.h
    yal/frontend/parser/ststmtassign.cpp
    yal/frontend/parser/stexprvarref.h
    yal/frontend/parser/stexprvarref.cpp
    yal/frontend/parser/stexproperators.cpp
    yal/frontend/parser/stexproperators.h
    yal/frontend/parser/stexprliterals.h
    yal/frontend/parser/stexprliterals.cpp
    yal/frontend/parser/stexprfncall.cpp
    yal/frontend/parser/stexprfncall.h
    yal/frontend/parser/stexprstructinit.cpp
    yal/frontend/parser/stexprstructinit.h
    yal/frontend/parser/stexprcasts.cpp
    yal/frontend/parser/stexprcasts.h
    yal/frontend/parser/stparserimpl.lemon
    yal/frontend/parser/stparser.cpp
    yal/frontend/parser/stparser.h
    yal/frontend/parser/stparserimpl.cpp
    yal/frontend/parser/stparserimpl.h
    yal/frontend/parser/sttype.h
    yal/frontend/parser/sttype.cpp
    yal/frontend/parser/errorparser.cpp
    yal/frontend/parser/errorparser.h

    yal/frontend/module.h
    yal/frontend/module.cpp
    yal/frontend/modulemanager.h
    yal/frontend/modulemanager.cpp

    yal/frontend/types/operators.h

    yal/transpiler/transpiler.h
    yal/transpiler/c/transpilerc.h
    yal/transpiler/c/transpilerc.cpp
    yal/transpiler/c/codegenc.cpp
    yal/transpiler/c/codegenc.h
    yal/transpiler/c/gentypesc.h
    yal/transpiler/c/gentypesc.cpp

    yal/util/log.h
    yal/util/log.cpp
    yal/util/prettyprint.cpp
    yal/util/prettyprint.h
    yal/util/allocatorstack.cpp
    yal/util/allocatorstack.h
    yal/util/stringref.cpp
    yal/util/stringref.h
    yal/util/cast.h
    yal/util/format.h
    yal/util/format.cpp
    yal/util/formattypes.h
    yal/util/formathex.h
    yal/util/formathex.cpp
    yal/util/strconversions.h
    yal/util/strconversions.cpp
    yal/util/iteratorrange.h
    yal/util/stackjump.h
    yal/util/path.h
    yal/util/path.cpp
    yal/util/codewriter.cpp
    yal/util/codewriter.h
    )

# include parser / lexer DSL's
set_source_files_properties(yal/frontend/lexer/lexer_re2c.in
    PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(yal/parser/parserimpl.lemon
    PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(yal/frontend/parser/stparserimpl.lemon
    PROPERTIES HEADER_FILE_ONLY TRUE)

# include .def files
set_source_files_properties(yal/parser/astnodes.def
    PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(yal/parser/declbasetypes.def
    PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(yal/parser/typelist.def
    PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties( yal/frontend/parser/syntaxtreetypes.def
    PROPERTIES HEADER_FILE_ONLY TRUE)


#if re2c is availabe use custom targets
if (${RE2C_TOOLS_FOUND})
    RE2C_GEN(
        "${CMAKE_CURRENT_SOURCE_DIR}/yal/frontend/lexer/lexer_re2c.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/yal/frontend/lexer/lexer_re2c.cpp")
endif()

#if lemon is availabe use custom targets
if (${LEMON_TOOLS_FOUND})
    LEMON_GEN(
        "${CMAKE_CURRENT_SOURCE_DIR}/yal/parser/"
        parserimpl.lemon
        parserimpl.cpp)

    LEMON_GEN(
        "${CMAKE_CURRENT_SOURCE_DIR}/yal/frontend/parser/"
        stparserimpl.lemon
        stparserimpl.cpp)
endif()

add_library(${YAL_LIB_NAME} STATIC
    ${YAL_SOURCES}
    )


target_include_directories( ${YAL_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${YAL_CONFIG_INCLUDE_DIR}
    )

YAL_APPLY_COMPILER_FLAGS_WERROR(${YAL_LIB_NAME})
YAL_REMOVE_WERROR_FROM_SOURCE(yal/parser/parserimpl.cpp)
YAL_REMOVE_WERROR_FROM_SOURCE(yal/frontend/parser/stparserimpl.cpp)
