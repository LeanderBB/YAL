%pure-parser
%{
#include <stdlib.h>
#include <stdio.h>
#include <yal/yal.h>
#include <yal/parser/parser_state.h>
#include <yal/memory/memory_utils.h>
#include <yal/parser/bison_utils.h>
#include <yal/ast/astnodetypes.h>
#include <yal/ast/astbasenode.h>
#include <yal/ast/assignoperatornode.h>
#include <yal/ast/codebodynode.h>
#include <yal/ast/compareoperatornode.h>
#include <yal/ast/constantnode.h>
#include <yal/util/constantvalue.h>
#include <yal/ast/argumentdeclnode.h>
#include <yal/ast/variabledeclnode.h>
#include <yal/ast/dualoperatornode.h>
#include <yal/ast/singleoperatornode.h>
#include <yal/ast/functionnode.h>
#include <yal/ast/conditionnode.h>
#include <yal/ast/returnnode.h>

#include "yal_bison_parser.hpp"
#include "yal_flex_lexer.h"
using namespace yal;
extern void yyerror(YYLTYPE* location,
                     void* scanner,
                     yal::ParserState_t* state,
                     const char* s);
#define YYERROR_VERBOSE 1
%}

%code requires
{
#include <yal/yal.h>
#include <yal/parser/parser_state.h>
#include <yal/memory/memory_utils.h>
#include <yal/parser/bison_utils.h>
#include <yal/util/constantvalue.h>
#include <yal/ast/astnodetypes.h>
#include <yal/ast/astbasenode.h>
#include <yal/ast/assignoperatornode.h>
#include <yal/ast/codebodynode.h>
#include <yal/ast/compareoperatornode.h>
#include <yal/ast/constantnode.h>
#include <yal/ast/argumentdeclnode.h>
#include <yal/ast/variabledeclnode.h>
#include <yal/ast/dualoperatornode.h>
#include <yal/ast/singleoperatornode.h>
#include <yal/ast/functionnode.h>
#include <yal/ast/conditionnode.h>
#include <yal/ast/returnnode.h>

}

%lex-param {void* scanner}
%parse-param {void* scanner} {yal::ParserState_t *state}
%locations

// define union for data return between flex and bison
%union
{
    yal_i32 integer32;
    yal_u32 uinteger32;
    yal_i64 integer64;
    yal_u64 uinteger64;
    yal_f32 float32;
    yal_f64 float64;
    yal_bool boolean;
    yal::ConstantType builtinType;
    char* text;
    class yal::ConditionNode* nodeIf;
    class yal::ExpressionNode* nodeExp;
    class yal::StatementNode* nodeSt;
    class yal::CodeBodyNode* nodeCodeBody;
    class yal::AstBaseNode* node;
    class yal::ExpressionList* nodeExpList;
    class yal::ArgumentDeclNode* nodeDeclArg;
    class yal::ArgumentDeclsNode* nodeDeclArgs;
}

// define the token association with the union member
%token <integer32> TK_INT32 "integer 32bit"
%token <uinteger32> TK_UINT32 "unsigned integer 32bit"
%token <integer64> TK_INT64 "integer 64bit"
%token <uinteger64> TK_UINT64 "unsigned integer 64bit"
%token <float32> TK_FLT32 "decimal 32bit"
%token <float64> TK_FLT64 "decimal 64bit"
%token <boolean> TK_BOOL "boolean"
%token <text> TK_TEXT "string"
%token <text> TK_ID "identifier"


%token TK_FUNC_BEGIN "func"
%token TK_RETURN "return" 
%token TK_VAR_DECL "var"
%token TK_SPACE " "


%token TK_TYPE_BOOL "type boolean"
%token TK_TYPE_INT32 "type integer 32bit"
%token TK_TYPE_UINT32 "type unsigned integer 32bit"
%token TK_TYPE_INT64 "type integer 64bit"
%token TK_TYPE_UINT64 "type unsigned integer 64bit"
%token TK_TYPE_F32 "type decimal 32bit"
%token TK_TYPE_F64 "type decimal 64bit"


%token TK_IF "if"
%token TK_ELIF "elif"
%token TK_ELSE "else"
%token TK_END "end"
%token END TK_NL "new line"

// operatores
%token TK_OP_ASSIGN "="
%token TK_OP_ASSIGN_PLUS "+="
%token TK_OP_ASSIGN_MINUS "-="
%token TK_OP_ASSIGN_MULT "*="
%token TK_OP_ASSIGN_DIV "/="
%token TK_OP_ASSIGN_AND "&="
%token TK_OP_ASSIGN_OR "|="
%token TK_OP_ASSIGN_XOR "^="
%token TK_OP_ASSIGN_SHIFT_LEFT "<<="
%token TK_OP_ASSIGN_SHIFT_RIGHT ">>="
%token TK_NOT "not"
%token TK_BIT_TIL "~"
%token TK_OR "or"
%token TK_AND "and"
%token TK_BIT_OR "|"
%token TK_BIT_XOR "^"
%token TK_BIT_AND "&"
%token TK_CMP_EQ "=="
%token TK_CMP_NE "!="
%token TK_CMP_GT ">"
%token TK_CMP_GE ">="
%token TK_CMP_LE "<="
%token TK_CMP_LT "<"
%token TK_SHIFT_LEFT "<<"
%token TK_SHIFT_RIGHT ">>"
%token TK_OP_PLUS "+"
%token TK_OP_MINUS "-"
%token TK_OP_DIV "/"
%token TK_OP_MULT "*"


//%left TK_OP_ASSIGN TK_OP_ASSIGN_PLUS TK_OP_ASSIGN_MINUS TK_OP_ASSIGN_MULT TK_OP_ASSIGN_DIV TK_OP_ASSIGN_AND TK_OP_ASSIGN_OR TK_OP_ASSIGN_XOR TK_OP_ASSIGN_SHIFT_LEFT TK_OP_ASSIGN_SHIFT_RIGHT
%precedence TK_PREC_ASSIGN
%left TK_OR
%left TK_AND
%left TK_BIT_OR
%left TK_BIT_XOR
%left TK_BIT_AND
%left TK_CMP_EQ TK_CMP_NE
%left TK_CMP_GT TK_CMP_LT TK_CMP_GE TK_CMP_LE
%left TK_SHIFT_LEFT TK_SHIFT_RIGHT
%left TK_OP_PLUS TK_OP_MINUS
%left TK_OP_MULT TK_OP_DIV
%precedence TK_PREC_NEG


%start program
%type <node> func_decl 
%type <nodeSt> statement var_decl st_upper
%type <nodeExp> expression constant func_call
%type <nodeIf> if_statement if_statement_next
%type <nodeCodeBody> code_body
%type <nodeExpList>  call_args
%type <builtinType> builtin_type func_return
%type <nodeDeclArg> decl_arg
%type <nodeDeclArgs> decl_args
%%

program: program func_decl TK_NL  {state->program.push_back(static_cast<yal::AstBaseNode*>(yal::BisonYyltypeToLocation(yylloc),$2));}
| program statement TK_NL{state->program.push_back($2);}
| program TK_NL
| %empty
;

func_decl: TK_FUNC_BEGIN TK_ID '(' decl_args ')' func_return TK_NL code_body TK_END { $$ = new yal::FunctionDeclNode(yal::BisonYyltypeToLocation(yylloc), $2, $4, $6, $8); }
;

func_return: %empty { $$ = yal::kConstantTypeNone; }
| ':' builtin_type { $$ = $2; }
| ':' TK_ID { $$ = yal::kConstantTypeId; }
;

code_body: st_upper  { $$ = new CodeBodyNode(yal::BisonYyltypeToLocation(yylloc)); if($1) {$$->addStatement($1);}}
| code_body st_upper {if($2) {$$->addStatement($2);}}
;

st_upper: statement TK_NL {$$ = $1;}
| TK_NL {$$ = nullptr;}
;

statement: var_decl { $$ = $1;}
| expression { $$ = static_cast<yal::StatementNode*>($1);}
| if_statement { $$ = static_cast<yal::StatementNode*>($1);}
| TK_RETURN expression {$$ = new yal::ReturnNode(yal::BisonYyltypeToLocation(yylloc), $2);}
| TK_RETURN {$$ = new yal::ReturnNode(yal::BisonYyltypeToLocation(yylloc), nullptr);}
;

if_statement: TK_IF '(' expression ')' TK_NL code_body if_statement_next {$$ = new yal::ConditionNode(yal::BisonYyltypeToLocation(yylloc), $3, $6, $7);}

if_statement_next: TK_ELIF '(' expression ')' TK_NL code_body if_statement_next {$$ = new ConditionNode(yal::BisonYyltypeToLocation(yylloc), $3, $6, $7);}
    | TK_ELSE TK_NL code_body TK_END {$$ = new ConditionNode(yal::BisonYyltypeToLocation(yylloc), nullptr, $3, nullptr);}
    | TK_END {$$ = nullptr;}
    ;

expression: '(' expression ')' {$$ = new yal::SingleOperatorNode(yal::BisonYyltypeToLocation(yylloc), kSingleOperatorTypeBlock, $2);}
    | TK_OP_MINUS expression %prec TK_PREC_NEG {$$ = new yal::SingleOperatorNode(yal::BisonYyltypeToLocation(yylloc), kSingleOperatorTypeNeg, $2);}
    | TK_NOT expression  %prec TK_PREC_NEG {$$ = new yal::SingleOperatorNode(yal::BisonYyltypeToLocation(yylloc), kSingleOperatorTypeNot, $2);}
    | TK_BIT_TIL expression %prec TK_PREC_NEG {$$ = new yal::SingleOperatorNode(yal::BisonYyltypeToLocation(yylloc), kSingleOperatorTypeTil, $2);}
    | expression TK_AND expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kDualOperatorTypeAnd, $1, $3);}
    | expression TK_OR expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kDualOperatorTypeOr, $1, $3);}
    | expression TK_CMP_GT expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kCompareOperatorTypeGT, $1, $3);}
    | expression TK_CMP_GE expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kCompareOperatorTypeGE, $1, $3);}
    | expression TK_CMP_LE expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kCompareOperatorTypeLE, $1, $3);}
    | expression TK_CMP_LT expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kCompareOperatorTypeLT, $1, $3);}
    | expression TK_CMP_EQ expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kCompareOperatorTypeEQ, $1, $3);}
    | expression TK_CMP_NE expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kCompareOperatorTypeNE, $1, $3);}
    | expression TK_OP_PLUS expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kDualOperatorTypeAdd, $1, $3);}
    | expression TK_OP_MINUS expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kDualOperatorTypeSub, $1, $3);}
    | expression TK_OP_MULT expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kDualOperatorTypeMult, $1, $3);}
    | expression TK_OP_DIV expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kDualOperatorTypeDiv, $1, $3);}
    | expression TK_BIT_OR expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kDualOperatorTypeBitOr, $1, $3);}
    | expression TK_BIT_XOR expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kDualOperatorTypeBitXor, $1, $3);}
    | expression TK_BIT_AND expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kDualOperatorTypeBitAnd, $1, $3);}
    | expression TK_SHIFT_LEFT expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kDualOperatorTypeShiftLeft, $1, $3);}
    | expression TK_SHIFT_RIGHT expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kDualOperatorTypeShiftRight, $1, $3);}
    | TK_ID TK_OP_ASSIGN expression %prec TK_PREC_ASSIGN { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kAssignOperatorTypeSet, $3);}
    | TK_ID TK_OP_ASSIGN_PLUS expression %prec TK_PREC_ASSIGN  { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kAssignOperatorTypePlus, $3);}
    | TK_ID TK_OP_ASSIGN_MINUS expression %prec TK_PREC_ASSIGN  { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kAssignOperatorTypeMinus, $3);}
    | TK_ID TK_OP_ASSIGN_MULT expression %prec TK_PREC_ASSIGN  { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kAssignOperatorTypeMult, $3);}
    | TK_ID TK_OP_ASSIGN_DIV expression %prec TK_PREC_ASSIGN  { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kAssignOperatorTypeDiv, $3);}
    | TK_ID TK_OP_ASSIGN_AND expression  %prec TK_PREC_ASSIGN  { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kAssignOperatorTypeAnd, $3);}
    | TK_ID TK_OP_ASSIGN_OR expression %prec TK_PREC_ASSIGN  { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kAssignOperatorTypeOr, $3);}
    | TK_ID TK_OP_ASSIGN_XOR expression %prec TK_PREC_ASSIGN  { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kAssignOperatorTypeXor, $3);}
    | TK_ID TK_OP_ASSIGN_SHIFT_LEFT expression %prec TK_PREC_ASSIGN  { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kAssignOperatorTypeShiftLeft, $3);}
    | TK_ID TK_OP_ASSIGN_SHIFT_RIGHT expression %prec TK_PREC_ASSIGN  { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kAssignOperatorTypeShiftRight, $3);}
    | constant
    | func_call
    ;
func_call: TK_ID '(' call_args ')' {$$ = new yal::FunctionCallNode(yal::BisonYyltypeToLocation(yylloc), $1, $3);}
    ;

call_args: call_args ',' expression {$$->expressions.push_back($3);}
    | expression { $$ = new yal::ExpressionList(yal::BisonYyltypeToLocation(yylloc)); $$->expressions.push_back($1);}
    | %empty { $$ = new yal::ExpressionList(yal::BisonYyltypeToLocation(yylloc)); }
    ;


decl_args : decl_args ','  decl_arg { $$->addArgument($3);}
    | decl_arg {$$ = new yal::ArgumentDeclsNode(yal::BisonYyltypeToLocation(yylloc)); $$->addArgument($1);}
    | %empty {$$ = new yal::ArgumentDeclsNode(yal::BisonYyltypeToLocation(yylloc));}
    ;

decl_arg: builtin_type TK_ID {$$ = new yal::ArgumentDeclNode(yal::BisonYyltypeToLocation(yylloc), $1, $2);}
    | TK_ID TK_ID {$$ = new yal::ArgumentDeclNode(yal::BisonYyltypeToLocation(yylloc), $1, $2);}
    ;

var_decl: TK_VAR_DECL TK_ID TK_OP_ASSIGN expression { $$ = new yal::VariableDeclNode(yal::BisonYyltypeToLocation(yylloc), $2, $4);}
;

builtin_type: TK_TYPE_BOOL {$$ = kConstantTypeBool;}
| TK_TYPE_INT32 {$$ = kConstantTypeInt32;}
| TK_TYPE_UINT32 {$$ = kConstantTypeUInt32;}
| TK_TYPE_INT64 {$$ = kConstantTypeInt64;}
| TK_TYPE_UINT64 {$$ = kConstantTypeUInt64;}
| TK_TYPE_F32 {$$ = kConstantTypeFloat32;}
| TK_TYPE_F64 {$$ = kConstantTypeFloat64;}
;

constant: TK_BOOL { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_INT32 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_UINT32 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc),yal::ConstantValue($1));}
| TK_INT64 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_UINT64 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_FLT32 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_FLT64 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_TEXT {printf("Found <text>:%s\n", $1); $$ = nullptr; }
| TK_ID { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1)); }
;


//NLS:TK_NL NLS
//  | TK_NL
//  ;
%%
