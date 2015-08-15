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
#include "yal/ast/whileloopnode.h"
#include <cstdio>
#include <limits>

namespace yal
{


class ByteCodeGenerator::IScopeAction
{
public:
    virtual ~IScopeAction(){}

    virtual bool onScopeEnter(ByteCodeGenerator& generator) = 0;

    virtual bool onScopeExit(ByteCodeGenerator& generator) = 0;

    virtual bool runScopeExitOnReturn() const = 0;
};

class ByteCodeGenerator::GlobalScopeAction : public ByteCodeGenerator::IScopeAction
{
public:

    GlobalScopeAction(const char* globalName,
                      const ConstantType globalType,
                      const yal_u32 globalIdx,
                      Register reg):
        _global(globalName),
        _reg(reg),
        _globalType(globalType),
        _globalIdx(globalIdx),
        _shouldRelease(true)
    {
        _reg.disablePopRelease();
    }

    bool onScopeEnter(ByteCodeGenerator& generator) override
    {
        if (generator._globalToLocalMap.insert(std::make_pair(_global, _reg)).second == false)
        {
            _shouldRelease = false;
        }
        return true;
    }

    bool onScopeExit(ByteCodeGenerator& generator) override
    {
        if (_shouldRelease)
        {
            _reg.release(generator._regAllocator);
            generator._globalToLocalMap.erase(_global);

            const yalvm_bytecode_inst_t inst = StoreGlobalByteCodeInst(_globalType);

            const yalvm_bytecode_t code = yalvm_bytecode_pack_dst_value(inst,
                                                                        _reg.registerIdx(),
                                                                        _globalIdx);
            generator._buffer.append(code);
        }
        return true;
    }

    bool runScopeExitOnReturn() const override
    {
        return true;
    }

protected:
    const char* _global;
    Register _reg;
    const ConstantType _globalType;
    const yal_u32 _globalIdx;
    bool _shouldRelease;
};


ByteCodeGenerator::Register::Register():
    _registerIdx(RegisterAllocator::UnusedRegisterValue),
    _isTemporary(false),
    _popRelease(true)
{
}

ByteCodeGenerator::Register::Register(RegisterAllocator& alloctor,
                                      const char* variable,
                                      const yal_u32 scopeLevel):
    _registerIdx(alloctor.registerForVariable(variable, scopeLevel)),
    _scopeLevel(scopeLevel),
    _isTemporary(false),
    _popRelease(false)
{

}

ByteCodeGenerator::Register::Register(RegisterAllocator& alloctor):
    _registerIdx(alloctor.allocateTemporary()),
    _isTemporary(true),
    _popRelease(true)
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

void
ByteCodeGenerator::Register::disablePopRelease()
{
    _popRelease = false;
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
            bool result = _regAllocator.allocate(arg->argumentName(), arg->symbolTable()->level());
            (void) result;
            YAL_ASSERT(result != RegisterAllocator::UnusedRegisterValue);
        }
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
    _scopeActions.push(ScopeActionVecPtr_t(new ScopeActionVec_t()));
    return registerScope(node.symbolTable());
}

bool
ByteCodeGenerator::onScopeEnd(const AstBaseNode& node)
{
    bool result = true;
    if (!_scopeActions.empty())
    {
        result = runScopeEndActions();
        _scopeActions.pop();
        unregisterScope(node.symbolTable());
    }
    return result;
}

bool
ByteCodeGenerator::onScopeBeginGlobal()
{
    _scopeActions.push(ScopeActionVecPtr_t(new ScopeActionVec_t()));
    return true;
}

bool
ByteCodeGenerator::onScopeEndGlobal()
{
    bool result = true;
    if (!_scopeActions.empty())
    {
        result = runScopeEndActions();
        _scopeActions.pop();
    }
    return result;
}

bool
ByteCodeGenerator::runScopeEndActions(const bool exitCauseByReturn)
{
    ScopeActionVecPtr_t& actions = _scopeActions.top();
    for(auto it = actions->rbegin(); it != actions->rend(); ++it)
    {
        if (exitCauseByReturn && !it->get()->runScopeExitOnReturn())
        {
            continue;
        }
        if (!it->get()->onScopeExit(*this))
        {
            return false;
        }
    }
    return true;
}

bool
ByteCodeGenerator::addScopeAction(ByteCodeGenerator::IScopeAction* action)
{
    if (_scopeActions.empty())
    {
        return false;
    }

    ScopeActionVecPtr_t& actions = _scopeActions.top();

    if (action->onScopeEnter(*this))
    {
        actions->push_back(ScopeActionPtr_t(action));
        return true;
    }
    return false;
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
        const auto global_cache_it = _globalToLocalMap.find(var_symbol->symbolName());
        if (global_cache_it != _globalToLocalMap.end())
        {
            var_register = global_cache_it->second;
        }
        else
        {
            var_register = Register(_regAllocator);
            if(!getGlobalVarIdx(global_idx, variable_name))
            {
                logError(node);
                return;
            }

            var_register.disablePopRelease();
            // register the global variable with the cache
            if (!addScopeAction(new GlobalScopeAction(var_symbol->symbolName(),
                                                      var_symbol->returnType(),
                                                      global_idx,
                                                      var_register)))
            {
                _formater.format("Could not add global variable scope action");
                logError(node);
                return;
            }

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

    pushRegister(var_register);
    node.expression()->accept(*this);
    Register tmp_register = popRegister();
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

    if (inst != YALVM_BYTECODE_COPY_REGISTER)
    {
        code = yalvm_bytecode_pack_three_registers(inst, var_register.registerIdx(),
                                                   var_register.registerIdx(),
                                                   tmp_register.registerIdx());
    }
    else
    {
        code = yalvm_bytecode_pack_two_registers(inst, var_register.registerIdx(),
                                                 tmp_register.registerIdx());
    }

    _buffer.append(code);

    if (tmp_register.popRelease())
    {
        tmp_register.release(_regAllocator);
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

    yalvm_bytecode_inst_t inst = CompareOperatorByteCodeInst(node.compareOperatorType(),
                                                             node.typeInfo());

    YAL_ASSERT(inst != YALVM_BYTECODE_TOTAL);

    yalvm_bytecode_t code = yalvm_bytecode_pack_three_registers(inst, result_register.registerIdx(),
                                                                left_register.registerIdx(),
                                                                right_register.registerIdx());
    _buffer.append(code);

    if (left_register.popRelease())
    {
        left_register.release(_regAllocator);
    }
    if (right_register.popRelease())
    {
        right_register.release(_regAllocator);
    }
}

void
ByteCodeGenerator::visit(ConstantNode& node)
{
    yal_u32 inst_value = 0;
    yalvm_bytecode_inst_t instruction = YALVM_BYTECODE_TOTAL;

    Register cur_register = Register(_regAllocator);

    switch(node.constantType())
    {
    case kConstantTypeId:
    {
        const char* var_name = node.constantValue().valueAsId();

        const SymbolTable* sym_table = node.symbolTable();

        Symbol* symbol = sym_table->resolveSymbol(var_name);
        const VariableSym* var_symbol = symbol_cast<VariableSym>(symbol);

        YAL_ASSERT(var_symbol);

        const bool var_is_global = var_symbol->isGlobal();
        if (var_is_global)
        {
            // check if the global variable is cached in a local register
            const auto global_cache_it = _globalToLocalMap.find(var_symbol->symbolName());
            if (global_cache_it != _globalToLocalMap.end())
            {
                cur_register.release(_regAllocator);
                pushRegister(global_cache_it->second);
                return;
            }
            else
            {
                if (!getGlobalVarIdx(inst_value, var_name))
                {
                    logError(node);
                    return;
                }
                cur_register.disablePopRelease();

                instruction = LoadGlobalByteCodeInst(var_symbol->returnType());
                // register the global variable with the cache
                if (!addScopeAction(new GlobalScopeAction(var_symbol->symbolName(),
                                                          var_symbol->returnType(),
                                                          inst_value,
                                                          cur_register)))
                {
                    _formater.format("Could not add global variable scope action");
                    logError(node);
                    return;
                }

                break;
            }
        }
        else
        {
            cur_register.release(_regAllocator);
            cur_register = Register(_regAllocator, var_name,
                                           var_symbol->scopeLevel());
            if (!cur_register.isValid())
            {
                _formater.format("Variable '%s' has not been registered\n", var_name);
                logError(node);
                return;
            }
            pushRegister(cur_register);
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
        exp->accept(*this);
        Register tmp_register  = popRegister();
        YAL_ASSERT(tmp_register.isValid());
        yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(YALVM_BYTECODE_PUSH_ARG,
                                                                 tmp_register.registerIdx());
        _buffer.append(code);
        if (tmp_register.popRelease())
        {
            tmp_register.release(_regAllocator);
        }
    }

}

void
ByteCodeGenerator::visit(VariableDeclNode& node)
{
    node.acceptBase(*this);
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

    yalvm_bytecode_inst_t inst = DualOperatorByteCodeInst(node.dualOperatorType(),
                                                          node.typeInfo());

    YAL_ASSERT(inst != YALVM_BYTECODE_TOTAL);

    yalvm_bytecode_t code = yalvm_bytecode_pack_three_registers(inst, cur_register.registerIdx(),
                                                                left_register.registerIdx(),
                                                                right_register.registerIdx());
    _buffer.append(code);

    if (left_register.popRelease())
    {
        left_register.release(_regAllocator);
    }

    if (right_register.popRelease())
    {
        right_register.release(_regAllocator);
    }
}

void
ByteCodeGenerator::visit(SingleOperatorNode& node)
{
    (void) node;

    node.expression()->accept(*this);

    // nothing to do for block operator
    if (!didError() && node.singleOperatorType() != kSingleOperatorTypeBlock)
    {

        const Register& top = topRegister();

        yalvm_bytecode_inst_t inst = SingeOperatorByteCodeInst(node.singleOperatorType(),
                                                               node.typeInfo());

        YAL_ASSERT(inst != YALVM_BYTECODE_TOTAL);

        yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(inst,
                                                                 top.registerIdx());

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

    Register function_call_reg = Register(_regAllocator);
    YAL_ASSERT(function_call_reg.isValid());

    Register return_register;
    if (function_sym->hasReturnValue())
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

        if (tmp_register.popRelease())
        {
            tmp_register.release(_regAllocator);
        }
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
        if (tmp_register.popRelease())
        {
            tmp_register.release(_regAllocator);
        }
    }

    if (!runScopeEndActions(true))
    {
        _formater.format("Failed to run scope exit actions");
        logError(node);
        return;
    }

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

        const yalvm_bytecode_inst_t print_inst = PrintByteCodeInst(exp->returnTypeInfo());
        const yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(print_inst,
                                                                       tmp_register.registerIdx());
        _buffer.append(code);

        if(tmp_register.popRelease())
        {
            tmp_register.release(_regAllocator);
        }

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

    if (tmp_register.popRelease())
    {
        tmp_register.release(_regAllocator);
    }

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

}
