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

#include "yal/compiler/cpasstyperegistration.h"
#include "yal/ast/module.h"
#include "yal/ast/astcontext.h"
#include "yal/util/log.h"
#include "yal/ast/declbase.h"
#include "yal/ast/astvisitor.h"
#include "yal/ast/decltypefunction.h"
#include "yal/ast/declfunction.h"
#include "yal/ast/declstruct.h"
#include "yal/ast/declvar.h"
#include "yal/ast/declmodule.h"
#include "yal/ast/typecontext.h"
namespace yal {

    class TypeRegistrationVisitor : public AstVisitor {
    public:

        TypeRegistrationVisitor(Log& log,
                                Module& module):
            m_log(log),
            m_module(module),
            m_errorFound(false) {

        }

        void visit(DeclStruct& node) override final;
        void visit(DeclTypeFunction& node) override final;
        void visit(DeclFunction& node) override final;
        void visit(DeclVar& node) override final;

    public:
        Log& m_log;
        Module& m_module;
        bool m_errorFound;
    };


    bool
    CPassTypeRegistration::run(Log& log, Module& module) {
        DeclModule* rootNode = module.getRootAstNode();

        TypeRegistrationVisitor typeRegVisitor(log, module);

        for(auto& decl : rootNode->getDeclarations()) {
            decl->acceptVisitor(typeRegVisitor);
            if (typeRegVisitor.m_errorFound) {
                break;
            }
        }
        return !typeRegVisitor.m_errorFound;
    }


    void
    TypeRegistrationVisitor::visit(DeclStruct& node) {
        m_log.message("DeclSturct %\n", node.getName());
    }

    void
    TypeRegistrationVisitor::visit(DeclTypeFunction& node) {
        m_log.message("DeclTypeFunction %\n", node.getName());
    }

    void
    TypeRegistrationVisitor::visit(DeclFunction& node) {
        m_log.message("DeclFunction %\n", node.getName());
        TypeContext& typectx = m_module.getTypeContext();

        const Type* existingType = typectx.getByIdentifier(node.getIdentifier());

        if (existingType == nullptr) {
            // check function param decls
            // see if any are undefined
            // create new function type and
            // add it to the context
        } else {
            m_log.error("Function % can not be declared, an existing type \
already exists with the same identifier\n", node.getIdentifier().getAsString());
            m_errorFound = true;
        }

    }

    void
    TypeRegistrationVisitor::visit(DeclVar&) {
        m_log.error("Global variable declarations not yet supported!\n");
        m_errorFound = true;
    }
}
