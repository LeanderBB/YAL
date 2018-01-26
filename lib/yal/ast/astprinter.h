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

#include "yal/ast/astvisitor.h"
#include "yal/util/format.h"
#include <vector>
namespace yal {
    struct SourceInfo;
    class ByteStream;
    class Qualifier;
    class AstPrinter : public AstVisitor {
    public:
        AstPrinter(ByteStream&);

#define YAL_AST_NODE_TYPE(type) void visit(type& node) override;
#include "yal/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE

    private:

        template <typename ...Args>
        void print(const char* format,
                   const Args&... args) {
            Format(m_formater, format, args...);
            printToStream();
        }

        template <typename ...Args>
        void printOnLine(const char* format,
                         const Args&... args) {
            Format(m_formater, format, args...);
            printToStreamNoPrefix();
        }

        void print();

        void printToStream();

        void printToStreamNoPrefix();

        void scopeBegin(const bool lastNode = true);

        void scopeEnd();

    private:
        void printQualifier(const Qualifier& qualifier);

        void printSourceInfo(const SourceInfo& info);

    private:
        ByteStream& m_stream;
        FormaterStack<1024> m_formater;
        std::vector<char> m_identationChars;
    };

}
