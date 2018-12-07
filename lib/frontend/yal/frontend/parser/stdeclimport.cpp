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

#include "yal/frontend/parser/stdeclimport.h"
#include "yal/frontend/parser/stparser.h"
#include "yal/util/stringpool.h"

namespace yal::frontend {

    STDeclImport::STDeclImport(STParser& parser):
    STDecl(SyntaxTreeType::STDeclImport),
    m_moduleChunks(parser.getSTContext().getStdAllocatorWrapper<ModuleChuncks::value_type>()),
    m_moduleIdentifier() {
        m_moduleChunks.reserve(4);
    }

    void
    STDeclImport::addModuleChunk(const STIdentifier* identifier) {
        m_moduleChunks.push_back(identifier);
    }

    void
    STDeclImport::finalize(STParser& parser) {
        StringPool& strPool = parser.getStringPool();
        std::string fullStr;
        size_t totalSize = 0;
        for (auto& id : m_moduleChunks) {
            totalSize += id->getString().size();
        }
        totalSize += (m_moduleChunks.size() - 1) * 2;
        fullStr.reserve(totalSize);
        bool isFirst = true;
        for (auto& id : m_moduleChunks) {
            if (!isFirst) {
                fullStr += "::";
            }
            StringRef idStr = id->getString();
            fullStr.append(idStr.data(), idStr.size());
            isFirst = false;
        }
        m_moduleIdentifier = strPool.getOrCreate(std::move(fullStr));
    }

}
