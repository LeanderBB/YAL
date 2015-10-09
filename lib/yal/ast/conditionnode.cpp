#include "yal/ast/conditionnode.h"
#include "yal/ast/codebodynode.h"

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
    if(condition)
    {
        condition->setParentNode(this);
    }

    if (onTrue)
    {
        onTrue->setParentNode(this);
    }

    if (onFalse)
    {
        onFalse ->setParentNode(this);
    }
}

ConditionNode::~ConditionNode()
{

}

}
