#ifndef __YAL_FUNCTIONNODE_H__
#define __YAL_FUNCTIONNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class FunctionCallArgsNode : public ExpressionList
{
public:
    YAL_AST_NODE_ACCEPT_HDR(FunctionCallArgsNode)

    FunctionCallArgsNode(const SourceLocationInfo& loc):
        ExpressionList(loc)
    {

    }

    virtual ~FunctionCallArgsNode()
    {

    }
};

class FunctionCallNode : public ExpressionNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(FunctionCallNode)

    FunctionCallNode(const SourceLocationInfo& loc,
                     ExpressionNode* objectExpression,
                     const char* name,
                     FunctionCallArgsNode *args);

    virtual ~FunctionCallNode();

    const char* functionName() const
    {
        return _functionName;
    }

    FunctionCallArgsNode* functionArguments() const
    {
        return _functionArgs;
    }

    yal_u32 functionArgumentsCount() const
    {
        return _functionArgs ? _functionArgs->expressions.size() : 0;
    }

    bool hasFunctionArguments() const
    {
        return _functionArgs;
    }

    bool isObjectCall() const
    {
        return _objectExpression != nullptr;
    }

    ExpressionNode* objectExpression() const
    {
        return _objectExpression;
    }

private:
    const char* _functionName;
    FunctionCallArgsNode* _functionArgs;
    ExpressionNode* _objectExpression;
};


class FunctionDeclBaseNode : public StatementNode
{
public:

    static std::string GenFunctionName(const Type* objectType,
                                       const char* name);

    static std::string GenFunctionNameNative(const Type* objectType,
                                             const char* name,
                                             const Type* returnType);

    FunctionDeclBaseNode(const SourceLocationInfo& loc,
                         const char* name,
                         Type* objectType,
                         ArgumentDeclsNode* args,
                         Type *returnType);

    virtual ~FunctionDeclBaseNode();

    const char* functionName() const
    {
        return _functionName.c_str();
    }

    const char* nativeFunctionName() const
    {
        return _nativeFunctionName.c_str();
    }

    ArgumentDeclsNode* functionArguments() const
    {
        return _functionArgs;
    }

    bool hasFunctionArguments() const
    {
        return _functionArgs;
    }

    bool hasReturnValue() const;

    Type* returnValueType() const
    {
        return _returnValueType;
    }

    bool isObjectFunction() const
    {
        return _objectType != nullptr;
    }

    Type* objectType() const
    {
        return _objectType;
    }
protected:
    const std::string _functionName;
    const std::string _nativeFunctionName;
    ArgumentDeclsNode* _functionArgs;
    Type* _returnValueType;
    Type* _objectType;
};


class FunctionDeclNode : public FunctionDeclBaseNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(FunctionDeclNode)

    FunctionDeclNode(const SourceLocationInfo& loc,
                     const char* name,
                     Type* objectType,
                     ArgumentDeclsNode* args,
                     Type *returnType,
                     CodeBodyNode* code);

    virtual ~FunctionDeclNode();

    CodeBodyNode* functionCode() const
    {
        return _codeBody;
    }

private:
    CodeBodyNode* _codeBody;
};

class FunctionDeclNativeNode : public FunctionDeclBaseNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(FunctionDeclNativeNode)

    FunctionDeclNativeNode(const SourceLocationInfo& loc,
                           const char* name,
                           Type* objectType,
                           ArgumentDeclsNode* args,
                           Type *returnType);

    virtual ~FunctionDeclNativeNode();
};

}
#endif
