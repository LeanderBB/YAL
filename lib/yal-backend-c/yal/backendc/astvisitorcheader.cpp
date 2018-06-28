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

#include "yal/backendc/astvisitorcheader.h"

#include "yal/backendc/backendc.h"
#include "yal/backendc/ctypegen.h"
#include "yal/frontend/module.h"
#include "yal/frontend/ast/astvisitorimpl.h"

namespace yal::backend::c {

    AstVisitorCHeader::AstVisitorCHeader(ByteStream& stream,
                                         yal::frontend::Module& module,
                                         const CTypeCache& cache) :
        m_writer(stream),
        m_module(module),
        m_typeCache(cache){

    }

    void
    AstVisitorCHeader::visit(const yal::frontend::DeclFunction& decl) {
        CTypeGen::GenDelcFunction(m_writer,
                                  m_typeCache,
                                  decl);
    }

    void
    AstVisitorCHeader::visit(const yal::frontend::DeclTypeFunction& decl) {
        CTypeGen::GenDelcFunction(m_writer,
                                  m_typeCache,
                                  decl);
    }

    void
    AstVisitorCHeader::visit(const yal::frontend::DeclTypeFunctions& node) {
        for (auto& decl : node.getDecls()) {
            resolve(*decl);
            m_writer.write(";\n\n");
        }
    }

    void
    AstVisitorCHeader::visit(const yal::frontend::DeclStruct& decl) {
        CTypeGen::GenDelcStruct(m_writer,
                                m_typeCache,
                                decl);
    }

    void
    AstVisitorCHeader::visit(const yal::frontend::DeclModule& decl) {
        for (auto& decl : decl.getDeclarations()) {
            resolve(*decl);
            m_writer.write(";\n\n");
        }
    }

    void
    AstVisitorCHeader::visit(const yal::frontend::DeclVar&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::DeclParamVar&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::DeclStrongAlias&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::DeclWeakAlias&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::StmtReturn&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::StmtDecl&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::StmtAssign&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprVarRef&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprUnaryOperator&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprBinaryOperator&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprBoolLiteral&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprIntegerLiteral&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprFloatLiteral&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprStructVarRef&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprFnCall&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprTypeFnCall&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::DeclParamVarSelf&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprVarRefSelf&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprCast&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::ExprStructInit&) {}

    void
    AstVisitorCHeader::visit(const yal::frontend::StmtListScoped&) {}

    bool
    AstVisitorCHeader::execute(const BackendOptions& options) {
        m_writer.write("// Auto Generated with YAL Backend C %, do not modify!\n",
                       YAL_BACKEND_C_VERSION_STR);
        m_writer.write("// Header for module %\n\n", m_module.getPath());

        std::string headerGuard = m_module.getName().replace("::","_");
        for(auto& c: headerGuard) {
            c = std::toupper(c);
        }

        if (options.pragmaOnce) {
            m_writer.write("#pragma once\n\n");
        } else {
            m_writer.write(
                        "#ifndef %_H\n#define %_H\n\n",
                        headerGuard,
                        headerGuard);
        }

        CTypeGen::GetBuilitinTypeInfo(m_writer);

         resolve(*m_module.getDeclNode());

        if (!options.pragmaOnce) {
            m_writer.write("\n#endif //%_H\n",
                           headerGuard);
        }
        return true;
    }
}
