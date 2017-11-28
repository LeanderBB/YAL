/*
 *  Copyright 2017 by Leander Beernaert (leanderbb@gmail.com)
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
#include "yal/ast/refbase.h"

namespace yal {

    //TODO : Specialize this for resolved and unresolved types
    // so that we can run a type resolution pass on the ast tree and
    // handle duplicates there followed by an actual type checker afterwards
    class RefType : public RefBase
    {
    public:
        RefType(Module& module,
                const AstType astType,
                const Type* type,
                const Qualifier qualifier);

        Qualifier getQualifier() const {
            return m_qualifier;
        }

    private:
        const Qualifier m_qualifier;
    };

}
