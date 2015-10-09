#include "yal/ast/printnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(PrintArgsNode)
YAL_AST_NODE_ACCEPT_IMP(PrintNode)

PrintNode::PrintNode(const SourceLocationInfo& loc,
                     PrintArgsNode* args):
    StatementNode(loc),
    _arguments(args)
{
    args->setParentNode(this);
}

}
