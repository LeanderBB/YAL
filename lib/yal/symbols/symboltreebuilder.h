#ifndef __YAL_SYMBOLTREEBUILDER_H__
#define __YAL_SYMBOLTREEBUILDER_H__

#include "yal/ast/astnodevisitor.h"
#include "yal/symbols/symboltable.h"
#include "yal/symbols/functionsym.h"
#include "yal/symbols/variablesym.h"
#include "yal/util/errorhandler.h"
#include "yal/util/outputformater.h"
#include "yal/parser/parser_state.h"
#include <stack>

namespace yal
{

class SymbolTreeBuilder : public AstNodeVisitor
{
public:
    SymbolTreeBuilder(ErrorHandler& errHandler);

    virtual ~SymbolTreeBuilder();

    bool process(ParserState& state);

#define YAL_NODE_LIST_FUNC(CLASS) virtual void visit(CLASS& node) override;
#include "yal/ast/astnodelist.h"
#undef YAL_NODE_LIST_FUNC

    const SymbolTable& globalTable() const
    {
        return _globalSym;
    }

protected:

    void beginScope();

    void endScope();

    void logError(const AstBaseNode& node);

    bool didError() const
    {
        return _error;
    }

    SymbolTable* currentScope()
    {
        return _curScope;
    }

    bool currentScopeIsGlobalScope() const;

private:
    typedef std::stack<SymbolTable*> SymbolTableStack_t;
    ParserState* _parserState;
    SymbolTable _globalSym;
    SymbolTable* _curScope;
    FunctionSym* _curFunctionDecl;
    const FunctionSym* _curFunctionCall;
    ErrorHandler& _errHandler;
    SymbolTableStack_t _scopeStack;
    DataType _expResult;
    OutputFormater _formater;
    yal_u32 _scopeCounter;
    bool _error;
};

}
#endif
