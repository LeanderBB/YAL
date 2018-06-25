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

#include "yal/frontend/ast/decltypefunctions.h"
#include "yal/frontend/module.h"
#include "yal/frontend/ast/astvisitor.h"
#include "yal/frontend/ast/decltypefunction.h"
namespace yal::frontend {

    DeclTypeFunctions::DeclTypeFunctions(Module& module,
                                         DeclScope &parentScope,
                                         const SourceInfo &srcInfo,
                                         Type &type):
        DeclBase(module,
                 AstType::DeclTypeFunctions,
                 srcInfo,
                 parentScope),
        m_type(type),
        m_decls(Decls::allocator_type(module.getASTContext().getAllocator())),
        m_implScope(*this, &parentScope) {

    }

    void
    DeclTypeFunctions::setDecls(Decls&& decls) {
        m_decls = std::move(decls);
    }

    void
    DeclTypeFunctions::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

}
