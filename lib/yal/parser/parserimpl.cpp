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

#include <cassert>
#include <vector>
#include "yal/parser/parser.h"
#include "yal/parser/parserhelper.h"
#include "yal/util/log.h"
#include "yal/util/stringref.h"
#include "yal/ast/module.h"
#include "yal/ast/typebuiltin.h"
#include "yal/ast/astnodes.h"
#include "yal/lexer/lexer.h"
#define YYMALLOCARGTYPE size_t
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
#define YAL_TOKEN_FUNCTION                       45
#define YAL_TOKEN_PAR_BEGIN                      46
#define YAL_TOKEN_PAR_END                        47
#define YAL_TOKEN_SELF                           48
#define YAL_TOKEN_COMMA                          49
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
#define YYNSTATE             130
#define YYNRULE              105
#define YY_MAX_SHIFT         129
#define YY_MIN_SHIFTREDUCE   195
#define YY_MAX_SHIFTREDUCE   299
#define YY_MIN_REDUCE        300
#define YY_MAX_REDUCE        404
#define YY_ERROR_ACTION      405
#define YY_ACCEPT_ACTION     406
#define YY_NO_ACTION         407
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
#define YY_ACTTAB_COUNT (1028)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    14,  104,  121,  204,  327,   15,   16,   88,   92,   78,
 /*    10 */   345,   13,  207,  208,  209,  210,  211,  212,  213,  214,
 /*    20 */   215,  216,  217,   26,   25,   24,  307,  306,  129,  120,
 /*    30 */   306,   91,   32,    6,  266,  104,    8,   96,   94,  118,
 /*    40 */   295,  296,  297,    7,  406,  128,  342,   14,  121,  204,
 /*    50 */    34,   36,   15,   16,   37,  361,   78,  122,   13,  207,
 /*    60 */   208,  209,  210,  211,  212,  213,  214,  215,  216,  217,
 /*    70 */    37,  347,   37,  348,  253,   37,  117,   79,  221,   32,
 /*    80 */   345,  266,   37,   85,  126,  126,  118,  295,  296,  297,
 /*    90 */   124,  102,   89,  343,   14,   82,  398,  328,   47,   15,
 /*   100 */    16,   33,  228,   78,   53,   13,  207,  208,  209,  210,
 /*   110 */   211,  212,  213,  214,  215,  216,  217,  267,  126,   17,
 /*   120 */   105,  122,  305,  305,  311,  268,   32,   17,  266,  269,
 /*   130 */    40,   17,  335,  118,  295,  296,  297,   11,   29,   30,
 /*   140 */    49,  225,   38,   23,   22,   18,   20,   19,   21,   28,
 /*   150 */    27,   26,   25,   24,   29,   30,  305,  120,   93,   23,
 /*   160 */    22,   18,   20,   19,   21,   28,   27,   26,   25,   24,
 /*   170 */   290,   95,  331,  120,  344,   56,  201,  251,   44,  207,
 /*   180 */   208,  209,  210,  211,  212,  213,  214,  215,  216,  217,
 /*   190 */    54,  112,  233,  252,   45,  220,   29,   30,  397,    1,
 /*   200 */     9,   23,   22,   18,   20,   19,   21,   28,   27,   26,
 /*   210 */    25,   24,   29,   30,   53,  120,   48,   23,   22,   18,
 /*   220 */    20,   19,   21,   28,   27,   26,   25,   24,   43,   90,
 /*   230 */   232,  120,   51,   10,  103,  249,   52,  207,  208,  209,
 /*   240 */   210,  211,  212,  213,  214,  215,  216,  217,  250,  229,
 /*   250 */   231,  224,  100,  110,   29,   30,  101,    2,   39,   23,
 /*   260 */    22,   18,   20,   19,   21,   28,   27,   26,   25,   24,
 /*   270 */    29,   30,  332,  120,   12,   23,   22,   18,   20,   19,
 /*   280 */    21,   28,   27,   26,   25,   24,  120,    4,  111,  120,
 /*   290 */    31,  116,    5,   41,  119,  205,   42,  201,   46,  263,
 /*   300 */   207,  208,  209,  210,  211,  212,  213,  214,  215,  216,
 /*   310 */   217,  299,  125,  218,  127,  258,   35,  403,   29,   30,
 /*   320 */   302,  302,  302,   23,   22,   18,   20,   19,   21,   28,
 /*   330 */    27,   26,   25,   24,   30,  302,  302,  120,   23,   22,
 /*   340 */    18,   20,   19,   21,   28,   27,   26,   25,   24,  302,
 /*   350 */   302,  302,  120,  302,  302,  302,  302,  302,  126,  302,
 /*   360 */   302,  302,  302,  302,   84,  126,  302,  302,  302,  302,
 /*   370 */   302,   84,  302,  302,  302,  302,  302,    3,   86,  353,
 /*   380 */    57,  365,  366,  364,   97,  126,  302,   67,  365,  366,
 /*   390 */   364,   84,  302,  367,  302,  302,  305,   83,  302,  302,
 /*   400 */   367,  302,  302,  305,    3,   98,  353,   57,  365,  366,
 /*   410 */   364,   97,  302,  302,  302,  302,  302,  302,  302,  302,
 /*   420 */   367,  302,  302,  305,  302,  302,  302,  302,  405,  405,
 /*   430 */   405,  405,  405,  405,   28,   27,   26,   25,   24,  302,
 /*   440 */   302,  302,  120,   23,   22,   18,   20,   19,   21,   28,
 /*   450 */    27,   26,   25,   24,  302,  126,  302,  120,  302,  302,
 /*   460 */   302,   84,  126,  302,  302,  302,  302,  302,   84,  302,
 /*   470 */   302,  302,  302,  302,  302,  302,  352,   57,  365,  366,
 /*   480 */   364,   97,  126,  302,   67,  365,  366,  364,   84,  302,
 /*   490 */   367,  302,  302,  305,   81,  302,  302,  367,  302,  302,
 /*   500 */   305,  302,  302,  302,   67,  365,  366,  364,  302,  301,
 /*   510 */   302,  302,  302,  302,   80,  302,  302,  367,  302,  302,
 /*   520 */   305,   99,   55,  302,   87,   50,  302,  126,  302,  302,
 /*   530 */   302,  302,  302,   84,  302,  300,  324,  123,  302,  302,
 /*   540 */   302,  302,  302,  302,  302,  302,  302,  126,  302,   58,
 /*   550 */   365,  366,  364,   84,  302,  302,  302,  302,  302,  302,
 /*   560 */   302,  302,  367,  302,  302,  305,  302,  302,  126,   63,
 /*   570 */   365,  366,  364,  126,   84,  302,  302,  302,  302,   84,
 /*   580 */   302,  302,  367,  302,  302,  305,  302,  302,  302,  302,
 /*   590 */    64,  365,  366,  364,  126,   59,  365,  366,  364,  302,
 /*   600 */    84,  302,  302,  367,  302,  302,  305,  302,  367,  302,
 /*   610 */   302,  305,  302,  302,  302,  302,   65,  365,  366,  364,
 /*   620 */   302,  126,  302,  302,  302,  302,  302,   84,  126,  367,
 /*   630 */   302,  302,  305,  302,   84,  302,  302,  302,  302,  302,
 /*   640 */   302,  302,  302,  106,  365,  366,  364,  302,  302,  126,
 /*   650 */   107,  365,  366,  364,  126,   84,  367,  302,  302,  305,
 /*   660 */    84,  302,  302,  367,  302,  302,  305,  302,  302,  302,
 /*   670 */   302,  108,  365,  366,  364,  126,  109,  365,  366,  364,
 /*   680 */   126,   84,  302,  302,  367,  302,   84,  305,  302,  367,
 /*   690 */   302,  302,  305,  302,  302,  302,  302,   66,  365,  366,
 /*   700 */   364,  302,   69,  365,  366,  364,  302,  302,  126,  302,
 /*   710 */   367,  302,  302,  305,   84,  367,  302,  302,  305,  302,
 /*   720 */   302,  302,  302,  302,  302,  302,  302,  302,  302,  302,
 /*   730 */    70,  365,  366,  364,  126,  302,  302,  302,  126,  302,
 /*   740 */    84,  302,  302,  367,   84,  302,  305,  302,  302,  302,
 /*   750 */   302,  302,  302,  302,  302,  302,   71,  365,  366,  364,
 /*   760 */    72,  365,  366,  364,  126,  302,  302,  302,  302,  367,
 /*   770 */    84,  302,  305,  367,  302,  302,  305,  302,  302,  302,
 /*   780 */   302,  302,  302,  302,  302,  302,   73,  365,  366,  364,
 /*   790 */   126,  302,  302,  302,  302,  302,   84,  126,  302,  367,
 /*   800 */   302,  302,  305,   84,  302,  302,  302,  302,  302,  302,
 /*   810 */   302,  302,   74,  365,  366,  364,  302,  126,  302,  113,
 /*   820 */   365,  366,  364,   84,  302,  367,  302,  302,  305,  302,
 /*   830 */   302,  302,  367,  302,  302,  305,  302,  302,  302,  114,
 /*   840 */   365,  366,  364,  302,  302,  126,  302,  302,  302,  302,
 /*   850 */   302,   84,  367,  302,  302,  305,  302,  302,  302,  302,
 /*   860 */   302,  302,  302,  302,  302,  302,  302,  115,  365,  366,
 /*   870 */   364,  126,  302,  302,  302,  302,  126,   84,  302,  302,
 /*   880 */   367,  302,   84,  305,  302,  302,  302,  302,  302,  302,
 /*   890 */   302,  302,  302,   76,  365,  366,  364,  302,   77,  365,
 /*   900 */   366,  364,  302,  302,  126,  302,  367,  302,  302,  305,
 /*   910 */    84,  367,  302,  302,  305,  302,  302,  302,  302,  302,
 /*   920 */   302,  302,  302,  302,  302,  302,   68,  365,  366,  364,
 /*   930 */   126,  302,  302,  302,  302,  126,   84,  302,  302,  367,
 /*   940 */   302,   84,  305,  302,  302,  302,  302,  302,  302,  302,
 /*   950 */   302,  302,   75,  365,  366,  364,  302,   61,  365,  366,
 /*   960 */   364,  302,  302,  126,  302,  367,  302,  302,  305,   84,
 /*   970 */   367,  302,  302,  305,  302,  302,  302,  302,  302,  302,
 /*   980 */   302,  302,  302,  302,  302,   62,  365,  366,  364,  126,
 /*   990 */   302,  302,  302,  302,  302,   84,  302,  302,  367,  302,
 /*  1000 */   302,  305,  302,  302,  302,  302,  302,  302,  302,  302,
 /*  1010 */   302,   60,  365,  366,  364,  302,  302,  302,  302,  302,
 /*  1020 */   302,  302,  302,  302,  367,  302,  302,  305,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    14,   23,   24,   25,   88,   19,   20,   67,   68,   23,
 /*    10 */    70,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*    20 */    34,   35,   36,   15,   16,   17,   48,   40,   22,   21,
 /*    30 */    43,   91,   46,   46,   48,   23,   50,   51,   52,   53,
 /*    40 */    54,   55,   56,   61,   62,   39,   70,   14,   24,   25,
 /*    50 */    46,   45,   19,   20,   91,   92,   23,   23,   25,   26,
 /*    60 */    27,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*    70 */    91,   92,   91,   92,   41,   91,   92,   67,   44,   46,
 /*    80 */    70,   48,   91,   92,   58,   58,   53,   54,   55,   56,
 /*    90 */    64,   64,   69,   70,   14,   94,   95,   88,   89,   19,
 /*   100 */    20,    1,   47,   23,   49,   25,   26,   27,   28,   29,
 /*   110 */    30,   31,   32,   33,   34,   35,   36,   47,   58,   49,
 /*   120 */    23,   23,   96,   96,   64,   47,   46,   49,   48,   47,
 /*   130 */    40,   49,   63,   53,   54,   55,   56,    1,    2,    3,
 /*   140 */    76,   41,   40,    7,    8,    9,   10,   11,   12,   13,
 /*   150 */    14,   15,   16,   17,    2,    3,   96,   21,   65,    7,
 /*   160 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   170 */    44,   65,   63,   21,   70,   49,   23,   41,   46,   26,
 /*   180 */    27,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*   190 */    73,   40,   44,   41,   43,   42,    2,    3,   95,   43,
 /*   200 */     1,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   210 */    16,   17,    2,    3,   49,   21,   49,    7,    8,    9,
 /*   220 */    10,   11,   12,   13,   14,   15,   16,   17,   49,   48,
 /*   230 */    47,   21,   23,    1,   23,   41,   23,   26,   27,   28,
 /*   240 */    29,   30,   31,   32,   33,   34,   35,   36,   41,   44,
 /*   250 */    23,   41,   40,   23,    2,    3,   40,   43,   40,    7,
 /*   260 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   270 */     2,    3,   46,   21,   40,    7,    8,    9,   10,   11,
 /*   280 */    12,   13,   14,   15,   16,   17,   21,   46,   40,   21,
 /*   290 */    46,    9,   46,   10,   23,   25,   40,   23,   43,   47,
 /*   300 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   310 */    36,   38,   37,   41,   23,   47,   40,    0,    2,    3,
 /*   320 */    97,   97,   97,    7,    8,    9,   10,   11,   12,   13,
 /*   330 */    14,   15,   16,   17,    3,   97,   97,   21,    7,    8,
 /*   340 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   97,
 /*   350 */    97,   97,   21,   97,   97,   97,   97,   97,   58,   97,
 /*   360 */    97,   97,   97,   97,   64,   58,   97,   97,   97,   97,
 /*   370 */    97,   64,   97,   97,   97,   97,   97,   77,   78,   79,
 /*   380 */    80,   81,   82,   83,   84,   58,   97,   80,   81,   82,
 /*   390 */    83,   64,   97,   93,   97,   97,   96,   90,   97,   97,
 /*   400 */    93,   97,   97,   96,   77,   78,   79,   80,   81,   82,
 /*   410 */    83,   84,   97,   97,   97,   97,   97,   97,   97,   97,
 /*   420 */    93,   97,   97,   96,   97,   97,   97,   97,    7,    8,
 /*   430 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   97,
 /*   440 */    97,   97,   21,    7,    8,    9,   10,   11,   12,   13,
 /*   450 */    14,   15,   16,   17,   97,   58,   97,   21,   97,   97,
 /*   460 */    97,   64,   58,   97,   97,   97,   97,   97,   64,   97,
 /*   470 */    97,   97,   97,   97,   97,   97,   79,   80,   81,   82,
 /*   480 */    83,   84,   58,   97,   80,   81,   82,   83,   64,   97,
 /*   490 */    93,   97,   97,   96,   90,   97,   97,   93,   97,   97,
 /*   500 */    96,   97,   97,   97,   80,   81,   82,   83,   97,   59,
 /*   510 */    60,   97,   97,   97,   90,   97,   97,   93,   97,   97,
 /*   520 */    96,   71,   72,   97,   74,   75,   97,   58,   97,   97,
 /*   530 */    97,   97,   97,   64,   97,   85,   86,   87,   97,   97,
 /*   540 */    97,   97,   97,   97,   97,   97,   97,   58,   97,   80,
 /*   550 */    81,   82,   83,   64,   97,   97,   97,   97,   97,   97,
 /*   560 */    97,   97,   93,   97,   97,   96,   97,   97,   58,   80,
 /*   570 */    81,   82,   83,   58,   64,   97,   97,   97,   97,   64,
 /*   580 */    97,   97,   93,   97,   97,   96,   97,   97,   97,   97,
 /*   590 */    80,   81,   82,   83,   58,   80,   81,   82,   83,   97,
 /*   600 */    64,   97,   97,   93,   97,   97,   96,   97,   93,   97,
 /*   610 */    97,   96,   97,   97,   97,   97,   80,   81,   82,   83,
 /*   620 */    97,   58,   97,   97,   97,   97,   97,   64,   58,   93,
 /*   630 */    97,   97,   96,   97,   64,   97,   97,   97,   97,   97,
 /*   640 */    97,   97,   97,   80,   81,   82,   83,   97,   97,   58,
 /*   650 */    80,   81,   82,   83,   58,   64,   93,   97,   97,   96,
 /*   660 */    64,   97,   97,   93,   97,   97,   96,   97,   97,   97,
 /*   670 */    97,   80,   81,   82,   83,   58,   80,   81,   82,   83,
 /*   680 */    58,   64,   97,   97,   93,   97,   64,   96,   97,   93,
 /*   690 */    97,   97,   96,   97,   97,   97,   97,   80,   81,   82,
 /*   700 */    83,   97,   80,   81,   82,   83,   97,   97,   58,   97,
 /*   710 */    93,   97,   97,   96,   64,   93,   97,   97,   96,   97,
 /*   720 */    97,   97,   97,   97,   97,   97,   97,   97,   97,   97,
 /*   730 */    80,   81,   82,   83,   58,   97,   97,   97,   58,   97,
 /*   740 */    64,   97,   97,   93,   64,   97,   96,   97,   97,   97,
 /*   750 */    97,   97,   97,   97,   97,   97,   80,   81,   82,   83,
 /*   760 */    80,   81,   82,   83,   58,   97,   97,   97,   97,   93,
 /*   770 */    64,   97,   96,   93,   97,   97,   96,   97,   97,   97,
 /*   780 */    97,   97,   97,   97,   97,   97,   80,   81,   82,   83,
 /*   790 */    58,   97,   97,   97,   97,   97,   64,   58,   97,   93,
 /*   800 */    97,   97,   96,   64,   97,   97,   97,   97,   97,   97,
 /*   810 */    97,   97,   80,   81,   82,   83,   97,   58,   97,   80,
 /*   820 */    81,   82,   83,   64,   97,   93,   97,   97,   96,   97,
 /*   830 */    97,   97,   93,   97,   97,   96,   97,   97,   97,   80,
 /*   840 */    81,   82,   83,   97,   97,   58,   97,   97,   97,   97,
 /*   850 */    97,   64,   93,   97,   97,   96,   97,   97,   97,   97,
 /*   860 */    97,   97,   97,   97,   97,   97,   97,   80,   81,   82,
 /*   870 */    83,   58,   97,   97,   97,   97,   58,   64,   97,   97,
 /*   880 */    93,   97,   64,   96,   97,   97,   97,   97,   97,   97,
 /*   890 */    97,   97,   97,   80,   81,   82,   83,   97,   80,   81,
 /*   900 */    82,   83,   97,   97,   58,   97,   93,   97,   97,   96,
 /*   910 */    64,   93,   97,   97,   96,   97,   97,   97,   97,   97,
 /*   920 */    97,   97,   97,   97,   97,   97,   80,   81,   82,   83,
 /*   930 */    58,   97,   97,   97,   97,   58,   64,   97,   97,   93,
 /*   940 */    97,   64,   96,   97,   97,   97,   97,   97,   97,   97,
 /*   950 */    97,   97,   80,   81,   82,   83,   97,   80,   81,   82,
 /*   960 */    83,   97,   97,   58,   97,   93,   97,   97,   96,   64,
 /*   970 */    93,   97,   97,   96,   97,   97,   97,   97,   97,   97,
 /*   980 */    97,   97,   97,   97,   97,   80,   81,   82,   83,   58,
 /*   990 */    97,   97,   97,   97,   97,   64,   97,   97,   93,   97,
 /*  1000 */    97,   96,   97,   97,   97,   97,   97,   97,   97,   97,
 /*  1010 */    97,   80,   81,   82,   83,   97,   97,   97,   97,   97,
 /*  1020 */    97,   97,   97,   97,   93,   97,   97,   96,
};
#define YY_SHIFT_USE_DFLT (1028)
#define YY_SHIFT_COUNT    (129)
#define YY_SHIFT_MIN      (-22)
#define YY_SHIFT_MAX      (436)
static const short yy_shift_ofst[] = {
 /*     0 */  1028,  -14,  -14,  -14,   80,   80,   80,    6,   33,   80,
 /*    10 */    80,   80,   80,   80,   80,   80,   80,   80,   80,   80,
 /*    20 */    80,   80,   80,   80,   80,   80,   80,   80,   80,   80,
 /*    30 */    80,   80,   80,   80,  -22,  153,  211,  274,   24,   24,
 /*    40 */    24,   24,   24,   12,   12,   97,   98,   34,   12,   90,
 /*    50 */     4,  102,  102,   12,   90,  132,   97,  136,  152,  194,
 /*    60 */   210,  252,  268,  316,  316,  316,  316,  316,  331,  421,
 /*    70 */   421,  421,  421,  421,  421,  436,    8,    8,  -13,   55,
 /*    80 */    70,   78,  126,   82,  151,  100,  148,  156,  165,  167,
 /*    90 */   179,  181,  183,  199,  209,  232,  213,  207,  205,  214,
 /*   100 */   227,  212,  216,  226,  218,  234,  265,  265,  265,  265,
 /*   110 */   241,  230,  248,  265,  265,  265,  244,  282,  283,  246,
 /*   120 */   271,  270,  256,  255,  272,  273,  275,  276,  291,  317,
};
#define YY_REDUCE_USE_DFLT (-85)
#define YY_REDUCE_COUNT (56)
#define YY_REDUCE_MIN   (-84)
#define YY_REDUCE_MAX   (931)
static const short yy_reduce_ofst[] = {
 /*     0 */   -18,  300,  327,  397,  307,  404,  424,  450,  469,  489,
 /*    10 */   510,  515,  536,  563,  570,  591,  596,  617,  622,  650,
 /*    20 */   676,  680,  706,  732,  739,  759,  787,  813,  818,  846,
 /*    30 */   872,  877,  905,  931,  -60,   26,   27,   60,  -37,  -21,
 /*    40 */   -19,  -16,   -9,   23,   10,    1,    9,  -84,  -24,   69,
 /*    50 */    64,   93,  106,  104,  109,  117,  103,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   303,  350,  350,  351,  394,  394,  394,  405,  405,  405,
 /*    10 */   405,  405,  405,  405,  405,  405,  405,  405,  405,  405,
 /*    20 */   405,  405,  405,  405,  405,  405,  405,  405,  405,  405,
 /*    30 */   405,  405,  405,  405,  346,  405,  405,  405,  307,  307,
 /*    40 */   307,  307,  307,  405,  346,  396,  405,  405,  405,  349,
 /*    50 */   405,  362,  362,  405,  349,  405,  405,  405,  405,  405,
 /*    60 */   405,  405,  405,  360,  359,  399,  392,  393,  376,  387,
 /*    70 */   386,  385,  384,  383,  382,  375,  378,  377,  369,  405,
 /*    80 */   405,  405,  405,  405,  405,  405,  405,  405,  341,  340,
 /*    90 */   339,  405,  405,  405,  405,  405,  405,  405,  405,  405,
 /*   100 */   405,  405,  405,  306,  405,  405,  391,  390,  389,  388,
 /*   110 */   405,  405,  405,  381,  380,  379,  405,  405,  405,  370,
 /*   120 */   405,  308,  405,  405,  405,  405,  304,  405,  405,  405,
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
  "SCOPE_END",     "FUNCTION",      "PAR_BEGIN",     "PAR_END",     
  "SELF",          "COMMA",         "RETURN",        "VAR",         
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
 /*  27 */ "struct_decl_vars ::= struct_decl_vars struct_decl_var",
 /*  28 */ "struct_decl_vars ::= struct_decl_var",
 /*  29 */ "struct_decl_var ::= IDENTIFIER COLON qualified_type ASSIGN expression SEMI_COLON",
 /*  30 */ "struct_decl_var ::= IDENTIFIER COLON qualified_type SEMI_COLON",
 /*  31 */ "function_header ::= function_start function_param_list function_return_decl",
 /*  32 */ "function_start ::= FUNCTION IDENTIFIER",
 /*  33 */ "function_param_list ::= PAR_BEGIN function_args_decl PAR_END",
 /*  34 */ "decl_function ::= function_header SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  35 */ "type_function_header ::= type_function_start type_function_param_list function_return_decl",
 /*  36 */ "type_function_start ::= FUNCTION type_specifier COLON COLON IDENTIFIER",
 /*  37 */ "type_function_param_list ::= PAR_BEGIN type_function_args_decl PAR_END",
 /*  38 */ "type_function_decl ::= type_function_header SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  39 */ "type_function_args_decl ::= qualifier SELF",
 /*  40 */ "type_function_args_decl ::= qualifier SELF COMMA type_function_args_decl_other",
 /*  41 */ "type_function_args_decl ::= function_args_decl",
 /*  42 */ "type_function_args_decl_other ::= type_function_args_decl_other COMMA function_arg_decl",
 /*  43 */ "type_function_args_decl_other ::= function_arg_decl",
 /*  44 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  45 */ "function_args_decl ::= function_arg_decl",
 /*  46 */ "function_args_decl ::=",
 /*  47 */ "function_arg_decl ::= IDENTIFIER COLON qualified_type",
 /*  48 */ "function_return_decl ::= COLON qualified_type",
 /*  49 */ "function_return_decl ::=",
 /*  50 */ "statement_list_or_empty ::=",
 /*  51 */ "statement_list_or_empty ::= statement_list",
 /*  52 */ "statement_list ::= statement_list statement",
 /*  53 */ "statement_list ::= statement",
 /*  54 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  55 */ "statement ::= decl_var SEMI_COLON",
 /*  56 */ "statement ::= expression SEMI_COLON",
 /*  57 */ "statement ::= RETURN expression SEMI_COLON",
 /*  58 */ "statement ::= RETURN SEMI_COLON",
 /*  59 */ "decl_var ::= VAR IDENTIFIER var_type_spec ASSIGN expression",
 /*  60 */ "decl_var ::= LET IDENTIFIER var_type_spec ASSIGN expression",
 /*  61 */ "var_type_spec ::= COLON qualified_type",
 /*  62 */ "var_type_spec ::=",
 /*  63 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  64 */ "expression ::= literal",
 /*  65 */ "expression ::= unaryexp",
 /*  66 */ "expression ::= binaryexp",
 /*  67 */ "expression ::= struct_init",
 /*  68 */ "expression ::= RANGE_CAST LT qualified_type GT PAR_BEGIN expression PAR_END",
 /*  69 */ "expression ::= IDENTIFIER",
 /*  70 */ "expression ::= expression DOT IDENTIFIER",
 /*  71 */ "expression ::= SELF",
 /*  72 */ "expression ::= IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  73 */ "expression ::= expression DOT IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  74 */ "expression ::= type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  75 */ "binaryexp ::= expression AND expression",
 /*  76 */ "binaryexp ::= expression OR expression",
 /*  77 */ "binaryexp ::= expression PLUS expression",
 /*  78 */ "binaryexp ::= expression MINUS expression",
 /*  79 */ "binaryexp ::= expression MULT expression",
 /*  80 */ "binaryexp ::= expression DIV expression",
 /*  81 */ "binaryexp ::= expression MOD expression",
 /*  82 */ "binaryexp ::= expression EQ expression",
 /*  83 */ "binaryexp ::= expression NE expression",
 /*  84 */ "binaryexp ::= expression LE expression",
 /*  85 */ "binaryexp ::= expression LT expression",
 /*  86 */ "binaryexp ::= expression GE expression",
 /*  87 */ "binaryexp ::= expression GT expression",
 /*  88 */ "unaryexp ::= NOT expression",
 /*  89 */ "unaryexp ::= BIT_NOT expression",
 /*  90 */ "unaryexp ::= MINUS expression",
 /*  91 */ "unaryexp ::= REFERENCE expression",
 /*  92 */ "function_call_args ::= function_call_args COMMA expression",
 /*  93 */ "function_call_args ::= expression",
 /*  94 */ "function_call_args ::=",
 /*  95 */ "struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END",
 /*  96 */ "struct_member_init_list ::=",
 /*  97 */ "struct_member_init_list ::= struct_member_init_list COMMA struct_member_init",
 /*  98 */ "struct_member_init_list ::= struct_member_init",
 /*  99 */ "struct_member_init ::= IDENTIFIER COLON expression",
 /* 100 */ "literal ::= INTEGER_LITERAL",
 /* 101 */ "literal ::= FLOAT_LITERAL",
 /* 102 */ "literal ::= BOOL_LITERAL",
 /* 103 */ "module ::= decls END",
 /* 104 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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
  { 89, -2 },
  { 89, -1 },
  { 88, -6 },
  { 88, -4 },
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
{
    yylhsminor.yy175 =yymsp[-1].minor.yy175; yymsp[-1].minor.yy175->addDecl(yymsp[0].minor.yy60);
}
  yymsp[-1].minor.yy175 = yylhsminor.yy175;
        break;
      case 1: /* decls ::= decls decl_function */
{
        yylhsminor.yy175 =yymsp[-1].minor.yy175; yymsp[-1].minor.yy175->addDecl(yymsp[0].minor.yy161);
}
  yymsp[-1].minor.yy175 = yylhsminor.yy175;
        break;
      case 2: /* decls ::= decls type_function_decl */
{
        yylhsminor.yy175 =yymsp[-1].minor.yy175; yymsp[-1].minor.yy175->addDecl(yymsp[0].minor.yy129);
}
  yymsp[-1].minor.yy175 = yylhsminor.yy175;
        break;
      case 3: /* decls ::= */
{
        yymsp[1].minor.yy175 =  pParser->getModule().getDeclNode();
     }
        break;
      case 4: /* type_specifier ::= type_builtin */
{
        yylhsminor.yy64=yymsp[0].minor.yy64;
}
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 5: /* type_specifier ::= type_array */
{yymsp[0].minor.yy64 = nullptr;}
        break;
      case 6: /* type_specifier ::= IDENTIFIER */
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
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 7: /* qualifier ::= */
{yymsp[1].minor.yy174 = 0;}
        break;
      case 8: /* qualifier ::= MUT */
{yymsp[0].minor.yy174 = yal::parser::kQualifierMutable;}
        break;
      case 9: /* qualifier ::= REFERENCE */
{yymsp[0].minor.yy174 = yal::parser::kQualifierReference;}
        break;
      case 10: /* qualifier ::= MUT REFERENCE */
{
        yymsp[-1].minor.yy174 = yal::parser::kQualifierReference | yal::parser::kQualifierMutable;
 }
        break;
      case 11: /* qualified_type ::= qualifier type_specifier */
{
    if(yymsp[0].minor.yy64) {
        yymsp[0].minor.yy64->setQualifier(yal::parser::MakeQualifierFromFlags(yymsp[-1].minor.yy174));
    }
    yylhsminor.yy64 = yymsp[0].minor.yy64;
}
  yymsp[-1].minor.yy64 = yylhsminor.yy64;
        break;
      case 12: /* type_builtin ::= TYPE_BOOL */
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinBool());
    auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
    yylhsminor.yy64->setSourceInfo(srcInfo);
    }
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 13: /* type_builtin ::= TYPE_INT8 */
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI8());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 14: /* type_builtin ::= TYPE_UINT8 */
{
    yylhsminor.yy64  = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU8());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 15: /* type_builtin ::= TYPE_INT16 */
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI16());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 16: /* type_builtin ::= TYPE_UINT16 */
{
    yylhsminor.yy64  = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU16());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 17: /* type_builtin ::= TYPE_INT32 */
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI32());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 18: /* type_builtin ::= TYPE_UINT32 */
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU32());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 19: /* type_builtin ::= TYPE_INT64 */
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI64());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 20: /* type_builtin ::= TYPE_UINT64 */
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU64());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 21: /* type_builtin ::= TYPE_FLOAT */
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinFloat32());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 22: /* type_builtin ::= TYPE_DOUBLE */
{
    yylhsminor.yy64 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinFloat64());
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy64->setSourceInfo(srcInfo);
    }
  yymsp[0].minor.yy64 = yylhsminor.yy64;
        break;
      case 23: /* decl_type ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON */
{
    yymsp[-4].minor.yy60 = nullptr;
}
        break;
      case 24: /* decl_type ::= decl_struct */
{ yylhsminor.yy60 = yymsp[0].minor.yy152;}
  yymsp[0].minor.yy60 = yylhsminor.yy60;
        break;
      case 25: /* struct_header ::= TYPE IDENTIFIER COLON STRUCT */
{
    yylhsminor.yy152 = pParser->newAstNode<yal::DeclStruct>(yymsp[-2].minor.yy0.tokenStr, pParser->getActiveScope());
    yylhsminor.yy152->setSourceInfo(pParser->createSourceInfo(yymsp[-3].minor.yy0, yymsp[-2].minor.yy0));
    pParser->onDeclBegin(yylhsminor.yy152);
}
  yymsp[-3].minor.yy152 = yylhsminor.yy152;
        break;
      case 26: /* decl_struct ::= struct_header SCOPE_BEGIN struct_decl_vars SCOPE_END */
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
  yymsp[-3].minor.yy152 = yylhsminor.yy152;
        break;
      case 27: /* struct_decl_vars ::= struct_decl_vars struct_decl_var */
{
    yylhsminor.yy193 = yymsp[-1].minor.yy193;
    if (pParser->onDecl(yymsp[0].minor.yy44)) {
        yylhsminor.yy193->addDeclVar(yymsp[0].minor.yy44);
    }
}
  yymsp[-1].minor.yy193 = yylhsminor.yy193;
        break;
      case 28: /* struct_decl_vars ::= struct_decl_var */
{
    yylhsminor.yy193 = pParser->newAstNode<yal::DeclStructMembers>();
    if (pParser->onDecl(yymsp[0].minor.yy44)) {
        yylhsminor.yy193->addDeclVar(yymsp[0].minor.yy44);
    }
}
  yymsp[0].minor.yy193 = yylhsminor.yy193;
        break;
      case 29: /* struct_decl_var ::= IDENTIFIER COLON qualified_type ASSIGN expression SEMI_COLON */
{
    yylhsminor.yy44 = pParser->newAstNode<yal::DeclVar>(yymsp[-5].minor.yy0.tokenStr,
                                          pParser->getActiveScope(),
                                          yal::Qualifier(), yymsp[-3].minor.yy64, yymsp[-1].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-5].minor.yy0, yymsp[0].minor.yy0);
    yylhsminor.yy44->setSourceInfo(srcInfo);
}
  yymsp[-5].minor.yy44 = yylhsminor.yy44;
        break;
      case 30: /* struct_decl_var ::= IDENTIFIER COLON qualified_type SEMI_COLON */
{
    yylhsminor.yy44 = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr,
                                          pParser->getActiveScope(),
                                          yal::Qualifier(), yymsp[-1].minor.yy64, nullptr);
    auto srcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy0, yymsp[0].minor.yy0);
    yylhsminor.yy44->setSourceInfo(srcInfo);
}
  yymsp[-3].minor.yy44 = yylhsminor.yy44;
        break;
      case 31: /* function_header ::= function_start function_param_list function_return_decl */
{
    yylhsminor.yy161 = yymsp[-2].minor.yy161;
    yylhsminor.yy161->setParams(yymsp[-1].minor.yy120);
    yylhsminor.yy161->setReturnType(yymsp[0].minor.yy64);

}
  yymsp[-2].minor.yy161 = yylhsminor.yy161;
        break;
      case 32: /* function_start ::= FUNCTION IDENTIFIER */
{
     yylhsminor.yy161 = pParser->newAstNode<yal::DeclFunction>(yymsp[0].minor.yy0.tokenStr, pParser->getActiveScope());
     yylhsminor.yy161->setSourceInfo(pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy0));
     pParser->onDeclBegin(yylhsminor.yy161);
}
  yymsp[-1].minor.yy161 = yylhsminor.yy161;
        break;
      case 33: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
      case 37: /* type_function_param_list ::= PAR_BEGIN type_function_args_decl PAR_END */ yytestcase(yyruleno==37);
{
    yymsp[-2].minor.yy120 = yymsp[-1].minor.yy120;
}
        break;
      case 34: /* decl_function ::= function_header SCOPE_BEGIN statement_list_or_empty SCOPE_END */
{
    yylhsminor.yy161 = yymsp[-3].minor.yy161;
    yylhsminor.yy161->setFunctionBody(yymsp[-1].minor.yy2);
    yylhsminor.yy161->setSourceInfo(pParser->createSourceInfo(yymsp[-3].minor.yy161->getSourceInfo(), yymsp[0].minor.yy0));
    pParser->onDeclEnd();
}
  yymsp[-3].minor.yy161 = yylhsminor.yy161;
        break;
      case 35: /* type_function_header ::= type_function_start type_function_param_list function_return_decl */
{
    yylhsminor.yy129 = yymsp[-2].minor.yy129;
    yylhsminor.yy129->setParams(yymsp[-1].minor.yy120);
    yylhsminor.yy129->setReturnType(yymsp[0].minor.yy64);
}
  yymsp[-2].minor.yy129 = yylhsminor.yy129;
        break;
      case 36: /* type_function_start ::= FUNCTION type_specifier COLON COLON IDENTIFIER */
{
    yylhsminor.yy129 = pParser->newAstNode<yal::DeclTypeFunction>(yymsp[0].minor.yy0.tokenStr,
                                                   pParser->getActiveScope(),
                                                   yymsp[-3].minor.yy64);
    yylhsminor.yy129->setSourceInfo(pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0));
    pParser->onDeclBegin(yylhsminor.yy129);
}
  yymsp[-4].minor.yy129 = yylhsminor.yy129;
        break;
      case 38: /* type_function_decl ::= type_function_header SCOPE_BEGIN statement_list_or_empty SCOPE_END */
{
    yylhsminor.yy129 = yymsp[-3].minor.yy129;
    yylhsminor.yy129->setFunctionBody(yymsp[-1].minor.yy2);
    yylhsminor.yy129->setSourceInfo(pParser->createSourceInfo(yymsp[-3].minor.yy129->getSourceInfo(), yymsp[0].minor.yy0));
    pParser->onDeclEnd();
}
  yymsp[-3].minor.yy129 = yylhsminor.yy129;
        break;
      case 39: /* type_function_args_decl ::= qualifier SELF */
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
  yymsp[-1].minor.yy120 = yylhsminor.yy120;
        break;
      case 40: /* type_function_args_decl ::= qualifier SELF COMMA type_function_args_decl_other */
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
  yymsp[-3].minor.yy120 = yylhsminor.yy120;
        break;
      case 41: /* type_function_args_decl ::= function_args_decl */
{
        yylhsminor.yy120 = yymsp[0].minor.yy120;
}
  yymsp[0].minor.yy120 = yylhsminor.yy120;
        break;
      case 42: /* type_function_args_decl_other ::= type_function_args_decl_other COMMA function_arg_decl */
{
    if (yymsp[0].minor.yy173 != nullptr && pParser->onDecl(yymsp[0].minor.yy173)) {
        yylhsminor.yy120 = yymsp[-2].minor.yy120;
        yylhsminor.yy120->addDeclParam(yymsp[0].minor.yy173);
    }
}
  yymsp[-2].minor.yy120 = yylhsminor.yy120;
        break;
      case 43: /* type_function_args_decl_other ::= function_arg_decl */
{
    if (yymsp[0].minor.yy173 != nullptr && pParser->onDecl(yymsp[0].minor.yy173)) {
        yylhsminor.yy120 = pParser->newAstNode<yal::DeclParamVarContainer>();
        yylhsminor.yy120->addDeclParam(yymsp[0].minor.yy173);
    }
}
  yymsp[0].minor.yy120 = yylhsminor.yy120;
        break;
      case 44: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
{
    if (yymsp[0].minor.yy173 != nullptr && pParser->onDecl(yymsp[0].minor.yy173)){
        yymsp[-2].minor.yy120->addDeclParam(yymsp[0].minor.yy173);
        yylhsminor.yy120 = yymsp[-2].minor.yy120;
    }
}
  yymsp[-2].minor.yy120 = yylhsminor.yy120;
        break;
      case 45: /* function_args_decl ::= function_arg_decl */
{
    if (yymsp[0].minor.yy173 != nullptr && pParser->onDecl(yymsp[0].minor.yy173)){
        yylhsminor.yy120 = pParser->newAstNode<yal::DeclParamVarContainer>();
        yylhsminor.yy120->addDeclParam(yymsp[0].minor.yy173);
    }
}
  yymsp[0].minor.yy120 = yylhsminor.yy120;
        break;
      case 46: /* function_args_decl ::= */
{
        yymsp[1].minor.yy120 = nullptr;
}
        break;
      case 47: /* function_arg_decl ::= IDENTIFIER COLON qualified_type */
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
  yymsp[-2].minor.yy173 = yylhsminor.yy173;
        break;
      case 48: /* function_return_decl ::= COLON qualified_type */
{ yymsp[-1].minor.yy64 = yymsp[0].minor.yy64;
}
        break;
      case 49: /* function_return_decl ::= */
      case 62: /* var_type_spec ::= */ yytestcase(yyruleno==62);
{yymsp[1].minor.yy64 = nullptr;}
        break;
      case 50: /* statement_list_or_empty ::= */
{yymsp[1].minor.yy2 = nullptr;}
        break;
      case 51: /* statement_list_or_empty ::= statement_list */
{yylhsminor.yy2 = yymsp[0].minor.yy2;}
  yymsp[0].minor.yy2 = yylhsminor.yy2;
        break;
      case 52: /* statement_list ::= statement_list statement */
{
    yymsp[-1].minor.yy2->addStatement(yymsp[0].minor.yy170);
    yylhsminor.yy2=yymsp[-1].minor.yy2;
}
  yymsp[-1].minor.yy2 = yylhsminor.yy2;
        break;
      case 53: /* statement_list ::= statement */
{
    yylhsminor.yy2 = pParser->newAstNode<yal::StatementList>();
    yylhsminor.yy2->addStatement(yymsp[0].minor.yy170);
}
  yymsp[0].minor.yy2 = yylhsminor.yy2;
        break;
      case 54: /* statement ::= expression ASSIGN expression SEMI_COLON */
{
   yylhsminor.yy170 = pParser->newAstNode<yal::StmtAssign>(yymsp[-3].minor.yy49,yymsp[-1].minor.yy49);
}
  yymsp[-3].minor.yy170 = yylhsminor.yy170;
        break;
      case 55: /* statement ::= decl_var SEMI_COLON */
{
    yylhsminor.yy170 = yymsp[-1].minor.yy176;
}
  yymsp[-1].minor.yy170 = yylhsminor.yy170;
        break;
      case 56: /* statement ::= expression SEMI_COLON */
{yylhsminor.yy170 = yymsp[-1].minor.yy49;}
  yymsp[-1].minor.yy170 = yylhsminor.yy170;
        break;
      case 57: /* statement ::= RETURN expression SEMI_COLON */
{
    yylhsminor.yy170 = pParser->newAstNode<yal::StmtReturn>(yymsp[-1].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy49->getSourceInfo());
    yylhsminor.yy170->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy170 = yylhsminor.yy170;
        break;
      case 58: /* statement ::= RETURN SEMI_COLON */
{
    yylhsminor.yy170 = pParser->newAstNode<yal::StmtReturn>();
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0,yymsp[-1].minor.yy0);
    yylhsminor.yy170->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy170 = yylhsminor.yy170;
        break;
      case 59: /* decl_var ::= VAR IDENTIFIER var_type_spec ASSIGN expression */
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
  yymsp[-4].minor.yy176 = yylhsminor.yy176;
        break;
      case 60: /* decl_var ::= LET IDENTIFIER var_type_spec ASSIGN expression */
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
  yymsp[-4].minor.yy176 = yylhsminor.yy176;
        break;
      case 61: /* var_type_spec ::= COLON qualified_type */
{yymsp[-1].minor.yy64 = yymsp[0].minor.yy64;}
        break;
      case 63: /* expression ::= PAR_BEGIN expression PAR_END */
{yymsp[-2].minor.yy49 = yymsp[-1].minor.yy49;}
        break;
      case 64: /* expression ::= literal */
{yylhsminor.yy49 = yymsp[0].minor.yy49;}
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 65: /* expression ::= unaryexp */
{yylhsminor.yy49 = yymsp[0].minor.yy117;}
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 66: /* expression ::= binaryexp */
{yylhsminor.yy49 = yymsp[0].minor.yy69;}
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 67: /* expression ::= struct_init */
{ yylhsminor.yy49 = yymsp[0].minor.yy95;}
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 68: /* expression ::= RANGE_CAST LT qualified_type GT PAR_BEGIN expression PAR_END */
{
    yylhsminor.yy49 = pParser->newAstNode<yal::ExprRangeCast>(yymsp[-4].minor.yy64, yymsp[-1].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0);
    yylhsminor.yy49->setSourceInfo(srcInfo);
}
  yymsp[-6].minor.yy49 = yylhsminor.yy49;
        break;
      case 69: /* expression ::= IDENTIFIER */
{
    auto decl = pParser->resolveVarRef(yymsp[0].minor.yy0);
    if (decl != nullptr) {
        yylhsminor.yy49 = pParser->newAstNode<yal::ExprVarRef>(decl);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy49->setSourceInfo(srcInfo);
    }
}
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 70: /* expression ::= expression DOT IDENTIFIER */
{
    yylhsminor.yy49 = pParser->newAstNode<yal::ExprStructVarRef>(yymsp[-2].minor.yy49, yymsp[0].minor.yy0.tokenStr);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy49->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy49 = yylhsminor.yy49;
        break;
      case 71: /* expression ::= SELF */
{
    auto decl = pParser->resolveVarRefSelf(yymsp[0].minor.yy0);
    if (decl != nullptr) {
        yylhsminor.yy49  = pParser->newAstNode<yal::ExprVarRefSelf>(decl);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy49->setSourceInfo(srcInfo);
    }
}
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 72: /* expression ::= IDENTIFIER PAR_BEGIN function_call_args PAR_END */
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
  yymsp[-3].minor.yy49 = yylhsminor.yy49;
        break;
      case 73: /* expression ::= expression DOT IDENTIFIER PAR_BEGIN function_call_args PAR_END */
{
    if (yymsp[-1].minor.yy146 != nullptr) {
        yymsp[-1].minor.yy146->updateSourceInfo();
    }
    yylhsminor.yy49 = pParser->newAstNode<yal::ExprTypeFnCall>(yymsp[-5].minor.yy49, yymsp[-3].minor.yy0, yymsp[-1].minor.yy146);
    auto srcInfo = pParser->createSourceInfo(yymsp[-5].minor.yy49->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy49->setSourceInfo(srcInfo);

}
  yymsp[-5].minor.yy49 = yylhsminor.yy49;
        break;
      case 74: /* expression ::= type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_call_args PAR_END */
{
    if (yymsp[-1].minor.yy146 != nullptr) {
        yymsp[-1].minor.yy146->updateSourceInfo();
    }
    yylhsminor.yy49 = pParser->newAstNode<yal::ExprTypeFnCallStatic>(yymsp[-6].minor.yy64, yymsp[-3].minor.yy0, yymsp[-1].minor.yy146);
    auto srcInfo = pParser->createSourceInfo(yymsp[-6].minor.yy64->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy49->setSourceInfo(srcInfo);
}
  yymsp[-6].minor.yy49 = yylhsminor.yy49;
        break;
      case 75: /* binaryexp ::= expression AND expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 76: /* binaryexp ::= expression OR expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 77: /* binaryexp ::= expression PLUS expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 78: /* binaryexp ::= expression MINUS expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 79: /* binaryexp ::= expression MULT expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 80: /* binaryexp ::= expression DIV expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 81: /* binaryexp ::= expression MOD expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 82: /* binaryexp ::= expression EQ expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 83: /* binaryexp ::= expression NE expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 84: /* binaryexp ::= expression LE expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 85: /* binaryexp ::= expression LT expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 86: /* binaryexp ::= expression GE expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 87: /* binaryexp ::= expression GT expression */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy49, yymsp[0].minor.yy49);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy49->getSourceInfo(), yymsp[0].minor.yy49->getSourceInfo());
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 88: /* unaryexp ::= NOT expression */
{
    yylhsminor.yy117 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
    yylhsminor.yy117->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy117 = yylhsminor.yy117;
        break;
      case 89: /* unaryexp ::= BIT_NOT expression */
{
    yylhsminor.yy117 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
    yylhsminor.yy117->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy117 = yylhsminor.yy117;
        break;
      case 90: /* unaryexp ::= MINUS expression */
{
    yylhsminor.yy117 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
    yylhsminor.yy117->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy117 = yylhsminor.yy117;
        break;
      case 91: /* unaryexp ::= REFERENCE expression */
{
    yylhsminor.yy117 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
    yylhsminor.yy117->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy117 = yylhsminor.yy117;
        break;
      case 92: /* function_call_args ::= function_call_args COMMA expression */
{
    yylhsminor.yy146 = yymsp[-2].minor.yy146;
    yylhsminor.yy146->addExpression(yymsp[0].minor.yy49);
}
  yymsp[-2].minor.yy146 = yylhsminor.yy146;
        break;
      case 93: /* function_call_args ::= expression */
{
    yylhsminor.yy146 = pParser->newAstNode<yal::ExprList>();
    yylhsminor.yy146->addExpression(yymsp[0].minor.yy49);
}
  yymsp[0].minor.yy146 = yylhsminor.yy146;
        break;
      case 94: /* function_call_args ::= */
{
    yymsp[1].minor.yy146= nullptr;
}
        break;
      case 95: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
{
    if (yymsp[-1].minor.yy188) {
        yymsp[-1].minor.yy188->updateSourceInfo();
    }

    yylhsminor.yy95 = pParser->newAstNode<yal::ExprStructInit>(yymsp[-3].minor.yy64, yymsp[-1].minor.yy188);
    auto srcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy64->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy95->setSourceInfo(srcInfo);
}
  yymsp[-3].minor.yy95 = yylhsminor.yy95;
        break;
      case 96: /* struct_member_init_list ::= */
{yymsp[1].minor.yy188 = nullptr;}
        break;
      case 97: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
{
     yymsp[-2].minor.yy188->addStructMemberInit(yymsp[0].minor.yy14);
     yylhsminor.yy188 = yymsp[-2].minor.yy188;
}
  yymsp[-2].minor.yy188 = yylhsminor.yy188;
        break;
      case 98: /* struct_member_init_list ::= struct_member_init */
{
        yylhsminor.yy188 = pParser->newAstNode<yal::StructMemberInitList>();
        yylhsminor.yy188->addStructMemberInit(yymsp[0].minor.yy14);
}
  yymsp[0].minor.yy188 = yylhsminor.yy188;
        break;
      case 99: /* struct_member_init ::= IDENTIFIER COLON expression */
{
    yylhsminor.yy14 = pParser->newAstNode<yal::StructMemberInit>(yymsp[-2].minor.yy0.tokenStr,yymsp[0].minor.yy49);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy49->getSourceInfo());
    yylhsminor.yy14->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy14 = yylhsminor.yy14;
        break;
      case 100: /* literal ::= INTEGER_LITERAL */
{
        yylhsminor.yy49 = pParser->newIntegerLiteral(yymsp[0].minor.yy0);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy49->setSourceInfo(srcInfo);
   }
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 101: /* literal ::= FLOAT_LITERAL */
{
        yylhsminor.yy49 = pParser->newFloatLiteral(yymsp[0].minor.yy0);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy49->setSourceInfo(srcInfo);
}
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      case 102: /* literal ::= BOOL_LITERAL */
{
        yylhsminor.yy49 = pParser->newAstNode<yal::ExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy49->setSourceInfo(srcInfo);
}
  yymsp[0].minor.yy49 = yylhsminor.yy49;
        break;
      default:
      /* (103) module ::= decls END */ yytestcase(yyruleno==103);
      /* (104) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==104);
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
    }
#endif
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
