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
#include "yal/frontend/modulemanager.h"
#include "yal/os/path.h"
#include <algorithm>

namespace yal::frontend {

    enum {kStackSize = (sizeof(Module) * 16)};
    ModuleManager::ModuleManager(const StringRef moduleSourceDir)
        : m_sourceDir(moduleSourceDir)
        , m_allocator(kStackSize)
        , m_modules() {

    }

    ModuleManager::~ModuleManager() {
        clear();
    }

    static std::string
    ModuleNameFromPath(const StringRef rootdir,
                       const StringRef moduledir) {
        const size_t rootOffset = rootdir.length() != 0 ? rootdir.size() + 1: 0;
        const StringRef basename = moduledir.subStr(rootOffset);
        const StringRef moduleName = Path::RemoveExtension(basename);
        return moduleName.replace(Path::kPathSepStr,"::");
    }

    Module*
    ModuleManager::createNew(const SourceItem& item) {
        SourceManager::Handle handle = item.getHanlde();

        // generate module name from path
        const StringRef modulePath = item.getPath();
        std::optional<std::string> fullPath = Path::GetRealPath(modulePath);
        std::string fullModulePath = fullPath.value_or(modulePath.toString());

        // check if module is actually in the path
        if (!m_sourceDir.compare(fullModulePath, m_sourceDir.size())){
            // Doesn't belog to this source directory, use add external
            return nullptr;
        }

        std::string moduleName = ModuleNameFromPath(m_sourceDir, fullModulePath);

        if (getModuleBySourceHanlde(handle) == nullptr
                && getModuleByName(moduleName) == nullptr) {
            const size_t moduleCount = m_modules.size();
            void* ptr = m_allocator.allocate(sizeof(Module));
            Module* module = new (ptr) Module(*this,
                                              std::move(moduleName),
                                              std::move(fullModulePath),
                                              handle,
                                              static_cast<Module::Id>(moduleCount));
            m_modules.push_back(module);
            return module;
        }
        return nullptr;
    }

    const Module*
    ModuleManager::getModuleById(const Module::Id id) const {
        const size_t idx = static_cast<size_t>(id);
        return idx < m_modules.size()
                ? m_modules[idx]
                  : nullptr;
    }

    const Module*
    ModuleManager::getModuleByName(const StringRef name) const {

        auto it = std::find_if(std::begin(m_modules),
                               std::end(m_modules),
                               [&](const Module* v) {
            return v->getName() ==  name;
        });
        return it == m_modules.end()
                ? nullptr
                : *it;
    }

    const Module*
    ModuleManager::getModuleBySourceHanlde(const SourceManager::Handle handle) {
        auto it = std::find_if(std::begin(m_modules),
                               std::end(m_modules),
                               [&](const Module* v) {
            return handle == v->getSourceHandle();
        });
        return it == m_modules.end()
                ? nullptr
                : *it;
    }

    void
    ModuleManager::clear() {
        for (auto& module : m_modules) {
            module->~Module();
        }
        m_modules.clear();
        m_allocator.reset();
    }

}
