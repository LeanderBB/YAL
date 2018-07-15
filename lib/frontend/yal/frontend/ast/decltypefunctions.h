/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/ast/declbase.h"
#include "yal/frontend/ast/declscope.h"

namespace yal::frontend {
    class DeclTypeFunction;
    class DeclTypeFunctions final : public DeclBase {
    public:
        using Decls  = std::vector<DeclTypeFunction*, StdAllocatorWrapperStack<DeclTypeFunction*>>;

        DeclTypeFunctions(Module& module,
                          DeclScope &parentScope,
                          const SourceInfo& srcInfo,
                          Type& type);

        const Type& getType() const {
            return m_type;
        }

        void setDecls(Decls&& decls);

        Decls& getDecls() {
            return m_decls;
        }

        const Decls& getDecls() const {
            return m_decls;
        }

        DeclScope& getImplScope() {
            return m_implScope;
        }

        const DeclScope& getImplScope() const {
            return m_implScope;
        }

    private:
        Type& m_type;
        Decls m_decls;
        DeclScope m_implScope;
    };

}
