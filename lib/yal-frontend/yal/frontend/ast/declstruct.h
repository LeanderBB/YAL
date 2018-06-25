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

#include "yal/frontend/ast/declbase.h"
#include "yal/frontend/ast/declscope.h"

#include <optional>

namespace yal::frontend {

    class DeclVar;
    class TypeStruct;
    class DeclStruct : public DeclNamed {
    public:

        using Members = std::vector<DeclVar*, StdAllocatorWrapperStack<DeclVar*>>;

        DeclStruct(Module& module,
                   TypeStruct* type,
                   DeclScope &scope,
                   Members&& members);

        DeclStruct(Module& module,
                   TypeStruct* type,
                   DeclScope& scope);

        size_t calculateMemSizeWithAlignment(const size_t alignment,
                                             const size_t sizeofPtr) const;

        std::optional<const DeclVar*> getMemberByName(const StringRef name) const;

        virtual void acceptVisitor(AstVisitor& visitor) override;

        const Members& getMembers() const {
            return m_members;
        }

        void setMembers(Members&& members);

        Members& getMembers() {
            return m_members;
        }

        DeclScope& getStructDeclScope() {
            return m_structDeclScope;
        }

        const DeclScope& getStructDeclScope() const {
            return m_structDeclScope;
        }

        const TypeStruct& getStructType() const {
            return *m_structType;
        }

        TypeStruct& getStructType() {
            return *m_structType;
        }

    private:
        TypeStruct* m_structType;
        DeclScope m_structDeclScope;
        Members m_members;
    };

}
