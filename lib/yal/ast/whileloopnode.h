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
        return _condition;
    }

    CodeBodyNode* code() const
    {
        return _codeBody;
    }

private:
    ExpressionNode* _condition;
    CodeBodyNode* _codeBody;
};

}
#endif
