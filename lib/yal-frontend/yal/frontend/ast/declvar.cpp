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

#include "yal/frontend/ast/declvar.h"
#include "yal/frontend/ast/astvisitor.h"
#include "yal/frontend/types/type.h"

namespace yal::frontend {

    DeclVar::DeclVar(Module& module,
                     StringRef name,
                     const SourceInfo &sourceInfo,
                     DeclScope& scope,
                     const QualType&varType,
                     StmtExpression *expr):
        DeclVar(module,
                AstType::DeclVar,
                name,
                sourceInfo,
                scope,
                varType,
                expr) {
    }

    DeclVar::DeclVar(Module& module,
                     const AstType type,
                     StringRef name,
                     const SourceInfo &sourceInfo,
                     DeclScope &scope,
                     const QualType &varType,
                     StmtExpression *expr) :
        DeclBase(module, type, Identifier(name), sourceInfo, scope),
        m_qualType(varType),
        m_expression(expr == nullptr ? StmtExpressionOpt() : StmtExpressionOpt(expr)){

        Qualifier newQual = m_qualType.getQualifier();
        newQual.setLValue(true);
        m_qualType.setQualifier(newQual);
    }

    void
    DeclVar::setQualType(const QualType& qt) {
        YAL_ASSERT(qt.isValid());
        m_qualType = qt;
    }

    void
    DeclVar::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

}
