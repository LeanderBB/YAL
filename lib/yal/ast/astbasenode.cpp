#include "yal/ast/astbasenode.h"
namespace yal
{

AstBaseNode::AstBaseNode(const SourceLocationInfo& loc):
    _loc(loc)
{

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

}

ExpressionList::~ExpressionList()
{

}

}
