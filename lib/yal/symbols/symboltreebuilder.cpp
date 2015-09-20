#include "yal/symbols/symboltreebuilder.h"
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
#include "yal/symbols/type_utils.h"
#include "yal/parser/parser_state.h"
#include "yal/ast/printnode.h"
#include "yal/ast/whileloopnode.h"
#include "yal/types/typeregistry.h"
#include "yal/types/functiontype.h"
#include "yal/types/builtintype.h"

#include <cstdio>

namespace yal
{
SymbolTreeBuilder::SymbolTreeBuilder(ErrorHandler &errHandler,
                                     TypeRegistry &typeRegistry):
    AstNodeVisitor(),
    _parserState(nullptr),
    _globalSym(nullptr, 0),
    _curScope(&_globalSym),
    _curFunctionDecl(nullptr),
    _curFunctionCall(nullptr),
    _errHandler(errHandler),
    _typeRegistry(typeRegistry),
    _scopeCounter(0),
    _error(false)
{

}

SymbolTreeBuilder::~SymbolTreeBuilder()
{

}

bool
SymbolTreeBuilder::process(ParserState& state)
{
    _parserState = &state;
    for (auto& v : state.program)
    {
        v->accept(*this);
        if (didError())
        {
            break;
        }
    }

    return !didError();
}

void
SymbolTreeBuilder::beginScope()
{
    _scopeStack.push(_curScope);
    SymbolTable* tmp = _curScope;
    _curScope = new SymbolTable(tmp, ++_scopeCounter);
    tmp->addChild(_curScope);
}

void
SymbolTreeBuilder::endScope()
{
    if (!_scopeStack.empty())
    {
        --_scopeCounter;
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
    return _curScope == &_globalSym;
}

void
SymbolTreeBuilder::logError(const AstBaseNode& node)
{
    (void) node;
    const SourceLocationInfo& loc = node.locationInfo();
    _errHandler.onCompileError(_formater.str(), _formater.strLen(), loc);
    _error = true;
}

void
SymbolTreeBuilder::visit(AssignOperatorNode& node)
{
    node.setSymbolTable(currentScope());
    // check if var has been declared
    const char* variable_name = node.variableName();
    Symbol* sym = _curScope->resolveSymbol(variable_name);
    if (!sym)
    {
        _formater.format("Symbol '%s' has not been declared\n", variable_name);
        logError(node);
        return;
    }

    if (!sym->isAssignable())
    {
        _formater.format("Cannot assign expression to symbol '%s', since it is not assignable \n", variable_name);
        logError(node);
        return;
    }

    const AstBaseNode* ast_node = sym->astNode();
    Type* var_type = ast_node->nodeType();
    node.expression()->accept(*this);

    // validate types
    if (!didError())
    {
        if (_expResult->isPromotableTo(var_type))
        {
            _formater.format("Cannot assign expression to symbol '%s', expression type %s cannot be cast to %s\n",
                             variable_name,
                             _expResult->typeString(),
                             var_type->typeString());
            logError(node);
            return;
        }

        node.setNodeType(var_type);
        node.setExpressionType(var_type);
    }

    sym->touchWrite();
}

void
SymbolTreeBuilder::visit(CodeBodyNode& node)
{
    node.setSymbolTable(currentScope());
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
SymbolTreeBuilder::visit(CompareOperatorNode& node)
{
    node.setSymbolTable(currentScope());
    node.leftExpression()->accept(*this);

    Type* left_type = _expResult;

    if (!didError())
    {
        node.rightExpression()->accept(*this);
    }

    // evaluate right type
    Type* right_type = _expResult;
    if (right_type->isPromotableTo(left_type))
    {
        _formater.format("Cannot convert right type to left type\n");
        logError(node);
        return;
    }

    _expResult = left_type;
    node.setTypeInfo(left_type, left_type);
}

void
SymbolTreeBuilder::visit(ConstantNode& node)
{
    node.setSymbolTable(currentScope());
    const ConstantType const_type = node.constantValue().type();
    if (const_type == kConstantTypeId)
    {
        const char* symbol_name = node.constantValue().valueAsId();
        auto sym = _curScope->resolveSymbol(symbol_name);
        if (!sym)
        {
            _formater.format("Symbol '%s' has not been declared\n", symbol_name);
            logError(node);
            return;
        }

        if (sym->isVariable())
        {
            _formater.format("Symbol '%s' is not a variable \n", symbol_name);
            logError(node);
            return;
        }

        _expResult = sym->astNode()->nodeType();
        node.setTypeInfo(_expResult, _expResult);
        sym->touchRead();
    }
    else
    {
        // check module if a constant that matches this value is available
        if (!node.constantValue().valueFitsInByteCode())
        {
            ModuleConstant* mod_constant = _parserState->module.constant(node.constantValue());
            if (!mod_constant)
            {
                _parserState->module.addConstant(new ModuleConstant(node.constantValue()));
            }
        }

        _expResult = node.constantType();
        node.setTypeInfo(_expResult, _expResult);
    }
}

void
SymbolTreeBuilder::visit(ArgumentDeclNode& node)
{
    node.setSymbolTable(currentScope());
    if (node.argumentId())
    {
        _formater.format("Custom types not yet supported!!!\n");
        logError(node);
    }

    YAL_ASSERT(_curFunctionDecl);

    auto sym = _curScope->resolveSymbol(node.argumentName());
    if (sym && sym->scope() == _curScope)
    {
        _formater.format("Variable '%s' has already been declared as an argument\n",
                         node.argumentName());
        logError(node);
        return;
    }

    const ConstantType arg_type = node.argumentType();
    Type* arg_data_type;
    if (arg_type == kConstantTypeId)
    {
        sym = _curScope->resolveSymbol(node.argumentId());
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

        arg_data_type = sym->astNode()->nodeType();
    }
    else
    {
        arg_data_type = BuiltinType::GetBuiltinTypeForConstantType(arg_type);
    }

    _curScope->declareSymbol(new Symbol(node.argumentName(),
                                        currentScope()->level(),
                                        &node,
                                        Symbol::kFlagAssignable));
    node.setNodeType(arg_data_type);
}

void
SymbolTreeBuilder::visit(ArgumentDeclsNode& node)
{
    if (node.arguments().size() > kMaxFunctionArgs)
    {
        _formater.format("Function '%s' exceeds maximum arg count (%u)\n",
                         _curFunctionCall->symbolName(), kMaxFunctionArgs);
        logError(node);
        return;
    }

    node.setSymbolTable(currentScope());
    for(auto& v : node.arguments())
    {
        if (!didError())
        {
            v->accept(*this);
        }
    }
}

void
SymbolTreeBuilder::visit(FunctionCallArgsNode& node)
{
    YAL_ASSERT(_curFunctionCall);
    node.setSymbolTable(currentScope());

    FunctionType* func_type = cast_type<FunctionType>(_curFunctionCall->astNode()->nodeType());
    YAL_ASSERT(func_type);
    const yal_u32 nargs = func_type->argumentCount();
    if (nargs != node.expressions.size())
    {
        _formater.format("Function '%s' expects %u arguments, you have provided %u\n",
                         _curFunctionCall->symbolName(), nargs,
                         static_cast<yal_u32>(node.expressions.size()));
        logError(node);
        return;
    }

    yal_u32 idx = 0;
    for(auto& v : node.expressions)
    {
        if (!didError())
        {
            v->accept(*this);
            const Type* arg_type = func_type->typeOfArgument(idx);
            if (!_expResult->isPromotableTo(arg_type))
            {
                _formater.format("Function '%s' argument %u expects type %s, but has %s\n",
                                 _curFunctionCall->symbolName(), idx,
                                 arg_type->typeString(),
                                 _expResult->typeString());
                logError(node);
                return;
            }
        }
        else
        {
            return;
        }
        ++idx;
    }
}

void
SymbolTreeBuilder::visit(VariableDeclNode& node)
{
    node.setSymbolTable(currentScope());
    const char* var_name = node.variableName();
    auto sym = _curScope->resolveSymbol(var_name);

    if (sym)
    {
        if (currentScope()->level() == sym->scope()->level())
        {
            _formater.format("Symbol '%s' has already been declared in the current scope\n",var_name);
            logError(node);
            return;
        }

       /* if (!sym->isVariable())
        {
            _formater.format("Symbol '%s' is not a variable\n",var_name);
            logError(node);
            return;
        }*/

    }

    node.expression()->accept(*this);

    // check if there is a return type
    if (_expResult->isVoidType())
    {
        _formater.format("Cannot assign void to variable '%s'\n", var_name);
        logError(node);
    }

    if (!didError())
    {
        const bool is_global_var = currentScopeIsGlobalScope();
        auto sym_var = new Symbol(var_name,
                                  currentScope()->level(),
                                  &node,
                                  ((is_global_var) ? Symbol::kFlagGlobalSymbol : 0) | Symbol::kFlagAssignable);
        _curScope->declareSymbol(sym_var);
        node.setTypeInfo(_expResult, _expResult);

        // register with module global
        if (is_global_var)
        {
            YAL_ASSERT(_parserState->module.global(var_name) == nullptr);
            _parserState->module.addGlobal(new ModuleGlobal(sym_var));
        }
    }
}

void
SymbolTreeBuilder::visit(DualOperatorNode& node)
{

    node.setSymbolTable(currentScope());
    node.leftExpression()->accept(*this);
    const OperatorType op_type = node.dualOperatorType();

    Type* cur_result_type = _expResult;

    if (!cur_result_type->acceptsOperator(op_type))
    {
        _formater.format("'%s' does not accept opertor '%s'\n",
                         OperatorTypeToStr(op_type));
        logError(node);
        return;
    }

    if (!didError())
    {
        node.rightExpression()->accept(*this);
    }

    const bool requires_integer = OperatorRequiresUnsignedInt(op_type);
    if (requires_integer && !_expResult->isUnsignedInteger())
    {
        _formater.format("Operator '%s' requires that right expression has an unsigned integer result\n",
                         OperatorTypeToStr(op_type));
        logError(node);
        return;
    }

    if (_expResult->isPromotableTo(cur_result_type))
    {
        _formater.format("Can not promote '%s' to '%s\n",
                         _expResult->typeString(), cur_result_type->typeString());
        logError(node);
        return;
    }

    node.setTypeInfo(_expResult, _expResult);
}

void
SymbolTreeBuilder::visit(SingleOperatorNode& node)
{
    node.setSymbolTable(currentScope());
    node.expression()->accept(*this);

    const OperatorType op_type = node.singleOperatorType();
    const bool requires_signed_int = OperatorRequiresSignedInt(op_type);
    const bool requires_unsigned_int = OperatorRequiresUnsignedInt(op_type);


    if (requires_signed_int && !_expResult->isSignedInteger())
    {
        _formater.format("Operator '%s' requires a signed integer\n",
                         OperatorTypeToStr(node.singleOperatorType()));
        logError(node);
        return;
    }

    if (requires_unsigned_int && !_expResult->isUnsignedInteger())
    {
        _formater.format("Operator '%s' requires an unsigend integer\n",
                         OperatorTypeToStr(node.singleOperatorType()));
        logError(node);
        return;
    }

    node.setTypeInfo(_expResult,_expResult);
}

void
SymbolTreeBuilder::visit(FunctionCallNode& node)
{
    node.setSymbolTable(currentScope());
    const char* func_name = node.functionName();
    auto sym = _curScope->resolveSymbol(func_name);
    if (!sym)
    {
        _formater.format("Symbol '%s' is undefined\n", func_name);
        logError(node);
        return;
    }

    if (!sym->isFunction())
    {
        _formater.format("Symbol '%s' is not a function\n", func_name);
        logError(node);
        return;
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
    node.setExpressionType(_expResult);
    _curFunctionCall = nullptr;
}

void
SymbolTreeBuilder::visit(FunctionDeclNode& node)
{
    node.setSymbolTable(currentScope());

    const char* func_name = node.functionName();
    auto sym = _curScope->resolveSymbol(func_name);
    if (sym)
    {
        _formater.format("Symbol name '%s' already declared\n",
                         func_name);
        logError(node);
        return;
    }

    // declare func
    FunctionType* type = _typeRegistry.registerFunction(&node);
    if (!type)
    {
        _formater.format("Failed to register function '%s'\n",
                         func_name);
        logError(node);
        return;
    }

    _curFunctionDecl = new Symbol(func_name, currentScope()->level(), &node);
    _curScope->declareSymbol(_curFunctionDecl);

    // begin new scope
    beginScope();

    node.functionArguments()->accept(*this);

    if (!didError())
    {
        node.functionCode()->accept(*this);
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
SymbolTreeBuilder::visit(ConditionNode& node)
{
    node.setSymbolTable(currentScope());

    if (node.hasConditionComponent())
    {
        node.condition()->accept(*this);
        if (!didError())
        {
            // check condition expression
            if (!_expResult->isPromotableToBoolean())
            {
                _formater.format("Condition expression can not be promoted to boolean\n");
                logError(node);
                return;
            }
        }
    }

    if (node.hasOnTrueComponent()  && !didError())
    {
        // begin new scope
        beginScope();
        node.onTrue()->accept(*this);
        endScope();
    }

    if (node.hasOnFalseComponent() && !didError())
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
        _formater.format("Return statement outside function scope\n");
        logError(node);
        return;
    }

    const char* function_name = _curFunctionDecl->symbolName();
    const FunctionType* func_type = cast_type<FunctionType>(_curFunctionDecl->astNode()->nodeType());
    const Type* func_return = func_type->typeOfReturnValue();
    // current return type

    if (!func_return->isVoidType() && !node.expression())
    {
        _formater.format("Function '%s' has return value of %s, but we are not returning any\n",
                         function_name, _expResult->typeString());
        logError(node);
        return;
    }


    if (node.expression())
    {
        if (func_return->isVoidType())
        {
            _formater.format("Function '%s' does not have a return value, but we are returning %s\n",
                             function_name, _expResult->typeString());
            logError(node);
            return;
        }

        node.expression()->accept(*this);

        if (!didError())
        {
            // check if the return types match
            if(_expResult->isPromotableTo(func_return))
            {
                _formater.format("Function '%s' returns '%s', but we are returning %s\n",
                                 function_name,
                                 func_return->typeString(),
                                 _expResult->typeString());
                logError(node);
                return;
            }
        }
    }

    node.setSymbolTable(currentScope());
    node.setTypeInfo(_expResult, _expResult);
}

void
SymbolTreeBuilder::visit(WhileLoopNode& node)
{
    node.setSymbolTable(currentScope());

    node.condition()->accept(*this);
    if (!didError())
    {
        // check condition expression
        if (!_expResult->isPromotableToBoolean())
        {
            _formater.format("Condition does not produce valid boolean\n");
            logError(node);
            return;
        }

        // begin new scope
        beginScope();
        node.code()->accept(*this);
        endScope();
    }
}

void
SymbolTreeBuilder::visit(PrintNode& node)
{
    node.arguments()->accept(*this);
}


void
SymbolTreeBuilder::visit(PrintArgsNode& node)
{
    node.setSymbolTable(currentScope());
    yal_u32 idx = 0;

    for(auto& v : node.expressions)
    {
        if (!didError())
        {
            v->accept(*this);
        }
        else
        {
            return;
        }
        ++idx;
    }
}

}
