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
#include "yal/backendc/ctypebuiltin.h"
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
        const CType* ctypeFn = cache.getCType(decl.getFunctionType());
        YAL_ASSERT(ctypeFn != nullptr);

        // write return type
        if (decl.hasReturnValue()) {
            const yal::frontend::QualType qt = decl.getReturnType();
            const CType* ctype = cache.getCType(*qt.getType());
            YAL_ASSERT(ctype != nullptr);
            GenQualTypeReturn(writer, qt, *ctype);
        } else {
            writer.write("void");
        }
        writer.write();

        // write fwd decl
        writer.write("%(", ctypeFn->getCIdentifier());
        const auto& params = decl.getParams();
        if (!params.empty()) {
            writer.ident();
            bool isFirst = true;
            for (auto& param : params) {
                writer.write(!isFirst ? "\n," : "\n");
                const yal::frontend::QualType qtParam = param->getQualType();
                const CType* ctypeParam = cache.getCType(*qtParam.getType());
                YAL_ASSERT(ctypeParam != nullptr);
                GenQualTypeFnParam(writer, qtParam, *ctypeParam);
                writer.write("%", param->getName());
                isFirst = false;
            }
            writer.unident();
            writer.write("\n)");
        } else {
            writer.write(") ");
        }

    }

    void
    CTypeGen::GenDelcStruct(yal::CodeWriter& writer,
                            const CTypeCache& cache,
                            const yal::frontend::DeclStruct& decl) {
        const CType* ctypeStruct = cache.getCType(decl.getStructType());
        YAL_ASSERT(ctypeStruct != nullptr);
        writer.write("typedef struct % {\n", ctypeStruct->getCIdentifier());
        writer.ident();
        for (auto& member : decl.getMembers()) {
            const yal::frontend::QualType qtMember = member->getQualType();
            const CType* ctypeMember = cache.getCType(*qtMember.getType());
            YAL_ASSERT(ctypeMember != nullptr);
            GenQualType(writer, qtMember, *ctypeMember);
            writer.write("%;\n", member->getName());
        }
        writer.unident();
        writer.write("} %", ctypeStruct->getCIdentifier());
    }

    void
    CTypeGen::GenQualType(CodeWriter& writer,
                          const yal::frontend::QualType& qt,
                          const CType& type) {
        if (qt.isImmutable()) {
            writer.write("const ");
        }

        writer.write("% ",type.getCIdentifier());

        if (qt.isReference()) {
            writer.write("*");
        }
    }

    void
    CTypeGen::GenQualTypeFnParam(CodeWriter& writer,
                                 const yal::frontend::QualType& qt,
                                 const CType& type) {
        if (qt.isImmutable()) {
            writer.write("const ");
        }

        writer.write("% ",type.getCIdentifier());

        if (qt.isReference()) {
            writer.write("*");
        } else if (!type.getFrontendType().isTriviallyCopiable()) {
            writer.write("*");
        }
    }

    void
    CTypeGen::GenQualTypeReturn(CodeWriter &writer,
                                const yal::frontend::QualType& qt,
                                const CType& type) {
        // only add const if type is reference, as all return
        // types are copies
        if (qt.isImmutable() && qt.isReference()) {
            writer.write("const ");
        }

        writer.write("% ",type.getCIdentifier());

        if (qt.isReference()) {
            writer.write("*");
        }
    }

    void
    CTypeGen::GetBuilitinTypeInfo(yal::CodeWriter& writer) {
        //TODO: Replace with a special header at some point
        //writer.write("#include <yal/core/builtin.h>\n");

        GenBuiltinHeader(writer);
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


    void
    CTypeGen::GenBuiltinHeader(yal::CodeWriter& writer) {
       CTypeBuilitin::GenBuiltinHeader(writer);
    }

    void
    CTypeGen::GenBuiltinSource(yal::CodeWriter& writer) {
        CTypeBuilitin::GenBuiltinSource(writer);
    }

}
