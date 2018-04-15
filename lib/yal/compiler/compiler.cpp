/*
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

#include "yal/compiler/compiler.h"
#include "yal/util/log.h"
#include "yal/frontend/modulemanager.h"
#include "yal/frontend/lexer/lexer.h"
#include "yal/parser/parser.h"
#include "yal/frontend/parser/stparser.h"
#include "yal/ast/astprinter.h"
#include "yal/compiler/stages/stagedecls.h"
#include "yal/compiler/stages/stageexprtype.h"
#include "yal/compiler/stages/stagefnreturn.h"
#include "yal/compiler/stages/stagemovecheck.h"
#include "yal/compiler/stages/stagesizecalc.h"
#include "yal/compiler/stages/stageexprlistbreakdown.h"
#include "yal/frontend/passes/parser/passparser.h"
#include "yal/ast/declmodule.h"
#include "yal/io/memorystream.h"

namespace yal {
    Compiler::Compiler(Log& log,
                       ErrorReporter& errorReporter,
                       SourceManager& srcManager,
                       frontend::ModuleManager& moduleManager):
        m_log(log),
        m_srcManager(srcManager),
        m_moduleManager(moduleManager),
        m_errorReporter(errorReporter) {

    }


    void
    Compiler::setFlags(const uint64_t flags) {
        m_flags = flags;
    }

    frontend::Module*
    Compiler::compile(const SourceManager::Handle source){
        yal::SourceItem* sourceItem = m_srcManager.getItem(source);
        if (sourceItem == nullptr) {
            m_log.error("Could not locate item from source handle.\n");
            return nullptr;
        }

        yal::frontend::Module* module =
                m_moduleManager.createNew(sourceItem->getPath(), source);
        if (module == nullptr) {
            m_log.error("Failed to create module with name '%'.\n",
                        sourceItem->getPath());
            return nullptr;
        }


        frontend::PassParser stageParser(this->getErrorReporter(), *module, *sourceItem);

        if (!stageParser.execute()) {
            return nullptr;
        }

        if (!sourceItem->getByteStream().seek(0)) {
            m_log.error("Failed to seek beginning of stream");
            return nullptr;
        }


        // Run Decl / Type Resolve
        {
            StageDecls stageDecls(*this, *sourceItem, *module);
            if (!stageDecls.execute()) {
                return nullptr;
            }
        }

        // Resolve expression types

        StageFnReturn stageFnRet(*this);
        StageExprType stageExpr(*this, *module);
        StageMoveCheck stageMoveCheck(*this, *module);
        StageSizeCalc stageSizeCalc(*this);
        StageExprListBreakdown stageExprBreakdown(*this, *module);

        auto& decls = module->getDeclNode()->getDeclarations();

        for (auto& decl : decls) {

            if (!stageFnRet.execute(decl)) {
                return nullptr;
            }

            if (!stageExpr.execute(decl)) {
                return nullptr;
            }

            if (!stageMoveCheck.execute(decl)) {
                return nullptr;
            }

            if (!stageExprBreakdown.execute(decl)) {
                return nullptr;
            }

            if (!stageSizeCalc.execute(decl)) {
                return nullptr;
            }
        }

        return module;
    }
}
