#ifndef __YAL_FUNCTIONNODE_H__
#define __YAL_FUNCTIONNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class FunctionCallNode : public ExpressionNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(FunctionCallNode)

    FunctionCallNode(const SourceLocationInfo& loc,
                     const char* name,
                     ExpressionList* args);

    virtual ~FunctionCallNode();

    const char* functionName() const
    {
        return _functionName;
    }

    ExpressionList* functionArguments() const
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
    ExpressionList* _functionArgs;
};


class FunctionDeclNode : public AstBaseNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(FunctionDeclNode)

    FunctionDeclNode(const SourceLocationInfo& loc,
                     const char* name,
                     ArgumentDeclsNode* args,
                     const ConstantType returnType,
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

    bool hasReturnType() const
    {
        return _returnType != kConstantTypeNone;
    }

    ConstantType returnType() const
    {
        return _returnType;
    }

private:
    const char* _functionName;
    ArgumentDeclsNode* _functionArgs;
    CodeBodyNode* _codeBody;
    const ConstantType _returnType;
};

}
#endif
