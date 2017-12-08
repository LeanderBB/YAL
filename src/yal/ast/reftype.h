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
#include "yal/io/sourcemanager.h"
#include "yal/ast/identifier.h"
#include "yal/ast/type.h"
namespace yal {

    class Module;
    class AstVisitor;

    class RefType {
    public:

        static void* operator new(std::size_t bytes,
                                  Module& ctx);

        RefType(Module& module,
                const StringRef identifier);

        RefType(Module& module,
                const Identifier& identifier);

        RefType(Module& module,
                const Type* resolvedType);

        RefType(Module& module,
                const Qualifier qualifier,
                const Identifier& identifier);

        RefType(Module& module,
                const Qualifier qualifier,
                const Type* resolvedType);

        ~RefType();

        AstType getAstType() const {
            return AstType::RefType;
        }

        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        SourceInfo& getSourceInfo() {
            return const_cast<SourceInfo&>(
                        static_cast<const RefType*>(this)->getSourceInfo());
        }

        void setSourceInfo(const SourceInfo& sourceInfo);


        Qualifier getQualifier() const {
            return m_qualifier;
        }

        void setQualifier(const Qualifier qualifier);

        bool isResolved() const {
            return m_resolvedType != nullptr;
        }

        const Type* getResolvedType() const {
            return m_resolvedType;
        }

        void setResolvedType(const Type* type);

        Identifier getIdentitfier() const;

        void acceptVisitor(AstVisitor& visitor);

    protected:
        Module& m_module;
        SourceInfo m_sourceInfo;
        Qualifier m_qualifier;
        Identifier m_identifier;
        const Type* m_resolvedType;
    };

    inline AstType get_typeid(const RefType& ref) {
        return ref.getAstType();
    }


}
