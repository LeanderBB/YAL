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

#include "yal/frontend/module.h"
#include "yal/frontend/ast/declmodule.h"
#include "yal/os/path.h"

namespace yal::frontend {

    Module::Module(ModuleManager &manager,
                   std::string &&name,
                   std::string &&path,
                   const SourceManager::Handle handle,
                   const Id id):
        m_id(id),
        m_sourceHandle(handle),
        m_manager(manager),
        m_name(std::move(name)),
        m_path(std::move(path)),
        m_astContext(),
        m_typeContext(*this),
        m_rootNode(nullptr){

        m_rootNode = newASTNode<DeclModule>(m_name);
        YAL_ASSERT_MESSAGE(m_rootNode != nullptr, "Failed to allocate root node");
    }


    bool
    Module::import(const Module& module) {
        if (isImported(module)) {
            return true;
        }
        Module& mutMod = const_cast<Module&>(module);
        TypeContext& modTypeCtx = mutMod.getTypeContext();

        // Import types
        auto fn = [this](Type& type) {
            if (!m_typeContext.hasType(type.getIdentifier())) {
                m_typeContext.registerType(&type);
            }
        };
        modTypeCtx.forEachType(fn);

        // Add to list of imported modules
        m_importedModules.push_back(&mutMod);
        return true;
    }

    bool
    Module::isImported(const Module& module) const {
        auto it = std::find_if(std::begin(m_importedModules),
                               std::end(m_importedModules),
                               [id = module.getId()](const Module* v) {
            return v->getId() == id;
        });
        return it != std::end(m_importedModules);
    }

}
