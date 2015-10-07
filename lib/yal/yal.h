#ifndef __YAL_YAL_H__
#define __YAL_YAL_H__

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "yal/config.h"



/* Type Definition ----------------------------------------------------------*/

typedef int8_t   yal_i8;
typedef uint8_t  yal_u8;
typedef int16_t  yal_i16;
typedef uint16_t yal_u16;
typedef int32_t  yal_i32;
typedef uint32_t yal_u32;
typedef int64_t  yal_i64;
typedef uint64_t yal_u64;
typedef float    yal_f32;
typedef double   yal_f64;

#ifdef __cplusplus
typedef bool yal_bool;
#define yal_true (true)
#define yal_false (false)
#else
typedef yal_i8   yal_bool;
#define yal_true ((yal_bool)1)
#define yal_false ((yal_bool)0)
#endif

/* Debug Definitions---------------------------------------------------------*/

#if defined(YAL_DEBUG)
#include <assert.h>
#define YAL_ASSERT(x) assert(x)
#else
#define YAL_ASSERT(x)
#endif

/* Config -------------------------------------------------------------------*/

enum
{
    kMaxRegisters = 250,
    kMaxFunctionArgs = 16,
    kValue24UnsignedMax = 0xFFFFFF,
    kValue24SignedMax = 0x7FFFFF,
    kValue24SignedMin = 0x800001
};


/* Other --------------------------------------------------------------------*/

#define YAL_ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#endif
