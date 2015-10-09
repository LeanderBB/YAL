#include "yal/ast/argumentdeclnode.h"
#include "yal/types/type.h"
namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(ArgumentDeclNode)

ArgumentDeclNode::ArgumentDeclNode(const SourceLocationInfo& loc,
                                   Type *type,
                                   const char* name):
    AstBaseNode(loc),
    _argName(name),
    _argType(type)
{
}


ArgumentDeclNode::~ArgumentDeclNode()
{

}

bool
ArgumentDeclNode::isCustomType() const
{
    return !_argType->isBuiltinType();
}

YAL_AST_NODE_ACCEPT_IMP(ArgumentDeclsNode)

ArgumentDeclsNode::ArgumentDeclsNode(const SourceLocationInfo &loc):
    AstBaseNode(loc)
{

}

ArgumentDeclsNode::ArgumentDeclsNode(const SourceLocationInfo& loc,
                                     ASTDeclArgVec_t&& argVec):
    AstBaseNode(loc),
    _arguments(std::move(argVec))
{
    for (auto& args : _arguments)
    {
        args->setParentNode(this);
    }
}

ArgumentDeclsNode::~ArgumentDeclsNode()
{

}

void
ArgumentDeclsNode::addArgument(ArgumentDeclNode* node)
{
    node->setParentNode(this);
    _arguments.push_back(node);
}

}

