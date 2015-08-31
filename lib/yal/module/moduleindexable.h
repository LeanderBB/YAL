#ifndef __YAL_MODULEINDEXABLE_H__
#define __YAL_MODULEINDEXABLE_H__

#include "yal/yal.h"

namespace yal
{

class ModuleIndexable
{
public:
    static const yal_u32 IndexUnused;
    static const yal_u32 IndexMax;

    virtual ~ModuleIndexable(){}

    yal_u32 moduleIndex() const
    {
        return _moduleIndex;
    }

protected:
    ModuleIndexable():
        _moduleIndex(IndexUnused)
    {

    }

private:
    void setModuleIndex(const yal_u32 idx) const
    {
        _moduleIndex = idx;
    }

private:
    friend class Module;
    mutable yal_u32 _moduleIndex;

};

}

#endif

