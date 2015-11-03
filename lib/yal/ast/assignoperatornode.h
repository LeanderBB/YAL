#ifndef __YAL_ASSIGNOPERATORNODE_H__
#define __YAL_ASSIGNOPERATORNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class AssignOperatorNode : public BinaryExpressionNode
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

private:
    const OperatorType _assignOpType;
};

}
#endif
