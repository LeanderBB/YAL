/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 23 "parserimpl.lemon"

#include <cassert>
#include <vector>
#include "yal/parser/parser.h"
#include "yal/parser/parserhelper.h"
#include "yal/util/log.h"
#include "yal/util/stringref.h"
#include "yal/frontend/module.h"
#include "yal/ast/typebuiltin.h"
#include "yal/ast/astnodes.h"
#include "yal/frontend/lexer/lexer.h"
#define YYMALLOCARGTYPE size_t
#line 41 "parserimpl.c"
#include "parserimpl.h"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
#if INTERFACE
#define YAL_TOKEN_ASSIGN                          1
#define YAL_TOKEN_OR                              2
#define YAL_TOKEN_AND                             3
#define YAL_TOKEN_BIT_OR                          4
#define YAL_TOKEN_BIT_XOR                         5
#define YAL_TOKEN_BIT_AND                         6
#define YAL_TOKEN_EQ                              7
#define YAL_TOKEN_NE                              8
#define YAL_TOKEN_GT                              9
#define YAL_TOKEN_LT                             10
#define YAL_TOKEN_GE                             11
#define YAL_TOKEN_LE                             12
#define YAL_TOKEN_PLUS                           13
#define YAL_TOKEN_MINUS                          14
#define YAL_TOKEN_MULT                           15
#define YAL_TOKEN_DIV                            16
#define YAL_TOKEN_MOD                            17
#define YAL_TOKEN_NEGATE                         18
#define YAL_TOKEN_BIT_NOT                        19
#define YAL_TOKEN_NOT                            20
#define YAL_TOKEN_DOT                            21
#define YAL_TOKEN_END                            22
#define YAL_TOKEN_IDENTIFIER                     23
#define YAL_TOKEN_MUT                            24
#define YAL_TOKEN_REFERENCE                      25
#define YAL_TOKEN_TYPE_BOOL                      26
#define YAL_TOKEN_TYPE_INT8                      27
#define YAL_TOKEN_TYPE_UINT8                     28
#define YAL_TOKEN_TYPE_INT16                     29
#define YAL_TOKEN_TYPE_UINT16                    30
#define YAL_TOKEN_TYPE_INT32                     31
#define YAL_TOKEN_TYPE_UINT32                    32
#define YAL_TOKEN_TYPE_INT64                     33
#define YAL_TOKEN_TYPE_UINT64                    34
#define YAL_TOKEN_TYPE_FLOAT                     35
#define YAL_TOKEN_TYPE_DOUBLE                    36
#define YAL_TOKEN_ARRAY_BEGIN                    37
#define YAL_TOKEN_ARRAY_END                      38
#define YAL_TOKEN_TYPE                           39
#define YAL_TOKEN_COLON                          40
#define YAL_TOKEN_SEMI_COLON                     41
#define YAL_TOKEN_STRUCT                         42
#define YAL_TOKEN_SCOPE_BEGIN                    43
#define YAL_TOKEN_SCOPE_END                      44
#define YAL_TOKEN_COMMA                          45
#define YAL_TOKEN_FUNCTION                       46
#define YAL_TOKEN_PAR_BEGIN                      47
#define YAL_TOKEN_PAR_END                        48
#define YAL_TOKEN_SELF                           49
#define YAL_TOKEN_RETURN                         50
#define YAL_TOKEN_VAR                            51
#define YAL_TOKEN_LET                            52
#define YAL_TOKEN_RANGE_CAST                     53
#define YAL_TOKEN_INTEGER_LITERAL                54
#define YAL_TOKEN_FLOAT_LITERAL                  55
#define YAL_TOKEN_BOOL_LITERAL                   56
#endif
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    YALParserTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is YALParserTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    YALParserARG_SDECL     A static variable declaration for the %extra_argument
**    YALParserARG_PDECL     A parameter declaration for the %extra_argument
**    YALParserARG_STORE     Code to store %extra_argument into yypParser
**    YALParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 98
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALParserTOKENTYPE yal::TokenInfo
#endif
typedef union {
  int yyinit;
  YALParserTOKENTYPE yy0;
  yal::StatementList* yy2;
  yal::StringRefPod yy8;
  yal::StructMemberInit* yy14;
  yal::DeclVar* yy44;
  yal::StmtExpression* yy49;
  yal::DeclBase* yy60;
  yal::RefType* yy64;
  yal::ExprBinaryOperator* yy69;
  yal::ExprStructInit* yy95;
  yal::ExprUnaryOperator* yy117;
  yal::DeclParamVarContainer* yy120;
  yal::DeclTypeFunction* yy129;
  yal::ExprList* yy146;
  yal::DeclStruct* yy152;
  yal::DeclFunction* yy161;
  yal::Statement* yy170;
  yal::DeclParamVar* yy173;
  uint32_t yy174;
  yal::DeclModule* yy175;
  yal::StmtDecl* yy176;
  yal::StructMemberInitList* yy188;
  yal::DeclStructMembers* yy193;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#if INTERFACE
#define YALParserARG_SDECL  yal::Parser *pParser ;
#define YALParserARG_PDECL , yal::Parser *pParser 
#define YALParserARG_FETCH  yal::Parser *pParser  = yypParser->pParser 
#define YALParserARG_STORE yypParser->pParser  = pParser 
#endif
#define YYNSTATE             128
#define YYNRULE              104
#define YY_MAX_SHIFT         127
#define YY_MIN_SHIFTREDUCE   192
#define YY_MAX_SHIFTREDUCE   295
#define YY_MIN_REDUCE        296
#define YY_MAX_REDUCE        399
#define YY_ERROR_ACTION      400
#define YY_ACCEPT_ACTION     401
#define YY_NO_ACTION         402
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if:
**    (1)  The yy_shift_ofst[S]+X value is out of range, or
**    (2)  yy_lookahead[yy_shift_ofst[S]+X] is not equal to X, or
**    (3)  yy_shift_ofst[S] equal YY_SHIFT_USE_DFLT.
** (Implementation note: YY_SHIFT_USE_DFLT is chosen so that
** YY_SHIFT_USE_DFLT+X will be out of range for all possible lookaheads X.
** Hence only tests (1) and (2) need to be evaluated.)
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (905)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    14,  118,  119,  201,  118,   15,   16,   86,   90,   76,
 /*    10 */   340,   13,  204,  205,  206,  207,  208,  209,  210,  211,
 /*    20 */   212,  213,  214,   26,   25,   24,  127,  303,   97,  112,
 /*    30 */   120,   89,  337,   32,   40,  262,    8,   94,   92,  110,
 /*    40 */   291,  292,  293,  126,    7,  401,  124,  330,   14,   47,
 /*    50 */    35,  392,  122,   15,   16,   36,  356,   76,   33,   13,
 /*    60 */   204,  205,  206,  207,  208,  209,  210,  211,  212,  213,
 /*    70 */   214,  119,  201,   36,  109,  249,   36,  342,   36,  343,
 /*    80 */    37,   32,   91,  262,  301,  124,  124,  110,  291,  292,
 /*    90 */   293,  116,  307,   36,  325,  339,   14,   79,  393,   87,
 /*   100 */   338,   15,   16,   82,   93,   76,  340,   13,  204,  205,
 /*   110 */   206,  207,  208,  209,  210,  211,  212,  213,  214,   17,
 /*   120 */   324,   83,  263,  301,  301,  302,  326,  104,  302,   32,
 /*   130 */    43,  262,    6,  286,   51,  110,  291,  292,  293,   11,
 /*   140 */    29,   30,  218,   55,   44,   23,   22,   18,   20,   19,
 /*   150 */    21,   28,   27,   26,   25,   24,   29,   30,  323,  112,
 /*   160 */   229,   23,   22,   18,   20,   19,   21,   28,   27,   26,
 /*   170 */    25,   24,   53,   52,   17,  112,  224,  264,  198,  247,
 /*   180 */     1,  204,  205,  206,  207,  208,  209,  210,  211,  212,
 /*   190 */   213,  214,   52,   17,   46,  248,  265,  217,   29,   30,
 /*   200 */    88,   42,    9,   23,   22,   18,   20,   19,   21,   28,
 /*   210 */    27,   26,   25,   24,   29,   30,  228,  112,   49,   23,
 /*   220 */    22,   18,   20,   19,   21,   28,   27,   26,   25,   24,
 /*   230 */    10,   50,  246,  112,  225,  112,    4,  245,   12,  102,
 /*   240 */    29,   30,  103,  108,   31,   23,   22,   18,   20,   19,
 /*   250 */    21,   28,   27,   26,   25,   24,   29,   30,   38,  112,
 /*   260 */   259,   23,   22,   18,   20,   19,   21,   28,   27,   26,
 /*   270 */    25,   24,    5,  111,    2,  112,  227,  114,   45,  115,
 /*   280 */   327,   39,  202,   41,  295,  215,  254,  125,  123,   34,
 /*   290 */    30,  398,  298,  298,   23,   22,   18,   20,   19,   21,
 /*   300 */    28,   27,   26,   25,   24,  124,  298,  298,  112,  298,
 /*   310 */   298,   81,  298,  124,  298,  298,  298,  298,  298,   81,
 /*   320 */   298,  298,  298,  298,    3,   84,  348,   56,  360,  361,
 /*   330 */   359,   95,    3,   96,  348,   56,  360,  361,  359,   95,
 /*   340 */   362,  298,  298,  301,  298,  298,  298,  298,  362,  117,
 /*   350 */   298,  301,  204,  205,  206,  207,  208,  209,  210,  211,
 /*   360 */   212,  213,  214,  198,  298,  298,  204,  205,  206,  207,
 /*   370 */   208,  209,  210,  211,  212,  213,  214,  400,  400,  400,
 /*   380 */   400,  400,  400,   28,   27,   26,   25,   24,  298,  298,
 /*   390 */   298,  112,   23,   22,   18,   20,   19,   21,   28,   27,
 /*   400 */    26,   25,   24,  298,  298,  298,  112,  124,  298,  298,
 /*   410 */   298,  298,  124,   81,  298,  298,  124,  298,   81,  298,
 /*   420 */   298,  298,   81,  298,  298,  298,  298,  298,  347,   56,
 /*   430 */   360,  361,  359,   95,   65,  360,  361,  359,   65,  360,
 /*   440 */   361,  359,  362,  298,   80,  301,  124,  362,   78,  298,
 /*   450 */   301,  362,   81,  298,  301,  298,  298,  298,  298,  298,
 /*   460 */   297,  298,  298,  298,  298,  298,  298,  298,   65,  360,
 /*   470 */   361,  359,  113,   54,  298,   85,   48,  124,   77,  298,
 /*   480 */   298,  362,  298,   81,  301,  298,  296,  320,  121,  298,
 /*   490 */   298,  298,  298,  298,  298,  298,  298,  124,  298,   57,
 /*   500 */   360,  361,  359,   81,  124,  298,  298,  298,  298,  298,
 /*   510 */    81,  298,  362,  298,  298,  301,  298,  298,  298,   61,
 /*   520 */   360,  361,  359,  298,  298,  298,   62,  360,  361,  359,
 /*   530 */   124,  298,  362,  298,  124,  301,   81,  298,  298,  362,
 /*   540 */    81,  298,  301,  298,  298,  298,  298,  298,  298,  298,
 /*   550 */   298,  298,   58,  360,  361,  359,   63,  360,  361,  359,
 /*   560 */   124,  298,  298,  298,  298,  362,   81,  298,  301,  362,
 /*   570 */   298,  298,  301,  298,  298,  298,  298,  298,  298,  298,
 /*   580 */   124,  298,   98,  360,  361,  359,   81,  124,  298,  298,
 /*   590 */   298,  298,  298,   81,  298,  362,  298,  298,  301,  298,
 /*   600 */   298,  298,   99,  360,  361,  359,  298,  298,  298,  100,
 /*   610 */   360,  361,  359,  124,  298,  362,  298,  124,  301,   81,
 /*   620 */   298,  298,  362,   81,  298,  301,  298,  298,  298,  298,
 /*   630 */   298,  298,  298,  298,  298,  101,  360,  361,  359,   64,
 /*   640 */   360,  361,  359,  124,  298,  298,  298,  298,  362,   81,
 /*   650 */   298,  301,  362,  298,  298,  301,  298,  298,  298,  298,
 /*   660 */   298,  298,  298,  124,  298,   67,  360,  361,  359,   81,
 /*   670 */   124,  298,  298,  298,  298,  298,   81,  298,  362,  298,
 /*   680 */   298,  301,  298,  298,  298,   68,  360,  361,  359,  298,
 /*   690 */   298,  298,   69,  360,  361,  359,  124,  298,  362,  298,
 /*   700 */   124,  301,   81,  298,  298,  362,   81,  298,  301,  298,
 /*   710 */   298,  298,  298,  298,  298,  298,  298,  298,   70,  360,
 /*   720 */   361,  359,   71,  360,  361,  359,  124,  298,  298,  298,
 /*   730 */   298,  362,   81,  298,  301,  362,  298,  298,  301,  298,
 /*   740 */   298,  298,  298,  298,  298,  298,  124,  298,   72,  360,
 /*   750 */   361,  359,   81,  124,  298,  298,  298,  298,  298,   81,
 /*   760 */   298,  362,  298,  298,  301,  298,  298,  298,  105,  360,
 /*   770 */   361,  359,  298,  298,  298,  106,  360,  361,  359,  124,
 /*   780 */   298,  362,  298,  124,  301,   81,  298,  298,  362,   81,
 /*   790 */   298,  301,  298,  298,  298,  298,  298,  298,  298,  298,
 /*   800 */   298,  107,  360,  361,  359,   74,  360,  361,  359,  124,
 /*   810 */   298,  298,  298,  298,  362,   81,  298,  301,  362,  298,
 /*   820 */   298,  301,  298,  298,  298,  298,  298,  298,  298,  124,
 /*   830 */   298,   75,  360,  361,  359,   81,  124,  298,  298,  298,
 /*   840 */   298,  298,   81,  298,  362,  298,  298,  301,  298,  298,
 /*   850 */   298,   66,  360,  361,  359,  298,  298,  298,   73,  360,
 /*   860 */   361,  359,  124,  298,  362,  298,  124,  301,   81,  298,
 /*   870 */   298,  362,   81,  298,  301,  298,  298,  298,  298,  298,
 /*   880 */   298,  298,  298,  298,   59,  360,  361,  359,   60,  360,
 /*   890 */   361,  359,  298,  298,  298,  298,  298,  362,  298,  298,
 /*   900 */   301,  362,  298,  298,  301,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    14,   23,   24,   25,   23,   19,   20,   67,   68,   23,
 /*    10 */    70,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*    20 */    34,   35,   36,   15,   16,   17,   22,   49,   23,   21,
 /*    30 */    23,   91,   70,   47,   40,   49,   50,   51,   52,   53,
 /*    40 */    54,   55,   56,   39,   61,   62,   58,   63,   14,   76,
 /*    50 */    46,   95,   64,   19,   20,   91,   92,   23,   47,   25,
 /*    60 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*    70 */    36,   24,   25,   91,   92,   41,   91,   92,   91,   92,
 /*    80 */    40,   47,   65,   49,   96,   58,   58,   53,   54,   55,
 /*    90 */    56,   64,   64,   91,   92,   70,   14,   94,   95,   69,
 /*   100 */    70,   19,   20,   67,   65,   23,   70,   25,   26,   27,
 /*   110 */    28,   29,   30,   31,   32,   33,   34,   35,   36,   45,
 /*   120 */    88,   89,   48,   96,   96,   40,   63,   40,   43,   47,
 /*   130 */    43,   49,   47,   44,   45,   53,   54,   55,   56,    1,
 /*   140 */     2,    3,   44,   45,   47,    7,    8,    9,   10,   11,
 /*   150 */    12,   13,   14,   15,   16,   17,    2,    3,   88,   21,
 /*   160 */    44,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   170 */    16,   17,   73,   45,   45,   21,   48,   48,   23,   41,
 /*   180 */    43,   26,   27,   28,   29,   30,   31,   32,   33,   34,
 /*   190 */    35,   36,   45,   45,   45,   41,   48,   42,    2,    3,
 /*   200 */    49,   45,    1,    7,    8,    9,   10,   11,   12,   13,
 /*   210 */    14,   15,   16,   17,    2,    3,   48,   21,   23,    7,
 /*   220 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   230 */     1,   23,   41,   21,   44,   21,   47,   41,   40,   23,
 /*   240 */     2,    3,   40,    9,   47,    7,    8,    9,   10,   11,
 /*   250 */    12,   13,   14,   15,   16,   17,    2,    3,   10,   21,
 /*   260 */    48,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   270 */    16,   17,   47,   23,   43,   21,   23,   40,   43,   40,
 /*   280 */    47,   40,   25,   40,   38,   41,   48,   23,   37,   40,
 /*   290 */     3,    0,   97,   97,    7,    8,    9,   10,   11,   12,
 /*   300 */    13,   14,   15,   16,   17,   58,   97,   97,   21,   97,
 /*   310 */    97,   64,   97,   58,   97,   97,   97,   97,   97,   64,
 /*   320 */    97,   97,   97,   97,   77,   78,   79,   80,   81,   82,
 /*   330 */    83,   84,   77,   78,   79,   80,   81,   82,   83,   84,
 /*   340 */    93,   97,   97,   96,   97,   97,   97,   97,   93,   23,
 /*   350 */    97,   96,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   360 */    34,   35,   36,   23,   97,   97,   26,   27,   28,   29,
 /*   370 */    30,   31,   32,   33,   34,   35,   36,    7,    8,    9,
 /*   380 */    10,   11,   12,   13,   14,   15,   16,   17,   97,   97,
 /*   390 */    97,   21,    7,    8,    9,   10,   11,   12,   13,   14,
 /*   400 */    15,   16,   17,   97,   97,   97,   21,   58,   97,   97,
 /*   410 */    97,   97,   58,   64,   97,   97,   58,   97,   64,   97,
 /*   420 */    97,   97,   64,   97,   97,   97,   97,   97,   79,   80,
 /*   430 */    81,   82,   83,   84,   80,   81,   82,   83,   80,   81,
 /*   440 */    82,   83,   93,   97,   90,   96,   58,   93,   90,   97,
 /*   450 */    96,   93,   64,   97,   96,   97,   97,   97,   97,   97,
 /*   460 */    59,   60,   97,   97,   97,   97,   97,   97,   80,   81,
 /*   470 */    82,   83,   71,   72,   97,   74,   75,   58,   90,   97,
 /*   480 */    97,   93,   97,   64,   96,   97,   85,   86,   87,   97,
 /*   490 */    97,   97,   97,   97,   97,   97,   97,   58,   97,   80,
 /*   500 */    81,   82,   83,   64,   58,   97,   97,   97,   97,   97,
 /*   510 */    64,   97,   93,   97,   97,   96,   97,   97,   97,   80,
 /*   520 */    81,   82,   83,   97,   97,   97,   80,   81,   82,   83,
 /*   530 */    58,   97,   93,   97,   58,   96,   64,   97,   97,   93,
 /*   540 */    64,   97,   96,   97,   97,   97,   97,   97,   97,   97,
 /*   550 */    97,   97,   80,   81,   82,   83,   80,   81,   82,   83,
 /*   560 */    58,   97,   97,   97,   97,   93,   64,   97,   96,   93,
 /*   570 */    97,   97,   96,   97,   97,   97,   97,   97,   97,   97,
 /*   580 */    58,   97,   80,   81,   82,   83,   64,   58,   97,   97,
 /*   590 */    97,   97,   97,   64,   97,   93,   97,   97,   96,   97,
 /*   600 */    97,   97,   80,   81,   82,   83,   97,   97,   97,   80,
 /*   610 */    81,   82,   83,   58,   97,   93,   97,   58,   96,   64,
 /*   620 */    97,   97,   93,   64,   97,   96,   97,   97,   97,   97,
 /*   630 */    97,   97,   97,   97,   97,   80,   81,   82,   83,   80,
 /*   640 */    81,   82,   83,   58,   97,   97,   97,   97,   93,   64,
 /*   650 */    97,   96,   93,   97,   97,   96,   97,   97,   97,   97,
 /*   660 */    97,   97,   97,   58,   97,   80,   81,   82,   83,   64,
 /*   670 */    58,   97,   97,   97,   97,   97,   64,   97,   93,   97,
 /*   680 */    97,   96,   97,   97,   97,   80,   81,   82,   83,   97,
 /*   690 */    97,   97,   80,   81,   82,   83,   58,   97,   93,   97,
 /*   700 */    58,   96,   64,   97,   97,   93,   64,   97,   96,   97,
 /*   710 */    97,   97,   97,   97,   97,   97,   97,   97,   80,   81,
 /*   720 */    82,   83,   80,   81,   82,   83,   58,   97,   97,   97,
 /*   730 */    97,   93,   64,   97,   96,   93,   97,   97,   96,   97,
 /*   740 */    97,   97,   97,   97,   97,   97,   58,   97,   80,   81,
 /*   750 */    82,   83,   64,   58,   97,   97,   97,   97,   97,   64,
 /*   760 */    97,   93,   97,   97,   96,   97,   97,   97,   80,   81,
 /*   770 */    82,   83,   97,   97,   97,   80,   81,   82,   83,   58,
 /*   780 */    97,   93,   97,   58,   96,   64,   97,   97,   93,   64,
 /*   790 */    97,   96,   97,   97,   97,   97,   97,   97,   97,   97,
 /*   800 */    97,   80,   81,   82,   83,   80,   81,   82,   83,   58,
 /*   810 */    97,   97,   97,   97,   93,   64,   97,   96,   93,   97,
 /*   820 */    97,   96,   97,   97,   97,   97,   97,   97,   97,   58,
 /*   830 */    97,   80,   81,   82,   83,   64,   58,   97,   97,   97,
 /*   840 */    97,   97,   64,   97,   93,   97,   97,   96,   97,   97,
 /*   850 */    97,   80,   81,   82,   83,   97,   97,   97,   80,   81,
 /*   860 */    82,   83,   58,   97,   93,   97,   58,   96,   64,   97,
 /*   870 */    97,   93,   64,   97,   96,   97,   97,   97,   97,   97,
 /*   880 */    97,   97,   97,   97,   80,   81,   82,   83,   80,   81,
 /*   890 */    82,   83,   97,   97,   97,   97,   97,   93,   97,   97,
 /*   900 */    96,   93,   97,   97,   96,
};
#define YY_SHIFT_USE_DFLT (905)
#define YY_SHIFT_COUNT    (127)
#define YY_SHIFT_MIN      (-22)
#define YY_SHIFT_MAX      (385)
static const short yy_shift_ofst[] = {
 /*     0 */   905,  -14,  -14,  -14,   82,   82,   82,    4,   34,   82,
 /*    10 */    82,   82,   82,   82,   82,   82,   82,   82,   82,   82,
 /*    20 */    82,   82,   82,   82,   82,   82,   82,   82,   82,   82,
 /*    30 */    82,   82,   82,  -22,  155,  326,  340,   47,   47,   47,
 /*    40 */    47,   47,  -19,    5,  -19,    7,  -19,   -6,   11,   40,
 /*    50 */    40,    5,  -19,   -6,   97,    7,  138,  154,  196,  212,
 /*    60 */   238,  254,  254,  254,  254,  254,  287,  370,  370,  370,
 /*    70 */   370,  370,  370,  385,    8,    8,   85,   74,  129,   89,
 /*    80 */   148,   87,  128,   98,  116,  137,  147,  149,  156,  151,
 /*    90 */   168,  201,  195,  229,  208,  191,  190,  198,  214,  214,
 /*   100 */   214,  214,  189,  216,  202,  214,  214,  214,  197,  234,
 /*   110 */   248,  225,  250,  231,  253,  237,  239,  233,  241,  257,
 /*   120 */   243,  235,  244,  246,  251,  249,  264,  291,
};
#define YY_REDUCE_USE_DFLT (-61)
#define YY_REDUCE_COUNT (55)
#define YY_REDUCE_MIN   (-60)
#define YY_REDUCE_MAX   (808)
static const short yy_reduce_ofst[] = {
 /*     0 */   -17,  247,  255,  349,  354,  358,  388,  401,  419,  439,
 /*    10 */   446,  472,  476,  502,  522,  529,  555,  559,  585,  605,
 /*    20 */   612,  638,  642,  668,  688,  695,  721,  725,  751,  771,
 /*    30 */   778,  804,  808,  -60,  -12,   27,   28,  -36,  -18,  -15,
 /*    40 */   -13,    2,   30,    3,   36,   32,  -38,  -16,  -27,   17,
 /*    50 */    39,  -44,   25,   63,   99,   70,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   299,  345,  345,  346,  389,  389,  389,  400,  400,  400,
 /*    10 */   400,  400,  400,  400,  400,  400,  400,  400,  400,  400,
 /*    20 */   400,  400,  400,  400,  400,  400,  400,  400,  400,  400,
 /*    30 */   400,  400,  400,  341,  400,  400,  400,  303,  303,  303,
 /*    40 */   303,  303,  400,  391,  341,  400,  400,  344,  400,  357,
 /*    50 */   357,  400,  400,  344,  400,  400,  400,  400,  400,  400,
 /*    60 */   400,  355,  354,  394,  387,  388,  371,  382,  381,  380,
 /*    70 */   379,  378,  377,  370,  373,  372,  364,  400,  400,  400,
 /*    80 */   400,  400,  400,  400,  400,  400,  336,  335,  334,  400,
 /*    90 */   400,  400,  400,  400,  400,  400,  400,  400,  386,  385,
 /*   100 */   384,  383,  400,  400,  400,  376,  375,  374,  400,  400,
 /*   110 */   400,  365,  400,  400,  400,  400,  400,  302,  400,  304,
 /*   120 */   400,  400,  400,  400,  300,  400,  400,  400,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  YALParserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
  yyStackEntry *yystackEnd;            /* Last entry in the stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void YALParserTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "ASSIGN",        "OR",            "AND",         
  "BIT_OR",        "BIT_XOR",       "BIT_AND",       "EQ",          
  "NE",            "GT",            "LT",            "GE",          
  "LE",            "PLUS",          "MINUS",         "MULT",        
  "DIV",           "MOD",           "NEGATE",        "BIT_NOT",     
  "NOT",           "DOT",           "END",           "IDENTIFIER",  
  "MUT",           "REFERENCE",     "TYPE_BOOL",     "TYPE_INT8",   
  "TYPE_UINT8",    "TYPE_INT16",    "TYPE_UINT16",   "TYPE_INT32",  
  "TYPE_UINT32",   "TYPE_INT64",    "TYPE_UINT64",   "TYPE_FLOAT",  
  "TYPE_DOUBLE",   "ARRAY_BEGIN",   "ARRAY_END",     "TYPE",        
  "COLON",         "SEMI_COLON",    "STRUCT",        "SCOPE_BEGIN", 
  "SCOPE_END",     "COMMA",         "FUNCTION",      "PAR_BEGIN",   
  "PAR_END",       "SELF",          "RETURN",        "VAR",         
  "LET",           "RANGE_CAST",    "INTEGER_LITERAL",  "FLOAT_LITERAL",
  "BOOL_LITERAL",  "error",         "type_builtin",  "decl_function",
  "type_function_decl",  "decls",         "module",        "function_return_decl",
  "type_specifier",  "var_type_spec",  "named_decl",    "function_args_decl",
  "type_function_args_decl",  "type_function_args_decl_other",  "function_arg_decl",  "function_header",
  "function_start",  "function_param_list",  "type_function_header",  "type_function_start",
  "type_function_param_list",  "statement_list",  "statement_list_or_empty",  "statement",   
  "expression",    "unaryexp",      "binaryexp",     "literal",     
  "decl_var",      "decl_type",     "decl_struct",   "struct_header",
  "struct_decl_var",  "struct_decl_vars",  "function_call_args",  "qualifier",   
  "qualified_type",  "struct_init",   "struct_member_init_list",  "struct_member_init",
  "type_array",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "decls ::= decls decl_type",
 /*   1 */ "decls ::= decls decl_function",
 /*   2 */ "decls ::= decls type_function_decl",
 /*   3 */ "decls ::=",
 /*   4 */ "type_specifier ::= type_builtin",
 /*   5 */ "type_specifier ::= type_array",
 /*   6 */ "type_specifier ::= IDENTIFIER",
 /*   7 */ "qualifier ::=",
 /*   8 */ "qualifier ::= MUT",
 /*   9 */ "qualifier ::= REFERENCE",
 /*  10 */ "qualifier ::= MUT REFERENCE",
 /*  11 */ "qualified_type ::= qualifier type_specifier",
 /*  12 */ "type_builtin ::= TYPE_BOOL",
 /*  13 */ "type_builtin ::= TYPE_INT8",
 /*  14 */ "type_builtin ::= TYPE_UINT8",
 /*  15 */ "type_builtin ::= TYPE_INT16",
 /*  16 */ "type_builtin ::= TYPE_UINT16",
 /*  17 */ "type_builtin ::= TYPE_INT32",
 /*  18 */ "type_builtin ::= TYPE_UINT32",
 /*  19 */ "type_builtin ::= TYPE_INT64",
 /*  20 */ "type_builtin ::= TYPE_UINT64",
 /*  21 */ "type_builtin ::= TYPE_FLOAT",
 /*  22 */ "type_builtin ::= TYPE_DOUBLE",
 /*  23 */ "decl_type ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON",
 /*  24 */ "decl_type ::= decl_struct",
 /*  25 */ "struct_header ::= TYPE IDENTIFIER COLON STRUCT",
 /*  26 */ "decl_struct ::= struct_header SCOPE_BEGIN struct_decl_vars SCOPE_END",
 /*  27 */ "struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var",
 /*  28 */ "struct_decl_vars ::= struct_decl_var",
 /*  29 */ "struct_decl_var ::= IDENTIFIER COLON qualified_type",
 /*  30 */ "function_header ::= function_start function_param_list function_return_decl",
 /*  31 */ "function_start ::= FUNCTION IDENTIFIER",
 /*  32 */ "function_param_list ::= PAR_BEGIN function_args_decl PAR_END",
 /*  33 */ "decl_function ::= function_header SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  34 */ "type_function_header ::= type_function_start type_function_param_list function_return_decl",
 /*  35 */ "type_function_start ::= FUNCTION type_specifier COLON COLON IDENTIFIER",
 /*  36 */ "type_function_param_list ::= PAR_BEGIN type_function_args_decl PAR_END",
 /*  37 */ "type_function_decl ::= type_function_header SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  38 */ "type_function_args_decl ::= qualifier SELF",
 /*  39 */ "type_function_args_decl ::= qualifier SELF COMMA type_function_args_decl_other",
 /*  40 */ "type_function_args_decl ::= function_args_decl",
 /*  41 */ "type_function_args_decl_other ::= type_function_args_decl_other COMMA function_arg_decl",
 /*  42 */ "type_function_args_decl_other ::= function_arg_decl",
 /*  43 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  44 */ "function_args_decl ::= function_arg_decl",
 /*  45 */ "function_args_decl ::=",
 /*  46 */ "function_arg_decl ::= IDENTIFIER COLON qualified_type",
 /*  47 */ "function_return_decl ::= COLON qualified_type",
 /*  48 */ "function_return_decl ::=",
 /*  49 */ "statement_list_or_empty ::=",
 /*  50 */ "statement_list_or_empty ::= statement_list",
 /*  51 */ "statement_list ::= statement_list statement",
 /*  52 */ "statement_list ::= statement",
 /*  53 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  54 */ "statement ::= decl_var SEMI_COLON",
 /*  55 */ "statement ::= expression SEMI_COLON",
 /*  56 */ "statement ::= RETURN expression SEMI_COLON",
 /*  57 */ "statement ::= RETURN SEMI_COLON",
 /*  58 */ "decl_var ::= VAR IDENTIFIER var_type_spec ASSIGN expression",
 /*  59 */ "decl_var ::= LET IDENTIFIER var_type_spec ASSIGN expression",
 /*  60 */ "var_type_spec ::= COLON qualified_type",
 /*  61 */ "var_type_spec ::=",
 /*  62 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  63 */ "expression ::= literal",
 /*  64 */ "expression ::= unaryexp",
 /*  65 */ "expression ::= binaryexp",
 /*  66 */ "expression ::= struct_init",
 /*  67 */ "expression ::= RANGE_CAST LT qualified_type GT PAR_BEGIN expression PAR_END",
 /*  68 */ "expression ::= IDENTIFIER",
 /*  69 */ "expression ::= expression DOT IDENTIFIER",
 /*  70 */ "expression ::= SELF",
 /*  71 */ "expression ::= IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  72 */ "expression ::= expression DOT IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  73 */ "expression ::= type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  74 */ "binaryexp ::= expression AND expression",
 /*  75 */ "binaryexp ::= expression OR expression",
 /*  76 */ "binaryexp ::= expression PLUS expression",
 /*  77 */ "binaryexp ::= expression MINUS expression",
 /*  78 */ "binaryexp ::= expression MULT expression",
 /*  79 */ "binaryexp ::= expression DIV expression",
 /*  80 */ "binaryexp ::= expression MOD expression",
 /*  81 */ "binaryexp ::= expression EQ expression",
 /*  82 */ "binaryexp ::= expression NE expression",
 /*  83 */ "binaryexp ::= expression LE expression",
 /*  84 */ "binaryexp ::= expression LT expression",
 /*  85 */ "binaryexp ::= expression GE expression",
 /*  86 */ "binaryexp ::= expression GT expression",
 /*  87 */ "unaryexp ::= NOT expression",
 /*  88 */ "unaryexp ::= BIT_NOT expression",
 /*  89 */ "unaryexp ::= MINUS expression",
 /*  90 */ "unaryexp ::= REFERENCE expression",
 /*  91 */ "function_call_args ::= function_call_args COMMA expression",
 /*  92 */ "function_call_args ::= expression",
 /*  93 */ "function_call_args ::=",
 /*  94 */ "struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END",
 /*  95 */ "struct_member_init_list ::=",
 /*  96 */ "struct_member_init_list ::= struct_member_init_list COMMA struct_member_init",
 /*  97 */ "struct_member_init_list ::= struct_member_init",
 /*  98 */ "struct_member_init ::= IDENTIFIER COLON expression",
 /*  99 */ "literal ::= INTEGER_LITERAL",
 /* 100 */ "literal ::= FLOAT_LITERAL",
 /* 101 */ "literal ::= BOOL_LITERAL",
 /* 102 */ "module ::= decls END",
 /* 103 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to YALParserAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void YALParserInit(void *yypParser){
  yyParser *pParser = (yyParser*)yypParser;
#ifdef YYTRACKMAXSTACKDEPTH
  pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
  pParser->yytos = NULL;
  pParser->yystack = NULL;
  pParser->yystksz = 0;
  if( yyGrowStack(pParser) ){
    pParser->yystack = &pParser->yystk0;
    pParser->yystksz = 1;
  }
#endif
#ifndef YYNOERRORRECOVERY
  pParser->yyerrcnt = -1;
#endif
  pParser->yytos = pParser->yystack;
  pParser->yystack[0].stateno = 0;
  pParser->yystack[0].major = 0;
#if YYSTACKDEPTH>0
  pParser->yystackEnd = &pParser->yystack[YYSTACKDEPTH-1];
#endif
}

#ifndef YALParser_ENGINEALWAYSONSTACK
/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to YALParser and YALParserFree.
*/
void *YALParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ) YALParserInit(pParser);
  return pParser;
}
#endif /* YALParser_ENGINEALWAYSONSTACK */


/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  YALParserARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/*
** Clear all secondary memory allocations from the parser
*/
void YALParserFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef YALParser_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void YALParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  YALParserFinalize(p);
  (*freeProc)(p);
}
#endif /* YALParser_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int YALParserStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   YALParserARG_FETCH;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   YALParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YALParserTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>yypParser->yystackEnd ){
    yypParser->yytos--;
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yypParser->yytos--;
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;       /* Symbol on the left-hand side of the rule */
  signed char nrhs;     /* Negative of the number of RHS symbols in the rule */
} yyRuleInfo[] = {
  { 61, -2 },
  { 61, -2 },
  { 61, -2 },
  { 61, 0 },
  { 64, -1 },
  { 64, -1 },
  { 64, -1 },
  { 91, 0 },
  { 91, -1 },
  { 91, -1 },
  { 91, -2 },
  { 92, -2 },
  { 58, -1 },
  { 58, -1 },
  { 58, -1 },
  { 58, -1 },
  { 58, -1 },
  { 58, -1 },
  { 58, -1 },
  { 58, -1 },
  { 58, -1 },
  { 58, -1 },
  { 58, -1 },
  { 85, -5 },
  { 85, -1 },
  { 87, -4 },
  { 86, -4 },
  { 89, -3 },
  { 89, -1 },
  { 88, -3 },
  { 71, -3 },
  { 72, -2 },
  { 73, -3 },
  { 59, -4 },
  { 74, -3 },
  { 75, -5 },
  { 76, -3 },
  { 60, -4 },
  { 68, -2 },
  { 68, -4 },
  { 68, -1 },
  { 69, -3 },
  { 69, -1 },
  { 67, -3 },
  { 67, -1 },
  { 67, 0 },
  { 70, -3 },
  { 63, -2 },
  { 63, 0 },
  { 78, 0 },
  { 78, -1 },
  { 77, -2 },
  { 77, -1 },
  { 79, -4 },
  { 79, -2 },
  { 79, -2 },
  { 79, -3 },
  { 79, -2 },
  { 84, -5 },
  { 84, -5 },
  { 65, -2 },
  { 65, 0 },
  { 80, -3 },
  { 80, -1 },
  { 80, -1 },
  { 80, -1 },
  { 80, -1 },
  { 80, -7 },
  { 80, -1 },
  { 80, -3 },
  { 80, -1 },
  { 80, -4 },
  { 80, -6 },
  { 80, -7 },
  { 82, -3 },
  { 82, -3 },
  { 82, -3 },
  { 82, -3 },
  { 82, -3 },
  { 82, -3 },
  { 82, -3 },
  { 82, -3 },
  { 82, -3 },
  { 82, -3 },
  { 82, -3 },
  { 82, -3 },
  { 82, -3 },
  { 81, -2 },
  { 81, -2 },
  { 81, -2 },
  { 81, -2 },
  { 90, -3 },
  { 90, -1 },
  { 90, 0 },
  { 93, -4 },
  { 94, 0 },
  { 94, -3 },
  { 94, -1 },
  { 95, -3 },
  { 83, -1 },
  { 83, -1 },
  { 83, -1 },
  { 62, -2 },
  { 96, -3 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno        /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  YALParserARG_FETCH;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=yypParser->yystackEnd ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* decls ::= decls decl_type */
#line 115 "parserimpl.lemon"
{
    yylhsminor.yy175 =yymsp[-1].minor.yy175; yymsp[-1].minor.yy175->addDecl(yymsp[0].minor.yy60);
}
#line 1295 "parserimpl.c"
  yymsp[-1].minor.yy175 = yylhsminor.yy175;
        break;
      case 1: /* decls ::= decls decl_function */
#line 118 "parserimpl.lemon"
{
        yylhsminor.yy175 =yymsp[-1].minor.yy175; yymsp[-1].minor.yy175->addDecl(yymsp[0].minor.yy161);
}
#line 1303 "parserimpl.c"
  yymsp[-1].minor.yy175 = yylhsminor.yy175;
        break;
      case 2: /* decls ::= decls type_function_decl */
#line 121 "parserimpl.lemon"
{
        yylhsminor.yy175 =yymsp[-1].minor.yy175; yymsp[-1].minor.yy175->addDecl(yymsp[0].minor.yy129);
}
#line 1311 "parserimpl.c"
  yymsp[-1].minor.yy175 = yylhsminor.yy175;
        break;
      case 3: /* decls ::= */
#line 124 "parserimpl.lemon"
{
        yymsp[1].minor.yy175 =  pParser->getModule().getDeclNode();
     }
#line 1319 "parserimpl.c"
        break;
      case 4: /* type_specifier ::= type_builtin */
#line 130 "parserimpl.lemon"
{
        yylhsminor.yy64=yymsp[0].minor.yy64;
}
#line 1326 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 5: /* type_specifier ::= type_array */
#line 133 "parserimpl.lemon"
{yymsp[0].minor.yy64 = nullptr;}
#line 1332 "parserimpl.c"
        break;
      case 6: /* type_specifier ::= IDENTIFIER */
#line 134 "parserimpl.lemon"
{
    auto type = pParser->resolveType(yymsp[0].minor.yy0);
    if (type != nullptr) {
        yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(type);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    } else {
        yylhsminor.yy64 = nullptr;
    }
}
#line 1346 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 7: /* qualifier ::= */
#line 145 "parserimpl.lemon"
{yymsp[1].minor.yy174 = 0;}
#line 1352 "parserimpl.c"
        break;
      case 8: /* qualifier ::= MUT */
#line 146 "parserimpl.lemon"
{yymsp[0].minor.yy174 = yal::parser::kQualifierMutable;}
#line 1357 "parserimpl.c"
        break;
      case 9: /* qualifier ::= REFERENCE */
#line 147 "parserimpl.lemon"
{yymsp[0].minor.yy174 = yal::parser::kQualifierReference;}
#line 1362 "parserimpl.c"
        break;
      case 10: /* qualifier ::= MUT REFERENCE */
#line 148 "parserimpl.lemon"
{
        yymsp[-1].minor.yy174 = yal::parser::kQualifierReference | yal::parser::kQualifierMutable;
 }
#line 1369 "parserimpl.c"
        break;
      case 11: /* qualified_type ::= qualifier type_specifier */
#line 151 "parserimpl.lemon"
{
    if(yymsp[0].minor.yy64) {
        yymsp[0].minor.yy64->setQualifier(yal::parser::MakeQualifierFromFlags(yymsp[-1].minor.yy174));
    }
    yylhsminor.yy64 = yymsp[0].minor.yy64;
}
#line 1379 "parserimpl.c"
  yymsp[-1].minor.yy64 = yylhsminor.yy64;
        break;
      case 12: /* type_builtin ::= TYPE_BOOL */
#line 158 "parserimpl.lemon"
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinBool());
    auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
    yylhsminor.yy64->setSourceInfo(srcInfo);
    }
#line 1389 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 13: /* type_builtin ::= TYPE_INT8 */
#line 163 "parserimpl.lemon"
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI8());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
#line 1399 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 14: /* type_builtin ::= TYPE_UINT8 */
#line 168 "parserimpl.lemon"
{
    yylhsminor.yy64  = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU8());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
#line 1409 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 15: /* type_builtin ::= TYPE_INT16 */
#line 173 "parserimpl.lemon"
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI16());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
#line 1419 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 16: /* type_builtin ::= TYPE_UINT16 */
#line 178 "parserimpl.lemon"
{
    yylhsminor.yy64  = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU16());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
#line 1429 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 17: /* type_builtin ::= TYPE_INT32 */
#line 183 "parserimpl.lemon"
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI32());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
#line 1439 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 18: /* type_builtin ::= TYPE_UINT32 */
#line 188 "parserimpl.lemon"
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU32());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
#line 1449 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 19: /* type_builtin ::= TYPE_INT64 */
#line 193 "parserimpl.lemon"
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI64());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
#line 1459 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 20: /* type_builtin ::= TYPE_UINT64 */
#line 198 "parserimpl.lemon"
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU64());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
#line 1469 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 21: /* type_builtin ::= TYPE_FLOAT */
#line 203 "parserimpl.lemon"
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinFloat32());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
#line 1479 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 22: /* type_builtin ::= TYPE_DOUBLE */
#line 208 "parserimpl.lemon"
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinFloat64());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
#line 1489 "parserimpl.c"
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 23: /* decl_type ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON */
#line 218 "parserimpl.lemon"
{
    yymsp[-4].minor.yy60 = nullptr;
}
#line 1497 "parserimpl.c"
        break;
      case 24: /* decl_type ::= decl_struct */
#line 222 "parserimpl.lemon"
{ yylhsminor.yy60 = yymsp[0].minor.yy152;}
#line 1502 "parserimpl.c"
  yymsp[0].minor.yy60 = yylhsminor.yy60;
        break;
      case 25: /* struct_header ::= TYPE IDENTIFIER COLON STRUCT */
#line 226 "parserimpl.lemon"
{
    yylhsminor.yy152 = pParser->newAstNode<yal::DeclStruct>(yymsp[-2].minor.yy0.tokenStr, pParser->getActiveScope());
    yylhsminor.yy152->setSourceInfo(pParser->createSourceInfo(yymsp[-3].minor.yy0, yymsp[-2].minor.yy0));
    pParser->onDeclBegin(yylhsminor.yy152);
}
#line 1512 "parserimpl.c"
  yymsp[-3].minor.yy152 = yylhsminor.yy152;
        break;
      case 26: /* decl_struct ::= struct_header SCOPE_BEGIN struct_decl_vars SCOPE_END */
#line 232 "parserimpl.lemon"
{
    yylhsminor.yy152 = yymsp[-3].minor.yy152;
    if (yymsp[-1].minor.yy193 != nullptr) {
        yymsp[-1].minor.yy193->updateSourceInfo();
        yylhsminor.yy152->setMembers(yymsp[-1].minor.yy193);
    }
    auto srcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy152->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy152->setSourceInfo(srcInfo);
    pParser->onDeclEnd();
}
#line 1527 "parserimpl.c"
  yymsp[-3].minor.yy152 = yylhsminor.yy152;
        break;
      case 27: /* struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
#line 243 "parserimpl.lemon"
{
    yylhsminor.yy193 = yymsp[-2].minor.yy193;
    if (pParser->onDecl(yymsp[0].minor.yy44)) {
        yylhsminor.yy193->addDeclVar(yymsp[0].minor.yy44);
    }
}
#line 1538 "parserimpl.c"
  yymsp[-2].minor.yy193 = yylhsminor.yy193;
        break;
      case 28: /* struct_decl_vars ::= struct_decl_var */
#line 249 "parserimpl.lemon"
{
    yylhsminor.yy193 = pParser->newAstNode<yal::DeclStructMembers>();
    if (pParser->onDecl(yymsp[0].minor.yy44)) {
        yylhsminor.yy193->addDeclVar(yymsp[0].minor.yy44);
    }
}
#line 1549 "parserimpl.c"
  yymsp[0].minor.yy193 = yylhsminor.yy193;
        break;
      case 29: /* struct_decl_var ::= IDENTIFIER COLON qualified_type */
#line 256 "parserimpl.lemon"
{
    if (yymsp[0].minor.yy64) {
        yylhsminor.yy44 = pParser->newAstNode<yal::DeclVar>(yymsp[-2].minor.yy0.tokenStr,
                                              pParser->getActiveScope(),
                                              yal::Qualifier(), yymsp[0].minor.yy64, nullptr);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy64->getSourceInfo());
        yylhsminor.yy44->setSourceInfo(srcInfo);
    } else {
        yylhsminor.yy44 = nullptr;
    }
}
#line 1565 "parserimpl.c"
  yymsp[-2].minor.yy44 = yylhsminor.yy44;
        break;
      case 30: /* function_header ::= function_start function_param_list function_return_decl */
#line 272 "parserimpl.lemon"
{
    yylhsminor.yy161 = yymsp[-2].minor.yy161;
    yylhsminor.yy161->setParams(yymsp[-1].minor.yy120);
    yylhsminor.yy161->setReturnType(yymsp[0].minor.yy64);

}
#line 1576 "parserimpl.c"
  yymsp[-2].minor.yy161 = yylhsminor.yy161;
        break;
      case 31: /* function_start ::= FUNCTION IDENTIFIER */
#line 279 "parserimpl.lemon"
{
     yylhsminor.yy161 = pParser->newAstNode<yal::DeclFunction>(yymsp[0].minor.yy0.tokenStr, pParser->getActiveScope());
     yylhsminor.yy161->setSourceInfo(pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy0));
     pParser->onDeclBegin(yylhsminor.yy161);
}
#line 1586 "parserimpl.c"
  yymsp[-1].minor.yy161 = yylhsminor.yy161;
        break;
      case 32: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
      case 36: /* type_function_param_list ::= PAR_BEGIN type_function_args_decl PAR_END */ yytestcase(yyruleno==36);
#line 285 "parserimpl.lemon"
{
    yymsp[-2].minor.yy120 = yymsp[-1].minor.yy120;
}
#line 1595 "parserimpl.c"
        break;
      case 33: /* decl_function ::= function_header SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 289 "parserimpl.lemon"
{
    yylhsminor.yy161 = yymsp[-3].minor.yy161;
    yylhsminor.yy161->setFunctionBody(yymsp[-1].minor.yy2);
    yylhsminor.yy161->setSourceInfo(pParser->createSourceInfo(yymsp[-3].minor.yy161->getSourceInfo(), yymsp[0].minor.yy0));
    pParser->onDeclEnd();
}
#line 1605 "parserimpl.c"
  yymsp[-3].minor.yy161 = yylhsminor.yy161;
        break;
      case 34: /* type_function_header ::= type_function_start type_function_param_list function_return_decl */
#line 298 "parserimpl.lemon"
{
    yylhsminor.yy129 = yymsp[-2].minor.yy129;
    yylhsminor.yy129->setParams(yymsp[-1].minor.yy120);
    yylhsminor.yy129->setReturnType(yymsp[0].minor.yy64);
}
#line 1615 "parserimpl.c"
  yymsp[-2].minor.yy129 = yylhsminor.yy129;
        break;
      case 35: /* type_function_start ::= FUNCTION type_specifier COLON COLON IDENTIFIER */
#line 304 "parserimpl.lemon"
{
    yylhsminor.yy129 = pParser->newAstNode<yal::DeclTypeFunction>(yymsp[0].minor.yy0.tokenStr,
                                                   pParser->getActiveScope(),
                                                   yymsp[-3].minor.yy64);
    yylhsminor.yy129->setSourceInfo(pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0));
    pParser->onDeclBegin(yylhsminor.yy129);
}
#line 1627 "parserimpl.c"
  yymsp[-4].minor.yy129 = yylhsminor.yy129;
        break;
      case 37: /* type_function_decl ::= type_function_header SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 316 "parserimpl.lemon"
{
    yylhsminor.yy129 = yymsp[-3].minor.yy129;
    yylhsminor.yy129->setFunctionBody(yymsp[-1].minor.yy2);
    yylhsminor.yy129->setSourceInfo(pParser->createSourceInfo(yymsp[-3].minor.yy129->getSourceInfo(), yymsp[0].minor.yy0));
    pParser->onDeclEnd();
}
#line 1638 "parserimpl.c"
  yymsp[-3].minor.yy129 = yylhsminor.yy129;
        break;
      case 38: /* type_function_args_decl ::= qualifier SELF */
#line 326 "parserimpl.lemon"
{
        yylhsminor.yy120 = pParser->newAstNode<yal::DeclParamVarContainer>();
        auto qualifier = yal::parser::MakeQualifierFromFlags(yymsp[-1].minor.yy174);
        auto refType = pParser->newAstNode<yal::RefType>(qualifier, pParser->resolveSelfType());
        auto selfVar = pParser->newAstNode<yal::DeclParamVarSelf>(pParser->getActiveScope(),refType);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        selfVar->setSourceInfo(srcInfo);
        yylhsminor.yy120->addDeclParam(selfVar);
        pParser->onDecl(selfVar);
}
#line 1653 "parserimpl.c"
  yymsp[-1].minor.yy120 = yylhsminor.yy120;
        break;
      case 39: /* type_function_args_decl ::= qualifier SELF COMMA type_function_args_decl_other */
#line 337 "parserimpl.lemon"
{
    yylhsminor.yy120 = yymsp[0].minor.yy120;
    auto qualifier = yal::parser::MakeQualifierFromFlags(yymsp[-3].minor.yy174);
    auto refType = pParser->newAstNode<yal::RefType>(qualifier, pParser->resolveSelfType());
    auto selfVar = pParser->newAstNode<yal::DeclParamVarSelf>(pParser->getActiveScope(),refType);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[-2].minor.yy0);
    selfVar->setSourceInfo(srcInfo);
    yylhsminor.yy120->addDeclParam(selfVar);
    pParser->onDecl(selfVar);
}
#line 1668 "parserimpl.c"
  yymsp[-3].minor.yy120 = yylhsminor.yy120;
        break;
      case 40: /* type_function_args_decl ::= function_args_decl */
#line 348 "parserimpl.lemon"
{
        yylhsminor.yy120 = yymsp[0].minor.yy120;
}
#line 1676 "parserimpl.c"
  yymsp[0].minor.yy120 = yylhsminor.yy120;
        break;
      case 41: /* type_function_args_decl_other ::= type_function_args_decl_other COMMA function_arg_decl */
#line 352 "parserimpl.lemon"
{
    if (yymsp[0].minor.yy173 != nullptr && pParser->onDecl(yymsp[0].minor.yy173)) {
        yylhsminor.yy120 = yymsp[-2].minor.yy120;
        yylhsminor.yy120->addDeclParam(yymsp[0].minor.yy173);
    }
}
#line 1687 "parserimpl.c"
  yymsp[-2].minor.yy120 = yylhsminor.yy120;
        break;
      case 42: /* type_function_args_decl_other ::= function_arg_decl */
#line 359 "parserimpl.lemon"
{
    if (yymsp[0].minor.yy173 != nullptr && pParser->onDecl(yymsp[0].minor.yy173)) {
        yylhsminor.yy120 = pParser->newAstNode<yal::DeclParamVarContainer>();
        yylhsminor.yy120->addDeclParam(yymsp[0].minor.yy173);
    }
}
#line 1698 "parserimpl.c"
  yymsp[0].minor.yy120 = yylhsminor.yy120;
        break;
      case 43: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
#line 368 "parserimpl.lemon"
{
    if (yymsp[0].minor.yy173 != nullptr && pParser->onDecl(yymsp[0].minor.yy173)){
        yymsp[-2].minor.yy120->addDeclParam(yymsp[0].minor.yy173);
        yylhsminor.yy120 = yymsp[-2].minor.yy120;
    }
}
#line 1709 "parserimpl.c"
  yymsp[-2].minor.yy120 = yylhsminor.yy120;
        break;
      case 44: /* function_args_decl ::= function_arg_decl */
#line 375 "parserimpl.lemon"
{
    if (yymsp[0].minor.yy173 != nullptr && pParser->onDecl(yymsp[0].minor.yy173)){
        yylhsminor.yy120 = pParser->newAstNode<yal::DeclParamVarContainer>();
        yylhsminor.yy120->addDeclParam(yymsp[0].minor.yy173);
    }
}
#line 1720 "parserimpl.c"
  yymsp[0].minor.yy120 = yylhsminor.yy120;
        break;
      case 45: /* function_args_decl ::= */
#line 381 "parserimpl.lemon"
{
        yymsp[1].minor.yy120 = nullptr;
}
#line 1728 "parserimpl.c"
        break;
      case 46: /* function_arg_decl ::= IDENTIFIER COLON qualified_type */
#line 385 "parserimpl.lemon"
{
    if (yymsp[0].minor.yy64) {
        yylhsminor.yy173 = pParser->newAstNode<yal::DeclParamVar>(yymsp[-2].minor.yy0.tokenStr,
                                                   pParser->getActiveScope(),
                                                   yal::Qualifier(),yymsp[0].minor.yy64);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy64->getSourceInfo());
        yylhsminor.yy173->setSourceInfo(srcInfo);
    } else {
        yylhsminor.yy173 = nullptr;
    }
}
#line 1743 "parserimpl.c"
  yymsp[-2].minor.yy173 = yylhsminor.yy173;
        break;
      case 47: /* function_return_decl ::= COLON qualified_type */
#line 397 "parserimpl.lemon"
{ yymsp[-1].minor.yy64 = yymsp[0].minor.yy64;
}
#line 1750 "parserimpl.c"
        break;
      case 48: /* function_return_decl ::= */
      case 61: /* var_type_spec ::= */ yytestcase(yyruleno==61);
#line 399 "parserimpl.lemon"
{yymsp[1].minor.yy64 = nullptr;}
#line 1756 "parserimpl.c"
        break;
      case 49: /* statement_list_or_empty ::= */
#line 403 "parserimpl.lemon"
{yymsp[1].minor.yy2 = nullptr;}
#line 1761 "parserimpl.c"
        break;
      case 50: /* statement_list_or_empty ::= statement_list */
#line 404 "parserimpl.lemon"
{yylhsminor.yy2 = yymsp[0].minor.yy2;}
#line 1766 "parserimpl.c"
  yymsp[0].minor.yy2 = yylhsminor.yy2;
        break;
      case 51: /* statement_list ::= statement_list statement */
#line 407 "parserimpl.lemon"
{
    yymsp[-1].minor.yy2->addStatement(yymsp[0].minor.yy170);
    yylhsminor.yy2=yymsp[-1].minor.yy2;
}
#line 1775 "parserimpl.c"
  yymsp[-1].minor.yy2 = yylhsminor.yy2;
        break;
      case 52: /* statement_list ::= statement */
#line 411 "parserimpl.lemon"
{
    yylhsminor.yy2 = pParser->newAstNode<yal::StatementList>();
    yylhsminor.yy2->addStatement(yymsp[0].minor.yy170);
}
#line 1784 "parserimpl.c"
  yymsp[0].minor.yy2 = yylhsminor.yy2;
        break;
      case 53: /* statement ::= expression ASSIGN expression SEMI_COLON */
#line 417 "parserimpl.lemon"
{
   yylhsminor.yy170 = pParser->newAstNode<yal::StmtAssign>(yymsp[-3].minor.yy49,yymsp[-1].minor.yy49);
}
#line 1792 "parserimpl.c"
  yymsp[-3].minor.yy170 = yylhsminor.yy170;
        break;
      case 54: /* statement ::= decl_var SEMI_COLON */
#line 420 "parserimpl.lemon"
{
    yylhsminor.yy170 = yymsp[-1].minor.yy176;
}
#line 1800 "parserimpl.c"
  yymsp[-1].minor.yy170 = yylhsminor.yy170;
        break;
      case 55: /* statement ::= expression SEMI_COLON */
#line 423 "parserimpl.lemon"
{yylhsminor.yy170 = yymsp[-1].minor.yy49;}
#line 1806 "parserimpl.c"
  yymsp[-1].minor.yy170 = yylhsminor.yy170;
        break;
      case 56: /* statement ::= RETURN expression SEMI_COLON */
#line 424 "parserimpl.lemon"
{
    yylhsminor.yy170 = pParser->newAstNode<yal::StmtReturn>(yymsp[-1].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy49->getSourceInfo());
    yylhsminor.yy170->setSourceInfo(srcInfo);
}
#line 1816 "parserimpl.c"
  yymsp[-2].minor.yy170 = yylhsminor.yy170;
        break;
      case 57: /* statement ::= RETURN SEMI_COLON */
#line 429 "parserimpl.lemon"
{
    yylhsminor.yy170 = pParser->newAstNode<yal::StmtReturn>();
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0,yymsp[-1].minor.yy0);
    yylhsminor.yy170->setSourceInfo(srcInfo);
}
#line 1826 "parserimpl.c"
  yymsp[-1].minor.yy170 = yylhsminor.yy170;
        break;
      case 58: /* decl_var ::= VAR IDENTIFIER var_type_spec ASSIGN expression */
#line 437 "parserimpl.lemon"
{
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr,
                                                     pParser->getActiveScope(),
                                                     yal::Qualifier(), yymsp[-2].minor.yy64, yymsp[0].minor.yy49);
    auto varSrcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[-3].minor.yy0);
    varDecl->setSourceInfo(varSrcInfo);
    if (pParser->onDecl(varDecl)) {
        yylhsminor.yy176= pParser->newAstNode<yal::StmtDecl>(varDecl);
        auto srcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy176->setSourceInfo(srcInfo);
    }
}
#line 1843 "parserimpl.c"
  yymsp[-4].minor.yy176 = yylhsminor.yy176;
        break;
      case 59: /* decl_var ::= LET IDENTIFIER var_type_spec ASSIGN expression */
#line 450 "parserimpl.lemon"
{
    yal::Qualifier qualifier;
    qualifier.setImmutable();
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr,
                                                     pParser->getActiveScope(),
                                                     qualifier, yymsp[-2].minor.yy64, yymsp[0].minor.yy49);
    auto varSrcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[-3].minor.yy0);
    varDecl->setSourceInfo(varSrcInfo);
    if (pParser->onDecl(varDecl)) {
        yylhsminor.yy176= pParser->newAstNode<yal::StmtDecl>(varDecl);
        auto srcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy176->setSourceInfo(srcInfo);
    }
}
#line 1862 "parserimpl.c"
  yymsp[-4].minor.yy176 = yylhsminor.yy176;
        break;
      case 60: /* var_type_spec ::= COLON qualified_type */
#line 465 "parserimpl.lemon"
{yymsp[-1].minor.yy64 = yymsp[0].minor.yy64;}
#line 1868 "parserimpl.c"
        break;
      case 62: /* expression ::= PAR_BEGIN expression PAR_END */
#line 471 "parserimpl.lemon"
{yymsp[-2].minor.yy49 = yymsp[-1].minor.yy49;}
#line 1873 "parserimpl.c"
        break;
      case 63: /* expression ::= literal */
#line 472 "parserimpl.lemon"
{yylhsminor.yy49 = yymsp[0].minor.yy49;}
#line 1878 "parserimpl.c"
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 64: /* expression ::= unaryexp */
#line 473 "parserimpl.lemon"
{yylhsminor.yy49 = yymsp[0].minor.yy117;}
#line 1884 "parserimpl.c"
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 65: /* expression ::= binaryexp */
#line 474 "parserimpl.lemon"
{yylhsminor.yy49 = yymsp[0].minor.yy69;}
#line 1890 "parserimpl.c"
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 66: /* expression ::= struct_init */
#line 475 "parserimpl.lemon"
{ yylhsminor.yy49 = yymsp[0].minor.yy95;}
#line 1896 "parserimpl.c"
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 67: /* expression ::= RANGE_CAST LT qualified_type GT PAR_BEGIN expression PAR_END */
#line 477 "parserimpl.lemon"
{
    yylhsminor.yy49 = pParser->newAstNode<yal::ExprRangeCast>(yymsp[-4].minor.yy64, yymsp[-1].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0);
    yylhsminor.yy49->setSourceInfo(srcInfo);
}
#line 1906 "parserimpl.c"
  yymsp[-6].minor.yy49 = yylhsminor.yy49;
        break;
      case 68: /* expression ::= IDENTIFIER */
#line 483 "parserimpl.lemon"
{
    auto decl = pParser->resolveVarRef(yymsp[0].minor.yy0);
    if (decl != nullptr) {
        yylhsminor.yy49 = pParser->newAstNode<yal::ExprVarRef>(decl);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy49->setSourceInfo(srcInfo);
    }
}
#line 1919 "parserimpl.c"
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 69: /* expression ::= expression DOT IDENTIFIER */
#line 492 "parserimpl.lemon"
{
    yylhsminor.yy49 = pParser->newAstNode<yal::ExprStructVarRef>(yymsp[-2].minor.yy49, yymsp[0].minor.yy0.tokenStr);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy49->setSourceInfo(srcInfo);
}
#line 1929 "parserimpl.c"
  yymsp[-2].minor.yy49 = yylhsminor.yy49;
        break;
      case 70: /* expression ::= SELF */
#line 498 "parserimpl.lemon"
{
    auto decl = pParser->resolveVarRefSelf(yymsp[0].minor.yy0);
    if (decl != nullptr) {
        yylhsminor.yy49  = pParser->newAstNode<yal::ExprVarRefSelf>(decl);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy49->setSourceInfo(srcInfo);
    }
}
#line 1942 "parserimpl.c"
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 71: /* expression ::= IDENTIFIER PAR_BEGIN function_call_args PAR_END */
#line 507 "parserimpl.lemon"
{
    auto type = pParser->resolveType(yymsp[-3].minor.yy0);
    if (type != nullptr ) {
        auto fnType = pParser->newAstNode<yal::RefType>(type);
        auto fnSrcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy0, yymsp[-3].minor.yy0);
        fnType->setSourceInfo(fnSrcInfo);
        if (yymsp[-1].minor.yy146 != nullptr) {
            yymsp[-1].minor.yy146->updateSourceInfo();
        }
        yylhsminor.yy49 = pParser->newAstNode<yal::ExprFnCall>(fnType, yymsp[-1].minor.yy146);
        auto srcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy49->setSourceInfo(srcInfo);
    }
}
#line 1961 "parserimpl.c"
  yymsp[-3].minor.yy49 = yylhsminor.yy49;
        break;
      case 72: /* expression ::= expression DOT IDENTIFIER PAR_BEGIN function_call_args PAR_END */
#line 522 "parserimpl.lemon"
{
    if (yymsp[-1].minor.yy146 != nullptr) {
        yymsp[-1].minor.yy146->updateSourceInfo();
    }
    yylhsminor.yy49 = pParser->newAstNode<yal::ExprTypeFnCall>(yymsp[-5].minor.yy49, yymsp[-3].minor.yy0, yymsp[-1].minor.yy146);
    auto srcInfo = pParser->createSourceInfo(yymsp[-5].minor.yy49->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy49->setSourceInfo(srcInfo);

}
#line 1975 "parserimpl.c"
  yymsp[-5].minor.yy49 = yylhsminor.yy49;
        break;
      case 73: /* expression ::= type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_call_args PAR_END */
#line 532 "parserimpl.lemon"
{
    if (yymsp[-1].minor.yy146 != nullptr) {
        yymsp[-1].minor.yy146->updateSourceInfo();
    }
    yylhsminor.yy49 = pParser->newAstNode<yal::ExprTypeFnCallStatic>(yymsp[-6].minor.yy64, yymsp[-3].minor.yy0, yymsp[-1].minor.yy146);
    auto srcInfo = pParser->createSourceInfo(yymsp[-6].minor.yy64->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy49->setSourceInfo(srcInfo);
}
#line 1988 "parserimpl.c"
  yymsp[-6].minor.yy49 = yylhsminor.yy49;
        break;
      case 74: /* binaryexp ::= expression AND expression */
#line 543 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 1999 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 75: /* binaryexp ::= expression OR expression */
#line 549 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 2010 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 76: /* binaryexp ::= expression PLUS expression */
#line 555 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 2021 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 77: /* binaryexp ::= expression MINUS expression */
#line 561 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 2032 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 78: /* binaryexp ::= expression MULT expression */
#line 567 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 2043 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 79: /* binaryexp ::= expression DIV expression */
#line 573 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
}
#line 2052 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 80: /* binaryexp ::= expression MOD expression */
#line 577 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 2063 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 81: /* binaryexp ::= expression EQ expression */
#line 584 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 2074 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 82: /* binaryexp ::= expression NE expression */
#line 591 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 2085 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 83: /* binaryexp ::= expression LE expression */
#line 598 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 2096 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 84: /* binaryexp ::= expression LT expression */
#line 605 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 2107 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 85: /* binaryexp ::= expression GE expression */
#line 612 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 2118 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 86: /* binaryexp ::= expression GT expression */
#line 619 "parserimpl.lemon"
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 2129 "parserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 87: /* unaryexp ::= NOT expression */
#line 628 "parserimpl.lemon"
{
    yylhsminor.yy117 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
    yylhsminor.yy117->setSourceInfo(srcInfo);
}
#line 2140 "parserimpl.c"
  yymsp[-1].minor.yy117 = yylhsminor.yy117;
        break;
      case 88: /* unaryexp ::= BIT_NOT expression */
#line 634 "parserimpl.lemon"
{
    yylhsminor.yy117 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
    yylhsminor.yy117->setSourceInfo(srcInfo);
}
#line 2151 "parserimpl.c"
  yymsp[-1].minor.yy117 = yylhsminor.yy117;
        break;
      case 89: /* unaryexp ::= MINUS expression */
#line 640 "parserimpl.lemon"
{
    yylhsminor.yy117 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
    yylhsminor.yy117->setSourceInfo(srcInfo);
}
#line 2162 "parserimpl.c"
  yymsp[-1].minor.yy117 = yylhsminor.yy117;
        break;
      case 90: /* unaryexp ::= REFERENCE expression */
#line 647 "parserimpl.lemon"
{
    yylhsminor.yy117 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
    yylhsminor.yy117->setSourceInfo(srcInfo);
}
#line 2173 "parserimpl.c"
  yymsp[-1].minor.yy117 = yylhsminor.yy117;
        break;
      case 91: /* function_call_args ::= function_call_args COMMA expression */
#line 656 "parserimpl.lemon"
{
    yylhsminor.yy146 = yymsp[-2].minor.yy146;
    yylhsminor.yy146->addExpression(yymsp[0].minor.yy49);
}
#line 2182 "parserimpl.c"
  yymsp[-2].minor.yy146 = yylhsminor.yy146;
        break;
      case 92: /* function_call_args ::= expression */
#line 660 "parserimpl.lemon"
{
    yylhsminor.yy146 = pParser->newAstNode<yal::ExprList>();
    yylhsminor.yy146->addExpression(yymsp[0].minor.yy49);
}
#line 2191 "parserimpl.c"
  yymsp[0].minor.yy146 = yylhsminor.yy146;
        break;
      case 93: /* function_call_args ::= */
#line 664 "parserimpl.lemon"
{
    yymsp[1].minor.yy146= nullptr;
}
#line 2199 "parserimpl.c"
        break;
      case 94: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
#line 671 "parserimpl.lemon"
{
    if (yymsp[-1].minor.yy188) {
        yymsp[-1].minor.yy188->updateSourceInfo();
    }

    yylhsminor.yy95 = pParser->newAstNode<yal::ExprStructInit>(yymsp[-3].minor.yy64, yymsp[-1].minor.yy188);
    auto srcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy64->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy95->setSourceInfo(srcInfo);
}
#line 2212 "parserimpl.c"
  yymsp[-3].minor.yy95 = yylhsminor.yy95;
        break;
      case 95: /* struct_member_init_list ::= */
#line 681 "parserimpl.lemon"
{yymsp[1].minor.yy188 = nullptr;}
#line 2218 "parserimpl.c"
        break;
      case 96: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
#line 682 "parserimpl.lemon"
{
     yymsp[-2].minor.yy188->addStructMemberInit(yymsp[0].minor.yy14);
     yylhsminor.yy188 = yymsp[-2].minor.yy188;
}
#line 2226 "parserimpl.c"
  yymsp[-2].minor.yy188 = yylhsminor.yy188;
        break;
      case 97: /* struct_member_init_list ::= struct_member_init */
#line 686 "parserimpl.lemon"
{
        yylhsminor.yy188 = pParser->newAstNode<yal::StructMemberInitList>();
        yylhsminor.yy188->addStructMemberInit(yymsp[0].minor.yy14);
}
#line 2235 "parserimpl.c"
  yymsp[0].minor.yy188 = yylhsminor.yy188;
        break;
      case 98: /* struct_member_init ::= IDENTIFIER COLON expression */
#line 691 "parserimpl.lemon"
{
    yylhsminor.yy14 = pParser->newAstNode<yal::StructMemberInit>(yymsp[-2].minor.yy0.tokenStr,yymsp[0].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
    yylhsminor.yy14->setSourceInfo(srcInfo);
}
#line 2245 "parserimpl.c"
  yymsp[-2].minor.yy14 = yylhsminor.yy14;
        break;
      case 99: /* literal ::= INTEGER_LITERAL */
#line 699 "parserimpl.lemon"
{
        yylhsminor.yy49 = pParser->newIntegerLiteral(yymsp[0].minor.yy0);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy49->setSourceInfo(srcInfo);
   }
#line 2255 "parserimpl.c"
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 100: /* literal ::= FLOAT_LITERAL */
#line 704 "parserimpl.lemon"
{
        yylhsminor.yy49 = pParser->newFloatLiteral(yymsp[0].minor.yy0);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy49->setSourceInfo(srcInfo);
}
#line 2265 "parserimpl.c"
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 101: /* literal ::= BOOL_LITERAL */
#line 709 "parserimpl.lemon"
{
        yylhsminor.yy49 = pParser->newAstNode<yal::ExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy49->setSourceInfo(srcInfo);
}
#line 2275 "parserimpl.c"
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      default:
      /* (102) module ::= decls END */ yytestcase(yyruleno==102);
      /* (103) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==103);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[yysize].stateno,(YYCODETYPE)yygoto);

  /* There are no SHIFTREDUCE actions on nonterminals because the table
  ** generator has simplified them to pure REDUCE actions. */
  assert( !(yyact>YY_MAX_SHIFT && yyact<=YY_MAX_SHIFTREDUCE) );

  /* It is not possible for a REDUCE to be followed by an error */
  assert( yyact!=YY_ERROR_ACTION );

  if( yyact==YY_ACCEPT_ACTION ){
    yypParser->yytos += yysize;
    yy_accept(yypParser);
  }else{
    yymsp += yysize+1;
    yypParser->yytos = yymsp;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  YALParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  YALParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YALParserTOKENTYPE yyminor         /* The minor type of the error token */
){
  YALParserARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 40 "parserimpl.lemon"


    pParser->logParseFailure();
    {
#if defined(YAL_DEBUG)
        yal::Log::MultiLineScope multiLineScope(pParser->getLog());
        pParser->getLog().error("Expected one of the following tokens:\n");
        int n = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
        for (int i = 0; i < n; ++i) {
            int a = yy_find_shift_action(yypParser, (YYCODETYPE)i);
            if (a < YYNSTATE + YYNRULE) {
                pParser->getLog().error("%\n",yyTokenName[i]);
            }
        }
#endif
    }
#line 2358 "parserimpl.c"
/************ End %syntax_error code ******************************************/
  YALParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  YALParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  YALParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "YALParserAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void YALParser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  YALParserTOKENTYPE yyminor       /* The value for the token */
  YALParserARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  YALParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
