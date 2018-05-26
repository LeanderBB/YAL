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

#include "yal/compiler/stages/stagesizecalc.h"
#include "yal/compiler/compiler.h"
#include "yal/ast/astnodes.h"
#include "yal/util/prettyprint.h"
#include "yal/util/log.h"
#include "yal/util/stackjump.h"
namespace yal {

    StageSizeCalc::StageSizeCalc(Compiler& compiler):
        m_compiler(compiler) {
    }

    bool
    StageSizeCalc::execute(DeclBase* decl) {
        (void) m_compiler;
        DeclStruct* structDecl = dyn_cast<DeclStruct>(decl);
        if (structDecl != nullptr) {
            decl->getModule().getAstTypeContext().updateTypeSize(structDecl);
        }
        return true;
    }
}
