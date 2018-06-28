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

#include "yal/backendc/ctype.h"
#include "yal/backendc/ctypebuiltin.h"
#include "yal/frontend/types/type.h"
#include "yal/frontend/types/typebuiltin.h"
#include "yal/frontend/types/typecontext.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/types/typestruct.h"
#include "yal/frontend/types/qualtype.h"
#include "yal/util/format.h"

namespace yal::backend::c {

    static StringRef
    GetCIdentifierBuiltin(const frontend::TypeBuiltin& type) {
        return CTypeBuilitin::GetCTypeName(type);
    }

    static std::string
    GenCIdentifierStruct(const frontend::Type& type) {
        const std::string fullIdentifier = type.getIdentifier().getFullIdentifier();
        StringRef ref(fullIdentifier);
        std::string result = ref.replace("::","_");
        return result;
    }

    static std::string
    GenCIdentifierFunction(const frontend::Type& type) {
        std::string result = type.getIdentifier().getFullIdentifier();
        StringRef ref(result);
        return ref.replace("::","_");
    }

    static std::string
    GenCIdentifier(const frontend::Type& type) {
        switch(type.getKind()) {
        case frontend::Type::Kind::TypeBuiltin:{
            const frontend::TypeBuiltin* typeBuiltIn =
                    yal::dyn_cast<frontend::TypeBuiltin>(&type);
            YAL_ASSERT(typeBuiltIn != nullptr);
            return GetCIdentifierBuiltin(*typeBuiltIn).toString();
        }
        case frontend::Type::Kind::TypeFunction:
            return GenCIdentifierFunction(type);
        case frontend::Type::Kind::TypeStruct:
            return GenCIdentifierStruct(type);
        default:
            YAL_ASSERT_MESSAGE(false, "Unknown type kind");
            return "Unknown";
        }
    }

    CType::CType(const frontend::Type &type):
        m_type(type),
        m_identifier(GenCIdentifier(type)) {

    }


    // CTypeCache ------------------------------------------------------

    void
    CTypeCache::init(const frontend::TypeContext& context) {
        m_ctypes.clear();
        m_ctypes.reserve(context.getTypeCount());
        uint64_t idx = 0;
        auto fn = [this, &idx](const frontend::Type& type) {
            const uint64_t typeId = type.getTypeId();
            (void) typeId;
            // make sure the types are oreded correctly
            YAL_ASSERT(typeId == idx);
            m_ctypes.emplace_back(type);
            ++idx;
        };
        context.forEachType(fn);
    }


    const CType*
    CTypeCache::getCType(const frontend::Type& type) const {
        const uint64_t typeId = type.getTypeId();
        return typeId < m_ctypes.size()
                ? &m_ctypes[typeId]
                : nullptr;
    }
}
