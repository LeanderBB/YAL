#ifndef __YAL_WHILELOOPNODE_H__
#define __YAL_WHILELOOPNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class WhileLoopNode : public StatementNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(WhileLoopNode)

    WhileLoopNode(const SourceLocationInfo& loc,
                  ExpressionNode *condition,
                  CodeBodyNode *code);

    virtual ~WhileLoopNode();

    ExpressionNode* condition() const
    {
        return _condition.get();
    }

    CodeBodyNode* code() const
    {
        return _codeBody.get();
    }

private:
    std::unique_ptr<ExpressionNode> _condition;
    std::unique_ptr<CodeBodyNode> _codeBody;
};

}
#endif
