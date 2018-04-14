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

#include "yal/ast/structmemberinit.h"
#include "yal/ast/astvisitor.h"

namespace yal {

    StructMemberInit::StructMemberInit(frontend::Module &module,
                                       const StringRef memberName,
                                       StmtExpression *initExpression):
    m_module(module),
    m_sourceInfo(),
    m_memberName(memberName),
    m_initExpr(initExpression){

    }

    void
    StructMemberInit::setSourceInfo(const SourceInfo& sourceInfo) {
        m_sourceInfo = sourceInfo;
    }

    void
    StructMemberInit::setInitExpr(StmtExpression *initExpr) {
        m_initExpr = initExpr;
    }

    void
    StructMemberInit::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

    StructMemberInitList::StructMemberInitList(frontend::Module& module):
    BaseType(module) {

    }

    void
    StructMemberInitList::addStructMemberInit(StructMemberInit* memberInit) {
        m_nodes.push_back(memberInit);
    }

    void
    StructMemberInitList::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

}
