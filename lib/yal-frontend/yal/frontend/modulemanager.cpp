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

    enum {kStackSize = 4096};
    ModuleManager::ModuleManager() :
        m_allocator(kStackSize),
        m_modules() {

    }

    ModuleManager::~ModuleManager() {
        clear();
    }

    Module*
    ModuleManager::createNew(const StringRef name,
                             const SourceItem& item) {
        SourceManager::Handle handle = item.getHanlde();
        if (getModuleBySourceHanlde(handle) == nullptr
                && getModuleByName(name) == nullptr) {
            const StringRef modulePath = item.getPath();
            std::optional<std::string> fullPath = Path::GetRealPath(modulePath);
            std::string fullModulePath = fullPath.value_or(modulePath.toString());
            const size_t moduleCount = m_modules.size();
            void* ptr = m_allocator.allocate(sizeof(Module));
            Module* module = new (ptr) Module(*this,
                                              name,
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
