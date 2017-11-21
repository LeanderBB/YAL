/* This file was automatically generated.  Do not edit! */
#define YALParserTOKENTYPE yal::StringRefPod
#define YALParserARG_PDECL , yal::Parser *pParser 
void YALParser(void *yyp,int yymajor,YALParserTOKENTYPE yyminor YALParserARG_PDECL);
#if defined(YYTRACKMAXSTACKDEPTH)
int YALParserStackPeak(void *p);
#endif
void YALParserFree(void *p,void(*freeProc)(void *));
void *YALParserAlloc(void *(*mallocProc)(size_t));
#if !defined(NDEBUG)
void YALParserTrace(FILE *TraceFILE,char *zTracePrompt);
#endif
#define YALParserARG_STORE yypParser->pParser  = pParser 
#define YALParserARG_FETCH  yal::Parser *pParser  = yypParser->pParser 
#define YALParserARG_SDECL  yal::Parser *pParser ;
#define YAL_TOKEN_BOOL_LITERAL                   52
#define YAL_TOKEN_DECIMAL_LITERAL                51
#define YAL_TOKEN_INTEGER_LITERAL                50
#define YAL_TOKEN_LET                            49
#define YAL_TOKEN_VAR                            48
#define YAL_TOKEN_RETURN                         47
#define YAL_TOKEN_COMMA                          46
#define YAL_TOKEN_PAR_END                        45
#define YAL_TOKEN_PAR_BEGIN                      44
#define YAL_TOKEN_FUNCTION                       43
#define YAL_TOKEN_SEMI_COLON                     42
#define YAL_TOKEN_SCOPE_END                      41
#define YAL_TOKEN_SCOPE_BEGIN                    40
#define YAL_TOKEN_STRUCT                         39
#define YAL_TOKEN_COLON                          38
#define YAL_TOKEN_TYPE                           37
#define YAL_TOKEN_ARRAY_END                      36
#define YAL_TOKEN_ARRAY_BEGIN                    35
#define YAL_TOKEN_TYPE_DOUBLE                    34
#define YAL_TOKEN_TYPE_FLOAT                     33
#define YAL_TOKEN_TYPE_UINT64                    32
#define YAL_TOKEN_TYPE_INT64                     31
#define YAL_TOKEN_TYPE_UINT32                    30
#define YAL_TOKEN_TYPE_INT32                     29
#define YAL_TOKEN_TYPE_UINT16                    28
#define YAL_TOKEN_TYPE_INT16                     27
#define YAL_TOKEN_TYPE_UINT8                     26
#define YAL_TOKEN_TYPE_INT8                      25
#define YAL_TOKEN_TYPE_BOOL                      24
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
