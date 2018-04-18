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

namespace yal {
    class ErrorPrinter;
}

namespace yal::frontend {

    class Module;
    class Type;

    class ErrorDuplicateDecl final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorDuplicateDecl(const Module& module,
                           const Type& newType,
                           const Type& oldType);

        StringRef getErrorName() const final override;

        void printDetail(ErrorPrinter& printer) const final override;

        const SourceInfo& getSourceInfo() const final override;

    private:
        const SourceInfo& getSourceInfoFromType(const Type& type) const;
    private:
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

    private:
        const StringRef& m_typeName;
        const SourceInfo& m_srcInfo;
    };
}

