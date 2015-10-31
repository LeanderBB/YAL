#include "yal/bytecode/bytecodebuilder.h"
#include "yal/symbols/symboltreebuilder.h"
#include "yal/parser/parser_state.h"
#include "yal/ast/functionnode.h"
#include "yal/bytecode/bytecodegenerator.h"
#include "yal/bytecode/bytecodeprinter.h"
#include "yal/ast/variabledeclnode.h"
#include "yal/bytecode/bytecodegenerator.h"
#include "yal/util/outputformater.h"
#include <yalvm/yalvm_binary.h>
#include <yalvm/yalvm_hashing.h>
#include "yal/ast/functionnode.h"
#include "yal/ast/argumentdeclnode.h"
#include "yal/ast/variabledeclnode.h"
namespace yal
{

ByteCodeBuilder::ByteCodeBuilder(OutputSink &codeOutput,
                                 ErrorHandler &errorHandler):
    _codeOutput(codeOutput),
    _errorHandler(errorHandler)
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
        _errorHandler.onError("Constant 32bit count exceeds maximum value", 0);
        return false;
    }

    if (constants64_vec.size() >= YALVM_BIN_MAX_CONSTANTS)
    {
        _errorHandler.onError("Constant 64bit count exceeds maximum value", 0);
        return false;
    }


    // validate global count
    if (globals32_vec.size() >= YALVM_BIN_MAX_GLOBALS)
    {
        _errorHandler.onError("Global 32bit count exceeds maximum value", 0);
        return false;
    }

    if (globals64_vec.size() >= YALVM_BIN_MAX_GLOBALS)
    {
        _errorHandler.onError("Global 64bit count exceeds maximum value", 0);
        return false;
    }

    // validate function count
    if (function_vec.size() >= YALVM_BIN_MAX_FUNCTIONS)
    {
        _errorHandler.onError("Function count exceeds maximum value", 0);
        return false;
    }

    if (strings_vec.size() >= YALVM_BIN_MAX_STRINGS)
    {
        _errorHandler.onError("String count exceeds maximum value", 0);
        return false;
    }

    if (module.totalStringSizeBytes() >= YALVM_BIN_MAX_STRINGS_SIZE)
    {
        _errorHandler.onError("Exceeded maximum string storage capacity", 0);
        return false;
    }

    // generate global and init code


    ByteCodeGenerator static_init_code(state.module, _errorHandler);
    ByteCodeGenerator static_destroy_code(state.module, _errorHandler);

    ByteCodeGenerator global_function(state.module, _errorHandler);
    static_init_code.onScopeBeginGlobal(state.symbolTree.globalScope());
    static_destroy_code.onScopeEndGlobal(state.symbolTree.globalScope());
    for (auto& v : state.program)
    {
        // if symbol is not a function definition add it to the
        // global function scope
        if (!ast_typeof<FunctionDeclNode>(v))
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
                                 ? static_init_code.buffer().size() + 1
                                 : 0);
    static_init_hdr.n_registers = static_init_code.maxRegisterCount();
    static_dtr_hdr.code_size = (static_destroy_code.buffer().size()
                                ? static_destroy_code.buffer().size() + 1
                                : 0);
    static_dtr_hdr.n_registers = static_destroy_code.maxRegisterCount();


    // setup header
    yalvm_bin_header_t header;
    yalvm_bin_header_init(&header);
    header.n_constants32 = constants32_vec.size();
    header.n_constants64 = constants64_vec.size();
    header.n_globals32 = globals32_vec.size();
    header.n_globals64 = globals64_vec.size();
    header.n_globalsPtr = globalsPtr_vec.size();
    header.n_functions = function_vec.size() + 1; // +1 for global code
    header.n_strings = strings_vec.size();
    header.strings_size = module.totalStringSizeBytes();
    header.static_size = (sizeof(static_init_hdr) * 2 / sizeof(yalvm_bytecode_t))
            + static_init_hdr.code_size
            + static_dtr_hdr.code_size;


    // write header
    if (_codeOutput.write(&header, sizeof(header)) != sizeof(header))
    {
        _errorHandler.onError("Could not write header to code output", 0);
        return false;
    }

    // write 32bit constants

    for (auto& constant : constants32_vec)
    {
        const yal_u32 val = constant->value().valueAs32BitRaw();
        if (_codeOutput.write(&val, sizeof(val)) != sizeof(val))
        {
            _errorHandler.onError("Could not write constant 32 to code output", 0);
            return false;
        }
    }

    // write 64bit constants
    for (auto& constant : constants64_vec)
    {
        const yal_u64 val = constant->value().valueAs64BitRaw();
        if (_codeOutput.write(&val, sizeof(val)) != sizeof(val))
        {
            _errorHandler.onError("Could not write constant 64 to code output", 0);
            return false;
        }
    }


    // write strings
    for (auto& string: strings_vec)
    {
        const char* text = string->value().valueAsText();
        yalvm_u32 string_len = strlen(text);

        if (_codeOutput.write(&string_len, sizeof(string_len)) != sizeof(string_len))
        {
            _errorHandler.onError("Could not write string size to code output", 0);
            return false;
        }

        if (_codeOutput.write(text, string_len + 1) != string_len + 1)
        {
            _errorHandler.onError("Could not write string to code output", 0);
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
            _errorHandler.onError("Static init code sizer greater than maxium value", 0);
            return false;
        }
    }

    if (_codeOutput.write(&static_init_hdr, sizeof(static_init_hdr))
            != sizeof(static_init_hdr))
    {
        _errorHandler.onError("Could not write static init header to code output", 0);
        return false;
    }

    if (static_init_hdr.code_size)
    {
        const size_t real_size = static_init_hdr.code_size * sizeof(yalvm_bytecode_t);
        if (_codeOutput.write(static_init_code.buffer().buffer(), real_size)
                != real_size)
        {
            _errorHandler.onError("Could not write static init code to code output", 0);
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
            _errorHandler.onError("Static init code sizer greater than maxium value", 0);
            return false;
        }
    }

    if (_codeOutput.write(&static_dtr_hdr, sizeof(static_dtr_hdr))
            != sizeof(static_dtr_hdr))
    {
        _errorHandler.onError("Could not write static destroy header to code output", 0);
        return false;
    }

    if (static_dtr_hdr.code_size)
    {
        const size_t real_size = static_dtr_hdr.code_size * sizeof(yalvm_bytecode_t);
        if (_codeOutput.write(static_destroy_code.buffer().buffer(), real_size)
                != real_size)
        {
            _errorHandler.onError("Could not write static destroy code to code output", 0);
            return false;
        }
    }


    // write functions
    {
        ByteCodeGenerator function_code(state.module, _errorHandler);
        for (auto& function : function_vec)
        {
            FunctionDeclNode* decl_node = function->functionNode();
            const char* function_name = function->functionName();

            // process bytecode
            if (!function_code.generateFunction(*decl_node))
            {
                return false;
            }

            yalvm_func_header_t function_header;
            yalvm_func_header_init(&function_header, function_name);

            // set function args
            if (decl_node->hasFunctionArguments())
            {
                function_header.n_arguments = decl_node->functionArguments()->arguments().size();
            }

            // set function register count
            function_header.n_registers = function_code.maxRegisterCount();

            // set function code size;
            const size_t function_code_size = function_code.buffer().size();
            if (function_code_size >= YALVM_BIN_MAX_FUNCTION_CODE_SIZE)
            {
                _errorHandler.onError("Function code greater than maxium value", 0);
                return false;
            }

            function_header.code_size = function_code_size;

            if (_codeOutput.write(&function_header, sizeof(function_header))
                    != sizeof(function_header))
            {
                _errorHandler.onError("Could not write function header to code output", 0);
                return false;
            }

            const size_t real_size = function_code_size * sizeof(yalvm_bytecode_t);
            if (_codeOutput.write(function_code.buffer().buffer(), real_size)
                    != real_size)
            {
                _errorHandler.onError("Could not write function code to code output", 0);
                return false;
            }
        }
    }
    // write global function

    yalvm_func_header_t global_header;
    yalvm_func_header_init(&global_header, yalvm_func_global_name());
    if (global_function.buffer().size())
    {
        global_function.addHaltInst();
        global_header.n_registers = global_function.maxRegisterCount();
        const size_t function_code_size = global_function.buffer().size();
        if (function_code_size >= YALVM_BIN_MAX_FUNCTION_CODE_SIZE)
        {
            _errorHandler.onError("Global  code greater than maxium value", 0);
            return false;
        }
        global_header.code_size = function_code_size;
    }

    // write global function header
    if (_codeOutput.write(&global_header, sizeof(global_header))
            != sizeof(global_header))
    {
        _errorHandler.onError("Could not write global header to code output", 0);
        return false;
    }

    // write global function code
    if (global_header.code_size)
    {
        const size_t real_size = global_header.code_size * sizeof(yalvm_bytecode_t);
        if (_codeOutput.write(global_function.buffer().buffer(), real_size)
                != real_size)
        {
            _errorHandler.onError("Could not write global code to code output", 0);
            return false;
        }
    }


    return true;
}

}
