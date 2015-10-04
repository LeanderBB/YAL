#include "yal/bytecode/bytecodeprinter.h"
#include "yal/bytecode/bytecodebuffer.h"
#include "yal/bytecode/bytecodebuilder.h"

namespace yal
{

ByteCodePrinter::ByteCodePrinter(InputSink& input,
                                 OutputSink &sink):
    _input(input),
    _sink(sink),
    _formater()
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
        _formater.formatAndWrite(_sink, "Could not read binary header\n");
       return false;
    }

    if(!yalvm_bin_header_valid_magic(&bin_header))
    {
        _formater.formatAndWrite(_sink, "Input is not a valid yalvm binary\n");
       return false;
    }

    // print header info
    _formater.formatAndWrite(_sink, "# Constants 32: %u\n",
                             bin_header.n_constants32);
    _formater.formatAndWrite(_sink, "# Constants 64: %u\n",
                             bin_header.n_constants64);
    _formater.formatAndWrite(_sink, "# Globals   32: %u\n",
                             bin_header.n_globals32);
    _formater.formatAndWrite(_sink, "# Globals   64: %u\n",
                             bin_header.n_globals64);
    _formater.formatAndWrite(_sink, "# functions   : %u\n",
                             bin_header.n_functions);
    _formater.formatAndWrite(_sink, "# strings     : %u\n",
                             bin_header.n_strings);
    _formater.formatAndWrite(_sink, "# string size : %u\n",
                             bin_header.strings_size);
    _formater.formatAndWrite(_sink, "\n");

    // print strings

    yal_u32 string_offset = yalvm_bin_header_offset_strings(&bin_header);

    if (!_input.seekSet(string_offset))
    {
        _formater.formatAndWrite(_sink, "Could not seek to string start\n");
        return false;
    }

    for (yal_u32 i = 0; i < bin_header.n_strings; ++i)
    {
        yalvm_u32 size = 0;

        if (_input.read(&size, sizeof(size)) != sizeof(size))
        {
            _formater.formatAndWrite(_sink, "Could not read string size\n");
            return false;
        }

        char buffer[size + 1];

        if (_input.read(buffer, sizeof(buffer)) != sizeof(buffer))
        {
            _formater.formatAndWrite(_sink, "Could not read string\n");
            return false;
        }

        _formater.formatAndWrite(_sink, "> String %u: \"%s\"\n", i, buffer);
    }

    // write functions
    yal_u32 functions_offset = yalvm_bin_header_offset_functions(&bin_header);

    if (!_input.seekSet(functions_offset))
    {
        _formater.formatAndWrite(_sink, "Could not seek to function start\n");
        return false;
    }

    YAL_ASSERT(functions_offset == _input.tell());

    // process functions
    for(yal_u32 i = 0; i < bin_header.n_functions; ++i)
    {
        yalvm_func_header_t function_header;
        if (_input.read(&function_header, sizeof(function_header))
                         != sizeof(function_header))
        {
            _formater.formatAndWrite(_sink, "Could not read function header (%u)\n",
                                     i);
            return false;
        }

        if (!yalvm_func_header_valid_magic(&function_header))
        {
            _formater.formatAndWrite(_sink, "Input data is not a valid function header (%u)\n",
                                     i);
            return false;
        }
        // log info
        _formater.formatAndWrite(_sink, "> Function %u:%x\n", i,
                                 function_header.hash);
        _formater.formatAndWrite(_sink, ">   Arguments: %u\n",
                                 function_header.n_arguments);
        _formater.formatAndWrite(_sink, ">   Registers: %u\n",
                                 function_header.n_registers);
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
    const yal_u32 buffer_size = function_header.code_size;

    for (yal_u32 i = 0; i < buffer_size; ++i)
    {
        yalvm_bytecode_t code;
        if (_input.read(&code, sizeof(code)) != sizeof(code))
        {
             _formater.formatAndWrite(_sink, "Could not read function code %u\n", i);
            return false;
        }

        const yalvm_bytecode_inst_t inst = yalvm_bytecode_unpack_instruction(code);

        const char* code_str = yalvm_bytecode_inst_to_str(inst);
        _formater.formatAndWrite(_sink, "[%04u] %08X %15s:", i, code, code_str);


        switch(inst)
        {
        /* End of execution */
        case YALVM_BYTECODE_HALT:
            _formater.formatAndWrite(_sink, "\n");
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
            print1Reg(code);
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
            print1Reg(code);
            break;
        /* Shift Left */
        case YALVM_BYTECODE_SHIFTL_32:
        case YALVM_BYTECODE_SHIFTL_64:
        /* Shift Right */
        case YALVM_BYTECODE_SHIFTR_32:
        case  YALVM_BYTECODE_SHIFTR_64:
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
            print1Reg(code);
            break;
        /* Call function */
        case YALVM_BYTECODE_CALL:
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
        case YALVM_BYTECODE_PRINT_STR:
            print1Reg(code);
            break;
        case YALVM_BYTECODE_PRINT_NL:
           printf("\n");
           break;
        default:
            printf("Uknow byte code \n");
            return false;
        }
    }
    return true;
}

void
ByteCodePrinter::print3Args(const yalvm_bytecode_t code)
{
    yal_u8 dst, src1, src2;
    yalvm_bytecode_unpack_registers(code, &dst, &src1, &src2);
    _formater.formatAndWrite(_sink," r%03u, r%03u, r%03u\n", dst, src1, src2);
}

void
ByteCodePrinter::print2Argsi(const yalvm_bytecode_t code)
{
    yal_u8 dst;
    yalvm_i16 val;
    yalvm_bytecode_unpack_dst_value_signed(code, &dst, &val);
    _formater.formatAndWrite(_sink," r%03u, %i\n", dst, val);
}

void
ByteCodePrinter::print2Argsu(const yalvm_bytecode_t code)
{
    yal_u8 dst;
    yal_u16 val;
    yalvm_bytecode_unpack_dst_value(code, &dst, &val);
    _formater.formatAndWrite(_sink," r%03u, %u\n", dst, val);
}

void
ByteCodePrinter::print1Argsu(const yalvm_bytecode_t code)
{
    yal_u32 val;
    yalvm_bytecode_unpack_value(code, &val);
    _formater.formatAndWrite(_sink," %u\n",val);
}

void
ByteCodePrinter::print1Argsi(const yalvm_bytecode_t code)
{
    yal_i32 val;
    yalvm_bytecode_unpack_value_signed(code, &val);
    _formater.formatAndWrite(_sink," %d\n",val);
}


void
ByteCodePrinter::print2Regs(const yalvm_bytecode_t code)
{
    yal_u8 dst, src1, src2;
    yalvm_bytecode_unpack_registers(code, &dst, &src1, &src2);
    _formater.formatAndWrite(_sink," r%03u, r%03u\n", dst, src1);
}

void
ByteCodePrinter::print1Reg(const yalvm_bytecode_t code)
{
    yal_u8 dst, src1, src2;
    yalvm_bytecode_unpack_registers(code, &dst, &src1, &src2);
    _formater.formatAndWrite(_sink," r%03u\n", dst);
}

}

