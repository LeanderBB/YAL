/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/parser/stdecltypefunctions.h"
#include "yal/frontend/parser/sttype.h"
#include "yal/frontend/parser/stdeclfunction.h"
#include "yal/frontend/parser/syntaxtreevisitor.h"

namespace yal::frontend {

    STDeclTypeFunctions::STDeclTypeFunctions(const STType* type,
                                             const ParseListDecl::Range range,
                                             STParser& parser):
        STDecl(SyntaxTreeType::STDeclTypeFunctions),
        m_type(type),
        m_decls(parser.getSTContext().getStdAllocatorWrapper<Decls::value_type>()){
         parser.getDeclList().moveRange<STDeclFunction, STDecl>(m_decls, range);
    }


    void
    STDeclTypeFunctions::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
