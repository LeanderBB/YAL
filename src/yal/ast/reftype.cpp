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

namespace yal {

    RefType::RefType(Module& module,
                     const AstType astType,
                     const Qualifier qualifier):
        RefBase(module, astType),
        m_qualifier(qualifier) {

    }

    void
    RefType::setQualifier(const Qualifier qualifier) {
        m_qualifier = qualifier;
    }

    RefTypeUnresolved::RefTypeUnresolved(Module& module,
                                         const Qualifier qualifier,
                                         const StringRef& typeName):
        RefType(module, AstType::RefTypeUnresolved, qualifier),
        m_typeName(typeName) {
    }


    RefTypeUnresolved::RefTypeUnresolved(Module& module,
                                         const StringRef& typeName):
        RefTypeUnresolved(module, Qualifier(), typeName) {

    }

    void
    RefTypeUnresolved::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

    RefTypeResolved::RefTypeResolved(Module& module,
                                     const Qualifier qualifier,
                                     const Type* resolvedType):
        RefType(module, AstType::RefTypeResolved, qualifier),
        m_resolvedType(resolvedType) {

    }

    RefTypeResolved::RefTypeResolved(Module& module,
                                     const Type* resolvedType):
        RefTypeResolved(module, Qualifier(), resolvedType){

    }

    void
    RefTypeResolved::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
