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
                                   ExpressionNode* objectExpression,
                                   const char* name,
                                   FunctionCallArgsNode* args):
    ExpressionNode(loc),
    _functionName(name),
    _functionArgs(args),
    _objectExpression(objectExpression)
{
    args->setParentNode(this);
    if (objectExpression)
    {
        objectExpression->setParentNode(this);
    }
}

FunctionCallNode::~FunctionCallNode()
{

}


std::string
FunctionDeclBaseNode::GenFunctionName(const Type* objectType,
                                      const char* name)
{
    std::string fn_name;
    if (objectType)
    {
        fn_name = objectType->nativeTypeString();
        fn_name += "_";
    }
    fn_name += name;
    return fn_name;
}

std::string
FunctionDeclBaseNode::GenFunctionNameNative(const Type* objectType,
                                            const char* name,
                                            const Type* returnType)
{
    std::string fn_name;
    if (objectType)
    {
        fn_name = objectType->nativeTypeString();
        fn_name += "_";
    }
    fn_name += name;

    if (!returnType->isVoidType())
    {
        fn_name += "_";
        fn_name += returnType->nativeTypeString();
    }

    return fn_name;
}

FunctionDeclBaseNode::FunctionDeclBaseNode(const SourceLocationInfo& loc,
                                           const char* name,
                                           Type *objectType,
                                           ArgumentDeclsNode *args,
                                           Type *returnType):
    StatementNode(loc),
    _functionName(name),
    _functionNameWithType(GenFunctionName(objectType, name)),
    _nativeFunctionName(GenFunctionNameNative(objectType, name, returnType)),
    _functionArgs(args),
    _returnValueType(returnType),
    _objectType(objectType)
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
                                   Type* objectType,
                                   ArgumentDeclsNode *args,
                                   Type *returnType,
                                   CodeBodyNode *code):
    FunctionDeclBaseNode(loc, name, objectType, args, returnType),
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
                                               Type* objectType,
                                               ArgumentDeclsNode *args,
                                               Type *returnType):
    FunctionDeclBaseNode(loc, name, objectType, args, returnType)
{
}

FunctionDeclNativeNode::~FunctionDeclNativeNode()
{

}



}
