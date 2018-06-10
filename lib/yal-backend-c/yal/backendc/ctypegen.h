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

#pragma once

#include "yal/util/stringref.h"

namespace yal {
    class CodeWriter;
}

namespace yal::frontend {
    class DeclFunction;
    class DeclStruct;
    class QualType;
    enum class UnaryOperatorType;
    enum class BinaryOperatorType;
}

namespace yal::backend::c {

    class CTypeCache;
    class CType;

    struct CTypeGen  {

        static void GenDelcFunction(yal::CodeWriter& writer,
                                    const CTypeCache& cache,
                                    const yal::frontend::DeclFunction& decl);

        static void GenDelcStruct(yal::CodeWriter& writer,
                                  const CTypeCache& cache,
                                  const yal::frontend::DeclStruct& decl);

        static void GenQualType(CodeWriter& writer,
                                const yal::frontend::QualType& qt,
                                const CType& type);

        static void GenQualTypeFnParam(CodeWriter& writer,
                                       const yal::frontend::QualType& qt,
                                       const CType& type);

        static void GenQualTypeReturn(CodeWriter& writer,
                                      const yal::frontend::QualType& qt,
                                      const CType& type);

        static void GetBuilitinTypeInfo(yal::CodeWriter& writer);

        static StringRef GenUnaryOperator(const yal::frontend::UnaryOperatorType op);

        static StringRef GenBinaryOperator(const yal::frontend::BinaryOperatorType op);
    };
}
