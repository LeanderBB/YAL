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

#include "yal/frontend/ast/stmtlistscoped.h"
#include "yal/frontend/ast/astvisitor.h"
#include "yal/frontend/module.h"

namespace yal::frontend {

    StmtListScoped::StmtListScoped(Module& module,
                                   const SourceInfo& info,
                                   DeclScope *parentScope):
        Statement(module, AstType::StmtListScoped, info),
        m_statements(StatementList::allocator_type(module.getASTContext().getAllocator())),
        m_scope(this, parentScope) {

    }

    void
    StmtListScoped::setStatements(StatementList&& statements) {
        m_statements = std::move(statements);
    }

    void
    StmtListScoped::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
