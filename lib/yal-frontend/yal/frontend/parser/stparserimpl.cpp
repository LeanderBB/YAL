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
#define YAL_TOKEN_DOT                            22
#define YAL_TOKEN_END                            23
#define YAL_TOKEN_IDENTIFIER                     24
#define YAL_TOKEN_SELF                           25
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
#define YAL_TOKEN_MUT                            39
#define YAL_TOKEN_REFERENCE                      40
#define YAL_TOKEN_TYPE                           41
#define YAL_TOKEN_COLON                          42
#define YAL_TOKEN_SEMI_COLON                     43
#define YAL_TOKEN_STRUCT                         44
#define YAL_TOKEN_SCOPE_BEGIN                    45
#define YAL_TOKEN_SCOPE_END                      46
#define YAL_TOKEN_COMMA                          47
#define YAL_TOKEN_PAR_BEGIN                      48
#define YAL_TOKEN_PAR_END                        49
#define YAL_TOKEN_FUNCTION                       50
#define YAL_TOKEN_RETURN                         51
#define YAL_TOKEN_VAR                            52
#define YAL_TOKEN_INTEGER_LITERAL                53
#define YAL_TOKEN_INTEGER_LITERAL_I8             54
#define YAL_TOKEN_INTEGER_LITERAL_I16            55
#define YAL_TOKEN_INTEGER_LITERAL_I32            56
#define YAL_TOKEN_INTEGER_LITERAL_I64            57
#define YAL_TOKEN_INTEGER_LITERAL_U8             58
#define YAL_TOKEN_INTEGER_LITERAL_U16            59
#define YAL_TOKEN_INTEGER_LITERAL_U32            60
#define YAL_TOKEN_INTEGER_LITERAL_U64            61
#define YAL_TOKEN_FLOAT_LITERAL                  62
#define YAL_TOKEN_FLOAT_LITERAL_32               63
#define YAL_TOKEN_FLOAT_LITERAL_64               64
#define YAL_TOKEN_BOOL_LITERAL                   65
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
#define YYNOCODE 101
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#endif
typedef union {
  int yyinit;
  YALSTParserTOKENTYPE yy0;
  yal::frontend::STDeclFunction* yy14;
  yal::frontend::STQualType* yy25;
  yal::frontend::STDeclModule* yy46;
  yal::frontend::STIdentifier* yy51;
  yal::frontend::STStructMember* yy53;
  yal::frontend::ParseListStmt::Range yy55;
  yal::frontend::ParseListExpr::Range yy62;
  yal::frontend::STStatement* yy89;
  yal::frontend::STDeclParam* yy101;
  yal::frontend::STDeclStruct* yy109;
  yal::frontend::STDeclVar* yy137;
  yal::frontend::STExprBinaryOperator* yy140;
  uint32_t yy141;
  yal::frontend::ParseListDeclParam::Range yy156;
  yal::frontend::STType* yy170;
  yal::frontend::ParseListStructMember::Range yy172;
  yal::frontend::STExprStructInit* yy180;
  yal::frontend::STDecl* yy188;
  yal::frontend::ParseListStructInit::Range yy189;
  yal::frontend::STExpression* yy192;
  yal::frontend::STStructMemberInit* yy193;
  yal::frontend::STExprUnaryOperator* yy198;
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
#define YYNSTATE             113
#define YYNRULE              107
#define YY_MAX_SHIFT         112
#define YY_MIN_SHIFTREDUCE   185
#define YY_MAX_SHIFTREDUCE   291
#define YY_MIN_REDUCE        292
#define YY_MAX_REDUCE        398
#define YY_ERROR_ACTION      399
#define YY_ACCEPT_ACTION     400
#define YY_NO_ACTION         401
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
#define YY_ACTTAB_COUNT (991)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    13,  110,  110,  108,  318,   14,   15,  189,  314,   81,
 /*    10 */   190,  196,  197,  198,  199,  200,  201,  202,  203,  204,
 /*    20 */   205,  206,  105,  209,  110,   12,   86,  333,   55,   35,
 /*    30 */     2,  301,  301,   30,  189,   91,    8,   50,  277,  278,
 /*    40 */   279,  280,  281,  282,  283,  284,  285,  286,  287,  288,
 /*    50 */   289,  346,   13,   35,  301,   33,  400,   14,   15,   77,
 /*    60 */   382,   81,  190,  196,  197,  198,  199,  200,  201,  202,
 /*    70 */   203,  204,  205,  206,  292,  293,  294,   12,   54,  104,
 /*    80 */   236,  320,  105,  209,   39,   30,  112,   80,  329,   84,
 /*    90 */   277,  278,  279,  280,  281,  282,  283,  284,  285,  286,
 /*   100 */   287,  288,  289,  353,   56,   35,   13,   32,   25,   24,
 /*   110 */    23,   14,   15,   31,   52,   81,  190,  196,  197,  198,
 /*   120 */   199,  200,  201,  202,  203,  204,  205,  206,  106,   46,
 /*   130 */   331,   12,   35,   16,  324,  249,   35,   40,   91,   30,
 /*   140 */   323,   82,  272,   44,  277,  278,  279,  280,  281,  282,
 /*   150 */   283,  284,  285,  286,  287,  288,  289,   10,   41,   28,
 /*   160 */    29,  214,   45,  381,   22,   21,   17,   19,   18,   20,
 /*   170 */    27,   26,   25,   24,   23,   41,   28,   29,   52,   47,
 /*   180 */   106,   22,   21,   17,   19,   18,   20,   27,   26,   25,
 /*   190 */    24,   23,  322,   49,   97,   52,  189,   37,  195,  234,
 /*   200 */   196,  197,  198,  199,  200,  201,  202,  203,  204,  205,
 /*   210 */   206,  105,  209,   16,   87,  250,  235,   96,  107,   41,
 /*   220 */    28,   29,   16,  101,  251,   22,   21,   17,   19,   18,
 /*   230 */    20,   27,   26,   25,   24,   23,   41,   28,   29,   52,
 /*   240 */   103,  190,   22,   21,   17,   19,   18,   20,   27,   26,
 /*   250 */    25,   24,   23,   36,  332,  218,   52,  110,  111,   79,
 /*   260 */   232,  102,  299,  354,   89,  302,   53,  220,  302,    1,
 /*   270 */    48,   85,    9,  237,  233,   52,    4,   83,  338,   57,
 /*   280 */   350,  351,  349,  241,  352,  219,   11,  301,    5,  110,
 /*   290 */    51,   79,    3,  102,  299,  354,   89,   42,    6,    7,
 /*   300 */   210,   43,   38,  291,  212,   54,  104,  109,    4,   88,
 /*   310 */   338,   57,  350,  351,  349,  328,  352,   34,  110,  301,
 /*   320 */    79,  397,  102,  299,  354,   89,  294,  294,  294,  294,
 /*   330 */   294,  294,  294,  294,  294,  294,  294,    4,   90,  338,
 /*   340 */    57,  350,  351,  349,  294,  352,  294,  294,  301,  294,
 /*   350 */   294,   41,   28,   29,  294,  294,  294,   22,   21,   17,
 /*   360 */    19,   18,   20,   27,   26,   25,   24,   23,  294,  294,
 /*   370 */   294,   52,  294,  294,  294,  294,  294,  294,  110,  294,
 /*   380 */    79,  294,  102,  299,  354,   89,   81,  294,  196,  197,
 /*   390 */   198,  199,  200,  201,  202,  203,  204,  205,  206,  337,
 /*   400 */    57,  350,  351,  349,  294,  352,   29,  294,  301,  294,
 /*   410 */    22,   21,   17,   19,   18,   20,   27,   26,   25,   24,
 /*   420 */    23,  110,  294,   79,   52,  102,  299,  354,  294,  294,
 /*   430 */   110,  294,   79,  294,  102,  299,  354,  294,  294,  294,
 /*   440 */   294,  294,  294,   64,  350,  351,  349,   78,  352,  294,
 /*   450 */   294,  301,   64,  350,  351,  349,   76,  352,  294,  294,
 /*   460 */   301,  294,  294,  294,  294,  294,  294,  294,  294,  110,
 /*   470 */   294,   79,  294,  102,  299,  354,  195,  294,  196,  197,
 /*   480 */   198,  199,  200,  201,  202,  203,  204,  205,  206,  294,
 /*   490 */   294,   64,  350,  351,  349,   75,  352,  294,  294,  301,
 /*   500 */   399,  399,  399,  399,  399,  399,   27,   26,   25,   24,
 /*   510 */    23,  294,  294,  294,   52,   22,   21,   17,   19,   18,
 /*   520 */    20,   27,   26,   25,   24,   23,  110,  294,   79,   52,
 /*   530 */   102,  299,  354,  294,  294,  294,  294,  294,  294,  110,
 /*   540 */   294,   79,  294,  102,  299,  354,  294,  294,   58,  350,
 /*   550 */   351,  349,  294,  352,  294,  294,  301,  294,  294,  294,
 /*   560 */   294,   61,  350,  351,  349,  294,  352,  294,  294,  301,
 /*   570 */   110,  294,   79,  294,  102,  299,  354,  294,  294,  110,
 /*   580 */   294,   79,  294,  102,  299,  354,  294,  294,  294,  294,
 /*   590 */   294,  294,   59,  350,  351,  349,  294,  352,  294,  294,
 /*   600 */   301,   62,  350,  351,  349,  294,  352,  294,  294,  301,
 /*   610 */   294,  110,  294,   79,  294,  102,  299,  354,  294,  294,
 /*   620 */   110,  294,   79,  294,  102,  299,  354,  294,  294,  294,
 /*   630 */   294,  294,  294,   92,  350,  351,  349,  294,  352,  294,
 /*   640 */   294,  301,   93,  350,  351,  349,  294,  352,  294,  110,
 /*   650 */   301,   79,  294,  102,  299,  354,  294,  294,  294,  294,
 /*   660 */   294,  294,  110,  294,   79,  294,  102,  299,  354,  294,
 /*   670 */   294,   94,  350,  351,  349,  294,  352,  294,  294,  301,
 /*   680 */   294,  294,  294,  294,   95,  350,  351,  349,  294,  352,
 /*   690 */   294,  294,  301,  294,  110,  294,   79,  294,  102,  299,
 /*   700 */   354,  294,  294,  110,  294,   79,  294,  102,  299,  354,
 /*   710 */   294,  294,  294,  294,  294,  294,   63,  350,  351,  349,
 /*   720 */   294,  352,  294,  294,  301,   66,  350,  351,  349,  294,
 /*   730 */   352,  294,  110,  301,   79,  294,  102,  299,  354,  294,
 /*   740 */   294,  110,  294,   79,  294,  102,  299,  354,  294,  294,
 /*   750 */   294,  294,  294,  294,   67,  350,  351,  349,  294,  352,
 /*   760 */   294,  294,  301,   68,  350,  351,  349,  294,  352,  294,
 /*   770 */   110,  301,   79,  294,  102,  299,  354,  294,  294,  110,
 /*   780 */   294,   79,  294,  102,  299,  354,  294,  294,  294,  294,
 /*   790 */   294,  294,   69,  350,  351,  349,  294,  352,  294,  294,
 /*   800 */   301,   70,  350,  351,  349,  294,  352,  294,  110,  301,
 /*   810 */    79,  294,  102,  299,  354,  294,  294,  110,  294,   79,
 /*   820 */   294,  102,  299,  354,  294,  294,  294,  294,  294,  294,
 /*   830 */    71,  350,  351,  349,  294,  352,  294,  294,  301,   98,
 /*   840 */   350,  351,  349,  294,  352,  294,  110,  301,   79,  294,
 /*   850 */   102,  299,  354,  294,  294,  110,  294,   79,  294,  102,
 /*   860 */   299,  354,  294,  294,  294,  294,  294,  294,   99,  350,
 /*   870 */   351,  349,  294,  352,  294,  294,  301,  100,  350,  351,
 /*   880 */   349,  294,  352,  294,  110,  301,   79,  294,  102,  299,
 /*   890 */   354,  294,  294,  110,  294,   79,  294,  102,  299,  354,
 /*   900 */   294,  294,  294,  294,  294,  294,   73,  350,  351,  349,
 /*   910 */   294,  352,  294,  294,  301,   74,  350,  351,  349,  294,
 /*   920 */   352,  294,  110,  301,   79,  294,  102,  299,  354,  294,
 /*   930 */   294,  110,  294,   79,  294,  102,  299,  354,  294,  294,
 /*   940 */   294,  294,  294,  294,   65,  350,  351,  349,  294,  352,
 /*   950 */   294,  294,  301,   72,  350,  351,  349,  294,  352,  294,
 /*   960 */   110,  301,   79,  294,  102,  299,  354,  294,  294,  294,
 /*   970 */   294,  294,  294,  294,  294,  294,  294,  294,  294,  294,
 /*   980 */   294,  294,   60,  350,  351,  349,  294,  352,  294,  294,
 /*   990 */   301,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    15,   69,   69,   71,   71,   20,   21,   24,   25,   24,
 /*    10 */    25,   26,   27,   28,   29,   30,   31,   32,   33,   34,
 /*    20 */    35,   36,   39,   40,   69,   40,   71,   70,   73,   72,
 /*    30 */    45,   99,   99,   48,   24,   73,   51,   52,   53,   54,
 /*    40 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*    50 */    65,   70,   15,   72,   99,   67,   68,   20,   21,   97,
 /*    60 */    98,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*    70 */    33,   34,   35,   36,   77,   78,   79,   40,   72,   73,
 /*    80 */    43,   84,   39,   40,   42,   48,   23,   81,   82,   80,
 /*    90 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   100 */    63,   64,   65,   70,   41,   72,   15,   48,   16,   17,
 /*   110 */    18,   20,   21,   50,   22,   24,   25,   26,   27,   28,
 /*   120 */    29,   30,   31,   32,   33,   34,   35,   36,   73,   83,
 /*   130 */    70,   40,   72,   47,   70,   49,   72,   42,   73,   48,
 /*   140 */    85,   86,   46,   47,   53,   54,   55,   56,   57,   58,
 /*   150 */    59,   60,   61,   62,   63,   64,   65,    1,    2,    3,
 /*   160 */     4,   46,   47,   98,    8,    9,   10,   11,   12,   13,
 /*   170 */    14,   15,   16,   17,   18,    2,    3,    4,   22,   73,
 /*   180 */    73,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   190 */    17,   18,   85,   73,   42,   22,   24,   45,   24,   43,
 /*   200 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   210 */    36,   39,   40,   47,   87,   49,   43,   73,   44,    2,
 /*   220 */     3,    4,   47,   73,   49,    8,    9,   10,   11,   12,
 /*   230 */    13,   14,   15,   16,   17,   18,    2,    3,    4,   22,
 /*   240 */    80,   25,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   250 */    16,   17,   18,   47,   74,   49,   22,   69,   73,   71,
 /*   260 */    43,   73,   74,   75,   76,   42,   83,   46,   45,   45,
 /*   270 */    42,   42,    1,   46,   43,   22,   88,   89,   90,   91,
 /*   280 */    92,   93,   94,   49,   96,   46,   42,   99,   48,   69,
 /*   290 */    42,   71,   45,   73,   74,   75,   76,   42,   48,   48,
 /*   300 */    40,   42,   45,   38,   43,   72,   73,   37,   88,   89,
 /*   310 */    90,   91,   92,   93,   94,   82,   96,   42,   69,   99,
 /*   320 */    71,    0,   73,   74,   75,   76,  100,  100,  100,  100,
 /*   330 */   100,  100,  100,  100,  100,  100,  100,   88,   89,   90,
 /*   340 */    91,   92,   93,   94,  100,   96,  100,  100,   99,  100,
 /*   350 */   100,    2,    3,    4,  100,  100,  100,    8,    9,   10,
 /*   360 */    11,   12,   13,   14,   15,   16,   17,   18,  100,  100,
 /*   370 */   100,   22,  100,  100,  100,  100,  100,  100,   69,  100,
 /*   380 */    71,  100,   73,   74,   75,   76,   24,  100,   26,   27,
 /*   390 */    28,   29,   30,   31,   32,   33,   34,   35,   36,   90,
 /*   400 */    91,   92,   93,   94,  100,   96,    4,  100,   99,  100,
 /*   410 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   420 */    18,   69,  100,   71,   22,   73,   74,   75,  100,  100,
 /*   430 */    69,  100,   71,  100,   73,   74,   75,  100,  100,  100,
 /*   440 */   100,  100,  100,   91,   92,   93,   94,   95,   96,  100,
 /*   450 */   100,   99,   91,   92,   93,   94,   95,   96,  100,  100,
 /*   460 */    99,  100,  100,  100,  100,  100,  100,  100,  100,   69,
 /*   470 */   100,   71,  100,   73,   74,   75,   24,  100,   26,   27,
 /*   480 */    28,   29,   30,   31,   32,   33,   34,   35,   36,  100,
 /*   490 */   100,   91,   92,   93,   94,   95,   96,  100,  100,   99,
 /*   500 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   510 */    18,  100,  100,  100,   22,    8,    9,   10,   11,   12,
 /*   520 */    13,   14,   15,   16,   17,   18,   69,  100,   71,   22,
 /*   530 */    73,   74,   75,  100,  100,  100,  100,  100,  100,   69,
 /*   540 */   100,   71,  100,   73,   74,   75,  100,  100,   91,   92,
 /*   550 */    93,   94,  100,   96,  100,  100,   99,  100,  100,  100,
 /*   560 */   100,   91,   92,   93,   94,  100,   96,  100,  100,   99,
 /*   570 */    69,  100,   71,  100,   73,   74,   75,  100,  100,   69,
 /*   580 */   100,   71,  100,   73,   74,   75,  100,  100,  100,  100,
 /*   590 */   100,  100,   91,   92,   93,   94,  100,   96,  100,  100,
 /*   600 */    99,   91,   92,   93,   94,  100,   96,  100,  100,   99,
 /*   610 */   100,   69,  100,   71,  100,   73,   74,   75,  100,  100,
 /*   620 */    69,  100,   71,  100,   73,   74,   75,  100,  100,  100,
 /*   630 */   100,  100,  100,   91,   92,   93,   94,  100,   96,  100,
 /*   640 */   100,   99,   91,   92,   93,   94,  100,   96,  100,   69,
 /*   650 */    99,   71,  100,   73,   74,   75,  100,  100,  100,  100,
 /*   660 */   100,  100,   69,  100,   71,  100,   73,   74,   75,  100,
 /*   670 */   100,   91,   92,   93,   94,  100,   96,  100,  100,   99,
 /*   680 */   100,  100,  100,  100,   91,   92,   93,   94,  100,   96,
 /*   690 */   100,  100,   99,  100,   69,  100,   71,  100,   73,   74,
 /*   700 */    75,  100,  100,   69,  100,   71,  100,   73,   74,   75,
 /*   710 */   100,  100,  100,  100,  100,  100,   91,   92,   93,   94,
 /*   720 */   100,   96,  100,  100,   99,   91,   92,   93,   94,  100,
 /*   730 */    96,  100,   69,   99,   71,  100,   73,   74,   75,  100,
 /*   740 */   100,   69,  100,   71,  100,   73,   74,   75,  100,  100,
 /*   750 */   100,  100,  100,  100,   91,   92,   93,   94,  100,   96,
 /*   760 */   100,  100,   99,   91,   92,   93,   94,  100,   96,  100,
 /*   770 */    69,   99,   71,  100,   73,   74,   75,  100,  100,   69,
 /*   780 */   100,   71,  100,   73,   74,   75,  100,  100,  100,  100,
 /*   790 */   100,  100,   91,   92,   93,   94,  100,   96,  100,  100,
 /*   800 */    99,   91,   92,   93,   94,  100,   96,  100,   69,   99,
 /*   810 */    71,  100,   73,   74,   75,  100,  100,   69,  100,   71,
 /*   820 */   100,   73,   74,   75,  100,  100,  100,  100,  100,  100,
 /*   830 */    91,   92,   93,   94,  100,   96,  100,  100,   99,   91,
 /*   840 */    92,   93,   94,  100,   96,  100,   69,   99,   71,  100,
 /*   850 */    73,   74,   75,  100,  100,   69,  100,   71,  100,   73,
 /*   860 */    74,   75,  100,  100,  100,  100,  100,  100,   91,   92,
 /*   870 */    93,   94,  100,   96,  100,  100,   99,   91,   92,   93,
 /*   880 */    94,  100,   96,  100,   69,   99,   71,  100,   73,   74,
 /*   890 */    75,  100,  100,   69,  100,   71,  100,   73,   74,   75,
 /*   900 */   100,  100,  100,  100,  100,  100,   91,   92,   93,   94,
 /*   910 */   100,   96,  100,  100,   99,   91,   92,   93,   94,  100,
 /*   920 */    96,  100,   69,   99,   71,  100,   73,   74,   75,  100,
 /*   930 */   100,   69,  100,   71,  100,   73,   74,   75,  100,  100,
 /*   940 */   100,  100,  100,  100,   91,   92,   93,   94,  100,   96,
 /*   950 */   100,  100,   99,   91,   92,   93,   94,  100,   96,  100,
 /*   960 */    69,   99,   71,  100,   73,   74,   75,  100,  100,  100,
 /*   970 */   100,  100,  100,  100,  100,  100,  100,  100,  100,  100,
 /*   980 */   100,  100,   91,   92,   93,   94,  100,   96,  100,  100,
 /*   990 */    99,
};
#define YY_SHIFT_USE_DFLT (991)
#define YY_SHIFT_COUNT    (112)
#define YY_SHIFT_MIN      (-17)
#define YY_SHIFT_MAX      (507)
static const short yy_shift_ofst[] = {
 /*     0 */   991,  -15,  -15,  -15,  -15,   91,   91,   91,   37,   91,
 /*    10 */    91,   91,   91,   91,   91,   91,   91,   91,   91,   91,
 /*    20 */    91,   91,   91,   91,   91,   91,   91,   91,   91,   91,
 /*    30 */    91,  362,  -17,   63,  174,  452,  172,   10,   10,   43,
 /*    40 */    43,   43,   43,   43,   10,   10,   42,   59,   10,   95,
 /*    50 */    10,   10,   10,   42,  216,   59,   10,  156,  173,  217,
 /*    60 */   234,  349,  349,  349,  349,  402,  492,  492,  492,  492,
 /*    70 */   492,  492,  507,   92,   92,   86,  166,   96,  175,  152,
 /*    80 */   206,  223,  115,  221,  224,  228,  229,  271,  227,  231,
 /*    90 */   239,  244,  253,  253,  253,  253,  240,  248,  253,  253,
 /*   100 */   253,  250,  251,  247,  255,  260,  259,  257,  261,  265,
 /*   110 */   270,  275,  321,
};
#define YY_REDUCE_USE_DFLT (-69)
#define YY_REDUCE_COUNT (56)
#define YY_REDUCE_MIN   (-68)
#define YY_REDUCE_MAX   (891)
static const short yy_reduce_ofst[] = {
 /*     0 */   -12,  188,  220,  249,  309,  352,  361,  400,  457,  470,
 /*    10 */   501,  510,  542,  551,  580,  593,  625,  634,  663,  672,
 /*    20 */   701,  710,  739,  748,  777,  786,  815,  824,  853,  862,
 /*    30 */   891,  -45,    6,   -3,  -68,  -67,  233,  -38,   55,  -43,
 /*    40 */   -19,   33,   60,   64,   65,  107,    9,   46,  106,  127,
 /*    50 */   120,  144,  150,  160,  180,  183,  185,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   295,  335,  335,  335,  336,  378,  378,  378,  399,  399,
 /*    10 */   399,  399,  399,  399,  399,  399,  399,  399,  399,  399,
 /*    20 */   399,  399,  399,  399,  399,  399,  399,  399,  399,  399,
 /*    30 */   399,  399,  330,  399,  399,  399,  314,  380,  399,  314,
 /*    40 */   314,  314,  314,  314,  399,  399,  334,  399,  399,  347,
 /*    50 */   399,  399,  399,  334,  399,  399,  399,  399,  399,  399,
 /*    60 */   399,  345,  383,  376,  377,  360,  371,  370,  369,  368,
 /*    70 */   367,  366,  359,  362,  361,  399,  399,  399,  399,  399,
 /*    80 */   399,  296,  399,  399,  399,  399,  399,  399,  399,  399,
 /*    90 */   399,  399,  375,  374,  373,  372,  399,  399,  365,  364,
 /*   100 */   363,  355,  298,  399,  399,  315,  399,  399,  399,  399,
 /*   110 */   300,  399,  399,
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
  "BIT_NOT",       "NOT",           "DOT",           "END",         
  "IDENTIFIER",    "SELF",          "TYPE_BOOL",     "TYPE_INT8",   
  "TYPE_UINT8",    "TYPE_INT16",    "TYPE_UINT16",   "TYPE_INT32",  
  "TYPE_UINT32",   "TYPE_INT64",    "TYPE_UINT64",   "TYPE_FLOAT",  
  "TYPE_DOUBLE",   "ARRAY_BEGIN",   "ARRAY_END",     "MUT",         
  "REFERENCE",     "TYPE",          "COLON",         "SEMI_COLON",  
  "STRUCT",        "SCOPE_BEGIN",   "SCOPE_END",     "COMMA",       
  "PAR_BEGIN",     "PAR_END",       "FUNCTION",      "RETURN",      
  "VAR",           "INTEGER_LITERAL",  "INTEGER_LITERAL_I8",  "INTEGER_LITERAL_I16",
  "INTEGER_LITERAL_I32",  "INTEGER_LITERAL_I64",  "INTEGER_LITERAL_U8",  "INTEGER_LITERAL_U16",
  "INTEGER_LITERAL_U32",  "INTEGER_LITERAL_U64",  "FLOAT_LITERAL",  "FLOAT_LITERAL_32",
  "FLOAT_LITERAL_64",  "BOOL_LITERAL",  "error",         "decls",       
  "module",        "type_builtin",  "type_qualified",  "type_specifier",
  "qualifier",     "identifier",    "self",          "identifier_or_self",
  "decl_var",      "decl_type",     "decl_function",  "decl_type_function",
  "function_return_decl",  "function_args_decl",  "function_arg_decl",  "function_param_list",
  "decl_struct",   "struct_decl_var",  "struct_decl_vars",  "var_type_spec",
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
 /*  52 */ "statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  53 */ "decl_var ::= VAR identifier var_type_spec ASSIGN expression",
 /*  54 */ "var_type_spec ::= COLON type_qualified",
 /*  55 */ "var_type_spec ::=",
 /*  56 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  57 */ "expression ::= literal",
 /*  58 */ "expression ::= unaryexp",
 /*  59 */ "expression ::= binaryexp",
 /*  60 */ "expression ::= struct_init",
 /*  61 */ "expression ::= expression AS type_qualified",
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
 /*  93 */ "literal ::= INTEGER_LITERAL_I8",
 /*  94 */ "literal ::= INTEGER_LITERAL_I16",
 /*  95 */ "literal ::= INTEGER_LITERAL_I32",
 /*  96 */ "literal ::= INTEGER_LITERAL_I64",
 /*  97 */ "literal ::= INTEGER_LITERAL_U8",
 /*  98 */ "literal ::= INTEGER_LITERAL_U16",
 /*  99 */ "literal ::= INTEGER_LITERAL_U32",
 /* 100 */ "literal ::= INTEGER_LITERAL_U64",
 /* 101 */ "literal ::= FLOAT_LITERAL",
 /* 102 */ "literal ::= FLOAT_LITERAL_32",
 /* 103 */ "literal ::= FLOAT_LITERAL_64",
 /* 104 */ "literal ::= BOOL_LITERAL",
 /* 105 */ "module ::= decls END",
 /* 106 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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
  { 67, -2 },
  { 67, -2 },
  { 67, -2 },
  { 67, 0 },
  { 73, -1 },
  { 74, -1 },
  { 75, -1 },
  { 75, -1 },
  { 71, -1 },
  { 71, -1 },
  { 71, -1 },
  { 69, -1 },
  { 69, -1 },
  { 69, -1 },
  { 69, -1 },
  { 69, -1 },
  { 69, -1 },
  { 69, -1 },
  { 69, -1 },
  { 69, -1 },
  { 69, -1 },
  { 69, -1 },
  { 72, 0 },
  { 72, -1 },
  { 72, -1 },
  { 72, -2 },
  { 70, -2 },
  { 77, -5 },
  { 77, -1 },
  { 84, -7 },
  { 86, -3 },
  { 86, -1 },
  { 85, -3 },
  { 83, -3 },
  { 78, -7 },
  { 79, -10 },
  { 81, -3 },
  { 81, -1 },
  { 81, 0 },
  { 82, -3 },
  { 82, -2 },
  { 80, -2 },
  { 80, 0 },
  { 89, 0 },
  { 89, -1 },
  { 88, -2 },
  { 88, -1 },
  { 90, -4 },
  { 90, -2 },
  { 90, -2 },
  { 90, -3 },
  { 90, -2 },
  { 90, -3 },
  { 76, -5 },
  { 87, -2 },
  { 87, 0 },
  { 91, -3 },
  { 91, -1 },
  { 91, -1 },
  { 91, -1 },
  { 91, -1 },
  { 91, -3 },
  { 91, -1 },
  { 91, -3 },
  { 91, -4 },
  { 91, -6 },
  { 91, -7 },
  { 93, -3 },
  { 93, -3 },
  { 93, -3 },
  { 93, -3 },
  { 93, -3 },
  { 93, -3 },
  { 93, -3 },
  { 93, -3 },
  { 93, -3 },
  { 93, -3 },
  { 93, -3 },
  { 93, -3 },
  { 93, -3 },
  { 92, -2 },
  { 92, -2 },
  { 92, -2 },
  { 92, -2 },
  { 95, -3 },
  { 95, -1 },
  { 95, 0 },
  { 96, -4 },
  { 97, 0 },
  { 97, -3 },
  { 97, -1 },
  { 98, -3 },
  { 94, -1 },
  { 94, -1 },
  { 94, -1 },
  { 94, -1 },
  { 94, -1 },
  { 94, -1 },
  { 94, -1 },
  { 94, -1 },
  { 94, -1 },
  { 94, -1 },
  { 94, -1 },
  { 94, -1 },
  { 94, -1 },
  { 68, -2 },
  { 99, -3 },
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
    yylhsminor.yy46 =yymsp[-1].minor.yy46; yymsp[-1].minor.yy46->addDecl(yymsp[0].minor.yy188);
}
#line 1326 "stparserimpl.c"
  yymsp[-1].minor.yy46 = yylhsminor.yy46;
        break;
      case 1: /* decls ::= decls decl_function */
      case 2: /* decls ::= decls decl_type_function */ yytestcase(yyruleno==2);
#line 128 "stparserimpl.lemon"
{
        yylhsminor.yy46 =yymsp[-1].minor.yy46; yymsp[-1].minor.yy46->addDecl(yymsp[0].minor.yy14);
}
#line 1335 "stparserimpl.c"
  yymsp[-1].minor.yy46 = yylhsminor.yy46;
        break;
      case 3: /* decls ::= */
#line 134 "stparserimpl.lemon"
{
        yymsp[1].minor.yy46 =  pParser->getDeclModule();
     }
#line 1343 "stparserimpl.c"
        break;
      case 4: /* identifier ::= IDENTIFIER */
      case 5: /* self ::= SELF */ yytestcase(yyruleno==5);
#line 141 "stparserimpl.lemon"
{
        yylhsminor.yy51 = pParser->createNode<yal::frontend::STIdentifier>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy51->setSourceInfo(srcInfo);
}
#line 1353 "stparserimpl.c"
  yymsp[0].minor.yy51 = yylhsminor.yy51;
        break;
      case 6: /* identifier_or_self ::= identifier */
      case 7: /* identifier_or_self ::= self */ yytestcase(yyruleno==7);
#line 153 "stparserimpl.lemon"
{yylhsminor.yy51=yymsp[0].minor.yy51;}
#line 1360 "stparserimpl.c"
  yymsp[0].minor.yy51 = yylhsminor.yy51;
        break;
      case 8: /* type_specifier ::= type_builtin */
#line 156 "stparserimpl.lemon"
{
        yylhsminor.yy170=yymsp[0].minor.yy170;
}
#line 1368 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 9: /* type_specifier ::= type_array */
#line 160 "stparserimpl.lemon"
{yymsp[0].minor.yy170 = nullptr;}
#line 1374 "stparserimpl.c"
        break;
      case 10: /* type_specifier ::= IDENTIFIER */
#line 161 "stparserimpl.lemon"
{
    yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yymsp[0].minor.yy0.tokenStr);
    yylhsminor.yy170->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0,yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1382 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 11: /* type_builtin ::= TYPE_BOOL */
#line 166 "stparserimpl.lemon"
{
    yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Bool);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy170->setSourceInfo(srcInfo);
    }
#line 1392 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 12: /* type_builtin ::= TYPE_INT8 */
#line 171 "stparserimpl.lemon"
{
        yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy170->setSourceInfo(srcInfo);
    }
#line 1402 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 13: /* type_builtin ::= TYPE_UINT8 */
#line 176 "stparserimpl.lemon"
{
        yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy170->setSourceInfo(srcInfo);
    }
#line 1412 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 14: /* type_builtin ::= TYPE_INT16 */
#line 181 "stparserimpl.lemon"
{
        yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy170->setSourceInfo(srcInfo);
    }
#line 1422 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 15: /* type_builtin ::= TYPE_UINT16 */
#line 186 "stparserimpl.lemon"
{
        yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy170->setSourceInfo(srcInfo);
    }
#line 1432 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 16: /* type_builtin ::= TYPE_INT32 */
#line 191 "stparserimpl.lemon"
{
        yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy170->setSourceInfo(srcInfo);
    }
#line 1442 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 17: /* type_builtin ::= TYPE_UINT32 */
#line 196 "stparserimpl.lemon"
{
        yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy170->setSourceInfo(srcInfo);
    }
#line 1452 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 18: /* type_builtin ::= TYPE_INT64 */
#line 201 "stparserimpl.lemon"
{
        yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy170->setSourceInfo(srcInfo);
    }
#line 1462 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 19: /* type_builtin ::= TYPE_UINT64 */
#line 206 "stparserimpl.lemon"
{
        yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy170->setSourceInfo(srcInfo);
    }
#line 1472 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 20: /* type_builtin ::= TYPE_FLOAT */
#line 211 "stparserimpl.lemon"
{
        yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy170->setSourceInfo(srcInfo);
    }
#line 1482 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 21: /* type_builtin ::= TYPE_DOUBLE */
#line 216 "stparserimpl.lemon"
{
        yylhsminor.yy170 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy170->setSourceInfo(srcInfo);
    }
#line 1492 "stparserimpl.c"
  yymsp[0].minor.yy170 = yylhsminor.yy170;
        break;
      case 22: /* qualifier ::= */
#line 224 "stparserimpl.lemon"
{yymsp[1].minor.yy141 = 0;}
#line 1498 "stparserimpl.c"
        break;
      case 23: /* qualifier ::= MUT */
#line 225 "stparserimpl.lemon"
{yymsp[0].minor.yy141 = yal::frontend::STQualType::Qualifiers::kQualMutable;}
#line 1503 "stparserimpl.c"
        break;
      case 24: /* qualifier ::= REFERENCE */
#line 226 "stparserimpl.lemon"
{yymsp[0].minor.yy141 = yal::frontend::STQualType::Qualifiers::kQualReference;}
#line 1508 "stparserimpl.c"
        break;
      case 25: /* qualifier ::= MUT REFERENCE */
#line 227 "stparserimpl.lemon"
{
        yymsp[-1].minor.yy141 = yal::frontend::STQualType::Qualifiers::kQualReference | yal::frontend::STQualType::Qualifiers::kQualMutable;
 }
#line 1515 "stparserimpl.c"
        break;
      case 26: /* type_qualified ::= qualifier type_specifier */
#line 230 "stparserimpl.lemon"
{
    yylhsminor.yy25 = pParser->createNode<yal::frontend::STQualType>(yymsp[0].minor.yy170, yymsp[-1].minor.yy141);
}
#line 1522 "stparserimpl.c"
  yymsp[-1].minor.yy25 = yylhsminor.yy25;
        break;
      case 27: /* decl_type ::= TYPE identifier COLON type_specifier SEMI_COLON */
#line 236 "stparserimpl.lemon"
{
    yymsp[-4].minor.yy188 = nullptr;
}
#line 1530 "stparserimpl.c"
        break;
      case 28: /* decl_type ::= decl_struct */
#line 240 "stparserimpl.lemon"
{ yylhsminor.yy188 = yymsp[0].minor.yy109;}
#line 1535 "stparserimpl.c"
  yymsp[0].minor.yy188 = yylhsminor.yy188;
        break;
      case 29: /* decl_struct ::= TYPE identifier COLON STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END */
#line 245 "stparserimpl.lemon"
{
    yylhsminor.yy109 = pParser->createNode<yal::frontend::STDeclStruct>(yymsp[-5].minor.yy51, yymsp[-1].minor.yy172, *pParser);
    auto srcInfo = CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy109->setSourceInfo(srcInfo);
}
#line 1545 "stparserimpl.c"
  yymsp[-6].minor.yy109 = yylhsminor.yy109;
        break;
      case 30: /* struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
#line 251 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy172 = list.add(yymsp[-2].minor.yy172, yymsp[0].minor.yy53);
}
#line 1554 "stparserimpl.c"
  yymsp[-2].minor.yy172 = yylhsminor.yy172;
        break;
      case 31: /* struct_decl_vars ::= struct_decl_var */
#line 255 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy172 = list.add(yymsp[0].minor.yy53);
}
#line 1563 "stparserimpl.c"
  yymsp[0].minor.yy172 = yylhsminor.yy172;
        break;
      case 32: /* struct_decl_var ::= identifier COLON type_qualified */
#line 260 "stparserimpl.lemon"
{
    yylhsminor.yy53 = pParser->createNode<yal::frontend::STStructMember>(yymsp[-2].minor.yy51, yymsp[0].minor.yy25);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy51->getSourceInfo(), yymsp[0].minor.yy25->getSourceInfo(),pParser->getSourceHandle());
    yylhsminor.yy53->setSourceInfo(srcInfo);
}
#line 1573 "stparserimpl.c"
  yymsp[-2].minor.yy53 = yylhsminor.yy53;
        break;
      case 33: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
#line 269 "stparserimpl.lemon"
{
    yymsp[-2].minor.yy156 = yymsp[-1].minor.yy156;
}
#line 1581 "stparserimpl.c"
        break;
      case 34: /* decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 274 "stparserimpl.lemon"
{
    yylhsminor.yy14 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy51, nullptr, yymsp[-3].minor.yy25, yymsp[-4].minor.yy156, yymsp[-1].minor.yy55, *pParser);
    yylhsminor.yy14->setSourceInfo(CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1589 "stparserimpl.c"
  yymsp[-6].minor.yy14 = yylhsminor.yy14;
        break;
      case 35: /* decl_type_function ::= FUNCTION type_specifier COLON COLON identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 282 "stparserimpl.lemon"
{
    yylhsminor.yy14 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy51, yymsp[-8].minor.yy170, yymsp[-3].minor.yy25, yymsp[-4].minor.yy156, yymsp[-1].minor.yy55, *pParser);
    yylhsminor.yy14->setSourceInfo(CreateSourceInfo(yymsp[-9].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1598 "stparserimpl.c"
  yymsp[-9].minor.yy14 = yylhsminor.yy14;
        break;
      case 36: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
#line 289 "stparserimpl.lemon"
{
        auto& list = pParser->getDeclParamList();
        yylhsminor.yy156 = list.add(yymsp[-2].minor.yy156, yymsp[0].minor.yy101);
}
#line 1607 "stparserimpl.c"
  yymsp[-2].minor.yy156 = yylhsminor.yy156;
        break;
      case 37: /* function_args_decl ::= function_arg_decl */
#line 294 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclParamList();
    yylhsminor.yy156 = list.add(yymsp[0].minor.yy101);
}
#line 1616 "stparserimpl.c"
  yymsp[0].minor.yy156 = yylhsminor.yy156;
        break;
      case 38: /* function_args_decl ::= */
#line 299 "stparserimpl.lemon"
{
   yymsp[1].minor.yy156 = pParser->getDeclParamList().getRangeEmpty();
}
#line 1624 "stparserimpl.c"
        break;
      case 39: /* function_arg_decl ::= identifier COLON type_qualified */
#line 303 "stparserimpl.lemon"
{
        yylhsminor.yy101 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[-2].minor.yy51, yymsp[0].minor.yy25);
        yylhsminor.yy101->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy51->getSourceInfo(),yymsp[0].minor.yy25->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1632 "stparserimpl.c"
  yymsp[-2].minor.yy101 = yylhsminor.yy101;
        break;
      case 40: /* function_arg_decl ::= qualifier self */
#line 308 "stparserimpl.lemon"
{
        auto qualType = pParser->createNode<yal::frontend::STQualType>(nullptr, yymsp[-1].minor.yy141);
        yylhsminor.yy101 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[0].minor.yy51, qualType);
        yylhsminor.yy101->setSourceInfo(yymsp[0].minor.yy51->getSourceInfo());
}
#line 1642 "stparserimpl.c"
  yymsp[-1].minor.yy101 = yylhsminor.yy101;
        break;
      case 41: /* function_return_decl ::= COLON type_qualified */
#line 314 "stparserimpl.lemon"
{ yymsp[-1].minor.yy25 = yymsp[0].minor.yy25;}
#line 1648 "stparserimpl.c"
        break;
      case 42: /* function_return_decl ::= */
      case 55: /* var_type_spec ::= */ yytestcase(yyruleno==55);
#line 315 "stparserimpl.lemon"
{yymsp[1].minor.yy25 = nullptr;}
#line 1654 "stparserimpl.c"
        break;
      case 43: /* statement_list_or_empty ::= */
#line 319 "stparserimpl.lemon"
{yymsp[1].minor.yy55 = pParser->getStmtList().getRangeEmpty();}
#line 1659 "stparserimpl.c"
        break;
      case 44: /* statement_list_or_empty ::= statement_list */
#line 320 "stparserimpl.lemon"
{yylhsminor.yy55 = yymsp[0].minor.yy55;}
#line 1664 "stparserimpl.c"
  yymsp[0].minor.yy55 = yylhsminor.yy55;
        break;
      case 45: /* statement_list ::= statement_list statement */
#line 323 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy55 = list.add(yymsp[-1].minor.yy55,yymsp[0].minor.yy89);
}
#line 1673 "stparserimpl.c"
  yymsp[-1].minor.yy55 = yylhsminor.yy55;
        break;
      case 46: /* statement_list ::= statement */
#line 327 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy55 = list.add(yymsp[0].minor.yy89);
}
#line 1682 "stparserimpl.c"
  yymsp[0].minor.yy55 = yylhsminor.yy55;
        break;
      case 47: /* statement ::= expression ASSIGN expression SEMI_COLON */
#line 333 "stparserimpl.lemon"
{
   yylhsminor.yy89 = pParser->createNode<yal::frontend::STStmtAssign>(yymsp[-3].minor.yy192,yymsp[-1].minor.yy192);
   yylhsminor.yy89->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy192->getSourceInfo(), yymsp[-1].minor.yy192->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1691 "stparserimpl.c"
  yymsp[-3].minor.yy89 = yylhsminor.yy89;
        break;
      case 48: /* statement ::= decl_var SEMI_COLON */
#line 337 "stparserimpl.lemon"
{
    yylhsminor.yy89 = pParser->createNode<yal::frontend::STStmtDecl>(yymsp[-1].minor.yy137);
    yylhsminor.yy89->setSourceInfo(yymsp[-1].minor.yy137->getSourceInfo());
}
#line 1700 "stparserimpl.c"
  yymsp[-1].minor.yy89 = yylhsminor.yy89;
        break;
      case 49: /* statement ::= expression SEMI_COLON */
#line 342 "stparserimpl.lemon"
{
    yylhsminor.yy89 = pParser->createNode<yal::frontend::STStmtExpression>(yymsp[-1].minor.yy192);
    yylhsminor.yy89->setSourceInfo(yymsp[-1].minor.yy192->getSourceInfo());
}
#line 1709 "stparserimpl.c"
  yymsp[-1].minor.yy89 = yylhsminor.yy89;
        break;
      case 50: /* statement ::= RETURN expression SEMI_COLON */
#line 347 "stparserimpl.lemon"
{
    yylhsminor.yy89 = pParser->createNode<yal::frontend::STStmtReturn>(yymsp[-1].minor.yy192);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy89->setSourceInfo(srcInfo);
}
#line 1719 "stparserimpl.c"
  yymsp[-2].minor.yy89 = yylhsminor.yy89;
        break;
      case 51: /* statement ::= RETURN SEMI_COLON */
#line 353 "stparserimpl.lemon"
{
    yylhsminor.yy89 = pParser->createNode<yal::frontend::STStmtReturn>(nullptr);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[-1].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy89->setSourceInfo(srcInfo);
}
#line 1729 "stparserimpl.c"
  yymsp[-1].minor.yy89 = yylhsminor.yy89;
        break;
      case 52: /* statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 359 "stparserimpl.lemon"
{
    yylhsminor.yy89 = pParser->createNode<yal::frontend::STStmtListScoped>(yymsp[-1].minor.yy55, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy89->setSourceInfo(srcInfo);
}
#line 1739 "stparserimpl.c"
  yymsp[-2].minor.yy89 = yylhsminor.yy89;
        break;
      case 53: /* decl_var ::= VAR identifier var_type_spec ASSIGN expression */
#line 367 "stparserimpl.lemon"
{
    yylhsminor.yy137 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy51, yymsp[-2].minor.yy25, yymsp[0].minor.yy192);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy137->setSourceInfo(srcInfo);
}
#line 1749 "stparserimpl.c"
  yymsp[-4].minor.yy137 = yylhsminor.yy137;
        break;
      case 54: /* var_type_spec ::= COLON type_qualified */
#line 373 "stparserimpl.lemon"
{yymsp[-1].minor.yy25 = yymsp[0].minor.yy25;}
#line 1755 "stparserimpl.c"
        break;
      case 56: /* expression ::= PAR_BEGIN expression PAR_END */
#line 379 "stparserimpl.lemon"
{yymsp[-2].minor.yy192 = yymsp[-1].minor.yy192;}
#line 1760 "stparserimpl.c"
        break;
      case 57: /* expression ::= literal */
#line 380 "stparserimpl.lemon"
{yylhsminor.yy192 = yymsp[0].minor.yy192;}
#line 1765 "stparserimpl.c"
  yymsp[0].minor.yy192 = yylhsminor.yy192;
        break;
      case 58: /* expression ::= unaryexp */
#line 381 "stparserimpl.lemon"
{yylhsminor.yy192 = yymsp[0].minor.yy198;}
#line 1771 "stparserimpl.c"
  yymsp[0].minor.yy192 = yylhsminor.yy192;
        break;
      case 59: /* expression ::= binaryexp */
#line 382 "stparserimpl.lemon"
{yylhsminor.yy192 = yymsp[0].minor.yy140;}
#line 1777 "stparserimpl.c"
  yymsp[0].minor.yy192 = yylhsminor.yy192;
        break;
      case 60: /* expression ::= struct_init */
#line 383 "stparserimpl.lemon"
{ yylhsminor.yy192 = yymsp[0].minor.yy180;}
#line 1783 "stparserimpl.c"
  yymsp[0].minor.yy192 = yylhsminor.yy192;
        break;
      case 61: /* expression ::= expression AS type_qualified */
#line 385 "stparserimpl.lemon"
{
    yylhsminor.yy192 = pParser->createNode<yal::frontend::STExprCast>(yymsp[-2].minor.yy192, yymsp[0].minor.yy25);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(),
                                                   yymsp[0].minor.yy25->getSourceInfo(),
                                                   pParser->getSourceHandle());
    yylhsminor.yy192->setSourceInfo(srcInfo);
}
#line 1795 "stparserimpl.c"
  yymsp[-2].minor.yy192 = yylhsminor.yy192;
        break;
      case 62: /* expression ::= identifier_or_self */
#line 393 "stparserimpl.lemon"
{
    yylhsminor.yy192 = pParser->createNode<yal::frontend::STExprVarRef>(yymsp[0].minor.yy51);
    yylhsminor.yy192->setSourceInfo(yymsp[0].minor.yy51->getSourceInfo());
}
#line 1804 "stparserimpl.c"
  yymsp[0].minor.yy192 = yylhsminor.yy192;
        break;
      case 63: /* expression ::= expression DOT identifier */
#line 398 "stparserimpl.lemon"
{
    yylhsminor.yy192 = pParser->createNode<yal::frontend::STExprStructVarRef>(yymsp[-2].minor.yy192, yymsp[0].minor.yy51);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy51->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy192->setSourceInfo(srcInfo);
}
#line 1814 "stparserimpl.c"
  yymsp[-2].minor.yy192 = yylhsminor.yy192;
        break;
      case 64: /* expression ::= identifier PAR_BEGIN function_call_args PAR_END */
#line 404 "stparserimpl.lemon"
{
    yylhsminor.yy192 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy51,yymsp[-1].minor.yy62,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy51->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy192->setSourceInfo(srcInfo);
}
#line 1824 "stparserimpl.c"
  yymsp[-3].minor.yy192 = yylhsminor.yy192;
        break;
      case 65: /* expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END */
#line 410 "stparserimpl.lemon"
{
    yylhsminor.yy192 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy51,yymsp[-5].minor.yy192,yymsp[-1].minor.yy62,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-5].minor.yy192->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy192->setSourceInfo(srcInfo);
}
#line 1834 "stparserimpl.c"
  yymsp[-5].minor.yy192 = yylhsminor.yy192;
        break;
      case 66: /* expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END */
#line 416 "stparserimpl.lemon"
{
    yylhsminor.yy192 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy51,yymsp[-6].minor.yy170,yymsp[-1].minor.yy62,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy170->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy192->setSourceInfo(srcInfo);
}
#line 1844 "stparserimpl.c"
  yymsp[-6].minor.yy192 = yylhsminor.yy192;
        break;
      case 67: /* binaryexp ::= expression AND expression */
#line 424 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1855 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 68: /* binaryexp ::= expression OR expression */
#line 430 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1866 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 69: /* binaryexp ::= expression PLUS expression */
#line 436 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1877 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 70: /* binaryexp ::= expression MINUS expression */
#line 442 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1888 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 71: /* binaryexp ::= expression MULT expression */
#line 448 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1899 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 72: /* binaryexp ::= expression DIV expression */
#line 454 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
}
#line 1908 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 73: /* binaryexp ::= expression MOD expression */
#line 458 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1919 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 74: /* binaryexp ::= expression EQ expression */
#line 465 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1930 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 75: /* binaryexp ::= expression NE expression */
#line 472 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1941 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 76: /* binaryexp ::= expression LE expression */
#line 479 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1952 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 77: /* binaryexp ::= expression LT expression */
#line 486 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1963 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 78: /* binaryexp ::= expression GE expression */
#line 493 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1974 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 79: /* binaryexp ::= expression GT expression */
#line 500 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy192, yymsp[0].minor.yy192);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy192->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 1985 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 80: /* unaryexp ::= NOT expression */
#line 509 "stparserimpl.lemon"
{
    yylhsminor.yy198 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy192);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy198->setSourceInfo(srcInfo);
}
#line 1996 "stparserimpl.c"
  yymsp[-1].minor.yy198 = yylhsminor.yy198;
        break;
      case 81: /* unaryexp ::= BIT_NOT expression */
#line 515 "stparserimpl.lemon"
{
    yylhsminor.yy198 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy192);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy198->setSourceInfo(srcInfo);
}
#line 2007 "stparserimpl.c"
  yymsp[-1].minor.yy198 = yylhsminor.yy198;
        break;
      case 82: /* unaryexp ::= MINUS expression */
#line 521 "stparserimpl.lemon"
{
    yylhsminor.yy198 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy192);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy198->setSourceInfo(srcInfo);
}
#line 2018 "stparserimpl.c"
  yymsp[-1].minor.yy198 = yylhsminor.yy198;
        break;
      case 83: /* unaryexp ::= REFERENCE expression */
#line 528 "stparserimpl.lemon"
{
    yylhsminor.yy198 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy192);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy198->setSourceInfo(srcInfo);
}
#line 2029 "stparserimpl.c"
  yymsp[-1].minor.yy198 = yylhsminor.yy198;
        break;
      case 84: /* function_call_args ::= function_call_args COMMA expression */
#line 537 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy62 = list.add(yymsp[-2].minor.yy62, yymsp[0].minor.yy192);
}
#line 2038 "stparserimpl.c"
  yymsp[-2].minor.yy62 = yylhsminor.yy62;
        break;
      case 85: /* function_call_args ::= expression */
#line 541 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy62 = list.add(yymsp[0].minor.yy192);
}
#line 2047 "stparserimpl.c"
  yymsp[0].minor.yy62 = yylhsminor.yy62;
        break;
      case 86: /* function_call_args ::= */
#line 545 "stparserimpl.lemon"
{
    yymsp[1].minor.yy62= pParser->getExprList().getRangeEmpty();
}
#line 2055 "stparserimpl.c"
        break;
      case 87: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
#line 552 "stparserimpl.lemon"
{
    yylhsminor.yy180 = pParser->createNode<yal::frontend::STExprStructInit>(yymsp[-3].minor.yy170, yymsp[-1].minor.yy189, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy170->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy180->setSourceInfo(srcInfo);
}
#line 2064 "stparserimpl.c"
  yymsp[-3].minor.yy180 = yylhsminor.yy180;
        break;
      case 88: /* struct_member_init_list ::= */
#line 558 "stparserimpl.lemon"
{yymsp[1].minor.yy189 = pParser->getStructInitList().getRangeEmpty();}
#line 2070 "stparserimpl.c"
        break;
      case 89: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
#line 559 "stparserimpl.lemon"
{
     auto& list = pParser->getStructInitList();
     yylhsminor.yy189 = list.add(yymsp[-2].minor.yy189, yymsp[0].minor.yy193);
}
#line 2078 "stparserimpl.c"
  yymsp[-2].minor.yy189 = yylhsminor.yy189;
        break;
      case 90: /* struct_member_init_list ::= struct_member_init */
#line 563 "stparserimpl.lemon"
{
    auto& list = pParser->getStructInitList();
    yylhsminor.yy189 = list.add(yymsp[0].minor.yy193);
}
#line 2087 "stparserimpl.c"
  yymsp[0].minor.yy189 = yylhsminor.yy189;
        break;
      case 91: /* struct_member_init ::= identifier COLON expression */
#line 568 "stparserimpl.lemon"
{
    yylhsminor.yy193 = pParser->createNode<yal::frontend::STStructMemberInit>(yymsp[-2].minor.yy51,yymsp[0].minor.yy192);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy51->getSourceInfo(), yymsp[0].minor.yy192->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy193->setSourceInfo(srcInfo);
}
#line 2097 "stparserimpl.c"
  yymsp[-2].minor.yy193 = yylhsminor.yy193;
        break;
      case 92: /* literal ::= INTEGER_LITERAL */
      case 93: /* literal ::= INTEGER_LITERAL_I8 */ yytestcase(yyruleno==93);
      case 94: /* literal ::= INTEGER_LITERAL_I16 */ yytestcase(yyruleno==94);
      case 95: /* literal ::= INTEGER_LITERAL_I32 */ yytestcase(yyruleno==95);
      case 96: /* literal ::= INTEGER_LITERAL_I64 */ yytestcase(yyruleno==96);
      case 97: /* literal ::= INTEGER_LITERAL_U8 */ yytestcase(yyruleno==97);
      case 98: /* literal ::= INTEGER_LITERAL_U16 */ yytestcase(yyruleno==98);
      case 99: /* literal ::= INTEGER_LITERAL_U32 */ yytestcase(yyruleno==99);
      case 100: /* literal ::= INTEGER_LITERAL_U64 */ yytestcase(yyruleno==100);
#line 576 "stparserimpl.lemon"
{
        yylhsminor.yy192 = pParser->createNode<yal::frontend::STExprIntegerLiteral>(yymsp[0].minor.yy0);
        auto srcInfo =yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy192->setSourceInfo(srcInfo);
   }
#line 2115 "stparserimpl.c"
  yymsp[0].minor.yy192 = yylhsminor.yy192;
        break;
      case 101: /* literal ::= FLOAT_LITERAL */
      case 102: /* literal ::= FLOAT_LITERAL_32 */ yytestcase(yyruleno==102);
      case 103: /* literal ::= FLOAT_LITERAL_64 */ yytestcase(yyruleno==103);
#line 621 "stparserimpl.lemon"
{
        yylhsminor.yy192 = pParser->createNode<yal::frontend::STExprFloatLiteral>(yymsp[0].minor.yy0);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy192->setSourceInfo(srcInfo);
}
#line 2127 "stparserimpl.c"
  yymsp[0].minor.yy192 = yylhsminor.yy192;
        break;
      case 104: /* literal ::= BOOL_LITERAL */
#line 638 "stparserimpl.lemon"
{
        yylhsminor.yy192 = pParser->createNode<yal::frontend::STExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy192->setSourceInfo(srcInfo);
}
#line 2137 "stparserimpl.c"
  yymsp[0].minor.yy192 = yylhsminor.yy192;
        break;
      default:
      /* (105) module ::= decls END */ yytestcase(yyruleno==105);
      /* (106) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==106);
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
#line 2221 "stparserimpl.c"
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
