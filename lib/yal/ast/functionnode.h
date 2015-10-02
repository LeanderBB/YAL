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

private:
    const char* _functionName;
    FunctionCallArgsNode* _functionArgs;
};


class FunctionDeclNode : public AstBaseNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(FunctionDeclNode)

    FunctionDeclNode(const SourceLocationInfo& loc,
                     const char* name,
                     ArgumentDeclsNode* args,
                     Type *returnType,
                     CodeBodyNode* code);

    virtual ~FunctionDeclNode();

    const char* functionName() const
    {
        return _functionName;
    }

    ArgumentDeclsNode* functionArguments() const
    {
        return _functionArgs;
    }

    CodeBodyNode* functionCode() const
    {
        return _codeBody;
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

private:
    const char* _functionName;
    ArgumentDeclsNode* _functionArgs;
    CodeBodyNode* _codeBody;
    Type* _returnValueType;
};

}
#endif
