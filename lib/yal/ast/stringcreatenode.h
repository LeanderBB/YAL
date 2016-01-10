#ifndef __YAL_STRINGCREATENODE_H__
#define __YAL_STRINGCREATENODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class StringCreateNode : public ExpressionNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(StringCreateNode)

    StringCreateNode(ConstantNode* constNode);

    virtual ~StringCreateNode(){}

    ConstantNode* constantNode() const
    {
        return _constNode.get();
    }

    std::unique_ptr<ConstantNode>& constantNodePtr()
    {
        return _constNode;
    }

private:
    std::unique_ptr<ConstantNode> _constNode;
};

}
#endif
