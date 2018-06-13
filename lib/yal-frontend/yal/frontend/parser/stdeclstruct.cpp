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

#include "yal/frontend/parser/stdeclstruct.h"
#include "yal/frontend/parser/syntaxtreevisitor.h"

namespace yal::frontend {

    STStructMember::STStructMember(const STIdentifier* name,
                                   const STQualType* type):
        m_name(name),
        m_type(type) {

    }

    void STStructMember::setSourceInfo(const SourceInfo &sourceInfo) {
        m_sourceInfo = sourceInfo;
    }

    STDeclStruct::STDeclStruct(const STIdentifier *name,
                               const ParseListStructMember::Range &range,
                               STParser &parser):
        STDecl(SyntaxTreeType::STDeclStruct, name),
        m_members(parser.getSTContext().getStdAllocatorWrapper<Members::value_type>()) {
        parser.getStructMemberList().moveRange(m_members, range);
    }

    void
    STDeclStruct::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
