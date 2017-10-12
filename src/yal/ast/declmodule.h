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

#include <vector>

namespace yal {
    class DeclBase;
    class Module;
    class DeclModule {
    public:

        static void* operator new(std::size_t bytes,
                                  Module& ctx);

        DeclModule(Module& module);

        DeclModule(Module& module,
                   std::vector<DeclBase*>&& declarations);

        std::vector<DeclBase*>& getDeclarations()  {
            const DeclModule* const_this = this;
            return const_cast<std::vector<DeclBase*>&>(const_this->getDeclarations());
        }
        const std::vector<DeclBase*>& getDeclarations() const {
            return m_declartions;
        }

        Module& getModule() {
            return m_module;
        }

        const Module& getModule() const {
            return m_module;
        }

        void addDecl(DeclBase* node);

    private:
        Module& m_module;
        std::vector<DeclBase*> m_declartions;
    };
}
