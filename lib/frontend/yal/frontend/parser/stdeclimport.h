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

#pragma once

#include "yal/frontend/parser/syntaxtree.h"

namespace yal::frontend {
    class STParser;
    class STDeclImport : public STDecl {
    public:
        using ModuleChuncks = STVector<const STIdentifier*>;

        STDeclImport(STParser& parser);

        void addModuleChunk(const STIdentifier* identifier);

        void finalize(STParser& parser);

        const ModuleChuncks& getModuleChuncks() const {
            return m_moduleChunks;
        }

        StringRef getModuleIdentifier() const {
            return m_moduleIdentifier;
        }

    private:
        ModuleChuncks m_moduleChunks;
        StringRef m_moduleIdentifier;
    };
}
