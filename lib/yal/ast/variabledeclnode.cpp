#include "yal/ast/variabledeclnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(VariableDeclNode)

VariableDeclNode::VariableDeclNode(const SourceLocationInfo& loc,
                       const char *varName,
                       ExpressionNode *pExp):
    ExpressionNode(loc),
    _varName(varName),
    _expression(pExp)
{
    _expression->setParentNode(this);
}

VariableDeclNode::~VariableDeclNode()
{

}

}

