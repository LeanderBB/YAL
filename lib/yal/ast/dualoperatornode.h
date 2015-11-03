#ifndef __YAL_DUALOPERATORNODE_H__
#define __YAL_DUALOPERATORNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class DualOperatorNode : public BinaryExpressionNode
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
private:
    const OperatorType _dualOperatorType;

};

}
#endif
