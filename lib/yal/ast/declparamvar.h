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

#include "yal/ast/declvar.h"
#include "yal/ast/nodecontainer.h"

namespace yal {

    class DeclParamVar : public DeclVar{
    protected:
        DeclParamVar(Module& module,
                     const AstType astType,
                     StringRef name,
                     const Qualifier qualifier,
                     RefType* varType);
    public:
        DeclParamVar(Module& module,
                     StringRef name,
                     const Qualifier qualifier,
                     RefType* varType);

        virtual void acceptVisitor(AstVisitor& visitor) override;
    };

    class DeclParamVarSelf : public DeclParamVar {
    public:
        DeclParamVarSelf(Module& module,
                         RefType* refType);

        virtual void acceptVisitor(AstVisitor& visitor) override;
    };

    class DeclParamVarContainer :
            public NodeContainer<DeclParamVar*,AstType::DeclParamVarContainer, true> {
        using BaseType = NodeContainer<DeclParamVar*,AstType::DeclParamVarContainer, true>;
    public:

        DeclParamVarContainer(Module& module);

        DeclParamVarContainer(Module& module,
                              ContainerType&& params);

        void addDeclParam(DeclParamVar* decl);

        void acceptVisitor(AstVisitor& visitor);

    };

    inline AstType get_typeid(const DeclParamVarContainer& container) {
        return container.getAstType();
    }
}
