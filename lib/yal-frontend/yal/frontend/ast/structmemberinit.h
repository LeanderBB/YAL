/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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
#include "yal/frontend/types/type.h"
#include "yal/io/sourcemanager.h"
#include "yal/util/cast.h"
#include "yal/util/stringref.h"

namespace yal::frontend {

    class StmtExpression;

    class StructMemberInit
    {
    public:
        StructMemberInit(Module& module,
                         const SourceInfo& srcInfo,
                         const StringRef memberName,
                         StmtExpression* initExpression);

        YAL_NO_COPY_CLASS(StructMemberInit);

        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        Module& getModule() {
            return m_module;
        }

        const Module& getModule() const {
            return m_module;
        }

        StringRef getMemberName() const {
            return m_memberName;
        }

        StmtExpression* getInitExpr() const {
            return m_initExpr;
        }

        void setInitExpr(StmtExpression* initExpr);

    private:
        Module& m_module;
        SourceInfo m_sourceInfo;
        StringRef m_memberName;
        StmtExpression* m_initExpr;

    };
}
