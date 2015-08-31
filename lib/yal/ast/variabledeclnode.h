#ifndef __YAL_VARIABLEDECLNODE_H__
#define __YAL_VARIABLEDECLNODE_H__

#include "yal/ast/assignoperatornode.h"

namespace yal
{

class VariableDeclNode : public AssignOperatorNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR_BASE(VariableDeclNode, AssignOperatorNode)

    VariableDeclNode(const SourceLocationInfo& loc,
               const char* varName,
               ExpressionNode* pExp);

    virtual ~VariableDeclNode();
};

}

#endif
