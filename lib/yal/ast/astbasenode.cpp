#include "yal/ast/astbasenode.h"
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
