#include "yal/ast/singleoperatornode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(SingleOperatorNode)

SingleOperatorNode::SingleOperatorNode(const SourceLocationInfo& loc,
                             const OperatorType optype,
                             ExpressionNode* exp):
    ExpressionNode(loc),
    _singleOperatorType(optype),
    _pExpression(exp)
{
    YAL_ASSERT(_singleOperatorType & kOperatorMaskSingle);
}

SingleOperatorNode::~SingleOperatorNode()
{

}

}
