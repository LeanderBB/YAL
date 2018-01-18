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
#include "yal/ast/type.h"
#include "yal/ast/reftype.h"
namespace yal {

    DeclVar::DeclVar(Module& module,
                     StringRef name,
                     const Qualifier qualifier,
                     RefType* varType,
                     StmtExpression *expr):
        DeclBase(module, AstType::DeclVar, name),
        m_varType(varType),
        m_qualType(varType->getQualType()),
        m_expression(expr){

        Qualifier newQual = m_qualType.getQualifier();

        if (qualifier.isMutable()) {
            newQual.setMutable();
        }
        newQual.setLValue(true);
        m_qualType.setQualifier(newQual);
    }


    DeclVar::DeclVar(Module& module,
                     const AstType type,
                     const Qualifier qualifier,
                     StringRef name,
                     RefType *varType) :
        DeclBase(module, type, name),
        m_varType(varType),
        m_qualType(varType->getQualType()),
        m_expression(nullptr){

        Qualifier newQual = m_qualType.getQualifier();

        if (qualifier.isMutable()) {
            newQual.setMutable();
        }
        newQual.setLValue(true);
        m_qualType.setQualifier(newQual);
    }

    void
    DeclVar::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

}
