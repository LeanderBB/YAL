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
#include "yal/ast/nodecontainer.h"

namespace yal {

    class StmtExpression;
    class AstVisitor;

    class StructMemberInit
    {
    public:
        StructMemberInit(Module& module,
                         const StringRef memberName,
                         StmtExpression* initExpression);

        YAL_NO_COPY_CLASS(StructMemberInit);

        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        SourceInfo& getSourceInfo() {
            return const_cast<SourceInfo&>(
                        static_cast<const StructMemberInit*>(this)->getSourceInfo());
        }

        void setSourceInfo(const SourceInfo& sourceInfo);

        AstType getAstType() const {
            return AstType::StructMemberInit;
        }

        Module& getModule() {
            return m_module;
        }

        const Module& getModule() const {
            return m_module;
        }

        void acceptVisitor(AstVisitor& visitor);

        StringRef getMemberName() const {
            return m_memberName;
        }

        StmtExpression* getInitExpr() const {
            return m_initExpr;
        }

    private:
        Module& m_module;
        SourceInfo m_sourceInfo;
        StringRef m_memberName;
        StmtExpression* m_initExpr;

    };

    inline AstType get_typeid(const StructMemberInit& node) {
        return node.getAstType();
    }


    class StructMemberInitList :
            public NodeContainer<StructMemberInit*, AstType::StructMemberInitList> {
        using BaseType =  NodeContainer<StructMemberInit*, AstType::StructMemberInitList>;
    public:

        StructMemberInitList(Module& module);

        void addStructMemberInit(StructMemberInit* memberInit);

        void acceptVisitor(AstVisitor& visitor);
    };

    inline AstType get_typeid(const StructMemberInitList& container) {
        return container.getAstType();
    }
}
