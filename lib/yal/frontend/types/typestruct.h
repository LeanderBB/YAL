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

namespace yal::frontend {

    class STDeclStruct;
    class DeclStruct;
    class TypeStruct : public Type
    {
    public:
        TypeStruct(const Module& module,
                   const STDeclStruct* decl);

        const STDeclStruct* getSTDecl() const {
            return m_stdecl;
        }

        void setDecl(DeclStruct* decl);

        DeclStruct* getDecl() {
            return m_decl;
        }

    private:
        const STDeclStruct* m_stdecl;
        DeclStruct* m_decl;

    };
}
