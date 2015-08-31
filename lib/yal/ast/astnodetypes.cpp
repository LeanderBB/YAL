#include "yal/ast/astnodetypes.h"

namespace yal
{

bool
IsExpressionNode(const AstType type)
{
    switch(type)
    {
    case kAstTypeSingleOperatorNode:
    case kAstTypeDualOperatorNode:
    case kAstTypeAssignOperatorNode:
    case kAstTypeConstantNode:
        return true;
    default:
        return false;
    }
}


}
