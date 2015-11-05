#ifndef __YAL_SYMBOLTREEBUILDER_H__
#define __YAL_SYMBOLTREEBUILDER_H__

#include "yal/ast/astnodevisitor.h"
#include "yal/symbols/scope.h"
#include "yal/util/errorhandler.h"
#include "yal/ast/astbasenode.h"
#include <stack>
#include <sstream>


namespace yal
{
class TypeRegistry;
class Type;
class ParserState;
class SymbolTreeBuilder : public AstNodeVisitor
{
public:
    SymbolTreeBuilder(TypeRegistry& typeRegistry);

    virtual ~SymbolTreeBuilder();

    void process(ParserState& state);

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

    Scope* currentScope()
    {
        return _curScope;
    }

    bool currentScopeIsGlobalScope() const;

private:
    typedef std::stack<Scope*> SymbolTableStack_t;
    typedef std::stack<Symbol*> SymbolStack_t;
    ParserState* _parserState;
    Scope _globalScope;
    Scope* _curScope;
    Symbol* _curFunctionDecl;
    const Symbol* _curFunctionCall;
    TypeRegistry& _typeRegistry;
    SymbolTableStack_t _scopeStack;
    ExpressionResult _expResult;
    StatementNode* _curStatment;
    SymbolStack_t _symStack;
};

}
#endif
