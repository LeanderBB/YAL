#include "yal/ast/returnnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(ReturnNode)

ReturnNode::ReturnNode(const SourceLocationInfo &loc,
                     ExpressionNode* expression):
    StatementNode(loc),
    _pExpression(expression)
{

}

ReturnNode::~ReturnNode()
{

}

}
