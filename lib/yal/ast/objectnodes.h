#ifndef __YAL_OBJECTNODES_H__
#define __YAL_OBJECTNODES_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class ObjectNode : public ExpressionNode
{
public:
    ObjectNode(ExpressionNode* node);

    virtual ~ObjectNode() {}

    ExpressionNode* expression() const
    {
        return _expression.get();
    }

protected:
    std::unique_ptr<ExpressionNode> _expression;
};

class ObjectCreateNode : public ObjectNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(ObjectCreateNode)

    ObjectCreateNode(ExpressionNode* node);

    virtual ~ObjectCreateNode() {}

};


class ObjectRetainNode : public ObjectNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(ObjectRetainNode)

    ObjectRetainNode(ExpressionNode* node);

    virtual ~ObjectRetainNode() {}
};

class ObjectReleaseNode : public ObjectNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(ObjectReleaseNode)

    ObjectReleaseNode(ExpressionNode* node);

    virtual ~ObjectReleaseNode() {}
};

}

#endif
