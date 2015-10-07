#ifndef __YAL_ARGUMENTDECLNODE_H__
#define __YAL_ARGUMENTDECLNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class ArgumentDeclNode : public AstBaseNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(ArgumentDeclNode)

    ArgumentDeclNode(const SourceLocationInfo& loc,
               Type* type,
               const char* name);

    virtual ~ArgumentDeclNode();

    const char* argumentName() const
    {
        return _argName;
    }

    Type* argumentType() const
    {
        return _argType;
    }

    bool isCustomType() const;

private:
    const char* _argName;
    Type* _argType;
};

typedef YALVector<ArgumentDeclNode*> ASTDeclArgVec_t;


class ArgumentDeclsNode : public AstBaseNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(ArgumentDeclsNode)

    ArgumentDeclsNode(const SourceLocationInfo& loc);

    ArgumentDeclsNode(const SourceLocationInfo &loc,
                ASTDeclArgVec_t&& argVec);

    virtual ~ArgumentDeclsNode();

    void addArgument(ArgumentDeclNode* node);

    ASTDeclArgVec_t& arguments()
    {
        return _arguments;
    }

    const ASTDeclArgVec_t& arguments() const
    {
        return _arguments;
    }

    yal_u32 argumentCount() const
    {
        return _arguments.size();
    }

private:
    ASTDeclArgVec_t _arguments;
};

}
#endif
