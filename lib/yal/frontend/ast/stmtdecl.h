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

#include "yal/frontend/ast/statement.h"

namespace yal::frontend {

    class DeclBase;

    class StmtDecl : public Statement{
    public:

        StmtDecl(Module& module,
                 const SourceInfo& srcInfo,
                 DeclBase* decl);

        const DeclBase* getDecl() const {
            return m_decl;
        }

        DeclBase* getDecl() {
            return m_decl;
        }

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        DeclBase* m_decl;
    };
}

namespace yal {
    template <>
    inline yal::frontend::AstType
    get_typeid_instance<yal::frontend::StmtDecl>(const yal::frontend::StmtDecl& node) {
        return node.getAstType();
    }
}
