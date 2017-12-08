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


    TypeContextErrorHandler::TypeContextErrorHandler(Log& log,
                                                     const SourceManager &manager):
        m_log(log),
        m_srcManager(manager){

    }

    void
    TypeContextErrorHandler::onDuplicate(const DeclBase* newType,
                                         const Type* existingType) {
        PrettyPrint::SourceErrorPrint(m_log, newType->getSourceInfo(), m_srcManager);
        m_log.error("Can not declare '%', type has already been delcared as:\n");
        switch(existingType->getKind()) {
        case Type::Kind::TypeDecl :{
            const TypeDecl* declType = static_cast<const TypeDecl*>(existingType);
            PrettyPrint::SourceErrorPrint(m_log,
                                          declType->getDecl()->getSourceInfo(),
                                          m_srcManager);
            break;
        }
        case Type::Kind::TypeBuiltin:
            m_log.error("builtin type '%'\n", existingType->getIdentifier().getAsString());
            break;
        default:
            YAL_ASSERT_MESSAGE(false, "Should not be reached");
        }
    }

    void
    TypeContextErrorHandler::onUnresolvedReturnType(const DeclFunction* function){
        PrettyPrint::SourceErrorPrint(m_log,
                                      function->getReturnType()->getSourceInfo(),
                                      m_srcManager);
        m_log.error("Function '%' has unresovled return type '%'\n",
                    function->getIdentifier().getAsString(),
                    function->getReturnType()->getIdentitfier().getAsString());
    }

    void
    TypeContextErrorHandler::onUnresolvedParamType(const DeclFunction* function,
                                                   const DeclParamVar* paramDecl) {
        PrettyPrint::SourceErrorPrint(m_log,
                                      paramDecl->getSourceInfo(),
                                      m_srcManager);
        m_log.error("Function '%' has unresovled param type '%'\n",
                    function->getIdentifier().getAsString(),
                    paramDecl->getVarType()->getIdentitfier().getAsString());
    }

    void
    TypeContextErrorHandler::onUnresolvedReturnType(const DeclTypeFunction* function) {
        m_log.error("Type Function '%' has unresovled return type '%'\n",
                    function->getIdentifier().getAsString(),
                    function->getReturnType()->getIdentitfier().getAsString());
    }

    void
    TypeContextErrorHandler::onUnresolvedParamType(const DeclTypeFunction* function,
                                                   const DeclParamVar* paramDecl) {
        PrettyPrint::SourceErrorPrint(m_log,
                                      paramDecl->getSourceInfo(),
                                      m_srcManager);
        m_log.error("Type Function '%' has unresovled param type '%'\n",
                    function->getIdentifier().getAsString(),
                    paramDecl->getVarType()->getIdentitfier().getAsString());
    }

    void
    TypeContextErrorHandler::onUnresolvedTargetType(const DeclTypeFunction* typeFunction) {
        PrettyPrint::SourceErrorPrint(m_log,
                                      typeFunction->getTargetType()->getSourceInfo(),
                                      m_srcManager);
        m_log.error("Type Function '%' has unresovled target type '%'\n",
                    typeFunction->getIdentifier().getAsString(),
                    typeFunction->getTargetType()->getIdentitfier().getAsString());
    }

    void
    TypeContextErrorHandler::onUntargetableType(const DeclTypeFunction* typeFunction) {
        PrettyPrint::SourceErrorPrint(m_log,
                                      typeFunction->getTargetType()->getSourceInfo(),
                                      m_srcManager);
        m_log.error("Type '%' can not accept type functions\n",
                    typeFunction->getTargetType()->getIdentitfier().getAsString());
    }


    void
    TypeContextErrorHandler::onUnresolvedMemberType(const DeclStruct* decl,
                                                    const DeclVar* declVar) {
        m_log.error("Struct '%' has unresovled member type '%' for variable '%'\n",
                    decl->getIdentifier().getAsString(),
                    declVar->getVarType()->getIdentitfier().getAsString(),
                    declVar->getIdentifier().getAsString());
    }


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

    const Type*
    TypeContext::addType(DeclFunction* decl,
                         TypeContextErrorHandler *errHandler) {
        const Type* existingType = getByIdentifier(decl->getIdentifier());

        if (existingType != nullptr ) {
            if (errHandler != nullptr) {
                errHandler->onDuplicate(decl, existingType);
            }
            return nullptr;
        }

        // check if all function params are defined
        DeclParamVarContainer* fnParams = decl->getParams();
        for (auto it = fnParams->childBegin(); it != fnParams->childEnd(); ++it) {
            DeclParamVar* declparam = *it;
            const Type* paramType = resolve(declparam->getVarType());
            if (paramType == nullptr) {
                if (errHandler != nullptr) {
                    errHandler->onUnresolvedParamType(decl, declparam);
                }
                return nullptr;
            }
        }

        // check if return type is defined
        RefType* returnRefType = decl->getReturnType();
        if (returnRefType != nullptr) {
            const Type* returnType = resolve(returnRefType);
            if (returnType == nullptr) {
                if (errHandler != nullptr) {
                    errHandler->onUnresolvedReturnType(decl);
                }
                return nullptr;
            }
        }

        auto type = m_allocator.construct<TypeDecl>(decl);
        m_types.insert(std::make_pair(type->getIdentifier().getAsString(), type));
        return type;
    }

    const Type*
    TypeContext::addType(DeclTypeFunction* decl,
                         TypeContextErrorHandler *errHandler) {
        const Type* existingType = getByIdentifier(decl->getIdentifier());

        if (existingType != nullptr) {
            if (errHandler != nullptr) {
                errHandler->onDuplicate(decl, existingType);
            }
            return nullptr;
        }

        // check if target type exists

        const Type* targetType = resolve(decl->getTargetType());
        if (targetType == nullptr) {
            if (errHandler != nullptr) {
                errHandler->onUnresolvedTargetType(decl);
            }
            return nullptr;
        }

        if (!targetType->isFunctionTargetable()) {
            if (errHandler != nullptr) {
                errHandler->onUnresolvedTargetType(decl);
            }
            return nullptr;
        }

        // check if all function params are defined
        DeclParamVarContainer* fnParams = decl->getParams();

        auto it = fnParams->childBegin();

        // Fix self type
        if (fnParams->getCount() != 0) {
            DeclParamVarSelf* selfParam = dyn_cast<DeclParamVarSelf>(*it);
            if (selfParam != nullptr) {
                selfParam->getVarType()->setResolvedType(targetType);
            }
            ++it;
        }

        for (; it != fnParams->childEnd(); ++it) {
            DeclParamVar* declparam = *it;
            const Type* paramType = resolve(declparam->getVarType());
            if (paramType == nullptr) {
                if (errHandler != nullptr) {
                    errHandler->onUnresolvedParamType(decl, declparam);
                }
                return nullptr;
            }
        }

        // check if return type is defined
        RefType* returnRefType = decl->getReturnType();
        if (returnRefType != nullptr) {
            const Type* returnType = resolve(returnRefType);
            if (returnType == nullptr) {
                if (errHandler != nullptr) {
                    errHandler->onUnresolvedReturnType(decl);
                }
                return nullptr;
            }
        }

        auto type = m_allocator.construct<TypeDecl>(decl);
        m_types.insert(std::make_pair(type->getIdentifier().getAsString(), type));
        Type* mutTargetType = const_cast<Type*>(targetType);
        mutTargetType->addFunction(type);
        return type;
    }

    const Type*
    TypeContext::addType(DeclStruct* decl,
                         TypeContextErrorHandler *errHandler) {

        const Type* existingType = getByIdentifier(decl->getIdentifier());

        if (existingType != nullptr) {
            if (errHandler != nullptr) {
                errHandler->onDuplicate(decl, existingType);
            }
            return nullptr;
        }

        DeclStructMembers* members = decl->getMembers();

        for (auto it = members->childBegin(); it != members->childEnd(); ++it) {
            DeclVar*  memberDecl = *it;
            const Type* varType = resolve(memberDecl->getVarType());
            if (varType == nullptr) {
                if (errHandler != nullptr) {
                    errHandler->onUnresolvedMemberType(decl, memberDecl);
                }
                return nullptr;
            }
        }

        auto type = m_allocator.construct<TypeDecl>(decl);
        m_types.insert(std::make_pair(type->getIdentifier().getAsString(), type));
        return type;
    }

    const Type *TypeContext::resolve(RefType *typeRef) const {
        if (!typeRef->isResolved()) {
            const Type* resolvedType = getByIdentifier(typeRef->getIdentitfier());
            if (resolvedType != nullptr) {
                typeRef->setResolvedType(resolvedType);
            }
            return resolvedType;
        } else {
            return typeRef->getResolvedType();
        }
    }
}
