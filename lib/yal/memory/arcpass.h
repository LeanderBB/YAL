#ifndef __YAL_ARCPASS_H__
#define __YAL_ARCPASS_H__

#include "yal/ast/astnodevisitor.h"
#include "yal/parser/parser_state.h"
#include "yal/symbols/scopeaction.h"
namespace yal
{

class ArcPass : public AstNodeVisitor
{
public:
    ArcPass();

    virtual ~ArcPass(){}

    void process(ParserState& state);

protected:

#define YAL_NODE_LIST_FUNC(CLASS) virtual void visit(CLASS& node) override;
#include "yal/ast/astnodelist.h"
#undef YAL_NODE_LIST_FUNC

};

class ArcScopeActionVisitor : public ScopeActionVisitor
{
public:

    ArcScopeActionVisitor(CodeBodyNode& node):
        _codeBody(node)
    {
    }

    virtual ~ArcScopeActionVisitor() {}

#define YAL_SCOPE_ACTION_LIST_FUNC(x)\
virtual void visitOnEnter(const x& action) override; \
virtual void visitOnExit(const x& action) override;
#include "yal/symbols/scopeactionlist.h"
#undef YAL_SCOPE_ACTION_LIST_FUNC

private:
    CodeBodyNode& _codeBody;
};

}
#endif
