/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/ast/exprintegerliteral.h"
#include "yal/frontend/module.h"
#include "yal/frontend/types/typebuiltin.h"
#include "yal/frontend/ast/astvisitor.h"

namespace yal::frontend{


    StringRef IntegerTypeToStr(const IntegerType type) {
        static const char* sStrType[] = {
            "i8",
            "u8",
            "i16",
            "u16",
            "i32",
            "u32",
            "i64",
            "u64"
        };
        YAL_ASSERT(static_cast<size_t>(type)
                   < (sizeof(sStrType)/sizeof(sStrType[0])));
        return sStrType[static_cast<uint8_t>(type)];
    }

    ExprIntegerLiteral::ExprIntegerLiteral(Module& module,
                                           const SourceInfo& srcInfo,
                                           const IntegerType integerType,
                                           const uint64_t value) :
        StmtExpression(module, AstType::ExprIntegerLiteral, srcInfo),
        m_integerType(integerType),
        m_literalValue(value){

        const Type* type= nullptr;
        switch (integerType) {
        case IntegerType::I8:
            type = module.getTypeContext().getTypeBuiltinI8();
            break;
        case IntegerType::U8:
            type = module.getTypeContext().getTypeBuiltinU8();
            break;
        case IntegerType::I16:
            type = module.getTypeContext().getTypeBuiltinI16();
            break;
        case IntegerType::U16:
            type = module.getTypeContext().getTypeBuiltinU16();
            break;
        case IntegerType::I32:
            type = module.getTypeContext().getTypeBuiltinI32();
            break;
        case IntegerType::U32:
            type = module.getTypeContext().getTypeBuiltinU32();
            break;
        case IntegerType::I64:
            type = module.getTypeContext().getTypeBuiltinI64();
            break;
        case IntegerType::U64:
            type = module.getTypeContext().getTypeBuiltinU64();
            break;
        default:
            YAL_ASSERT_MESSAGE(false, "Unknown integer type");
            break;
        }
        YAL_ASSERT(type != nullptr);
        Qualifier qual = Qualifier();
        qual.setMutable();
        m_qualType = QualType::Create(qual,type);
    }

    int8_t
    ExprIntegerLiteral::getValueAsI8() const {
        YAL_ASSERT(m_integerType == IntegerType::I8);
        return static_cast<int8_t>(reinterpret_cast<const int64_t&>(m_literalValue));
    }

    uint8_t
    ExprIntegerLiteral::getValueAsU8() const {
        YAL_ASSERT(m_integerType == IntegerType::U8);
        return static_cast<uint8_t>(m_literalValue);
    }

    int16_t
    ExprIntegerLiteral::getValueAsI16() const {
        YAL_ASSERT(m_integerType == IntegerType::I16);
        return static_cast<int16_t>(reinterpret_cast<const int64_t&>(m_literalValue));
    }

    uint16_t
    ExprIntegerLiteral::getValueAsU16() const{
        YAL_ASSERT(m_integerType == IntegerType::U16);
        return static_cast<uint32_t>(m_literalValue);
    }

    int32_t
    ExprIntegerLiteral::getValueAsI32() const {
        YAL_ASSERT(m_integerType == IntegerType::I32);
        return static_cast<int32_t>(reinterpret_cast<const int64_t&>(m_literalValue));
    }

    uint32_t
    ExprIntegerLiteral::getValueAsU32() const{
        YAL_ASSERT(m_integerType == IntegerType::U32);
        return static_cast<uint32_t>(m_literalValue);
    }

    int64_t
    ExprIntegerLiteral::getValueAsI64() const {
        YAL_ASSERT(m_integerType == IntegerType::I64);
        return reinterpret_cast<const int64_t&>(m_literalValue);
    }

    uint64_t
    ExprIntegerLiteral::getValueAsU64() const {
        YAL_ASSERT(m_integerType == IntegerType::U64);
        return m_literalValue;
    }

    void
    ExprIntegerLiteral::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}

