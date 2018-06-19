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

#pragma once
#include "yal/frontend/types/type.h"

namespace yal::frontend {

    class TypeBuiltin final : public Type {
    public:

        enum class DataType {
            Boolean,
            Int8,
            UInt8,
            Int16,
            UInt16,
            Int32,
            UInt32,
            Int64,
            UInt64,
            Float32,
            Float64
        };

        TypeBuiltin(const DataType builtinType);

        ~TypeBuiltin() = default;

        bool isBool() const;
        bool isInt8() const;
        bool isUInt8() const;
        bool isInt16() const;
        bool isUInt16() const;
        bool isInt32() const;
        bool isUInt32() const;
        bool isInt64() const;
        bool isUInt64() const;
        bool isFloat32() const;
        bool isFloat64() const;
        bool isSigned() const;
        bool isUnsigned() const;
        bool isInteger() const;
        bool isFloat() const;
        bool isNumeric() const;

        DataType getDataType() const {
            return m_builtinType;
        }

    protected:
        virtual bool isCastableToAutoImpl(const Type& other) const override final;

        virtual bool isCastableToRequestImpl(const Type& other) const override final;
    private:
        const DataType m_builtinType;
    };

}
