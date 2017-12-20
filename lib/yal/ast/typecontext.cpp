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
        m_types(),
        m_typeIdCounter(0){

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Boolean);
            type->m_typeId = m_typeIdCounter++;
            m_typeBool = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int8);
            type->m_typeId = m_typeIdCounter++;
            m_typeI8 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt8);
            type->m_typeId = m_typeIdCounter++;
            m_typeU8 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int16);
            type->m_typeId = m_typeIdCounter++;
            m_typeI16 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt16);
            type->m_typeId = m_typeIdCounter++;
            m_typeU16 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int32);
            type->m_typeId = m_typeIdCounter++;
            m_typeI32 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt32);
            type->m_typeId = m_typeIdCounter++;
            m_typeU32 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }
        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Int64);
            type->m_typeId = m_typeIdCounter++;
            m_typeI64 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::UInt64);
            type->m_typeId = m_typeIdCounter++;
            m_typeU64 = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Float);
            type->m_typeId = m_typeIdCounter++;
            m_typeFloat = type;
            m_types.insert(std::make_pair(type->getIdentifier().getAsString(),
                                          type));
        }

        {
            auto type = m_allocator.construct<TypeBuiltin>(TypeBuiltin::DataType::Double);
            type->m_typeId = m_typeIdCounter++;
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
        type->m_typeId = m_typeIdCounter++;
        m_types.insert(std::make_pair(type->getIdentifier().getAsString(), type));
        decl->setDeclType(type);
        return type;
    }

    Type*
    TypeContext::addType(DeclTypeFunction* decl) {
        YAL_ASSERT(hasType(decl->getIdentifier()) == false);
        auto type = m_allocator.construct<TypeDecl>(decl);
        type->m_typeId = m_typeIdCounter++;
        m_types.insert(std::make_pair(type->getIdentifier().getAsString(), type));
        const_cast<Type*>(decl->getTargetType()->getType())->addFunction(type);
        decl->setDeclType(type);
        return type;
    }

    Type*
    TypeContext::addType(DeclStruct* decl) {
        YAL_ASSERT(hasType(decl->getIdentifier()) == false);
        auto type = m_allocator.construct<TypeDecl>(decl);
        type->m_typeId = m_typeIdCounter++;
        m_types.insert(std::make_pair(type->getIdentifier().getAsString(), type));
        decl->setDeclType(type);
        return type;
    }
}
