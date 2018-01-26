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
#include <string>
#include "yal/util/codewriter.h"
namespace yal {

    class Module;
    class ByteStream;
    class Identifier;
    class Type;
    class QualType;
    class CodeGenC {
    public:

        CodeGenC(Module& module,
                 ByteStream& streamSrc,
                 ByteStream& streamHdr);

        bool writeFwdDecls();


    private:
        class FwdDeclVisitor;
        class GenVisitor;

        void scopeBegin(CodeWriter& stream);

        void scopeEnd(CodeWriter& stream,
                      const bool useSemiColon);

    private:
        enum {kInternalBufferSizBytes = 4096};
        Module& m_module;
        CodeWriter m_streamSrc;
        CodeWriter m_streamHdr;
        FormaterStack<kInternalBufferSizBytes> m_formater;
        uint32_t m_identLevel;
    };

}
