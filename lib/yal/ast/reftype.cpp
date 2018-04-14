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
#include "yal/ast/reftype.h"
#include "yal/ast/astvisitor.h"
#include "yal/ast/astcontext.h"
#include "yal/frontend/module.h"
namespace yal {

    RefType::RefType(frontend::Module& module,
                     const Type *resolvedType):
        RefType(module, Qualifier(), resolvedType) {
    }

    RefType::RefType(frontend::Module& module,
                     const Qualifier qualifier,
                     const Type *resolvedType):
        m_module(module),
        m_sourceInfo(),
        m_qualifier(qualifier),
        m_identifier(),
        m_resolvedType(resolvedType) {

    }

    RefType::~RefType() {

    }

    void
    RefType::setSourceInfo(const SourceInfo& sourceInfo) {
        m_sourceInfo = sourceInfo;
    }

    void
    RefType::setQualifier(const Qualifier qualifier) {
        m_qualifier = qualifier;
    }

    Identifier
    RefType::getIdentitfier() const {
        return m_resolvedType->getIdentifier();
    }

    void
    RefType::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
