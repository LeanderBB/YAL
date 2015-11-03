#include "yal/ast/assignoperatornode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(AssignOperatorNode)

AssignOperatorNode::AssignOperatorNode(const SourceLocationInfo &loc,
                                       ExpressionNode* expL,
                                       const OperatorType assignOp,
                                       ExpressionNode* expR):
    BinaryExpressionNode(loc, expL, expR),
    _assignOpType(assignOp)
{
    YAL_ASSERT(_assignOpType & (kOperatorMaskArithemetic | kOperatorMaskBit | kOperatorTypeCopy));
}

AssignOperatorNode::~AssignOperatorNode()
{

}
}

