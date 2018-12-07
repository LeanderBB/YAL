/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/passes/decl/stpredeclvisitor.h"

#include "yal/error/errorreporter.h"
#include "yal/frontend/module.h"
#include "yal/frontend/modulemanager.h"
#include "yal/frontend/parser/stdeclfunction.h"
#include "yal/frontend/parser/stdeclmodule.h"
#include "yal/frontend/parser/stdeclstruct.h"
#include "yal/frontend/parser/stdecltypefunctions.h"
#include "yal/frontend/parser/syntaxtreevisitorimpl.h"
#include "yal/frontend/passes/passes.h"
#include "yal/frontend/passes/decl/errorspassdecl.h"
#include "yal/frontend/types/types.h"

namespace yal::frontend {

    class STPreDeclVisitor::ScopeGuard {
    public:
        YAL_NO_COPY_CLASS(ScopeGuard);

        ScopeGuard(STPreDeclVisitor& visitor,
                   const ScopeState& newState):
            m_visitor(visitor),
            m_prevState(visitor.m_state) {
            m_visitor.m_state = newState;
        }

        ~ScopeGuard() {
            m_visitor.m_state = m_prevState;
        }

    private:
        STPreDeclVisitor& m_visitor;
        STPreDeclVisitor::ScopeState m_prevState;
    };

    STPreDeclVisitor::STPreDeclVisitor(PassOptions& options):
        m_errReporter(options.errReporter),
        m_module(options.module),
        m_moduleManager(options.modManager) {

    }

    void
    STPreDeclVisitor::visit(const STDeclFunction& declFunction) {
        TypeContext& typeCtx = m_module.getTypeContext();

        Type* target = m_state.declimplTarget;
        // check if function has already been declared
        TypeFunction* typeFn = typeCtx.allocateType<TypeFunction>(m_module, &declFunction, target);
        const Type* existingType = typeCtx.getByIdentifier(typeFn->getIdentifier());
        if (existingType == nullptr) {
            typeCtx.registerType(typeFn);
        } else {
            // report error
            auto errorPtr = std::make_unique<ErrorDuplicateTypeDecl>(m_module,
                                                                     *typeFn,
                                                                     *existingType);
            m_errReporter.report(std::move(errorPtr));
            return;
        }

        // register function with type
        if (target != nullptr) {

            // check if the function already exists on target
            const TypeFunction* existingFn = target->getFunctionWithName(typeFn->getIdentifier().getName());
            if (existingFn != nullptr) {
                auto errorPtr = std::make_unique<ErrorDuplicateTypeDecl>(m_module,
                                                                         *typeFn,
                                                                         *existingFn);
                m_errReporter.report(std::move(errorPtr));
                return;
            }
            YAL_ASSERT(target->isFunctionTargetable());
            TypeTargetable* typeTargetable = static_cast<TypeTargetable*>(target);
            typeTargetable->addFunction(typeFn);
        }
    }

    void
    STPreDeclVisitor::visit(const STDeclStruct& declStruct) {
        TypeContext& typeCtx = m_module.getTypeContext();

        // check if struct has already been declared
        TypeStruct* typeStruct = typeCtx.allocateType<TypeStruct>(m_module, declStruct);
        const Type* existingType = typeCtx.getByIdentifier(typeStruct->getIdentifier());
        if (existingType == nullptr) {
            typeCtx.registerType(typeStruct);
        } else {
            // report error
            auto errorPtr = std::make_unique<ErrorDuplicateTypeDecl>(m_module,
                                                                     *typeStruct,
                                                                     *existingType);
            m_errReporter.report(std::move(errorPtr));
        }
    }

    void
    STPreDeclVisitor::visit(const STDeclModule& declModule) {
        for(auto& decl: declModule.getDecls()) {
            if (m_errReporter.hasFatalError()) {
                return;
            }
            resolve(*decl);
        }
    }

    void
    STPreDeclVisitor::visit(const STDeclTypeFunctions& declTypeFunctions) {
        const STType& targetType = declTypeFunctions.getType();

        // Resolve type
        Type* target = resolveType(targetType);
        if (target == nullptr) {
            auto errorPtr = std::make_unique<ErrorUndefinedTypeRef>(targetType.getIdentifier().getString(),
                                                                    targetType.getSourceInfo());
            m_errReporter.report(std::move(errorPtr));
            return;
        }

        if (target->isAliasWeak()) {
            auto errorPtr = std::make_unique<ErrorFnImplOnAlias>(*target);
            m_errReporter.report(std::move(errorPtr));
            return;
        }

        if (!target->isFunctionTargetable()) {
            auto errorPtr = std::make_unique<ErrorFnImplOnNonTargetType>(targetType.getSourceInfo(),
                                                                         target);
            m_errReporter.report(std::move(errorPtr));
            return;
        }

        // set new state
        ScopeState newState;
        newState.declimplTarget = target;
        ScopeGuard guard(*this, newState);

        for (auto& decl : declTypeFunctions.getDecls()) {
            resolve(*decl);
        }
    }

    void
    STPreDeclVisitor::visit(const STDeclAlias& node) {
        TypeContext& typeCtx = m_module.getTypeContext();
        const STType& aliasedType = node.getAliasedType();

        // see if alias type has been declared
        Type* resolvedType = resolveType(aliasedType);
        if (resolvedType == nullptr) {
            auto errorPtr = std::make_unique<ErrorUndefinedTypeRef>(aliasedType.getIdentifier().getString(),
                                                                    aliasedType.getSourceInfo());
            m_errReporter.report(std::move(errorPtr));
            return;
        }

        if (resolvedType->isFunction()
                || resolvedType->isTypeFunction()
                || resolvedType->isTypeFunctionStatic()) {
            auto errorPtr = std::make_unique<ErrorAliasOfFunction>(node);
            m_errReporter.report(std::move(errorPtr));
            return;
        }

        // Create new type
        Type* typeAlias = nullptr;
        if (!node.isStrong()) {
            typeAlias = typeCtx.allocateType<TypeAliasWeak>(m_module,
                                                            node,
                                                            *resolvedType);
        } else {
            typeAlias = typeCtx.allocateType<TypeAliasStrong>(m_module,
                                                              node,
                                                              *resolvedType);
        }
        // check for duplicate types
        const Type* existingType = typeCtx.getByIdentifier(typeAlias->getIdentifier());
        if (existingType == nullptr) {
            typeCtx.registerType(typeAlias);
        } else {
            // report error
            auto errorPtr = std::make_unique<ErrorDuplicateTypeDecl>(m_module,
                                                                     *typeAlias,
                                                                     *existingType);
            m_errReporter.report(std::move(errorPtr));
            return;
        }
    }

    void
    STPreDeclVisitor::visit(const STDeclImport& node) {
        const StringRef importedModule = node.getModuleIdentifier();
        const Module* module = m_moduleManager.getModuleByName(importedModule);
        if (module == nullptr) {
            auto errorPtr = std::make_unique<ErrorImportNotFound>(node);
            m_errReporter.report(std::move(errorPtr));
            return;
        }
        if (!m_module.import(*module)) {
            YAL_ASSERT_MESSAGE(false, "Failed to import?");
        }
    }

    void
    STPreDeclVisitor::visit(const STDeclVar&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STStmtReturn&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STStmtDecl&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STStmtExpression&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STStmtAssign&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STExprVarRef&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STExprUnaryOperator&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STExprBinaryOperator&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STExprBoolLiteral&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STExprIntegerLiteral&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STExprFloatLiteral&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STExprStructVarRef&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STExprFnCall&) {
    }

    void
    STPreDeclVisitor::visit(const STExprCast&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STExprStructInit&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    void
    STPreDeclVisitor::visit(const STStmtListScoped&) {
        YAL_ASSERT_NOT_IMPLEMENTED();
    }

    Type*
    STPreDeclVisitor::resolveType(const STType& stType) {
        TypeContext& typeCtx = m_module.getTypeContext();
        return typeCtx.resolveType(stType);
    }
}
