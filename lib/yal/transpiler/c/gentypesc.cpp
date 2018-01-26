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

#include "yal/transpiler/c/gentypesc.h"
#include "yal/ast/typebuiltin.h"
#include "yal/ast/typedecl.h"
namespace yal {

    static const char*
    CTypeForDataType(const TypeBuiltin::DataType dt) {
        switch(dt) {
        case TypeBuiltin::DataType::Boolean:
            return "int8_t";
        case TypeBuiltin::DataType::Int8:
            return "int8_t";
        case TypeBuiltin::DataType::UInt8:
            return "uint8_t";
        case TypeBuiltin::DataType::Int16:
            return "int16_t";
        case TypeBuiltin::DataType::UInt16:
            return "uint16_t";
        case TypeBuiltin::DataType::Int32:
            return "int32_t";
        case TypeBuiltin::DataType::UInt32:
            return "uint32_t";
        case TypeBuiltin::DataType::Int64:
            return "int64_t";
        case TypeBuiltin::DataType::UInt64:
            return "uint64_t";
        case TypeBuiltin::DataType::Float32:
            return "float";
        case TypeBuiltin::DataType::Float64:
            return "double";
        default:
            YAL_ASSERT_MESSAGE(false, "Unknown builtin type");
            return "Unknown";
        }
    }

    std::string
    GenTypesC::GenIdentifier(const Identifier& identifier) {
        return identifier.getAsString().replace("::", "_");
    }

    void
    GenTypesC::GenIdentifierFromType(CodeWriter &writer,
                                     const Type &type) {
        switch (type.getKind()) {
        case Type::Kind::TypeBuiltin:
            GenIdentifierFromType(writer, static_cast<const TypeBuiltin&>(type));
            break;

        case Type::Kind::TypeDecl:
            GenIdentifierFromType(writer, static_cast<const TypeDecl&>(type));
            break;
        default:
            YAL_ASSERT(false);
        }
    }

    void
    GenTypesC::GenIdentifierFromType(CodeWriter &writer,
                                     const TypeBuiltin& type) {
        writer.write("%", CTypeForDataType(type.getDataType()));
    }

    void
    GenTypesC::GenIdentifierFromType(CodeWriter &writer,
                                     const TypeDecl& type) {
        std::string cidentifier = GenIdentifier(type.getIdentifier());
        const char* formatStr = type.isStruct()
                ? "struct %"
                : "%";
        writer.write(formatStr, cidentifier.c_str());
    }

    void
    GenTypesC::GenFromQualType(CodeWriter &writer,
                               const QualType& qt){
        const Qualifier qual = qt.getQualifier();
        const Type* type = qt.getType();

        if (qual.isImmutable()) {
            writer.write("const ");
        }


        GenTypesC::GenIdentifierFromType(writer, *type);

        // TODO: return types / input types have different pointer
        // semantic if we have RVO enabled or disabled
        if (qual.isReference()) {
            writer.write("*");
        }

        writer.write(" ");
    }
}
