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

#include "yal/compiler/cpassscopetyperesolve.h"
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
#include "yal/util/prettyprint.h"
namespace yal {

    class TypeResolverVisitor : public AstVisitor {
    public:

        TypeResolverVisitor(Log& log,
                            Module& module,
                            const SourceManager &manager):
            m_log(log),
            m_module(module),
            m_errHandler(log, manager),
            m_errorFound(false) {

        }

        void visit(DeclStruct& node) override final;
        void visit(DeclTypeFunction& node) override final;
        void visit(DeclFunction& node) override final;
        void visit(DeclVar& node) override final;

    public:
        Log& m_log;
        Module& m_module;
        TypeContextErrorHandler m_errHandler;
        bool m_errorFound;
    };


    class ScopeErrorHanlder {
    public:
        ScopeErrorHanlder(Log& log,
                          const SourceManager& srcManager):
            m_log(log),
            m_srcManager(srcManager) {
            (void) m_log;
            (void) m_srcManager;
        }

    private:
        Log& m_log;
        const SourceManager& m_srcManager;
    };


    class ScopeResolverVisitor : public AstVisitor {
    public:
        ScopeResolverVisitor(Log& log,
                             Module& module,
                             const SourceManager& manager): m_log(log),
            m_module(module),
            m_errHandler(log, manager),
            m_errorFound(false) {
        }

        void visit(DeclStruct& node) override final;
        void visit(DeclTypeFunction& node) override final;
        void visit(DeclFunction& node) override final;
        void visit(DeclVar& node) override final;

    public:
        Log& m_log;
        Module& m_module;
        ScopeErrorHanlder m_errHandler;
        bool m_errorFound;
    };

    bool
    CPassScopeTypeResolve::run(Log& log, Module& module,
                               const SourceManager &manager) {
        DeclModule* rootNode = module.getRootAstNode();

        TypeResolverVisitor typeRegVisitor(log, module, manager);
        ScopeResolverVisitor scopeVisitor(log, module, manager);

        for(auto& decl : rootNode->getDeclarations()) {
            decl->acceptVisitor(typeRegVisitor);
            if (typeRegVisitor.m_errorFound) {
                break;
            }
            decl->acceptVisitor(scopeVisitor);
            if (scopeVisitor.m_errorFound) {
                break;
            }
        }
        return !typeRegVisitor.m_errorFound && !scopeVisitor.m_errorFound;
    }


    void
    TypeResolverVisitor::visit(DeclStruct& node) {
        const Type* newType = m_module.getTypeContext().addType(&node, &m_errHandler);
        m_errorFound = newType == nullptr;
    }

    void
    TypeResolverVisitor::visit(DeclTypeFunction& node) {
        const Type* newType = m_module.getTypeContext().addType(&node, &m_errHandler);
        m_errorFound = newType == nullptr;
    }

    void
    TypeResolverVisitor::visit(DeclFunction& node) {
        const Type* newType = m_module.getTypeContext().addType(&node, &m_errHandler);
        m_errorFound = newType == nullptr;
    }

    void
    TypeResolverVisitor::visit(DeclVar&) {
        m_log.error("Global variable declarations not yet supported!\n");
        m_errorFound = true;
    }


    void
    ScopeResolverVisitor::visit(DeclStruct&) {
       // PrettyPrint::SourceErrorPrint(m_log, node.getSourceInfo(), m_srcManager);
    }

    void
    ScopeResolverVisitor::visit(DeclTypeFunction&){

    }

    void
    ScopeResolverVisitor::visit(DeclFunction&){

    }

    void
    ScopeResolverVisitor::visit(DeclVar&) {

    }
}
