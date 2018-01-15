/*
 *  Copyright 2017 by Leander Beernaert (leanderbb@gmail.com)
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

#include "yal/ast/module.h"
#include "yal/ast/declmodule.h"
#include <cstring>
namespace yal {

    std::string
    Module::ModuleNameFromPath(const StringRef str) {
        // search for last '.'
        size_t posDot = str.size()+1;
        for (size_t i = 0; i< str.size();++i) {
            const size_t pos = str.size() - i - 1;
            if (str[pos] == '.') {
                posDot = pos;
                break;
            }
        }
         // search for '/' or '\\'
        const size_t secondSearchStart = (posDot == str.size() + 1)
                ? str.size() - 1
                : posDot - 1;
        posDot = std::min(posDot, str.size());
        size_t posSlash = 0;
        for (size_t i = secondSearchStart; i >0 ; --i) {
            if (str[i] == '\\' || str[i] == '/') {
                posSlash = i + 1;
                break;
            }
        }
        std::string name(str.data() + posSlash, (posDot - posSlash));
        return name;
    }

    Module::Module(const StringRef name,
                   ModuleManager& manager,
                   const SourceManager::Handle handle,
                   const Id id):
        m_id(id),
        m_sourceHandle(handle),
        m_manager(manager),
        m_name(ModuleNameFromPath(name)),
        m_astContext(),
        m_typeContext(),
        m_rootNode(nullptr){

        m_rootNode = newASTNode<DeclModule>(m_name);
        YAL_ASSERT_MESSAGE(m_rootNode != nullptr, "Failed to allocate root node");
    }

}
