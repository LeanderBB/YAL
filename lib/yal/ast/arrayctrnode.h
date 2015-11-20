#ifndef __YAL_ARRAYCTRNODE_H__
#define __YAL_ARRAYCTRNODE_H__

#include <yal/ast/astbasenode.h>

namespace yal
{

class ArrayCtrNode : public MultiExpressionNode
{
public:
    YAL_AST_NODE_ACCEPT_HDR(ArrayCtrNode)

    ArrayCtrNode(const SourceLocationInfo& loc,
                 ArrayType* explicitType = nullptr);

    virtual ~ArrayCtrNode() {}


    bool hasExplicitType() const
    {
        return _explicitType != nullptr;
    }

    ArrayType* explicitType() const
    {
        return _explicitType;
    }

    void setExplicitType(ArrayType* type)
    {
        _explicitType = type;
    }

private:
    ArrayType* _explicitType;
};

}
#endif
