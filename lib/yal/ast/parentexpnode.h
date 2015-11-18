#ifndef __YAL_PARENTEXPNODE_H__
#define __YAL_PARENTEXPNODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class ParentExpNode : public UnaryExpressionNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(ParentExpNode)

    ParentExpNode(const SourceLocationInfo& loc,
                  ExpressionNode* exp);

   virtual ~ParentExpNode() {}

};

}
#endif
