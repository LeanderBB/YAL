/* This file was automatically generated.  Do not edit! */
int YALSTParserFallback(int iToken);
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#define YALSTParserARG_PDECL , yal::frontend::STParser *pParser 
void YALSTParser(void *yyp,int yymajor,YALSTParserTOKENTYPE yyminor YALSTParserARG_PDECL);
#if defined(YYCOVERAGE)
int YALSTParserCoverage(FILE *out);
#endif
#if defined(YYTRACKMAXSTACKDEPTH)
int YALSTParserStackPeak(void *p);
#endif
#if !defined(YALSTParser_ENGINEALWAYSONSTACK)
void YALSTParserFree(void *p,void(*freeProc)(void *));
#endif
void YALSTParserFinalize(void *p);
#define YALSTParserCTX_PDECL
#if !defined(YALSTParser_ENGINEALWAYSONSTACK)
void *YALSTParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)YALSTParserCTX_PDECL);
#endif
void YALSTParserInit(void *yypRawParser YALSTParserCTX_PDECL);
#if !defined(NDEBUG)
void YALSTParserTrace(FILE *TraceFILE,char *zTracePrompt);
#endif
#define YALSTParserCTX_STORE
#define YALSTParserCTX_FETCH
#define YALSTParserCTX_PARAM
#define YALSTParserCTX_SDECL
#define YALSTParserARG_STORE yypParser->pParser =pParser ;
#define YALSTParserARG_FETCH  yal::frontend::STParser *pParser =yypParser->pParser ;
#define YALSTParserARG_PARAM ,pParser 
#define YALSTParserARG_SDECL  yal::frontend::STParser *pParser ;
#define YAL_TOKEN_BOOL_LITERAL                   72
#define YAL_TOKEN_FLOAT_LITERAL_64               71
#define YAL_TOKEN_FLOAT_LITERAL_32               70
#define YAL_TOKEN_FLOAT_LITERAL                  69
#define YAL_TOKEN_INTEGER_LITERAL_U64            68
#define YAL_TOKEN_INTEGER_LITERAL_U32            67
#define YAL_TOKEN_INTEGER_LITERAL_U16            66
#define YAL_TOKEN_INTEGER_LITERAL_U8             65
#define YAL_TOKEN_INTEGER_LITERAL_I64            64
#define YAL_TOKEN_INTEGER_LITERAL_I32            63
#define YAL_TOKEN_INTEGER_LITERAL_I16            62
#define YAL_TOKEN_INTEGER_LITERAL_I8             61
#define YAL_TOKEN_INTEGER_LITERAL                60
#define YAL_TOKEN_LET                            59
#define YAL_TOKEN_VAR                            58
#define YAL_TOKEN_RETURN                         57
#define YAL_TOKEN_IMPL                           56
#define YAL_TOKEN_FUNCTION                       55
#define YAL_TOKEN_PAR_END                        54
#define YAL_TOKEN_PAR_BEGIN                      53
#define YAL_TOKEN_COMMA                          52
#define YAL_TOKEN_SCOPE_END                      51
#define YAL_TOKEN_SCOPE_BEGIN                    50
#define YAL_TOKEN_STRUCT                         49
#define YAL_TOKEN_FROM                           48
#define YAL_TOKEN_ALIAS                          47
#define YAL_TOKEN_TYPE                           46
#define YAL_TOKEN_COLON                          45
#define YAL_TOKEN_SEMI_COLON                     44
#define YAL_TOKEN_IMPORT                         43
#define YAL_TOKEN_MUT                            42
#define YAL_TOKEN_ARRAY_END                      41
#define YAL_TOKEN_ARRAY_BEGIN                    40
#define YAL_TOKEN_TYPE_DOUBLE                    39
#define YAL_TOKEN_TYPE_FLOAT                     38
#define YAL_TOKEN_TYPE_UINT64                    37
#define YAL_TOKEN_TYPE_INT64                     36
#define YAL_TOKEN_TYPE_UINT32                    35
#define YAL_TOKEN_TYPE_INT32                     34
#define YAL_TOKEN_TYPE_UINT16                    33
#define YAL_TOKEN_TYPE_INT16                     32
#define YAL_TOKEN_TYPE_UINT8                     31
#define YAL_TOKEN_TYPE_INT8                      30
#define YAL_TOKEN_TYPE_BOOL                      29
#define YAL_TOKEN_SELF                           28
#define YAL_TOKEN_IDENTIFIER                     27
#define YAL_TOKEN_IDENTIFIER_MULTI               26
#define YAL_TOKEN_END                            25
#define YAL_TOKEN_DOT                            24
#define YAL_TOKEN_REFERENCE                      23
#define YAL_TOKEN_DEREF                          22
#define YAL_TOKEN_NOT                            21
#define YAL_TOKEN_BIT_NOT                        20
#define YAL_TOKEN_NEGATE                         19
#define YAL_TOKEN_MOD                            18
#define YAL_TOKEN_DIV                            17
#define YAL_TOKEN_MULT                           16
#define YAL_TOKEN_MINUS                          15
#define YAL_TOKEN_PLUS                           14
#define YAL_TOKEN_LE                             13
#define YAL_TOKEN_GE                             12
#define YAL_TOKEN_LT                             11
#define YAL_TOKEN_GT                             10
#define YAL_TOKEN_NE                              9
#define YAL_TOKEN_EQ                              8
#define YAL_TOKEN_BIT_AND                         7
#define YAL_TOKEN_BIT_XOR                         6
#define YAL_TOKEN_BIT_OR                          5
#define YAL_TOKEN_AND                             4
#define YAL_TOKEN_OR                              3
#define YAL_TOKEN_AS                              2
#define YAL_TOKEN_ASSIGN                          1
#define INTERFACE 0
