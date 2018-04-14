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
#include "yal/util/cast.h"

namespace yal {

    namespace frontend {
    class Module;
    }
    class AstVisitor;

    class Statement {
    public:

        Statement(frontend::Module& module,
                  const AstType type);

        virtual ~Statement();

        YAL_NO_COPY_CLASS(Statement);

        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        SourceInfo& getSourceInfo() {
            return const_cast<SourceInfo&>(
                        static_cast<const Statement*>(this)->getSourceInfo());
        }

        void setSourceInfo(const SourceInfo& sourceInfo);

        AstType getAstType() const {
            return m_astType;
        }

        virtual void acceptVisitor(AstVisitor& visitor) = 0;

    protected:
        frontend::Module& m_module;
        const AstType m_astType;
        SourceInfo m_sourceInfo;
    };

    template<>
    struct cast_typeid<Statement> {
        typedef AstType type;
    };

    inline AstType get_typeid(const Statement& ref) {
        return ref.getAstType();
    }
}
