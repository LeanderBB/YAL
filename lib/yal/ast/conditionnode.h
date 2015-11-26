#ifndef __YAL_CONDITIONNODE_H__
#define __YAL_CONDITIONNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class ConditionNode : public StatementNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(ConditionNode)

    ConditionNode(const SourceLocationInfo& loc,
                  ExpressionNode* condition,
                  CodeBodyNode* onTrue,
                  ConditionNode* onFalse = nullptr);

    virtual ~ConditionNode();

    ExpressionNode* condition() const
    {
        return _pCondition;
    }

    CodeBodyNode* onTrue() const
    {
        return _pOnTrue;
    }

    ConditionNode* onFalse() const
    {
        return _pOnFalse;
    }

    bool hasConditionComponent() const
    {
        return _pCondition != nullptr;
    }

    bool hasOnTrueComponent() const
    {
        return _pOnTrue != nullptr;
    }

    bool hasOnFalseComponent() const
    {
        return _pOnFalse != nullptr;
    }

private:
    ExpressionNode* _pCondition;
    CodeBodyNode* _pOnTrue;
    ConditionNode* _pOnFalse;
};

}
#endif
