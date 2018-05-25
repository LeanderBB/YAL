/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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
#include "yal/frontend/ast/exprstructvarref.h"
#include "yal/frontend/ast/exprvarref.h"
#include "yal/frontend/passes/passes.h"

namespace yal::frontend {

    // ErrorMoveUseAfterMove ------------------------------------------------

    const ErrorCode  ErrorMoveUseAfterMove::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Move), 1);

    ErrorMoveUseAfterMove::ErrorMoveUseAfterMove(const ExprVarRef& expr,
                                                 const Statement& stmtWhereMoved):
        Error(kCode),
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

    // ErrorMoveUseAfterMove ------------------------------------------------

    const ErrorCode  ErrorMoveStructVar::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Move), 2);

    ErrorMoveStructVar::ErrorMoveStructVar(const ExprStructVarRef& expr):
        Error(kCode),
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

}
