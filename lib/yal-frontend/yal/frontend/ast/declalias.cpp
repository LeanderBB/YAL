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

#include "yal/frontend/ast/declalias.h"
#include "yal/frontend/module.h"
#include "yal/frontend/parser/stdeclalias.h"
#include "yal/frontend/types/typealias.h"

namespace yal::frontend {

   DeclAliasWeak::DeclAliasWeak(Module& module,
                  DeclScope& scope,
                  TypeAliasWeak& aliasType):
       DeclNamed (module,
                  AstType::DeclAliasWeak,
                  aliasType.getIdentifier(),
                  aliasType.getSTDecl().getSourceInfo(),
                  scope),
       m_aliasType(aliasType) {

   }

   DeclAliasStrong::DeclAliasStrong(Module& module,
                   DeclScope& scope,
                   TypeAliasStrong& aliasType):
       DeclNamed (module,
                  AstType::DeclAliasStrong,
                  aliasType.getIdentifier(),
                  aliasType.getSTDecl().getSourceInfo(),
                  scope),
       m_aliasType(aliasType) {
   }
}
