/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
 *
 *  This file is part of YAL.
 *
 *  YAL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  YAL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with YAL. If not, see <http://www.gnu.org/licenses/>.
 */

#include "yal/backendc/ctypebuiltin.h"
#include "yal/frontend/types/typebuiltin.h"
#include "yal/util/codewriter.h"
#include "yal/util/stringref.h"

#include <limits>
#include <type_traits>

namespace yf = yal::frontend;
namespace yal::backend::c {

    static const bool kBuilitinConverionTable
    [uint8_t(yf::TypeBuiltin::DataType::DataTypeCount)]
    [uint8_t(yf::TypeBuiltin::DataType::DataTypeCount)] = {
            //Bool, Int8 , UInt8, Int16, UInt16, Int32, UInt32, Int64, UInt64, Flt32, Flt64
    {false, false, false, false, false, false, false, false, false, false, false} // Bool
            ,{false, false, true , false, false, false, false, false, false, false, false} // Int8
            ,{false, true , false, false, false, false, false, false, false, false, false} // UInt8
            ,{false, true , true , false, true , false, false, false, false, false, false} // Int16
            ,{false, true , true , true , false, false, false, false, false, false, false} // UInt16
            ,{false, true , true , true , true , false, true , false, false, false , false} // Int32
            ,{false, true , true , true , true , true , false, false, false, false , false} // UInt32
            ,{false, true , true , true , true , true , true , false, true , false , false} // Int64
            ,{false, false, false, false, false, false, false, false, false, false , false} // UInt64
            ,{false, false, false, false, false, false, false, false, false, false, false} // Float32
            ,{false, false, false, false, false, false, false, false, false, false, false} // Float64
};

    static const StringRef
    kBuiltinCTypeTable[uint8_t(yf::TypeBuiltin::DataType::DataTypeCount)] = {
            "yal_bool"
            ,"yal_i8"
            ,"yal_u8"
            ,"yal_i16"
            ,"yal_u16"
            ,"yal_i32"
            ,"yal_u32"
            ,"yal_i64"
            ,"yal_u64"
            ,"yal_f32"
            ,"yal_f64"
};

    StringRef
    CTypeBuilitin::GetCTypeName(const frontend::TypeBuiltin& type) {
        const frontend::TypeBuiltin::DataType dt = type.getDataType();
        const uint8_t index = uint8_t(dt);
        YAL_ASSERT(index < uint8_t(yf::TypeBuiltin::DataType::DataTypeCount));
        return kBuiltinCTypeTable[index];
    }

    bool
    CTypeBuilitin::GenConversion(yal::CodeWriter& writer,
                                 const frontend::TypeBuiltin& from,
                                 const frontend::TypeBuiltin& to) {
        const uint8_t indexFrom = uint8_t(from.getDataType());
        const uint8_t indexTo = uint8_t(to.getDataType());
        YAL_ASSERT(indexFrom < uint8_t(yf::TypeBuiltin::DataType::DataTypeCount));
        YAL_ASSERT(indexTo < uint8_t(yf::TypeBuiltin::DataType::DataTypeCount));

        if (!kBuilitinConverionTable[indexFrom][indexTo]) {
            return false;
        }

        StringRef typeFrom = kBuiltinCTypeTable[indexFrom];
        StringRef typeTo = kBuiltinCTypeTable[indexTo];
        writer.write("%_as_%",
                     typeFrom, typeTo);
        return true;
    }

    void
    CTypeBuilitin::GenBuiltinHeader(yal::CodeWriter& writer) {
        writer.write("#ifndef __YAL_CORE_BUILTIN__\n");
        writer.write("#define __YAL_CORE_BUILTIN__\n\n");

        writer.write("#include<stdint.h>\n");
        writer.write("typedef uint8_t yal_bool;\n");
        writer.write("#define YAL_TRUE ((yal_bool)1)\n");
        writer.write("#define YAL_FALSE ((yal_bool)0)\n");
        writer.write("typedef int8_t yal_i8;\n");
        writer.write("typedef int16_t yal_i16;\n");
        writer.write("typedef int32_t yal_i32;\n");
        writer.write("typedef int64_t yal_i64;\n");
        writer.write("typedef uint8_t yal_u8;\n");
        writer.write("typedef uint16_t yal_u16;\n");
        writer.write("typedef uint32_t yal_u32;\n");
        writer.write("typedef uint64_t yal_u64;\n");
        writer.write("typedef float yal_f32;\n");
        writer.write("typedef double yal_f64;\n");

        // builtin conversion function
        const uint8_t typeCount = uint8_t(yf::TypeBuiltin::DataType::DataTypeCount);
        for (uint8_t i = 0; i < typeCount; ++i) {
            for (uint8_t j = 0; j < typeCount; ++j) {
                if (kBuilitinConverionTable[i][j]) {
                    StringRef typeFrom = kBuiltinCTypeTable[i];
                    StringRef typeTo = kBuiltinCTypeTable[j];
                    writer.write("extern % %_as_%(const % in);\n",
                                 typeTo, typeFrom, typeTo, typeFrom);
                }
            }
        }

        writer.write("#endif // __YAL_CORE_BUILTIN__\n\n");
    }

    template <typename From, typename To>
    static typename std::enable_if<sizeof(To) >= sizeof(From)>::type
    ConvertSignedToUnsigned(CodeWriter& writer) {
        static_assert(std::is_signed<From>::value);
        static_assert(std::is_unsigned<To>::value);
        // negative check
        writer.write("if (in < 0) {\n");
        writer.ident();
        writer.write("return 0;\n");
        writer.unident();
        writer.write("}\n\n");
        writer.write("return in;\n");
    }

    template <typename From, typename To>
    static typename std::enable_if<sizeof(To) < sizeof(From)>::type
    ConvertSignedToUnsigned(CodeWriter& writer) {
        static_assert(std::is_signed<From>::value);
        static_assert(std::is_unsigned<To>::value);
        // negative check
        writer.write("if (in < 0) {\n");
        writer.ident();
        writer.write("return 0;\n");
        writer.unident();
        writer.write("}\n\n");
        const From max = From(std::numeric_limits<To>::max());
        writer.write("return (in > %) ? % : in;\n",
                     max, max);
    }

    template <typename From, typename To>
    static void
    ConvertUnsignedToSigned(CodeWriter& writer) {
        static_assert(std::is_signed<To>::value);
        static_assert(std::is_unsigned<From>::value);
        const From max = From(std::numeric_limits<To>::max());
        writer.write("return (in > %) ? % : in;\n",
                     max, max);
    }

    template <typename From, typename To>
    static void
    ConvertUnsignedToUnsigned(CodeWriter& writer) {
        static_assert(std::is_unsigned<To>::value);
        static_assert(std::is_unsigned<From>::value);
        static_assert(sizeof(From) >= sizeof(To));
        const From max = From(std::numeric_limits<To>::max());
        writer.write("return (in > %) ? % : in;\n",
                     max, max);
    }

    template <typename From, typename To>
    static void
    ConvertSingedToSigned(CodeWriter& writer) {
        static_assert(std::is_signed<To>::value);
        static_assert(std::is_signed<From>::value);
        static_assert(sizeof(From) >= sizeof(To));
        const From max = From(std::numeric_limits<To>::max());
        const From min = From(std::numeric_limits<To>::lowest());
        writer.write("return (in > %) ? % : (in < % ? % : in);\n",
                     max, max, min, min);
    }

    static void
    GenConversionSource(CodeWriter& writer,
                        const yf::TypeBuiltin::DataType from,
                        const yf::TypeBuiltin::DataType to) {
        switch(from) {
        // i8
        case yf::TypeBuiltin::DataType::Int8: {
            switch(to) {
            case yf::TypeBuiltin::DataType::UInt8:
                ConvertSignedToUnsigned<int8_t, uint8_t>(writer);
                break;
            default:
                YAL_ASSERT_MESSAGE(false, "Unhandled type conversion");
                break;
            }
            break;
        }
            // u8
        case yf::TypeBuiltin::DataType::UInt8: {
            switch(to) {
            case yf::TypeBuiltin::DataType::Int8:
                ConvertUnsignedToSigned<uint8_t, int8_t>(writer);
                break;
            default:
                YAL_ASSERT_MESSAGE(false, "Unhandled type conversion");
                break;
            }
            break;
        }
            // i16
        case yf::TypeBuiltin::DataType::Int16: {
            switch(to) {
            case yf::TypeBuiltin::DataType::UInt8:
                ConvertSignedToUnsigned<int16_t, uint8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int8:
                ConvertSingedToSigned<int16_t, int8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::UInt16:
                ConvertSignedToUnsigned<int16_t, uint16_t>(writer);
                break;
            default:
                YAL_ASSERT_MESSAGE(false, "Unhandled type conversion");
                break;
            }
            break;
        }
            // u16
        case yf::TypeBuiltin::DataType::UInt16: {
            switch(to) {
            case yf::TypeBuiltin::DataType::UInt8:
                ConvertUnsignedToUnsigned<uint16_t, uint8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int8:
                ConvertUnsignedToSigned<uint16_t, int8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int16:
                ConvertUnsignedToSigned<uint16_t, int16_t>(writer);
                break;
            default:
                YAL_ASSERT_MESSAGE(false, "Unhandled type conversion");
                break;
            }
            break;
        }
            // i32
        case yf::TypeBuiltin::DataType::Int32: {
            switch(to) {
            case yf::TypeBuiltin::DataType::UInt8:
                ConvertSignedToUnsigned<int32_t, uint8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int8:
                ConvertSingedToSigned<int32_t, int8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::UInt16:
                ConvertSignedToUnsigned<int32_t, uint16_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int16:
                ConvertSingedToSigned<int32_t, int16_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::UInt32:
                ConvertSignedToUnsigned<int32_t, uint32_t>(writer);
                break;
            default:
                YAL_ASSERT_MESSAGE(false, "Unhandled type conversion");
                break;
            }
            break;
        }
            // u32
        case yf::TypeBuiltin::DataType::UInt32: {
            switch(to) {
            case yf::TypeBuiltin::DataType::UInt8:
                ConvertUnsignedToUnsigned<uint32_t, uint8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int8:
                ConvertUnsignedToSigned<uint32_t, int8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::UInt16:
                ConvertUnsignedToUnsigned<uint32_t, uint16_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int16:
                ConvertUnsignedToSigned<uint32_t, int16_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int32:
                ConvertUnsignedToSigned<uint32_t, int32_t>(writer);
                break;
            default:
                YAL_ASSERT_MESSAGE(false, "Unhandled type conversion");
                break;
            }
            break;
        }
            // i64
        case yf::TypeBuiltin::DataType::Int64: {
            switch(to) {
            case yf::TypeBuiltin::DataType::UInt8:
                ConvertSignedToUnsigned<int64_t, uint8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int8:
                ConvertSingedToSigned<int64_t, int8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::UInt16:
                ConvertSignedToUnsigned<int64_t, uint16_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int16:
                ConvertSingedToSigned<int64_t, int16_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::UInt32:
                ConvertSignedToUnsigned<int64_t, uint32_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int32:
                ConvertSingedToSigned<int64_t, int32_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::UInt64:
                ConvertSignedToUnsigned<int64_t, uint64_t>(writer);
                break;
            default:
                YAL_ASSERT_MESSAGE(false, "Unhandled type conversion");
                break;
            }
            break;
        }
        // u64
        case yf::TypeBuiltin::DataType::UInt64: {
            switch(to) {
            case yf::TypeBuiltin::DataType::UInt8:
                ConvertUnsignedToUnsigned<uint64_t, uint8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int8:
                ConvertUnsignedToSigned<uint64_t, int8_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::UInt16:
                ConvertUnsignedToUnsigned<uint64_t, uint16_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int16:
                ConvertUnsignedToSigned<uint64_t, int16_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::UInt32:
                ConvertUnsignedToUnsigned<uint64_t, uint32_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int32:
                ConvertUnsignedToSigned<uint64_t, int32_t>(writer);
                break;
            case yf::TypeBuiltin::DataType::Int64:
                ConvertUnsignedToSigned<uint64_t, int64_t>(writer);
                break;
            default:
                YAL_ASSERT_MESSAGE(false, "Unhandled type conversion");
                break;
            }
             break;
        }
        default:
            YAL_ASSERT_MESSAGE(false, "Unhandled type conversion");
            break;
        }
    }

    void
    CTypeBuilitin::GenBuiltinSource(yal::CodeWriter& writer) {
        writer.write("// Builtin conversion functions \n");
        const uint8_t typeCount = uint8_t(yf::TypeBuiltin::DataType::DataTypeCount);
        for (uint8_t i = 0; i < typeCount; ++i) {
            for (uint8_t j = 0; j < typeCount; ++j) {
                if (kBuilitinConverionTable[i][j]) {
                    const StringRef typeFrom = kBuiltinCTypeTable[i];
                    const StringRef typeTo = kBuiltinCTypeTable[j];
                    const yf::TypeBuiltin::DataType dtFrom = yf::TypeBuiltin::DataType(i);
                    const yf::TypeBuiltin::DataType dtTo= yf::TypeBuiltin::DataType(j);
                    writer.write("% %_as_%(const % in) {\n",
                                 typeTo, typeFrom, typeTo, typeFrom);
                    writer.ident();
                    GenConversionSource(writer, dtFrom, dtTo);
                    writer.unident();
                    writer.write("}\n\n");
                }
            }
        }
    }
}
