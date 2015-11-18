#include "yal/ast/parentexpnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(ParentExpNode)

ParentExpNode::ParentExpNode(const SourceLocationInfo& loc,
                             ExpressionNode* exp):
    UnaryExpressionNode(loc, exp)
{

}

}

