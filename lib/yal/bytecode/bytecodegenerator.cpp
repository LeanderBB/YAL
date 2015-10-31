#include "yal/bytecode/bytecodegenerator.h"
#include "yal/ast/asthdrs.h"
#include "yal/types/typehdrs.h"
#include "yal/symbols/scope.h"
#include "yal/symbols/objectscopeaction.h"
#include "yal/bytecode/bytecode_utils.h"
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
    _scopeVisitor(*this),
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

    if (!onScopeBegin(*node.functionCode()))
    {
        return false;
    }

    node.functionCode()->accept(*this);
    if (!onScopeEnd(*node.functionCode()))
    {
        return false;
    }

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

bool
ByteCodeGenerator::onScopeBeginGlobal(const Scope& scope)
{
    scope.onScopeEnter(_scopeVisitor);
    return true;
}

bool
ByteCodeGenerator::onScopeEndGlobal(const Scope &scope)
{
    scope.onScopeExit(_scopeVisitor);
    return true;
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
            bool result = _regAllocator.allocate(arg->argumentName(), arg->scope()->level());
            (void) result;
            YAL_ASSERT(result != RegisterAllocator::UnusedRegisterValue);
        }
    }

    Symbol* func_sym = node.scope()->resolveSymbol(node.functionName());
    YAL_ASSERT(func_sym && func_sym->isFunction());

    FunctionType* fn_type = cast_type<FunctionType>(func_sym->astNode()->nodeType());
    YAL_ASSERT(fn_type);
    return fn_type->hasReturnValue();
}

bool
ByteCodeGenerator::registerScope(const Scope* scope)
{
    const SymbolTable& sym_table = scope->symbolTable();

    auto symbolit = sym_table.symbolsBegin();
    auto symbolit_end = sym_table.symbolsEnd();

    for(; symbolit != symbolit_end; ++symbolit)
    {
        const SymbolTable::SymbolPtr_t& symbol = symbolit->second;
        if(symbol->isVariable())
        {
            yal_i32 result = _regAllocator.allocate(symbol->symbolName(),
                                                    scope->level());
            if (result == RegisterAllocator::UnusedRegisterValue)
            {
                return false;
            }
        }
    }
    return true;
}

void
ByteCodeGenerator::unregisterScope(const Scope *scope)
{
    const SymbolTable& sym_table = scope->symbolTable();

    auto symbolit = sym_table.symbolsBegin();
    auto symbolit_end = sym_table.symbolsEnd();

    for(; symbolit != symbolit_end; ++symbolit)
    {
        const SymbolTable::SymbolPtr_t& symbol = symbolit->second;
        if(symbol->isVariable())
        {
            _regAllocator.deallocate(symbol->symbolName(),
                                     scope->level());
        }
    }
}

void
ByteCodeGenerator::pushRegister(const Register reg)
{
    _registerStack.push(reg);
}

ByteCodeGenerator::Register
ByteCodeGenerator::popRegister()
{
    if (!_registerStack.empty())
    {
        const Register top = _registerStack.top();
        _registerStack.pop();
        return top;
    }
    else
    {
        return Register();
    }
}

const ByteCodeGenerator::Register&
ByteCodeGenerator::topRegister() const
{
    return _registerStack.top();
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

bool
ByteCodeGenerator::onScopeBegin(const AstBaseNode& node)
{
    const Scope* scope = node.scope();
    if (registerScope(scope))
    {
        scope->onScopeEnter(_scopeVisitor);
        return true;
    }
    return false;
}

bool
ByteCodeGenerator::onScopeEnd(const AstBaseNode& node)
{
    bool result = true;
    const Scope* scope = node.scope();
    scope->onScopeExit(_scopeVisitor);
    unregisterScope(scope);
    return result;
}

void
ByteCodeGenerator::loadVariableIntoRegister(const char* varName,
                                            const AstBaseNode& node)
{
    yal_u32 inst_value = 0;
    yalvm_bytecode_inst_t instruction = YALVM_BYTECODE_TOTAL;

    Register cur_register;

    const Scope* scope = node.scope();
    const SymbolTable& sym_table = scope->symbolTable();
    const Symbol* var_symbol = sym_table.resolveSymbol(varName);

    YAL_ASSERT(var_symbol && var_symbol->isVariable());
    const bool var_is_global = var_symbol->isGlobalSymbol();
    if (var_is_global)
    {
        instruction = LoadGlobalByteCodeInst(var_symbol->astNode()->nodeType());
        cur_register = Register(_regAllocator);
        if (!getGlobalVarIdx(inst_value, varName))
        {
            logError(node);
            return;
        }
    }
    else
    {
        cur_register = Register(_regAllocator, varName,
                                var_symbol->scope()->level());
        if (!cur_register.isValid())
        {
            _formater.format("Variable '%s' has not been registered\n", varName);
            logError(node);
            return;
        }
        pushRegister(cur_register);
        return;
    }

    pushRegister(cur_register);

    yalvm_bytecode_t code = yalvm_bytecode_pack_dst_value(instruction,
                                                          cur_register.registerIdx(),
                                                          inst_value);
    _buffer.append(code);
}

void
ByteCodeGenerator::visit(AssignOperatorNode& node)
{
    // process left part
    node.expressionLeft()->accept(*this);
    Register left_register = popRegister();
    YAL_ASSERT(left_register.isValid());

    if (didError())
    {
        return;
    }


    // process right part
    node.expressionRight()->accept(*this);
    Register right_register = popRegister();
    YAL_ASSERT(right_register.isValid());

    if (didError())
    {
        return;
    }

    const ExpressionResult& exp_type = node.expressionResult();
    if (exp_type.type->isBuiltinType())
    {

        yalvm_bytecode_inst_t inst =  AssignOperatorByteCodeInst(node.assignOperatorType(),
                                                                 exp_type.type);


        yalvm_bytecode_t code;

        if (inst != YALVM_BYTECODE_COPY_REGISTER)
        {
            code = yalvm_bytecode_pack_three_registers(inst, left_register.registerIdx(),
                                                       left_register.registerIdx(),
                                                       right_register.registerIdx());
        }
        else
        {
            code = yalvm_bytecode_pack_two_registers(inst, left_register.registerIdx(),
                                                     right_register.registerIdx());
        }

        _buffer.append(code);

        // store global result
        const Symbol* assign_sym = node.expressionLeft()->expressionResult().symbol;
        if (assign_sym->isGlobalVariable())
        {
            inst = StoreGlobalByteCodeInst(exp_type.type);

            yal_u32 global_idx = ModuleIndexable::IndexUnused;
            if(!getGlobalVarIdx(global_idx, assign_sym->symbolName()))
            {
                logError(node);
                return;
            }

            code = yalvm_bytecode_pack_dst_value(inst, left_register.registerIdx(), global_idx);

            _buffer.append(code);
        }

    }
    else
    {
        _formater.format("Assignment of custom data types not yet implemented\n");
        logError(node);
        return;
    }


    if (node.parentNode() && node.parentNode()->isExpressionNode())
    {
        pushRegister(left_register);
    }
    else
    {
        left_register.release(_regAllocator);
    }

    right_register.release(_regAllocator);
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
    node.leftExpression()->accept(*this);
    Register left_register = popRegister();
    YAL_ASSERT(left_register.isValid());


    if (didError())
    {
        return;
    }

    // process right part
    node.rightExpression()->accept(*this);
    Register right_register = popRegister();
    YAL_ASSERT(right_register.isValid());


    // create result register

    Register result_register(_regAllocator);
    pushRegister(result_register);

    const Type* exp_type = node.expressionResult().type;
    if (exp_type->isBuiltinType())
    {
        yalvm_bytecode_inst_t inst = CompareOperatorByteCodeInst(node.compareOperatorType(),
                                                                 exp_type);

        YAL_ASSERT(inst != YALVM_BYTECODE_TOTAL);

        yalvm_bytecode_t code = yalvm_bytecode_pack_three_registers(inst, result_register.registerIdx(),
                                                                    left_register.registerIdx(),
                                                                    right_register.registerIdx());
        _buffer.append(code);

    }
    else
    {
        _formater.format("Compare of custom data types not yet implemented\n");
        logError(node);
        return;
    }

    left_register.release(_regAllocator);
    right_register.release(_regAllocator);
}

void
ByteCodeGenerator::visit(ConstantNode& node)
{
    yal_u32 inst_value = 0;
    yalvm_bytecode_inst_t instruction = YALVM_BYTECODE_TOTAL;

    Register cur_register = Register(_regAllocator);

    const Type* exp_type = node.expressionResult().type;
    YAL_ASSERT(exp_type->isBuiltinType());
    switch(node.constantValue().type())
    {
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

    pushRegister(cur_register);

    yalvm_bytecode_t code = yalvm_bytecode_pack_dst_value(instruction,
                                                          cur_register.registerIdx(),
                                                          inst_value);
    _buffer.append(code);
}

void
ByteCodeGenerator::visit(ArgumentDeclNode& node)
{
    (void) node;
    _formater.format("Node '%s' Should not be reached\n", node.astTypeStr());
    logError(node);
    return;
}

void
ByteCodeGenerator::visit(ArgumentDeclsNode& node)
{
    (void) node;
    _formater.format("Node '%s' Should not be reached\n", node.astTypeStr());
    logError(node);
    return;
}

void
ByteCodeGenerator::visit(FunctionCallArgsNode& node)
{
    for(auto& exp : node.expressions)
    {
        exp->accept(*this);
        Register tmp_register  = popRegister();
        YAL_ASSERT(tmp_register.isValid());
        yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(YALVM_BYTECODE_PUSH_ARG,
                                                                 tmp_register.registerIdx());
        _buffer.append(code);
        tmp_register.release(_regAllocator);

    }

}

void
ByteCodeGenerator::visit(VariableDeclNode& node)
{
    const Scope* scope = node.scope();
    const SymbolTable& sym_table = scope->symbolTable();

    Symbol* var_symbol = sym_table.resolveSymbol(node.variableName());
    const char* variable_name = node.variableName();

    YAL_ASSERT(var_symbol->isVariable());

    Register var_register;
    const bool var_is_global = var_symbol->isGlobalSymbol();
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
                                var_symbol->scope()->level());
    }

    if (!var_register.isValid())
    {
        _formater.format("Variable '%s' is not registered\n", variable_name);
        logError(node);
        return;
    }

    node.expression()->accept(*this);

    if (didError())
    {
        return;
    }

    Register tmp = popRegister();
    YAL_ASSERT(tmp.isValid());

    yalvm_bytecode_t code = yalvm_bytecode_pack_two_registers(YALVM_BYTECODE_COPY_REGISTER,
                                                              var_register.registerIdx(),
                                                              tmp.registerIdx());
    _buffer.append(code);

    if (var_symbol->isGlobalVariable())
    {
        const yalvm_bytecode_inst_t inst = StoreGlobalByteCodeInst(var_symbol->astNode()->nodeType());

        yal_u32 global_idx = ModuleIndexable::IndexUnused;
        if(!getGlobalVarIdx(global_idx, variable_name))
        {
            logError(node);
            return;
        }

        code = yalvm_bytecode_pack_dst_value(inst, var_register.registerIdx(), global_idx);

        _buffer.append(code);
    }

    var_register.release(_regAllocator);
    tmp.release(_regAllocator);
}

void
ByteCodeGenerator::visit(DualOperatorNode& node)
{
    (void) node;


    // process left part
    node.leftExpression()->accept(*this);
    Register left_register = popRegister();
    YAL_ASSERT(left_register.isValid());

    if (didError())
    {
        return;
    }

    // process right part
    node.rightExpression()->accept(*this);
    Register right_register = popRegister();
    YAL_ASSERT(right_register.isValid());

    if (didError())
    {
        return;
    }


    Register cur_register = Register(_regAllocator);

    pushRegister(cur_register);

    const Type* exp_type = node.expressionResult().type;

    if (exp_type->isBuiltinType())
    {
        yalvm_bytecode_inst_t inst = DualOperatorByteCodeInst(node.dualOperatorType(),
                                                              exp_type);

        YAL_ASSERT(inst != YALVM_BYTECODE_TOTAL);

        yalvm_bytecode_t code = yalvm_bytecode_pack_three_registers(inst, cur_register.registerIdx(),
                                                                    left_register.registerIdx(),
                                                                    right_register.registerIdx());
        _buffer.append(code);
    }
    else
    {
        _formater.format("Dual operators for custom data types not yet implemented\n");
        logError(node);
        return;
    }

    left_register.release(_regAllocator);
    right_register.release(_regAllocator);
}

void
ByteCodeGenerator::visit(SingleOperatorNode& node)
{
    (void) node;

    node.expression()->accept(*this);

    // nothing to do for block operator
    if (!didError())
    {

        const Register& top = topRegister();

        const Type* exp_type = node.expressionResult().type;

        if (exp_type->isBuiltinType())
        {
            yalvm_bytecode_inst_t inst = SingeOperatorByteCodeInst(node.singleOperatorType(),
                                                                   exp_type);

            YAL_ASSERT(inst != YALVM_BYTECODE_TOTAL);

            yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(inst,
                                                                     top.registerIdx());

            _buffer.append(code);
        }
        else
        {
            _formater.format("Dual operators for custom data types not yet implemented\n");
            logError(node);
            return;
        }
    }
}

void
ByteCodeGenerator::visit(FunctionCallNode& node)
{

    const char* function_name = node.functionName();

    // load function symbol
    const Symbol* function_sym = node.scope()->resolveSymbol(function_name);
    if (!function_sym)
    {
        _formater.format("Could not find function symbol for '%s'\n",
                         function_name);
        logError(node);
        return;
    }

    YAL_ASSERT(function_sym->isFunction());


    yal_u32 function_idx = 0;
    if (!getFunctionIdx(function_idx, function_name))
    {
        _formater.format("Could not find function number for  '%s'\n",
                         function_name);
        logError(node);
        return;
    }

    Register function_call_reg = Register(_regAllocator);
    YAL_ASSERT(function_call_reg.isValid());

    const FunctionType* func_type = cast_type<FunctionType>(function_sym->astNode()->nodeType());
    Register return_register;
    if (func_type->hasReturnValue())
    {
        return_register = Register(_regAllocator);
        pushRegister(return_register);
    }

    yalvm_bytecode_t code_load = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_LOAD_FUNCTION,
                                                               function_call_reg.registerIdx(),
                                                               function_idx);
    _buffer.append(code_load);

    // process arguments
    if (node.hasFunctionArguments())
    {
        node.functionArguments()->accept(*this);
    }

    yalvm_bytecode_t code_call = yalvm_bytecode_pack_three_registers(YALVM_BYTECODE_CALL,
                                                                     function_call_reg.registerIdx(),
                                                                     return_register.registerIdx(),
                                                                     node.functionArgumentsCount());
    _buffer.append(code_call);

    function_call_reg.release(_regAllocator);
    return;
}

void
ByteCodeGenerator::visit(FunctionDeclNode& node)
{
    (void) node;
    _formater.format("Node '%s' Should not be reached\n", node.astTypeStr());
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
        node.condition()->accept(*this);
        tmp_register  = popRegister();
        YAL_ASSERT(tmp_register.isValid());

        // check of error
        if (didError())
        {
            return;
        }

        yalvm_bytecode_t condition_code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_JUMP_FALSE,
                                                                        tmp_register.registerIdx(),
                                                                        1);
        jump_to_false_offset  = _buffer.append(condition_code);
        YAL_ASSERT(jump_to_false_offset <  std::numeric_limits<yal_i32>::max());
        YAL_ASSERT(node.hasOnTrueComponent());
        tmp_register.release(_regAllocator);
    }

    onScopeBegin(*node.onTrue());
    node.onTrue()->accept(*this);
    onScopeEnd(*node.onTrue());

    if (node.hasConditionComponent())
    {
        // update previous value
        const size_t buffer_size = _buffer.size() - 1;
        YAL_ASSERT(buffer_size <  std::numeric_limits<yal_i32>::max());
        const yal_i32 current_offset = static_cast<yal_i32>(buffer_size);
        yal_i32 diff = current_offset - static_cast<yal_i32>(jump_to_false_offset);
        if (diff >= std::numeric_limits<yal_i16>::max())
        {
            _formater.format("Condition jump instruction exceeds maximum jump offset\n");
            logError(node);
            return;
        }
        _buffer.replace(jump_to_false_offset, yalvm_bytecode_pack_dst_value_signed(YALVM_BYTECODE_JUMP_FALSE,
                                                                                   tmp_register.registerIdx(),
                                                                                   diff));
    }

    if (node.hasOnFalseComponent())
    {
        // mark jump instruction
        const size_t jump_to_end_offset = _buffer.append(yalvm_bytecode_pack_value(YALVM_BYTECODE_JUMP, 0));
        YAL_ASSERT(jump_to_end_offset <  std::numeric_limits<yal_i32>::max());
        onScopeBegin(*node.onFalse());
        node.onFalse()->accept(*this);
        onScopeEnd(*node.onFalse());
        // check of error
        if (didError())
        {
            return;
        }

        // update jmp instruction
        const size_t buffer_size = _buffer.size() - 1;
        YAL_ASSERT(buffer_size <  std::numeric_limits<yal_i32>::max());
        const yal_i32 current_offset = static_cast<yal_i32>(buffer_size);
        const yal_i32 diff = current_offset - static_cast<yal_i32>(jump_to_end_offset);
        if (diff >= std::numeric_limits<yal_i16>::max())
        {
            _formater.format("Condition jump instruction exceeds maximum jump offset\n");
            logError(node);
            return;
        }

        _buffer.replace(jump_to_end_offset, yalvm_bytecode_pack_value_signed(YALVM_BYTECODE_JUMP,
                                                                             diff));
    }

    return;
}

void
ByteCodeGenerator::visit(ReturnNode& node)
{
    yal_u8 register_idx = 0xFF;
    if (node.expression())
    {
        node.expression()->accept(*this);
        Register tmp_register = popRegister();
        YAL_ASSERT(tmp_register.isValid());
        if (didError())
        {
            return;
        }

        register_idx = tmp_register.registerIdx();
        tmp_register.release(_regAllocator);

    }


    // run scope end actions
    node.scope()->onScopeExit(_scopeVisitor);

    const yalvm_bytecode_t code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_RETURN,
                                                                register_idx,
                                                                0);
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
        exp->accept(*this);
        Register tmp_register  = popRegister();
        YAL_ASSERT(tmp_register.isValid());

        const yalvm_bytecode_inst_t print_inst = PrintByteCodeInst(exp->expressionResult().type);
        const yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(print_inst,
                                                                       tmp_register.registerIdx());
        _buffer.append(code);

        tmp_register.release(_regAllocator);
    }

}

void
ByteCodeGenerator::visit(WhileLoopNode& node)
{

    Register tmp_register;
    const size_t jump_to_condition = _buffer.size();
    YAL_ASSERT(jump_to_condition <  std::numeric_limits<yal_i32>::max());

    node.condition()->accept(*this);
    tmp_register  = popRegister();
    YAL_ASSERT(tmp_register.isValid());


    // check of error
    if (didError())
    {
        return;
    }

    const yalvm_bytecode_t condition_code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_JUMP_FALSE,
                                                                          tmp_register.registerIdx(),
                                                                          1);
    const size_t jump_on_false_inst_loc = _buffer.append(condition_code);
    YAL_ASSERT(jump_on_false_inst_loc <  std::numeric_limits<yal_i32>::max());

    tmp_register.release(_regAllocator);

    // generate loop code
    onScopeBegin(*node.code());
    node.code()->accept(*this);
    onScopeEnd(*node.code());

    // check of error
    if (didError())
    {
        return;
    }

    // jump back to condition
    size_t buffer_size = _buffer.size();
    YAL_ASSERT(buffer_size <  std::numeric_limits<yal_i32>::max());
    yal_i32 current_offset = static_cast<yal_i32>(buffer_size);
    yal_i32 diff = static_cast<yal_i32>(jump_to_condition) - (current_offset + 1);
    if (diff <= std::numeric_limits<yal_i16>::min())
    {
        _formater.format("While loop jump instruction exceeds maximum jump offset\n");
        logError(node);
        return;
    }

    const yalvm_bytecode_t jump_code = yalvm_bytecode_pack_value_signed(YALVM_BYTECODE_JUMP,
                                                                        diff);
    buffer_size = _buffer.append(jump_code);

    // update on false jump
    YAL_ASSERT(buffer_size <  std::numeric_limits<yal_i32>::max());
    current_offset = static_cast<yal_i32>(buffer_size);
    diff = current_offset - static_cast<yal_i32>(jump_on_false_inst_loc);
    if (diff >= std::numeric_limits<yal_i16>::max())
    {
        _formater.format("Condition jump instruction exceeds maximum jump offset\n");
        logError(node);
        return;
    }
    _buffer.replace(jump_on_false_inst_loc, yalvm_bytecode_pack_dst_value_signed(YALVM_BYTECODE_JUMP_FALSE,
                                                                                 tmp_register.registerIdx(),
                                                                                 diff));

    return;
}

void
ByteCodeGenerator::visit(VariableAccessNode& node)
{

    const char* var_name = node.variableName();
    loadVariableIntoRegister(var_name, node);

}

void
ByteCodeGenerator::visit(StringCreateNode& node)
{
    // skip parsing constant node, load constant direcly in vm
    yal_u32 constant_idx;
    if (!getConstantIdx(constant_idx, node.constantNode()->constantValue()))
    {
        logError(node);
        return;
    }

    Register reg_string(_regAllocator);
    pushRegister(reg_string);

    yalvm_bytecode_t code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_STRING_CREATE,
                                                          reg_string.registerIdx(),
                                                          constant_idx);

    _buffer.append(code);
}


void
ByteCodeGenerator::ByteCodeGeneratorScopeActionVisitor::visitOnEnter(const ObjectScopeAction&)
{
    // do nothing
}

void
ByteCodeGenerator::ByteCodeGeneratorScopeActionVisitor::visitOnExit(const ObjectScopeAction& action)
{
    const Symbol* sym = action.symbol();
    const Type* sym_type = sym->astNode()->nodeType();
    YAL_ASSERT(sym->isVariable() && sym_type->isObjectType());

    //FIXME: Write type specific DTORS
    _generator.loadVariableIntoRegister(sym->symbolName(), *sym->astNode());

    Register var_reg = _generator.popRegister();

    const yalvm_bytecode_inst_t inst = sym_type->isStringType()
            ? YALVM_BYTECODE_STRING_RELEASE
            : YALVM_BYTECODE_OBJECT_RELEASE;

    const yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(inst,
                                                                   var_reg.registerIdx());

    _generator._buffer.append(code);
}

}
