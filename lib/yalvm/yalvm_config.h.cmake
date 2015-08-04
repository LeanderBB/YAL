#ifndef __YALVM_CONFIG_H__
#define __YALVM_CONFIG_H__

/* NOTE: Auto-generated file, do not modify */

/* NOTE: Do not include this file manually, always include yalvm.h instead */

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


#define YALVM_USE_CSTDLIB



#if defined(YALVM_USE_CSTDLIB)
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

#endif

#endif

