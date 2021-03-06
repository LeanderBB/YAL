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

#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/parser/stdeclfunction.h"
#include "yal/frontend/module.h"

namespace yal::frontend {

    Identifier
    TypeFunction::CreateIdentitier(const Module& module,
                                   const STDeclFunction* decl,
                                   const Type* target) {
        if (target != nullptr) {
            return Identifier(decl->getFunctionName().getString(),
                              target->getIdentifier().getName(),
                              module);
        } else {
            return Identifier(decl->getFunctionName().getString(),
                              module);
        }
    }

    TypeFunction::TypeFunction(const Module& module,
                               const STDeclFunction* decl,
                               const Type *target):
        Type(&module, Kind::TypeFunction, CreateIdentitier(module, decl, target)),
        m_stdecl(decl),
        m_decl(nullptr) {

        m_moduleType = 1;

        if (target == nullptr) {
            m_function = 1;
        } else {
            m_typefunction = 1;

            const STDeclFunction::Params& params = decl->getParams();
            if (params.empty()) {
                m_typefunctionStatic = 1;
            } else {
                if (params[0]->getName().getString() != "self") {
                     m_typefunctionStatic = 1;
                }
            }
        }
    }

    void
    TypeFunction::setDecl(DeclFunction* decl) {
        m_decl = decl;
    }

    SourceInfoOpt
    TypeFunction::getSourceInfo() const {
        return SourceInfoOpt(m_stdecl->getName().getSourceInfo());
    }
}
