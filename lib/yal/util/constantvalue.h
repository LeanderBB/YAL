#ifndef __YAL_CONSTANTVALUE_H__
#define __YAL_CONSTANTVALUE_H__

#include "yal/yal.h"
#include "yal/enums.h"
#include "yal/symbols/symbol.h"

namespace yal
{

class ConstantValue
{
public:

    explicit ConstantValue(const yal_bool boolean);

    explicit ConstantValue(const yal_i32 integer32);

    explicit ConstantValue(const yal_u32 uinteger32);

    explicit ConstantValue(const yal_i64 integer64);

    explicit ConstantValue(const yal_u64 uinteger64);

    explicit ConstantValue(const yal_f32 float32);

    explicit ConstantValue(const yal_f64 float64);

    explicit ConstantValue(const char* text);

    ~ConstantValue();

    ConstantType type() const
    {
        return _type;
    }

    yal_bool valueAsBoolean() const;

    yal_i32 valueAsInt32() const;

    yal_u32 valueAsUInt32() const;

    yal_i64 valueAsInt64() const;

    yal_u64 valueAsUInt64() const;

    yal_f32 valueAsFloat32() const;

    yal_f64 valueAsFloat64() const;

    yal_u32 valueAs32BitRaw() const;

    yal_u64 valueAs64BitRaw() const;

    const char* valueAsText() const;

    bool valueFitsInByteCode() const;

    bool valueIs32Bits() const;

    bool valueIs64Bits() const;

    bool operator == (const ConstantValue& other) const;

    bool operator != (const ConstantValue& other) const;

private:
    const ConstantType _type;
    union
    {
        yal_bool boolean;
        yal_i32 integer32;
        yal_u32 uinteger32;
        yal_i64 integer64;
        yal_u64 uinteger64;
        yal_f32 float32;
        yal_f64 float64;
        const char* id;
    } _value;
};

}
#endif
