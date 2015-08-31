#ifndef __YAL_ASTCODEBODY_H__
#define __YAL_ASTCODEBODY_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class CodeBodyNode : public AstBaseNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(CodeBodyNode)

    CodeBodyNode(const SourceLocationInfo& loc);

    CodeBodyNode(const SourceLocationInfo &loc,
                StatementNodeVec_t&& statementsVec);

    virtual ~CodeBodyNode();

    void addStatement(StatementNode* statement);

public:
    StatementNodeVec_t statements;
};

}
#endif
