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

#include "yal/frontend/ast/stmtexpression.h"

namespace yal::frontend {

    enum class IntegerType {
        I8,
        U8,
        I16,
        U16,
        I32,
        U32,
        I64,
        U64
    };

    StringRef IntegerTypeToStr(const IntegerType type);

    class ExprIntegerLiteral : public StmtExpression
    {
    public:

        ExprIntegerLiteral(Module& module,
                           const SourceInfo &srcInfo,
                           const IntegerType integerType,
                           uint64_t value);

        uint64_t getLiteralValue() const {
            return m_literalValue;
        }

        IntegerType getIntegerType() const {
            return m_integerType;
        }

        int8_t getValueAsI8() const;

        uint8_t getValueAsU8() const;

        int16_t getValueAsI16() const;

        uint16_t getValueAsU16() const;

        int32_t getValueAsI32() const;

        uint32_t getValueAsU32() const;

        int64_t getValueAsI64() const;

        uint64_t getValueAsU64() const;

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        const IntegerType m_integerType;
        uint64_t m_literalValue;
    };

}
