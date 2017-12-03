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
#include "yal/ast/typebuiltin.h"
#include "yal/ast/declfunction.h"
#include "yal/ast/decltypefunction.h"
#include "yal/ast/declmodule.h"
#include "yal/ast/reftype.h"
#include "yal/ast/declvar.h"
#include "yal/ast/declparamvar.h"
#include "yal/ast/stmtexpression.h"
#include "yal/ast/stmtreturn.h"
#include "yal/ast/exprunaryoperator.h"
#include "yal/ast/exprbinaryoperator.h"
#include "yal/ast/statement.h"
#include "yal/ast/statementlist.h"
#include "yal/ast/exprintegerliteral.h"
#include "yal/ast/stmtreturn.h"
#include "yal/ast/exprboolliteral.h"
#include "yal/ast/stmtdecl.h"
#include "yal/lexer/lexer.h"
#include "yal/ast/stmtassign.h"
#include "yal/ast/exprvarref.h"
#include "yal/ast/declstruct.h"
#include "yal/ast/exprstructvarref.h"
#include "yal/ast/exprstructfncall.h"
#include "yal/ast/exprlist.h"
#include "yal/ast/exprdecimalliteral.h"
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
#define YAL_TOKEN_STRUCT                         41
#define YAL_TOKEN_SCOPE_BEGIN                    42
#define YAL_TOKEN_SCOPE_END                      43
#define YAL_TOKEN_SEMI_COLON                     44
#define YAL_TOKEN_FUNCTION                       45
#define YAL_TOKEN_PAR_BEGIN                      46
#define YAL_TOKEN_PAR_END                        47
#define YAL_TOKEN_SELF                           48
#define YAL_TOKEN_COMMA                          49
#define YAL_TOKEN_RETURN                         50
#define YAL_TOKEN_VAR                            51
#define YAL_TOKEN_LET                            52
#define YAL_TOKEN_INTEGER_LITERAL                53
#define YAL_TOKEN_DECIMAL_LITERAL                54
#define YAL_TOKEN_BOOL_LITERAL                   55
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
#define YYNOCODE 86
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALParserTOKENTYPE yal::TokenInfo
#endif
typedef union {
  int yyinit;
  YALParserTOKENTYPE yy0;
  yal::RefTypeResolved* yy8;
  yal::StmtDecl* yy13;
  yal::DeclParamVarContainer* yy24;
  uint32_t yy41;
  yal::RefType* yy48;
  yal::StatementList* yy50;
  yal::ExprList* yy56;
  yal::DeclModule* yy63;
  yal::DeclFunction* yy69;
  yal::ExprBinaryOperator* yy71;
  yal::DeclTypeFunction* yy73;
  yal::DeclStructMembers* yy85;
  yal::DeclVar* yy96;
  yal::StringRefPod yy120;
  yal::ExprUnaryOperator* yy125;
  yal::Statement* yy128;
  yal::DeclParamVar* yy145;
  yal::DeclStruct* yy156;
  yal::StmtExpression* yy159;
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
#define YYNSTATE             106
#define YYNRULE              87
#define YY_MAX_SHIFT         105
#define YY_MIN_SHIFTREDUCE   159
#define YY_MAX_SHIFTREDUCE   245
#define YY_MIN_REDUCE        246
#define YY_MAX_REDUCE        332
#define YY_ERROR_ACTION      333
#define YY_ACCEPT_ACTION     334
#define YY_NO_ACTION         335
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
#define YY_ACTTAB_COUNT (437)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     8,   24,   25,   32,  334,   80,   18,   17,   13,   15,
 /*    10 */    14,   16,   23,   22,   21,   20,   19,   24,   25,   48,
 /*    20 */    96,   44,   18,   17,   13,   15,   14,   16,   23,   22,
 /*    30 */    21,   20,   19,   98,  168,   99,   96,  247,  248,   24,
 /*    40 */    25,   31,  284,  206,   18,   17,   13,   15,   14,   16,
 /*    50 */    23,   22,   21,   20,   19,   24,   25,  246,   96,  207,
 /*    60 */    18,   17,   13,   15,   14,   16,   23,   22,   21,   20,
 /*    70 */    19,   31,  285,  271,   96,   31,  298,   24,   25,   31,
 /*    80 */    97,  204,   18,   17,   13,   15,   14,   16,   23,   22,
 /*    90 */    21,   20,   19,   21,   20,   19,   96,   72,  280,   96,
 /*   100 */   213,  279,  275,   24,   25,   80,   98,  168,   18,   17,
 /*   110 */    13,   15,   14,   16,   23,   22,   21,   20,   19,  186,
 /*   120 */   272,   40,   96,   10,   50,  302,  303,  301,   11,   12,
 /*   130 */   253,    1,   94,   57,  302,  303,  301,   10,   99,   34,
 /*   140 */   281,   70,   11,   12,   35,  102,   94,   68,   42,   81,
 /*   150 */   282,   87,  200,   47,  221,   26,    9,  218,  182,    5,
 /*   160 */    84,   82,  241,  242,  243,  220,  201,    9,  274,   26,
 /*   170 */    83,  218,  251,    5,   84,   82,  241,  242,  243,  165,
 /*   180 */    44,   41,  171,  172,  173,  174,  175,  176,  177,  178,
 /*   190 */   179,  180,  181,   37,   25,   73,   43,  100,   18,   17,
 /*   200 */    13,   15,   14,   16,   23,   22,   21,   20,   19,   28,
 /*   210 */    86,   76,   96,  171,  172,  173,  174,  175,  176,  177,
 /*   220 */   178,  179,  180,  181,  165,   77,   78,  171,  172,  173,
 /*   230 */   174,  175,  176,  177,  178,  179,  180,  181,   18,   17,
 /*   240 */    13,   15,   14,   16,   23,   22,   21,   20,   19,   33,
 /*   250 */     6,   45,   96,  333,  333,  333,  333,  333,  333,   23,
 /*   260 */    22,   21,   20,   19,    7,   46,  205,   96,   10,   71,
 /*   270 */    75,   38,  282,   11,   12,    3,  183,   94,   10,   54,
 /*   280 */   302,  303,  301,   11,   12,   74,    2,   94,  290,   49,
 /*   290 */   302,  303,  301,   85,   55,  302,  303,  301,  208,   95,
 /*   300 */    26,   96,  218,   57,  302,  303,  301,  241,  242,  243,
 /*   310 */    26,   69,  218,   51,  302,  303,  301,  241,  242,  243,
 /*   320 */   289,   49,  302,  303,  301,   85,   56,  302,  303,  301,
 /*   330 */    88,  302,  303,  301,   89,  302,  303,  301,    4,   27,
 /*   340 */    36,   90,  302,  303,  301,  169,   39,   60,  302,  303,
 /*   350 */   301,   61,  302,  303,  301,   62,  302,  303,  301,  245,
 /*   360 */    63,  302,  303,  301,   64,  302,  303,  301,   65,  302,
 /*   370 */   303,  301,   91,  302,  303,  301,   92,  302,  303,  301,
 /*   380 */    93,  302,  303,  301,   66,  302,  303,  301,   67,  302,
 /*   390 */   303,  301,   58,  302,  303,  301,   59,  302,  303,  301,
 /*   400 */    52,  302,  303,  301,   53,  302,  303,  301,  102,  102,
 /*   410 */   105,  101,   29,  103,   79,  257,  331,  248,  248,  248,
 /*   420 */   248,  248,  248,  248,  248,  248,  248,  104,  248,  248,
 /*   430 */   248,  248,  248,   30,  248,  251,  251,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     1,    2,    3,   60,   61,   23,    7,    8,    9,   10,
 /*    10 */    11,   12,   13,   14,   15,   16,   17,    2,    3,   47,
 /*    20 */    21,   49,    7,    8,    9,   10,   11,   12,   13,   14,
 /*    30 */    15,   16,   17,   24,   25,   23,   21,   58,   59,    2,
 /*    40 */     3,   82,   83,   44,    7,    8,    9,   10,   11,   12,
 /*    50 */    13,   14,   15,   16,   17,    2,    3,   78,   21,   44,
 /*    60 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*    70 */    17,   82,   83,   79,   21,   82,   83,    2,    3,   82,
 /*    80 */    83,   44,    7,    8,    9,   10,   11,   12,   13,   14,
 /*    90 */    15,   16,   17,   15,   16,   17,   21,   68,   69,   21,
 /*   100 */    47,   69,   71,    2,    3,   23,   24,   25,    7,    8,
 /*   110 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   44,
 /*   120 */    79,   80,   21,   14,   73,   74,   75,   76,   19,   20,
 /*   130 */    48,   42,   23,   73,   74,   75,   76,   14,   23,   40,
 /*   140 */    69,   81,   19,   20,   40,   57,   23,   66,   62,   64,
 /*   150 */    69,   63,   43,   62,   47,   46,   49,   48,   43,   50,
 /*   160 */    51,   52,   53,   54,   55,   47,   43,   49,   71,   46,
 /*   170 */    64,   48,   84,   50,   51,   52,   53,   54,   55,   23,
 /*   180 */    49,   49,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   190 */    34,   35,   36,   49,    3,   48,   47,   41,    7,    8,
 /*   200 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   46,
 /*   210 */    23,   23,   21,   26,   27,   28,   29,   30,   31,   32,
 /*   220 */    33,   34,   35,   36,   23,   40,   40,   26,   27,   28,
 /*   230 */    29,   30,   31,   32,   33,   34,   35,   36,    7,    8,
 /*   240 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   40,
 /*   250 */     1,   23,   21,    7,    8,    9,   10,   11,   12,   13,
 /*   260 */    14,   15,   16,   17,    1,   23,   44,   21,   14,   66,
 /*   270 */    67,   46,   69,   19,   20,   46,   44,   23,   14,   73,
 /*   280 */    74,   75,   76,   19,   20,   82,   70,   23,   72,   73,
 /*   290 */    74,   75,   76,   77,   73,   74,   75,   76,   44,   23,
 /*   300 */    46,   21,   48,   73,   74,   75,   76,   53,   54,   55,
 /*   310 */    46,   81,   48,   73,   74,   75,   76,   53,   54,   55,
 /*   320 */    72,   73,   74,   75,   76,   77,   73,   74,   75,   76,
 /*   330 */    73,   74,   75,   76,   73,   74,   75,   76,   46,    1,
 /*   340 */    40,   73,   74,   75,   76,   25,   42,   73,   74,   75,
 /*   350 */    76,   73,   74,   75,   76,   73,   74,   75,   76,   38,
 /*   360 */    73,   74,   75,   76,   73,   74,   75,   76,   73,   74,
 /*   370 */    75,   76,   73,   74,   75,   76,   73,   74,   75,   76,
 /*   380 */    73,   74,   75,   76,   73,   74,   75,   76,   73,   74,
 /*   390 */    75,   76,   73,   74,   75,   76,   73,   74,   75,   76,
 /*   400 */    73,   74,   75,   76,   73,   74,   75,   76,   57,   57,
 /*   410 */    22,   37,   40,   23,   63,   63,    0,   85,   85,   85,
 /*   420 */    85,   85,   85,   85,   85,   85,   85,   39,   85,   85,
 /*   430 */    85,   85,   85,   45,   85,   84,   84,
};
#define YY_SHIFT_USE_DFLT (437)
#define YY_SHIFT_COUNT    (105)
#define YY_SHIFT_MIN      (-28)
#define YY_SHIFT_MAX      (416)
static const short yy_shift_ofst[] = {
 /*     0 */   437,  109,  123,  264,  264,  254,  264,  264,  264,  264,
 /*    10 */   264,  264,  264,  264,  264,  264,  264,  264,  264,  264,
 /*    20 */   264,  264,  264,  264,  264,  264,  264,  264,   82,  156,
 /*    30 */   187,  201,  388,    9,    9,    9,    9,  -18,  -18,   12,
 /*    40 */   115,  -18,   89,   99,  -18,  104,  104,   89,   99,   -1,
 /*    50 */    15,   37,   53,   75,  101,  101,  101,  101,  191,  231,
 /*    60 */   246,  246,  246,  246,  246,  246,   78,   78,  -28,  107,
 /*    70 */   118,  131,  132,  144,  147,  149,  163,  188,  185,  186,
 /*    80 */   209,  249,  228,  263,  242,  222,  225,  232,  280,  280,
 /*    90 */   280,  280,  280,  280,  229,  292,  276,  338,  320,  300,
 /*   100 */   304,  321,  374,  372,  390,  416,
};
#define YY_REDUCE_USE_DFLT (-58)
#define YY_REDUCE_COUNT (48)
#define YY_REDUCE_MIN   (-57)
#define YY_REDUCE_MAX   (352)
static const short yy_reduce_ofst[] = {
 /*     0 */   -57,  216,  248,   60,  230,   51,  206,  221,  240,  253,
 /*    10 */   257,  261,  268,  274,  278,  282,  287,  291,  295,  299,
 /*    20 */   303,  307,  311,  315,  319,  323,  327,  331,  203,   88,
 /*    30 */   351,  352,  -21,  -41,  -11,   -7,   -3,   29,   81,   41,
 /*    40 */    -6,   32,   31,   86,   71,   85,  106,   97,   91,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   249,  333,  333,  327,  327,  333,  333,  333,  333,  333,
 /*    10 */   333,  333,  333,  333,  333,  333,  333,  333,  333,  333,
 /*    20 */   333,  333,  333,  333,  333,  333,  333,  333,  283,  333,
 /*    30 */   333,  333,  333,  253,  253,  253,  253,  333,  283,  333,
 /*    40 */   333,  333,  333,  286,  333,  299,  299,  333,  286,  333,
 /*    50 */   333,  333,  333,  333,  297,  296,  325,  326,  310,  309,
 /*    60 */   321,  320,  319,  318,  317,  316,  312,  311,  333,  333,
 /*    70 */   333,  278,  277,  276,  333,  333,  333,  333,  333,  333,
 /*    80 */   333,  333,  333,  333,  333,  333,  252,  333,  324,  323,
 /*    90 */   322,  315,  314,  313,  304,  306,  333,  333,  254,  333,
 /*   100 */   333,  333,  250,  333,  333,  333,
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
  "COLON",         "STRUCT",        "SCOPE_BEGIN",   "SCOPE_END",   
  "SEMI_COLON",    "FUNCTION",      "PAR_BEGIN",     "PAR_END",     
  "SELF",          "COMMA",         "RETURN",        "VAR",         
  "LET",           "INTEGER_LITERAL",  "DECIMAL_LITERAL",  "BOOL_LITERAL",
  "error",         "type_builtin",  "decl_function",  "type_function_decl",
  "decls",         "module",        "function_return_decl",  "type_specifier",
  "var_type_spec",  "named_decl",    "function_args_decl",  "type_function_args_decl",
  "type_function_args_decl_other",  "function_arg_decl",  "statement_list",  "function_scope",
  "statement",     "expression",    "unaryexp",      "binaryexp",   
  "literal",       "var_decl",      "type_decl",     "type_var_decl",
  "type_var_decls",  "function_call_args",  "qualifier",     "qualified_type",
  "type_array",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "decls ::= decls type_decl",
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
 /*  23 */ "type_decl ::= TYPE IDENTIFIER COLON STRUCT SCOPE_BEGIN type_var_decls SCOPE_END",
 /*  24 */ "type_decl ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON",
 /*  25 */ "type_var_decls ::= type_var_decls type_var_decl",
 /*  26 */ "type_var_decls ::= type_var_decl",
 /*  27 */ "type_var_decl ::= IDENTIFIER COLON qualified_type ASSIGN expression SEMI_COLON",
 /*  28 */ "decl_function ::= FUNCTION IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope",
 /*  29 */ "type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN type_function_args_decl PAR_END function_return_decl function_scope",
 /*  30 */ "type_function_args_decl ::= qualifier SELF",
 /*  31 */ "type_function_args_decl ::= qualifier SELF COMMA type_function_args_decl_other",
 /*  32 */ "type_function_args_decl ::= function_args_decl",
 /*  33 */ "type_function_args_decl_other ::= type_function_args_decl_other COMMA function_arg_decl",
 /*  34 */ "type_function_args_decl_other ::= function_arg_decl",
 /*  35 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  36 */ "function_args_decl ::= function_arg_decl",
 /*  37 */ "function_args_decl ::=",
 /*  38 */ "function_arg_decl ::= IDENTIFIER COLON qualified_type",
 /*  39 */ "function_return_decl ::= COLON qualified_type",
 /*  40 */ "function_return_decl ::=",
 /*  41 */ "function_scope ::= SCOPE_BEGIN SCOPE_END",
 /*  42 */ "function_scope ::= SCOPE_BEGIN statement_list SCOPE_END",
 /*  43 */ "statement_list ::= statement_list statement",
 /*  44 */ "statement_list ::= statement",
 /*  45 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  46 */ "statement ::= var_decl SEMI_COLON",
 /*  47 */ "statement ::= expression SEMI_COLON",
 /*  48 */ "statement ::= RETURN expression SEMI_COLON",
 /*  49 */ "statement ::= RETURN SEMI_COLON",
 /*  50 */ "var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression",
 /*  51 */ "var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression",
 /*  52 */ "var_type_spec ::= COLON qualified_type",
 /*  53 */ "var_type_spec ::=",
 /*  54 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  55 */ "expression ::= literal",
 /*  56 */ "expression ::= unaryexp",
 /*  57 */ "expression ::= binaryexp",
 /*  58 */ "expression ::= IDENTIFIER",
 /*  59 */ "expression ::= SELF",
 /*  60 */ "expression ::= expression DOT IDENTIFIER",
 /*  61 */ "expression ::= IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  62 */ "expression ::= expression DOT IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  63 */ "binaryexp ::= expression AND expression",
 /*  64 */ "binaryexp ::= expression OR expression",
 /*  65 */ "binaryexp ::= expression PLUS expression",
 /*  66 */ "binaryexp ::= expression MINUS expression",
 /*  67 */ "binaryexp ::= expression MULT expression",
 /*  68 */ "binaryexp ::= expression DIV expression",
 /*  69 */ "binaryexp ::= expression MOD expression",
 /*  70 */ "binaryexp ::= expression EQ expression",
 /*  71 */ "binaryexp ::= expression NE expression",
 /*  72 */ "binaryexp ::= expression LE expression",
 /*  73 */ "binaryexp ::= expression LT expression",
 /*  74 */ "binaryexp ::= expression GE expression",
 /*  75 */ "binaryexp ::= expression GT expression",
 /*  76 */ "unaryexp ::= NOT expression",
 /*  77 */ "unaryexp ::= BIT_NOT expression",
 /*  78 */ "unaryexp ::= MINUS expression",
 /*  79 */ "function_call_args ::= function_call_args COMMA expression",
 /*  80 */ "function_call_args ::= expression",
 /*  81 */ "function_call_args ::=",
 /*  82 */ "literal ::= INTEGER_LITERAL",
 /*  83 */ "literal ::= DECIMAL_LITERAL",
 /*  84 */ "literal ::= BOOL_LITERAL",
 /*  85 */ "module ::= decls END",
 /*  86 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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
  { 60, -2 },
  { 60, -2 },
  { 60, -2 },
  { 60, 0 },
  { 63, -1 },
  { 63, -1 },
  { 63, -1 },
  { 82, 0 },
  { 82, -1 },
  { 82, -1 },
  { 82, -2 },
  { 83, -2 },
  { 57, -1 },
  { 57, -1 },
  { 57, -1 },
  { 57, -1 },
  { 57, -1 },
  { 57, -1 },
  { 57, -1 },
  { 57, -1 },
  { 57, -1 },
  { 57, -1 },
  { 57, -1 },
  { 78, -7 },
  { 78, -5 },
  { 80, -2 },
  { 80, -1 },
  { 79, -6 },
  { 58, -7 },
  { 59, -10 },
  { 67, -2 },
  { 67, -4 },
  { 67, -1 },
  { 68, -3 },
  { 68, -1 },
  { 66, -3 },
  { 66, -1 },
  { 66, 0 },
  { 69, -3 },
  { 62, -2 },
  { 62, 0 },
  { 71, -2 },
  { 71, -3 },
  { 70, -2 },
  { 70, -1 },
  { 72, -4 },
  { 72, -2 },
  { 72, -2 },
  { 72, -3 },
  { 72, -2 },
  { 77, -5 },
  { 77, -5 },
  { 64, -2 },
  { 64, 0 },
  { 73, -3 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -3 },
  { 73, -4 },
  { 73, -6 },
  { 75, -3 },
  { 75, -3 },
  { 75, -3 },
  { 75, -3 },
  { 75, -3 },
  { 75, -3 },
  { 75, -3 },
  { 75, -3 },
  { 75, -3 },
  { 75, -3 },
  { 75, -3 },
  { 75, -3 },
  { 75, -3 },
  { 74, -2 },
  { 74, -2 },
  { 74, -2 },
  { 81, -3 },
  { 81, -1 },
  { 81, 0 },
  { 76, -1 },
  { 76, -1 },
  { 76, -1 },
  { 61, -2 },
  { 84, -3 },
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
      case 0: /* decls ::= decls type_decl */
{
    yylhsminor.yy63 =yymsp[-1].minor.yy63; yymsp[-1].minor.yy63->addDecl(yymsp[0].minor.yy156);
}
  yymsp[-1].minor.yy63 = yylhsminor.yy63;
        break;
      case 1: /* decls ::= decls decl_function */
{
        yylhsminor.yy63 =yymsp[-1].minor.yy63; yymsp[-1].minor.yy63->addDecl(yymsp[0].minor.yy69);
}
  yymsp[-1].minor.yy63 = yylhsminor.yy63;
        break;
      case 2: /* decls ::= decls type_function_decl */
{
        yylhsminor.yy63 =yymsp[-1].minor.yy63; yymsp[-1].minor.yy63->addDecl(yymsp[0].minor.yy73);
}
  yymsp[-1].minor.yy63 = yylhsminor.yy63;
        break;
      case 3: /* decls ::= */
{
        yymsp[1].minor.yy63 =  pParser->newAstNode<yal::DeclModule>();
        pParser->onAstNodeCreate(yymsp[1].minor.yy63);
     }
        break;
      case 4: /* type_specifier ::= type_builtin */
{yylhsminor.yy48=yymsp[0].minor.yy8;}
  yymsp[0].minor.yy48 = yylhsminor.yy48;
        break;
      case 5: /* type_specifier ::= type_array */
{yymsp[0].minor.yy48 = nullptr;}
        break;
      case 6: /* type_specifier ::= IDENTIFIER */
{
        yylhsminor.yy48 = pParser->newAstNode<yal::RefTypeUnresolved>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy48->setSourceInfo(srcInfo);
 }
  yymsp[0].minor.yy48 = yylhsminor.yy48;
        break;
      case 7: /* qualifier ::= */
{yymsp[1].minor.yy41 = 0;}
        break;
      case 8: /* qualifier ::= MUT */
{yymsp[0].minor.yy41 = yal::parser::kQualifierMutable;}
        break;
      case 9: /* qualifier ::= REFERENCE */
{yymsp[0].minor.yy41 = yal::parser::kQualifierReference;}
        break;
      case 10: /* qualifier ::= MUT REFERENCE */
{
        yymsp[-1].minor.yy41 = yal::parser::kQualifierReference | yal::parser::kQualifierMutable;
 }
        break;
      case 11: /* qualified_type ::= qualifier type_specifier */
{
        yymsp[0].minor.yy48->setQualifier(yal::parser::MakeQualifierFromFlags(yymsp[-1].minor.yy41));
        yylhsminor.yy48 = yymsp[0].minor.yy48;
}
  yymsp[-1].minor.yy48 = yylhsminor.yy48;
        break;
      case 12: /* type_builtin ::= TYPE_BOOL */
{
        yymsp[0].minor.yy8 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinBool());
        }
        break;
      case 13: /* type_builtin ::= TYPE_INT8 */
{
        yymsp[0].minor.yy8 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinI8());
        }
        break;
      case 14: /* type_builtin ::= TYPE_UINT8 */
{
        yymsp[0].minor.yy8  = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinU8());
        }
        break;
      case 15: /* type_builtin ::= TYPE_INT16 */
{
        yymsp[0].minor.yy8 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinI16());
        }
        break;
      case 16: /* type_builtin ::= TYPE_UINT16 */
{
        yymsp[0].minor.yy8  = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinU16());
        }
        break;
      case 17: /* type_builtin ::= TYPE_INT32 */
{
        yymsp[0].minor.yy8 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinI32());
        }
        break;
      case 18: /* type_builtin ::= TYPE_UINT32 */
{
        yymsp[0].minor.yy8 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinU32());
        }
        break;
      case 19: /* type_builtin ::= TYPE_INT64 */
{
        yymsp[0].minor.yy8 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinI64());
        }
        break;
      case 20: /* type_builtin ::= TYPE_UINT64 */
{
        yymsp[0].minor.yy8 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinU64());
        }
        break;
      case 21: /* type_builtin ::= TYPE_FLOAT */
{
        yymsp[0].minor.yy8 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinFloat());
        }
        break;
      case 22: /* type_builtin ::= TYPE_DOUBLE */
{
        yymsp[0].minor.yy8 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinDouble());
        }
        break;
      case 23: /* type_decl ::= TYPE IDENTIFIER COLON STRUCT SCOPE_BEGIN type_var_decls SCOPE_END */
{
    yylhsminor.yy156 = pParser->newAstNode<yal::DeclStruct>(yymsp[-5].minor.yy0.tokenStr, yymsp[-1].minor.yy85);
        if (yymsp[-1].minor.yy85 != nullptr) {
        yymsp[-1].minor.yy85->updateSourceInfo();
        }
    auto srcInfo = pParser->createSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0);
    yylhsminor.yy156->setSourceInfo(srcInfo);
}
  yymsp[-6].minor.yy156 = yylhsminor.yy156;
        break;
      case 24: /* type_decl ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON */
{
    yymsp[-4].minor.yy156 = nullptr;
}
        break;
      case 25: /* type_var_decls ::= type_var_decls type_var_decl */
{
    yylhsminor.yy85 = yymsp[-1].minor.yy85;
    yylhsminor.yy85->addDeclVar(yymsp[0].minor.yy96);
}
  yymsp[-1].minor.yy85 = yylhsminor.yy85;
        break;
      case 26: /* type_var_decls ::= type_var_decl */
{
    yylhsminor.yy85 = pParser->newAstNode<yal::DeclStructMembers>();
    yylhsminor.yy85->addDeclVar(yymsp[0].minor.yy96);
}
  yymsp[0].minor.yy85 = yylhsminor.yy85;
        break;
      case 27: /* type_var_decl ::= IDENTIFIER COLON qualified_type ASSIGN expression SEMI_COLON */
{
    yylhsminor.yy96 = pParser->newAstNode<yal::DeclVar>(yymsp[-5].minor.yy0.tokenStr, yal::Qualifier(), yymsp[-3].minor.yy48, yymsp[-1].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-5].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy96->setSourceInfo(srcInfo);
}
  yymsp[-5].minor.yy96 = yylhsminor.yy96;
        break;
      case 28: /* decl_function ::= FUNCTION IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope */
{
        yylhsminor.yy69 = pParser->newAstNode<yal::DeclFunction>(yymsp[-5].minor.yy0.tokenStr, yymsp[-3].minor.yy24, yymsp[-1].minor.yy48, yymsp[0].minor.yy50);
        if (yymsp[-3].minor.yy24 != nullptr) {
            yymsp[-3].minor.yy24->updateSourceInfo();
        }
        if (yymsp[0].minor.yy50 != nullptr) {
            yymsp[0].minor.yy50->updateSourceInfo();
        }
        yal::SourceInfo srcInfo;
        if (yymsp[0].minor.yy50 != nullptr) {
            srcInfo = pParser->createSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy50->getSourceInfo());
        } else if (yymsp[-1].minor.yy48 != nullptr) {
            srcInfo = pParser->createSourceInfo(yymsp[-6].minor.yy0, yymsp[-1].minor.yy48->getSourceInfo());
        } else {
            srcInfo = pParser->createSourceInfo(yymsp[-6].minor.yy0, yymsp[-2].minor.yy0);
        }
        yylhsminor.yy69->setSourceInfo(srcInfo);
}
  yymsp[-6].minor.yy69 = yylhsminor.yy69;
        break;
      case 29: /* type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN type_function_args_decl PAR_END function_return_decl function_scope */
{
            yylhsminor.yy73 = pParser->newAstNode<yal::DeclTypeFunction>(yymsp[-5].minor.yy0.tokenStr, yymsp[-3].minor.yy24, yymsp[-8].minor.yy48, yymsp[-1].minor.yy48, yymsp[0].minor.yy50);
        if (yymsp[-3].minor.yy24 != nullptr) {
            yymsp[-3].minor.yy24->updateSourceInfo();
        }
        if (yymsp[0].minor.yy50 != nullptr) {
            yymsp[0].minor.yy50->updateSourceInfo();
        }
        yal::SourceInfo srcInfo;
        if (yymsp[0].minor.yy50!= nullptr) {
            srcInfo = pParser->createSourceInfo(yymsp[-9].minor.yy0, yymsp[0].minor.yy50->getSourceInfo());
        } else if (yymsp[-1].minor.yy48 != nullptr) {
            srcInfo = pParser->createSourceInfo(yymsp[-9].minor.yy0, yymsp[-1].minor.yy48->getSourceInfo());
        } else {
            srcInfo = pParser->createSourceInfo(yymsp[-9].minor.yy0, yymsp[-2].minor.yy0);
        }
        yylhsminor.yy73->setSourceInfo(srcInfo);
}
  yymsp[-9].minor.yy73 = yylhsminor.yy73;
        break;
      case 30: /* type_function_args_decl ::= qualifier SELF */
{
        yylhsminor.yy24 = pParser->newAstNode<yal::DeclParamVarContainer>();
        auto qualifier = yal::parser::MakeQualifierFromFlags(yymsp[-1].minor.yy41);
        auto selfVar = pParser->newAstNode<yal::DeclParamVarSelf>(qualifier);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        selfVar->setSourceInfo(srcInfo);
        yylhsminor.yy24->addDeclParam(selfVar);

}
  yymsp[-1].minor.yy24 = yylhsminor.yy24;
        break;
      case 31: /* type_function_args_decl ::= qualifier SELF COMMA type_function_args_decl_other */
{
    yylhsminor.yy24 = yymsp[0].minor.yy24;
    auto qualifier = yal::parser::MakeQualifierFromFlags(yymsp[-3].minor.yy41);
    auto selfVar = pParser->newAstNode<yal::DeclParamVarSelf>(qualifier);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[-2].minor.yy0);
    selfVar->setSourceInfo(srcInfo);
    yylhsminor.yy24->addDeclParam(selfVar);
}
  yymsp[-3].minor.yy24 = yylhsminor.yy24;
        break;
      case 32: /* type_function_args_decl ::= function_args_decl */
{
        yylhsminor.yy24 = yymsp[0].minor.yy24;
}
  yymsp[0].minor.yy24 = yylhsminor.yy24;
        break;
      case 33: /* type_function_args_decl_other ::= type_function_args_decl_other COMMA function_arg_decl */
{
    yylhsminor.yy24 = yymsp[-2].minor.yy24;
    yylhsminor.yy24->addDeclParam(yymsp[0].minor.yy145);
}
  yymsp[-2].minor.yy24 = yylhsminor.yy24;
        break;
      case 34: /* type_function_args_decl_other ::= function_arg_decl */
      case 36: /* function_args_decl ::= function_arg_decl */ yytestcase(yyruleno==36);
{
        yylhsminor.yy24 = pParser->newAstNode<yal::DeclParamVarContainer>();
        yylhsminor.yy24->addDeclParam(yymsp[0].minor.yy145);
}
  yymsp[0].minor.yy24 = yylhsminor.yy24;
        break;
      case 35: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
{
        yymsp[-2].minor.yy24->addDeclParam(yymsp[0].minor.yy145);
        yylhsminor.yy24 = yymsp[-2].minor.yy24;
}
  yymsp[-2].minor.yy24 = yylhsminor.yy24;
        break;
      case 37: /* function_args_decl ::= */
{
        yymsp[1].minor.yy24 = nullptr;
}
        break;
      case 38: /* function_arg_decl ::= IDENTIFIER COLON qualified_type */
{
    yylhsminor.yy145 = pParser->newAstNode<yal::DeclParamVar>(yymsp[-2].minor.yy0.tokenStr, yal::Qualifier(),yymsp[0].minor.yy48);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy48->getSourceInfo());
    yylhsminor.yy145->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy145 = yylhsminor.yy145;
        break;
      case 39: /* function_return_decl ::= COLON qualified_type */
{ yymsp[-1].minor.yy48 = yymsp[0].minor.yy48;
}
        break;
      case 40: /* function_return_decl ::= */
      case 53: /* var_type_spec ::= */ yytestcase(yyruleno==53);
{yymsp[1].minor.yy48 = nullptr;}
        break;
      case 41: /* function_scope ::= SCOPE_BEGIN SCOPE_END */
{ yymsp[-1].minor.yy50 = nullptr;}
        break;
      case 42: /* function_scope ::= SCOPE_BEGIN statement_list SCOPE_END */
{yymsp[-2].minor.yy50 = yymsp[-1].minor.yy50;}
        break;
      case 43: /* statement_list ::= statement_list statement */
{
    yymsp[-1].minor.yy50->addStatement(yymsp[0].minor.yy128);
    yylhsminor.yy50=yymsp[-1].minor.yy50;
}
  yymsp[-1].minor.yy50 = yylhsminor.yy50;
        break;
      case 44: /* statement_list ::= statement */
{
    yylhsminor.yy50 = pParser->newAstNode<yal::StatementList>();
    yylhsminor.yy50->addStatement(yymsp[0].minor.yy128);
}
  yymsp[0].minor.yy50 = yylhsminor.yy50;
        break;
      case 45: /* statement ::= expression ASSIGN expression SEMI_COLON */
{
   yylhsminor.yy128 = pParser->newAstNode<yal::StmtAssign>(yymsp[-3].minor.yy159,yymsp[-1].minor.yy159);
}
  yymsp[-3].minor.yy128 = yylhsminor.yy128;
        break;
      case 46: /* statement ::= var_decl SEMI_COLON */
{
    yylhsminor.yy128 = yymsp[-1].minor.yy13;
}
  yymsp[-1].minor.yy128 = yylhsminor.yy128;
        break;
      case 47: /* statement ::= expression SEMI_COLON */
{yylhsminor.yy128 = yymsp[-1].minor.yy159;}
  yymsp[-1].minor.yy128 = yylhsminor.yy128;
        break;
      case 48: /* statement ::= RETURN expression SEMI_COLON */
{
    yylhsminor.yy128 = pParser->newAstNode<yal::StmtReturn>(yymsp[-1].minor.yy159);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy159->getSourceInfo());
    yylhsminor.yy128->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy128 = yylhsminor.yy128;
        break;
      case 49: /* statement ::= RETURN SEMI_COLON */
{
    yylhsminor.yy128 = pParser->newAstNode<yal::StmtReturn>();
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0,yymsp[-1].minor.yy0);
    yylhsminor.yy128->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy128 = yylhsminor.yy128;
        break;
      case 50: /* var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression */
{
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr, yal::Qualifier(), yymsp[-2].minor.yy48, yymsp[0].minor.yy159);
    auto varSrcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[-3].minor.yy0);
    varDecl->setSourceInfo(varSrcInfo);
    yylhsminor.yy13= pParser->newAstNode<yal::StmtDecl>(varDecl);
    auto srcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy159->getSourceInfo());
    yylhsminor.yy13->setSourceInfo(srcInfo);

}
  yymsp[-4].minor.yy13 = yylhsminor.yy13;
        break;
      case 51: /* var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression */
{
    yal::Qualifier qualifier;
    qualifier.setImmutable();
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr, qualifier, yymsp[-2].minor.yy48, yymsp[0].minor.yy159);
    auto varSrcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[-3].minor.yy0);
    varDecl->setSourceInfo(varSrcInfo);
    yylhsminor.yy13= pParser->newAstNode<yal::StmtDecl>(varDecl);
    auto srcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy159->getSourceInfo());
    yylhsminor.yy13->setSourceInfo(srcInfo);
}
  yymsp[-4].minor.yy13 = yylhsminor.yy13;
        break;
      case 52: /* var_type_spec ::= COLON qualified_type */
{yymsp[-1].minor.yy48 = yymsp[0].minor.yy48;}
        break;
      case 54: /* expression ::= PAR_BEGIN expression PAR_END */
{yymsp[-2].minor.yy159 = yymsp[-1].minor.yy159;}
        break;
      case 55: /* expression ::= literal */
{yylhsminor.yy159 = yymsp[0].minor.yy159;}
  yymsp[0].minor.yy159 = yylhsminor.yy159;
        break;
      case 56: /* expression ::= unaryexp */
{yylhsminor.yy159 = yymsp[0].minor.yy125;}
  yymsp[0].minor.yy159 = yylhsminor.yy159;
        break;
      case 57: /* expression ::= binaryexp */
{yylhsminor.yy159 = yymsp[0].minor.yy71;}
  yymsp[0].minor.yy159 = yylhsminor.yy159;
        break;
      case 58: /* expression ::= IDENTIFIER */
{
    yylhsminor.yy159 = pParser->newAstNode<yal::ExprVarRef>(yymsp[0].minor.yy0.tokenStr);
    auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
    yylhsminor.yy159->setSourceInfo(srcInfo);
}
  yymsp[0].minor.yy159 = yylhsminor.yy159;
        break;
      case 59: /* expression ::= SELF */
{
    yylhsminor.yy159  = pParser->newAstNode<yal::ExprVarRefSelf>();
    auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
    yylhsminor.yy159->setSourceInfo(srcInfo);
}
  yymsp[0].minor.yy159 = yylhsminor.yy159;
        break;
      case 60: /* expression ::= expression DOT IDENTIFIER */
{
    yylhsminor.yy159 = pParser->newAstNode<yal::ExprStructVarRef>(yymsp[-2].minor.yy159, yymsp[0].minor.yy0.tokenStr);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy159->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy159 = yylhsminor.yy159;
        break;
      case 61: /* expression ::= IDENTIFIER PAR_BEGIN function_call_args PAR_END */
{
    auto fnType = pParser->newAstNode<yal::RefTypeUnresolved>(yymsp[-3].minor.yy0.tokenStr);
    auto fnSrcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy0, yymsp[-3].minor.yy0);
    fnType->setSourceInfo(fnSrcInfo);
    if (yymsp[-1].minor.yy56 != nullptr) {
        yymsp[-1].minor.yy56->updateSourceInfo();
    }
    yylhsminor.yy159 = pParser->newAstNode<yal::ExprFnCall>(fnType, yymsp[-1].minor.yy56);
    auto srcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy0, yymsp[0].minor.yy0);
    yylhsminor.yy159->setSourceInfo(srcInfo);
}
  yymsp[-3].minor.yy159 = yylhsminor.yy159;
        break;
      case 62: /* expression ::= expression DOT IDENTIFIER PAR_BEGIN function_call_args PAR_END */
{
    auto fnType = pParser->newAstNode<yal::RefTypeUnresolved>(yymsp[-3].minor.yy0.tokenStr);
    auto fnSrcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy0, yymsp[-3].minor.yy0);
    fnType->setSourceInfo(fnSrcInfo);
    if (yymsp[-1].minor.yy56 != nullptr) {
        yymsp[-1].minor.yy56->updateSourceInfo();
    }
    yylhsminor.yy159 = pParser->newAstNode<yal::ExprStructFnCall>(yymsp[-5].minor.yy159, fnType, yymsp[-1].minor.yy56);
    auto srcInfo = pParser->createSourceInfo(yymsp[-5].minor.yy159->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy159->setSourceInfo(srcInfo);
}
  yymsp[-5].minor.yy159 = yylhsminor.yy159;
        break;
      case 63: /* binaryexp ::= expression AND expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 64: /* binaryexp ::= expression OR expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 65: /* binaryexp ::= expression PLUS expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 66: /* binaryexp ::= expression MINUS expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 67: /* binaryexp ::= expression MULT expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 68: /* binaryexp ::= expression DIV expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 69: /* binaryexp ::= expression MOD expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 70: /* binaryexp ::= expression EQ expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 71: /* binaryexp ::= expression NE expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 72: /* binaryexp ::= expression LE expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 73: /* binaryexp ::= expression LT expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 74: /* binaryexp ::= expression GE expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 75: /* binaryexp ::= expression GT expression */
{
        yylhsminor.yy71 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy159->getSourceInfo(), yymsp[0].minor.yy159->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 76: /* unaryexp ::= NOT expression */
{
    yylhsminor.yy125 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy159);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy159->getSourceInfo());
    yylhsminor.yy125->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy125 = yylhsminor.yy125;
        break;
      case 77: /* unaryexp ::= BIT_NOT expression */
{
    yylhsminor.yy125 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy159);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy159->getSourceInfo());
    yylhsminor.yy125->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy125 = yylhsminor.yy125;
        break;
      case 78: /* unaryexp ::= MINUS expression */
{
    yylhsminor.yy125 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy159);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy159->getSourceInfo());
    yylhsminor.yy125->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy125 = yylhsminor.yy125;
        break;
      case 79: /* function_call_args ::= function_call_args COMMA expression */
{
    yylhsminor.yy56 = yymsp[-2].minor.yy56;
    yylhsminor.yy56->addExpression(yymsp[0].minor.yy159);
}
  yymsp[-2].minor.yy56 = yylhsminor.yy56;
        break;
      case 80: /* function_call_args ::= expression */
{
    yylhsminor.yy56 = pParser->newAstNode<yal::ExprList>();
    yylhsminor.yy56->addExpression(yymsp[0].minor.yy159);
}
  yymsp[0].minor.yy56 = yylhsminor.yy56;
        break;
      case 81: /* function_call_args ::= */
{
    yymsp[1].minor.yy56= nullptr;
}
        break;
      case 82: /* literal ::= INTEGER_LITERAL */
{
        yylhsminor.yy159 = pParser->newIntegerLiteral(yymsp[0].minor.yy0);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy159->setSourceInfo(srcInfo);
   }
  yymsp[0].minor.yy159 = yylhsminor.yy159;
        break;
      case 83: /* literal ::= DECIMAL_LITERAL */
{
        yylhsminor.yy159 = pParser->newDecimalLiteral(yymsp[0].minor.yy0);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy159->setSourceInfo(srcInfo);
}
  yymsp[0].minor.yy159 = yylhsminor.yy159;
        break;
      case 84: /* literal ::= BOOL_LITERAL */
{
        yylhsminor.yy159 = pParser->newAstNode<yal::ExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy159->setSourceInfo(srcInfo);
}
  yymsp[0].minor.yy159 = yylhsminor.yy159;
        break;
      default:
      /* (85) module ::= decls END */ yytestcase(yyruleno==85);
      /* (86) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==86);
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
