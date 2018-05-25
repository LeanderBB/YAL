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

#include "yal/frontend/frontend.h"

#include "yal/error/errorreporter.h"
#include "yal/frontend/module.h"
#include "yal/frontend/modulemanager.h"
#include "yal/frontend/passes/passes.h"
#include "yal/frontend/passes/parser/passparser.h"
#include "yal/frontend/passes/decl/passdecl.h"
#include "yal/frontend/passes/fnret/passfnret.h"
#include "yal/frontend/passes/move/passmove.h"
#include "yal/frontend/passes/type/passtype.h"
#include "yal/io/sourceitems.h"

namespace yal::frontend {

    Frontend::Frontend(ErrorReporter &errReporter,
                       SourceManager &srcManager,
                       ModuleManager &moduleManager):
        m_errReporter(errReporter),
        m_srcManager(srcManager),
        m_moduleManager(moduleManager) {

    }


    Module*
    Frontend::compile(const SourceManager::Handle item,
                      const FrontendOptions& options) {
        (void) options;

        SourceItemOpt sourceItemOpt = m_srcManager.getItem(item);
        if (!sourceItemOpt.has_value()) {
            return nullptr;
        }
        SourceItem* sourceItem = sourceItemOpt.value();

        yal::frontend::Module* module =
                m_moduleManager.createNew(sourceItem->getPath(), item);
        if (module == nullptr) {
            return nullptr;
        }

        // setup pass options
        PassOptions passOptions(m_errReporter,
                                m_srcManager,
                                *module,
                                *sourceItem);
        // create passes
        PassParser passParser;
        PassDecl passDecl;
        PassFnRet passFnRet;
        PassType passType;
        PassMove passMove;

        if (!passParser.execute(passOptions)) {
            goto exit;
        }

        if (!passDecl.execute(passOptions)) {
            goto exit;
        }

        if (!passFnRet.execute(passOptions)) {
            goto exit;
        }

        if (!passType.execute(passOptions)) {
            goto exit;
        }

        if (!passMove.execute(passOptions)) {
            goto exit;
        }
exit:
        return m_errReporter.hasErrors()
                ? nullptr
                : module;
    }

}
