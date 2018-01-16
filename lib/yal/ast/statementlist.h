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

#include "yal/ast/nodecontainer.h"

namespace  yal {

    class Statement;
    class AstVisitor;

    class StatementList :
            public NodeContainer<Statement*, AstType::StatementList, false> {
        using BaseType = NodeContainer<Statement*, AstType::StatementList, false>;
    public:

        StatementList(Module& module);

        StatementList(Module& module,
                      ContainerType&& params);

        void addStatement(Statement* st);

        void acceptVisitor(AstVisitor& visitor);
    };


    inline AstType get_typeid(const StatementList& container) {
        return container.getAstType();
    }

}


