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

#include "yal/frontend/types/typebuiltin.h"

namespace yal::frontend {

    TypeBuiltin::TypeBuiltin(const DataType builtinType):
        Type(Kind::TypeBuiltin),
        m_builtinType(builtinType) {

        switch(m_builtinType) {
        case DataType::Boolean:
            m_identifier.setIdString("boolean");
            m_sizeBytes = 1;
            break;
        case DataType::Int8:
            m_identifier.setIdString("i8");
            m_sizeBytes = 1;
            break;
        case DataType::UInt8:
            m_identifier.setIdString("u8");
            m_sizeBytes = 1;
            break;
        case DataType::Int16:
            m_identifier.setIdString("i16");
            m_sizeBytes = 2;
            break;
        case DataType::UInt16:
            m_identifier.setIdString("u16");
            m_sizeBytes = 1;
            break;
        case DataType::Int32:
            m_identifier.setIdString("i32");
            m_sizeBytes = 4;
            break;
        case DataType::UInt32:
            m_identifier.setIdString("u32");
            m_sizeBytes = 4;
            break;
        case DataType::Int64:
            m_identifier.setIdString("i64");
            m_sizeBytes = 8;
            break;
        case DataType::UInt64:
            m_identifier.setIdString("u64");
            m_sizeBytes = 8;
            break;
        case DataType::Float32:
            m_identifier.setIdString("f32");
            m_sizeBytes = 4;
            break;
        case DataType::Float64:
            m_identifier.setIdString("f64");
            m_sizeBytes = 8;
            break;
        default:
            YAL_ASSERT_MESSAGE(false, "Should not be reached!");
            m_sizeBytes = 0;
            break;
        }
        m_trivialCopy = 1;
        m_functionTargetable = 1;
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
    TypeBuiltin::isFloat32() const{
        return m_builtinType == DataType::Float32;
    }

    bool
    TypeBuiltin::isFloat64() const {
        return m_builtinType == DataType::Float64;
    }

    bool
    TypeBuiltin::isSigned() const {
        switch(m_builtinType) {
        case DataType::Int8:
        case DataType::Int16:
        case DataType::Int32:
        case DataType::Int64:
        case DataType::Float32:
        case DataType::Float64:
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
    TypeBuiltin::isFloat() const {
        switch(m_builtinType) {
        case DataType::Float32:
        case DataType::Float64:
            return true;
        default:
            return false;
        }
    }

    bool
    TypeBuiltin::isNumeric() const {
        switch(m_builtinType) {
        case DataType::Int8:
        case DataType::Int16:
        case DataType::Int32:
        case DataType::Int64:
        case DataType::UInt8:
        case DataType::UInt16:
        case DataType::UInt32:
        case DataType::UInt64:
        case DataType::Float32:
        case DataType::Float64:
            return true;
        default:
            return false;
        }
    }

    bool
    TypeBuiltin::isCastableToAutoImpl(const Type &other) const {
        if (other.getKind() != Kind::TypeBuiltin) {
            return false;
        }

        const TypeBuiltin& builtinOther = static_cast<const TypeBuiltin&>(other);

        if (builtinOther.getDataType() == m_builtinType) {
            return true;
        }

        switch(m_builtinType) {
        case DataType::Int8:
            switch(builtinOther.getDataType()) {
            case DataType::Int16:
            case DataType::Int32:
            case DataType::Int64:
                return true;
            default:
                return false;
            }
        case DataType::Int16:
            switch(builtinOther.getDataType()) {
            case DataType::Int32:
            case DataType::Int64:
                return true;
            default:
                return false;
            }
        case DataType::Int32:
            return builtinOther.getDataType() == DataType::Int64;

        case DataType::UInt8:
            switch(builtinOther.getDataType()) {
            case DataType::UInt16:
            case DataType::UInt32:
            case DataType::UInt64:
                return true;
            default:
                return false;
            }
        case DataType::UInt16:
            switch(builtinOther.getDataType()) {
            case DataType::UInt32:
            case DataType::UInt64:
                return true;
            default:
                return false;
            }
        case DataType::UInt32:
            return builtinOther.getDataType() == DataType::UInt64;
        case DataType::Float32:
            return builtinOther.getDataType() == DataType::Float64;
        default:
            break;
        }
        return false;
    }

    bool
    TypeBuiltin::isCastableToRequestImpl(const Type &other) const {
        return other.getKind() == Kind::TypeBuiltin;
    }

}
