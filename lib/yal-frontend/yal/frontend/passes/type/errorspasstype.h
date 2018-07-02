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

    class DeclTypeFunction;
    class ExprCast;
    class ExprFnCall;
    class ExprStructVarRef;
    class ExprStructInit;
    class ExprTypeFnCall;
    class ExprUnaryOperator;
    class StmtExpression;
    class StmtAssign;
    class Type;

    class ErrorTypeBase : public ErrorFrontend {
    public:
        ErrorTypeBase(const ErrorCode code,
                      const QualType& typeFrom,
                      const QualType& typeTo,
                      const SourceInfo& loc);

    protected:
        const SourceInfo& getSourceInfo() const override final;

    protected:
        const QualType m_typeFrom;
        const QualType m_typeTo;
        const SourceInfo m_srcInfo;
    };

    class ErrorTypeIncompatible final : public ErrorTypeBase {
    public:
        static const ErrorCode kCode;

        ErrorTypeIncompatible(const QualType& typeFrom,
                              const QualType& typeTo,
                              const SourceInfo& loc);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

    };


    class ErrorTypeMutability final : public ErrorTypeBase {
    public:
        static const ErrorCode kCode;

        ErrorTypeMutability(const QualType& typeFrom,
                            const QualType& typeTo,
                            const SourceInfo& loc);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;
    };


    class ErrorTypeReference final : public ErrorTypeBase {
    public:
        static const ErrorCode kCode;

        ErrorTypeReference(const QualType& typeFrom,
                           const QualType& typeTo,
                           const SourceInfo& loc);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;
    };

    class ErrorTypeExprUnassignable final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeExprUnassignable(const StmtExpression& expr);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const StmtExpression& m_expr;
    };

    class ErrorTypeBitOpNonUnsigned final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeBitOpNonUnsigned(const StmtExpression& expr);

        StringRef getErrorName() const override final;


    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const StmtExpression& m_expr;
    };

    class ErrorTypeUnaryOpNegate final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeUnaryOpNegate(const ExprUnaryOperator& expr);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprUnaryOperator& m_expr;
    };

    class ErrorTypeBoolOpNonBool final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeBoolOpNonBool(const StmtExpression& expr);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const StmtExpression& m_expr;
    };

    class ErrorTypeUnaryOpRefDoubleRef final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeUnaryOpRefDoubleRef(const ExprUnaryOperator& expr);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprUnaryOperator& m_expr;
    };

    class ErrorTypeUnaryOpRefNonLValue final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeUnaryOpRefNonLValue(const ExprUnaryOperator& expr);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprUnaryOperator& m_expr;
    };

    class ErrorTypeMissingTrait final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeMissingTrait(const Type& type,
                              const SourceInfo& srcInfo);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const Type& m_type;
        const SourceInfo m_srcInfo;
    };

    class ErrorTypeStructVarRefNonStruct final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeStructVarRefNonStruct(const ExprStructVarRef& expr);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprStructVarRef& m_expr;
    };

    class ErrorTypeStructVarRefUndefinedMember final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeStructVarRefUndefinedMember(const ExprStructVarRef& expr);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprStructVarRef& m_expr;
    };

    class ErrorTypeFnCallInvalidArgCount final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeFnCallInvalidArgCount(const ExprFnCall& node);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprFnCall& m_expr;
    };

    class ErrorTypeStructInitNumMembers final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeStructInitNumMembers(const ExprStructInit& node);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprStructInit& m_expr;
    };

    class ErrorTypeAssignToImmutable final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeAssignToImmutable(const StmtAssign& node);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const StmtAssign& m_stmt;
    };

    class ErrorTypeTypeFnCallImmutable final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeTypeFnCallImmutable(const ExprTypeFnCall& expr,
                                     const DeclTypeFunction& decl);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprTypeFnCall& m_expr;
        const DeclTypeFunction& m_decl;
    };

    class ErrorTypeIncompatibleCast final : public ErrorTypeBase {
    public:
        static const ErrorCode kCode;

        ErrorTypeIncompatibleCast(const ExprCast& expr);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;
    };

    class ErrorTypeCastReference final : public ErrorTypeBase {
    public:
        static const ErrorCode kCode;

        ErrorTypeCastReference(const ExprCast& expr);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;
    };

    class ErrorTypeUnaryOpDerefNonRef final : public ErrorFrontend {
    public:
        static const ErrorCode kCode;

        ErrorTypeUnaryOpDerefNonRef(const ExprUnaryOperator& expr);

        StringRef getErrorName() const override final;

    protected:
        void printDetail(ErrorPrinter& printer)const override final;

        const SourceInfo& getSourceInfo() const override final;

    private:
        const ExprUnaryOperator& m_expr;
    };
}
