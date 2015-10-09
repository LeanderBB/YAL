#include "yal/ast/codebodynode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(CodeBodyNode)

CodeBodyNode::CodeBodyNode(const SourceLocationInfo &loc):
    AstBaseNode(loc)
{

}

CodeBodyNode::CodeBodyNode(const SourceLocationInfo& loc,
                         StatementNodeVec_t &&statementsVec):
    AstBaseNode(loc),
    statements(std::move(statementsVec))
{
    for (auto& node : statementsVec)
    {
        node->setParentNode(this);
    }
}

CodeBodyNode::~CodeBodyNode()
{

}

void
CodeBodyNode::addStatement(StatementNode *statement)
{
    statement->setParentNode(this);
    statements.push_back(statement);
}

}

