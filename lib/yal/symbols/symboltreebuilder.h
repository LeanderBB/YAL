#ifndef __YAL_SYMBOLTREEBUILDER_H__
#define __YAL_SYMBOLTREEBUILDER_H__

#include "yal/ast/astnodevisitor.h"
#include "yal/symbols/scope.h"
#include "yal/util/errorhandler.h"
#include "yal/util/outputformater.h"
#include "yal/parser/parser_state.h"
#include <stack>


namespace yal
{
class TypeRegistry;
class Type;
class SymbolTreeBuilder : public AstNodeVisitor
{
public:
    SymbolTreeBuilder(ErrorHandler& errHandler,
                      TypeRegistry& typeRegistry);

    virtual ~SymbolTreeBuilder();

    bool process(ParserState& state);

#define YAL_NODE_LIST_FUNC(CLASS) virtual void visit(CLASS& node) override;
#include "yal/ast/astnodelist.h"
#undef YAL_NODE_LIST_FUNC

    const Scope& globalScope() const
    {
        return _globalScope;
    }

protected:

    void beginScope();

    void endScope();

    void logError(const AstBaseNode& node);

    bool didError() const
    {
        return _error;
    }

    Scope* currentScope()
    {
        return _curScope;
    }

    bool currentScopeIsGlobalScope() const;

private:
    typedef std::stack<Scope*> SymbolTableStack_t;
    ParserState* _parserState;
    Scope _globalScope;
    Scope* _curScope;
    Symbol* _curFunctionDecl;
    const Symbol* _curFunctionCall;
    ErrorHandler& _errHandler;
    TypeRegistry& _typeRegistry;
    SymbolTableStack_t _scopeStack;
    ExpressionResult _expResult;
    OutputFormater _formater;
    bool _error;
};

}
#endif
