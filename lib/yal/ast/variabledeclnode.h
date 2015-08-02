#ifndef __YAL_VARIABLEDECLNODE_H__
#define __YAL_VARIABLEDECLNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class VariableDeclNode : public StatementNode
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
        return _pExpression;
    }

private:
    const char* _varName;
    ExpressionNode* _pExpression;
};

}

#endif
