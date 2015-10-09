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
 * [  obj descriptor 0 ]
 *       ...
 * [  obj descriptor N ]
 * [     string 0      ]
 *       ...
 * [     string N      ]
 * [ static create code]
 * [static destroy code]
 * [yalvm_func_header 0]
 * [ ?function 0 name  ]
 * [  function 0       ]
 *
 * [yalvm_func_header N]
 * [ ?function N name  ]
 * [  function N       ]
 * [yalvm_func_header N]
 * [yalvm_func_header g]
 * [  global function  ]
 */

enum yalvm_bin_enums
{
    YALVM_BIN_MAX_FUNCTION_CODE_SIZE    = 0xFFFF,
    YALVM_BIN_MAX_GLOBALS               = 0xFFFF,
    YALVM_BIN_MAX_CONSTANTS             = 0xFFFF,
    YALVM_BIN_MAX_FUNCTIONS             = 0xFFFF,
    YALVM_BIN_MAX_STRINGS               = 0xFFFF,
    YALVM_BIN_MAX_STRINGS_SIZE          = 0XFFFF
};

#define YAVLM_BIN_INDEX_INVALID 0xFFFFFFFF


/* binary header*/
#pragma pack(push, 1)
typedef struct
{
    yalvm_u32 magic;          /* magic number check                          */
    yalvm_u16 n_constants32;  /* number of 32bit constants                   */
    yalvm_u16 n_constants64;  /* number of 64bit constants                   */
    yalvm_u16 n_globals32;    /* number of 32bit globals                     */
    yalvm_u16 n_globals64;    /* number of 64bit globals                     */
    yalvm_u16 n_functions;    /* number of functions                         */
    yalvm_u16 n_objdescs;     /* number of object descs                      */
    yalvm_u16 n_strings;      /* number of strings                           */
    yalvm_u16 strings_size;   /* total size of all strings                   */
    yalvm_u16 n_globalsPtr;   /* number of Ptr globals                       */
    yalvm_u16 static_size;    /* size of the static init/destroy regions / 4 */
} yalvm_bin_header_t;
#pragma pack(pop)


yalvm_bool
yalvm_bin_header_valid_magic(const yalvm_bin_header_t* header);

yalvm_u32
yalvm_bin_header_offset_functions(const yalvm_bin_header_t* header);

yalvm_u32
yalvm_bin_header_offset_constant32(const yalvm_bin_header_t* header,
                                   const yalvm_u16 index);

yalvm_u32
yalvm_bin_header_offset_constant64(const yalvm_bin_header_t* header,
                                   const yalvm_u16 index);

yalvm_u32
yalvm_bin_header_offset_objdescs(const yalvm_bin_header_t* header,
                                 const yalvm_u16 index);

yalvm_u32
yalvm_bin_header_offset_strings(const yalvm_bin_header_t* header);

yalvm_u32
yalvm_bin_header_offset_static_init(const yalvm_bin_header_t* header);


void
yalvm_bin_header_init(yalvm_bin_header_t* header);


/* globals init header */
#pragma pack(push, 1)
typedef struct
{
    yalvm_u32 magic;          /* magic number check          */
    yalvm_u16 code_size;      /* code size  / 4              */
    yalvm_u16 n_registers;    /* number or registers         */
} yalvm_static_code_hdr_t;
#pragma pack(pop)

void
yalvm_static_code_hdr_init(yalvm_static_code_hdr_t* hdr);

yalvm_bool
yalvm_static_code_hdr_valid_magic(const yalvm_static_code_hdr_t* header);

/* Function header */
#pragma pack(push, 1)
typedef struct
{
    yalvm_u32 magic;          /* magic number check          */
    yalvm_u32 hash;           /* function id hash            */
    yalvm_u8  n_arguments;    /* number of arguments         */
    yalvm_u8  n_registers;    /* number or registers         */
    yalvm_u16 code_size;      /* function code size  / 4     */
    yalvm_u16 name_len;       /* name of the function symbol */
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


/* Object descriptor */
#pragma pack(push, 1)
typedef struct
{
    yalvm_u16 mem_size;     /* memory size               */
    yalvm_u16 ctor_idx;     /* constructor function idx  */
    yalvm_u16 dtor_idx;     /* destructor function idx   */
    yalvm_u16 __unused;     /* unused                    */
} yalvm_obj_descriptor_t;
#pragma pack(pop)


typedef struct yalvm_binary
{
    const yalvm_bin_header_t*       header;
    const yalvm_u8*                 binary;
    const yalvm_u8*                 binary_end;
    const yalvm_u32*                constants32;
    const yalvm_u64*                constants64;
    const char**                    strings;
    const yalvm_func_header_t**     functions;
    const yalvm_static_code_hdr_t*  global_init_code;
    const yalvm_static_code_hdr_t*  global_dtor_code;
} yalvm_binary_t;

void
yalvm_binary_init(yalvm_binary_t* binary);

void
yalvm_binary_destroy(yalvm_binary_t* binary);

yalvm_bool
yalvm_binary_load(yalvm_binary_t *binary,
                  const void* binary_data,
                  const yalvm_size binary_size);

yalvm_u32
yalvm_binary_num_globals32(const yalvm_binary_t* binary);

yalvm_u32
yalvm_binary_num_globals64(const yalvm_binary_t* binary);

yalvm_u32
yalvm_binary_num_globalsPtr(const yalvm_binary_t* binary);

yalvm_u32
yalvm_binary_num_functions(const yalvm_binary_t* binary);



YALVM_MODULE_END

#endif
