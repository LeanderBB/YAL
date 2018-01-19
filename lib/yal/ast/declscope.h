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
#include "yal/util/stringref.h"
#include <unordered_map>
#include "yal/util/iteratorrange.h"
namespace yal {

    class DeclBase;
    class Identifier;
    class DeclFunction;
    class DeclTypeFunction;
    class DeclStruct;
    class DeclModule;
    class DeclScope {
    private:
        using DeclMap = std::unordered_map<StringRef, DeclBase*>;
    public:
        using DeclRange= IteratorRange<DeclMap::iterator>;
        using DeclRangeConst= IteratorRange<DeclMap::const_iterator>;

        enum class Kind {
            Module,
            Function,
            TypeFunction,
            Struct,
            Scope
        };

        DeclScope();
        DeclScope(DeclFunction* decl,
                  const DeclScope* parent);
        DeclScope(DeclTypeFunction* decl,
                  const DeclScope* parent);
        DeclScope(DeclStruct* decl,
                  const DeclScope* parent);
        DeclScope(DeclModule* decl);

        bool hasDecl(const DeclBase* decl,
                     const bool local) const;

        bool hasDecl(const Identifier& identIteratorRangeifier,
                     const bool local) const;

        DeclBase* getDecl(const Identifier &identifier,
                          const bool local) const;

        void addDecl(DeclBase* decl);

        void setParentScope(const DeclScope* scope);

        const DeclScope* getParentScope() const {
            return m_parentScope;
        }

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
private:
        const DeclScope* m_parentScope;
        DeclMap m_declMap;
        DeclBase* m_scopeDecl;
        Kind m_scopeKind;
    };
}

