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

    class STDeclAlias;
    class DeclAliasWeak;

    class TypeAliasWeak final : public Type {
    public:
        TypeAliasWeak(const Module& module,
                      const STDeclAlias& decl,
                      Type& aliasedType);

        void setDeclAlias(DeclAliasWeak& declAlias);

        const STDeclAlias& getSTDecl() const {
            return m_stDecl;
        }

        const DeclAliasWeak& getDecl() const {
            YAL_ASSERT(m_decl != nullptr);
            return *m_decl;
        }

        const Type& getAliasedType() const {
            return m_aliasedType;
        }

        Type& getAliasedType() {
            return m_aliasedType;
        }

        virtual SourceInfoOpt getSourceInfo() const override final;

    protected:
        bool isCastableToAutoImpl(const Type&) const override final;

        bool isCastableToRequestImpl(const Type&) const override final;

        const TypeFunction* getFunctionWithIdImpl(const Identifier&) const override final;

        const TypeFunction* getFunctionWithNameImpl(const StringRef) const override final;

        const Type& resolve() const override final;
    private:
        Type& m_aliasedType;
        const STDeclAlias& m_stDecl;
        DeclAliasWeak* m_decl;
    };

}
