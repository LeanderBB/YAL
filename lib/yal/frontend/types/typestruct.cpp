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

#include "yal/frontend/types/typestruct.h"
#include "yal/frontend/parser/stdeclstruct.h"
#include "yal/frontend/module.h"

namespace yal::frontend {

    TypeStruct::TypeStruct(const Module& module,
                           const STDeclStruct &decl):
        Type(&module, Kind::TypeStruct,
             Identifier( decl.getName()->getString(), module)),
        m_stdecl(decl),
        m_decl(nullptr) {

        m_moduleType = 1;
        m_struct = 1;
        m_functionTargetable = 1;
    }

    void
    TypeStruct::setDecl(DeclStruct& decl) {
        m_decl = &decl;
    }

    SourceInfoOpt
    TypeStruct::getSourceInfo() const {
        return SourceInfoOpt(m_stdecl.getName()->getSourceInfo());
    }
}
