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

#include "yal/frontend/ast/declbase.h"
#include "yal/frontend/module.h"
#include "yal/frontend/ast/astcontext.h"

namespace yal::frontend {

    DeclBase::DeclBase(Module &module,
                       const AstType type,
                       const Identifier& identifier,
                       const SourceInfo& sourceInfo,
                       DeclScope *scope):
        m_module(module),
        m_scopeWhereDecl(scope),
        m_astType(type),
        m_sourceInfo(sourceInfo),
        m_identifier(identifier){

    }

    DeclBase::DeclBase(Module& module,
                       const AstType type,
                       const Identifier &identifier):
        m_module(module),
        m_scopeWhereDecl(nullptr),
        m_astType(type),
        m_sourceInfo(),
        m_identifier(identifier) {

    }

    DeclBase::~DeclBase() {

    }

    bool
    DeclBase::isVariableDecl() const {
        return m_astType == AstType::DeclVar
                || m_astType == AstType::DeclParamVar
                || m_astType == AstType::DeclParamVarSelf;
    }


    void
    DeclBase::setScopeWhereDeclared(DeclScope* scope) {
        m_scopeWhereDecl = scope;
    }
}
