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
#include "yal/ast/refbase.h"

namespace yal {

    class RefType : public RefBase
    {
    public:
        RefType(Module& module,
                const AstType astType,
                const Qualifier qualifier);

        Qualifier getQualifier() const {
            return m_qualifier;
        }

        void setQualifier(const Qualifier qualifier);

    private:
        Qualifier m_qualifier;
    };


    class RefTypeUnresolved : public RefType {
    public:

        RefTypeUnresolved(Module& module,
                          const Qualifier qualifier,
                          const StringRef& typeName);

        RefTypeUnresolved(Module& module,
                          const StringRef& typeName);

        StringRef getTypeName() const {
            return m_typeName;
        }

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        StringRef m_typeName;
    };


    class RefTypeResolved : public RefType {
    public:

        RefTypeResolved(Module& module,
                        const Qualifier qualifier,
                        const Type* resolvedType);

        RefTypeResolved(Module& module,
                        const Type* resolvedType);

        const Type* getResolvedType() const {
            return m_resolvedType;
        }

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        const Type* m_resolvedType;
    };
}
