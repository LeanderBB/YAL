#include "yal/ast/assignoperatornode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(AssignOperatorNode)

AssignOperatorNode::AssignOperatorNode(const SourceLocationInfo &loc,
                                       ExpressionNode* expL,
                                       const OperatorType assignOp,
                                       ExpressionNode* expR):
    ExpressionNode(loc),
    _pExpressionLeft(expL),
    _pExpressionRight(expR),
    _assignOpType(assignOp)
{
    expL->setParentNode(this);
    expR->setParentNode(this);
    YAL_ASSERT(_assignOpType & (kOperatorMaskArithemetic | kOperatorMaskBit | kOperatorTypeCopy));
}

AssignOperatorNode::~AssignOperatorNode()
{

}

}

