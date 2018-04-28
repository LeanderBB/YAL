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

#include "yal/frontend/ast/asttypes.h"
#include "yal/io/sourcemanager.h"
#include "yal/util/allocator/allocatorstack.h"
#include "yal/util/cast.h"

namespace yal::frontend {

    class Module;
    class AstVisitor;

    class Statement {
    public:

        Statement(Module& module,
                  const AstType type,
                  const SourceInfo& srcInfo);

        virtual ~Statement();

        YAL_NO_COPY_CLASS(Statement);

        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        AstType getAstType() const {
            return m_astType;
        }

        virtual void acceptVisitor(AstVisitor& visitor) = 0;

    protected:
        Module& m_module;
        const AstType m_astType;
        SourceInfo m_sourceInfo;
    };

    using StatementList = std::vector<Statement*, StdAllocatorWrapperStack<Statement*>>;
}

namespace yal {

    template<>
    struct cast_typeid<yal::frontend::Statement> {
        typedef yal::frontend::AstType type;
    };

    inline yal::frontend::AstType get_typeid(const yal::frontend::Statement& ref) {
        return ref.getAstType();
    }
}
