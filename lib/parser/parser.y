%pure-parser
%{
#include <stdlib.h>
#include <stdio.h>
#include <yal/yal.h>
#include <yal/parser/parser_state.h>
#include <yal/memory/memory_utils.h>
#include <yal/parser/bison_utils.h>
#include <yal/ast/asthdrs.h>
#include <yal/types/builtintype.h>
#include <yal/types/undefined.h>
#include <yal/types/type.h>
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
#include <yal/ast/asthdrs.h>
#include <yal/types/typehdrs.h>

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
    yal::Type* type;
    char* text;
    class yal::ConditionNode* nodeIf;
    class yal::ExpressionNode* nodeExp;
    class yal::StatementNode* nodeSt;
    class yal::CodeBodyNode* nodeCodeBody;
    class yal::AstBaseNode* node;
    class yal::ArgumentDeclNode* nodeDeclArg;
    class yal::ArgumentDeclsNode* nodeDeclArgs;
    class yal::PrintArgsNode* nodePrintArgs;
    class yal::FunctionCallArgsNode* nodeFunCallArgs;
    class yal::WhileLoopNode* nodeWhileLoop;
    class yal::ArrayCtrNode* nodeArrayCtr;
    class yal::UserTypeMembersNode* memberVec;
    class yal::UserTypeMemberNode* userTypeMember;
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
%token <text> TK_ID "id"


%token TK_FUNC_BEGIN "fn"
%token TK_RETURN "return" 
%token TK_VAR_DECL "var"
%token TK_SPACE " "
%token TK_SCOPE_BGN "{"
%token TK_SCOPE_END "}"


%token TK_TYPE_BOOL "type boolean"
%token TK_TYPE_INT32 "type integer 32bit"
%token TK_TYPE_UINT32 "type unsigned integer 32bit"
%token TK_TYPE_INT64 "type integer 64bit"
%token TK_TYPE_UINT64 "type unsigned integer 64bit"
%token TK_TYPE_F32 "type decimal 32bit"
%token TK_TYPE_F64 "type decimal 64bit"
%token TK_TYPE_STRING "type string"

%token TK_IF "if"
%token TK_ELIF "elif"
%token TK_ELSE "else"
%token TK_END "end"
//%token END TK_NL "new line"
%token TK_PRINT "print"
%token TK_WHILE "while"
%token TK_NATIVE "native"
%token TK_ATTRIB_EXCALL "attribute excall"
%token TK_SELF "self"
%token TK_USER_TYPE "type"

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
%token TK_MOD "mod"
%token TK_DOT "."
%token TK_ARRAY_BGN "["
%token TK_ARRAY_END "]"
%token TK_SEMI_COLON ";"

//%precedence TK_PREC_CALL
%right TK_OP_ASSIGN TK_OP_ASSIGN_PLUS TK_OP_ASSIGN_MINUS TK_OP_ASSIGN_MULT TK_OP_ASSIGN_DIV TK_OP_ASSIGN_AND TK_OP_ASSIGN_OR TK_OP_ASSIGN_XOR TK_OP_ASSIGN_SHIFT_LEFT TK_OP_ASSIGN_SHIFT_RIGHT
//%precedence TK_PREC_ASSIGN
%left TK_OR
%left TK_AND
%left TK_BIT_OR
%left TK_BIT_XOR
%left TK_BIT_AND
%left TK_CMP_EQ TK_CMP_NE
%left TK_CMP_GT TK_CMP_LT TK_CMP_GE TK_CMP_LE
%left TK_SHIFT_LEFT TK_SHIFT_RIGHT
%left TK_OP_PLUS TK_OP_MINUS
%left TK_OP_MULT TK_OP_DIV TK_MOD
%precedence TK_PREC_NEG
%precedence TK_PREC_NOT
%precedence TK_ARRAY_BGN TK_DOT


%start program
%type <node> func_decl
%type <nodeSt> statement var_decl st_upper print_statement user_type_decl
%type <nodeExp> expression constant array_ctr var_decl_exp var_access
%type <nodeIf> if_statement if_statement_next
%type <nodePrintArgs> print_args
%type <nodeFunCallArgs> call_args
%type <nodeCodeBody> code_body
%type <type> type func_return basic_type compound_type
%type <nodeDeclArg> decl_arg
%type <nodeDeclArgs> decl_args
%type <nodeWhileLoop> while_statement
%type <nodeArrayCtr> array_ctr_exp
%type <memberVec> user_type_members
%type <userTypeMember> user_type_member
%%

program: program func_decl {state->program.emplace_back(static_cast<yal::StatementNode*>(yal::BisonYyltypeToLocation(yylloc),$2));}
| program statement {state->program.emplace_back($2);}
| program user_type_decl {state->program.emplace_back($2);}
| %empty
;


user_type_decl: TK_USER_TYPE TK_ID TK_SCOPE_BGN user_type_members TK_SCOPE_END TK_SEMI_COLON
 { $$ = new yal::UserTypeDeclNode(yal::BisonYyltypeToLocation(yylloc), $2, $4);}


user_type_members: user_type_member  {$$ = new yal::UserTypeMembersNode(yal::BisonYyltypeToLocation(yylloc)); $$->addUserTypeMember($1);}
    | user_type_members user_type_member  { $$->addUserTypeMember($2);}
    ;

user_type_member: type TK_ID TK_SEMI_COLON {$$ = new yal::UserTypeMemberNode(yal::BisonYyltypeToLocation(yylloc), $1, $2);}
    ;

attribute: TK_ATTRIB_EXCALL
    ;

attributes: attribute
  | %empty
  ;

func_decl: TK_NATIVE TK_FUNC_BEGIN TK_ID '(' decl_args ')' func_return TK_SEMI_COLON
{ $$ = new yal::FunctionDeclNativeNode(yal::BisonYyltypeToLocation(yylloc), $3, nullptr, $5, $7); }
   | TK_NATIVE TK_FUNC_BEGIN type ':'':' TK_ID '(' decl_args ')' func_return TK_SEMI_COLON
{ $$ = new yal::FunctionDeclNativeNode(yal::BisonYyltypeToLocation(yylloc), $6, $3, $8, $10); }
   |  attributes TK_FUNC_BEGIN TK_ID '(' decl_args ')' func_return TK_SCOPE_BGN code_body TK_SCOPE_END
{ $$ = new yal::FunctionDeclNode(yal::BisonYyltypeToLocation(yylloc), $3, nullptr, $5, $7, $9); }
   |  attributes TK_FUNC_BEGIN type ':'':' TK_ID '(' decl_args ')' func_return TK_SCOPE_BGN code_body TK_SCOPE_END
{ $$ = new yal::FunctionDeclNode(yal::BisonYyltypeToLocation(yylloc), $6, $3, $8, $10, $12); }
;


func_return: %empty { $$ = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kVoid); }
| ':' type { $$ = $2; }
;

code_body: st_upper  { $$ = new CodeBodyNode(yal::BisonYyltypeToLocation(yylloc)); if($1) {$$->addStatement($1);}}
| code_body st_upper {if($2) {$$->addStatement($2);}}
;

st_upper: statement {$$ = $1;}
;

statement: var_decl TK_SEMI_COLON{ $$ = $1;}
| print_statement{ $$ = static_cast<yal::StatementNode*>($1);}
| expression TK_SEMI_COLON { $$ = static_cast<yal::StatementNode*>($1);}
| if_statement { $$ = static_cast<yal::StatementNode*>($1);}
| while_statement { $$ = static_cast<yal::StatementNode*>($1); }
| TK_RETURN expression TK_SEMI_COLON{$$ = new yal::ReturnNode(yal::BisonYyltypeToLocation(yylloc), $2);}
| TK_RETURN TK_SEMI_COLON{$$ = new yal::ReturnNode(yal::BisonYyltypeToLocation(yylloc), nullptr);}
;

print_statement: TK_PRINT '(' print_args ')' TK_SEMI_COLON {$$ = new yal::PrintNode(yal::BisonYyltypeToLocation(yylloc), $3);}
;

print_args : print_args ',' expression {$$->expressions.emplace_back($3);}
| expression { $$ = new yal::PrintArgsNode(yal::BisonYyltypeToLocation(yylloc)); $$->expressions.emplace_back($1);}
;

if_statement: TK_IF '(' expression ')' TK_SCOPE_BGN code_body TK_SCOPE_END if_statement_next {$$ = new yal::ConditionNode(yal::BisonYyltypeToLocation(yylloc), $3, $6, $8);}

if_statement_next: TK_ELIF '(' expression ')' TK_SCOPE_BGN code_body TK_SCOPE_END if_statement_next {$$ = new ConditionNode(yal::BisonYyltypeToLocation(yylloc), $3, $6, $8);}
    | TK_ELSE TK_SCOPE_BGN code_body TK_SCOPE_END {$$ = new ConditionNode(yal::BisonYyltypeToLocation(yylloc), nullptr, $3, nullptr);}
    | %empty {$$ = nullptr;}
    ;

while_statement: TK_WHILE '(' expression ')' TK_SCOPE_BGN code_body TK_SCOPE_END { $$ = new yal::WhileLoopNode(yal::BisonYyltypeToLocation(yylloc), $3, $6);}
;


expression: expression TK_ARRAY_BGN expression TK_ARRAY_END {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeArray, $1, $3);}
    |'(' expression ')'  {$$ =  new yal::ParentExpNode(yal::BisonYyltypeToLocation(yylloc), $2);}
    | TK_OP_MINUS expression %prec TK_PREC_NEG {$$ = new yal::SingleOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeMinus, $2);}
    | TK_NOT expression  %prec TK_PREC_NOT {$$ = new yal::SingleOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeNot, $2);}
    | TK_BIT_TIL expression %prec TK_PREC_NEG {$$ = new yal::SingleOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitNot, $2);}
    | expression TK_AND expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeAnd, $1, $3);}
    | expression TK_OR expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeOr, $1, $3);}
    | expression TK_CMP_GT expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeGt, $1, $3);}
    | expression TK_CMP_GE expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeGe, $1, $3);}
    | expression TK_CMP_LE expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeLe, $1, $3);}
    | expression TK_CMP_LT expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeLt, $1, $3);}
    | expression TK_CMP_EQ expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeEq, $1, $3);}
    | expression TK_CMP_NE expression {$$ = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeNe, $1, $3);}
    | expression TK_OP_PLUS expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypePlus, $1, $3);}
    | expression TK_OP_MINUS expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeMinus, $1, $3);}
    | expression TK_OP_MULT expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeMult, $1, $3);}
    | expression TK_OP_DIV expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeDiv, $1, $3);}
    | expression TK_MOD expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeMod, $1, $3);}
    | expression TK_BIT_OR expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitOr, $1, $3);}
    | expression TK_BIT_XOR expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitXor, $1, $3);}
    | expression TK_BIT_AND expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitAnd, $1, $3);}
    | expression TK_SHIFT_LEFT expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitShiftLeft, $1, $3);}
    | expression TK_SHIFT_RIGHT expression {$$ = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitShiftRight, $1, $3);}
    | expression TK_OP_ASSIGN expression { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kOperatorTypeCopy, $3);}
    | expression TK_OP_ASSIGN_PLUS expression  { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kOperatorTypePlus, $3);}
    | expression TK_OP_ASSIGN_MINUS expression { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kOperatorTypeMinus, $3);}
    | expression TK_OP_ASSIGN_MULT expression { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kOperatorTypeMult, $3);}
    | expression TK_OP_ASSIGN_DIV expression { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kOperatorTypeDiv, $3);}
    | expression TK_OP_ASSIGN_AND expression { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kOperatorTypeBitAnd, $3);}
    | expression TK_OP_ASSIGN_OR expression { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kOperatorTypeBitOr, $3);}
    | expression TK_OP_ASSIGN_XOR expression { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kOperatorTypeBitXor, $3);}
    | expression TK_OP_ASSIGN_SHIFT_LEFT expression { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kOperatorTypeBitShiftLeft, $3);}
    | expression TK_OP_ASSIGN_SHIFT_RIGHT expression { $$ = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), $1, kOperatorTypeBitShiftRight, $3);}
    | constant {$$ = $1;}
    | var_access {$$ = $1;}
    | TK_ID '(' call_args ')' {$$ = new yal::FunctionCallNode(yal::BisonYyltypeToLocation(yylloc), nullptr, $1, $3);}
    | expression TK_DOT TK_ID '(' call_args ')'{$$ = new yal::FunctionCallNode(yal::BisonYyltypeToLocation(yylloc), $1, $3, $5);}
    ;

var_access : TK_ID { $$ = new yal::VariableAccessNode(yal::BisonYyltypeToLocation(yylloc), $1); }
| TK_SELF { $$ = new yal::VariableAccessNode(yal::BisonYyltypeToLocation(yylloc), strdup("self"));}
;


call_args: call_args ',' expression {$$->expressions.emplace_back($3);}
    | expression { $$ = new yal::FunctionCallArgsNode(yal::BisonYyltypeToLocation(yylloc)); $$->expressions.emplace_back($1);}
    | %empty { $$ = new yal::FunctionCallArgsNode(yal::BisonYyltypeToLocation(yylloc)); }
    ;


decl_args : decl_args ','  decl_arg { $$->addArgument($3);}
    | decl_arg {$$ = new yal::ArgumentDeclsNode(yal::BisonYyltypeToLocation(yylloc)); $$->addArgument($1);}
    | %empty {$$ = new yal::ArgumentDeclsNode(yal::BisonYyltypeToLocation(yylloc));}
    ;

decl_arg: type TK_ID {$$ = new yal::ArgumentDeclNode(yal::BisonYyltypeToLocation(yylloc), $1, $2);}
    ;

var_decl: TK_VAR_DECL TK_ID TK_OP_ASSIGN var_decl_exp { $$ = new yal::VariableDeclNode(yal::BisonYyltypeToLocation(yylloc), $2, $4, nullptr);}
| TK_VAR_DECL ':' type TK_ID TK_OP_ASSIGN var_decl_exp { $$ = new yal::VariableDeclNode(yal::BisonYyltypeToLocation(yylloc), $4, $6, $3);}
;

var_decl_exp: expression {$$ = $1;}
| array_ctr {$$ = $1;}
;


basic_type:TK_TYPE_BOOL {$$ = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kBool);}
| TK_TYPE_INT32 {$$ = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kInt32);}
| TK_TYPE_UINT32 {$$ = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kUInt32);}
| TK_TYPE_INT64 {$$ = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kInt64);}
| TK_TYPE_UINT64 {$$ = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kUInt64);}
| TK_TYPE_F32 {$$ = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kFloat32);}
| TK_TYPE_F64 {$$ = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kFloat64);}
| TK_ID { $$ = state->registry.registerUndefined($1); yal_free($1);}
| TK_TYPE_STRING { $$ = yal::StringType::GetType(); }
;

compound_type: type TK_ARRAY_BGN TK_ARRAY_END { $$ = state->registry.registerArray($1); }
;

type: basic_type {$$ = $1;}
| compound_type {$$ = $1;}
;


constant: TK_BOOL { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_INT32 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_UINT32 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc),yal::ConstantValue($1));}
| TK_INT64 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_UINT64 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_FLT32 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_FLT64 { $$ = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1));}
| TK_TEXT { $$ =  new yal::StringCreateNode (new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue($1)));}
;

array_ctr_exp : array_ctr_exp ',' expression {$1->addExpression($3); $$=$1;}
| expression { $$ = new yal::ArrayCtrNode(yal::BisonYyltypeToLocation(yylloc)); $$->addExpression($1);}
| %empty { $$ = new yal::ArrayCtrNode(yal::BisonYyltypeToLocation(yylloc));}
;

array_ctr: TK_ARRAY_BGN array_ctr_exp TK_ARRAY_END { $$ = $2;}
;

//NLS:TK_NL NLS
//  | TK_NL
//  ;
%%
