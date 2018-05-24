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

#include "yal/error/error.h"
#include "yal/io/sourcemanager.h"
namespace yal {
    class ErrorPrinter;
}

namespace yal::frontend {

    class Module;
    class DeclBase;
    class DeclVar;
    class STIdentifier;
    class STStmtAssign;
    class Type;
    class TypeFunction;
    class TypeStruct;

    class ErrorDuplicateTypeDecl final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorDuplicateTypeDecl(const Module& module,
                               const Type& newType,
                               const Type& oldType);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;

    private:
        const SourceInfo& getSourceInfoFromType(const Type& type) const;
    public:
        const Module& m_module;
        const Type& m_newType;
        const Type& m_oldType;
    };


    class ErrorUndefinedTypeRef final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorUndefinedTypeRef(const StringRef& typeName,
                              const SourceInfo& info);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;

    public:
        const StringRef m_typeName;
        const SourceInfo m_srcInfo;
    };

    class ErrorDuplicateSymbol final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorDuplicateSymbol(const StringRef& sym1,
                             const SourceInfo& sym1SrcInfo,
                             const StringRef& sym2,
                             const SourceInfo& sym2SrcInfo);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;

    public:
        const StringRef m_sym1;
        const SourceInfo m_sym1SrcInfo;
        const StringRef m_sym2;
        const SourceInfo m_sym2SrcInfo;
    };

    class ErrorUndefinedSymbol final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorUndefinedSymbol(const StringRef& sym,
                             const SourceInfo& symSrcInfo);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;

    public:
        const StringRef m_sym;
        const SourceInfo m_symSrcInfo;
    };

    class ErrorFnOnNonTargetType final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorFnOnNonTargetType(const StringRef& sym,
                               const SourceInfo& symSrcInfo,
                               const Type* type);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;
    public:
        const StringRef m_fnName;
        const SourceInfo m_fnSrcInfo;
        const Type* m_targetType;
    };


    class ErrorSymNotDeclVar final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorSymNotDeclVar(const StringRef& sym,
                           const SourceInfo& symSrcInfo,
                           const DeclBase& decl);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;
    public:
        const StringRef m_symName;
        const SourceInfo m_symSrcInfo;
        const DeclBase& m_decl;
    };


    class STExprIntegerLiteral;
    class ErrorInvalidIntLiteral final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorInvalidIntLiteral(const STExprIntegerLiteral& expr);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;
    public:
        const STExprIntegerLiteral& m_expr;
    };

    class STExprFloatLiteral;
    class ErrorInvalidFloatLiteral final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorInvalidFloatLiteral(const STExprFloatLiteral& expr);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;
    public:
        const STExprFloatLiteral& m_expr;
    };

    class ErrorTypeIsNotStruct final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorTypeIsNotStruct(const Type& expr,
                             const SourceInfo &srcInfo);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;
    public:
        const Type& m_type;
        const SourceInfo m_srcInfo;
    };

    class ErrorTypeIsNotFunction final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorTypeIsNotFunction(const Type& expr,
                               const SourceInfo& srcInfo);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;
    public:
        const Type& m_type;
        const SourceInfo m_srcInfo;
    };

    class ErrorTypeIsNotTypeFunction final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorTypeIsNotTypeFunction(const Type& expr,
                                   const SourceInfo& srcInfo);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;
    public:
        const Type& m_type;
        const SourceInfo m_srcInfo;
    };

    class ErrorTypeFunctionUndefined final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorTypeFunctionUndefined(const Type& type,
                                   const STIdentifier& name);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;
    public:
        const Type& m_type;
        const StringRef m_sym;
        const SourceInfo m_srcInfo;
    };

    class ErrorTypeFunctionIsNotStatic final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorTypeFunctionIsNotStatic(const TypeFunction& expr,
                                     const SourceInfo& srcInfo);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;
    public:
        const TypeFunction& m_type;
        const SourceInfo m_srcInfo;
    };

    class ErrorInvalidUseOfSelf final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorInvalidUseOfSelf(const TypeFunction& expr,
                              const SourceInfo& srcInfo);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;
    public:
        const TypeFunction& m_type;
        const SourceInfo m_srcInfo;
    };

    class ErrorInvalidLocationForSelf final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorInvalidLocationForSelf(const TypeFunction& expr,
                                    const SourceInfo& srcInfo);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;
    public:
        const TypeFunction& m_type;
        const SourceInfo m_srcInfo;
    };

    class ErrorUndefinedStructMember final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorUndefinedStructMember(const TypeStruct& type,
                                   const StringRef& sym,
                                   const SourceInfo& symSrcInfo);

        ErrorUndefinedStructMember(const TypeStruct& type,
                                   const STIdentifier& id);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;

    public:
        const TypeStruct& m_type;
        const StringRef m_sym;
        const SourceInfo m_symSrcInfo;
    };

    class ErrorAssignRefWithInvalidScope final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorAssignRefWithInvalidScope(const STStmtAssign& assign,
                                       const DeclVar& declLeft,
                                       const DeclVar& declRight);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;

    public:
        const STStmtAssign& m_stmt;
        const DeclVar& m_declLeft;
        const DeclVar& m_declRight;
    };

}

