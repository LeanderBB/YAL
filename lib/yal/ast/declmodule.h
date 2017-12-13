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
#include "yal/ast/declscope.h"
#include "yal/ast/declbase.h"
namespace yal {
    class Module;
    class AstVisitor;
    class DeclModule : public DeclBase{
    public:

        DeclModule(Module& module,
                   StringRef name);

        std::vector<DeclBase*>& getDeclarations()  {
            const DeclModule* const_this = this;
            return const_cast<std::vector<DeclBase*>&>(const_this->getDeclarations());
        }
        const std::vector<DeclBase*>& getDeclarations() const {
            return m_declartions;
        }

        void addDecl(DeclBase* node);

        DeclScope* getDeclScope() {
            return &m_declScope;
        }

        const DeclScope* getDeclScope() const {
            return &m_declScope;
        }

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        std::vector<DeclBase*> m_declartions;
        DeclScope m_declScope;
    };
}
