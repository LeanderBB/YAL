#ifndef __YALVM_BINARY_H__
#define __YALVM_BINARY_H__

#include "yalvm/yalvm.h"

YALVM_MODULE_BGN

/*
 * the yalvm binary must be organizad in the following manner
 *
 * [ yalvmn_bin_header ]
 * [  constant 32 0    ]
 *       ...
 * [  constant 32 N    ]
 * [  constant 64 0    ]
 *       ...
 * [  constant 64 N    ]
 * [  globals  32 0    ]
 *       ...
 * [  globals  32 N    ]
 * [  globals  64 0    ]
 *       ...
 * [  globals  64 N    ]
 * [  globals  64 0    ]
 *       ...
 * [yalvm_func_header 0]
 * [  function 0       ]
 *
 * [yalvm_func_header N]
 * [  function N       ]
 * [yalvm_func_header N]
 * [yalvm_func_header g]
 * [  global function  ]
 */

enum yalvm_bin_enums
{
    YALVM_BIN_SIZEOF_CONSTANT_32 = 4,
    YALVM_BIN_SIZEOF_CONSTANT_64 = 8,
    YALVM_BIN_SIZEOF_GLOBAL_32 = 8,
    YALVM_BIN_SIZEOF_GLOBAL_64 = 12,
    YALVM_BIN_MAX_FUNCTION_CODE_SIZE = 0xFFFF,
    YALVM_BIN_MAX_GLOBALS = 0xFFFF,
    YALVM_BIN_MAX_CONSTANTS = 0xFFFF,
    YALVM_BIN_MAX_FUNCTIONS = 0xFFFF,
};

#define YAVLM_BIN_INDEX_INVALID 0xFFFFFFFF


/* binary header*/
#pragma pack(push, 1)
typedef struct
{
    yalvm_u32 magic;          /* magic number check        */
    yalvm_u16 n_constants32;  /* number of 32bit constants */
    yalvm_u16 n_constants64;  /* number of 64bit constants */
    yalvm_u16 n_globals32;    /* number of 32bit globals   */
    yalvm_u16 n_globals64;    /* number of 64bit globals   */
    yalvm_u16 n_functions;    /* number of functions       */
    yalvm_u16 flags;          /* unused                    */
} yalvm_bin_header_t;
#pragma pack(pop)


yalvm_bool
yalvm_bin_header_valid_magic(const yalvm_bin_header_t* header);

yalvm_u32
yalvm_bin_header_offset_functions(const yalvm_bin_header_t* header);

yalvm_u32
yalvm_bin_header_offset_global32(const yalvm_bin_header_t* header,
                                 const yalvm_u16 index);

yalvm_u32
yalvm_bin_header_offset_global64(const yalvm_bin_header_t* header,
                                 const yalvm_u16 index);

yalvm_u32
yalvm_bin_header_offset_constant32(const yalvm_bin_header_t* header,
                                   const yalvm_u16 index);

yalvm_u32
yalvm_bin_header_offset_constant64(const yalvm_bin_header_t* header,
                                   const yalvm_u16 index);

void
yalvm_bin_header_init(yalvm_bin_header_t* header);

/* Global 32 */
#pragma pack(push, 1)
typedef struct
{
    yalvm_u32 hash;
    yalvm_u32 val;
} yalvm_bin_global32_t;
#pragma pack(pop)

// YALVM_STATIC_ASSERT((sizeof(yalvm_bin_global32_t) == YALVM_BIN_SIZEOF_GLOBAL_32),
//                    "yalvm_bin_global32_t does not mach enum size")

/* Global 64 */
#pragma pack(push, 1)
typedef struct
{
    yalvm_u32 hash;
    yalvm_u64 val;
} yalvm_bin_global64_t;
#pragma pack(pop)


// YALVM_STATIC_ASSERT(sizeof(yalvm_bin_global64_t) == YALVM_BIN_SIZEOF_GLOBAL_64,
//                    "yalvm_bin_global64_t does not mach enum size")

/* Function header */
#pragma pack(push, 1)
typedef struct
{
    yalvm_u32 magic;          /* magic number check        */
    yalvm_u32 hash;           /* function id hash */
    yalvm_u8  n_arguments;    /* number of arguments       */
    yalvm_u8  n_registers;    /* number or registers       */
    yalvm_u16 code_size;      /* function code size  / 4   */
} yalvm_func_header_t;
#pragma pack(pop)


void
yalvm_func_header_init(yalvm_func_header_t* header,
                       const char* function_name);

yalvm_bool
yalvm_func_header_valid_magic(const yalvm_func_header_t* header);

const char*
yalvm_func_global_name();

yalvm_u32
yalvm_func_global_hash();


YALVM_MODULE_END

#endif
