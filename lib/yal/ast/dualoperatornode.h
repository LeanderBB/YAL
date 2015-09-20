#ifndef __YAL_DUALOPERATORNODE_H__
#define __YAL_DUALOPERATORNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class DualOperatorNode : public ExpressionNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(DualOperatorNode)

    DualOperatorNode(const SourceLocationInfo& loc,
                const OperatorType optype,
                ExpressionNode *expLeft,
                ExpressionNode *expRight);

    virtual ~DualOperatorNode();

    OperatorType dualOperatorType() const
    {
        return _dualOperatorType;
    }

    ExpressionNode* leftExpression() const
    {
        return _leftExp;
    }

    ExpressionNode* rightExpression() const
    {
        return _rightExp;
    }

private:
    const OperatorType _dualOperatorType;
    ExpressionNode* _leftExp;
    ExpressionNode* _rightExp;
};

}
#endif
