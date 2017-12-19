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

#include "yal/ast/declbase.h"
#include "yal/ast/nodecontainer.h"
#include "yal/ast/declscope.h"

namespace yal {

    class DeclVar;
    class DeclStructMembers :
            public NodeContainer<DeclVar*,AstType::DeclStructMembers> {
        using BaseType = NodeContainer<DeclVar*,AstType::DeclStructMembers>;
    public:

        DeclStructMembers(Module& module);

        DeclStructMembers(Module& module,
                          ContainerType&& params);

        void addDeclVar(DeclVar* decl);

        void acceptVisitor(AstVisitor& visitor);
    };

    inline AstType get_typeid(const DeclStructMembers& container) {
        return container.getAstType();
    }


    class DeclStruct : public DeclBase {
    public:

        DeclStruct(Module& module,
                   StringRef typeName,
                   DeclStructMembers* members);

        DeclStruct(Module& module,
                   StringRef typeName);


        DeclStructMembers* getMembers() const {
            return m_members;
        }

        void setMembers(DeclStructMembers* members);

        DeclScope* getDeclScope() {
            return &m_declScope;
        }

        const DeclScope* getDeclScope() const {
            return &m_declScope;
        }

        const DeclVar* getMemberByName(const StringRef name) const;

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        DeclStructMembers* m_members;
        DeclScope m_declScope;

    };

}
