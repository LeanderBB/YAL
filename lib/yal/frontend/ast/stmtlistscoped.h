/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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

#include "yal/frontend/ast/statement.h"
#include "yal/frontend/ast/declscope.h"

namespace yal::frontend {

    class StmtListScoped final : public Statement
    {
    public:
        StmtListScoped(Module& module,
                       const SourceInfo& info,
                       DeclScope* parentScope);

        StatementList& getStatements() {
            return m_statements;
        }

        const StatementList& getStatements() const {
            return m_statements;
        }

        void setStatements(StatementList&& statements);

        DeclScope& getListScope() {
            return m_scope;
        }

        void acceptVisitor(AstVisitor& visitor) override final;

    private:
        StatementList m_statements;
        DeclScope m_scope;
    };
}
