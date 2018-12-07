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

#include "yal/util/stringref.h"
#include <vector>

namespace yal {
    struct Formater;
}

namespace yal::frontend {
    class Type;
    class TypeContext;
}

namespace yal::backend::c {

    class CType {
    public:

        CType(const frontend::Type& type);

        const frontend::Type& getFrontendType() const {
            return m_type;
        }

        StringRef getCIdentifier() const {
            return m_identifier;
        }

    private:
        const frontend::Type& m_type;
        const std::string m_identifier;
    };

    class CTypeCache {
    public:
        CTypeCache() = default;

        YAL_NO_COPY_CLASS(CTypeCache);

        void init(const frontend::TypeContext& ctx);

        const CType* getCType(const frontend::Type& type) const;

    private:
        using TypeList = std::vector<CType>;
        TypeList m_ctypes;
    };

}
