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
#define YAL_TOKEN_SELF                           24
#define YAL_TOKEN_TYPE_BOOL                      25
#define YAL_TOKEN_TYPE_INT8                      26
#define YAL_TOKEN_TYPE_UINT8                     27
#define YAL_TOKEN_TYPE_INT16                     28
#define YAL_TOKEN_TYPE_UINT16                    29
#define YAL_TOKEN_TYPE_INT32                     30
#define YAL_TOKEN_TYPE_UINT32                    31
#define YAL_TOKEN_TYPE_INT64                     32
#define YAL_TOKEN_TYPE_UINT64                    33
#define YAL_TOKEN_TYPE_FLOAT                     34
#define YAL_TOKEN_TYPE_DOUBLE                    35
#define YAL_TOKEN_ARRAY_BEGIN                    36
#define YAL_TOKEN_ARRAY_END                      37
#define YAL_TOKEN_MUT                            38
#define YAL_TOKEN_REFERENCE                      39
#define YAL_TOKEN_TYPE                           40
#define YAL_TOKEN_COLON                          41
#define YAL_TOKEN_SEMI_COLON                     42
#define YAL_TOKEN_STRUCT                         43
#define YAL_TOKEN_SCOPE_BEGIN                    44
#define YAL_TOKEN_SCOPE_END                      45
#define YAL_TOKEN_COMMA                          46
#define YAL_TOKEN_PAR_BEGIN                      47
#define YAL_TOKEN_PAR_END                        48
#define YAL_TOKEN_FUNCTION                       49
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
#define YYNOCODE 93
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#endif
typedef union {
  int yyinit;
  YALSTParserTOKENTYPE yy0;
  yal::frontend::STExprStructInit::MemberInitList* yy32;
  yal::frontend::STExprStructInit* yy36;
  yal::frontend::STDeclFunction::Params* yy42;
  yal::frontend::STDecl* yy44;
  uint32_t yy53;
  yal::frontend::STDeclStruct* yy61;
  yal::StringRefPod yy70;
  yal::frontend::STStructMember* yy77;
  yal::frontend::STDeclParam* yy93;
  yal::frontend::STQualType* yy97;
  yal::frontend::STDeclVar* yy98;
  yal::frontend::STIdentifier* yy99;
  yal::frontend::STStatement* yy100;
  yal::frontend::STExprFnCall::ParamList* yy101;
  yal::frontend::STDeclModule* yy126;
  yal::frontend::STDeclFunction* yy127;
  yal::frontend::STStatementList* yy129;
  yal::frontend::STType* yy138;
  yal::frontend::STDeclStruct::Members* yy140;
  yal::frontend::STExprUnaryOperator* yy150;
  yal::frontend::STExprBinaryOperator* yy156;
  yal::frontend::STStructMemberInit* yy169;
  yal::frontend::STExpression* yy176;
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
#define YYNSTATE             121
#define YYNRULE              97
#define YY_MAX_SHIFT         120
#define YY_MIN_SHIFTREDUCE   182
#define YY_MAX_SHIFTREDUCE   278
#define YY_MIN_REDUCE        279
#define YY_MAX_REDUCE        375
#define YY_ERROR_ACTION      376
#define YY_ACCEPT_ACTION     377
#define YY_NO_ACTION         378
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
#define YY_ACTTAB_COUNT (1038)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    13,  118,  186,  116,   96,   14,   15,  186,  301,   86,
 /*    10 */   187,  193,  194,  195,  196,  197,  198,  199,  200,  201,
 /*    20 */   202,  203,  113,  206,  118,   12,   91,   89,   58,   82,
 /*    30 */   369,   40,  288,   31,   57,  112,    7,   53,   51,  108,
 /*    40 */   274,  275,  276,   85,  316,  279,  280,  281,   13,  118,
 /*    50 */    33,  305,  307,   14,   15,  288,  186,   86,  187,  193,
 /*    60 */   194,  195,  196,  197,  198,  199,  200,  201,  202,  203,
 /*    70 */    47,  113,  206,   12,   57,  112,  233,   25,   24,   23,
 /*    80 */   288,   31,  114,   55,  315,   34,  377,  108,  274,  275,
 /*    90 */   276,   13,  113,  206,  310,   87,   14,   15,  269,   45,
 /*   100 */    86,  187,  193,  194,  195,  196,  197,  198,  199,  200,
 /*   110 */   201,  202,  203,  320,  114,   36,   12,  333,  107,   36,
 /*   120 */    36,  318,   48,   36,   31,  311,  309,   36,   41,   96,
 /*   130 */   108,  274,  275,  276,   10,   28,   29,   16,  111,  246,
 /*   140 */    22,   21,   17,   19,   18,   20,   27,   26,   25,   24,
 /*   150 */    23,   16,   92,  247,   55,  368,   16,  118,  248,   84,
 /*   160 */    50,  110,  286,  341,   94,  102,   93,   37,   38,  215,
 /*   170 */    52,  289,  120,  101,  289,  231,  109,    3,   88,  325,
 /*   180 */    60,  337,  338,  336,  187,  339,  211,   46,  288,   56,
 /*   190 */    59,  118,  319,   84,  217,  110,  286,  341,   94,   32,
 /*   200 */   119,    1,   49,   90,    8,    9,  230,   11,  216,   55,
 /*   210 */    54,    3,   95,  325,   60,  337,  338,  336,    4,  339,
 /*   220 */    28,   29,  288,   30,  106,   22,   21,   17,   19,   18,
 /*   230 */    20,   27,   26,   25,   24,   23,   28,   29,   42,   55,
 /*   240 */     5,   22,   21,   17,   19,   18,   20,   27,   26,   25,
 /*   250 */    24,   23,    6,    2,   43,   55,  207,   44,   28,   29,
 /*   260 */   232,  209,  278,   22,   21,   17,   19,   18,   20,   27,
 /*   270 */    26,   25,   24,   23,   28,   29,  229,   55,  117,   22,
 /*   280 */    21,   17,   19,   18,   20,   27,   26,   25,   24,   23,
 /*   290 */   118,   39,   84,   55,  110,  286,  341,   94,   35,  374,
 /*   300 */   281,  281,  281,  281,  243,  281,  281,  281,  281,  281,
 /*   310 */   281,  281,  324,   60,  337,  338,  336,  281,  339,  281,
 /*   320 */   238,  288,  281,   28,   29,  281,  281,  281,   22,   21,
 /*   330 */    17,   19,   18,   20,   27,   26,   25,   24,   23,  281,
 /*   340 */   281,  281,   55,  281,  281,  192,  281,  193,  194,  195,
 /*   350 */   196,  197,  198,  199,  200,  201,  202,  203,  281,  281,
 /*   360 */   281,  281,  281,  281,  281,  115,  281,  281,  281,   29,
 /*   370 */   281,  281,  281,   22,   21,   17,   19,   18,   20,   27,
 /*   380 */    26,   25,   24,   23,  118,  281,   84,   55,  110,  286,
 /*   390 */   341,  281,  281,  118,  281,   84,  281,  110,  286,  341,
 /*   400 */   281,  281,  281,  281,  281,  281,  281,   69,  337,  338,
 /*   410 */   336,   83,  339,  281,  281,  288,   69,  337,  338,  336,
 /*   420 */    81,  339,  281,  281,  288,  281,  281,  281,  281,  281,
 /*   430 */   118,  281,   84,  281,  110,  286,  341,  281,  281,   86,
 /*   440 */   281,  193,  194,  195,  196,  197,  198,  199,  200,  201,
 /*   450 */   202,  203,  281,   69,  337,  338,  336,   80,  339,  281,
 /*   460 */   192,  288,  193,  194,  195,  196,  197,  198,  199,  200,
 /*   470 */   201,  202,  203,  376,  376,  376,  376,  376,  376,   27,
 /*   480 */    26,   25,   24,   23,  281,  281,  281,   55,   22,   21,
 /*   490 */    17,   19,   18,   20,   27,   26,   25,   24,   23,  281,
 /*   500 */   281,  281,   55,  118,  281,   84,  281,  110,  286,  341,
 /*   510 */   281,  281,  118,  281,   84,  281,  110,  286,  341,  281,
 /*   520 */   281,  281,  281,  281,  281,  281,   61,  337,  338,  336,
 /*   530 */   281,  339,  281,  281,  288,   65,  337,  338,  336,  281,
 /*   540 */   339,  281,  118,  288,   84,  281,  110,  286,  341,  281,
 /*   550 */   281,  118,  281,   84,  281,  110,  286,  341,  281,  281,
 /*   560 */   281,  281,  281,  281,  281,   66,  337,  338,  336,  281,
 /*   570 */   339,  281,  281,  288,   62,  337,  338,  336,  281,  339,
 /*   580 */   281,  281,  288,  281,  281,  281,  281,  118,  281,   84,
 /*   590 */   281,  110,  286,  341,  281,  281,  118,  281,   84,  281,
 /*   600 */   110,  286,  341,  281,  281,  281,  281,  281,  281,  281,
 /*   610 */    67,  337,  338,  336,  281,  339,  281,  281,  288,   97,
 /*   620 */   337,  338,  336,  281,  339,  281,  118,  288,   84,  281,
 /*   630 */   110,  286,  341,  281,  281,  118,  281,   84,  281,  110,
 /*   640 */   286,  341,  281,  281,  281,  281,  281,  281,  281,   98,
 /*   650 */   337,  338,  336,  281,  339,  281,  281,  288,   99,  337,
 /*   660 */   338,  336,  281,  339,  281,  281,  288,  281,  281,  281,
 /*   670 */   281,  118,  281,   84,  281,  110,  286,  341,  281,  281,
 /*   680 */   118,  281,   84,  281,  110,  286,  341,  281,  281,  281,
 /*   690 */   281,  281,  281,  281,  100,  337,  338,  336,  281,  339,
 /*   700 */   281,  281,  288,   68,  337,  338,  336,  281,  339,  281,
 /*   710 */   118,  288,   84,  281,  110,  286,  341,  281,  281,  118,
 /*   720 */   281,   84,  281,  110,  286,  341,  281,  281,  281,  281,
 /*   730 */   281,  281,  281,   71,  337,  338,  336,  281,  339,  281,
 /*   740 */   281,  288,   72,  337,  338,  336,  281,  339,  281,  281,
 /*   750 */   288,  281,  281,  281,  281,  118,  281,   84,  281,  110,
 /*   760 */   286,  341,  281,  281,  118,  281,   84,  281,  110,  286,
 /*   770 */   341,  281,  281,  281,  281,  281,  281,  281,   73,  337,
 /*   780 */   338,  336,  281,  339,  281,  281,  288,   74,  337,  338,
 /*   790 */   336,  281,  339,  281,  118,  288,   84,  281,  110,  286,
 /*   800 */   341,  281,  281,  118,  281,   84,  281,  110,  286,  341,
 /*   810 */   281,  281,  281,  281,  281,  281,  281,   75,  337,  338,
 /*   820 */   336,  281,  339,  281,  281,  288,   76,  337,  338,  336,
 /*   830 */   281,  339,  281,  281,  288,  281,  281,  281,  281,  118,
 /*   840 */   281,   84,  281,  110,  286,  341,  281,  281,  118,  281,
 /*   850 */    84,  281,  110,  286,  341,  281,  281,  281,  281,  281,
 /*   860 */   281,  281,  103,  337,  338,  336,  281,  339,  281,  281,
 /*   870 */   288,  104,  337,  338,  336,  281,  339,  281,  118,  288,
 /*   880 */    84,  281,  110,  286,  341,  281,  281,  118,  281,   84,
 /*   890 */   281,  110,  286,  341,  281,  281,  281,  281,  281,  281,
 /*   900 */   281,  105,  337,  338,  336,  281,  339,  281,  281,  288,
 /*   910 */    78,  337,  338,  336,  281,  339,  281,  281,  288,  281,
 /*   920 */   281,  281,  118,  281,   84,  281,  110,  286,  341,  281,
 /*   930 */   281,  118,  281,   84,  281,  110,  286,  341,  281,  281,
 /*   940 */   281,  281,  281,  281,  281,   79,  337,  338,  336,  281,
 /*   950 */   339,  281,  281,  288,   70,  337,  338,  336,  281,  339,
 /*   960 */   281,  281,  288,  118,  281,   84,  281,  110,  286,  341,
 /*   970 */   281,  281,  118,  281,   84,  281,  110,  286,  341,  281,
 /*   980 */   281,  281,  281,  281,  281,  281,   77,  337,  338,  336,
 /*   990 */   281,  339,  281,  281,  288,   63,  337,  338,  336,  281,
 /*  1000 */   339,  281,  281,  288,  281,  281,  118,  281,   84,  281,
 /*  1010 */   110,  286,  341,  281,  281,  281,  281,  281,  281,  281,
 /*  1020 */   281,  281,  281,  281,  281,  281,  281,  281,  281,   64,
 /*  1030 */   337,  338,  336,  281,  339,  281,  281,  288,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    14,   60,   23,   62,   64,   19,   20,   23,   24,   23,
 /*    10 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*    20 */    34,   35,   38,   39,   60,   39,   62,   71,   64,   89,
 /*    30 */    90,   41,   91,   47,   63,   64,   50,   51,   52,   53,
 /*    40 */    54,   55,   56,   72,   73,   68,   69,   70,   14,   60,
 /*    50 */    47,   62,   75,   19,   20,   91,   23,   23,   24,   25,
 /*    60 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*    70 */    74,   38,   39,   39,   63,   64,   42,   15,   16,   17,
 /*    80 */    91,   47,   64,   21,   73,   58,   59,   53,   54,   55,
 /*    90 */    56,   14,   38,   39,   76,   77,   19,   20,   45,   46,
 /*   100 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*   110 */    33,   34,   35,   61,   64,   63,   39,   61,   61,   63,
 /*   120 */    63,   61,   64,   63,   47,   61,   76,   63,   41,   64,
 /*   130 */    53,   54,   55,   56,    1,    2,    3,   46,   71,   48,
 /*   140 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   150 */    17,   46,   78,   48,   21,   90,   46,   60,   48,   62,
 /*   160 */    64,   64,   65,   66,   67,   41,   78,   46,   44,   48,
 /*   170 */    64,   41,   22,   64,   44,   42,   64,   80,   81,   82,
 /*   180 */    83,   84,   85,   86,   24,   88,   45,   46,   91,   74,
 /*   190 */    40,   60,   65,   62,   45,   64,   65,   66,   67,   49,
 /*   200 */    64,   44,   41,   41,    1,    1,   42,   41,   45,   21,
 /*   210 */    41,   80,   81,   82,   83,   84,   85,   86,   47,   88,
 /*   220 */     2,    3,   91,   47,    9,    7,    8,    9,   10,   11,
 /*   230 */    12,   13,   14,   15,   16,   17,    2,    3,   10,   21,
 /*   240 */    47,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   250 */    16,   17,   47,   44,   41,   21,   39,   41,    2,    3,
 /*   260 */    42,   42,   37,    7,    8,    9,   10,   11,   12,   13,
 /*   270 */    14,   15,   16,   17,    2,    3,   42,   21,   36,    7,
 /*   280 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   290 */    60,   44,   62,   21,   64,   65,   66,   67,   41,    0,
 /*   300 */    92,   92,   92,   92,   48,   92,   92,   92,   92,   92,
 /*   310 */    92,   92,   82,   83,   84,   85,   86,   92,   88,   92,
 /*   320 */    48,   91,   92,    2,    3,   92,   92,   92,    7,    8,
 /*   330 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   92,
 /*   340 */    92,   92,   21,   92,   92,   23,   92,   25,   26,   27,
 /*   350 */    28,   29,   30,   31,   32,   33,   34,   35,   92,   92,
 /*   360 */    92,   92,   92,   92,   92,   43,   92,   92,   92,    3,
 /*   370 */    92,   92,   92,    7,    8,    9,   10,   11,   12,   13,
 /*   380 */    14,   15,   16,   17,   60,   92,   62,   21,   64,   65,
 /*   390 */    66,   92,   92,   60,   92,   62,   92,   64,   65,   66,
 /*   400 */    92,   92,   92,   92,   92,   92,   92,   83,   84,   85,
 /*   410 */    86,   87,   88,   92,   92,   91,   83,   84,   85,   86,
 /*   420 */    87,   88,   92,   92,   91,   92,   92,   92,   92,   92,
 /*   430 */    60,   92,   62,   92,   64,   65,   66,   92,   92,   23,
 /*   440 */    92,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   450 */    34,   35,   92,   83,   84,   85,   86,   87,   88,   92,
 /*   460 */    23,   91,   25,   26,   27,   28,   29,   30,   31,   32,
 /*   470 */    33,   34,   35,    7,    8,    9,   10,   11,   12,   13,
 /*   480 */    14,   15,   16,   17,   92,   92,   92,   21,    7,    8,
 /*   490 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   92,
 /*   500 */    92,   92,   21,   60,   92,   62,   92,   64,   65,   66,
 /*   510 */    92,   92,   60,   92,   62,   92,   64,   65,   66,   92,
 /*   520 */    92,   92,   92,   92,   92,   92,   83,   84,   85,   86,
 /*   530 */    92,   88,   92,   92,   91,   83,   84,   85,   86,   92,
 /*   540 */    88,   92,   60,   91,   62,   92,   64,   65,   66,   92,
 /*   550 */    92,   60,   92,   62,   92,   64,   65,   66,   92,   92,
 /*   560 */    92,   92,   92,   92,   92,   83,   84,   85,   86,   92,
 /*   570 */    88,   92,   92,   91,   83,   84,   85,   86,   92,   88,
 /*   580 */    92,   92,   91,   92,   92,   92,   92,   60,   92,   62,
 /*   590 */    92,   64,   65,   66,   92,   92,   60,   92,   62,   92,
 /*   600 */    64,   65,   66,   92,   92,   92,   92,   92,   92,   92,
 /*   610 */    83,   84,   85,   86,   92,   88,   92,   92,   91,   83,
 /*   620 */    84,   85,   86,   92,   88,   92,   60,   91,   62,   92,
 /*   630 */    64,   65,   66,   92,   92,   60,   92,   62,   92,   64,
 /*   640 */    65,   66,   92,   92,   92,   92,   92,   92,   92,   83,
 /*   650 */    84,   85,   86,   92,   88,   92,   92,   91,   83,   84,
 /*   660 */    85,   86,   92,   88,   92,   92,   91,   92,   92,   92,
 /*   670 */    92,   60,   92,   62,   92,   64,   65,   66,   92,   92,
 /*   680 */    60,   92,   62,   92,   64,   65,   66,   92,   92,   92,
 /*   690 */    92,   92,   92,   92,   83,   84,   85,   86,   92,   88,
 /*   700 */    92,   92,   91,   83,   84,   85,   86,   92,   88,   92,
 /*   710 */    60,   91,   62,   92,   64,   65,   66,   92,   92,   60,
 /*   720 */    92,   62,   92,   64,   65,   66,   92,   92,   92,   92,
 /*   730 */    92,   92,   92,   83,   84,   85,   86,   92,   88,   92,
 /*   740 */    92,   91,   83,   84,   85,   86,   92,   88,   92,   92,
 /*   750 */    91,   92,   92,   92,   92,   60,   92,   62,   92,   64,
 /*   760 */    65,   66,   92,   92,   60,   92,   62,   92,   64,   65,
 /*   770 */    66,   92,   92,   92,   92,   92,   92,   92,   83,   84,
 /*   780 */    85,   86,   92,   88,   92,   92,   91,   83,   84,   85,
 /*   790 */    86,   92,   88,   92,   60,   91,   62,   92,   64,   65,
 /*   800 */    66,   92,   92,   60,   92,   62,   92,   64,   65,   66,
 /*   810 */    92,   92,   92,   92,   92,   92,   92,   83,   84,   85,
 /*   820 */    86,   92,   88,   92,   92,   91,   83,   84,   85,   86,
 /*   830 */    92,   88,   92,   92,   91,   92,   92,   92,   92,   60,
 /*   840 */    92,   62,   92,   64,   65,   66,   92,   92,   60,   92,
 /*   850 */    62,   92,   64,   65,   66,   92,   92,   92,   92,   92,
 /*   860 */    92,   92,   83,   84,   85,   86,   92,   88,   92,   92,
 /*   870 */    91,   83,   84,   85,   86,   92,   88,   92,   60,   91,
 /*   880 */    62,   92,   64,   65,   66,   92,   92,   60,   92,   62,
 /*   890 */    92,   64,   65,   66,   92,   92,   92,   92,   92,   92,
 /*   900 */    92,   83,   84,   85,   86,   92,   88,   92,   92,   91,
 /*   910 */    83,   84,   85,   86,   92,   88,   92,   92,   91,   92,
 /*   920 */    92,   92,   60,   92,   62,   92,   64,   65,   66,   92,
 /*   930 */    92,   60,   92,   62,   92,   64,   65,   66,   92,   92,
 /*   940 */    92,   92,   92,   92,   92,   83,   84,   85,   86,   92,
 /*   950 */    88,   92,   92,   91,   83,   84,   85,   86,   92,   88,
 /*   960 */    92,   92,   91,   60,   92,   62,   92,   64,   65,   66,
 /*   970 */    92,   92,   60,   92,   62,   92,   64,   65,   66,   92,
 /*   980 */    92,   92,   92,   92,   92,   92,   83,   84,   85,   86,
 /*   990 */    92,   88,   92,   92,   91,   83,   84,   85,   86,   92,
 /*  1000 */    88,   92,   92,   91,   92,   92,   60,   92,   62,   92,
 /*  1010 */    64,   65,   66,   92,   92,   92,   92,   92,   92,   92,
 /*  1020 */    92,   92,   92,   92,   92,   92,   92,   92,   92,   83,
 /*  1030 */    84,   85,   86,   92,   88,   92,   92,   91,
};
#define YY_SHIFT_USE_DFLT (1038)
#define YY_SHIFT_COUNT    (120)
#define YY_SHIFT_MIN      (-21)
#define YY_SHIFT_MAX      (481)
static const short yy_shift_ofst[] = {
 /*     0 */  1038,  -14,  -14,  -14,   77,   77,   77,   34,   77,   77,
 /*    10 */    77,   77,   77,   77,   77,   77,   77,   77,   77,   77,
 /*    20 */    77,   77,   77,   77,   77,   77,   77,   77,   77,   77,
 /*    30 */    77,   77,  416,  -16,  150,  322,  437,   33,  -21,  -21,
 /*    40 */    54,   54,   54,   54,   54,  -21,  -21,  -10,    3,  -21,
 /*    50 */    87,  -21,   87,  -21,  -21,  -21,  -10,  160,    3,  -21,
 /*    60 */   133,  218,  234,  256,  272,  321,  321,  321,  321,  321,
 /*    70 */   366,  466,  466,  466,  466,  466,  466,  481,   62,   62,
 /*    80 */    91,  105,   53,  110,  124,  121,  130,  141,  149,  157,
 /*    90 */   161,  162,  203,  204,  164,  163,  166,  188,  188,  188,
 /*   100 */   188,  171,  169,  188,  188,  188,  176,  215,  228,  193,
 /*   110 */   205,  209,  213,  217,  216,  247,  219,  225,  242,  257,
 /*   120 */   299,
};
#define YY_REDUCE_USE_DFLT (-61)
#define YY_REDUCE_COUNT (59)
#define YY_REDUCE_MIN   (-60)
#define YY_REDUCE_MAX   (946)
static const short yy_reduce_ofst[] = {
 /*     0 */    27,   97,  131,  230,  324,  333,  370,  443,  452,  482,
 /*    10 */   491,  527,  536,  566,  575,  611,  620,  650,  659,  695,
 /*    20 */   704,  734,  743,  779,  788,  818,  827,  862,  871,  903,
 /*    30 */   912,  946,  -36,  -29,  -23,  -59,  -11,   11,  -60,   18,
 /*    40 */    52,   56,   57,   60,   64,   65,   50,  -44,   -4,   58,
 /*    50 */    74,   96,   88,  106,  109,  112,   67,  127,  115,  136,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   282,  322,  322,  323,  365,  365,  365,  376,  376,  376,
 /*    10 */   376,  376,  376,  376,  376,  376,  376,  376,  376,  376,
 /*    20 */   376,  376,  376,  376,  376,  376,  376,  376,  376,  376,
 /*    30 */   376,  376,  376,  317,  376,  376,  376,  301,  367,  376,
 /*    40 */   301,  301,  301,  301,  301,  376,  376,  321,  376,  376,
 /*    50 */   334,  376,  334,  376,  376,  376,  321,  376,  376,  376,
 /*    60 */   376,  376,  376,  376,  376,  332,  331,  370,  363,  364,
 /*    70 */   347,  358,  357,  356,  355,  354,  353,  346,  349,  348,
 /*    80 */   376,  376,  376,  376,  376,  376,  283,  376,  376,  376,
 /*    90 */   376,  376,  376,  376,  376,  376,  376,  362,  361,  360,
 /*   100 */   359,  376,  376,  352,  351,  350,  376,  376,  376,  342,
 /*   110 */   285,  376,  376,  302,  376,  376,  376,  376,  287,  376,
 /*   120 */   376,
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
  "$",             "ASSIGN",        "OR",            "AND",         
  "BIT_OR",        "BIT_XOR",       "BIT_AND",       "EQ",          
  "NE",            "GT",            "LT",            "GE",          
  "LE",            "PLUS",          "MINUS",         "MULT",        
  "DIV",           "MOD",           "NEGATE",        "BIT_NOT",     
  "NOT",           "DOT",           "END",           "IDENTIFIER",  
  "SELF",          "TYPE_BOOL",     "TYPE_INT8",     "TYPE_UINT8",  
  "TYPE_INT16",    "TYPE_UINT16",   "TYPE_INT32",    "TYPE_UINT32", 
  "TYPE_INT64",    "TYPE_UINT64",   "TYPE_FLOAT",    "TYPE_DOUBLE", 
  "ARRAY_BEGIN",   "ARRAY_END",     "MUT",           "REFERENCE",   
  "TYPE",          "COLON",         "SEMI_COLON",    "STRUCT",      
  "SCOPE_BEGIN",   "SCOPE_END",     "COMMA",         "PAR_BEGIN",   
  "PAR_END",       "FUNCTION",      "RETURN",        "VAR",         
  "LET",           "RANGE_CAST",    "INTEGER_LITERAL",  "FLOAT_LITERAL",
  "BOOL_LITERAL",  "error",         "decls",         "module",      
  "type_builtin",  "type_qualified",  "type_specifier",  "qualifier",   
  "identifier",    "self",          "identifier_or_self",  "decl_var",    
  "decl_type",     "decl_function",  "decl_type_function",  "function_return_decl",
  "function_args_decl",  "function_arg_decl",  "function_param_list",  "decl_struct", 
  "struct_decl_var",  "struct_decl_vars",  "var_type_spec",  "named_decl",  
  "statement_list",  "statement_list_or_empty",  "statement",     "expression",  
  "unaryexp",      "binaryexp",     "literal",       "function_call_args",
  "struct_init",   "struct_member_init_list",  "struct_member_init",  "type_array",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "decls ::= decls decl_type",
 /*   1 */ "decls ::= decls decl_function",
 /*   2 */ "decls ::= decls decl_type_function",
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
 /*  35 */ "decl_type_function ::= FUNCTION type_specifier COLON COLON identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  36 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  37 */ "function_args_decl ::= function_arg_decl",
 /*  38 */ "function_args_decl ::=",
 /*  39 */ "function_arg_decl ::= identifier COLON type_qualified",
 /*  40 */ "function_arg_decl ::= qualifier self",
 /*  41 */ "function_return_decl ::= COLON type_qualified",
 /*  42 */ "function_return_decl ::=",
 /*  43 */ "statement_list_or_empty ::=",
 /*  44 */ "statement_list_or_empty ::= statement_list",
 /*  45 */ "statement_list ::= statement_list statement",
 /*  46 */ "statement_list ::= statement",
 /*  47 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  48 */ "statement ::= decl_var SEMI_COLON",
 /*  49 */ "statement ::= expression SEMI_COLON",
 /*  50 */ "statement ::= RETURN expression SEMI_COLON",
 /*  51 */ "statement ::= RETURN SEMI_COLON",
 /*  52 */ "decl_var ::= VAR identifier var_type_spec ASSIGN expression",
 /*  53 */ "decl_var ::= LET identifier var_type_spec ASSIGN expression",
 /*  54 */ "var_type_spec ::= COLON type_qualified",
 /*  55 */ "var_type_spec ::=",
 /*  56 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  57 */ "expression ::= literal",
 /*  58 */ "expression ::= unaryexp",
 /*  59 */ "expression ::= binaryexp",
 /*  60 */ "expression ::= struct_init",
 /*  61 */ "expression ::= RANGE_CAST LT type_qualified GT PAR_BEGIN expression PAR_END",
 /*  62 */ "expression ::= identifier_or_self",
 /*  63 */ "expression ::= expression DOT identifier",
 /*  64 */ "expression ::= identifier PAR_BEGIN function_call_args PAR_END",
 /*  65 */ "expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END",
 /*  66 */ "expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END",
 /*  67 */ "binaryexp ::= expression AND expression",
 /*  68 */ "binaryexp ::= expression OR expression",
 /*  69 */ "binaryexp ::= expression PLUS expression",
 /*  70 */ "binaryexp ::= expression MINUS expression",
 /*  71 */ "binaryexp ::= expression MULT expression",
 /*  72 */ "binaryexp ::= expression DIV expression",
 /*  73 */ "binaryexp ::= expression MOD expression",
 /*  74 */ "binaryexp ::= expression EQ expression",
 /*  75 */ "binaryexp ::= expression NE expression",
 /*  76 */ "binaryexp ::= expression LE expression",
 /*  77 */ "binaryexp ::= expression LT expression",
 /*  78 */ "binaryexp ::= expression GE expression",
 /*  79 */ "binaryexp ::= expression GT expression",
 /*  80 */ "unaryexp ::= NOT expression",
 /*  81 */ "unaryexp ::= BIT_NOT expression",
 /*  82 */ "unaryexp ::= MINUS expression",
 /*  83 */ "unaryexp ::= REFERENCE expression",
 /*  84 */ "function_call_args ::= function_call_args COMMA expression",
 /*  85 */ "function_call_args ::= expression",
 /*  86 */ "function_call_args ::=",
 /*  87 */ "struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END",
 /*  88 */ "struct_member_init_list ::=",
 /*  89 */ "struct_member_init_list ::= struct_member_init_list COMMA struct_member_init",
 /*  90 */ "struct_member_init_list ::= struct_member_init",
 /*  91 */ "struct_member_init ::= identifier COLON expression",
 /*  92 */ "literal ::= INTEGER_LITERAL",
 /*  93 */ "literal ::= FLOAT_LITERAL",
 /*  94 */ "literal ::= BOOL_LITERAL",
 /*  95 */ "module ::= decls END",
 /*  96 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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
  { 58, -2 },
  { 58, -2 },
  { 58, -2 },
  { 58, 0 },
  { 64, -1 },
  { 65, -1 },
  { 66, -1 },
  { 66, -1 },
  { 62, -1 },
  { 62, -1 },
  { 62, -1 },
  { 60, -1 },
  { 60, -1 },
  { 60, -1 },
  { 60, -1 },
  { 60, -1 },
  { 60, -1 },
  { 60, -1 },
  { 60, -1 },
  { 60, -1 },
  { 60, -1 },
  { 60, -1 },
  { 63, 0 },
  { 63, -1 },
  { 63, -1 },
  { 63, -2 },
  { 61, -2 },
  { 68, -5 },
  { 68, -1 },
  { 75, -7 },
  { 77, -3 },
  { 77, -1 },
  { 76, -3 },
  { 74, -3 },
  { 69, -7 },
  { 70, -10 },
  { 72, -3 },
  { 72, -1 },
  { 72, 0 },
  { 73, -3 },
  { 73, -2 },
  { 71, -2 },
  { 71, 0 },
  { 81, 0 },
  { 81, -1 },
  { 80, -2 },
  { 80, -1 },
  { 82, -4 },
  { 82, -2 },
  { 82, -2 },
  { 82, -3 },
  { 82, -2 },
  { 67, -5 },
  { 67, -5 },
  { 78, -2 },
  { 78, 0 },
  { 83, -3 },
  { 83, -1 },
  { 83, -1 },
  { 83, -1 },
  { 83, -1 },
  { 83, -7 },
  { 83, -1 },
  { 83, -3 },
  { 83, -4 },
  { 83, -6 },
  { 83, -7 },
  { 85, -3 },
  { 85, -3 },
  { 85, -3 },
  { 85, -3 },
  { 85, -3 },
  { 85, -3 },
  { 85, -3 },
  { 85, -3 },
  { 85, -3 },
  { 85, -3 },
  { 85, -3 },
  { 85, -3 },
  { 85, -3 },
  { 84, -2 },
  { 84, -2 },
  { 84, -2 },
  { 84, -2 },
  { 87, -3 },
  { 87, -1 },
  { 87, 0 },
  { 88, -4 },
  { 89, 0 },
  { 89, -3 },
  { 89, -1 },
  { 90, -3 },
  { 86, -1 },
  { 86, -1 },
  { 86, -1 },
  { 59, -2 },
  { 91, -3 },
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
#line 125 "stparserimpl.lemon"
{
    yylhsminor.yy126 =yymsp[-1].minor.yy126; yymsp[-1].minor.yy126->addDecl(yymsp[0].minor.yy44);
}
#line 1306 "stparserimpl.c"
  yymsp[-1].minor.yy126 = yylhsminor.yy126;
        break;
      case 1: /* decls ::= decls decl_function */
      case 2: /* decls ::= decls decl_type_function */ yytestcase(yyruleno==2);
#line 128 "stparserimpl.lemon"
{
        yylhsminor.yy126 =yymsp[-1].minor.yy126; yymsp[-1].minor.yy126->addDecl(yymsp[0].minor.yy127);
}
#line 1315 "stparserimpl.c"
  yymsp[-1].minor.yy126 = yylhsminor.yy126;
        break;
      case 3: /* decls ::= */
#line 134 "stparserimpl.lemon"
{
        yymsp[1].minor.yy126 =  pParser->getDeclModule();
     }
#line 1323 "stparserimpl.c"
        break;
      case 4: /* identifier ::= IDENTIFIER */
      case 5: /* self ::= SELF */ yytestcase(yyruleno==5);
#line 141 "stparserimpl.lemon"
{
        yylhsminor.yy99 = pParser->createNode<yal::frontend::STIdentifier>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy99->setSourceInfo(srcInfo);
}
#line 1333 "stparserimpl.c"
  yymsp[0].minor.yy99 = yylhsminor.yy99;
        break;
      case 6: /* identifier_or_self ::= identifier */
      case 7: /* identifier_or_self ::= self */ yytestcase(yyruleno==7);
#line 153 "stparserimpl.lemon"
{yylhsminor.yy99=yymsp[0].minor.yy99;}
#line 1340 "stparserimpl.c"
  yymsp[0].minor.yy99 = yylhsminor.yy99;
        break;
      case 8: /* type_specifier ::= type_builtin */
#line 156 "stparserimpl.lemon"
{
        yylhsminor.yy138=yymsp[0].minor.yy138;
}
#line 1348 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 9: /* type_specifier ::= type_array */
#line 160 "stparserimpl.lemon"
{yymsp[0].minor.yy138 = nullptr;}
#line 1354 "stparserimpl.c"
        break;
      case 10: /* type_specifier ::= IDENTIFIER */
#line 161 "stparserimpl.lemon"
{
    yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yymsp[0].minor.yy0.tokenStr);
    yylhsminor.yy138->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0,yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1362 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 11: /* type_builtin ::= TYPE_BOOL */
#line 166 "stparserimpl.lemon"
{
    yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Bool);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy138->setSourceInfo(srcInfo);
    }
#line 1372 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 12: /* type_builtin ::= TYPE_INT8 */
#line 171 "stparserimpl.lemon"
{
        yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy138->setSourceInfo(srcInfo);
    }
#line 1382 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 13: /* type_builtin ::= TYPE_UINT8 */
#line 176 "stparserimpl.lemon"
{
        yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy138->setSourceInfo(srcInfo);
    }
#line 1392 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 14: /* type_builtin ::= TYPE_INT16 */
#line 181 "stparserimpl.lemon"
{
        yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy138->setSourceInfo(srcInfo);
    }
#line 1402 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 15: /* type_builtin ::= TYPE_UINT16 */
#line 186 "stparserimpl.lemon"
{
        yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy138->setSourceInfo(srcInfo);
    }
#line 1412 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 16: /* type_builtin ::= TYPE_INT32 */
#line 191 "stparserimpl.lemon"
{
        yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy138->setSourceInfo(srcInfo);
    }
#line 1422 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 17: /* type_builtin ::= TYPE_UINT32 */
#line 196 "stparserimpl.lemon"
{
        yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy138->setSourceInfo(srcInfo);
    }
#line 1432 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 18: /* type_builtin ::= TYPE_INT64 */
#line 201 "stparserimpl.lemon"
{
        yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy138->setSourceInfo(srcInfo);
    }
#line 1442 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 19: /* type_builtin ::= TYPE_UINT64 */
#line 206 "stparserimpl.lemon"
{
        yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy138->setSourceInfo(srcInfo);
    }
#line 1452 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 20: /* type_builtin ::= TYPE_FLOAT */
#line 211 "stparserimpl.lemon"
{
        yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy138->setSourceInfo(srcInfo);
    }
#line 1462 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 21: /* type_builtin ::= TYPE_DOUBLE */
#line 216 "stparserimpl.lemon"
{
        yylhsminor.yy138 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy138->setSourceInfo(srcInfo);
    }
#line 1472 "stparserimpl.c"
  yymsp[0].minor.yy138 = yylhsminor.yy138;
        break;
      case 22: /* qualifier ::= */
#line 224 "stparserimpl.lemon"
{yymsp[1].minor.yy53 = 0;}
#line 1478 "stparserimpl.c"
        break;
      case 23: /* qualifier ::= MUT */
#line 225 "stparserimpl.lemon"
{yymsp[0].minor.yy53 = yal::frontend::STQualType::Qualifiers::kQualMutable;}
#line 1483 "stparserimpl.c"
        break;
      case 24: /* qualifier ::= REFERENCE */
#line 226 "stparserimpl.lemon"
{yymsp[0].minor.yy53 = yal::frontend::STQualType::Qualifiers::kQualReference;}
#line 1488 "stparserimpl.c"
        break;
      case 25: /* qualifier ::= MUT REFERENCE */
#line 227 "stparserimpl.lemon"
{
        yymsp[-1].minor.yy53 = yal::frontend::STQualType::Qualifiers::kQualReference | yal::frontend::STQualType::Qualifiers::kQualMutable;
 }
#line 1495 "stparserimpl.c"
        break;
      case 26: /* type_qualified ::= qualifier type_specifier */
#line 230 "stparserimpl.lemon"
{
    yylhsminor.yy97 = pParser->createNode<yal::frontend::STQualType>(yymsp[0].minor.yy138, yymsp[-1].minor.yy53);
}
#line 1502 "stparserimpl.c"
  yymsp[-1].minor.yy97 = yylhsminor.yy97;
        break;
      case 27: /* decl_type ::= TYPE identifier COLON type_specifier SEMI_COLON */
#line 236 "stparserimpl.lemon"
{
    yymsp[-4].minor.yy44 = nullptr;
}
#line 1510 "stparserimpl.c"
        break;
      case 28: /* decl_type ::= decl_struct */
#line 240 "stparserimpl.lemon"
{ yylhsminor.yy44 = yymsp[0].minor.yy61;}
#line 1515 "stparserimpl.c"
  yymsp[0].minor.yy44 = yylhsminor.yy44;
        break;
      case 29: /* decl_struct ::= TYPE identifier COLON STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END */
#line 245 "stparserimpl.lemon"
{
    yylhsminor.yy61 = pParser->createNode<yal::frontend::STDeclStruct>(yymsp[-5].minor.yy99, yymsp[-1].minor.yy140);
    auto srcInfo = CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy61->setSourceInfo(srcInfo);
}
#line 1525 "stparserimpl.c"
  yymsp[-6].minor.yy61 = yylhsminor.yy61;
        break;
      case 30: /* struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
#line 251 "stparserimpl.lemon"
{
    yylhsminor.yy140 = yymsp[-2].minor.yy140;
    yylhsminor.yy140->push_back(yymsp[0].minor.yy77);
}
#line 1534 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 31: /* struct_decl_vars ::= struct_decl_var */
#line 255 "stparserimpl.lemon"
{
    yylhsminor.yy140 = pParser->createVector<const yal::frontend::STStructMember*>();
    yylhsminor.yy140->push_back(yymsp[0].minor.yy77);
}
#line 1543 "stparserimpl.c"
  yymsp[0].minor.yy140 = yylhsminor.yy140;
        break;
      case 32: /* struct_decl_var ::= identifier COLON type_qualified */
#line 260 "stparserimpl.lemon"
{
    yylhsminor.yy77 = pParser->createNode<yal::frontend::STStructMember>(yymsp[-2].minor.yy99, yymsp[0].minor.yy97);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy99->getSourceInfo(), yymsp[0].minor.yy97->getSourceInfo(),pParser->getSourceHandle());
    yylhsminor.yy77->setSourceInfo(srcInfo);
}
#line 1553 "stparserimpl.c"
  yymsp[-2].minor.yy77 = yylhsminor.yy77;
        break;
      case 33: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
#line 269 "stparserimpl.lemon"
{
    yymsp[-2].minor.yy42 = yymsp[-1].minor.yy42;
}
#line 1561 "stparserimpl.c"
        break;
      case 34: /* decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 274 "stparserimpl.lemon"
{
    yylhsminor.yy127 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy99, nullptr, yymsp[-3].minor.yy97, yymsp[-4].minor.yy42, yymsp[-1].minor.yy129);
    yylhsminor.yy127->setSourceInfo(CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1569 "stparserimpl.c"
  yymsp[-6].minor.yy127 = yylhsminor.yy127;
        break;
      case 35: /* decl_type_function ::= FUNCTION type_specifier COLON COLON identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 282 "stparserimpl.lemon"
{
    yylhsminor.yy127 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy99, yymsp[-8].minor.yy138, yymsp[-3].minor.yy97, yymsp[-4].minor.yy42, yymsp[-1].minor.yy129);
    yylhsminor.yy127->setSourceInfo(CreateSourceInfo(yymsp[-9].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1578 "stparserimpl.c"
  yymsp[-9].minor.yy127 = yylhsminor.yy127;
        break;
      case 36: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
#line 289 "stparserimpl.lemon"
{
        yymsp[-2].minor.yy42->push_back(yymsp[0].minor.yy93);
        yylhsminor.yy42 = yymsp[-2].minor.yy42;
}
#line 1587 "stparserimpl.c"
  yymsp[-2].minor.yy42 = yylhsminor.yy42;
        break;
      case 37: /* function_args_decl ::= function_arg_decl */
#line 294 "stparserimpl.lemon"
{
    yylhsminor.yy42 = pParser->createVector<const yal::frontend::STDeclParam*>();
    yylhsminor.yy42->push_back(yymsp[0].minor.yy93);
}
#line 1596 "stparserimpl.c"
  yymsp[0].minor.yy42 = yylhsminor.yy42;
        break;
      case 38: /* function_args_decl ::= */
#line 299 "stparserimpl.lemon"
{
        yymsp[1].minor.yy42 = nullptr;
}
#line 1604 "stparserimpl.c"
        break;
      case 39: /* function_arg_decl ::= identifier COLON type_qualified */
#line 303 "stparserimpl.lemon"
{
        yylhsminor.yy93 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[-2].minor.yy99, yymsp[0].minor.yy97);
        yylhsminor.yy93->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy99->getSourceInfo(),yymsp[0].minor.yy97->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1612 "stparserimpl.c"
  yymsp[-2].minor.yy93 = yylhsminor.yy93;
        break;
      case 40: /* function_arg_decl ::= qualifier self */
#line 308 "stparserimpl.lemon"
{
        auto qualType = pParser->createNode<yal::frontend::STQualType>(nullptr, yymsp[-1].minor.yy53);
        yylhsminor.yy93 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[0].minor.yy99, qualType);
        yylhsminor.yy93->setSourceInfo(yymsp[0].minor.yy99->getSourceInfo());
}
#line 1622 "stparserimpl.c"
  yymsp[-1].minor.yy93 = yylhsminor.yy93;
        break;
      case 41: /* function_return_decl ::= COLON type_qualified */
#line 314 "stparserimpl.lemon"
{ yymsp[-1].minor.yy97 = yymsp[0].minor.yy97;}
#line 1628 "stparserimpl.c"
        break;
      case 42: /* function_return_decl ::= */
      case 55: /* var_type_spec ::= */ yytestcase(yyruleno==55);
#line 315 "stparserimpl.lemon"
{yymsp[1].minor.yy97 = nullptr;}
#line 1634 "stparserimpl.c"
        break;
      case 43: /* statement_list_or_empty ::= */
#line 319 "stparserimpl.lemon"
{yymsp[1].minor.yy129 = nullptr;}
#line 1639 "stparserimpl.c"
        break;
      case 44: /* statement_list_or_empty ::= statement_list */
#line 320 "stparserimpl.lemon"
{yylhsminor.yy129 = yymsp[0].minor.yy129;}
#line 1644 "stparserimpl.c"
  yymsp[0].minor.yy129 = yylhsminor.yy129;
        break;
      case 45: /* statement_list ::= statement_list statement */
#line 323 "stparserimpl.lemon"
{
    yymsp[-1].minor.yy129->push_back(yymsp[0].minor.yy100);
    yylhsminor.yy129=yymsp[-1].minor.yy129;
}
#line 1653 "stparserimpl.c"
  yymsp[-1].minor.yy129 = yylhsminor.yy129;
        break;
      case 46: /* statement_list ::= statement */
#line 327 "stparserimpl.lemon"
{
    yylhsminor.yy129 = pParser->createVector<const yal::frontend::STStatement*>();
    yylhsminor.yy129->push_back(yymsp[0].minor.yy100);
}
#line 1662 "stparserimpl.c"
  yymsp[0].minor.yy129 = yylhsminor.yy129;
        break;
      case 47: /* statement ::= expression ASSIGN expression SEMI_COLON */
#line 333 "stparserimpl.lemon"
{
   yylhsminor.yy100 = pParser->createNode<yal::frontend::STStmtAssign>(yymsp[-3].minor.yy176,yymsp[-1].minor.yy176);
   yylhsminor.yy100->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy176->getSourceInfo(), yymsp[-1].minor.yy176->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1671 "stparserimpl.c"
  yymsp[-3].minor.yy100 = yylhsminor.yy100;
        break;
      case 48: /* statement ::= decl_var SEMI_COLON */
#line 337 "stparserimpl.lemon"
{
    yylhsminor.yy100 = pParser->createNode<yal::frontend::STStmtDecl>(yymsp[-1].minor.yy98);
    yylhsminor.yy100->setSourceInfo(yymsp[-1].minor.yy98->getSourceInfo());
}
#line 1680 "stparserimpl.c"
  yymsp[-1].minor.yy100 = yylhsminor.yy100;
        break;
      case 49: /* statement ::= expression SEMI_COLON */
#line 342 "stparserimpl.lemon"
{
    yylhsminor.yy100 = pParser->createNode<yal::frontend::STStmtExpression>(yymsp[-1].minor.yy176);
    yylhsminor.yy100->setSourceInfo(yymsp[-1].minor.yy176->getSourceInfo());
}
#line 1689 "stparserimpl.c"
  yymsp[-1].minor.yy100 = yylhsminor.yy100;
        break;
      case 50: /* statement ::= RETURN expression SEMI_COLON */
#line 347 "stparserimpl.lemon"
{
    yylhsminor.yy100 = pParser->createNode<yal::frontend::STStmtReturn>(yymsp[-1].minor.yy176);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy100->setSourceInfo(srcInfo);
}
#line 1699 "stparserimpl.c"
  yymsp[-2].minor.yy100 = yylhsminor.yy100;
        break;
      case 51: /* statement ::= RETURN SEMI_COLON */
#line 353 "stparserimpl.lemon"
{
    yylhsminor.yy100 = pParser->createNode<yal::frontend::STStmtReturn>(nullptr);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[-1].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy100->setSourceInfo(srcInfo);
}
#line 1709 "stparserimpl.c"
  yymsp[-1].minor.yy100 = yylhsminor.yy100;
        break;
      case 52: /* decl_var ::= VAR identifier var_type_spec ASSIGN expression */
      case 53: /* decl_var ::= LET identifier var_type_spec ASSIGN expression */ yytestcase(yyruleno==53);
#line 361 "stparserimpl.lemon"
{
    yylhsminor.yy98 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy99, yymsp[-2].minor.yy97, yymsp[0].minor.yy176);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy98->setSourceInfo(srcInfo);
}
#line 1720 "stparserimpl.c"
  yymsp[-4].minor.yy98 = yylhsminor.yy98;
        break;
      case 54: /* var_type_spec ::= COLON type_qualified */
#line 373 "stparserimpl.lemon"
{yymsp[-1].minor.yy97 = yymsp[0].minor.yy97;}
#line 1726 "stparserimpl.c"
        break;
      case 56: /* expression ::= PAR_BEGIN expression PAR_END */
#line 379 "stparserimpl.lemon"
{yymsp[-2].minor.yy176 = yymsp[-1].minor.yy176;}
#line 1731 "stparserimpl.c"
        break;
      case 57: /* expression ::= literal */
#line 380 "stparserimpl.lemon"
{yylhsminor.yy176 = yymsp[0].minor.yy176;}
#line 1736 "stparserimpl.c"
  yymsp[0].minor.yy176 = yylhsminor.yy176;
        break;
      case 58: /* expression ::= unaryexp */
#line 381 "stparserimpl.lemon"
{yylhsminor.yy176 = yymsp[0].minor.yy150;}
#line 1742 "stparserimpl.c"
  yymsp[0].minor.yy176 = yylhsminor.yy176;
        break;
      case 59: /* expression ::= binaryexp */
#line 382 "stparserimpl.lemon"
{yylhsminor.yy176 = yymsp[0].minor.yy156;}
#line 1748 "stparserimpl.c"
  yymsp[0].minor.yy176 = yylhsminor.yy176;
        break;
      case 60: /* expression ::= struct_init */
#line 383 "stparserimpl.lemon"
{ yylhsminor.yy176 = yymsp[0].minor.yy36;}
#line 1754 "stparserimpl.c"
  yymsp[0].minor.yy176 = yylhsminor.yy176;
        break;
      case 61: /* expression ::= RANGE_CAST LT type_qualified GT PAR_BEGIN expression PAR_END */
#line 385 "stparserimpl.lemon"
{
    yylhsminor.yy176 = pParser->createNode<yal::frontend::STExprRangeCast>(yymsp[-4].minor.yy97, yymsp[-1].minor.yy176);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy176->setSourceInfo(srcInfo);
}
#line 1764 "stparserimpl.c"
  yymsp[-6].minor.yy176 = yylhsminor.yy176;
        break;
      case 62: /* expression ::= identifier_or_self */
#line 391 "stparserimpl.lemon"
{
    yylhsminor.yy176 = pParser->createNode<yal::frontend::STExprVarRef>(yymsp[0].minor.yy99);
    yylhsminor.yy176->setSourceInfo(yymsp[0].minor.yy99->getSourceInfo());
}
#line 1773 "stparserimpl.c"
  yymsp[0].minor.yy176 = yylhsminor.yy176;
        break;
      case 63: /* expression ::= expression DOT identifier */
#line 396 "stparserimpl.lemon"
{
    yylhsminor.yy176 = pParser->createNode<yal::frontend::STExprStructVarRef>(yymsp[-2].minor.yy176, yymsp[0].minor.yy99);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy99->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy176->setSourceInfo(srcInfo);
}
#line 1783 "stparserimpl.c"
  yymsp[-2].minor.yy176 = yylhsminor.yy176;
        break;
      case 64: /* expression ::= identifier PAR_BEGIN function_call_args PAR_END */
#line 402 "stparserimpl.lemon"
{
    yylhsminor.yy176 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy99,yymsp[-1].minor.yy101);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy99->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy176->setSourceInfo(srcInfo);
}
#line 1793 "stparserimpl.c"
  yymsp[-3].minor.yy176 = yylhsminor.yy176;
        break;
      case 65: /* expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END */
#line 408 "stparserimpl.lemon"
{
    yylhsminor.yy176 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy99,yymsp[-5].minor.yy176,yymsp[-1].minor.yy101);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-5].minor.yy176->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy176->setSourceInfo(srcInfo);
}
#line 1803 "stparserimpl.c"
  yymsp[-5].minor.yy176 = yylhsminor.yy176;
        break;
      case 66: /* expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END */
#line 414 "stparserimpl.lemon"
{
    yylhsminor.yy176 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy99,yymsp[-6].minor.yy138,yymsp[-1].minor.yy101);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy138->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy176->setSourceInfo(srcInfo);
}
#line 1813 "stparserimpl.c"
  yymsp[-6].minor.yy176 = yylhsminor.yy176;
        break;
      case 67: /* binaryexp ::= expression AND expression */
#line 422 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1824 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 68: /* binaryexp ::= expression OR expression */
#line 428 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1835 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 69: /* binaryexp ::= expression PLUS expression */
#line 434 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1846 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 70: /* binaryexp ::= expression MINUS expression */
#line 440 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1857 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 71: /* binaryexp ::= expression MULT expression */
#line 446 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1868 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 72: /* binaryexp ::= expression DIV expression */
#line 452 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
}
#line 1877 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 73: /* binaryexp ::= expression MOD expression */
#line 456 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1888 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 74: /* binaryexp ::= expression EQ expression */
#line 463 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1899 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 75: /* binaryexp ::= expression NE expression */
#line 470 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1910 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 76: /* binaryexp ::= expression LE expression */
#line 477 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1921 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 77: /* binaryexp ::= expression LT expression */
#line 484 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1932 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 78: /* binaryexp ::= expression GE expression */
#line 491 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1943 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 79: /* binaryexp ::= expression GT expression */
#line 498 "stparserimpl.lemon"
{
        yylhsminor.yy156 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy176, yymsp[0].minor.yy176);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy176->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy156->setSourceInfo(srcInfo);
}
#line 1954 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 80: /* unaryexp ::= NOT expression */
#line 507 "stparserimpl.lemon"
{
    yylhsminor.yy150 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy176);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy150->setSourceInfo(srcInfo);
}
#line 1965 "stparserimpl.c"
  yymsp[-1].minor.yy150 = yylhsminor.yy150;
        break;
      case 81: /* unaryexp ::= BIT_NOT expression */
#line 513 "stparserimpl.lemon"
{
    yylhsminor.yy150 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy176);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy150->setSourceInfo(srcInfo);
}
#line 1976 "stparserimpl.c"
  yymsp[-1].minor.yy150 = yylhsminor.yy150;
        break;
      case 82: /* unaryexp ::= MINUS expression */
#line 519 "stparserimpl.lemon"
{
    yylhsminor.yy150 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy176);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy150->setSourceInfo(srcInfo);
}
#line 1987 "stparserimpl.c"
  yymsp[-1].minor.yy150 = yylhsminor.yy150;
        break;
      case 83: /* unaryexp ::= REFERENCE expression */
#line 526 "stparserimpl.lemon"
{
    yylhsminor.yy150 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy176);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy150->setSourceInfo(srcInfo);
}
#line 1998 "stparserimpl.c"
  yymsp[-1].minor.yy150 = yylhsminor.yy150;
        break;
      case 84: /* function_call_args ::= function_call_args COMMA expression */
#line 535 "stparserimpl.lemon"
{
    yylhsminor.yy101 = yymsp[-2].minor.yy101;
    yylhsminor.yy101->push_back(yymsp[0].minor.yy176);
}
#line 2007 "stparserimpl.c"
  yymsp[-2].minor.yy101 = yylhsminor.yy101;
        break;
      case 85: /* function_call_args ::= expression */
#line 539 "stparserimpl.lemon"
{
    yylhsminor.yy101 = pParser->createVector<const yal::frontend::STExpression*>();
    yylhsminor.yy101->push_back(yymsp[0].minor.yy176);
}
#line 2016 "stparserimpl.c"
  yymsp[0].minor.yy101 = yylhsminor.yy101;
        break;
      case 86: /* function_call_args ::= */
#line 543 "stparserimpl.lemon"
{
    yymsp[1].minor.yy101= nullptr;
}
#line 2024 "stparserimpl.c"
        break;
      case 87: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
#line 550 "stparserimpl.lemon"
{
    yylhsminor.yy36 = pParser->createNode<yal::frontend::STExprStructInit>(yymsp[-3].minor.yy138, yymsp[-1].minor.yy32);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy138->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy36->setSourceInfo(srcInfo);
}
#line 2033 "stparserimpl.c"
  yymsp[-3].minor.yy36 = yylhsminor.yy36;
        break;
      case 88: /* struct_member_init_list ::= */
#line 556 "stparserimpl.lemon"
{yymsp[1].minor.yy32 = nullptr;}
#line 2039 "stparserimpl.c"
        break;
      case 89: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
#line 557 "stparserimpl.lemon"
{
     yymsp[-2].minor.yy32->push_back(yymsp[0].minor.yy169);
     yylhsminor.yy32 = yymsp[-2].minor.yy32;
}
#line 2047 "stparserimpl.c"
  yymsp[-2].minor.yy32 = yylhsminor.yy32;
        break;
      case 90: /* struct_member_init_list ::= struct_member_init */
#line 561 "stparserimpl.lemon"
{
        yylhsminor.yy32 = pParser->createVector<const yal::frontend::STStructMemberInit*>();
        yylhsminor.yy32->push_back(yymsp[0].minor.yy169);
}
#line 2056 "stparserimpl.c"
  yymsp[0].minor.yy32 = yylhsminor.yy32;
        break;
      case 91: /* struct_member_init ::= identifier COLON expression */
#line 566 "stparserimpl.lemon"
{
    yylhsminor.yy169 = pParser->createNode<yal::frontend::STStructMemberInit>(yymsp[-2].minor.yy99,yymsp[0].minor.yy176);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy99->getSourceInfo(), yymsp[0].minor.yy176->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy169->setSourceInfo(srcInfo);
}
#line 2066 "stparserimpl.c"
  yymsp[-2].minor.yy169 = yylhsminor.yy169;
        break;
      case 92: /* literal ::= INTEGER_LITERAL */
#line 574 "stparserimpl.lemon"
{
        yylhsminor.yy176 = pParser->createNode<yal::frontend::STExprIntegerLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo =yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy176->setSourceInfo(srcInfo);
   }
#line 2076 "stparserimpl.c"
  yymsp[0].minor.yy176 = yylhsminor.yy176;
        break;
      case 93: /* literal ::= FLOAT_LITERAL */
#line 579 "stparserimpl.lemon"
{
        yylhsminor.yy176 = pParser->createNode<yal::frontend::STExprFloatLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy176->setSourceInfo(srcInfo);
}
#line 2086 "stparserimpl.c"
  yymsp[0].minor.yy176 = yylhsminor.yy176;
        break;
      case 94: /* literal ::= BOOL_LITERAL */
#line 584 "stparserimpl.lemon"
{
        yylhsminor.yy176 = pParser->createNode<yal::frontend::STExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy176->setSourceInfo(srcInfo);
}
#line 2096 "stparserimpl.c"
  yymsp[0].minor.yy176 = yylhsminor.yy176;
        break;
      default:
      /* (95) module ::= decls END */ yytestcase(yyruleno==95);
      /* (96) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==96);
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
#line 2180 "stparserimpl.c"
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
