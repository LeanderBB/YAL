/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_HOME_LBEERNAERT_CODE_BUILD_YAL_QTCREATOR_LIB_PARSER_YAL_BISON_PARSER_HPP_INCLUDED
# define YY_YY_HOME_LBEERNAERT_CODE_BUILD_YAL_QTCREATOR_LIB_PARSER_YAL_BISON_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 24 "parser.y" /* yacc.c:1909  */

#include <yal/yal.h>
#include <yal/parser/parser_state.h>
#include <yal/memory/memory_utils.h>
#include <yal/parser/bison_utils.h>
#include <yal/util/constantvalue.h>
#include <yal/ast/asthdrs.h>
#include <yal/types/typehdrs.h>


#line 55 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.hpp" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TK_INT32 = 258,
    TK_UINT32 = 259,
    TK_INT64 = 260,
    TK_UINT64 = 261,
    TK_FLT32 = 262,
    TK_FLT64 = 263,
    TK_BOOL = 264,
    TK_TEXT = 265,
    TK_ID = 266,
    TK_FUNC_BEGIN = 267,
    TK_RETURN = 268,
    TK_VAR_DECL = 269,
    TK_SPACE = 270,
    TK_TYPE_BOOL = 271,
    TK_TYPE_INT32 = 272,
    TK_TYPE_UINT32 = 273,
    TK_TYPE_INT64 = 274,
    TK_TYPE_UINT64 = 275,
    TK_TYPE_F32 = 276,
    TK_TYPE_F64 = 277,
    TK_TYPE_STRING = 278,
    TK_IF = 279,
    TK_ELIF = 280,
    TK_ELSE = 281,
    TK_END = 282,
    END = 283,
    TK_NL = 284,
    TK_PRINT = 285,
    TK_WHILE = 286,
    TK_NATIVE = 287,
    TK_ATTRIB_EXCALL = 288,
    TK_SELF = 289,
    TK_OP_ASSIGN = 290,
    TK_OP_ASSIGN_PLUS = 291,
    TK_OP_ASSIGN_MINUS = 292,
    TK_OP_ASSIGN_MULT = 293,
    TK_OP_ASSIGN_DIV = 294,
    TK_OP_ASSIGN_AND = 295,
    TK_OP_ASSIGN_OR = 296,
    TK_OP_ASSIGN_XOR = 297,
    TK_OP_ASSIGN_SHIFT_LEFT = 298,
    TK_OP_ASSIGN_SHIFT_RIGHT = 299,
    TK_NOT = 300,
    TK_BIT_TIL = 301,
    TK_OR = 302,
    TK_AND = 303,
    TK_BIT_OR = 304,
    TK_BIT_XOR = 305,
    TK_BIT_AND = 306,
    TK_CMP_EQ = 307,
    TK_CMP_NE = 308,
    TK_CMP_GT = 309,
    TK_CMP_GE = 310,
    TK_CMP_LE = 311,
    TK_CMP_LT = 312,
    TK_SHIFT_LEFT = 313,
    TK_SHIFT_RIGHT = 314,
    TK_OP_PLUS = 315,
    TK_OP_MINUS = 316,
    TK_OP_DIV = 317,
    TK_OP_MULT = 318,
    TK_MOD = 319,
    TK_DOT = 320,
    TK_ARRAY_BGN = 321,
    TK_ARRAY_END = 322,
    TK_PREC_NEG = 323,
    TK_PREC_NOT = 324
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 41 "parser.y" /* yacc.c:1909  */

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

#line 160 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (void* scanner, yal::ParserState_t *state);

#endif /* !YY_YY_HOME_LBEERNAERT_CODE_BUILD_YAL_QTCREATOR_LIB_PARSER_YAL_BISON_PARSER_HPP_INCLUDED  */
