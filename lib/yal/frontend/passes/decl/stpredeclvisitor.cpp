/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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
#include "yal/frontend/parser/stdeclfunction.h"
#include "yal/frontend/parser/stdeclmodule.h"
#include "yal/frontend/parser/stdeclstruct.h"
#include "yal/frontend/passes/decl/errorspassdecl.h"
#include "yal/frontend/types/typecontext.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/types/typestruct.h"

namespace yal::frontend {

    STPreDeclVisitor::STPreDeclVisitor(ErrorReporter &errReporter,
                                       Module &module):
        m_errReporter(errReporter),
        m_module(module) {

    }

    void
    STPreDeclVisitor::visit(const STDeclFunction& declFunction) {
        TypeContext& typeCtx = m_module.getTypeContext();

        Type* target = nullptr;
        // check if target type has beend declared
        const STType* targetType = declFunction.getFunctionTarget();
        if (targetType != nullptr ) {
            const Identifier targetId(targetType->getIdentifier(),m_module);
            target = typeCtx.getByIdentifier(targetId);

            if (target == nullptr) {
                auto errorPtr = std::make_unique<ErrorUndefinedTypeRef>(targetId.getName(),
                                                                        targetType->getSourceInfo());
                m_errReporter.report(std::move(errorPtr));
                return;
            }

            if (!target->isFunctionTargetable()) {
                auto errorPtr = std::make_unique<ErrorFnOnNonTargetType>(declFunction.getFunctionName()->getString(),
                                                                         declFunction.getFunctionName()->getSourceInfo(),
                                                                         target);
                m_errReporter.report(std::move(errorPtr));
                return;
            }
        }

        // check if function has already been declared
        TypeFunction* typeFn = typeCtx.allocateType<TypeFunction>(m_module, &declFunction);
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
            target->addFunction(typeFn);
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
            decl->acceptVisitor(*this);
        }
    }
}
