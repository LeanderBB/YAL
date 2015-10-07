#include "yal/ast/dualoperatornode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(DualOperatorNode)

DualOperatorNode::DualOperatorNode(const SourceLocationInfo& loc,
                         const OperatorType optype,
                         ExpressionNode* expLeft,
                         ExpressionNode* expRight):
    ExpressionNode(loc),
    _dualOperatorType(optype),
    _leftExp(expLeft),
    _rightExp(expRight)
{
    YAL_ASSERT(_dualOperatorType & (kOperatorMaskArithemetic | kOperatorMaskBit | kOperatorMaskLogic));
}

DualOperatorNode::~DualOperatorNode()
{

}

}

