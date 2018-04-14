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

#include "yal/parser/syntaxtree.h"

namespace yal {
    class STDecl;
    class STDeclModule final : public SyntaxTree {
    public:

        using Decls = std::vector<STDecl*>;

        STDeclModule();

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

        void addDecl(STDecl* decl);

        const Decls& getDecls() const {
            return m_decls;
        }

    protected:
        Decls m_decls;
    };
}