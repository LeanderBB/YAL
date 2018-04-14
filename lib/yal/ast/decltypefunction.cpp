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
#include "yal/ast/decltypefunction.h"
#include "yal/frontend/module.h"
#include "yal/ast/astvisitor.h"
#include "yal/ast/declparamvar.h"
#include "yal/ast/reftype.h"
namespace yal {

    DeclTypeFunction::DeclTypeFunction(frontend::Module& module,
                                       const StringRef functionName,
                                       const DeclScope *scope,
                                       DeclParamVarContainer* params,
                                       RefType *returnType,
                                       RefType *destType,
                                       StatementList* body):
        DeclFunctionBase(module,
                         AstType::DeclTypeFunction,
                         functionName,
                         scope,
                         params,
                         returnType,
                         body),
        m_functionName(destType->getIdentitfier().getAsString().toString()
                       + "::"
                       + functionName.toString()),
        m_targetType(destType),
        m_fnDeclScope(this, scope){
        m_identifier.setIdString(m_functionName);
    }

    DeclTypeFunction::DeclTypeFunction(frontend::Module& module,
                                       const StringRef functionName,
                                       const DeclScope *scope,
                                       RefType* destType):
        DeclTypeFunction(module, functionName, scope,
                         nullptr, nullptr, destType, nullptr) {

    }


    bool
    DeclTypeFunction::isStatic() const{
        if (m_params != nullptr) {
            return dyn_cast<DeclParamVarSelf>(*m_params->childBegin()) == nullptr;
        }
        return true;
    }

    bool DeclTypeFunction::isImmutable() const {
        if (m_params != nullptr) {
            const DeclParamVarSelf* selfVar =dyn_cast<DeclParamVarSelf>(*m_params->childBegin());
            if (selfVar != nullptr) {
                return selfVar->getQualType().getQualifier().isImmutable();
            }
        }
        return true;
    }

    void
    DeclTypeFunction::acceptVisitor(AstVisitor &visitor) {
        visitor.visit(*this);
    }
}
