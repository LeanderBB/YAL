#include "yal/ast/variabledeclnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(VariableDeclNode)

VariableDeclNode::VariableDeclNode(const SourceLocationInfo& loc,
                       const char *varName,
                       ExpressionNode *pExp):
    AssignOperatorNode(loc, varName, kOperatorTypeCopy, pExp)
{

}

VariableDeclNode::~VariableDeclNode()
{

}

}

