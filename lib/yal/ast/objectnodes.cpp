#include "yal/ast/objectnodes.h"

namespace yal
{

ObjectNode::ObjectNode(ExpressionNode *node):
    ExpressionNode(SourceLocationInfo()),
    _expression(node)
{
    node->setParentNode(this);
    this->setNodeType(node->nodeType());
    this->setExpressionResult(node->expressionResult());
}


YAL_AST_NODE_ACCEPT_IMP(ObjectCreateNode)

ObjectCreateNode::ObjectCreateNode(ExpressionNode *node):
    ObjectNode(node)
{

}

YAL_AST_NODE_ACCEPT_IMP(ObjectRetainNode)

ObjectRetainNode::ObjectRetainNode(ExpressionNode *node):
    ObjectNode(node)
{

}

YAL_AST_NODE_ACCEPT_IMP(ObjectReleaseNode)

ObjectReleaseNode::ObjectReleaseNode(ExpressionNode *node):
    ObjectNode(node)
{

}

}

