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

#include "yal/frontend/types/type.h"

namespace yal::frontend {

    class STDeclStruct;
    class DeclStruct;
    class TypeStruct : public TypeTargetable
    {
    public:
        TypeStruct(const Module& module,
                   const STDeclStruct& decl);

        const STDeclStruct& getSTDecl() const {
            return m_stdecl;
        }

        void setDecl(DeclStruct& decl);

        DeclStruct& getDecl() {
            YAL_ASSERT(m_decl != nullptr);
            return *m_decl;
        }

        const DeclStruct& getDecl() const {
            YAL_ASSERT(m_decl != nullptr);
            return *m_decl;
        }

        virtual SourceInfoOpt getSourceInfo() const override;

    private:
        const STDeclStruct& m_stdecl;
        DeclStruct* m_decl;

    };
}
