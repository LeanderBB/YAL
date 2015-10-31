#ifndef __YAL_VARIABLEDECLNODE_H__
#define __YAL_VARIABLEDECLNODE_H__

#include "yal/ast/assignoperatornode.h"

namespace yal
{

class VariableDeclNode : public ExpressionNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(VariableDeclNode)

    VariableDeclNode(const SourceLocationInfo& loc,
               const char* varName,
               ExpressionNode* pExp);

    virtual ~VariableDeclNode();

    const char* variableName() const
    {
        return _varName;
    }

    ExpressionNode* expression() const
    {
        return _expression;
    }

    void setExpression(ExpressionNode* exp)
    {
        _expression = exp;
    }

protected:
    const char* _varName;
    ExpressionNode* _expression;
};

}

#endif
