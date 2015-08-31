#include "yal/util/constantvalue.h"
#include <limits>
#include <cstring>

namespace yal
{
ConstantValue::ConstantValue(const yal_bool boolean):
    _type(kConstantTypeBool)
{
    _value.boolean = boolean;
}

ConstantValue::ConstantValue(const yal_i32 integer32):
    _type(kConstantTypeInt32)
{
    _value.integer32 = integer32;
}

ConstantValue::ConstantValue(const yal_u32 uinteger32):
    _type(kConstantTypeUInt32)
{
    _value.uinteger32 = uinteger32;
}

ConstantValue::ConstantValue(const yal_i64 integer64):
    _type(kConstantTypeInt64)
{
    _value.integer64 = integer64;
}

ConstantValue::ConstantValue(const yal_u64 uinteger64):
    _type(kConstantTypeUInt64)
{
    _value.uinteger64 = uinteger64;
}

ConstantValue::ConstantValue(const yal_f32 float32):
    _type(kConstantTypeFloat32)
{
    _value.float32 = float32;
}

ConstantValue::ConstantValue(const yal_f64 float64):
    _type(kConstantTypeFloat64)
{
    _value.float64 = float64;
}

ConstantValue::ConstantValue(const char* id,
                             const bool isText):
    _type(isText ? kConstantTypeText : kConstantTypeId)
{
    _value.id = id;
}

ConstantValue::~ConstantValue()
{

}

yal_bool
ConstantValue::valueAsBoolean() const
{
    YAL_ASSERT(_type.data.builtin == kConstantTypeBool);
    return _value.boolean;
}

yal_i32
ConstantValue::valueAsInt32() const
{
    YAL_ASSERT(_type.data.builtin == kConstantTypeInt32);
    return _value.integer32;
}

yal_u32
ConstantValue::valueAsUInt32() const
{
    YAL_ASSERT(_type.data.builtin == kConstantTypeUInt32);
    return _value.uinteger32;
}

yal_i64
ConstantValue::valueAsInt64() const
{
    YAL_ASSERT(_type.data.builtin == kConstantTypeInt64);
    return _value.integer64;
}

yal_u64
ConstantValue::valueAsUInt64() const
{
    YAL_ASSERT(_type.data.builtin == kConstantTypeUInt64);
    return _value.uinteger64;
}

yal_f32
ConstantValue::valueAsFloat32() const
{
    YAL_ASSERT(_type.data.builtin == kConstantTypeFloat32);
    return _value.float32;
}

yal_f64
ConstantValue::valueAsFloat64() const
{
    YAL_ASSERT(_type.data.builtin == kConstantTypeFloat64);
    return _value.float64;
}


yal_u32
ConstantValue::valueAs32BitRaw() const
{
    return _value.uinteger32;
}

yal_u64
ConstantValue::valueAs64BitRaw() const
{
    return _value.uinteger64;
}

const char*
ConstantValue::valueAsId() const
{
    YAL_ASSERT(_type.data.builtin == kConstantTypeId);
    return _value.id;
}

const char*
ConstantValue::valueAsText() const
{
    YAL_ASSERT(_type.data.builtin == kConstantTypeText);
    return _value.id;
}

bool
ConstantValue::valueFitsInByteCode() const
{
    switch(_type.data.builtin)
    {
    case kConstantTypeBool:
        return true;
    case kConstantTypeInt32:
    {
        const yal_i32 val = valueAsInt32();
        return val >= std::numeric_limits<yal_i16>::min()
                && val <= std::numeric_limits<yal_i16>::max();
    }
    case kConstantTypeUInt32:
    {
        const yal_u32 val = valueAsUInt32();
        return val <= std::numeric_limits<yal_u16>::max();
    }
    case kConstantTypeInt64:
    {
        const yal_i64 val = valueAsInt64();
        return val >= std::numeric_limits<yal_i16>::min()
                && val <= std::numeric_limits<yal_i16>::max();
    }
    case kConstantTypeUInt64:
    {
        const yal_u64 val = valueAsUInt64();
        return val <= std::numeric_limits<yal_u16>::max();
    }
    case kConstantTypeFloat32:
    case kConstantTypeFloat64:
    default:
        return false;
    }
}

bool
ConstantValue::valueIs32Bits() const
{
    return ConstantTypeIs32Bits(_type.data.builtin);
}

bool
ConstantValue::valueIs64Bits() const
{
    return ConstantTypeIs64Bits(_type.data.builtin);
}

bool
ConstantValue::operator == (const ConstantValue& other) const
{
    if (_type != other._type)
    {
        return false;
    }

    switch(_type.data.builtin)
    {
    case kConstantTypeText:
        return strcmp(valueAsText(), other.valueAsText()) == 0;
    case kConstantTypeBool:
        return valueAsBoolean() == other.valueAsBoolean();
    case kConstantTypeInt32:
        return valueAsInt32() == other.valueAsInt32();
    case kConstantTypeUInt32:
        return valueAsUInt32() == other.valueAsUInt32();
    case kConstantTypeFloat32:
        return valueAsFloat32() == other.valueAsFloat32();
    case kConstantTypeFloat64:
        return valueAsFloat64() == other.valueAsFloat64();
    case kConstantTypeInt64:
        return valueAsInt64() == other.valueAsInt64();
    case kConstantTypeUInt64:
        return valueAsUInt64() == other.valueAsUInt64();
    default:
        break;
    }
    return false;
}

bool
ConstantValue::operator != (const ConstantValue& other) const
{
    return ! operator ==(other);
}


}
