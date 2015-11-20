#include "yal/ast/dualoperatornode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(DualOperatorNode)

DualOperatorNode::DualOperatorNode(const SourceLocationInfo& loc,
                         const OperatorType optype,
                         ExpressionNode* expLeft,
                         ExpressionNode* expRight):
    BinaryExpressionNode(loc, expLeft, expRight),
    _dualOperatorType(optype)
{
    YAL_ASSERT(_dualOperatorType & (kOperatorMaskArithemetic | kOperatorMaskBit | kOperatorMaskLogic | kOperatorTypeArray));
}

DualOperatorNode::~DualOperatorNode()
{

}

}

