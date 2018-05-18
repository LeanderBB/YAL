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

#pragma once

#include "yal/frontend/types/type.h"

#include <string>

namespace yal::frontend {

    class STDeclFunction;
    class DeclFunction;

    class TypeFunction : public Type
    {
    public:

        static Identifier CreateIdentitier(const Module& module,
                                           const STDeclFunction* decl);

        TypeFunction(const Module& module,
                     const STDeclFunction* decl);

        const STDeclFunction* getSTDecl() const {
            return m_stdecl;
        }

        DeclFunction* getDecl() {
            return m_decl;
        }

        const DeclFunction* getDecl() const {
            return m_decl;
        }

        void setDecl(DeclFunction* decl);

        virtual SourceInfoOpt getSourceInfo() const override;

    private:
        const STDeclFunction* m_stdecl;
        DeclFunction* m_decl;
    };
}
