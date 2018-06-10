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

#pragma once

#include "yal/frontend/errorfrontend.h"
#include "yal/frontend/types/qualtype.h"
#include "yal/io/sourcemanager.h"

namespace yal {
    class ErrorPrinter;
}


namespace yal::frontend {

    class ExprFnCall;
    class ExprStructVarRef;
    class ExprVarRef;
    class Statement;

    class ErrorMoveUseAfterMove final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorMoveUseAfterMove(const ExprVarRef& expr,
                              const Statement& stmtWhereMoved);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprVarRef& m_expr;
        const Statement& m_stmtWhereMoved;
    };

    class ErrorMoveStructVar final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorMoveStructVar(const ExprStructVarRef& expr);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprStructVarRef& m_expr;
    };

    class ErrorMoveFnCallRValue final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorMoveFnCallRValue(const ExprFnCall& expr,
                              const uint32_t argIndex);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprFnCall& m_expr;
        const uint32_t m_argIndex;
    };
}
