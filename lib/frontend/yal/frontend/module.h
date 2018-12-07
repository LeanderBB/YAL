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

#pragma once

#include "yal/io/sourcemanager.h"
#include "yal/frontend/ast/astcontext.h"
#include "yal/frontend/parser/stcontext.h"
#include "yal/frontend/types/typecontext.h"
#include "yal/util/stringpool.h"
#include <string>

namespace yal::frontend {

    class DeclModule;
    class ModuleManager;

    class Module {
    public:
        typedef uint32_t Id;

        using ModuleList = std::vector<Module*>;

        Module(ModuleManager &manager,
               std::string &&name,
               std::string&& path,
               const SourceManager::Handle handle,
               const Id id);

        YAL_MOVE_ONLY_CLASS(Module);

        const StringRef getName() const {
            return m_name;
        }

        const StringRef getPath() const {
            return m_path;
        }

        SourceManager::Handle getSourceHandle() const {
            return m_sourceHandle;
        }

        ASTContext& getASTContext() {
            return m_astContext;
        }

        STContext& getSTContext() {
            return m_stcontext;
        }

        const DeclModule* getDeclNode() const {
            return m_rootNode;
        }

        DeclModule* getDeclNode() {
            return m_rootNode;
        }

        Id getId() const {
            return m_id;
        }

        ModuleManager& getManager() {
            const Module* cthis = this;
            return const_cast<ModuleManager&>(cthis->getManager());
        }

        const ModuleManager& getManager() const {
            return m_manager;
        }

        TypeContext& getTypeContext() {
            return m_typeContext;
        }

        const TypeContext& getTypeContext() const {
            return m_typeContext;
        }

        StringPool& getStringPool() {
            return m_stringPool;
        }

        const StringPool& getStringPool() const {
            return m_stringPool;
        }

        template <typename T, typename... ARGS>
        T* newASTNode(ARGS&& ...args) {
            return  m_astContext.getAllocator()
                    .construct<T>(*this, std::forward<ARGS>(args)...);
        }

        bool import(const Module& module);

        bool isImported(const Module& module) const;

        const ModuleList& getImported() const {
            return m_importedModules;
        }

    private:
        const Id m_id;
        const SourceManager::Handle m_sourceHandle;
        ModuleManager& m_manager;
        const std::string m_name;
        const std::string m_path;
        ASTContext m_astContext;
        STContext m_stcontext;
        TypeContext m_typeContext;
        StringPool m_stringPool;
        DeclModule* m_rootNode;
        ModuleList m_importedModules;
    };
}
