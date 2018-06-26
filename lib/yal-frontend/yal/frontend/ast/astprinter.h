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

#include "yal/frontend/ast/astvisitor.h"
#include "yal/util/codewriter.h"

namespace yal {
    class ByteStream;
    struct SourceInfo;
}

namespace yal::frontend {


    class Qualifier;

    class AstPrinter : public AstVisitorStatic<AstPrinter, true> {
    public:
        AstPrinter(ByteStream&);

#define YAL_AST_NODE_TYPE(type) void visit(const type& node);
#include "yal/frontend/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE

    private:
        void scopeBegin(const bool lastNode = true);

        void scopeEnd();

        void printQualifier(const Qualifier& qualifier);

        void printSourceInfo(const SourceInfo& info);

    private:
        CodeWriter m_writer;
    };

}
