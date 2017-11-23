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

#include "yal/ast/declvar.h"
#include "yal/ast/astvisitor.h"
namespace yal {

    DeclVar::DeclVar(Module& module,
                     StringRef name,
                     const Qualifier qualifier,
                     const RefType* varType,
                     StmtExpression *expr):
        DeclBase(module, AstType::DeclVar, name),
        m_varType(varType),
        m_qualifier(qualifier),
        m_expression(expr){

    }


    DeclVar::DeclVar(Module& module,
                     const AstType type,
                     const Qualifier qualifier,
                     StringRef name,
                     const RefType* varType) :
        DeclBase(module, type, name),
        m_varType(varType),
        m_qualifier(qualifier),
        m_expression(nullptr){

    }

    void
    DeclVar::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

}
