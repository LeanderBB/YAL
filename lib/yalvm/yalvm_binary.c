#include "yalvm/yalvm_binary.h"
#include "yalvm/yalvm_hashing.h"
#include "yalvm/yalvm_bytecode.h"
#include "yalvm/yalvm_external.h"

#include <dlfcn.h>

static const yalvm_u32 yalvm_bin_header_magic  = 0x10e5af18;
static const yalvm_u32 yalvm_func_header_magic = 0x04974ceb;
static const yalvm_u32 yalvm_static_hader_magic = 0xe351dac4;


static const void*
yalvm_bin_load_native_function(const char* name)
{
    return dlsym(RTLD_DEFAULT, name);
}

yalvm_bool
yalvm_bin_header_valid_magic(const yalvm_bin_header_t* header)
{
    return header->magic == yalvm_bin_header_magic;
}

yalvm_u32
yalvm_bin_header_offset_functions(const yalvm_bin_header_t* header)
{
    yalvm_u32 offset = yalvm_bin_header_offset_static_init(header);
    offset += (header->static_size * 4);
    return offset;
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
    yalvm_u32 offset = yalvm_bin_header_offset_constant64(header, header->n_constants64);
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

yalvm_u32
yalvm_bin_header_offset_static_init(const yalvm_bin_header_t* header)
{
    yalvm_u32 offset = yalvm_bin_header_offset_strings(header);
    offset += header->strings_size;
    return offset;
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
    header->static_size = 0;
}

void
yalvm_static_code_hdr_init(yalvm_static_code_hdr_t* hdr)
{
    hdr->magic = yalvm_static_hader_magic;
    hdr->code_size = 0;
    hdr->n_registers = 0;
}

yalvm_bool
yalvm_static_code_hdr_valid_magic(const yalvm_static_code_hdr_t* header)
{
    return header->magic == yalvm_static_hader_magic;
}


void
yalvm_func_header_init(yalvm_func_header_t* header)
{
    header->magic = yalvm_func_header_magic;
    header->code_size = 0;
    header->n_arguments = 0;
    header->n_registers = 0;
    header->flags = 0;
    header->name_len = 0;
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


const char*
yalvm_func_header_name(const yalvm_func_header_t* header)
{
    return header->name_len ? (const char*) (++header) : NULL;
}

const void*
yalvm_func_header_code(const yalvm_func_header_t* header)
{
    return YALVM_PTR_ADD(header, sizeof(yalvm_func_header_t) + header->name_len);
}

yalvm_size
yalvm_func_header_total_size(const yalvm_func_header_t* header)
{
    return sizeof(yalvm_func_header_t)  + header->name_len
            + (header->code_size *  sizeof(yalvm_bytecode_t));
}

void
yalvm_binary_init(yalvm_binary_t* binary)
{
    memset(binary, 0, sizeof(yalvm_binary_t));
}

void
yalvm_binary_destroy(yalvm_binary_t* binary)
{
    if (binary->strings)
    {
        YALVM_SAFE_FREE(binary->strings);
    }

    if (binary->functions)
    {
        YALVM_SAFE_FREE(binary->functions);
    }
}

yalvm_bool
yalvm_binary_load(yalvm_binary_t* binary,
                  const void* binary_data,
                  const yalvm_size binary_size)
{
    const yalvm_u8* input = (const yalvm_u8*) binary_data;
    const yalvm_u8* input_end = input + binary_size;

    /* validate input */
    const yalvm_bin_header_t* bin_header = (yalvm_bin_header_t*) input;

    if (!yalvm_bin_header_valid_magic(bin_header))
    {
        return yalvm_false;
    }

    /* setup input ptrs */
    binary->header = bin_header;
    binary->binary = binary_data;
    binary->binary_end = input_end;

    binary->constants32 = (yalvm_u32*) (input
                                        + yalvm_bin_header_offset_constant32(bin_header, 0));

    binary->constants64 = (yalvm_u64*) (input
                                        + yalvm_bin_header_offset_constant64(bin_header, 0));


    const void* string_offset = (input + yalvm_bin_header_offset_strings(bin_header));
    /* cache string locations */
    if (bin_header->n_strings)
    {
        binary->strings = (const char**)yalvm_malloc(sizeof(char*) * bin_header->n_strings);
        size_t offset = 0;
        for (yalvm_u16 i = 0; i < bin_header->n_strings; ++i)
        {
            const void* cur_ptr = YALVM_PTR_ADD(string_offset, offset);
            const yalvm_u32* string_size = (const yalvm_u32*) cur_ptr;
            offset += sizeof(yalvm_u32);

            binary->strings[i] = (const char*) YALVM_PTR_ADD(string_offset, offset);
            offset += (*string_size) + 1;
        }
    }

    const yalvm_u32 global_init_offset = yalvm_bin_header_offset_static_init(bin_header);
    binary->global_init_code = (yalvm_static_code_hdr_t*) YALVM_PTR_ADD(input, global_init_offset);

    if (!yalvm_static_code_hdr_valid_magic(binary->global_init_code))
    {
        yalvm_binary_destroy(binary);
        return yalvm_false;
    }

    const yalvm_u32 global_dtor_offset = global_init_offset
            + sizeof(yalvm_static_code_hdr_t)
            + binary->global_init_code->code_size * 4;

    binary->global_dtor_code = (yalvm_static_code_hdr_t*) YALVM_PTR_ADD(input, global_dtor_offset);

    if (!yalvm_static_code_hdr_valid_magic(binary->global_dtor_code))
    {
        yalvm_binary_destroy(binary);
        return yalvm_false;
    }

    /* process functions */
    binary->functions = (yalvm_binary_function_t*)
            yalvm_malloc(sizeof(yalvm_binary_function_t) * bin_header->n_functions + 1);

    input += yalvm_bin_header_offset_functions(bin_header);

    for(yalvm_u32 i = 0; i < bin_header->n_functions; ++i)
    {
        yalvm_func_header_t* function_header = (yalvm_func_header_t*)input;

        if (input >= input_end || !yalvm_func_header_valid_magic(function_header))
        {
            yalvm_binary_destroy(binary);
            return yalvm_false;
        }

        binary->functions[i].hdr = function_header;
        binary->functions[i].code = function_header->flags & YALVM_FUNC_FLAG_NATIVE
                ? yalvm_bin_load_native_function(yalvm_func_header_name(function_header))
                : yalvm_func_header_code(function_header);

        if (binary->functions[i].code == NULL)
        {
            yalvm_binary_destroy(binary);
            return yalvm_false;
        }

        input += yalvm_func_header_total_size(function_header);
    }

    return yalvm_true;
}

yalvm_u32
yalvm_binary_num_globals32(const yalvm_binary_t* binary)
{
    return binary->header->n_globals32;
}

yalvm_u32
yalvm_binary_num_globals64(const yalvm_binary_t* binary)
{
    return binary->header->n_globals64;
}

yalvm_u32
yalvm_binary_num_globalsPtr(const yalvm_binary_t* binary)
{
    return binary->header->n_globalsPtr;
}

yalvm_u32
yalvm_binary_num_functions(const yalvm_binary_t* binary)
{
    return binary->header->n_functions;
}


