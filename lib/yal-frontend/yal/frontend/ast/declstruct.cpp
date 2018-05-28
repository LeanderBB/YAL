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

#include "yal/frontend/ast/declstruct.h"
#include "yal/frontend/ast/astvisitor.h"
#include "yal/frontend/ast/declvar.h"
#include "yal/frontend/module.h"
#include "yal/frontend/parser/stdeclstruct.h"
#include "yal/frontend/types/typestruct.h"

namespace yal::frontend {

    DeclStruct::DeclStruct(Module& module,
                           TypeStruct* type,
                           DeclScope& scope,
                           Members&& members):
        DeclBase(module,
                 AstType::DeclStruct,
                 type->getIdentifier(),
                 type->getSTDecl().getSourceInfo(),
                 scope),
        m_structType(type),
        m_structDeclScope(this, &scope),
        m_members(std::move(members)) {

        for (auto& member : members) {
            member->setScopeWhereDeclared(&m_structDeclScope);
        }
    }

    DeclStruct::DeclStruct(Module& module,
                           TypeStruct* type,
                           DeclScope& scope):
        DeclBase(module,
                 AstType::DeclStruct,
                 type->getIdentifier(),
                 type->getSTDecl().getSourceInfo(),
                 scope),
        m_structType(type),
        m_structDeclScope(this, &scope),
        m_members(Members::allocator_type(module.getASTContext().getAllocator())) {

    }

    std::optional<const DeclVar *>
    DeclStruct::getMemberByName(const StringRef name) const {
        for(auto& decl : m_members) {
            if (decl->getName() == name) {
                return std::optional<const DeclVar *>(decl);
            }
        }

        return std::optional<const DeclVar *>();
    }

    size_t
    DeclStruct::calculateMemSizeWithAlignment(const size_t alignment,
                                              const size_t sizeofPtr) const {
        size_t size = 0;
        for (auto& member : m_members) {

            const QualType qt = member->getQualType();
            const Qualifier qual = qt.getQualifier();
            const Type* type = qt.getType();
            size_t memberSize = 0;
            if (qual.isPointer()) {
                YAL_ASSERT_NOT_IMPLEMENTED();
            }

            if (qual.isReference()) {
                memberSize = sizeofPtr;
            } else {
                const size_t typeSize = type->getSizeBytes();
                YAL_ASSERT(typeSize != 0);
                memberSize = typeSize;
            }

            if (memberSize < alignment
                    && (memberSize % alignment) + memberSize <= alignment) {
                size += memberSize;
            } else {
                // align size
                size += memberSize;
                size = Type::AlignTo(size, alignment);
            }
        }
        YAL_ASSERT(size != 0);
        return  size = Type::AlignTo(size, sizeofPtr);
    }

    void
    DeclStruct::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

    void
    DeclStruct::setMembers(Members&& members) {
        m_members = std::move(members);
    }
}
