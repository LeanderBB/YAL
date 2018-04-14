/* This file was automatically generated.  Do not edit! */
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#define YALSTParserARG_PDECL , yal::frontend::STParser *pParser 
void YALSTParser(void *yyp,int yymajor,YALSTParserTOKENTYPE yyminor YALSTParserARG_PDECL);
#if defined(YYTRACKMAXSTACKDEPTH)
int YALSTParserStackPeak(void *p);
#endif
#if !defined(YALSTParser_ENGINEALWAYSONSTACK)
void YALSTParserFree(void *p,void(*freeProc)(void *));
#endif
void YALSTParserFinalize(void *p);
#if !defined(YALSTParser_ENGINEALWAYSONSTACK)
void *YALSTParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE));
#endif
void YALSTParserInit(void *yypParser);
#if !defined(NDEBUG)
void YALSTParserTrace(FILE *TraceFILE,char *zTracePrompt);
#endif
#define YALSTParserARG_STORE yypParser->pParser  = pParser 
#define YALSTParserARG_FETCH  yal::frontend::STParser *pParser  = yypParser->pParser 
#define YALSTParserARG_SDECL  yal::frontend::STParser *pParser ;
#define YAL_TOKEN_BOOL_LITERAL                   56
#define YAL_TOKEN_FLOAT_LITERAL                  55
#define YAL_TOKEN_INTEGER_LITERAL                54
#define YAL_TOKEN_RANGE_CAST                     53
#define YAL_TOKEN_LET                            52
#define YAL_TOKEN_VAR                            51
#define YAL_TOKEN_RETURN                         50
#define YAL_TOKEN_FUNCTION                       49
#define YAL_TOKEN_PAR_END                        48
#define YAL_TOKEN_PAR_BEGIN                      47
#define YAL_TOKEN_COMMA                          46
#define YAL_TOKEN_SCOPE_END                      45
#define YAL_TOKEN_SCOPE_BEGIN                    44
#define YAL_TOKEN_STRUCT                         43
#define YAL_TOKEN_SEMI_COLON                     42
#define YAL_TOKEN_COLON                          41
#define YAL_TOKEN_TYPE                           40
#define YAL_TOKEN_REFERENCE                      39
#define YAL_TOKEN_MUT                            38
#define YAL_TOKEN_ARRAY_END                      37
#define YAL_TOKEN_ARRAY_BEGIN                    36
#define YAL_TOKEN_TYPE_DOUBLE                    35
#define YAL_TOKEN_TYPE_FLOAT                     34
#define YAL_TOKEN_TYPE_UINT64                    33
#define YAL_TOKEN_TYPE_INT64                     32
#define YAL_TOKEN_TYPE_UINT32                    31
#define YAL_TOKEN_TYPE_INT32                     30
#define YAL_TOKEN_TYPE_UINT16                    29
#define YAL_TOKEN_TYPE_INT16                     28
#define YAL_TOKEN_TYPE_UINT8                     27
#define YAL_TOKEN_TYPE_INT8                      26
#define YAL_TOKEN_TYPE_BOOL                      25
#define YAL_TOKEN_SELF                           24
#define YAL_TOKEN_IDENTIFIER                     23
#define YAL_TOKEN_END                            22
#define YAL_TOKEN_DOT                            21
#define YAL_TOKEN_NOT                            20
#define YAL_TOKEN_BIT_NOT                        19
#define YAL_TOKEN_NEGATE                         18
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
