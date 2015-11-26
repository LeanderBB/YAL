#include "yal/bytecode/bytecodebuilder.h"
#include "yal/symbols/symboltreebuilder.h"
#include "yal/parser/parser_state.h"
#include "yal/ast/functionnode.h"
#include "yal/bytecode/bytecodegenerator.h"
#include "yal/bytecode/bytecodeprinter.h"
#include "yal/ast/variabledeclnode.h"
#include "yal/bytecode/bytecodegenerator.h"
#include <yalvm/yalvm_binary.h>
#include <yalvm/yalvm_hashing.h>
#include "yal/ast/asthdrs.h"
#include "yal/types/typehdrs.h"
namespace yal
{

ByteCodeBuilder::ByteCodeBuilder(OutputSink &codeOutput):
    _codeOutput(codeOutput)
{

}

ByteCodeBuilder::~ByteCodeBuilder()
{

}

bool
ByteCodeBuilder::process(ParserState_t& state)
{
    return writeModuleInfo(state);
}

bool
ByteCodeBuilder::writeModuleInfo(ParserState& state)
{
    Module& module = state.module;

    const Module::OrderedConstantVec_t& constants32_vec = module.constants32();
    const Module::OrderedConstantVec_t& constants64_vec = module.constants64();

    const Module::OrderedGlobalVec_t& globals32_vec = module.globals32();
    const Module::OrderedGlobalVec_t& globals64_vec = module.globals64();
    const Module::OrderedGlobalVec_t& globalsPtr_vec = module.globalsPtr();

    const Module::OrderedFunctionVec_t& function_vec = module.functions();

    const Module::OrderedStringsVec_t& strings_vec = module.strings();

    // validate constant count
    if (constants32_vec.size() >= YALVM_BIN_MAX_CONSTANTS)
    {
        //_errorHandler.onError("Constant 32bit count exceeds maximum value", 0);
        return false;
    }

    if (constants64_vec.size() >= YALVM_BIN_MAX_CONSTANTS)
    {
        //_errorHandler.onError("Constant 64bit count exceeds maximum value", 0);
        return false;
    }


    // validate global count
    if (globals32_vec.size() >= YALVM_BIN_MAX_GLOBALS)
    {
        //_errorHandler.onError("Global 32bit count exceeds maximum value", 0);
        return false;
    }

    if (globals64_vec.size() >= YALVM_BIN_MAX_GLOBALS)
    {
        //_errorHandler.onError("Global 64bit count exceeds maximum value", 0);
        return false;
    }

    // validate function count
    if (function_vec.size() >= YALVM_BIN_MAX_FUNCTIONS)
    {
        //_errorHandler.onError("Function count exceeds maximum value", 0);
        return false;
    }

    if (strings_vec.size() >= YALVM_BIN_MAX_STRINGS)
    {
        //_errorHandler.onError("String count exceeds maximum value", 0);
        return false;
    }

    if (module.totalStringSizeBytes() >= YALVM_BIN_MAX_STRINGS_SIZE)
    {
        //_errorHandler.onError("Exceeded maximum string storage capacity", 0);
        return false;
    }

    // generate global and init code


    ByteCodeGenerator static_init_code(state.module, state.registry);
    ByteCodeGenerator static_destroy_code(state.module, state.registry);

    ByteCodeGenerator global_function(state.module, state.registry);
    static_init_code.onScopeBeginGlobal(state.symbolTree.globalScope());
    static_destroy_code.onScopeEndGlobal(state.symbolTree.globalScope());
    for (auto& v : state.program)
    {
        // if symbol is not a function definition add it to the
        // global function scope
        if (!ast_typeof<FunctionDeclNode>(v)
                && !ast_typeof<FunctionDeclNativeNode>(v))
        {

            ByteCodeGenerator& generator = (ast_typeof<VariableDeclNode>(v))
                    ? static_init_code
                    : global_function;
            if (!generator.generate(*v))
            {
                return false;
            }
        }
        // otherwise generate the bytecode for the function
    }

    // setup static code headers
    yalvm_static_code_hdr_t static_init_hdr, static_dtr_hdr;

    yalvm_static_code_hdr_init(&static_init_hdr);
    yalvm_static_code_hdr_init(&static_dtr_hdr);

    static_init_hdr.code_size = (static_init_code.buffer().size()
                                 ? static_cast<yalvm_u16>(static_init_code.buffer().size() + 1)
                                 : 0);
    static_init_hdr.n_registers = static_init_code.maxRegisterCount();
    static_dtr_hdr.code_size = (static_destroy_code.buffer().size()
                                ? static_cast<yalvm_u16>(static_destroy_code.buffer().size() + 1)
                                : 0);
    static_dtr_hdr.n_registers = static_destroy_code.maxRegisterCount();


    // setup header
    yalvm_bin_header_t header;
    yalvm_bin_header_init(&header);
    header.n_constants32 = static_cast<yalvm_u16>(constants32_vec.size());
    header.n_constants64 = static_cast<yalvm_u16>(constants64_vec.size());
    header.n_globals32 = static_cast<yalvm_u16>(globals32_vec.size());
    header.n_globals64 = static_cast<yalvm_u16>(globals64_vec.size());
    header.n_globalsPtr = static_cast<yalvm_u16>(globalsPtr_vec.size());
    header.n_functions = static_cast<yalvm_u16>(function_vec.size() + 1); // +1 for global code
    header.n_strings = static_cast<yalvm_u16>(strings_vec.size());
    header.strings_size = static_cast<yalvm_u16>(module.totalStringSizeBytes());
    header.static_size = static_cast<yalvm_u32>((sizeof(static_init_hdr) * 2 / sizeof(yalvm_bytecode_t))
        + static_init_hdr.code_size
        + static_dtr_hdr.code_size);


    // write header
    if (_codeOutput.write(&header, sizeof(header)) != sizeof(header))
    {
        //_errorHandler.onError("Could not write header to code output", 0);
        return false;
    }

    // write 32bit constants

    for (auto& constant : constants32_vec)
    {
        const yal_u32 val = constant->value().valueAs32BitRaw();
        if (_codeOutput.write(&val, sizeof(val)) != sizeof(val))
        {
            //_errorHandler.onError("Could not write constant 32 to code output", 0);
            return false;
        }
    }

    // write 64bit constants
    for (auto& constant : constants64_vec)
    {
        const yal_u64 val = constant->value().valueAs64BitRaw();
        if (_codeOutput.write(&val, sizeof(val)) != sizeof(val))
        {
            //_errorHandler.onError("Could not write constant 64 to code output", 0);
            return false;
        }
    }


    // write strings
    for (auto& string: strings_vec)
    {
        const char* text = string->value().valueAsText();
        yalvm_u32 string_len = static_cast<yalvm_u32>(strlen(text));

        if (_codeOutput.write(&string_len, sizeof(string_len)) != sizeof(string_len))
        {
            //_errorHandler.onError("Could not write string size to code output", 0);
            return false;
        }

        if (_codeOutput.write(text, string_len + 1) != string_len + 1)
        {
            //_errorHandler.onError("Could not write string to code output", 0);
            return false;
        }
    }


    // write static init code

    if (static_init_code.buffer().size())
    {
        static_init_code.addHaltInst();
        const size_t function_code_size = static_init_code.buffer().size();
        if (function_code_size >= YALVM_BIN_MAX_FUNCTION_CODE_SIZE)
        {
            //_errorHandler.onError("Static init code sizer greater than maxium value", 0);
            return false;
        }
    }

    if (_codeOutput.write(&static_init_hdr, sizeof(static_init_hdr))
            != sizeof(static_init_hdr))
    {
        //_errorHandler.onError("Could not write static init header to code output", 0);
        return false;
    }

    if (static_init_hdr.code_size)
    {
        const size_t real_size = static_init_hdr.code_size * sizeof(yalvm_bytecode_t);
        if (_codeOutput.write(static_init_code.buffer().buffer(), real_size)
                != real_size)
        {
            // _errorHandler.onError("Could not write static init code to code output", 0);
            return false;
        }
    }

    // write static destroy code
    if (static_destroy_code.buffer().size())
    {
        static_destroy_code.addHaltInst();
        const size_t function_code_size = static_destroy_code.buffer().size();
        if (function_code_size >= YALVM_BIN_MAX_FUNCTION_CODE_SIZE)
        {
            //_errorHandler.onError("Static init code sizer greater than maxium value", 0);
            return false;
        }
    }

    if (_codeOutput.write(&static_dtr_hdr, sizeof(static_dtr_hdr))
            != sizeof(static_dtr_hdr))
    {
        //_errorHandler.onError("Could not write static destroy header to code output", 0);
        return false;
    }

    if (static_dtr_hdr.code_size)
    {
        const size_t real_size = static_dtr_hdr.code_size * sizeof(yalvm_bytecode_t);
        if (_codeOutput.write(static_destroy_code.buffer().buffer(), real_size)
                != real_size)
        {
            //_errorHandler.onError("Could not write static destroy code to code output", 0);
            return false;
        }
    }


    // write functions
    {
        ByteCodeGenerator function_code(state.module, state.registry);
        for (auto& function : function_vec)
        {
            FunctionDeclBaseNode* decl_node = function->functionNode();
            const FunctionType* fn_type = cast_type<const FunctionType>(function->symbol()->symbolType());
            std::string function_name = function->nativeFunctionName();

            // process bytecode
            if (decl_node && ast_typeof<FunctionDeclNode>(decl_node))
            {
                function_code.generateFunction(*ast_cast<FunctionDeclNode>(decl_node));
            }

            yalvm_func_header_t function_header;
            yalvm_func_header_init(&function_header);

            bool is_native_func = false;
            if (fn_type->isNativeImpl())
            {
                function_header.flags |= YALVM_FUNC_FLAG_NATIVE;
                function_name = "yal_native_" + function_name;
                is_native_func = true;
            }

            function_header.n_arguments = 0;
            // check if is an object call function
            if (fn_type->isObjectFunction())
            {
                function_header.n_arguments = 1;
            }

            // set function args
            if (fn_type->hasArguments())
            {
                function_header.n_arguments += fn_type->argumentCount();
            }

            // set function register count
            function_header.n_registers = !is_native_func
                    ? function_code.maxRegisterCount()
                    : fn_type->argumentCount();
            function_header.name_len = static_cast<yalvm_u16>(function_name.length() + 1);

            // set function code size;
            if (!is_native_func)
            {
                const size_t function_code_size = function_code.buffer().size();
                if (function_code_size >= YALVM_BIN_MAX_FUNCTION_CODE_SIZE)
                {
                    //_errorHandler.onError("Function code greater than maxium value", 0);
                    return false;
                }
                function_header.code_size = static_cast<yalvm_u16>(function_code_size);
            }
            else
            {
                function_header.code_size = 0;
            }

            if (_codeOutput.write(&function_header, sizeof(function_header))
                    != sizeof(function_header))
            {
                //_errorHandler.onError("Could not write function header to code output", 0);
                return false;
            }

            if (_codeOutput.write(function_name.c_str(), function_header.name_len)
                    != function_header.name_len)
            {
                return false;
            }

            if (is_native_func)
            {
                continue;
            }

            const size_t real_size = function_header.code_size * sizeof(yalvm_bytecode_t);
            if (_codeOutput.write(function_code.buffer().buffer(), real_size)
                    != real_size)
            {
                //_errorHandler.onError("Could not write function code to code output", 0);
                return false;
            }
        }
    }
    // write global function

    yalvm_func_header_t global_header;
    yalvm_func_header_init(&global_header);
    const char* global_function_name = yalvm_func_global_name();
    global_header.name_len = static_cast<yalvm_u16>(strlen(global_function_name) + 1);


    if (global_function.buffer().size())
    {
        global_function.addHaltInst();
        global_header.n_registers = global_function.maxRegisterCount();
        const size_t function_code_size = global_function.buffer().size();
        if (function_code_size >= YALVM_BIN_MAX_FUNCTION_CODE_SIZE)
        {
            // _errorHandler.onError("Global  code greater than maxium value", 0);
            return false;
        }
        global_header.code_size = static_cast<yal_u16>(function_code_size);
    }



    // write global function header
    if (_codeOutput.write(&global_header, sizeof(global_header))
            != sizeof(global_header))
    {
        //_errorHandler.onError("Could not write global header to code output", 0);
        return false;
    }

    // write global function name

    if (_codeOutput.write(global_function_name, global_header.name_len)
            != global_header.name_len)
    {
        return false;
    }


    // write global function code
    if (global_header.code_size)
    {
        const size_t real_size = global_header.code_size * sizeof(yalvm_bytecode_t);
        if (_codeOutput.write(global_function.buffer().buffer(), real_size)
                != real_size)
        {
            //_errorHandler.onError("Could not write global code to code output", 0);
            return false;
        }
    }


    return true;
}

}
