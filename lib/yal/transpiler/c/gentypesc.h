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

#include "yal/util/codewriter.h"
namespace yal {
    class Type;
    class TypeBuiltin;
    class TypeDecl;
    class Identifier;
    class QualType;
    class DeclFunctionBase;
    class DeclStruct;
    namespace frontend{
    enum class UnaryOperatorType;
    enum class BinaryOperatorType;
    }
    class GenTypesC {
    public:

        static std::string GenIdentifier(const Identifier& identifier);

        static std::string GenIdentifier(const StringRef& string);

        static void GenIdentifierFromType(CodeWriter &writer,
                                          const Type &type);

        static void GenIdentifierFromType(CodeWriter &writer,
                                          const TypeBuiltin& type);

        static void GenIdentifierFromType(CodeWriter &writer,
                                          const TypeDecl& type);

        static void GenFromQualType(CodeWriter &writer,
                                    const QualType& type);

        static void GenDeclFunctionIntro(CodeWriter& writer,
                                         const DeclFunctionBase& function);

        static void GenDeclStruct(CodeWriter& writer,
                                  const DeclStruct& decl);

        static void GenUnaryOperator(CodeWriter& writer,
                                     const frontend::UnaryOperatorType op);

        static void GenBinaryOperator(CodeWriter& writer,
                                      const frontend::BinaryOperatorType op);
    };
}
