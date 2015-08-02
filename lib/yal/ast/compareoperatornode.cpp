#include "yal/ast/compareoperatornode.h"
namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(CompareOperatorNode)

CompareOperatorNode::CompareOperatorNode(const SourceLocationInfo& loc,
                           const CompareOperatorType optype,
                           ExpressionNode *expLeft,
                           ExpressionNode *expRight):
    ExpressionNode(loc),
    _compareType(optype),
    _leftExp(expLeft),
    _rightExp(expRight)
{

}

CompareOperatorNode::~CompareOperatorNode()
{

}
}

