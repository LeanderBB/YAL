/*
 *
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

#include "yal/frontend/ast/declmodule.h"
#include "yal/frontend/module.h"
#include "yal/frontend/ast/astcontext.h"
#include "yal/frontend/ast/astvisitor.h"

namespace yal::frontend {

    DeclModule::DeclModule(Module& module,
                           StringRef name):
        DeclBase(module, AstType::DeclModule, Identifier(name)),
        m_declartions(StdAllocatorWrapperStack<DeclBase*>(module.getASTContext().getAllocator())),
        m_declScope(this){

    }

    void
    DeclModule::setDecls(Decls&& decls) {
        m_declartions = std::move(decls);
    }

    void
    DeclModule::acceptVisitor(AstVisitor &visitor) {
        visitor.visit(*this);
    }
}
