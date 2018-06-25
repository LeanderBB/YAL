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

#include "yal/frontend/ast/declfunction.h"
#include "yal/frontend/module.h"
#include "yal/frontend/ast/astvisitor.h"
#include "yal/frontend/ast/declparamvar.h"
#include "yal/frontend/parser/stdeclfunction.h"
#include "yal/frontend/types/typefunction.h"

namespace yal::frontend {

    DeclFunction::DeclFunction(Module& module,
                               const AstType type,
                               TypeFunction* fnType,
                               DeclScope& scope,
                               const QualType& returnType,
                               Params&& params,
                               Body&& body):
        DeclNamed(module,
                  type,
                  fnType->getIdentifier(),
                  fnType->getSTDecl()->getSourceInfo(),
                  scope),
        m_fnType(fnType),
        m_returnType(returnType),
        m_fnDeclScope(this, &scope),
        m_params(std::move(params)),
        m_functionBody(std::move(body)) {

    }

    DeclFunction::DeclFunction(Module& module,
                               const AstType type,
                               TypeFunction* fnType,
                               DeclScope& scope,
                               const QualType& returnType):
        DeclNamed(module,
                  type,
                  fnType->getIdentifier(),
                  fnType->getSTDecl()->getSourceInfo(),
                  scope),
        m_fnType(fnType),
        m_returnType(returnType),
        m_fnDeclScope(this, &scope),
        m_params(Params::allocator_type(module.getASTContext().getAllocator())),
        m_functionBody(Body::allocator_type(module.getASTContext().getAllocator())) {

    }


    DeclFunction::DeclFunction(Module& module,
                               TypeFunction* fnType,
                               DeclScope& scope,
                               const QualType& returnType,
                               Params&& params,
                               Body&& body):
        DeclFunction(module,
                     AstType::DeclFunction,
                     fnType,
                     scope,
                     returnType,
                     std::move(params),
                     std::move(body)) {
    }

    DeclFunction::DeclFunction(Module& module,
                               TypeFunction* fnType,
                               DeclScope& scope,
                               const QualType& returnType):
        DeclFunction(module,
                     AstType::DeclFunction,
                     fnType,
                     scope,
                     returnType) {
    }

    void
    DeclFunction::setParams(Params&& params) {
#if defined(YAL_ASSERT_ENABLED)
        for(auto& delc : params) {
            YAL_ASSERT(delc->getScopeWhereDeclared() == &m_fnDeclScope);
        }
#endif
        m_params = std::move(params);
    }

    void
    DeclFunction::setBody(Body&& body) {
        m_functionBody = std::move(body);
    }

    void
    DeclFunction::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }


}
