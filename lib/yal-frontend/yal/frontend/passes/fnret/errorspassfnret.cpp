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

#include "yal/frontend/passes/fnret/errorspassfnret.h"
#include "yal/error/errorprinter.h"
#include "yal/frontend/ast/declfunction.h"
#include "yal/frontend/passes/passes.h"

namespace yal::frontend {

   const ErrorCode  ErrorFnNotAllStmtReturn::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::FnRet), 1);

    ErrorFnNotAllStmtReturn::ErrorFnNotAllStmtReturn(const DeclFunction& declFn):
        Error(kCode),
        m_decl(declFn) {

    }

    StringRef
    ErrorFnNotAllStmtReturn::getErrorName() const {
        return "Function Return Validation";
    }

    void
    ErrorFnNotAllStmtReturn::printDetail(ErrorPrinter &printer) const {
        FormatAppend(printer.getFormater(),
                     "Not all code paths in function '%' return a value.\n",
                     m_decl.getIdentifier());
    };


    const SourceInfo&
    ErrorFnNotAllStmtReturn::getSourceInfo() const {
        return m_decl.getSourceInfo();
    }
}
