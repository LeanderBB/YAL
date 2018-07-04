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

#include "yal/frontend/ast/declbase.h"

namespace yal::frontend {

    class DeclAliasWeak final : public DeclNamed {
    public:
        DeclAliasWeak(Module& module,
                      DeclScope& scope,
                      TypeAliasWeak& aliasType);


        const TypeAliasWeak& getAliasType() const {
            return m_aliasType;
        }

    private:
        TypeAliasWeak& m_aliasType;
    };


    class DeclAliasStrong final : public DeclNamed {
    public:
        DeclAliasStrong(Module& module,
                        DeclScope& scope,
                        TypeAliasStrong& aliasType);

        const TypeAliasStrong& getAliasType() const {
            return m_aliasType;
        }

    private:
        TypeAliasStrong& m_aliasType;
    };

}
