#include "yal/ast/conditionnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(ConditionNode)

ConditionNode::ConditionNode(const SourceLocationInfo &loc,
             ExpressionNode* condition,
             CodeBodyNode* onTrue,
             ConditionNode* onFalse):
    StatementNode(loc),
    _pCondition(condition),
    _pOnTrue(onTrue),
    _pOnFalse(onFalse)
{

}

ConditionNode::~ConditionNode()
{

}

}
