#ifndef __YAL_RETURNNODE_H__
#define __YAL_RETURNNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class ReturnNode : public StatementNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(ReturnNode)

    ReturnNode(const SourceLocationInfo& loc,
              ExpressionNode* expression);

    virtual ~ReturnNode();

    ExpressionNode* expression() const
    {
        return _pExpression.get();
    }

    void replaceExpression(ExpressionNode* expression)
    {
        _pExpression.reset(expression);
        _pExpression->setParentNode(this);
    }

private:
    std::unique_ptr<ExpressionNode> _pExpression;
};

}
#endif
