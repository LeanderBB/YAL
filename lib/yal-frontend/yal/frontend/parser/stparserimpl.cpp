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
#define YAL_TOKEN_ALIAS                          43
#define YAL_TOKEN_SEMI_COLON                     44
#define YAL_TOKEN_FROM                           45
#define YAL_TOKEN_STRUCT                         46
#define YAL_TOKEN_SCOPE_BEGIN                    47
#define YAL_TOKEN_SCOPE_END                      48
#define YAL_TOKEN_COMMA                          49
#define YAL_TOKEN_COLON                          50
#define YAL_TOKEN_PAR_BEGIN                      51
#define YAL_TOKEN_PAR_END                        52
#define YAL_TOKEN_FUNCTION                       53
#define YAL_TOKEN_IMPL                           54
#define YAL_TOKEN_RETURN                         55
#define YAL_TOKEN_VAR                            56
#define YAL_TOKEN_LET                            57
#define YAL_TOKEN_INTEGER_LITERAL                58
#define YAL_TOKEN_INTEGER_LITERAL_I8             59
#define YAL_TOKEN_INTEGER_LITERAL_I16            60
#define YAL_TOKEN_INTEGER_LITERAL_I32            61
#define YAL_TOKEN_INTEGER_LITERAL_I64            62
#define YAL_TOKEN_INTEGER_LITERAL_U8             63
#define YAL_TOKEN_INTEGER_LITERAL_U16            64
#define YAL_TOKEN_INTEGER_LITERAL_U32            65
#define YAL_TOKEN_INTEGER_LITERAL_U64            66
#define YAL_TOKEN_FLOAT_LITERAL                  67
#define YAL_TOKEN_FLOAT_LITERAL_32               68
#define YAL_TOKEN_FLOAT_LITERAL_64               69
#define YAL_TOKEN_BOOL_LITERAL                   70
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
#define YYNOCODE 108
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#endif
typedef union {
  int yyinit;
  YALSTParserTOKENTYPE yy0;
  yal::frontend::ParseListDecl::Range yy7;
  yal::frontend::STDecl* yy36;
  yal::frontend::ParseListStructMember::Range yy44;
  yal::frontend::STDeclAlias* yy56;
  yal::frontend::STDeclFunction* yy57;
  yal::frontend::STType* yy68;
  yal::frontend::STDeclVar* yy73;
  yal::frontend::STStructMemberInit* yy79;
  yal::frontend::ParseListStructInit::Range yy80;
  yal::frontend::STIdentifier* yy89;
  yal::frontend::ParseListDeclParam::Range yy102;
  uint32_t yy105;
  yal::frontend::STStatement* yy106;
  yal::frontend::STStructMember* yy133;
  yal::frontend::ParseListStmt::Range yy137;
  yal::frontend::ParseListExpr::Range yy152;
  yal::frontend::STDeclStruct* yy153;
  yal::frontend::STDeclParam* yy157;
  yal::frontend::STDeclTypeFunctions* yy159;
  yal::frontend::STQualType* yy169;
  yal::frontend::STExprBinaryOperator* yy172;
  yal::frontend::STExpression* yy178;
  yal::frontend::STDeclModule* yy186;
  yal::frontend::STExprUnaryOperator* yy187;
  yal::frontend::STExprStructInit* yy188;
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
#define YYNSTATE             118
#define YYNRULE              113
#define YY_MAX_SHIFT         117
#define YY_MIN_SHIFTREDUCE   194
#define YY_MAX_SHIFTREDUCE   306
#define YY_MIN_REDUCE        307
#define YY_MAX_REDUCE        419
#define YY_ERROR_ACTION      420
#define YY_ACCEPT_ACTION     421
#define YY_NO_ACTION         422
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
#define YY_ACTTAB_COUNT (1056)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    14,   12,   26,   25,   24,   15,   16,  200,   13,  117,
 /*    10 */    55,   86,  201,  207,  208,  209,  210,  211,  212,  213,
 /*    20 */   214,  215,  216,  217,   57,  109,   60,  116,  116,   89,
 /*    30 */   335,   59,    1,   87,  348,  345,   31,   59,   34,   50,
 /*    40 */     7,   53,   51,  292,  293,  294,  295,  296,  297,  298,
 /*    50 */   299,  300,  301,  302,  303,  304,   14,   12,   42,  318,
 /*    60 */   318,   15,   16,  116,   13,  113,  346,   86,  201,  207,
 /*    70 */   208,  209,  210,  211,  212,  213,  214,  215,  216,  217,
 /*    80 */   220,   57,  109,  200,  331,  249,  116,   49,  114,   95,
 /*    90 */    90,  347,   31,   17,  220,  318,  263,  200,  110,  292,
 /*   100 */   293,  294,  295,  296,  297,  298,  299,  300,  301,  302,
 /*   110 */   303,  304,  110,   83,  403,   14,   12,  352,  318,   35,
 /*   120 */    15,   16,   37,   13,   36,  112,   86,  201,  207,  208,
 /*   130 */   209,  210,  211,  212,  213,  214,  215,  216,  217,  420,
 /*   140 */   420,  420,  420,  420,  420,   28,   27,   26,   25,   24,
 /*   150 */    91,   31,   32,  421,   52,   55,  101,  220,  292,  293,
 /*   160 */   294,  295,  296,  297,  298,  299,  300,  301,  302,  303,
 /*   170 */   304,   10,   43,   29,   30,  110,  287,   47,   23,   22,
 /*   180 */    18,   20,   19,   21,   28,   27,   26,   25,   24,   43,
 /*   190 */    29,   30,  225,   48,   55,   23,   22,   18,   20,   19,
 /*   200 */    21,   28,   27,   26,   25,   24,   41,  366,  373,   35,
 /*   210 */    35,   55,  108,   95,  247,  206,  111,  207,  208,  209,
 /*   220 */   210,  211,  212,  213,  214,  215,  216,  217,  340,   88,
 /*   230 */    17,  248,  106,  264,   43,   29,   30,  350,  402,   35,
 /*   240 */    23,   22,   18,   20,   19,   21,   28,   27,   26,   25,
 /*   250 */    24,   43,   29,   30,  201,  351,   55,   23,   22,   18,
 /*   260 */    20,   19,   21,   28,   27,   26,   25,   24,   17,   33,
 /*   270 */   341,  265,   35,   55,  111,  116,  245,   85,   56,  107,
 /*   280 */   316,  374,    8,   93,  231,  307,  339,  309,  311,   59,
 /*   290 */    58,   80,   46,  310,    3,   92,  357,   61,  370,  371,
 /*   300 */   369,  255,  372,   38,    9,  250,  229,  318,  312,  116,
 /*   310 */   246,   85,  230,  107,  316,  374,   39,   93,  319,  102,
 /*   320 */    11,  319,   55,    4,   54,    2,    5,    6,    3,   94,
 /*   330 */   357,   61,  370,  371,  369,   44,  372,   43,   29,   30,
 /*   340 */   221,  318,   45,   23,   22,   18,   20,   19,   21,   28,
 /*   350 */    27,   26,   25,   24,   40,  224,  116,  223,   85,   55,
 /*   360 */   107,  316,  374,  306,   93,  115,  418,  309,  309,  309,
 /*   370 */   309,  309,  309,  309,  309,  309,  309,  356,   61,  370,
 /*   380 */   371,  369,  309,  372,  309,   30,  309,  309,  318,   23,
 /*   390 */    22,   18,   20,   19,   21,   28,   27,   26,   25,   24,
 /*   400 */   309,  309,  309,  309,  116,   55,   85,  309,  107,  316,
 /*   410 */   374,  309,  309,  309,  309,  309,  116,  309,   85,  309,
 /*   420 */   107,  316,  374,  309,  309,  309,   69,  370,  371,  369,
 /*   430 */    84,  372,  309,  309,  309,  309,  318,  309,   69,  370,
 /*   440 */   371,  369,   82,  372,  309,  309,  309,  309,  318,  309,
 /*   450 */   309,  309,  116,  309,   85,  309,  107,  316,  374,  309,
 /*   460 */   309,  309,  309,  309,  309,  309,  309,  309,  309,  309,
 /*   470 */   309,  309,  309,  309,   69,  370,  371,  369,   81,  372,
 /*   480 */   309,  309,  309,  309,  318,   23,   22,   18,   20,   19,
 /*   490 */    21,   28,   27,   26,   25,   24,  309,  309,  309,  309,
 /*   500 */   309,   55,  116,  309,   85,  309,  107,  316,  374,  309,
 /*   510 */   309,  309,  309,  116,  309,   85,  309,  107,  316,  374,
 /*   520 */   309,  309,  309,  309,   62,  370,  371,  369,  309,  372,
 /*   530 */   309,  309,  309,  309,  318,   65,  370,  371,  369,  309,
 /*   540 */   372,  309,  309,  309,  309,  318,  309,  309,  309,  116,
 /*   550 */   309,   85,  309,  107,  316,  374,  309,  309,  309,  309,
 /*   560 */   116,  309,   85,  309,  107,  316,  374,  309,  309,  309,
 /*   570 */   309,   66,  370,  371,  369,  309,  372,  309,  309,  309,
 /*   580 */   309,  318,   63,  370,  371,  369,  309,  372,  309,  309,
 /*   590 */   309,  309,  318,  116,  309,   85,  309,  107,  316,  374,
 /*   600 */   309,  309,  309,  309,  116,  309,   85,  309,  107,  316,
 /*   610 */   374,  309,  309,  309,  309,   67,  370,  371,  369,  309,
 /*   620 */   372,  309,  309,  309,  309,  318,   96,  370,  371,  369,
 /*   630 */   309,  372,  309,  309,  309,  116,  318,   85,  309,  107,
 /*   640 */   316,  374,  309,  309,  309,  309,  116,  309,   85,  309,
 /*   650 */   107,  316,  374,  309,  309,  309,  309,   97,  370,  371,
 /*   660 */   369,  309,  372,  309,  309,  309,  309,  318,   98,  370,
 /*   670 */   371,  369,  309,  372,  309,  309,  309,  309,  318,  116,
 /*   680 */   309,   85,  309,  107,  316,  374,  309,  309,  309,  309,
 /*   690 */   116,  309,   85,  309,  107,  316,  374,  309,  309,  309,
 /*   700 */   309,   99,  370,  371,  369,  309,  372,  309,  309,  309,
 /*   710 */   309,  318,  100,  370,  371,  369,  309,  372,  309,  309,
 /*   720 */   309,  116,  318,   85,  309,  107,  316,  374,  309,  309,
 /*   730 */   309,  309,  116,  309,   85,  309,  107,  316,  374,  309,
 /*   740 */   309,  309,  309,   68,  370,  371,  369,  309,  372,  309,
 /*   750 */   309,  309,  309,  318,   71,  370,  371,  369,  309,  372,
 /*   760 */   309,  309,  309,  309,  318,  116,  309,   85,  309,  107,
 /*   770 */   316,  374,  309,  309,  309,  309,  116,  309,   85,  309,
 /*   780 */   107,  316,  374,  309,  309,  309,  309,   72,  370,  371,
 /*   790 */   369,  309,  372,  309,  309,  309,  309,  318,   73,  370,
 /*   800 */   371,  369,  309,  372,  309,  309,  309,  116,  318,   85,
 /*   810 */   309,  107,  316,  374,  309,  309,  309,  309,  116,  309,
 /*   820 */    85,  309,  107,  316,  374,  309,  309,  309,  309,   74,
 /*   830 */   370,  371,  369,  309,  372,  309,  309,  309,  309,  318,
 /*   840 */    75,  370,  371,  369,  309,  372,  309,  309,  309,  309,
 /*   850 */   318,  116,  309,   85,  309,  107,  316,  374,  309,  309,
 /*   860 */   309,  309,  116,  309,   85,  309,  107,  316,  374,  309,
 /*   870 */   309,  309,  309,   76,  370,  371,  369,  309,  372,  309,
 /*   880 */   309,  309,  309,  318,  103,  370,  371,  369,  309,  372,
 /*   890 */   309,  309,  309,  116,  318,   85,  309,  107,  316,  374,
 /*   900 */   309,  309,  309,  309,  116,  309,   85,  309,  107,  316,
 /*   910 */   374,  309,  309,  309,  309,  104,  370,  371,  369,  309,
 /*   920 */   372,  309,  309,  309,  309,  318,  105,  370,  371,  369,
 /*   930 */   309,  372,  309,  309,  309,  309,  318,  116,  309,   85,
 /*   940 */   309,  107,  316,  374,  309,  309,  309,  309,  116,  309,
 /*   950 */    85,  309,  107,  316,  374,  309,  309,  309,  309,   78,
 /*   960 */   370,  371,  369,  309,  372,  309,  309,  309,  309,  318,
 /*   970 */    79,  370,  371,  369,  309,  372,  309,  309,  309,  116,
 /*   980 */   318,   85,  309,  107,  316,  374,  309,  309,  309,  309,
 /*   990 */   116,  309,   85,  309,  107,  316,  374,  309,  309,  309,
 /*  1000 */   309,   70,  370,  371,  369,  309,  372,  309,  309,  309,
 /*  1010 */   309,  318,   77,  370,  371,  369,  309,  372,  309,  309,
 /*  1020 */   309,  309,  318,  116,  309,   85,  309,  107,  316,  374,
 /*  1030 */   309,  309,  309,  309,  309,  309,  309,  309,  309,  309,
 /*  1040 */   309,  309,  309,  309,  309,   64,  370,  371,  369,  309,
 /*  1050 */   372,  309,  309,  309,  309,  318,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    15,   16,   16,   17,   18,   20,   21,   26,   23,   25,
 /*    10 */    24,   26,   27,   28,   29,   30,   31,   32,   33,   34,
 /*    20 */    35,   36,   37,   38,   77,   78,   42,   74,   74,   76,
 /*    30 */    76,   53,   47,   86,   87,   84,   51,   53,   54,   78,
 /*    40 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*    50 */    65,   66,   67,   68,   69,   70,   15,   16,   50,  106,
 /*    60 */   106,   20,   21,   74,   23,   76,   84,   26,   27,   28,
 /*    70 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*    80 */    23,   77,   78,   26,   27,   44,   74,  105,   76,   78,
 /*    90 */    92,   87,   51,   49,   23,  106,   52,   26,   41,   58,
 /*   100 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*   110 */    69,   70,   41,  102,  103,   15,   16,   75,  106,   77,
 /*   120 */    20,   21,   43,   23,   45,   46,   26,   27,   28,   29,
 /*   130 */    30,   31,   32,   33,   34,   35,   36,   37,   38,    8,
 /*   140 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   150 */    92,   51,   72,   73,   78,   24,   78,   23,   58,   59,
 /*   160 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*   170 */    70,    1,    2,    3,    4,   41,   48,   49,    8,    9,
 /*   180 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    2,
 /*   190 */     3,    4,   48,   49,   24,    8,    9,   10,   11,   12,
 /*   200 */    13,   14,   15,   16,   17,   18,   50,   75,   75,   77,
 /*   210 */    77,   24,   85,   78,   44,   26,   78,   28,   29,   30,
 /*   220 */    31,   32,   33,   34,   35,   36,   37,   38,   90,   91,
 /*   230 */    49,   44,   78,   52,    2,    3,    4,   75,  103,   77,
 /*   240 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   250 */    18,    2,    3,    4,   27,   79,   24,    8,    9,   10,
 /*   260 */    11,   12,   13,   14,   15,   16,   17,   18,   49,   51,
 /*   270 */    75,   52,   77,   24,   78,   74,   44,   76,   88,   78,
 /*   280 */    79,   80,    1,   82,   48,   81,   90,   83,   84,   53,
 /*   290 */    78,   78,   47,   89,   93,   94,   95,   96,   97,   98,
 /*   300 */    99,   52,  101,   49,    1,   48,   52,  106,  104,   74,
 /*   310 */    44,   76,   48,   78,   79,   80,   47,   82,   47,   50,
 /*   320 */    50,   50,   24,   51,   50,   47,   51,   51,   93,   94,
 /*   330 */    95,   96,   97,   98,   99,   50,  101,    2,    3,    4,
 /*   340 */    23,  106,   50,    8,    9,   10,   11,   12,   13,   14,
 /*   350 */    15,   16,   17,   18,   47,   44,   74,   44,   76,   24,
 /*   360 */    78,   79,   80,   40,   82,   39,    0,  107,  107,  107,
 /*   370 */   107,  107,  107,  107,  107,  107,  107,   95,   96,   97,
 /*   380 */    98,   99,  107,  101,  107,    4,  107,  107,  106,    8,
 /*   390 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   400 */   107,  107,  107,  107,   74,   24,   76,  107,   78,   79,
 /*   410 */    80,  107,  107,  107,  107,  107,   74,  107,   76,  107,
 /*   420 */    78,   79,   80,  107,  107,  107,   96,   97,   98,   99,
 /*   430 */   100,  101,  107,  107,  107,  107,  106,  107,   96,   97,
 /*   440 */    98,   99,  100,  101,  107,  107,  107,  107,  106,  107,
 /*   450 */   107,  107,   74,  107,   76,  107,   78,   79,   80,  107,
 /*   460 */   107,  107,  107,  107,  107,  107,  107,  107,  107,  107,
 /*   470 */   107,  107,  107,  107,   96,   97,   98,   99,  100,  101,
 /*   480 */   107,  107,  107,  107,  106,    8,    9,   10,   11,   12,
 /*   490 */    13,   14,   15,   16,   17,   18,  107,  107,  107,  107,
 /*   500 */   107,   24,   74,  107,   76,  107,   78,   79,   80,  107,
 /*   510 */   107,  107,  107,   74,  107,   76,  107,   78,   79,   80,
 /*   520 */   107,  107,  107,  107,   96,   97,   98,   99,  107,  101,
 /*   530 */   107,  107,  107,  107,  106,   96,   97,   98,   99,  107,
 /*   540 */   101,  107,  107,  107,  107,  106,  107,  107,  107,   74,
 /*   550 */   107,   76,  107,   78,   79,   80,  107,  107,  107,  107,
 /*   560 */    74,  107,   76,  107,   78,   79,   80,  107,  107,  107,
 /*   570 */   107,   96,   97,   98,   99,  107,  101,  107,  107,  107,
 /*   580 */   107,  106,   96,   97,   98,   99,  107,  101,  107,  107,
 /*   590 */   107,  107,  106,   74,  107,   76,  107,   78,   79,   80,
 /*   600 */   107,  107,  107,  107,   74,  107,   76,  107,   78,   79,
 /*   610 */    80,  107,  107,  107,  107,   96,   97,   98,   99,  107,
 /*   620 */   101,  107,  107,  107,  107,  106,   96,   97,   98,   99,
 /*   630 */   107,  101,  107,  107,  107,   74,  106,   76,  107,   78,
 /*   640 */    79,   80,  107,  107,  107,  107,   74,  107,   76,  107,
 /*   650 */    78,   79,   80,  107,  107,  107,  107,   96,   97,   98,
 /*   660 */    99,  107,  101,  107,  107,  107,  107,  106,   96,   97,
 /*   670 */    98,   99,  107,  101,  107,  107,  107,  107,  106,   74,
 /*   680 */   107,   76,  107,   78,   79,   80,  107,  107,  107,  107,
 /*   690 */    74,  107,   76,  107,   78,   79,   80,  107,  107,  107,
 /*   700 */   107,   96,   97,   98,   99,  107,  101,  107,  107,  107,
 /*   710 */   107,  106,   96,   97,   98,   99,  107,  101,  107,  107,
 /*   720 */   107,   74,  106,   76,  107,   78,   79,   80,  107,  107,
 /*   730 */   107,  107,   74,  107,   76,  107,   78,   79,   80,  107,
 /*   740 */   107,  107,  107,   96,   97,   98,   99,  107,  101,  107,
 /*   750 */   107,  107,  107,  106,   96,   97,   98,   99,  107,  101,
 /*   760 */   107,  107,  107,  107,  106,   74,  107,   76,  107,   78,
 /*   770 */    79,   80,  107,  107,  107,  107,   74,  107,   76,  107,
 /*   780 */    78,   79,   80,  107,  107,  107,  107,   96,   97,   98,
 /*   790 */    99,  107,  101,  107,  107,  107,  107,  106,   96,   97,
 /*   800 */    98,   99,  107,  101,  107,  107,  107,   74,  106,   76,
 /*   810 */   107,   78,   79,   80,  107,  107,  107,  107,   74,  107,
 /*   820 */    76,  107,   78,   79,   80,  107,  107,  107,  107,   96,
 /*   830 */    97,   98,   99,  107,  101,  107,  107,  107,  107,  106,
 /*   840 */    96,   97,   98,   99,  107,  101,  107,  107,  107,  107,
 /*   850 */   106,   74,  107,   76,  107,   78,   79,   80,  107,  107,
 /*   860 */   107,  107,   74,  107,   76,  107,   78,   79,   80,  107,
 /*   870 */   107,  107,  107,   96,   97,   98,   99,  107,  101,  107,
 /*   880 */   107,  107,  107,  106,   96,   97,   98,   99,  107,  101,
 /*   890 */   107,  107,  107,   74,  106,   76,  107,   78,   79,   80,
 /*   900 */   107,  107,  107,  107,   74,  107,   76,  107,   78,   79,
 /*   910 */    80,  107,  107,  107,  107,   96,   97,   98,   99,  107,
 /*   920 */   101,  107,  107,  107,  107,  106,   96,   97,   98,   99,
 /*   930 */   107,  101,  107,  107,  107,  107,  106,   74,  107,   76,
 /*   940 */   107,   78,   79,   80,  107,  107,  107,  107,   74,  107,
 /*   950 */    76,  107,   78,   79,   80,  107,  107,  107,  107,   96,
 /*   960 */    97,   98,   99,  107,  101,  107,  107,  107,  107,  106,
 /*   970 */    96,   97,   98,   99,  107,  101,  107,  107,  107,   74,
 /*   980 */   106,   76,  107,   78,   79,   80,  107,  107,  107,  107,
 /*   990 */    74,  107,   76,  107,   78,   79,   80,  107,  107,  107,
 /*  1000 */   107,   96,   97,   98,   99,  107,  101,  107,  107,  107,
 /*  1010 */   107,  106,   96,   97,   98,   99,  107,  101,  107,  107,
 /*  1020 */   107,  107,  106,   74,  107,   76,  107,   78,   79,   80,
 /*  1030 */   107,  107,  107,  107,  107,  107,  107,  107,  107,  107,
 /*  1040 */   107,  107,  107,  107,  107,   96,   97,   98,   99,  107,
 /*  1050 */   101,  107,  107,  107,  107,  106,
};
#define YY_SHIFT_USE_DFLT (1056)
#define YY_SHIFT_COUNT    (117)
#define YY_SHIFT_MIN      (-22)
#define YY_SHIFT_MAX      (477)
static const short yy_shift_ofst[] = {
 /*     0 */  1056,  -15,  -15,  -15,  100,  100,  100,   41,  100,  100,
 /*    10 */   100,  100,  100,  100,  100,  100,  100,  100,  100,  100,
 /*    20 */   100,  100,  100,  100,  100,  100,  100,  100,  100,  100,
 /*    30 */   100,  100,  -16,   57,  189,  189,  189,  189,   71,  -19,
 /*    40 */   -19,  134,  134,  134,  134,  134,  -22,  -19,  -19,  236,
 /*    50 */     8,  -19,    8,  -19,  -19,  -19,  156,  227,  218,  -19,
 /*    60 */   -19,  170,  187,  232,  249,  335,  335,  335,  335,  335,
 /*    70 */   381,  131,  131,  131,  131,  131,  131,  477,  -14,  -14,
 /*    80 */    79,   44,  181,  128,  219,  269,  271,  254,  144,  245,
 /*    90 */   281,  303,  257,  266,  264,  270,  298,  298,  298,  298,
 /*   100 */   298,  272,  274,  298,  298,  298,  275,  276,  278,  285,
 /*   110 */   317,  292,  307,  311,  313,  323,  326,  366,
};
#define YY_REDUCE_USE_DFLT (-54)
#define YY_REDUCE_COUNT (60)
#define YY_REDUCE_MIN   (-53)
#define YY_REDUCE_MAX   (949)
static const short yy_reduce_ofst[] = {
 /*     0 */    80,  201,  235,  282,  330,  342,  378,  428,  439,  475,
 /*    10 */   486,  519,  530,  561,  572,  605,  616,  647,  658,  691,
 /*    20 */   702,  733,  744,  777,  788,  819,  830,  863,  874,  905,
 /*    30 */   916,  949,  204,  -53,  -47,  -46,  -11,   12,    4,   11,
 /*    40 */   138,   42,  132,  133,  162,  195,  -18,  135,  196,  -49,
 /*    50 */    -2,  -39,   58,   76,   78,  154,  127,  176,  190,  212,
 /*    60 */   213,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   308,  354,  354,  355,  399,  399,  399,  420,  420,  420,
 /*    10 */   420,  420,  420,  420,  420,  420,  420,  420,  420,  420,
 /*    20 */   420,  420,  420,  420,  420,  420,  420,  420,  420,  420,
 /*    30 */   420,  420,  420,  349,  420,  420,  420,  420,  331,  401,
 /*    40 */   420,  331,  331,  331,  331,  331,  420,  420,  420,  420,
 /*    50 */   367,  420,  367,  420,  420,  420,  353,  420,  420,  420,
 /*    60 */   420,  420,  420,  420,  420,  365,  364,  404,  397,  398,
 /*    70 */   380,  391,  390,  389,  388,  387,  386,  379,  382,  381,
 /*    80 */   420,  420,  420,  420,  420,  420,  313,  420,  420,  420,
 /*    90 */   420,  420,  420,  420,  420,  420,  396,  395,  394,  393,
 /*   100 */   392,  420,  420,  385,  384,  383,  375,  315,  420,  420,
 /*   110 */   332,  420,  420,  420,  420,  420,  317,  420,
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
  "ARRAY_END",     "MUT",           "TYPE",          "ALIAS",       
  "SEMI_COLON",    "FROM",          "STRUCT",        "SCOPE_BEGIN", 
  "SCOPE_END",     "COMMA",         "COLON",         "PAR_BEGIN",   
  "PAR_END",       "FUNCTION",      "IMPL",          "RETURN",      
  "VAR",           "LET",           "INTEGER_LITERAL",  "INTEGER_LITERAL_I8",
  "INTEGER_LITERAL_I16",  "INTEGER_LITERAL_I32",  "INTEGER_LITERAL_I64",  "INTEGER_LITERAL_U8",
  "INTEGER_LITERAL_U16",  "INTEGER_LITERAL_U32",  "INTEGER_LITERAL_U64",  "FLOAT_LITERAL",
  "FLOAT_LITERAL_32",  "FLOAT_LITERAL_64",  "BOOL_LITERAL",  "error",       
  "decls",         "module",        "type_builtin",  "type_qualified",
  "type_specifier",  "qualifier",     "identifier",    "self",        
  "identifier_or_self",  "decl",          "decl_var",      "decl_alias",  
  "decl_function",  "function_return_decl",  "function_args_decl",  "function_arg_decl",
  "function_param_list",  "decl_struct",   "struct_decl_var",  "struct_decl_vars",
  "var_type_spec",  "statement_list",  "statement_list_or_empty",  "statement",   
  "expression",    "unaryexp",      "binaryexp",     "literal",     
  "function_call_args",  "struct_init",   "struct_member_init_list",  "struct_member_init",
  "decl_type_functions_scope",  "decl_type_function_list",  "type_array",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "decls ::= decls decl",
 /*   1 */ "decls ::=",
 /*   2 */ "decl ::= decl_alias",
 /*   3 */ "decl ::= decl_struct",
 /*   4 */ "decl ::= decl_function",
 /*   5 */ "decl ::= decl_type_functions_scope",
 /*   6 */ "identifier ::= IDENTIFIER",
 /*   7 */ "self ::= SELF",
 /*   8 */ "identifier_or_self ::= identifier",
 /*   9 */ "identifier_or_self ::= self",
 /*  10 */ "type_specifier ::= type_builtin",
 /*  11 */ "type_specifier ::= type_array",
 /*  12 */ "type_specifier ::= IDENTIFIER",
 /*  13 */ "type_builtin ::= TYPE_BOOL",
 /*  14 */ "type_builtin ::= TYPE_INT8",
 /*  15 */ "type_builtin ::= TYPE_UINT8",
 /*  16 */ "type_builtin ::= TYPE_INT16",
 /*  17 */ "type_builtin ::= TYPE_UINT16",
 /*  18 */ "type_builtin ::= TYPE_INT32",
 /*  19 */ "type_builtin ::= TYPE_UINT32",
 /*  20 */ "type_builtin ::= TYPE_INT64",
 /*  21 */ "type_builtin ::= TYPE_UINT64",
 /*  22 */ "type_builtin ::= TYPE_FLOAT",
 /*  23 */ "type_builtin ::= TYPE_DOUBLE",
 /*  24 */ "qualifier ::=",
 /*  25 */ "qualifier ::= MUT",
 /*  26 */ "qualifier ::= REFERENCE",
 /*  27 */ "qualifier ::= MUT REFERENCE",
 /*  28 */ "type_qualified ::= qualifier type_specifier",
 /*  29 */ "decl_alias ::= TYPE identifier ALIAS type_specifier SEMI_COLON",
 /*  30 */ "decl_alias ::= TYPE identifier FROM type_specifier SEMI_COLON",
 /*  31 */ "decl_struct ::= TYPE identifier STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END",
 /*  32 */ "struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var",
 /*  33 */ "struct_decl_vars ::= struct_decl_var",
 /*  34 */ "struct_decl_var ::= identifier COLON type_qualified",
 /*  35 */ "function_param_list ::= PAR_BEGIN function_args_decl PAR_END",
 /*  36 */ "decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  37 */ "decl_type_functions_scope ::= IMPL type_specifier SCOPE_BEGIN decl_type_function_list SCOPE_END",
 /*  38 */ "decl_type_function_list ::= decl_type_function_list decl_function",
 /*  39 */ "decl_type_function_list ::= decl_function",
 /*  40 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  41 */ "function_args_decl ::= function_arg_decl",
 /*  42 */ "function_args_decl ::=",
 /*  43 */ "function_arg_decl ::= identifier COLON type_qualified",
 /*  44 */ "function_arg_decl ::= qualifier self",
 /*  45 */ "function_return_decl ::= COLON type_qualified",
 /*  46 */ "function_return_decl ::=",
 /*  47 */ "statement_list_or_empty ::=",
 /*  48 */ "statement_list_or_empty ::= statement_list",
 /*  49 */ "statement_list ::= statement_list statement",
 /*  50 */ "statement_list ::= statement",
 /*  51 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  52 */ "statement ::= decl_var SEMI_COLON",
 /*  53 */ "statement ::= expression SEMI_COLON",
 /*  54 */ "statement ::= RETURN expression SEMI_COLON",
 /*  55 */ "statement ::= RETURN SEMI_COLON",
 /*  56 */ "statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  57 */ "decl_var ::= VAR identifier var_type_spec ASSIGN expression",
 /*  58 */ "decl_var ::= LET identifier var_type_spec ASSIGN expression",
 /*  59 */ "var_type_spec ::= COLON type_qualified",
 /*  60 */ "var_type_spec ::=",
 /*  61 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  62 */ "expression ::= literal",
 /*  63 */ "expression ::= unaryexp",
 /*  64 */ "expression ::= binaryexp",
 /*  65 */ "expression ::= struct_init",
 /*  66 */ "expression ::= expression AS type_qualified",
 /*  67 */ "expression ::= identifier_or_self",
 /*  68 */ "expression ::= expression DOT identifier",
 /*  69 */ "expression ::= identifier PAR_BEGIN function_call_args PAR_END",
 /*  70 */ "expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END",
 /*  71 */ "expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END",
 /*  72 */ "binaryexp ::= expression AND expression",
 /*  73 */ "binaryexp ::= expression OR expression",
 /*  74 */ "binaryexp ::= expression PLUS expression",
 /*  75 */ "binaryexp ::= expression MINUS expression",
 /*  76 */ "binaryexp ::= expression MULT expression",
 /*  77 */ "binaryexp ::= expression DIV expression",
 /*  78 */ "binaryexp ::= expression MOD expression",
 /*  79 */ "binaryexp ::= expression EQ expression",
 /*  80 */ "binaryexp ::= expression NE expression",
 /*  81 */ "binaryexp ::= expression LE expression",
 /*  82 */ "binaryexp ::= expression LT expression",
 /*  83 */ "binaryexp ::= expression GE expression",
 /*  84 */ "binaryexp ::= expression GT expression",
 /*  85 */ "unaryexp ::= NOT expression",
 /*  86 */ "unaryexp ::= BIT_NOT expression",
 /*  87 */ "unaryexp ::= MINUS expression",
 /*  88 */ "unaryexp ::= REFERENCE expression",
 /*  89 */ "unaryexp ::= MULT expression",
 /*  90 */ "function_call_args ::= function_call_args COMMA expression",
 /*  91 */ "function_call_args ::= expression",
 /*  92 */ "function_call_args ::=",
 /*  93 */ "struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END",
 /*  94 */ "struct_member_init_list ::=",
 /*  95 */ "struct_member_init_list ::= struct_member_init_list COMMA struct_member_init",
 /*  96 */ "struct_member_init_list ::= struct_member_init",
 /*  97 */ "struct_member_init ::= identifier COLON expression",
 /*  98 */ "literal ::= INTEGER_LITERAL",
 /*  99 */ "literal ::= INTEGER_LITERAL_I8",
 /* 100 */ "literal ::= INTEGER_LITERAL_I16",
 /* 101 */ "literal ::= INTEGER_LITERAL_I32",
 /* 102 */ "literal ::= INTEGER_LITERAL_I64",
 /* 103 */ "literal ::= INTEGER_LITERAL_U8",
 /* 104 */ "literal ::= INTEGER_LITERAL_U16",
 /* 105 */ "literal ::= INTEGER_LITERAL_U32",
 /* 106 */ "literal ::= INTEGER_LITERAL_U64",
 /* 107 */ "literal ::= FLOAT_LITERAL",
 /* 108 */ "literal ::= FLOAT_LITERAL_32",
 /* 109 */ "literal ::= FLOAT_LITERAL_64",
 /* 110 */ "literal ::= BOOL_LITERAL",
 /* 111 */ "module ::= decls END",
 /* 112 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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
  { 72, -2 },
  { 72, 0 },
  { 81, -1 },
  { 81, -1 },
  { 81, -1 },
  { 81, -1 },
  { 78, -1 },
  { 79, -1 },
  { 80, -1 },
  { 80, -1 },
  { 76, -1 },
  { 76, -1 },
  { 76, -1 },
  { 74, -1 },
  { 74, -1 },
  { 74, -1 },
  { 74, -1 },
  { 74, -1 },
  { 74, -1 },
  { 74, -1 },
  { 74, -1 },
  { 74, -1 },
  { 74, -1 },
  { 74, -1 },
  { 77, 0 },
  { 77, -1 },
  { 77, -1 },
  { 77, -2 },
  { 75, -2 },
  { 83, -5 },
  { 83, -5 },
  { 89, -6 },
  { 91, -3 },
  { 91, -1 },
  { 90, -3 },
  { 88, -3 },
  { 84, -7 },
  { 104, -5 },
  { 105, -2 },
  { 105, -1 },
  { 86, -3 },
  { 86, -1 },
  { 86, 0 },
  { 87, -3 },
  { 87, -2 },
  { 85, -2 },
  { 85, 0 },
  { 94, 0 },
  { 94, -1 },
  { 93, -2 },
  { 93, -1 },
  { 95, -4 },
  { 95, -2 },
  { 95, -2 },
  { 95, -3 },
  { 95, -2 },
  { 95, -3 },
  { 82, -5 },
  { 82, -5 },
  { 92, -2 },
  { 92, 0 },
  { 96, -3 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -3 },
  { 96, -1 },
  { 96, -3 },
  { 96, -4 },
  { 96, -6 },
  { 96, -7 },
  { 98, -3 },
  { 98, -3 },
  { 98, -3 },
  { 98, -3 },
  { 98, -3 },
  { 98, -3 },
  { 98, -3 },
  { 98, -3 },
  { 98, -3 },
  { 98, -3 },
  { 98, -3 },
  { 98, -3 },
  { 98, -3 },
  { 97, -2 },
  { 97, -2 },
  { 97, -2 },
  { 97, -2 },
  { 97, -2 },
  { 100, -3 },
  { 100, -1 },
  { 100, 0 },
  { 101, -4 },
  { 102, 0 },
  { 102, -3 },
  { 102, -1 },
  { 103, -3 },
  { 99, -1 },
  { 99, -1 },
  { 99, -1 },
  { 99, -1 },
  { 99, -1 },
  { 99, -1 },
  { 99, -1 },
  { 99, -1 },
  { 99, -1 },
  { 99, -1 },
  { 99, -1 },
  { 99, -1 },
  { 99, -1 },
  { 73, -2 },
  { 106, -3 },
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
      case 0: /* decls ::= decls decl */
#line 124 "stparserimpl.lemon"
{
    yylhsminor.yy186 =yymsp[-1].minor.yy186; yymsp[-1].minor.yy186->addDecl(yymsp[0].minor.yy36);
}
#line 1361 "stparserimpl.c"
  yymsp[-1].minor.yy186 = yylhsminor.yy186;
        break;
      case 1: /* decls ::= */
#line 127 "stparserimpl.lemon"
{
        yymsp[1].minor.yy186 =  pParser->getDeclModule();
}
#line 1369 "stparserimpl.c"
        break;
      case 2: /* decl ::= decl_alias */
#line 131 "stparserimpl.lemon"
{
    yylhsminor.yy36 = yymsp[0].minor.yy56;
}
#line 1376 "stparserimpl.c"
  yymsp[0].minor.yy36 = yylhsminor.yy36;
        break;
      case 3: /* decl ::= decl_struct */
#line 134 "stparserimpl.lemon"
{
    yylhsminor.yy36 = yymsp[0].minor.yy153;
}
#line 1384 "stparserimpl.c"
  yymsp[0].minor.yy36 = yylhsminor.yy36;
        break;
      case 4: /* decl ::= decl_function */
#line 137 "stparserimpl.lemon"
{
    yylhsminor.yy36 = yymsp[0].minor.yy57;
}
#line 1392 "stparserimpl.c"
  yymsp[0].minor.yy36 = yylhsminor.yy36;
        break;
      case 5: /* decl ::= decl_type_functions_scope */
#line 140 "stparserimpl.lemon"
{
    yylhsminor.yy36 = yymsp[0].minor.yy159;
}
#line 1400 "stparserimpl.c"
  yymsp[0].minor.yy36 = yylhsminor.yy36;
        break;
      case 6: /* identifier ::= IDENTIFIER */
      case 7: /* self ::= SELF */ yytestcase(yyruleno==7);
#line 146 "stparserimpl.lemon"
{
        yylhsminor.yy89 = pParser->createNode<yal::frontend::STIdentifier>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy89->setSourceInfo(srcInfo);
}
#line 1411 "stparserimpl.c"
  yymsp[0].minor.yy89 = yylhsminor.yy89;
        break;
      case 8: /* identifier_or_self ::= identifier */
      case 9: /* identifier_or_self ::= self */ yytestcase(yyruleno==9);
#line 158 "stparserimpl.lemon"
{yylhsminor.yy89=yymsp[0].minor.yy89;}
#line 1418 "stparserimpl.c"
  yymsp[0].minor.yy89 = yylhsminor.yy89;
        break;
      case 10: /* type_specifier ::= type_builtin */
#line 161 "stparserimpl.lemon"
{
        yylhsminor.yy68=yymsp[0].minor.yy68;
}
#line 1426 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 11: /* type_specifier ::= type_array */
#line 165 "stparserimpl.lemon"
{yymsp[0].minor.yy68 = nullptr;}
#line 1432 "stparserimpl.c"
        break;
      case 12: /* type_specifier ::= IDENTIFIER */
#line 166 "stparserimpl.lemon"
{
    yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yymsp[0].minor.yy0.tokenStr);
    yylhsminor.yy68->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0,yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1440 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 13: /* type_builtin ::= TYPE_BOOL */
#line 171 "stparserimpl.lemon"
{
    yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Bool);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy68->setSourceInfo(srcInfo);
    }
#line 1450 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 14: /* type_builtin ::= TYPE_INT8 */
#line 176 "stparserimpl.lemon"
{
        yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy68->setSourceInfo(srcInfo);
    }
#line 1460 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 15: /* type_builtin ::= TYPE_UINT8 */
#line 181 "stparserimpl.lemon"
{
        yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy68->setSourceInfo(srcInfo);
    }
#line 1470 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 16: /* type_builtin ::= TYPE_INT16 */
#line 186 "stparserimpl.lemon"
{
        yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy68->setSourceInfo(srcInfo);
    }
#line 1480 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 17: /* type_builtin ::= TYPE_UINT16 */
#line 191 "stparserimpl.lemon"
{
        yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy68->setSourceInfo(srcInfo);
    }
#line 1490 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 18: /* type_builtin ::= TYPE_INT32 */
#line 196 "stparserimpl.lemon"
{
        yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy68->setSourceInfo(srcInfo);
    }
#line 1500 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 19: /* type_builtin ::= TYPE_UINT32 */
#line 201 "stparserimpl.lemon"
{
        yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy68->setSourceInfo(srcInfo);
    }
#line 1510 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 20: /* type_builtin ::= TYPE_INT64 */
#line 206 "stparserimpl.lemon"
{
        yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy68->setSourceInfo(srcInfo);
    }
#line 1520 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 21: /* type_builtin ::= TYPE_UINT64 */
#line 211 "stparserimpl.lemon"
{
        yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy68->setSourceInfo(srcInfo);
    }
#line 1530 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 22: /* type_builtin ::= TYPE_FLOAT */
#line 216 "stparserimpl.lemon"
{
        yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy68->setSourceInfo(srcInfo);
    }
#line 1540 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 23: /* type_builtin ::= TYPE_DOUBLE */
#line 221 "stparserimpl.lemon"
{
        yylhsminor.yy68 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy68->setSourceInfo(srcInfo);
    }
#line 1550 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 24: /* qualifier ::= */
#line 229 "stparserimpl.lemon"
{yymsp[1].minor.yy105 = 0;}
#line 1556 "stparserimpl.c"
        break;
      case 25: /* qualifier ::= MUT */
#line 230 "stparserimpl.lemon"
{yymsp[0].minor.yy105 = yal::frontend::STQualType::Qualifiers::kQualMutable;}
#line 1561 "stparserimpl.c"
        break;
      case 26: /* qualifier ::= REFERENCE */
#line 231 "stparserimpl.lemon"
{yymsp[0].minor.yy105 = yal::frontend::STQualType::Qualifiers::kQualReference;}
#line 1566 "stparserimpl.c"
        break;
      case 27: /* qualifier ::= MUT REFERENCE */
#line 232 "stparserimpl.lemon"
{
        yymsp[-1].minor.yy105 = yal::frontend::STQualType::Qualifiers::kQualReference | yal::frontend::STQualType::Qualifiers::kQualMutable;
 }
#line 1573 "stparserimpl.c"
        break;
      case 28: /* type_qualified ::= qualifier type_specifier */
#line 235 "stparserimpl.lemon"
{
    yylhsminor.yy169 = pParser->createNode<yal::frontend::STQualType>(yymsp[0].minor.yy68, yymsp[-1].minor.yy105);
}
#line 1580 "stparserimpl.c"
  yymsp[-1].minor.yy169 = yylhsminor.yy169;
        break;
      case 29: /* decl_alias ::= TYPE identifier ALIAS type_specifier SEMI_COLON */
#line 240 "stparserimpl.lemon"
{
    yylhsminor.yy56 = pParser->createNode<yal::frontend::STDeclAlias>(yymsp[-3].minor.yy89, yymsp[-1].minor.yy68, false);
    auto srcInfo = CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy56->setSourceInfo(srcInfo);
}
#line 1590 "stparserimpl.c"
  yymsp[-4].minor.yy56 = yylhsminor.yy56;
        break;
      case 30: /* decl_alias ::= TYPE identifier FROM type_specifier SEMI_COLON */
#line 246 "stparserimpl.lemon"
{
    yylhsminor.yy56 = pParser->createNode<yal::frontend::STDeclAlias>(yymsp[-3].minor.yy89, yymsp[-1].minor.yy68, true);
    auto srcInfo = CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy56->setSourceInfo(srcInfo);
}
#line 1600 "stparserimpl.c"
  yymsp[-4].minor.yy56 = yylhsminor.yy56;
        break;
      case 31: /* decl_struct ::= TYPE identifier STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END */
#line 253 "stparserimpl.lemon"
{
    yylhsminor.yy153 = pParser->createNode<yal::frontend::STDeclStruct>(yymsp[-4].minor.yy89, yymsp[-1].minor.yy44, *pParser);
    auto srcInfo = CreateSourceInfo(yymsp[-5].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy153->setSourceInfo(srcInfo);
}
#line 1610 "stparserimpl.c"
  yymsp[-5].minor.yy153 = yylhsminor.yy153;
        break;
      case 32: /* struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
#line 259 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy44 = list.add(yymsp[-2].minor.yy44, yymsp[0].minor.yy133);
}
#line 1619 "stparserimpl.c"
  yymsp[-2].minor.yy44 = yylhsminor.yy44;
        break;
      case 33: /* struct_decl_vars ::= struct_decl_var */
#line 263 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy44 = list.add(yymsp[0].minor.yy133);
}
#line 1628 "stparserimpl.c"
  yymsp[0].minor.yy44 = yylhsminor.yy44;
        break;
      case 34: /* struct_decl_var ::= identifier COLON type_qualified */
#line 268 "stparserimpl.lemon"
{
    yylhsminor.yy133 = pParser->createNode<yal::frontend::STStructMember>(yymsp[-2].minor.yy89, yymsp[0].minor.yy169);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy89->getSourceInfo(), yymsp[0].minor.yy169->getSourceInfo(),pParser->getSourceHandle());
    yylhsminor.yy133->setSourceInfo(srcInfo);
}
#line 1638 "stparserimpl.c"
  yymsp[-2].minor.yy133 = yylhsminor.yy133;
        break;
      case 35: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
#line 277 "stparserimpl.lemon"
{
    yymsp[-2].minor.yy102 = yymsp[-1].minor.yy102;
}
#line 1646 "stparserimpl.c"
        break;
      case 36: /* decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 282 "stparserimpl.lemon"
{
    yylhsminor.yy57 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy89, yymsp[-3].minor.yy169, yymsp[-4].minor.yy102, yymsp[-1].minor.yy137, *pParser);
    yylhsminor.yy57->setSourceInfo(CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1654 "stparserimpl.c"
  yymsp[-6].minor.yy57 = yylhsminor.yy57;
        break;
      case 37: /* decl_type_functions_scope ::= IMPL type_specifier SCOPE_BEGIN decl_type_function_list SCOPE_END */
#line 289 "stparserimpl.lemon"
{
        yylhsminor.yy159 = pParser->createNode<yal::frontend::STDeclTypeFunctions>(yymsp[-3].minor.yy68,yymsp[-1].minor.yy7, *pParser);
        yylhsminor.yy159->setSourceInfo(CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1663 "stparserimpl.c"
  yymsp[-4].minor.yy159 = yylhsminor.yy159;
        break;
      case 38: /* decl_type_function_list ::= decl_type_function_list decl_function */
#line 294 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclList();
    yylhsminor.yy7 = list.add(yymsp[-1].minor.yy7, yymsp[0].minor.yy57);
}
#line 1672 "stparserimpl.c"
  yymsp[-1].minor.yy7 = yylhsminor.yy7;
        break;
      case 39: /* decl_type_function_list ::= decl_function */
#line 299 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclList();
    yylhsminor.yy7 = list.add(yymsp[0].minor.yy57);
}
#line 1681 "stparserimpl.c"
  yymsp[0].minor.yy7 = yylhsminor.yy7;
        break;
      case 40: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
#line 307 "stparserimpl.lemon"
{
        auto& list = pParser->getDeclParamList();
        yylhsminor.yy102 = list.add(yymsp[-2].minor.yy102, yymsp[0].minor.yy157);
}
#line 1690 "stparserimpl.c"
  yymsp[-2].minor.yy102 = yylhsminor.yy102;
        break;
      case 41: /* function_args_decl ::= function_arg_decl */
#line 312 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclParamList();
    yylhsminor.yy102 = list.add(yymsp[0].minor.yy157);
}
#line 1699 "stparserimpl.c"
  yymsp[0].minor.yy102 = yylhsminor.yy102;
        break;
      case 42: /* function_args_decl ::= */
#line 317 "stparserimpl.lemon"
{
   yymsp[1].minor.yy102 = pParser->getDeclParamList().getRangeEmpty();
}
#line 1707 "stparserimpl.c"
        break;
      case 43: /* function_arg_decl ::= identifier COLON type_qualified */
#line 321 "stparserimpl.lemon"
{
        yylhsminor.yy157 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[-2].minor.yy89, yymsp[0].minor.yy169);
        yylhsminor.yy157->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy89->getSourceInfo(),yymsp[0].minor.yy169->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1715 "stparserimpl.c"
  yymsp[-2].minor.yy157 = yylhsminor.yy157;
        break;
      case 44: /* function_arg_decl ::= qualifier self */
#line 326 "stparserimpl.lemon"
{
        auto qualType = pParser->createNode<yal::frontend::STQualType>(nullptr, yymsp[-1].minor.yy105);
        yylhsminor.yy157 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[0].minor.yy89, qualType);
        yylhsminor.yy157->setSourceInfo(yymsp[0].minor.yy89->getSourceInfo());
}
#line 1725 "stparserimpl.c"
  yymsp[-1].minor.yy157 = yylhsminor.yy157;
        break;
      case 45: /* function_return_decl ::= COLON type_qualified */
#line 332 "stparserimpl.lemon"
{ yymsp[-1].minor.yy169 = yymsp[0].minor.yy169;}
#line 1731 "stparserimpl.c"
        break;
      case 46: /* function_return_decl ::= */
      case 60: /* var_type_spec ::= */ yytestcase(yyruleno==60);
#line 333 "stparserimpl.lemon"
{yymsp[1].minor.yy169 = nullptr;}
#line 1737 "stparserimpl.c"
        break;
      case 47: /* statement_list_or_empty ::= */
#line 337 "stparserimpl.lemon"
{yymsp[1].minor.yy137 = pParser->getStmtList().getRangeEmpty();}
#line 1742 "stparserimpl.c"
        break;
      case 48: /* statement_list_or_empty ::= statement_list */
#line 338 "stparserimpl.lemon"
{yylhsminor.yy137 = yymsp[0].minor.yy137;}
#line 1747 "stparserimpl.c"
  yymsp[0].minor.yy137 = yylhsminor.yy137;
        break;
      case 49: /* statement_list ::= statement_list statement */
#line 341 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy137 = list.add(yymsp[-1].minor.yy137,yymsp[0].minor.yy106);
}
#line 1756 "stparserimpl.c"
  yymsp[-1].minor.yy137 = yylhsminor.yy137;
        break;
      case 50: /* statement_list ::= statement */
#line 345 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy137 = list.add(yymsp[0].minor.yy106);
}
#line 1765 "stparserimpl.c"
  yymsp[0].minor.yy137 = yylhsminor.yy137;
        break;
      case 51: /* statement ::= expression ASSIGN expression SEMI_COLON */
#line 351 "stparserimpl.lemon"
{
   yylhsminor.yy106 = pParser->createNode<yal::frontend::STStmtAssign>(yymsp[-3].minor.yy178,yymsp[-1].minor.yy178);
   yylhsminor.yy106->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy178->getSourceInfo(), yymsp[-1].minor.yy178->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1774 "stparserimpl.c"
  yymsp[-3].minor.yy106 = yylhsminor.yy106;
        break;
      case 52: /* statement ::= decl_var SEMI_COLON */
#line 355 "stparserimpl.lemon"
{
    yylhsminor.yy106 = pParser->createNode<yal::frontend::STStmtDecl>(yymsp[-1].minor.yy73);
    yylhsminor.yy106->setSourceInfo(yymsp[-1].minor.yy73->getSourceInfo());
}
#line 1783 "stparserimpl.c"
  yymsp[-1].minor.yy106 = yylhsminor.yy106;
        break;
      case 53: /* statement ::= expression SEMI_COLON */
#line 360 "stparserimpl.lemon"
{
    yylhsminor.yy106 = pParser->createNode<yal::frontend::STStmtExpression>(yymsp[-1].minor.yy178);
    yylhsminor.yy106->setSourceInfo(yymsp[-1].minor.yy178->getSourceInfo());
}
#line 1792 "stparserimpl.c"
  yymsp[-1].minor.yy106 = yylhsminor.yy106;
        break;
      case 54: /* statement ::= RETURN expression SEMI_COLON */
#line 365 "stparserimpl.lemon"
{
    yylhsminor.yy106 = pParser->createNode<yal::frontend::STStmtReturn>(yymsp[-1].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy106->setSourceInfo(srcInfo);
}
#line 1802 "stparserimpl.c"
  yymsp[-2].minor.yy106 = yylhsminor.yy106;
        break;
      case 55: /* statement ::= RETURN SEMI_COLON */
#line 371 "stparserimpl.lemon"
{
    yylhsminor.yy106 = pParser->createNode<yal::frontend::STStmtReturn>(nullptr);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[-1].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy106->setSourceInfo(srcInfo);
}
#line 1812 "stparserimpl.c"
  yymsp[-1].minor.yy106 = yylhsminor.yy106;
        break;
      case 56: /* statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 377 "stparserimpl.lemon"
{
    yylhsminor.yy106 = pParser->createNode<yal::frontend::STStmtListScoped>(yymsp[-1].minor.yy137, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy106->setSourceInfo(srcInfo);
}
#line 1822 "stparserimpl.c"
  yymsp[-2].minor.yy106 = yylhsminor.yy106;
        break;
      case 57: /* decl_var ::= VAR identifier var_type_spec ASSIGN expression */
#line 385 "stparserimpl.lemon"
{
    yylhsminor.yy73 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy89, yymsp[-2].minor.yy169, yymsp[0].minor.yy178, false);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy73->setSourceInfo(srcInfo);
}
#line 1832 "stparserimpl.c"
  yymsp[-4].minor.yy73 = yylhsminor.yy73;
        break;
      case 58: /* decl_var ::= LET identifier var_type_spec ASSIGN expression */
#line 391 "stparserimpl.lemon"
{
    yylhsminor.yy73 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy89, yymsp[-2].minor.yy169, yymsp[0].minor.yy178, true);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy73->setSourceInfo(srcInfo);
}
#line 1842 "stparserimpl.c"
  yymsp[-4].minor.yy73 = yylhsminor.yy73;
        break;
      case 59: /* var_type_spec ::= COLON type_qualified */
#line 397 "stparserimpl.lemon"
{yymsp[-1].minor.yy169 = yymsp[0].minor.yy169;}
#line 1848 "stparserimpl.c"
        break;
      case 61: /* expression ::= PAR_BEGIN expression PAR_END */
#line 403 "stparserimpl.lemon"
{yymsp[-2].minor.yy178 = yymsp[-1].minor.yy178;}
#line 1853 "stparserimpl.c"
        break;
      case 62: /* expression ::= literal */
#line 404 "stparserimpl.lemon"
{yylhsminor.yy178 = yymsp[0].minor.yy178;}
#line 1858 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 63: /* expression ::= unaryexp */
#line 405 "stparserimpl.lemon"
{yylhsminor.yy178 = yymsp[0].minor.yy187;}
#line 1864 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 64: /* expression ::= binaryexp */
#line 406 "stparserimpl.lemon"
{yylhsminor.yy178 = yymsp[0].minor.yy172;}
#line 1870 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 65: /* expression ::= struct_init */
#line 407 "stparserimpl.lemon"
{ yylhsminor.yy178 = yymsp[0].minor.yy188;}
#line 1876 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 66: /* expression ::= expression AS type_qualified */
#line 409 "stparserimpl.lemon"
{
    yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprCast>(yymsp[-2].minor.yy178, yymsp[0].minor.yy169);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(),
                                                   yymsp[0].minor.yy169->getSourceInfo(),
                                                   pParser->getSourceHandle());
    yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 1888 "stparserimpl.c"
  yymsp[-2].minor.yy178 = yylhsminor.yy178;
        break;
      case 67: /* expression ::= identifier_or_self */
#line 417 "stparserimpl.lemon"
{
    yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprVarRef>(yymsp[0].minor.yy89);
    yylhsminor.yy178->setSourceInfo(yymsp[0].minor.yy89->getSourceInfo());
}
#line 1897 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 68: /* expression ::= expression DOT identifier */
#line 422 "stparserimpl.lemon"
{
    yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprStructVarRef>(yymsp[-2].minor.yy178, yymsp[0].minor.yy89);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy89->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 1907 "stparserimpl.c"
  yymsp[-2].minor.yy178 = yylhsminor.yy178;
        break;
      case 69: /* expression ::= identifier PAR_BEGIN function_call_args PAR_END */
#line 428 "stparserimpl.lemon"
{
    yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy89,yymsp[-1].minor.yy152,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy89->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 1917 "stparserimpl.c"
  yymsp[-3].minor.yy178 = yylhsminor.yy178;
        break;
      case 70: /* expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END */
#line 434 "stparserimpl.lemon"
{
    yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy89,yymsp[-5].minor.yy178,yymsp[-1].minor.yy152,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-5].minor.yy178->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 1927 "stparserimpl.c"
  yymsp[-5].minor.yy178 = yylhsminor.yy178;
        break;
      case 71: /* expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END */
#line 440 "stparserimpl.lemon"
{
    yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy89,yymsp[-6].minor.yy68,yymsp[-1].minor.yy152,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy68->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 1937 "stparserimpl.c"
  yymsp[-6].minor.yy178 = yylhsminor.yy178;
        break;
      case 72: /* binaryexp ::= expression AND expression */
#line 448 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 1948 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 73: /* binaryexp ::= expression OR expression */
#line 454 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 1959 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 74: /* binaryexp ::= expression PLUS expression */
#line 460 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 1970 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 75: /* binaryexp ::= expression MINUS expression */
#line 466 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 1981 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 76: /* binaryexp ::= expression MULT expression */
#line 472 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 1992 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 77: /* binaryexp ::= expression DIV expression */
#line 478 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
}
#line 2001 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 78: /* binaryexp ::= expression MOD expression */
#line 482 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 2012 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 79: /* binaryexp ::= expression EQ expression */
#line 489 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 2023 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 80: /* binaryexp ::= expression NE expression */
#line 496 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 2034 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 81: /* binaryexp ::= expression LE expression */
#line 503 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 2045 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 82: /* binaryexp ::= expression LT expression */
#line 510 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 2056 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 83: /* binaryexp ::= expression GE expression */
#line 517 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 2067 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 84: /* binaryexp ::= expression GT expression */
#line 524 "stparserimpl.lemon"
{
        yylhsminor.yy172 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy172->setSourceInfo(srcInfo);
}
#line 2078 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 85: /* unaryexp ::= NOT expression */
#line 533 "stparserimpl.lemon"
{
    yylhsminor.yy187 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy187->setSourceInfo(srcInfo);
}
#line 2089 "stparserimpl.c"
  yymsp[-1].minor.yy187 = yylhsminor.yy187;
        break;
      case 86: /* unaryexp ::= BIT_NOT expression */
#line 539 "stparserimpl.lemon"
{
    yylhsminor.yy187 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy187->setSourceInfo(srcInfo);
}
#line 2100 "stparserimpl.c"
  yymsp[-1].minor.yy187 = yylhsminor.yy187;
        break;
      case 87: /* unaryexp ::= MINUS expression */
#line 545 "stparserimpl.lemon"
{
    yylhsminor.yy187 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy187->setSourceInfo(srcInfo);
}
#line 2111 "stparserimpl.c"
  yymsp[-1].minor.yy187 = yylhsminor.yy187;
        break;
      case 88: /* unaryexp ::= REFERENCE expression */
#line 552 "stparserimpl.lemon"
{
    yylhsminor.yy187 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy187->setSourceInfo(srcInfo);
}
#line 2122 "stparserimpl.c"
  yymsp[-1].minor.yy187 = yylhsminor.yy187;
        break;
      case 89: /* unaryexp ::= MULT expression */
#line 559 "stparserimpl.lemon"
{
    yylhsminor.yy187 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Derefence,
                                               yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy187->setSourceInfo(srcInfo);
}
#line 2133 "stparserimpl.c"
  yymsp[-1].minor.yy187 = yylhsminor.yy187;
        break;
      case 90: /* function_call_args ::= function_call_args COMMA expression */
#line 569 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy152 = list.add(yymsp[-2].minor.yy152, yymsp[0].minor.yy178);
}
#line 2142 "stparserimpl.c"
  yymsp[-2].minor.yy152 = yylhsminor.yy152;
        break;
      case 91: /* function_call_args ::= expression */
#line 573 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy152 = list.add(yymsp[0].minor.yy178);
}
#line 2151 "stparserimpl.c"
  yymsp[0].minor.yy152 = yylhsminor.yy152;
        break;
      case 92: /* function_call_args ::= */
#line 577 "stparserimpl.lemon"
{
    yymsp[1].minor.yy152= pParser->getExprList().getRangeEmpty();
}
#line 2159 "stparserimpl.c"
        break;
      case 93: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
#line 584 "stparserimpl.lemon"
{
    yylhsminor.yy188 = pParser->createNode<yal::frontend::STExprStructInit>(yymsp[-3].minor.yy68, yymsp[-1].minor.yy80, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy68->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy188->setSourceInfo(srcInfo);
}
#line 2168 "stparserimpl.c"
  yymsp[-3].minor.yy188 = yylhsminor.yy188;
        break;
      case 94: /* struct_member_init_list ::= */
#line 590 "stparserimpl.lemon"
{yymsp[1].minor.yy80 = pParser->getStructInitList().getRangeEmpty();}
#line 2174 "stparserimpl.c"
        break;
      case 95: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
#line 591 "stparserimpl.lemon"
{
     auto& list = pParser->getStructInitList();
     yylhsminor.yy80 = list.add(yymsp[-2].minor.yy80, yymsp[0].minor.yy79);
}
#line 2182 "stparserimpl.c"
  yymsp[-2].minor.yy80 = yylhsminor.yy80;
        break;
      case 96: /* struct_member_init_list ::= struct_member_init */
#line 595 "stparserimpl.lemon"
{
    auto& list = pParser->getStructInitList();
    yylhsminor.yy80 = list.add(yymsp[0].minor.yy79);
}
#line 2191 "stparserimpl.c"
  yymsp[0].minor.yy80 = yylhsminor.yy80;
        break;
      case 97: /* struct_member_init ::= identifier COLON expression */
#line 600 "stparserimpl.lemon"
{
    yylhsminor.yy79 = pParser->createNode<yal::frontend::STStructMemberInit>(yymsp[-2].minor.yy89,yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy89->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy79->setSourceInfo(srcInfo);
}
#line 2201 "stparserimpl.c"
  yymsp[-2].minor.yy79 = yylhsminor.yy79;
        break;
      case 98: /* literal ::= INTEGER_LITERAL */
      case 99: /* literal ::= INTEGER_LITERAL_I8 */ yytestcase(yyruleno==99);
      case 100: /* literal ::= INTEGER_LITERAL_I16 */ yytestcase(yyruleno==100);
      case 101: /* literal ::= INTEGER_LITERAL_I32 */ yytestcase(yyruleno==101);
      case 102: /* literal ::= INTEGER_LITERAL_I64 */ yytestcase(yyruleno==102);
      case 103: /* literal ::= INTEGER_LITERAL_U8 */ yytestcase(yyruleno==103);
      case 104: /* literal ::= INTEGER_LITERAL_U16 */ yytestcase(yyruleno==104);
      case 105: /* literal ::= INTEGER_LITERAL_U32 */ yytestcase(yyruleno==105);
      case 106: /* literal ::= INTEGER_LITERAL_U64 */ yytestcase(yyruleno==106);
#line 608 "stparserimpl.lemon"
{
        yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprIntegerLiteral>(yymsp[0].minor.yy0);
        auto srcInfo =yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy178->setSourceInfo(srcInfo);
   }
#line 2219 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 107: /* literal ::= FLOAT_LITERAL */
      case 108: /* literal ::= FLOAT_LITERAL_32 */ yytestcase(yyruleno==108);
      case 109: /* literal ::= FLOAT_LITERAL_64 */ yytestcase(yyruleno==109);
#line 653 "stparserimpl.lemon"
{
        yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprFloatLiteral>(yymsp[0].minor.yy0);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 2231 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 110: /* literal ::= BOOL_LITERAL */
#line 670 "stparserimpl.lemon"
{
        yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 2241 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      default:
      /* (111) module ::= decls END */ yytestcase(yyruleno==111);
      /* (112) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==112);
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
#line 2325 "stparserimpl.c"
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
