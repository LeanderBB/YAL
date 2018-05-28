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

#include "yal/frontend/ast/declscope.h"
#include "yal/frontend/ast/declbase.h"
#include "yal/frontend/types/identifier.h"
#include "yal/frontend/ast/declfunction.h"
#include "yal/frontend/ast/declstruct.h"
#include "yal/frontend/ast/declmodule.h"
#include "yal/frontend/ast/decltypefunction.h"

namespace yal::frontend {

    DeclScope::DeclScope():
        m_parentScope(nullptr),
        m_declMap(),
        m_scopeDecl(nullptr),
        m_scopeKind(Kind::Scope) {

    }

    DeclScope::DeclScope(DeclFunction* decl,
                         DeclScope* parent):
        m_parentScope(parent),
        m_declMap(),
        m_scopeDecl(decl),
        m_scopeKind(Kind::Function){

    }

    DeclScope::DeclScope(DeclTypeFunction* decl,
                         DeclScope* parent):
        m_parentScope(parent),
        m_declMap(),
        m_scopeDecl(decl),
        m_scopeKind(Kind::TypeFunction) {

    }

    DeclScope::DeclScope(DeclStruct* decl,
                         DeclScope* parent):
        m_parentScope(parent),
        m_declMap(),
        m_scopeDecl(decl),
        m_scopeKind(Kind::Struct) {

    }

    DeclScope::DeclScope(StmtListScoped*,
                         DeclScope* parent):
        m_parentScope(parent),
        m_declMap(),
        m_scopeDecl(nullptr),
        m_scopeKind(Kind::StmtList) {

    }

    DeclScope::DeclScope(DeclModule* decl):
        m_parentScope(nullptr),
        m_declMap(),
        m_scopeDecl(decl),
        m_scopeKind(Kind::Module){

    }

    bool
    DeclScope::hasDecl(const DeclBase* decl,
                       const bool local) const {
        const Identifier& identifier = decl->getIdentifier();
        return hasDecl(identifier, local);
    }

    bool
    DeclScope::hasDecl(const Identifier& identifier,
                       const bool local) const {
        auto it = m_declMap.find(&identifier);
        if (it == m_declMap.end()) {
            return (m_parentScope != nullptr && !local)
                    ? m_parentScope->hasDecl(identifier, local)
                    : false;
        }
        return true;
    }

    DeclBase*
    DeclScope::getDecl(const Identifier& identifier,
                       const bool local) const {
        auto it = m_declMap.find(&identifier);
        if (it == m_declMap.end()) {
            return (m_parentScope != nullptr && !local)
                    ? m_parentScope->getDecl(identifier, local)
                    : nullptr;
        }
        return it->second;
    }

    void
    DeclScope::addDecl(DeclBase* decl) {
        YAL_ASSERT(!hasDecl(decl, false));
        const Identifier& identifier = decl->getIdentifier();
        m_declMap[&identifier] = decl;
    }

    void
    DeclScope::setParentScope(DeclScope *scope) {
        m_parentScope = scope;
    }

    const DeclScope*
    DeclScope::getFirstScopeOfTypeDecl() const {
        if (this->isScopePartOfDecl()) {
            return this;
        }
        return m_parentScope != nullptr
                ? m_parentScope->getFirstScopeOfTypeDecl()
                : nullptr;
    }

    bool
    DeclScope::isModuleScope() const {
        return m_scopeKind == Kind::Module;
    }

    bool
    DeclScope::isFunctionTypeScope() const {
        return isFunctionScope() || isTypeFunctionScope();
    }

    bool
    DeclScope::isFunctionScope() const {
        return m_scopeKind == Kind::Function;
    }

    bool
    DeclScope::isTypeFunctionScope() const {
        return m_scopeKind == Kind::TypeFunction;
    }

    bool
    DeclScope::isScopedScope() const {
        return m_scopeKind == Kind::Scope;
    }

    bool
    DeclScope::isStructScope() const {
        return m_scopeKind == Kind::Struct;
    }

    bool
    DeclScope::isStmtListScope() const {
        return m_scopeKind == Kind::StmtList;
    }
}
