#include "yal/ast/assignoperatornode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(AssignOperatorNode)

AssignOperatorNode::AssignOperatorNode(const SourceLocationInfo &loc,
                     const char* id,
                     const OperatorType assignOp,
                     ExpressionNode* exp):
    ExpressionNode(loc),
    _varName(id),
    _pExpression(exp),
    _assignOpType(assignOp)
{
    YAL_ASSERT(_assignOpType & (kOperatorMaskArithemetic | kOperatorMaskBit | kOperatorTypeCopy));
}

AssignOperatorNode::~AssignOperatorNode()
{

}

}

