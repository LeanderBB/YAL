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
#include "yal/frontend/ast/declmodule.h"
#include "yal/frontend/ast/decltypefunction.h"

namespace yal::backend::c {

    AstVisitorCHeader::AstVisitorCHeader(ByteStream& stream,
                                         yal::frontend::Module& module,
                                         const CTypeCache& cache) :
        m_writer(stream),
        m_module(module),
        m_typeCache(cache){

    }

    void
    AstVisitorCHeader::visit(yal::frontend::DeclFunction& decl) {
        CTypeGen::GenDelcFunction(m_writer,
                                  m_typeCache,
                                  decl);
    }

    void
    AstVisitorCHeader::visit(yal::frontend::DeclTypeFunction& decl) {
        CTypeGen::GenDelcFunction(m_writer,
                                  m_typeCache,
                                  decl);
    }

    void
    AstVisitorCHeader::visit(yal::frontend::DeclStruct& decl) {
        CTypeGen::GenDelcStruct(m_writer,
                                m_typeCache,
                                decl);
    }

    void
    AstVisitorCHeader::visit(yal::frontend::DeclModule& decl) {
        Formater& formater = m_writer.getFormater();
        for (auto& decl : decl.getDeclarations()) {
            decl->acceptVisitor(*this);
            Format(formater, ";\n\n");
            m_writer.write(formater);
        }
    }

    bool
    AstVisitorCHeader::execute(const BackendOptions& options) {
        Formater& formater = m_writer.getFormater();
        Format(formater, "// Auto Generated with YAL Backend C %, do not modify!\n",
               YAL_BACKEND_C_VERSION_STR);
        FormatAppend(formater, "// Header for module %\n\n", m_module.getPath());
        m_writer.write(formater);

        std::string headerGuard = m_module.getName().replace("::","_");
        for(auto& c: headerGuard) {
            c = std::toupper(c);
        }

        if (options.pragmaOnce) {
            Format(formater, "#pragma once\n\n");
        } else {
            Format(formater,
                   "#ifndef %_H\n#define %_H\n\n",
                   headerGuard,
                   headerGuard);
        }
        m_writer.write(formater);

        CTypeGen::GetBuilitinTypeInfo(m_writer);

        m_module.getDeclNode()->acceptVisitor(*this);

        if (!options.pragmaOnce) {
            Format(formater,
                   "\n#endif //%_H\n",
                   headerGuard);
        }
        return true;
    }
}
