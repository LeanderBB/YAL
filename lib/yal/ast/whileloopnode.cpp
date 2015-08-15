#include "yal/ast/whileloopnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(WhileLoopNode)

WhileLoopNode::WhileLoopNode(const SourceLocationInfo& loc,
                             ExpressionNode* condition,
                             CodeBodyNode* code):
    StatementNode(loc),
    _condition(condition),
    _codeBody(code)
{

}

WhileLoopNode::~WhileLoopNode()
{

}

}
