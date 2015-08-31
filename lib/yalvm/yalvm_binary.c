#include "yalvm/yalvm_binary.h"
#include "yalvm/yalvm_hashing.h"

static const yalvm_u32 yalvm_bin_header_magic  = 0x10e5af18;
static const yalvm_u32 yalvm_func_header_magic = 0x04974ceb;

yalvm_bool
yalvm_bin_header_valid_magic(const yalvm_bin_header_t* header)
{
    return header->magic == yalvm_bin_header_magic;
}

yalvm_u32
yalvm_bin_header_offset_functions(const yalvm_bin_header_t* header)
{
    yalvm_u32 offset = yalvm_bin_header_offset_strings(header);
    offset += header->strings_size;
    return offset;
}

yalvm_u32
yalvm_bin_header_offset_global32(const yalvm_bin_header_t* header,
                          const yalvm_u16 index)
{
     yalvm_u32 offset = yalvm_bin_header_offset_constant64(header, header->n_constants64);
    return (index <= header->n_globals32)
            ? offset + (sizeof(yalvm_bin_global32_t) * index)
            : YAVLM_BIN_INDEX_INVALID;
}

yalvm_u32
yalvm_bin_header_offset_global64(const yalvm_bin_header_t* header,
                          const yalvm_u16 index)
{
    yalvm_u32 offset = yalvm_bin_header_offset_global32(header, header->n_globals32);
    if (index <= header->n_globals64)
    {
        return offset + (sizeof(yalvm_bin_global64_t) * index);
    }
    else
    {
        return YAVLM_BIN_INDEX_INVALID;
    }
}

yalvm_u32
yalvm_bin_header_offset_constant32(const yalvm_bin_header_t* header,
                            const yalvm_u16 index)
{
    yalvm_u32 offset = sizeof(yalvm_bin_header_t);
    if (index <= header->n_constants32)
    {
        return offset + (sizeof(yalvm_u32) * index);
    }
    else
    {
        return YAVLM_BIN_INDEX_INVALID;
    }
}

yalvm_u32
yalvm_bin_header_offset_constant64(const yalvm_bin_header_t* header,
                            const yalvm_u16 index)
{
    yalvm_u32 offset = yalvm_bin_header_offset_constant32(header, header->n_constants32);
    if (index <= header->n_constants64)
    {
        return offset + (sizeof(yalvm_u64) * index);
    }
    else
    {
        return YAVLM_BIN_INDEX_INVALID;
    }
}

yalvm_u32
yalvm_bin_header_offset_objdescs(const yalvm_bin_header_t* header,
                                 const yalvm_u16 index)
{
    yalvm_u32 offset = yalvm_bin_header_offset_global64(header, header->n_globals64);
    if (index <= header->n_objdescs)
    {
        return offset + (sizeof(yalvm_obj_descriptor_t) * index);
    }
    else
    {
        return YAVLM_BIN_INDEX_INVALID;
    }
}

yalvm_u32
yalvm_bin_header_offset_strings(const yalvm_bin_header_t* header)
{
    return yalvm_bin_header_offset_objdescs(header, header->n_objdescs);
}


void
yalvm_bin_header_init(yalvm_bin_header_t* header)
{
    header->magic = yalvm_bin_header_magic;
    header->n_objdescs = 0;
    header->n_constants32 = 0;
    header->n_constants64 = 0;
    header->n_functions =0;
    header->n_globals32 = 0;
    header->n_globals64 = 0;
    header->n_strings = 0;
    header->strings_size = 0;
}

void
yalvm_func_header_init(yalvm_func_header_t* header,
                       const char* function_name)
{
    header->magic = yalvm_func_header_magic;
    header->hash = yalvm_one_at_a_time_hash(function_name);
    header->code_size = 0;
    header->n_arguments = 0;
    header->n_registers = 0;
}

yalvm_bool
yalvm_func_header_valid_magic(const yalvm_func_header_t* header)
{
    return header->magic == yalvm_func_header_magic;
}

const char*
yalvm_func_global_name()
{
    static const char* name = "globalfunction";
    return name;
}

yalvm_u32
yalvm_func_global_hash()
{
    const yalvm_u32 hash = yalvm_one_at_a_time_hash(yalvm_func_global_name());
    return hash;
}


