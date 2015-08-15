#include "yal/ast/functionnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(FunctionCallArgsNode)

YAL_AST_NODE_ACCEPT_IMP(FunctionCallNode)

FunctionCallNode::FunctionCallNode(const SourceLocationInfo &loc,
                                   const char* name,
                                   FunctionCallArgsNode* args):
    ExpressionNode(loc),
    _functionName(name),
    _functionArgs(args)
{

}

FunctionCallNode::~FunctionCallNode()
{

}

YAL_AST_NODE_ACCEPT_IMP(FunctionDeclNode)

FunctionDeclNode::FunctionDeclNode(const SourceLocationInfo& loc,
                                   const char* name,
                                   ArgumentDeclsNode *args,
                                   const ConstantType returnType,
                                   CodeBodyNode *code):
    AstBaseNode(loc),
    _functionName(name),
    _functionArgs(args),
    _codeBody(code),
    _returnType(returnType)
{

}

FunctionDeclNode::~FunctionDeclNode()
{

}


}
