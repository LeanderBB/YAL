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

YAL_AST_NODE_ACCEPT_IMP(FunctionDeclNode)

FunctionDeclNode::FunctionDeclNode(const SourceLocationInfo& loc,
                                   const char* name,
                                   ArgumentDeclsNode *args,
                                   Type *returnType,
                                   CodeBodyNode *code):
    AstBaseNode(loc),
    _functionName(name),
    _functionArgs(args),
    _codeBody(code),
    _returnValueType(returnType)
{
    args->setParentNode(this);
    code->setParentNode(this);
}

FunctionDeclNode::~FunctionDeclNode()
{

}

bool
FunctionDeclNode::hasReturnValue() const
{
    return !_returnValueType->isVoidType();
}


}
