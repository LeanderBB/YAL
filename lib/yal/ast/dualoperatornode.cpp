#include "yal/ast/dualoperatornode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(DualOperatorNode)

DualOperatorNode::DualOperatorNode(const SourceLocationInfo& loc,
                         const DualOperatorType optype,
                         ExpressionNode* expLeft,
                         ExpressionNode* expRight):
    ExpressionNode(loc),
    _dualOperatorType(optype),
    _leftExp(expLeft),
    _rightExp(expRight)
{

}

DualOperatorNode::~DualOperatorNode()
{

}

}

