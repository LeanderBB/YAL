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
#include "yal/frontend/module.h"
#include "yal/frontend/parser/sttype.h"
#include "yal/frontend/types/typebuiltin.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/types/identifier.h"

namespace yal::frontend {

    enum {
        kAllocatorBlockSize = 512
    };

    TypeContext::TypeContext(const Module &module) :
        m_module(module),
        m_typeIdCounter(0),
        m_allocator(kAllocatorBlockSize),
        m_types(),
        m_typeList(){
        m_typeList.reserve(128);
        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Boolean);
            type->m_typeId = TypeId(m_typeIdCounter++);
            m_typeBool = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
            m_typeList.push_back(type);
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int8);
            type->m_typeId = TypeId(m_typeIdCounter++);
            m_typeI8 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
            m_typeList.push_back(type);
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt8);
            type->m_typeId = TypeId(m_typeIdCounter++);
            m_typeU8 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
            m_typeList.push_back(type);
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int16);
            type->m_typeId = TypeId(m_typeIdCounter++);
            m_typeI16 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
            m_typeList.push_back(type);
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt16);
            type->m_typeId = TypeId(m_typeIdCounter++);
            m_typeU16 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
            m_typeList.push_back(type);
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int32);
            type->m_typeId = TypeId(m_typeIdCounter++);
            m_typeI32 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
            m_typeList.push_back(type);
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt32);
            type->m_typeId = TypeId(m_typeIdCounter++);
            m_typeU32 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
            m_typeList.push_back(type);
        }
        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int64);
            type->m_typeId = TypeId(m_typeIdCounter++);
            m_typeI64 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
            m_typeList.push_back(type);
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt64);
            type->m_typeId = TypeId(m_typeIdCounter++);
            m_typeU64 = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
            m_typeList.push_back(type);
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Float32);
            type->m_typeId = TypeId(m_typeIdCounter++);
            m_typeFloat = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
            m_typeList.push_back(type);
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Float64);
            type->m_typeId = TypeId(m_typeIdCounter++);
            m_typeDouble = type;
            m_types.insert(std::make_pair(&type->getIdentifier(),
                                          type));
            m_typeList.push_back(type);
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

    Type *TypeContext::getByIdentifier(const Identifier& identifier) const{
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
        type->m_typeId = type->getModule() != nullptr
                ? TypeId(m_typeIdCounter++, *type->getModule())
                : TypeId(m_typeIdCounter++);
        m_types.insert(std::make_pair(&type->getIdentifier(), type));
        m_typeList.push_back(type);
    }

    Type*
    TypeContext::resolveType(const STType& type) const {
        switch(type.getType()) {
        case STType::Type::Bool:
            return getTypeBuiltinBool();
        case STType::Type::Int8:
            return getTypeBuiltinI8();
        case STType::Type::Int16:
            return getTypeBuiltinI16();
        case STType::Type::Int32:
            return getTypeBuiltinI32();
        case STType::Type::Int64:
            return getTypeBuiltinI64();
        case STType::Type::UInt8:
            return getTypeBuiltinU8();
        case STType::Type::UInt16:
            return getTypeBuiltinU16();
        case STType::Type::UInt32:
            return getTypeBuiltinU32();
        case STType::Type::UInt64:
            return getTypeBuiltinU64();
        case STType::Type::Float32:
            return getTypeBuiltinFloat32();
        case STType::Type::Float64:
            return getTypeBuiltinFloat64();
        case STType::Type::Custom:{
            const STIdentifier& identifier = type.getIdentifier();
            return resolveType(identifier);
        }
        default:
            YAL_ASSERT(false);
            return nullptr;
        }
    }

    Type*
    TypeContext::resolveType(const STIdentifier& identifier) const {
        if (!identifier.isComplex()) {
            const Identifier id(identifier.getString(), m_module);
            return getByIdentifier(id);
        } else {
            StringRef idStr = identifier.getString();
            const Module* externModule = nullptr;
            size_t moduleNameSize = 0;
            // 1) Check if any imported modules match the prefix
            const Module::ModuleList& imported = m_module.getImported();
            for (auto& module : imported) {
                StringRef moduleName = module->getName();
                if (moduleName.compare(moduleName, moduleName.size())) {
                    externModule = module;
                    moduleNameSize = moduleName.size();
                    break;
                }
            }


            // 2) check individuall identifiers to see if they match any type
            const Module& modToSearch = externModule != nullptr
                    ? *externModule
                    : m_module;
            size_t offset = externModule != nullptr ? moduleNameSize + 2 : 0;
            size_t loc = idStr.findFirstOf(offset, ':');

            if (loc == StringRef::npos
                    && offset >= idStr.size()
                    && externModule != nullptr) {
                // Identifier is not conclusive, abort
                return nullptr;
            }
            Type* result = nullptr;
            while (loc != StringRef::npos && result == nullptr) {
                StringRef idSubStr = idStr.subStr(offset, loc - offset);
                Identifier typeId(idSubStr, modToSearch);
                result = getByIdentifier(typeId);
                offset = loc + 2;
                loc = idStr.findFirstOf(offset, ':');
            }

            if (result == nullptr && offset < idStr.size()) {
                // if we didn't find any type, it means we need to process
                // the last bit and look that up
                StringRef idSubStr = idStr.subStr(offset);
                Identifier typeId(idSubStr, modToSearch);
                result = getByIdentifier(typeId);
            } else {
                if (loc != StringRef::npos) {
                    // invalid result, we should have run out of "::" by now
                    return nullptr;
                }

                YAL_ASSERT(offset < idStr.size());
                StringRef idSubStr = idStr.subStr(offset);
                // If we did find a type, we need to see if it's a function
                // call
                if(result->isFunctionTargetable()) {
                    return const_cast<TypeFunction*>(result->getFunctionWithName(idSubStr));
                } else {
                    return  nullptr;
                }
            }
            return result;
        }
    }
}
