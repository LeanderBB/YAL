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
            public NodeContainer<DeclVar*,AstType::DeclStructMembers, false> {
        using BaseType = NodeContainer<DeclVar*,AstType::DeclStructMembers, false>;
    public:

        DeclStructMembers(frontend::Module& module);

        DeclStructMembers(frontend::Module& module,
                          ContainerType&& params);

        void addDeclVar(DeclVar* decl);

        void acceptVisitor(AstVisitor& visitor);
    };

    inline AstType get_typeid(const DeclStructMembers& container) {
        return container.getAstType();
    }


    class DeclStruct : public DeclBase, public DeclTyped {
    public:

        DeclStruct(frontend::Module& module,
                   StringRef typeName,
                   const DeclScope* scope,
                   DeclStructMembers* members);

        DeclStruct(frontend::Module& module,
                   StringRef typeName,
                   const DeclScope *scope);


        DeclStructMembers* getMembers() const {
            return m_members;
        }

        void setMembers(DeclStructMembers* members);

        DeclScope* getStructDeclScope() {
            return &m_structDeclScope;
        }

        const DeclScope* getStructDeclScope() const {
            return &m_structDeclScope;
        }

        size_t calculateMemSizeWithAlignment(const size_t alignment,
                                             const size_t sizeofPtr) const;

        const DeclVar* getMemberByName(const StringRef name) const;

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        DeclStructMembers* m_members;
        DeclScope m_structDeclScope;

    };

}
