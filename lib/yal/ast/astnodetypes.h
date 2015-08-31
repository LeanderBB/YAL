#ifndef __YAL_ASTNODETYPES_H__
#define __YAL_ASTNODETYPES_H__

namespace yal
{

class AstBaseNode;
enum AstType
{
#define YAL_NODE_LIST_FUNC(x) kAstType ## x,
#include "yal/ast/astnodelist.h"
#undef YAL_NODE_LIST_FUNC
    kAstTypeMax
};

#define YAL_AST_NODE_TYPE(class) kAstType ## class

#define YAL_NODE_LIST_FUNC(x) class x;
#include "yal/ast/astnodelist.h"
#undef YAL_NODE_LIST_FUNC

bool
IsExpressionNode(const AstType type);

}

#endif

