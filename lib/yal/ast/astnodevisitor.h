#ifndef __YAL_ASTNODEVISITOR_H__
#define __YAL_ASTNODEVISITOR_H__

#include "yal/ast/astnodetypes.h"

namespace yal
{

class AstNodeVisitor
{
public:

    virtual ~AstNodeVisitor(){}
#define YAL_NODE_LIST_FUNC(CLASS) virtual void visit(CLASS& node) = 0;
#include "yal/ast/astnodelist.h"
#undef YAL_NODE_LIST_FUNC

private:
};


}

#endif

