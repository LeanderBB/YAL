#ifndef __YALVM_H__
#define __YALVM_H__

#if defined(__cplusplus)
#define YALVM_MODULE_BGN extern "C" {
#define YALVM_MODULE_END }
#else
#define YALVM_MODULE_BGN
#define YALVM_MODULE_END
#endif

#include "yalvm/yalvm_config.h"
#include "yalvm/yalvm_external.h"

YALVM_MODULE_BGN

/* Type Definition ----------------------------------------------------------*/
typedef yalvm_i8   yalvm_bool;
#define yalvm_true ((yalvm_bool)1)
#define yalvm_false ((yalvm_bool)0)

/* Debug Definitions---------------------------------------------------------*/

#if defined(YAL_DEBUG) || defined(YALVM_DEBUG)
#include <assert.h>
#define YALVM_ASSERT(x) assert(x)
#else
#define YALVM_ASSERT(x)
#endif

#define YALVM_INLINE static inline

#if defined(__clang__)
#define YALVM_STATIC_ASSERT(exp, msg)
#else
#define YALVM_STATIC_ASSERT(exp, msg) static_assert(exp, msg);
#endif

/* Utils -------------------------------------------------------------------*/

#define YALVM_PTR_ADD(ptr, value) ((yalvm_u8*)ptr) + value
#define YALVM_ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define YALVM_SAFE_FREE(x)  free(x); x = NULL
YALVM_MODULE_END

#endif

