#ifndef __YAL_VARIABLEACCESSNODE_H__
#define __YAL_VARIABLEACCESSNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class VariableAccessNode : public ExpressionNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(VariableAccessNode)

    VariableAccessNode(const SourceLocationInfo& loc,
                       const char* variableName);

    virtual ~VariableAccessNode();

    const char* variableName() const
    {
        return _variableName;
    }

private:
    const char* _variableName;
};

}

#endif
