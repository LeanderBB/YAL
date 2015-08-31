#include "yal/ast/constantnode.h"
#include "yal/memory/memory_utils.h"
namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(ConstantNode)

ConstantNode::ConstantNode(const SourceLocationInfo& loc,
                           const ConstantValue& value):
    ExpressionNode(loc),
    _constantValue(value)
{
}

ConstantNode::~ConstantNode()
{

}



}
