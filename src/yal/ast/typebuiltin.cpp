/*
 *  Copyright 2017 by Leander Beernaert (leanderbb@gmail.com)
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

#include "yal/ast/typebuiltin.h"

namespace yal {


    static const TypeBuiltin sTypeBool(TypeBuiltin::DataType::Boolean);
    static const TypeBuiltin sTypeInt8(TypeBuiltin::DataType::Int8);
    static const TypeBuiltin sTypeUInt8(TypeBuiltin::DataType::UInt8);
    static const TypeBuiltin sTypeInt16(TypeBuiltin::DataType::Int16);
    static const TypeBuiltin sTypeUInt16(TypeBuiltin::DataType::UInt16);
    static const TypeBuiltin sTypeInt32(TypeBuiltin::DataType::Int32);
    static const TypeBuiltin sTypeUInt32(TypeBuiltin::DataType::UInt32);
    static const TypeBuiltin sTypeInt64(TypeBuiltin::DataType::Int64);
    static const TypeBuiltin sTypeUInt64(TypeBuiltin::DataType::UInt64);
    static const TypeBuiltin sTypeFloat(TypeBuiltin::DataType::Float);
    static const TypeBuiltin sTypeDouble(TypeBuiltin::DataType::Double);

    const TypeBuiltin*
    TypeBuiltin::GetTypeForDataType(const DataType dataType) {
        switch(dataType) {
        case DataType::Boolean:
            return &sTypeBool;
        case DataType::Int8:
            return &sTypeInt8;
        case DataType::UInt8:
            return &sTypeUInt8;
        case DataType::Int16:
            return &sTypeInt16;
        case DataType::UInt16:
            return &sTypeUInt16;
        case DataType::Int32:
            return &sTypeInt32;
        case DataType::UInt32:
            return &sTypeUInt32;
        case DataType::Int64:
            return &sTypeInt64;
        case DataType::UInt64:
            return &sTypeUInt64;
        case DataType::Float:
            return &sTypeFloat;
        case DataType::Double:
            return &sTypeDouble;
        default:
            YAL_ASSERT_MESSAGE(false, "Should not be reached!");
            return nullptr;
        }
    }

    TypeBuiltin::TypeBuiltin(const DataType builtinType):
        Type(Kind::TypeBuiltin),
        m_builtinType(builtinType) {

        switch(m_builtinType) {
        case DataType::Boolean:
            m_name = "boolean";
            m_sizeBytes = 1;
            break;
        case DataType::Int8:
            m_name = "i8";
            m_sizeBytes = 1;
            break;
        case DataType::UInt8:
            m_name = "u8";
            m_sizeBytes = 1;
            break;
        case DataType::Int16:
            m_name = "i16";
            m_sizeBytes = 2;
            break;
        case DataType::UInt16:
            m_name = "u16";
            m_sizeBytes = 1;
            break;
        case DataType::Int32:
            m_name = "i32";
            m_sizeBytes = 4;
            break;
        case DataType::UInt32:
            m_name = "u32";
            m_sizeBytes = 4;
            break;
        case DataType::Int64:
            m_name = "i64";
            m_sizeBytes = 8;
            break;
        case DataType::UInt64:
            m_name = "u64";
            m_sizeBytes = 8;
            break;
        case DataType::Float:
            m_name = "float";
            m_sizeBytes = 4;
            break;
        case DataType::Double:
            m_name = "double";
            m_sizeBytes = 8;
            break;
        default:
            YAL_ASSERT_MESSAGE(false, "Should not be reached!");
            m_sizeBytes = 0;
            break;
        }
        m_flags |= kFlagDefined;
    }


    bool
    TypeBuiltin::isBool() const {
        return m_builtinType == DataType::Boolean;
    }

    bool
    TypeBuiltin::isInt8() const {
        return m_builtinType == DataType::Int8;
    }

    bool
    TypeBuiltin::isUInt8() const{
        return m_builtinType == DataType::UInt8;
    }

    bool
    TypeBuiltin::isInt16() const {
        return m_builtinType == DataType::Int16;
    }

    bool
    TypeBuiltin::isUInt16() const {
        return m_builtinType == DataType::UInt16;
    }

    bool
    TypeBuiltin::isInt32() const{
        return m_builtinType == DataType::Int32;
    }

    bool
    TypeBuiltin::isUInt32() const{
        return m_builtinType == DataType::UInt32;
    }

    bool
    TypeBuiltin::isInt64() const{
        return m_builtinType == DataType::Int64;
    }

    bool
    TypeBuiltin::isUInt64() const{
        return m_builtinType == DataType::UInt64;
    }

    bool
    TypeBuiltin::isFloat() const{
        return m_builtinType == DataType::Float;
    }

    bool
    TypeBuiltin::isDouble() const {
        return m_builtinType == DataType::Double;
    }

    bool
    TypeBuiltin::isSigned() const {
        switch(m_builtinType) {
        case DataType::Int8:
        case DataType::Int16:
        case DataType::Int32:
        case DataType::Int64:
        case DataType::Float:
        case DataType::Double:
            return true;
        default:
            return false;
        }
    }

    bool
    TypeBuiltin::isUnsigned() const {
        switch(m_builtinType) {
        case DataType::UInt8:
        case DataType::UInt16:
        case DataType::UInt32:
        case DataType::UInt64:
            return true;
        default:
            return false;
        }
    }

    bool
    TypeBuiltin::isInteger() const {
        switch(m_builtinType) {
        case DataType::Int8:
        case DataType::Int16:
        case DataType::Int32:
        case DataType::Int64:
        case DataType::UInt8:
        case DataType::UInt16:
        case DataType::UInt32:
        case DataType::UInt64:
            return true;
        default:
            return false;
        }
    }

    bool
    TypeBuiltin::isDecimal() const {
        switch(m_builtinType) {
        case DataType::Float:
        case DataType::Double:
            return true;
        default:
            return false;
        }
    }



}
