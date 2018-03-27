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

#include "yal/parser/ststatement.h"

namespace yal {

    class STStmtDecl final : public STStatement {
    public:
        STStmtDecl(const SyntaxTree* decl);

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

        const SyntaxTree* getDecl() const {
            return m_decl;
        }
    private:
        const SyntaxTree* m_decl;
    };

}
