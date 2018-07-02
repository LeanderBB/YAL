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

#include "yal/frontend/passes/move/errorspassmove.h"
#include "yal/error/errorprinter.h"
#include "yal/frontend/ast/declvar.h"
#include "yal/frontend/ast/exprfncall.h"
#include "yal/frontend/ast/exprstructvarref.h"
#include "yal/frontend/ast/exprunaryoperator.h"
#include "yal/frontend/ast/exprvarref.h"
#include "yal/frontend/passes/passes.h"

namespace yal::frontend {

    // ErrorMoveUseAfterMove ------------------------------------------------

    const ErrorCode  ErrorMoveUseAfterMove::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Move), 1);

    ErrorMoveUseAfterMove::ErrorMoveUseAfterMove(const ExprVarRef& expr,
                                                 const Statement& stmtWhereMoved):
        ErrorFrontend(kCode),
        m_expr(expr),
        m_stmtWhereMoved(stmtWhereMoved) {
        flagAsFatal();
    }

    StringRef
    ErrorMoveUseAfterMove::getErrorName() const {
        return "Use After Move";
    }

    void
    ErrorMoveUseAfterMove::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        const StringRef varName = m_expr.getDeclVar().getName();
        FormatAppend(formater,
                     "Attempting to use moved '%' in the expression above.\n",
                     varName);

        const SourceInfo& stmtSrcInfo = m_stmtWhereMoved.getSourceInfo();
        SourceItemOpt item = printer.getSourceManager().getItem(stmtSrcInfo.handle);
        if (item.has_value()) {
            FormatAppend(formater,
                         "Variable '%' was moved here:\n",
                         varName);
            printer.printSourceInfo(printer.getFormater(),*item.value(), stmtSrcInfo);
        }
    };

    const SourceInfo&
    ErrorMoveUseAfterMove::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorMoveStructVar ------------------------------------------------

    const ErrorCode  ErrorMoveStructVar::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Move), 2);

    ErrorMoveStructVar::ErrorMoveStructVar(const ExprStructVarRef& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
        flagAsFatal();
    }

    StringRef
    ErrorMoveStructVar::getErrorName() const {
        return "Struct Member Move";
    }

    void
    ErrorMoveStructVar::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Attempting to move '%' out of struct without replacing it.\n"
                     "Members from structs can't be moved without a replacement value.\n",
                     m_expr.getMemberName());
    };

    const SourceInfo&
    ErrorMoveStructVar::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }

    // ErrorMoveFnCallRValue ------------------------------------------------

    const ErrorCode  ErrorMoveFnCallRValue::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Move), 3);

    ErrorMoveFnCallRValue::ErrorMoveFnCallRValue(const ExprFnCall& expr,
                                                 const uint32_t argIndex):
        ErrorFrontend(kCode),
        m_expr(expr),
        m_argIndex(argIndex){
        flagAsFatal();
    }

    StringRef
    ErrorMoveFnCallRValue::getErrorName() const {
        return "Move expression with a RValue.";
    }

    void
    ErrorMoveFnCallRValue::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Argument '%' for function call expects a movable type, "
                     "yet the expression produces an RValue which can't be moved.\n",
                     m_argIndex);
    };

    const SourceInfo&
    ErrorMoveFnCallRValue::getSourceInfo() const {
        auto& args = m_expr.getFunctionArgs();
        YAL_ASSERT(m_argIndex < args.size());
        return args[m_argIndex]->getSourceInfo();
    }

    // ErrorMoveStructVar ------------------------------------------------

    const ErrorCode  ErrorMoveDereference::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Move), 4);

    ErrorMoveDereference::ErrorMoveDereference(const ExprUnaryOperator& expr):
        ErrorFrontend(kCode),
        m_expr(expr) {
        flagAsFatal();
    }

    StringRef
    ErrorMoveDereference::getErrorName() const {
        return "Move of derefenced value";
    }

    void
    ErrorMoveDereference::printDetail(ErrorPrinter &printer) const {
        auto& formater = printer.getFormater();
        FormatAppend(formater,
                     "Attempting to move value by dereferencing a reference."
                     "References can only be updated, but never moved.\n"
                     "Expression is of type '%' and is not trivially copiable.\n",
                     m_expr.getExpression()->getQualType());
    };

    const SourceInfo&
    ErrorMoveDereference::getSourceInfo() const {
        return m_expr.getSourceInfo();
    }
}
