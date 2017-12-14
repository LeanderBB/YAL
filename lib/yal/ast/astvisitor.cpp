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


#include "yal/ast/astvisitor.h"
#include "yal/ast/statementlist.h"
#include "yal/ast/declparamvar.h"
#include "yal/ast/declmodule.h"
#include "yal/ast/statement.h"
#include "yal/ast/declstruct.h"
#include "yal/ast/exprlist.h"
#include "yal/ast/stmtexpression.h"
namespace yal {


    void
    RecursiveAstVisitor::visit(DeclModule& node){
        for (auto& decl : node.getDeclarations()) {
            decl->acceptVisitor(*this);
        }
    }

    void
    RecursiveAstVisitor::visit(StatementList& node){
        for (auto it = node.childBegin(); it != node.childEnd(); ++it) {
            (*it)->acceptVisitor(*this);
        }
    }


    void
    RecursiveAstVisitor::visit(DeclParamVarContainer& node) {
        for (auto it = node.childBegin(); it != node.childEnd(); ++it) {
            (*it)->acceptVisitor(*this);
        }
    }

    void
    RecursiveAstVisitor::visit(DeclStructMembers& node) {
        for (auto it = node.childBegin(); it != node.childEnd(); ++it) {
            (*it)->acceptVisitor(*this);
        }
    }

    void
    RecursiveAstVisitor::visit(ExprList& node) {
        for (auto it = node.childBegin(); it != node.childEnd(); ++it) {
            (*it)->acceptVisitor(*this);
        }
    }
}
