#include "yal/ast/whileloopnode.h"

namespace yal
{

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
