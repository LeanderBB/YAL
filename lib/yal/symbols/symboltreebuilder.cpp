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
    for (auto& v : state.program)
    {
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
SymbolTreeBuilder::visit(AssignOperatorNode& node)
{
    node.setScope(currentScope());


    node.expressionLeft()->accept(*this);

    ExpressionResult left_exp_result = _expResult;

    if (!left_exp_result.symbol || (left_exp_result.symbol && !left_exp_result.symbol->isAssignable()))
    {
        throw SemanticException("Expression is not assignable", node);
    }


    // Catch implicit String creation

    ConstantNode* constant_node = ast_cast<ConstantNode>(node.expressionRight());
    if (constant_node && constant_node->constantType()->isStringContant())
    {
        node.replaceExpressionRight(new ObjectCreateNode( new StringCreateNode(constant_node)));
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
    node.setNodeType(left_type.type);
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

    _expResult = ExpressionResult(node.constantType());
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
                                                    &node,
                                                    Symbol::kFlagAssignable);
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

    FunctionType* func_type = cast_type<FunctionType>(_curFunctionCall->astNode()->nodeType());
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

    ExpressionNode* exp = node.expression();

    // Catch implicit String creation
    ConstantNode* constant_node = ast_cast<ConstantNode>(exp);
    if (constant_node && constant_node->constantType()->isStringContant())
    {
        node.replaceExpression(new ObjectCreateNode(new StringCreateNode(constant_node)));
    }

    node.expression()->accept(*this);

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
                                            &node,
                                            ((is_global_var) ? Symbol::kFlagGlobalSymbol : 0) | Symbol::kFlagAssignable);
    YAL_ASSERT(sym_var);
    node.setNodeType(_expResult.type);
    node.setExpressionResult(_expResult);

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

    node.setNodeType(_expResult.type);
    node.setExpressionResult(_expResult);
}

void
SymbolTreeBuilder::visit(FunctionCallNode& node)
{
    node.setScope(currentScope());
    const char* func_name = node.functionName();
    auto sym = _curScope->resolveSymbol(func_name);
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

    _curFunctionCall = sym;

    if (node.hasFunctionArguments())
    {
        node.functionArguments()->accept(*this);
    }

    sym->touchCall();

    const FunctionType* func_type = cast_type<FunctionType>(sym->astNode()->nodeType());
    YAL_ASSERT(func_type);
    _expResult = func_type->typeOfReturnValue();
    node.setExpressionResult(_expResult);
    _curFunctionCall = nullptr;
}

void
SymbolTreeBuilder::visit(FunctionDeclNode& node)
{
    node.setScope(currentScope());

    const char* func_name = node.functionName();
    auto sym = _curScope->resolveSymbol(func_name);
    if (sym)
    {
        std::stringstream stream;
        stream << "Symbol name '" << func_name
               << "' already delcared"
               << std::endl;
        throw SemanticException(stream.str(), node);
    }

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

    _curFunctionDecl = _curScope->declareSymbol(func_name, &node, 0);
    YAL_ASSERT(_curFunctionDecl);

    // begin new scope
    beginScope();

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
    const FunctionType* func_type = cast_type<FunctionType>(_curFunctionDecl->astNode()->nodeType());
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

    }

    node.setScope(currentScope());
    node.setNodeType(_expResult.type);
}

void
SymbolTreeBuilder::visit(WhileLoopNode& node)
{
    node.setScope(currentScope());

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
    node.arguments()->accept(*this);
}


void
SymbolTreeBuilder::visit(PrintArgsNode& node)
{
    node.setScope(currentScope());

    for(auto& v : node.expressions)
    {
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

    _expResult = ExpressionResult(sym->astNode()->nodeType(), sym);
    node.setNodeType(_expResult.type);
    node.setExpressionResult(_expResult);
    sym->touchRead();
}

void
SymbolTreeBuilder::visit(StringCreateNode& node)
{
    node.setScope(currentScope());
    node.constantNode()->accept(*this);
    _expResult = ExpressionResult(StringType::GetType());
    node.setNodeType(_expResult.type);
    node.setExpressionResult(_expResult.type);
}

void
SymbolTreeBuilder::visit(ObjectCreateNode& node)
{
     node.setScope(currentScope());
     node.expression()->accept(*this);
     node.setNodeType(_expResult.type);
     node.setExpressionResult(_expResult.type);
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

}
