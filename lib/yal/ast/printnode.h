#ifndef __YAL_PRINTNODE_H__
#define __YAL_PRINTNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class PrintArgsNode : public ExpressionList
{
public:
    YAL_AST_NODE_ACCEPT_HDR(PrintArgsNode)

    PrintArgsNode(const SourceLocationInfo& loc):
        ExpressionList(loc)
    {

    }

    virtual ~PrintArgsNode()
    {

    }
};


class PrintNode : public StatementNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(PrintNode)

    PrintNode(const SourceLocationInfo& loc,
              PrintArgsNode* args);


    virtual ~PrintNode(){}

    yal_u32 argumentCount() const
    {
        return _arguments->expressions.size();
    }

    PrintArgsNode* arguments() const
    {
        return _arguments;
    }

private:
    PrintArgsNode* _arguments;
};

}

#endif
