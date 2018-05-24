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

#include "yal/frontend/types/identifier.h"
#include "yal/util/stringref.h"
#include "yal/util/iteratorrange.h"

namespace yal::frontend {

    class DeclBase;
    class Identifier;
    class DeclFunction;
    class DeclTypeFunction;
    class DeclStruct;
    class DeclModule;
    class StmtListScoped;

    class DeclScope {
    private:
        using DeclMap = IdentifierPtrMap<DeclBase*>;
    public:
        using DeclRange= IteratorRange<DeclMap::iterator>;
        using DeclRangeConst= IteratorRange<DeclMap::const_iterator>;

        enum class Kind {
            Module,
            Function,
            TypeFunction,
            Struct,
            StmtList,
            Scope
        };

        DeclScope();

        DeclScope(DeclFunction* decl,
                  DeclScope* parent);

        DeclScope(DeclTypeFunction* decl,
                  DeclScope* parent);

        DeclScope(DeclStruct* decl,
                  DeclScope* parent);

        DeclScope(StmtListScoped* node,
                  DeclScope* parent);

        DeclScope(DeclModule* decl);

        bool hasDecl(const DeclBase* decl,
                     const bool local) const;

        bool hasDecl(const Identifier& identIteratorRangeifier,
                     const bool local) const;

        DeclBase* getDecl(const Identifier &identifier,
                          const bool local) const;

        void addDecl(DeclBase* decl);

        void setParentScope(DeclScope* scope);

        DeclScope* getParentScope() {
            return m_parentScope;
        }

        const DeclScope* getParentScope() const {
            return m_parentScope;
        }

        const DeclScope* getFirstScopeOfTypeDecl() const;

        DeclRange getDecls() {
            return DeclRange(m_declMap.begin(), m_declMap.end());
        }

        DeclRangeConst getDeclsConst() const{
            return DeclRangeConst(m_declMap.cbegin(), m_declMap.cend());
        }

        Kind getKind() const {
            return m_scopeKind;
        }

        bool isScopePartOfDecl() const {
            return m_scopeDecl != nullptr;
        }

        DeclBase* getScopeDecl() const {
            return m_scopeDecl;
        }

        bool isModuleScope() const;
        bool isFunctionTypeScope() const;
        bool isFunctionScope() const;
        bool isTypeFunctionScope() const;
        bool isScopedScope() const;
        bool isStructScope() const;
        bool isStmtListScope() const;

private:
        DeclScope* m_parentScope;
        DeclMap m_declMap;
        DeclBase* m_scopeDecl;
        Kind m_scopeKind;
    };
}

