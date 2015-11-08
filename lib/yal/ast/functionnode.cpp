#include "yal/ast/functionnode.h"
#include "yal/types/type.h"
#include "yal/types/builtintype.h"
#include "yal/ast/argumentdeclnode.h"
#include "yal/ast/codebodynode.h"
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
    args->setParentNode(this);
}

FunctionCallNode::~FunctionCallNode()
{

}


FunctionDeclBaseNode::FunctionDeclBaseNode(const SourceLocationInfo& loc,
                                           const char* name,
                                           ArgumentDeclsNode *args,
                                           Type *returnType):
    AstBaseNode(loc),
    _functionName(name),
    _functionArgs(args),
    _returnValueType(returnType)
{
    args->setParentNode(this);
}

FunctionDeclBaseNode::~FunctionDeclBaseNode()
{

}

bool
FunctionDeclBaseNode::hasReturnValue() const
{
    return !_returnValueType->isVoidType();
}


YAL_AST_NODE_ACCEPT_IMP(FunctionDeclNode)

FunctionDeclNode::FunctionDeclNode(const SourceLocationInfo& loc,
                                   const char* name,
                                   ArgumentDeclsNode *args,
                                   Type *returnType,
                                   CodeBodyNode *code):
    FunctionDeclBaseNode(loc, name, args, returnType),
    _codeBody(code)
{
    code->setParentNode(this);
}

FunctionDeclNode::~FunctionDeclNode()
{

}

YAL_AST_NODE_ACCEPT_IMP(FunctionDeclNativeNode)

FunctionDeclNativeNode::FunctionDeclNativeNode(const SourceLocationInfo& loc,
                                               const char* name,
                                               ArgumentDeclsNode *args,
                                               Type *returnType):
    FunctionDeclBaseNode(loc, name, args, returnType)
{
}

FunctionDeclNativeNode::~FunctionDeclNativeNode()
{

}



}
