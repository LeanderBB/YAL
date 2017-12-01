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
#define YAL_TOKEN_COMMA                          48
#define YAL_TOKEN_RETURN                         49
#define YAL_TOKEN_VAR                            50
#define YAL_TOKEN_LET                            51
#define YAL_TOKEN_INTEGER_LITERAL                52
#define YAL_TOKEN_DECIMAL_LITERAL                53
#define YAL_TOKEN_BOOL_LITERAL                   54
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
#define YYNOCODE 84
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALParserTOKENTYPE yal::TokenInfo
#endif
typedef union {
  int yyinit;
  YALParserTOKENTYPE yy0;
  yal::Statement* yy40;
  yal::ExprBinaryOperator* yy43;
  yal::DeclParamVar* yy59;
  yal::DeclStruct* yy64;
  yal::DeclVar* yy68;
  yal::ExprList* yy72;
  yal::RefTypeResolved* yy76;
  yal::DeclTypeFunction* yy89;
  yal::DeclStructMembers* yy90;
  yal::DeclModule* yy103;
  yal::StatementList* yy106;
  yal::StringRefPod yy114;
  yal::RefType* yy140;
  yal::DeclParamVarContainer* yy141;
  yal::StmtExpression* yy155;
  yal::ExprUnaryOperator* yy157;
  yal::StmtDecl* yy163;
  uint32_t yy164;
  yal::DeclFunction* yy165;
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
#define YYNSTATE             100
#define YYNRULE              81
#define YY_MAX_SHIFT         99
#define YY_MIN_SHIFTREDUCE   150
#define YY_MAX_SHIFTREDUCE   230
#define YY_MIN_REDUCE        231
#define YY_MAX_REDUCE        311
#define YY_ERROR_ACTION      312
#define YY_ACCEPT_ACTION     313
#define YY_NO_ACTION         314
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
#define YY_ACTTAB_COUNT (435)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     8,   24,   25,  232,  233,   75,   18,   17,   13,   15,
 /*    10 */    14,   16,   23,   22,   21,   20,   19,   24,   25,  157,
 /*    20 */    91,  231,   18,   17,   13,   15,   14,   16,   23,   22,
 /*    30 */    21,   20,   19,   31,  313,   93,   91,   67,  262,   24,
 /*    40 */    25,   68,  262,  192,   18,   17,   13,   15,   14,   16,
 /*    50 */    23,   22,   21,   20,   19,   24,   25,  256,   91,  193,
 /*    60 */    18,   17,   13,   15,   14,   16,   23,   22,   21,   20,
 /*    70 */    19,   30,    1,  264,   91,   41,   42,   24,   25,  257,
 /*    80 */    39,  190,   18,   17,   13,   15,   14,   16,   23,   22,
 /*    90 */    21,   20,   19,   21,   20,   19,   91,   46,   42,   91,
 /*   100 */   199,  206,    9,   24,   25,   30,  260,  265,   18,   17,
 /*   110 */    13,   15,   14,   16,   23,   22,   21,   20,   19,  177,
 /*   120 */   156,   34,   91,  162,  163,  164,  165,  166,  167,  168,
 /*   130 */   169,  170,  171,  172,   99,   25,  205,    9,   94,   18,
 /*   140 */    17,   13,   15,   14,   16,   23,   22,   21,   20,   19,
 /*   150 */    10,   98,  261,   91,   35,   11,   12,   29,   30,   89,
 /*   160 */   278,   40,   10,   49,  282,  283,  281,   11,   12,   76,
 /*   170 */    30,   89,   92,  269,   48,  282,  283,  281,   80,  186,
 /*   180 */    78,  259,   26,   45,   93,    5,   79,   77,  226,  230,
 /*   190 */   227,  187,  159,   32,   26,  242,   71,    5,   79,   77,
 /*   200 */   226,  230,  227,   81,  173,   72,  162,  163,  164,  165,
 /*   210 */   166,  167,  168,  169,  170,  171,  172,   73,  156,   33,
 /*   220 */     6,  162,  163,  164,  165,  166,  167,  168,  169,  170,
 /*   230 */   171,  172,   18,   17,   13,   15,   14,   16,   23,   22,
 /*   240 */    21,   20,   19,   43,    7,   44,   91,  312,  312,  312,
 /*   250 */   312,  312,  312,   23,   22,   21,   20,   19,  191,  174,
 /*   260 */    91,   91,   10,   96,   96,   27,   38,   11,   12,   82,
 /*   270 */    74,   89,   10,   53,  282,  283,  281,   11,   12,   36,
 /*   280 */     2,   89,  270,   48,  282,  283,  281,   80,    3,  236,
 /*   290 */   236,    4,  194,   90,   26,   37,   54,  282,  283,  281,
 /*   300 */   226,  230,  227,  229,   26,   56,  282,  283,  281,   28,
 /*   310 */   226,  230,  227,   70,   95,   97,  309,   56,  282,  283,
 /*   320 */   281,   50,  282,  283,  281,   69,   55,  282,  283,  281,
 /*   330 */   233,  233,   83,  282,  283,  281,  233,  233,   84,  282,
 /*   340 */   283,  281,   85,  282,  283,  281,   59,  282,  283,  281,
 /*   350 */   233,   60,  282,  283,  281,   61,  282,  283,  281,  233,
 /*   360 */   233,  233,  233,   62,  282,  283,  281,   63,  282,  283,
 /*   370 */   281,   64,  282,  283,  281,   86,  282,  283,  281,   87,
 /*   380 */   282,  283,  281,   88,  282,  283,  281,  233,   65,  282,
 /*   390 */   283,  281,   66,  282,  283,  281,   57,  282,  283,  281,
 /*   400 */    58,  282,  283,  281,   51,  282,  283,  281,   96,   52,
 /*   410 */   282,  283,  281,  233,   47,  233,  233,  233,  233,  233,
 /*   420 */   233,  233,  233,  233,  233,  233,  233,  233,  233,  233,
 /*   430 */   233,  233,  233,  233,  236,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     1,    2,    3,   57,   58,   23,    7,    8,    9,   10,
 /*    10 */    11,   12,   13,   14,   15,   16,   17,    2,    3,   24,
 /*    20 */    21,   75,    7,    8,    9,   10,   11,   12,   13,   14,
 /*    30 */    15,   16,   17,   59,   60,   23,   21,   65,   66,    2,
 /*    40 */     3,   65,   66,   44,    7,    8,    9,   10,   11,   12,
 /*    50 */    13,   14,   15,   16,   17,    2,    3,   76,   21,   44,
 /*    60 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*    70 */    17,   79,   42,   81,   21,   47,   48,    2,    3,   76,
 /*    80 */    77,   44,    7,    8,    9,   10,   11,   12,   13,   14,
 /*    90 */    15,   16,   17,   15,   16,   17,   21,   47,   48,   21,
 /*   100 */    47,   47,   48,    2,    3,   79,   68,   81,    7,    8,
 /*   110 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   44,
 /*   120 */    23,   40,   21,   26,   27,   28,   29,   30,   31,   32,
 /*   130 */    33,   34,   35,   36,   22,    3,   47,   48,   41,    7,
 /*   140 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   150 */    14,   39,   66,   21,   40,   19,   20,   45,   79,   23,
 /*   160 */    81,   61,   14,   70,   71,   72,   73,   19,   20,   63,
 /*   170 */    79,   23,   81,   69,   70,   71,   72,   73,   74,   43,
 /*   180 */    63,   68,   46,   61,   23,   49,   50,   51,   52,   53,
 /*   190 */    54,   43,   25,   46,   46,   80,   23,   49,   50,   51,
 /*   200 */    52,   53,   54,   23,   43,   40,   26,   27,   28,   29,
 /*   210 */    30,   31,   32,   33,   34,   35,   36,   40,   23,   40,
 /*   220 */     1,   26,   27,   28,   29,   30,   31,   32,   33,   34,
 /*   230 */    35,   36,    7,    8,    9,   10,   11,   12,   13,   14,
 /*   240 */    15,   16,   17,   23,    1,   23,   21,    7,    8,    9,
 /*   250 */    10,   11,   12,   13,   14,   15,   16,   17,   44,   44,
 /*   260 */    21,   21,   14,   56,   56,    1,   42,   19,   20,   62,
 /*   270 */    62,   23,   14,   70,   71,   72,   73,   19,   20,   46,
 /*   280 */    67,   23,   69,   70,   71,   72,   73,   74,   46,   82,
 /*   290 */    82,   46,   44,   23,   46,   40,   70,   71,   72,   73,
 /*   300 */    52,   53,   54,   38,   46,   70,   71,   72,   73,   40,
 /*   310 */    52,   53,   54,   78,   37,   23,    0,   70,   71,   72,
 /*   320 */    73,   70,   71,   72,   73,   78,   70,   71,   72,   73,
 /*   330 */    83,   83,   70,   71,   72,   73,   83,   83,   70,   71,
 /*   340 */    72,   73,   70,   71,   72,   73,   70,   71,   72,   73,
 /*   350 */    83,   70,   71,   72,   73,   70,   71,   72,   73,   83,
 /*   360 */    83,   83,   83,   70,   71,   72,   73,   70,   71,   72,
 /*   370 */    73,   70,   71,   72,   73,   70,   71,   72,   73,   70,
 /*   380 */    71,   72,   73,   70,   71,   72,   73,   83,   70,   71,
 /*   390 */    72,   73,   70,   71,   72,   73,   70,   71,   72,   73,
 /*   400 */    70,   71,   72,   73,   70,   71,   72,   73,   56,   70,
 /*   410 */    71,   72,   73,   83,   62,   83,   83,   83,   83,   83,
 /*   420 */    83,   83,   83,   83,   83,   83,   83,   83,   83,   83,
 /*   430 */    83,   83,   83,   83,   82,
};
#define YY_SHIFT_USE_DFLT (435)
#define YY_SHIFT_COUNT    (99)
#define YY_SHIFT_MIN      (-18)
#define YY_SHIFT_MAX      (316)
static const short yy_shift_ofst[] = {
 /*     0 */   435,  136,  148,  258,  258,  248,  258,  258,  258,  258,
 /*    10 */   258,  258,  258,  258,  258,  258,  258,  258,  258,  258,
 /*    20 */   258,  258,  258,  258,  258,  258,  258,  258,   97,  180,
 /*    30 */   195,  112,  -18,   -5,   -5,   -5,  -18,   -5,   12,  161,
 /*    40 */    30,   81,  -18,  114,  114,   30,   81,  167,   -1,   15,
 /*    50 */    37,   53,   75,  101,  101,  101,  101,  132,  225,  240,
 /*    60 */   240,  240,  240,  240,  240,   78,   78,   28,   50,   54,
 /*    70 */    89,  147,  173,  165,  177,  179,  219,  220,  243,  222,
 /*    80 */   214,  233,  215,  239,  239,  239,  239,  239,  239,  242,
 /*    90 */   245,  270,  264,  255,  224,  265,  277,  269,  292,  316,
};
#define YY_REDUCE_USE_DFLT (-55)
#define YY_REDUCE_COUNT (47)
#define YY_REDUCE_MIN   (-54)
#define YY_REDUCE_MAX   (352)
static const short yy_reduce_ofst[] = {
 /*     0 */   -26,  213,  104,  235,  247,   93,  203,  226,  251,  256,
 /*    10 */   262,  268,  272,  276,  281,  285,  293,  297,  301,  305,
 /*    20 */   309,  313,  318,  322,  326,  330,  334,  339,  207,  208,
 /*    30 */   352,  -54,  -28,   -8,   26,   79,  -24,   91,    3,  -19,
 /*    40 */    38,  100,   86,  106,  117,  113,  122,  115,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   234,  312,  312,  306,  306,  312,  312,  312,  312,  312,
 /*    10 */   312,  312,  312,  312,  312,  312,  312,  312,  312,  312,
 /*    20 */   312,  312,  312,  312,  312,  312,  312,  312,  312,  312,
 /*    30 */   312,  312,  263,  239,  239,  239,  263,  239,  312,  312,
 /*    40 */   312,  266,  312,  279,  279,  312,  266,  241,  312,  312,
 /*    50 */   312,  312,  312,  277,  276,  304,  305,  289,  288,  300,
 /*    60 */   299,  298,  297,  296,  295,  291,  290,  312,  312,  312,
 /*    70 */   312,  312,  312,  312,  312,  312,  312,  312,  312,  312,
 /*    80 */   312,  237,  312,  303,  302,  301,  294,  293,  292,  284,
 /*    90 */   285,  312,  312,  312,  312,  312,  235,  312,  312,  312,
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
  "COMMA",         "RETURN",        "VAR",           "LET",         
  "INTEGER_LITERAL",  "DECIMAL_LITERAL",  "BOOL_LITERAL",  "error",       
  "type_builtin",  "decl_function",  "type_function_decl",  "decls",       
  "module",        "function_return_decl",  "type_specifier",  "var_type_spec",
  "named_decl",    "function_args_decl",  "function_arg_decl",  "statement_list",
  "function_scope",  "statement",     "expression",    "unaryexp",    
  "binaryexp",     "literal",       "var_decl",      "type_decl",   
  "type_var_decl",  "type_var_decls",  "function_call_args",  "pre_qualifier",
  "post_qualifier",  "qualified_type",  "type_array",  
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
 /*   7 */ "pre_qualifier ::= MUT",
 /*   8 */ "pre_qualifier ::=",
 /*   9 */ "post_qualifier ::= REFERENCE",
 /*  10 */ "post_qualifier ::=",
 /*  11 */ "qualified_type ::= pre_qualifier type_specifier post_qualifier",
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
 /*  29 */ "type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope",
 /*  30 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  31 */ "function_args_decl ::= function_arg_decl",
 /*  32 */ "function_args_decl ::=",
 /*  33 */ "function_arg_decl ::= IDENTIFIER COLON qualified_type",
 /*  34 */ "function_return_decl ::= COLON qualified_type",
 /*  35 */ "function_return_decl ::=",
 /*  36 */ "function_scope ::= SCOPE_BEGIN SCOPE_END",
 /*  37 */ "function_scope ::= SCOPE_BEGIN statement_list SCOPE_END",
 /*  38 */ "statement_list ::= statement_list statement",
 /*  39 */ "statement_list ::= statement",
 /*  40 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  41 */ "statement ::= var_decl SEMI_COLON",
 /*  42 */ "statement ::= expression SEMI_COLON",
 /*  43 */ "statement ::= RETURN expression SEMI_COLON",
 /*  44 */ "statement ::= RETURN SEMI_COLON",
 /*  45 */ "var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression",
 /*  46 */ "var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression",
 /*  47 */ "var_type_spec ::= COLON qualified_type",
 /*  48 */ "var_type_spec ::=",
 /*  49 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  50 */ "expression ::= literal",
 /*  51 */ "expression ::= unaryexp",
 /*  52 */ "expression ::= binaryexp",
 /*  53 */ "expression ::= IDENTIFIER",
 /*  54 */ "expression ::= expression DOT IDENTIFIER",
 /*  55 */ "expression ::= IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  56 */ "expression ::= expression DOT IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  57 */ "binaryexp ::= expression AND expression",
 /*  58 */ "binaryexp ::= expression OR expression",
 /*  59 */ "binaryexp ::= expression PLUS expression",
 /*  60 */ "binaryexp ::= expression MINUS expression",
 /*  61 */ "binaryexp ::= expression MULT expression",
 /*  62 */ "binaryexp ::= expression DIV expression",
 /*  63 */ "binaryexp ::= expression MOD expression",
 /*  64 */ "binaryexp ::= expression EQ expression",
 /*  65 */ "binaryexp ::= expression NE expression",
 /*  66 */ "binaryexp ::= expression LE expression",
 /*  67 */ "binaryexp ::= expression LT expression",
 /*  68 */ "binaryexp ::= expression GE expression",
 /*  69 */ "binaryexp ::= expression GT expression",
 /*  70 */ "unaryexp ::= NOT expression",
 /*  71 */ "unaryexp ::= BIT_NOT expression",
 /*  72 */ "unaryexp ::= MINUS expression",
 /*  73 */ "function_call_args ::= function_call_args COMMA expression",
 /*  74 */ "function_call_args ::= expression",
 /*  75 */ "function_call_args ::=",
 /*  76 */ "literal ::= INTEGER_LITERAL",
 /*  77 */ "literal ::= BOOL_LITERAL",
 /*  78 */ "module ::= decls END",
 /*  79 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
 /*  80 */ "literal ::= DECIMAL_LITERAL",
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
  { 59, -2 },
  { 59, -2 },
  { 59, -2 },
  { 59, 0 },
  { 62, -1 },
  { 62, -1 },
  { 62, -1 },
  { 79, -1 },
  { 79, 0 },
  { 80, -1 },
  { 80, 0 },
  { 81, -3 },
  { 56, -1 },
  { 56, -1 },
  { 56, -1 },
  { 56, -1 },
  { 56, -1 },
  { 56, -1 },
  { 56, -1 },
  { 56, -1 },
  { 56, -1 },
  { 56, -1 },
  { 56, -1 },
  { 75, -7 },
  { 75, -5 },
  { 77, -2 },
  { 77, -1 },
  { 76, -6 },
  { 57, -7 },
  { 58, -10 },
  { 65, -3 },
  { 65, -1 },
  { 65, 0 },
  { 66, -3 },
  { 61, -2 },
  { 61, 0 },
  { 68, -2 },
  { 68, -3 },
  { 67, -2 },
  { 67, -1 },
  { 69, -4 },
  { 69, -2 },
  { 69, -2 },
  { 69, -3 },
  { 69, -2 },
  { 74, -5 },
  { 74, -5 },
  { 63, -2 },
  { 63, 0 },
  { 70, -3 },
  { 70, -1 },
  { 70, -1 },
  { 70, -1 },
  { 70, -1 },
  { 70, -3 },
  { 70, -4 },
  { 70, -6 },
  { 72, -3 },
  { 72, -3 },
  { 72, -3 },
  { 72, -3 },
  { 72, -3 },
  { 72, -3 },
  { 72, -3 },
  { 72, -3 },
  { 72, -3 },
  { 72, -3 },
  { 72, -3 },
  { 72, -3 },
  { 72, -3 },
  { 71, -2 },
  { 71, -2 },
  { 71, -2 },
  { 78, -3 },
  { 78, -1 },
  { 78, 0 },
  { 73, -1 },
  { 73, -1 },
  { 60, -2 },
  { 82, -3 },
  { 73, -1 },
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
    yylhsminor.yy103 =yymsp[-1].minor.yy103; yymsp[-1].minor.yy103->addDecl(yymsp[0].minor.yy64);
}
  yymsp[-1].minor.yy103 = yylhsminor.yy103;
        break;
      case 1: /* decls ::= decls decl_function */
{
        yylhsminor.yy103 =yymsp[-1].minor.yy103; yymsp[-1].minor.yy103->addDecl(yymsp[0].minor.yy165);
}
  yymsp[-1].minor.yy103 = yylhsminor.yy103;
        break;
      case 2: /* decls ::= decls type_function_decl */
{
        yylhsminor.yy103 =yymsp[-1].minor.yy103; yymsp[-1].minor.yy103->addDecl(yymsp[0].minor.yy89);
}
  yymsp[-1].minor.yy103 = yylhsminor.yy103;
        break;
      case 3: /* decls ::= */
{
        yymsp[1].minor.yy103 =  pParser->newAstNode<yal::DeclModule>();
        pParser->onAstNodeCreate(yymsp[1].minor.yy103);
     }
        break;
      case 4: /* type_specifier ::= type_builtin */
{yylhsminor.yy140=yymsp[0].minor.yy76;}
  yymsp[0].minor.yy140 = yylhsminor.yy140;
        break;
      case 5: /* type_specifier ::= type_array */
{yymsp[0].minor.yy140 = nullptr;}
        break;
      case 6: /* type_specifier ::= IDENTIFIER */
{
        yylhsminor.yy140 = pParser->newAstNode<yal::RefTypeUnresolved>(yymsp[0].minor.yy0.tokenStr);
 }
  yymsp[0].minor.yy140 = yylhsminor.yy140;
        break;
      case 7: /* pre_qualifier ::= MUT */
{yymsp[0].minor.yy164 = yal::parser::kQualifierMutable;}
        break;
      case 8: /* pre_qualifier ::= */
      case 10: /* post_qualifier ::= */ yytestcase(yyruleno==10);
{yymsp[1].minor.yy164 = 0;}
        break;
      case 9: /* post_qualifier ::= REFERENCE */
{yymsp[0].minor.yy164 = yal::parser::kQualifierReference;}
        break;
      case 11: /* qualified_type ::= pre_qualifier type_specifier post_qualifier */
{
        const uint32_t qualifierFlags = yymsp[-2].minor.yy164 | yymsp[0].minor.yy164;
        yymsp[-1].minor.yy140->setQualifier(yal::parser::MakeQualifierFromFlags(qualifierFlags));
        yylhsminor.yy140 = yymsp[-1].minor.yy140;
}
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 12: /* type_builtin ::= TYPE_BOOL */
{
        yymsp[0].minor.yy76 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinBool());
        }
        break;
      case 13: /* type_builtin ::= TYPE_INT8 */
{
        yymsp[0].minor.yy76 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinI8());
        }
        break;
      case 14: /* type_builtin ::= TYPE_UINT8 */
{
        yymsp[0].minor.yy76  = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinU8());
        }
        break;
      case 15: /* type_builtin ::= TYPE_INT16 */
{
        yymsp[0].minor.yy76 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinI16());
        }
        break;
      case 16: /* type_builtin ::= TYPE_UINT16 */
{
        yymsp[0].minor.yy76  = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinU16());
        }
        break;
      case 17: /* type_builtin ::= TYPE_INT32 */
{
        yymsp[0].minor.yy76 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinI32());
        }
        break;
      case 18: /* type_builtin ::= TYPE_UINT32 */
{
        yymsp[0].minor.yy76 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinU32());
        }
        break;
      case 19: /* type_builtin ::= TYPE_INT64 */
{
        yymsp[0].minor.yy76 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinI64());
        }
        break;
      case 20: /* type_builtin ::= TYPE_UINT64 */
{
        yymsp[0].minor.yy76 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinU64());
        }
        break;
      case 21: /* type_builtin ::= TYPE_FLOAT */
{
        yymsp[0].minor.yy76 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinFloat());
        }
        break;
      case 22: /* type_builtin ::= TYPE_DOUBLE */
{
        yymsp[0].minor.yy76 = pParser->newAstNode<yal::RefTypeResolved>(pParser->getModule().getTypeContext().getTypeBuiltinDouble());
        }
        break;
      case 23: /* type_decl ::= TYPE IDENTIFIER COLON STRUCT SCOPE_BEGIN type_var_decls SCOPE_END */
{
     yymsp[-6].minor.yy64 = pParser->newAstNode<yal::DeclStruct>(yymsp[-5].minor.yy0.tokenStr, yymsp[-1].minor.yy90);
}
        break;
      case 24: /* type_decl ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON */
{
    yymsp[-4].minor.yy64 = nullptr;
}
        break;
      case 25: /* type_var_decls ::= type_var_decls type_var_decl */
{
    yylhsminor.yy90 = yymsp[-1].minor.yy90;
    yylhsminor.yy90->addDeclVar(yymsp[0].minor.yy68);
}
  yymsp[-1].minor.yy90 = yylhsminor.yy90;
        break;
      case 26: /* type_var_decls ::= type_var_decl */
{
    yylhsminor.yy90 = pParser->newAstNode<yal::DeclStructMembers>();
    yylhsminor.yy90->addDeclVar(yymsp[0].minor.yy68);
}
  yymsp[0].minor.yy90 = yylhsminor.yy90;
        break;
      case 27: /* type_var_decl ::= IDENTIFIER COLON qualified_type ASSIGN expression SEMI_COLON */
{
    yylhsminor.yy68 = pParser->newAstNode<yal::DeclVar>(yymsp[-5].minor.yy0.tokenStr, yal::Qualifier(), yymsp[-3].minor.yy140, yymsp[-1].minor.yy155);
}
  yymsp[-5].minor.yy68 = yylhsminor.yy68;
        break;
      case 28: /* decl_function ::= FUNCTION IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope */
{
        yymsp[-6].minor.yy165 = pParser->newAstNode<yal::DeclFunction>(yymsp[-5].minor.yy0.tokenStr, yymsp[-3].minor.yy141, yymsp[-1].minor.yy140, yymsp[0].minor.yy106);
        }
        break;
      case 29: /* type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope */
{
            yymsp[-9].minor.yy89 = pParser->newAstNode<yal::DeclTypeFunction>(yymsp[-5].minor.yy0.tokenStr, yymsp[-3].minor.yy141, yymsp[-8].minor.yy140, yymsp[-1].minor.yy140, yymsp[0].minor.yy106);
        }
        break;
      case 30: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
{
        yymsp[-2].minor.yy141->addDeclParam(yymsp[0].minor.yy59);
        yylhsminor.yy141 = yymsp[-2].minor.yy141;
}
  yymsp[-2].minor.yy141 = yylhsminor.yy141;
        break;
      case 31: /* function_args_decl ::= function_arg_decl */
{
        yylhsminor.yy141 = pParser->newAstNode<yal::DeclParamVarContainer>();
        yylhsminor.yy141->addDeclParam(yymsp[0].minor.yy59);
}
  yymsp[0].minor.yy141 = yylhsminor.yy141;
        break;
      case 32: /* function_args_decl ::= */
{
        yymsp[1].minor.yy141 = nullptr;
}
        break;
      case 33: /* function_arg_decl ::= IDENTIFIER COLON qualified_type */
{
    yylhsminor.yy59 = pParser->newAstNode<yal::DeclParamVar>(yymsp[-2].minor.yy0.tokenStr, yal::Qualifier(),yymsp[0].minor.yy140);
}
  yymsp[-2].minor.yy59 = yylhsminor.yy59;
        break;
      case 34: /* function_return_decl ::= COLON qualified_type */
{ yymsp[-1].minor.yy140 = yymsp[0].minor.yy140;}
        break;
      case 35: /* function_return_decl ::= */
      case 48: /* var_type_spec ::= */ yytestcase(yyruleno==48);
{yymsp[1].minor.yy140 = nullptr;}
        break;
      case 36: /* function_scope ::= SCOPE_BEGIN SCOPE_END */
{ yymsp[-1].minor.yy106 = nullptr;}
        break;
      case 37: /* function_scope ::= SCOPE_BEGIN statement_list SCOPE_END */
{yymsp[-2].minor.yy106 = yymsp[-1].minor.yy106;}
        break;
      case 38: /* statement_list ::= statement_list statement */
{
    yymsp[-1].minor.yy106->addStatement(yymsp[0].minor.yy40);
    yylhsminor.yy106=yymsp[-1].minor.yy106;
}
  yymsp[-1].minor.yy106 = yylhsminor.yy106;
        break;
      case 39: /* statement_list ::= statement */
{
    yylhsminor.yy106 = pParser->newAstNode<yal::StatementList>();
    yylhsminor.yy106->addStatement(yymsp[0].minor.yy40);
}
  yymsp[0].minor.yy106 = yylhsminor.yy106;
        break;
      case 40: /* statement ::= expression ASSIGN expression SEMI_COLON */
{
   yylhsminor.yy40 = pParser->newAstNode<yal::StmtAssign>(yymsp[-3].minor.yy155,yymsp[-1].minor.yy155);
}
  yymsp[-3].minor.yy40 = yylhsminor.yy40;
        break;
      case 41: /* statement ::= var_decl SEMI_COLON */
{
    yylhsminor.yy40 = yymsp[-1].minor.yy163;
}
  yymsp[-1].minor.yy40 = yylhsminor.yy40;
        break;
      case 42: /* statement ::= expression SEMI_COLON */
{yylhsminor.yy40 = yymsp[-1].minor.yy155;}
  yymsp[-1].minor.yy40 = yylhsminor.yy40;
        break;
      case 43: /* statement ::= RETURN expression SEMI_COLON */
{
    yymsp[-2].minor.yy40 = pParser->newAstNode<yal::StmtReturn>(yymsp[-1].minor.yy155);
}
        break;
      case 44: /* statement ::= RETURN SEMI_COLON */
{
    yymsp[-1].minor.yy40 = pParser->newAstNode<yal::StmtReturn>();
}
        break;
      case 45: /* var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression */
{
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr, yal::Qualifier(), yymsp[-2].minor.yy140, yymsp[0].minor.yy155);
    yymsp[-4].minor.yy163= pParser->newAstNode<yal::StmtDecl>(varDecl);
}
        break;
      case 46: /* var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression */
{
    yal::Qualifier qualifier;
    qualifier.setImmutable();
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr, qualifier, yymsp[-2].minor.yy140, yymsp[0].minor.yy155);
    yymsp[-4].minor.yy163= pParser->newAstNode<yal::StmtDecl>(varDecl);
}
        break;
      case 47: /* var_type_spec ::= COLON qualified_type */
{yymsp[-1].minor.yy140 = yymsp[0].minor.yy140;}
        break;
      case 49: /* expression ::= PAR_BEGIN expression PAR_END */
{yymsp[-2].minor.yy155 = yymsp[-1].minor.yy155;}
        break;
      case 50: /* expression ::= literal */
{yylhsminor.yy155 = yymsp[0].minor.yy155;}
  yymsp[0].minor.yy155 = yylhsminor.yy155;
        break;
      case 51: /* expression ::= unaryexp */
{yylhsminor.yy155 = yymsp[0].minor.yy157;}
  yymsp[0].minor.yy155 = yylhsminor.yy155;
        break;
      case 52: /* expression ::= binaryexp */
{yylhsminor.yy155 = yymsp[0].minor.yy43;}
  yymsp[0].minor.yy155 = yylhsminor.yy155;
        break;
      case 53: /* expression ::= IDENTIFIER */
{
    yylhsminor.yy155 = pParser->newAstNode<yal::ExprVarRef>(yymsp[0].minor.yy0.tokenStr);
}
  yymsp[0].minor.yy155 = yylhsminor.yy155;
        break;
      case 54: /* expression ::= expression DOT IDENTIFIER */
{
    yylhsminor.yy155 = pParser->newAstNode<yal::ExprStructVarRef>(yymsp[-2].minor.yy155, yymsp[0].minor.yy0.tokenStr);
}
  yymsp[-2].minor.yy155 = yylhsminor.yy155;
        break;
      case 55: /* expression ::= IDENTIFIER PAR_BEGIN function_call_args PAR_END */
{
    auto fnType = pParser->newAstNode<yal::RefTypeUnresolved>(yymsp[-3].minor.yy0.tokenStr);
    yylhsminor.yy155 = pParser->newAstNode<yal::ExprFnCall>(fnType, yymsp[-1].minor.yy72);
}
  yymsp[-3].minor.yy155 = yylhsminor.yy155;
        break;
      case 56: /* expression ::= expression DOT IDENTIFIER PAR_BEGIN function_call_args PAR_END */
{
    auto fnType = pParser->newAstNode<yal::RefTypeUnresolved>(yymsp[-3].minor.yy0.tokenStr);
    yylhsminor.yy155 = pParser->newAstNode<yal::ExprStructFnCall>(yymsp[-5].minor.yy155, fnType, yymsp[-1].minor.yy72);
}
  yymsp[-5].minor.yy155 = yylhsminor.yy155;
        break;
      case 57: /* binaryexp ::= expression AND expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 58: /* binaryexp ::= expression OR expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 59: /* binaryexp ::= expression PLUS expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 60: /* binaryexp ::= expression MINUS expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 61: /* binaryexp ::= expression MULT expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 62: /* binaryexp ::= expression DIV expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 63: /* binaryexp ::= expression MOD expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 64: /* binaryexp ::= expression EQ expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 65: /* binaryexp ::= expression NE expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 66: /* binaryexp ::= expression LE expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 67: /* binaryexp ::= expression LT expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 68: /* binaryexp ::= expression GE expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 69: /* binaryexp ::= expression GT expression */
{
        yylhsminor.yy43 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy155, yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy43 = yylhsminor.yy43;
        break;
      case 70: /* unaryexp ::= NOT expression */
{
    yymsp[-1].minor.yy157 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy155);
}
        break;
      case 71: /* unaryexp ::= BIT_NOT expression */
{
    yymsp[-1].minor.yy157 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy155);
}
        break;
      case 72: /* unaryexp ::= MINUS expression */
{
    yymsp[-1].minor.yy157 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy155);
}
        break;
      case 73: /* function_call_args ::= function_call_args COMMA expression */
{
    yylhsminor.yy72 = yymsp[-2].minor.yy72;
    yylhsminor.yy72->addExpression(yymsp[0].minor.yy155);
}
  yymsp[-2].minor.yy72 = yylhsminor.yy72;
        break;
      case 74: /* function_call_args ::= expression */
{
    yylhsminor.yy72 = pParser->newAstNode<yal::ExprList>();
    yylhsminor.yy72->addExpression(yymsp[0].minor.yy155);
}
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 75: /* function_call_args ::= */
{
    yymsp[1].minor.yy72= nullptr;
}
        break;
      case 76: /* literal ::= INTEGER_LITERAL */
{
        yylhsminor.yy155 = pParser->newIntegerLiteral(yymsp[0].minor.yy0);
   }
  yymsp[0].minor.yy155 = yylhsminor.yy155;
        break;
      case 77: /* literal ::= BOOL_LITERAL */
{
        yylhsminor.yy155 = pParser->newAstNode<yal::ExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
}
  yymsp[0].minor.yy155 = yylhsminor.yy155;
        break;
      default:
      /* (78) module ::= decls END */ yytestcase(yyruleno==78);
      /* (79) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==79);
      /* (80) literal ::= DECIMAL_LITERAL */ yytestcase(yyruleno==80);
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
