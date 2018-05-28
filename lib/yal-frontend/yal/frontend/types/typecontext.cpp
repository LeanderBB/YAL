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

#include "yal/frontend/types/typecontext.h"
#include "yal/frontend/types/typebuiltin.h"
#include "yal/frontend/types/identifier.h"

namespace yal::frontend {

    enum {
        kAllocatorBlockSize = 1024
    };

    TypeContext::TypeContext() :
        m_allocator(kAllocatorBlockSize),
        m_types(),
        m_typeIdCounter(0){

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Boolean);
            type->m_typeId = m_typeIdCounter++;
            m_typeBool = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int8);
            type->m_typeId = m_typeIdCounter++;
            m_typeI8 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt8);
            type->m_typeId = m_typeIdCounter++;
            m_typeU8 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int16);
            type->m_typeId = m_typeIdCounter++;
            m_typeI16 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt16);
            type->m_typeId = m_typeIdCounter++;
            m_typeU16 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int32);
            type->m_typeId = m_typeIdCounter++;
            m_typeI32 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt32);
            type->m_typeId = m_typeIdCounter++;
            m_typeU32 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
        }
        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int64);
            type->m_typeId = m_typeIdCounter++;
            m_typeI64 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt64);
            type->m_typeId = m_typeIdCounter++;
            m_typeU64 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Float32);
            type->m_typeId = m_typeIdCounter++;
            m_typeFloat = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Float64);
            type->m_typeId = m_typeIdCounter++;
            m_typeDouble = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
        }

    }

    bool
    TypeContext::hasType(const Identifier& identifier) const {
        const auto it = m_types.find(&identifier);
        return it != m_types.end();
    }

    bool
    TypeContext::hasType (const Type& type) const {
        return hasType(type.getIdentifier());
    }

    const Type*
    TypeContext::getByIdentifier(const Identifier& identifier) const{
        const auto it = m_types.find(&identifier);
        if(it != m_types.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    void
    TypeContext::registerType(Type* type) {
        YAL_ASSERT(hasType(type->getIdentifier()) == false);
        type->m_typeId = m_typeIdCounter++;
        m_types.insert(std::make_pair(&type->getIdentifier(), type));
    }

}
