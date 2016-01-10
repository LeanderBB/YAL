#include "yal/symbols/symboltreebuilder.h"
#include "yal/ast/asthdrs.h"
#include "yal/types/typehdrs.h"
#include "yal/symbols/objectscopeaction.h"
#include "yal/parser/parser_state.h"
#include "yal/symbols/semanticexception.h"
#include <cstdio>

namespace yal
{
SymbolTreeBuilder::SymbolTreeBuilder(TypeRegistry &typeRegistry):
    AstNodeVisitor(),
    _parserState(nullptr),
    _globalScope(nullptr, 0),
    _curScope(&_globalScope),
    _curFunctionDecl(nullptr),
    _curFunctionCall(nullptr),
    _typeRegistry(typeRegistry)
{

}

SymbolTreeBuilder::~SymbolTreeBuilder()
{

}

void
SymbolTreeBuilder::process(ParserState& state)
{
    _parserState = &state;

    addYalVmFunctions();

    for (auto& v : state.program)
    {
        _curStatment = v.get();
        v->accept(*this);
    }
}

void
SymbolTreeBuilder::beginScope()
{
    _scopeStack.push(_curScope);
    Scope* tmp = _curScope;
    _curScope = tmp->createChildScope();
}

void
SymbolTreeBuilder::endScope()
{
    if (!_scopeStack.empty())
    {
        _curScope = _scopeStack.top();
        _scopeStack.pop();
    }
    else
    {
        YAL_ASSERT(false && "Should not happend, empty stack!!!");
    }
}

bool
SymbolTreeBuilder::currentScopeIsGlobalScope() const
{
    return _curScope == &_globalScope;
}

void
SymbolTreeBuilder::addYalVmFunctions()
{
    ArrayType::RegisterBuiltinFunctions(_parserState->module, _typeRegistry, _globalScope);
}

void
SymbolTreeBuilder::visit(AssignOperatorNode& node)
{
    node.setScope(currentScope());


    node.expressionLeft()->accept(*this);

    ExpressionResult left_exp_result = _expResult;

    if (!left_exp_result.symbol || (left_exp_result.symbol && !left_exp_result.symbol->isAssignable()))
    {
        throw SemanticException("Expression is not assignable", node);
    }

    node.expressionRight()->accept(*this);

    // validate types
    if (!_expResult.type->isPromotableTo(left_exp_result.type))
    {
        std::stringstream stream;
        stream <<"Cannot convert right expression to left, expression type "
              << _expResult.type->typeString()
              <<" cannot be cast to "
             << left_exp_result.type->typeString()
             << std::endl;
        throw SemanticException(stream.str(), node);
    }

    _expResult.symbol->markAssigned();

    if (node.assignOperatorType() == kOperatorTypeCopy
            && !left_exp_result.type->isObjectType()
            && left_exp_result.symbol->isTemporary())
    {
        _curStatment->removeSymbolFromScope(_expResult.symbol);
        _curScope->eraseSymbol(_expResult.symbol);
        node.expressionRight()->setExpressionResult(ExpressionResult(_expResult.type, left_exp_result.symbol));
    }
    node.setNodeType(left_exp_result.type);
    node.setExpressionResult(left_exp_result);

    left_exp_result.symbol->touchWrite();
}

void
SymbolTreeBuilder::visit(CodeBodyNode& node)
{
    node.setScope(currentScope());
    for(auto& v : node.statements)
    {
        _curStatment = v.get();
        v->accept(*this);
    }
}

void
SymbolTreeBuilder::visit(CompareOperatorNode& node)
{
    node.setScope(currentScope());
    node.expressionLeft()->accept(*this);

    ExpressionResult left_type = _expResult;

    node.expressionRight()->accept(*this);

    // evaluate right type
    ExpressionResult right_type = _expResult;
    if (!right_type.type->isPromotableTo(left_type.type))
    {
        std::stringstream stream;
        stream <<"Cannot convert right expression to left, expression type "
              << _expResult.type->typeString()
              <<" cannot be cast to "
             << left_type.type->typeString()
             << std::endl;
        throw SemanticException(stream.str(), node);
    }

    _expResult = left_type;

    Symbol* tmp_sym = _curScope->declareTemporarySymbol(_expResult.type);
    YAL_ASSERT(tmp_sym);
    _curStatment->addSymbolToScope(tmp_sym);

    _expResult = ExpressionResult(BuiltinType::GetBuiltinType(BuiltinType::kBool), tmp_sym);
    node.setNodeType(_expResult.type);
    node.setExpressionResult(_expResult);
}

void
SymbolTreeBuilder::visit(ConstantNode& node)
{
    node.setScope(currentScope());
    // check module if a constant that matches this value is available
    if (!node.constantValue().valueFitsInByteCode())
    {
        ModuleConstant* mod_constant = _parserState->module.constant(node.constantValue());
        if (!mod_constant)
        {
            _parserState->module.addConstant(new ModuleConstant(node.constantValue()));
        }
    }

    Symbol* tmp_sym = _curScope->declareTemporarySymbol(node.constantType());
    YAL_ASSERT(tmp_sym);
    _curStatment->addSymbolToScope(tmp_sym);
    _expResult = ExpressionResult(node.constantType(), tmp_sym);
    node.setNodeType(_expResult.type);
    node.setExpressionResult(_expResult);
}

void
SymbolTreeBuilder::visit(ArgumentDeclNode& node)
{
    node.setScope(currentScope());
    if (node.isCustomType())
    {
        throw SemanticException("Custom types not yet supported!!!\n", node);
    }

    YAL_ASSERT(_curFunctionDecl);

    auto sym = _curScope->resolveSymbol(node.argumentName());
    if (sym && sym->scope() == _curScope)
    {
        std::stringstream stream;
        stream <<"Variable '"
              << node.argumentName()
              << "' has already been declared as an argument"
              << std::endl;
        throw SemanticException(stream.str(), node);
    }

    Type* arg_data_type = node.argumentType();
    if (node.isCustomType())
    {
        /*sym = _curScope->resolveSymbol(node.argumentId());
        if (!sym)
        {
            _formater.format("Could not find type '%s'\n",
                             node.argumentId());
            logError(node);
            return;
        }

        if (sym->isVariable())
        {
            _formater.format("Symbol type '%s' cannot be a variable name\n",
                             node.argumentId());
            logError(node);
            return;
        }

        arg_data_type = sym->astNode()->nodeType();*/
    }

    const Symbol* result = _curScope->declareSymbol(node.argumentName(),
                                                    arg_data_type,
                                                    Symbol::kFlagAssignable | Symbol::kFlagFunctionParam | Symbol::kFlagVariable);
    (void) result;
    YAL_ASSERT(result);
    node.setNodeType(arg_data_type);
}

void
SymbolTreeBuilder::visit(ArgumentDeclsNode& node)
{
    if (node.arguments().size() > kMaxFunctionArgs)
    {
        std::stringstream stream;
        stream <<"Function '"
              <<_curFunctionCall->symbolName()
             << "' exceeds maximum arg count ("
             << kMaxFunctionArgs << ")"
             << std::endl;
        throw SemanticException(stream.str(), node);
    }

    node.setScope(currentScope());
    for(auto& v : node.arguments())
    {
        v->accept(*this);
    }
}

void
SymbolTreeBuilder::visit(FunctionCallArgsNode& node)
{
    YAL_ASSERT(_curFunctionCall);
    node.setScope(currentScope());

    FunctionType* func_type = cast_type<FunctionType>(_curFunctionCall->symbolType());
    YAL_ASSERT(func_type);
    const yal_u32 nargs = func_type->argumentCount();
    if (nargs != node.expressions.size())
    {
        std::stringstream stream;
        stream <<"Function '"
              <<_curFunctionCall->symbolName()
             << "' expects"<< nargs << "arguments, you have provided "
             << node.expressions.size()
             << std::endl;
        throw SemanticException(stream.str(), node);
    }

    yal_u32 idx = 0;
    for(auto& v : node.expressions)
    {
        v->accept(*this);
        const Type* arg_type = func_type->typeOfArgument(idx);
        if (!_expResult.type->isPromotableTo(arg_type))
        {
            std::stringstream stream;
            stream <<"Function '"
                  <<_curFunctionCall->symbolName()
                 << "' argument "<< idx << "expects type '"
                 << arg_type->typeString() << "', but you have provided type '"
                 << _expResult.type->typeString() << "'"
                 << std::endl;
            throw SemanticException(stream.str(), node);
        }
        ++idx;
    }
}

void
SymbolTreeBuilder::visit(VariableDeclNode& node)
{
    node.setScope(currentScope());
    const char* var_name = node.variableName();
    auto sym = _curScope->resolveSymbol(var_name);

    if (sym)
    {
        if (currentScope() == sym->scope())
        {
            std::stringstream stream;
            stream <<"Symbol '"
                  << var_name
                  << "' has already been declared in the current scope"
                  << std::endl;
            throw SemanticException(stream.str(), node);
        }
    }

    node.expression()->accept(*this);
    const ExpressionResult& exp_result = node.expression()->expressionResult();

    if (node.hasExplicitType())
    {
        const Type* explicit_type = node.explicitType();
        if (!_expResult.type->isPromotableTo(explicit_type))
        {
            std::stringstream stream;
            stream <<"Cannot convert expression type to the variable's explicit type: "
                  << _expResult.type->typeString()
                  <<" can not be cast to "
                 << explicit_type->typeString()
                 << std::endl;
            throw SemanticException(stream.str(), node);
        }
    }

    // check if there is a return type
    if (_expResult.type->isVoidType())
    {
        std::stringstream stream;
        stream <<"Cannot assign void to variable '"
              << var_name << "'"
              << std::endl;
        throw SemanticException(stream.str(), node);
    }

    const bool is_global_var = currentScopeIsGlobalScope();
    auto sym_var = _curScope->declareSymbol(var_name,
                                            _expResult.type,
                                            ((is_global_var) ? Symbol::kFlagGlobalSymbol : 0) | Symbol::kFlagAssignable | Symbol::kFlagVariable);
    YAL_ASSERT(sym_var);
    node.setNodeType(_expResult.type);
    _expResult.symbol->markAssigned();
    node.setExpressionResult(ExpressionResult(_expResult.type, sym_var));


    if (!exp_result.type->isObjectType() && exp_result.symbol->isTemporary())
    {

        _curStatment->removeSymbolFromScope(exp_result.symbol);
        _curScope->eraseSymbol(exp_result.symbol);
        node.expression()->setExpressionResult(ExpressionResult(_expResult.type, sym_var));
    }

    // register with module global
    if (is_global_var)
    {
        YAL_ASSERT(_parserState->module.global(var_name) == nullptr);
        _parserState->module.addGlobal(new ModuleGlobal(sym_var));
    }

    if (node.nodeType()->isObjectType())
    {
        currentScope()->addScopeAction(new ObjectScopeAction(sym_var));
    }

}

void
SymbolTreeBuilder::visit(DualOperatorNode& node)
{

    node.setScope(currentScope());
    node.expressionLeft()->accept(*this);
    const OperatorType op_type = node.dualOperatorType();

    ExpressionResult cur_result= _expResult;

    if (!cur_result.type->acceptsOperator(op_type))
    {
        std::stringstream stream;
        stream << "Type '" << cur_result.type->typeString()
               << "' does not accept opertor '"
               << OperatorTypeToStr(op_type) << "'"
               << std::endl;
        throw SemanticException(stream.str(), node);
    }

    node.expressionRight()->accept(*this);

    const bool requires_integer = OperatorRequiresInteger(op_type);
    if (requires_integer && !_expResult.type->isInteger())
    {
        std::stringstream stream;
        stream << "Operator '" << OperatorTypeToStr(op_type)
               << "' requires that right expression has an integer result"
               << std::endl;
        throw SemanticException(stream.str(), node);
    }


    const Type* exp_type = cur_result.type;
    if (op_type == kOperatorTypeArray)
    {
        ArrayType* array_type = cast_type<ArrayType>(cur_result.type);
        if (!array_type)
        {
            std::stringstream stream;
            stream <<"Invalid use of array operator on type '"
                  << cur_result.type->typeString() << "'" << std::endl;
            throw SemanticException(stream.str(), node);
        }

        exp_type = array_type->valueType();
    }
    else
    {
        if (!_expResult.type->isPromotableTo(cur_result.type))
        {
            std::stringstream stream;
            stream <<"Cannot convert right expression to left, expression type "
                  << _expResult.type->typeString()
                  <<" cannot be cast to "
                 << cur_result.type->typeString()
                 << std::endl;
            throw SemanticException(stream.str(), node);
        }
    }

    Symbol* tmp_sym = _curScope->declareTemporarySymbol(const_cast<Type*>(exp_type),
                                                        exp_type->isObjectType() ? Symbol::kFlagNewObject : 0);
    _curStatment->addSymbolToScope(tmp_sym);

    _expResult = ExpressionResult(const_cast<Type*>(exp_type), tmp_sym);
    node.setNodeType(_expResult.type);
    node.setExpressionResult(_expResult);
}

void
SymbolTreeBuilder::visit(SingleOperatorNode& node)
{
    node.setScope(currentScope());
    node.expression()->accept(*this);

    const OperatorType op_type = node.singleOperatorType();
    const bool requires_signed_int = OperatorRequiresSignedType(op_type);
    const bool requires_integer = OperatorRequiresInteger(op_type);


    if (requires_signed_int && !_expResult.type->isSignedType())
    {
        std::stringstream stream;
        stream << "Operator '" << OperatorTypeToStr(node.singleOperatorType())
               << "' requires that right expression has a signed type"
               << std::endl;
        throw SemanticException(stream.str(), node);
    }

    if (requires_integer && !_expResult.type->isInteger())
    {
        std::stringstream stream;
        stream << "Operator '" << OperatorTypeToStr(node.singleOperatorType())
               << "' requires that right expression has an integer result"
               << std::endl;
        throw SemanticException(stream.str(), node);
    }

    Symbol* tmp_sym = _curScope->declareTemporarySymbol(_expResult.type,
                                                        _expResult.type->isObjectType() ? Symbol::kFlagNewObject : 0);
    _curStatment->addSymbolToScope(tmp_sym);

    _expResult = ExpressionResult(_expResult.type, tmp_sym);

    node.setNodeType(_expResult.type);
    node.setExpressionResult(_expResult);
}

void
SymbolTreeBuilder::visit(FunctionCallNode& node)
{
    node.setScope(currentScope());
    std::string func_name = FunctionDeclBaseNode::GenFunctionName(nullptr, node.functionName());
    Symbol* sym = nullptr;
    if (node.isObjectCall())
    {
        node.objectExpression()->accept(*this);
        const Type* exp_type = node.objectExpression()->expressionResult().type;
        func_name = FunctionDeclBaseNode::GenFunctionName(exp_type, func_name.c_str());
        sym = _curScope->resolveSymbol(func_name.c_str());

        // no matching function found, check builtin function
        if (!sym)
        {
            const char* builtin_function = exp_type->builtinFunctionSymName(node.functionName());
            if (builtin_function)
            {
                func_name = builtin_function;
                sym = _curScope->resolveSymbol(builtin_function);
            }
        }
    }
    else
    {
        sym = _curScope->resolveSymbol(func_name.c_str());
    }

    if (!sym)
    {
        std::stringstream stream;
        stream << "Symbol '" << func_name
               << "' is undefined"
               << std::endl;
        throw SemanticException(stream.str(), node);
    }

    if (!sym->isFunction())
    {
        std::stringstream stream;
        stream << "Symbol '" << func_name
               << "' is not a function"
               << std::endl;
        throw SemanticException(stream.str(), node);
    }

    const FunctionType* func_type = cast_type<FunctionType>(sym->symbolType());

    if (node.isObjectCall())
    {
        if (!func_type->isObjectFunction())
        {
            std::stringstream stream;
            stream << "Can not call function '" << func_name
                   << "' as an object function type"
                   << std::endl;
            throw SemanticException(stream.str(), node);
        }

        const Type* object_type = func_type->typeOfObject();
        const Type* exp_type = node.objectExpression()->expressionResult().type;
        if (!exp_type->isPromotableTo(object_type))
        {
            std::stringstream stream;
            stream << "Can not call function '" << func_name
                   << "', can not promote type '" << exp_type->typeString()
                   << "' to type '" << object_type->typeString() << "'"
                   << std::endl;
            throw SemanticException(stream.str(), node);
        }
    }

    _curFunctionCall = sym;

    if (node.hasFunctionArguments())
    {
        node.functionArguments()->accept(*this);
    }

    sym->touchCall();


    YAL_ASSERT(func_type);
    Type* return_type = func_type->typeOfReturnValue();

    Symbol* tmp_sym = _curScope->declareTemporarySymbol(return_type, return_type->isObjectType() ? Symbol::kFlagNewObject : 0);
    _curStatment->addSymbolToScope(tmp_sym);

    _expResult = ExpressionResult(return_type, tmp_sym);
    node.setExpressionResult(_expResult);
    _curFunctionCall = nullptr;
    node.setNodeType(return_type);
}

void
SymbolTreeBuilder::visit(FunctionDeclBaseNode& node)
{
    node.setScope(currentScope());

    const char* func_name = node.functionNameWithType();
    auto sym = _curScope->resolveSymbol(func_name);
    if (sym)
    {
        std::stringstream stream;
        stream << "Symbol name '" << func_name
        << "' already delcared"
        << std::endl;
        throw SemanticException(stream.str(), node);
    }

    // check for object type
    if (node.isObjectFunction())
    {
        Type* object_type = node.objectType();
        if (object_type->isUndefined())
        {
            std::stringstream stream;
            stream << "Function's object type '"
            << object_type->typeString() << "'"
            << "for function '" <<func_name
            << "' is undefined."
            << std::endl;
            throw SemanticException(stream.str(), node);
        }

        const char* builtin_function = object_type->builtinFunctionSymName(node.functionName());
        if (builtin_function)
        {
            std::stringstream stream;
            stream << "Can not declare function '"
            << func_name << "'"
            << " for type '" <<object_type->typeString()
            << "' since it conflicts with a builtin function."
            << std::endl;
            throw SemanticException(stream.str(), node);
        }
    }



}

void
SymbolTreeBuilder::visit(FunctionDeclNode& node)
{
    visit(static_cast<FunctionDeclBaseNode&>(node));

    const char* func_name = node.functionNameWithType();

    // declare func
    FunctionType* type = _typeRegistry.registerFunction(&node);
    if (!type)
    {
        std::stringstream stream;
        stream << "Failed to register '" << func_name
        << "'"
        << std::endl;
        throw SemanticException(stream.str(), node);
    }

    _curFunctionDecl = _curScope->declareSymbol(func_name, type, 0);
    YAL_ASSERT(_curFunctionDecl);

    // begin new scope
    beginScope();


    // check for object type
    if (node.isObjectFunction())
    {
        Type* object_type = node.objectType();
        Symbol* self_sym = _curScope->declareSymbol("self", object_type,
                                                    (!object_type->isObjectType() ? Symbol::kFlagReference  : 0)| Symbol::kFlagAssignable | Symbol::kFlagVariable);
        (void) self_sym;
        YAL_ASSERT(self_sym);
    }


    node.functionArguments()->accept(*this);


    node.functionCode()->accept(*this);

    // register function in module
    YAL_ASSERT(_parserState->module.function(func_name) == nullptr);
    _parserState->module.addFunction(new ModuleFunction(_curFunctionDecl,
                                                        &node));

    // end scope
    endScope();
    _curFunctionDecl = nullptr;
}

void
SymbolTreeBuilder::visit(ConditionNode& node)
{
    node.setScope(currentScope());

    if (node.hasConditionComponent())
    {
        _curStatment = node.condition();
        node.condition()->accept(*this);
        // check condition expression
        if (!_expResult.type->isPromotableToBoolean())
        {
            throw SemanticException("Condition expression can not be promoted to boolean\n",
                                    node);
        }
    }

    if (node.hasOnTrueComponent())
    {
        // begin new scope
        beginScope();
        node.onTrue()->accept(*this);
        endScope();
    }

    if (node.hasOnFalseComponent())
    {
        // begin new scope
        beginScope();
        node.onFalse()->accept(*this);
        endScope();
    }
}

void
SymbolTreeBuilder::visit(ReturnNode& node)
{
    if (!_curFunctionDecl)
    {
        throw SemanticException("Return statement outside function scope\n",
                                node);
    }

    const char* function_name = _curFunctionDecl->symbolName();
    const FunctionType* func_type = cast_type<FunctionType>(_curFunctionDecl->symbolType());
    const Type* func_return = func_type->typeOfReturnValue();
    // current return type

    if (!func_return->isVoidType() && !node.expression())
    {
        std::stringstream stream;
        stream <<"Function '"
              << function_name
              << "' has return type '"
              << func_return->typeString()
              << "', but we are not returning any" << std::endl;
        throw SemanticException(stream.str(), node);
    }


    if (node.expression())
    {
        if (func_return->isVoidType())
        {
            std::stringstream stream;
            stream <<"Function '"
                  << function_name
                  << "' has return type '"
                  << func_return->typeString()
                  << "', but we are returning type '"
                  << _expResult.type->typeString() <<"'" <<  std::endl;
            throw SemanticException(stream.str(), node);
        }

        node.expression()->accept(*this);

        // check if the return types match
        if(!_expResult.type->isPromotableTo(func_return))
        {
            std::stringstream stream;
            stream <<"Function '"
                  << function_name
                  << "' has return type '"
                  << func_return->typeString()
                  << "', but we are returning type '"
                  << _expResult.type->typeString() <<"'" <<  std::endl;
            throw SemanticException(stream.str(), node);
        }
        _expResult.symbol->markReturnValue();
    }

    node.setScope(currentScope());
    node.setNodeType(_expResult.type);
}

void
SymbolTreeBuilder::visit(WhileLoopNode& node)
{
    node.setScope(currentScope());

    _curStatment = &node;
    node.condition()->accept(*this);
    // check condition expression
    if (!_expResult.type->isPromotableToBoolean())
    {
        throw SemanticException("Condition expression can not be promoted to boolean\n",
                                node);
    }

    // begin new scope
    beginScope();
    node.code()->accept(*this);
    endScope();
}

void
SymbolTreeBuilder::visit(PrintNode& node)
{
    _curStatment = &node;
    node.arguments()->accept(*this);
}


void
SymbolTreeBuilder::visit(PrintArgsNode& node)
{
    node.setScope(currentScope());

    for(auto& v : node.expressions)
    {
        // optimize for strings
        StringCreateNode* str_node = ast_cast<StringCreateNode>(v.get());
        if (str_node)
        {
            v = (std::move(str_node->constantNodePtr()));
            v->setParentNode(&node);
        }
        v->accept(*this);
    }
}

void
SymbolTreeBuilder::visit(VariableAccessNode& node)
{
    node.setScope(currentScope());
    const char* symbol_name = node.variableName();
    auto sym = _curScope->resolveSymbol(symbol_name);
    if (!sym)
    {
        std::stringstream stream;
        stream << "Symbol '" << symbol_name
               << "' is undefined"
               << std::endl;
        throw SemanticException(stream.str(), node);
    }

    if (!sym->isVariable())
    {
        std::stringstream stream;
        stream << "Symbol '" << symbol_name
               << "' is not a variable"
               << std::endl;
        throw SemanticException(stream.str(), node);
    }

    _expResult = ExpressionResult(sym->symbolType(), sym);
    node.setNodeType(_expResult.type);
    node.setExpressionResult(_expResult);
    sym->touchRead();
}

void
SymbolTreeBuilder::visit(StringCreateNode& node)
{
    node.setScope(currentScope());
    node.constantNode()->accept(*this);
    Symbol* tmp_sym = _curScope->declareTemporarySymbol(StringType::GetType(), Symbol::kFlagNewObject);
    _curStatment->addSymbolToScope(tmp_sym);
    _expResult = ExpressionResult(StringType::GetType(), tmp_sym);
    node.setNodeType(_expResult.type);
    node.setExpressionResult(_expResult);
}

void
SymbolTreeBuilder::visit(ObjectCreateNode&)
{
    YAL_ASSERT(false && "Should not be reached");
}

void
SymbolTreeBuilder::visit(ObjectRetainNode&)
{
    YAL_ASSERT(false && "Should not be reached");
}

void
SymbolTreeBuilder::visit(ObjectReleaseNode&)
{
    YAL_ASSERT(false && "Should not be reached");
}

void
SymbolTreeBuilder::visit(FunctionDeclNativeNode& node)
{
    visit(static_cast<FunctionDeclBaseNode&>(node));

    const char* func_name = node.functionNameWithType();

    // declare func
    FunctionType* type = _typeRegistry.registerFunction(&node);
    if (!type)
    {
        std::stringstream stream;
        stream << "Failed to register '" << func_name
               << "'"
               << std::endl;
        throw SemanticException(stream.str(), node);
    }

    _curFunctionDecl = _curScope->declareSymbol(func_name, type, 0);
    YAL_ASSERT(_curFunctionDecl);

    beginScope();

    if (node.hasFunctionArguments())
    {
        node.functionArguments()->accept(*this);
    }

    // register function in module
    YAL_ASSERT(_parserState->module.function(func_name) == nullptr);
    _parserState->module.addFunction(new ModuleFunction(_curFunctionDecl,
                                                              &node));

    // end scope
    endScope();
    _curFunctionDecl = nullptr;
}

void
SymbolTreeBuilder::visit(ParentExpNode& node)
{
    node.setScope(currentScope());
    node.expression()->accept(*this);
}

void
SymbolTreeBuilder::visit(ArrayCtrNode& node)
{
    node.setScope(currentScope());
    ExpressionNodeVec_t& expressions = node.expressions();

    // check if we have explicit type
    if (expressions.empty() && !node.hasExplicitType())
    {
        VariableDeclNode* parent_node = ast_cast<VariableDeclNode>(node.parentNode());

        if (!parent_node || (parent_node && !parent_node->explicitType()->isArrayType()))
        {
            throw SemanticException("Can not construct empty array without explicity specifying the array's value type\n", node);
        }
        else
        {
            ArrayType* array_type = cast_type<ArrayType>(parent_node->explicitType());
            YAL_ASSERT(array_type);
            node.setExplicitType(array_type);
        }
    }

    const ArrayType* array_type = node.explicitType();
    const Type* value_type = array_type ? array_type->valueType() : nullptr;
    // handle array expressions if any
    if (!expressions.empty())
    {
        size_t idx = 0;
        for(auto& exp : expressions)
        {
            ++idx;
            exp->accept(*this);
            const ExpressionResult& result = exp->expressionResult();
            // if array type is not set, use the first element of the array to
            // determine the type
            if (!array_type)
            {
                array_type = _parserState->registry.registerArray(result.type);
                value_type = array_type->valueType();
                continue;
            }

            if (!result.type->isPromotableTo(value_type))
            {
                std::stringstream stream;

                stream << "Expression's (" << idx << ") type can not be promoted to array type: ";
                stream << "can not convert '"<< result.type->typeString() << "' to '"
                       << value_type->typeString() << "'" << std::endl;
                throw SemanticException(stream.str(), *exp);
            }

        }
    }

    // array type should be set by now
    YAL_ASSERT(array_type);

    Symbol* tmp_sym = _curScope->declareTemporarySymbol(const_cast<ArrayType*>(array_type), Symbol::kFlagNewObject);
    YAL_ASSERT(tmp_sym);
    _curStatment->addSymbolToScope(tmp_sym);

    _expResult = ExpressionResult(const_cast<ArrayType*>(array_type), tmp_sym);
    node.setNodeType(const_cast<ArrayType*>(array_type));
    node.setExpressionResult(_expResult);
}

void
SymbolTreeBuilder::visit(UserTypeMemberNode& node)
{
    (void) node;
}

void
SymbolTreeBuilder::visit(UserTypeMembersNode& node)
{
    (void) node;
}

void
SymbolTreeBuilder::visit(UserTypeDeclNode& node)
{
    (void) node;
}

}
