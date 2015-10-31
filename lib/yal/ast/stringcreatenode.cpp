#include "yal/ast/stringcreatenode.h"
#include "yal/ast/variabledeclnode.h"
#include "yal/ast/constantnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(StringCreateNode)

StringCreateNode::StringCreateNode(ConstantNode* constNode):
    ExpressionNode(constNode->locationInfo()),
    _constNode(constNode)
{
    constNode->setParentNode(this);
    YAL_ASSERT(constNode->constantType()->isStringContant());
}

}

