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
#include "yal/compiler/stages/stagedecls.h"
#include "yal/compiler/compiler.h"
#include "yal/parser/parser.h"
#include "yal/lexer/lexer.h"
#include "yal/ast/module.h"
#include "yal/io/sourcemanager.h"
#include "yal/ast/typecontext.h"
#include "yal/util/prettyprint.h"
#include "yal/io/memorystream.h"
#include "yal/util/log.h"
#include "yal/ast/typedecl.h"
#include "yal/ast/declscope.h"
#include "yal/ast/declmodule.h"
#include "yal/util/cast.h"
#include "yal/ast/declfunction.h"
#include "yal/ast/decltypefunction.h"
#include "yal/ast/declstruct.h"
#include "yal/ast/declvar.h"
#include "yal/ast/declparamvar.h"
#include "yal/ast/reftype.h"
#include "yal/ast/type.h"
namespace yal {


    StageDecls::StageDecls(Compiler& compiler,
                           SourceItem &sourceItem,
                           Module& module):
        m_compiler(compiler),
        m_sourceItem(sourceItem),
        m_module(module),
        m_activeScope(nullptr){

    }

    bool
    StageDecls::execute() {
        m_activeScope = m_module.getDeclNode()->getDeclScope();
        yal::Lexer lexer (m_sourceItem.getByteStream());
        yal::Parser parser(lexer, m_compiler.getLog(), m_module, *this);

        const yal::Parser::Result parseResult = parser.run();
        if (parseResult == yal::Parser::Result::Ok) {
            YAL_ASSERT(m_activeScope->getKind() == DeclScope::Kind::Module);
            return true;
        }
        return false;
    }


    Type*
    StageDecls::resolveType(const TokenInfo &ti) {
        Identifier identifier(ti.tokenStr, m_module);
        TypeContext& typeCtx = m_module.getTypeContext();
        Type* result = typeCtx.getByIdentifier(identifier);

        if (result == nullptr) {
            PrettyPrint::SourceErrorPrint(m_sourceItem,
                                          m_compiler.getLog(),
                                          ti.lineStart,
                                          ti.lineEnd,
                                          ti.columnStart,
                                          ti.columnEnd);
            m_compiler.getLog().error("Failed to resolve type with identifier '%'\n",
                                      identifier.getAsString());
        } else {
            // Check if decl is actually in scope
            const TypeDecl* typeDecl = dyn_cast<TypeDecl>(result);
            if (typeDecl != nullptr && !m_activeScope->hasDecl(typeDecl->getDecl(), false)) {
                PrettyPrint::SourceErrorPrint(m_sourceItem,
                                              m_compiler.getLog(),
                                              ti.lineStart,
                                              ti.lineEnd,
                                              ti.columnStart,
                                              ti.columnEnd);
                m_compiler.getLog().error("Type '%' is not accessible in this scope\n",
                                          identifier.getAsString());
                return nullptr;
            }
        }
        return result;
    }

    void
    StageDecls::pushDeclScope(DeclScope* scope){
        YAL_ASSERT(m_activeScope!= nullptr);
        scope->setParentScope(m_activeScope);
        m_activeScope = scope;
    }

    void
    StageDecls::popDeclScope() {
        YAL_ASSERT(m_activeScope!= nullptr);
        YAL_ASSERT(m_activeScope->getParentScope() != nullptr);
        m_activeScope = m_activeScope->getParentScope();
    }


    bool
    StageDecls::onDecl(DeclFunction* decl) {
        const DeclBase* oldDecl = m_activeScope->getDecl(decl->getIdentifier(), false);
        if (oldDecl != nullptr) {
            onDuplicateDecl(oldDecl, decl);
            return false;
        }
        m_activeScope->addDecl(decl);
        pushDeclScope(decl->getDeclScope());
        m_module.getTypeContext().addType(decl);
        return true;
    }

    bool
    StageDecls::onDecl(DeclTypeFunction* decl) {
        const DeclBase* oldDecl = m_activeScope->getDecl(decl->getIdentifier(), false);
        if (oldDecl != nullptr) {
            onDuplicateDecl(oldDecl, decl);
            return false;
        }

        const RefType* reftype =decl->getTargetType();
        const Type* targetType = reftype->getType();

        if (!targetType->isFunctionTargetable()) {
            Log& log = m_compiler.getLog();
            PrettyPrint::SourceErrorPrint(log,
                                          reftype->getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Type '%' does not support type functions\n",
                      targetType->getIdentifier().getAsString());
            return false;
        }

        m_activeScope->addDecl(decl);
        pushDeclScope(decl->getDeclScope());
        m_module.getTypeContext().addType(decl);
        return true;
    }

    bool
    StageDecls::onDecl(DeclStruct* decl) {
        const DeclBase* oldDecl = m_activeScope->getDecl(decl->getIdentifier(),false);
        if (oldDecl != nullptr) {
            onDuplicateDecl(oldDecl, decl);
            return false;
        }
        m_activeScope->addDecl(decl);
        pushDeclScope(decl->getDeclScope());
        m_module.getTypeContext().addType(decl);
        return true;
    }

    bool
    StageDecls::onDecl(DeclVar* decl) {
        const DeclBase* oldDecl = m_activeScope->getDecl(decl->getIdentifier(), true);
        if (oldDecl != nullptr) {
            onDuplicateDecl(oldDecl, decl);
            return false;
        }
        m_activeScope->addDecl(decl);
        return true;
    }

    bool
    StageDecls::onDecl(DeclParamVar* decl) {
        const DeclBase* oldDecl = m_activeScope->getDecl(decl->getIdentifier(), true);
        if (oldDecl != nullptr) {
            onDuplicateDecl(oldDecl, decl);
            return false;
        }
        m_activeScope->addDecl(decl);
        return true;
    }

    void
    StageDecls::onDuplicateDecl(const DeclBase* oldDecl,
                                const DeclBase* newDecl) {
        Log& log = m_compiler.getLog();
        PrettyPrint::SourceErrorPrint(log,
                                      newDecl->getSourceInfo(),
                                      m_compiler.getSourceManager());
        log.error("Can not declare '%', existing decl arleady exists here:\n",
                  newDecl->getName());
        PrettyPrint::SourceErrorPrint(log,
                                      oldDecl->getSourceInfo(),
                                      m_compiler.getSourceManager());
    }
}
