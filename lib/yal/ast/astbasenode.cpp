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
