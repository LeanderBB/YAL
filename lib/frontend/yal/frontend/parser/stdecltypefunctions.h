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
#include "yal/frontend/parser/stparser.h"

namespace yal::frontend {

    class STType;
    class STDeclFunction;

    class STDeclTypeFunctions final : public STDecl {
    public:
        using Decls = STVector<const STDeclFunction*>;

        STDeclTypeFunctions(const STType* type,
                            const ParseListDecl::Range range,
                            STParser& parser);

        const STType& getType() const {
            return *m_type;
        }

        const Decls& getDecls() const {
            return m_decls;
        }

    private:
        const STType* m_type;
        Decls m_decls;
    };

}
