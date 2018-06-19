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

#include "yal/frontend/passes/type/errorspasstype.h"
#include "yal/error/errorprinter.h"
#include "yal/frontend/ast/astnodes.h"
#include "yal/frontend/passes/passes.h"
#include "yal/frontend/types/qualtype.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/types/typestruct.h"

namespace yal::frontend {

    ErrorTypeBase::ErrorTypeBase(const ErrorCode code,
                                 const QualType& typeFrom,
                                 const QualType& typeTo,
                                 const SourceInfo& loc):
        ErrorFrontend(code),
        m_typeFrom(typeFrom),
        m_typeTo(typeTo),
        m_srcInfo(loc) {

    }

    const SourceInfo&
    ErrorTypeBase::getSourceInfo() const {
        return m_srcInfo;
    }

    // ErrorTypeIncompatible--------------------------------------------------

    const ErrorCode  ErrorTypeIncompatible::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 1);

    ErrorTypeIncompatible::ErrorTypeIncompatible(const QualType& typeFrom,
                                                 const QualType& typeTo,
                                                 const SourceInfo& loc):
        ErrorTypeBase(kCode, typeFrom, typeTo, loc) {
    }

    StringRef
    ErrorTypeIncompatible::getErrorName() const {
        return "Invalid Type Conversion";
    }

    void
    ErrorTypeIncompatible::printDetail(ErrorPrinter &printer) const {
        FormatAppend(printer.getFormater(),
                     "Can't convert from type '%' to type '%'\n",
                     m_typeFrom, m_typeTo);
        FormatAppend(printer.getFormater(),
                     "Note: Type might be convertible when using cast operator 'as',\n");
    };

    // ErrorTypeMutability ----------------------------------------------------

    const ErrorCode  ErrorTypeMutability::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 2);

    ErrorTypeMutability::ErrorTypeMutability(const QualType& typeFrom,
                                             const QualType& typeTo,
                                             const SourceInfo& loc):
        ErrorTypeBase(kCode, typeFrom, typeTo, loc) {
    }

    StringRef
    ErrorTypeMutability::getErrorName() const {
        return "Invalid Mutablity Conversion";
    }

    void
    ErrorTypeMutability::printDetail(ErrorPrinter &printer) const {
        FormatAppend(printer.getFormater(),
                     "Can't convert from immutable '%' to mutable '%'\n",
                     m_typeFrom, m_typeTo);
        if (!m_typeTo.isTriviallyCopiable() || !m_typeFrom.isTriviallyCopiable()) {
            FormatAppend(printer.getFormater(),
                         "Note: Destination or source type is not trivially "
                         "copiable, you can use the copy operator to "
                         "create a copy of the type instead.");
        }
    };

    // ErrorTypeReference ----------------------------------------------------

    const ErrorCode  ErrorTypeReference::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 3);

    ErrorTypeReference::ErrorTypeReference(const QualType& typeFrom,
                                           const QualType& typeTo,
                                           const SourceInfo& loc):
        ErrorTypeBase(kCode, typeFrom, typeTo, loc) {
    }

    StringRef
    ErrorTypeReference::getErrorName() const {
        return "Invalid Reference Conversion";
    }

    void
    ErrorTypeReference::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();

        if (!m_typeFrom.isReference() && m_typeTo.isReference()) {
            FormatAppend(formater,
                         "Can't convert from non reference '%' to reference '%'"
                         ", use the '&' operator\n", m_typeFrom, m_typeTo);
        } else {
            FormatAppend(formater,
                         "Cannot convert from reference '%' to non reference '%'.\n",
                         m_typeFrom, m_typeTo);
        }
    };

    // ErrorTypeExprUnassignable ----------------------------------------------

    const ErrorCode  ErrorTypeExprUnassignable::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 4);

    ErrorTypeExprUnassignable::ErrorTypeExprUnassignable(const StmtExpression& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
    }

    StringRef
    ErrorTypeExprUnassignable::getErrorName() const {
        return "Non Assignable Expression";
    }

    void
    ErrorTypeExprUnassignable::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Result of expression above is not assignable.\n"
                     "Expression results in type: %, which is not an L-value\n",
                     m_expr.getQualType());
    };

    const SourceInfo&
    ErrorTypeExprUnassignable::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorTypeUnaryOpBitNot ----------------------------------------------

    const ErrorCode  ErrorTypeBitOpNonUnsigned::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 5);

    ErrorTypeBitOpNonUnsigned::ErrorTypeBitOpNonUnsigned(const StmtExpression& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
    }

    StringRef
    ErrorTypeBitOpNonUnsigned::getErrorName() const {
        return "Invalid use of bit operator";
    }

    void
    ErrorTypeBitOpNonUnsigned::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Bit operatoros only operate on unsigned integer types.\n");
    };

    const SourceInfo&
    ErrorTypeBitOpNonUnsigned::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorTypeUnaryOpNegate ----------------------------------------------

    const ErrorCode  ErrorTypeUnaryOpNegate::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 6);

    ErrorTypeUnaryOpNegate::ErrorTypeUnaryOpNegate(const ExprUnaryOperator& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
    }

    StringRef
    ErrorTypeUnaryOpNegate::getErrorName() const {
        return "Invalid use of negation operator";
    }

    void
    ErrorTypeUnaryOpNegate::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Operator negate (-) only operates on signed integer types.\n"
                     "Expression is of type '%'\n",
                     m_expr.getQualType());
    };

    const SourceInfo&
    ErrorTypeUnaryOpNegate::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorTypeUnaryOpNot ----------------------------------------------

    const ErrorCode  ErrorTypeBoolOpNonBool::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 7);

    ErrorTypeBoolOpNonBool::ErrorTypeBoolOpNonBool(const StmtExpression& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
    }

    StringRef
    ErrorTypeBoolOpNonBool::getErrorName() const {
        return "Invalid use of boolean operator";
    }

    void
    ErrorTypeBoolOpNonBool::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Bool operators only operates on boolean types.\n");
    };

    const SourceInfo&
    ErrorTypeBoolOpNonBool::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorTypeUnaryOpRefDoubleRef ----------------------------------------------

    const ErrorCode  ErrorTypeUnaryOpRefDoubleRef::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 8);

    ErrorTypeUnaryOpRefDoubleRef::ErrorTypeUnaryOpRefDoubleRef(const ExprUnaryOperator& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
    }

    StringRef
    ErrorTypeUnaryOpRefDoubleRef::getErrorName() const {
        return "Double Reference";
    }

    void
    ErrorTypeUnaryOpRefDoubleRef::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Can't create a reference to another reference.\n"
                     "Expression is of type '%'\n",
                     m_expr.getQualType());
    };

    const SourceInfo&
    ErrorTypeUnaryOpRefDoubleRef::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorTypeUnaryOpRefNonLValue ----------------------------------------------

    const ErrorCode  ErrorTypeUnaryOpRefNonLValue::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 9);

    ErrorTypeUnaryOpRefNonLValue::ErrorTypeUnaryOpRefNonLValue(const ExprUnaryOperator& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
    }

    StringRef
    ErrorTypeUnaryOpRefNonLValue::getErrorName() const {
        return "Reference of non L-Value type";
    }

    void
    ErrorTypeUnaryOpRefNonLValue::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Can't create a reference to a non LValue type.\n"
                     "Result of expression is already a reference: '%'\n",
                     m_expr.getQualType());
    };

    const SourceInfo&
    ErrorTypeUnaryOpRefNonLValue::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorTypeUnaryOpRefNonLValue ----------------------------------------------

    const ErrorCode ErrorTypeMissingTrait::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 10);

    ErrorTypeMissingTrait::ErrorTypeMissingTrait(const Type& type,
                                                 const SourceInfo& srcInfo):
        ErrorFrontend(kCode),
        m_type(type),
        m_srcInfo(srcInfo) {
    }

    StringRef
    ErrorTypeMissingTrait::getErrorName() const {
        return "Missing Trait Impl";
    }

    void
    ErrorTypeMissingTrait::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Can't use type '%' in context as it doesn't implement Trait [TraitName]\n",
                     m_type);
    };

    const SourceInfo&
    ErrorTypeMissingTrait::getSourceInfo() const {
        return m_srcInfo;
    }

    // ErrorTypeStructVarRefNonStruct ---------------------------------------

    const ErrorCode  ErrorTypeStructVarRefNonStruct::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 11);

    ErrorTypeStructVarRefNonStruct::ErrorTypeStructVarRefNonStruct(const ExprStructVarRef& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
    }

    StringRef
    ErrorTypeStructVarRefNonStruct::getErrorName() const {
        return "Invalid Struct Var Ref";
    }

    void
    ErrorTypeStructVarRefNonStruct::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Attempting to access struct member on type '%' which is "
                     "not of a struct type\n",
                     m_expr.getExpression()->getQualType());
    };

    const SourceInfo&
    ErrorTypeStructVarRefNonStruct::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorTypeStructVarRefUndefinedMember ----------------------------------

    const ErrorCode  ErrorTypeStructVarRefUndefinedMember::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 12);

    ErrorTypeStructVarRefUndefinedMember::ErrorTypeStructVarRefUndefinedMember(const ExprStructVarRef& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
    }

    StringRef
    ErrorTypeStructVarRefUndefinedMember::getErrorName() const {
        return "Undefned Struct Member Ref";
    }

    void
    ErrorTypeStructVarRefUndefinedMember::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Attempting to access struct member named '%' which doesn't "
                     "exist on struct '%'\n",
                     m_expr.getMemberName(),
                     m_expr.getExpression()->getQualType().getType());
    };

    const SourceInfo&
    ErrorTypeStructVarRefUndefinedMember::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorTypeStructVarRefUndefinedMember ----------------------------------

    const ErrorCode  ErrorTypeFnCallInvalidArgCount::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 13);

    ErrorTypeFnCallInvalidArgCount::ErrorTypeFnCallInvalidArgCount(const ExprFnCall& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
        flagAsFatal();
    }

    StringRef
    ErrorTypeFnCallInvalidArgCount::getErrorName() const {
        return "Invalid function arguments count";
    }

    void
    ErrorTypeFnCallInvalidArgCount::printDetail(ErrorPrinter &printer) const {

        const TypeFunction* typeFn = m_expr.getFunctionType();
        const DeclFunction& declFn = typeFn->getDecl();
        const ExprList& args = m_expr.getFunctionArgs();
        const DeclFunction::Params& params = declFn.getParams();

        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Function '%' requires % argument(s), but you have provided "
                     "% argument(s).",
                     declFn.getIdentifier(), params.size(), args.size());
    }

    const SourceInfo&
    ErrorTypeFnCallInvalidArgCount::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorTypeStructVarRefUndefinedMember ----------------------------------

    const ErrorCode  ErrorTypeStructInitNumMembers::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 14);

    ErrorTypeStructInitNumMembers::ErrorTypeStructInitNumMembers(const ExprStructInit& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
    }

    StringRef
    ErrorTypeStructInitNumMembers::getErrorName() const {
        return "Invalid Struct Init Member Count";
    }

    void
    ErrorTypeStructInitNumMembers::printDetail(ErrorPrinter &printer) const {
        const QualType qt = m_expr.getQualType();
        const TypeStruct* typeStruct = dyn_cast<TypeStruct>(qt.getType());
        YAL_ASSERT(typeStruct != nullptr);
        const DeclStruct& decl = typeStruct->getDecl();

        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Not all memeber of struct '%' are initialized in expression. "
                     "Only % of % argument(s) were provided.\n",
                     decl.getIdentifier(),
                     m_expr.getMemberInitExprList().size(),
                     decl.getMembers().size());
    }

    const SourceInfo&
    ErrorTypeStructInitNumMembers::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorTypeAssignToImmutable -------------------------------------------

    const ErrorCode  ErrorTypeAssignToImmutable::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 15);

    ErrorTypeAssignToImmutable::ErrorTypeAssignToImmutable(const StmtAssign& stmt):
        ErrorFrontend(kCode),
        m_stmt(stmt) {
    }

    StringRef
    ErrorTypeAssignToImmutable::getErrorName() const {
        return "Assign to Immutable";
    }

    void
    ErrorTypeAssignToImmutable::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Can't assign a new value to imutable immutable expression\n");
    }

    const SourceInfo&
    ErrorTypeAssignToImmutable::getSourceInfo() const {
        return m_stmt.getSourceInfo();
    }

    // ErrorTypeTypeFnCallImmutable -------------------------------------------

    const ErrorCode  ErrorTypeTypeFnCallImmutable::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 16);

    ErrorTypeTypeFnCallImmutable::ErrorTypeTypeFnCallImmutable(const ExprTypeFnCall& expr,
                                                               const DeclTypeFunction& decl):
        ErrorFrontend(kCode),
        m_expr(expr),
        m_decl(decl) {
    }

    StringRef
    ErrorTypeTypeFnCallImmutable::getErrorName() const {
        return "Invalid Type Function Call Mutability";
    }

    void
    ErrorTypeTypeFnCallImmutable::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        const StmtExpression* fnExpr = m_expr.getExpression().value_or(nullptr);
        YAL_ASSERT(fnExpr != nullptr);

        FormatAppend(formater,
                     "Can't call mutable type function '%' on immutable type '%'.\n",
                     m_decl.getIdentifier().getName(),
                     *fnExpr->getQualType().getType());
        const SourceInfo& srcInfo = m_decl.getSourceInfo();
        SourceItemOpt item = printer.getSourceManager().getItem(srcInfo.handle);
        if (item.has_value()) {
            FormatAppend(formater,
                         "Function '%' defined here:\n", m_decl.getIdentifier());
            printer.printSourceInfo(printer.getFormater(), *item.value(), srcInfo);
        }
    }

    const SourceInfo&
    ErrorTypeTypeFnCallImmutable::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorTypeTypeFnCallImmutable -------------------------------------------

    const ErrorCode  ErrorTypeIncompatibleCast::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 17);

    ErrorTypeIncompatibleCast::ErrorTypeIncompatibleCast(const ExprCast& expr):
        ErrorTypeBase(kCode,
                      expr.getExpression().getQualType(),
                      expr.getQualType(),
                      expr.getSourceInfo()) {
    }

    StringRef
    ErrorTypeIncompatibleCast::getErrorName() const {
        return "Invalid Type Cast";
    }

    void
    ErrorTypeIncompatibleCast::printDetail(ErrorPrinter &printer) const {
        FormatAppend(printer.getFormater(),
                     "Can't cast from type '%' to type '%'\n",
                     m_typeFrom.getType(), m_typeTo.getType());
    }

    // ErrorTypeCastReference -------------------------------------------

    const ErrorCode  ErrorTypeCastReference::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Type), 18);

    ErrorTypeCastReference::ErrorTypeCastReference(const ExprCast& expr):
        ErrorTypeBase(kCode,
                      expr.getExpression().getQualType(),
                      expr.getQualType(),
                      expr.getSourceInfo()) {
    }

    StringRef
    ErrorTypeCastReference::getErrorName() const {
        return "Invalid Reference Cast";
    }

    void
    ErrorTypeCastReference::printDetail(ErrorPrinter &printer) const {
        FormatAppend(printer.getFormater(),
                     "Can only cast non-trivially copiable types as reference.\n"
                     "Attempting to cast from '%' to '%'\n",
                     m_typeFrom, m_typeTo);
    }
}
