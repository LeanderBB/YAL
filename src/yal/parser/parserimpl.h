/* This file was automatically generated.  Do not edit! */
#define YALParserTOKENTYPE void*
#define YALParserARG_PDECL
void YALParser(void *yyp,int yymajor,YALParserTOKENTYPE yyminor YALParserARG_PDECL);
#if defined(YYTRACKMAXSTACKDEPTH)
int YALParserStackPeak(void *p);
#endif
void YALParserFree(void *p,void(*freeProc)(void *));
void *YALParserAlloc(void *(*mallocProc)(size_t));
#if !defined(NDEBUG)
void YALParserTrace(FILE *TraceFILE,char *zTracePrompt);
#endif
#define YALParserARG_STORE
#define YALParserARG_FETCH
#define YALParserARG_SDECL
#define YAL_TOKEN_SEMI_COLON                     27
#define YAL_TOKEN_SCOPE_END                      26
#define YAL_TOKEN_SCOPE_BEGIN                    25
#define YAL_TOKEN_TYPE                           24
#define YAL_TOKEN_COLON                          23
#define YAL_TOKEN_NAME                           22
#define YAL_TOKEN_END                            21
#define YAL_TOKEN_DOT                            20
#define YAL_TOKEN_NOT                            19
#define YAL_TOKEN_NEG                            18
#define YAL_TOKEN_MOD                            17
#define YAL_TOKEN_DIV                            16
#define YAL_TOKEN_MULT                           15
#define YAL_TOKEN_MINUS                          14
#define YAL_TOKEN_PLUS                           13
#define YAL_TOKEN_LE                             12
#define YAL_TOKEN_GE                             11
#define YAL_TOKEN_LT                             10
#define YAL_TOKEN_GT                              9
#define YAL_TOKEN_NE                              8
#define YAL_TOKEN_EQ                              7
#define YAL_TOKEN_BIT_AND                         6
#define YAL_TOKEN_BIT_XOR                         5
#define YAL_TOKEN_BIT_OR                          4
#define YAL_TOKEN_AND                             3
#define YAL_TOKEN_OR                              2
#define YAL_TOKEN_ASSIGN                          1
#define INTERFACE 0
