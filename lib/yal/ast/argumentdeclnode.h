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
               const ConstantType type,
               const char* name);

    ArgumentDeclNode(const SourceLocationInfo& loc,
               const char* id,
               const char* name);

    virtual ~ArgumentDeclNode();

    const char* argumentName() const
    {
        return _argName;
    }

    const char* argumentId() const
    {
        return _argId;
    }

    ConstantType argumentType() const
    {
        return _argType;
    }

    bool isCostumType() const;

private:
    const char* _argName;
    const char* _argId;
    const ConstantType _argType;
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
private:
    ASTDeclArgVec_t _arguments;
};

}
#endif
