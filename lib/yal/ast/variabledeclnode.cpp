#include "yal/ast/variabledeclnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(VariableDeclNode)

VariableDeclNode::VariableDeclNode(const SourceLocationInfo& loc,
                       const char *varName,
                       ExpressionNode *pExp):
    StatementNode(loc),
    _varName(varName),
    _pExpression(pExp)
{

}

VariableDeclNode::~VariableDeclNode()
{

}

}

