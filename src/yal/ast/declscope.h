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

namespace yal {

    class DeclBase;
    class Identifier;
    class DeclScope {
    public:

        enum class Kind {
            Module,
            Function,
            TypeFunction,
            Struct
        };

        DeclScope(const Kind kind);

        bool hasDecl(const DeclBase* decl,
                     const bool local) const;

        bool hasDecl(const Identifier& identifier,
                     const bool local) const;

        DeclBase* getDecl(const Identifier &identifier,
                          const bool local);

        void addDecl(DeclBase* decl);

        void setParentScope(DeclScope* scope);

        DeclScope* getParentScope() const {
            return m_parentScope;
        }

        Kind getKind() const {
            return m_scopeKind;
        }
    private:
        using DeclMap = std::unordered_map<StringRef, DeclBase*>;
        DeclScope* m_parentScope;
        DeclMap m_declMap;
        Kind m_scopeKind;
    };
}

