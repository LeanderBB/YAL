#ifndef __YAL_COMPAREOPERATORNODE_H__
#define __YAL_COMPAREOPERATORNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class CompareOperatorNode : public BinaryExpressionNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(CompareOperatorNode)

    CompareOperatorNode(const SourceLocationInfo& loc,
                 const OperatorType optype,
                 ExpressionNode* expLeft,
                 ExpressionNode* expRight);

    virtual ~CompareOperatorNode();

    OperatorType compareOperatorType() const
    {
        return _compareType;
    }

private:
    const OperatorType _compareType;
};

}

#endif
