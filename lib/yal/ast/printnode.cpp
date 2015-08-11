#include "yal/ast/printnode.h"

namespace yal
{

PrintNode::PrintNode(const SourceLocationInfo& loc,
                     PrintArgsNode* args):
    StatementNode(loc),
    _arguments(args)
{

}

}
