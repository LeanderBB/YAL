#ifndef __YAL_COMPAREOPERATORNODE_H__
#define __YAL_COMPAREOPERATORNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class CompareOperatorNode : public ExpressionNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(CompareOperatorNode)

    CompareOperatorNode(const SourceLocationInfo& loc,
                 const CompareOperatorType optype,
                 ExpressionNode* expLeft,
                 ExpressionNode* expRight);

    virtual ~CompareOperatorNode();

    CompareOperatorType compareOperatorType() const
    {
        return _compareType;
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
    const CompareOperatorType _compareType;
    ExpressionNode* _leftExp;
    ExpressionNode* _rightExp;
};

}

#endif
