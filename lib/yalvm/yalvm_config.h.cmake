#ifndef __YALVM_CONFIG_H__
#define __YALVM_CONFIG_H__

/* NOTE: Auto-generated file, do not modify */

/* NOTE: Do not include this file manually, always include yalvm.h instead */

#if defined(_MSC_VER )
// disable useless warnings in MSVC
#pragma warning(disable: 4514 4711 4055 4244 4800 4820)
#endif

#cmakedefine YALVM_VERSION_MAJOR @YALVM_VERSION_MAJOR@

#if !defined(YALVM_VERSION_MAJOR)
#define YALVM_VERSION_MAJOR 0
#endif

#cmakedefine YALVM_VERSION_MINOR @YALVM_VERSION_MINOR@

#if !defined(YALVM_VERSION_MINOR)
#define YALVM_VERSION_MINOR 0
#endif

#cmakedefine YALVM_VERSION_PATCH @YALVM_VERSION_PATCH@

#if !defined(YALVM_VERSION_PATCH)
#define YALVM_VERSION_PATCH 0
#endif

#define YALVM_VERSION_STR @YALVM_VERSION_STR@

#define YALVM_USE_CSTDLIB



#if defined(YALVM_USE_CSTDLIB)
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

typedef int8_t   yalvm_i8;
typedef uint8_t  yalvm_u8;
typedef int16_t  yalvm_i16;
typedef uint16_t yalvm_u16;
typedef int32_t  yalvm_i32;
typedef uint32_t yalvm_u32;
typedef int64_t  yalvm_i64;
typedef uint64_t yalvm_u64;
typedef float    yalvm_f32;
typedef double   yalvm_f64;
typedef size_t   yalvm_size;

#define YALVM_PRIi8  PRIi8
#define YALVM_PRIi16 PRIi16
#define YALVM_PRIi32 PRIi32
#define YALVM_PRIi64 PRIi64

#define YALVM_PRIu8  PRIu8
#define YALVM_PRIu16 PRIu16
#define YALVM_PRIu32 PRIu32
#define YALVM_PRIu64 PRIu64

#define YALVM_PRIf32 "f"
#define YALVM_PRIf64 "f"

#define YALVM_PRI_HEX8  SCNx8
#define YALVM_PRI_HEX16 SCNx16
#define YALVM_PRI_HEX32 SCNx32
#define YALVM_PRI_HEX64 SCNx64

#else

typedef char            yalvm_i8;
typedef unsigned char   yalvm_u8;
typedef short           yalvm_i16;
typedef unsigned short  yalvm_u16;
typedef int             yalvm_i32;
typedef unsigned int    yalvm_u32;
typedef long long       yalvm_i64;
typedef long long       yalvm_u64;
typedef float           yalvm_f32;
typedef double          yalvm_f64;
typedef long            yalvm_size;

#define YALVM_PRIi8  "hhd"
#define YALVM_PRIi16 "hd"
#define YALVM_PRIi32 "d"
#define YALVM_PRIi64 "lld"

#define YALVM_PRIu8  "hhu"
#define YALVM_PRIu16 "hu"
#define YALVM_PRIu32 "u"
#define YALVM_PRIu64 "llu"

#define YALVM_PRIf32 "f"
#define YALVM_PRIf64 "f"

#define YALVM_PRI_HEX8  "hhx"
#define YALVM_PRI_HEX16 "hx"
#define YALVM_PRI_HEX32 "x"
#define YALVM_PRI_HEX64 "llx"

#endif

#if defined YALVM_32BIT
#define YALVM_PRIsz  YALVM_PRIu32
#else
#define YALVM_PRIsz  YALVM_PRIu64
#endif




#endif

