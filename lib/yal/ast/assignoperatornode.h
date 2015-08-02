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
              const char* id,
              const AssignOperatorType assignOp,
              ExpressionNode* exp);

    virtual ~AssignOperatorNode();

    const char* variableName() const
    {
        return _varName;
    }

    AssignOperatorType assignOperatorType() const
    {
        return _assignOpType;
    }

    ExpressionNode* expression() const
    {
        return _pExpression;
    }

private:
    const char* _varName;
    ExpressionNode* _pExpression;
    const AssignOperatorType _assignOpType;
};

}
#endif
