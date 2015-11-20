#include "yal/ast/variabledeclnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(VariableDeclNode)

VariableDeclNode::VariableDeclNode(const SourceLocationInfo& loc,
                       const char *varName,
                       ExpressionNode *pExp,
                       Type *explicitType):
    UnaryExpressionNode(loc, pExp),
    _varName(varName),
    _explicitType(explicitType)
{
    _expression->setParentNode(this);
}

VariableDeclNode::~VariableDeclNode()
{

}

}

