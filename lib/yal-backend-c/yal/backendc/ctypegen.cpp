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

#include "yal/backendc/ctypegen.h"

#include "yal/backendc/ctype.h"
#include "yal/frontend/ast/declfunction.h"
#include "yal/frontend/ast/declparamvar.h"
#include "yal/frontend/ast/declmodule.h"
#include "yal/frontend/ast/declstruct.h"
#include "yal/frontend/types/operators.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/types/typestruct.h"
#include "yal/util/codewriter.h"
#include "yal/util/format.h"

namespace yal::backend::c {

    void
    CTypeGen::GenDelcFunction(yal::CodeWriter& writer,
                              const CTypeCache& cache,
                              const yal::frontend::DeclFunction& decl) {
        Formater& formater = writer.getFormater();
        const CType* ctypeFn = cache.getCType(decl.getFunctionType());
        YAL_ASSERT(ctypeFn != nullptr);

        // write return type
        if (decl.hasReturnValue()) {
            const yal::frontend::QualType qt = decl.getReturnType();
            const CType* ctype = cache.getCType(*qt.getType());
            YAL_ASSERT(ctype != nullptr);
            FormatReset(formater);
            GenQualTypeReturn(formater, qt, *ctype);
            writer.write(formater);
        } else {
            Format(formater,"void");
            writer.write(formater);
        }
        writer.write();

        // write fwd decl
        Format(formater, "%(", ctypeFn->getCIdentifier());
        writer.write(formater);
        const auto& params = decl.getParams();
        if (!params.empty()) {
            writer.write();
            writer.ident();
            bool isFirst = true;
            for (auto& param : params) {
                FormatReset(formater);
                FormatAppend(formater, !isFirst ? "," : " ");
                const yal::frontend::QualType qtParam = param->getQualType();
                const CType* ctypeParam = cache.getCType(*qtParam.getType());
                YAL_ASSERT(ctypeParam != nullptr);
                GenQualType(formater, qtParam, *ctypeParam);
                FormatAppend(formater, "%\n", param->getName());
                isFirst = false;
                writer.write(formater);
            }
            writer.unident();
        }
        Format(formater,")");
        writer.write(formater);
    }

    void
    CTypeGen::GenDelcStruct(yal::CodeWriter& writer,
                            const CTypeCache& cache,
                            const yal::frontend::DeclStruct& decl) {
        Formater& formater = writer.getFormater();
        const CType* ctypeStruct = cache.getCType(decl.getStructType());
        YAL_ASSERT(ctypeStruct != nullptr);
        Format(formater, "typedef struct {\n");
        writer.write(formater);
        writer.ident();
        for (auto& member : decl.getMembers()) {
            FormatReset(formater);
            const yal::frontend::QualType qtMember = member->getQualType();
            const CType* ctypeMember = cache.getCType(*qtMember.getType());
            YAL_ASSERT(ctypeMember != nullptr);
            GenQualType(formater, qtMember, *ctypeMember);
            FormatAppend(formater, "%;\n", member->getName());
            writer.write(formater);
        }
        writer.unident();
        Format(formater, "} %", ctypeStruct->getCIdentifier());
        writer.write(formater);
    }

    void
    CTypeGen::GenQualType(Formater& formater,
                          const yal::frontend::QualType& qt,
                          const CType& type) {
        if (qt.isImmutable()) {
            FormatAppend(formater, "const ");
        }

        FormatAppend(formater, "% ",type.getCIdentifier());

        if (qt.isReference()) {
            FormatAppend(formater, "*");
        }
    }

    void
    CTypeGen::GenQualTypeReturn(Formater& formater,
                                const yal::frontend::QualType& qt,
                                const CType& type) {
        // only add const if type is reference, as all return
        // types are copies
        if (qt.isImmutable() && qt.isReference()) {
            FormatAppend(formater, "const ");
        }

        FormatAppend(formater, "% ",type.getCIdentifier());

        if (qt.isReference()) {
            FormatAppend(formater, "*");
        }
    }

    void
    CTypeGen::GetBuilitinTypeInfo(yal::CodeWriter& writer) {
        //TODO: Replace with a special header at some point
        Formater& formater = writer.getFormater();
        Format(formater, "#if !defined(YAL_BUILTIN_TYPES_DEFINED)\n");
        FormatAppend(formater, "#define YAL_BUILTIN_TYPES_DEFINED\n");
        FormatAppend(formater, "#include<stdint.h>\n");
        FormatAppend(formater, "typedef uint8_t yal_bool;\n");
        FormatAppend(formater, "#define YAL_TRUE ((yal_bool)1)\n");
        FormatAppend(formater, "#define YAL_FALSE ((yal_bool)0)\n");
        FormatAppend(formater, "typedef int8_t yal_i8;\n");
        FormatAppend(formater, "typedef int16_t yal_i16;\n");
        FormatAppend(formater, "typedef int32_t yal_i32;\n");
        FormatAppend(formater, "typedef int64_t yal_i64;\n");
        FormatAppend(formater, "typedef uint8_t yal_u8;\n");
        FormatAppend(formater, "typedef uint16_t yal_u16;\n");
        FormatAppend(formater, "typedef uint32_t yal_u32;\n");
        FormatAppend(formater, "typedef uint64_t yal_u64;\n");
        FormatAppend(formater, "typedef float yal_f32;\n");
        FormatAppend(formater, "typedef double yal_f64;\n");
        FormatAppend(formater, "#endif // YAL_BUILTIN_TYPES_DEFINED\n\n");
        writer.write(formater);
    }

    StringRef
    CTypeGen::GenUnaryOperator(const yal::frontend::UnaryOperatorType op) {
        switch (op) {
        case yal::frontend::UnaryOperatorType::Not:
            return "!";
        case yal::frontend::UnaryOperatorType::BitNot:
            return "~";
        case yal::frontend::UnaryOperatorType::Negate:
            return "-";
        case yal::frontend::UnaryOperatorType::Reference:
            return "&";
        default:
            YAL_ASSERT_MESSAGE(false, "unknown unary operator type");
            return "";
        }
    }

    StringRef
    CTypeGen::GenBinaryOperator(const yal::frontend::BinaryOperatorType op) {
        switch(op) {
        case yal::frontend::BinaryOperatorType::Plus:
            return "+";
        case yal::frontend::BinaryOperatorType::Minus:
            return "-";
        case yal::frontend::BinaryOperatorType::Div:
           return "/";
        case yal::frontend::BinaryOperatorType::Mult:
            return "*";
        case yal::frontend::BinaryOperatorType::Mod:
            return "%";
        case yal::frontend::BinaryOperatorType::And:
            return "&&";
        case yal::frontend::BinaryOperatorType::Or:
            return "||";
        case yal::frontend::BinaryOperatorType::BitAnd:
            return "&";
        case yal::frontend::BinaryOperatorType::BitOr:
            return "|";
        case yal::frontend::BinaryOperatorType::BitXor:
            return "^";
        case yal::frontend::BinaryOperatorType::Eq:
            return "==";
        case yal::frontend::BinaryOperatorType::Ne:
            return "!=";
        case yal::frontend::BinaryOperatorType::Gt:
            return ">";
        case yal::frontend::BinaryOperatorType::Ge:
            return ">=";
        case yal::frontend::BinaryOperatorType::Le:
            return "<=";
        case yal::frontend::BinaryOperatorType::Lt:
            return "<";
        default:
            YAL_ASSERT_MESSAGE(false, "unknown binary operator type");
        }
    }

}
