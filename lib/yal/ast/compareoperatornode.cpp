#include "yal/ast/compareoperatornode.h"
namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(CompareOperatorNode)

CompareOperatorNode::CompareOperatorNode(const SourceLocationInfo& loc,
                           const OperatorType optype,
                           ExpressionNode *expLeft,
                           ExpressionNode *expRight):
    ExpressionNode(loc),
    _compareType(optype),
    _leftExp(expLeft),
    _rightExp(expRight)
{
    YAL_ASSERT(_compareType & kOperatorMaskComparators);
}

CompareOperatorNode::~CompareOperatorNode()
{

}
}

