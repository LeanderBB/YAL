#include "yal/ast/argumentdeclnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(ArgumentDeclNode)

ArgumentDeclNode::ArgumentDeclNode(const SourceLocationInfo& loc,
                                   const ConstantType type,
                                   const char* name):
    AstBaseNode(loc),
    _argName(name),
    _argId(nullptr),
    _argType(type)
{
    YAL_ASSERT(type != kConstantTypeId);
}

ArgumentDeclNode::ArgumentDeclNode(const SourceLocationInfo& loc,
                                   const char* id,
                                   const char* name):
    AstBaseNode(loc),
    _argName(name),
    _argId(id),
    _argType(kConstantTypeId)
{

}

ArgumentDeclNode::~ArgumentDeclNode()
{

}

bool
ArgumentDeclNode::isCostumType() const
{
    return _argType == kConstantTypeId;
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

}

ArgumentDeclsNode::~ArgumentDeclsNode()
{

}

void
ArgumentDeclsNode::addArgument(ArgumentDeclNode* node)
{
    _arguments.push_back(node);
}

}

