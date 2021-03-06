#include "yal/ast/astbasenode.h"
#include <algorithm>
namespace yal
{


AstBaseNode::AstBaseNode(const SourceLocationInfo& loc):
    _loc(loc)
{

}

const SourceLocationInfo&
AstBaseNode::locationInfo() const
{
    if (!isSourceAstNode() && _parentNode)
    {
        return _parentNode->locationInfo();
    }
    return _loc;
}

bool
AstBaseNode::isSourceAstNode() const
{
    return _loc.valid();
}

bool
StatementNode::removeSymbolFromScope(Symbol *sym)
{
    auto it = std::find(_stSymbolScope.begin(),
                        _stSymbolScope.end(), sym);
    if (it != _stSymbolScope.end())
    {
        _stSymbolScope.erase(it);
        return true;
    }
    return false;
}


bool
MultiExpressionNode::replaceExpression(const ExpressionNode* old,
                                       ExpressionNode* newExp)
{
    auto it = std::find_if(_expressions.begin(), _expressions.end(),
                        [old](const std::unique_ptr<ExpressionNode>& n) { return n.get() == old;});
    if (it != _expressions.end())
    {
        it->reset(newExp);
        return true;
    }
    return false;
}

ExpressionList::ExpressionList(const SourceLocationInfo &loc):
    AstBaseNode(loc)
{

}

ExpressionList::ExpressionList(const SourceLocationInfo& loc,
                               ExpressionNodeVec_t&& list):
    AstBaseNode(loc),
    expressions(std::move(list))
{
    for (auto& exp : expressions)
    {
        exp->setParentNode(this);
    }
}

ExpressionList::~ExpressionList()
{

}

}
