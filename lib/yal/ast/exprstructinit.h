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


#include "yal/ast/stmtexpression.h"

namespace yal {

    class RefType;
    class StructMemberInitList;
    class ExprStructInit : public StmtExpression
    {
    public:
        ExprStructInit(Module& module,
                       RefType* structType,
                       StructMemberInitList* memberInitList);


        RefType* getStructType() const {
            return m_structType;
        }

        StructMemberInitList* getMemberInitList() const {
            return m_memberInitList;
        }

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        RefType* m_structType;
        StructMemberInitList* m_memberInitList;
    };

}
