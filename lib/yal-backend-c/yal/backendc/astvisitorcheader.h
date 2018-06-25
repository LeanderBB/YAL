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

#pragma once

#include "yal/frontend/ast/astvisitor.h"
#include "yal/util/codewriter.h"

namespace yal::frontend {
    class Module;
}

namespace yal::backend::c {

    class CTypeCache;
    struct BackendOptions;

    class AstVisitorCHeader final : public yal::frontend::AstVisitor {
    public:
        AstVisitorCHeader(ByteStream& stream,
                          yal::frontend::Module& module,
                          const CTypeCache& cache);

        void visit(yal::frontend::DeclFunction& decl) override final;

        void visit(yal::frontend::DeclTypeFunction& decl) override final;

        void visit(yal::frontend::DeclTypeFunctions& decls) override final;

        void visit(yal::frontend::DeclStruct& decl) override final;

        void visit(yal::frontend::DeclModule& decl) override final;

        bool execute(const BackendOptions& options);
    private:
        CodeWriter m_writer;
        yal::frontend::Module& m_module;
        const CTypeCache& m_typeCache;
    };

}
