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
#line 23 "stparserimpl.lemon"


#include "yal/frontend/lexer/lexer.h"
#include "yal/frontend/parser/stparser.h"
#include "yal/frontend/parser/syntaxtreenodes.h"
#include "yal/util/log.h"
#include "yal/util/stringref.h"


#include <cassert>
#include <vector>
#define YYMALLOCARGTYPE size_t
#line 41 "stparserimpl.c"
#include "stparserimpl.h"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
#if INTERFACE
#define YAL_TOKEN_ASSIGN                          1
#define YAL_TOKEN_AS                              2
#define YAL_TOKEN_OR                              3
#define YAL_TOKEN_AND                             4
#define YAL_TOKEN_BIT_OR                          5
#define YAL_TOKEN_BIT_XOR                         6
#define YAL_TOKEN_BIT_AND                         7
#define YAL_TOKEN_EQ                              8
#define YAL_TOKEN_NE                              9
#define YAL_TOKEN_GT                             10
#define YAL_TOKEN_LT                             11
#define YAL_TOKEN_GE                             12
#define YAL_TOKEN_LE                             13
#define YAL_TOKEN_PLUS                           14
#define YAL_TOKEN_MINUS                          15
#define YAL_TOKEN_MULT                           16
#define YAL_TOKEN_DIV                            17
#define YAL_TOKEN_MOD                            18
#define YAL_TOKEN_NEGATE                         19
#define YAL_TOKEN_BIT_NOT                        20
#define YAL_TOKEN_NOT                            21
#define YAL_TOKEN_DEREF                          22
#define YAL_TOKEN_REFERENCE                      23
#define YAL_TOKEN_DOT                            24
#define YAL_TOKEN_END                            25
#define YAL_TOKEN_IDENTIFIER                     26
#define YAL_TOKEN_SELF                           27
#define YAL_TOKEN_TYPE_BOOL                      28
#define YAL_TOKEN_TYPE_INT8                      29
#define YAL_TOKEN_TYPE_UINT8                     30
#define YAL_TOKEN_TYPE_INT16                     31
#define YAL_TOKEN_TYPE_UINT16                    32
#define YAL_TOKEN_TYPE_INT32                     33
#define YAL_TOKEN_TYPE_UINT32                    34
#define YAL_TOKEN_TYPE_INT64                     35
#define YAL_TOKEN_TYPE_UINT64                    36
#define YAL_TOKEN_TYPE_FLOAT                     37
#define YAL_TOKEN_TYPE_DOUBLE                    38
#define YAL_TOKEN_ARRAY_BEGIN                    39
#define YAL_TOKEN_ARRAY_END                      40
#define YAL_TOKEN_MUT                            41
#define YAL_TOKEN_TYPE                           42
#define YAL_TOKEN_COLON                          43
#define YAL_TOKEN_SEMI_COLON                     44
#define YAL_TOKEN_STRUCT                         45
#define YAL_TOKEN_SCOPE_BEGIN                    46
#define YAL_TOKEN_SCOPE_END                      47
#define YAL_TOKEN_COMMA                          48
#define YAL_TOKEN_PAR_BEGIN                      49
#define YAL_TOKEN_PAR_END                        50
#define YAL_TOKEN_FUNCTION                       51
#define YAL_TOKEN_IMPL                           52
#define YAL_TOKEN_RETURN                         53
#define YAL_TOKEN_VAR                            54
#define YAL_TOKEN_LET                            55
#define YAL_TOKEN_INTEGER_LITERAL                56
#define YAL_TOKEN_INTEGER_LITERAL_I8             57
#define YAL_TOKEN_INTEGER_LITERAL_I16            58
#define YAL_TOKEN_INTEGER_LITERAL_I32            59
#define YAL_TOKEN_INTEGER_LITERAL_I64            60
#define YAL_TOKEN_INTEGER_LITERAL_U8             61
#define YAL_TOKEN_INTEGER_LITERAL_U16            62
#define YAL_TOKEN_INTEGER_LITERAL_U32            63
#define YAL_TOKEN_INTEGER_LITERAL_U64            64
#define YAL_TOKEN_FLOAT_LITERAL                  65
#define YAL_TOKEN_FLOAT_LITERAL_32               66
#define YAL_TOKEN_FLOAT_LITERAL_64               67
#define YAL_TOKEN_BOOL_LITERAL                   68
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
**    YALSTParserTOKENTYPE     is the data type used for minor type for terminal
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
**                       which is YALSTParserTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    YALSTParserARG_SDECL     A static variable declaration for the %extra_argument
**    YALSTParserARG_PDECL     A parameter declaration for the %extra_argument
**    YALSTParserARG_STORE     Code to store %extra_argument into yypParser
**    YALSTParserARG_FETCH     Code to extract %extra_argument from yypParser
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
#define YYNOCODE 105
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#endif
typedef union {
  int yyinit;
  YALSTParserTOKENTYPE yy0;
  yal::frontend::STDeclVar* yy25;
  yal::frontend::ParseListDeclParam::Range yy36;
  uint32_t yy37;
  yal::frontend::STDeclStruct* yy38;
  yal::frontend::ParseListStructMember::Range yy39;
  yal::frontend::STStructMemberInit* yy41;
  yal::frontend::STStatement* yy49;
  yal::frontend::STDeclModule* yy62;
  yal::frontend::STExprUnaryOperator* yy70;
  yal::frontend::ParseListStructInit::Range yy76;
  yal::frontend::STType* yy82;
  yal::frontend::ParseListDecl::Range yy83;
  yal::frontend::STStructMember* yy85;
  yal::frontend::STDecl* yy92;
  yal::frontend::ParseListExpr::Range yy102;
  yal::frontend::STDeclTypeFunctions* yy123;
  yal::frontend::STExprBinaryOperator* yy140;
  yal::frontend::STExpression* yy152;
  yal::frontend::STExprStructInit* yy156;
  yal::frontend::STDeclParam* yy165;
  yal::frontend::STDeclFunction* yy182;
  yal::frontend::STQualType* yy185;
  yal::frontend::ParseListStmt::Range yy199;
  yal::frontend::STIdentifier* yy203;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#if INTERFACE
#define YALSTParserARG_SDECL  yal::frontend::STParser *pParser ;
#define YALSTParserARG_PDECL , yal::frontend::STParser *pParser 
#define YALSTParserARG_FETCH  yal::frontend::STParser *pParser  = yypParser->pParser 
#define YALSTParserARG_STORE yypParser->pParser  = pParser 
#endif
#define YYNSTATE             116
#define YYNRULE              111
#define YY_MAX_SHIFT         115
#define YY_MIN_SHIFTREDUCE   190
#define YY_MAX_SHIFTREDUCE   300
#define YY_MIN_REDUCE        301
#define YY_MAX_REDUCE        411
#define YY_ERROR_ACTION      412
#define YY_ACCEPT_ACTION     413
#define YY_NO_ACTION         414
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
#define YY_ACTTAB_COUNT (1125)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    14,   12,   26,   25,   24,   15,   16,  194,   13,  115,
 /*    10 */    54,   84,  195,  201,  202,  203,  204,  205,  206,  207,
 /*    20 */   208,  209,  210,  211,   56,  107,   59,   58,   56,  107,
 /*    30 */    41,    1,   85,  340,   31,   58,   35,  339,    7,   52,
 /*    40 */    50,  286,  287,  288,  289,  290,  291,  292,  293,  294,
 /*    50 */   295,  296,  297,  298,   14,   12,  337,  301,  302,   15,
 /*    60 */    16,   88,   13,  329,  338,   84,  195,  201,  202,  203,
 /*    70 */   204,  205,  206,  207,  208,  209,  210,  211,  303,  113,
 /*    80 */   109,  111,  113,  243,   87,   48,   89,  113,   31,  327,
 /*    90 */   214,  332,   86,  194,  323,  286,  287,  288,  289,  290,
 /*   100 */   291,  292,  293,  294,  295,  296,  297,  298,  108,   49,
 /*   110 */   310,   14,   12,  310,   93,   40,   15,   16,  310,   13,
 /*   120 */    33,  413,   84,  195,  201,  202,  203,  204,  205,  206,
 /*   130 */   207,  208,  209,  210,  211,   51,  225,   81,  395,  344,
 /*   140 */    58,   36,  358,  106,   36,   31,  365,  342,   36,   36,
 /*   150 */    32,  214,  286,  287,  288,  289,  290,  291,  292,  293,
 /*   160 */   294,  295,  296,  297,  298,   10,   42,   29,   30,  108,
 /*   170 */   281,   46,   23,   22,   18,   20,   19,   21,   28,   27,
 /*   180 */    26,   25,   24,   42,   29,   30,   99,   93,   54,   23,
 /*   190 */    22,   18,   20,   19,   21,   28,   27,   26,   25,   24,
 /*   200 */   333,   17,   36,  257,   17,   54,  258,   17,  241,  259,
 /*   210 */   100,  394,  200,   38,  201,  202,  203,  204,  205,  206,
 /*   220 */   207,  208,  209,  210,  211,  242,  219,   47,   42,   29,
 /*   230 */    30,  110,  195,  104,   23,   22,   18,   20,   19,   21,
 /*   240 */    28,   27,   26,   25,   24,   42,   29,   30,  343,   57,
 /*   250 */    54,   23,   22,   18,   20,   19,   21,   28,   27,   26,
 /*   260 */    25,   24,  311,  214,  109,  311,  194,   54,   55,   37,
 /*   270 */   239,  223,  114,   45,    8,  331,    9,  244,  113,  240,
 /*   280 */    83,  108,  105,  308,  366,   91,  224,   11,   54,   53,
 /*   290 */     4,    5,  215,  249,    6,    2,    3,   90,  349,   60,
 /*   300 */   362,  363,  361,   43,  364,   44,  300,   39,  113,  310,
 /*   310 */    83,  217,  105,  308,  366,   91,  112,   34,  410,  303,
 /*   320 */   303,  303,  303,  303,  303,  303,    3,   92,  349,   60,
 /*   330 */   362,  363,  361,  303,  364,   42,   29,   30,  303,  310,
 /*   340 */   303,   23,   22,   18,   20,   19,   21,   28,   27,   26,
 /*   350 */    25,   24,  303,  303,  303,  303,  113,   54,   83,  303,
 /*   360 */   105,  308,  366,   91,  303,  412,  412,  412,  412,  412,
 /*   370 */   412,   28,   27,   26,   25,   24,  348,   60,  362,  363,
 /*   380 */   361,   54,  364,  303,   30,  303,  303,  310,   23,   22,
 /*   390 */    18,   20,   19,   21,   28,   27,   26,   25,   24,  303,
 /*   400 */   303,  113,  303,   83,   54,  105,  308,  366,  303,  303,
 /*   410 */   303,  303,  113,  303,   83,  303,  105,  308,  366,  303,
 /*   420 */   303,  303,   68,  362,  363,  361,   82,  364,  303,  303,
 /*   430 */   303,  303,  310,   68,  362,  363,  361,   80,  364,  303,
 /*   440 */   303,  303,  303,  310,  303,  303,  303,  303,  303,  113,
 /*   450 */   303,   83,  303,  105,  308,  366,  303,  303,  303,  303,
 /*   460 */   303,  303,  303,  303,  303,  303,  303,  303,  303,  303,
 /*   470 */    68,  362,  363,  361,   79,  364,  303,  303,  303,  303,
 /*   480 */   310,  200,  303,  201,  202,  203,  204,  205,  206,  207,
 /*   490 */   208,  209,  210,  211,   23,   22,   18,   20,   19,   21,
 /*   500 */    28,   27,   26,   25,   24,  303,  303,  113,  303,   83,
 /*   510 */    54,  105,  308,  366,  303,  303,  303,  303,  303,  303,
 /*   520 */   303,  303,  303,  303,  303,  303,  303,  303,   61,  362,
 /*   530 */   363,  361,  303,  364,  303,  303,  303,  303,  310,  303,
 /*   540 */   303,  303,  303,  113,  303,   83,  303,  105,  308,  366,
 /*   550 */   303,  303,  303,  303,  113,  303,   83,  303,  105,  308,
 /*   560 */   366,  303,  303,  303,   64,  362,  363,  361,  303,  364,
 /*   570 */   303,  303,  303,  303,  310,   65,  362,  363,  361,  303,
 /*   580 */   364,  303,  113,  303,   83,  310,  105,  308,  366,  303,
 /*   590 */   303,  303,  303,  303,  303,  303,  303,  303,  303,  303,
 /*   600 */   303,  303,  303,   62,  362,  363,  361,  303,  364,  303,
 /*   610 */   303,  303,  303,  310,  303,  303,  113,  303,   83,  303,
 /*   620 */   105,  308,  366,  303,  303,  303,  303,  113,  303,   83,
 /*   630 */   303,  105,  308,  366,  303,  303,  303,   66,  362,  363,
 /*   640 */   361,  303,  364,  303,  303,  303,  303,  310,   94,  362,
 /*   650 */   363,  361,  303,  364,  303,  113,  303,   83,  310,  105,
 /*   660 */   308,  366,  303,  303,  303,  303,  303,  303,  303,  303,
 /*   670 */   303,  303,  303,  303,  303,  303,   95,  362,  363,  361,
 /*   680 */   303,  364,  303,  303,  303,  303,  310,  303,  303,  113,
 /*   690 */   303,   83,  303,  105,  308,  366,  303,  303,  303,  303,
 /*   700 */   113,  303,   83,  303,  105,  308,  366,  303,  303,  303,
 /*   710 */    96,  362,  363,  361,  303,  364,  303,  303,  303,  303,
 /*   720 */   310,   97,  362,  363,  361,  303,  364,  303,  113,  303,
 /*   730 */    83,  310,  105,  308,  366,  303,  303,  303,  303,  303,
 /*   740 */   303,  303,  303,  303,  303,  303,  303,  303,  303,   98,
 /*   750 */   362,  363,  361,  303,  364,  303,  303,  303,  303,  310,
 /*   760 */   303,  303,  113,  303,   83,  303,  105,  308,  366,  303,
 /*   770 */   303,  303,  303,  113,  303,   83,  303,  105,  308,  366,
 /*   780 */   303,  303,  303,   67,  362,  363,  361,  303,  364,  303,
 /*   790 */   303,  303,  303,  310,   70,  362,  363,  361,  303,  364,
 /*   800 */   303,  113,  303,   83,  310,  105,  308,  366,  303,  303,
 /*   810 */   303,  303,  303,  303,  303,  303,  303,  303,  303,  303,
 /*   820 */   303,  303,   71,  362,  363,  361,  303,  364,  303,  303,
 /*   830 */   303,  303,  310,  303,  303,  113,  303,   83,  303,  105,
 /*   840 */   308,  366,  303,  303,  303,  303,  113,  303,   83,  303,
 /*   850 */   105,  308,  366,  303,  303,  303,   72,  362,  363,  361,
 /*   860 */   303,  364,  303,  303,  303,  303,  310,   73,  362,  363,
 /*   870 */   361,  303,  364,  303,  113,  303,   83,  310,  105,  308,
 /*   880 */   366,  303,  303,  303,  303,  303,  303,  303,  303,  303,
 /*   890 */   303,  303,  303,  303,  303,   74,  362,  363,  361,  303,
 /*   900 */   364,  303,  303,  303,  303,  310,  303,  303,  113,  303,
 /*   910 */    83,  303,  105,  308,  366,  303,  303,  303,  303,  113,
 /*   920 */   303,   83,  303,  105,  308,  366,  303,  303,  303,   75,
 /*   930 */   362,  363,  361,  303,  364,  303,  303,  303,  303,  310,
 /*   940 */   101,  362,  363,  361,  303,  364,  303,  113,  303,   83,
 /*   950 */   310,  105,  308,  366,  303,  303,  303,  303,  303,  303,
 /*   960 */   303,  303,  303,  303,  303,  303,  303,  303,  102,  362,
 /*   970 */   363,  361,  303,  364,  303,  303,  303,  303,  310,  303,
 /*   980 */   303,  113,  303,   83,  303,  105,  308,  366,  303,  303,
 /*   990 */   303,  303,  113,  303,   83,  303,  105,  308,  366,  303,
 /*  1000 */   303,  303,  103,  362,  363,  361,  303,  364,  303,  303,
 /*  1010 */   303,  303,  310,   77,  362,  363,  361,  303,  364,  303,
 /*  1020 */   113,  303,   83,  310,  105,  308,  366,  303,  303,  303,
 /*  1030 */   303,  303,  303,  303,  303,  303,  303,  303,  303,  303,
 /*  1040 */   303,   78,  362,  363,  361,  303,  364,  303,  303,  303,
 /*  1050 */   303,  310,  303,  303,  113,  303,   83,  303,  105,  308,
 /*  1060 */   366,  303,  303,  303,  303,  113,  303,   83,  303,  105,
 /*  1070 */   308,  366,  303,  303,  303,   69,  362,  363,  361,  303,
 /*  1080 */   364,  303,  303,  303,  303,  310,   76,  362,  363,  361,
 /*  1090 */   303,  364,  303,  113,  303,   83,  310,  105,  308,  366,
 /*  1100 */   303,  303,  303,  303,  303,  303,  303,  303,  303,  303,
 /*  1110 */   303,  303,  303,  303,   63,  362,  363,  361,  303,  364,
 /*  1120 */   303,  303,  303,  303,  310,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    15,   16,   16,   17,   18,   20,   21,   26,   23,   25,
 /*    10 */    24,   26,   27,   28,   29,   30,   31,   32,   33,   34,
 /*    20 */    35,   36,   37,   38,   75,   76,   42,   51,   75,   76,
 /*    30 */    43,   46,   83,   84,   49,   51,   52,   84,   53,   54,
 /*    40 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*    50 */    65,   66,   67,   68,   15,   16,   81,   80,   81,   20,
 /*    60 */    21,   89,   23,   86,   81,   26,   27,   28,   29,   30,
 /*    70 */    31,   32,   33,   34,   35,   36,   37,   38,  101,   72,
 /*    80 */    76,   74,   72,   44,   74,  102,   89,   72,   49,   74,
 /*    90 */    23,   87,   88,   26,   27,   56,   57,   58,   59,   60,
 /*   100 */    61,   62,   63,   64,   65,   66,   67,   68,   41,   76,
 /*   110 */   103,   15,   16,  103,   76,   43,   20,   21,  103,   23,
 /*   120 */    70,   71,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   130 */    34,   35,   36,   37,   38,   76,   47,   99,  100,   73,
 /*   140 */    51,   75,   73,   82,   75,   49,   73,   73,   75,   75,
 /*   150 */    49,   23,   56,   57,   58,   59,   60,   61,   62,   63,
 /*   160 */    64,   65,   66,   67,   68,    1,    2,    3,    4,   41,
 /*   170 */    47,   48,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   180 */    16,   17,   18,    2,    3,    4,   76,   76,   24,    8,
 /*   190 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   200 */    73,   48,   75,   50,   48,   24,   50,   48,   44,   50,
 /*   210 */    43,  100,   26,   46,   28,   29,   30,   31,   32,   33,
 /*   220 */    34,   35,   36,   37,   38,   44,   47,   48,    2,    3,
 /*   230 */     4,   45,   27,   76,    8,    9,   10,   11,   12,   13,
 /*   240 */    14,   15,   16,   17,   18,    2,    3,    4,   77,   76,
 /*   250 */    24,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   260 */    17,   18,   43,   23,   76,   46,   26,   24,   85,   48,
 /*   270 */    44,   50,   76,   46,    1,   87,    1,   47,   72,   44,
 /*   280 */    74,   41,   76,   77,   78,   79,   47,   43,   24,   43,
 /*   290 */    49,   49,   23,   50,   49,   46,   90,   91,   92,   93,
 /*   300 */    94,   95,   96,   43,   98,   43,   40,   46,   72,  103,
 /*   310 */    74,   44,   76,   77,   78,   79,   39,   43,    0,  104,
 /*   320 */   104,  104,  104,  104,  104,  104,   90,   91,   92,   93,
 /*   330 */    94,   95,   96,  104,   98,    2,    3,    4,  104,  103,
 /*   340 */   104,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   350 */    17,   18,  104,  104,  104,  104,   72,   24,   74,  104,
 /*   360 */    76,   77,   78,   79,  104,    8,    9,   10,   11,   12,
 /*   370 */    13,   14,   15,   16,   17,   18,   92,   93,   94,   95,
 /*   380 */    96,   24,   98,  104,    4,  104,  104,  103,    8,    9,
 /*   390 */    10,   11,   12,   13,   14,   15,   16,   17,   18,  104,
 /*   400 */   104,   72,  104,   74,   24,   76,   77,   78,  104,  104,
 /*   410 */   104,  104,   72,  104,   74,  104,   76,   77,   78,  104,
 /*   420 */   104,  104,   93,   94,   95,   96,   97,   98,  104,  104,
 /*   430 */   104,  104,  103,   93,   94,   95,   96,   97,   98,  104,
 /*   440 */   104,  104,  104,  103,  104,  104,  104,  104,  104,   72,
 /*   450 */   104,   74,  104,   76,   77,   78,  104,  104,  104,  104,
 /*   460 */   104,  104,  104,  104,  104,  104,  104,  104,  104,  104,
 /*   470 */    93,   94,   95,   96,   97,   98,  104,  104,  104,  104,
 /*   480 */   103,   26,  104,   28,   29,   30,   31,   32,   33,   34,
 /*   490 */    35,   36,   37,   38,    8,    9,   10,   11,   12,   13,
 /*   500 */    14,   15,   16,   17,   18,  104,  104,   72,  104,   74,
 /*   510 */    24,   76,   77,   78,  104,  104,  104,  104,  104,  104,
 /*   520 */   104,  104,  104,  104,  104,  104,  104,  104,   93,   94,
 /*   530 */    95,   96,  104,   98,  104,  104,  104,  104,  103,  104,
 /*   540 */   104,  104,  104,   72,  104,   74,  104,   76,   77,   78,
 /*   550 */   104,  104,  104,  104,   72,  104,   74,  104,   76,   77,
 /*   560 */    78,  104,  104,  104,   93,   94,   95,   96,  104,   98,
 /*   570 */   104,  104,  104,  104,  103,   93,   94,   95,   96,  104,
 /*   580 */    98,  104,   72,  104,   74,  103,   76,   77,   78,  104,
 /*   590 */   104,  104,  104,  104,  104,  104,  104,  104,  104,  104,
 /*   600 */   104,  104,  104,   93,   94,   95,   96,  104,   98,  104,
 /*   610 */   104,  104,  104,  103,  104,  104,   72,  104,   74,  104,
 /*   620 */    76,   77,   78,  104,  104,  104,  104,   72,  104,   74,
 /*   630 */   104,   76,   77,   78,  104,  104,  104,   93,   94,   95,
 /*   640 */    96,  104,   98,  104,  104,  104,  104,  103,   93,   94,
 /*   650 */    95,   96,  104,   98,  104,   72,  104,   74,  103,   76,
 /*   660 */    77,   78,  104,  104,  104,  104,  104,  104,  104,  104,
 /*   670 */   104,  104,  104,  104,  104,  104,   93,   94,   95,   96,
 /*   680 */   104,   98,  104,  104,  104,  104,  103,  104,  104,   72,
 /*   690 */   104,   74,  104,   76,   77,   78,  104,  104,  104,  104,
 /*   700 */    72,  104,   74,  104,   76,   77,   78,  104,  104,  104,
 /*   710 */    93,   94,   95,   96,  104,   98,  104,  104,  104,  104,
 /*   720 */   103,   93,   94,   95,   96,  104,   98,  104,   72,  104,
 /*   730 */    74,  103,   76,   77,   78,  104,  104,  104,  104,  104,
 /*   740 */   104,  104,  104,  104,  104,  104,  104,  104,  104,   93,
 /*   750 */    94,   95,   96,  104,   98,  104,  104,  104,  104,  103,
 /*   760 */   104,  104,   72,  104,   74,  104,   76,   77,   78,  104,
 /*   770 */   104,  104,  104,   72,  104,   74,  104,   76,   77,   78,
 /*   780 */   104,  104,  104,   93,   94,   95,   96,  104,   98,  104,
 /*   790 */   104,  104,  104,  103,   93,   94,   95,   96,  104,   98,
 /*   800 */   104,   72,  104,   74,  103,   76,   77,   78,  104,  104,
 /*   810 */   104,  104,  104,  104,  104,  104,  104,  104,  104,  104,
 /*   820 */   104,  104,   93,   94,   95,   96,  104,   98,  104,  104,
 /*   830 */   104,  104,  103,  104,  104,   72,  104,   74,  104,   76,
 /*   840 */    77,   78,  104,  104,  104,  104,   72,  104,   74,  104,
 /*   850 */    76,   77,   78,  104,  104,  104,   93,   94,   95,   96,
 /*   860 */   104,   98,  104,  104,  104,  104,  103,   93,   94,   95,
 /*   870 */    96,  104,   98,  104,   72,  104,   74,  103,   76,   77,
 /*   880 */    78,  104,  104,  104,  104,  104,  104,  104,  104,  104,
 /*   890 */   104,  104,  104,  104,  104,   93,   94,   95,   96,  104,
 /*   900 */    98,  104,  104,  104,  104,  103,  104,  104,   72,  104,
 /*   910 */    74,  104,   76,   77,   78,  104,  104,  104,  104,   72,
 /*   920 */   104,   74,  104,   76,   77,   78,  104,  104,  104,   93,
 /*   930 */    94,   95,   96,  104,   98,  104,  104,  104,  104,  103,
 /*   940 */    93,   94,   95,   96,  104,   98,  104,   72,  104,   74,
 /*   950 */   103,   76,   77,   78,  104,  104,  104,  104,  104,  104,
 /*   960 */   104,  104,  104,  104,  104,  104,  104,  104,   93,   94,
 /*   970 */    95,   96,  104,   98,  104,  104,  104,  104,  103,  104,
 /*   980 */   104,   72,  104,   74,  104,   76,   77,   78,  104,  104,
 /*   990 */   104,  104,   72,  104,   74,  104,   76,   77,   78,  104,
 /*  1000 */   104,  104,   93,   94,   95,   96,  104,   98,  104,  104,
 /*  1010 */   104,  104,  103,   93,   94,   95,   96,  104,   98,  104,
 /*  1020 */    72,  104,   74,  103,   76,   77,   78,  104,  104,  104,
 /*  1030 */   104,  104,  104,  104,  104,  104,  104,  104,  104,  104,
 /*  1040 */   104,   93,   94,   95,   96,  104,   98,  104,  104,  104,
 /*  1050 */   104,  103,  104,  104,   72,  104,   74,  104,   76,   77,
 /*  1060 */    78,  104,  104,  104,  104,   72,  104,   74,  104,   76,
 /*  1070 */    77,   78,  104,  104,  104,   93,   94,   95,   96,  104,
 /*  1080 */    98,  104,  104,  104,  104,  103,   93,   94,   95,   96,
 /*  1090 */   104,   98,  104,   72,  104,   74,  103,   76,   77,   78,
 /*  1100 */   104,  104,  104,  104,  104,  104,  104,  104,  104,  104,
 /*  1110 */   104,  104,  104,  104,   93,   94,   95,   96,  104,   98,
 /*  1120 */   104,  104,  104,  104,  103,
};
#define YY_SHIFT_USE_DFLT (1125)
#define YY_SHIFT_COUNT    (115)
#define YY_SHIFT_MIN      (-24)
#define YY_SHIFT_MAX      (486)
static const short yy_shift_ofst[] = {
 /*     0 */  1125,  -15,  -15,  -15,   96,   96,   96,   39,   96,   96,
 /*    10 */    96,   96,   96,   96,   96,   96,   96,   96,   96,   96,
 /*    20 */    96,   96,   96,   96,   96,   96,   96,   96,   96,   96,
 /*    30 */    96,   96,   67,  -16,  186,  455,  455,  240,  -19,  -19,
 /*    40 */   128,  128,  128,  128,  128,  -24,  -19,  -19,   89,  -13,
 /*    50 */   -19,  -13,  -19,  -19,  -19,   72,  205,  101,  -19,  -19,
 /*    60 */   164,  181,  226,  243,  333,  333,  333,  333,  333,  380,
 /*    70 */   357,  357,  357,  357,  357,  357,  486,  -14,  -14,  153,
 /*    80 */   156,  123,  159,  167,  219,  221,  179,  227,  273,  275,
 /*    90 */   230,  235,  239,  244,  264,  264,  264,  264,  264,  241,
 /*   100 */   246,  264,  264,  264,  242,  245,  249,  260,  269,  262,
 /*   110 */   261,  267,  266,  277,  274,  318,
};
#define YY_REDUCE_USE_DFLT (-52)
#define YY_REDUCE_COUNT (59)
#define YY_REDUCE_MIN   (-51)
#define YY_REDUCE_MAX   (1021)
static const short yy_reduce_ofst[] = {
 /*     0 */    50,  206,  236,  284,  329,  340,  377,  435,  471,  482,
 /*    10 */   510,  544,  555,  583,  617,  628,  656,  690,  701,  729,
 /*    20 */   763,  774,  802,  836,  847,  875,  909,  920,  948,  982,
 /*    30 */   993, 1021,  -51,  -23,    7,   10,   15,  -47,   38,    4,
 /*    40 */    66,   69,   73,   74,  127,  -17,  111,  188,  -25,  -28,
 /*    50 */    33,   -3,   59,  110,  157,   61,  171,  183,  173,  196,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   304,  346,  346,  347,  391,  391,  391,  412,  412,  412,
 /*    10 */   412,  412,  412,  412,  412,  412,  412,  412,  412,  412,
 /*    20 */   412,  412,  412,  412,  412,  412,  412,  412,  412,  412,
 /*    30 */   412,  412,  341,  412,  412,  412,  412,  323,  393,  412,
 /*    40 */   323,  323,  323,  323,  323,  412,  412,  412,  412,  359,
 /*    50 */   412,  359,  412,  412,  412,  345,  412,  412,  412,  412,
 /*    60 */   412,  412,  412,  412,  357,  356,  396,  389,  390,  372,
 /*    70 */   383,  382,  381,  380,  379,  378,  371,  374,  373,  412,
 /*    80 */   412,  412,  412,  412,  305,  412,  412,  412,  412,  412,
 /*    90 */   412,  412,  412,  412,  388,  387,  386,  385,  384,  412,
 /*   100 */   412,  377,  376,  375,  367,  307,  412,  412,  324,  412,
 /*   110 */   412,  412,  412,  309,  412,  412,
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
  YALSTParserARG_SDECL                /* A place to hold %extra_argument */
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
void YALSTParserTrace(FILE *TraceFILE, char *zTracePrompt){
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
  "$",             "ASSIGN",        "AS",            "OR",          
  "AND",           "BIT_OR",        "BIT_XOR",       "BIT_AND",     
  "EQ",            "NE",            "GT",            "LT",          
  "GE",            "LE",            "PLUS",          "MINUS",       
  "MULT",          "DIV",           "MOD",           "NEGATE",      
  "BIT_NOT",       "NOT",           "DEREF",         "REFERENCE",   
  "DOT",           "END",           "IDENTIFIER",    "SELF",        
  "TYPE_BOOL",     "TYPE_INT8",     "TYPE_UINT8",    "TYPE_INT16",  
  "TYPE_UINT16",   "TYPE_INT32",    "TYPE_UINT32",   "TYPE_INT64",  
  "TYPE_UINT64",   "TYPE_FLOAT",    "TYPE_DOUBLE",   "ARRAY_BEGIN", 
  "ARRAY_END",     "MUT",           "TYPE",          "COLON",       
  "SEMI_COLON",    "STRUCT",        "SCOPE_BEGIN",   "SCOPE_END",   
  "COMMA",         "PAR_BEGIN",     "PAR_END",       "FUNCTION",    
  "IMPL",          "RETURN",        "VAR",           "LET",         
  "INTEGER_LITERAL",  "INTEGER_LITERAL_I8",  "INTEGER_LITERAL_I16",  "INTEGER_LITERAL_I32",
  "INTEGER_LITERAL_I64",  "INTEGER_LITERAL_U8",  "INTEGER_LITERAL_U16",  "INTEGER_LITERAL_U32",
  "INTEGER_LITERAL_U64",  "FLOAT_LITERAL",  "FLOAT_LITERAL_32",  "FLOAT_LITERAL_64",
  "BOOL_LITERAL",  "error",         "decls",         "module",      
  "type_builtin",  "type_qualified",  "type_specifier",  "qualifier",   
  "identifier",    "self",          "identifier_or_self",  "decl_var",    
  "decl_type",     "decl_function",  "function_return_decl",  "function_args_decl",
  "function_arg_decl",  "function_param_list",  "decl_struct",   "struct_decl_var",
  "struct_decl_vars",  "var_type_spec",  "statement_list",  "statement_list_or_empty",
  "statement",     "expression",    "unaryexp",      "binaryexp",   
  "literal",       "function_call_args",  "struct_init",   "struct_member_init_list",
  "struct_member_init",  "decl_type_functions_scope",  "decl_type_function_list",  "type_array",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "decls ::= decls decl_type",
 /*   1 */ "decls ::= decls decl_function",
 /*   2 */ "decls ::= decls decl_type_functions_scope",
 /*   3 */ "decls ::=",
 /*   4 */ "identifier ::= IDENTIFIER",
 /*   5 */ "self ::= SELF",
 /*   6 */ "identifier_or_self ::= identifier",
 /*   7 */ "identifier_or_self ::= self",
 /*   8 */ "type_specifier ::= type_builtin",
 /*   9 */ "type_specifier ::= type_array",
 /*  10 */ "type_specifier ::= IDENTIFIER",
 /*  11 */ "type_builtin ::= TYPE_BOOL",
 /*  12 */ "type_builtin ::= TYPE_INT8",
 /*  13 */ "type_builtin ::= TYPE_UINT8",
 /*  14 */ "type_builtin ::= TYPE_INT16",
 /*  15 */ "type_builtin ::= TYPE_UINT16",
 /*  16 */ "type_builtin ::= TYPE_INT32",
 /*  17 */ "type_builtin ::= TYPE_UINT32",
 /*  18 */ "type_builtin ::= TYPE_INT64",
 /*  19 */ "type_builtin ::= TYPE_UINT64",
 /*  20 */ "type_builtin ::= TYPE_FLOAT",
 /*  21 */ "type_builtin ::= TYPE_DOUBLE",
 /*  22 */ "qualifier ::=",
 /*  23 */ "qualifier ::= MUT",
 /*  24 */ "qualifier ::= REFERENCE",
 /*  25 */ "qualifier ::= MUT REFERENCE",
 /*  26 */ "type_qualified ::= qualifier type_specifier",
 /*  27 */ "decl_type ::= TYPE identifier COLON type_specifier SEMI_COLON",
 /*  28 */ "decl_type ::= decl_struct",
 /*  29 */ "decl_struct ::= TYPE identifier COLON STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END",
 /*  30 */ "struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var",
 /*  31 */ "struct_decl_vars ::= struct_decl_var",
 /*  32 */ "struct_decl_var ::= identifier COLON type_qualified",
 /*  33 */ "function_param_list ::= PAR_BEGIN function_args_decl PAR_END",
 /*  34 */ "decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  35 */ "decl_type_functions_scope ::= IMPL type_specifier SCOPE_BEGIN decl_type_function_list SCOPE_END",
 /*  36 */ "decl_type_function_list ::= decl_type_function_list decl_function",
 /*  37 */ "decl_type_function_list ::= decl_function",
 /*  38 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  39 */ "function_args_decl ::= function_arg_decl",
 /*  40 */ "function_args_decl ::=",
 /*  41 */ "function_arg_decl ::= identifier COLON type_qualified",
 /*  42 */ "function_arg_decl ::= qualifier self",
 /*  43 */ "function_return_decl ::= COLON type_qualified",
 /*  44 */ "function_return_decl ::=",
 /*  45 */ "statement_list_or_empty ::=",
 /*  46 */ "statement_list_or_empty ::= statement_list",
 /*  47 */ "statement_list ::= statement_list statement",
 /*  48 */ "statement_list ::= statement",
 /*  49 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  50 */ "statement ::= decl_var SEMI_COLON",
 /*  51 */ "statement ::= expression SEMI_COLON",
 /*  52 */ "statement ::= RETURN expression SEMI_COLON",
 /*  53 */ "statement ::= RETURN SEMI_COLON",
 /*  54 */ "statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  55 */ "decl_var ::= VAR identifier var_type_spec ASSIGN expression",
 /*  56 */ "decl_var ::= LET identifier var_type_spec ASSIGN expression",
 /*  57 */ "var_type_spec ::= COLON type_qualified",
 /*  58 */ "var_type_spec ::=",
 /*  59 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  60 */ "expression ::= literal",
 /*  61 */ "expression ::= unaryexp",
 /*  62 */ "expression ::= binaryexp",
 /*  63 */ "expression ::= struct_init",
 /*  64 */ "expression ::= expression AS type_qualified",
 /*  65 */ "expression ::= identifier_or_self",
 /*  66 */ "expression ::= expression DOT identifier",
 /*  67 */ "expression ::= identifier PAR_BEGIN function_call_args PAR_END",
 /*  68 */ "expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END",
 /*  69 */ "expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END",
 /*  70 */ "binaryexp ::= expression AND expression",
 /*  71 */ "binaryexp ::= expression OR expression",
 /*  72 */ "binaryexp ::= expression PLUS expression",
 /*  73 */ "binaryexp ::= expression MINUS expression",
 /*  74 */ "binaryexp ::= expression MULT expression",
 /*  75 */ "binaryexp ::= expression DIV expression",
 /*  76 */ "binaryexp ::= expression MOD expression",
 /*  77 */ "binaryexp ::= expression EQ expression",
 /*  78 */ "binaryexp ::= expression NE expression",
 /*  79 */ "binaryexp ::= expression LE expression",
 /*  80 */ "binaryexp ::= expression LT expression",
 /*  81 */ "binaryexp ::= expression GE expression",
 /*  82 */ "binaryexp ::= expression GT expression",
 /*  83 */ "unaryexp ::= NOT expression",
 /*  84 */ "unaryexp ::= BIT_NOT expression",
 /*  85 */ "unaryexp ::= MINUS expression",
 /*  86 */ "unaryexp ::= REFERENCE expression",
 /*  87 */ "unaryexp ::= MULT expression",
 /*  88 */ "function_call_args ::= function_call_args COMMA expression",
 /*  89 */ "function_call_args ::= expression",
 /*  90 */ "function_call_args ::=",
 /*  91 */ "struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END",
 /*  92 */ "struct_member_init_list ::=",
 /*  93 */ "struct_member_init_list ::= struct_member_init_list COMMA struct_member_init",
 /*  94 */ "struct_member_init_list ::= struct_member_init",
 /*  95 */ "struct_member_init ::= identifier COLON expression",
 /*  96 */ "literal ::= INTEGER_LITERAL",
 /*  97 */ "literal ::= INTEGER_LITERAL_I8",
 /*  98 */ "literal ::= INTEGER_LITERAL_I16",
 /*  99 */ "literal ::= INTEGER_LITERAL_I32",
 /* 100 */ "literal ::= INTEGER_LITERAL_I64",
 /* 101 */ "literal ::= INTEGER_LITERAL_U8",
 /* 102 */ "literal ::= INTEGER_LITERAL_U16",
 /* 103 */ "literal ::= INTEGER_LITERAL_U32",
 /* 104 */ "literal ::= INTEGER_LITERAL_U64",
 /* 105 */ "literal ::= FLOAT_LITERAL",
 /* 106 */ "literal ::= FLOAT_LITERAL_32",
 /* 107 */ "literal ::= FLOAT_LITERAL_64",
 /* 108 */ "literal ::= BOOL_LITERAL",
 /* 109 */ "module ::= decls END",
 /* 110 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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
** second argument to YALSTParserAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void YALSTParserInit(void *yypParser){
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

#ifndef YALSTParser_ENGINEALWAYSONSTACK
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
** to YALSTParser and YALSTParserFree.
*/
void *YALSTParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ) YALSTParserInit(pParser);
  return pParser;
}
#endif /* YALSTParser_ENGINEALWAYSONSTACK */


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
  YALSTParserARG_FETCH;
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
void YALSTParserFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef YALSTParser_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void YALSTParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  YALSTParserFinalize(p);
  (*freeProc)(p);
}
#endif /* YALSTParser_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int YALSTParserStackPeak(void *p){
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
   YALSTParserARG_FETCH;
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
   YALSTParserARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  YALSTParserTOKENTYPE yyMinor        /* The minor token to shift in */
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
  { 70, -2 },
  { 70, -2 },
  { 70, -2 },
  { 70, 0 },
  { 76, -1 },
  { 77, -1 },
  { 78, -1 },
  { 78, -1 },
  { 74, -1 },
  { 74, -1 },
  { 74, -1 },
  { 72, -1 },
  { 72, -1 },
  { 72, -1 },
  { 72, -1 },
  { 72, -1 },
  { 72, -1 },
  { 72, -1 },
  { 72, -1 },
  { 72, -1 },
  { 72, -1 },
  { 72, -1 },
  { 75, 0 },
  { 75, -1 },
  { 75, -1 },
  { 75, -2 },
  { 73, -2 },
  { 80, -5 },
  { 80, -1 },
  { 86, -7 },
  { 88, -3 },
  { 88, -1 },
  { 87, -3 },
  { 85, -3 },
  { 81, -7 },
  { 101, -5 },
  { 102, -2 },
  { 102, -1 },
  { 83, -3 },
  { 83, -1 },
  { 83, 0 },
  { 84, -3 },
  { 84, -2 },
  { 82, -2 },
  { 82, 0 },
  { 91, 0 },
  { 91, -1 },
  { 90, -2 },
  { 90, -1 },
  { 92, -4 },
  { 92, -2 },
  { 92, -2 },
  { 92, -3 },
  { 92, -2 },
  { 92, -3 },
  { 79, -5 },
  { 79, -5 },
  { 89, -2 },
  { 89, 0 },
  { 93, -3 },
  { 93, -1 },
  { 93, -1 },
  { 93, -1 },
  { 93, -1 },
  { 93, -3 },
  { 93, -1 },
  { 93, -3 },
  { 93, -4 },
  { 93, -6 },
  { 93, -7 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 94, -2 },
  { 94, -2 },
  { 94, -2 },
  { 94, -2 },
  { 94, -2 },
  { 97, -3 },
  { 97, -1 },
  { 97, 0 },
  { 98, -4 },
  { 99, 0 },
  { 99, -3 },
  { 99, -1 },
  { 100, -3 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 71, -2 },
  { 103, -3 },
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
  YALSTParserARG_FETCH;
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
#line 123 "stparserimpl.lemon"
{
    yylhsminor.yy62 =yymsp[-1].minor.yy62; yymsp[-1].minor.yy62->addDecl(yymsp[0].minor.yy92);
}
#line 1366 "stparserimpl.c"
  yymsp[-1].minor.yy62 = yylhsminor.yy62;
        break;
      case 1: /* decls ::= decls decl_function */
#line 126 "stparserimpl.lemon"
{
        yylhsminor.yy62 =yymsp[-1].minor.yy62; yymsp[-1].minor.yy62->addDecl(yymsp[0].minor.yy182);
}
#line 1374 "stparserimpl.c"
  yymsp[-1].minor.yy62 = yylhsminor.yy62;
        break;
      case 2: /* decls ::= decls decl_type_functions_scope */
#line 129 "stparserimpl.lemon"
{
    yylhsminor.yy62 =yymsp[-1].minor.yy62; yymsp[-1].minor.yy62->addDecl(yymsp[0].minor.yy123);
}
#line 1382 "stparserimpl.c"
  yymsp[-1].minor.yy62 = yylhsminor.yy62;
        break;
      case 3: /* decls ::= */
#line 132 "stparserimpl.lemon"
{
        yymsp[1].minor.yy62 =  pParser->getDeclModule();
}
#line 1390 "stparserimpl.c"
        break;
      case 4: /* identifier ::= IDENTIFIER */
      case 5: /* self ::= SELF */ yytestcase(yyruleno==5);
#line 139 "stparserimpl.lemon"
{
        yylhsminor.yy203 = pParser->createNode<yal::frontend::STIdentifier>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy203->setSourceInfo(srcInfo);
}
#line 1400 "stparserimpl.c"
  yymsp[0].minor.yy203 = yylhsminor.yy203;
        break;
      case 6: /* identifier_or_self ::= identifier */
      case 7: /* identifier_or_self ::= self */ yytestcase(yyruleno==7);
#line 151 "stparserimpl.lemon"
{yylhsminor.yy203=yymsp[0].minor.yy203;}
#line 1407 "stparserimpl.c"
  yymsp[0].minor.yy203 = yylhsminor.yy203;
        break;
      case 8: /* type_specifier ::= type_builtin */
#line 154 "stparserimpl.lemon"
{
        yylhsminor.yy82=yymsp[0].minor.yy82;
}
#line 1415 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 9: /* type_specifier ::= type_array */
#line 158 "stparserimpl.lemon"
{yymsp[0].minor.yy82 = nullptr;}
#line 1421 "stparserimpl.c"
        break;
      case 10: /* type_specifier ::= IDENTIFIER */
#line 159 "stparserimpl.lemon"
{
    yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yymsp[0].minor.yy0.tokenStr);
    yylhsminor.yy82->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0,yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1429 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 11: /* type_builtin ::= TYPE_BOOL */
#line 164 "stparserimpl.lemon"
{
    yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Bool);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1439 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 12: /* type_builtin ::= TYPE_INT8 */
#line 169 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1449 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 13: /* type_builtin ::= TYPE_UINT8 */
#line 174 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1459 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 14: /* type_builtin ::= TYPE_INT16 */
#line 179 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1469 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 15: /* type_builtin ::= TYPE_UINT16 */
#line 184 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1479 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 16: /* type_builtin ::= TYPE_INT32 */
#line 189 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1489 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 17: /* type_builtin ::= TYPE_UINT32 */
#line 194 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1499 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 18: /* type_builtin ::= TYPE_INT64 */
#line 199 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1509 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 19: /* type_builtin ::= TYPE_UINT64 */
#line 204 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1519 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 20: /* type_builtin ::= TYPE_FLOAT */
#line 209 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1529 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 21: /* type_builtin ::= TYPE_DOUBLE */
#line 214 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1539 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 22: /* qualifier ::= */
#line 222 "stparserimpl.lemon"
{yymsp[1].minor.yy37 = 0;}
#line 1545 "stparserimpl.c"
        break;
      case 23: /* qualifier ::= MUT */
#line 223 "stparserimpl.lemon"
{yymsp[0].minor.yy37 = yal::frontend::STQualType::Qualifiers::kQualMutable;}
#line 1550 "stparserimpl.c"
        break;
      case 24: /* qualifier ::= REFERENCE */
#line 224 "stparserimpl.lemon"
{yymsp[0].minor.yy37 = yal::frontend::STQualType::Qualifiers::kQualReference;}
#line 1555 "stparserimpl.c"
        break;
      case 25: /* qualifier ::= MUT REFERENCE */
#line 225 "stparserimpl.lemon"
{
        yymsp[-1].minor.yy37 = yal::frontend::STQualType::Qualifiers::kQualReference | yal::frontend::STQualType::Qualifiers::kQualMutable;
 }
#line 1562 "stparserimpl.c"
        break;
      case 26: /* type_qualified ::= qualifier type_specifier */
#line 228 "stparserimpl.lemon"
{
    yylhsminor.yy185 = pParser->createNode<yal::frontend::STQualType>(yymsp[0].minor.yy82, yymsp[-1].minor.yy37);
}
#line 1569 "stparserimpl.c"
  yymsp[-1].minor.yy185 = yylhsminor.yy185;
        break;
      case 27: /* decl_type ::= TYPE identifier COLON type_specifier SEMI_COLON */
#line 234 "stparserimpl.lemon"
{
    yymsp[-4].minor.yy92 = nullptr;
}
#line 1577 "stparserimpl.c"
        break;
      case 28: /* decl_type ::= decl_struct */
#line 238 "stparserimpl.lemon"
{ yylhsminor.yy92 = yymsp[0].minor.yy38;}
#line 1582 "stparserimpl.c"
  yymsp[0].minor.yy92 = yylhsminor.yy92;
        break;
      case 29: /* decl_struct ::= TYPE identifier COLON STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END */
#line 243 "stparserimpl.lemon"
{
    yylhsminor.yy38 = pParser->createNode<yal::frontend::STDeclStruct>(yymsp[-5].minor.yy203, yymsp[-1].minor.yy39, *pParser);
    auto srcInfo = CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 1592 "stparserimpl.c"
  yymsp[-6].minor.yy38 = yylhsminor.yy38;
        break;
      case 30: /* struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
#line 249 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy39 = list.add(yymsp[-2].minor.yy39, yymsp[0].minor.yy85);
}
#line 1601 "stparserimpl.c"
  yymsp[-2].minor.yy39 = yylhsminor.yy39;
        break;
      case 31: /* struct_decl_vars ::= struct_decl_var */
#line 253 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy39 = list.add(yymsp[0].minor.yy85);
}
#line 1610 "stparserimpl.c"
  yymsp[0].minor.yy39 = yylhsminor.yy39;
        break;
      case 32: /* struct_decl_var ::= identifier COLON type_qualified */
#line 258 "stparserimpl.lemon"
{
    yylhsminor.yy85 = pParser->createNode<yal::frontend::STStructMember>(yymsp[-2].minor.yy203, yymsp[0].minor.yy185);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy203->getSourceInfo(), yymsp[0].minor.yy185->getSourceInfo(),pParser->getSourceHandle());
    yylhsminor.yy85->setSourceInfo(srcInfo);
}
#line 1620 "stparserimpl.c"
  yymsp[-2].minor.yy85 = yylhsminor.yy85;
        break;
      case 33: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
#line 267 "stparserimpl.lemon"
{
    yymsp[-2].minor.yy36 = yymsp[-1].minor.yy36;
}
#line 1628 "stparserimpl.c"
        break;
      case 34: /* decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 272 "stparserimpl.lemon"
{
    yylhsminor.yy182 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy203, yymsp[-3].minor.yy185, yymsp[-4].minor.yy36, yymsp[-1].minor.yy199, *pParser);
    yylhsminor.yy182->setSourceInfo(CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1636 "stparserimpl.c"
  yymsp[-6].minor.yy182 = yylhsminor.yy182;
        break;
      case 35: /* decl_type_functions_scope ::= IMPL type_specifier SCOPE_BEGIN decl_type_function_list SCOPE_END */
#line 279 "stparserimpl.lemon"
{
        yylhsminor.yy123 = pParser->createNode<yal::frontend::STDeclTypeFunctions>(yymsp[-3].minor.yy82,yymsp[-1].minor.yy83, *pParser);
        yylhsminor.yy123->setSourceInfo(CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1645 "stparserimpl.c"
  yymsp[-4].minor.yy123 = yylhsminor.yy123;
        break;
      case 36: /* decl_type_function_list ::= decl_type_function_list decl_function */
#line 284 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclList();
    yylhsminor.yy83 = list.add(yymsp[-1].minor.yy83, yymsp[0].minor.yy182);
}
#line 1654 "stparserimpl.c"
  yymsp[-1].minor.yy83 = yylhsminor.yy83;
        break;
      case 37: /* decl_type_function_list ::= decl_function */
#line 289 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclList();
    yylhsminor.yy83 = list.add(yymsp[0].minor.yy182);
}
#line 1663 "stparserimpl.c"
  yymsp[0].minor.yy83 = yylhsminor.yy83;
        break;
      case 38: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
#line 297 "stparserimpl.lemon"
{
        auto& list = pParser->getDeclParamList();
        yylhsminor.yy36 = list.add(yymsp[-2].minor.yy36, yymsp[0].minor.yy165);
}
#line 1672 "stparserimpl.c"
  yymsp[-2].minor.yy36 = yylhsminor.yy36;
        break;
      case 39: /* function_args_decl ::= function_arg_decl */
#line 302 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclParamList();
    yylhsminor.yy36 = list.add(yymsp[0].minor.yy165);
}
#line 1681 "stparserimpl.c"
  yymsp[0].minor.yy36 = yylhsminor.yy36;
        break;
      case 40: /* function_args_decl ::= */
#line 307 "stparserimpl.lemon"
{
   yymsp[1].minor.yy36 = pParser->getDeclParamList().getRangeEmpty();
}
#line 1689 "stparserimpl.c"
        break;
      case 41: /* function_arg_decl ::= identifier COLON type_qualified */
#line 311 "stparserimpl.lemon"
{
        yylhsminor.yy165 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[-2].minor.yy203, yymsp[0].minor.yy185);
        yylhsminor.yy165->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy203->getSourceInfo(),yymsp[0].minor.yy185->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1697 "stparserimpl.c"
  yymsp[-2].minor.yy165 = yylhsminor.yy165;
        break;
      case 42: /* function_arg_decl ::= qualifier self */
#line 316 "stparserimpl.lemon"
{
        auto qualType = pParser->createNode<yal::frontend::STQualType>(nullptr, yymsp[-1].minor.yy37);
        yylhsminor.yy165 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[0].minor.yy203, qualType);
        yylhsminor.yy165->setSourceInfo(yymsp[0].minor.yy203->getSourceInfo());
}
#line 1707 "stparserimpl.c"
  yymsp[-1].minor.yy165 = yylhsminor.yy165;
        break;
      case 43: /* function_return_decl ::= COLON type_qualified */
#line 322 "stparserimpl.lemon"
{ yymsp[-1].minor.yy185 = yymsp[0].minor.yy185;}
#line 1713 "stparserimpl.c"
        break;
      case 44: /* function_return_decl ::= */
      case 58: /* var_type_spec ::= */ yytestcase(yyruleno==58);
#line 323 "stparserimpl.lemon"
{yymsp[1].minor.yy185 = nullptr;}
#line 1719 "stparserimpl.c"
        break;
      case 45: /* statement_list_or_empty ::= */
#line 327 "stparserimpl.lemon"
{yymsp[1].minor.yy199 = pParser->getStmtList().getRangeEmpty();}
#line 1724 "stparserimpl.c"
        break;
      case 46: /* statement_list_or_empty ::= statement_list */
#line 328 "stparserimpl.lemon"
{yylhsminor.yy199 = yymsp[0].minor.yy199;}
#line 1729 "stparserimpl.c"
  yymsp[0].minor.yy199 = yylhsminor.yy199;
        break;
      case 47: /* statement_list ::= statement_list statement */
#line 331 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy199 = list.add(yymsp[-1].minor.yy199,yymsp[0].minor.yy49);
}
#line 1738 "stparserimpl.c"
  yymsp[-1].minor.yy199 = yylhsminor.yy199;
        break;
      case 48: /* statement_list ::= statement */
#line 335 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy199 = list.add(yymsp[0].minor.yy49);
}
#line 1747 "stparserimpl.c"
  yymsp[0].minor.yy199 = yylhsminor.yy199;
        break;
      case 49: /* statement ::= expression ASSIGN expression SEMI_COLON */
#line 341 "stparserimpl.lemon"
{
   yylhsminor.yy49 = pParser->createNode<yal::frontend::STStmtAssign>(yymsp[-3].minor.yy152,yymsp[-1].minor.yy152);
   yylhsminor.yy49->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy152->getSourceInfo(), yymsp[-1].minor.yy152->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1756 "stparserimpl.c"
  yymsp[-3].minor.yy49 = yylhsminor.yy49;
        break;
      case 50: /* statement ::= decl_var SEMI_COLON */
#line 345 "stparserimpl.lemon"
{
    yylhsminor.yy49 = pParser->createNode<yal::frontend::STStmtDecl>(yymsp[-1].minor.yy25);
    yylhsminor.yy49->setSourceInfo(yymsp[-1].minor.yy25->getSourceInfo());
}
#line 1765 "stparserimpl.c"
  yymsp[-1].minor.yy49 = yylhsminor.yy49;
        break;
      case 51: /* statement ::= expression SEMI_COLON */
#line 350 "stparserimpl.lemon"
{
    yylhsminor.yy49 = pParser->createNode<yal::frontend::STStmtExpression>(yymsp[-1].minor.yy152);
    yylhsminor.yy49->setSourceInfo(yymsp[-1].minor.yy152->getSourceInfo());
}
#line 1774 "stparserimpl.c"
  yymsp[-1].minor.yy49 = yylhsminor.yy49;
        break;
      case 52: /* statement ::= RETURN expression SEMI_COLON */
#line 355 "stparserimpl.lemon"
{
    yylhsminor.yy49 = pParser->createNode<yal::frontend::STStmtReturn>(yymsp[-1].minor.yy152);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy49->setSourceInfo(srcInfo);
}
#line 1784 "stparserimpl.c"
  yymsp[-2].minor.yy49 = yylhsminor.yy49;
        break;
      case 53: /* statement ::= RETURN SEMI_COLON */
#line 361 "stparserimpl.lemon"
{
    yylhsminor.yy49 = pParser->createNode<yal::frontend::STStmtReturn>(nullptr);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[-1].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy49->setSourceInfo(srcInfo);
}
#line 1794 "stparserimpl.c"
  yymsp[-1].minor.yy49 = yylhsminor.yy49;
        break;
      case 54: /* statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 367 "stparserimpl.lemon"
{
    yylhsminor.yy49 = pParser->createNode<yal::frontend::STStmtListScoped>(yymsp[-1].minor.yy199, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy49->setSourceInfo(srcInfo);
}
#line 1804 "stparserimpl.c"
  yymsp[-2].minor.yy49 = yylhsminor.yy49;
        break;
      case 55: /* decl_var ::= VAR identifier var_type_spec ASSIGN expression */
#line 375 "stparserimpl.lemon"
{
    yylhsminor.yy25 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy203, yymsp[-2].minor.yy185, yymsp[0].minor.yy152, false);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy25->setSourceInfo(srcInfo);
}
#line 1814 "stparserimpl.c"
  yymsp[-4].minor.yy25 = yylhsminor.yy25;
        break;
      case 56: /* decl_var ::= LET identifier var_type_spec ASSIGN expression */
#line 381 "stparserimpl.lemon"
{
    yylhsminor.yy25 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy203, yymsp[-2].minor.yy185, yymsp[0].minor.yy152, true);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy25->setSourceInfo(srcInfo);
}
#line 1824 "stparserimpl.c"
  yymsp[-4].minor.yy25 = yylhsminor.yy25;
        break;
      case 57: /* var_type_spec ::= COLON type_qualified */
#line 387 "stparserimpl.lemon"
{yymsp[-1].minor.yy185 = yymsp[0].minor.yy185;}
#line 1830 "stparserimpl.c"
        break;
      case 59: /* expression ::= PAR_BEGIN expression PAR_END */
#line 393 "stparserimpl.lemon"
{yymsp[-2].minor.yy152 = yymsp[-1].minor.yy152;}
#line 1835 "stparserimpl.c"
        break;
      case 60: /* expression ::= literal */
#line 394 "stparserimpl.lemon"
{yylhsminor.yy152 = yymsp[0].minor.yy152;}
#line 1840 "stparserimpl.c"
  yymsp[0].minor.yy152 = yylhsminor.yy152;
        break;
      case 61: /* expression ::= unaryexp */
#line 395 "stparserimpl.lemon"
{yylhsminor.yy152 = yymsp[0].minor.yy70;}
#line 1846 "stparserimpl.c"
  yymsp[0].minor.yy152 = yylhsminor.yy152;
        break;
      case 62: /* expression ::= binaryexp */
#line 396 "stparserimpl.lemon"
{yylhsminor.yy152 = yymsp[0].minor.yy140;}
#line 1852 "stparserimpl.c"
  yymsp[0].minor.yy152 = yylhsminor.yy152;
        break;
      case 63: /* expression ::= struct_init */
#line 397 "stparserimpl.lemon"
{ yylhsminor.yy152 = yymsp[0].minor.yy156;}
#line 1858 "stparserimpl.c"
  yymsp[0].minor.yy152 = yylhsminor.yy152;
        break;
      case 64: /* expression ::= expression AS type_qualified */
#line 399 "stparserimpl.lemon"
{
    yylhsminor.yy152 = pParser->createNode<yal::frontend::STExprCast>(yymsp[-2].minor.yy152, yymsp[0].minor.yy185);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(),
                                                   yymsp[0].minor.yy185->getSourceInfo(),
                                                   pParser->getSourceHandle());
    yylhsminor.yy152->setSourceInfo(srcInfo);
}
#line 1870 "stparserimpl.c"
  yymsp[-2].minor.yy152 = yylhsminor.yy152;
        break;
      case 65: /* expression ::= identifier_or_self */
#line 407 "stparserimpl.lemon"
{
    yylhsminor.yy152 = pParser->createNode<yal::frontend::STExprVarRef>(yymsp[0].minor.yy203);
    yylhsminor.yy152->setSourceInfo(yymsp[0].minor.yy203->getSourceInfo());
}
#line 1879 "stparserimpl.c"
  yymsp[0].minor.yy152 = yylhsminor.yy152;
        break;
      case 66: /* expression ::= expression DOT identifier */
#line 412 "stparserimpl.lemon"
{
    yylhsminor.yy152 = pParser->createNode<yal::frontend::STExprStructVarRef>(yymsp[-2].minor.yy152, yymsp[0].minor.yy203);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy203->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy152->setSourceInfo(srcInfo);
}
#line 1889 "stparserimpl.c"
  yymsp[-2].minor.yy152 = yylhsminor.yy152;
        break;
      case 67: /* expression ::= identifier PAR_BEGIN function_call_args PAR_END */
#line 418 "stparserimpl.lemon"
{
    yylhsminor.yy152 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy203,yymsp[-1].minor.yy102,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy203->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy152->setSourceInfo(srcInfo);
}
#line 1899 "stparserimpl.c"
  yymsp[-3].minor.yy152 = yylhsminor.yy152;
        break;
      case 68: /* expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END */
#line 424 "stparserimpl.lemon"
{
    yylhsminor.yy152 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy203,yymsp[-5].minor.yy152,yymsp[-1].minor.yy102,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-5].minor.yy152->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy152->setSourceInfo(srcInfo);
}
#line 1909 "stparserimpl.c"
  yymsp[-5].minor.yy152 = yylhsminor.yy152;
        break;
      case 69: /* expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END */
#line 430 "stparserimpl.lemon"
{
    yylhsminor.yy152 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy203,yymsp[-6].minor.yy82,yymsp[-1].minor.yy102,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy82->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy152->setSourceInfo(srcInfo);
}
#line 1919 "stparserimpl.c"
  yymsp[-6].minor.yy152 = yylhsminor.yy152;
        break;
      case 70: /* binaryexp ::= expression AND expression */
#line 438 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1930 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 71: /* binaryexp ::= expression OR expression */
#line 444 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1941 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 72: /* binaryexp ::= expression PLUS expression */
#line 450 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1952 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 73: /* binaryexp ::= expression MINUS expression */
#line 456 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1963 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 74: /* binaryexp ::= expression MULT expression */
#line 462 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1974 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 75: /* binaryexp ::= expression DIV expression */
#line 468 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
}
#line 1983 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 76: /* binaryexp ::= expression MOD expression */
#line 472 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1994 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 77: /* binaryexp ::= expression EQ expression */
#line 479 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 2005 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 78: /* binaryexp ::= expression NE expression */
#line 486 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 2016 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 79: /* binaryexp ::= expression LE expression */
#line 493 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 2027 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 80: /* binaryexp ::= expression LT expression */
#line 500 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 2038 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 81: /* binaryexp ::= expression GE expression */
#line 507 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 2049 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 82: /* binaryexp ::= expression GT expression */
#line 514 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy152, yymsp[0].minor.yy152);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy152->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 2060 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 83: /* unaryexp ::= NOT expression */
#line 523 "stparserimpl.lemon"
{
    yylhsminor.yy70 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy152);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy70->setSourceInfo(srcInfo);
}
#line 2071 "stparserimpl.c"
  yymsp[-1].minor.yy70 = yylhsminor.yy70;
        break;
      case 84: /* unaryexp ::= BIT_NOT expression */
#line 529 "stparserimpl.lemon"
{
    yylhsminor.yy70 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy152);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy70->setSourceInfo(srcInfo);
}
#line 2082 "stparserimpl.c"
  yymsp[-1].minor.yy70 = yylhsminor.yy70;
        break;
      case 85: /* unaryexp ::= MINUS expression */
#line 535 "stparserimpl.lemon"
{
    yylhsminor.yy70 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy152);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy70->setSourceInfo(srcInfo);
}
#line 2093 "stparserimpl.c"
  yymsp[-1].minor.yy70 = yylhsminor.yy70;
        break;
      case 86: /* unaryexp ::= REFERENCE expression */
#line 542 "stparserimpl.lemon"
{
    yylhsminor.yy70 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy152);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy70->setSourceInfo(srcInfo);
}
#line 2104 "stparserimpl.c"
  yymsp[-1].minor.yy70 = yylhsminor.yy70;
        break;
      case 87: /* unaryexp ::= MULT expression */
#line 549 "stparserimpl.lemon"
{
    yylhsminor.yy70 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Derefence,
                                               yymsp[0].minor.yy152);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy70->setSourceInfo(srcInfo);
}
#line 2115 "stparserimpl.c"
  yymsp[-1].minor.yy70 = yylhsminor.yy70;
        break;
      case 88: /* function_call_args ::= function_call_args COMMA expression */
#line 559 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy102 = list.add(yymsp[-2].minor.yy102, yymsp[0].minor.yy152);
}
#line 2124 "stparserimpl.c"
  yymsp[-2].minor.yy102 = yylhsminor.yy102;
        break;
      case 89: /* function_call_args ::= expression */
#line 563 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy102 = list.add(yymsp[0].minor.yy152);
}
#line 2133 "stparserimpl.c"
  yymsp[0].minor.yy102 = yylhsminor.yy102;
        break;
      case 90: /* function_call_args ::= */
#line 567 "stparserimpl.lemon"
{
    yymsp[1].minor.yy102= pParser->getExprList().getRangeEmpty();
}
#line 2141 "stparserimpl.c"
        break;
      case 91: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
#line 574 "stparserimpl.lemon"
{
    yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprStructInit>(yymsp[-3].minor.yy82, yymsp[-1].minor.yy76, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy82->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 2150 "stparserimpl.c"
  yymsp[-3].minor.yy156 = yylhsminor.yy156;
        break;
      case 92: /* struct_member_init_list ::= */
#line 580 "stparserimpl.lemon"
{yymsp[1].minor.yy76 = pParser->getStructInitList().getRangeEmpty();}
#line 2156 "stparserimpl.c"
        break;
      case 93: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
#line 581 "stparserimpl.lemon"
{
     auto& list = pParser->getStructInitList();
     yylhsminor.yy76 = list.add(yymsp[-2].minor.yy76, yymsp[0].minor.yy41);
}
#line 2164 "stparserimpl.c"
  yymsp[-2].minor.yy76 = yylhsminor.yy76;
        break;
      case 94: /* struct_member_init_list ::= struct_member_init */
#line 585 "stparserimpl.lemon"
{
    auto& list = pParser->getStructInitList();
    yylhsminor.yy76 = list.add(yymsp[0].minor.yy41);
}
#line 2173 "stparserimpl.c"
  yymsp[0].minor.yy76 = yylhsminor.yy76;
        break;
      case 95: /* struct_member_init ::= identifier COLON expression */
#line 590 "stparserimpl.lemon"
{
    yylhsminor.yy41 = pParser->createNode<yal::frontend::STStructMemberInit>(yymsp[-2].minor.yy203,yymsp[0].minor.yy152);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy203->getSourceInfo(), yymsp[0].minor.yy152->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy41->setSourceInfo(srcInfo);
}
#line 2183 "stparserimpl.c"
  yymsp[-2].minor.yy41 = yylhsminor.yy41;
        break;
      case 96: /* literal ::= INTEGER_LITERAL */
      case 97: /* literal ::= INTEGER_LITERAL_I8 */ yytestcase(yyruleno==97);
      case 98: /* literal ::= INTEGER_LITERAL_I16 */ yytestcase(yyruleno==98);
      case 99: /* literal ::= INTEGER_LITERAL_I32 */ yytestcase(yyruleno==99);
      case 100: /* literal ::= INTEGER_LITERAL_I64 */ yytestcase(yyruleno==100);
      case 101: /* literal ::= INTEGER_LITERAL_U8 */ yytestcase(yyruleno==101);
      case 102: /* literal ::= INTEGER_LITERAL_U16 */ yytestcase(yyruleno==102);
      case 103: /* literal ::= INTEGER_LITERAL_U32 */ yytestcase(yyruleno==103);
      case 104: /* literal ::= INTEGER_LITERAL_U64 */ yytestcase(yyruleno==104);
#line 598 "stparserimpl.lemon"
{
        yylhsminor.yy152 = pParser->createNode<yal::frontend::STExprIntegerLiteral>(yymsp[0].minor.yy0);
        auto srcInfo =yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy152->setSourceInfo(srcInfo);
   }
#line 2201 "stparserimpl.c"
  yymsp[0].minor.yy152 = yylhsminor.yy152;
        break;
      case 105: /* literal ::= FLOAT_LITERAL */
      case 106: /* literal ::= FLOAT_LITERAL_32 */ yytestcase(yyruleno==106);
      case 107: /* literal ::= FLOAT_LITERAL_64 */ yytestcase(yyruleno==107);
#line 643 "stparserimpl.lemon"
{
        yylhsminor.yy152 = pParser->createNode<yal::frontend::STExprFloatLiteral>(yymsp[0].minor.yy0);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy152->setSourceInfo(srcInfo);
}
#line 2213 "stparserimpl.c"
  yymsp[0].minor.yy152 = yylhsminor.yy152;
        break;
      case 108: /* literal ::= BOOL_LITERAL */
#line 660 "stparserimpl.lemon"
{
        yylhsminor.yy152 = pParser->createNode<yal::frontend::STExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy152->setSourceInfo(srcInfo);
}
#line 2223 "stparserimpl.c"
  yymsp[0].minor.yy152 = yylhsminor.yy152;
        break;
      default:
      /* (109) module ::= decls END */ yytestcase(yyruleno==109);
      /* (110) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==110);
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
  YALSTParserARG_FETCH;
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
  YALSTParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YALSTParserTOKENTYPE yyminor         /* The minor type of the error token */
){
  YALSTParserARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 40 "stparserimpl.lemon"


    yal::frontend::STParser::OnParseError(*pParser);
    {
#if defined(YAL_DEBUG)
        /*
        yal::frontend::Log::MultiLineScope multiLineScope(pParser->getLog());
        pParser->getLog().error("Expected one of the following tokens:\n");
        int n = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
        for (int i = 0; i < n; ++i) {
            int a = yy_find_shift_action(yypParser, (YYCODETYPE)i);
            if (a < YYNSTATE + YYNRULE) {
                pParser->getLog().error("%\n",yyTokenName[i]);
            }
        }*/
#endif
    }
#line 2307 "stparserimpl.c"
/************ End %syntax_error code ******************************************/
  YALSTParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  YALSTParserARG_FETCH;
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
  YALSTParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "YALSTParserAlloc" which describes the current state of the parser.
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
void YALSTParser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  YALSTParserTOKENTYPE yyminor       /* The value for the token */
  YALSTParserARG_PDECL               /* Optional %extra_argument parameter */
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
  YALSTParserARG_STORE;

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
