#include "yal/bytecode/bytecodegenerator.h"
#include "yal/ast/astnodetypes.h"
#include "yal/ast/astbasenode.h"
#include "yal/ast/assignoperatornode.h"
#include "yal/ast/codebodynode.h"
#include "yal/ast/compareoperatornode.h"
#include "yal/ast/constantnode.h"
#include "yal/ast/argumentdeclnode.h"
#include "yal/ast/variabledeclnode.h"
#include "yal/ast/dualoperatornode.h"
#include "yal/ast/singleoperatornode.h"
#include "yal/ast/functionnode.h"
#include "yal/ast/conditionnode.h"
#include "yal/ast/returnnode.h"
#include "yal/symbols/symboltable.h"
#include "yal/symbols/variablesym.h"
#include "yal/symbols/functionsym.h"
#include "yal/bytecode/bytecode_utils.h"
#include "yal/ast/printnode.h"
#include <cstdio>
#include <limits>

namespace yal
{

ByteCodeGenerator::Register::Register():
    _registerIdx(RegisterAllocator::UnusedRegisterValue),
    _isTemporary(false)
{
}

ByteCodeGenerator::Register::Register(RegisterAllocator& alloctor,
                                      const char* variable,
                                      const yal_u32 scopeLevel):
    _registerIdx(alloctor.registerForVariable(variable, scopeLevel)),
    _scopeLevel(scopeLevel),
    _isTemporary(false)
{

}

ByteCodeGenerator::Register::Register(RegisterAllocator& alloctor):
    _registerIdx(alloctor.allocateTemporary()),
    _isTemporary(true)
{

}


bool
ByteCodeGenerator::Register::isValid() const
{
    return _registerIdx != RegisterAllocator::UnusedRegisterValue;
}

void
ByteCodeGenerator::Register::release(RegisterAllocator& alloctor)
{
    if (_isTemporary)
    {
        alloctor.deallocateTemporary(_registerIdx);
    }
}

ByteCodeGenerator::ByteCodeGenerator(Module &moduleInfo,
                                     ErrorHandler &errorHandler):
    _moduleInfo(moduleInfo),
    _errorHandler(errorHandler),
    _buffer(),
    _regAllocator(),
    _didError(false)
{

}

ByteCodeGenerator::~ByteCodeGenerator()
{

}

bool
ByteCodeGenerator::generateFunction(FunctionDeclNode& node)
{
    reset();
    const bool has_return_value = setupFunction(node);
    node.functionCode()->accept(*this);
    yalvm_bytecode_t code;

    if (has_return_value)
    {
        code = yalvm_bytecode_pack_instruction(YALVM_BYTECODE_HALT);
    }
    else
    {
        code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_RETURN, 0xFF, 0);
    }
    _buffer.append(code);
    return !didError();
}

bool
ByteCodeGenerator::generate(AstBaseNode &node)
{
    node.accept(*this);
    return !didError();
}

void
ByteCodeGenerator::reset()
{
    _buffer.reset();
    _regAllocator.reset();
    while(!_registerStack.empty())
    {
        _registerStack.pop();
    }
    _currentRegister = Register();
    _didError = false;
}

void
ByteCodeGenerator::addHaltInst()
{
    _buffer.append(yalvm_bytecode_pack_value(YALVM_BYTECODE_HALT,0));
}

yal_u8
ByteCodeGenerator::maxRegisterCount() const
{
    return _regAllocator.maxRegisterCount();
}

void
ByteCodeGenerator::logError(const AstBaseNode& node)
{
    _errorHandler.onCompileError(_formater.str(), _formater.strLen(), node.locationInfo());
    setError();
}

bool
ByteCodeGenerator::setupFunction(const yal::FunctionDeclNode &node)
{
    if (node.hasFunctionArguments())
    {
        // register function arguments to the first registers in
        // ascending order
        const ArgumentDeclsNode* func_args = node.functionArguments();
        for (auto& arg : func_args->arguments())
        {
            bool result = _regAllocator.allocate(arg->argumentName(), node.symbolTable()->level());
            (void) result;
            YAL_ASSERT(result != RegisterAllocator::UnusedRegisterValue);
        }
    }

    // preallocate each variable that is not scoped
    const SymbolTable* function_table = node.functionCode()->symbolTable();

    const bool scope_result = registerScope(function_table);
    YAL_ASSERT(scope_result);
    if (!scope_result)
    {
        return false;
    }

    FunctionSym* func_sym = symbol_cast<FunctionSym>(node.symbolTable()->resolveSymbol(node.functionName()));
    YAL_ASSERT(func_sym);

    return func_sym->hasReturnValue();
}

bool
ByteCodeGenerator::registerScope(const SymbolTable* table)
{
    auto symbolit = table->symbolsBegin();
    auto symbolit_end = table->symbolsEnd();

    for(; symbolit != symbolit_end; ++symbolit)
    {
        const SymbolTable::SymbolPtr_t& symbol = symbolit->second;
        if(symbol_typeof<VariableSym>(symbol.get()))
        {
            yal_i32 result = _regAllocator.allocate(symbol->symbolName(),
                                                    table->level());
            if (result == RegisterAllocator::UnusedRegisterValue)
            {
                return false;
            }
        }
    }
    return true;
}

void
ByteCodeGenerator::unregisterScope(const SymbolTable* table)
{
    auto symbolit = table->symbolsBegin();
    auto symbolit_end = table->symbolsEnd();

    for(; symbolit != symbolit_end; ++symbolit)
    {
        const SymbolTable::SymbolPtr_t& symbol = symbolit->second;
        if(symbol_typeof<VariableSym>(symbol.get()))
        {
            _regAllocator.deallocate(symbol->symbolName(),
                                     table->level());
        }
    }
}

void
ByteCodeGenerator::pushAndSetRegister(const Register reg)
{
    _registerStack.push(_currentRegister);
    _currentRegister = reg;
}

void
ByteCodeGenerator::popRegister()
{
    if (!_registerStack.empty())
    {
        const Register top = _registerStack.top();
        _currentRegister.release(_regAllocator);
        _currentRegister = top;
        _registerStack.pop();
    }
}

bool
ByteCodeGenerator::getGlobalVarIdx(yal_u32& idx,
                                   const char* varName)
{
    const ModuleGlobal* global_var = _moduleInfo.global(varName);
    if (!global_var)
    {
        _formater.format("Could not find global variable '%s'", varName);
        return false;
    }
    idx = global_var->moduleIndex();
    if (idx == ModuleIndexable::IndexUnused)
    {
        _formater.format("Global variable '%s' has not been registered",
                         varName);
        return false;
    }
    return true;
}

bool
ByteCodeGenerator::getFunctionIdx(yal_u32& idx,
                                  const char* functionName)
{
    const ModuleFunction* function = _moduleInfo.function(functionName);
    if (!function)
    {
        _formater.format("Could not find function '%s'", functionName);
        return false;
    }
    idx = function->moduleIndex();
    if (idx == ModuleIndexable::IndexUnused)
    {
        _formater.format("Function '%s' has not been registered",
                         functionName);
        return false;
    }
    return true;
}

bool
ByteCodeGenerator::getConstantIdx(yal_u32& idx,
                                  const ConstantValue& val)
{
    const ModuleConstant* constant = _moduleInfo.constant(val);
    if (!constant)
    {
        _formater.format("Could not find a matching constant");
        return false;
    }
    idx = constant->moduleIndex();
    if (idx == ModuleIndexable::IndexUnused)
    {
        _formater.format("Constant has not been registered");
        return false;
    }
    return true;
}


void
ByteCodeGenerator::visit(AssignOperatorNode& node)
{
    (void) node;
    // lookup variable register

    const SymbolTable* sym_table = node.symbolTable();

    Symbol* symbol = sym_table->resolveSymbol(node.variableName());
    const VariableSym* var_symbol = symbol_cast<VariableSym>(symbol);
    const char* variable_name = node.variableName();

    YAL_ASSERT(var_symbol);

    Register var_register;
    const bool var_is_global = var_symbol->isGlobal();
    yal_u32 global_idx = ModuleIndexable::IndexUnused;
    if (var_is_global)
    {
        var_register = Register(_regAllocator);
        if(!getGlobalVarIdx(global_idx, variable_name))
        {
            logError(node);
            return;
        }
    }
    else
    {
        var_register = Register(_regAllocator, variable_name,
                                var_symbol->scopeLevel());
    }

    if (!var_register.isValid())
    {
        _formater.format("Variable '%s' is not registered\n", variable_name);
        logError(node);
        return;
    }

    pushAndSetRegister(Register(_regAllocator));
    node.expression()->accept(*this);
    const Register tmp_register = _currentRegister;
    YAL_ASSERT(tmp_register.isValid());
    popRegister();

    // handle error
    if (didError())
    {
        return;
    }

    yalvm_bytecode_inst_t inst =  AssignOperatorByteCodeInst(node.assignOperatorType(),
                                                             node.typeInfo());


    yalvm_bytecode_t code;

    if (inst == YALVM_BYTECODE_COPY_REGISTER)
    {

        code = yalvm_bytecode_pack_two_registers(inst, var_register.registerIdx(),
                                                 tmp_register.registerIdx());
    }
    else
    {
        code = yalvm_bytecode_pack_three_registers(inst, var_register.registerIdx(),
                                                   var_register.registerIdx(),
                                                   tmp_register.registerIdx());
    }
    _buffer.append(code);

    // store global result
    if (var_is_global)
    {
        inst = StoreGlobalByteCodeInst(node.typeInfo());

        code = yalvm_bytecode_pack_dst_value(inst, var_register.registerIdx(), global_idx);

        _buffer.append(code);
    }

}

void
ByteCodeGenerator::visit(CodeBodyNode& node)
{
    for(auto& v : node.statements)
    {
        if (!didError())
        {
            v->accept(*this);
        }
        else
        {
            return;
        }
    }
}

void
ByteCodeGenerator::visit(CompareOperatorNode& node)
{

    // process left part

    pushAndSetRegister(Register(_regAllocator));
    node.leftExpression()->accept(*this);

    const Register left_register = _currentRegister;
    YAL_ASSERT(left_register.isValid());


    if (didError())
    {
        return;
    }

    // process right part
    pushAndSetRegister(Register(_regAllocator));
    node.rightExpression()->accept(*this);
    const Register right_register = _currentRegister;
    YAL_ASSERT(right_register.isValid());

    popRegister();
    popRegister();

    yalvm_bytecode_inst_t inst = CompareOperatorByteCodeInst(node.compareOperatorType(),
                                                             node.typeInfo());

    YAL_ASSERT(inst != YALVM_BYTECODE_TOTAL);

    yalvm_bytecode_t code = yalvm_bytecode_pack_three_registers(inst, _currentRegister.registerIdx(),
                                                                left_register.registerIdx(),
                                                                right_register.registerIdx());
    _buffer.append(code);
}

void
ByteCodeGenerator::visit(ConstantNode& node)
{
    yal_u32 inst_value = 0;
    yalvm_bytecode_inst_t instruction = YALVM_BYTECODE_TOTAL;

    switch(node.constantType())
    {
    case kConstantTypeId:
    {
        const char* var_name = node.constantValue().valueAsId();

        const SymbolTable* sym_table = node.symbolTable();

        Symbol* symbol = sym_table->resolveSymbol(var_name);
        const VariableSym* var_symbol = symbol_cast<VariableSym>(symbol);

        YAL_ASSERT(var_symbol);

        Register var_register;
        const bool var_is_global = var_symbol->isGlobal();
        if (var_is_global)
        {
            var_register = Register(_regAllocator);
            if (!getGlobalVarIdx(inst_value, var_name))
            {
                logError(node);
                return;
            }
            instruction = LoadGlobalByteCodeInst(var_symbol->returnType());
            break;
        }
        else
        {
            popRegister();
            Register target_reg = Register(_regAllocator, var_name,
                                           var_symbol->scopeLevel());
            if (!target_reg.isValid())
            {
                _formater.format("Variable '%s' has not been registered\n", var_name);
                logError(node);
                return;
            }
            pushAndSetRegister(target_reg);
            return;
        }
    }
    case kConstantTypeText:
    {
        if (!getConstantIdx(inst_value, node.constantValue()))
        {
            logError(node);
            return;
        }
        instruction = YALVM_BYTECODE_LOAD_STRING;
        break;
    }
    case kConstantTypeInt32:
    {
        yal_i32 value = node.constantValue().valueAsInt32();
        if (node.constantValue().valueFitsInByteCode())
        {
            union{
                yal_i32 s;
                yal_u32 u;
            } signed_to_unsigned;
            signed_to_unsigned.s = value;
            inst_value = signed_to_unsigned.u;
            instruction = YALVM_BYTECODE_LOAD_VALUE;
        }
        else
        {
            if (!getConstantIdx(inst_value, node.constantValue()))
            {
                logError(node);
                return;
            }
            instruction = YALVM_BYTECODE_LOAD_CONST_32;
        }
        break;
    }
    case kConstantTypeUInt32:
    {
        yal_u32 value = node.constantValue().valueAsUInt32();
        if (node.constantValue().valueFitsInByteCode())
        {
            inst_value = value;
            instruction = YALVM_BYTECODE_LOAD_VALUE;
        }
        else
        {
            if (!getConstantIdx(inst_value, node.constantValue()))
            {
                logError(node);
                return;
            }
            instruction = YALVM_BYTECODE_LOAD_CONST_32;
        }
        break;
    }
    case kConstantTypeInt64:
    {
        yal_i64 value = node.constantValue().valueAsInt64();
        if (node.constantValue().valueFitsInByteCode())
        {
            union{
                yal_i64 s;
                yal_u64 u;
            } signed_to_unsigned;
            signed_to_unsigned.s = value;
            inst_value = signed_to_unsigned.u;
            instruction = YALVM_BYTECODE_LOAD_VALUE;
        }
        else
        {
            if (!getConstantIdx(inst_value, node.constantValue()))
            {
                logError(node);
                return;
            }
            instruction = YALVM_BYTECODE_LOAD_CONST_64;
        }
        break;
    }
    case kConstantTypeUInt64:
    {
        yal_u64 value = node.constantValue().valueAsUInt64();
        if (node.constantValue().valueFitsInByteCode())
        {
            inst_value = value;
            instruction = YALVM_BYTECODE_LOAD_VALUE;
        }
        else
        {
            if (!getConstantIdx(inst_value, node.constantValue()))
            {
                logError(node);
                return;
            }
            instruction = YALVM_BYTECODE_LOAD_CONST_64;
        }
        break;
    }
    case kConstantTypeBool:
    {
        yal_bool value = node.constantValue().valueAsBoolean();
        inst_value =  (value) ? 1 : 0;
        instruction = YALVM_BYTECODE_LOAD_VALUE;
    }
    case kConstantTypeFloat32:
    {
        if (!getConstantIdx(inst_value, node.constantValue()))
        {
            logError(node);
            return;
        }
        instruction = YALVM_BYTECODE_LOAD_CONST_32;
        break;
    }
    case kConstantTypeFloat64:
    {
        if (!getConstantIdx(inst_value, node.constantValue()))
        {
            logError(node);
            return;
        }
        instruction = YALVM_BYTECODE_LOAD_CONST_64;
        break;
    }
    default:
        YAL_ASSERT(false && "should not be reached\n");
        setError();
        return;
    }

    yalvm_bytecode_t code = yalvm_bytecode_pack_dst_value(instruction,
                                                          _currentRegister.registerIdx(),
                                                          inst_value);
    _buffer.append(code);
}

void
ByteCodeGenerator::visit(ArgumentDeclNode& node)
{
    (void) node;
    _formater.format("Node '%s' Should not be reached\n", node.nodeTypeStr());
    logError(node);
    return;
}

void
ByteCodeGenerator::visit(ArgumentDeclsNode& node)
{
    (void) node;
    _formater.format("Node '%s' Should not be reached\n", node.nodeTypeStr());
    logError(node);
    return;
}

void
ByteCodeGenerator::visit(FunctionCallArgsNode& node)
{
    for(auto& exp : node.expressions)
    {
        pushAndSetRegister(Register(_regAllocator));
        exp->accept(*this);
        Register tmp_register  = _currentRegister;
        YAL_ASSERT(tmp_register.isValid());
        popRegister();

        yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(YALVM_BYTECODE_PUSH_ARG,
                                                                 tmp_register.registerIdx());
        _buffer.append(code);
    }

}

void
ByteCodeGenerator::visit(VariableDeclNode& node)
{

    const SymbolTable* sym_table = node.symbolTable();


    Symbol* symbol = sym_table->resolveSymbol(node.variableName());
    const VariableSym* var_symbol = symbol_cast<VariableSym>(symbol);
    const char* var_name = node.variableName();

    YAL_ASSERT(var_symbol);

    Register var_register;
    const bool var_is_global = var_symbol->isGlobal();
    if (var_is_global)
    {
        var_register = Register(_regAllocator);
    }
    else
    {
        var_register =  Register(_regAllocator, var_name,
                                 node.symbolTable()->level());

        // check if variable has been allocated
        if (!var_register.isValid())
        {
            _formater.format("Variable '%s' has not been registered\n", var_name);
            logError(node);
            return;
        }

    }

    pushAndSetRegister(var_register);
    node.expression()->accept(*this);
    popRegister();

    if (var_is_global)
    {
        yal_u32 global_idx = 0;
        if (!getGlobalVarIdx(global_idx, var_name))
        {
            logError(node);
            return;
        }

        yalvm_bytecode_inst_t instruction = StoreGlobalByteCodeInst(node.typeInfo());

        yalvm_bytecode_t code = yalvm_bytecode_pack_dst_value(instruction,
                                                              var_register.registerIdx(),
                                                              global_idx);

        _buffer.append(code);
    }

}

void
ByteCodeGenerator::visit(DualOperatorNode& node)
{
    (void) node;


    // process left part

    pushAndSetRegister(Register(_regAllocator));
    node.leftExpression()->accept(*this);

    const Register left_register = _currentRegister;
    YAL_ASSERT(left_register.isValid());


    if (didError())
    {
        return;
    }

    // process right part
    pushAndSetRegister(Register(_regAllocator));
    node.rightExpression()->accept(*this);
    const Register right_register = _currentRegister;
    YAL_ASSERT(right_register.isValid());

    popRegister();
    popRegister();

    yalvm_bytecode_inst_t inst = DualOperatorByteCodeInst(node.dualOperatorType(),
                                                          node.typeInfo());

    YAL_ASSERT(inst != YALVM_BYTECODE_TOTAL);

    yalvm_bytecode_t code = yalvm_bytecode_pack_three_registers(inst, _currentRegister.registerIdx(),
                                                                left_register.registerIdx(),
                                                                right_register.registerIdx());
    _buffer.append(code);
}

void
ByteCodeGenerator::visit(SingleOperatorNode& node)
{
    (void) node;

    node.expression()->accept(*this);

    // nothing to do for block operator
    if (!didError() && node.singleOperatorType() != kSingleOperatorTypeBlock)
    {

        yalvm_bytecode_inst_t inst = SingeOperatorByteCodeInst(node.singleOperatorType(),
                                                               node.typeInfo());

        YAL_ASSERT(inst != YALVM_BYTECODE_TOTAL);

        yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(inst,
                                                                 _currentRegister.registerIdx());

        _buffer.append(code);

    }
}

void
ByteCodeGenerator::visit(FunctionCallNode& node)
{

    const char* function_name = node.functionName();

    // load function symbol
    const FunctionSym* function_sym = symbol_cast<FunctionSym>(node.symbolTable()->resolveSymbol(function_name));
    if (!function_sym)
    {
        _formater.format("Could not find function symbol for '%s'\n",
                         function_name);
        logError(node);
        return;
    }


    yal_u32 function_idx = 0;
    if (!getFunctionIdx(function_idx, function_name))
    {
        _formater.format("Could not find function number for  '%s'\n",
                         function_name);
        logError(node);
        return;
    }

    bool should_pop_register = false;
    if(!_currentRegister.isValid())
    {
        pushAndSetRegister(Register(_regAllocator));
        should_pop_register = true;
    }
    YAL_ASSERT(_currentRegister.isValid());

    Register return_register;
    if (function_sym->hasReturnValue())
    {
        return_register = _currentRegister;
        pushAndSetRegister(Register(_regAllocator));
    }

    yalvm_bytecode_t code_load = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_LOAD_FUNCTION,
                                                               _currentRegister.registerIdx(),
                                                               function_idx);
    _buffer.append(code_load);

    // process arguments
    if (node.hasFunctionArguments())
    {
        node.functionArguments()->accept(*this);
    }

    yalvm_bytecode_t code_call = yalvm_bytecode_pack_three_registers(YALVM_BYTECODE_CALL,
                                                                     _currentRegister.registerIdx(),
                                                                     return_register.registerIdx(),
                                                                     node.functionArgumentsCount());
    _buffer.append(code_call);

    if (return_register.isValid())
    {
        popRegister();
    }

    if (should_pop_register)
    {
        popRegister();
    }
    return;
}

void
ByteCodeGenerator::visit(FunctionDeclNode& node)
{
    (void) node;
    _formater.format("Node '%s' Should not be reached\n", node.nodeTypeStr());
    logError(node);
    return;
}

void
ByteCodeGenerator::visit(ConditionNode& node)
{

    Register tmp_register;
    size_t jump_to_false_offset = 0;
    if (node.hasConditionComponent())
    {
        pushAndSetRegister(Register(_regAllocator));
        node.condition()->accept(*this);
        tmp_register  = _currentRegister;
        YAL_ASSERT(tmp_register.isValid());
        popRegister();

        // check of error
        if (didError())
        {
            return;
        }

        yalvm_bytecode_t condition_code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_JUMP_FALSE,
                                                                        tmp_register.registerIdx(),
                                                                        1);
        jump_to_false_offset  = _buffer.append(condition_code);
        YAL_ASSERT(node.hasOnTrueComponent());
    }

    registerScope(node.onTrue()->symbolTable());
    node.onTrue()->accept(*this);
    unregisterScope(node.onTrue()->symbolTable());

    if (node.hasConditionComponent())
    {
        // update previous value
        const size_t current_offset = _buffer.size();
        size_t diff = current_offset - jump_to_false_offset;
        YAL_ASSERT(diff < std::numeric_limits<yal_u16>::max());
        _buffer.replace(jump_to_false_offset, yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_JUMP_FALSE,
                                                                            tmp_register.registerIdx(),
                                                                            static_cast<yal_u16>(diff)));
    }

    if (node.hasOnFalseComponent())
    {
        // mark jump instruction
        const size_t jump_to_end_offset = _buffer.append(yalvm_bytecode_pack_value(YALVM_BYTECODE_JUMP, 0));
        registerScope(node.onFalse()->symbolTable());
        node.onFalse()->accept(*this);
        unregisterScope(node.onFalse()->symbolTable());
        // check of error
        if (didError())
        {
            return;
        }

        // update jmp instruction
        const size_t current_offset = _buffer.size();
        const size_t diff = current_offset - jump_to_end_offset;
        YAL_ASSERT(diff < std::numeric_limits<yal_u16>::max());

        _buffer.replace(jump_to_end_offset, yalvm_bytecode_pack_value(YALVM_BYTECODE_JUMP,
                                                                      static_cast<yal_u16>(diff)));
    }

    return;
}

void
ByteCodeGenerator::visit(ReturnNode& node)
{
    yalvm_bytecode_t code = 0;

    pushAndSetRegister(Register(_regAllocator));
    if (node.expression())
    {
        node.expression()->accept(*this);
        const Register tmp_register = _currentRegister;
        YAL_ASSERT(tmp_register.isValid());
        popRegister();
        if (didError())
        {
            return;
        }

        code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_RETURN,
                                             tmp_register.registerIdx(),
                                             0);
    }
    else
    {
        code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_RETURN,
                                             0xFF,
                                             0);
    }
    _buffer.append(code);
    return;
}

void
ByteCodeGenerator::visit(PrintNode& node)
{
    node.arguments()->accept(*this);
    const yalvm_bytecode_t code = yalvm_bytecode_pack_instruction(YALVM_BYTECODE_PRINT_NL);
    _buffer.append(code);
}

void
ByteCodeGenerator::visit(PrintArgsNode& node)
{
    for(auto& exp : node.expressions)
    {
        pushAndSetRegister(Register(_regAllocator));
        exp->accept(*this);
        Register tmp_register  = _currentRegister;
        YAL_ASSERT(tmp_register.isValid());
        popRegister();

        const yalvm_bytecode_inst_t print_inst = PrintByteCodeInst(exp->typeInfo());
        const yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(print_inst,
                                                                       tmp_register.registerIdx());
        _buffer.append(code);
    }

}

}
