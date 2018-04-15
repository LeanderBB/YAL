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

#include "yal/frontend/types/typedecl.h"
#include "yal/ast/declbase.h"
#include "yal/ast/decltypefunction.h"
#include "yal/ast/declstruct.h"
#include "yal/ast/declfunction.h"

namespace yal::frontend {

    TypeDecl::TypeDecl(yal::DeclFunction* decl):
        Type(&decl->getModule(), Kind::TypeDecl, Identifier(decl->getName(), decl->getModule())),
        m_decl(decl) {
        m_function = 1;
    }

    TypeDecl::TypeDecl(yal::DeclTypeFunction* decl):
        Type(&decl->getModule(), Kind::TypeDecl, Identifier(decl->getName(), decl->getModule())),
        m_decl(decl) {
        m_typefunction = 1;
    }

    TypeDecl::TypeDecl(yal::DeclStruct* decl):
        Type(&decl->getModule(), Kind::TypeDecl, Identifier(decl->getName(), decl->getModule())),
        m_decl(decl) {
        m_struct = 1;
        m_functionTargetable = 1;
    }
}
