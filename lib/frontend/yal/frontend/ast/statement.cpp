/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/ast/statement.h"
#include "yal/frontend/ast/astcontext.h"
#include "yal/frontend/module.h"

namespace yal::frontend {

    Statement::Statement(Module& module,
                         const AstType type,
                         const SourceInfo &srcInfo):
        m_module(module),
        m_astType(type),
        m_sourceInfo(srcInfo) {

    }

    Statement::~Statement() {

    }

}


