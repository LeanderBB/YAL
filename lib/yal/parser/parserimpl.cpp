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
#include "yal/ast/exprtypefncall.h"
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
#define YYNOCODE 94
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALParserTOKENTYPE yal::TokenInfo
#endif
typedef union {
  int yyinit;
  YALParserTOKENTYPE yy0;
  yal::StmtExpression* yy7;
  yal::DeclParamVar* yy71;
  yal::DeclParamVarContainer* yy74;
  yal::DeclStruct* yy78;
  uint32_t yy81;
  yal::ExprUnaryOperator* yy85;
  yal::StmtDecl* yy93;
  yal::StringRefPod yy94;
  yal::Statement* yy104;
  yal::ExprList* yy128;
  yal::DeclVar* yy138;
  yal::DeclStructMembers* yy149;
  yal::ExprBinaryOperator* yy153;
  yal::DeclTypeFunction* yy165;
  yal::StatementList* yy170;
  yal::DeclBase* yy174;
  yal::RefType* yy180;
  yal::DeclFunction* yy183;
  yal::DeclModule* yy184;
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
#define YYNSTATE             110
#define YYNRULE              95
#define YY_MAX_SHIFT         109
#define YY_MIN_SHIFTREDUCE   168
#define YY_MAX_SHIFTREDUCE   262
#define YY_MIN_REDUCE        263
#define YY_MAX_REDUCE        357
#define YY_ERROR_ACTION      358
#define YY_ACCEPT_ACTION     359
#define YY_NO_ACTION         360
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
#define YY_ACTTAB_COUNT (433)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    10,   26,   27,    1,  359,   90,   20,   19,   15,   17,
 /*    10 */    16,   18,   25,   24,   23,   22,   21,   26,   27,  200,
 /*    20 */    99,   47,   20,   19,   15,   17,   16,   18,   25,   24,
 /*    30 */    23,   22,   21,  101,  177,  102,   99,   33,  323,  174,
 /*    40 */   223,  290,  180,  181,  182,  183,  184,  185,  186,  187,
 /*    50 */   188,  189,  190,   33,  309,  304,  224,  106,  193,   26,
 /*    60 */    27,   33,  310,  104,   20,   19,   15,   17,   16,   18,
 /*    70 */    25,   24,   23,   22,   21,   26,   27,  238,   99,   11,
 /*    80 */    20,   19,   15,   17,   16,   18,   25,   24,   23,   22,
 /*    90 */    21,  237,  268,   11,   99,   33,  100,   89,  221,   36,
 /*   100 */   180,  181,  182,  183,  184,  185,  186,  187,  188,  189,
 /*   110 */   190,  314,   50,  327,  328,  326,   83,   75,  305,   69,
 /*   120 */   230,  106,  307,   26,   27,  291,   41,   88,   20,   19,
 /*   130 */    15,   17,   16,   18,   25,   24,   23,   22,   21,   26,
 /*   140 */    27,  297,   99,   30,   20,   19,   15,   17,   16,   18,
 /*   150 */    25,   24,   23,   22,   21,   12,  268,   43,   99,   34,
 /*   160 */    13,   14,  197,  106,   97,  109,   27,   79,   81,  274,
 /*   170 */    20,   19,   15,   17,   16,   18,   25,   24,   23,   22,
 /*   180 */    21,  306,  108,   48,   99,  293,   39,   28,   32,  235,
 /*   190 */   205,    7,   82,   80,  258,  259,  260,  174,  268,    2,
 /*   200 */   180,  181,  182,  183,  184,  185,  186,  187,  188,  189,
 /*   210 */   190,   20,   19,   15,   17,   16,   18,   25,   24,   23,
 /*   220 */    22,   21,   47,   76,  102,   99,  358,  358,  358,  358,
 /*   230 */   358,  358,   25,   24,   23,   22,   21,   90,  101,  177,
 /*   240 */    99,   12,   42,  264,  265,  194,   13,   14,   38,  204,
 /*   250 */    97,   58,  327,  328,  326,   85,   49,    8,   73,   44,
 /*   260 */    45,   71,  270,    9,   46,  222,  201,  203,  225,  263,
 /*   270 */   287,  103,    3,   28,   86,  235,   58,  327,  328,  326,
 /*   280 */   258,  259,  260,   12,  294,   87,   70,   99,   13,   14,
 /*   290 */    35,   29,   97,    4,   72,  315,   50,  327,  328,  326,
 /*   300 */    83,  178,    4,   84,  315,   50,  327,  328,  326,   83,
 /*   310 */    51,  327,  328,  326,    5,   28,   40,  235,   74,   78,
 /*   320 */    37,  307,  258,  259,  260,    6,   55,  327,  328,  326,
 /*   330 */    56,  327,  328,  326,   52,  327,  328,  326,   57,  327,
 /*   340 */   328,  326,   77,   98,  191,  262,   31,  105,  107,   91,
 /*   350 */   327,  328,  326,   92,  327,  328,  326,   93,  327,  328,
 /*   360 */   326,   61,  327,  328,  326,  356,   62,  327,  328,  326,
 /*   370 */   265,  265,   63,  327,  328,  326,   64,  327,  328,  326,
 /*   380 */    65,  327,  328,  326,   66,  327,  328,  326,   94,  327,
 /*   390 */   328,  326,   95,  327,  328,  326,  265,   96,  327,  328,
 /*   400 */   326,  265,   67,  327,  328,  326,   68,  327,  328,  326,
 /*   410 */    59,  327,  328,  326,   60,  327,  328,  326,   53,  327,
 /*   420 */   328,  326,   23,   22,   21,  265,  265,  265,   99,   54,
 /*   430 */   327,  328,  326,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     1,    2,    3,   60,   61,   23,    7,    8,    9,   10,
 /*    10 */    11,   12,   13,   14,   15,   16,   17,    2,    3,   47,
 /*    20 */    21,   49,    7,    8,    9,   10,   11,   12,   13,   14,
 /*    30 */    15,   16,   17,   24,   25,   23,   21,   90,   91,   23,
 /*    40 */    41,   87,   26,   27,   28,   29,   30,   31,   32,   33,
 /*    50 */    34,   35,   36,   90,   91,   69,   41,   57,   42,    2,
 /*    60 */     3,   90,   91,   63,    7,    8,    9,   10,   11,   12,
 /*    70 */    13,   14,   15,   16,   17,    2,    3,   47,   21,   49,
 /*    80 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*    90 */    17,   47,   92,   49,   21,   90,   91,   23,   41,   40,
 /*   100 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   110 */    36,   78,   79,   80,   81,   82,   83,   68,   69,   66,
 /*   120 */    47,   57,   69,    2,    3,   87,   88,   63,    7,    8,
 /*   130 */     9,   10,   11,   12,   13,   14,   15,   16,   17,    2,
 /*   140 */     3,   62,   21,   46,    7,    8,    9,   10,   11,   12,
 /*   150 */    13,   14,   15,   16,   17,   14,   92,   75,   21,   40,
 /*   160 */    19,   20,   41,   57,   23,   22,    3,   64,   64,   63,
 /*   170 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   180 */    17,   69,   39,   72,   21,   62,   46,   46,   45,   48,
 /*   190 */    44,   50,   51,   52,   53,   54,   55,   23,   92,   43,
 /*   200 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   210 */    36,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   220 */    16,   17,   49,   48,   23,   21,    7,    8,    9,   10,
 /*   230 */    11,   12,   13,   14,   15,   16,   17,   23,   24,   25,
 /*   240 */    21,   14,   49,   58,   59,   44,   19,   20,   49,   47,
 /*   250 */    23,   79,   80,   81,   82,   70,   71,    1,   73,   74,
 /*   260 */    23,   89,   48,    1,   23,   41,   44,   23,   41,   84,
 /*   270 */    85,   86,   43,   46,   40,   48,   79,   80,   81,   82,
 /*   280 */    53,   54,   55,   14,   46,   40,   89,   21,   19,   20,
 /*   290 */    40,    1,   23,   76,   77,   78,   79,   80,   81,   82,
 /*   300 */    83,   25,   76,   77,   78,   79,   80,   81,   82,   83,
 /*   310 */    79,   80,   81,   82,   46,   46,   43,   48,   66,   67,
 /*   320 */    40,   69,   53,   54,   55,   46,   79,   80,   81,   82,
 /*   330 */    79,   80,   81,   82,   79,   80,   81,   82,   79,   80,
 /*   340 */    81,   82,   90,   23,   41,   38,   40,   37,   23,   79,
 /*   350 */    80,   81,   82,   79,   80,   81,   82,   79,   80,   81,
 /*   360 */    82,   79,   80,   81,   82,    0,   79,   80,   81,   82,
 /*   370 */    93,   93,   79,   80,   81,   82,   79,   80,   81,   82,
 /*   380 */    79,   80,   81,   82,   79,   80,   81,   82,   79,   80,
 /*   390 */    81,   82,   79,   80,   81,   82,   93,   79,   80,   81,
 /*   400 */    82,   93,   79,   80,   81,   82,   79,   80,   81,   82,
 /*   410 */    79,   80,   81,   82,   79,   80,   81,   82,   79,   80,
 /*   420 */    81,   82,   15,   16,   17,   93,   93,   93,   21,   79,
 /*   430 */    80,   81,   82,
};
#define YY_SHIFT_USE_DFLT (433)
#define YY_SHIFT_COUNT    (109)
#define YY_SHIFT_MIN      (-28)
#define YY_SHIFT_MAX      (407)
static const short yy_shift_ofst[] = {
 /*     0 */   433,  143,  141,  141,  141,  269,  269,  227,  269,  269,
 /*    10 */   269,  269,  269,  269,  269,  269,  269,  269,  269,  269,
 /*    20 */   269,  269,  269,  269,  269,  269,  269,  269,  269,  269,
 /*    30 */   214,   16,   74,  174,    9,    9,    9,    9,  -18,  -18,
 /*    40 */    12,  201,  -18,   59,   97,  119,  119,  -18,   59,  140,
 /*    50 */    -1,   15,   57,   73,  121,  137,  137,  137,  137,  163,
 /*    60 */   204,  219,  219,  219,  219,  219,  219,  407,  407,  -28,
 /*    70 */    30,   44,  146,  156,  173,  193,  199,  175,  202,  256,
 /*    80 */   237,  262,  241,  224,  222,  229,  244,  234,  245,  238,
 /*    90 */   250,  266,  266,  266,  266,  266,  266,  268,  279,  320,
 /*   100 */   290,  276,  280,  273,  303,  307,  310,  306,  325,  365,
};
#define YY_REDUCE_USE_DFLT (-58)
#define YY_REDUCE_COUNT (49)
#define YY_REDUCE_MIN   (-57)
#define YY_REDUCE_MAX   (350)
static const short yy_reduce_ofst[] = {
 /*     0 */   -57,  185,  217,  226,   33,  172,  197,  231,  247,  251,
 /*    10 */   255,  259,  270,  274,  278,  282,  287,  293,  297,  301,
 /*    20 */   305,  309,  313,  318,  323,  327,  331,  335,  339,  350,
 /*    30 */   252,    0,   64,  106,  -53,  -37,  -29,    5,   49,   53,
 /*    40 */    38,  -46,  -14,   79,   82,  103,  104,  112,  123,  111,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   266,  358,  312,  312,  313,  352,  352,  358,  358,  358,
 /*    10 */   358,  358,  358,  358,  358,  358,  358,  358,  358,  358,
 /*    20 */   358,  358,  358,  358,  358,  358,  358,  358,  358,  358,
 /*    30 */   308,  358,  358,  358,  270,  270,  270,  270,  358,  308,
 /*    40 */   358,  358,  358,  311,  358,  324,  324,  358,  311,  358,
 /*    50 */   358,  358,  358,  358,  358,  322,  321,  350,  351,  335,
 /*    60 */   334,  346,  345,  344,  343,  342,  341,  337,  336,  358,
 /*    70 */   358,  358,  358,  358,  303,  302,  301,  358,  358,  358,
 /*    80 */   358,  358,  358,  358,  358,  358,  358,  358,  358,  269,
 /*    90 */   358,  349,  348,  347,  340,  339,  338,  329,  331,  358,
 /*   100 */   358,  271,  358,  358,  358,  358,  267,  358,  358,  358,
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
  "LET",           "INTEGER_LITERAL",  "DECIMAL_LITERAL",  "BOOL_LITERAL",
  "error",         "type_builtin",  "decl_function",  "type_function_decl",
  "decls",         "module",        "function_return_decl",  "type_specifier",
  "var_type_spec",  "named_decl",    "function_args_decl",  "type_function_args_decl",
  "type_function_args_decl_other",  "function_arg_decl",  "function_header",  "function_start",
  "function_param_list",  "type_function_header",  "type_function_start",  "type_function_param_list",
  "statement_list",  "statement_list_or_empty",  "statement",     "expression",  
  "unaryexp",      "binaryexp",     "literal",       "decl_var",    
  "decl_type",     "decl_struct",   "struct_header",  "struct_decl_var",
  "struct_decl_vars",  "function_call_args",  "qualifier",     "qualified_type",
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
 /*  66 */ "expression ::= IDENTIFIER",
 /*  67 */ "expression ::= SELF",
 /*  68 */ "expression ::= expression DOT IDENTIFIER",
 /*  69 */ "expression ::= IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  70 */ "expression ::= expression DOT IDENTIFIER PAR_BEGIN function_call_args PAR_END",
 /*  71 */ "binaryexp ::= expression AND expression",
 /*  72 */ "binaryexp ::= expression OR expression",
 /*  73 */ "binaryexp ::= expression PLUS expression",
 /*  74 */ "binaryexp ::= expression MINUS expression",
 /*  75 */ "binaryexp ::= expression MULT expression",
 /*  76 */ "binaryexp ::= expression DIV expression",
 /*  77 */ "binaryexp ::= expression MOD expression",
 /*  78 */ "binaryexp ::= expression EQ expression",
 /*  79 */ "binaryexp ::= expression NE expression",
 /*  80 */ "binaryexp ::= expression LE expression",
 /*  81 */ "binaryexp ::= expression LT expression",
 /*  82 */ "binaryexp ::= expression GE expression",
 /*  83 */ "binaryexp ::= expression GT expression",
 /*  84 */ "unaryexp ::= NOT expression",
 /*  85 */ "unaryexp ::= BIT_NOT expression",
 /*  86 */ "unaryexp ::= MINUS expression",
 /*  87 */ "function_call_args ::= function_call_args COMMA expression",
 /*  88 */ "function_call_args ::= expression",
 /*  89 */ "function_call_args ::=",
 /*  90 */ "literal ::= INTEGER_LITERAL",
 /*  91 */ "literal ::= DECIMAL_LITERAL",
 /*  92 */ "literal ::= BOOL_LITERAL",
 /*  93 */ "module ::= decls END",
 /*  94 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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
  { 90, 0 },
  { 90, -1 },
  { 90, -1 },
  { 90, -2 },
  { 91, -2 },
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
  { 84, -5 },
  { 84, -1 },
  { 86, -4 },
  { 85, -4 },
  { 88, -2 },
  { 88, -1 },
  { 87, -6 },
  { 70, -3 },
  { 71, -2 },
  { 72, -3 },
  { 58, -4 },
  { 73, -3 },
  { 74, -5 },
  { 75, -3 },
  { 59, -4 },
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
  { 77, 0 },
  { 77, -1 },
  { 76, -2 },
  { 76, -1 },
  { 78, -4 },
  { 78, -2 },
  { 78, -2 },
  { 78, -3 },
  { 78, -2 },
  { 83, -5 },
  { 83, -5 },
  { 64, -2 },
  { 64, 0 },
  { 79, -3 },
  { 79, -1 },
  { 79, -1 },
  { 79, -1 },
  { 79, -1 },
  { 79, -1 },
  { 79, -3 },
  { 79, -4 },
  { 79, -6 },
  { 81, -3 },
  { 81, -3 },
  { 81, -3 },
  { 81, -3 },
  { 81, -3 },
  { 81, -3 },
  { 81, -3 },
  { 81, -3 },
  { 81, -3 },
  { 81, -3 },
  { 81, -3 },
  { 81, -3 },
  { 81, -3 },
  { 80, -2 },
  { 80, -2 },
  { 80, -2 },
  { 89, -3 },
  { 89, -1 },
  { 89, 0 },
  { 82, -1 },
  { 82, -1 },
  { 82, -1 },
  { 61, -2 },
  { 92, -3 },
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
    yylhsminor.yy184 =yymsp[-1].minor.yy184; yymsp[-1].minor.yy184->addDecl(yymsp[0].minor.yy174);
}
  yymsp[-1].minor.yy184 = yylhsminor.yy184;
        break;
      case 1: /* decls ::= decls decl_function */
{
        yylhsminor.yy184 =yymsp[-1].minor.yy184; yymsp[-1].minor.yy184->addDecl(yymsp[0].minor.yy183);
}
  yymsp[-1].minor.yy184 = yylhsminor.yy184;
        break;
      case 2: /* decls ::= decls type_function_decl */
{
        yylhsminor.yy184 =yymsp[-1].minor.yy184; yymsp[-1].minor.yy184->addDecl(yymsp[0].minor.yy165);
}
  yymsp[-1].minor.yy184 = yylhsminor.yy184;
        break;
      case 3: /* decls ::= */
{
        yymsp[1].minor.yy184 =  pParser->getModule().getDeclNode();
     }
        break;
      case 4: /* type_specifier ::= type_builtin */
{yylhsminor.yy180=yymsp[0].minor.yy180;}
  yymsp[0].minor.yy180 = yylhsminor.yy180;
        break;
      case 5: /* type_specifier ::= type_array */
{yymsp[0].minor.yy180 = nullptr;}
        break;
      case 6: /* type_specifier ::= IDENTIFIER */
{
    auto type = pParser->resolveType(yymsp[0].minor.yy0);
    if (type != nullptr) {
        yylhsminor.yy180 = pParser->newAstNode<yal::RefType>(type);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy180->setSourceInfo(srcInfo);
    } else {
        yylhsminor.yy180 = nullptr;
    }
}
  yymsp[0].minor.yy180 = yylhsminor.yy180;
        break;
      case 7: /* qualifier ::= */
{yymsp[1].minor.yy81 = 0;}
        break;
      case 8: /* qualifier ::= MUT */
{yymsp[0].minor.yy81 = yal::parser::kQualifierMutable;}
        break;
      case 9: /* qualifier ::= REFERENCE */
{yymsp[0].minor.yy81 = yal::parser::kQualifierReference;}
        break;
      case 10: /* qualifier ::= MUT REFERENCE */
{
        yymsp[-1].minor.yy81 = yal::parser::kQualifierReference | yal::parser::kQualifierMutable;
 }
        break;
      case 11: /* qualified_type ::= qualifier type_specifier */
{
    if(yymsp[0].minor.yy180) {
        yymsp[0].minor.yy180->setQualifier(yal::parser::MakeQualifierFromFlags(yymsp[-1].minor.yy81));
    }
    yylhsminor.yy180 = yymsp[0].minor.yy180;
}
  yymsp[-1].minor.yy180 = yylhsminor.yy180;
        break;
      case 12: /* type_builtin ::= TYPE_BOOL */
{
    yymsp[0].minor.yy180 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinBool());
    }
        break;
      case 13: /* type_builtin ::= TYPE_INT8 */
{
    yymsp[0].minor.yy180 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI8());
    }
        break;
      case 14: /* type_builtin ::= TYPE_UINT8 */
{
    yymsp[0].minor.yy180  = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU8());
    }
        break;
      case 15: /* type_builtin ::= TYPE_INT16 */
{
    yymsp[0].minor.yy180 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI16());
    }
        break;
      case 16: /* type_builtin ::= TYPE_UINT16 */
{
    yymsp[0].minor.yy180  = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU16());
    }
        break;
      case 17: /* type_builtin ::= TYPE_INT32 */
{
    yymsp[0].minor.yy180 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI32());
    }
        break;
      case 18: /* type_builtin ::= TYPE_UINT32 */
{
    yymsp[0].minor.yy180 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU32());
    }
        break;
      case 19: /* type_builtin ::= TYPE_INT64 */
{
    yymsp[0].minor.yy180 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinI64());
    }
        break;
      case 20: /* type_builtin ::= TYPE_UINT64 */
{
    yymsp[0].minor.yy180 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinU64());
    }
        break;
      case 21: /* type_builtin ::= TYPE_FLOAT */
{
    yymsp[0].minor.yy180 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinFloat());
    }
        break;
      case 22: /* type_builtin ::= TYPE_DOUBLE */
{
    yymsp[0].minor.yy180 = pParser->newAstNode<yal::RefType>(pParser->getModule().getTypeContext().getTypeBuiltinDouble());
    }
        break;
      case 23: /* decl_type ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON */
{
    yymsp[-4].minor.yy174 = nullptr;
}
        break;
      case 24: /* decl_type ::= decl_struct */
{ yylhsminor.yy174 = yymsp[0].minor.yy78;}
  yymsp[0].minor.yy174 = yylhsminor.yy174;
        break;
      case 25: /* struct_header ::= TYPE IDENTIFIER COLON STRUCT */
{
    yylhsminor.yy78 = pParser->newAstNode<yal::DeclStruct>(yymsp[-2].minor.yy0.tokenStr);
    yylhsminor.yy78->setSourceInfo(pParser->createSourceInfo(yymsp[-3].minor.yy0, yymsp[-2].minor.yy0));
    pParser->onDeclBegin(yylhsminor.yy78);
}
  yymsp[-3].minor.yy78 = yylhsminor.yy78;
        break;
      case 26: /* decl_struct ::= struct_header SCOPE_BEGIN struct_decl_vars SCOPE_END */
{
    yylhsminor.yy78 = yymsp[-3].minor.yy78;
    if (yymsp[-1].minor.yy149 != nullptr) {
        yymsp[-1].minor.yy149->updateSourceInfo();
        yylhsminor.yy78->setMembers(yymsp[-1].minor.yy149);
    }
    auto srcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy78->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy78->setSourceInfo(srcInfo);
    pParser->onDeclEnd();
}
  yymsp[-3].minor.yy78 = yylhsminor.yy78;
        break;
      case 27: /* struct_decl_vars ::= struct_decl_vars struct_decl_var */
{
    yylhsminor.yy149 = yymsp[-1].minor.yy149;
    if (pParser->onDecl(yymsp[0].minor.yy138)) {
        yylhsminor.yy149->addDeclVar(yymsp[0].minor.yy138);
    }
}
  yymsp[-1].minor.yy149 = yylhsminor.yy149;
        break;
      case 28: /* struct_decl_vars ::= struct_decl_var */
{
    yylhsminor.yy149 = pParser->newAstNode<yal::DeclStructMembers>();
    if (pParser->onDecl(yymsp[0].minor.yy138)) {
        yylhsminor.yy149->addDeclVar(yymsp[0].minor.yy138);
    }
}
  yymsp[0].minor.yy149 = yylhsminor.yy149;
        break;
      case 29: /* struct_decl_var ::= IDENTIFIER COLON qualified_type ASSIGN expression SEMI_COLON */
{
    yylhsminor.yy138 = pParser->newAstNode<yal::DeclVar>(yymsp[-5].minor.yy0.tokenStr, yal::Qualifier(), yymsp[-3].minor.yy180, yymsp[-1].minor.yy7);
    auto srcInfo = pParser->createSourceInfo(yymsp[-5].minor.yy0, yymsp[0].minor.yy0);
    yylhsminor.yy138->setSourceInfo(srcInfo);
}
  yymsp[-5].minor.yy138 = yylhsminor.yy138;
        break;
      case 30: /* function_header ::= function_start function_param_list function_return_decl */
{
    yylhsminor.yy183 = yymsp[-2].minor.yy183;
    yylhsminor.yy183->setParams(yymsp[-1].minor.yy74);
    yylhsminor.yy183->setReturnType(yymsp[0].minor.yy180);

}
  yymsp[-2].minor.yy183 = yylhsminor.yy183;
        break;
      case 31: /* function_start ::= FUNCTION IDENTIFIER */
{
     yylhsminor.yy183 = pParser->newAstNode<yal::DeclFunction>(yymsp[0].minor.yy0.tokenStr);
     yylhsminor.yy183->setSourceInfo(pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy0));
     pParser->onDeclBegin(yylhsminor.yy183);
}
  yymsp[-1].minor.yy183 = yylhsminor.yy183;
        break;
      case 32: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
      case 36: /* type_function_param_list ::= PAR_BEGIN type_function_args_decl PAR_END */ yytestcase(yyruleno==36);
{
    yymsp[-2].minor.yy74 = yymsp[-1].minor.yy74;
}
        break;
      case 33: /* decl_function ::= function_header SCOPE_BEGIN statement_list_or_empty SCOPE_END */
{
    yylhsminor.yy183 = yymsp[-3].minor.yy183;
    yylhsminor.yy183->setFunctionBody(yymsp[-1].minor.yy170);
    yylhsminor.yy183->setSourceInfo(pParser->createSourceInfo(yymsp[-3].minor.yy183->getSourceInfo(), yymsp[0].minor.yy0));
    pParser->onDeclEnd();
}
  yymsp[-3].minor.yy183 = yylhsminor.yy183;
        break;
      case 34: /* type_function_header ::= type_function_start type_function_param_list function_return_decl */
{
    yylhsminor.yy165 = yymsp[-2].minor.yy165;
    yylhsminor.yy165->setParams(yymsp[-1].minor.yy74);
    yylhsminor.yy165->setReturnType(yymsp[0].minor.yy180);
}
  yymsp[-2].minor.yy165 = yylhsminor.yy165;
        break;
      case 35: /* type_function_start ::= FUNCTION type_specifier COLON COLON IDENTIFIER */
{
    yylhsminor.yy165 = pParser->newAstNode<yal::DeclTypeFunction>(yymsp[0].minor.yy0.tokenStr, yymsp[-3].minor.yy180);
    yylhsminor.yy165->setSourceInfo(pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0));
    pParser->onDeclBegin(yylhsminor.yy165);
}
  yymsp[-4].minor.yy165 = yylhsminor.yy165;
        break;
      case 37: /* type_function_decl ::= type_function_header SCOPE_BEGIN statement_list_or_empty SCOPE_END */
{
    yylhsminor.yy165 = yymsp[-3].minor.yy165;
    yylhsminor.yy165->setFunctionBody(yymsp[-1].minor.yy170);
    yylhsminor.yy165->setSourceInfo(pParser->createSourceInfo(yymsp[-3].minor.yy165->getSourceInfo(), yymsp[0].minor.yy0));
    pParser->onDeclEnd();
}
  yymsp[-3].minor.yy165 = yylhsminor.yy165;
        break;
      case 38: /* type_function_args_decl ::= qualifier SELF */
{
        yylhsminor.yy74 = pParser->newAstNode<yal::DeclParamVarContainer>();
        auto qualifier = yal::parser::MakeQualifierFromFlags(yymsp[-1].minor.yy81);
        auto refType = pParser->newAstNode<yal::RefType>(qualifier, pParser->resolveSelfType());
        auto selfVar = pParser->newAstNode<yal::DeclParamVarSelf>(refType);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        selfVar->setSourceInfo(srcInfo);
        yylhsminor.yy74->addDeclParam(selfVar);
        pParser->onDecl(selfVar);
}
  yymsp[-1].minor.yy74 = yylhsminor.yy74;
        break;
      case 39: /* type_function_args_decl ::= qualifier SELF COMMA type_function_args_decl_other */
{
    yylhsminor.yy74 = yymsp[0].minor.yy74;
    auto qualifier = yal::parser::MakeQualifierFromFlags(yymsp[-3].minor.yy81);
    auto refType = pParser->newAstNode<yal::RefType>(qualifier, pParser->resolveSelfType());
    auto selfVar = pParser->newAstNode<yal::DeclParamVarSelf>(refType);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[-2].minor.yy0);
    selfVar->setSourceInfo(srcInfo);
    yylhsminor.yy74->addDeclParam(selfVar);
    pParser->onDecl(selfVar);
}
  yymsp[-3].minor.yy74 = yylhsminor.yy74;
        break;
      case 40: /* type_function_args_decl ::= function_args_decl */
{
        yylhsminor.yy74 = yymsp[0].minor.yy74;
}
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 41: /* type_function_args_decl_other ::= type_function_args_decl_other COMMA function_arg_decl */
{
    yylhsminor.yy74 = yymsp[-2].minor.yy74;
    yylhsminor.yy74->addDeclParam(yymsp[0].minor.yy71);
}
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 42: /* type_function_args_decl_other ::= function_arg_decl */
{
        yylhsminor.yy74 = pParser->newAstNode<yal::DeclParamVarContainer>();
        yylhsminor.yy74->addDeclParam(yymsp[0].minor.yy71);
}
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 43: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
{
    if (yymsp[0].minor.yy71 != nullptr && pParser->onDecl(yymsp[0].minor.yy71)){
        yymsp[-2].minor.yy74->addDeclParam(yymsp[0].minor.yy71);
        yylhsminor.yy74 = yymsp[-2].minor.yy74;
    }
}
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 44: /* function_args_decl ::= function_arg_decl */
{
    if (yymsp[0].minor.yy71 != nullptr && pParser->onDecl(yymsp[0].minor.yy71)){
        yylhsminor.yy74 = pParser->newAstNode<yal::DeclParamVarContainer>();
        yylhsminor.yy74->addDeclParam(yymsp[0].minor.yy71);
    }
}
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 45: /* function_args_decl ::= */
{
        yymsp[1].minor.yy74 = nullptr;
}
        break;
      case 46: /* function_arg_decl ::= IDENTIFIER COLON qualified_type */
{
    if (yymsp[0].minor.yy180) {
        yylhsminor.yy71 = pParser->newAstNode<yal::DeclParamVar>(yymsp[-2].minor.yy0.tokenStr, yal::Qualifier(),yymsp[0].minor.yy180);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy180->getSourceInfo());
        yylhsminor.yy71->setSourceInfo(srcInfo);
    } else {
        yylhsminor.yy71 = nullptr;
    }
}
  yymsp[-2].minor.yy71 = yylhsminor.yy71;
        break;
      case 47: /* function_return_decl ::= COLON qualified_type */
{ yymsp[-1].minor.yy180 = yymsp[0].minor.yy180;
}
        break;
      case 48: /* function_return_decl ::= */
      case 61: /* var_type_spec ::= */ yytestcase(yyruleno==61);
{yymsp[1].minor.yy180 = nullptr;}
        break;
      case 49: /* statement_list_or_empty ::= */
{yymsp[1].minor.yy170 = nullptr;}
        break;
      case 50: /* statement_list_or_empty ::= statement_list */
{yylhsminor.yy170 = yymsp[0].minor.yy170;}
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 51: /* statement_list ::= statement_list statement */
{
    yymsp[-1].minor.yy170->addStatement(yymsp[0].minor.yy104);
    yylhsminor.yy170=yymsp[-1].minor.yy170;
}
  yymsp[-1].minor.yy170 = yylhsminor.yy170;
        break;
      case 52: /* statement_list ::= statement */
{
    yylhsminor.yy170 = pParser->newAstNode<yal::StatementList>();
    yylhsminor.yy170->addStatement(yymsp[0].minor.yy104);
}
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 53: /* statement ::= expression ASSIGN expression SEMI_COLON */
{
   yylhsminor.yy104 = pParser->newAstNode<yal::StmtAssign>(yymsp[-3].minor.yy7,yymsp[-1].minor.yy7);
}
  yymsp[-3].minor.yy104 = yylhsminor.yy104;
        break;
      case 54: /* statement ::= decl_var SEMI_COLON */
{
    yylhsminor.yy104 = yymsp[-1].minor.yy93;
}
  yymsp[-1].minor.yy104 = yylhsminor.yy104;
        break;
      case 55: /* statement ::= expression SEMI_COLON */
{yylhsminor.yy104 = yymsp[-1].minor.yy7;}
  yymsp[-1].minor.yy104 = yylhsminor.yy104;
        break;
      case 56: /* statement ::= RETURN expression SEMI_COLON */
{
    yylhsminor.yy104 = pParser->newAstNode<yal::StmtReturn>(yymsp[-1].minor.yy7);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy7->getSourceInfo());
    yylhsminor.yy104->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy104 = yylhsminor.yy104;
        break;
      case 57: /* statement ::= RETURN SEMI_COLON */
{
    yylhsminor.yy104 = pParser->newAstNode<yal::StmtReturn>();
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0,yymsp[-1].minor.yy0);
    yylhsminor.yy104->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy104 = yylhsminor.yy104;
        break;
      case 58: /* decl_var ::= VAR IDENTIFIER var_type_spec ASSIGN expression */
{
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr, yal::Qualifier(), yymsp[-2].minor.yy180, yymsp[0].minor.yy7);
    auto varSrcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[-3].minor.yy0);
    varDecl->setSourceInfo(varSrcInfo);
    if (pParser->onDecl(varDecl)) {
        yylhsminor.yy93= pParser->newAstNode<yal::StmtDecl>(varDecl);
        auto srcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy93->setSourceInfo(srcInfo);
    }

}
  yymsp[-4].minor.yy93 = yylhsminor.yy93;
        break;
      case 59: /* decl_var ::= LET IDENTIFIER var_type_spec ASSIGN expression */
{
    yal::Qualifier qualifier;
    qualifier.setImmutable();
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr, qualifier, yymsp[-2].minor.yy180, yymsp[0].minor.yy7);
    auto varSrcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[-3].minor.yy0);
    varDecl->setSourceInfo(varSrcInfo);
    if (pParser->onDecl(varDecl)) {
        yylhsminor.yy93= pParser->newAstNode<yal::StmtDecl>(varDecl);
        auto srcInfo = pParser->createSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy93->setSourceInfo(srcInfo);
    }
}
  yymsp[-4].minor.yy93 = yylhsminor.yy93;
        break;
      case 60: /* var_type_spec ::= COLON qualified_type */
{yymsp[-1].minor.yy180 = yymsp[0].minor.yy180;}
        break;
      case 62: /* expression ::= PAR_BEGIN expression PAR_END */
{yymsp[-2].minor.yy7 = yymsp[-1].minor.yy7;}
        break;
      case 63: /* expression ::= literal */
{yylhsminor.yy7 = yymsp[0].minor.yy7;}
  yymsp[0].minor.yy7 = yylhsminor.yy7;
        break;
      case 64: /* expression ::= unaryexp */
{yylhsminor.yy7 = yymsp[0].minor.yy85;}
  yymsp[0].minor.yy7 = yylhsminor.yy7;
        break;
      case 65: /* expression ::= binaryexp */
{yylhsminor.yy7 = yymsp[0].minor.yy153;}
  yymsp[0].minor.yy7 = yylhsminor.yy7;
        break;
      case 66: /* expression ::= IDENTIFIER */
{
    auto decl = pParser->resolveVarRef(yymsp[0].minor.yy0);
    if (decl != nullptr) {
        yylhsminor.yy7 = pParser->newAstNode<yal::ExprVarRef>(decl);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy7->setSourceInfo(srcInfo);
    }
}
  yymsp[0].minor.yy7 = yylhsminor.yy7;
        break;
      case 67: /* expression ::= SELF */
{
    auto decl = pParser->resolveVarRefSelf(yymsp[0].minor.yy0);
    if (decl != nullptr) {
        yylhsminor.yy7  = pParser->newAstNode<yal::ExprVarRefSelf>(decl);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy7->setSourceInfo(srcInfo);
    }
}
  yymsp[0].minor.yy7 = yylhsminor.yy7;
        break;
      case 68: /* expression ::= expression DOT IDENTIFIER */
{
    yylhsminor.yy7 = pParser->newAstNode<yal::ExprStructVarRef>(yymsp[-2].minor.yy7, yymsp[0].minor.yy0.tokenStr);
    auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy7->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy7 = yylhsminor.yy7;
        break;
      case 69: /* expression ::= IDENTIFIER PAR_BEGIN function_call_args PAR_END */
{
    auto type = pParser->resolveType(yymsp[-3].minor.yy0);
    if (type != nullptr ) {
        auto fnType = pParser->newAstNode<yal::RefType>(type);
        auto fnSrcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy0, yymsp[-3].minor.yy0);
        fnType->setSourceInfo(fnSrcInfo);
        if (yymsp[-1].minor.yy128 != nullptr) {
            yymsp[-1].minor.yy128->updateSourceInfo();
        }
        yylhsminor.yy7 = pParser->newAstNode<yal::ExprFnCall>(fnType, yymsp[-1].minor.yy128);
        auto srcInfo = pParser->createSourceInfo(yymsp[-3].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy7->setSourceInfo(srcInfo);
    }
}
  yymsp[-3].minor.yy7 = yylhsminor.yy7;
        break;
      case 70: /* expression ::= expression DOT IDENTIFIER PAR_BEGIN function_call_args PAR_END */
{
    if (yymsp[-1].minor.yy128 != nullptr) {
        yymsp[-1].minor.yy128->updateSourceInfo();
    }
    yylhsminor.yy7 = pParser->newAstNode<yal::ExprTypeFnCall>(yymsp[-5].minor.yy7, yymsp[-3].minor.yy0, yymsp[-1].minor.yy128);
    auto srcInfo = pParser->createSourceInfo(yymsp[-5].minor.yy7->getSourceInfo(), yymsp[0].minor.yy0);
    yylhsminor.yy7->setSourceInfo(srcInfo);

}
  yymsp[-5].minor.yy7 = yylhsminor.yy7;
        break;
      case 71: /* binaryexp ::= expression AND expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 72: /* binaryexp ::= expression OR expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 73: /* binaryexp ::= expression PLUS expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 74: /* binaryexp ::= expression MINUS expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 75: /* binaryexp ::= expression MULT expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 76: /* binaryexp ::= expression DIV expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 77: /* binaryexp ::= expression MOD expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 78: /* binaryexp ::= expression EQ expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 79: /* binaryexp ::= expression NE expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 80: /* binaryexp ::= expression LE expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 81: /* binaryexp ::= expression LT expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 82: /* binaryexp ::= expression GE expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 83: /* binaryexp ::= expression GT expression */
{
        yylhsminor.yy153 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
        auto srcInfo = pParser->createSourceInfo(yymsp[-2].minor.yy7->getSourceInfo(), yymsp[0].minor.yy7->getSourceInfo());
        yylhsminor.yy153->setSourceInfo(srcInfo);
}
  yymsp[-2].minor.yy153 = yylhsminor.yy153;
        break;
      case 84: /* unaryexp ::= NOT expression */
{
    yylhsminor.yy85 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy7);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy7->getSourceInfo());
    yylhsminor.yy85->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy85 = yylhsminor.yy85;
        break;
      case 85: /* unaryexp ::= BIT_NOT expression */
{
    yylhsminor.yy85 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy7);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy7->getSourceInfo());
    yylhsminor.yy85->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy85 = yylhsminor.yy85;
        break;
      case 86: /* unaryexp ::= MINUS expression */
{
    yylhsminor.yy85 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy7);
    auto srcInfo = pParser->createSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy7->getSourceInfo());
    yylhsminor.yy85->setSourceInfo(srcInfo);
}
  yymsp[-1].minor.yy85 = yylhsminor.yy85;
        break;
      case 87: /* function_call_args ::= function_call_args COMMA expression */
{
    yylhsminor.yy128 = yymsp[-2].minor.yy128;
    yylhsminor.yy128->addExpression(yymsp[0].minor.yy7);
}
  yymsp[-2].minor.yy128 = yylhsminor.yy128;
        break;
      case 88: /* function_call_args ::= expression */
{
    yylhsminor.yy128 = pParser->newAstNode<yal::ExprList>();
    yylhsminor.yy128->addExpression(yymsp[0].minor.yy7);
}
  yymsp[0].minor.yy128 = yylhsminor.yy128;
        break;
      case 89: /* function_call_args ::= */
{
    yymsp[1].minor.yy128= nullptr;
}
        break;
      case 90: /* literal ::= INTEGER_LITERAL */
{
        yylhsminor.yy7 = pParser->newIntegerLiteral(yymsp[0].minor.yy0);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy7->setSourceInfo(srcInfo);
   }
  yymsp[0].minor.yy7 = yylhsminor.yy7;
        break;
      case 91: /* literal ::= DECIMAL_LITERAL */
{
        yylhsminor.yy7 = pParser->newDecimalLiteral(yymsp[0].minor.yy0);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy7->setSourceInfo(srcInfo);
}
  yymsp[0].minor.yy7 = yylhsminor.yy7;
        break;
      case 92: /* literal ::= BOOL_LITERAL */
{
        yylhsminor.yy7 = pParser->newAstNode<yal::ExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = pParser->createSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0);
        yylhsminor.yy7->setSourceInfo(srcInfo);
}
  yymsp[0].minor.yy7 = yylhsminor.yy7;
        break;
      default:
      /* (93) module ::= decls END */ yytestcase(yyruleno==93);
      /* (94) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==94);
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
