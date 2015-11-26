#include "yal/bytecode/bytecodegenerator.h"

#include <cstdio>
#include <limits>
#include <sstream>

#include "yal/ast/asthdrs.h"
#include "yal/types/typehdrs.h"
#include "yal/symbols/scope.h"
#include "yal/symbols/objectscopeaction.h"
#include "yal/bytecode/bytecode_utils.h"
#include "yal/bytecode/bytecodegenexception.h"
#include "yal/symbols/symbol.h"

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
    YAL_ASSERT(isValid());
}

ByteCodeGenerator::Register::Register(const Symbol *sym,
                                      RegisterAllocator& alloctor):
    _registerIdx(alloctor.registerForVariable(sym->symbolName(),
                                              sym->scope()->level())),
    _isTemporary(false)
{
    YAL_ASSERT(isValid());
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
                                     TypeRegistry &typeRegistry):
    _moduleInfo(moduleInfo),
    _buffer(),
    _regAllocator(),
    _scopeVisitor(*this),
    _globalMap(),
    _conditionOffsets(),
    _trackConditionOffsets(false),
    _parenthesisOffsetStack(),
    _curParenthesisOffsets(nullptr),
    _typeRegistry(typeRegistry)
{

}

ByteCodeGenerator::~ByteCodeGenerator()
{

}

void
ByteCodeGenerator::generateFunction(FunctionDeclNode& node)
{
    reset();
    const bool has_return_value = setupFunction(node);

    if (!onScopeBegin(*node.functionCode()))
    {
        std::stringstream stream;
        stream << "Failed to open scope for function '"
               << node.functionName() << "'" << std::endl;
        throw ByteCodeGenException(stream.str(), node);
    }

    node.functionCode()->accept(*this);
    if (!onScopeEnd(*node.functionCode()))
    {
        std::stringstream stream;
        stream << "Failed to close scope for function '"
               << node.functionName() << "'" << std::endl;
        throw ByteCodeGenException(stream.str(), node);
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
}

bool
ByteCodeGenerator::generate(StatementNode &node)
{
    onStatementScopeBegin(node);
    node.accept(*this);
    onStatementScopeEnd(node);
    return true;
}

void
ByteCodeGenerator::reset()
{
    _globalMap.clear();
    _buffer.reset();
    _regAllocator.reset();
    while(!_registerStack.empty())
    {
        _registerStack.pop();
    }
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

bool
ByteCodeGenerator::setupFunction(const yal::FunctionDeclBaseNode &node)
{
    if (node.isObjectFunction())
    {
        // declare self variable register
        yal_i32 result = _regAllocator.allocate("self",
                                             node.functionArguments()->scope()->level());
        (void) result;
        YAL_ASSERT(result != RegisterAllocator::UnusedRegisterValue);
    }

    if (node.hasFunctionArguments())
    {
        // register function arguments to the first registers in
        // ascending order
        const ArgumentDeclsNode* func_args = node.functionArguments();
        for (auto& arg : func_args->arguments())
        {
            yal_i32 result = _regAllocator.allocate(arg->argumentName(), arg->scope()->level());
            (void) result;
            YAL_ASSERT(result != RegisterAllocator::UnusedRegisterValue);
        }
    }

    Symbol* func_sym = node.scope()->resolveSymbol(node.functionNameWithType());
    YAL_ASSERT(func_sym && func_sym->isFunction());

    FunctionType* fn_type = cast_type<FunctionType>(func_sym->symbolType());
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

void
ByteCodeGenerator::getGlobalVarIdx(yal_u32& idx,
                                   const char* varName)
{
    const ModuleGlobal* global_var = _moduleInfo.global(varName);
    if (!global_var)
    {
        std::stringstream stream;
        stream << "Global Index: could not find variable '"
               << varName<< "'" << std::endl;
        throw ByteCodeGenException(stream.str());
    }
    idx = global_var->moduleIndex();
    if (idx == ModuleIndexable::IndexUnused)
    {
        std::stringstream stream;
        stream << "Global Index: variable '"
               << varName<< "' has not been registered" << std::endl;
        throw ByteCodeGenException(stream.str());
    }
}

void
ByteCodeGenerator::getFunctionIdx(yal_u32& idx,
                                  const char* functionName)
{
    const ModuleFunction* function = _moduleInfo.function(functionName);
    if (!function)
    {
        std::stringstream stream;
        stream << "Global Index: could not find function '"
               << functionName<< "'" << std::endl;
        throw ByteCodeGenException(stream.str());
    }
    idx = function->moduleIndex();
    if (idx == ModuleIndexable::IndexUnused)
    {
        std::stringstream stream;
        stream << "Global Index: function '"
               << functionName<< "' has not been registered." << std::endl;
        throw ByteCodeGenException(stream.str());
    }
}

void ByteCodeGenerator::getConstantIdx(yal_u32& idx,
                                       const ConstantValue& val)
{
    const ModuleConstant* constant = _moduleInfo.constant(val);
    if (!constant)
    {
        std::stringstream stream;
        stream << "Global Index: Could not find a matching constant." << std::endl;
        throw ByteCodeGenException(stream.str());
    }
    idx = constant->moduleIndex();
    if (idx == ModuleIndexable::IndexUnused)
    {
        std::stringstream stream;
        stream << "Global Index: Constant has not been registered." << std::endl;
        throw ByteCodeGenException(stream.str());
    }
}

bool
ByteCodeGenerator::onScopeBegin(const AstBaseNode& node)
{
    const Scope* scope = node.scope();
    clearGlobalMap();
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

bool
ByteCodeGenerator::onStatementScopeBegin(const StatementNode& node)
{
    const StatementNode::SymScope_t& tmp_symbols = node.symbolScope();
    for(auto& symbol : tmp_symbols)
    {
        yal_i32 result = _regAllocator.allocate(symbol->symbolName(),
                                                symbol->scope()->level());
        if (result == RegisterAllocator::UnusedRegisterValue)
        {
            return false;
        }
    }
    return true;
}

bool
ByteCodeGenerator::onStatementScopeEnd(const StatementNode& node)
{
    const StatementNode::SymScope_t& tmp_symbols = node.symbolScope();
    for(auto& symbol : tmp_symbols)
    {
        if (symbol->symbolType()->isObjectType()
                && !symbol->isReturnValue() && !symbol->isAssigned())
        {
            const Register reg(_regAllocator, symbol->symbolName(),
                               symbol->scope()->level());
            releaseObject(*symbol, reg);
        }

        _regAllocator.deallocate(symbol->symbolName(),
                                 symbol->scope()->level());

    }
    return true;

}

void
ByteCodeGenerator::loadVariableIntoRegister(const char* varName,
                                            const AstBaseNode& node)
{
    const Scope* scope = node.scope();
    const SymbolTable& sym_table = scope->symbolTable();
    const Symbol* var_symbol = sym_table.resolveSymbol(varName);

    loadVariableIntoRegister(var_symbol, var_symbol->scope());
}


void
ByteCodeGenerator::loadVariableIntoRegister(const yal::Symbol *var_symbol,
                                            const Scope* scope)
{
    yal_u32 inst_value = 0;
    yalvm_bytecode_inst_t instruction = YALVM_BYTECODE_TOTAL;

    Register cur_register;

    const char* varName = var_symbol->symbolName();
    const bool var_is_global = var_symbol->isGlobalSymbol();
    if (var_is_global)
    {
        auto& reg = _globalMap[var_symbol->symbolName()];
        if (reg.isValid())
        {
            pushRegister(reg);
            return;
        }
        else
        {
            instruction = LoadGlobalByteCodeInst(var_symbol->symbolType());
            yal_i32 result = _regAllocator.allocate(varName, scope->level());
            (void) result;
            YAL_ASSERT(result != RegisterAllocator::UnusedRegisterValue);
            cur_register = Register(_regAllocator, varName, scope->level());
            getGlobalVarIdx(inst_value, varName);
            reg = cur_register;
        }
    }
    else if(var_symbol->isReference())
    {
        Register ref_register = Register(_regAllocator, varName,
                                         scope->level());
        cur_register = Register(_regAllocator);
        instruction = YALVM_BYTECODE_LOAD_REF;
        pushRegister(cur_register);

        yalvm_bytecode_t code = yalvm_bytecode_pack_two_registers(instruction,
                                                                  cur_register.registerIdx(),
                                                                  ref_register.registerIdx());
        _buffer.append(code);
        return;
    }
    else
    {
        cur_register = Register(_regAllocator, varName,
                                scope->level());
        if (!cur_register.isValid())
        {
            std::stringstream stream;
            stream << "Variable '"
            << varName<< "' has not been mapped to a register." << std::endl;
            throw ByteCodeGenException(stream.str());
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
ByteCodeGenerator::releaseObject(const Symbol &sym,
                                 const Register& reg)
{
    Type* node_type = sym.symbolType();

    class ObjectTypeVisitor : public TypeVisitor
    {
    public:
        ObjectTypeVisitor(ByteCodeGenerator& generator,
                          const Register& reg):
            _generator(generator),
            _register(reg)
        {

        }

        void visit(const StringType&) override
        {
            const yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(YALVM_BYTECODE_STRING_DEALLOC,
                                                                           _register.registerIdx());
            _generator._buffer.append(code);
        }

        void visit(const BuiltinType&) override
        {
            YAL_ASSERT(false && "Should not be reached");
        }

        void visit(const FunctionType&) override
        {
            YAL_ASSERT(false && "Should not be reached");
        }

        void visit(const UndefinedType&) override
        {
            YAL_ASSERT(false && "Should not be reached");
        }

        void visit(const ArrayType& t) override
        {
            const yalvm_bytecode_inst_t inst = t.valueType()->isObjectType()
                ? YALVM_BYTECODE_ARRAY_DEALLOC_OBJ
                : YALVM_BYTECODE_ARRAY_DEALLOC;
            const yalvm_bytecode_t code =
                yalvm_bytecode_pack_one_register(inst,
                                                 _register.registerIdx());
            _generator._buffer.append(code);
        }

        void visit(const AnyType&) override
        {
             YAL_ASSERT(false && "Should not be reached");
        }

    private:
        ByteCodeGenerator& _generator;
        const Register&  _register;

    } visitor(*this, reg);


    yalvm_bytecode_t code = 0;
    // add empty code and replace later on
    size_t code_offset = _buffer.append(code);
    // generate dtor code
    node_type->accept(visitor);
    // calculate difference
    size_t diff = _buffer.size() - (code_offset + 1);

    if (diff > std::numeric_limits<yal_u16>::max())
    {
        throw ByteCodeGenException("Object release code exceeds maximum jump call");
    }

    // replace instruction
    code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_OBJECT_RELEASE,
                                         reg.registerIdx(),
                                         static_cast<yal_u32>(diff));
    _buffer.replace(code_offset, code);
}

ByteCodeGenerator::Register
ByteCodeGenerator::registerForSymbol(const Symbol* sym)
{
    loadVariableIntoRegister(sym, sym->scope());
    return popRegister();
}

void
ByteCodeGenerator::clearGlobalMap()
{
    for (auto& v : _globalMap)
    {
        v.second.release(_regAllocator);
    }
    _globalMap.clear();
}

void
ByteCodeGenerator::handleConditionalOffset(const AstBaseNode& node,
                                           const size_t onTrueOffset,
                                           const size_t onFalseOffset)
{
    for(auto& offset: _conditionOffsets)
    {

        yal_u8 dst_reg;
        yal_i16 value;
        yalvm_bytecode_t code = _buffer.buffer()[offset];
        const yalvm_bytecode_inst_t inst = yalvm_bytecode_unpack_instruction(code);
        yalvm_bytecode_unpack_dst_value_signed(code, &dst_reg, &value);

        const size_t end_offset = (inst == YALVM_BYTECODE_JUMP_FALSE)
                ? onFalseOffset : onTrueOffset;

        const size_t diff = end_offset - offset - 1;

        if (diff >= std::numeric_limits<yal_i16>::max())
        {
            std::stringstream stream;
            stream << "Mid-Condition jump instruction exceeds maximum jump offset." << std::endl;
            throw ByteCodeGenException(stream.str(), node);
        }

        code = yalvm_bytecode_pack_dst_value_signed(inst, dst_reg,
                                                    static_cast<yal_i16>(diff));
        _buffer.replace(offset, code);
    }
    _conditionOffsets.clear();
}


void
ByteCodeGenerator::onParentesesEnter(ParentExpNode&)
{
    _parenthesisOffsetStack.push(ParenthesisOffsetVec_t());
    _curParenthesisOffsets = &_parenthesisOffsetStack.top();
}

void
ByteCodeGenerator::onParentesesExit(ParentExpNode& node)
{
    // perform cleanup

    for (auto& offset : *_curParenthesisOffsets)
    {

        yal_u8 dst_reg;
        yal_i16 value;
        yalvm_bytecode_t code = _buffer.buffer()[offset];
        const yalvm_bytecode_inst_t inst = yalvm_bytecode_unpack_instruction(code);
        yalvm_bytecode_unpack_dst_value_signed(code, &dst_reg, &value);

        const size_t diff = _buffer.size() - offset - 1;
        if (diff >= std::numeric_limits<yal_i16>::max())
        {
            std::stringstream stream;
            stream << "Parentheses jump instruction exceeds maximum jump offset." << std::endl;
            throw ByteCodeGenException(stream.str(), node);
        }

        code = yalvm_bytecode_pack_dst_value_signed(inst, dst_reg,
                                                    static_cast<yal_i16>(diff));
        _buffer.replace(offset, code);
    }

    // correct stack
    _parenthesisOffsetStack.pop();
    _curParenthesisOffsets = _parenthesisOffsetStack.empty()
            ? nullptr
            : &_parenthesisOffsetStack.top();
}

void
ByteCodeGenerator::visit(AssignOperatorNode& node)
{
    // process left part
    node.expressionLeft()->accept(*this);
    Register left_register = popRegister();
    YAL_ASSERT(left_register.isValid());

    // process right part
    node.expressionRight()->accept(*this);
    Register right_register = popRegister();
    YAL_ASSERT(right_register.isValid());

    const ExpressionResult& left_exp_result = node.expressionLeft()->expressionResult();


    if (left_exp_result.type->isObjectType())
    {
        // Remove reference and call dtor
        YAL_ASSERT(left_exp_result.symbol);
        releaseObject(*left_exp_result.symbol, left_register);
    }

    const ExpressionResult& right_exp_result = node.expressionRight()->expressionResult();
    if (right_exp_result.type->isObjectType() && !right_exp_result.symbol->isNewObject())
    {

        const yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(YALVM_BYTECODE_OBJECT_ACQUIRE,
                                                                       right_register.registerIdx());
        _buffer.append(code);
    }

    const ExpressionResult& exp_type = node.expressionResult();
    if (exp_type.type->isBuiltinType())
    {
        yalvm_bytecode_inst_t assign_inst =
                AssignOperatorByteCodeInst(node.assignOperatorType(), exp_type.type);
        yalvm_bytecode_t code;

        const Symbol* assign_sym = left_exp_result.symbol;
        Register ref_register;

        if (assign_sym->isReference())
        {
            ref_register = Register(_regAllocator, assign_sym->symbolName(),
                                    assign_sym->scope()->level());
        }

        if (assign_inst != YALVM_BYTECODE_COPY_REGISTER)
        {
            code = yalvm_bytecode_pack_three_registers(assign_inst, left_register.registerIdx(),
                                                       left_register.registerIdx(),
                                                       right_register.registerIdx());
            _buffer.append(code);

        }
        else if (left_exp_result.symbol != right_exp_result.symbol)
        {
            code = yalvm_bytecode_pack_two_registers(assign_inst, left_register.registerIdx(),
                                                     right_register.registerIdx());
            _buffer.append(code);
        }
        // store global result

        if (assign_sym->isGlobalVariable())
        {
            const yalvm_bytecode_inst_t inst = StoreGlobalByteCodeInst(exp_type.type);

            yal_u32 global_idx = ModuleIndexable::IndexUnused;
            getGlobalVarIdx(global_idx, assign_sym->symbolName());

            code = yalvm_bytecode_pack_dst_value(inst, left_register.registerIdx(), global_idx);

            _buffer.append(code);
        }
        else  if(assign_sym->isReference())
        {
            code = yalvm_bytecode_pack_two_registers(YALVM_BYTECODE_STORE_REF,
                                                     ref_register.registerIdx(),
                                                     left_register.registerIdx());
            _buffer.append(code);
        }
    }
    else
    {
        std::stringstream stream;
        stream << "Assignment of custom types not yet implemented" << std::endl;
        throw ByteCodeGenException(stream.str(), node);
    }


    const AstBaseNode* parent_node = node.parentNode();
    if (parent_node && (parent_node->isExpressionNode() || ast_typeof<ConditionNode>(parent_node)))
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
        onStatementScopeBegin(*v);
        v->accept(*this);
        onStatementScopeEnd(*v);
    }
}

void
ByteCodeGenerator::visit(CompareOperatorNode& node)
{

    // process left part
    node.expressionLeft()->accept(*this);
    Register left_register = popRegister();
    YAL_ASSERT(left_register.isValid());

    // process right part
    node.expressionRight()->accept(*this);
    Register right_register = popRegister();
    YAL_ASSERT(right_register.isValid());


    // create result register

    Register result_register = registerForSymbol(node.expressionResult().symbol);

    pushRegister(result_register);

    const Type* exp_type = node.expressionLeft()->expressionResult().type;
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
        std::stringstream stream;
        stream << "Comparison of custom types not yet implemented" << std::endl;
        throw ByteCodeGenException(stream.str(), node);
    }

    left_register.release(_regAllocator);
    right_register.release(_regAllocator);
}

void
ByteCodeGenerator::visit(ConstantNode& node)
{
    yal_u32 inst_value = 0;
    yalvm_bytecode_inst_t instruction = YALVM_BYTECODE_TOTAL;

    Register cur_register = registerForSymbol(node.expressionResult().symbol);

    const Type* exp_type = node.expressionResult().type;
    (void) exp_type;
    YAL_ASSERT(exp_type->isBuiltinType());
    switch(node.constantValue().type())
    {
    case kConstantTypeText:
    {
        getConstantIdx(inst_value, node.constantValue());
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
            getConstantIdx(inst_value, node.constantValue());
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
            getConstantIdx(inst_value, node.constantValue());
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
            getConstantIdx(inst_value, node.constantValue());
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
            getConstantIdx(inst_value, node.constantValue());
            instruction = YALVM_BYTECODE_LOAD_CONST_64;
        }
        break;
    }
    case kConstantTypeBool:
    {
        yal_bool value = node.constantValue().valueAsBoolean();
        inst_value =  (value) ? 1 : 0;
        instruction = YALVM_BYTECODE_LOAD_VALUE;
        break;
    }
    case kConstantTypeFloat32:
    {
        getConstantIdx(inst_value, node.constantValue());
        instruction = YALVM_BYTECODE_LOAD_CONST_32;
        break;
    }
    case kConstantTypeFloat64:
    {
        getConstantIdx(inst_value, node.constantValue());
        instruction = YALVM_BYTECODE_LOAD_CONST_64;
        break;
    }
    default:
        YAL_ASSERT(false && "should not be reached\n");
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
    YAL_ASSERT(false && "should not be reached");
}

void
ByteCodeGenerator::visit(ArgumentDeclsNode& node)
{
    (void) node;
    YAL_ASSERT(false && "should not be reached");
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

    loadVariableIntoRegister(variable_name, node);
    Register var_register = popRegister();

    if (!var_register.isValid())
    {
        std::stringstream stream;
        stream << "Variable '"
               << variable_name<< "' has not been mapped to a register." << std::endl;
        throw ByteCodeGenException(stream.str(), node);
    }

    node.expression()->accept(*this);
    const ExpressionResult& exp_result = node.expression()->expressionResult();

    Register tmp = popRegister();
    YAL_ASSERT(tmp.isValid());

    if (exp_result.type->isObjectType() && !exp_result.symbol->isNewObject())
    {

        const yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(YALVM_BYTECODE_OBJECT_ACQUIRE,
                                                                       tmp.registerIdx());
        _buffer.append(code);
    }

    yalvm_bytecode_t code;
    if (node.expression()->expressionResult().symbol != var_symbol)
    {

        code = yalvm_bytecode_pack_two_registers(YALVM_BYTECODE_COPY_REGISTER,
                                                 var_register.registerIdx(),
                                                 tmp.registerIdx());
        _buffer.append(code);
    }

    if (var_symbol->isGlobalVariable())
    {
        const yalvm_bytecode_inst_t inst = StoreGlobalByteCodeInst(var_symbol->symbolType());

        yal_u32 global_idx = ModuleIndexable::IndexUnused;
        getGlobalVarIdx(global_idx, variable_name);

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

    Register cur_register = registerForSymbol(node.expressionResult().symbol);

    // process left part
    node.expressionLeft()->accept(*this);
    Register left_register = popRegister();
    YAL_ASSERT(left_register.isValid());

    if (node.dualOperatorType() & (kOperatorTypeAnd | kOperatorTypeOr))
    {

        const yalvm_bytecode_inst_t inst = (node.dualOperatorType() == kOperatorTypeAnd)
                ? YALVM_BYTECODE_JUMP_FALSE
                : YALVM_BYTECODE_JUMP_TRUE;
        const yalvm_bytecode_t code =
                yalvm_bytecode_pack_dst_value_signed(inst, left_register.registerIdx(), 0);

        if(_curParenthesisOffsets)
        {
            // store result value in the register, currently needed for correct
            // conditional resultion.
            // TODO: Find a less hacky way to get this to work
            const yalvm_bytecode_t load_val =
                    yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_LOAD_VALUE,
                                                  cur_register.registerIdx(),
                                                  (node.dualOperatorType() == kOperatorTypeAnd)
                                                  ? 0
                                                  : 1);
            _buffer.append(load_val);

            const size_t offset = _buffer.append(code);
            _curParenthesisOffsets->push_back(offset);
        }
        else if (_trackConditionOffsets)
        {
            // insert empty code to be replace later on
            const size_t offset = _buffer.append(code);
            _conditionOffsets.push_back(offset);
        }
    }

    // process right part
    node.expressionRight()->accept(*this);
    Register right_register = popRegister();
    YAL_ASSERT(right_register.isValid());

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
        std::stringstream stream;
        stream << "Operators on custom data types not yet supported." << std::endl;
        throw ByteCodeGenException(stream.str(), node);
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

    const Register exp_result = popRegister();

    Register tmp_register = registerForSymbol(node.expressionResult().symbol);
    YAL_ASSERT(tmp_register.isValid());
    pushRegister(tmp_register);

    const Type* exp_type = node.expressionResult().type;

    if (exp_type->isBuiltinType())
    {
        yalvm_bytecode_inst_t inst = SingeOperatorByteCodeInst(node.singleOperatorType(),
                                                               exp_type);

        YAL_ASSERT(inst != YALVM_BYTECODE_TOTAL);

        yalvm_bytecode_t code = yalvm_bytecode_pack_two_registers(inst,
                                                                  tmp_register.registerIdx(),
                                                                  exp_result.registerIdx());

        _buffer.append(code);
    }
    else
    {
        std::stringstream stream;
        stream << "Operators on custom data types not yet supported." << std::endl;
        throw ByteCodeGenException(stream.str(), node);
    }
}

void
ByteCodeGenerator::visit(FunctionCallNode& node)
{
    std::string function_name = node.functionName();
    const Symbol* function_sym = nullptr;
    const Type* exp_type = nullptr;
    if (node.isObjectCall())
    {
        exp_type = node.objectExpression()->expressionResult().type;
        function_name = FunctionDeclBaseNode::GenFunctionName(exp_type, function_name.c_str());
        function_sym = node.scope()->resolveSymbol(function_name.c_str());

        // no matching function found, check builtin function
        if (!function_sym )
        {
            const char* builtin_function = exp_type->builtinFunctionSymName(node.functionName());
            if (builtin_function)
            {
                function_name = builtin_function;
                function_sym = node.scope()->resolveSymbol(builtin_function);
            }
        }
    }
    else
    {
        function_sym = node.scope()->resolveSymbol(function_name.c_str());
    }



    // load function symbol
    if (!function_sym)
    {
        std::stringstream stream;
        stream << "Could not find function symbol with name '"
               << function_name << "'" << std::endl;
        throw ByteCodeGenException(stream.str(), node);
    }

    YAL_ASSERT(function_sym->isFunction());


    yal_u32 function_idx = 0;
    getFunctionIdx(function_idx, function_name.c_str());

    Register function_call_reg = Register(_regAllocator);
    YAL_ASSERT(function_call_reg.isValid());

    const FunctionType* func_type = cast_type<FunctionType>(function_sym->symbolType());

    yalvm_bytecode_t code_load = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_LOAD_FUNCTION,
                                                               function_call_reg.registerIdx(),
                                                               function_idx);
    _buffer.append(code_load);


    // process call expression
    Register object_expression_reg;
    if (node.isObjectCall())
    {
        node.objectExpression()->accept(*this);
        object_expression_reg = popRegister();

        const yalvm_bytecode_inst_t inst = node.objectExpression()->expressionResult().type->isObjectType()
                ? YALVM_BYTECODE_PUSH_ARG
                : YALVM_BYTECODE_PUSH_REF;

        const yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(inst,
                                                                       object_expression_reg.registerIdx());
        _buffer.append(code);
    }

    // process arguments
    if (node.hasFunctionArguments())
    {
        node.functionArguments()->accept(*this);
    }

    Register return_register;
    if (func_type->hasReturnValue())
    {
        return_register = registerForSymbol(node.expressionResult().symbol);
        pushRegister(return_register);
    }

    yalvm_bytecode_t code_call = yalvm_bytecode_pack_three_registers(
                function_sym->isNativeFunction() ? YALVM_BYTECODE_CALL_NATIVE : YALVM_BYTECODE_CALL,
                function_call_reg.registerIdx(),
                return_register.registerIdx(),
                node.functionArgumentsCount());
    _buffer.append(code_call);

    function_call_reg.release(_regAllocator);

    if (node.isObjectCall() && node.objectExpression()->expressionResult().symbol->isGlobalVariable())
    {
        const Symbol* global_sym = node.objectExpression()->expressionResult().symbol;
        const yalvm_bytecode_inst_t inst = StoreGlobalByteCodeInst(global_sym->symbolType());
        yal_u32 global_idx = ModuleIndexable::IndexUnused;
        getGlobalVarIdx(global_idx, global_sym->symbolName());

        const yalvm_bytecode_t code = yalvm_bytecode_pack_dst_value(inst,
                                                                    object_expression_reg.registerIdx(),
                                                                    global_idx);
        _buffer.append(code);
    }
    clearGlobalMap();
    return;
}

void
ByteCodeGenerator::visit(FunctionDeclNode& node)
{
    (void) node;
    YAL_ASSERT(false && "Should not be reached");
}

void
ByteCodeGenerator::visit(ConditionNode& node)
{
    Register tmp_register;
    size_t jump_to_false_offset = 0;
    size_t abs_on_true_offset = 0;
    size_t abs_on_false_offset = 0;
    if (node.hasConditionComponent())
    {
        _trackConditionOffsets = true;
        onStatementScopeBegin(*node.condition());
        node.condition()->accept(*this);
        onStatementScopeEnd(*node.condition());
        _trackConditionOffsets = false;
        tmp_register  = popRegister();
        YAL_ASSERT(tmp_register.isValid());

        yalvm_bytecode_t condition_code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_JUMP_FALSE,
                                                                        tmp_register.registerIdx(),
                                                                        1);
        jump_to_false_offset  = _buffer.append(condition_code);
        YAL_ASSERT(jump_to_false_offset <  std::numeric_limits<yal_i32>::max());
        YAL_ASSERT(node.hasOnTrueComponent());
        tmp_register.release(_regAllocator);
    }

    abs_on_true_offset = _buffer.size();
    onScopeBegin(*node.onTrue());
    node.onTrue()->accept(*this);
    onScopeEnd(*node.onTrue());

    if (node.hasConditionComponent())
    {
        // update previous value
        const size_t buffer_size = _buffer.size() - (node.hasOnFalseComponent() ? 0 : 1);
        YAL_ASSERT(buffer_size <  std::numeric_limits<yal_i32>::max());
        const yal_i32 current_offset = static_cast<yal_i32>(buffer_size);
        yal_i32 diff = current_offset - static_cast<yal_i32>(jump_to_false_offset);
        if (diff >= std::numeric_limits<yal_i16>::max())
        {
            std::stringstream stream;
            stream << "Condition jump (true) instruction exceeds maximum jump offset." << std::endl;
            throw ByteCodeGenException(stream.str(), node);
        }
        _buffer.replace(jump_to_false_offset, yalvm_bytecode_pack_dst_value_signed(YALVM_BYTECODE_JUMP_FALSE,
                                                                                   tmp_register.registerIdx(),
                                                                                   diff));
        abs_on_false_offset = _buffer.size();
    }

    if (node.hasOnFalseComponent())
    {
        // mark jump instruction
        const size_t jump_to_end_offset = _buffer.append(yalvm_bytecode_pack_value(YALVM_BYTECODE_JUMP, 0));
        YAL_ASSERT(jump_to_end_offset <  std::numeric_limits<yal_i32>::max());
        abs_on_false_offset = _buffer.size();
        onScopeBegin(*node.onFalse());
        node.onFalse()->accept(*this);
        onScopeEnd(*node.onFalse());


        // update jmp instruction
        const size_t buffer_size = _buffer.size() - 1;
        YAL_ASSERT(buffer_size <  std::numeric_limits<yal_i32>::max());
        const yal_i32 current_offset = static_cast<yal_i32>(buffer_size);
        const yal_i32 diff = current_offset - static_cast<yal_i32>(jump_to_end_offset);
        if (diff >= std::numeric_limits<yal_i16>::max())
        {
            std::stringstream stream;
            stream << "Condition jump (false) instruction exceeds maximum jump offset." << std::endl;
            throw ByteCodeGenException(stream.str(), node);
        }

        _buffer.replace(jump_to_end_offset, yalvm_bytecode_pack_value_signed(YALVM_BYTECODE_JUMP,
                                                                             diff));
    }

    // update any conditional jumps
    if (node.hasConditionComponent())
    {
        handleConditionalOffset(node,abs_on_true_offset, abs_on_false_offset);
    }

    clearGlobalMap();
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

        register_idx = tmp_register.registerIdx();
        tmp_register.release(_regAllocator);

        const ExpressionResult& exp_result = node.expression()->expressionResult();
        if (exp_result.type->isObjectType()
                && !exp_result.symbol->isNewObject())
        {
            const yalvm_bytecode_t code = yalvm_bytecode_pack_one_register(YALVM_BYTECODE_OBJECT_ACQUIRE,
                                                                           register_idx);
            _buffer.append(code);
        }

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

    onStatementScopeBegin(*node.condition());
    node.condition()->accept(*this);
    onStatementScopeEnd(*node.condition());
    tmp_register  = popRegister();
    YAL_ASSERT(tmp_register.isValid());

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

    // jump back to condition
    size_t buffer_size = _buffer.size();
    YAL_ASSERT(buffer_size <  std::numeric_limits<yal_i32>::max());
    yal_i32 current_offset = static_cast<yal_i32>(buffer_size);
    yal_i32 diff = static_cast<yal_i32>(jump_to_condition) - (current_offset + 1);
    if (diff <= std::numeric_limits<yal_i16>::min())
    {
        std::stringstream stream;
        stream << "Condition jump (while onFalse) instruction exceeds maximum jump offset." << std::endl;
        throw ByteCodeGenException(stream.str(), node);
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
        std::stringstream stream;
        stream << "Condition jump (while onLoop) instruction exceeds maximum jump offset." << std::endl;
        throw ByteCodeGenException(stream.str(), node);
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
    getConstantIdx(constant_idx, node.constantNode()->constantValue());

    Register reg_string = registerForSymbol(node.expressionResult().symbol);
    pushRegister(reg_string);

    yalvm_bytecode_t code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_STRING_ALLOC,
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
    YAL_ASSERT(sym->isVariable() && sym->symbolType()->isObjectType());
    _generator.loadVariableIntoRegister(sym, sym->scope());
    Register var_reg = _generator.popRegister();
    _generator.releaseObject(*sym, var_reg);
}

void
ByteCodeGenerator::visit(ObjectCreateNode&)
{
    YAL_ASSERT(false && "Should not be reached");
}

void
ByteCodeGenerator::visit(ObjectRetainNode&)
{
    YAL_ASSERT(false && "Should not be reached");
}

void
ByteCodeGenerator::visit(ObjectReleaseNode&)
{
    YAL_ASSERT(false && "Should not be reached");
}

void
ByteCodeGenerator::visit(FunctionDeclNativeNode&)
{
    YAL_ASSERT(false && "Should not be reached");
}

void ByteCodeGenerator::visit(ParentExpNode& node)
{
    onParentesesEnter(node);
    node.expression()->accept(*this);
    onParentesesExit(node);
}

void
ByteCodeGenerator::visit(ArrayCtrNode& node)
{
    const ArrayType* type = cast_type<const ArrayType>(node.nodeType());
    YAL_ASSERT(type && "ArracyCtrNode must have array type");
    const Type* value_type = type->valueType();

    Register cur_register = registerForSymbol(node.expressionResult().symbol);
    YAL_ASSERT(cur_register.isValid());


    yalvm_bytecode_inst_t alloc_inst = YALVM_BYTECODE_TOTAL, insert_inst = YALVM_BYTECODE_TOTAL;
    if (value_type->is32BitSized())
    {
        alloc_inst = YALVM_BYTECODE_ARRAY_ALLOC_32;
        insert_inst = YALVM_BYTECODE_ARRAY_PUT_32;
    }
    else if (value_type->is64BitSized())
    {
        alloc_inst = YALVM_BYTECODE_ARRAY_ALLOC_64;
        insert_inst = YALVM_BYTECODE_ARRAY_PUT_64;
    }
    else
    {
        YAL_ASSERT(value_type->isObjectType());
        insert_inst = YALVM_BYTECODE_ARRAY_PUT_OBJ;
        alloc_inst = YALVM_BYTECODE_ARRAY_ALLOC_OBJ;
    }



    const ExpressionNodeVec_t& expressions = node.expressions();
    if (node.expressions().empty())
    {
        // alloc empty array
        yalvm_bytecode_t code = yalvm_bytecode_pack_two_registers(alloc_inst,
                                                                  cur_register.registerIdx(),
                                                                  0xFF);
        _buffer.append(code);
    }
    else
    {
        // alloc size array
        if (expressions.size() >= std::numeric_limits<yal_u16>::max())
        {
            std::stringstream stream;
            stream << "Maxium allowed expressions for in place array construction exceeds the limit: "
                   << "current=" << expressions.size() << " max="
                   << std::numeric_limits<yal_u16>::max() << std::endl;
            throw ByteCodeGenException(stream.str(), node);
        }

        Register tmp_reg(_regAllocator);
        YAL_ASSERT(tmp_reg.isValid());
        yalvm_bytecode_t code = yalvm_bytecode_pack_dst_value(YALVM_BYTECODE_LOAD_VALUE,
                                                              tmp_reg.registerIdx(),
                                                              static_cast<yalvm_u16>(expressions.size()));
        tmp_reg.release(_regAllocator);
        _buffer.append(code);

        code = yalvm_bytecode_pack_two_registers(alloc_inst,
                                                 cur_register.registerIdx(),
                                                 tmp_reg.registerIdx());
        _buffer.append(code);

        // generate expressions
        for (auto& exp : expressions)
        {
            exp->accept(*this);
            Register exp_reg = popRegister();
            YAL_ASSERT(exp_reg.isValid());

            code = yalvm_bytecode_pack_three_registers(insert_inst,
                                                       cur_register.registerIdx(),
                                                       0xFF,
                                                       exp_reg.registerIdx());
            _buffer.append(code);
        }
    }

    pushRegister(cur_register);
}

}
