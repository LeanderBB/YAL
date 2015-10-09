#ifndef __YAL_ASSIGNOPERATORNODE_H__
#define __YAL_ASSIGNOPERATORNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class AssignOperatorNode : public ExpressionNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(AssignOperatorNode)

    AssignOperatorNode(const SourceLocationInfo& loc,
              ExpressionNode* expL,
              const OperatorType assignOp,
              ExpressionNode* expR);

    virtual ~AssignOperatorNode();

    OperatorType assignOperatorType() const
    {
        return _assignOpType;
    }

    ExpressionNode* expressionLeft() const
    {
        return _pExpressionLeft;
    }

    ExpressionNode* expressionRight() const
    {
        return _pExpressionRight;
    }

private:
    ExpressionNode* _pExpressionLeft;
    ExpressionNode* _pExpressionRight;
    const OperatorType _assignOpType;
};

}
#endif
