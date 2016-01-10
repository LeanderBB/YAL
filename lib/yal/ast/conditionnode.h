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
        return _pCondition.get();
    }

    CodeBodyNode* onTrue() const
    {
        return _pOnTrue.get();
    }

    ConditionNode* onFalse() const
    {
        return _pOnFalse.get();
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
    std::unique_ptr<ExpressionNode> _pCondition;
    std::unique_ptr<CodeBodyNode> _pOnTrue;
    std::unique_ptr<ConditionNode> _pOnFalse;
};

}
#endif
