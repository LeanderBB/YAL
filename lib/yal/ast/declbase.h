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
#pragma once

#include "yal/ast/asttypes.h"
#include "yal/ast/type.h"
#include "yal/io/sourcemanager.h"
#include "yal/util/cast.h"
#include "yal/util/stringref.h"
#include "yal/ast/identifier.h"
namespace yal{

    namespace frontend {
        class Module;
    }
    class AstVisitor;
    class Type;
    class TypeContext;
    class DeclScope;
    class DeclBase
    {
    public:

        DeclBase(frontend::Module& module,
                 const AstType type,
                 const Identifier &identifier,
                 const DeclScope* scope);

        YAL_NO_COPY_CLASS(DeclBase);

        virtual ~DeclBase();


        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        SourceInfo& getSourceInfo() {
            return const_cast<SourceInfo&>(
                        static_cast<const DeclBase*>(this)->getSourceInfo());
        }

        void setSourceInfo(const SourceInfo& sourceInfo);

        AstType getAstType() const {
            return m_astType;
        }

        StringRef getName() const {
            return m_identifier.getName();
        }

        const Identifier& getIdentifier() const {
            return m_identifier;
        }

        frontend::Module& getModule() {
            return m_module;
        }

        const frontend::Module& getModule() const {
            return m_module;
        }

        const DeclScope* getScopeWhereDeclared() const {
            return m_scopeWhereDecl;
        }

        bool isVariableDecl() const;

        virtual void acceptVisitor(AstVisitor& visitor) = 0;

    protected:
        frontend::Module& m_module;
        const DeclScope* m_scopeWhereDecl;
        const AstType m_astType;
        SourceInfo m_sourceInfo;
        Identifier m_identifier;
    };

    template<>
    struct cast_typeid<DeclBase> {
        typedef AstType type;
    };

    inline AstType get_typeid(const DeclBase& decl) {
        return decl.getAstType();
    }

    class DeclTyped {
    public:
        const Type* getDeclType() const {
            return m_declType;
        }

    private:
        friend class TypeContext;
        void setDeclType(const Type* type) {
            m_declType = type;
        }
    private:
        const Type* m_declType = nullptr;
    };

}
