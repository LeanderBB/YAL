/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/ast/declvar.h"

namespace yal::frontend {

    class DeclParamVar : public DeclVar {
    protected:
        DeclParamVar(Module& module,
                     const AstType astType,
                     StringRef name,
                     const SourceInfo& sourceInfo,
                     DeclScope &scope,
                     const QualType &type);
    public:
        DeclParamVar(Module& module,
                     StringRef name,
                     const SourceInfo& sourceInfo,
                     DeclScope &scope,
                     const QualType& type);
    };

    class DeclParamVarSelf : public DeclParamVar {
    public:
        DeclParamVarSelf(Module& module,
                         const SourceInfo& sourceInfo,
                         DeclScope& scope,
                         const QualType& refType);

    };
}

namespace yal {
    template <>
    inline yal::frontend::AstType
    get_typeid_instance<yal::frontend::DeclParamVar>(const yal::frontend::DeclParamVar& decl) {
        return decl.getAstType();
    }
}
