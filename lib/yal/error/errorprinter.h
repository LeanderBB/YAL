/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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

#include "yal/yal.h"
#include "yal/util/format.h"

namespace yal {

    class ByteStream;
    class SourceManager;
    class SourceItem;
    struct SourceInfo;
    class ErrorReporter;
    class Error;

    class ErrorPrinter {
    public:

        ErrorPrinter(ByteStream& stream,
                     SourceManager& srcManager);

        ErrorPrinter(ByteStream& msgStream,
                     ByteStream& errStream,
                     SourceManager& srcManager);

        YAL_NO_COPY_CLASS(ErrorPrinter);

        SourceManager& getSourceManager() {
            return m_srcManager;
        }

        void print(const ErrorReporter& reporter);

        void enableColorCodes(const bool value);

        void printSourceInfo(ByteStream& output,
                             SourceItem &item,
                             const SourceInfo& srcInfo);
    private:

        const char* getErrorTypeString(const Error& error) const;

    private:
        enum {kInternalBufferSize = 4096};
        FormaterStack<kInternalBufferSize> m_formater;
        ByteStream& m_errStream;
        ByteStream& m_msgStream;
        SourceManager& m_srcManager;
        unsigned m_useColorCodes:1;
    };

}
