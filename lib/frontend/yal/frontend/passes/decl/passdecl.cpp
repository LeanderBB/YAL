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

#include "yal/frontend/passes/decl/passdecl.h"
#include "yal/error/errorreporter.h"
#include "yal/frontend/module.h"
#include "yal/frontend/parser/stdeclmodule.h"
#include "yal/frontend/passes/passes.h"
#include "yal/frontend/passes/decl/astbuilder.h"
#include "yal/frontend/passes/decl/stpredeclvisitor.h"

namespace yal::frontend {

    PassDecl::PassDecl() {

    }


    bool
    PassDecl::execute(PassOptions &options) {
        // Pre-declare root scope types
        ErrorReporter& errReporter = options.errReporter;
        STPreDeclVisitor preDeclVisitor(options);

        // Run pre-decl pass
        const STDeclModule* stDeclModule =
                options.module.getSTContext().getDeclModule();
        preDeclVisitor.resolve(*stDeclModule);
        if (errReporter.hasErrors()) {
            return false;
        }

        // run Ast builder
        {
            AstBuilder astBuilder(options);
            if (!astBuilder.execute()) {
                return false;
            }
        }

        // TODO: run type size calculator now that everything is known
        return !errReporter.hasErrors();
    }

}
