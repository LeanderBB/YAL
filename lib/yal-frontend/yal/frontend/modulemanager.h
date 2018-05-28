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

#include "yal/frontend/module.h"
#include "yal/io/sourcemanager.h"
#include <vector>
#include "yal/util/allocator/allocatorstack.h"

namespace yal {
    class DeclModule;
}

namespace yal::frontend {

    class ModuleManager {
    public:

        ModuleManager();

        ~ModuleManager();

        YAL_NO_COPY_CLASS(ModuleManager);

        Module* createNew(const StringRef name,
                          const SourceManager::Handle handle);

        const Module *getModuleById(const Module::Id id) const;

        const Module* getModuleByName(const StringRef name) const;

        const Module* getModuleBySourceHanlde(const SourceManager::Handle handle);

        void clear();
    private:
        AllocatorStack m_allocator;
        std::vector<Module*> m_modules;
    };
}
