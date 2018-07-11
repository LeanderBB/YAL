#
# YAL Frontend Library
#

set(YAL_VERSION_MAJOR 0)
set(YAL_VERSION_MINOR 1)
set(YAL_VERSION_PATCH 0)

project(yal-frontend
    VERSION ${YAL_VERSION_MAJOR}.${YAL_VERSION_MINOR}.${YAL_VERSION_PATCH}
    LANGUAGES CXX)

include(lexerparsertools)


set(YAL_FRONTEND_VERSION_STR "\"${YAL_FRONTEND_VERSION_MAJOR}.${YAL_FRONTEND_VERSION_MINOR}.${YAL_FRONTEND_VERSION_PATCH}\"")
set(YAL_FRONTEND_CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/)
set(YAL_FRONTEND_CONFIG_FILE ${YAL_FRONTEND_CONFIG_INCLUDE_DIR}/yal/frontend/config.h)
configure_file(yal/frontend/config.h.cmake ${YAL_FRONTEND_CONFIG_FILE})


set(YAL_LIB_NAME yal-frontend)


set(YAL_SOURCES
    ${YAL_FRONTEND_CONFIG_FILE}
    yal/frontend/ast/astcontext.h
    yal/frontend/ast/astcontext.cpp
    yal/frontend/ast/astnodes.h
    yal/frontend/ast/declbase.h
    yal/frontend/ast/declbase.cpp
    yal/frontend/ast/declbasetypes.def
    yal/frontend/ast/asttypes.h
    yal/frontend/ast/asttypes.cpp
    yal/frontend/ast/astnodes.def
    yal/frontend/ast/declfunction.h
    yal/frontend/ast/declfunction.cpp
    yal/frontend/ast/decltypefunction.h
    yal/frontend/ast/decltypefunction.cpp
    yal/frontend/ast/declmodule.cpp
    yal/frontend/ast/declmodule.h
    yal/frontend/ast/declalias.h
    yal/frontend/ast/declalias.cpp
    yal/frontend/ast/statement.h
    yal/frontend/ast/statement.cpp
    yal/frontend/ast/stmtexpression.cpp
    yal/frontend/ast/stmtexpression.h
    yal/frontend/ast/stmtreturn.cpp
    yal/frontend/ast/stmtreturn.h
    yal/frontend/ast/exprunaryoperator.cpp
    yal/frontend/ast/exprunaryoperator.h
    yal/frontend/ast/exprbinaryoperator.cpp
    yal/frontend/ast/exprbinaryoperator.h
    yal/frontend/ast/declvar.h
    yal/frontend/ast/declvar.cpp
    yal/frontend/ast/declparamvar.cpp
    yal/frontend/ast/declparamvar.h
    yal/frontend/ast/exprboolliteral.cpp
    yal/frontend/ast/exprboolliteral.h
    yal/frontend/ast/exprintegerliteral.h
    yal/frontend/ast/exprintegerliteral.cpp
    yal/frontend/ast/stmtdecl.cpp
    yal/frontend/ast/stmtdecl.h
    yal/frontend/ast/stmtassign.cpp
    yal/frontend/ast/stmtassign.h
    yal/frontend/ast/exprvarref.cpp
    yal/frontend/ast/exprvarref.h
    yal/frontend/ast/astvisitor.h
    yal/frontend/ast/astvisitorimpl.h
    yal/frontend/ast/astvisitor.cpp
    yal/frontend/ast/astprinter.cpp
    yal/frontend/ast/astprinter.h
    yal/frontend/ast/declstruct.h
    yal/frontend/ast/declstruct.cpp
    yal/frontend/ast/exprstructvarref.cpp
    yal/frontend/ast/exprstructvarref.h
    yal/frontend/ast/exprfncall.h
    yal/frontend/ast/exprfncall.cpp
    yal/frontend/ast/exprtypefncall.h
    yal/frontend/ast/exprtypefncall.cpp
    yal/frontend/ast/exprfloatliteral.cpp
    yal/frontend/ast/exprfloatliteral.h
    yal/frontend/ast/declscope.cpp
    yal/frontend/ast/declscope.h
    yal/frontend/ast/exprcast.h
    yal/frontend/ast/exprcast.cpp
    yal/frontend/ast/exprstructinit.cpp
    yal/frontend/ast/exprstructinit.h
    yal/frontend/ast/structmemberinit.h
    yal/frontend/ast/structmemberinit.cpp
    yal/frontend/ast/astsearch.cpp
    yal/frontend/ast/astsearch.h
    yal/frontend/ast/stmtlistscoped.cpp
    yal/frontend/ast/stmtlistscoped.h
    yal/frontend/ast/decltypefunctions.h
    yal/frontend/ast/decltypefunctions.cpp
    yal/frontend/ast/declimport.h
    yal/frontend/ast/declimport.cpp
    yal/frontend/frontend.cpp
    yal/frontend/frontend.h
    yal/frontend/lexer/lexer.h
    yal/frontend/lexer/lexer.cpp
    yal/frontend/lexer/tokens.h
    yal/frontend/lexer/tokens.cpp
    yal/frontend/lexer/lexer_re2c.in
    yal/frontend/lexer/lexer_re2c.cpp
    yal/frontend/lexer/errorlexer.cpp
    yal/frontend/lexer/errorlexer.h
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
    yal/frontend/parser/ststmtexpression.cpp
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
    yal/frontend/parser/stcontext.cpp
    yal/frontend/parser/stcontext.h
    yal/frontend/parser/ststmtlistscoped.cpp
    yal/frontend/parser/ststmtlistscoped.h
    yal/frontend/parser/stdecltypefunctions.h
    yal/frontend/parser/stdecltypefunctions.cpp
    yal/frontend/parser/syntaxtreevisitorimpl.h
    yal/frontend/parser/stdeclalias.cpp
    yal/frontend/parser/stdeclalias.h
    yal/frontend/parser/stdeclimport.h
    yal/frontend/parser/stdeclimport.cpp
    yal/frontend/module.h
    yal/frontend/module.cpp
    yal/frontend/modulemanager.h
    yal/frontend/modulemanager.cpp
    yal/frontend/types/operators.h
    yal/frontend/types/type.h
    yal/frontend/types/type.cpp
    yal/frontend/types/qualifier.cpp
    yal/frontend/types/qualifier.h
    yal/frontend/types/qualtype.cpp
    yal/frontend/types/qualtype.h
    yal/frontend/types/typelist.def
    yal/frontend/types/typebuiltin.cpp
    yal/frontend/types/typebuiltin.h
    yal/frontend/types/typecontext.cpp
    yal/frontend/types/typecontext.h
    yal/frontend/types/typefunction.cpp
    yal/frontend/types/typefunction.h
    yal/frontend/types/typestruct.cpp
    yal/frontend/types/typestruct.h
    yal/frontend/types/identifier.cpp
    yal/frontend/types/identifier.h
    yal/frontend/types/typealias.h
    yal/frontend/types/typealias.cpp
    yal/frontend/types/types.h
    yal/frontend/passes/passes.h
    yal/frontend/passes/parser/passparser.h
    yal/frontend/passes/parser/passparser.cpp
    yal/frontend/passes/decl/passdecl.h
    yal/frontend/passes/decl/passdecl.cpp
    yal/frontend/passes/decl/stpredeclvisitor.h
    yal/frontend/passes/decl/stpredeclvisitor.cpp
    yal/frontend/passes/decl/errorspassdecl.cpp
    yal/frontend/passes/decl/errorspassdecl.h
    yal/frontend/passes/decl/astbuilder.cpp
    yal/frontend/passes/decl/astbuilder.h
    yal/frontend/passes/fnret/passfnret.cpp
    yal/frontend/passes/fnret/passfnret.h
    yal/frontend/passes/fnret/returncheckvisitor.cpp
    yal/frontend/passes/fnret/returncheckvisitor.h
    yal/frontend/passes/fnret/errorspassfnret.cpp
    yal/frontend/passes/fnret/errorspassfnret.h
    yal/frontend/passes/type/passtype.cpp
    yal/frontend/passes/type/passtype.h
    yal/frontend/passes/type/astvisitorexprtype.cpp
    yal/frontend/passes/type/astvisitorexprtype.h
    yal/frontend/passes/type/errorspasstype.cpp
    yal/frontend/passes/type/errorspasstype.h
    yal/frontend/passes/move/passmove.cpp
    yal/frontend/passes/move/passmove.h
    yal/frontend/passes/move/astvisitormovecheck.cpp
    yal/frontend/passes/move/astvisitormovecheck.h
    yal/frontend/passes/move/errorspassmove.cpp
    yal/frontend/passes/move/errorspassmove.h
    yal/frontend/errorfrontend.cpp
    yal/frontend/errorfrontend.h
    )



# include parser / lexer DSL's
set_source_files_properties(yal/frontend/lexer/lexer_re2c.in
    PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(yal/frontend/parser/stparserimpl.lemon
    PROPERTIES HEADER_FILE_ONLY TRUE)

# include .def files
set_source_files_properties(yal/frontend/ast/astnodes.def
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
        "${CMAKE_CURRENT_SOURCE_DIR}/yal/frontend/parser/"
        stparserimpl.lemon
        stparserimpl.cpp)
endif()

add_library(${YAL_LIB_NAME} STATIC
    ${YAL_SOURCES}
    )

add_library(yal::frontend
    ALIAS ${YAL_LIB_NAME})

target_include_directories( ${YAL_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${YAL_CONFIG_INCLUDE_DIR}
    )

target_link_libraries(${YAL_LIB_NAME}
    PUBLIC yal::core)

#YAL_APPLY_COMPILER_FLAGS_WERROR(${YAL_LIB_NAME})
YAL_REMOVE_WERROR_FROM_SOURCE(yal/frontend/parser/stparserimpl.cpp)
