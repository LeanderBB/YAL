#include "yal/ast/arrayctrnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(ArrayCtrNode)

ArrayCtrNode::ArrayCtrNode(const SourceLocationInfo &loc,
                           ArrayType *explicitType):
    MultiExpressionNode(loc),
    _explicitType(explicitType)
{

}


}

