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

#include "yal/ast/declscope.h"
#include "yal/ast/declbase.h"
#include "yal/ast/identifier.h"
namespace yal {

    DeclScope::DeclScope(const Kind kind):
        m_parentScope(nullptr),
        m_declMap(),
        m_scopeKind(kind){

    }

    bool
    DeclScope::hasDecl(const DeclBase* decl) const {
        const Identifier& identifier = decl->getIdentifier();
        auto it = m_declMap.find(identifier.getAsString());
        if (it == m_declMap.end()) {
            return (m_parentScope != nullptr)
                    ? m_parentScope->hasDecl(decl)
                    : false;
        }
        return true;
    }

    void
    DeclScope::addDecl(DeclBase* decl) {
        YAL_ASSERT(!hasDecl(decl));
        const Identifier& identifier = decl->getIdentifier();
        m_declMap[identifier.getAsString()] = decl;
    }

    void
    DeclScope::setParentScope(DeclScope* scope) {
        m_parentScope = scope;
    }

}
