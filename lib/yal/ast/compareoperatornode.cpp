#include "yal/ast/compareoperatornode.h"
namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(CompareOperatorNode)

CompareOperatorNode::CompareOperatorNode(const SourceLocationInfo& loc,
                           const OperatorType optype,
                           ExpressionNode *expLeft,
                           ExpressionNode *expRight):
    BinaryExpressionNode(loc, expLeft, expRight),
    _compareType(optype)
{
    YAL_ASSERT(_compareType & kOperatorMaskComparators);
}

CompareOperatorNode::~CompareOperatorNode()
{

}
}

