#include "yal/bytecode/bytecodeprinter.h"
#include "yal/bytecode/bytecodebuffer.h"
#include "yal/bytecode/bytecodebuilder.h"
#include <exception>
#include <sstream>
#include <iostream>
#include <iomanip>
namespace yal
{

ByteCodePrinter::ByteCodePrinter(InputSink& input,
                                 std::ostream &sink):
    _input(input),
    _sink(sink)
{

}

ByteCodePrinter::~ByteCodePrinter()
{

}

bool
ByteCodePrinter::process()
{

    // read binary header
    yalvm_bin_header_t bin_header;

    if (_input.read(&bin_header, sizeof(bin_header)) != sizeof(bin_header))
    {
        throw std::runtime_error("Could not read binary header");
    }

    if(!yalvm_bin_header_valid_magic(&bin_header))
    {
        throw std::runtime_error("Input is not a valid yalvm binary");
    }

    // print header info
    _sink << "# Constants 32: " << bin_header.n_constants32 << std::endl;
    _sink << "# Constants 64: " << bin_header.n_constants64 << std::endl;
    _sink << "# Globals   32: " << bin_header.n_globals32 << std::endl;
    _sink << "# Globals   64: " << bin_header.n_globals64 << std::endl;
    _sink << "# functions   : " << bin_header.n_functions << std::endl;
    _sink << "# strings     : " << bin_header.n_strings << std::endl;
    _sink << "# string size : " << bin_header.strings_size << std::endl;
    _sink << std::endl;

    // print strings

    yal_u32 string_offset = yalvm_bin_header_offset_strings(&bin_header);

    if (!_input.seekSet(string_offset))
    {
        throw std::runtime_error("Could not seek to string start");
    }

    for (yal_u32 i = 0; i < bin_header.n_strings; ++i)
    {
        yalvm_u32 size = 0;

        if (_input.read(&size, sizeof(size)) != sizeof(size))
        {
            throw std::runtime_error("Could not read string size");
        }

        char buffer[size + 1];

        if (_input.read(buffer, sizeof(buffer)) != sizeof(buffer))
        {
            throw std::runtime_error("Could not read string");
            return false;
        }

        _sink << "> String " << i << ": \"" << buffer << "\"" << std::endl;
    }

    // write global init code

    yal_u32 global_init_offset = yalvm_bin_header_offset_static_init(&bin_header);

    if (!_input.seekSet(global_init_offset))
    {
        throw std::runtime_error("Could not seek to global init start");
        return false;
    }

    yalvm_static_code_hdr_t static_hdr;
    if (_input.read(&static_hdr, sizeof(static_hdr))
            != sizeof(static_hdr))
    {
        throw std::runtime_error("Could not read static init header");
    }

    if (!yalvm_static_code_hdr_valid_magic(&static_hdr))
    {
        throw std::runtime_error("Input data is not a static int header");
    }

    _sink << std::endl << "> Global Init Code" << std::endl;
    print(static_hdr);


    // write global dtor code
    if (_input.read(&static_hdr, sizeof(static_hdr))
            != sizeof(static_hdr))
    {
        throw std::runtime_error("Could not read static dtor header");
    }

    if (!yalvm_static_code_hdr_valid_magic(&static_hdr))
    {
        throw std::runtime_error("Input data is not a static dtor header");
    }

    _sink << std::endl << "> Global Dtor Code" << std::endl;
    print(static_hdr);

    // write functions
    yal_u32 functions_offset = yalvm_bin_header_offset_functions(&bin_header);

    if (!_input.seekSet(functions_offset))
    {
        throw std::runtime_error("Could not seek to function start");
    }

    YAL_ASSERT(functions_offset == _input.tell());

    // process functions
    for(yal_u32 i = 0; i < bin_header.n_functions; ++i)
    {
        yalvm_func_header_t function_header;
        if (_input.read(&function_header, sizeof(function_header))
                != sizeof(function_header))
        {
            std::stringstream stream;
            stream << "Could not read function header (" << i << ")";
            throw std::runtime_error(stream.str());
        }

        if (!yalvm_func_header_valid_magic(&function_header))
        {
            std::stringstream stream;
            stream << "Input data is not a valid function header (" << i << ")";
            throw std::runtime_error(stream.str());
        }

        _sink << std::endl <<  "> Function " << i << ": ";
        if (function_header.name_len)
        {
            char function_name[function_header.name_len];

            if (_input.read(function_name, function_header.name_len)
                    != function_header.name_len)
            {
                return false;
            }
            _sink << function_name << std::endl;
        }
        else
        {
            _sink << "No name" << std::endl;
        }
        // log info


        _sink << ">   Arguments: " << (yal_u32) function_header.n_arguments << std::endl;
        _sink << ">   Registers: " << (yal_u32) function_header.n_registers << std::endl;
        _sink << ">   Native   : " << (function_header.flags & YALVM_FUNC_FLAG_NATIVE ? "Yes" : "No") << std::endl;


        if (!print(function_header))
        {
            return false;
        }
    }
    return true;
}

bool
ByteCodePrinter::print(const yalvm_func_header_t &function_header)
{
    if (function_header.flags & YALVM_FUNC_FLAG_NATIVE)
    {
        return true;
    }
    return print(function_header.code_size);
}

bool
ByteCodePrinter::print(const yalvm_static_code_hdr_t& static_hdr)
{
    return print(static_hdr.code_size);
}

bool
ByteCodePrinter::print(const size_t max)
{
    for (yal_u32 i = 0; i < max; ++i)
    {
        yalvm_bytecode_t code;
        if (_input.read(&code, sizeof(code)) != sizeof(code))
        {
            std::stringstream stream;
            stream << "Could not read function code (" << i << ")";
            throw std::runtime_error(stream.str());
        }

        const yalvm_bytecode_inst_t inst = yalvm_bytecode_unpack_instruction(code);

        const char* code_str = yalvm_bytecode_inst_to_str(inst);
        _sink << std::setfill('0');
        _sink  << "[" << std::setw(4) << i << "] "
               << std::setw(8) << std::setbase(16) << code << " "
               << std::setfill(' ') <<std::setbase(10) << std::setw(15) << code_str << ": ";
        //i, code, code_str);


        switch(inst)
        {
        /* End of execution */
        case YALVM_BYTECODE_HALT:
            _sink << std::endl;
            break;
            /* Load Global */
        case YALVM_BYTECODE_LOAD_GLOBAL_32:
        case YALVM_BYTECODE_LOAD_GLOBAL_64:
        case YALVM_BYTECODE_LOAD_GLOBAL_PTR:
            print2Argsu(code);
            break;
            /* Store Global */
        case YALVM_BYTECODE_STORE_GLOBAL_32:
        case YALVM_BYTECODE_STORE_GLOBAL_64:
        case YALVM_BYTECODE_STORE_GLOBAL_PTR:
            print2Argsu(code);
            break;
            /* Load value from memory */
        case YALVM_BYTECODE_LOAD_MEMORY:
            print3Args(code);
            break;
            /* Store value in memory */
        case YALVM_BYTECODE_STORE_MEMORY:
            print3Args(code);
            break;
            /* Load value from bytecode */
        case YALVM_BYTECODE_LOAD_VALUE:
            print2Argsu(code);
            break;
            /* Load constant  */
        case YALVM_BYTECODE_LOAD_CONST_32:
        case YALVM_BYTECODE_LOAD_CONST_64:
            print2Argsu(code);
            break;
            /* Summation instruction */
        case YALVM_BYTECODE_ADD_I:
        case YALVM_BYTECODE_ADD_IL:
        case YALVM_BYTECODE_ADD_U:
        case YALVM_BYTECODE_ADD_UL:
        case YALVM_BYTECODE_ADD_F:
        case YALVM_BYTECODE_ADD_FL:
            /* Substraction instruction */
        case YALVM_BYTECODE_SUB_I:
        case YALVM_BYTECODE_SUB_IL:
        case YALVM_BYTECODE_SUB_U:
        case YALVM_BYTECODE_SUB_UL:
        case YALVM_BYTECODE_SUB_F:
        case YALVM_BYTECODE_SUB_FL:
            /* Multiplication instruction */
        case YALVM_BYTECODE_MUL_I:
        case YALVM_BYTECODE_MUL_IL:
        case YALVM_BYTECODE_MUL_U:
        case YALVM_BYTECODE_MUL_UL:
        case YALVM_BYTECODE_MUL_F:
        case YALVM_BYTECODE_MUL_FL:
            /* Division instruction */
        case YALVM_BYTECODE_DIV_I:
        case YALVM_BYTECODE_DIV_IL:
        case YALVM_BYTECODE_DIV_U:
        case YALVM_BYTECODE_DIV_UL:
        case YALVM_BYTECODE_DIV_F:
        case YALVM_BYTECODE_DIV_FL:
            print3Args(code);
            break;
            /* Negative (-var) */
        case YALVM_BYTECODE_NEG_I:
        case YALVM_BYTECODE_NEG_IL:
        case YALVM_BYTECODE_NEG_F:
        case YALVM_BYTECODE_NEG_FL:
            print2Regs(code);
            break;
            /* Bit and */
        case YALVM_BYTECODE_BIT_AND_32:
        case YALVM_BYTECODE_BIT_AND_64:
            /* Bit or */
        case YALVM_BYTECODE_BIT_OR_32:
        case YALVM_BYTECODE_BIT_OR_64:
            /* Bit xor */
        case YALVM_BYTECODE_BIT_XOR_32:
        case YALVM_BYTECODE_BIT_XOR_64:
            print3Args(code);
            break;
            /* Bit not */
        case YALVM_BYTECODE_BIT_NOT_32:
        case YALVM_BYTECODE_BIT_NOT_64:
            print2Regs(code);
            break;
            /* Shift Left */
        case YALVM_BYTECODE_SHIFTL_32:
        case YALVM_BYTECODE_SHIFTL_64:
            /* Shift Right */
        case YALVM_BYTECODE_SHIFTR_32:
        case  YALVM_BYTECODE_SHIFTR_64:
            /* Mod */
        case YALVM_BYTECODE_MOD_I:
        case YALVM_BYTECODE_MOD_IL:
        case YALVM_BYTECODE_MOD_U:
        case YALVM_BYTECODE_MOD_UL:
            /* Logical And */
        case  YALVM_BYTECODE_AND:
            /* Logical Or */
        case YALVM_BYTECODE_OR:
            print3Args(code);
            break;
            /* Logical not */
        case YALVM_BYTECODE_NOT:
            print2Regs(code);
            break;
            /* Compare Greater */
        case YALVM_BYTECODE_COMPARE_GT_I:
        case YALVM_BYTECODE_COMPARE_GT_IL:
        case YALVM_BYTECODE_COMPARE_GT_U:
        case YALVM_BYTECODE_COMPARE_GT_UL:
        case YALVM_BYTECODE_COMPARE_GT_F:
        case YALVM_BYTECODE_COMPARE_GT_FL:
            /* Compare Greater or Equal */
        case YALVM_BYTECODE_COMPARE_GE_I:
        case YALVM_BYTECODE_COMPARE_GE_IL:
        case YALVM_BYTECODE_COMPARE_GE_U:
        case YALVM_BYTECODE_COMPARE_GE_UL:
        case YALVM_BYTECODE_COMPARE_GE_F:
        case YALVM_BYTECODE_COMPARE_GE_FL:
            /* Compare Lesser */
        case YALVM_BYTECODE_COMPARE_LT_I:
        case YALVM_BYTECODE_COMPARE_LT_IL:
        case YALVM_BYTECODE_COMPARE_LT_U:
        case YALVM_BYTECODE_COMPARE_LT_UL:
        case YALVM_BYTECODE_COMPARE_LT_F:
        case YALVM_BYTECODE_COMPARE_LT_FL:
            /* Compare Lesser or Equal */
        case YALVM_BYTECODE_COMPARE_LE_I:
        case YALVM_BYTECODE_COMPARE_LE_IL:
        case YALVM_BYTECODE_COMPARE_LE_U:
        case YALVM_BYTECODE_COMPARE_LE_UL:
        case YALVM_BYTECODE_COMPARE_LE_F:
        case YALVM_BYTECODE_COMPARE_LE_FL:
            /* Compare Equal */
        case YALVM_BYTECODE_COMPARE_EQ_I:
        case YALVM_BYTECODE_COMPARE_EQ_IL:
        case YALVM_BYTECODE_COMPARE_EQ_U:
        case YALVM_BYTECODE_COMPARE_EQ_UL:
        case YALVM_BYTECODE_COMPARE_EQ_F:
        case YALVM_BYTECODE_COMPARE_EQ_FL:
            /* Compare not Equal*/
        case YALVM_BYTECODE_COMPARE_NE_I:
        case YALVM_BYTECODE_COMPARE_NE_IL:
        case YALVM_BYTECODE_COMPARE_NE_U:
        case YALVM_BYTECODE_COMPARE_NE_UL:
        case YALVM_BYTECODE_COMPARE_NE_F:
        case YALVM_BYTECODE_COMPARE_NE_FL:
            print3Args(code);
            break;
            /* Uncoditional Jmp */
        case YALVM_BYTECODE_JUMP:
            print1Argsi(code);
            break;
            /* Conditional Jmp */
        case YALVM_BYTECODE_JUMP_TRUE:
        case YALVM_BYTECODE_JUMP_FALSE:
            print2Argsi(code);
            break;
            /* Load Function */
        case YALVM_BYTECODE_LOAD_FUNCTION:
            print2Argsu(code);
            break;
            /* Push Function Argument */
        case YALVM_BYTECODE_PUSH_ARG:
        case YALVM_BYTECODE_PUSH_REF:
            print1Reg(code);
            break;
            /* Call function */
        case YALVM_BYTECODE_CALL:
        case YALVM_BYTECODE_CALL_NATIVE:
            print2Regs(code);
            break;
            /* Return Instruction */
        case YALVM_BYTECODE_RETURN:
            print1Reg(code);
            break;
            /* Copy Register */
        case YALVM_BYTECODE_COPY_REGISTER:
            print2Regs(code);
            break;
            /* Load String */
        case YALVM_BYTECODE_LOAD_STRING:
            print2Argsu(code);
            break;
            /* Print instruction */
        case YALVM_BYTECODE_PRINT_I:
        case YALVM_BYTECODE_PRINT_IL:
        case YALVM_BYTECODE_PRINT_U:
        case YALVM_BYTECODE_PRINT_UL:
        case YALVM_BYTECODE_PRINT_F:
        case YALVM_BYTECODE_PRINT_FL:
        case YALVM_BYTECODE_PRINT_STR_CONSTANT:
            print1Reg(code);
            break;
        case YALVM_BYTECODE_PRINT_STR_OBJECT:
            print1Reg(code);
            break;
            /* Objects */
        case YALVM_BYTECODE_OBJECT_ALLOC:
            print2Argsu(code);
            break;
        case YALVM_BYTECODE_OBJECT_ACQUIRE:
            print1Reg(code);
            break;
        case YALVM_BYTECODE_OBJECT_RELEASE:
            print2Argsu(code);
            break;
            /* Strings Objects */
        case YALVM_BYTECODE_STRING_ALLOC:
            print2Argsu(code);
            break;
        case YALVM_BYTECODE_STRING_DEALLOC:
            print1Reg(code);
            break;
        case YALVM_BYTECODE_PRINT_NL:
            break;
            /* Load Reference */
        case YALVM_BYTECODE_LOAD_REF:
            print2Regs(code);
            break;
            /* Store Reference */
        case YALVM_BYTECODE_STORE_REF:
            print2Regs(code);
            break;
            /* arrary manip */
        case YALVM_BYTECODE_ARRAY_ALLOC_32:
        case YALVM_BYTECODE_ARRAY_ALLOC_64:
        case YALVM_BYTECODE_ARRAY_ALLOC_OBJ:
            print2Regs(code);
            break;
        case YALVM_BYTECODE_ARRAY_PUT_32:
        case YALVM_BYTECODE_ARRAY_PUT_64:
        case YALVM_BYTECODE_ARRAY_PUT_OBJ:
        case YALVM_BYTECODE_ARRAY_GET_32:
        case YALVM_BYTECODE_ARRAY_GET_64:
        case YALVM_BYTECODE_ARRAY_GET_OBJ:
            print3Args(code);
            break;
        case YALVM_BYTECODE_ARRAY_DEL_32:
        case YALVM_BYTECODE_ARRAY_DEL_64:
        case YALVM_BYTECODE_ARRAY_DEL_OBJ:
            print3Args(code);
            break;
        case YALVM_BYTECODE_ARRAY_LEN:
            print2Regs(code);
            break;
        case YALVM_BYTECODE_ARRAY_DEALLOC:
            print1Reg(code);
            break;
        default:
            _sink << "Unknown byte code ";
            return false;
        }
        _sink << std::endl;
    }
    return true;
}

void
ByteCodePrinter::print3Args(const yalvm_bytecode_t code)
{
    yal_u8 dst, src1, src2;
    yalvm_bytecode_unpack_registers(code, &dst, &src1, &src2);

    _sink << std::setfill('0');
    _sink << "r" << std::setw(3)  << (yal_u32) dst << ", "
          << "r" << std::setw(3)  << (yal_u32) src1 << ", "
          << "r" << std::setw(3)  << (yal_u32) src2;
}

void
ByteCodePrinter::print2Argsi(const yalvm_bytecode_t code)
{
    yal_u8 dst;
    yalvm_i16 val;
    yalvm_bytecode_unpack_dst_value_signed(code, &dst, &val);
    _sink << std::setfill('0');
    _sink << "r" << std::setw(3)  <<  (yal_u32) dst << ", "
          << (yal_i16) val;
}

void
ByteCodePrinter::print2Argsu(const yalvm_bytecode_t code)
{
    yal_u8 dst;
    yal_u16 val;
    yalvm_bytecode_unpack_dst_value(code, &dst, &val);
    _sink << std::setfill('0');
    _sink << "r" << std::setw(3)  <<  (yal_u32) dst << ", "
          << (yal_u16) val;
}

void
ByteCodePrinter::print1Argsu(const yalvm_bytecode_t code)
{
    yal_u32 val;
    yalvm_bytecode_unpack_value(code, &val);
    _sink << val;
}

void
ByteCodePrinter::print1Argsi(const yalvm_bytecode_t code)
{
    yal_i32 val;
    yalvm_bytecode_unpack_value_signed(code, &val);
    _sink << val;
}


void
ByteCodePrinter::print2Regs(const yalvm_bytecode_t code)
{
    yal_u8 dst, src1, src2;
    yalvm_bytecode_unpack_registers(code, &dst, &src1, &src2);

    _sink << std::setfill('0');
    _sink << "r" << std::setw(3)  <<  (yal_u32) dst << ", "
          << "r" << std::setw(3)  <<  (yal_u32) src1;
}

void
ByteCodePrinter::print1Reg(const yalvm_bytecode_t code)
{
    yal_u8 dst, src1, src2;
    yalvm_bytecode_unpack_registers(code, &dst, &src1, &src2);

    _sink << std::setfill('0');
    _sink << "r" << std::setw(3)  <<  (yal_u32) dst;
}

}

