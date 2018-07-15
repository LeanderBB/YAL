/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/ast/decltypefunction.h"

#include "yal/frontend/module.h"
#include "yal/frontend/ast/astvisitor.h"
#include "yal/frontend/ast/declparamvar.h"

namespace yal::frontend {

    DeclTypeFunction::DeclTypeFunction(Module& module,
                                       TypeFunction* fnType,
                                       DeclScope &scope,
                                       const QualType& returnType,
                                       const QualType& targetType,
                                       Params&& params,
                                       Body&& body):
        DeclFunction(module,
                     AstType::DeclTypeFunction,
                     fnType,
                     scope,
                     returnType,
                     std::move(params),
                     std::move(body)),
        m_targetType(targetType) {
    }

    DeclTypeFunction::DeclTypeFunction(Module& module,
                     TypeFunction* fnType,
                     DeclScope& scope,
                     const QualType& returnType,
                     const QualType& targetType):
        DeclFunction(module,
                     AstType::DeclTypeFunction,
                     fnType,
                     scope,
                     returnType),
        m_targetType(targetType) {

    }


    bool
    DeclTypeFunction::isStatic() const{
        if (!m_params.empty()) {
            return dyn_cast<DeclParamVarSelf>(m_params.front()) == nullptr;
        }
        return true;
    }

    bool DeclTypeFunction::isImmutable() const {
        if (!m_params.empty()) {
            const DeclParamVarSelf* selfVar =dyn_cast<DeclParamVarSelf>(m_params.front());
            if (selfVar != nullptr) {
                return selfVar->getQualType().getQualifier().isImmutable();
            }
        }
        return true;
    }

}
