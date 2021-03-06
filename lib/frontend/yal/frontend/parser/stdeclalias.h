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

#include "yal/frontend/parser/syntaxtree.h"

namespace yal::frontend {

    class STDeclAlias : public STDeclNamed {
    public:

        STDeclAlias(const STIdentifier* name,
                    const STType* aliased,
                    const bool isStrong);

        const STType& getAliasedType() const {
            return *m_aliasedType;
        }

        bool isStrong() const {
            return m_strong;
        }

    private:
        const STType* m_aliasedType;
        const bool m_strong;
    };


}
