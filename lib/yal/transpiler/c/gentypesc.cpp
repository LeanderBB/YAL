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
#include "yal/ast/astnodes.h"
namespace yal {

    static const char*
    CTypeForDataType(const TypeBuiltin::DataType dt) {
        switch(dt) {
        case TypeBuiltin::DataType::Boolean:
            return "yal_bool";
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
        return GenIdentifier(identifier.getAsString());
    }

    std::string
    GenTypesC::GenIdentifier(const StringRef& string) {
        return string.replace("::", "_");
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
        if (type.isStruct()) {
            writer.write("t_%", cidentifier);
        } else {
            writer.write("%", cidentifier);
        }
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


    void
    GenTypesC::GenDeclFunctionIntro(CodeWriter& writer,
                                    const DeclFunctionBase& function) {
        if (function.hasReturnValue()) {
            GenTypesC::GenFromQualType(writer, function.getReturnQualType());
        } else {
            writer.write("void ");
        }
        writer.write();
        const std::string id = GenTypesC::GenIdentifier(function.getIdentifier());
        writer.write("%", id);
        if (function.hasFunctionParameters()) {
            const DeclParamVarContainer* params = function.getParams();
            bool first = true;
            writer.write("(");
            writer.ident();
            for (auto& param : params->getChildRangeConst()) {
                if (!first) {
                    writer.write("\n,");
                }

                const QualType paramQt = param->getQualType();
                GenTypesC::GenFromQualType(writer, paramQt);
                // movable variables need to passed in by pointer
                if (paramQt.isMovable()) {
                    writer.write("%*", param->getName());
                } else {
                    writer.write("%", param->getName());
                }
                first = false;
            }
            writer.uindent();
            writer.write(")");
        } else {
            writer.write("()");
        }
    }

    void
    GenTypesC::GenDeclStruct(CodeWriter& writer,
                             const DeclStruct& decl){
        const std::string identifier
                = GenTypesC::GenIdentifier(decl.getIdentifier());
        writer.write("struct % {\n", identifier);
        writer.ident();
        for (auto& member : decl.getMembers()->getChildRangeConst()) {
            GenTypesC::GenFromQualType(writer, member->getQualType());
            writer.write("%;\n", member->getName());
        }
        writer.uindent();
        writer.write("};\ntypedef struct % t_%;\n\n", identifier, identifier);
    }

    void
    GenTypesC::GenUnaryOperator(CodeWriter& writer,
                                const UnaryOperatorType op) {
        switch (op) {
        case UnaryOperatorType::Not:
            writer.write("!");
            break;
        case UnaryOperatorType::BitNot:
            writer.write("~");
            break;
        case UnaryOperatorType::Negate:
            writer.write("-");
            break;
        case UnaryOperatorType::Reference:
            writer.write("&");
            break;
        default:
            YAL_ASSERT_MESSAGE(false, "unknown unary operator type");
        }
    }

    void
    GenTypesC::GenBinaryOperator(CodeWriter& writer,
                                 const BinaryOperatorType op) {
        switch(op) {
        case BinaryOperatorType::Plus:
            writer.write("+");
            break;
        case BinaryOperatorType::Minus:
            writer.write("-");
            break;
        case BinaryOperatorType::Div:
            writer.write("/");
            break;
        case BinaryOperatorType::Mult:
            writer.write("*");
            break;
        case BinaryOperatorType::Mod:
            writer.write("%");
            break;
        case BinaryOperatorType::And:
            writer.write("&&");
            break;
        case BinaryOperatorType::Or:
            writer.write("||");
            break;
        case BinaryOperatorType::BitAnd:
            writer.write("&");
            break;
        case BinaryOperatorType::BitOr:
            writer.write("|");
            break;
        case BinaryOperatorType::BitXor:
            writer.write("^");
            break;
        case BinaryOperatorType::Eq:
            writer.write("==");
            break;
        case BinaryOperatorType::Ne:
            writer.write("!=");
            break;
        case BinaryOperatorType::Gt:
            writer.write(">");
            break;
        case BinaryOperatorType::Ge:
            writer.write(">=");
            break;
        case BinaryOperatorType::Le:
            writer.write("<=");
            break;
        case BinaryOperatorType::Lt:
            writer.write("<");
            break;
        default:
            YAL_ASSERT_MESSAGE(false, "unknown binary operator type");
        }
    }
}
