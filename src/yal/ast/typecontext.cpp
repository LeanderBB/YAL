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

#include "yal/ast/typecontext.h"
#include "yal/ast/typebuiltin.h"
#include "yal/ast/identifier.h"
#include "yal/ast/declfunction.h"
#include "yal/ast/decltypefunction.h"
#include "yal/ast/declstruct.h"
#include "yal/ast/declparamvar.h"
#include "yal/ast/reftype.h"
#include "yal/ast/typedecl.h"
#include "yal/util/log.h"
#include "yal/util/prettyprint.h"
namespace yal{

    enum {
        kAllocatorBlockSize = 1024
    };

    TypeContext::TypeContext() :
        m_allocator(kAllocatorBlockSize),
        m_types(){

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Boolean);
            m_typeBool = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int8);
            m_typeI8 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt8);
            m_typeU8 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int16);
            m_typeI16 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt16);
            m_typeU16 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int32);
            m_typeI32 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt32);
            m_typeU32 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }
        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int64);
            m_typeI64 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt64);
            m_typeU64 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Float);
            m_typeFloat = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Double);
            m_typeDouble = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

    }

    bool
    TypeContext::hasType(const Identifier& identifier) const {
        const auto it = m_types.find(identifier.getAsString());
        return it != m_types.end();
    }

    const Type*
    TypeContext::getByIdentifier(const Identifier& identifier) const{
        const auto it = m_types.find(identifier.getAsString());
        if(it != m_types.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    Type*
    TypeContext::addType(DeclFunction* decl) {
        YAL_ASSERT(hasType(decl->getIdentifier()) == false);
        auto type = m_allocator.construct<TypeDecl>(decl);
        m_types.insert(std::make_pair(type->getIdentifier().getAsString(), type));
        return type;
    }

    Type*
    TypeContext::addType(DeclTypeFunction* decl) {
        YAL_ASSERT(hasType(decl->getIdentifier()) == false);
        auto type = m_allocator.construct<TypeDecl>(decl);
        m_types.insert(std::make_pair(type->getIdentifier().getAsString(), type));
        return type;
    }

    Type*
    TypeContext::addType(DeclStruct* decl) {
        YAL_ASSERT(hasType(decl->getIdentifier()) == false);
        auto type = m_allocator.construct<TypeDecl>(decl);
        m_types.insert(std::make_pair(type->getIdentifier().getAsString(), type));
        return type;
    }
}
