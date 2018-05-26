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
#include "yal/util/allocator/allocatorstack.h"

namespace yal::frontend {

    class Module;
    class AstVisitor;

    class DeclModule : public DeclBase {
    public:

        using Decls = std::vector<DeclBase*, StdAllocatorWrapperStack<DeclBase*>>;

        DeclModule(Module& module,
                   StringRef name);

        virtual ~DeclModule() = default;

        Decls& getDeclarations() {
            return m_declartions;
        }

        const Decls& getDeclarations() const {
            return m_declartions;
        }

        void setDecls(Decls&& decls);

        DeclScope& getModuleDeclScope() {
            return m_declScope;
        }

        const DeclScope& getModuleDeclScope() const {
            return m_declScope;
        }

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        Decls m_declartions;
        DeclScope m_declScope;
    };
}
